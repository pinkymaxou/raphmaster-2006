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
        <form class="pure-form pure-form-stacked">
            <fieldset class="pure-group">
                <legend>Wi-Fi (soft access point)</legend>
                <div class="pure-control-group">
                    <label for="idWiFiSoAPPassword">Password</label>
                    <input type="password" id="idWiFiSoAPPassword" class="pure-input-1-2" placeholder="Password" />
                </div>

                <legend>Wi-Fi (station mode)</legend>
                <label for="idWifiSTAIsActive" class="pure-checkbox">
                    <input type="checkbox" id="idWifiSTAIsActive" style="display: inline" /> Activate STA mode
                </label>

                <div class="pure-control-group">
                    <label for="idWiFiSTASSID">SSID</label>
                    <input type="text" id="idWiFiSTASSID" class="pure-input-1-2" placeholder="SSID" />
                </div>
                <div class="pure-control-group">
                    <label for="idWiFiSTAPassword">Password</label>
                    <input type="password" id="idWiFiSTAPassword" class="pure-input-1-2" placeholder="Password" />
                </div>
            </fieldset>

            <button class="button-normal button-normal-primary">Save and reboot</button>
        </form>
        `;
    }
}