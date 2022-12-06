#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "HardwareGPIO.h"

#define CONTROL_MAXIMUMSTEPS_COUNT (15)

#define CONTROL_STACKSIZE (3500)
#define CONTROL_PRIORITY  (1)

#define CONTROL_STEPTIMEOUT_MS (15*1000)

typedef struct
{
    uint32_t u32StationID;
    uint32_t u32Qty_ml;
} CONTROL_MakerStep;

typedef struct
{
    uint32_t u32RecipeId;

    CONTROL_MakerStep sMakerSteps[CONTROL_MAXIMUMSTEPS_COUNT];
    uint32_t u32MakerStepCount;
} CONTROL_SOrder;

typedef enum
{
    CONTROL_ESTATE_IdleWaitingForOrder = 0,

    CONTROL_ESTATE_MoveToHomeStart = 1,
    CONTROL_ESTATE_WaitingForGlass = 2,
    CONTROL_ESTATE_MoveToStation = 3,
    CONTROL_ESTATE_FillingGlass = 4,
    CONTROL_ESTATE_MoveBackToHomeEnd = 5,
    CONTROL_ESTATE_WaitForRemovingGlass = 6,

    CONTROL_ESTATE_Cancelled = 7,

    CONTROL_ESTATE_CmdHomeAll = 50,
    CONTROL_ESTATE_CmdMoveAxis = 51,
    CONTROL_ESTATE_CmdMoveToStation = 52
} CONTROL_ESTATE;

typedef struct
{
    uint32_t u32RecipeId;   // 0 = not linked to any recipe

    CONTROL_ESTATE eState;

    bool bIsCancelRequest;
    // Positions
    int32_t s32X; // negative = LEFT, positive = RIGHT
    int32_t s32Z; // negative = TOWARD FRONT, positive = TOWARD BACK
    int32_t s32Y; // negative = DOWN, positive = UP

    double dPercent;

    uint32_t u32BacklogCount;
} CONTROL_SInfo;

void CONTROL_Init();

void CONTROL_StartTask();

bool CONTROL_QueueOrder(const CONTROL_SOrder* pSOrder);

bool CONTROL_QueueMoveAxis(HARDWAREGPIO_EAXIS eAxis, int32_t s32Value);

bool CONTROL_QueueMoveToStation(uint32_t u32StationId);

bool CONTROL_QueueHomeAllAxis();

void CONTROL_Cancel();

CONTROL_SInfo CONTROL_GetInfos();

#endif