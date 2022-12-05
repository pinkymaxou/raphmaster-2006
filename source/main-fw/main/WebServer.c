#include "WebServer.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include <stdio.h>
#include <sys/param.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "esp_app_format.h"
#include "assets/EmbedWWW.h"
#include "esp_ota_ops.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "cJSON.h"
#include "Settings.h"
#include "StationSettings.h"
#include "Control.h"
#include "main.h"
#include "HardwareGPIO.h"
#include "CocktailExplorer.h"

#define TAG "webserver"

/* Max length a file path can have on storage */
#define HTTPSERVER_BUFFERSIZE (1024*12)

#define DEFAULT_RELATIVE_URI "/index.html"

#define API_EXPORTSETTINGSJSON_URI "/api/exportsettingsjson"
#define API_IMPORTSETTINGSJSON_URI "/api/importsettingsjson"

#define API_GETCOCKTAILSJSON_URI "/api/getcocktails"
#define API_GETINGREDIENTSLIQUIDSJSON_URI "/api/getingredients/liquids"
#define API_GETAVAILABLEINGREDIENTSJSON_URI "/api/getavailableingredients"
#define API_GETAVAILABLEINGREDIENTSJSON_ID_URI "/api/getavailableingredients/"

#define API_GETSTATINGREDIENTSJSON_URI "/api/getstatingredients"

#define API_GETSTATIONSETTINGSJSON_URI "/api/getstationsettings"
#define API_SETSTATIONSETTINGSJSON_URI "/api/setstationsettings"

#define API_GETNETWORKSETTINGSJSON_URI "/api/getnetworksettings"
#define API_SETNETWORKSETTINGSJSON_URI "/api/setnetworksettings"

#define API_ADDORDERJSON_URI "/api/addorder"

#define API_EXPORTSTATIONSETTINGSJSON_URI "/api/exportstationsettings"
#define API_IMPORTSTATIONSETTINGSJSON_URI "/api/importstationsettings"

#define API_GETSYSINFOJSON_URI "/api/getsysinfo"

#define ACTION_POST_REBOOT "/action/reboot"

static esp_err_t api_get_handler(httpd_req_t *req);
static esp_err_t api_post_handler(httpd_req_t *req);

static esp_err_t file_get_handler(httpd_req_t *req);
static esp_err_t file_post_handler(httpd_req_t *req);
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filename);

static esp_err_t file_otauploadpost_handler(httpd_req_t *req);

static const EFEMBEDWWW_SFile* GetFile(const char* strFilename, uint32_t u32Len);

static char* GetSysInfo();

static char* GetNetworkSettings();
static bool SetNetworkSettings(const char* szJSON, uint32_t u32Length);

static bool HandleAddOrder(const char* szData, uint32_t u32Length);

static void ToHexString(char *dstHexString, const uint8_t* data, uint32_t u32Length);
static const char* GetESPChipId(esp_chip_model_t eChipid);

static uint8_t m_u8Buffers[HTTPSERVER_BUFFERSIZE];

/*! @brief this variable is set by linker script, don't rename it. It contains app image informations. */
extern const esp_app_desc_t esp_app_desc;

static const httpd_uri_t m_sHttpUI = {
    .uri       = "/*",
    .method    = HTTP_GET,
    .handler   = file_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = ""
};

static const httpd_uri_t m_sHttpGetAPI = {
    .uri       = "/api/*",
    .method    = HTTP_GET,
    .handler   = api_get_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = ""
};

static const httpd_uri_t m_sHttpPostAPI = {
    .uri       = "/api/*",
    .method    = HTTP_POST,
    .handler   = api_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = ""
};
static const httpd_uri_t m_sHttpActionPost = {
    .uri       = "/action/*",
    .method    = HTTP_POST,
    .handler   = file_post_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = ""
};

static const httpd_uri_t m_sHttpOTAUploadPost = {
    .uri       = "/ota/upload",
    .method    = HTTP_POST,
    .handler   = file_otauploadpost_handler,
    /* Let's pass response string in user
     * context to demonstrate it's usage */
    .user_ctx  = ""
};
void WEBSERVER_Init()
{
    httpd_handle_t server = NULL;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.lru_purge_enable = true;
    config.uri_match_fn = httpd_uri_match_wildcard;
    config.max_open_sockets = 13;
    config.stack_size = 5500;

    // Start the httpd server
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK) {
        // Set URI handlers
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &m_sHttpActionPost);
        httpd_register_uri_handler(server, &m_sHttpGetAPI);
        httpd_register_uri_handler(server, &m_sHttpPostAPI);
        httpd_register_uri_handler(server, &m_sHttpUI);
        httpd_register_uri_handler(server, &m_sHttpOTAUploadPost);
        // return server;
    }
}

