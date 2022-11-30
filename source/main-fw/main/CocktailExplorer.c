#include <stdio.h>
#include "esp_log.h"
#include "esp_heap_caps.h"

#include "CocktailExplorer.h"
#include "StationSettings.h"
#include "assets/EmbedCocktailDb.h"
#include "cJSON.h"

#define TAG "CocktailExplorer"

// Load everything into memory, it's easier
static cocktaildb_IngredientFile* m_psIngredientFile;
static cocktaildb_RecipeFile* m_psRecipeFile;

static bool IsIngredientLiquid(const cocktaildb_Ingredient* pIngredient);
static uint32_t GetAvailableIngredients(const cocktaildb_Ingredient* sAvailableIngredientIds[STATIONSETTINGS_STATION_COUNT]);

void COCKTAILEXPLORER_Init()
{
    m_psIngredientFile = heap_caps_malloc(sizeof(cocktaildb_IngredientFile), MALLOC_CAP_SPIRAM);
    m_psRecipeFile = heap_caps_malloc(sizeof(cocktaildb_RecipeFile), MALLOC_CAP_SPIRAM);

    ESP_LOGI(TAG, "Loading ingredient, block size: %d", sizeof(cocktaildb_IngredientFile));

    const EFEMBEDCOCKTAILDB_SFile* psIngredientFile = &EFEMBEDCOCKTAILDB_g_sFiles[EFEMBEDCOCKTAILDB_EFILE_INGREDIENTS_BIN];

    pb_istream_t ingredientStream = pb_istream_from_buffer(psIngredientFile->pu8StartAddr, psIngredientFile->u32Length);
    bool status1 = pb_decode(&ingredientStream, cocktaildb_IngredientFile_fields, m_psIngredientFile);
    if (!status1)
    {
        ESP_LOGE(TAG, "Unable to open file");
        return;
    }

    ESP_LOGI(TAG, "Loaded ingredient, total: %d", m_psIngredientFile->ingredient_entries_count);

    ESP_LOGI(TAG, "Loading recipes, block size: %d", sizeof(cocktaildb_RecipeFile));

    const EFEMBEDCOCKTAILDB_SFile* psRecipeFile = &EFEMBEDCOCKTAILDB_g_sFiles[EFEMBEDCOCKTAILDB_EFILE_RECIPES_BIN];

    pb_istream_t recipeStream = pb_istream_from_buffer(psRecipeFile->pu8StartAddr, psRecipeFile->u32Length);
    bool status2 = pb_decode(&recipeStream, cocktaildb_RecipeFile_fields, m_psRecipeFile);
    if (!status2)
    {
        ESP_LOGE(TAG, "Unable to open file");
        return;
    }

    ESP_LOGI(TAG, "Loaded recipes, total: %d", m_psRecipeFile->entries_count);
}

const cocktaildb_Ingredient* COCKTAILEXPLORER_GetIngredientFile(uint32_t u32ID)
{
    for(int i = 0; i < m_psIngredientFile->ingredient_entries_count; i++)
    {
        const cocktaildb_Ingredient* pIngredientFile = &m_psIngredientFile->ingredient_entries[i];
        if (pIngredientFile->id == u32ID)
            return pIngredientFile;
    }

    return NULL;
}

