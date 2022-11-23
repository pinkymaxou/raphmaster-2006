using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using System.Linq;

namespace recipe_export
{
    class Program
    {
        class Ingredient
        {
            public string Name = "";
            public string Qty = "";
        }

        class CocktailRecipe
        {
            public string Name = "";

            public List<Ingredient> Ingredients = new List<Ingredient>();
        }

        static void Main(string[] args)
        {
            try
            {
                string json = File.ReadAllText("cocktails/cocktails.json");

                dynamic cocktails = JsonConvert.DeserializeObject(json);

                List<CocktailRecipe> cocktailRecipes = new List<CocktailRecipe>();

                foreach (dynamic cocktail in cocktails)
                {
                    CocktailRecipe newCR = new CocktailRecipe()
                    {
                        Name = Convert.ToString(cocktail.name),
                    };

                    foreach (dynamic ingredient in cocktail.ingredients)
                    {
                        newCR.Ingredients.Add(new Ingredient()
                        {
                            Name = ingredient.name,
                            Qty = ingredient.qty
                        });
                    }

                    cocktailRecipes.Add(newCR);
                }

                // Group by ingredients
                var ingreGroups = cocktailRecipes
                    .SelectMany(p => p.Ingredients)
                    .GroupBy(p => p.Name)
                    .OrderBy(p => p.Key)
                    .Select(p => new { Name = p.Key, Count = p.Count() })
                    .ToArray();


            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("Exception: " + ex.Message);
            }
        }
    }
}
