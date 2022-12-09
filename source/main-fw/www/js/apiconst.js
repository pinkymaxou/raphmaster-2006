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
	leaf_or_sprig: 8
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
            return 5;
    }
    return 11;
}
