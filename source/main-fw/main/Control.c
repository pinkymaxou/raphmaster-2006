#include "Control.h"
#include "CocktailExplorer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "StationSettings.h"

#define TAG "Control"

typedef enum
{
    EINSTRUCTION_Order,     /* Order a new cocktail*/
    EINSTRUCTION_HomeAll,   /* Home all axis */

    EINSTRUCTION_MoveAxis,
    EINSTRUCTION_MoveToStation
} EINSTRUCTION;

typedef struct
{
    EINSTRUCTION eInstruction;
    union
    {
        CONTROL_SOrder sOrder;
        struct
        {
            HARDWAREGPIO_EAXIS eAxis;
            int32_t s32Value;
        } sMoveAxis;
        struct
        {
            uint32_t u32StationId;
        } sMoveToStation;
    } uArg;
} SQueueInstruction;

typedef struct
{
    CONTROL_ESTATE eState;

    SQueueInstruction sQueueInstruction;

    double dPercent;

    volatile bool bIsCancelRequest;
    // Positions
    int32_t s32CurrentX; // negative = LEFT, positive = RIGHT
    int32_t s32CurrentY; // negative = DOWN, positive = UP
    int32_t s32CurrentZ; // negative = TOWARD FRONT, positive = TOWARD BACK
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

    m_hQueueHandle = xQueueCreate(MAXIMUM_QUEUE_LEN, sizeof(SQueueInstruction));

    // IDLE state
    m_sHandle.eState = CONTROL_ESTATE_IdleWaitingForOrder;
    m_sHandle.bIsCancelRequest = false;

    // Step counts
    memset(&m_sHandle.sQueueInstruction, 0, sizeof(SQueueInstruction));

    m_sHandle.s32CurrentX = 0;
    m_sHandle.s32CurrentY = 0;
    m_sHandle.s32CurrentZ = 0;
}

void CONTROL_StartTask()
{
    // Create the new task with default priority and stack size
    xTaskCreate(ControlThreadRun, "control", CONTROL_STACKSIZE, NULL, CONTROL_PRIORITY, &m_hTaskHandle);
}

bool CONTROL_QueueOrder(const CONTROL_SOrder* pSOrder)
{
    const SQueueInstruction sQueueInstruction =
    {
        .eInstruction = EINSTRUCTION_Order,
        .uArg = { .sOrder = *pSOrder }
    };

    // Fill data
    if ( xQueueSend(m_hQueueHandle, &sQueueInstruction, 0) != pdPASS )
    {
        ESP_LOGE(TAG, "Cannot queue order");
        return false;
    }
    ESP_LOGI(TAG, "Add new order to queue, recipeid: %d, step count: %d", pSOrder->u32RecipeId, pSOrder->u32MakerStepCount);
    return true;
}

bool CONTROL_QueueHomeAllAxis()
{
    const SQueueInstruction sQueueInstruction =
    {
        .eInstruction = EINSTRUCTION_HomeAll,
    };

    // Fill data
    if ( xQueueSend(m_hQueueHandle, &sQueueInstruction, 0) != pdPASS )
    {
        ESP_LOGE(TAG, "Cannot queue order");
        return false;
    }
    ESP_LOGI(TAG, "Home all axis");
    return true;
}

bool CONTROL_QueueMoveAxis(HARDWAREGPIO_EAXIS eAxis, int32_t s32Value)
{
    const SQueueInstruction sQueueInstruction =
    {
        .eInstruction = EINSTRUCTION_MoveAxis,
        .uArg = {
            .sMoveAxis = {
                .eAxis = eAxis,
                .s32Value = s32Value
            }
        }
    };

    // Fill data
    if ( xQueueSend(m_hQueueHandle, &sQueueInstruction, 0) != pdPASS )
    {
        ESP_LOGE(TAG, "Cannot queue order");
        return false;
    }
    ESP_LOGI(TAG, "Move axis");
    return true;
}

bool CONTROL_QueueMoveToStation(uint32_t u32StationId)
{
    const SQueueInstruction sQueueInstruction =
    {
        .eInstruction = EINSTRUCTION_MoveAxis,
        .uArg = {
            .sMoveToStation = {
                .u32StationId = u32StationId
            }
        }
    };

    // Fill data
    if ( xQueueSend(m_hQueueHandle, &sQueueInstruction, 0) != pdPASS )
    {
        ESP_LOGE(TAG, "Cannot queue order");
        return false;
    }
    ESP_LOGI(TAG, "Move axis");
    return true;
}

void CONTROL_Cancel()
{
    ESP_LOGI(TAG, "Cancelling ...");
    m_sHandle.bIsCancelRequest = true;
}

