using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace recipe_export.import
{
    class ImportRecipe
    {
        public string Name = "";
        public string ImgSrc = "";

        public bool IsCocktail = false;

        public List<ImportIngredient> Ingredients = new List<ImportIngredient>();
    }

}
