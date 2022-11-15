#ifndef _EMBEDDEDFILES_H_
#define _EMBEDDEDFILES_H_

#include <stdint.h>

typedef enum
{
   EF_EFLAGS_None = 0,
   EF_EFLAGS_GZip = 1,
} EF_EFLAGS;

typedef struct
{
   const char* strFilename;
   uint32_t u32Length;
   EF_EFLAGS eFlags;
   const uint8_t* pu8StartAddr;
} EF_SFile;

typedef enum
{
  EF_EFILE_INDEX_HTML = 0,    /*!< @brief File: index.html (size: 752 B) */
  EF_EFILE_CSS_BASE_MIN_CSS = 1,    /*!< @brief File: css/base-min.css (size: 2.2 KB) */
  EF_EFILE_CSS_CONTENT_CSS = 2,    /*!< @brief File: css/content.css (size: 0 B) */
  EF_EFILE_CSS_FORMS_MIN_CSS = 3,    /*!< @brief File: css/forms-min.css (size: 11.3 KB) */
  EF_EFILE_CSS_GRIDS_RESPONSIVE_MIN_CSS = 4,    /*!< @brief File: css/grids-responsive-min.css (size: 13.9 KB) */
  EF_EFILE_CSS_PURE_MIN_CSS = 5,    /*!< @brief File: css/pure-min.css (size: 15.4 KB) */
  EF_EFILE_JS_VIEWS_ABOUTPAGE_JS = 6,    /*!< @brief File: js/views/AboutPage.js (size: 263 B) */
  EF_EFILE_JS_VIEWS_ABSTRACTVIEW_JS = 7,    /*!< @brief File: js/views/AbstractView.js (size: 216 B) */
  EF_EFILE_JS_APP_JS = 8,    /*!< @brief File: js/app.js (size: 1.7 KB) */
  EF_EFILE_JS_VIEWS_INDEXPAGE_JS = 9,    /*!< @brief File: js/views/IndexPage.js (size: 1019 B) */
  EF_EFILE_JS_VIEWS_SETTINGSPAGE_JS = 10,    /*!< @brief File: js/views/SettingsPage.js (size: 977 B) */
  EF_EFILE_JS_VUE_MIN_JS = 11,    /*!< @brief File: js/vue.min.js (size: 92.0 KB) */
  EF_EFILE_COUNT = 12
} EF_EFILE;

/*! @brief Check if compressed flag is active */
#define EF_ISFILECOMPRESSED(x) ((x & EF_EFLAGS_GZip) == EF_EFLAGS_GZip)

extern const EF_SFile EF_g_sFiles[EF_EFILE_COUNT];
extern const uint8_t EF_g_u8Blobs[];

#endif
