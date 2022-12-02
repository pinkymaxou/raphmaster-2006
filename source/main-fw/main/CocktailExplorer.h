#ifndef _COCKTAILEXPLORER_H_
#define _COCKTAILEXPLORER_H_

#include "nanopb/pb_decode.h"
#include "cocktaildb/cocktaildb.pb.h"

void COCKTAILEXPLORER_Init();

const cocktaildb_Ingredient* COCKTAILEXPLORER_GetIngredientFile(uint32_t u32ID);

const cocktaildb_Recipe* COCKTAILEXPLORER_GetRecipe(uint32_t u32ID);

char* COCKTAILEXPLORER_GetAllRecipes(uint32_t recipe_id);

char* COCKTAILEXPLORER_GetAllIngredients(bool bIsLiquidOnly);

char* COCKTAILEXPLORER_GetAllAvailableIngredients();

char* COCKTAILEXPLORER_GetStatIngredients();

char* COCKTAILEXPLORER_GetStationSettings();

bool COCKTAILEXPLORER_SetStationSettings(const char* szRequestBuffer, uint32_t u32Length);

#endif