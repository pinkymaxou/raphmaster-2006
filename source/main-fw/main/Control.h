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

void CONTROL_Init();

void CONTROL_StartTask();

bool CONTROL_QueueOrder(const CONTROL_SOrder* pSOrder);

bool CONTROL_QueueMoveAxis(HARDWAREGPIO_EAXIS eAxis, int32_t s32Value);

bool CONTROL_QueueMoveToStation(uint32_t u32StationId);

bool CONTROL_QueueHomeAllAxis();

void CONTROL_Cancel();

#endif