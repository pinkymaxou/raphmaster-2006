#ifndef _EMBEDCOCKTAILDB_H_
#define _EMBEDCOCKTAILDB_H_

#include <stdint.h>

typedef enum
{
   EFEMBEDCOCKTAILDB_EFLAGS_None = 0,
   EFEMBEDCOCKTAILDB_EFLAGS_GZip = 1,
} EFEMBEDCOCKTAILDB_EFLAGS;

typedef struct
{
   const char* strFilename;
   uint32_t u32Length;
   EFEMBEDCOCKTAILDB_EFLAGS eFlags;
   const uint8_t* pu8StartAddr;
} EFEMBEDCOCKTAILDB_SFile;

typedef enum
{
  EFEMBEDCOCKTAILDB_EFILE_INGREDIENTS_BIN = 0,    /*!< @brief File: ingredients.bin (size: 3.8 KB) */
  EFEMBEDCOCKTAILDB_EFILE_RECIPES_BIN = 1,    /*!< @brief File: recipes.bin (size: 4.8 KB) */
  EFEMBEDCOCKTAILDB_EFILE_COUNT = 2
} EFEMBEDCOCKTAILDB_EFILE;

/*! @brief Check if compressed flag is active */
#define EFEMBEDCOCKTAILDB_ISFILECOMPRESSED(x) ((x & EFEMBEDCOCKTAILDB_EFLAGS_GZip) == EFEMBEDCOCKTAILDB_EFLAGS_GZip)

extern const EFEMBEDCOCKTAILDB_SFile EFEMBEDCOCKTAILDB_g_sFiles[EFEMBEDCOCKTAILDB_EFILE_COUNT];
extern const uint8_t EFEMBEDCOCKTAILDB_g_u8Blobs[];

#endif
