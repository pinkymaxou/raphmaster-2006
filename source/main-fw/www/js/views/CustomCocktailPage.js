import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.setTitle("Custom cocktail");

        this.mIngredients = [
            { slotid: 1, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid: 2, name: "Clamato", remainqty: 1500, totalqty: 2000 },
            { slotid: 3, name: "Grenadine", remainqty: 1500, totalqty: 2000 },
            { slotid: 4, name: "White rhum", remainqty: 1500, totalqty: 2000 },
            { slotid: 5, name: "Brown rhum", remainqty: 1500, totalqty: 2000 },
            { slotid: 6, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid: 7, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid: 8, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid: 9, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid:10, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid:11, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid:12, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid:13, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid:14, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid:15, name: "Vokda", remainqty: 1500, totalqty: 2000 },
            { slotid:16, name: "Vokda", remainqty: 1500, totalqty: 2000 }
        ];
    }

    async loaded() {

        // <div class="customingredient_item_name">Vokda</div>
        // <div class="customingredient_item_qty_container">
        //     <select>
        //         <option>None</option>
        //         <option>1&nbsp;ounces</option>
        //         <option>1.5&nbsp;ounces</option>
        //         <option>2&nbsp;ounces</option>
        //         <option>3&nbsp;ounces</option>
        //     </select>
        // </div>
        let idCustomIngredientList = document.querySelector("#idCustomIngredientList");

        this.mIngredients.forEach(
            (ingredient) =>
            {
                let newIngredientItemDIV = document.createElement("div");
                newIngredientItemDIV.classList.add("customingredient_item_name");
                newIngredientItemDIV.appendChild(document.createTextNode(ingredient.name));

                let newIngredientItemQtyContainerDIV = document.createElement("div");
                newIngredientItemQtyContainerDIV.classList.add("customingredient_item_qty_container");

                let newSelect = document.createElement("select");

                for(let once = 0; once <= 3; once += 0.5) {
                    let newOption = document.createElement("option");                   
                    let text = document.createTextNode(once > 0 ? (once + " oz") : "---");

                    newOption.appendChild(text);
                    newSelect.appendChild(newOption);
                }

                newIngredientItemQtyContainerDIV.appendChild(newSelect);

                idCustomIngredientList.appendChild(newIngredientItemDIV);
                idCustomIngredientList.appendChild(newIngredientItemQtyContainerDIV);
            });

    }

    async getHtml() {
        return `
        <div id="idCustomIngredientList" class="customingredient_container">
            <!-- Items -->
        </div>
        `;
    }
}