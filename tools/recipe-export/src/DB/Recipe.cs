using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace recipe_export.DB
{
    class Recipe
    {
        public string Name = "";

        public bool IsCocktail = false;

        public List<Ingredient> Ingredients = new List<Ingredient>();
    }
}
