#ifndef _COCKTAILEXPLORER_H_
#define _COCKTAILEXPLORER_H_

#include "nanopb/pb_decode.h"
#include "cocktaildb/cocktaildb.pb.h"

void COCKTAILEXPLORER_Init();

const cocktaildb_IngredientFile* COCKTAILEXPLORER_GetIngredientFiles();

const cocktaildb_RecipeFile* COCKTAILEXPLORER_GetRecipeFiles();

char* COCKTAILEXPLORER_GetAllRecipes();

#endif