using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace FarmAC.Pages
{
   public partial class FarmAC : System.Web.UI.Page
   {
      private string curUserInit;

      protected void Page_Load(object sender,EventArgs e)
      {

         if(!IsPostBack)
         {
            curUserInit="2";
            ViewState["UserInit"]=curUserInit;
            //controlTabPage.Visible=false;
         }
         else
         {
            curUserInit=ViewState["UserInit"].ToString();
         }
      }
   }
}