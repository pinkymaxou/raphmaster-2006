#include "MiscApi.h"
#include "cJSON.h"
#include "esp_app_format.h"

#include "main.h"

static void ToHexString(char *dstHexString, const uint8_t* data, uint32_t u32Length);
static const char* GetESPChipId(esp_chip_model_t eChipid);

/*! @brief this variable is set by linker script, don't rename it. It contains app image informations. */
extern const esp_app_desc_t esp_app_desc;

char* MISCAPI_GetSysInfo()
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
