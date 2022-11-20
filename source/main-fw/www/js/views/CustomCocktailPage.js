import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.setTitle("Custom cocktail");

        this.mIngredients = [
            { slotid: 1, name: "Vokda", remainqty: 1500, totalqty: 2000 }
        ];
    }

    async loaded() {

    }

    async getHtml() {
        return `
        <div id="idCustomIngredientList" class="customingredient_container">
            <div class="customingredient_item">
                <div class="customingredient_item_name">Vokda</div>
                <div class="customingredient_item_qty_container">
                    <div class="customingredient_item_qty_item">1 ounces</div>
                    <div class="customingredient_item_qty_item">1.5 ounces</div>
                    <div class="customingredient_item_qty_item">2 ounces</div>
                    <div class="customingredient_item_qty_item">3 ounces</div>
                </div>
            </div>
            <div class="customingredient_item">
                <div class="customingredient_item_name">Clamato</div>
                <div class="customingredient_item_qty_container">
                    <div class="customingredient_item_qty_item">1 ounces</div>
                    <div class="customingredient_item_qty_item">1.5 ounces</div>
                    <div class="customingredient_item_qty_item">2 ounces</div>
                    <div class="customingredient_item_qty_item">3 ounces</div>
                </div>
            </div>
            <div class="customingredient_item">
                <div class="customingredient_item_name">Grenadine</div>
                <div class="customingredient_item_qty_container">
                    <div class="customingredient_item_qty_item">1 ounces</div>
                    <div class="customingredient_item_qty_item">1.5 ounces</div>
                    <div class="customingredient_item_qty_item">2 ounces</div>
                    <div class="customingredient_item_qty_item">3 ounces</div>
                </div>
            </div>
        </div>
        `;
    }
}