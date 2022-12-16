idf_component_register(SRCS "main.c" "Settings.c" "StationSettings.c" "webserver/WebServer.c" "HardwareGPIO.c" "CocktailExplorer.c" "Control.c"
    "assets/EmbedWWW.c" "assets/EmbedCocktailDb.c" "nanopb/pb_common.c" 
    "webserver/api/SettingsApi.c" "webserver/api/CocktailApi.c" "webserver/api/ProductionApi.c" "webserver/api/MiscApi.c"
    "nanopb/pb_decode.c" "nanopb/pb_encode.c" "cocktaildb/cocktaildb.pb.c"
    INCLUDE_DIRS "." "nanopb"
	REQUIRES lwip nvsjson nvs_flash json esp_http_server esp_wifi app_update fatfs esp_timer esp_hw_support esp_common log)
