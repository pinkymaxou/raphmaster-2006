import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("Network");
    }

    async loaded() {
        this.mNetworkSettings = null;

        this.mSAPPasswordChanged = false;
        this.mSTAPasswordChanged = false;

        // Get system informations
        await fetch("/api/getnetworksettings")
            .then((response) => response.json())
            .then((data) => this.mNetworkSettings = data)
            .catch((ex) => console.error('getnetworksettings', ex));

        let closureThis = this;

        // Load config
        document.getElementById("idWiFiSAPSSID").innerHTML = this.mNetworkSettings.wifi_sap.ssid;
        let idWiFiSAPPassword = document.getElementById("idWiFiSAPPassword");
        idWiFiSAPPassword.value = "";
        idWiFiSAPPassword.addEventListener("change", () => closureThis.mSAPPasswordChanged = true);

        document.getElementById("idWiFiSTASSID").value = this.mNetworkSettings.wifi_sta.ssid;
        document.getElementById("idWifiSTAIsActive").checked = this.mNetworkSettings.wifi_sta.is_active;
        let idWiFiSTAPassword = document.getElementById("idWiFiSTAPassword");
        idWiFiSTAPassword.value = "";
        idWiFiSTAPassword.addEventListener("change", () => closureThis.mSTAPasswordChanged = true);

        // Bind buttons
        let idBtSave = document.querySelector("#idBtSave");
        idBtSave.addEventListener('click', function() {
            let networkSettings =
            {
                wifi_sap : { password: (closureThis.mSAPPasswordChanged ? document.getElementById("idWiFiSAPPassword").value : null) },
                wifi_sta :
                {
                    is_active: document.getElementById("idWifiSTAIsActive").checked,
                    ssid: document.getElementById("idWiFiSTASSID").value,
                    password: (closureThis.mSTAPasswordChanged ? document.getElementById("idWiFiSTAPassword").value : null)
                },
            };

            console.log("setnetworksettings", networkSettings);

            // Save
            fetch('/api/setnetworksettings', {
                method: 'POST',
                headers: {
                  'Content-Type': 'application/json',
                },
                body: JSON.stringify(networkSettings),
              })
            .then((data) => {
                console.log('Success:', data);
            })
            .catch((error) => {
                console.error('Error:', error);
            });
        });
    }

    async getHtml() {
        return `
        <div class="networksetting-container">
            <fieldset>
                <legend>Wi-Fi (soft access point)</legend>
                <div class="networksetting-2fields-container">
                    <label for="idWiFiSAPSSID">SSID</label>
                    <p id="idWiFiSAPSSID" style="margin: 0; font-weight: bold;">SSID</p>

                    <label for="idWiFiSAPPassword">Password</label>
                    <input type="password" id="idWiFiSAPPassword" placeholder="Password" />
                </div>
            </fieldset>

            <fieldset>
                <legend>Wi-Fi (station mode)</legend>
                <label for="idWifiSTAIsActive" class="pure-checkbox">
                    <input type="checkbox" id="idWifiSTAIsActive" style="display: inline; margin-bottom: 10px;" /> Activate STA mode
                </label>

                <div class="networksetting-2fields-container">
                    <label for="idWiFiSTASSID">SSID</label>
                    <input type="text" id="idWiFiSTASSID" placeholder="SSID" />

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