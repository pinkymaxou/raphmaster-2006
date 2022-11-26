#include "StationSettings.h"
#include <string.h>

NVSJSON_SHandle g_sStationSettingHandle;

static const NVSJSON_SSettingEntry m_arrsConfigEntries[STATIONSETTINGS_EENTRY_Count] =
{
    // Stations
    [STATIONSETTINGS_EENTRY_STATION1_POS_X] = NVSJSON_INITINT32_RNG("S1.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION1_POS_Y] = NVSJSON_INITINT32_RNG("S1.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION1_LOADID] = NVSJSON_INITINT32_RNG("S1.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION1_TOTALQTY] = NVSJSON_INITINT32_RNG("S1.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION1_USEDQTY] = NVSJSON_INITINT32_RNG("S1.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION2_POS_X] = NVSJSON_INITINT32_RNG("S2.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION2_POS_Y] = NVSJSON_INITINT32_RNG("S2.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION2_LOADID] = NVSJSON_INITINT32_RNG("S2.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION2_TOTALQTY] = NVSJSON_INITINT32_RNG("S2.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION2_USEDQTY] = NVSJSON_INITINT32_RNG("S2.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION3_POS_X] = NVSJSON_INITINT32_RNG("S3.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION3_POS_Y] = NVSJSON_INITINT32_RNG("S3.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION3_LOADID] = NVSJSON_INITINT32_RNG("S3.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION3_TOTALQTY] = NVSJSON_INITINT32_RNG("S3.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION3_USEDQTY] = NVSJSON_INITINT32_RNG("S3.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION4_POS_X] = NVSJSON_INITINT32_RNG("S4.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION4_POS_Y] = NVSJSON_INITINT32_RNG("S4.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION4_LOADID] = NVSJSON_INITINT32_RNG("S4.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION4_TOTALQTY] = NVSJSON_INITINT32_RNG("S4.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION4_USEDQTY] = NVSJSON_INITINT32_RNG("S4.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION5_POS_X] = NVSJSON_INITINT32_RNG("S5.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION5_POS_Y] = NVSJSON_INITINT32_RNG("S5.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION5_LOADID] = NVSJSON_INITINT32_RNG("S5.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION5_TOTALQTY] = NVSJSON_INITINT32_RNG("S5.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION5_USEDQTY] = NVSJSON_INITINT32_RNG("S5.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION6_POS_X] = NVSJSON_INITINT32_RNG("S6.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION6_POS_Y] = NVSJSON_INITINT32_RNG("S6.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION6_LOADID] = NVSJSON_INITINT32_RNG("S6.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION6_TOTALQTY] = NVSJSON_INITINT32_RNG("S6.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION6_USEDQTY] = NVSJSON_INITINT32_RNG("S6.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION7_POS_X] = NVSJSON_INITINT32_RNG("S7.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION7_POS_Y] = NVSJSON_INITINT32_RNG("S7.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION7_LOADID] = NVSJSON_INITINT32_RNG("S7.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION7_TOTALQTY] = NVSJSON_INITINT32_RNG("S7.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION7_USEDQTY] = NVSJSON_INITINT32_RNG("S7.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION8_POS_X] = NVSJSON_INITINT32_RNG("S8.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION8_POS_Y] = NVSJSON_INITINT32_RNG("S8.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION8_LOADID] = NVSJSON_INITINT32_RNG("S8.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION8_TOTALQTY] = NVSJSON_INITINT32_RNG("S8.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION8_USEDQTY] = NVSJSON_INITINT32_RNG("S8.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION9_POS_X] = NVSJSON_INITINT32_RNG("S9.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION9_POS_Y] = NVSJSON_INITINT32_RNG("S9.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION9_LOADID] = NVSJSON_INITINT32_RNG("S9.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION9_TOTALQTY] = NVSJSON_INITINT32_RNG("S9.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION9_USEDQTY] = NVSJSON_INITINT32_RNG("S9.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION10_POS_X] = NVSJSON_INITINT32_RNG("S10.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION10_POS_Y] = NVSJSON_INITINT32_RNG("S10.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION10_LOADID] = NVSJSON_INITINT32_RNG("S10.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION10_TOTALQTY] = NVSJSON_INITINT32_RNG("S10.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION10_USEDQTY] = NVSJSON_INITINT32_RNG("S10.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION11_POS_X] = NVSJSON_INITINT32_RNG("S11.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION11_POS_Y] = NVSJSON_INITINT32_RNG("S11.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION11_LOADID] = NVSJSON_INITINT32_RNG("S11.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION11_TOTALQTY] = NVSJSON_INITINT32_RNG("S11.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION11_USEDQTY] = NVSJSON_INITINT32_RNG("S11.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION12_POS_X] = NVSJSON_INITINT32_RNG("S12.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION12_POS_Y] = NVSJSON_INITINT32_RNG("S12.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION12_LOADID] = NVSJSON_INITINT32_RNG("S12.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION12_TOTALQTY] = NVSJSON_INITINT32_RNG("S12.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION12_USEDQTY] = NVSJSON_INITINT32_RNG("S12.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION13_POS_X] = NVSJSON_INITINT32_RNG("S13.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION13_POS_Y] = NVSJSON_INITINT32_RNG("S13.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION13_LOADID] = NVSJSON_INITINT32_RNG("S13.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION13_TOTALQTY] = NVSJSON_INITINT32_RNG("S13.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION13_USEDQTY] = NVSJSON_INITINT32_RNG("S13.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION14_POS_X] = NVSJSON_INITINT32_RNG("S14.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION14_POS_Y] = NVSJSON_INITINT32_RNG("S14.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION14_LOADID] = NVSJSON_INITINT32_RNG("S14.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION14_TOTALQTY] = NVSJSON_INITINT32_RNG("S14.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION14_USEDQTY] = NVSJSON_INITINT32_RNG("S14.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION15_POS_X] = NVSJSON_INITINT32_RNG("S15.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION15_POS_Y] = NVSJSON_INITINT32_RNG("S15.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION15_LOADID] = NVSJSON_INITINT32_RNG("S15.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION15_TOTALQTY] = NVSJSON_INITINT32_RNG("S15.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION15_USEDQTY] = NVSJSON_INITINT32_RNG("S15.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),

    [STATIONSETTINGS_EENTRY_STATION16_POS_X] = NVSJSON_INITINT32_RNG("S16.PosX", "Position X", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION16_POS_Y] = NVSJSON_INITINT32_RNG("S16.PosY", "Position Y", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION16_LOADID] = NVSJSON_INITINT32_RNG("S16.LoadID", "Ingredient ID, 0 = nothing", 0, 0, 100000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION16_TOTALQTY] = NVSJSON_INITINT32_RNG("S16.TotalML", "Total Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
    [STATIONSETTINGS_EENTRY_STATION16_USEDQTY] = NVSJSON_INITINT32_RNG("S16.UsedML", "Used Qty (ml)", 0, 0, 6000, NVSJSON_EFLAGS_None),
}

const NVSJSON_SConfig m_sConfig = { .szNamespace = "stationcfg" };

void STATIONSETTINGS_Init()
{
    NVSJSON_Init(&g_sSettingHandle, &m_sConfig, m_arrsConfigEntries, STATIONSETTINGS_EENTRY_Count);
}

static bool ValidateWifiPassword(const NVSJSON_SSettingEntry* pSettingEntry, const char* szValue)
{
    const size_t n = strlen(szValue);
    return n > 8 || n == 0;
}