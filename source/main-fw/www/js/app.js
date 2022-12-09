
// Assuming fraction can be pretty ...
function getPrettyFraction(qtyValue)
{
    let wholeValue = Math.trunc(qtyValue);
    let fractionValue = Math.abs(qtyValue - wholeValue);

    let fractionText = "";
    if (fractionValue > 0) {
        let diff = Math.abs(fractionValue);
        if (diff < 0.125) {
            fractionText = "";
            fractionValue = 0;
        }
        else if (diff >= 0.125 && diff < 0.375)
            fractionText = "1/4";
        else if (diff >= 0.375 && diff < 0.625)
            fractionText = "1/2";
        else if (diff >= 0.625 && diff < 0.875)
            fractionText = "3/4";
        else {
            // Just consider is a whole part
            fractionText = "";
            fractionValue = 0;
            wholeValue++;
        }

    }

    if (wholeValue > 0 && fractionValue > 0)
        return String(wholeValue) + " " + fractionText;

    if (fractionValue > 0)
        return fractionText;

    if (wholeValue > 0)
        return String(wholeValue);

    return "";
}

function getPrettyQty(qtyValue, unit)
{
    let qtyText = "";
    if (unit == EQtyType.unitary) {
        qtyText = getPrettyFraction(qtyValue);
    }
    else if (unit == EQtyType.liquid_ml) {
        qtyText = String(qtyValue) + " ml";
    }
    else if (unit == EQtyType.liquid_oz) {
        qtyText = getPrettyFraction(qtyValue) + " oz";
    }
    else if (unit == EQtyType.cup) {
        qtyText = getPrettyFraction(qtyValue) + " cup";
    }
    else if (unit == EQtyType.pinch) {
        qtyText = getPrettyFraction(qtyValue) + " pinch";
    }
    else if (unit == EQtyType.teaspoon) {
        qtyText = getPrettyFraction(qtyValue) + " tsp";
    }
    else if (unit == EQtyType.tablespoon) {
        qtyText = getPrettyFraction(qtyValue) + " tbsp";
    }
    else if (unit == EQtyType.dash) {
        qtyText = getPrettyFraction(qtyValue) + " dash";
    }
    else if (unit == EQtyType.drop) {
        qtyText = getPrettyFraction(qtyValue) + " drop";
    }
    return qtyText;
}

function getControlPrettyStepString(controlStep)
{
    switch(controlStep) {
        case EControlState.IdleWaitingForOrder:
            return "Idling";
        case EControlState.MoveToHomeStart:
            return "Moving the sleigh to home";
        case EControlState.WaitingForGlass:
            return "Please deposit your glass on the sleigh";
        case EControlState.MoveToStation:
            return "Moving to a station";
        case EControlState.FillingGlass:
            return "Filling glass";
        case EControlState.MoveBackToHomeEnd:
            return "Moving back to home";
        case EControlState.WaitForRemovingGlass:
            return "Wait for the glass to be removed";
        case EControlState.Cancelled:
            return "Last operation cancelled";

        case EControlState.CmdHomeAll:
            return "Manual: Home all";
        case EControlState.CmdMoveAxis:
            return "Manual: Move axis";
        case EControlState.CmdMoveToStation:
            return "Manual: Move to station";
    }

    return "-/-"
}

// const g_eventStatusUpdate = new );

async function timerHandler() {

    // Get system informations
    await fetch('/api/getstatus')
        .then((response) => {
            if (!response.ok) {
                throw new Error('Unable to process the order');
            }
            return response.json();
        })
        .then((data) =>
        {
            //console.log("data", data);
            document.dispatchEvent(new CustomEvent('status_update', {
                detail: { status : data },
                cancelable: true
              }));

            setTimeout(timerHandler, 500);
        })
        .catch((ex) =>
        {
            setTimeout(timerHandler, 5000);
            console.error('getstatus', ex);
        });
}

window.addEventListener("load", (event) => {
    console.log("page is fully loaded");

    setTimeout(timerHandler, 500);
});

