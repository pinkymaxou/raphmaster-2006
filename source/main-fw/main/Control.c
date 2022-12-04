#include "Control.h"
#include "CocktailExplorer.h"
#include "HardwareGPIO.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "StationSettings.h"

#define TAG "Control"

typedef enum
{
    ESTATE_IdleWaitingForOrder = 0,

    ESTATE_MoveToHomeStart = 1,
    ESTATE_WaitingForGlass = 2,
    ESTATE_MoveToStation = 3,
    ESTATE_FillingGlass = 4,
    ESTATE_MoveBackToHomeEnd = 5,
    ESTATE_WaitForRemovingGlass = 6,

    ESTATE_Cancelled = 7,

    ESTATE_StartHoming = 8,
    ESTATE_InProgressHoming = 9,
} ESTATE;

typedef struct
{
    ESTATE eState;

    bool bIsCancelRequest;

    CONTROL_SOrder sOrder;

    // Positions
    int32_t s32CurrentX; // negative = LEFT, positive = RIGHT
    int32_t s32CurrentY; // negative = DOWN, positive = UP
    int32_t s32CurrentZ; // negative = TOWARD FRONT, positive = TOWARD BACK

    bool bIsHomingDone;
} SHandle;

#define MAXIMUM_QUEUE_LEN (5)

static SHandle m_sHandle = {0};
static QueueHandle_t m_hQueueHandle = NULL;
static TaskHandle_t m_hTaskHandle = NULL;

static void ControlThreadRun(void* pParam);

static bool DoAxisHoming(HARDWAREGPIO_EAXIS eAxis);
static bool WaitUntilGlassIsThere();
static bool WaitUntilGlassRemoved();
static bool FillGlass(uint16_t u16Qty);
static bool MoveToCoordinate(int32_t s32X, int32_t s32Z);

void CONTROL_Init()
{
    ESP_LOGI(TAG, "Init control");

    m_hQueueHandle = xQueueCreate(MAXIMUM_QUEUE_LEN, sizeof(CONTROL_SOrder));

    // IDLE state
    m_sHandle.eState = ESTATE_IdleWaitingForOrder;
    m_sHandle.bIsCancelRequest = false;

    m_sHandle.bIsHomingDone = false;

    // Step counts
    m_sHandle.sOrder.u32MakerStepCount = 0;

    m_sHandle.s32CurrentX = 0;
    m_sHandle.s32CurrentY = 0;
    m_sHandle.s32CurrentZ = 0;
}

void CONTROL_StartTask()
{
    // Create the new task with default priority and stack size
    xTaskCreate(ControlThreadRun, "control", 3500, NULL, 1, &m_hTaskHandle);
}

bool CONTROL_QueueOrder(const CONTROL_SOrder* pSOrder)
{
    ESP_LOGI(TAG, "Add new order to queue, recipeid: %d, step count: %d", pSOrder->u32RecipeId, pSOrder->u32MakerStepCount);

    // Fill data
    if ( xQueueSend(m_hQueueHandle, pSOrder, 0) != pdPASS )
        return false;

    return true;
}

