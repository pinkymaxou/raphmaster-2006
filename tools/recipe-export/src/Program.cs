using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using System.Linq;
using recipe_export.import;
using recipe_export.db;

namespace recipe_export
{
    class Program
    {
        static void Main(string[] args)
        {
            try
            {
                string json = File.ReadAllText("import/cocktails-import.json");

                dynamic cocktails = JsonConvert.DeserializeObject(json);

                List<ImportRecipe> cocktailRecipes = new List<ImportRecipe>();

                foreach (dynamic cocktail in cocktails)
                {
                    ImportRecipe newCR = new ImportRecipe()
                    {
                        Name = Convert.ToString(cocktail.name),
                        IsCocktail = Convert.ToBoolean(cocktail.is_cocktail)
                    };

                    foreach (dynamic ingredient in cocktail.ingredients)
                    {
                        newCR.Ingredients.Add(new ImportIngredient()
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

                string ingredientAlls = String.Join("\r\n", ingreGroups.Select(p => p.Name));

                long t = DateTime.Now.Ticks;


                string dbJsonText = File.ReadAllText("db/database.json");

                DbJSON dbJSON = JsonConvert.DeserializeObject<DbJSON>(dbJsonText);

                // Add unknown ingredients
                foreach (ImportRecipe importRecipe in cocktailRecipes)
                {
                    DbRecipe dbRecipe = dbJSON.Recipes.FirstOrDefault(p => p.name == importRecipe.Name);
                    if (dbRecipe != null)
                        continue;

                    // Match ingredients
                    dbRecipe = new DbRecipe()
                    {
                        guid = Guid.NewGuid(),
                        name = importRecipe.Name,
                        is_cocktail = importRecipe.IsCocktail,
                    };

                    foreach (ImportIngredient importIngredient in importRecipe.Ingredients)
                    {
                        DbIngredient dbIngredient = dbJSON.Ingredients.FirstOrDefault(p => p.name == importIngredient.Name);
                        if (dbIngredient == null)
                            continue;

                        DbRecipeStep dbRecipeStep = new DbRecipeStep()
                        {
                            ingredient_guid = dbIngredient.guid,
                            qty = importIngredient.Qty,
                        };

                        dbRecipe.Steps.Add(dbRecipeStep);
                    }

                    dbJSON.Recipes.Add(dbRecipe);
                }


                using (StreamWriter sw = new StreamWriter("db/database.json", false))
                {
                    sw.WriteLine(JsonConvert.SerializeObject(dbJSON, Formatting.Indented));
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("Exception: " + ex.Message);
            }
        }
    }
}