/* An HTTP GET handler */
static esp_err_t file_get_handler(httpd_req_t *req)
{
    const EFEMBEDWWW_SFile* pFile = NULL;

    ESP_LOGI(TAG, "Opening file uri: %s", req->uri);

    int pathlen = strlen(req->uri);
    const char* queryMark = strchr(req->uri, '?');
    if (queryMark) {
        pathlen = MIN(pathlen, queryMark - req->uri);
        queryMark++; // Skip ? character
        ESP_LOGI(TAG, "uri: %s, length: %d, query: %s", req->uri, pathlen, queryMark);
    }

    // Redirect root to index.html
    if (strncmp(req->uri, "/", pathlen) == 0 ||
        strncmp(req->uri, "/about", pathlen) == 0 ||
        strncmp(req->uri, "/network", pathlen) == 0 ||
        strncmp(req->uri, "/calib", pathlen) == 0 ||
        strncmp(req->uri, "/stationsettings", pathlen) == 0 ||
        strncmp(req->uri, "/listcocktailpage", pathlen) == 0 ||
        strncmp(req->uri, "/customcocktail", pathlen) == 0 ||
        strncmp(req->uri, "/status", pathlen) == 0)
    {
        pFile = GetFile(DEFAULT_RELATIVE_URI+1, strlen(DEFAULT_RELATIVE_URI) - 1);
        // No cache
    }
    else {
        pFile = GetFile(req->uri+1, pathlen - 1);

        if (pFile != NULL) {
            httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600");
        }
    }

    if (pFile == NULL)
    {
        // In case it gets unwanted parameters
        if (strncmp(req->uri, "/customcocktail/", strlen("/customcocktail/")) == 0)
        {
            // Remember, browser keep 301 in cache so be careful
            ESP_LOGW(TAG, "Redirect URI: '%s', to '%s'", req->uri, DEFAULT_RELATIVE_URI);
            // Redirect to default page
            httpd_resp_set_type(req, "text/html");
            httpd_resp_set_status(req, "302 Moved Temporarily");
            httpd_resp_set_hdr(req, "Location", DEFAULT_RELATIVE_URI);
            httpd_resp_send(req, NULL, 0);
            return ESP_OK;
        }

        ESP_LOGE(TAG, "Failed to open file for reading");
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "File does not exist");
        return ESP_FAIL;
    }

    set_content_type_from_file(req, pFile->strFilename);
    ESP_LOGI(TAG, "Open file: %s", pFile->strFilename);

    // Fixed cache a 1h for now

    uint32_t u32Index = 0;

    while(u32Index < pFile->u32Length)
    {
        const uint32_t n = MIN(pFile->u32Length - u32Index, 2048);

        if (n > 0) {
            /* Send the buffer contents as HTTP response m_u8Buffers */
            esp_err_t errChunk = httpd_resp_send_chunk(req, (char*)(pFile->pu8StartAddr + u32Index), n);
            if (errChunk != ESP_OK) {
                ESP_LOGE(TAG, "File sending failed!, err: %s", esp_err_to_name(errChunk));
                /* Abort sending file */
                httpd_resp_sendstr_chunk(req, NULL);
                /* Respond with 500 Internal Server Error */
                httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to send file");
               return ESP_FAIL;
           }
        }
        u32Index += n;
    }

    httpd_resp_set_hdr(req, "Connection", "close");
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static esp_err_t file_post_handler(httpd_req_t *req)
{
    ESP_LOGI(TAG, "file_post_handler, url: %s", req->uri);

    if (strcmp(req->uri, ACTION_POST_REBOOT) == 0)
    {
        esp_restart();
    }
    else
    {
        goto ERROR;
    }

    httpd_resp_set_hdr(req, "Connection", "close");
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
    ERROR:
    ESP_LOGE(TAG, "Invalid request");
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Bad request");
    return ESP_FAIL;
}

