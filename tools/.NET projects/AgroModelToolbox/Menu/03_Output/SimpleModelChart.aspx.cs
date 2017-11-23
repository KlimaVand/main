using System;
using System.Web;
using AgroModelToolbox.Class;

namespace AgroModelToolbox.Menu._03_Output
{
   public partial class SimpleModelChart : System.Web.UI.Page
   {
      private string con;

      protected void Page_Load(object sender,EventArgs e)
      {
         LabelStatus.Text=Utility.DevelopmentStatus(0);
         con=Utility.GetConnectionString("conAgroModel");

         if(!IsPostBack)
         {
            LabelStatus.Text+=Utility.LogUser(con,"SimpleModelChart",HttpContext.Current.Request.UserHostAddress,HttpContext.Current.Request.UserAgent,false);
         }
      }
   }
}