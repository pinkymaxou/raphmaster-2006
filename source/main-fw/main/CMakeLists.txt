idf_component_register(SRCS "main.c" "Settings.c" "StationSettings.c" "webserver/WebServer.c" "HardwareGPIO.c" "CocktailExplorer.c" "Control.c"
    "assets/EmbedWWW.c" "assets/EmbedCocktailDb.c" "nanopb/pb_common.c" "webserver/api/Settings.c" "webserver/api/CocktailApi.c"
    "nanopb/pb_decode.c" "nanopb/pb_encode.c" "cocktaildb/cocktaildb.pb.c"
    INCLUDE_DIRS "." "nanopb")
