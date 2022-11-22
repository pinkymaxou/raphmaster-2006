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
                    Please put your glass on the sleigh
                </div>
            </div>
            <div class="progress-bar" style="max-width: 35rem;">
                <span class="progress-bar-fill" style="width: 70%;"></span>
            </div>
            <div>
                <p>Sex on the beach</b></p>
                <ol>
                    <li>Ice</li>
                    <li>Vodka</li>
                    <li>Peach snapps</li>
                    <li>Oranges wedge</li>
                    <li>Cranberry juice</li>
                    <li>Grenadine Syrup</li>
                </ol>
            </div>
        </div>
        `;
    }
}