static void ControlThreadRun(void* pParam)
{
    while(true)
    {
        // Orders
        SQueueInstruction sQueueInstruction;
        if (xQueueReceive(m_hQueueHandle, &sQueueInstruction, portMAX_DELAY) == pdPASS)
        {
            if (sQueueInstruction.eInstruction == EINSTRUCTION_Order)
            {
                if (sQueueInstruction.uArg.sOrder.u32MakerStepCount == 0)
                {
                    ESP_LOGE(TAG, "Invalid order, just skip it ...");
                    // If the order is wrong, we just skip it.
                    continue;
                }

                ESP_LOGI(TAG, "New order started with recipeid: %d, steps: %d", sQueueInstruction.uArg.sOrder.u32RecipeId, sQueueInstruction.uArg.sOrder.u32MakerStepCount);
            }
            memcpy(&m_sHandle.sQueueInstruction, &sQueueInstruction, sizeof(SQueueInstruction));
            m_sHandle.dPercent = 0.0d;
            m_sHandle.bIsCancelRequest = false;
        }

        // Order instruction
        if (sQueueInstruction.eInstruction == EINSTRUCTION_Order)
        {
            m_sHandle.dPercent = 0.0d;
            // Ingredients
            // Start with Y to be sure it's at the lowest position and won't interfere
            // Do homing on all axis before accepting glass
            m_sHandle.eState = CONTROL_ESTATE_MoveToHomeStart;
            if (!DoAxisHoming(HARDWAREGPIO_EAXIS_y))
                goto CANCEL;
            m_sHandle.dPercent = 0.1d;
            if (!DoAxisHoming(HARDWAREGPIO_EAXIS_z))
                goto CANCEL;
            m_sHandle.dPercent = 0.2d;
            if (!DoAxisHoming(HARDWAREGPIO_EAXIS_x))
                goto CANCEL;
            m_sHandle.dPercent = 0.3d;

            // Wait until user put his glass on the plate
            m_sHandle.eState = CONTROL_ESTATE_WaitingForGlass;
            if (!WaitUntilGlassIsThere())
                goto CANCEL;

            m_sHandle.dPercent = 0.4d;
            for(int i = 0; i < m_sHandle.sQueueInstruction.uArg.sOrder.u32MakerStepCount; i++)
            {
                const CONTROL_MakerStep* psMakerStep = &m_sHandle.sQueueInstruction.uArg.sOrder.sMakerSteps[i];

                // Get station id
                int32_t s32X = STATIONSETTINGS_GetValue(psMakerStep->u32StationID, STATIONSETTINGS_ESTATIONSET_PosX);
                int32_t s32Z = STATIONSETTINGS_GetValue(psMakerStep->u32StationID, STATIONSETTINGS_ESTATIONSET_PosZ);

                if (s32X == 0 && s32Z == 0)
                {
                    ESP_LOGE(TAG, "Invalid calibration, cancelled");
                    goto CANCEL;
                }

                // Move to station
                m_sHandle.eState = CONTROL_ESTATE_MoveToStation;
                if (!MoveToCoordinate(s32X, s32Z))
                    goto CANCEL;

                m_sHandle.eState = CONTROL_ESTATE_FillingGlass;
                if (!FillGlass(psMakerStep->u32Qty_ml))
                    goto CANCEL;

                m_sHandle.dPercent = 0.4d + 0.4d * ((double)(i+1) / (double)m_sHandle.sQueueInstruction.uArg.sOrder.u32MakerStepCount);
            }

            // Move back to home
            m_sHandle.dPercent = 0.8d;
            m_sHandle.eState = CONTROL_ESTATE_MoveBackToHomeEnd;
            if (!MoveToCoordinate(0, 0))
                goto CANCEL;

            m_sHandle.dPercent = 0.9d;
            m_sHandle.eState = CONTROL_ESTATE_WaitForRemovingGlass;
            if (!WaitUntilGlassRemoved())
                goto CANCEL;
            m_sHandle.dPercent = 1.0d;
            // Normal ending
            m_sHandle.eState = CONTROL_ESTATE_IdleWaitingForOrder;
            goto END;
        }
        else if (sQueueInstruction.eInstruction == EINSTRUCTION_HomeAll)
        {
            ESP_LOGI(TAG, "Executing: CmdHomeAll");
            m_sHandle.eState = CONTROL_ESTATE_CmdHomeAll;
            if (!DoAxisHoming(HARDWAREGPIO_EAXIS_y))
                goto CANCEL;
            if (!DoAxisHoming(HARDWAREGPIO_EAXIS_z))
                goto CANCEL;
            if (!DoAxisHoming(HARDWAREGPIO_EAXIS_x))
                goto CANCEL;
            ESP_LOGI(TAG, "Completed: CmdHomeAll");
        }
        else if (sQueueInstruction.eInstruction == EINSTRUCTION_MoveToStation)
        {
            m_sHandle.eState = CONTROL_ESTATE_CmdMoveToStation;
            int32_t s32X = STATIONSETTINGS_GetValue(sQueueInstruction.uArg.sMoveToStation.u32StationId, STATIONSETTINGS_ESTATIONSET_PosX);
            int32_t s32Z = STATIONSETTINGS_GetValue(sQueueInstruction.uArg.sMoveToStation.u32StationId, STATIONSETTINGS_ESTATIONSET_PosZ);

            ESP_LOGI(TAG, "Executing: CmdMoveToStation, goto coordinate: x: %d, z: %d", s32X, s32Z);

            if (MoveToCoordinate(s32X, s32Z))
                goto CANCEL;
            ESP_LOGI(TAG, "Completed: CmdMoveToStation");
        }
        else if (sQueueInstruction.eInstruction == EINSTRUCTION_MoveAxis)
        {
            m_sHandle.eState = CONTROL_ESTATE_CmdMoveAxis;
            ESP_LOGI(TAG, "Executing: CmdMoveAxis");
            HARDWAREGPIO_MoveStepperAsync(sQueueInstruction.uArg.sMoveAxis.eAxis, sQueueInstruction.uArg.sMoveAxis.s32Value);
            ESP_LOGI(TAG, "Completed: CmdMoveAxis");
        }
        else
        {
            ESP_LOGE(TAG, "Ignored instruction");
            goto CANCEL;
        }

        // Done gracefully, we can return to normal state
        m_sHandle.eState = CONTROL_ESTATE_IdleWaitingForOrder;
        goto END;
        CANCEL:
        m_sHandle.dPercent = 1.0d;
        m_sHandle.eState = CONTROL_ESTATE_Cancelled;
        HARDWAREGPIO_EnableAllSteppers(false);
        ESP_LOGE(TAG, "Cancelling order");
        vTaskDelay(pdMS_TO_TICKS(5000));
        m_sHandle.bIsCancelRequest = false;
        m_sHandle.eState = CONTROL_ESTATE_IdleWaitingForOrder;
        ESP_LOGI(TAG, "Cancel done");
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

    const TickType_t ttProcessTimeoutTicks = xTaskGetTickCount();

    uint8_t u8MeasureCount = 0;
    int32_t avgS32ScaleWeight_Gram = HARDWAREGPIO_GetScaleWeightGram();

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
    uint8_t u8MeasureCount = 0;
    int32_t avgS32ScaleWeight_Gram = HARDWAREGPIO_GetScaleWeightGram();

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
    for(int i = 0; i < 2000; i++)
    {
        HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_y, 1);
        m_sHandle.s32CurrentY++;
        vTaskDelay(1);
    }

    // Go back to home.
    DoAxisHoming(HARDWAREGPIO_EAXIS_y);
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

    const int32_t s32DiffX = s32X - m_sHandle.s32CurrentX;
    const int32_t s32DiffZ = s32Z - m_sHandle.s32CurrentZ;
    // TODO: Moving doesn't correctly cancel yet
    // cancellation and acceleration curve will be necessary.
    // The movement is too rough for now.
    HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_x, s32DiffX);
    m_sHandle.s32CurrentX += s32DiffX;
    HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_z, s32DiffZ);
    m_sHandle.s32CurrentZ += s32DiffZ;
    return true;
    ERROR:
    return false;
}

CONTROL_SInfo CONTROL_GetInfos()
{
    CONTROL_SInfo sInfo =
    {
        .u32RecipeId = 0,
        .eState = m_sHandle.eState,
        .bIsCancelRequest = m_sHandle.bIsCancelRequest,

        // General
        .s32X = m_sHandle.s32CurrentX,
        .s32Z = m_sHandle.s32CurrentZ,
        .s32Y = m_sHandle.s32CurrentY,

        .u32BacklogCount = uxQueueMessagesWaiting(m_hQueueHandle)
    };

    if (m_sHandle.sQueueInstruction.eInstruction == EINSTRUCTION_Order)
    {
        sInfo.u32RecipeId = m_sHandle.sQueueInstruction.uArg.sOrder.u32RecipeId;
        sInfo.dPercent = m_sHandle.dPercent;
    }

    return sInfo;
}
