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
            <p class="status_cocktailname">Sex on the beach</b></p>
            <div class="status_progress_container">
                <div class="progress-bar" style="margin-right: 10px;">
                    <span class="progress-bar-fill" style="width: 70%;"></span>
                </div>
                <button class="button-cancel">Cancel</button>
            </div>
        </div>
        `;
    }
}
