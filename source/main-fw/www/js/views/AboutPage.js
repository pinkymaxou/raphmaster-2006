import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("About");
    }

    async loaded() {
        const API_GETSYSINFO = '/api/getsysinfo';

        let infoItems = null;

        // Get system informations
        await fetch(API_GETSYSINFO)
            .then((response) => response.json())
            .then((data) => infoItems = data.infos)
            .catch((ex) => console.error('getSysInfo', ex));
 
        console.log("load", API_GETSYSINFO, " items: ", infoItems);

        let tbodySysInfo = document.querySelector("#tblBdSysInfo");

        // Add info items
        infoItems.forEach(
            (infoItem) =>
            {
                let newTr = tbodySysInfo.insertRow();

                const tdName = newTr.insertCell(); // create td only
                tdName.appendChild(document.createTextNode(infoItem.name));
                
                const tdValue = newTr.insertCell(); // create td only
                tdValue.style["overflow-wrap"] = "anywhere";
                tdValue.appendChild(document.createTextNode(infoItem.value));
            }
        );
    }

    async getHtml() {
        return `
        <div class="pure-menu pure-menu-horizontal">
            <li class="pure-menu-item">
                <a href="/network" class="pure-menu-link" data-link>Network Settings</a>
            </li>
            <li class="pure-menu-item">
                <a href="/slotsettings" class="pure-menu-link" data-link>Slots Settings</a>
            </li>
            <li class="pure-menu-item">
                <a href="/settings" class="pure-menu-link" data-link>General Settings</a>
            </li>
            <li class="pure-menu-item">
                <a href="/calib" class="pure-menu-link" data-link>Calibration</a>
            </li>
        </div>

        <table class="pure-table pure-table-bordered">
            <thead>
                <tr>
                    <th>Name</th>
                    <th>Value</th>
                </tr>
            </thead>
            <tbody id="tblBdSysInfo">
                <!-- Space for table items -->
            </tbody>
        </table>
        `;
    }
}