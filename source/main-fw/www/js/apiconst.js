const EQtyType = Object.freeze({
	none: 0,		// When unit doesn't apply
	unitary: 1, 	// Counter by round number

	liquid_ml: 2,	// Milliliter
	liquid_oz: 3,			// Ounce
	cup: 4,		    // Cup
	pinch: 6,		// Fingertip quantity
	teaspoon: 8,	// 4.5 ml (1/6 oz)
	tablespoon: 9,	// 14 ml (1/2 oz)
	dash: 10,		// 1 ml
	drop: 11,		// One drop
    squeeze: 12 	// squeeze
});
const EIngredientType = Object.freeze({
	unspecified: 0,
	liquid_alcohol: 1,
	reserved_0x02: 2,
	liquid_filler: 3,
	ice: 4,
	powder: 5,
	emulsifying: 6,
	fruit_or_vegetable: 7,
	leaf_or_sprig: 8,
	flavoring: 9
});

const EControlState = Object.freeze({
	unspecified: 0,
    IdleWaitingForOrder: 0,
    MoveToHomeStart: 1,
    WaitingForGlass: 2,
    MoveToStation: 3,
    FillingGlass: 4,
    MoveBackToHomeEnd: 5,
    WaitForRemovingGlass: 6,
    Cancelled: 7,

    CmdHomeAll: 50,
    CmdMoveAxis: 51,
    CmdMoveToStation: 52
});

const OneOz = 30;

function getIngredientTypeOrder(ingredientType) {
    switch(ingredientType) {
        case EIngredientType.liquid_alcohol:
        case EIngredientType.liquid_filler:
            return 1;
        case EIngredientType.ice:
            return 3;
        case EIngredientType.fruit_or_vegetable:
        case EIngredientType.emulsifying:
        case EIngredientType.powder:
            return 4;
        case EIngredientType.leaf_or_sprig:
        case EIngredientType.flavoring:
            return 5;
    }
    return 11;
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
    else if (unit == EQtyType.squeeze) {
        qtyText = getPrettyFraction(qtyValue) + " squeeze";
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
