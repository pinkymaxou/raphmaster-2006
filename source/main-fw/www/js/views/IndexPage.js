import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("Cocktails");
    }

    async loaded() {
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
                    { name: "Garnish: parsley sprig", qty: "" },
                    { name: "Garnish: green olives", qty: "" },
                    { name: "Garnish: lime wedge", qty: "1" },
                    { name: "Garnish: celery stalk", qty: "" }
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
            ];

        let idCocktailList = document.querySelector("#idCocktailList");

        // myself: That's stupid, why don't you use vueJS ?
        // myself : Because I want to save some space :(
        // <div class="pure-g cocktail_item">
        //     <img src="img/sex-on-the-beach.jpg" />
        //     <div>
        //         <p>Sex on the beach</p>
        //         <ul>
        //             <li>ice</li>
        //             <li>50ml vodka</li>
        //         </ul>
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

                // Ingredient items
                let newIngredientDIV = document.createElement("div");
                
                let newTitleP = document.createElement("p");
                newTitleP.appendChild(document.createTextNode(cocktail.name));
                newIngredientDIV.appendChild(newTitleP);

                // OL
                let newOL = document.createElement("ol");
                
                // Add ingredients
                cocktail.ingredients.forEach(
                    (ingredientItem) =>
                    {
                        let newLI = document.createElement("li");
                        // ingredientItem
                        newLI.appendChild(document.createTextNode(ingredientItem.name));
                        newOL.appendChild(newLI);
                    });
                
                newIngredientDIV.appendChild(newOL);

                newCocktailItemDIV.appendChild(newIngredientDIV);

                idCocktailList.appendChild(newCocktailItemDIV);
            });

    }

    async getHtml() {
        return `
        <div id="idCocktailList" class="cocktail_container">
            <div class="pure-g cocktail_item">
                <img src="img/sex-on-the-beach.jpg" />
                <div>
                    <p>Test</p>
                    <div class="cocktail_ingredient_container">
                        <div class="colingre_name">Vodka</div>
                        <div class="colingre_qty">25ml</div>
                        <div class="colingre_name">Peach Snapps</div>
                        <div class="colingre_qty">50ml</div>
                    </div>
                </div>
            </div>    
        </div>
        `;
    }
}