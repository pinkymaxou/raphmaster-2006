#ifndef _COCKTAILEXPLORER_H_
#define _COCKTAILEXPLORER_H_

#include "nanopb/pb_decode.h"
#include "cocktaildb/cocktaildb.pb.h"

void COCKTAILEXPLORER_Init();

// void COCKTAILEXPLORER_GetLock();

// void COCKTAILEXPLORER_ReleaseLock();

const cocktaildb_Ingredient* COCKTAILEXPLORER_GetIngredientFile(uint32_t u32ID);

const cocktaildb_Recipe* COCKTAILEXPLORER_GetRecipe(uint32_t u32ID);

char* COCKTAILEXPLORER_GetAllRecipes(uint32_t recipe_id);

char* COCKTAILEXPLORER_GetAllIngredients(bool bIsLiquidOnly);

char* COCKTAILEXPLORER_GetAllAvailableIngredients();

const cocktaildb_Ingredient* COCKTAILEXPLORER_GetAvailableIngredient(uint32_t ingredient_id, uint32_t* pu32StationId);

char* COCKTAILEXPLORER_GetStatIngredients();

char* COCKTAILEXPLORER_GetStationSettings();

bool COCKTAILEXPLORER_SetStationSettings(const char* szRequestBuffer, uint32_t u32Length);

#endif