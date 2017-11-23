using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    /// <summary>
    /// Summary description for ClearLocalFiles
    /// </summary>
    public class ClearLocalFiles : IHttpHandler
    {

        public void ProcessRequest(HttpContext context)
        {
            System.IO.DirectoryInfo directory = new System.IO.DirectoryInfo(("c:/upload/AnimalChange/") + context.Session["FarmNumber"]);
            foreach (System.IO.FileInfo file in directory.GetFiles())
                file.Delete();
            context.Response.Redirect("data.asp?TB=" + context.Session["tabNumber"] + "");
            context.Response.End();
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