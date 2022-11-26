idf_component_register(SRCS "main.c" "Settings.c" "StationSettings.c" "WebServer.c" "HardwareGPIO.c" "CocktailExplorer.c" 
    "assets/EmbedWWW.c" "assets/EmbedCocktailDb.c" "nanopb/pb_common.c" 
    "nanopb/pb_decode.c" "nanopb/pb_encode.c" "cocktaildb/cocktaildb.pb.c"
    INCLUDE_DIRS "." "nanopb")
