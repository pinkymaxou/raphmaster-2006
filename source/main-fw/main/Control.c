#include "Control.h"
#include "CocktailExplorer.h"
#include "HardwareGPIO.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

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

typedef enum
{
    EMOVETOHOME_Start,

    EMOVETOHOME_Y_StartMove,
    EMOVETOHOME_Y_Wait,
    EMOVETOHOME_X_StartMove,
    EMOVETOHOME_X_Wait,
    EMOVETOHOME_Z_StartMove,
    EMOVETOHOME_Z_Wait,

    EMOVETOHOME_Count
} EMOVETOHOME;

typedef enum
{
    EMOVETOSTATIONSTEP_Start,
    EMOVETOSTATIONSTEP_Wait
} EMOVETOSTATIONSTEP;

typedef enum
{
    EFILLINGGLASSSTEP_Start,
    EFILLINGGLASSSTEP_WaitStartFilling,
    EFILLINGGLASSSTEP_WaitUntilFillingCompleted,
    EFILLINGGLASSSTEP_WaitUntilRetreatCompleted
} EWAITINGFORGLASSSTEP;

typedef enum
{
    EMOVEBACKTOHOMEENDSTEP_Start,
    EMOVEBACKTOHOMEENDSTEP_Wait,
} EMOVEBACKTOHOMEENDSTEP;

typedef union
{
    struct
    {
        EMOVETOHOME eMoveToHome;
        TickType_t ttLastMeasureTicks;
    } sMoveToHome;
    struct
    {
        EMOVETOSTATIONSTEP eMoveToStationStep;

        uint32_t s32TargetX;
        uint32_t s32TargetZ;

        TickType_t ttLastMeasureTicks;
    } sMoveToStation;
    struct
    {
        int32_t s32ScaleWeightGram;
        uint8_t u8MeasureCount;

        TickType_t ttTimeoutGlassTicks;

        TickType_t ttLastMeasureTicks;
    } sWaitingForGlass;
    struct
    {
        EWAITINGFORGLASSSTEP eWaitingForGlassStep;

        uint32_t s32TargetY;
        uint32_t u32TimeHold_ms;

        int32_t s32RequiredQty_ml;
        TickType_t ttLastMeasureTicks;
    } sFillingGlass;
    struct
    {
        EMOVEBACKTOHOMEENDSTEP eMoveBackToHomeEnd;
        TickType_t ttLastMeasureTicks;
    } sMoveBackToHomeEnd;
} UStepData;

typedef struct
{
    ESTATE eState;

    bool bIsCancelRequest;

    CONTROL_SOrder sOrder;
    uint32_t u32CurrentStep;

    UStepData uStepData;

    // Positions
    int32_t s32CurrentX; // negative = LEFT, positive = RIGHT
    int32_t s32CurrentY; // negative = DOWN, positive = UP
    int32_t s32CurrentZ; // negative = TOWARD FRONT, positive = TOWARD BACK

    bool bIsHomingDone;
} SHandle;

#define MAXIMUM_QUEUE_LEN (5)

static SHandle m_sHandle = {0};
static QueueHandle_t m_hQueueHandle = NULL;

void CONTROL_Init()
{
    ESP_LOGI(TAG, "Init control");

    m_hQueueHandle = xQueueCreate(MAXIMUM_QUEUE_LEN, sizeof(CONTROL_SOrder));

    // IDLE state
    m_sHandle.eState = ESTATE_IdleWaitingForOrder;
    m_sHandle.bIsCancelRequest = false;

    m_sHandle.bIsHomingDone = false;

    m_sHandle.u32CurrentStep = 0;

    // Step counts
    m_sHandle.sOrder.u32MakerStepCount = 0;

    m_sHandle.s32CurrentX = 0;
    m_sHandle.s32CurrentY = 0;
    m_sHandle.s32CurrentZ = 0;
}

bool CONTROL_QueueOrder(const CONTROL_SOrder* pSOrder)
{
    ESP_LOGI(TAG, "Add new order to queue, recipeid: %d, step count: %d", pSOrder->u32RecipeId, pSOrder->u32MakerStepCount);

    // Fill data
    if ( xQueueSend(m_hQueueHandle, pSOrder, 0) != pdPASS )
        return false;

    return true;
}