static esp_err_t api_get_handler(httpd_req_t* req)
{
    char* pExportJSON = NULL;

    if (strcmp(req->uri, API_EXPORTSETTINGSJSON_URI) == 0)
    {
        pExportJSON = NVSJSON_ExportJSON(&g_sSettingHandle, true);
    }
    else if (strcmp(req->uri,API_EXPORTSTATIONSETTINGSJSON_URI) == 0)
    {
        pExportJSON = NVSJSON_ExportJSON(&g_sStationSettingHandle, false);
    }
    else if (strcmp(req->uri, API_GETSYSINFOJSON_URI) == 0)
    {
        pExportJSON = GetSysInfo();
    }
    else if (strcmp(req->uri, API_GETNETWORKSETTINGSJSON_URI) == 0)
    {
        pExportJSON = GetNetworkSettings();
    }
    else if (strcmp(req->uri, API_GETCOCKTAILSJSON_URI) == 0)
    {
        const int64_t u64Start = esp_timer_get_time();
        pExportJSON = COCKTAILEXPLORER_GetAllRecipes(0);
        ESP_LOGI(TAG, "Get all recipe time: %d ms", (int)(esp_timer_get_time() - u64Start) / 1000 );
    }
    else if (strcmp(req->uri, API_GETSTATIONSETTINGSJSON_URI) == 0)
    {
        const int64_t u64Start = esp_timer_get_time();
        pExportJSON = COCKTAILEXPLORER_GetStationSettings();
        ESP_LOGI(TAG, "Get station settings time: %d ms", (int)(esp_timer_get_time() - u64Start) / 1000 );
    }
    else if (strcmp(req->uri, API_GETSTATINGREDIENTSJSON_URI) == 0)
    {
        const int64_t u64Start = esp_timer_get_time();
        pExportJSON = COCKTAILEXPLORER_GetStatIngredients();
        ESP_LOGI(TAG, "Get stat ingredients time: %d ms", (int)(esp_timer_get_time() - u64Start) / 1000 );
    }
    else if (strcmp(req->uri, API_GETINGREDIENTSLIQUIDSJSON_URI) == 0)
    {
        const int64_t u64Start = esp_timer_get_time();
        pExportJSON = COCKTAILEXPLORER_GetAllIngredients(true);
        ESP_LOGI(TAG, "Get all liquid ingredients time: %d ms", (int)(esp_timer_get_time() - u64Start) / 1000 );
    }
    else if (strncmp(req->uri, API_GETAVAILABLEINGREDIENTSJSON_ID_URI, strlen(API_GETAVAILABLEINGREDIENTSJSON_ID_URI)) == 0)
    {
        const int uriLen = strlen(req->uri);
        const int n = strlen(API_GETAVAILABLEINGREDIENTSJSON_ID_URI);
        if (n < uriLen)
        {
            char* param = req->uri + n;
            int recipeId = atoi(param);
            pExportJSON = COCKTAILEXPLORER_GetAllRecipes(recipeId);
        }
    }
    else if (strcmp(req->uri, API_GETAVAILABLEINGREDIENTSJSON_URI) == 0)
    {
        const int64_t u64Start = esp_timer_get_time();
        pExportJSON = COCKTAILEXPLORER_GetAllAvailableIngredients();
        ESP_LOGI(TAG, "Get all available ingredients time: %d ms", (int)(esp_timer_get_time() - u64Start) / 1000 );
    }
    else
    {
        ESP_LOGE(TAG, "api_get_handler, url: %s", req->uri);
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Unknown request");
        goto END;
    }

    esp_err_t err = 0;
    if (pExportJSON == NULL || (err = httpd_resp_send(req, pExportJSON, strlen(pExportJSON))) != ESP_OK)
    {
        ESP_LOGE(TAG, "Unable to send data, uri: '%s', pointer: %d, err: %d", req->uri, (int)pExportJSON, (int)err);
        httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Unable to send data");
        goto END;
    }

    END:
    if (pExportJSON != NULL)
        free(pExportJSON);
    httpd_resp_set_hdr(req, "Connection", "close");
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
}

static esp_err_t api_post_handler(httpd_req_t *req)
{
    const char* szError = NULL;

    int n = httpd_req_recv(req, (char*)m_u8Buffers, HTTPSERVER_BUFFERSIZE);
    m_u8Buffers[n] = '\0';

    ESP_LOGI(TAG, "api_post_handler, url: %s", req->uri);
    if (strcmp(req->uri, API_IMPORTSETTINGSJSON_URI) == 0)
    {
        if (!NVSJSON_ImportJSON(&g_sSettingHandle, (const char*)m_u8Buffers))
        {
            szError = "Unable to import JSON";
            goto ERROR;
        }
        ESP_LOGI(TAG, "Import settings");
    }
    else if (strcmp(req->uri, API_IMPORTSTATIONSETTINGSJSON_URI) == 0)
    {
        if (!NVSJSON_ImportJSON(&g_sStationSettingHandle, (const char*)m_u8Buffers))
        {
            szError = "Unable to import JSON";
            goto ERROR;
        }
        ESP_LOGI(TAG, "Import station settings");
    }
    else if (strcmp(req->uri, API_ADDORDERJSON_URI) == 0)
    {
        if (!HandleAddOrder((char*)m_u8Buffers, n))
        {
            szError = "Unable to add order";
            goto ERROR;
        }
        ESP_LOGI(TAG, "Request: %s handled with success", API_ADDORDERJSON_URI);
    }
    else if (strcmp(req->uri, API_SETSTATIONSETTINGSJSON_URI) == 0)
    {
        if (!COCKTAILEXPLORER_SetStationSettings((char*)m_u8Buffers, n))
        {
            szError = "Unable to save data";
            goto ERROR;
        }
        ESP_LOGI(TAG, "Set station settings");
    }
    else if (strcmp(req->uri, API_SETNETWORKSETTINGSJSON_URI) == 0)
    {
        if (!SetNetworkSettings((char*)m_u8Buffers, n))
        {
            szError = "Unable to save data";
            goto ERROR;
        }
        ESP_LOGI(TAG, "Set station settings");
    }
    else
    {
        ESP_LOGE(TAG, "api_post_handler, url: %s", req->uri);
        httpd_resp_send_err(req, HTTPD_404_NOT_FOUND, "Unknown request");
    }

    httpd_resp_set_hdr(req, "Connection", "close");
    httpd_resp_send_chunk(req, NULL, 0);
    return ESP_OK;
    ERROR:
    if (szError != NULL)
        ESP_LOGE(TAG, "API post error: %s", szError);

    httpd_resp_set_hdr(req, "Connection", "close");
    httpd_resp_send_err(req, HTTPD_400_BAD_REQUEST, "Error");
    return ESP_OK;
}

