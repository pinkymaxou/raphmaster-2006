#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#define CONTROL_MAXIMUMSTEPS_COUNT (15)

void CONTROL_Init();

void CONTROL_Run();

bool CONTROL_QueueOrder(uint32_t u32RecipeId);

#endif