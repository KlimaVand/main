using System;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using System.Data;
using System.Configuration;
using System.Web.Security;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using Telerik.Web.UI;
using FarmAC.Controls;

public partial class Default : System.Web.UI.Page 
{
    protected void Page_Load(object sender, EventArgs e)
    {
        Response.Redirect("Pages/FarmAC.aspx");
    }
void mainPart()
{
string TITLE = "Farm AC";
HttpContext.Current.Session["hasLogin"] = "+";
HttpContext.Current.Session["loginTry"] = "+";
HttpContext.Current.Session["view"] = "normal";
HttpContext.Current.Session["User"] = "+";

General headers=new General(); 
headers.BeginHeader(TITLE,"+","+");
//CreateStyleLink "styles/stylesheet.css";
DefineScript();
headers.EndHeader();
headers.BeginBody();
//InitFarmNDB
//CreateLanguageDictionary
//CloseFarmNDB

   Response.Write("<div style="+"text-align:center"+">" + "\r");

   Response.Write("</div>" + "\r");
if (HttpContext.Current.Session["loginTry"] == "no" )
{
   Response.Write("<div style+text-align:center"+">" + "\r");
   Response.Write("<h1>"+"strNOT_LOGGED_IN"+".</h1>" + "\r");

   Response.Write("</div>" + "\r");
   Response.Write("<hr/>" + "\r");
   CreatePage();
}
else
   CreatePage();

}

void CreatePage()
{
   Response.Write("<form name="+"formLogin"+" action="+"+"+" method="+"post"+">" + "\r");
   Response.Write("<table align="+"center"+" border="+"0"+">" + "\r");

   Response.Write("<tr>" + "\r");                    //denne blok lukkes ved opdatering mellem servere
   Response.Write("<td colspan="+"2"+" align="+"center"+"><h1>"+"strWELCOME"+"</h1></td>" + "\r");
   Response.Write("</tr>" + "\r");
   Response.Write("<tr>" + "\r");
   Response.Write("<td colspan="+"2"+" align="+"center"+"><img src="+"images/farm.gif"+" width="+"200"+" height="+"128"+" border="+"0"+" alt="+"farm.gif"+"/></td>" + "\r");
   Response.Write("</tr>" + "\r");
   Response.Write("<tr>" + "\r");
   Response.Write("<th align="+"left"+">"+ "\r");
 //  Response.write Str.Item("languageselection") & Chr(13) 
   Response.Write("</th>" + "\r");
   Response.Write("<td align="+"right"+">" + "\r");
   CreateLanguageSelectionBox();
   Response.Write("</td>" + "\r");
   Response.Write("</tr>" + "\r");
   Response.Write("<tr>" + "\r");
   Response.Write("<th align="+"left"+">"+"username"+"</th>" + "\r");
   Response.Write("<td align="+"right"+"><input type="+"text"+" name="+"inputLoginName"+" style="+"width: 130px;"+" /></td>" + "\r");
   Response.Write("</tr>" + "\r");
   Response.Write("<tr>" + "\r");
   Response.Write("<th align="+"left"+">"+"userpassword"+"</th>" + "\r");
   Response.Write("<td align="+"right"+"><input type="+"password"+" name="+"inputPassword"+" style="+"width: 130px;"+" /></td>" + "\r");
   Response.Write("</tr>" + "\r");
   Response.Write("<tr>" + "\r");
   Response.Write("<td colspan="+"2"+" align="+"center"+">" + "\r");
   Response.Write("<button style="+"width: 130px; height: 20px; font-size: 10px"+" onclick="+"javascript:if(Form_Validator(document.formLogin)){document.forms.formLogin.action='LoginVerify.asp';document.forms.formLogin.submit();}"+">Login</button>" + "\r");
   Response.Write("</td>" + "\r");
   Response.Write("</tr>" + "\r");

   Response.Write("<tr>" + "\r");
   Response.Write("<td colspan="+"2"+" align="+"center"+"> &nbsp;" + "\r");
      Response.Write("</td>" + "\r");
   Response.Write("</tr>" + "\r");

   Response.Write("<tr>" + "\r");
   Response.Write("<td colspan="+"2"+" align="+"center"+">" + "\r");      


   Response.Write("</td>" + "\r");
   Response.Write("</tr>" + "\r");
   
   
   Response.Write("</table>" + "\r");
   Response.Write("</form>" + "\r");


}

void DefineScript()
{
   Response.Write("<script type="+"text/javascript"+" src="+"scripts/formvalidation.js"+"></script>" + "\r");
   Response.Write("<script type="+"text/javascript"+">" + "\r");
   // Form_Validator uses general functions from formvalidation.js to validate the form
   // It is specific to the form of this page
   Response.Write("  function Form_Validator(theForm)" + "\r");
   Response.Write("  {" + "\r");
   Response.Write("    OKval=InputValidator(theForm.inputLoginName, 1, 3, "+"Brugernavn"+", 1);" + "\r");
   Response.Write("    if (OKval)" + "\r");
   Response.Write("       {OKval=InputValidator(theForm.inputPassword, 3, 10, "+"Brugerkode"+", 1);}" + "\r");
   Response.Write("    return (OKval);" + "\r");
   Response.Write("  }" + "\r");
   Response.Write("</script>" + "\r");
}

void CreateLanguageSelectionBox()
{  

Response.Write( "<select style="+"width: 130px;"+" name="+"sprog"+" size="+"1"+" onchange="+"document.forms.formLogin.action='default.asp';document.forms.formLogin.submit();"+">");
Response.Write( "<option ");
if (((string)HttpContext.Current.Session["Language"]).CompareTo( "dk" )==0) 
{
    Response.Write("selected="+"selected"+" ");
    Response.Write("value="+"dk"+">" +"sprog1"+ "</option>" +"\n");  
    Response.Write( "<option "+"\n");
}
if (HttpContext.Current.Session["Language"] == "uk") 
{
    Response.Write("selected=" + "selected" + " ");
    Response.Write( "value="+"uk"+">"+"sprog2"+ "</option>"  +"\n");  
    Response.Write("<option "+"\n");
}
if (HttpContext.Current.Session["Language"] == "fr") 
{
    Response.Write("selected="+"selected"+" ");
    Response.Write("value="+"fr"+">" +"sprog3"+"</option>" +"\n");
    Response.Write("</select>" + "\n");
}


}
}
