using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;

namespace FarmAC.Controls
{
    public class General : System.Web.UI.UserControl
    {
        
//Grazing
private const double  PARAMETER_FE_GRAZING      = 301;
//Feeding requirement
private const double PARAMETER_FE              = 302; //FE for cattle in the table: "Parameter"
private const double PARAMETER_TS              = 303; //ts for cattle in the table: "Parameter"
private const double PARAMETER_N               = 304; //N for cattle in the table: "Parameter"
private const double PARAMETER_P               = 305; //P for cattle in the table: "Parameter"
//Production
private const double PARAMETER_MILK_N          = 306;
private const double PARAMETER_GROWTH_N        = 307;
//Manure
private const double PARAMETER_MANURE_N        = 308;
private const double PARAMETER_MANURE_N_FIELD  = 309;
private const double PARAMETER_MANURE_N_FAECES = 310;
private const double PARAMETER_MANURE_N_URINE  = 311;
private const double PARAMETER_BEDDING_N       = 312;
//dim time1, time2,time3,time4,time5,time6,time7,time8,time9,time10,time11,time12,time13,time14,time15,time16,time17


public void BeginHeader(string title,string keywords,string description)
{
   Response.Write("<!DOCTYPE html PUBLIC "+"-//W3C//DTD XHTML 1.0 Transitional//EN"+" "+"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd"+">");
   Response.Write("<html xmlns="+"http://www.w3.org/1999/xhtml"+">" + "\t");
   Response.Write("<head>");
   Response.Write("<title>"+title+"</title>");
   Response.Write("<meta http-equiv="+"Content-Type"+" content="+"text/html;charset=iso-8859-1"+"></meta>" + "\t");
   if (keywords != "+")
      Response.Write("<meta http-equiv="+"KEYWORDS"+" content="+"+"+keywords+"+"+"></meta>" + "\t");

   if (description != "+" )
      Response.Write("<meta http-equiv="+"DESCRIPTION"+" content="+"+"+description+"+"+"></meta>" + "\t");

   }

public void EndHeader()
{
   Response.Write("</head>" + "\t");
}

public void BeginBody()
{
    Response.Write("<body>" + "\t");
}

void BeginBodyData()
{
   Response.Write("<style>" + "\t"); 
   Response.Write(".animation" + "\t");
   Response.Write("{" + "\t");
   Response.Write("position:relative;" + "\t");
   Response.Write("top: 25%;" + "\t");
   Response.Write("left: 20px;" + "\t");
   Response.Write("animation:mymove 10s infinite;" + "\t");
   Response.Write("animation-timing-function:linear;" + "\t");

   Response.Write("-webkit-animation:mymove 5s infinite;" + "\t");
   Response.Write("-webkit-animation-timing-function:linear;" + "\t");
   Response.Write("}" + "\t");

   Response.Write("@keyframes mymove" + "\t");
   Response.Write("{" + "\t");
   Response.Write("from {left:20px;}" + "\t");
   Response.Write("to {left:500px;}" + "\t");
   Response.Write("}" + "\t");
   Response.Write("@-webkit-keyframes mymove" + "\t");
   Response.Write("{" + "\t");
   Response.Write("from {left:20px;}" + "\t");
   Response.Write("to {left:500px;}" + "\t");
   Response.Write("}" + "\t");
   Response.Write("</style>" + "\t");
   EndHeader();
   Response.Write("<script type="+"text/javascript"+"> var FarmNPopupWin = </script>" + "\t");
   Response.Write("<script type="+"text/javascript"+" src="+"scripts/formvalidation.js"+"></script>" + "\t");
   Response.Write("<body class="+"data"+" onFocus="+"if(FarmNPopupWin.open){FarmNPopupWin.close()};"+">" + "\t");
   Response.Write("<span id="+"veil"+" style="+"position: absolute; top: 0px; left: 0px; height: 100%; width: 100%; background-color: #C8E1F3; display: none;"+"><blink class="+"animation"+">Please wait...</blink></span>" + "\t");
   Response.Write("<script type="+"text/javascript"+">function veil() {document.getElementById('veil').style.display='block';if(document.getElementById('dynamichide')) {document.getElementById('dynamichide').style.display='none';}}</script>" + "\t");
}

void BeginBodyDataKeydown(string keydownString)
{
   Response.Write("<style>" + "\t"); 
   Response.Write(".animation" + "\t");
   Response.Write("{" + "\t");
   Response.Write("position:relative;" + "\t");
   Response.Write("top: 25%;" + "\t");
   Response.Write("left: 20px;" + "\t");
   Response.Write("animation:mymove 10s infinite;" + "\t");
   Response.Write("animation-timing-function:linear;" + "\t");

   Response.Write("-webkit-animation:mymove 5s infinite;" + "\t");
   Response.Write("-webkit-animation-timing-function:linear;" + "\t");
   Response.Write("}" + "\t");

   Response.Write("@keyframes mymove" + "\t");
   Response.Write("{" + "\t");
   Response.Write("from {left:20px;}" + "\t");
   Response.Write("to {left:500px;}" + "\t");
   Response.Write("}" + "\t");
   Response.Write("@-webkit-keyframes mymove" + "\t");
   Response.Write("{" + "\t");
   Response.Write("from {left:20px;}" + "\t");
   Response.Write("to {left:500px;}" + "\t");
   Response.Write("}" + "\t");
   Response.Write("</style>" + "\t");
   Response.Write("<script type="+"text/javascript"+">" + "\t");
   Response.Write(" <!-- //hide" + "\t");

   Response.Write("    function DisplayKeyCode(event){" + "\t");        
   Response.Write("     //var ev=new document.event(event);" + "\t");
   Response.Write("     alert('Key code: ' + event.keyCode);" + "\t");
   Response.Write("    }" + "\t");
   Response.Write("  //hide -->" + "\t");
   Response.Write("</script>" + "\t");   
   Response.Write("<script type="+"text/javascript"+"> var FarmNPopupWin = </script>" + "\t");
   Response.Write("<script type="+"text/javascript"+" src="+"scripts/formvalidation.js"+"></script>" + "\t");
   EndHeader();
   Response.Write("<body class="+"data"+" id="+"txtTest"+" onFocus="+"if(FarmNPopupWin.open){FarmNPopupWin.close()};"+" onkeydown="+"+"+keydownString+"+"+" onkeypress="+"+"+keydownString+"+"+">" + "\t");
   Response.Write("<span id="+"veil"+" style="+"position: absolute; top: 0px; left: 0px; height: 100%; width: 100%; background-color: #C8E1F3; display: none;"+"><blink class="+"animation"+">Please wait...</blink></span>" + "\t");
   Response.Write("<script type="+"text/javascript"+">function veil() {document.getElementById('veil').style.display='block';if(document.getElementById('dynamichide')) {document.getElementById('dynamichide').style.display='none';}}</script>" + "\t");
}

public void EndBody()
{

   Response.Write("</body>" + "\t");
   Response.Write("</html>" + "\t");
}

void CreateStyleLink(string strFileSpec)
{
   Response.Write("<link rel="+"StyleSheet"+" href="+"+"+strFileSpec+"+"+" type="+"text/css"+"></link>" + "\t");
}

void CreateSelectList(string aType,int nItem,string[][] arrItem)
{


   for(int i = 0;i<(nItem-1);i++)
   {
      if (arrItem[0][i] == aType)
         Response.Write("<option value="+"+"+arrItem[0][i]+"+"+" selected>"+arrItem[1][i]+"</option>");
      else  
         Response.Write("<option value="+"+"+arrItem[0][i]+"+"+">"+arrItem[1][i]+"</option>");
   
      }
}

void CreateSelectListLanguage(string aType,int nItem,string[][] arrItem)
{


   for(int i = 0;i< (nItem-1);i++)
   {
      if (arrItem[0][i]== aType)
         Response.Write("<option value="+"+"+arrItem[0][i]+"+"+" selected>"+"str"+arrItem[1][i]+"</option>");
      else  
         Response.Write("<option value="+"+"+arrItem[0][i]+"+"+">"+"str"+arrItem[1][i]+"</option>");
  
    }
}

void CreateSelectList_(string aType,int nItem,string[][] arrItem)
{
 
   bool isValidSelected=false;


   for(int i = 0;i<(nItem-1);i++)
      if (arrItem[i][0]== aType)
         Response.Write("<option value="+"+"+arrItem[i][0]+"+"+" selected>"+arrItem[i][1]+"</option>");
      else  
         Response.Write("<option value="+"+"+arrItem[i][0]+"+"+">"+arrItem[i][1]+"</option>");
    
}

void CreateSelectList_Language(string aType,int nItem,string[][] arrItem)
{
 

   for(int i = 0;i<(nItem-1);i++)
   {
      if (arrItem[i][0] ==(aType))
         Response.Write("<option value="+"+"+arrItem[i][0]+"+"+" selected>"+"str"+arrItem[i][1]+"</option>");
      else  
         Response.Write("<option value="+"+"+arrItem[i][0]+"+"+">"+"str"+arrItem[i][1]+"</option>");
   }
     
}

public void CreateSelectList_SoilType(string aType,int nItem,string[][] arrItem)
{
   

   for (int i = 0; i<( nItem-1);i++)
   {
       if (arrItem[i][0] == aType)
           Response.Write("<option value=" + "+" + arrItem[i][0] + "+" + " selected>" + "str" + arrItem[i][1] + "  " + arrItem[i][2] + "</option>");
       else
           Response.Write("<option value=" + "+" + arrItem[i][0] + "+" + ">" + "str" + arrItem[i][1] + "  " + arrItem[i][2] + "</option>");
   }
}
    }
}