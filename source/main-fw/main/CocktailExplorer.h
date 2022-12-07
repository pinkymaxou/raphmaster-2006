#ifndef _COCKTAILEXPLORER_H_
#define _COCKTAILEXPLORER_H_

#include "nanopb/pb_decode.h"
#include "cocktaildb/cocktaildb.pb.h"
#include "StationSettings.h"

void COCKTAILEXPLORER_Init();

// void COCKTAILEXPLORER_GetLock();

// void COCKTAILEXPLORER_ReleaseLock();

const cocktaildb_Ingredient* COCKTAILEXPLORER_GetIngredientById(uint32_t u32ID);

const cocktaildb_Recipe* COCKTAILEXPLORER_GetRecipeById(uint32_t u32ID);

const cocktaildb_Ingredient* COCKTAILEXPLORER_GetAvailableIngredient(uint32_t ingredient_id, uint32_t* pu32StationId);

bool COCKTAILEXPLORER_IsIngredientLiquid(const cocktaildb_Ingredient* pIngredient);

uint32_t COCKTAILEXPLORER_GetAvailableIngredients(const cocktaildb_Ingredient* sAvailableIngredientIds[STATIONSETTINGS_STATION_COUNT]);

const cocktaildb_IngredientFile* COCKTAILEXPLORER_GetIngredientFile();

const cocktaildb_RecipeFile* COCKTAILEXPLORER_GetRecipeFile();

#endif