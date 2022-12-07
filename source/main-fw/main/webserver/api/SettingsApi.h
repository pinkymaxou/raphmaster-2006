#ifndef _SETTINGSAPI_H_
#define _SETTINGSAPI_H_

#include <stdint.h>
#include <stdbool.h>

/* ==========================
   Station Settings
   ========================== */
char* SETTINGSAPI_GetStationSettings();

bool SETTINGSAPI_SetStationSettings(const char* szRequestBuffer, uint32_t u32Length);

/* ==========================
   Network Settings
   ========================== */
bool SETTINGSAPI_SetNetworkSettings(const char* szJSON, uint32_t u32Length);

char* SETTINGSAPI_GetNetworkSettings();

#endif