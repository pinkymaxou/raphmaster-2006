import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.setTitle("Status");
    }

    async loaded() {

    }

    async getHtml() {
        return `
        <div class="status_container">
            <div style="display: flex">
                <div class="lds-dual-ring" style="margin-top: 10px;"></div>
                <div class="status_userinstruction">
                    Please put a glass on the sleigh
                </div>
            </div>
            <div class="progress-bar">
                <span class="progress-bar-fill" style="width: 70%;"></span>
            </div>
            <div style="max-width: 30rem;">
                <p class="status_cocktailname">Sex on the beach</b></p>
                <div class="cocktail_ingredient_container">
                    <div class="status_ingredient_item colingre_name ingredient_row_odd"><input type="checkbox" class="status_ingredient_item_checkbox" readonly></input>Ice</div>
                    <div class="status_ingredient_item colingre_qty ingredient_row_odd"></div>
                    <div class="status_ingredient_item colingre_name"><input type="checkbox" class="status_ingredient_item_checkbox" readonly></input>Vodka</div>
                    <div class="status_ingredient_item colingre_qty">50ml</div>
                    <div class="status_ingredient_item colingre_name ingredient_row_odd"><input type="checkbox" class="status_ingredient_item_checkbox" readonly></input>Peach snapps</div>
                    <div class="status_ingredient_item colingre_qty ingredient_row_odd">25ml</div>
                    <div class="status_ingredient_item colingre_name"><input type="checkbox" class="status_ingredient_item_checkbox" readonly></input>Oranges wedge</div>
                    <div class="status_ingredient_item colingre_qty">2</div>
                    <div class="status_ingredient_item colingre_name ingredient_row_odd"><input type="checkbox" class="status_ingredient_item_checkbox" readonly></input>Cranberry juice</div>
                    <div class="status_ingredient_item colingre_qty ingredient_row_odd">50ml</div>
                    <div class="status_ingredient_item colingre_name"><input type="checkbox" class="status_ingredient_item_checkbox" readonly></input>Grenadine Syrup</div>
                    <div class="status_ingredient_item colingre_qty">25ml</div>
                </ol>
            </div>
        </div>
        `;
    }
}
