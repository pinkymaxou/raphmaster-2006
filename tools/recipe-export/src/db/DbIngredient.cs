using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace recipe_export.db
{
    public class DbIngredient
    {
        public Guid guid { get; set; }

        public string name = "";
        public string qty = "";

        public Guid? ingredient_guid { get; set; } = null;
    }

}
