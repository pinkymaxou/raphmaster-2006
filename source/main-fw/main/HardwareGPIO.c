#include "HardwareGPIO.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
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

void HARDWAREGPIO_MoveStepperAsync(HARDWAREGPIO_EAXIS eAxis, int32_t s32Count)
{

}

bool HARDWAREGPIO_CheckEndStop_LOW(HARDWAREGPIO_EAXIS eAxis)
{
    vTaskDelay(pdTICKS_TO_MS(5000));
    return true;
}

int32_t HARDWAREGPIO_GetScaleWeightGram()
{
    return 0;
}