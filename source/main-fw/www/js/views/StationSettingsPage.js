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

        let ingredients = [
            { id:  1, name: "Vokda" },           
            { id:  2, name: "Peach Schnapps" },           
        ];

        let stationItems = [
            { id:  1, totalQtyml: 0, remainingQtyml: 0 },
            { id:  2, totalQtyml: 0, remainingQtyml: 0 },
            { id:  3, totalQtyml: 0, remainingQtyml: 0 },
            { id:  4, totalQtyml: 0, remainingQtyml: 0 },
 
            { id:  5, totalQtyml: 0, remainingQtyml: 0 },
            { id:  6, totalQtyml: 0, remainingQtyml: 0 },
            { id:  7, totalQtyml: 0, remainingQtyml: 0 },
            { id:  8, totalQtyml: 0, remainingQtyml: 0 },
 
            { id:  9, totalQtyml: 0, remainingQtyml: 0 },
            { id: 10, totalQtyml: 0, remainingQtyml: 0 },
            { id: 11, totalQtyml: 0, remainingQtyml: 0 },
            { id: 12, totalQtyml: 0, remainingQtyml: 0 },

            { id: 13, totalQtyml: 0, remainingQtyml: 0 },
            { id: 14, totalQtyml: 0, remainingQtyml: 0 },
            { id: 15, totalQtyml: 0, remainingQtyml: 0 },
            { id: 16, totalQtyml: 0, remainingQtyml: 0 },
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

                // =====================
                const tdStation = newTr.insertCell(); // create td only
                tdStation.appendChild(document.createTextNode(stationItem.id));
                
                // =====================
                const tdValue = newTr.insertCell(); // create td only
                // Add select
                let cboSelectIngredient = document.createElement("select");
                
                // Ingredients
                cboSelectIngredient.appendChild(this.addIngredient(0, "--- None ---"));
                ingredients.forEach( (ingredient) => cboSelectIngredient.appendChild(this.addIngredient(ingredient.id, ingredient.name)) );                
                tdValue.appendChild(cboSelectIngredient);
 
                // =====================
                // Create textbox for total (qty)
                const tdValueTotalQtyML = newTr.insertCell(); // create td only
                let btnValueTotalQtyML = document.createElement("input");
                btnValueTotalQtyML.setAttribute("type", "number");
                btnValueTotalQtyML.setAttribute("min", 0);
                btnValueTotalQtyML.setAttribute("max", 6000);
                btnValueTotalQtyML.setAttribute("value", stationItem.totalQtyml);
                tdValueTotalQtyML.appendChild(btnValueTotalQtyML);

                // =====================
                // Create textbox for used (qty)
                const tdValueUsedQtyML = newTr.insertCell(); // create td only
                let enumUsedQtyText = document.createElement("input");
                enumUsedQtyText.setAttribute("type", "number");
                enumUsedQtyText.setAttribute("min", 0);
                enumUsedQtyText.setAttribute("max", 6000);
                enumUsedQtyText.setAttribute("value", stationItem.remainingQtyml);
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
        <button class="pure-button pure-button-primary">Save</button>
        `;
    }
}