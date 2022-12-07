#include "ProductionApi.h"
#include "esp_log.h"
#include "cJSON.h"
#include "../../Control.h"
#include "nanopb/pb_decode.h"
#include "cocktaildb/cocktaildb.pb.h"
#include "../../CocktailExplorer.h"

#define TAG "API/Production"

bool PRODUCTIONAPI_HandleOrder(const char* szData, uint32_t u32Length)
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
        pRecipe = COCKTAILEXPLORER_GetRecipeById(pOrder_recipeid->valueint);
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

char* PRODUCTIONAPI_GetStatus()
{
    cJSON* pRoot = NULL;

    char buff[100];
    pRoot = cJSON_CreateObject();
    if (pRoot == NULL)
        goto ERROR;

    const CONTROL_SInfo sInfo = CONTROL_GetInfos();

    cJSON_AddItemToObject(pRoot, "recipe_id", cJSON_CreateNumber(sInfo.u32RecipeId));
    cJSON_AddItemToObject(pRoot, "state", cJSON_CreateNumber(sInfo.eState));
    cJSON_AddItemToObject(pRoot, "is_cancel_request", cJSON_CreateBool(sInfo.bIsCancelRequest));
    cJSON_AddItemToObject(pRoot, "x", cJSON_CreateNumber(sInfo.s32X));
    cJSON_AddItemToObject(pRoot, "z", cJSON_CreateNumber(sInfo.s32Z));
    cJSON_AddItemToObject(pRoot, "y", cJSON_CreateNumber(sInfo.s32Y));
    cJSON_AddItemToObject(pRoot, "percent", cJSON_CreateNumber(sInfo.dPercent));
    cJSON_AddItemToObject(pRoot, "backlog_count", cJSON_CreateNumber(sInfo.u32BacklogCount));

    char* pStr = cJSON_PrintUnformatted(pRoot);
    cJSON_Delete(pRoot);
    return pStr;
    ERROR:
    if (pRoot != NULL)
        cJSON_Delete(pRoot);
    return NULL;
}
