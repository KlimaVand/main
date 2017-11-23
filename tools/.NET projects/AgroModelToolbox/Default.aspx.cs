using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace AgroModelToolbox
{
   public partial class _Default : System.Web.UI.Page
   {
      protected void Page_Load(object sender,EventArgs e)
      {
         Response.Redirect("~/Menu/01_Home/Home.aspx");
      }
   }
}