void CONTROL_Run()
{
    switch (m_sHandle.eState)
    {
        case ESTATE_Cancelled:
        {
            // TODO: Do nothing until another recipe start to be produced
            // In pause until it get started again.
            if (!m_sHandle.bIsCancelRequest)
            {
                ESP_LOGI(TAG, "Cancel has been cancelled ...");
                m_sHandle.eState = ESTATE_IdleWaitingForOrder;
            }
            break;
        }
        case ESTATE_IdleWaitingForOrder:
        {
            // Orders
            CONTROL_SOrder sOrder;
            if (xQueueReceive(m_hQueueHandle, &sOrder, 0) == pdPASS)
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
                m_sHandle.eState = ESTATE_MoveToHomeStart;
                m_sHandle.bIsCancelRequest = false;
                // Current step
                m_sHandle.u32CurrentStep = 0;
                // Ingredients
                // Start with Y to be sure it's at the lowest position and won't interfere
                m_sHandle.eState = ESTATE_MoveToHomeStart;
                m_sHandle.uStepData.sMoveToHome.eMoveToHome = EMOVETOHOME_Start;
            }
            break;
        }
        case ESTATE_MoveToHomeStart:
        {
            if (m_sHandle.bIsCancelRequest)
            {
                ESP_LOGE(TAG, "Cancelling homing ...");
                HARDWAREGPIO_EnableAllSteppers(false);
                m_sHandle.eState = ESTATE_Cancelled;
                break;
            }

            // TODO: Move back to the home position
            // assume touching home switch or going to 0 is the home position
            // Add a timeout just in case
            switch(m_sHandle.uStepData.sMoveToHome.eMoveToHome)
            {
                case EMOVETOHOME_Start:
                    ESP_LOGI(TAG, "Starting homing process on Y");
                    // Wake-up Y stepper
                    // Command it to move to "infinite"
                    HARDWAREGPIO_EnableAllSteppers(true);
                    m_sHandle.uStepData.sMoveToHome.eMoveToHome = EMOVETOHOME_Y_StartMove;
                    break;
                case EMOVETOHOME_Y_StartMove:
                {
                    HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_y, &m_sHandle.s32CurrentY, -100000);
                    m_sHandle.uStepData.sMoveToHome.eMoveToHome = EMOVETOHOME_Y_Wait;
                    break;
                }
                case EMOVETOHOME_Y_Wait:
                {
                    if (HARDWAREGPIO_CheckEndStop_LOW(HARDWAREGPIO_EAXIS_y))
                    {
                        m_sHandle.s32CurrentY = 0;
                        m_sHandle.uStepData.sMoveToHome.eMoveToHome = EMOVETOHOME_X_StartMove;
                        ESP_LOGI(TAG, "Home Y is done");
                    }
                    break;
                }
                case EMOVETOHOME_X_StartMove:
                {
                    HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_x, &m_sHandle.s32CurrentX, -100000);
                    m_sHandle.uStepData.sMoveToHome.eMoveToHome = EMOVETOHOME_X_Wait;
                    break;
                }
                case EMOVETOHOME_X_Wait:
                {
                    if (HARDWAREGPIO_CheckEndStop_LOW(HARDWAREGPIO_EAXIS_x))
                    {
                        m_sHandle.s32CurrentX = 0;
                        m_sHandle.uStepData.sMoveToHome.eMoveToHome = EMOVETOHOME_Z_StartMove;
                        ESP_LOGI(TAG, "Home X is done");
                    }
                    break;
                }
                case EMOVETOHOME_Z_StartMove:
                {
                    HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_z, &m_sHandle.s32CurrentZ, -100000);
                    m_sHandle.uStepData.sMoveToHome.eMoveToHome = EMOVETOHOME_Z_Wait;
                    break;
                }
                case EMOVETOHOME_Z_Wait:
                {
                    if (HARDWAREGPIO_CheckEndStop_LOW(HARDWAREGPIO_EAXIS_z))
                    {
                        m_sHandle.s32CurrentZ = 0;

                        // Measure count at 0 before ...
                        m_sHandle.uStepData.sWaitingForGlass.u8MeasureCount = 0;
                        m_sHandle.uStepData.sWaitingForGlass.s32ScaleWeightGram = HARDWAREGPIO_GetScaleWeightGram();

                        m_sHandle.uStepData.sWaitingForGlass.ttLastMeasureTicks = xTaskGetTickCount();
                        m_sHandle.uStepData.sWaitingForGlass.ttTimeoutGlassTicks = xTaskGetTickCount();

                        m_sHandle.eState = ESTATE_WaitingForGlass;
                        ESP_LOGI(TAG, "Home Z is done");
                    }
                    break;
                }
                case EMOVETOHOME_Count:
                default:
                    break;
            }
            break;
        }
        case ESTATE_WaitingForGlass:
        {
            if (m_sHandle.bIsCancelRequest || 
                (xTaskGetTickCount() - m_sHandle.uStepData.sWaitingForGlass.ttTimeoutGlassTicks) >= pdMS_TO_TICKS(30000))
            {
                ESP_LOGE(TAG, "Cancelling waiting for glass ...");
                m_sHandle.eState = ESTATE_Cancelled;
                break;
            }

            // TODO: Use the scale to know when the glass is on the sleigh.
            // 90 seconds timeout?
            if ( (xTaskGetTickCount() - m_sHandle.uStepData.sWaitingForGlass.ttLastMeasureTicks) > pdMS_TO_TICKS(50) )
            {
                m_sHandle.uStepData.sWaitingForGlass.ttLastMeasureTicks = xTaskGetTickCount();

                const int32_t s32ScaleWeightGram = HARDWAREGPIO_GetScaleWeightGram();

                m_sHandle.uStepData.sWaitingForGlass.s32ScaleWeightGram = (m_sHandle.uStepData.sWaitingForGlass.s32ScaleWeightGram / 9) + (s32ScaleWeightGram / 10);
                m_sHandle.uStepData.sWaitingForGlass.u8MeasureCount++;

                // If it moved, reset measures
                const int32_t s32MovingDiff = abs(m_sHandle.uStepData.sWaitingForGlass.s32ScaleWeightGram - s32ScaleWeightGram);
                if (s32MovingDiff > 5)
                {
                    ESP_LOGW(TAG, "Moving too much ...");
                    m_sHandle.uStepData.sWaitingForGlass.u8MeasureCount = 0;
                }

                if (m_sHandle.uStepData.sWaitingForGlass.u8MeasureCount >= 10)
                {
                    // If it's not heavy enough, do it again
                    const int32_t s32Offset = 0;

                    if (m_sHandle.uStepData.sWaitingForGlass.s32ScaleWeightGram - s32Offset > 20)
                    {
                        // Glass detected, go to next step.
                        // Find the station
                        ESP_LOGI(TAG, "Moving to the first station");
                        m_sHandle.u32CurrentStep = 0;

                        // TODO: Locate ingredient X, Z by station id
                        m_sHandle.uStepData.sMoveToStation.eMoveToStationStep = EMOVETOSTATIONSTEP_Start;
                        m_sHandle.uStepData.sMoveToStation.s32TargetX = 0;
                        m_sHandle.uStepData.sMoveToStation.s32TargetZ = 0;
                        m_sHandle.eState = ESTATE_MoveToStation;
                    }
                    m_sHandle.uStepData.sWaitingForGlass.u8MeasureCount = 0;
                }
            }
            break;
        }
        case ESTATE_MoveToStation:
        {
            if (m_sHandle.bIsCancelRequest)
            {
                ESP_LOGE(TAG, "Cancelling move to station ...");
                HARDWAREGPIO_EnableAllSteppers(false);
                m_sHandle.eState = ESTATE_Cancelled;
                break;
            }

            // TODO: Moving to a station
            switch(m_sHandle.uStepData.sMoveToStation.eMoveToStationStep)
            {
                case EMOVETOSTATIONSTEP_Start:
                {
                    ESP_LOGI(TAG, "Moving to station at X: %d, Z: %d", m_sHandle.uStepData.sMoveToStation.s32TargetX, m_sHandle.uStepData.sMoveToStation.s32TargetZ);
                    HARDWAREGPIO_EnableAllSteppers(true);
                    HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_x, &m_sHandle.s32CurrentX, m_sHandle.uStepData.sMoveToStation.s32TargetX);
                    HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_z, &m_sHandle.s32CurrentZ, m_sHandle.uStepData.sMoveToStation.s32TargetZ);

                    m_sHandle.uStepData.sMoveToStation.ttLastMeasureTicks = xTaskGetTickCount();
                    m_sHandle.uStepData.sMoveToStation.eMoveToStationStep = EMOVETOSTATIONSTEP_Wait;
                    break;
                }
                case EMOVETOSTATIONSTEP_Wait:
                {
                    if ((xTaskGetTickCount() - m_sHandle.uStepData.sMoveToStation.ttLastMeasureTicks) > pdMS_TO_TICKS(CONTROL_STEPTIMEOUT_MS))
                    {
                        ESP_LOGE(TAG, "Cancelling move to station ...");
                        HARDWAREGPIO_EnableAllSteppers(false);
                        m_sHandle.eState = ESTATE_Cancelled;
                        break;
                    }

                    if (m_sHandle.s32CurrentX == m_sHandle.uStepData.sMoveToStation.s32TargetX &&
                        m_sHandle.s32CurrentZ == m_sHandle.uStepData.sMoveToStation.s32TargetZ)
                    {
                        // TODO: Filling glass time
                        m_sHandle.uStepData.sFillingGlass.eWaitingForGlassStep = EFILLINGGLASSSTEP_Start;

                        m_sHandle.eState = ESTATE_FillingGlass;
                    }
                    break;
                }
            }
            break;
        }
        case ESTATE_FillingGlass:
        {
            if (m_sHandle.bIsCancelRequest)
            {
                ESP_LOGE(TAG, "Cancelling filling glass ...");
                HARDWAREGPIO_EnableAllSteppers(false);
                m_sHandle.eState = ESTATE_Cancelled;
                break;
            }

            switch(m_sHandle.uStepData.sFillingGlass.eWaitingForGlassStep)
            {
                case EFILLINGGLASSSTEP_Start:
                {
                    ESP_LOGE(TAG, "Starting filling glass ...");

                    m_sHandle.uStepData.sFillingGlass.ttLastMeasureTicks = xTaskGetTickCount();

                    // TODO: Load target Y
                    m_sHandle.uStepData.sFillingGlass.s32TargetY = 5000;
                    m_sHandle.uStepData.sFillingGlass.u32TimeHold_ms = 2500;

                    HARDWAREGPIO_EnableAllSteppers(true);
                    HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_y, &m_sHandle.s32CurrentY, m_sHandle.uStepData.sFillingGlass.s32TargetY);

                    m_sHandle.uStepData.sFillingGlass.eWaitingForGlassStep = EFILLINGGLASSSTEP_WaitStartFilling;
                    break;
                }
                case EFILLINGGLASSSTEP_WaitStartFilling:
                {
                    if ((xTaskGetTickCount() - m_sHandle.uStepData.sFillingGlass.ttLastMeasureTicks) > pdMS_TO_TICKS(CONTROL_STEPTIMEOUT_MS))
                    {
                        ESP_LOGE(TAG, "Cancelling filling glass ...");
                        HARDWAREGPIO_EnableAllSteppers(false);
                        m_sHandle.eState = ESTATE_Cancelled;
                        break;
                    }

                    if (m_sHandle.s32CurrentY == m_sHandle.uStepData.sFillingGlass.s32TargetY)
                    {
                        ESP_LOGE(TAG, "In place for filling");
                        m_sHandle.uStepData.sFillingGlass.ttLastMeasureTicks = xTaskGetTickCount();
                        m_sHandle.uStepData.sFillingGlass.eWaitingForGlassStep = EFILLINGGLASSSTEP_WaitUntilFillingCompleted;
                    }
                    break;
                }
                case EFILLINGGLASSSTEP_WaitUntilFillingCompleted:
                {
                    const TickType_t ttWait = xTaskGetTickCount() - m_sHandle.uStepData.sFillingGlass.ttLastMeasureTicks;

                    if (ttWait > pdMS_TO_TICKS(CONTROL_STEPTIMEOUT_MS))
                    {
                        ESP_LOGE(TAG, "Cancelling filling glass ...");
                        HARDWAREGPIO_EnableAllSteppers(false);
                        m_sHandle.eState = ESTATE_Cancelled;
                    }
                    else if (ttWait > pdMS_TO_TICKS(m_sHandle.uStepData.sFillingGlass.u32TimeHold_ms))
                    {
                        ESP_LOGE(TAG, "Time to retreat the arm");
                        HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_y, &m_sHandle.s32CurrentY, 0);
                        m_sHandle.uStepData.sFillingGlass.ttLastMeasureTicks = xTaskGetTickCount();
                        m_sHandle.uStepData.sFillingGlass.eWaitingForGlassStep = EFILLINGGLASSSTEP_WaitUntilRetreatCompleted;
                    }
                    break;
                }
                case EFILLINGGLASSSTEP_WaitUntilRetreatCompleted:
                {
                    if ((xTaskGetTickCount() - m_sHandle.uStepData.sFillingGlass.ttLastMeasureTicks) > pdMS_TO_TICKS(CONTROL_STEPTIMEOUT_MS))
                    {
                        ESP_LOGE(TAG, "Cancelling filling glass ...");
                        HARDWAREGPIO_EnableAllSteppers(false);
                        m_sHandle.eState = ESTATE_Cancelled;
                        break;
                    }

                    if (m_sHandle.s32CurrentY == m_sHandle.uStepData.sFillingGlass.s32TargetY)
                    {
                        ESP_LOGE(TAG, "Filling probe is now retreated");
                        // Fill another or go home
                        m_sHandle.uStepData.sMoveBackToHomeEnd.eMoveBackToHomeEnd = EMOVEBACKTOHOMEENDSTEP_Start;
                        m_sHandle.eState = ESTATE_MoveBackToHomeEnd;
                    }
                    break;
                }
            }
            // TODO: Trigger the pouring process and stay there until the scale detect the correct quantity has been poured.
            // go to the next station if there is something else to pour or go back to home
            break;
        }
        case ESTATE_MoveBackToHomeEnd:
        {
            switch (m_sHandle.uStepData.sMoveBackToHomeEnd.eMoveBackToHomeEnd)
            {
                case EMOVEBACKTOHOMEENDSTEP_Start:
                {
                    ESP_LOGI(TAG, "Move back to home");

                    m_sHandle.uStepData.sMoveBackToHomeEnd.eMoveBackToHomeEnd = EMOVEBACKTOHOMEENDSTEP_Wait;

                    m_sHandle.uStepData.sMoveBackToHomeEnd.ttLastMeasureTicks = xTaskGetTickCount();

                    HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_x, &m_sHandle.s32CurrentX, 0);
                    HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS_z, &m_sHandle.s32CurrentZ, 0);
                    break;
                }
                case EMOVEBACKTOHOMEENDSTEP_Wait:
                {
                    if ((xTaskGetTickCount() - m_sHandle.uStepData.sMoveBackToHomeEnd.ttLastMeasureTicks) > pdMS_TO_TICKS(CONTROL_STEPTIMEOUT_MS))
                    {
                        ESP_LOGE(TAG, "Cancelling move to station ...");
                        HARDWAREGPIO_EnableAllSteppers(false);
                        m_sHandle.eState = ESTATE_Cancelled;
                        break;
                    }

                    if (m_sHandle.s32CurrentX == 0 &&
                        m_sHandle.s32CurrentZ == 0)
                    {
                        ESP_LOGI(TAG, "Move back to home");
                        m_sHandle.eState = ESTATE_WaitForRemovingGlass;
                    }
                    break;
                }
            }
            break;
        }
        case ESTATE_WaitForRemovingGlass:
        {
            // Add code to detect when the scale is under the minimum
            const int32_t s32ScaleWeightGram = HARDWAREGPIO_GetScaleWeightGram();
            const int32_t s32Offset = 0;

            if (s32ScaleWeightGram - s32Offset <= 20)
            {
                ESP_LOGI(TAG, "Glass has been removed");
                m_sHandle.eState = ESTATE_IdleWaitingForOrder;
            }
            break;
        }

        /* Homing process */
        case ESTATE_StartHoming:
        {
            // TODO: Move X stepper to the left until it touch end switch

            // TODO: Move Z stepper toward the front until it touch end switch

            break;
        }
        case ESTATE_InProgressHoming:
        {
            // TODO: Wait until it touch the switch and consider it's the zero position
            break;
        }
    }
}

