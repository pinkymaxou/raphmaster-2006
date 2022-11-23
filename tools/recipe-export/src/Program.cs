using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using System.Linq;
using recipe_export.DB;

namespace recipe_export
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                string json = File.ReadAllText("cocktails/cocktails.json");

                dynamic cocktails = JsonConvert.DeserializeObject(json);

                List<Recipe> cocktailRecipes = new List<Recipe>();

                foreach (dynamic cocktail in cocktails)
                {
                    Recipe newCR = new Recipe()
                    {
                        Name = Convert.ToString(cocktail.name),
                        IsCocktail = Convert.ToBoolean(cocktail.is_cocktail)
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
