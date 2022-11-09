#include "EmbeddedFiles.h"


/*! @brief Total size: 0, total (including trailing 0s): 1 */
const EF_SFile EF_g_sFiles[EF_EFILE_COUNT] = 
{
   [EF_EFILE_INDEX_HTML] = { "index.html", 0, EF_EFLAGS_None, &EF_g_u8Blobs[0] }/* size: 0 */
};

const uint8_t EF_g_u8Blobs[] = {
0x00
};
