/* Automatically generated nanopb header */
/* Generated by nanopb-0.4.7-dev */

#ifndef PB_COCKTAILDB_COCKTAILDB_PB_H_INCLUDED
#define PB_COCKTAILDB_COCKTAILDB_PB_H_INCLUDED
#include <pb.h>

#if PB_PROTO_HEADER_VERSION != 40
#error Regenerate this file with the current version of nanopb generator.
#endif

/* Enum definitions */
typedef enum _cocktaildb_EIngredientType {
    cocktaildb_EIngredientType_unspecified = 0,
    cocktaildb_EIngredientType_liquid_alcohol = 1,
    cocktaildb_EIngredientType_garnish = 2,
    cocktaildb_EIngredientType_liquid_filler = 3,
    cocktaildb_EIngredientType_ice = 4,
    cocktaildb_EIngredientType_powder = 5,
    cocktaildb_EIngredientType_emulsifying = 6
} cocktaildb_EIngredientType;

typedef enum _cocktaildb_ELanguage {
    cocktaildb_ELanguage_english = 0,
    cocktaildb_ELanguage_french = 1
} cocktaildb_ELanguage;

typedef enum _cocktaildb_EQtyType {
    cocktaildb_EQtyType_none = 0, /* When unit doesn't apply */
    cocktaildb_EQtyType_unitary = 1, /* Counter by round number */
    cocktaildb_EQtyType_liquidML = 2, /* Milliliter */
    cocktaildb_EQtyType_oz = 3, /* Ounce */
    cocktaildb_EQtyType_cup = 4, /* Cup */
    cocktaildb_EQtyType_pinch = 6, /* Fingertip quantity */
    /* wheel = 7;		// Basically a slice of a fruit */
    cocktaildb_EQtyType_teaspoon = 8, /* 4.5 ml (1/6 oz) */
    cocktaildb_EQtyType_tablespoon = 9, /* 14 ml (1/2 oz) */
    cocktaildb_EQtyType_dash = 10, /* 1 ml */
    cocktaildb_EQtyType_drop = 11 /* One drop */
} cocktaildb_EQtyType;

/* Struct definitions */
typedef struct _cocktaildb_ProductCode {
    char upc_code[18];
} cocktaildb_ProductCode;

typedef struct _cocktaildb_Ingredient {
    uint32_t id; /* do not accept 0 as ID */
    char name[48];
    pb_size_t product_codes_count;
    cocktaildb_ProductCode product_codes[4];
    uint32_t recipe_id; /* 0 = null */
    cocktaildb_EIngredientType ingredient_type;
} cocktaildb_Ingredient;

typedef struct _cocktaildb_Qty {
    float value;
    cocktaildb_EQtyType type;
} cocktaildb_Qty;

typedef struct _cocktaildb_RecipeStep {
    uint32_t ingredient_id; /* do not accept 0 as ID */
    bool has_qty;
    cocktaildb_Qty qty;
} cocktaildb_RecipeStep;

typedef struct _cocktaildb_Recipe {
    uint32_t ID;
    char name[50];
    bool is_cocktail;
    pb_size_t recipe_steps_count;
    cocktaildb_RecipeStep recipe_steps[15];
    char imgfile[25];
} cocktaildb_Recipe;

typedef struct _cocktaildb_IngredientFile {
    pb_size_t entries_count;
    cocktaildb_Ingredient entries[250];
} cocktaildb_IngredientFile;

typedef struct _cocktaildb_RecipeFile {
    pb_size_t entries_count;
    cocktaildb_Recipe entries[100];
} cocktaildb_RecipeFile;

typedef struct _cocktaildb_Equivalence {
    pb_size_t ingredient_ids_count;
    uint32_t ingredient_ids[20];
    float equivalence; /* 0 = not so much, 1 = perfectly switchable */
} cocktaildb_Equivalence;

typedef struct _cocktaildb_MetaDataFile {
    pb_size_t equivalences_count;
    cocktaildb_Equivalence equivalences[50];
} cocktaildb_MetaDataFile;


/* Helper constants for enums */
#define _cocktaildb_EIngredientType_MIN cocktaildb_EIngredientType_unspecified
#define _cocktaildb_EIngredientType_MAX cocktaildb_EIngredientType_emulsifying
#define _cocktaildb_EIngredientType_ARRAYSIZE ((cocktaildb_EIngredientType)(cocktaildb_EIngredientType_emulsifying+1))

#define _cocktaildb_ELanguage_MIN cocktaildb_ELanguage_english
#define _cocktaildb_ELanguage_MAX cocktaildb_ELanguage_french
#define _cocktaildb_ELanguage_ARRAYSIZE ((cocktaildb_ELanguage)(cocktaildb_ELanguage_french+1))

#define _cocktaildb_EQtyType_MIN cocktaildb_EQtyType_none
#define _cocktaildb_EQtyType_MAX cocktaildb_EQtyType_drop
#define _cocktaildb_EQtyType_ARRAYSIZE ((cocktaildb_EQtyType)(cocktaildb_EQtyType_drop+1))


