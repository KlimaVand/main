using System;
using System.Collections.Generic;
using System.Data;
using System.IO;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

namespace FarmAC.Controls
{

    public partial class ctrluploadownparameters : System.Web.UI.UserControl
    {
        protected void Page_Load(object sender, EventArgs e)
        {
            String Uploader, File;
            string fs, fo, f, fldr, msg;
            fldr = "c:/upload/AnimalChange/" + HttpContext.Current.Session["FarmNumber"];


            if (Directory.Exists(fldr))
                msg = fldr + " exists.";
            else
            {
                Directory.CreateDirectory(fldr);
                msg = "Folder created";
            }
            if (FileUploadControl.HasFile)
            {
                try
                {
                    FileUploadControl.SaveAs(Server.MapPath("~/") + fldr);

                }
                catch (Exception ex)
                {

                }
            }
        }

    }
}