static esp_err_t file_otauploadpost_handler(httpd_req_t *req)
{
    // Temporarily deactivated for security reasons
    goto ERROR;

    ESP_LOGI(TAG, "file_otauploadpost_handler / uri: %s", req->uri);

    const esp_partition_t *configured = esp_ota_get_boot_partition();
    const esp_partition_t *running = esp_ota_get_running_partition();

    if (configured != running)
    {
        ESP_LOGW(TAG, "Configured OTA boot partition at offset 0x%08x, but running from offset 0x%08x",
                 configured->address, running->address);
        ESP_LOGW(TAG, "(This can happen if either the OTA boot data or preferred boot image become corrupted somehow.)");
    }

    ESP_LOGI(TAG, "Running partition type %d subtype %d (offset 0x%08x)",
             running->type, running->subtype, running->address);

    const esp_partition_t* update_partition = esp_ota_get_next_update_partition(NULL);
    assert(update_partition != NULL);
    ESP_LOGI(TAG, "Writing to partition subtype %d at offset 0x%x",
             update_partition->subtype, update_partition->address);

    esp_ota_handle_t update_handle = 0;

    esp_err_t err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
    err = esp_ota_begin(update_partition, OTA_WITH_SEQUENTIAL_WRITES, &update_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_ota_begin failed (%s)", esp_err_to_name(err));
        esp_ota_abort(update_handle);
        goto ERROR;
    }

    int n = httpd_req_recv(req, (char*)m_u8Buffers, HTTPSERVER_BUFFERSIZE);
    int binary_file_length = 0;

    while(n > 0)
    {
        ESP_LOGI(TAG, "file_otauploadpost_handler / receiving: %d bytes", n);

        err = esp_ota_write( update_handle, (const void *)m_u8Buffers, n);
        if (err != ESP_OK)
        {
            esp_ota_abort(update_handle);
            goto ERROR;
        }
        binary_file_length += n;
        ESP_LOGD(TAG, "Written image length %d", binary_file_length);

        n = httpd_req_recv(req, (char*)m_u8Buffers, HTTPSERVER_BUFFERSIZE);
    }

    err = esp_ota_end(update_handle);
    if (err != ESP_OK)
    {
        if (err == ESP_ERR_OTA_VALIDATE_FAILED) {
            ESP_LOGE(TAG, "Image validation failed, image is corrupted");
        } else {
            ESP_LOGE(TAG, "esp_ota_end failed (%s)!", esp_err_to_name(err));
        }

        // TODO: esp_ota_abort(update_handle); needed ?
        goto ERROR;
    }

    err = esp_ota_set_boot_partition(update_partition);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "esp_ota_set_boot_partition failed (%s)!", esp_err_to_name(err));
        goto ERROR;
    }

    ESP_LOGI(TAG, "OTA Completed !");
    ESP_LOGI(TAG, "Prepare to restart system!");

    esp_restart();

    httpd_resp_set_hdr(req, "Connection", "close");
    return ESP_OK;
ERROR:
    httpd_resp_set_hdr(req, "Connection", "close");
    httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Invalid image");
    return ESP_FAIL;
}

#define IS_FILE_EXT(filename, ext) \
    (strcasecmp(&filename[strlen(filename) - strlen(ext)], ext) == 0)

