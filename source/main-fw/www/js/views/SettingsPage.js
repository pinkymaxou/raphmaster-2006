import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("Settings");
    }

    async loaded() {

    }

    async getHtml() {
        return `
        <form class="pure-form">
            <fieldset class="pure-group">
                <input type="text" class="pure-input-1-2" placeholder="Username" />
                <input type="text" class="pure-input-1-2" placeholder="Password" />
                <input type="email" class="pure-input-1-2" placeholder="Email" />
            </fieldset>
            <fieldset class="pure-group">
                <input type="text" class="pure-input-1-2" placeholder="A title" />
                <textarea class="pure-input-1-2" placeholder="Textareas work too"></textarea>
            </fieldset>
            <button type="submit" class="pure-button pure-input-1-2 pure-button-primary">Sign in</button>
        </form>
        `;
    }
}