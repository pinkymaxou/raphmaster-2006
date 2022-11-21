import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.setTitle("Custom cocktail");

        this.mSlots = [
            { slotid: 1, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid: 2, name: "Clamato", remainqty: 1500, totalqty: 2000 },
            { slotid: 3, name: "Grenadine", remainqty: 1500, totalqty: 2000 },
            { slotid: 4, name: "White rhum", remainqty: 1500, totalqty: 2000 },
            { slotid: 5, name: "Brown rhum", remainqty: 1500, totalqty: 2000 },
            { slotid: 6, name: "Whiskey Sour", remainqty: 1500, totalqty: 2000 },
            { slotid: 7, name: "Dry Martini", remainqty: 1500, totalqty: 2000 },
            { slotid: 8, name: "Margarita", remainqty: 1500, totalqty: 2000 },
            { slotid: 9, name: "Sour Puss", remainqty: 1500, totalqty: 2000 },
            { slotid:10, name: "Gin", remainqty: 1500, totalqty: 2000 },
            { slotid:11, name: "Bayleys", remainqty: 1500, totalqty: 2000 },
            { slotid:12, name: "Cosmopolitan", remainqty: 1500, totalqty: 2000 },
            { slotid:13, name: "Peach schnapps ", remainqty: 1500, totalqty: 2000 },
            { slotid:14, name: "White mint cream", remainqty: 1500, totalqty: 2000 },
            { slotid:15, name: "Tequila", remainqty: 1500, totalqty: 2000 },
            { slotid:16, name: "Tia Maria", remainqty: 1500, totalqty: 2000 }
        ];
    }

    addQty(id, name) {
        let cboNewIngredientOpt = document.createElement("option");
        cboNewIngredientOpt.setAttribute("value", id);
        cboNewIngredientOpt.text = name;
        return cboNewIngredientOpt;
    }

    async loaded() {

        let idTBodyCustomIngredientList = document.querySelector("#idTBodyCustomIngredientList");

        let i = 0;
        this.mSlots.forEach(
            (slotItem) =>
            {
                let newTr = idTBodyCustomIngredientList.insertRow();

                // Alternate row
                if (i % 2 == 0) {
                    newTr.classList.add("custom-table-row-odd");
                }

                // =====================
                const tdSlot = newTr.insertCell(); // create td only
                tdSlot.appendChild(document.createTextNode(slotItem.slotid));
                
                // =====================
                const tdIngredient = newTr.insertCell(); // create td only
                tdIngredient.appendChild(document.createTextNode(slotItem.name));
                
                // =====================
                const tdValue = newTr.insertCell(); // create td only

                // Add select
                let cboSelectQty = document.createElement("select");
                cboSelectQty.setAttribute("width", "-webkit-fill-available");
                
                // Ingredients
                cboSelectQty.appendChild(this.addQty(0, " --- "));                
                for(let oz = 0.5; oz <= 3; oz += 0.5) {
                    cboSelectQty.appendChild(this.addQty(oz, oz +" oz"));
                }
                tdValue.appendChild(cboSelectQty);

                i++;
            });

    }

    async getHtml() {
        return `
        <table class="custom-table">
            <tbody id="idTBodyCustomIngredientList">
                <!-- Space for table items -->
            </tbody>
        </table>
        <br>
        <button class="pure-button pure-button-primary">Make</button>
        `;
    }
}