static void ControlThreadRun(void* pParam)
{
    while(true)
    {
        // Orders
        CONTROL_SOrder sOrder;
        if (xQueueReceive(m_hQueueHandle, &sOrder, portMAX_DELAY) == pdPASS)
        {
            if (sOrder.u32MakerStepCount == 0)
            {
                ESP_LOGE(TAG, "Invalid order, just skip it ...");
                // If the order is wrong, we just skip it.
                m_sHandle.eState = ESTATE_IdleWaitingForOrder;
                break;
            }

            ESP_LOGI(TAG, "New order started with recipeid: %d, steps: %d", sOrder.u32RecipeId, sOrder.u32MakerStepCount);

            memcpy(&m_sHandle.sOrder, &sOrder, sizeof(CONTROL_SOrder));
            m_sHandle.bIsCancelRequest = false;
        }
        // Ingredients
        // Start with Y to be sure it's at the lowest position and won't interfere
        // Do homing on all axis before accepting glass
        m_sHandle.eState = ESTATE_MoveToHomeStart;
        DoAxisHoming(HARDWAREGPIO_EAXIS_y);
        DoAxisHoming(HARDWAREGPIO_EAXIS_z);
        DoAxisHoming(HARDWAREGPIO_EAXIS_x);

        // Wait until user put his glass on the plate
        m_sHandle.eState = ESTATE_WaitingForGlass;
        if (!WaitUntilGlassIsThere())
            goto CANCEL;

        for(int i = 0; i < m_sHandle.sOrder.u32MakerStepCount; i++)
        {
            const CONTROL_MakerStep* psMakerStep = &m_sHandle.sOrder.sMakerSteps[i];

            // Get station id
            int32_t s32X = STATIONSETTINGS_GetValue(psMakerStep->u32StationID, STATIONSETTINGS_ESTATIONSET_PosX);
            int32_t s32Z = STATIONSETTINGS_GetValue(psMakerStep->u32StationID, STATIONSETTINGS_ESTATIONSET_PosZ);

            // Move to station
            m_sHandle.eState = ESTATE_MoveToStation;
            if (!MoveToCoordinate(s32X, s32Z))
                goto CANCEL;

            m_sHandle.eState = ESTATE_FillingGlass;
            if (!FillGlass(psMakerStep->u32Qty_ml))
                goto CANCEL;
        }

        // Move back to home
        m_sHandle.eState = ESTATE_MoveBackToHomeEnd;
        if (!MoveToCoordinate(0, 0))
            goto CANCEL;

        m_sHandle.eState = ESTATE_WaitForRemovingGlass;
        if (!WaitUntilGlassRemoved())
            goto CANCEL;

        goto END;
        CANCEL:
        m_sHandle.eState = ESTATE_Cancelled;
        HARDWAREGPIO_EnableAllSteppers(false);
        ESP_LOGE(TAG, "Cancelling order");
        END:
        // Leave some time, we want the poor idle tasks to have some time
        vTaskDelay(1);
    }

    vTaskDelete(NULL);
}

static bool DoAxisHoming(HARDWAREGPIO_EAXIS eAxis)
{
    ESP_LOGI(TAG, "Starting homing ...");

    if (HARDWAREGPIO_CheckEndStop_LOW(eAxis))
    {
        ESP_LOGI(TAG, "Already at home position");
        return true;
    }

    HARDWAREGPIO_EnableAllSteppers(true);
    // Give it some time to be sure it's activated
    vTaskDelay(pdMS_TO_TICKS(10));

    TickType_t ttProcessTimeoutTicks = xTaskGetTickCount();

    while(true)
    {
        if (m_sHandle.bIsCancelRequest)
        {
            ESP_LOGE(TAG, "Cancel requested by user");
            goto ERROR;
        }

        if ((xTaskGetTickCount() - ttProcessTimeoutTicks) > pdMS_TO_TICKS(10000))
        {
            ESP_LOGE(TAG, "Cancelling axis homing because of timeout condition");
            goto ERROR;
        }

        // Move until it reach the lowest end
        HARDWAREGPIO_MoveStepperAsync(eAxis, -1);
        if (HARDWAREGPIO_CheckEndStop_LOW(eAxis))
        {
            if (eAxis == HARDWAREGPIO_EAXIS_x)
                m_sHandle.s32CurrentX = 0;
            else if (eAxis == HARDWAREGPIO_EAXIS_y)
                m_sHandle.s32CurrentY = 0;
            else if (eAxis == HARDWAREGPIO_EAXIS_z)
                m_sHandle.s32CurrentZ = 0;
            break;
        }
        vTaskDelay(pdMS_TO_TICKS(1));
    }

    HARDWAREGPIO_EnableAllSteppers(false);
    return true;
    ERROR:
    HARDWAREGPIO_EnableAllSteppers(false);
    return false;
}

