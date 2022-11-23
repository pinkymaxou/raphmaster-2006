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
            <div style="display: flex; flex-direction: row;">
                <div class="lds-dual-ring" style="margin-top: 10px;"></div>
                <div class="status_userinstruction">
                    Please put a glass on the sleigh
                </div>
            </div>
            <div class="status_progress_container">
                <div class="progress-bar" style="margin-right: 0.5rem;">
                    <span class="progress-bar-fill" style="width: 70%;"></span>
                </div>
                <button class="button-cancel">Cancel</button>
            </div>
            <div style="max-width: 30rem;">
                <p class="status_cocktailname">Sex on the beach</b></p>
                <p class="status_infotitle">Handled steps</b></p>
                <div class="cocktail_ingredient_container">
                    <div class="status_ingredient_item colingre_name">Vodka</div>
                    <div class="status_ingredient_item colingre_qty">50ml</div>
                    <div class="status_ingredient_item colingre_name ingredient_row_odd">Peach snapps</div>
                    <div class="status_ingredient_item colingre_qty ingredient_row_odd">25ml</div>
                </div>
                <p class="status_infotitle">Not handled steps</b></p>
                <div class="cocktail_ingredient_container">
                    <div class="status_ingredient_item colingre_name ingredient_row_odd">Ice</div>
                    <div class="status_ingredient_item colingre_qty ingredient_row_odd"></div>
                    <div class="status_ingredient_item colingre_name">Oranges wedge</div>
                    <div class="status_ingredient_item colingre_qty">2</div>
                    <div class="status_ingredient_item colingre_name ingredient_row_odd">Cranberry juice</div>
                    <div class="status_ingredient_item colingre_qty ingredient_row_odd">50ml</div>
                    <div class="status_ingredient_item colingre_name">Grenadine Syrup</div>
                    <div class="status_ingredient_item colingre_qty">25ml</div>
                </ol>
            </div>
        </div>
        `;
    }
}
