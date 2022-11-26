#ifndef _COCKTAILEXPLORER_H_
#define _COCKTAILEXPLORER_H_

#include "nanopb/pb_decode.h"
#include "cocktaildb/cocktaildb.pb.h"

void COCKTAILEXPLORER_Init();

const cocktaildb_Ingredient* COCKTAILEXPLORER_GetIngredientFile(uint32_t u32ID);

char* COCKTAILEXPLORER_GetAllRecipes();

char* COCKTAILEXPLORER_GetAllIngredients(bool bIsLiquidOnly);

char* COCKTAILEXPLORER_GetAllAvailableIngredients();

#endif