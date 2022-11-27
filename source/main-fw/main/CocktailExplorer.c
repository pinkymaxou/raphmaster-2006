#include <stdio.h>
#include "esp_log.h"

#include "CocktailExplorer.h"
#include "StationSettings.h"
#include "assets/EmbedCocktailDb.h"
#include "cJSON.h"

#define TAG "CocktailExplorer"

// Load everything into memory, it's easier
static cocktaildb_IngredientFile m_sIngredientFile;
static cocktaildb_RecipeFile m_sRecipeFile;

void COCKTAILEXPLORER_Init()
{
    ESP_LOGI(TAG, "Loading ingredient, block size: %d", sizeof(m_sIngredientFile));

    const EFEMBEDCOCKTAILDB_SFile* psIngredientFile = &EFEMBEDCOCKTAILDB_g_sFiles[EFEMBEDCOCKTAILDB_EFILE_INGREDIENTS_BIN];

    pb_istream_t ingredientStream = pb_istream_from_buffer(psIngredientFile->pu8StartAddr, psIngredientFile->u32Length);
    bool status1 = pb_decode(&ingredientStream, cocktaildb_IngredientFile_fields, &m_sIngredientFile);
    if (!status1)
    {
        ESP_LOGE(TAG, "Unable to open file");
        return;
    }

    ESP_LOGI(TAG, "Loaded ingredient, total: %d", m_sIngredientFile.ingredient_entries_count);

    ESP_LOGI(TAG, "Loading recipes, block size: %d", sizeof(m_sRecipeFile));

    const EFEMBEDCOCKTAILDB_SFile* psRecipeFile = &EFEMBEDCOCKTAILDB_g_sFiles[EFEMBEDCOCKTAILDB_EFILE_RECIPES_BIN];

    pb_istream_t recipeStream = pb_istream_from_buffer(psRecipeFile->pu8StartAddr, psRecipeFile->u32Length);
    bool status2 = pb_decode(&recipeStream, cocktaildb_RecipeFile_fields, &m_sRecipeFile);
    if (!status2)
    {
        ESP_LOGE(TAG, "Unable to open file");
        return;
    }

    ESP_LOGI(TAG, "Loaded recipes, total: %d", m_sRecipeFile.entries_count);
}

const cocktaildb_Ingredient* COCKTAILEXPLORER_GetIngredientFile(uint32_t u32ID)
{
    for(int i = 0; i < m_sIngredientFile.ingredient_entries_count; i++)
    {
        const cocktaildb_Ingredient* pIngredientFile = &m_sIngredientFile.ingredient_entries[i];
        if (pIngredientFile->id == u32ID)
            return pIngredientFile;
    }

    return NULL;
}

char* COCKTAILEXPLORER_GetAllRecipes()
{
    cJSON* pRoot;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
        goto ERROR;

    char tmp[100+1];

    for(int i = 0; i < m_sRecipeFile.entries_count; i++)
    {
        cocktaildb_Recipe* pRecipe = &m_sRecipeFile.entries[i];

        cJSON* pNewRecipe = cJSON_CreateObject();
        cJSON_AddItemToObject(pNewRecipe, "id", cJSON_CreateNumber(pRecipe->id));
        cJSON_AddItemToObject(pNewRecipe, "name", cJSON_CreateString(pRecipe->name));

        if (strlen(pRecipe->imgfile) > 0)
        {
            snprintf(tmp, sizeof(tmp) - 1, "img/c/%s", pRecipe->imgfile);
            cJSON_AddItemToObject(pNewRecipe, "img", cJSON_CreateString(tmp));
        }

        cJSON* pSteps = cJSON_AddArrayToObject(pNewRecipe, "steps");

        for(int j = 0; j < pRecipe->recipe_steps_count; j++)
        {
            cocktaildb_RecipeStep* pRecipeStep = &pRecipe->recipe_steps[j];

            // We do not support group yet.
            if (pRecipeStep->which_ingredient != cocktaildb_RecipeStep_ingredient_id_tag)
                continue;

            const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientFile(pRecipeStep->ingredient.ingredient_id);
            if (pIngredient == NULL)
                continue;

            cJSON* pNewStep = cJSON_CreateObject();

            cJSON_AddItemToObject(pNewStep, "ingredient_id", cJSON_CreateNumber(pIngredient->id));
            cJSON_AddItemToObject(pNewStep, "name", cJSON_CreateString(pIngredient->name));
            cJSON_AddItemToObject(pNewStep, "type", cJSON_CreateNumber(pIngredient->ingredient_type));
            cJSON_AddItemToObject(pNewStep, "is_garnish", cJSON_CreateBool(pRecipeStep->is_garnish));

            if (pRecipeStep->has_qty)
            {
                cJSON_AddItemToObject(pNewStep, "qty", cJSON_CreateNumber(pRecipeStep->qty.value));
                cJSON_AddItemToObject(pNewStep, "unit", cJSON_CreateNumber(pRecipeStep->qty.type));
            }

            cJSON_AddItemToArray(pSteps, pNewStep);
        }

        cJSON_AddItemToArray(pRoot, pNewRecipe);
    }

    char* pStr =  cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return NULL;
}

