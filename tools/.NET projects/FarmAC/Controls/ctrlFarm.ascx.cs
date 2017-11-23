using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{
   public partial class ctrlFarm : System.Web.UI.UserControl
   {
      protected void Page_Load(object sender,EventArgs e)
      {

      }

      protected void RadGridFarm_DataBound(object sender,EventArgs e)
      {
         if(RadGridFarm.MasterTableView.Items.Count>0)
            RadGridFarm.MasterTableView.Items[0].Selected=true;
      }
   }
}