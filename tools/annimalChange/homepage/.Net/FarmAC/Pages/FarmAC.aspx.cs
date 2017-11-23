using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace FarmAC.Pages
{
    public partial class FarmAC : System.Web.UI.Page
    {
        private string curUserInit;

        
        public void logout()
        {
            HttpContext.Current.Session["hasLogin"] = "no";
            HttpContext.Current.Session["user"] = "";
            HttpContext.Current.Session["farmNumber"] = "";
            HttpContext.Current.Session["farmOwner"] = "";
            HttpContext.Current.Session["ShowResult"] = "False";
            HttpContext.Current.Session["UserInit"] = "2";
            HttpContext.Current.Session["FileName"]=null;
            HttpContext.Current.Session["AllNodes"] = null;
            ViewState["UserInit"] = "2";
            HttpContext.Current.Response.Redirect("../default.aspx");

        }
        public void selectFarm()
        {

   
            HttpContext.Current.Session["farmOwner"] = "";
            HttpContext.Current.Session["ShowResult"] = "False";
            HttpContext.Current.Session["UserInit"] = "2";
            HttpContext.Current.Session["FileName"] = null;
            HttpContext.Current.Session["AllNodes"] = null;
            ViewState["UserInit"] = "2";
            HttpContext.Current.Response.Redirect("SelectFarm.aspx");

        }
        protected void Page_Load(object sender, EventArgs e)
        {


            PanelHeader.Controls.Add(new LiteralControl("<h3>" + "Farm Nr" + ": " + Session["farmNumber"] + " and scenarie Nr " + Session["scenarioNumber"] + "</h3>"));
            PanelHeader.Controls.Add(new LiteralControl("&nbsp;<a href=\"?logout=true\" target=\"_top\">" + "Logout" + "</a>"));
            PanelHeader.Controls.Add(new LiteralControl("&nbsp;<a href=\"?selectFarm=true\" target=\"_top\">" + "Select Farm" + "</a>"));
            if(HttpContext.Current.Request["logout"]!=null)
            if(HttpContext.Current.Request["logout"].ToString()=="true")
            {
                logout();
            }
            if (HttpContext.Current.Request["selectFarm"] != null)
                if (HttpContext.Current.Request["selectFarm"].ToString() == "true")
                {
                    selectFarm();
                }
           
            PanelHeader.Controls.Add(new LiteralControl("</td><td><h1>" + "FarmAC" + "</h1><h5 style=\"color: red;\">" +"</h5>" +"\n"));
            PanelHeader.Controls.Add(new LiteralControl("</td><td><br/><br/><br/><h5>" + DateTime.Now.ToString() + "<br/>" + Session["user"] + "</h5>" + "\n"));

            if (!IsPostBack)
            {
                if (HttpContext.Current.Session["ShowResult"] == null)
                    HttpContext.Current.Session["ShowResult"] = false;
                try
                {
                    curUserInit = HttpContext.Current.Session["UserInit"].ToString();// ViewState["UserInit"].ToString();
                }
                catch
                {
                    curUserInit = "2";


                }
                ViewState["UserInit"] = curUserInit;
                if (curUserInit == "2")
                    Tab1_Click(sender, e);
                else if (curUserInit == "3")
                    Tab2_Click(sender, e);
                else if (curUserInit == "4")
                    Tab3_Click(sender, e);
                else if (curUserInit == "5")
                    Tab4_Click(sender, e);
                else if (curUserInit == "6")
                    Tab5_Click(sender, e);
                else if (curUserInit == "7")
                    Tab6_Click(sender, e);
                else if (curUserInit == "8")
                    Tab7_Click(sender, e);
                else if (curUserInit == "9")
                    Tab8_Click(sender, e);
                else if (curUserInit == "10")
                    Tab9_Click(sender, e);
                else if (curUserInit == "11")
                    Tab10_Click(sender, e);
                else if (curUserInit == "12")
                    Tab11_Click(sender, e);
                else if (curUserInit == "13")
                    Tab12_Click(sender, e);
            }
            else
            {
                //curUserInit = ViewState["UserInit"].ToString();
            }
            object st = ViewState["UserInit"]; 
            HttpContext.Current.Session["UserInit"] = ViewState["UserInit"];
        }
        protected void Tab1_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Clicked";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Initial";

            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 0;
            HttpContext.Current.Session["UserInit"] = "2";
            ViewState["UserInit"] = "2";
            reload();

        }

        protected void Tab2_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Clicked";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Initial";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 1;
            
            HttpContext.Current.Session["UserInit"] = "3";
            
            ViewState["UserInit"] = "3";
            reload();

           
        }

        protected void Tab3_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Clicked";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Initial";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 2;
            HttpContext.Current.Session["UserInit"] = "4";
            ViewState["UserInit"] = "4";
            reload();

        }
        protected void Tab4_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Clicked";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Initial";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 3;
            HttpContext.Current.Session["UserInit"] = "5";
            ViewState["UserInit"] = "5";
            reload();
        }
        protected void Tab5_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Clicked";
            Tab6.CssClass = "Initial";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 4;
            HttpContext.Current.Session["UserInit"] = "6";
            ViewState["UserInit"] = "6";
            reload();

        }
        protected void Tab6_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Clicked";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 5;
            HttpContext.Current.Session["UserInit"] = "7";
            ViewState["UserInit"] = "7";
            reload();

        }
        protected void Tab7_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Initial";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Clicked";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "ClickedWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 6;
            HttpContext.Current.Session["UserInit"] = "8";
            ViewState["UserInit"] = "8";
            reload();

        }
        protected void Tab8_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Initial";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Clicked";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "ClickedWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 7;
            HttpContext.Current.Session["balance"] = "N";
            ViewState["UserInit"] = "9";
            HttpContext.Current.Session["UserInit"] = "9";
            ViewState["UserInit"] = "9";
            reload();
        }
        protected void Tab9_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Initial";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Clicked";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "ClickedWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 8;
            HttpContext.Current.Session["balance"] = "C";
            ViewState["UserInit"] = "10";
            HttpContext.Current.Session["UserInit"] = "10";
            ViewState["UserInit"] = "10";
            reload();

        }
        protected void Tab10_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Initial";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Clicked";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "ClickedWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 9;
            HttpContext.Current.Session["balance"] = "GHG";
            ViewState["UserInit"] = "11";
            HttpContext.Current.Session["UserInit"] = "11";
            ViewState["UserInit"] = "11";
            reload();


        }
        protected void Tab11_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Initial";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Clicked";
            Tab12.CssClass = "Initial";
            MainView.ActiveViewIndex = 10;
            HttpContext.Current.Session["UserInit"] = "12";
            ViewState["UserInit"] = "12";
            reload();
        }
        protected void Tab12_Click(object sender, EventArgs e)
        {

            Tab1.CssClass = "Initial";
            Tab2.CssClass = "Initial";
            Tab3.CssClass = "Initial";
            Tab4.CssClass = "Initial";
            Tab5.CssClass = "Initial";
            Tab6.CssClass = "Initial";
            if (HttpContext.Current.Session["ShowResult"].ToString() == "True")
            {
                Tab7.CssClass = "Initial";
                Tab8.CssClass = "Initial";
                Tab9.CssClass = "Initial";
                Tab10.CssClass = "Initial";
            }
            else
            {
                Tab7.CssClass = "InitialWithoutModel";
                Tab8.CssClass = "InitialWithoutModel";
                Tab9.CssClass = "InitialWithoutModel";
                Tab10.CssClass = "InitialWithoutModel";
            }
            Tab11.CssClass = "Initial";
            Tab12.CssClass = "Clicked";
            MainView.ActiveViewIndex = 11;
            HttpContext.Current.Session["UserInit"] = "13";
            ViewState["UserInit"] = "13";
            reload();
        }
        protected void reload()
        {
            if (HttpContext.Current.Session["update"] != null)
                if (HttpContext.Current.Session["update"].ToString() == "true")
                {
                    HttpContext.Current.Session["update"] = "false";
                    Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
                }
                else
                {
                    HttpContext.Current.Session["update"] = "true";
                }
            else
            {
                HttpContext.Current.Session["update"] = "false";
                Response.Redirect(HttpContext.Current.Request.Url.AbsolutePath);
            }
        }
    }
}