#define cocktaildb_Ingredient_ingredient_type_ENUMTYPE cocktaildb_EIngredientType


#define cocktaildb_Qty_type_ENUMTYPE cocktaildb_EQtyType







#ifdef __cplusplus
extern "C" {
#endif

/* Initializer values for message structs */
#define cocktaildb_ProductCode_init_default      {""}
#define cocktaildb_Ingredient_init_default       {0, "", 0, {cocktaildb_ProductCode_init_default, cocktaildb_ProductCode_init_default, cocktaildb_ProductCode_init_default, cocktaildb_ProductCode_init_default}, 0, _cocktaildb_EIngredientType_MIN}
#define cocktaildb_Recipe_init_default           {0, "", 0, 0, {cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default, cocktaildb_RecipeStep_init_default}, ""}
#define cocktaildb_Qty_init_default              {0, _cocktaildb_EQtyType_MIN}
#define cocktaildb_RecipeStep_init_default       {0, false, cocktaildb_Qty_init_default}
#define cocktaildb_IngredientFile_init_default   {0, {cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default, cocktaildb_Ingredient_init_default}}
#define cocktaildb_RecipeFile_init_default       {0, {cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default, cocktaildb_Recipe_init_default}}
#define cocktaildb_Equivalence_init_default      {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0}
#define cocktaildb_MetaDataFile_init_default     {0, {cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default, cocktaildb_Equivalence_init_default}}
#define cocktaildb_ProductCode_init_zero         {""}
#define cocktaildb_Ingredient_init_zero          {0, "", 0, {cocktaildb_ProductCode_init_zero, cocktaildb_ProductCode_init_zero, cocktaildb_ProductCode_init_zero, cocktaildb_ProductCode_init_zero}, 0, _cocktaildb_EIngredientType_MIN}
#define cocktaildb_Recipe_init_zero              {0, "", 0, 0, {cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero, cocktaildb_RecipeStep_init_zero}, ""}
#define cocktaildb_Qty_init_zero                 {0, _cocktaildb_EQtyType_MIN}
#define cocktaildb_RecipeStep_init_zero          {0, false, cocktaildb_Qty_init_zero}
#define cocktaildb_IngredientFile_init_zero      {0, {cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero, cocktaildb_Ingredient_init_zero}}
#define cocktaildb_RecipeFile_init_zero          {0, {cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero, cocktaildb_Recipe_init_zero}}
#define cocktaildb_Equivalence_init_zero         {0, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, 0}
#define cocktaildb_MetaDataFile_init_zero        {0, {cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero, cocktaildb_Equivalence_init_zero}}

/* Field tags (for use in manual encoding/decoding) */
#define cocktaildb_ProductCode_upc_code_tag      1
#define cocktaildb_Ingredient_id_tag             1
#define cocktaildb_Ingredient_name_tag           2
#define cocktaildb_Ingredient_product_codes_tag  3
#define cocktaildb_Ingredient_recipe_id_tag      4
#define cocktaildb_Ingredient_ingredient_type_tag 5
#define cocktaildb_Qty_value_tag                 1
#define cocktaildb_Qty_type_tag                  2
#define cocktaildb_RecipeStep_ingredient_id_tag  2
#define cocktaildb_RecipeStep_qty_tag            3
#define cocktaildb_Recipe_ID_tag                 1
#define cocktaildb_Recipe_name_tag               2
#define cocktaildb_Recipe_is_cocktail_tag        5
#define cocktaildb_Recipe_recipe_steps_tag       6
#define cocktaildb_Recipe_imgfile_tag            7
#define cocktaildb_IngredientFile_entries_tag    1
#define cocktaildb_RecipeFile_entries_tag        1
#define cocktaildb_Equivalence_ingredient_ids_tag 1
#define cocktaildb_Equivalence_equivalence_tag   2
#define cocktaildb_MetaDataFile_equivalences_tag 1

/* Struct field encoding specification for nanopb */
#define cocktaildb_ProductCode_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, STRING,   upc_code,          1)
#define cocktaildb_ProductCode_CALLBACK NULL
#define cocktaildb_ProductCode_DEFAULT NULL

#define cocktaildb_Ingredient_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   id,                1) \
X(a, STATIC,   SINGULAR, STRING,   name,              2) \
X(a, STATIC,   REPEATED, MESSAGE,  product_codes,     3) \
X(a, STATIC,   SINGULAR, UINT32,   recipe_id,         4) \
X(a, STATIC,   SINGULAR, UENUM,    ingredient_type,   5)
#define cocktaildb_Ingredient_CALLBACK NULL
#define cocktaildb_Ingredient_DEFAULT NULL
#define cocktaildb_Ingredient_product_codes_MSGTYPE cocktaildb_ProductCode

#define cocktaildb_Recipe_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   ID,                1) \
X(a, STATIC,   SINGULAR, STRING,   name,              2) \
X(a, STATIC,   SINGULAR, BOOL,     is_cocktail,       5) \
X(a, STATIC,   REPEATED, MESSAGE,  recipe_steps,      6) \
X(a, STATIC,   SINGULAR, STRING,   imgfile,           7)
#define cocktaildb_Recipe_CALLBACK NULL
#define cocktaildb_Recipe_DEFAULT NULL
#define cocktaildb_Recipe_recipe_steps_MSGTYPE cocktaildb_RecipeStep

#define cocktaildb_Qty_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, FLOAT,    value,             1) \
X(a, STATIC,   SINGULAR, UENUM,    type,              2)
#define cocktaildb_Qty_CALLBACK NULL
#define cocktaildb_Qty_DEFAULT NULL

#define cocktaildb_RecipeStep_FIELDLIST(X, a) \
X(a, STATIC,   SINGULAR, UINT32,   ingredient_id,     2) \
X(a, STATIC,   OPTIONAL, MESSAGE,  qty,               3)
#define cocktaildb_RecipeStep_CALLBACK NULL
#define cocktaildb_RecipeStep_DEFAULT NULL
#define cocktaildb_RecipeStep_qty_MSGTYPE cocktaildb_Qty

#define cocktaildb_IngredientFile_FIELDLIST(X, a) \
X(a, STATIC,   REPEATED, MESSAGE,  entries,           1)
#define cocktaildb_IngredientFile_CALLBACK NULL
#define cocktaildb_IngredientFile_DEFAULT NULL
#define cocktaildb_IngredientFile_entries_MSGTYPE cocktaildb_Ingredient

#define cocktaildb_RecipeFile_FIELDLIST(X, a) \
X(a, STATIC,   REPEATED, MESSAGE,  entries,           1)
#define cocktaildb_RecipeFile_CALLBACK NULL
#define cocktaildb_RecipeFile_DEFAULT NULL
#define cocktaildb_RecipeFile_entries_MSGTYPE cocktaildb_Recipe

#define cocktaildb_Equivalence_FIELDLIST(X, a) \
X(a, STATIC,   REPEATED, UINT32,   ingredient_ids,    1) \
X(a, STATIC,   SINGULAR, FLOAT,    equivalence,       2)
#define cocktaildb_Equivalence_CALLBACK NULL
#define cocktaildb_Equivalence_DEFAULT NULL

#define cocktaildb_MetaDataFile_FIELDLIST(X, a) \
X(a, STATIC,   REPEATED, MESSAGE,  equivalences,      1)
#define cocktaildb_MetaDataFile_CALLBACK NULL
#define cocktaildb_MetaDataFile_DEFAULT NULL
#define cocktaildb_MetaDataFile_equivalences_MSGTYPE cocktaildb_Equivalence

extern const pb_msgdesc_t cocktaildb_ProductCode_msg;
extern const pb_msgdesc_t cocktaildb_Ingredient_msg;
extern const pb_msgdesc_t cocktaildb_Recipe_msg;
extern const pb_msgdesc_t cocktaildb_Qty_msg;
extern const pb_msgdesc_t cocktaildb_RecipeStep_msg;
extern const pb_msgdesc_t cocktaildb_IngredientFile_msg;
extern const pb_msgdesc_t cocktaildb_RecipeFile_msg;
extern const pb_msgdesc_t cocktaildb_Equivalence_msg;
extern const pb_msgdesc_t cocktaildb_MetaDataFile_msg;

/* Defines for backwards compatibility with code written before nanopb-0.4.0 */
#define cocktaildb_ProductCode_fields &cocktaildb_ProductCode_msg
#define cocktaildb_Ingredient_fields &cocktaildb_Ingredient_msg
#define cocktaildb_Recipe_fields &cocktaildb_Recipe_msg
#define cocktaildb_Qty_fields &cocktaildb_Qty_msg
#define cocktaildb_RecipeStep_fields &cocktaildb_RecipeStep_msg
#define cocktaildb_IngredientFile_fields &cocktaildb_IngredientFile_msg
#define cocktaildb_RecipeFile_fields &cocktaildb_RecipeFile_msg
#define cocktaildb_Equivalence_fields &cocktaildb_Equivalence_msg
#define cocktaildb_MetaDataFile_fields &cocktaildb_MetaDataFile_msg

/* Maximum encoded size of messages (where known) */
#define cocktaildb_Equivalence_size              125
#define cocktaildb_IngredientFile_size           37500
#define cocktaildb_Ingredient_size               147
#define cocktaildb_MetaDataFile_size             6350
#define cocktaildb_ProductCode_size              19
#define cocktaildb_Qty_size                      7
#define cocktaildb_RecipeFile_size               34300
#define cocktaildb_RecipeStep_size               15
#define cocktaildb_Recipe_size                   340

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
