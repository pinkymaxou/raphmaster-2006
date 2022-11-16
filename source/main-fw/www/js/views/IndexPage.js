import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("Cocktails");
    }

    async loaded() {

    }

    async getHtml() {
        return `

        <table class="pure-table">
        <thead>
            <tr>
                <th>#</th>
                <th>Make</th>
                <th>Model</th>
                <th>Year</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>1</td>
                <td>Honda</td>
                <td>Accord</td>
                <td>2009</td>
            </tr>
            <tr>
                <td>2</td>
                <td>Toyota</td>
                <td>Camry</td>
                <td>2012</td>
            </tr>
            <tr>
                <td>3</td>
                <td>Hyundai</td>
                <td>Elantra</td>
                <td>2010</td>
            </tr>
        </tbody>
    </table>
        `;
    }
}