/* Set HTTP response content type according to file extension */
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char* filename)
{
    if (IS_FILE_EXT(filename, ".pdf")) {
        return httpd_resp_set_type(req, "application/pdf");
    } else if (IS_FILE_EXT(filename, ".ico")) {
        return httpd_resp_set_type(req, "image/x-icon");
    } else if (IS_FILE_EXT(filename, ".css")) {
        return httpd_resp_set_type(req, "text/css");
    } else if (IS_FILE_EXT(filename, ".txt")) {
        return httpd_resp_set_type(req, "text/plain");
    } else if (IS_FILE_EXT(filename, ".js")) {
        return httpd_resp_set_type(req, "text/javascript");
    } else if (IS_FILE_EXT(filename, ".json")) {
        return httpd_resp_set_type(req, "application/json");
    } else if (IS_FILE_EXT(filename, ".ttf")) {
        return httpd_resp_set_type(req, "application/x-font-truetype");
    } else if (IS_FILE_EXT(filename, ".woff")) {
        return httpd_resp_set_type(req, "application/font-woff");
    } else if (IS_FILE_EXT(filename, ".html") || IS_FILE_EXT(filename, ".htm")) {
        return httpd_resp_set_type(req, "text/html");
    } else if (IS_FILE_EXT(filename, ".jpeg") || IS_FILE_EXT(filename, ".jpg")) {
        return httpd_resp_set_type(req, "image/jpeg");
    }
    else if (IS_FILE_EXT(filename, ".svg")) {
        return httpd_resp_set_type(req, "image/svg+xml");
    }

    /* This is a limited set only */
    /* For any other type always set as plain text */
    return httpd_resp_set_type(req, "text/plain");
}

static const EFEMBEDWWW_SFile* GetFile(const char* strFilename, uint32_t u32Len)
{
    for(int i = 0; i < EFEMBEDWWW_EFILE_COUNT; i++)
    {
        const EFEMBEDWWW_SFile* pFile = &EFEMBEDWWW_g_sFiles[i];
        if (strncmp(pFile->strFilename, strFilename, u32Len) == 0)
            return pFile;
    }

    return NULL;
}

