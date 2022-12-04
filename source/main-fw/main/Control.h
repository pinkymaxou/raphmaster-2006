#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define CONTROL_MAXIMUMSTEPS_COUNT (15)

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

void CONTROL_Run();

bool CONTROL_QueueOrder(const CONTROL_SOrder* pSOrder);

#endif