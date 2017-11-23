using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    public class Start : System.Web.UI.UserControl
    {
        string debug;
        public Start()
        {
            const string TITLE = "Farm AC";
            string version, view;


            if ((string)HttpContext.Current.Session["User"] == "+")
            {
                Response.Redirect("Default.asp");
                Response.End();
            }
            else
            {
                debug = Request["debug"];
                if (debug == "+")
                {
                    debug = "0";
                }
                view = (string)HttpContext.Current.Session["view"];
                if (view == "+")
                    view = "normal";

                General stuff = new General();
                stuff.BeginHeader(TITLE, "+", "+");

                CreateScript();
                stuff.EndHeader();
                CreatePage();

            }
        }

        void CreateScript()
        {
            Response.Write("<script language=" + "javascript" + ">" + "\r");
            Response.Write("<!--" + "\r");
            Response.Write("   if(top.frames.length != 0)" + "\r");
            Response.Write("      {top.location=self.document.location};" + "\r");
            Response.Write("      window.name='FarmNparent';" + "\r");
            Response.Write("//-->" + "\r");
            Response.Write("</script>" + "\r");
        }

        void CreatePage()
        {
            Response.Write("<frameset rows=" + "55,21,*,30" + " border=" + "0" + " frameborder=" + "no" + " marginwidth=" + "0" + " marginheight=" + "0" + " topmargin=" + "0" + " leftmargin=" + "0" + " framespacing=" + "0" + ">" + "\r");
            Response.Write("  <frame src=" + "header.asp" + " name=" + "frameHeader" + " frameborder=" + "no" + " marginwidth=" + "5" + " marginheight=" + "5" + " scrolling=" + "no" + " noresize>" + "\r");
            Response.Write("  <frame src=" + "tab1.asp?TB=" + (string)HttpContext.Current.Session["tabNumber"] + "&amp;debug=" + debug + "+" + " name=" + "frameTab" + " frameborder=" + "no" + " marginwidth=" + "0" + " marginheight=" + "0" + " scrolling=" + "no" + " noresize>" + "\r");
            Response.Write("  <frame style=" + "background-color: #C8E1F3;" + " background=" + "#C8E1F3" + " src=" + "data.asp?TB=" + (string)HttpContext.Current.Session["tabNumber"] + "&amp;version=" + (string)HttpContext.Current.Session["version"] + "&amp;debug=" + debug + "+" + " name=" + "frameData" + " frameborder=" + "no" + " marginwidth=" + "0" + " marginheight=" + "0" + " scrolling=" + "yes" + ">" + "\r");
            Response.Write("  <frame src=" + "Provider.asp" + " name=" + "frameProvider" + " frameborder=" + "no" + " marginwidth=" + "0" + " marginheight=" + "0" + " scrolling=" + "no" + " noresize>" + "\r");
            Response.Write("</frameset>" + "\r");
        }
    }
}