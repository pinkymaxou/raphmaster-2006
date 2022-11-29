import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.mColumnCount = 3;
        this.setTitle("Custom cocktail");

        console.log("test", params);
    }

    addQty(id, name) {
        let cboNewIngredientOpt = document.createElement("option");
        cboNewIngredientOpt.setAttribute("value", id);
        cboNewIngredientOpt.text = name;
        return cboNewIngredientOpt;
    }

    addIngredient(id, name) {
        let cboNewIngredientOpt = document.createElement("option");
        cboNewIngredientOpt.setAttribute("value", id);
        cboNewIngredientOpt.text = name;
        return cboNewIngredientOpt;
    }

    addOrderItem(orderListItem) {

        let idDivOrderList = document.querySelector("#idDivOrderList");

        // Alternate row
       /* if (this.mIndex % 2 == 0) {
            newTr.classList.add("custom-cocktail-grid-row-odd");
        }*/

        // =====================
        const tdIngredientDIV = document.createElement("div"); // create td only

        // Add select
        let cboSelectIngredient = document.createElement("select");
        cboSelectIngredient.setAttribute("id", "idSelectIngredient");
        cboSelectIngredient.style["width"] = "-webkit-fill-available";
        cboSelectIngredient.classList.add("custom-cocktail-grid-center-select");
        // Ingredients
        cboSelectIngredient.appendChild(this.addIngredient(0, "--- None ---"));
        this.mIngredients.forEach( (ingredient) => cboSelectIngredient.appendChild(this.addIngredient(ingredient.ingredient_id, ingredient.name)) );
        cboSelectIngredient.value = orderListItem.ingredient_id;
        tdIngredientDIV.appendChild(cboSelectIngredient);
        idDivOrderList.appendChild(tdIngredientDIV);

        // =====================
        const tdValueDIV = document.createElement("div");

        // Add select
        let cboSelectQty = document.createElement("select");
        cboSelectQty.setAttribute("id", "idSelectQty");
        cboSelectQty.style["width"] = "-webkit-fill-available";
        cboSelectQty.classList.add("custom-cocktail-grid-center-select");

        // Quantities
        cboSelectQty.appendChild(this.addQty(0, " --- "));
        for(let oz = 0.5; oz <= 8; oz += 0.5) {
            cboSelectQty.appendChild(this.addQty(oz, getPrettyFraction(oz) +" oz"));
        }
        tdValueDIV.appendChild(cboSelectQty);
        idDivOrderList.appendChild(tdValueDIV);

        // =====================
        const tdControlDIV = document.createElement("div");
        tdControlDIV.style["width"] = "max-content";

        let btDeleteOrder = document.createElement("button");
        btDeleteOrder.classList.add("button-cancel");
        btDeleteOrder.appendChild(document.createTextNode("-"));
        btDeleteOrder.addEventListener('click', function() {

            idDivOrderList.removeChild(tdIngredientDIV);
            idDivOrderList.removeChild(tdValueDIV);
            idDivOrderList.removeChild(tdControlDIV);
        });

        tdControlDIV.appendChild(btDeleteOrder);
        idDivOrderList.appendChild(tdControlDIV);

        this.mIndex++;
    }

    async loaded() {

        const API_GETINGREDIENTS = '/api/getavailableingredients';

        this.mIngredients = [];
        this.mIndex = 0;

        // Get system informations
        await fetch(API_GETINGREDIENTS)
            .then((response) => response.json())
            .then((data) => this.mIngredients = data)
            .catch((ex) => console.error('getavailableingredients', ex));
        this.mIngredients.sort((a, b) => a.name.localeCompare(b.name));

        this.mOrderList = [{ "ingredient_id" : 64 }, { "ingredient_id" : 29 }];

        this.mOrderList.forEach(
            (orderListItem) =>
            {
                this.addOrderItem(orderListItem);
            });

        let targetThis = this;

        // Bind buttons
        let idBtAddOrder = document.querySelector("#idBtAddOrder");
        idBtAddOrder.addEventListener('click', function() {
            targetThis.addOrderItem(null);
        });

        // Bind buttons
        let idBtOK = document.querySelector("#idBtOK");
        idBtOK.addEventListener('click', function() {
            // Get ordered item list
            let idDivOrderList = document.querySelector("#idDivOrderList");

            const matches = idDivOrderList.querySelectorAll("div");

            for(let i = 0; i < matches.length; i += targetThis.mColumnCount)
            {
                let idSelectIngredient = matches[i].querySelector("#idSelectIngredient");
                let idSelectQty = matches[i+1].querySelector("#idSelectQty");

                console.log("idSelectIngredient: ", idSelectIngredient.value, " idSelectQty: ", idSelectQty.value);
            }
        });
    }

    async getHtml() {
        return `
        <div>
            <div id="idDivOrderList" class="custom-cocktail-grid">
                <!-- Lines -->
            </div>

            <div class="button-bar">
                <button id="idBtAddOrder" class="button-normal button-bar-item">Add order</button>
            </div>

            <div class="button-bar">
                <button id="idBtOK" class="button-normal button-bar-item">MAKE COCKTAIL</button>
            </div>
        </div>
        `;
    }
}