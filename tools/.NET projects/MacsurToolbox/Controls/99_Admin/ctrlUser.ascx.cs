using System;
using System.Data;
using System.Data.SqlClient;
using System.Web;
using System.Web.Security;
using MacsurToolbox.Class;
using Telerik.Web.UI;

namespace MacsurToolbox.Controls._99_Admin
{
   public partial class ctrlUser : System.Web.UI.UserControl
   {
      private string con;
      private string aApplicationID;
      private string strSelectCommand;
      private string selectUserName;

      public string ApplicationID { set { aApplicationID=value; } }

      protected void Page_Load(object sender,EventArgs e)
      {
         con=Utility.GetConnectionString("ApplicationServices");
         LabelMessage.Text="";

         strSelectCommand="SELECT dbo.aspnet_Users.UserId, dbo.aspnet_Users.UserName, dbo.UserAdditional.Active, dbo.UserAdditional.FirstName + ' ' + dbo.UserAdditional.LastName + ' [' +  Macsur.dbo.Country.CountryName + ']' AS FullName, dbo.aspnet_Membership.Email, dbo.aspnet_Membership.IsApproved, dbo.aspnet_Membership.IsLockedOut, dbo.aspnet_Membership.CreateDate, dbo.aspnet_Users.LastActivityDate";
         strSelectCommand+=" FROM dbo.aspnet_Users";
         strSelectCommand+=" INNER JOIN dbo.aspnet_Membership ON dbo.aspnet_Users.UserId = dbo.aspnet_Membership.UserId";
         strSelectCommand+=" INNER JOIN dbo.UserAdditional ON dbo.aspnet_Users.UserId = dbo.UserAdditional.UserID";
         strSelectCommand+=" INNER JOIN  Macsur.dbo.Country ON UserAdditional.CountryID =  Macsur.dbo.Country.CountryID";
         strSelectCommand+=" WHERE (dbo.aspnet_Users.ApplicationId = '"+aApplicationID+"')";
         strSelectCommand+=" ORDER BY FullName";

         if(!IsPostBack) // Initialising
         {
            SqlDataSourceUserMembership.SelectCommand=strSelectCommand;
            RadGridUserMembership.DataBind();
            GridDataItem item=(GridDataItem)RadGridUserMembership.SelectedItems[0];
            selectUserName=item["UserName"].Text;
            ViewState["UserName"]=selectUserName;
            LabelNewPassword.Text="New password for "+GetUserFullName(selectUserName);
         }
         else
         {
            selectUserName=ViewState["UserName"].ToString();
         }

         RadWindowNewUser.OpenerElementID=RadButtonNewUser.ClientID;
         RadWindowChangePassword.OpenerElementID=RadButtonChangePassword.ClientID;
      }

      protected void RadGridUserMembership_DataBound(object sender,EventArgs e)
      {
         if(RadGridUserMembership.MasterTableView.Items.Count>0)
            RadGridUserMembership.MasterTableView.Items[0].Selected=true;
      }

      protected void RadGridUserMembership_SelectedIndexChanged(object sender,EventArgs e)
      {
         GridDataItem item=(GridDataItem)RadGridUserMembership.SelectedItems[0];
         selectUserName=item["UserName"].Text;
         ViewState["UserName"]=selectUserName;
         LabelNewPassword.Text="New password for "+GetUserFullName(selectUserName);
      }

      protected void RadButtonSaveNewUser_Click(object sender,EventArgs e)
      {
         try
         {
            MembershipUser newUser=Membership.CreateUser(RadTextBoxUserNameNew.Text,RadTextBoxPasswordNew.Text,RadTextBoxEmailNew.Text);
            LabelMessage.Text+=DataExec.InsertUserAdditional(con,newUser.ProviderUserKey.ToString(),RadTextBoxUserNameNew.Text,RadTextBoxFirstNameNew.Text,RadTextBoxLastNameNew.Text,RadTextBoxInstitutionNew.Text,RadTextBoxDepartmentNew.Text,RadComboBoxCountry.SelectedValue);
            SqlDataSourceUserMembership.SelectCommand=strSelectCommand;
            RadGridUserMembership.DataBind();
            RadGridUserAdditional.DataBind();
         }
         catch(MembershipCreateUserException ex)
         {
            LabelMessage.Text=GetErrorMessage(ex.StatusCode);
         }
         catch(HttpException ex)
         {
            LabelMessage.Text=ex.Message;
         }
      }