static bool WaitUntilGlassIsThere()
{
    ESP_LOGI(TAG, "Until until a glass get in place ...");

    const int32_t s32ScaleWeightGram = HARDWAREGPIO_GetScaleWeightGram();

    const TickType_t ttProcessTimeoutTicks = xTaskGetTickCount();

    uint8_t u8MeasureCount = 0;
    int32_t avgS32ScaleWeight_Gram = 0;

    while(true)
    {
        if (m_sHandle.bIsCancelRequest)
        {
            ESP_LOGE(TAG, "Cancel requested by user");
            goto ERROR;
        }

        if ((xTaskGetTickCount() - ttProcessTimeoutTicks) > pdMS_TO_TICKS(90*1000))
        {
            ESP_LOGE(TAG, "Cancelling axis homing because of timeout condition");
            goto ERROR;
        }

        const int32_t s32ScaleWeightGram = HARDWAREGPIO_GetScaleWeightGram();

        avgS32ScaleWeight_Gram = (avgS32ScaleWeight_Gram / 9) + (s32ScaleWeightGram / 10);
        u8MeasureCount++;

        // If it moved, reset measures
        const int32_t s32MovingDiff = abs(avgS32ScaleWeight_Gram - s32ScaleWeightGram);
        if (s32MovingDiff > 10)
        {
            ESP_LOGW(TAG, "Moving too much ...");
            u8MeasureCount = 0;
        }

        if (u8MeasureCount >= 10)
        {
            // If it's not heavy enough, do it again
            const int32_t s32Offset = 0;

            if (avgS32ScaleWeight_Gram - s32Offset > 20)
            {
                // Glass detected, go to next step.
                // Find the station
                ESP_LOGI(TAG, "Glass detection has detected a glass");
                break;
            }
            u8MeasureCount = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }

    return true;
    ERROR:
    return false;
}

static bool WaitUntilGlassRemoved()
{
    ESP_LOGI(TAG, "Wait until the glass get removed ...");

    // This process doesn't have a timeout.
    // the only way to cancel it is too cancel manually or put a glass
    const int32_t s32ScaleWeightGram = HARDWAREGPIO_GetScaleWeightGram();
    uint8_t u8MeasureCount = 0;
    int32_t avgS32ScaleWeight_Gram = 0;

    while(true)
    {
        if (m_sHandle.bIsCancelRequest)
        {
            ESP_LOGE(TAG, "Cancel requested by user");
            goto ERROR;
        }

        const int32_t s32ScaleWeightGram = HARDWAREGPIO_GetScaleWeightGram();

        avgS32ScaleWeight_Gram = (avgS32ScaleWeight_Gram / 9) + (s32ScaleWeightGram / 10);
        u8MeasureCount++;

        // If it moved, reset measures
        const int32_t s32MovingDiff = abs(avgS32ScaleWeight_Gram - s32ScaleWeightGram);
        if (s32MovingDiff > 10)
        {
            ESP_LOGW(TAG, "Moving too much ...");
            u8MeasureCount = 0;
        }

        if (u8MeasureCount >= 10)
        {
            // If it's not heavy enough, do it again
            const int32_t s32Offset = 0;

            if (avgS32ScaleWeight_Gram - s32Offset <= 20)
            {
                ESP_LOGI(TAG, "Glass detection has detected the glass have been removed");
                break;
            }
            u8MeasureCount = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(50));
    }

    return true;
    ERROR:
    return false;
}

static bool FillGlass(uint16_t u16Qty)
{
    ESP_LOGI(TAG, "Filling glass ...");

    if (m_sHandle.bIsCancelRequest)
    {
        ESP_LOGE(TAG, "Cancel requested by user");
        goto ERROR;
    }

    // It it's an optic, move the arm upward
    // if it's a peristaltic pump just spin until the weight fit.

    return true;
    ERROR:
    return false;
}

static bool MoveToCoordinate(int32_t s32X, int32_t s32Z)
{
    ESP_LOGI(TAG, "Moving to coordinate ...");

    if (m_sHandle.bIsCancelRequest)
    {
        ESP_LOGE(TAG, "Cancel requested by user");
        goto ERROR;
    }

    return true;
    ERROR:
    return false;
}

