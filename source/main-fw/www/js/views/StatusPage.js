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
                <p>Job: <b>Make Sex on the beach</b></p>
                <ol>
                    <li>Pour "White Rhum"</li>
                    <li>Pour "Vokda"</li>
                    <li>Pour "Grenadine"</li>
                    <li>Moving back to home</li>
                    <li>Waiting for user to take the glass</li>
                </ol>
            </div>

            <!-- After the glass get removed. -->
            <div>
                <div class="status_userinstruction">
                    Remaining instructions to finish "Sex on the beach"
                </div>
                <ol>
                    <li>Ice</li>
                    <li>Oranges wedge (2)</li>
                    <li>Cranberry juice (50ml)</li>
                    <li>Pour 7-up</li>
                </ol>
            </div>
        </div>
        `;
    }
}