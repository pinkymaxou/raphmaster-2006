using System;
using System.Collections.Generic;
using System.IO;
using Newtonsoft.Json;
using System.Linq;
using recipe_export.import;
using Google.Protobuf;
using System.Text.RegularExpressions;

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

                var allQtyGroups = cocktailRecipes
                    .SelectMany(p => p.Ingredients)
                    .GroupBy(p => p.Qty)
                    .OrderBy(p => p.Key)
                    .Select(p => new { Name = p.Key })
                    .ToArray();

                string allQtys = String.Join("\r\n", allQtyGroups.Select(p => p.Name));


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


                            Regex regex = new Regex(@"^(?<scal>((\d+\s+\d+\/\d+)|(\d+\/\d+)|(\d+)))\s*(?<unit>(oz|dash|cup|pinch|tsp|ml))?", RegexOptions.ExplicitCapture);

                            Cocktaildb.Qty newQty = new Cocktaildb.Qty()
                            {
                                Value = 0,
                                Type = Cocktaildb.EQtyType.None
                            };

                            if (!String.IsNullOrEmpty(importIngredient.Qty))
                            {
                                Match m = regex.Match(importIngredient.Qty);
                                if (!m.Success)
                                    throw new Exception("Fuck you");

                                string valueText = m.Groups["scal"].Value.Trim();
                                if (!float.TryParse(valueText, out float value))
                                {
                                    Regex fraction = new Regex(@"^((((?<int>\d+)\s(?<a>\d+)\/(?<b>\d+)))|(?<a>\d+)\/(?<b>\d+)|(?<int>\d+))", RegexOptions.ExplicitCapture);
                                    Match mFrac = fraction.Match(valueText);
                                    if (mFrac.Success)
                                    {
                                        string intText = mFrac.Groups["int"].Value;
                                        int intPart = 0;
                                        int.TryParse(intText, out intPart);

                                        value = intPart;
                                        if (int.TryParse(mFrac.Groups["a"].Value, out int intPartA) &&
                                            int.TryParse(mFrac.Groups["b"].Value, out int intPartB))
                                        {
                                            value += (float)intPartA / (float)intPartB;
                                        }
                                    }
                                    else
                                        throw new Exception("Invalid scal");
                                }

                                newQty.Value = value;

                                string unitText = m.Groups["unit"].Value.Trim();
                                if (unitText == "ml")
                                {
                                    newQty.Type = Cocktaildb.EQtyType.LiquidMl;
                                }
                                else if (unitText == "oz")
                                {
                                    newQty.Type = Cocktaildb.EQtyType.Oz;
                                }
                                else if (unitText == "tsp")
                                {
                                    newQty.Type = Cocktaildb.EQtyType.Teaspoon;
                                }
                                else if (unitText == "tbsp")
                                {
                                    newQty.Type = Cocktaildb.EQtyType.Tablespoon;
                                }
                                else if (unitText == "dash")
                                {
                                    newQty.Type = Cocktaildb.EQtyType.Dash;
                                }
                                else if (unitText == "pinch")
                                {
                                    newQty.Type = Cocktaildb.EQtyType.Pinch;
                                }
                                else if (unitText == "cup")
                                {
                                    newQty.Type = Cocktaildb.EQtyType.Cup;
                                }
                                else if (unitText == "")
                                {
                                    newQty.Value = 0;
                                    newQty.Type = Cocktaildb.EQtyType.None;
                                }
                                else
                                    throw new Exception("Unknown unit");
                            }

                            newRecipe.RecipeSteps.Add(new Cocktaildb.RecipeStep()
                            {
                                IngredientId = dbIngredient.Id,
                                Qty = newQty
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
