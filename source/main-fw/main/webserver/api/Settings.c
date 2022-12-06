#include "Settings.h"
#include "cJSON.h"
#include "esp_log.h"
#include "../../StationSettings.h"
#include "../../CocktailExplorer.h"

#define TAG "API/Settings"

char* COCKTAILEXPLORER_GetStationSettings()
{
    cJSON* pRoot = NULL;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
        goto ERROR;

    for(int stationId = 1; stationId <= STATIONSETTINGS_STATION_COUNT; stationId++)
    {
        cJSON* pNewStation = cJSON_CreateObject();
        if (pNewStation == NULL)
            goto ERROR;

        cJSON_AddItemToObject(pNewStation, "id", cJSON_CreateNumber(stationId));

        int32_t s32IngredientId = STATIONSETTINGS_GetValue(stationId, STATIONSETTINGS_ESTATIONSET_LoadID);
        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredient((uint32_t)s32IngredientId);
        if (pIngredient == NULL || !COCKTAILEXPLORER_IsIngredientLiquid(pIngredient))
            cJSON_AddItemToObject(pNewStation, "ingredient_id", cJSON_CreateNumber(0));
        else
            cJSON_AddItemToObject(pNewStation, "ingredient_id", cJSON_CreateNumber(pIngredient->id));

        const bool bIsPeristaltic = STATIONSETTINGS_ISPUMPSTATIONID(stationId);
        cJSON_AddItemToObject(pNewStation, "is_peristaltic", cJSON_CreateBool(bIsPeristaltic));

        int32_t s32UsedTotal_ml = STATIONSETTINGS_GetValue(stationId, STATIONSETTINGS_ESTATIONSET_TotalQty);
        cJSON_AddItemToObject(pNewStation, "total_ml", cJSON_CreateNumber(s32UsedTotal_ml));
        int32_t s32UsedQty_ml = STATIONSETTINGS_GetValue(stationId, STATIONSETTINGS_ESTATIONSET_UsedQty);
        cJSON_AddItemToObject(pNewStation, "used_ml", cJSON_CreateNumber(s32UsedQty_ml));

        cJSON_AddItemToArray(pRoot, pNewStation);
    }

    char* pStr =  cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    ESP_LOGE(TAG, "Unable to encode data");
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return NULL;
}

bool COCKTAILEXPLORER_SetStationSettings(const char* szRequestBuffer, uint32_t u32Length)
{
    cJSON* pRoot = cJSON_ParseWithLength(szRequestBuffer, u32Length);
    if (pRoot == NULL || !cJSON_IsArray(pRoot))
    {
        ESP_LOGE(TAG, "No an array");
        goto ERROR;
    }

    const cJSON* pSettingItems = NULL;
    cJSON_ArrayForEach(pSettingItems, pRoot)
    {
        const cJSON* pStationIdJSON = cJSON_GetObjectItemCaseSensitive(pSettingItems, "id");
        const cJSON* pIngredientIdJSON = cJSON_GetObjectItemCaseSensitive(pSettingItems, "ingredient_id");
        const cJSON* pTotalmlJSON = cJSON_GetObjectItemCaseSensitive(pSettingItems, "total_ml");
        const cJSON* pUsedmlJSON = cJSON_GetObjectItemCaseSensitive(pSettingItems, "used_ml");

        if (pStationIdJSON == NULL || !cJSON_IsNumber(pStationIdJSON) || !STATIONSETTINGS_ISVALIDSTATIONID(pStationIdJSON->valueint))
        {
            ESP_LOGE(TAG, "Invalid station id");
            goto ERROR;
        }
        if (pIngredientIdJSON == NULL || !cJSON_IsNumber(pIngredientIdJSON))
        {
            ESP_LOGE(TAG, "Invalid ingredient id");
            goto ERROR;
        }
        const cocktaildb_Ingredient* pIngredient = NULL;
        if (pIngredientIdJSON->valueint != 0)
        {
            pIngredient = COCKTAILEXPLORER_GetIngredient((uint32_t)pIngredientIdJSON->valueint);
            if (pIngredient == NULL || !COCKTAILEXPLORER_IsIngredientLiquid(pIngredient))
            {
                ESP_LOGE(TAG, "Invalid ingredient id, not found into db");
                goto ERROR;
            }
        }
        if (pTotalmlJSON == NULL || !cJSON_IsNumber(pTotalmlJSON))
        {
            ESP_LOGE(TAG, "Invalid total qty ml");
            goto ERROR;
        }
        if (pUsedmlJSON== NULL || !cJSON_IsNumber(pUsedmlJSON))
        {
            ESP_LOGE(TAG, "Invalid used qty id");
            goto ERROR;
        }

        if (STATIONSETTINGS_SetValue(pStationIdJSON->valueint, STATIONSETTINGS_ESTATIONSET_LoadID, pIngredientIdJSON->valueint) != NVSJSON_ESETRET_OK)
            goto ERROR;
        if (STATIONSETTINGS_SetValue(pStationIdJSON->valueint, STATIONSETTINGS_ESTATIONSET_TotalQty, pTotalmlJSON->valueint) != NVSJSON_ESETRET_OK)
            goto ERROR;
        if (STATIONSETTINGS_SetValue(pStationIdJSON->valueint, STATIONSETTINGS_ESTATIONSET_UsedQty, pUsedmlJSON->valueint) != NVSJSON_ESETRET_OK)
            goto ERROR;
        /* ESP_LOGI(TAG, "stationid: %d, ingredient_id: %d, total: %d, used: %d",
            pStationIdJSON->valueint, pIngredientIdJSON->valueint,
            pTotalmlJSON->valueint, pUsedmlJSON->valueint);*/
    }
    STATIONSETTINGS_CommitAll();

    cJSON_Delete(pRoot);
    return true;
    ERROR:
    ESP_LOGE(TAG, "Error during processing");
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return false;
}
