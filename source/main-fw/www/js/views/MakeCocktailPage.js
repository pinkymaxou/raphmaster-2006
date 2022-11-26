import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.mCocktails = null;

        this.setTitle("Cocktails");
    }

    async loaded() {
        let idCocktailList = document.querySelector("#idCocktailList");

        const API_GETCOCKTAILS = '/api/getcocktails';

        if (this.mCocktails == null) {
            // Get system informations
            await fetch(API_GETCOCKTAILS)
                .then((response) => response.json())
                .then((data) => this.mCocktails = data)
                .catch((ex) => console.error('getcocktails', ex));
        }
        // myself: That's stupid, why don't you use vueJS ?
        // myself : Because I want to save some space :(
        // <div class="pure-g cocktail_item">
        //     <img src="img/sex-on-the-beach.jpg" />
        //     <div>
        //         <p>Test</p>
        //         <div class="cocktail_ingredient_container">
        //             <div class="colingre_name">Vodka</div>
        //             <div class="colingre_qty">25ml</div>
        //             <div class="colingre_name">Peach Snapps</div>
        //             <div class="colingre_qty">50ml</div>
        //         </div>
        //         <button class="button-normal">Order</button>
        //     </div>
        // </div>   
        this.mCocktails.sort(
            function compareFn(a, b) 
            { 
                return a.name.localeCompare(b.name); 
            });

        this.mCocktails.forEach(
            (cocktail) =>
            {
                let newCocktailItemDIV = document.createElement("div");
                newCocktailItemDIV.classList.add("cocktail_item");

                // Add img
                if (cocktail.img) {
                    let newImg = document.createElement("img");
                    newImg.setAttribute("src", cocktail.img);
                    newCocktailItemDIV.appendChild(newImg);    
                }

                let newTitleP = document.createElement("p");
                newTitleP.appendChild(document.createTextNode(cocktail.name));
                newCocktailItemDIV.appendChild(newTitleP);

                // Ingredient items
                let newIngredientDIV = document.createElement("div");
                
                // Ingredient div
                let newIngredientGridDIV = document.createElement("div");
                newIngredientGridDIV.classList.add("cocktail_ingredient_container");
                
                // Add ingredients
                let ingrIndex = 0;
                cocktail.steps.sort(
                    function compareFn(a, b) 
                    { 
                        if (!a.is_garnish && b.is_garnish)
                            return -1;
                        else if (a.is_garnish && !b.is_garnish)
                            return 1;

                        // Garnish at the end
                        if (getIngredientTypeOrder(a.type) < getIngredientTypeOrder(b.type))
                            return -1;
                        if (getIngredientTypeOrder(a.type) > getIngredientTypeOrder(b.type))
                            return 1;

                        return a.name.localeCompare(b.name); 
                    });
                cocktail.steps.forEach(
                    (stepItem) =>
                    {
                        let newNameDIV = document.createElement("div");
                        newNameDIV.classList.add("colingre_name");
                        let nameText = stepItem.name;
                        if (stepItem.is_garnish) {
                            nameText = "GARNISH: " + nameText;
                        }
                        newNameDIV.appendChild(document.createTextNode(nameText));
                        newIngredientGridDIV.appendChild(newNameDIV);

                        let newQtyDIV = document.createElement("div");
                        newQtyDIV.classList.add("colingre_qty");
                        let qtyText = getPrettyQty(stepItem.qty, stepItem.unit);
                        newQtyDIV.appendChild(document.createTextNode(qtyText));
                        newIngredientGridDIV.appendChild(newQtyDIV);

                        // Add alternate row style
                        if (ingrIndex % 2 == 0) {
                            newNameDIV.classList.add("ingredient_row_odd");
                            newQtyDIV.classList.add("ingredient_row_odd");
                        }
                        ingrIndex++;
                    });
                
                newIngredientDIV.appendChild(newIngredientGridDIV);

                newCocktailItemDIV.appendChild(newIngredientDIV);

                // Controls
                let newControlDIV = document.createElement("div");
                newControlDIV.classList.add("cocktail_control");
                
                let newBtnOrderDrink = document.createElement("button");
                newBtnOrderDrink.appendChild(document.createTextNode("Order"));
                newBtnOrderDrink.classList.add("button-normal");
                newControlDIV.appendChild(newBtnOrderDrink);

                newCocktailItemDIV.appendChild(newControlDIV);

                idCocktailList.appendChild(newCocktailItemDIV);
            });

    }

    async getHtml() {
        return `
        <div id="idCocktailList" class="cocktail_container">
  
        </div>
        `;
    }
}