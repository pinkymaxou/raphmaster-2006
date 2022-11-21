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
            <div class="status_userinstruction">
                Please put the glass on the sleigh
            </div>
            <div class="progress-bar" style="max-width: 35rem;">
                <span class="progress-bar-fill" style="width: 70%;"></span>
            </div>
            <div>
                <ol>
                    <li>Moving to the "White Rhum" slot</li>
                    <li>Pouring "White Rhum"</li>
                    <li>Moving to the "Vokda" slot</li>
                    <li>Moving back to home</li>
                    <li>Waiting for user to take the glass</li>
                </ol>
            </div>
        </div>
        `;
    }
}