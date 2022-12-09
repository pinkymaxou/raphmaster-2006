export default class {
    constructor(params) {
        this.params = params;
    }

    setTitle(title) {
        document.title = title;
    }

    async loaded() {

    }

    async unloaded() {

    }

    async getHtml() {
        return "test12345";
    }
}