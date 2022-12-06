#ifndef _COCKTAIL_H_
#define _COCKTAIL_H_

#include <stdint.h>
#include <stdbool.h>

char* COCKTAILAPI_GetAllRecipes(uint32_t recipe_id);

char* COCKTAILAPI_GetAllIngredients(bool bIsLiquidOnly);

char* COCKTAILAPI_GetAllAvailableIngredients();

char* COCKTAILAPI_GetStatIngredients();

#endif