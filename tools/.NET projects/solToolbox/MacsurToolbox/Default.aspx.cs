using System;

namespace MacsurToolbox
{
   public partial class _Default : System.Web.UI.Page
   {
      protected void Page_Load(object sender,EventArgs e)
      {
         Response.Redirect("~/Menu/01_Home/Home.aspx");
      }
   }
}
