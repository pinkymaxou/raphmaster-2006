import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.setTitle("Custom cocktail");
    }

    async loaded() {

    }

    async getHtml() {
        return `
        Custom Cocktail Page
        `;
    }
}