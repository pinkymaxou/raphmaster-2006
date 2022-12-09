#include "WebServer.h"
#include "esp_log.h"
#include "esp_vfs.h"
#include <stdio.h>
#include <sys/param.h>
#include <sys/unistd.h>
#include <sys/stat.h>
#include <dirent.h>
#include "assets/EmbedWWW.h"
#include "esp_ota_ops.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "cJSON.h"
#include "Settings.h"
#include "ApiURL.h"
#include "StationSettings.h"
#include "Control.h"
#include "HardwareGPIO.h"
#include "CocktailExplorer.h"
#include "api/CocktailApi.h"
#include "api/SettingsApi.h"
#include "api/ProductionApi.h"
#include "api/MiscApi.h"

#define TAG "webserver"

/* Max length a file path can have on storage */
#define HTTPSERVER_BUFFERSIZE (1024*12)

static esp_err_t api_get_handler(httpd_req_t *req);
static esp_err_t api_post_handler(httpd_req_t *req);

static esp_err_t file_get_handler(httpd_req_t *req);
static esp_err_t file_post_handler(httpd_req_t *req);
static esp_err_t set_content_type_from_file(httpd_req_t *req, const char *filename);

static esp_err_t file_otauploadpost_handler(httpd_req_t *req);

static const EFEMBEDWWW_SFile* GetFile(const char* strFilename, uint32_t u32Len);

