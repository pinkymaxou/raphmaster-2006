import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("Calibration");
    }

    async loaded() {

        let tbodySysInfo = document.querySelector("#tblBdStations");

        let stationItems = [
            { id:  1, x: 0, y: 1},
            { id:  2, x: 0, y: 1},
            { id:  3, x: 0, y: 1},
            { id:  4, x: 0, y: 1},

            { id:  5, x: 0, y: 1},
            { id:  6, x: 0, y: 1},
            { id:  7, x: 0, y: 1},
            { id:  8, x: 0, y: 1},

            { id:  9, x: 0, y: 1},
            { id: 10, x: 0, y: 1},
            { id: 11, x: 0, y: 1},
            { id: 12, x: 0, y: 1},

            { id: 13, x: 0, y: 1},
            { id: 14, x: 0, y: 1},
            { id: 15, x: 0, y: 1},
            { id: 16, x: 0, y: 1},
        ];

        // Add info items
        let i = 0;

        stationItems.forEach(
            (stationItem) =>
            {
                let newTr = tbodySysInfo.insertRow();

                // Alternate row
                if (i % 2 == 0) {
                    newTr.classList.add("info-table-odd");
                }

                const tdName = newTr.insertCell(); // create td only
                tdName.appendChild(document.createTextNode(stationItem.id));

                const tdValueX = newTr.insertCell(); // create td only
                tdValueX.appendChild(document.createTextNode(stationItem.x));

                const tdValueY = newTr.insertCell(); // create td only
                tdValueY.appendChild(document.createTextNode(stationItem.y));

                const tdValueAct = newTr.insertCell(); // create td only

                // Save
                let btnSave = document.createElement("button");
                btnSave.classList.add("button-normal");
                //btnSave.classList.add("button-normal-primary");
                btnSave.appendChild(document.createTextNode("Set"));
                tdValueAct.appendChild(btnSave);

                // Goto
                let btnGoto = document.createElement("button");
                btnGoto.classList.add("button-normal");
                //btnGoto.classList.add("button-normal-primary");
                btnGoto.appendChild(document.createTextNode("Goto"));
                tdValueAct.appendChild(btnGoto);

                i++;
            });
    }

    async getHtml() {
        return `
            <legend>Calibrate all axis</legend>

            <button class="button-normal button-normal-primary">Home all</button><br><br>

            <br>

            <!-- Axis -->
            <table class="info-table">
            <thead>
                <tr>
                    <th>Axis</th>
                    <th>Value</th>
                    <th>Action</th>
                </tr>
            </thead>
            <tbody>
                <tr class="info-table-odd">
                    <td>X</td>
                    <td>0</td>
                    <td>
                        <div>
                            <button class="button-normal">&lt;&lt;&lt;</button>
                            <button class="button-normal">&lt;&lt;</button>
                            <button class="button-normal">&lt;</button><br>
                            <button class="button-normal">&gt;</button>
                            <button class="button-normal">&gt;&gt;</button>
                            <button class="button-normal">&gt;&gt;&gt;</button>
                        </div>
                    </td>
                </tr>
                <tr>
                    <td>Y</td>
                    <td>0</td>
                    <td>
                        <div>
                            <button class="button-normal">&lt;&lt;&lt;</button>
                            <button class="button-normal">&lt;&lt;</button>
                            <button class="button-normal">&lt;</button><br>
                            <button class="button-normal">&gt;</button>
                            <button class="button-normal">&gt;&gt;</button>
                            <button class="button-normal">&gt;&gt;&gt;</button>
                        </div>
                    </td>
                </tr>
                <tr class="info-table-odd">
                    <td>Z</td>
                    <td>0</td>
                    <td>
                    <div>
                        <button class="button-normal">&lt;&lt;&lt;</button>
                        <button class="button-normal">&lt;&lt;</button>
                        <button class="button-normal">&lt;</button><br>
                        <button class="button-normal">&gt;</button>
                        <button class="button-normal">&gt;&gt;</button>
                        <button class="button-normal">&gt;&gt;&gt;</button>
                    </div>
                    </td>
                </tr>
            </tbody>
            </table>
            <br>

            <table class="info-table">
            <thead>
                <tr>
                    <th>Station #</th>
                    <th>X</th>
                    <th>Y</th>
                    <th>Action</th>
                </tr>
            </thead>
            <tbody id="tblBdStations">
                <!-- Space for table items -->
            </tbody>
            </table>
            <br>
            <button class="button-normal button-normal-primary">Save</button>
        `;
    }
}