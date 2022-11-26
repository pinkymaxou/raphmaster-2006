import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.setTitle("Custom cocktail");
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
        let newTr = idTBodyCustomIngredientList.insertRow();

        // Alternate row
        if (this.mIndex % 2 == 0) {
            newTr.classList.add("custom-table-row-odd");
        }

        // =====================
        const tdIngredient = newTr.insertCell(); // create td only

        // Add select
        let cboSelectIngredient = document.createElement("select");
        cboSelectIngredient.style["width"] = "-webkit-fill-available";
        // Ingredients
        cboSelectIngredient.appendChild(this.addIngredient(0, "--- None ---"));
        this.mIngredients.forEach( (ingredient) => cboSelectIngredient.appendChild(this.addIngredient(ingredient.id, ingredient.name)) );                
        tdIngredient.appendChild(cboSelectIngredient);
        
        // =====================
        const tdValue = newTr.insertCell(); // create td only

        // Add select
        let cboSelectQty = document.createElement("select");
        cboSelectQty.style["width"] = "-webkit-fill-available";
        
        // Quantities
        cboSelectQty.appendChild(this.addQty(0, " --- "));                
        for(let oz = 0.5; oz <= 3; oz += 0.5) {
            cboSelectQty.appendChild(this.addQty(oz, oz +" oz"));
        }
        tdValue.appendChild(cboSelectQty);

        // =====================
        const tdControl = newTr.insertCell(); // create td only
        let ctlContainerDIV = document.createElement("div");

        let btDeleteOrder = document.createElement("button");
        btDeleteOrder.classList.add("button-normal");
        btDeleteOrder.appendChild(document.createTextNode("-"));
        ctlContainerDIV.appendChild(btDeleteOrder);

        let btAddOrder = document.createElement("button");
        btAddOrder.classList.add("button-normal");
        btAddOrder.appendChild(document.createTextNode("+"));
        ctlContainerDIV.appendChild(btAddOrder);

        tdControl.appendChild(ctlContainerDIV);

        this.mIndex++;
    }

    async loaded() {

        const API_GETINGREDIENTS = '/api/getingredients/liquids';

        this.mIngredients = [];
        this.mIndex = 0;

        // Get system informations
        await fetch(API_GETINGREDIENTS)
            .then((response) => response.json())
            .then((data) => this.mIngredients = data)
            .catch((ex) => console.error('getingredients', ex));

        let idTBodyCustomIngredientList = document.querySelector("#idTBodyCustomIngredientList");

        this.mOrderList = [{ "name" : "test" }];

        this.mOrderList.forEach(
            (orderListItem) =>
            {
                this.addOrderItem(orderListItem);
            });

    }

    async getHtml() {
        return `
        <table class="custom-table">
            <tbody id="idTBodyCustomIngredientList">
                <!-- Space for table items -->
            </tbody>
        </table>
        <button class="button-normal">Make</button>
        `;
    }
}