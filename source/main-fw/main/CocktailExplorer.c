#include "esp_log.h"

#include "CocktailExplorer.h" 
#include "assets/EmbedCocktailDb.h"
#include "cJSON.h"

#define TAG "CocktailExplorer"

static const cocktaildb_Ingredient* GetIngredientFile(uint32_t u32ID);

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

const cocktaildb_IngredientFile* COCKTAILEXPLORER_GetIngredientFiles()
{
    return &m_sIngredientFile;
}

const cocktaildb_RecipeFile* COCKTAILEXPLORER_GetRecipeFiles()
{
    return &m_sRecipeFile;
}

static const cocktaildb_Ingredient* GetIngredientFile(uint32_t u32ID)
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
    pRoot = cJSON_CreateObject();
    if (pRoot == NULL)
        goto ERROR;

    cJSON* pRecipes = cJSON_AddArrayToObject(pRoot, "recipes");

    for(int i = 0; i < m_sRecipeFile.entries_count; i++)
    {
        cocktaildb_Recipe* pRecipe = &m_sRecipeFile.entries[i];

        cJSON* pNewRecipe = cJSON_CreateObject();
        cJSON_AddItemToObject(pNewRecipe, "name", cJSON_CreateString(pRecipe->name));
        cJSON_AddItemToObject(pNewRecipe, "imgfile", cJSON_CreateString(pRecipe->imgfile));

        cJSON* pSteps = cJSON_AddArrayToObject(pNewRecipe, "steps");
        for(int j = 0; j < pRecipe->recipe_steps_count; j++)
        {
            cocktaildb_RecipeStep* pRecipeStep = &pRecipe->recipe_steps[j];

            const cocktaildb_Ingredient* pIngredient = GetIngredientFile(pRecipeStep->ingredient_id);
            if (pIngredient == NULL)
                continue;

            cJSON* pNewStep = cJSON_CreateObject();
            cJSON_AddItemToObject(pNewStep, "name", cJSON_CreateString(pIngredient->name));
            if (pIngredient->ingredient_type == cocktaildb_EIngredientType_alcohol)
                cJSON_AddItemToObject(pNewStep, "type", cJSON_CreateString("alcohol"));
            else if (pIngredient->ingredient_type == cocktaildb_EIngredientType_garnish)
                cJSON_AddItemToObject(pNewStep, "type", cJSON_CreateString("garnish"));

            if (pRecipeStep->has_qty)
            {
                cJSON_AddItemToObject(pNewStep, "qty", cJSON_CreateNumber(pRecipeStep->qty.value));
                cJSON_AddItemToObject(pNewStep, "unit", cJSON_CreateNumber(pRecipeStep->qty.type));
            }

            cJSON_AddItemToArray(pSteps, pNewStep);
        }

        cJSON_AddItemToArray(pRecipes, pNewRecipe);
    }

    char* pStr =  cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    cJSON_Delete(pRoot);
    return NULL;
}