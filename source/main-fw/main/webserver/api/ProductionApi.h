#ifndef _PRODUCTIONAPI_H_
#define _PRODUCTIONAPI_H_

#include <stdint.h>
#include <stdbool.h>

bool PRODUCTIONAPI_HandleOrder(const char* szData, uint32_t u32Length);

char* PRODUCTIONAPI_GetStatus();

#endif