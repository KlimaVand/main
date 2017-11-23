using System;
using System.Web;
using System.Web.Security;
using Telerik.Web.UI;

namespace AgroModelToolbox.Controls._99_Admin
{
   public partial class ctrlRole : System.Web.UI.UserControl
   {
      private string aApplicationID;
      private string strSelectCommandRole;
      private string strSelectCommandUser;
      private string selectRoleName;

      public string ApplicationID { set { aApplicationID=value; } }

      protected void Page_Load(object sender,EventArgs e)
      {
         LabelMessage.Text="";

         strSelectCommandRole="SELECT ApplicationId, RoleId, RoleName, LoweredRoleName";
         strSelectCommandRole+=" FROM dbo.aspnet_Roles";
         strSelectCommandRole+=" WHERE (ApplicationId = '"+aApplicationID+"')";
         strSelectCommandRole+=" ORDER BY RoleName";

         strSelectCommandUser="SELECT UserAdditional.UserInit, UserAdditional.FirstName + ' ' + UserAdditional.LastName + ' [' + AgroModel.dbo.Country.CountryName + ']' AS FullName, aspnet_Users.UserId, aspnet_Users.ApplicationId";
         strSelectCommandUser+=" FROM UserAdditional";
         strSelectCommandUser+=" INNER JOIN AgroModel.dbo.Country ON UserAdditional.CountryID = AgroModel.dbo.Country.CountryID";
         strSelectCommandUser+=" INNER JOIN aspnet_Users ON UserAdditional.UserID = aspnet_Users.UserId";
         strSelectCommandUser+=" WHERE (aspnet_Users.ApplicationId = '"+aApplicationID+"')";
         strSelectCommandUser+=" ORDER BY FullName";

         if(!IsPostBack) // Initialising
         {
            SqlDataSourceRole.SelectCommand=strSelectCommandRole;
            SqlDataSourceUser.SelectCommand=strSelectCommandUser;
            RadGridRole.DataBind();
            GridDataItem item=(GridDataItem)RadGridRole.SelectedItems[0];
            selectRoleName=item["RoleName"].Text;
            ViewState["RoleName"]=selectRoleName;
         }
         else
         {
            selectRoleName=ViewState["RoleName"].ToString();
         }

         RadWindowNewRole.OpenerElementID=RadButtonNewRole.ClientID;
         RadWindowAddUser.OpenerElementID=RadButtonAddUser.ClientID;
      }

      protected void RadGridRole_DataBound(object sender,EventArgs e)
      {
         if(RadGridRole.MasterTableView.Items.Count>0)
            RadGridRole.MasterTableView.Items[0].Selected=true;
      }

      protected void RadGridRole_SelectedIndexChanged(object sender,EventArgs e)
      {
         GridDataItem item=(GridDataItem)RadGridRole.SelectedItems[0];
         selectRoleName=item["RoleName"].Text;
         ViewState["RoleName"]=selectRoleName;
      }

      protected void RadButtonSaveNewRole_Click(object sender,EventArgs e)
      {
         string newRoleName=RadTextBoxRoleNameNew.Text.Trim();
         if(!Roles.RoleExists(newRoleName))
         {
            Roles.CreateRole(newRoleName);
         }
         RadTextBoxRoleNameNew.Text=string.Empty;
         SqlDataSourceRole.SelectCommand=strSelectCommandRole;
         RadGridRole.DataBind();
      }

      protected void RadGridRole_DeleteCommand(object sender,Telerik.Web.UI.GridCommandEventArgs e)
      {
         string strRoleName=this.RadGridRole.MasterTableView.Items[e.Item.ItemIndex].Cells[4].Text;
         Roles.DeleteRole(strRoleName);
         SqlDataSourceRole.SelectCommand=strSelectCommandRole;
         RadGridRole.DataBind();
      }

      protected void RadGridRoleUser_DeleteCommand(object sender,Telerik.Web.UI.GridCommandEventArgs e)
      {
         GridDataItem item=(GridDataItem)e.Item;
         string strUserName=item["UserName"].Text;
         Roles.RemoveUserFromRole(strUserName,selectRoleName);
      }

      protected void RadButtonSaveAddUser_Click(object sender,EventArgs e)
      {
         try
         {
            string strUserName=RadComboBoxUserNew.SelectedValue.ToString();
            Roles.AddUserToRole(strUserName,selectRoleName);
            RadGridRoleUser.DataBind();
         }
         catch(System.Configuration.Provider.ProviderException ex)
         {
            LabelMessage.Text=ex.Message;
         }
         catch(HttpException ex)
         {
            LabelMessage.Text=ex.Message;
         }
      }
   }
}