static char* GetSysInfo()
{
    cJSON* pRoot = NULL;

    char buff[100];
    pRoot = cJSON_CreateObject();
    if (pRoot == NULL)
        goto ERROR;

    cJSON* pEntries = cJSON_AddArrayToObject(pRoot, "infos");

    esp_chip_info_t sChipInfo;
    esp_chip_info(&sChipInfo);

    // Chip
    cJSON* pEntryJSON0 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON0, "name", cJSON_CreateString("Chip"));
    cJSON_AddItemToObject(pEntryJSON0, "value", cJSON_CreateString(GetESPChipId(sChipInfo.model)));
    cJSON_AddItemToArray(pEntries, pEntryJSON0);

    // Firmware
    cJSON* pEntryJSON1 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON1, "name", cJSON_CreateString("Firmware"));
    cJSON_AddItemToObject(pEntryJSON1, "value", cJSON_CreateString(esp_app_desc.version));
    cJSON_AddItemToArray(pEntries, pEntryJSON1);

    // Compile Time
    cJSON* pEntryJSON2 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON2, "name", cJSON_CreateString("Compile Time"));
    sprintf(buff, "%s %s", /*0*/esp_app_desc.date, /*0*/esp_app_desc.time);
    cJSON_AddItemToObject(pEntryJSON2, "value", cJSON_CreateString(buff));
    cJSON_AddItemToArray(pEntries, pEntryJSON2);

    // SHA256
    cJSON* pEntryJSON3 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON3, "name", cJSON_CreateString("SHA256"));
    char elfSHA256[sizeof(esp_app_desc.app_elf_sha256)*2 + 1] = {0,};
    ToHexString(elfSHA256, esp_app_desc.app_elf_sha256, sizeof(esp_app_desc.app_elf_sha256));
    cJSON_AddItemToObject(pEntryJSON3, "value", cJSON_CreateString(elfSHA256));
    cJSON_AddItemToArray(pEntries, pEntryJSON3);

    // IDF
    cJSON* pEntryJSON4 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON4, "name", cJSON_CreateString("IDF"));
    cJSON_AddItemToObject(pEntryJSON4, "value", cJSON_CreateString(esp_app_desc.idf_ver));
    cJSON_AddItemToArray(pEntries, pEntryJSON4);

    // WiFi-STA
    uint8_t u8Macs[6];
    cJSON* pEntryJSON6 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON6, "name", cJSON_CreateString("WiFi.STA"));
    esp_read_mac(u8Macs, ESP_MAC_WIFI_STA);
    sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X", /*0*/u8Macs[0], /*1*/u8Macs[1], /*2*/u8Macs[2], /*3*/u8Macs[3], /*4*/u8Macs[4], /*5*/u8Macs[5]);
    cJSON_AddItemToObject(pEntryJSON6, "value", cJSON_CreateString(buff));
    cJSON_AddItemToArray(pEntries, pEntryJSON6);

    // WiFi-AP
    cJSON* pEntryJSON5 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON5, "name", cJSON_CreateString("WiFi.AP"));
    esp_read_mac(u8Macs, ESP_MAC_WIFI_SOFTAP);
    sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X", /*0*/u8Macs[0], /*1*/u8Macs[1], /*2*/u8Macs[2], /*3*/u8Macs[3], /*4*/u8Macs[4], /*5*/u8Macs[5]);
    cJSON_AddItemToObject(pEntryJSON5, "value", cJSON_CreateString(buff));
    cJSON_AddItemToArray(pEntries, pEntryJSON5);

    // WiFi-BT
    cJSON* pEntryJSON7 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON7, "name", cJSON_CreateString("WiFi.BT"));
    esp_read_mac(u8Macs, ESP_MAC_BT);
    sprintf(buff, "%02X:%02X:%02X:%02X:%02X:%02X", /*0*/u8Macs[0], /*1*/u8Macs[1], /*2*/u8Macs[2], /*3*/u8Macs[3], /*4*/u8Macs[4], /*5*/u8Macs[5]);
    cJSON_AddItemToObject(pEntryJSON7, "value", cJSON_CreateString(buff));
    cJSON_AddItemToArray(pEntries, pEntryJSON7);

    // Memory (Internal)
    cJSON* pEntryJSON8 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON8, "name", cJSON_CreateString("Memory (all)"));
    const int totalSize = heap_caps_get_total_size(MALLOC_CAP_8BIT);
    const int usedSize = totalSize - heap_caps_get_free_size(MALLOC_CAP_8BIT);
    sprintf(buff, "%d / %d", /*0*/usedSize, /*1*/totalSize);
    cJSON_AddItemToObject(pEntryJSON8, "value", cJSON_CreateString(buff));
    cJSON_AddItemToArray(pEntries, pEntryJSON8);

    // Memory (External)
    cJSON* pEntryJSON81 = cJSON_CreateObject();
    multi_heap_info_t heap_infoExt;
    heap_caps_get_info(&heap_infoExt, MALLOC_CAP_SPIRAM | MALLOC_CAP_8BIT);
    const uint32_t u32TotalMemoryExt = heap_infoExt.total_free_bytes + heap_infoExt.total_allocated_bytes;
    cJSON_AddItemToObject(pEntryJSON81, "name", cJSON_CreateString("Memory (external)"));
    sprintf(buff, "%d / %d", /*0*/heap_infoExt.total_allocated_bytes, /*1*/u32TotalMemoryExt);
    cJSON_AddItemToObject(pEntryJSON81, "value", cJSON_CreateString(buff));
    cJSON_AddItemToArray(pEntries, pEntryJSON81);

    // WiFi-station (IP address)
    cJSON* pEntryJSON9 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON9, "name", cJSON_CreateString("WiFi (STA)"));
    esp_netif_ip_info_t wifiIpSta;
    MAIN_GetWiFiSTAIP(&wifiIpSta);
    sprintf(buff, IPSTR, IP2STR(&wifiIpSta.ip));
    cJSON_AddItemToObject(pEntryJSON9, "value", cJSON_CreateString(buff));
    cJSON_AddItemToArray(pEntries, pEntryJSON9);

    // WiFi-Soft AP (IP address)
    cJSON* pEntryJSON10 = cJSON_CreateObject();
    cJSON_AddItemToObject(pEntryJSON10, "name", cJSON_CreateString("WiFi (Soft-AP)"));
    esp_netif_ip_info_t wifiIpSoftAP;
    MAIN_GetWiFiSoftAPIP(&wifiIpSoftAP);
    sprintf(buff, IPSTR, IP2STR(&wifiIpSoftAP.ip));
    cJSON_AddItemToObject(pEntryJSON10, "value", cJSON_CreateString(buff));
    cJSON_AddItemToArray(pEntries, pEntryJSON10);

    char* pStr =  cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return NULL;
}

