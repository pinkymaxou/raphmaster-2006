#ifndef _COCKTAILEXPLORER_H_
#define _COCKTAILEXPLORER_H_

#include "nanopb/pb_decode.h"
#include "cocktaildb/cocktaildb.pb.h"

void COCKTAILEXPLORER_Init();

char* COCKTAILEXPLORER_GetAllRecipes();

char* COCKTAILEXPLORER_GetAllIngredients(bool bIsLiquidOnly);

#endif