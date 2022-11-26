import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("Station settings");
    }

    addIngredient(id, name) {
        let cboNewIngredientOpt = document.createElement("option");
        cboNewIngredientOpt.setAttribute("value", id);
        cboNewIngredientOpt.text = name;
        return cboNewIngredientOpt;
    }

    async loaded() {

        let tbodySysInfo = document.querySelector("#tblBdStations");

        const API_GETINGREDIENTS = '/api/getingredients/liquids';

        // Get system informations
        await fetch(API_GETINGREDIENTS)
            .then((response) => response.json())
            .then((data) => this.mIngredients = data)
            .catch((ex) => console.error('getingredients', ex));

        const API_GETSTATIONSETTINGS = '/api/getstationsettings';

        // Get system informations
        await fetch(API_GETSTATIONSETTINGS)
            .then((response) => response.json())
            .then((data) => this.mStationItems = data)
            .catch((ex) => console.error('getstationsettings', ex));
    
        // Add info items
        let i = 0;

        this.mStationItems.forEach(
            (stationItem) =>
            {
                let newTr = tbodySysInfo.insertRow();

                // Alternate row
                if (i % 2 == 0) {
                    newTr.classList.add("info-table-odd");
                }

                // =====================
                const tdStation = newTr.insertCell(); // create td only
                tdStation.appendChild(document.createTextNode(stationItem.id));
                
                // =====================
                const tdValue = newTr.insertCell(); // create td only
                // Add select
                let cboSelectIngredient = document.createElement("select");               
                // Ingredients
                cboSelectIngredient.appendChild(this.addIngredient(0, "--- None ---"));
                this.mIngredients.forEach( (ingredient) => cboSelectIngredient.appendChild(this.addIngredient(ingredient.id, ingredient.name)) );                
                tdValue.appendChild(cboSelectIngredient);
 
                // =====================
                // Create textbox for total (qty)
                const tdValueTotalQtyML = newTr.insertCell(); // create td only
                let btnValueTotalQtyML = document.createElement("input");
                btnValueTotalQtyML.setAttribute("type", "number");
                btnValueTotalQtyML.setAttribute("min", 0);
                btnValueTotalQtyML.setAttribute("max", 6000);
                btnValueTotalQtyML.setAttribute("value", stationItem.total_ml);
                tdValueTotalQtyML.appendChild(btnValueTotalQtyML);

                // =====================
                // Create textbox for used (qty)
                const tdValueUsedQtyML = newTr.insertCell(); // create td only
                let enumUsedQtyText = document.createElement("input");
                enumUsedQtyText.setAttribute("type", "number");
                enumUsedQtyText.setAttribute("min", 0);
                enumUsedQtyText.setAttribute("max", 6000);
                enumUsedQtyText.setAttribute("value", stationItem.used_ml);
                tdValueUsedQtyML.appendChild(enumUsedQtyText);

                i++;
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
            <tbody id="tblBdStations">
                <!-- Space for table items -->
            </tbody>
        </table>
        <br>
        <button class="button-normal">Save</button>
        `;
    }
}