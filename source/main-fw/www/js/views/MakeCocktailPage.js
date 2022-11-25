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
                newCocktailItemDIV.classList.add("pure-g");
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
                        // Garnish at the end
                        if (a.type == 0 && b.type == 1)
                            return -1;
                        if (a.type == 1 && b.type == 0)
                            return 1;

                        return a.name.localeCompare(b.name); 
                    });
                cocktail.steps.forEach(
                    (stepItem) =>
                    {
                        let newNameDIV = document.createElement("div");
                        newNameDIV.classList.add("colingre_name");
                        let nameText = stepItem.name;
                        if (stepItem.type == 1) {
                            nameText = "GARNISH: " + nameText;
                        }
                        newNameDIV.appendChild(document.createTextNode(nameText));
                        newIngredientGridDIV.appendChild(newNameDIV);

                        let newQtyDIV = document.createElement("div");
                        newQtyDIV.classList.add("colingre_qty");
                        let qtyText = "";
                        if (stepItem.unit == 1) {
                            qtyText = String(stepItem.qty);
                        }
                        else if (stepItem.unit == 2) {
                            qtyText = String(stepItem.qty) + " ml";
                        }
                        else if (stepItem.unit == 3) {
                            qtyText = String(stepItem.qty) + " oz";
                        }
                        else if (stepItem.unit == 4) {
                            qtyText = String(stepItem.qty) + " cup";
                        }
                        else if (stepItem.unit == 6) {
                            qtyText = String(stepItem.qty) + " pinch";
                        }
                        else if (stepItem.unit == 7) {
                            qtyText = String(stepItem.qty) + " wheel";
                        }
                        else if (stepItem.unit == 8) {
                            qtyText = String(stepItem.qty) + " tsp";
                        }
                        else if (stepItem.unit == 9) {
                            qtyText = String(stepItem.qty) + " tbsp";
                        }
                        else if (stepItem.unit == 10) {
                            qtyText = String(stepItem.qty) + " dash";
                        }
                        else if (stepItem.unit == 11) {
                            qtyText = String(stepItem.qty) + " drop";
                        }
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