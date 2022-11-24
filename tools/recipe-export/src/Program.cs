using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using System.Linq;
using recipe_export.import;
using Google.Protobuf;

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
                        IsCocktail = Convert.ToBoolean(cocktail.is_cocktail),
                        ImgSrc = Convert.ToString(cocktail.img)
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
                var allIngredientGroups = cocktailRecipes
                    .SelectMany(p => p.Ingredients)
                    .GroupBy(p => p.Name)
                    .OrderBy(p => p.Key)
                    .Select(p => new { Name = p.Key, Count = p.Count() })
                    .ToArray();
                //string ingredientAlls = String.Join("\r\n", ingreGroups.Select(p => p.Name));
                long t = DateTime.Now.Ticks;

                Cocktaildb.IngredientFile dbIngredientFile = new Cocktaildb.IngredientFile();

                using (FileStream fs = new FileStream("ingredients.bin", FileMode.OpenOrCreate, FileAccess.ReadWrite))
                {
                    fs.SetLength(0);

                    uint ingredientID = 1;

                    foreach (var ingreGroup in allIngredientGroups)
                    {
                        dbIngredientFile.Entries.Add(new Cocktaildb.Ingredient()
                        {
                            Id = ingredientID,
                            Name = ingreGroup.Name,
                        });

                        ingredientID++;
                    }

                    byte[] arrays = dbIngredientFile.ToByteArray();
                    fs.Write(arrays, 0, arrays.Length);
                }

                Cocktaildb.RecipeFile recipeFile = new Cocktaildb.RecipeFile();

                using (FileStream fs = new FileStream("recipes.bin", FileMode.OpenOrCreate, FileAccess.ReadWrite))
                {
                    fs.SetLength(0);

                    uint recipeStartID = 100000;
                    //uint

                    foreach (ImportRecipe importRecipe in cocktailRecipes)
                    {
                        Cocktaildb.Recipe newRecipe = new Cocktaildb.Recipe()
                        {
                            ID = recipeStartID++,
                            Name = importRecipe.Name,
                            Imgfile = importRecipe.ImgSrc,
                            IsCocktail = importRecipe.IsCocktail,
                        };

                        foreach (ImportIngredient importIngredient in importRecipe.Ingredients)
                        {
                            Cocktaildb.Ingredient dbIngredient = dbIngredientFile.Entries
                                .FirstOrDefault(p => p.Name == importIngredient.Name);

                            newRecipe.RecipeSteps.Add(new Cocktaildb.RecipeStep()
                            {
                                IngredientId = dbIngredient.Id,
                                Qty = new Cocktaildb.Qty()
                                {
                                    Value = 1,
                                    Type = Cocktaildb.EQtyType.Pinch
                                }
                            });
                        }

                        recipeFile.Entries.Add(newRecipe);
                    }

                    byte[] arrays = recipeFile.ToByteArray();
                    fs.Write(arrays, 0, arrays.Length);
                }
            }
            catch (Exception ex)
            {
                Console.Error.WriteLine("Exception: " + ex.Message);
            }
        }
    }
}
