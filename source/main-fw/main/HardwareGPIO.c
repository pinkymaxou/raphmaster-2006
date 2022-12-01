#include "HardwareGPIO.h"
#include "HWConfig.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_log.h"
#include "Settings.h"

#define TAG "HardwareGPIO"

void HARDWAREGPIO_Init()
{

}

void HARDWAREGPIO_EnableAllSteppers(bool bIsEnabled)
{

}

void HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS eAxis, int32_t* const ps32CurrentSteps, int32_t s32AbsPosition)
{

}

bool HARDWAREGPIO_CheckEndStop_LOW(HARDWAREGPIO_EAXIS eAxis)
{
    return true;
}