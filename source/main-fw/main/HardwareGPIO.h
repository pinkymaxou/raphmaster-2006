#ifndef HARDWAREGPIO_H_
#define HARDWAREGPIO_H_

#include <stdint.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef enum
{
    HARDWAREGPIO_EAXIS_x,
    HARDWAREGPIO_EAXIS_y,
    HARDWAREGPIO_EAXIS_z,

    HARDWAREGPIO_EAXIS_Count
} HARDWAREGPIO_EAXIS;

void HARDWAREGPIO_Init();

void HARDWAREGPIO_ActivateRelay();

void HARDWAREGPIO_SetSanityLED(float fltPercent);

// Steppers
void HARDWAREGPIO_EnableAllSteppers(bool bIsEnabled);

void HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS eAxis, int32_t* const ps32CurrentSteps, int32_t s32AbsPosition);

bool HARDWAREGPIO_CheckEndStop_LOW(HARDWAREGPIO_EAXIS eAxis);


int32_t HARDWAREGPIO_GetScaleWeightGram();

#endif