char* COCKTAILEXPLORER_GetAllIngredients(bool bIsLiquidOnly)
{
    cJSON* pRoot;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
        goto ERROR;

    for(int i = 0; i < m_sIngredientFile.ingredient_entries_count; i++)
    {
        cocktaildb_Ingredient* pIngredient = &m_sIngredientFile.ingredient_entries[i];

        bool bIsOK = true;
        if (bIsLiquidOnly)
            bIsOK = (pIngredient->ingredient_type == cocktaildb_EIngredientType_liquid_alcohol || pIngredient->ingredient_type == cocktaildb_EIngredientType_liquid_filler);
        if (!bIsOK)
            continue;

        cJSON* pNewRecipe = cJSON_CreateObject();
        cJSON_AddItemToObject(pNewRecipe, "id", cJSON_CreateNumber(pIngredient->id));
        cJSON_AddItemToObject(pNewRecipe, "name", cJSON_CreateString(pIngredient->name));

        cJSON_AddItemToArray(pRoot, pNewRecipe);
    }

    char* pStr =  cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return NULL;
}

char* COCKTAILEXPLORER_GetAllAvailableIngredients()
{
    cJSON* pRoot;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
        goto ERROR;

    for(int stationId = 1; stationId < STATIONSETTINGS_STATION_COUNT; stationId++)
    {
        int32_t s32IngredientId = STATIONSETTINGS_GetValue(stationId, STATIONSETTINGS_ESTATIONSET_LoadID);
        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientFile((uint32_t)s32IngredientId);
        if (pIngredient == NULL)
            continue;

        cJSON* pNewRecipe = cJSON_CreateObject();
        cJSON_AddItemToObject(pNewRecipe, "station_id", cJSON_CreateNumber(stationId));
        cJSON_AddItemToObject(pNewRecipe, "ingredient_id", cJSON_CreateNumber(pIngredient->id));
        cJSON_AddItemToObject(pNewRecipe, "name", cJSON_CreateString(pIngredient->name));

        cJSON_AddItemToArray(pRoot, pNewRecipe);
    }

    char* pStr =  cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return NULL;
}

char* COCKTAILEXPLORER_GetStationSettings()
{
    cJSON* pRoot = NULL;

    char buff[100];
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
        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientFile((uint32_t)s32IngredientId);
        if (pIngredient == NULL)
            cJSON_AddItemToObject(pNewStation, "ingredient_id", cJSON_CreateNumber(0));
        else
            cJSON_AddItemToObject(pNewStation, "ingredient_id", cJSON_CreateNumber(pIngredient->id));

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
            pIngredient = COCKTAILEXPLORER_GetIngredientFile((uint32_t)pIngredientIdJSON->valueint);
            if (pIngredient == NULL)
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