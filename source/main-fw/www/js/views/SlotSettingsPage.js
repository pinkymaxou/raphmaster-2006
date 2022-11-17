import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("Slot settings");
    }

    addIngredient(id, name) {
        let cboNewIngredientOpt = document.createElement("option");
        cboNewIngredientOpt.setAttribute("value", id);
        cboNewIngredientOpt.text = name;
        return cboNewIngredientOpt;
    }

    async loaded() {

        let tbodySysInfo = document.querySelector("#tblBdSlots");

        let ingredients = [
            { id:  1, name: "Vokda" },           
            { id:  2, name: "Peach Schnapps" },           
        ];

        let slotItems = [
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

        slotItems.forEach(
            (slotItem) =>
            {
                let newTr = tbodySysInfo.insertRow();

                // Alternate row
                if (i % 2 == 0) {
                    newTr.classList.add("pure-table-odd");
                }

                const tdSlot = newTr.insertCell(); // create td only
                tdSlot.appendChild(document.createTextNode(slotItem.id));
                
                const tdValue = newTr.insertCell(); // create td only
                let cboSelectIngredient = document.createElement("select");
                
                // Ingredients
                cboSelectIngredient.appendChild(this.addIngredient(0, "--- None ---"));
                ingredients.forEach(
                    (ingredient) => cboSelectIngredient.appendChild(this.addIngredient(ingredient.id, ingredient.name))
                );                
                tdValue.appendChild(cboSelectIngredient);
 
                // Create textbox for total (qty)
                const tdValueTotalQtyML = newTr.insertCell(); // create td only
                let btnValueTotalQtyML = document.createElement("input");
                btnValueTotalQtyML.setAttribute("type", "number");
                btnValueTotalQtyML.setAttribute("min", 250);
                btnValueTotalQtyML.setAttribute("max", 4500);
                btnValueTotalQtyML.setAttribute("value", slotItem.totalQtyml);
                tdValueTotalQtyML.appendChild(btnValueTotalQtyML);

                // Create textbox for remaining (qty)
                const tdValueRemainingQtyML = newTr.insertCell(); // create td only
                let btnValueRemainingQtyML = document.createElement("input");
                btnValueRemainingQtyML.setAttribute("type", "number");
                btnValueRemainingQtyML.setAttribute("min", 250);
                btnValueRemainingQtyML.setAttribute("max", 4500);
                btnValueRemainingQtyML.setAttribute("value", slotItem.remainingQtyml);
                tdValueRemainingQtyML.appendChild(btnValueRemainingQtyML);

                i++;
            });
    }

    async getHtml() {
        return `
        <table class="pure-table">
            <thead>
                <tr>
                    <th>Slot #</th>
                    <th>Ingredient</th>
                    <th>Total Qty (ml)</th>
                    <th>Used Qty (ml)</th>
                </tr>
            </thead>
            <tbody id="tblBdSlots">
                <!-- Space for table items -->
            </tbody>
        </table>
        <br>
        <button class="pure-button pure-button-primary">Save</button>
        `;
    }
}