char* COCKTAILEXPLORER_GetAllRecipes(uint32_t recipe_id)
{
    const char* szError = NULL;
    cJSON* pRoot;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
    {
        szError = "Cannot create array";
        goto ERROR;
    }

    char tmp[100+1];

    const cocktaildb_Ingredient* sAvailableIngredientIds[STATIONSETTINGS_STATION_COUNT];
    uint32_t u32AvailableIngredientCount = GetAvailableIngredients(sAvailableIngredientIds);

    for(int i = 0; i < m_psRecipeFile->entries_count; i++)
    {
        cocktaildb_Recipe* pRecipe = &m_psRecipeFile->entries[i];

        if (recipe_id != 0 &&
            pRecipe->id != recipe_id)
            continue;

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

            bool bIsAvail = false;
            if (IsIngredientLiquid(pIngredient))
            {
                // Check if the ingredient is available
                for(int i = 0; i < u32AvailableIngredientCount; i++)
                {
                    if (sAvailableIngredientIds[i]->id == pIngredient->id)
                    {
                        bIsAvail = true;
                        break;
                    }
                }
            }
            cJSON_AddItemToObject(pNewStep, "is_avail", cJSON_CreateBool(bIsAvail));

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
    if (pStr == NULL)
    {
        szError = "Cannot generate JSON, out of memory";
        goto ERROR;
    }
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    if (szError != NULL)
        ESP_LOGE(TAG, "Cocktail explorer error: %s", szError);
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

    for(int i = 0; i < m_psIngredientFile->ingredient_entries_count; i++)
    {
        cocktaildb_Ingredient* pIngredient = &m_psIngredientFile->ingredient_entries[i];
        if (!(!bIsLiquidOnly || IsIngredientLiquid(pIngredient)))
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
        if (pIngredient == NULL || !IsIngredientLiquid(pIngredient))
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

char* COCKTAILEXPLORER_GetStatIngredients()
{
    char* ret = NULL;
    cJSON* pRoot = NULL;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
        goto ERROR;

    for(int i = 0; i < m_psIngredientFile->ingredient_entries_count; i++)
    {
        const cocktaildb_Ingredient* pIngredient = &m_psIngredientFile->ingredient_entries[i];
        if (!IsIngredientLiquid(pIngredient))
            continue;

        uint32_t u32Stat = 0;

        // Scan recipe to find the ingredient
        for(int j = 0; j < m_psRecipeFile->entries_count; j++)
        {
            const cocktaildb_Recipe* pRecipe = &m_psRecipeFile->entries[j];

            for(int k = 0; k < pRecipe->recipe_steps_count; k++)
            {
                const cocktaildb_RecipeStep* pRecipeStep = &pRecipe->recipe_steps[k];

                if (pRecipeStep->which_ingredient != cocktaildb_RecipeStep_ingredient_id_tag ||
                    pRecipeStep->ingredient.ingredient_id != pIngredient->id)
                    continue;
                u32Stat++;
            }
        }

        cJSON* pNewRecipe = cJSON_CreateObject();
        cJSON_AddItemToObject(pNewRecipe, "ingredient_id", cJSON_CreateNumber(pIngredient->id));
        cJSON_AddItemToObject(pNewRecipe, "name", cJSON_CreateString(pIngredient->name));
        cJSON_AddItemToObject(pNewRecipe, "count", cJSON_CreateNumber(u32Stat));
        cJSON_AddItemToArray(pRoot, pNewRecipe);
    }

    ret = cJSON_PrintUnformatted(pRoot);
    goto END;
    ERROR:
    ESP_LOGE(TAG, "Unable to get stat ingredients");
    END:
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return ret;
}

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
        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientFile((uint32_t)s32IngredientId);
        if (pIngredient == NULL || !IsIngredientLiquid(pIngredient))
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
            if (pIngredient == NULL || !IsIngredientLiquid(pIngredient))
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

static bool IsIngredientLiquid(const cocktaildb_Ingredient* pIngredient)
{
    return pIngredient->ingredient_type == cocktaildb_EIngredientType_liquid_alcohol || pIngredient->ingredient_type == cocktaildb_EIngredientType_liquid_filler;
}

static uint32_t GetAvailableIngredients(const cocktaildb_Ingredient* sAvailableIngredientIds[STATIONSETTINGS_STATION_COUNT])
{
    uint32_t u32Count = 0;

    for(int stationId = 1; stationId < STATIONSETTINGS_STATION_COUNT; stationId++)
    {
        int32_t s32IngredientId = STATIONSETTINGS_GetValue(stationId, STATIONSETTINGS_ESTATIONSET_LoadID);
        if (s32IngredientId == 0)
            continue;
        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientFile((uint32_t)s32IngredientId);
         if (pIngredient == NULL || !IsIngredientLiquid(pIngredient))
            continue;
        sAvailableIngredientIds[u32Count] = pIngredient;
        u32Count++;
    }

    return u32Count;
}