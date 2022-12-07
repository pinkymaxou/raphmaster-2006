#include "CocktailApi.h"
#include "cJSON.h"
#include "esp_log.h"
#include "nanopb/pb_decode.h"
#include "cocktaildb/cocktaildb.pb.h"
#include "../../StationSettings.h"
#include "../../CocktailExplorer.h"

#define TAG "API/Cocktail"

char* COCKTAILAPI_GetAllRecipes(uint32_t recipe_id)
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
    uint32_t u32AvailableIngredientCount = COCKTAILEXPLORER_GetAvailableIngredients(sAvailableIngredientIds);

    const cocktaildb_RecipeFile* psRecipeFile = COCKTAILEXPLORER_GetRecipeFile();

    for(int i = 0; i < psRecipeFile->entries_count; i++)
    {
        const cocktaildb_Recipe* pRecipe = &psRecipeFile->entries[i];

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

            const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientById(pRecipeStep->ingredient.ingredient_id);
            if (pIngredient == NULL)
                continue;

            cJSON* pNewStep = cJSON_CreateObject();

            cJSON_AddItemToObject(pNewStep, "ingredient_id", cJSON_CreateNumber(pIngredient->id));
            cJSON_AddItemToObject(pNewStep, "name", cJSON_CreateString(pIngredient->name));
            cJSON_AddItemToObject(pNewStep, "type", cJSON_CreateNumber(pIngredient->ingredient_type));
            cJSON_AddItemToObject(pNewStep, "is_garnish", cJSON_CreateBool(pRecipeStep->is_garnish));

            bool bIsAvail = false;
            if (COCKTAILEXPLORER_IsIngredientLiquid(pIngredient))
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

char* COCKTAILAPI_GetAllIngredients(bool bIsLiquidOnly)
{
    cJSON* pRoot;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
        goto ERROR;

    const cocktaildb_IngredientFile* psIngredientFile = COCKTAILEXPLORER_GetIngredientFile();

    for(int i = 0; i < psIngredientFile->ingredient_entries_count; i++)
    {
        const cocktaildb_Ingredient* pIngredient = &psIngredientFile->ingredient_entries[i];
        if (!(!bIsLiquidOnly || COCKTAILEXPLORER_IsIngredientLiquid(pIngredient)))
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

char* COCKTAILAPI_GetAllAvailableIngredients()
{
    cJSON* pRoot;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
        goto ERROR;

    for(int stationId = 1; stationId <= STATIONSETTINGS_STATION_COUNT; stationId++)
    {
        int32_t s32IngredientId = STATIONSETTINGS_GetValue(stationId, STATIONSETTINGS_ESTATIONSET_LoadID);
        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientById((uint32_t)s32IngredientId);
        if (pIngredient == NULL || !COCKTAILEXPLORER_IsIngredientLiquid(pIngredient))
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

char* COCKTAILAPI_GetStatIngredients()
{
    char* ret = NULL;
    cJSON* pRoot = NULL;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
        goto ERROR;

    const cocktaildb_IngredientFile* psIngredientFile = COCKTAILEXPLORER_GetIngredientFile();
    const cocktaildb_RecipeFile* psRecipeFile = COCKTAILEXPLORER_GetRecipeFile();

    for(int i = 0; i < psIngredientFile->ingredient_entries_count; i++)
    {
        const cocktaildb_Ingredient* pIngredient = &psIngredientFile->ingredient_entries[i];
        if (!COCKTAILEXPLORER_IsIngredientLiquid(pIngredient))
            continue;

        uint32_t u32Stat = 0;

        // Scan recipe to find the ingredient
        for(int j = 0; j < psRecipeFile->entries_count; j++)
        {
            const cocktaildb_Recipe* pRecipe = &psRecipeFile->entries[j];

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
