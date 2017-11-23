using System;
using System.Collections.Generic;
using System.Data;
using System.Linq;
using System.Web;
using System.Web.Services.Description;
using System.Web.UI;
using System.Web.UI.HtmlControls;
using System.Web.UI.WebControls;

using System.ComponentModel;
using System.Drawing;

using System.Web.UI.Design;
using System.ComponentModel.Design;
using System.Reflection;
using System.IO;
using System.Xml;

using System.Configuration;

using System.Data.SqlClient;

namespace FarmAC.Controls
{
    
   public partial class resultN : System.Web.UI.UserControl
   {
       protected void Page_Load(object sender, EventArgs e)
       {
           if (HttpContext.Current.Session["UserInit"] != null)
               if (HttpContext.Current.Session["UserInit"].ToString() == "9")
           {
               resultGenralt gen = new resultGenralt();
               gen.commonArea("N", Panel1);
           }
       }
  
    
   }
}            