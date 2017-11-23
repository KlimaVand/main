using System;
using System.Collections.Generic;
using System.Configuration;
using System.Data;
using System.Data.SqlClient;
using System.IO;
using System.Linq;
using System.Text;
using System.Web;
using System.Xml;
using System.Xml.Linq;


namespace FarmAC.Controls
{
    public class logout
    {


        public logout()
        {
            HttpContext.Current.Session["hasLogin"] = "no";
            HttpContext.Current.Session["user"] = "";
            HttpContext.Current.Session["farmNumber"] = "";
            HttpContext.Current.Session["farmOwner"] = "";
            HttpContext.Current.Session["ShowResult"] = "False";

            HttpContext.Current.Response.Redirect("default.aspx");

        }


    }
}