      private string GetErrorMessage(MembershipCreateStatus status)
      {
         switch(status)
         {
            case MembershipCreateStatus.DuplicateUserName:
               return "Username already exists. Please enter a different user name.";

            case MembershipCreateStatus.DuplicateEmail:
               return "A username for that e-mail address already exists. Please enter a different e-mail address.";

            case MembershipCreateStatus.InvalidPassword:
               return "The password provided is invalid. Please enter a valid password value.";

            case MembershipCreateStatus.InvalidEmail:
               return "The e-mail address provided is invalid. Please check the value and try again.";

            case MembershipCreateStatus.InvalidAnswer:
               return "The password retrieval answer provided is invalid. Please check the value and try again.";

            case MembershipCreateStatus.InvalidQuestion:
               return "The password retrieval question provided is invalid. Please check the value and try again.";

            case MembershipCreateStatus.InvalidUserName:
               return "The user name provided is invalid. Please check the value and try again.";

            case MembershipCreateStatus.ProviderError:
               return "The authentication provider returned an error. Please verify your entry and try again. If the problem persists, please contact your system administrator.";

            case MembershipCreateStatus.UserRejected:
               return "The user creation request has been canceled. Please verify your entry and try again. If the problem persists, please contact your system administrator.";

            default:
               return "An unknown error occurred. Please verify your entry and try again. If the problem persists, please contact your system administrator.";
         }
      }

      private string GetUserFullName(string aUserName)
      {
         string returnValue="";
         SqlConnection conSQL=null;
         try
         {
            using(conSQL=new SqlConnection(con))
            {
               SqlCommand cmdGet=null;
               SqlDataReader aDataReader=null;

               string strSQLget="SELECT aspnet_Users.UserName, UserAdditional.FirstName + ' ' + UserAdditional.LastName + ' [' +  Macsur.dbo.Country.CountryName + ']' AS FullName";
               strSQLget+=" FROM aspnet_Users";
               strSQLget+=" INNER JOIN UserAdditional ON aspnet_Users.UserId = UserAdditional.UserID";
               strSQLget+=" INNER JOIN  Macsur.dbo.Country ON UserAdditional.CountryID =  Macsur.dbo.Country.CountryID";
               strSQLget+=" WHERE (aspnet_Users.UserName = '"+aUserName+"') AND (aspnet_Users.ApplicationId = '"+aApplicationID+"')";
               using(cmdGet=new SqlCommand(strSQLget,conSQL))
               {
                  cmdGet.CommandType=CommandType.Text;
                  conSQL.Open();
                  aDataReader=cmdGet.ExecuteReader();
                  aDataReader.Read();
                  if(aDataReader["UserName"].ToString()!="")
                     returnValue=aDataReader["FullName"].ToString();
                  else
                     returnValue="N/A";
                  conSQL.Close();
               }
            }
         }
         catch(Exception ex)
         {
            returnValue="Function: GetUserFullName, Message: "+ex.Message;
         }

         return returnValue;
      }

      protected void RadGridUserMembership_DeleteCommand(object sender,GridCommandEventArgs e)
      {
         string strUserName=this.RadGridUserMembership.MasterTableView.Items[e.Item.ItemIndex].Cells[4].Text;
         Membership.DeleteUser(strUserName,true);
         SqlDataSourceUserMembership.SelectCommand=strSelectCommand;
         RadGridUserMembership.DataBind();
         RadGridUserAdditional.DataBind();
      }

      protected void RadButtonSaveChangePassword_Click(object sender,EventArgs e)
      {
         try
         {
            MembershipUser aUser=Membership.GetUser(selectUserName);
            string resetPassword=aUser.ResetPassword();
            aUser.ChangePassword(resetPassword,RadTextBoxNewPassword.Text);
         }
         catch(Exception ex)
         {
            LabelMessage.Text=ex.Message;
         }
      }
   }
}