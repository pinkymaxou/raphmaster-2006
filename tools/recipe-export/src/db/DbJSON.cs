using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace recipe_export.db
{
    public class DbJSON
    {
        public List<DbIngredient> Ingredients { get; set; } = new List<DbIngredient>();
        public List<DbRecipe> Recipes { get; set; } = new List<DbRecipe>();
    }
}