static char* GetNetworkSettings()
{
    cJSON* pRoot = NULL;
    pRoot = cJSON_CreateObject();
    if (pRoot == NULL)
        goto ERROR;

    // Soft Access Point config
    wifi_config_t wifi_configSAP;
    esp_wifi_get_config(WIFI_IF_AP, &wifi_configSAP);

    cJSON* pNewWiFiSap = cJSON_CreateObject();
    cJSON_AddItemToObject(pNewWiFiSap, "ssid", cJSON_CreateString((char*)wifi_configSAP.ap.ssid));
    cJSON_AddItemToObject(pRoot, "wifi_sap", pNewWiFiSap);

    cJSON* pNewWiFiSTA = cJSON_CreateObject();
    char wifiSTASSID[32+1];
    size_t wifiSTASSIDLength = 32;
    NVSJSON_GetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WSTASSID, (char*)wifiSTASSID, &wifiSTASSIDLength);
    const bool bIsWifiSTAActive = NVSJSON_GetValueInt32(&g_sSettingHandle, SETTINGS_EENTRY_WSTAIsActive) == true;
    cJSON_AddItemToObject(pNewWiFiSTA, "is_active", cJSON_CreateBool(bIsWifiSTAActive));
    cJSON_AddItemToObject(pNewWiFiSTA, "ssid", cJSON_CreateString(wifiSTASSID));
    cJSON_AddItemToObject(pRoot, "wifi_sta", pNewWiFiSTA);

    char* pStr =  cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return NULL;
}

static bool SetNetworkSettings(const char* szJSON, uint32_t u32Length)
{
    const char* szError = NULL;

    cJSON* pRoot = cJSON_ParseWithLength(szJSON, u32Length);

    cJSON* pWifiSAP_Password = NULL;

    cJSON* pWifiSTA_IsActive = NULL;
    cJSON* pWifiSTA_SSID = NULL;
    cJSON* pWifiSTA_Password = NULL;

    // If not specified, just ignore
    cJSON* pWifiSAP = cJSON_GetObjectItemCaseSensitive(pRoot, "wifi_sap");
    if (pWifiSAP != NULL)
    {
        pWifiSAP_Password = cJSON_GetObjectItemCaseSensitive(pWifiSAP, "password");
        if (pWifiSAP_Password == NULL || cJSON_IsNull(pWifiSAP_Password))
            pWifiSAP_Password = NULL;
        else if (!cJSON_IsString(pWifiSAP_Password))
        {
            szError = "SAP password field is invalid";
            goto ERROR;
        }
    }

    // If not specified, just ignore
    cJSON* pWifiSTA = cJSON_GetObjectItemCaseSensitive(pRoot, "wifi_sta");
    if (pWifiSTA != NULL)
    {
        pWifiSTA_IsActive = cJSON_GetObjectItemCaseSensitive(pWifiSTA, "is_active");

        if (pWifiSTA_IsActive == NULL || cJSON_IsNull(pWifiSTA_IsActive))
            pWifiSTA_IsActive = NULL;
        else if (!cJSON_IsBool(pWifiSTA_IsActive))
        {
            szError = "STA is active field is invalid";
            goto ERROR;
        }

        pWifiSTA_SSID = cJSON_GetObjectItemCaseSensitive(pWifiSTA, "ssid");
        if (pWifiSTA_SSID == NULL || cJSON_IsNull(pWifiSTA_SSID))
            pWifiSTA_SSID = NULL;
        else if (!cJSON_IsString(pWifiSTA_SSID))
        {
            szError = "STA SSID field is invalid";
            goto ERROR;
        }

        pWifiSTA_Password = cJSON_GetObjectItemCaseSensitive(pWifiSTA, "password");
        if (pWifiSTA_Password == NULL || cJSON_IsNull(pWifiSTA_Password))
            pWifiSTA_Password = NULL;
        else if (!cJSON_IsString(pWifiSTA_Password))
        {
            szError = "STA password field is invalid";
            goto ERROR;
        }
    }

    // Check if value are valids
    for(int isDryRun = 1; isDryRun >= 0; isDryRun--)
    {
        if (pWifiSAP_Password != NULL)
            if (NVSJSON_SetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WAPPass, (bool)isDryRun, pWifiSAP_Password->valuestring) != NVSJSON_ESETRET_OK)
            {
                szError = "WAP password is invalid";
                goto ERROR;
            }

        if (pWifiSTA_IsActive != NULL)
            if (NVSJSON_SetValueInt32(&g_sSettingHandle, SETTINGS_EENTRY_WSTAIsActive, (bool)isDryRun, pWifiSTA_IsActive->valueint) != NVSJSON_ESETRET_OK)
            {
                szError = "STA SSID is invalid";
                goto ERROR;
            }

        if (pWifiSTA_SSID != NULL)
            if (NVSJSON_SetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WSTASSID, (bool)isDryRun, pWifiSTA_SSID->valuestring) != NVSJSON_ESETRET_OK)
            {
                szError = "STA SSID is invalid";
                goto ERROR;
            }

        if (pWifiSTA_Password != NULL)
            if (NVSJSON_SetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WSTAPass, (bool)isDryRun, pWifiSTA_Password->valuestring) != NVSJSON_ESETRET_OK)
            {
                szError = "STA password is invalid";
                goto ERROR;
            }
    }

    cJSON_Delete(pRoot);
    return true;
    ERROR:
    if (szError != NULL)
        ESP_LOGE(TAG, "Error: %s", szError);
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return false;
}

