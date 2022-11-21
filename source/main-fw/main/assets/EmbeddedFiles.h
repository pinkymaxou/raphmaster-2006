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
  EF_EFILE_INDEX_HTML = 0,    /*!< @brief File: index.html (size: 1.2 KB) */
  EF_EFILE_CSS_CONTENT_CSS = 1,    /*!< @brief File: css/content.css (size: 2.2 KB) */
  EF_EFILE_CSS_GRIDS_RESPONSIVE_MIN_CSS = 2,    /*!< @brief File: css/grids-responsive-min.css (size: 14.0 KB) */
  EF_EFILE_CSS_PURE_MIN_CSS = 3,    /*!< @brief File: css/pure-min.css (size: 15.4 KB) */
  EF_EFILE_JS_VIEWS_ABOUTPAGE_JS = 4,    /*!< @brief File: js/views/AboutPage.js (size: 2.3 KB) */
  EF_EFILE_JS_VIEWS_ABSTRACTVIEW_JS = 5,    /*!< @brief File: js/views/AbstractView.js (size: 249 B) */
  EF_EFILE_JS_APP_JS = 6,    /*!< @brief File: js/app.js (size: 0 B) */
  EF_EFILE_JS_VIEWS_CALIBPAGE_JS = 7,    /*!< @brief File: js/views/CalibPage.js (size: 5.7 KB) */
  EF_EFILE_JS_VIEWS_CUSTOMCOCKTAILPAGE_JS = 8,    /*!< @brief File: js/views/CustomCocktailPage.js (size: 3.7 KB) */
  EF_EFILE_JS_VIEWS_INDEXPAGE_JS = 9,    /*!< @brief File: js/views/IndexPage.js (size: 5.7 KB) */
  EF_EFILE_JS_VIEWS_NETWORKPAGE_JS = 10,    /*!< @brief File: js/views/NetworkPage.js (size: 1.6 KB) */
  EF_EFILE_JS_ROUTER_JS = 11,    /*!< @brief File: js/router.js (size: 2.3 KB) */
  EF_EFILE_JS_VIEWS_SETTINGSPAGE_JS = 12,    /*!< @brief File: js/views/SettingsPage.js (size: 306 B) */
  EF_EFILE_JS_VIEWS_SLOTSETTINGSPAGE_JS = 13,    /*!< @brief File: js/views/SlotSettingsPage.js (size: 4.5 KB) */
  EF_EFILE_FAVICON_ICO = 14,    /*!< @brief File: favicon.ico (size: 15.0 KB) */
  EF_EFILE_IMG_BLOODY_MARY_JPG = 15,    /*!< @brief File: img/bloody-mary.jpg (size: 4.9 KB) */
  EF_EFILE_IMG_GRASSHOPPER_JPG = 16,    /*!< @brief File: img/grasshopper.jpg (size: 3.1 KB) */
  EF_EFILE_FONT_ORBITRON_BOLD_TTF = 17,    /*!< @brief File: font/Orbitron-Bold.ttf (size: 23.7 KB) */
  EF_EFILE_FONT_ORBITRON_REGULAR_TTF = 18,    /*!< @brief File: font/Orbitron-Regular.ttf (size: 23.8 KB) */
  EF_EFILE_IMG_PORNSTAR_JPG = 19,    /*!< @brief File: img/pornstar.jpg (size: 4.8 KB) */
  EF_EFILE_FONT_ROBOTOMONO_BOLD_TTF = 20,    /*!< @brief File: font/RobotoMono-Bold.ttf (size: 85.0 KB) */
  EF_EFILE_FONT_ROBOTOMONO_REGULAR_TTF = 21,    /*!< @brief File: font/RobotoMono-Regular.ttf (size: 84.9 KB) */
  EF_EFILE_IMG_SEX_ON_THE_BEACH_JPG = 22,    /*!< @brief File: img/sex-on-the-beach.jpg (size: 5.7 KB) */
  EF_EFILE_COUNT = 23
} EF_EFILE;

/*! @brief Check if compressed flag is active */
#define EF_ISFILECOMPRESSED(x) ((x & EF_EFLAGS_GZip) == EF_EFLAGS_GZip)

extern const EF_SFile EF_g_sFiles[EF_EFILE_COUNT];
extern const uint8_t EF_g_u8Blobs[];

#endif
