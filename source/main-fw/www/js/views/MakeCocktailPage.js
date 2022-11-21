import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);

        this.mCocktails = [
            { name: "Sex on the beach", img: "img/sex-on-the-beach.jpg", 
                ingredients: [
                    { name: "Ice", qty: "" },
                    { name: "Vodka", qty: "50ml" },
                    { name: "Peach snapps", qty: "25ml" },
                    { name: "Oranges wedge", qty: "2" },
                    { name: "Cranberry juice", qty: "50ml" },
                    { name: "Grenadine Syrup", qty: "25ml" }
                ] 
            },
            { name: "Bloody Mary", img: "img/bloody-mary.jpg", 
                ingredients: [
                    { name: "Celery salt", qty: "" },
                    { name: "Lemon wedge", qty: "1" },
                    { name: "Lime wedge", qty: "1" },
                    { name: "Vodka", qty: "2 ounces" },
                    { name: "Tomato Juice", qty: "4 ounces" },
                    { name: "Prepared Horseradish", qty: "2 teaspoons" },
                    { name: "Tabasco sauce", qty: "2 dashes" },
                    { name: "Worcestershire sauce", qty: "2 dashes" },
                    { name: "Ground black pepper", qty: "1 pinch" },
                    { name: "Smoked paprika", qty: "1 pinch" },
                    { name: "Garnish: parsley sprig", qty: "1" },
                    { name: "Garnish: green olives", qty: "1" },
                    { name: "Garnish: lime wedge", qty: "1" },
                    { name: "Garnish: celery stalk", qty: "1" }
                ] 
            },
            { name: "Pornstar", img: "img/pornstar.jpg", 
                ingredients: [
                    { name: "Ice", qty: "" },
                    { name: "Curaçao bleu", qty: "1/2 ounce" },
                    { name: "Sour Puss", qty: "1/2 ounce" },
                    { name: "7-up", qty: "" }
                ] 
            },
            { name: "Grasshopper", img: "img/grasshopper.jpg", 
                ingredients: [
                    { name: "Creme de menthe", qty: "3/4 ounce" },
                    { name: "White creme de cacao", qty: "3/4 ounce" },
                    { name: "Heavy cream", qty: "1/4 ounce" },
                    { name: "Ice", qty: "1 cup" }
                ] 
            }
            ];

        this.setTitle("Cocktails");
    }

    async loaded() {
        let idCocktailList = document.querySelector("#idCocktailList");

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
        this.mCocktails.forEach(
            (cocktail) =>
            {
                let newCocktailItemDIV = document.createElement("div");
                newCocktailItemDIV.classList.add("pure-g");
                newCocktailItemDIV.classList.add("cocktail_item");

                // Add img
                let newImg = document.createElement("img");
                newImg.setAttribute("src", cocktail.img);
                newCocktailItemDIV.appendChild(newImg);

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
                cocktail.ingredients.forEach(
                    (ingredientItem) =>
                    {
                        let newNameDIV = document.createElement("div");
                        newNameDIV.classList.add("colingre_name");
                        newNameDIV.appendChild(document.createTextNode(ingredientItem.name));
                        newIngredientGridDIV.appendChild(newNameDIV);

                        let newQtyDIV = document.createElement("div");
                        newQtyDIV.classList.add("colingre_qty");
                        newQtyDIV.appendChild(document.createTextNode(ingredientItem.qty));
                        newIngredientGridDIV.appendChild(newQtyDIV);

                        // Add alternate row style
                        if (ingrIndex % 2 == 0) {
                            newNameDIV.classList.add("colingre_alternate");
                            newQtyDIV.classList.add("colingre_alternate");
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