static bool HandleAddOrder(const char* szData, uint32_t u32Length)
{
    const char* szError = NULL;
    cJSON* pRoot = cJSON_ParseWithLength(szData, u32Length);
    if (pRoot == NULL)
    {
        szError = "unable to decode json";
        goto ERROR;
    }

    cJSON* pOrder_recipeid = cJSON_GetObjectItemCaseSensitive(pRoot, "recipe_id");
    if (pOrder_recipeid == NULL || !cJSON_IsNumber(pOrder_recipeid))
    {
        szError = "recipe_id is missing from json";
        goto ERROR;
    }

    CONTROL_SOrder sOrder = {
        .u32RecipeId = 0,
        .u32MakerStepCount = 0
    };

    // Recipe isn't mandatory
    const cocktaildb_Recipe* pRecipe = NULL;
    if (pOrder_recipeid->valueint > 0)
    {
        pRecipe = COCKTAILEXPLORER_GetRecipe(pOrder_recipeid->valueint);
        if (pRecipe == NULL)
        {
            szError = "cannot find recipe";
            goto ERROR;
        }
    }

    cJSON* pOrder_steps = cJSON_GetObjectItemCaseSensitive(pRoot, "steps");
    if (pOrder_steps == NULL || !cJSON_IsArray(pOrder_steps))
    {
        szError = "steps is missing from json";
        goto ERROR;
    }

    sOrder.u32RecipeId = (uint32_t)pOrder_recipeid->valueint;
    sOrder.u32MakerStepCount = 0;

    const cJSON* pOrder_stepItem = NULL;
    cJSON_ArrayForEach(pOrder_stepItem, pOrder_steps)
    {
        const cJSON* pingredient_id = cJSON_GetObjectItemCaseSensitive(pOrder_stepItem, "ingredient_id");
        if (pingredient_id == NULL || !cJSON_IsNumber(pingredient_id))
        {
            szError = "ingredient_id cannot be found in JSON";
            goto ERROR;
        }

        // Find ingredient id and station related to it
        uint32_t u32StationId = 0;
        const cocktaildb_Ingredient* pIngredient = COCKTAILEXPLORER_GetAvailableIngredient(pingredient_id->valueint, &u32StationId);
        if (pIngredient == NULL)
        {
            szError = "cannot find the ingredient loaded into an existing station";
            goto ERROR;
        }

        const cJSON* pQty_ml = cJSON_GetObjectItemCaseSensitive(pOrder_stepItem, "qty_ml");
        if (pQty_ml == NULL || !cJSON_IsNumber(pQty_ml))
        {
            szError = "qty_ml cannot be found in JSON";
            goto ERROR;
        }

        if (pQty_ml->valueint <= 0 || pQty_ml->valueint > 1000)
        {
            szError = "quantity (ml) is not valid";
            goto ERROR;
        }

        CONTROL_MakerStep* pMakerStep = &sOrder.sMakerSteps[sOrder.u32MakerStepCount];
        // Find station by ingredient
        pMakerStep->u32StationID = u32StationId;
        pMakerStep->u32Qty_ml = pQty_ml->valueint;
        sOrder.u32MakerStepCount++;
    }

    if (sOrder.u32MakerStepCount == 0)
    {
        szError = "No steps";
        goto ERROR;
    }

    if (!CONTROL_QueueOrder(&sOrder))
    {
        szError = "Order list is full";
        goto ERROR;
    }

    ESP_LOGI(TAG, "Order added to queue");

    cJSON_Delete(pRoot);
    return true;
    ERROR:
    if (szError != NULL)
        ESP_LOGE(TAG, "Error: %s", szError);
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return false;
}

static void ToHexString(char *dstHexString, const uint8_t* data, uint32_t u32Length)
{
    for (uint32_t i = 0; i < u32Length; i++)
        sprintf(dstHexString + (i * 2), "%02X", data[i]);
}

static const char* GetESPChipId(esp_chip_model_t eChipid)
{
    switch(eChipid)
    {
        case CHIP_ESP32:
            return "ESP32";
        case CHIP_ESP32S2:
            return "ESP32-S2";
        case CHIP_ESP32C3:
            return "ESP32-C3";
        case CHIP_ESP32S3:
            return "ESP32-S3";
        case CHIP_ESP32H2:
            return "ESP32-H2";
    }
    return "";
}