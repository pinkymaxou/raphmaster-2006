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
  EF_EFILE_INDEX_HTML = 0,    /*!< @brief File: index.html (size: 1.3 KB) */
  EF_EFILE_CSS_CONTENT_CSS = 1,    /*!< @brief File: css/content.css (size: 5.1 KB) */
  EF_EFILE_CSS_GENERAL_CSS = 2,    /*!< @brief File: css/general.css (size: 1.4 KB) */
  EF_EFILE_JS_VIEWS_ABOUTPAGE_JS = 3,    /*!< @brief File: js/views/AboutPage.js (size: 2.5 KB) */
  EF_EFILE_JS_VIEWS_ABSTRACTVIEW_JS = 4,    /*!< @brief File: js/views/AbstractView.js (size: 249 B) */
  EF_EFILE_JS_APP_JS = 5,    /*!< @brief File: js/app.js (size: 0 B) */
  EF_EFILE_JS_VIEWS_CALIBPAGE_JS = 6,    /*!< @brief File: js/views/CalibPage.js (size: 5.8 KB) */
  EF_EFILE_JS_VIEWS_CUSTOMCOCKTAILPAGE_JS = 7,    /*!< @brief File: js/views/CustomCocktailPage.js (size: 3.6 KB) */
  EF_EFILE_JS_VIEWS_MAKECOCKTAILPAGE_JS = 8,    /*!< @brief File: js/views/MakeCocktailPage.js (size: 5.7 KB) */
  EF_EFILE_JS_VIEWS_NETWORKPAGE_JS = 9,    /*!< @brief File: js/views/NetworkPage.js (size: 1.6 KB) */
  EF_EFILE_JS_ROUTER_JS = 10,    /*!< @brief File: js/router.js (size: 2.5 KB) */
  EF_EFILE_JS_VIEWS_SETTINGSPAGE_JS = 11,    /*!< @brief File: js/views/SettingsPage.js (size: 306 B) */
  EF_EFILE_JS_VIEWS_STATIONSETTINGSPAGE_JS = 12,    /*!< @brief File: js/views/StationSettingsPage.js (size: 4.5 KB) */
  EF_EFILE_JS_VIEWS_STATUSPAGE_JS = 13,    /*!< @brief File: js/views/StatusPage.js (size: 2.5 KB) */
  EF_EFILE_FAVICON_ICO = 14,    /*!< @brief File: favicon.ico (size: 15.0 KB) */
  EF_EFILE_IMG_BLOODY_MARY_JPG = 15,    /*!< @brief File: img/bloody-mary.jpg (size: 4.9 KB) */
  EF_EFILE_IMG_GRASSHOPPER_JPG = 16,    /*!< @brief File: img/grasshopper.jpg (size: 3.1 KB) */
  EF_EFILE_FONT_ORBITRON_BOLD_WOFF = 17,    /*!< @brief File: font/Orbitron-Bold.woff (size: 13.3 KB) */
  EF_EFILE_FONT_ORBITRON_REGULAR_WOFF = 18,    /*!< @brief File: font/Orbitron-Regular.woff (size: 12.9 KB) */
  EF_EFILE_IMG_PORNSTAR_JPG = 19,    /*!< @brief File: img/pornstar.jpg (size: 4.8 KB) */
  EF_EFILE_FONT_ROBOTOMONO_BOLD_WOFF = 20,    /*!< @brief File: font/RobotoMono-Bold.woff (size: 52.4 KB) */
  EF_EFILE_FONT_ROBOTOMONO_REGULAR_WOFF = 21,    /*!< @brief File: font/RobotoMono-Regular.woff (size: 52.1 KB) */
  EF_EFILE_IMG_SEX_ON_THE_BEACH_JPG = 22,    /*!< @brief File: img/sex-on-the-beach.jpg (size: 5.7 KB) */
  EF_EFILE_COUNT = 23
} EF_EFILE;

/*! @brief Check if compressed flag is active */
#define EF_ISFILECOMPRESSED(x) ((x & EF_EFLAGS_GZip) == EF_EFLAGS_GZip)

extern const EF_SFile EF_g_sFiles[EF_EFILE_COUNT];
extern const uint8_t EF_g_u8Blobs[];

#endif
