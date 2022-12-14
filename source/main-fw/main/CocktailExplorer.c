#include <stdio.h>
#include "esp_log.h"
#include "esp_heap_caps.h"
#include "CocktailExplorer.h"
#include "assets/EmbedCocktailDb.h"
#include "cJSON.h"

#define TAG "CocktailExplorer"

static int32_t binarySearch(const int32_t array[], int size, int32_t s32SearchValue);

// Load everything into memory, it's easier
static cocktaildb_IngredientFile* m_psIngredientFile;
static int32_t* m_s32IngredientIDIndexes;
static int32_t* m_s32IngredientCategoryIDIndexes;

static cocktaildb_RecipeFile* m_psRecipeFile;

void COCKTAILEXPLORER_Init()
{
    m_psIngredientFile = heap_caps_malloc(sizeof(cocktaildb_IngredientFile), MALLOC_CAP_SPIRAM);
    m_psRecipeFile = heap_caps_malloc(sizeof(cocktaildb_RecipeFile), MALLOC_CAP_SPIRAM);

    // ------------------------------
    // Ingredient
    // ------------------------------
    ESP_LOGI(TAG, "Loading ingredient, block size: %d", sizeof(cocktaildb_IngredientFile));

    const EFEMBEDCOCKTAILDB_SFile* psIngredientFile = &EFEMBEDCOCKTAILDB_g_sFiles[EFEMBEDCOCKTAILDB_EFILE_INGREDIENTS_BIN];

    pb_istream_t ingredientStream = pb_istream_from_buffer(psIngredientFile->pu8StartAddr, psIngredientFile->u32Length);
    bool status1 = pb_decode(&ingredientStream, cocktaildb_IngredientFile_fields, m_psIngredientFile);
    if (!status1)
    {
        ESP_LOGE(TAG, "Unable to open file");
        return;
    }
    // Fill the index
    m_s32IngredientIDIndexes = heap_caps_malloc(sizeof(int32_t) * m_psIngredientFile->ingredient_entries_count, MALLOC_CAP_SPIRAM);
    for(int i = 0; i < m_psIngredientFile->ingredient_entries_count; i++)
    {
        const cocktaildb_Ingredient* pIngredient = &m_psIngredientFile->ingredient_entries[i];
        m_s32IngredientIDIndexes[i] = pIngredient->id;
    }

    m_s32IngredientCategoryIDIndexes = heap_caps_malloc(sizeof(int32_t) * m_psIngredientFile->ingredient_category_entries_count, MALLOC_CAP_SPIRAM);
    for(int i = 0; i < m_psIngredientFile->ingredient_category_entries_count; i++)
    {
        const cocktaildb_IngredientCategory* pIngredientCategory = &m_psIngredientFile->ingredient_category_entries[i];
        m_s32IngredientCategoryIDIndexes[i] = pIngredientCategory->id;
    }

    ESP_LOGI(TAG, "Loaded ingredient, total: %d, category: %d", m_psIngredientFile->ingredient_entries_count, m_psIngredientFile->ingredient_category_entries_count);

    // ------------------------------
    // Recipes
    // ------------------------------
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

const cocktaildb_IngredientFile* COCKTAILEXPLORER_GetIngredientFile()
{
    return m_psIngredientFile;
}

const cocktaildb_RecipeFile* COCKTAILEXPLORER_GetRecipeFile()
{
    return m_psRecipeFile;
}

const cocktaildb_Ingredient* COCKTAILEXPLORER_GetAvailableIngredient(uint32_t ingredient_id, uint32_t* pu32StationId)
{
   for(int stationId = 1; stationId <= STATIONSETTINGS_STATION_COUNT; stationId++)
    {
        int32_t s32IngredientId = STATIONSETTINGS_GetValue(stationId, STATIONSETTINGS_ESTATIONSET_LoadID);
        if (s32IngredientId != ingredient_id)
            continue;

        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientById((uint32_t)s32IngredientId);
        if (pIngredient == NULL || !COCKTAILEXPLORER_IsIngredientLiquid(pIngredient))
            continue;

        *pu32StationId = stationId;
        return pIngredient;
    }

    return NULL;
}

bool COCKTAILEXPLORER_IsIngredientLiquid(const cocktaildb_Ingredient* pIngredient)
{
    return pIngredient->ingredient_type == cocktaildb_EIngredientType_liquid_alcohol || pIngredient->ingredient_type == cocktaildb_EIngredientType_liquid_filler;
}

uint32_t COCKTAILEXPLORER_GetAvailableIngredients(const cocktaildb_Ingredient* sAvailableIngredientIds[STATIONSETTINGS_STATION_COUNT])
{
    uint32_t u32Count = 0;

    for(int stationId = 1; stationId <= STATIONSETTINGS_STATION_COUNT; stationId++)
    {
        int32_t s32IngredientId = STATIONSETTINGS_GetValue(stationId, STATIONSETTINGS_ESTATIONSET_LoadID);
        if (s32IngredientId == 0)
            continue;
        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetIngredientById((uint32_t)s32IngredientId);
         if (pIngredient == NULL || !COCKTAILEXPLORER_IsIngredientLiquid(pIngredient))
            continue;
        sAvailableIngredientIds[u32Count] = pIngredient;
        u32Count++;
    }

    return u32Count;
}

const cocktaildb_Ingredient* COCKTAILEXPLORER_GetIngredientById(uint32_t u32ID)
{
    const int32_t s32Index = binarySearch(m_s32IngredientIDIndexes, m_psIngredientFile->ingredient_entries_count, u32ID);
    if (s32Index < 0)
        return NULL;
    return &m_psIngredientFile->ingredient_entries[s32Index];
}

const cocktaildb_Recipe* COCKTAILEXPLORER_GetRecipeById(uint32_t u32ID)
{
    for(int i = 0; i < m_psRecipeFile->entries_count; i++)
    {
        const cocktaildb_Recipe* pRecipe = &m_psRecipeFile->entries[i];
        if (pRecipe->id == u32ID)
            return pRecipe;
    }

    return NULL;
}

static int32_t binarySearch(const int32_t array[], int size, int32_t s32SearchValue)
{
  int32_t low = 0;
  int32_t high = size - 1;

  while (low <= high)
  {
    int32_t mid = (low + high) / 2;

    int32_t midValue = array[mid];
    if (s32SearchValue < midValue)
      high = mid - 1;
    else if (s32SearchValue > midValue)
      low = mid + 1;
    else
        return mid;
  }
  // dans l'array, on retourne -1
  return -1;
}