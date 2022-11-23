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
                    { name: "Celery salt", qty: "1" },
                    { name: "Lemon wedge", qty: "1" },
                    { name: "Lime wedge", qty: "1" },
                    { name: "Vodka", qty: "2 oz" },
                    { name: "Tomato Juice", qty: "4 oz" },
                    { name: "Prepared Horseradish", qty: "2 tsp" },
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
                    { name: "Curaçao bleu", qty: "1/2 oz" },
                    { name: "Sour Puss", qty: "1/2 oz" },
                    { name: "7-up", qty: "" }
                ] 
            },
            { name: "Grasshopper", img: "img/grasshopper.jpg", 
                ingredients: [
                    { name: "Creme de menthe", qty: "3/4 oz" },
                    { name: "White creme de cacao", qty: "3/4 oz" },
                    { name: "Heavy cream", qty: "1/4 oz" },
                    { name: "Ice", qty: "1 cup" }
                ] 
            },
            { name: "Skittle", img: "img/skittle.jpg", 
                ingredients: [
                    { name: "Vodka", qty: "1 oz" },
                    { name: "Sweet and sour mix", qty: "1 oz" },
                    { name: "Melon liqueur", qty: "1 oz" },
                    { name: "Southern Comfort", qty: "1 oz" },
                    { name: "Pineapple juice", qty: "1 oz" },
                ]
            },
            { name: "Aperol Spritz", img: "img/aperol_spritz.jpg", 
                ingredients: [
                    { name: "Sparkling Wine", qty: "3 oz" },
                    { name: "Aperol", qty: "2 oz" },
                    { name: "Soda club", qty: "3/4 oz" },
                    { name: "Ice", qty: "1" },
                    { name: "Orange wedge", qty: "1" },
                ]
            },
            { name: "Martini", img: "img/martini.jpg", 
                ingredients: [
                    { name: "Gin", qty: "2 1/2 oz" },
                    { name: "Dry vermouth", qty: "1/2 oz" },
                    { name: "Orange", qty: "1 dash" },
                    { name: "Garnish: Lemon twist", qty: "" },
                ]
            },
            { name: "Daiquiri", img: "img/daiquiri.jpg", 
                ingredients: [
                    { name: "White rhum", qty: "2 oz" },
                    { name: "Lime juice", qty: "1 oz" },
                    { name: "Demerara sugar syrup", qty: "3/4 oz" },
                    { name: "Garnish: Lime twist", qty: "" },
                ]
            },
            { name: "Margarita", img: "img/margarita.jpg", 
                ingredients: [
                    { name: "Blanco tequila", qty: "2 oz" },
                    { name: "Orange liquor", qty: "1/2 oz" },
                    { name: "Lime juice", qty: "1 oz" },
                    { name: "Agave syrup", qty: "1/2 oz" },
                    { name: "Garnish: Lime wheel", qty: "" },
                    { name: "Garnish: Kosher salt", qty: "" },
                ]
            },
            { name: "Tom Collins", img: "img/tom-collins.jpg", 
                ingredients: [
                    { name: "Londo dry gin", qty: "2 oz" },
                    { name: "Lemon juice", qty: "1 oz" },
                    { name: "Syrup", qty: "1/2 oz" },
                    { name: "Club soda", qty: "" },
                    { name: "Garnish: Lemon wheel", qty: "" },
                    { name: "Garnish: Maraschino cherry", qty: "" },
                ]
            },
            { name: "Amaretto Sour", img: "img/amaretto-sour.jpg", 
                ingredients: [
                    { name: "Amaretto liquor", qty: "1 1/2 oz" },
                    { name: "Cask-proof bourbon", qty: "3/4 oz" },
                    { name: "Lemon juice", qty: "1 oz" },
                    { name: "Rich simple syrup", qty: "1 tsp" },
                    { name: "Egg white", qty: "1/2 oz" },
                    { name: "Garnish: Lemon twist", qty: "" },
                    { name: "Garnish: 2 brandied cherries", qty: "" },
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