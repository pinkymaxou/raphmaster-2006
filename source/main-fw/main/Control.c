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

    ESTATE_Cancelled = 6,

    ESTATE_StartHoming = 7,
    ESTATE_InProgressHoming = 8,
} ESTATE;

typedef struct
{
    uint32_t u32IngredientID;
    uint32_t u32Qty_ml;

    bool bIsHandled;
} MakerStep;

typedef struct
{
    uint32_t u32RecipeId;

    MakerStep sMakerSteps[CONTROL_MAXIMUMSTEPS_COUNT];
    uint32_t u32MakerStepCount;
} SOrder;

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

typedef union
{
    struct
    {
        EMOVETOHOME eMoveToHome;
    } sMoveToHome;
    struct
    {
        uint32_t sTarget32X;
        uint32_t sTarget32Y;
    } sMoveToStation;
} UStepData;

typedef struct
{
    ESTATE eState;

    bool bIsCancelRequest;

    SOrder sOrder;
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

    m_hQueueHandle = xQueueCreate(MAXIMUM_QUEUE_LEN, sizeof(SOrder));

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

bool CONTROL_QueueOrder(uint32_t u32RecipeId)
{
    const SOrder sOrder =
    {
        .u32RecipeId = u32RecipeId
    };

    // Fill data
    if ( xQueueSend(m_hQueueHandle, &sOrder, 0) != pdPASS )
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
            SOrder sOrder;
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

                memcpy(&m_sHandle.sOrder, &sOrder, sizeof(SOrder));
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
                    break;
                }
                case EMOVETOHOME_Z_Wait:
                {
                    if (HARDWAREGPIO_CheckEndStop_LOW(HARDWAREGPIO_EAXIS_z))
                    {
                        m_sHandle.s32CurrentZ = 0;
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
            // TODO: Use the scale to know when the glass is on the sleigh.
            // 90 seconds timeout?
            break;
        }
        case ESTATE_MoveToStation:
        {
            // TODO: Moving to a station
            break;
        }
        case ESTATE_FillingGlass:
        {
            // TODO: Trigger the pouring process and stay there until the scale detect the correct quantity has been poured.
            // go to the next station if there is something else to pour or go back to home
            break;
        }
        case ESTATE_MoveBackToHomeEnd:
        {
            // TODO: Move back to home
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

