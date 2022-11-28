#ifndef _STATIONSETTINGS_H_
#define _STATIONSETTINGS_H_

#include "nvsjson.h"

typedef enum
{
    // Stations section
    STATIONSETTINGS_EENTRY_STATION1_PosX,
    STATIONSETTINGS_EENTRY_STATION1_PosY,
    STATIONSETTINGS_EENTRY_STATION1_LoadID,
    STATIONSETTINGS_EENTRY_STATION1_TotalQty,
    STATIONSETTINGS_EENTRY_STATION1_UsedQty,

    STATIONSETTINGS_EENTRY_STATION2_PosX,
    STATIONSETTINGS_EENTRY_STATION2_PosY,
    STATIONSETTINGS_EENTRY_STATION2_LoadID,
    STATIONSETTINGS_EENTRY_STATION2_TotalQty,
    STATIONSETTINGS_EENTRY_STATION2_UsedQty,

    STATIONSETTINGS_EENTRY_STATION3_PosX,
    STATIONSETTINGS_EENTRY_STATION3_PosY,
    STATIONSETTINGS_EENTRY_STATION3_LoadID,
    STATIONSETTINGS_EENTRY_STATION3_TotalQty,
    STATIONSETTINGS_EENTRY_STATION3_UsedQty,

    STATIONSETTINGS_EENTRY_STATION4_PosX,
    STATIONSETTINGS_EENTRY_STATION4_PosY,
    STATIONSETTINGS_EENTRY_STATION4_LoadID,
    STATIONSETTINGS_EENTRY_STATION4_TotalQty,
    STATIONSETTINGS_EENTRY_STATION4_UsedQty,

    STATIONSETTINGS_EENTRY_STATION5_PosX,
    STATIONSETTINGS_EENTRY_STATION5_PosY,
    STATIONSETTINGS_EENTRY_STATION5_LoadID,
    STATIONSETTINGS_EENTRY_STATION5_TotalQty,
    STATIONSETTINGS_EENTRY_STATION5_UsedQty,

    STATIONSETTINGS_EENTRY_STATION6_PosX,
    STATIONSETTINGS_EENTRY_STATION6_PosY,
    STATIONSETTINGS_EENTRY_STATION6_LoadID,
    STATIONSETTINGS_EENTRY_STATION6_TotalQty,
    STATIONSETTINGS_EENTRY_STATION6_UsedQty,

    STATIONSETTINGS_EENTRY_STATION7_PosX,
    STATIONSETTINGS_EENTRY_STATION7_PosY,
    STATIONSETTINGS_EENTRY_STATION7_LoadID,
    STATIONSETTINGS_EENTRY_STATION7_TotalQty,
    STATIONSETTINGS_EENTRY_STATION7_UsedQty,

    STATIONSETTINGS_EENTRY_STATION8_PosX,
    STATIONSETTINGS_EENTRY_STATION8_PosY,
    STATIONSETTINGS_EENTRY_STATION8_LoadID,
    STATIONSETTINGS_EENTRY_STATION8_TotalQty,
    STATIONSETTINGS_EENTRY_STATION8_UsedQty,

    STATIONSETTINGS_EENTRY_STATION9_PosX,
    STATIONSETTINGS_EENTRY_STATION9_PosY,
    STATIONSETTINGS_EENTRY_STATION9_LoadID,
    STATIONSETTINGS_EENTRY_STATION9_TotalQty,
    STATIONSETTINGS_EENTRY_STATION9_UsedQty,

    STATIONSETTINGS_EENTRY_STATION10_PosX,
    STATIONSETTINGS_EENTRY_STATION10_PosY,
    STATIONSETTINGS_EENTRY_STATION10_LoadID,
    STATIONSETTINGS_EENTRY_STATION10_TotalQty,
    STATIONSETTINGS_EENTRY_STATION10_UsedQty,

    STATIONSETTINGS_EENTRY_STATION11_PosX,
    STATIONSETTINGS_EENTRY_STATION11_PosY,
    STATIONSETTINGS_EENTRY_STATION11_LoadID,
    STATIONSETTINGS_EENTRY_STATION11_TotalQty,
    STATIONSETTINGS_EENTRY_STATION11_UsedQty,

    STATIONSETTINGS_EENTRY_STATION12_PosX,
    STATIONSETTINGS_EENTRY_STATION12_PosY,
    STATIONSETTINGS_EENTRY_STATION12_LoadID,
    STATIONSETTINGS_EENTRY_STATION12_TotalQty,
    STATIONSETTINGS_EENTRY_STATION12_UsedQty,

    STATIONSETTINGS_EENTRY_STATION13_PosX,
    STATIONSETTINGS_EENTRY_STATION13_PosY,
    STATIONSETTINGS_EENTRY_STATION13_LoadID,
    STATIONSETTINGS_EENTRY_STATION13_TotalQty,
    STATIONSETTINGS_EENTRY_STATION13_UsedQty,

    STATIONSETTINGS_EENTRY_STATION14_PosX,
    STATIONSETTINGS_EENTRY_STATION14_PosY,
    STATIONSETTINGS_EENTRY_STATION14_LoadID,
    STATIONSETTINGS_EENTRY_STATION14_TotalQty,
    STATIONSETTINGS_EENTRY_STATION14_UsedQty,

    STATIONSETTINGS_EENTRY_STATION15_PosX,
    STATIONSETTINGS_EENTRY_STATION15_PosY,
    STATIONSETTINGS_EENTRY_STATION15_LoadID,
    STATIONSETTINGS_EENTRY_STATION15_TotalQty,
    STATIONSETTINGS_EENTRY_STATION15_UsedQty,

    STATIONSETTINGS_EENTRY_STATION16_PosX,
    STATIONSETTINGS_EENTRY_STATION16_PosY,
    STATIONSETTINGS_EENTRY_STATION16_LoadID,
    STATIONSETTINGS_EENTRY_STATION16_TotalQty,
    STATIONSETTINGS_EENTRY_STATION16_UsedQty,

    STATIONSETTINGS_EENTRY_STATION17_PosX,
    STATIONSETTINGS_EENTRY_STATION17_PosY,
    STATIONSETTINGS_EENTRY_STATION17_LoadID,
    STATIONSETTINGS_EENTRY_STATION17_TotalQty,
    STATIONSETTINGS_EENTRY_STATION17_UsedQty,

    STATIONSETTINGS_EENTRY_STATION18_PosX,
    STATIONSETTINGS_EENTRY_STATION18_PosY,
    STATIONSETTINGS_EENTRY_STATION18_LoadID,
    STATIONSETTINGS_EENTRY_STATION18_TotalQty,
    STATIONSETTINGS_EENTRY_STATION18_UsedQty,
    STATIONSETTINGS_EENTRY_Count
} STATIONSETTINGS_EENTRY;

typedef enum
{
    STATIONSETTINGS_ESTATIONSET_PosX = 0,
    STATIONSETTINGS_ESTATIONSET_PosY,
    STATIONSETTINGS_ESTATIONSET_LoadID,
    STATIONSETTINGS_ESTATIONSET_TotalQty,
    STATIONSETTINGS_ESTATIONSET_UsedQty,

    STATIONSETTINGS_ESTATIONSET_Count
} STATIONSETTINGS_ESTATIONSET;

#define STATIONSETTINGS_STATION_COUNT (18)
#define STATIONSETTINGS_ISVALIDSTATIONID(_stationid) (_stationid >= 1 && _stationid <= STATIONSETTINGS_STATION_COUNT)

void STATIONSETTINGS_Init();

int32_t STATIONSETTINGS_GetValue(int stationId, STATIONSETTINGS_ESTATIONSET eStationSet);

NVSJSON_ESETRET STATIONSETTINGS_SetValue(int stationId, STATIONSETTINGS_ESTATIONSET eStationSet, int32_t s32Value);

void STATIONSETTINGS_CommitAll();

extern NVSJSON_SHandle g_sStationSettingHandle;

#endif