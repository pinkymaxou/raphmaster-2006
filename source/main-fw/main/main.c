#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/timers.h"
#include "freertos/task.h"
#include "esp_ota_ops.h"
#include "esp_flash_partitions.h"
#include "esp_partition.h"
#include "esp_now.h"
#include "esp_crc.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_log.h"
#include "driver/gpio.h"
#include "HardwareGPIO.h"
#include "mdns.h"
#include "lwip/apps/netbiosns.h"
#include "HWConfig.h"
#include "FWConfig.h"
#include "WebServer.h"
#include "Settings.h"
#include "Control.h"
#include "StationSettings.h"
#include "CocktailExplorer.h"

#define TAG "main"

static esp_netif_t* m_pWifiSoftAP;
static esp_netif_t* m_pWifiSTA;

static void wifi_init();

static void wifisoftap_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);
static void wifistation_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data);

static void mdns_sn_init();

static void wifi_init()
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK( esp_wifi_init(&cfg) );
    ESP_ERROR_CHECK( esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    const bool isWiFiSTA = NVSJSON_GetValueInt32(&g_sSettingHandle, SETTINGS_EENTRY_WSTAIsActive) == 1;
    if (isWiFiSTA)
    {
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA) );
    }
    else
    {
        ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP) );
    }

    // Access point mode
    m_pWifiSoftAP = esp_netif_create_default_wifi_ap();

    esp_netif_ip_info_t ipInfo;
    IP4_ADDR(&ipInfo.ip, 192, 168, 4, 1);
    IP4_ADDR(&ipInfo.gw, 192, 168, 4, 1);
    IP4_ADDR(&ipInfo.netmask, 255, 255, 255, 0);
    esp_netif_dhcps_stop(m_pWifiSoftAP);
    esp_netif_set_ip_info(m_pWifiSoftAP, &ipInfo);
    esp_netif_dhcps_start(m_pWifiSoftAP);

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifisoftap_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_configAP = {
        .ap = {
            .ssid = {0},
            .ssid_len = 0,
            .max_connection = 5,
        },
    };

    uint8_t macAddr[6];
    esp_read_mac(macAddr, ESP_MAC_WIFI_SOFTAP);

    sprintf((char*)wifi_configAP.ap.ssid, FWCONFIG_STAAP_WIFI_SSID, macAddr[3], macAddr[4], macAddr[5]);
    int n = strlen((const char*)wifi_configAP.ap.ssid);
    wifi_configAP.ap.ssid_len = n;

    size_t staPassLength = 64;
    NVSJSON_GetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WAPPass, (char*)wifi_configAP.ap.password, &staPassLength);

    if (strlen((const char*)wifi_configAP.ap.password) == 0)
    {
        wifi_configAP.ap.authmode = WIFI_AUTH_OPEN;
    }
    else {
        wifi_configAP.ap.authmode = WIFI_AUTH_WPA_WPA2_PSK;
    }

    ESP_LOGI(TAG, "SoftAP: %s", wifi_configAP.ap.ssid);

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_configAP));

    if (isWiFiSTA)
    {
        m_pWifiSTA = esp_netif_create_default_wifi_sta();

        esp_event_handler_instance_t instance_any_id;
        esp_event_handler_instance_t instance_got_ip;
        ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                            ESP_EVENT_ANY_ID,
                                                            &wifistation_event_handler,
                                                            NULL,
                                                            &instance_any_id));
        ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                            IP_EVENT_STA_GOT_IP,
                                                            &wifistation_event_handler,
                                                            NULL,
                                                            &instance_got_ip));

        wifi_config_t wifi_configSTA = {
            .sta = {
                .threshold.authmode = WIFI_AUTH_WPA2_PSK,

                .pmf_cfg = {
                    .capable = true,
                    .required = false
                },
            },
        };

        size_t staSSIDLength = 32;
        NVSJSON_GetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WSTASSID, (char*)wifi_configSTA.sta.ssid, &staSSIDLength);

        size_t staPassLength = 64;
        NVSJSON_GetValueString(&g_sSettingHandle, SETTINGS_EENTRY_WSTAPass, (char*)wifi_configSTA.sta.password, &staPassLength);

        ESP_LOGI(TAG, "STA mode is active, attempt to connect to ssid: %s", wifi_configSTA.sta.ssid);

        ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_configSTA) );
    }

    ESP_ERROR_CHECK( esp_wifi_start());
}

void MAIN_GetWiFiSTAIP(esp_netif_ip_info_t* ip)
{
    esp_netif_get_ip_info(m_pWifiSTA, ip);
}

void MAIN_GetWiFiSoftAPIP(esp_netif_ip_info_t* ip)
{
    esp_netif_get_ip_info(m_pWifiSoftAP, ip);
}

static void mdns_sn_init()
{
    ESP_LOGI(TAG, "mdns_sn_init, hostname: '%s', desc: '%s', service: '%s'", FWCONFIG_MDNS_HOSTNAME, FWCONFIG_MDNS_DESCRIPTION, FWCONFIG_MDNS_SERVICENAME);

    mdns_init();
    mdns_hostname_set(FWCONFIG_MDNS_HOSTNAME);
    mdns_instance_name_set(FWCONFIG_MDNS_DESCRIPTION);

    mdns_txt_item_t serviceTxtData[] = {
        {"funct", "cocktail-maker"},
        {"path", "/"}
    };

    ESP_ERROR_CHECK(mdns_service_add(FWCONFIG_MDNS_SERVICENAME, "_http", "_tcp", 80, serviceTxtData,
                                     sizeof(serviceTxtData) / sizeof(serviceTxtData[0])));

    netbiosns_init();
    netbiosns_set_name(FWCONFIG_MDNS_HOSTNAME);
}

static void failed_alloc_cb(size_t size, uint32_t caps, const char *function_name)
{
    ESP_LOGE("HEAP", "Failed alloc: %d, caps: %d, function: %s", size, caps, function_name);
}

void app_main(void)
{
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK( nvs_flash_erase() );
        ret = nvs_flash_init();
    }

    heap_caps_register_failed_alloc_callback(failed_alloc_cb);

    ESP_ERROR_CHECK( ret );

    CONTROL_Init();

    SETTINGS_Init();

    STATIONSETTINGS_Init();

    HARDWAREGPIO_Init();

    COCKTAILEXPLORER_Init();

    wifi_init();

    mdns_sn_init();

    // |WIFI_PROTOCOL_LR
    ESP_ERROR_CHECK( esp_wifi_set_protocol(ESP_IF_WIFI_AP, WIFI_PROTOCOL_11B|WIFI_PROTOCOL_11G|WIFI_PROTOCOL_11N) );

    WEBSERVER_Init();

    CONTROL_StartTask();

    char* szAllTask = (char*)malloc(4096);
    vTaskList(szAllTask);
    ESP_LOGI(TAG, "vTaskList: \r\n\r\n%s", szAllTask);
    free(szAllTask);

    while (true)
    {
        // Leave some time, we want the poor idle tasks to have some time
        vTaskDelay(1);
    }
}

static void wifisoftap_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

static void wifistation_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG, "retry to connect to the AP");
        ESP_LOGI(TAG,"connect to the AP fail");
    } else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
        ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
    }
}
