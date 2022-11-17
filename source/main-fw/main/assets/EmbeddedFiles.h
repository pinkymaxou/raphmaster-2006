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
  EF_EFILE_INDEX_HTML = 0,    /*!< @brief File: index.html (size: 1.9 KB) */
  EF_EFILE_CSS_CONTENT_CSS = 1,    /*!< @brief File: css/content.css (size: 60 B) */
  EF_EFILE_CSS_GRIDS_RESPONSIVE_MIN_CSS = 2,    /*!< @brief File: css/grids-responsive-min.css (size: 14.0 KB) */
  EF_EFILE_CSS_PURE_MIN_CSS = 3,    /*!< @brief File: css/pure-min.css (size: 15.4 KB) */
  EF_EFILE_JS_VIEWS_ABOUTPAGE_JS = 4,    /*!< @brief File: js/views/AboutPage.js (size: 1.6 KB) */
  EF_EFILE_JS_VIEWS_ABSTRACTVIEW_JS = 5,    /*!< @brief File: js/views/AbstractView.js (size: 249 B) */
  EF_EFILE_JS_APP_JS = 6,    /*!< @brief File: js/app.js (size: 145 B) */
  EF_EFILE_JS_VIEWS_CALIBPAGE_JS = 7,    /*!< @brief File: js/views/CalibPage.js (size: 5.7 KB) */
  EF_EFILE_JS_VIEWS_INDEXPAGE_JS = 8,    /*!< @brief File: js/views/IndexPage.js (size: 1.0 KB) */
  EF_EFILE_JS_VIEWS_NETWORKPAGE_JS = 9,    /*!< @brief File: js/views/NetworkPage.js (size: 1.6 KB) */
  EF_EFILE_JS_ROUTER_JS = 10,    /*!< @brief File: js/router.js (size: 2.2 KB) */
  EF_EFILE_JS_VIEWS_SETTINGSPAGE_JS = 11,    /*!< @brief File: js/views/SettingsPage.js (size: 306 B) */
  EF_EFILE_JS_VIEWS_SLOTSETTINGSPAGE_JS = 12,    /*!< @brief File: js/views/SlotSettingsPage.js (size: 4.3 KB) */
  EF_EFILE_COUNT = 13
} EF_EFILE;

/*! @brief Check if compressed flag is active */
#define EF_ISFILECOMPRESSED(x) ((x & EF_EFLAGS_GZip) == EF_EFLAGS_GZip)

extern const EF_SFile EF_g_sFiles[EF_EFILE_COUNT];
extern const uint8_t EF_g_u8Blobs[];

#endif
