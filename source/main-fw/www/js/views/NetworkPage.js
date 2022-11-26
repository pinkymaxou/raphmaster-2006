import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("Network");
    }

    async loaded() {

    }

    async getHtml() {
        return `
        <div class="networksetting-container">
            <fieldset>
                <legend>Wi-Fi (soft access point)</legend>
                <div>
                    <label for="idWiFiSoAPPassword">Password</label>
                    <input type="password" id="idWiFiSoAPPassword" placeholder="Password" />
                </div>
            </fieldset>

            <fieldset>
                <legend>Wi-Fi (station mode)</legend>
                <label for="idWifiSTAIsActive" class="pure-checkbox">
                    <input type="checkbox" id="idWifiSTAIsActive" style="display: inline" /> Activate STA mode
                </label>

                <div class="">
                    <label for="idWiFiSTASSID">SSID</label>
                    <input type="text" id="idWiFiSTASSID" placeholder="SSID" />
                </div>
                <div>
                    <label for="idWiFiSTAPassword">Password</label>
                    <input type="password" id="idWiFiSTAPassword" placeholder="Password" />
                </div>
            </fieldset>
            <div class="button-bar">
                <button id="idBtSave" class="button-normal button-bar-item">Save and reboot</button>
            </div>
        </div>
        `;
    }
}