static uint8_t m_u8Buffers[HTTPSERVER_BUFFERSIZE];

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
        pFile = GetFile(APIURL_DEFAULT_RELATIVE_URI+1, strlen(APIURL_DEFAULT_RELATIVE_URI) - 1);
        // No cache
    }
    else {
        pFile = GetFile(req->uri+1, pathlen - 1);

        if (pFile != NULL)
            httpd_resp_set_hdr(req, "Cache-Control", "public, max-age=3600");
    }

    if (pFile == NULL)
    {
        // In case it gets unwanted parameters
        if (strncmp(req->uri, "/customcocktail/", strlen("/customcocktail/")) == 0)
        {
            // Remember, browser keep 301 in cache so be careful
            ESP_LOGW(TAG, "Redirect URI: '%s', to '%s'", req->uri, APIURL_DEFAULT_RELATIVE_URI);
            // Redirect to default page
            httpd_resp_set_type(req, "text/html");
            httpd_resp_set_status(req, "302 Moved Temporarily");
            httpd_resp_set_hdr(req, "Location", APIURL_DEFAULT_RELATIVE_URI);
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

    if (strcmp(req->uri, APIURL_ACTION_POST_REBOOT) == 0)
    {
        esp_restart();
    }
    else if (strcmp(req->uri, APIURL_ACTION_POST_CANCEL) == 0)
    {
        CONTROL_Cancel();
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

    if (strcmp(req->uri, APIURL_EXPORTSETTINGSJSON_URI) == 0)
    {
        pExportJSON = NVSJSON_ExportJSON(&g_sSettingHandle, true);
    }
    else if (strcmp(req->uri,APIURL_EXPORTSTATIONSETTINGSJSON_URI) == 0)
    {
        pExportJSON = NVSJSON_ExportJSON(&g_sStationSettingHandle, false);
    }
    else if (strcmp(req->uri, APIURL_GETSYSINFOJSON_URI) == 0)
    {
        pExportJSON = MISCAPI_GetSysInfo();
    }
    else if (strcmp(req->uri, APIURL_GETSTATUSJSON_URI) == 0)
    {
        pExportJSON = PRODUCTIONAPI_GetStatus();
    }
    else if (strcmp(req->uri, APIURL_GETNETWORKSETTINGSJSON_URI) == 0)
    {
        pExportJSON = SETTINGSAPI_GetNetworkSettings();
    }
    else if (strcmp(req->uri, APIURL_GETCOCKTAILSJSON_URI) == 0)
    {
        const int64_t u64Start = esp_timer_get_time();
        pExportJSON = COCKTAILAPI_GetAllRecipes(0);
        ESP_LOGI(TAG, "Get all recipe time: %d ms", (int)(esp_timer_get_time() - u64Start) / 1000 );
    }
    else if (strcmp(req->uri, APIURL_GETSTATIONSETTINGSJSON_URI) == 0)
    {
        const int64_t u64Start = esp_timer_get_time();
        pExportJSON = SETTINGSAPI_GetStationSettings();
        ESP_LOGI(TAG, "Get station settings time: %d ms", (int)(esp_timer_get_time() - u64Start) / 1000 );
    }
    else if (strcmp(req->uri, APIURL_GETSTATINGREDIENTSJSON_URI) == 0)
    {
        const int64_t u64Start = esp_timer_get_time();
        pExportJSON = COCKTAILAPI_GetStatIngredients();
        ESP_LOGI(TAG, "Get stat ingredients time: %d ms", (int)(esp_timer_get_time() - u64Start) / 1000 );
    }
    else if (strcmp(req->uri, APIURL_GETINGREDIENTSLIQUIDSJSON_URI) == 0)
    {
        const int64_t u64Start = esp_timer_get_time();
        pExportJSON = COCKTAILAPI_GetAllIngredients(true);
        ESP_LOGI(TAG, "Get all liquid ingredients time: %d ms", (int)(esp_timer_get_time() - u64Start) / 1000 );
    }
    else if (strncmp(req->uri, APIURL_GETAVAILABLEINGREDIENTSJSON_ID_URI, strlen(APIURL_GETAVAILABLEINGREDIENTSJSON_ID_URI)) == 0)
    {
        const int uriLen = strlen(req->uri);
        const int n = strlen(APIURL_GETAVAILABLEINGREDIENTSJSON_ID_URI);
        if (n < uriLen)
        {
            char* param = req->uri + n;
            int recipeId = atoi(param);
            pExportJSON = COCKTAILAPI_GetAllRecipes(recipeId);
        }
    }
    else if (strcmp(req->uri, APIURL_GETAVAILABLEINGREDIENTSJSON_URI) == 0)
    {
        const int64_t u64Start = esp_timer_get_time();
        pExportJSON = COCKTAILAPI_GetAllAvailableIngredients();
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
    if (strcmp(req->uri, APIURL_IMPORTSETTINGSJSON_URI) == 0)
    {
        if (!NVSJSON_ImportJSON(&g_sSettingHandle, (const char*)m_u8Buffers))
        {
            szError = "Unable to import JSON";
            goto ERROR;
        }
        ESP_LOGI(TAG, "Import settings");
    }
    else if (strcmp(req->uri, APIURL_IMPORTSTATIONSETTINGSJSON_URI) == 0)
    {
        if (!NVSJSON_ImportJSON(&g_sStationSettingHandle, (const char*)m_u8Buffers))
        {
            szError = "Unable to import JSON";
            goto ERROR;
        }
        ESP_LOGI(TAG, "Import station settings");
    }
    else if (strcmp(req->uri, APIURL_ADDORDERJSON_URI) == 0)
    {
        if (!PRODUCTIONAPI_HandleOrder((char*)m_u8Buffers, n))
        {
            szError = "Unable to add order";
            goto ERROR;
        }
        ESP_LOGI(TAG, "Request: %s handled with success", APIURL_ADDORDERJSON_URI);
    }
    else if (strcmp(req->uri, APIURL_SETSTATIONSETTINGSJSON_URI) == 0)
    {
        if (!SETTINGSAPI_SetStationSettings((char*)m_u8Buffers, n))
        {
            szError = "Unable to save data";
            goto ERROR;
        }
        ESP_LOGI(TAG, "Set station settings");
    }
    else if (strcmp(req->uri, APIURL_SETNETWORKSETTINGSJSON_URI) == 0)
    {
        if (!SETTINGSAPI_SetNetworkSettings((char*)m_u8Buffers, n))
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
