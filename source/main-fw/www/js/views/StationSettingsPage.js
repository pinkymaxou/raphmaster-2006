import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.mColumnCount = 4;
        this.setTitle("Station settings");
    }

    addIngredient(id, name) {
        let cboNewIngredientOpt = document.createElement("option");
        cboNewIngredientOpt.setAttribute("value", id);
        cboNewIngredientOpt.text = name;
        return cboNewIngredientOpt;
    }

    async loaded() {

        let idTblBdStations = document.querySelector("#idTblBdStations");

        const API_GETINGREDIENTS = '/api/getingredients/liquids';

        // Get system informations
        await fetch(API_GETINGREDIENTS)
            .then((response) => response.json())
            .then((data) => this.mIngredients = data)
            .catch((ex) => console.error('getingredients', ex));
        this.mIngredients.sort((a, b) => a.name.localeCompare(b.name));

        const API_GETSTATIONSETTINGS = '/api/getstationsettings';

        // Get system informations
        await fetch(API_GETSTATIONSETTINGS)
            .then((response) => response.json())
            .then((data) => this.mStationItems = data)
            .catch((ex) => console.error('getstationsettings', ex));

        // Add info items
        let i = 0;
        let targetThis = this;

        this.mStationItems.forEach(
            (stationItem) =>
            {
                let newTr = idTblBdStations.insertRow();

                // Alternate row
                if (i % 2 == 0) {
                    newTr.classList.add("info-table-odd");
                }

                // =====================
                const tdStation = newTr.insertCell(); // create td only
                let inputHiddenStationId = document.createElement("input");
                inputHiddenStationId.setAttribute("id", "idStationId");
                inputHiddenStationId.setAttribute("type", "hidden");
                inputHiddenStationId.setAttribute("value", stationItem.id);
                tdStation.appendChild(inputHiddenStationId);
                tdStation.appendChild(document.createTextNode(stationItem.id));

                // =====================
                const tdValue = newTr.insertCell(); // create td only
                // Add select
                let cboSelectIngredient = document.createElement("select");
                cboSelectIngredient.setAttribute("id", "idSelectIngredient");
                // Ingredients
                cboSelectIngredient.appendChild(this.addIngredient(0, "--- None ---"));
                this.mIngredients.forEach( (ingredient) => cboSelectIngredient.appendChild(this.addIngredient(ingredient.id, ingredient.name)) );
                cboSelectIngredient.value = stationItem.ingredient_id;
                tdValue.appendChild(cboSelectIngredient);

                // =====================
                // Create textbox for total (qty)
                const tdValueTotalQtyML = newTr.insertCell(); // create td only
                let inputUsedQtyText = document.createElement("input");
                inputUsedQtyText.setAttribute("id", "idValueTotalQty");
                inputUsedQtyText.setAttribute("type", "number");
                inputUsedQtyText.setAttribute("min", 0);
                inputUsedQtyText.setAttribute("max", 6000);
                inputUsedQtyText.setAttribute("value", stationItem.total_ml);
                tdValueTotalQtyML.appendChild(inputUsedQtyText);

                // =====================
                // Create textbox for used (qty)
                const tdValueUsedQtyML = newTr.insertCell(); // create td only
                let inputUsedQty = document.createElement("input");
                inputUsedQty.setAttribute("id", "idValueUsedQty");
                inputUsedQty.setAttribute("type", "number");
                inputUsedQty.setAttribute("min", 0);
                inputUsedQty.setAttribute("max", 6000);
                inputUsedQty.setAttribute("value", stationItem.used_ml);
                tdValueUsedQtyML.appendChild(inputUsedQty);

                i++;
            });

        // Bind buttons
        let idBtSave = document.querySelector("#idBtSave");
        idBtSave.addEventListener('click', function() {
            // Get ordered item list
            let idTblBdStations = document.querySelector("#idTblBdStations");
            const matches = idTblBdStations.querySelectorAll("tr td");

            let stationSettingItems = [];

            for(let i = 0; i < matches.length; i += targetThis.mColumnCount)
            {
                let idStationId = matches[i].querySelector("#idStationId");
                let idSelectIngredient = matches[i+1].querySelector("#idSelectIngredient");
                let idValueTotalQty = matches[i+2].querySelector("#idValueTotalQty");
                let idValueUsedQty = matches[i+3].querySelector("#idValueUsedQty");

                let item = { id: Number(idStationId.value), ingredient_id: Number(idSelectIngredient.value), total_ml: Number(idValueTotalQty.value), used_ml : Number(idValueUsedQty.value) };
                stationSettingItems.push(item);
            }

            fetch('/api/setstationsettings', {
                method: 'POST',
                headers: {
                  'Content-Type': 'application/json',
                },
                body: JSON.stringify(stationSettingItems),
              })
            .then((response) => {
                if (!response.ok) {
                    throw new Error('Unable to process the order');
                }
                console.log('Success:', response);
            })
            .catch((error) => {
                console.error('Error:', error);
                alert("Error: " + error);
            });
        });
    }

    async getHtml() {
        return `
        <table class="info-table">
            <thead>
                <tr>
                    <th>Station #</th>
                    <th>Ingredient</th>
                    <th>Total Qty (ml)</th>
                    <th>Used Qty (ml)</th>
                </tr>
            </thead>
            <tbody id="idTblBdStations">
                <!-- Space for table items -->
            </tbody>
        </table>
        <div class="button-bar">
            <button id="idBtSave" class="button-normal button-bar-item">Save</button>
        </div>
        `;
    }
}