using System;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;

using System.Data;
using System.Configuration;
using System.Web.Security;
using System.Web.UI.WebControls.WebParts;
using System.Web.UI.HtmlControls;
using FarmAC.Controls;
using System.Collections.Generic;

public partial class Default : System.Web.UI.Page 
{
    protected void Page_Load(object sender, EventArgs e)
    {
        mainPart();
       // Response.Redirect("Pages/Start.cs");
    }
void mainPart()
{
    if (!Page.IsPostBack)
    {
        while (LanuageSelction.Items.Count > 0)
            LanuageSelction.Items.RemoveAt(0);
        LanuageSelction.AutoPostBack = true;
        LanuageSelction.Items.Add("Dansk");
        LanuageSelction.Items.Add("English");
        LanuageSelction.Items.Add("français");
        object stuff = HttpContext.Current.Session["languageSelected"];
        if (stuff == null)
        {
            LanuageSelction.SelectedIndex = 1;
        }
        else
        {
            string selectedLanguage = stuff.ToString();
            if (selectedLanguage == "dk")
                LanuageSelction.SelectedIndex = 0;
            if (selectedLanguage == "uk")
                LanuageSelction.SelectedIndex = 1;
            if (selectedLanguage == "fr")
                LanuageSelction.SelectedIndex = 2;
        }
    }

string TITLE = "Farm AC";
HttpContext.Current.Session["hasLogin"] = "";
HttpContext.Current.Session["loginTry"] = "";
HttpContext.Current.Session["view"] = "normal";
HttpContext.Current.Session["User"] = "";
HttpContext.Current.Session["firstTime"] = "true";
//General headers=new General(); 
//headers.BeginHeader(TITLE,"+","+");
//CreateStyleLink "styles/stylesheet.css";
DefineScript();
//headers.EndHeader();
//headers.BeginBody();
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
/*   Response.Write("<form name="+"formLogin"+" action="+"+"+" method="+"post"+">" + "\r");
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
   Response.Write("</form>" + "\r");*/


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
if (HttpContext.Current.Session["Language"] == "dk" ) 
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
protected void changeLanguage(object sender, EventArgs e)
{
    DropDownList list=(DropDownList) sender;
    int f=list.SelectedIndex;
    if (list.SelectedIndex == 0)
        HttpContext.Current.Session["languageSelected"] = "dk";
    if (list.SelectedIndex == 1)
        HttpContext.Current.Session["languageSelected"] = "uk";
    if (list.SelectedIndex == 2)
        HttpContext.Current.Session["languageSelected"] = "fr";
}
protected void Button_Submit_Click(object sender, EventArgs e)
    {
        DataTable arrFarm=null;

        string fLoginName = inputLoginName.Text;// (string)HttpContext.Current.Request["inputLoginName"];
        string fPassword = inputPassword.Text;// (string)HttpContext.Current.Request["inputPassword"];
dbInterface database=new dbInterface();
database.InitFarmNDB();
        List<dbInterface.data> paramter = new List<dbInterface.data>();
       dbInterface.data data = new dbInterface.data();
       data.value = fLoginName;
       data.name = "@aLoginName";
       paramter.Add(data);
       data.value = fPassword;
       data.name = "@aPassword";
       paramter.Add(data);

       DataTable arrUser = database.GetValuesFromDB("GetUserByLogin", paramter);

       if (arrUser.Rows.Count != 0)
{
    object test = arrUser.Rows[0].ItemArray[0].ToString(); 
    string aUserID = arrUser.Rows[0].ItemArray[0].ToString();//;/(string)arrUser.Rows[0]["UserID"];
    paramter = new List<dbInterface.data>();
      data = new dbInterface.data();
      data.value = aUserID;
      data.name = "@aUserID";
      paramter.Add(data);
   arrFarm = database.GetValuesFromDB ("GetFarmList",paramter);

}
database.CloseFarmNDB();

if (arrUser.Rows.Count != 0)
{
   HttpContext.Current.Session["hasLogin"]       = "yes";
   string tmp = arrUser.Rows[0]["UserID"].ToString();
   HttpContext.Current.Session["userID"]         = arrUser.Rows[0]["UserID"];

 
   HttpContext.Current.Session["user"]            = arrUser.Rows[0]["UserName"];
   HttpContext.Current.Session["userType"]        = arrUser.Rows[0]["UserType"];
   try
   {
       HttpContext.Current.Session["farmNumber"] = arrFarm.Rows[0]["FarmNumber"];
       string tmpfdg = arrFarm.Rows[0]["FarmNumber"].ToString(); ;
       // string tmp2=(string) arrFarm.Rows[0]["FarmNumber"];
       HttpContext.Current.Session["farmOwner"]       = arrFarm.Rows[0]["FarmOwnerName"];
       HttpContext.Current.Session["scenarioNumber"]  = arrFarm.Rows[0]["SelectedScenario"];
       HttpContext.Current.Session["scenarioName"]    = arrFarm.Rows[0]["ScenarioName"];

       HttpContext.Current.Session["soilType"]        = arrFarm.Rows[0]["SoilType"];
       HttpContext.Current.Session["irrigation"]      = arrFarm.Rows[0]["Irrigation"];
       if (HttpContext.Current.Session["irrigation"]  == "true") 
          HttpContext.Current.Session["irrigation"]  = "1";
       else
          HttpContext.Current.Session["irrigation"]  = "0";

       HttpContext.Current.Session["farmType"]        = arrFarm.Rows[0]["FarmType"];
       HttpContext.Current.Session["yieldLevel"]      = arrFarm.Rows[0]["YieldLevel"];
       HttpContext.Current.Session["tabNumber"]      = "1";
   }
   catch
   {
       HttpContext.Current.Session["farmNumber"] =  -1;
   
       // string tmp2=(string) arrFarm.Rows[0]["FarmNumber"];
       HttpContext.Current.Session["farmOwner"]       = -1;
       HttpContext.Current.Session["scenarioNumber"]  = -1;
       HttpContext.Current.Session["scenarioName"]    = -1;
 

       HttpContext.Current.Session["soilType"]        =  -1;
       HttpContext.Current.Session["irrigation"]      =  -1;
       if (HttpContext.Current.Session["irrigation"]  == "true") 
          HttpContext.Current.Session["irrigation"]  = "1";
       else
          HttpContext.Current.Session["irrigation"]  = "0";

       HttpContext.Current.Session["farmType"]        =  -1;
       HttpContext.Current.Session["yieldLevel"]      =  -1;;
       HttpContext.Current.Session["tabNumber"]      = "1";
   
   }
}
else
    {
   HttpContext.Current.Session["hasLogin"]        = "no";
}
string returnURL;
if( (String)HttpContext.Current.Session["hasLogin"]  == "yes")
{
    HttpContext.Current.Session["loginTry"]  = "yes";

    object tmp3 = HttpContext.Current.Session["userType"];
    if (int.Parse(tmp3.ToString()) >= 100)
   {
       HttpContext.Current.Session.Timeout = 40;
       returnURL = ".\\Pages\\SelectFarm.aspx";
   }
   else
   {
     returnURL = "start.asp";
   }
   }
else
    {
        HttpContext.Current.Session["loginTry"] = "no";
        returnURL = HttpContext.Current.Request.Url.AbsolutePath;
    }

HttpContext.Current.Response.Redirect(returnURL);
HttpContext.Current.Response.End();
    }
}
