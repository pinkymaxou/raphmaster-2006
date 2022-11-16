import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("Calibration");
    }

    async loaded() {

    }

    async getHtml() {
        return `
            Calibration
        `;
    }
}