using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for Provider
    /// </summary>
    public class Provider : IHttpHandler
    {
        private string TITLE = "Provider";
        private string PROVIDER_HREF = "http://agro.au.dk/en/research/sektioner/climate-and-water/";
        private string PROVIDER_INST = "Aarhus University, Dept. of Agroecology";
        private string PROVIDER_GROUP = "Research section: Climate and Water";
        private string WEBMASTER_NAME = "Margit Jorgensen";
        private string WEBMASTER_EMAIL = "MargitS.Jorgensen@agrsci.dk";
        public void ProcessRequest(HttpContext context)
        {
            context.Response.Write("<address>Web site provided by " + PROVIDER_INST + " <a href=" + "" + PROVIDER_HREF + "" + " target=" + "_top" + ">" + PROVIDER_GROUP + "</a>. Report technical problems to <a href=" + "mailto:" + WEBMASTER_EMAIL + "" + ">Webmaster</a>." + "\r");
            context.Response.Write("<br/>Optimized for screen size 1024x768</address>" + "\r");
        }

        public bool IsReusable
        {
            get
            {
                return false;
            }
        }
    }
}