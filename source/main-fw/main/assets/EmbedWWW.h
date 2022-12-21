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
  EFEMBEDWWW_EFILE_INDEX_HTML = 0,    /*!< @brief File: index.html (size: 1.7 KB) */
  EFEMBEDWWW_EFILE_CSS_CONTENT_CSS = 1,    /*!< @brief File: css/content.css (size: 5.7 KB) */
  EFEMBEDWWW_EFILE_CSS_GENERAL_CSS = 2,    /*!< @brief File: css/general.css (size: 3.6 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_ABOUTPAGE_JS = 3,    /*!< @brief File: js/views/AboutPage.js (size: 1.9 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_ABSTRACTVIEW_JS = 4,    /*!< @brief File: js/views/AbstractView.js (size: 284 B) */
  EFEMBEDWWW_EFILE_JS_APICONST_JS = 5,    /*!< @brief File: js/apiconst.js (size: 4.8 KB) */
  EFEMBEDWWW_EFILE_JS_APP_JS = 6,    /*!< @brief File: js/app.js (size: 876 B) */
  EFEMBEDWWW_EFILE_JS_VIEWS_CALIBPAGE_JS = 7,    /*!< @brief File: js/views/CalibPage.js (size: 5.5 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_CUSTOMCOCKTAILPAGE_JS = 8,    /*!< @brief File: js/views/CustomCocktailPage.js (size: 8.0 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_LISTCOCKTAILPAGE_JS = 9,    /*!< @brief File: js/views/ListCocktailPage.js (size: 6.5 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_NETWORKPAGE_JS = 10,    /*!< @brief File: js/views/NetworkPage.js (size: 5.0 KB) */
  EFEMBEDWWW_EFILE_JS_ROUTER_JS = 11,    /*!< @brief File: js/router.js (size: 2.7 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_STATIONSETTINGSPAGE_JS = 12,    /*!< @brief File: js/views/StationSettingsPage.js (size: 6.4 KB) */
  EFEMBEDWWW_EFILE_JS_VIEWS_STATUSPAGE_JS = 13,    /*!< @brief File: js/views/StatusPage.js (size: 3.0 KB) */
  EFEMBEDWWW_EFILE_FAVICON_ICO = 14,    /*!< @brief File: favicon.ico (size: 15.0 KB) */
  EFEMBEDWWW_EFILE_IMG_C_AMARETTO_SOUR_JPG = 15,    /*!< @brief File: img/c/amaretto-sour.jpg (size: 3.5 KB) */
  EFEMBEDWWW_EFILE_IMG_C_APEROL_SPRITZ_JPG = 16,    /*!< @brief File: img/c/aperol_spritz.jpg (size: 5.1 KB) */
  EFEMBEDWWW_EFILE_IMG_SVG_BARS_SVG = 17,    /*!< @brief File: img/svg/bars.svg (size: 601 B) */
  EFEMBEDWWW_EFILE_IMG_C_BLOODY_MARY_JPG = 18,    /*!< @brief File: img/c/bloody-mary.jpg (size: 4.9 KB) */
  EFEMBEDWWW_EFILE_IMG_C_DAIQUIRI_JPG = 19,    /*!< @brief File: img/c/daiquiri.jpg (size: 4.0 KB) */
  EFEMBEDWWW_EFILE_IMG_SVG_FILE_EXPORT_SVG = 20,    /*!< @brief File: img/svg/file-export.svg (size: 626 B) */
  EFEMBEDWWW_EFILE_IMG_SVG_FILE_IMPORT_SVG = 21,    /*!< @brief File: img/svg/file-import.svg (size: 630 B) */
  EFEMBEDWWW_EFILE_IMG_SVG_GLASS_MARTINI_ALT_SVG = 22,    /*!< @brief File: img/svg/glass-martini-alt.svg (size: 503 B) */
  EFEMBEDWWW_EFILE_IMG_SVG_GLASS_WHISKEY_SVG = 23,    /*!< @brief File: img/svg/glass-whiskey.svg (size: 439 B) */
  EFEMBEDWWW_EFILE_IMG_C_GRASSHOPPER_JPG = 24,    /*!< @brief File: img/c/grasshopper.jpg (size: 3.1 KB) */
  EFEMBEDWWW_EFILE_IMG_SVG_INFO_SVG = 25,    /*!< @brief File: img/svg/info.svg (size: 568 B) */
  EFEMBEDWWW_EFILE_IMG_SVG_LANGUAGE_SVG = 26,    /*!< @brief File: img/svg/language.svg (size: 1.2 KB) */
  EFEMBEDWWW_EFILE_IMG_C_MARGARITA_JPG = 27,    /*!< @brief File: img/c/margarita.jpg (size: 2.9 KB) */
  EFEMBEDWWW_EFILE_IMG_C_MARTINI_JPG = 28,    /*!< @brief File: img/c/martini.jpg (size: 3.1 KB) */
  EFEMBEDWWW_EFILE_FONT_ORBITRON_BOLD_WOFF = 29,    /*!< @brief File: font/Orbitron-Bold.woff (size: 13.3 KB) */
  EFEMBEDWWW_EFILE_FONT_ORBITRON_REGULAR_WOFF = 30,    /*!< @brief File: font/Orbitron-Regular.woff (size: 12.9 KB) */
  EFEMBEDWWW_EFILE_IMG_C_PORNSTAR_JPG = 31,    /*!< @brief File: img/c/pornstar.jpg (size: 4.8 KB) */
  EFEMBEDWWW_EFILE_IMG_SVG_POWER_OFF_SVG = 32,    /*!< @brief File: img/svg/power-off.svg (size: 723 B) */
  EFEMBEDWWW_EFILE_FONT_ROBOTOMONO_BOLD_WOFF = 33,    /*!< @brief File: font/RobotoMono-Bold.woff (size: 52.4 KB) */
  EFEMBEDWWW_EFILE_FONT_ROBOTOMONO_REGULAR_WOFF = 34,    /*!< @brief File: font/RobotoMono-Regular.woff (size: 52.1 KB) */
  EFEMBEDWWW_EFILE_IMG_C_SEX_ON_THE_BEACH_JPG = 35,    /*!< @brief File: img/c/sex-on-the-beach.jpg (size: 5.7 KB) */
  EFEMBEDWWW_EFILE_IMG_C_SKITTLE_JPG = 36,    /*!< @brief File: img/c/skittle.jpg (size: 973 B) */
  EFEMBEDWWW_EFILE_IMG_C_TOM_COLLINS_JPG = 37,    /*!< @brief File: img/c/tom-collins.jpg (size: 3.0 KB) */
  EFEMBEDWWW_EFILE_IMG_SVG_WIFI_SVG = 38,    /*!< @brief File: img/svg/wifi.svg (size: 795 B) */
  EFEMBEDWWW_EFILE_IMG_SVG_WINE_BOTTLE_SVG = 39,    /*!< @brief File: img/svg/wine-bottle.svg (size: 702 B) */
  EFEMBEDWWW_EFILE_IMG_SVG_WRENCH_SVG = 40,    /*!< @brief File: img/svg/wrench.svg (size: 713 B) */
  EFEMBEDWWW_EFILE_COUNT = 41
} EFEMBEDWWW_EFILE;

/*! @brief Check if compressed flag is active */
#define EFEMBEDWWW_ISFILECOMPRESSED(x) ((x & EFEMBEDWWW_EFLAGS_GZip) == EFEMBEDWWW_EFLAGS_GZip)

extern const EFEMBEDWWW_SFile EFEMBEDWWW_g_sFiles[EFEMBEDWWW_EFILE_COUNT];
extern const uint8_t EFEMBEDWWW_g_u8Blobs[];

#endif
