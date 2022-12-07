#include "Settings.h"
#include "cJSON.h"
#include "esp_log.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "../../Settings.h"
#include "../../StationSettings.h"
#include "../../CocktailExplorer.h"

#define TAG "API/Settings"

char* SETTINGSAPI_GetStationSettings()
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
        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientById((uint32_t)s32IngredientId);
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

bool SETTINGSAPI_SetStationSettings(const char* szRequestBuffer, uint32_t u32Length)
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
            pIngredient = COCKTAILEXPLORER_GetIngredientById((uint32_t)pIngredientIdJSON->valueint);
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


bool SETTINGSAPI_SetNetworkSettings(const char* szJSON, uint32_t u32Length)
{
    const char* szError = NULL;

    cJSON* pRoot = cJSON_ParseWithLength(szJSON, u32Length);

    cJSON* pWifiSAP_Password = NULL;

    cJSON* pWifiSTA_IsActive = NULL;
    cJSON* pWifiSTA_SSID = NULL;
    cJSON* pWifiSTA_Password = NULL;

    // If not specified, just ignore
    cJSON* pWifiSAP = cJSON_GetObjectItemCaseSensitive(pRoot, "wifi_sap");
    if (pWifiSAP != NULL)
    {
        pWifiSAP_Password = cJSON_GetObjectItemCaseSensitive(pWifiSAP, "password");
        if (pWifiSAP_Password == NULL || cJSON_IsNull(pWifiSAP_Password))
            pWifiSAP_Password = NULL;
        else if (!cJSON_IsString(pWifiSAP_Password))
        {
            szError = "SAP password field is invalid";
            goto ERROR;
        }
    }

    // If not specified, just ignore
    cJSON* pWifiSTA = cJSON_GetObjectItemCaseSensitive(pRoot, "wifi_sta");
    if (pWifiSTA != NULL)
    {
        pWifiSTA_IsActive = cJSON_GetObjectItemCaseSensitive(pWifiSTA, "is_active");

        if (pWifiSTA_IsActive == NULL || cJSON_IsNull(pWifiSTA_IsActive))
            pWifiSTA_IsActive = NULL;
        else if (!cJSON_IsBool(pWifiSTA_IsActive))
        {
            szError = "STA is active field is invalid";
            goto ERROR;
        }

        pWifiSTA_SSID = cJSON_GetObjectItemCaseSensitive(pWifiSTA, "ssid");
        if (pWifiSTA_SSID == NULL || cJSON_IsNull(pWifiSTA_SSID))
            pWifiSTA_SSID = NULL;
        else if (!cJSON_IsString(pWifiSTA_SSID))
        {
            szError = "STA SSID field is invalid";
            goto ERROR;
        }

        pWifiSTA_Password = cJSON_GetObjectItemCaseSensitive(pWifiSTA, "password");
        if (pWifiSTA_Password == NULL || cJSON_IsNull(pWifiSTA_Password))
            pWifiSTA_Password = NULL;
        else if (!cJSON_IsString(pWifiSTA_Password))
        {
            szError = "STA password field is invalid";
            goto ERROR;
        }
    }

    // Check if value are valids
    for(int isDryRun = 1; isDryRun >= 0; isDryRun--)
    {
        if (pWifiSAP_Password != NULL)
            if (NVSJSON_SetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WAPPass, (bool)isDryRun, pWifiSAP_Password->valuestring) != NVSJSON_ESETRET_OK)
            {
                szError = "WAP password is invalid";
                goto ERROR;
            }

        if (pWifiSTA_IsActive != NULL)
            if (NVSJSON_SetValueInt32(&g_sSettingHandle, SETTINGS_EENTRY_WSTAIsActive, (bool)isDryRun, pWifiSTA_IsActive->valueint) != NVSJSON_ESETRET_OK)
            {
                szError = "STA SSID is invalid";
                goto ERROR;
            }

        if (pWifiSTA_SSID != NULL)
            if (NVSJSON_SetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WSTASSID, (bool)isDryRun, pWifiSTA_SSID->valuestring) != NVSJSON_ESETRET_OK)
            {
                szError = "STA SSID is invalid";
                goto ERROR;
            }

        if (pWifiSTA_Password != NULL)
            if (NVSJSON_SetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WSTAPass, (bool)isDryRun, pWifiSTA_Password->valuestring) != NVSJSON_ESETRET_OK)
            {
                szError = "STA password is invalid";
                goto ERROR;
            }
    }

    cJSON_Delete(pRoot);
    return true;
    ERROR:
    if (szError != NULL)
        ESP_LOGE(TAG, "Error: %s", szError);
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return false;
}

char* SETTINGSAPI_GetNetworkSettings()
{
    cJSON* pRoot = NULL;
    pRoot = cJSON_CreateObject();
    if (pRoot == NULL)
        goto ERROR;

    // Soft Access Point config
    wifi_config_t wifi_configSAP;
    esp_wifi_get_config(WIFI_IF_AP, &wifi_configSAP);

    cJSON* pNewWiFiSap = cJSON_CreateObject();
    cJSON_AddItemToObject(pNewWiFiSap, "ssid", cJSON_CreateString((char*)wifi_configSAP.ap.ssid));
    cJSON_AddItemToObject(pRoot, "wifi_sap", pNewWiFiSap);

    cJSON* pNewWiFiSTA = cJSON_CreateObject();
    char wifiSTASSID[32+1];
    size_t wifiSTASSIDLength = 32;
    NVSJSON_GetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WSTASSID, (char*)wifiSTASSID, &wifiSTASSIDLength);
    const bool bIsWifiSTAActive = NVSJSON_GetValueInt32(&g_sSettingHandle, SETTINGS_EENTRY_WSTAIsActive) == true;
    cJSON_AddItemToObject(pNewWiFiSTA, "is_active", cJSON_CreateBool(bIsWifiSTAActive));
    cJSON_AddItemToObject(pNewWiFiSTA, "ssid", cJSON_CreateString(wifiSTASSID));
    cJSON_AddItemToObject(pRoot, "wifi_sta", pNewWiFiSTA);

    char* pStr =  cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return NULL;
}
