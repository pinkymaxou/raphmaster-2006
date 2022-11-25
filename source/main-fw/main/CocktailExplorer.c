#include "esp_log.h"
#include "nanopb/pb_decode.h"

#include "CocktailExplorer.h" 
#include "assets/EmbedCocktailDb.h"
#include "cocktaildb/cocktaildb.pb.h"

#define TAG "CocktailExplorer"

static cocktaildb_IngredientFile m_sIngredientFile;
static cocktaildb_RecipeFile m_sRecipeFile;

void COCKTAILEXPLORER_Init()
{
    ESP_LOGI(TAG, "Loading ingredient");

    const EFEMBEDCOCKTAILDB_SFile* psIngredientFile = &EFEMBEDCOCKTAILDB_g_sFiles[EFEMBEDCOCKTAILDB_EFILE_INGREDIENTS_BIN];

    pb_istream_t ingredientStream = pb_istream_from_buffer(psIngredientFile->pu8StartAddr, psIngredientFile->u32Length);
    bool status1 = pb_decode(&ingredientStream, cocktaildb_IngredientFile_fields, &m_sIngredientFile);
    if (!status1)
    {
        ESP_LOGE(TAG, "Unable to open file");
        return;
    }
    
    ESP_LOGI(TAG, "Loaded ingredient");

    ESP_LOGI(TAG, "Loading recipes");

    const EFEMBEDCOCKTAILDB_SFile* psRecipeFile = &EFEMBEDCOCKTAILDB_g_sFiles[EFEMBEDCOCKTAILDB_EFILE_RECIPES_BIN];

    pb_istream_t recipeStream = pb_istream_from_buffer(psRecipeFile->pu8StartAddr, psRecipeFile->u32Length);
    bool status2 = pb_decode(&recipeStream, cocktaildb_RecipeFile_fields, &m_sRecipeFile);
    if (!status2)
    {
        ESP_LOGE(TAG, "Unable to open file");
        return;
    }
    
    ESP_LOGI(TAG, "Loaded recipes");
}
