#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include <stdint.h>
#include <stdbool.h>

char* COCKTAILEXPLORER_GetStationSettings();

bool COCKTAILEXPLORER_SetStationSettings(const char* szRequestBuffer, uint32_t u32Length);

#endif