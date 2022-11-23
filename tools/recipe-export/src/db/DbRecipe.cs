using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace recipe_export.db
{
    public class DbRecipe
    {
        public Guid guid { get; set; }

        public string name = "";

        public bool is_cocktail = false;

        public List<DbIngredient> ingredients = new List<DbIngredient>();
    }

}
