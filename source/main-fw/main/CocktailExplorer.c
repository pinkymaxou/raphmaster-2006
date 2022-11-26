#include <stdio.h>
#include "esp_log.h"

#include "CocktailExplorer.h" 
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
    
    ESP_LOGI(TAG, "Loaded ingredient, total: %d", m_sIngredientFile.entries_count);

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
    for(int i = 0; i < m_sIngredientFile.entries_count; i++)
    {
        const cocktaildb_Ingredient* pIngredientFile = &m_sIngredientFile.entries[i];
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
        cJSON_AddItemToObject(pNewRecipe, "id", cJSON_CreateNumber(pRecipe->ID));
        cJSON_AddItemToObject(pNewRecipe, "name", cJSON_CreateString(pRecipe->name));
        
        if (strlen(pRecipe->imgfile) > 0)
        {
            snprintf(tmp, sizeof(tmp) - 1, "img/%s", pRecipe->imgfile);
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
    cJSON_Delete(pRoot);
    return NULL;
}

char* COCKTAILEXPLORER_GetAllIngredients(bool bIsLiquidOnly)
{
    cJSON* pRoot;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
        goto ERROR;

    for(int i = 0; i < m_sIngredientFile.entries_count; i++)
    {
        cocktaildb_Ingredient* pIngredient = &m_sIngredientFile.entries[i];

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
    cJSON_Delete(pRoot);
    return NULL;  
}

char* COCKTAILEXPLORER_GetAllAvailableIngredients()
{
    cJSON* pRoot;
    pRoot = cJSON_CreateArray();
    if (pRoot == NULL)
        goto ERROR;

    for(int i = 0; i < m_sIngredientFile.entries_count; i++)
    {
        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientFile(0);
        if (pIngredient == NULL)
            continue;

        cJSON* pNewRecipe = cJSON_CreateObject();
        cJSON_AddItemToObject(pNewRecipe, "station_id", cJSON_CreateNumber(i+1));
        cJSON_AddItemToObject(pNewRecipe, "ingredient_id", cJSON_CreateNumber(pIngredient->id));
        cJSON_AddItemToObject(pNewRecipe, "name", cJSON_CreateString(pIngredient->name));
        
        cJSON_AddItemToArray(pRoot, pNewRecipe);
    }

    char* pStr =  cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    cJSON_Delete(pRoot);
    return NULL;  
}