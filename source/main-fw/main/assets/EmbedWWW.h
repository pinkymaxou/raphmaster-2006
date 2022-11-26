#ifndef _EMBEDWWW_H_
#define _EMBEDWWW_H_

#include <stdint.h>

typedef enum
{
   EFEMBEDWWW_EFLAGS_None = 0,
   EFEMBEDWWW_EFLAGS_GZip = 1,
} EFEMBEDWWW_EFLAGS;

typedef struct
{
   const char* strFilename;
   uint32_t u32Length;
   EFEMBEDWWW_EFLAGS eFlags;
   const uint8_t* pu8StartAddr;
} EFEMBEDWWW_SFile;

typedef enum
{
  EFEMBEDWWW_EFILE_INDEX_HTML = 0,    /*!< @brief File: index.html (size: 1.4 KB) */
  EFEMBEDWWW_EFILE_CSS_BUTTON_CSS = 1,    /*!< @brief File: css/button.css (size: 2.0 KB) */
  EFEMBEDWWW_EFILE_CSS_CONTENT_CSS = 2,    /*!< @brief File: css/content.css (size: 4.8 KB) */
  EFEMBEDWWW_EFILE_CSS_GENERAL_CSS = 3,    /*!< @brief File: css/general.css (size: 1.6 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_ABOUTPAGE_JS = 4,    /*!< @brief File: js/views/AboutPage.js (size: 2.3 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_ABSTRACTVIEW_JS = 5,    /*!< @brief File: js/views/AbstractView.js (size: 249 B) */
  EFEMBEDWWW_EFILE_JS_APP_JS = 6,    /*!< @brief File: js/app.js (size: 3.1 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_CALIBPAGE_JS = 7,    /*!< @brief File: js/views/CalibPage.js (size: 5.6 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_CUSTOMCOCKTAILPAGE_JS = 8,    /*!< @brief File: js/views/CustomCocktailPage.js (size: 5.4 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_LISTCOCKTAILPAGE_JS = 9,    /*!< @brief File: js/views/ListCocktailPage.js (size: 5.8 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_NETWORKPAGE_JS = 10,    /*!< @brief File: js/views/NetworkPage.js (size: 1.7 KB) */
  EFEMBEDWWW_EFILE_JS_ROUTER_JS = 11,    /*!< @brief File: js/router.js (size: 2.5 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_SETTINGSPAGE_JS = 12,    /*!< @brief File: js/views/SettingsPage.js (size: 306 B) */
  EFEMBEDWWW_EFILE_JS_VIEWS_STATIONSETTINGSPAGE_JS = 13,    /*!< @brief File: js/views/StationSettingsPage.js (size: 6.3 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_STATUSPAGE_JS = 14,    /*!< @brief File: js/views/StatusPage.js (size: 2.5 KB) */
  EFEMBEDWWW_EFILE_FAVICON_ICO = 15,    /*!< @brief File: favicon.ico (size: 15.0 KB) */
  EFEMBEDWWW_EFILE_IMG_AMARETTO_SOUR_JPG = 16,    /*!< @brief File: img/amaretto-sour.jpg (size: 3.5 KB) */
  EFEMBEDWWW_EFILE_IMG_APEROL_SPRITZ_JPG = 17,    /*!< @brief File: img/aperol_spritz.jpg (size: 5.1 KB) */
  EFEMBEDWWW_EFILE_IMG_BLOODY_MARY_JPG = 18,    /*!< @brief File: img/bloody-mary.jpg (size: 4.9 KB) */
  EFEMBEDWWW_EFILE_IMG_DAIQUIRI_JPG = 19,    /*!< @brief File: img/daiquiri.jpg (size: 4.0 KB) */
  EFEMBEDWWW_EFILE_IMG_GRASSHOPPER_JPG = 20,    /*!< @brief File: img/grasshopper.jpg (size: 3.1 KB) */
  EFEMBEDWWW_EFILE_IMG_MARGARITA_JPG = 21,    /*!< @brief File: img/margarita.jpg (size: 2.9 KB) */
  EFEMBEDWWW_EFILE_IMG_MARTINI_JPG = 22,    /*!< @brief File: img/martini.jpg (size: 3.1 KB) */
  EFEMBEDWWW_EFILE_FONT_ORBITRON_BOLD_WOFF = 23,    /*!< @brief File: font/Orbitron-Bold.woff (size: 13.3 KB) */
  EFEMBEDWWW_EFILE_FONT_ORBITRON_REGULAR_WOFF = 24,    /*!< @brief File: font/Orbitron-Regular.woff (size: 12.9 KB) */
  EFEMBEDWWW_EFILE_IMG_PORNSTAR_JPG = 25,    /*!< @brief File: img/pornstar.jpg (size: 4.8 KB) */
  EFEMBEDWWW_EFILE_FONT_ROBOTOMONO_BOLD_WOFF = 26,    /*!< @brief File: font/RobotoMono-Bold.woff (size: 52.4 KB) */
  EFEMBEDWWW_EFILE_FONT_ROBOTOMONO_REGULAR_WOFF = 27,    /*!< @brief File: font/RobotoMono-Regular.woff (size: 52.1 KB) */
  EFEMBEDWWW_EFILE_IMG_SEX_ON_THE_BEACH_JPG = 28,    /*!< @brief File: img/sex-on-the-beach.jpg (size: 5.7 KB) */
  EFEMBEDWWW_EFILE_IMG_SKITTLE_JPG = 29,    /*!< @brief File: img/skittle.jpg (size: 973 B) */
  EFEMBEDWWW_EFILE_IMG_TOM_COLLINS_JPG = 30,    /*!< @brief File: img/tom-collins.jpg (size: 3.0 KB) */
  EFEMBEDWWW_EFILE_COUNT = 31
} EFEMBEDWWW_EFILE;

/*! @brief Check if compressed flag is active */
#define EFEMBEDWWW_ISFILECOMPRESSED(x) ((x & EFEMBEDWWW_EFLAGS_GZip) == EFEMBEDWWW_EFLAGS_GZip)

extern const EFEMBEDWWW_SFile EFEMBEDWWW_g_sFiles[EFEMBEDWWW_EFILE_COUNT];
extern const uint8_t EFEMBEDWWW_g_u8Blobs[];

#endif
