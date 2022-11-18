import AbstractView from "./AbstractView.js";

export default class extends AbstractView {
    constructor(params) {
        super(params);
        this.setTitle("Cocktails");
    }

    async loaded() {

    }

    async getHtml() {
        return `
        <div class="pure-g cocktail_item">
            <img src="img/sex-on-the-beach.jpg" />
            <div>
                <p>Sex on the beach</p>
                <ul>
                    <li>ice</li>
                    <li>50ml vodka</li>
                    <li>25ml peach schnapps</li>
                    <li>2 oranges, juiced, plus 2 slices to garnish</li>
                    <li>50ml cranberry juice</li>
                    <li>glacé cherries, to garnish (optional)</li>
                </ul>
            </div>
            <div>
                <button class="pure-button pure-button-primary">Make this</button>
            </div>
        </div>    
        <div class="pure-g cocktail_item">
            <img src="img/bloody-mary.jpg" />
            <div>
                <p>Bloody Mary</p>
                <ul>
                    <li>Celery salt</li>
                    <li>1 lemon wedge</li>
                    <li>1 lime wedge</li>
                    <li>2 ounces vodka</li>
                    <li>4 ounces tomato juice</li>
                    <li>2 teaspoons prepared horseradish</li>
                    <li>2 dashes Tabasco sauce</li>
                    <li>2 dashes Worcestershire sauce</li>
                    <li>1 pinch ground black pepper</li>
                    <li>1 pinch smoked paprika</li>
                    <li>Garnish: parsley sprig</li>
                    <li>Garnish: green olives</li>
                    <li>Garnish: lime wedge</li>
                    <li>Garnish: celery stalk</li>
                </ul>
            </div>
        </div>
        <div class="pure-g cocktail_item">
            <img src="img/pornstar.jpg" />
            <div>
                <p>Pornstar</p>
                <ul>
                    <li>Ice</li>
                    <li>Curaçao bleu</li>
                    <li>Sour Puss</li>
                    <li>7-up</li>
                </ul>
            </div>
        </div>
        `;
    }
}