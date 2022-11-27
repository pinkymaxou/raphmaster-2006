using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace recipe_export.import
{
    class ImportIngredient
    {
        public string Name = "";
        public string Qty = "";
        public string UPCCode = "";
        public Cocktaildb.EIngredientType Type { get; set; } = Cocktaildb.EIngredientType.Unspecified;
        public bool IsGarnish = false;

        public string[] Categories { get; set; } = new string[0];
    }

}
