using System;
using MacsurToolbox.Class;

namespace MacsurToolbox.Menu._99_Admin
{
   public partial class AdminUser : System.Web.UI.Page
   {
      protected void Page_Load(object sender,EventArgs e)
      {
         controlUser.ApplicationID=Utility.ApplicationID;
         controlRole.ApplicationID=Utility.ApplicationID;
      }
   }
}