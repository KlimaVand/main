using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{
    public partial class ctrlDocumentation2 : System.Web.UI.UserControl
    {
        string language;
        protected void Page_Load(object sender, EventArgs e)
        {
            string test = HttpContext.Current.Session["UserInit"].ToString();
            if (HttpContext.Current.Session["UserInit"] != null)
                if (HttpContext.Current.Session["UserInit"].ToString() == "13")
            {
                if (HttpContext.Current.Session["languageSelected"] != null)
                    language = HttpContext.Current.Session["languageSelected"].ToString();
                else
                    language = "uk";
                CreateDocs();
            }
        }

        void CreateDocs()
        {

            this.contentPanel1.Attributes["src"] = "Configuring and Parameterising FarmAC.htm";
            this.Panel1.Controls.Add((contentPanel1));
        }
    }



}