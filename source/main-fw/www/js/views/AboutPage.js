import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("About");
    }

    async getHtml() {
        const API_GETSYSINFO = '/api/getsysinfo';

        await fetch(API_GETSYSINFO)
            .then((response) => response.json())
            .then((data) => console.log(data))
            .catch((ex) => console.error('getSysInfo', ex));
 
        return `
        <table class="pure-table">
            <thead>
                <tr>
                    <th>Name</th>
                    <th>Value</th>
                </tr>
            </thead>
            <tbody>
                <tr v-for="sysinfo in sysinfos">
                    <td>3</td>
                    <td>Hyundai</td>
                </tr>
            </tbody>
        </table>
        `;
    }
}