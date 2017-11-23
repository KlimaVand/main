<%
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
option explicit
%>
<!-- #include file="includes/General.asp" -->
<!-- #include file="includes/adovbs.asp" -->
<!-- #include file="includes/dbInterface.asp" -->
<!-- #include file="includes/LanguageDictionary.asp" -->

<%
Private Const TITLE = "Farm AC"
dim nFarm, arrFarm, arrFarm1, aFarm, f, dictFarm1
dim arrmaxFarmNumber, dictMaxFarmNumber, maxFarmNumber, delFarm
dim newFarmNumber,arr, dictFarm, refFarmNumber
dim nStandardFarm, arrStandardFarm, aStandardFarm, fStandard, dictStandardFarm
dim nStandardCattleFarm, arrStandardCattleFarm, aStandardCattleFarm, fCattleStandard, dictStandardCattleFarm
dim nStandardPigFarm, arrStandardPigFarm, aStandardPigFarm, fPigStandard, dictStandardPigFarm

if Session("hasLogin") = "yes" then
   Session("loginTry") = "yes"


'response.write request.form
	if request("show")<>"" then Session("farmNumber")=request("show")
	Session("scenarioNumber")=1
	Session("view")="disabled"
	response.redirect("start.asp")
'  if request("DeleteFarm")<>"" and request("selectFarm")<>"" then
'	redim arr(0)
'	arr(0)= request("selectFarm")
'	InitFarmNDB
'   delFarm = CallSPReturn ("DeleteFarm",arr)
'  
'  set dictFarm1 = CreateObject("Scripting.Dictionary")
'
'   nFarm = CallSP ("GetFarmList "&Session("UserID"), arrFarm1, dictFarm1)
'
'   
'   
''   nFarm = GetFarmList(Session("UserID"),arrFarm1)
'   Session("farmNumber")     = arrFarm1(0,0)
'  set dictFarm1 = Nothing
'  
'	CloseFarmNDB
''	 response.write delFarm
'	end if
'  if request("NewFarm")<>"" then
'	InitFarmNDB
'   set dictMaxFarmNumber = CreateObject("Scripting.Dictionary")
'   maxFarmNumber = CallSP ("GetMaxFarmNumberPrUser "&Session("UserID") , arrmaxFarmNumber, dictMaxFarmNumber)
'	 maxFarmNumber = arrmaxFarmNumber(0,dictMaxFarmNumber("MaxFarmNumber"))
'	 newFarmNumber = maxFarmNumber+1
'	 redim arr(1)
'	 arr(0)= Session("UserID")
'	 arr(1)= newFarmNumber
'	 f = CallSPReturn ("InsertNewFarm",arr)
'	 Session("farmNumber")= newFarmNumber
'	 set dictMaxFarmNumber = nothing
'	CloseFarmNDB
'	end if
'  if request("CopyFarm")<>"" then
'	InitFarmNDB
'   set dictMaxFarmNumber = CreateObject("Scripting.Dictionary")
'   maxFarmNumber = CallSP ("GetMaxFarmNumberPrUser "&Session("UserID") , arrmaxFarmNumber, dictMaxFarmNumber)
'	 maxFarmNumber = arrmaxFarmNumber(0,dictMaxFarmNumber("MaxFarmNumber"))
'	 newFarmNumber = maxFarmNumber+1
'	 redim arr(2)
'	 arr(0)= request("selectFarm")
'	 arr(1)= newFarmNumber
'	 arr(2)= Session("UserID")
'	 f = CallSPReturn ("copy_Farm",arr)
'	 Session("farmNumber")= newFarmNumber
'	 set dictMaxFarmNumber = nothing
'	CloseFarmNDB
'	end if
'  if request("CopyCattleFarm")<>"" then
'	InitFarmNDB
'   set dictMaxFarmNumber = CreateObject("Scripting.Dictionary")
'   maxFarmNumber = CallSP ("GetMaxFarmNumberPrUser "&Session("UserID") , arrmaxFarmNumber, dictMaxFarmNumber)
'	 maxFarmNumber = arrmaxFarmNumber(0,dictMaxFarmNumber("MaxFarmNumber"))
'	 newFarmNumber = maxFarmNumber+1
'	 redim arr(2)
'	 arr(0)= 178902'number of demo_CattleFarm
'	 arr(1)= newFarmNumber
'	 arr(2)= Session("UserID")
'	 f = CallSPReturn ("copy_Farm",arr)
'	 Session("farmNumber")= newFarmNumber
'	 set dictMaxFarmNumber = nothing
'	CloseFarmNDB
'	end if
'  if request("CopyPigFarm")<>"" then
'	InitFarmNDB
'   set dictMaxFarmNumber = CreateObject("Scripting.Dictionary")
'   maxFarmNumber = CallSP ("GetMaxFarmNumberPrUser "&Session("UserID") , arrmaxFarmNumber, dictMaxFarmNumber)
'	 maxFarmNumber = arrmaxFarmNumber(0,dictMaxFarmNumber("MaxFarmNumber"))
'	 newFarmNumber = maxFarmNumber+1
'	 redim arr(2)
'	 arr(0)= 178904'number of demo_PigFarm
'	 arr(1)= newFarmNumber
'	 arr(2)= Session("UserID")
'	 f = CallSPReturn ("copy_Farm",arr)
'	 Session("farmNumber")= newFarmNumber
'	 set dictMaxFarmNumber = nothing
'	CloseFarmNDB
'	end if
'  if request("CopyCropProductionFarm")<>"" then
'	InitFarmNDB
'   set dictMaxFarmNumber = CreateObject("Scripting.Dictionary")
'   maxFarmNumber = CallSP ("GetMaxFarmNumberPrUser "&Session("UserID") , arrmaxFarmNumber, dictMaxFarmNumber)
'	 maxFarmNumber = arrmaxFarmNumber(0,dictMaxFarmNumber("MaxFarmNumber"))
'	 newFarmNumber = maxFarmNumber+1
'	 redim arr(2)
'	 arr(0)= 178903'number of demo_CropProductionFarm
'	 arr(1)= newFarmNumber
'	 arr(2)= Session("UserID")
'	 f = CallSPReturn ("copy_Farm",arr)
'	 Session("farmNumber")= newFarmNumber
'	 set dictMaxFarmNumber = nothing
'	CloseFarmNDB
'	end if
'	
'	if Clng(request("CopyRef_Cow"))<>0 or Clng(request("CopyRef_Pig"))<>0 or Clng(request("CopyRef_Plant"))<>0 then
'	refFarmNumber = Clng(request("CopyRef_Cow"))
'	if refFarmNumber = 0 then refFarmNumber = Clng(request("CopyRef_Pig"))
'	if refFarmNumber = 0 then refFarmNumber = Clng(request("CopyRef_Plant"))
'	InitFarmNDB
'    set dictMaxFarmNumber = CreateObject("Scripting.Dictionary")
'    maxFarmNumber = CallSP ("GetMaxFarmNumberPrUser "&Session("UserID") , arrmaxFarmNumber, dictMaxFarmNumber)
'	 maxFarmNumber = arrmaxFarmNumber(0,dictMaxFarmNumber("MaxFarmNumber"))
'	 newFarmNumber = maxFarmNumber+1
'	 redim arr(2)
'	 arr(0)= refFarmNumber
'	 arr(1)= newFarmNumber
'	 arr(2)= Session("UserID")
'	 f = CallSPReturn ("copy_Farm",arr)
'	 Session("farmNumber")= newFarmNumber
'	 set dictMaxFarmNumber = nothing
'	CloseFarmNDB
'	end if
'	InitFarmNDB
'	
'  set dictFarm = CreateObject("Scripting.Dictionary")
'
'   nFarm = CallSP ("GetFarmData "&CLng(Session("farmNumber")), arrFarm, dictFarm)
'
''   nFarm        = GetFarmData(CLng(Session("farmNumber")),arrFarm)
'		CloseFarmNDB
'		   Session("farmOwner")      = arrFarm(0,dictFarm.item("FarmOwnerName"))
'		   Session("scenarioNumber") = arrFarm(0,dictFarm.item("SelectedScenario"))
'       Session("scenarioName")   = arrFarm(0,dictFarm.item("ScenarioName"))
'       Session("zipCode")   		 = arrFarm(0,dictFarm.item("FarmOwnerZipCode"))' ny sessionvariabel 21-11-06
'		   Session("soilType")       = arrFarm(0,dictFarm.item("SoilType"))
'		   Session("irrigation")     = arrFarm(0,dictFarm.item("Irrigation"))
'		   if Session("irrigation") = "true" then 
'		      Session("irrigation") = 1
'		   else
'		      Session("irrigation") = 0
'		   end if
'		   Session("farmType")       = arrFarm(0,8)
'		   Session("yieldLevel")     = arrFarm(0,10)
'	  set dictFarm = Nothing
'	  
'		
''if request("OK")<>""  then 	
'if request("OK")<>"" or request("NewFarm")<>"" or request("CopyFarm")<>"" or request("CopyCattleFarm")<>"" or request("CopyPigFarm")<>"" or request("CopyCropProductionFarm")<>"" or Clng(request("CopyRef_Cow"))<>0 or Clng(request("CopyRef_Pig"))<>0 or Clng(request("CopyRef_Plant"))<>0  then 	
'		  response.redirect("start.asp")
'		  response.end
''		end if
'		
'else
''end if
'
'aFarm    = clng(Session("farmNumber"))

'BeginHeader TITLE,"",""
'response.write "<meta http-equiv=""content-type"" content=""text/html; charset=iso-8859-1"" />"
'CreateStyleLink "styles/stylesheet.css"
'EndHeader
'BeginBody
''InitFarmNDB
''  set dictFarm = CreateObject("Scripting.Dictionary")
'''  set dictStandardFarm = CreateObject("Scripting.Dictionary")
'''  set dictStandardCattleFarm = CreateObject("Scripting.Dictionary")
'''  set dictStandardPigFarm = CreateObject("Scripting.Dictionary")
''
''   nFarm = CallSP ("GetFarmList "&Session("UserID"), arrFarm, dictFarm)
'''   nStandardFarm = CallSP ("GetStandardFarms", arrStandardFarm, dictStandardFarm)
'''   nStandardCattleFarm = CallSP ("GetStandardCattleFarms", arrStandardCattleFarm, dictStandardCattleFarm)
'''   nStandardPigFarm = CallSP ("GetStandardPigFarms", arrStandardPigFarm, dictStandardPigFarm)
''''  nFarm = GetFarmList(Session("UserID"),arrFarm)
''  CreateLanguageDictionary
''CloseFarmNDB
''for f=0 to ubound(arrFarm)
'' arrFarm(f,0)=clng(arrFarm(f,0))
''next
'''arrFarm(0,1)=clng(arrFarm(0,1))
'''response.write nFarm
''   CreatePage
'
'EndBody
else
	 response.redirect("default.asp")
	 response.end
end if
'**************************************************************************************************
sub CreatePage()
   Response.Write("<form name=""selectFarm"" action=""SelectFarm.asp"" method=""post"">" & vbCr)
'   Response.Write("<table align=""center"" border=""0"">" & vbCr)
   Response.Write("<table border=""0"">" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<th align=""left"">"& vbCr)
   Response.write Str.Item("strSELECT_FARM") & Chr(13) 
   Response.Write("</th>" & vbCr)
'if Session("userID")<>"25" then
''if Clng(Session("userID"))=2 or Clng(Session("userID"))=9 then
'   Response.Write("<td>" & vbCr)
'   Response.Write("<input type=""submit"" name=""CopyCattleFarm"" style=""width: 120px; height: 20px; font-size: 10px"" value="""&str.item("strCopyCattleFarm")&""" />" & vbCr)
'   Response.Write("</td>" & vbCr)
'   Response.Write("<td>" & vbCr)0
'   Response.Write("<input type=""submit"" name=""CopyPigFarm"" style=""width: 120px; height: 20px; font-size: 10px"" value="""&str.item("strCopyPigFarm")&""" />" & vbCr)
'   Response.Write("</td>" & vbCr)
'   Response.Write("<td>" & vbCr)
'   Response.Write("<input type=""submit"" name=""CopyCropProductionFarm"" style=""width: 120px; height: 20px; font-size: 10px"" value="""&str.item("strCopyCropFarm")&""" />" & vbCr)
'   Response.Write("</td>" & vbCr)
''   Response.Write("</tr>" & vbCr)
''   Response.Write("<tr>" & vbCr)
''   Response.Write("<td></td>" & vbCr)
''   Response.Write("<td align=""right"">" & vbCr)
''   Response.Write("<select name=""CopyRef_Cow"">")   
''   Response.Write("<option value=""178926"">"&Str.Item("strAmmekvægRepr")&"</option")   
''end if
'end if
'   Response.Write("<td>" & vbCr)
'   Response.Write("<input type=""submit"" name=""CopyFarm"" style=""width: 120px; height: 20px; font-size: 10px"" value=""Kopiér bedrift"" />" & vbCr)
'   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<td align=""right"">" & vbCr)
   Response.Write("<select name=""selectFarm"" id=""selectFarm"">")   
   CreateSelectList_ Clng(Session("FarmNumber")),nFarm,arrFarm
   Response.Write("</td>" & vbCr)
   Response.Write("<td>" & vbCr)
   Response.Write("<input type=""hidden"" name=""DeleteFarm"" value=""""""/>" & vbCr)
if nFarm > 1 then
   Response.Write("<input type=""button"" style=""width: 120px; height: 20px; font-size: 10px"" value="""&str.item("strDeleteFarm")&""" onClick=""if (confirm('Are you sure you want to delete the farm - '+document.forms.selectFarm.selectFarm.options[document.forms.selectFarm.selectFarm.selectedIndex].text+' - ?\nAll related data will be deleted permanently.')){document.forms.selectFarm.DeleteFarm.value='Slet';submit();}""/>" & vbCr)
end if
   Response.Write("</td>" & vbCr)
   Response.Write("<td>" & vbCr)
   Response.Write("<input type=""submit"" name=""NewFarm"" style=""width: 120px; height: 20px; font-size: 10px"" value="""&str.item("strCreateEmptyFarm")&""" />" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("<td>" & vbCr)
   Response.Write("<input type=""submit"" name=""CopyFarm"" style=""width: 120px; height: 20px; font-size: 10px"" value="""&str.item("strCopyFarm")&""" />" & vbCr)
   Response.Write("</td>" & vbCr)
'   Response.Write("<td>" & vbCr)
'   Response.Write("<input type=""submit"" name=""NewFarm"" style=""width: 120px; height: 20px; font-size: 10px"" value="""&str.item("strCreateEmptyFarm")&""" />" & vbCr)
'   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)

      Response.Write("<input type=""submit"" name=""OK"" style=""width: 100px; height: 20px; font-size: 10px"" value=""OK"" />" & vbCr)

   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   
   
   Response.Write("</table>" & vbCr)
   Response.Write("<br/><br/>" & vbCr)
   Response.Write("<table>" & vbCr)
if Session("userID")<>"25" and Cint(Session("userType"))<1000 then
''if Clng(Session("userID"))=2 or Clng(Session("userID"))=9 then
'   Response.Write("<tr>" & vbCr)
'   Response.Write("<td></td>" & vbCr)
''   Response.Write("<th align = ""left"" colspan=""2"">"&Str.Item("strDemonstrationsbedrifter")&"</th>" & vbCr)
'   Response.Write("</tr>" & vbCr)
'   Response.Write("<tr>" & vbCr)
'   Response.Write("<td align=""center"">" & vbCr)
'   Response.Write("<input type=""submit"" name=""CopyCattleFarm"" style=""width: 120px; height: 20px; font-size: 10px"" value="""&str.item("strCopyCattleFarm")&""" />" & vbCr)
'   Response.Write("</td>" & vbCr)
'   Response.Write("<td align=""center"">" & vbCr)
'   Response.Write("<input type=""submit"" name=""CopyPigFarm"" style=""width: 120px; height: 20px; font-size: 10px"" value="""&str.item("strCopyPigFarm")&""" />" & vbCr)
'   Response.Write("</td>" & vbCr)
'   Response.Write("<td align=""center"">" & vbCr)
'   Response.Write("<input type=""submit"" name=""CopyCropProductionFarm"" style=""width: 120px; height: 20px; font-size: 10px"" value="""&str.item("strCopyCropFarm")&""" />" & vbCr)
'   Response.Write("</td>" & vbCr)
'   Response.Write("</tr>" & vbCr)
else
' if Session("userID")<>"25" then
'   Response.Write("<tr>" & vbCr)
'   Response.Write("<td></td>" & vbCr)
'   Response.Write("<th align = ""left"" colspan=""2""><br/><br/><br/><br/>"&Str.Item("strRepræsentativeBedrifter")&"</th>" & vbCr)
'   Response.Write("</tr>" & vbCr)
'   Response.Write("<tr>" & vbCr)
'   Response.Write("<th>"&Str.Item("strCATTLE")&"</th>" & vbCr)
'   Response.Write("<th>"&Str.Item("strPIG")&"</th>" & vbCr)
''   Response.Write("<th>"&Str.Item("strCropProduction")&"</th>" & vbCr)
'   Response.Write("</tr>" & vbCr)
'   Response.Write("<tr>" & vbCr)
''   Response.Write("<td></td>" & vbCr)
'   Response.Write("<td align=""right"">" & vbCr)
'   Response.Write("<select name=""CopyRef_Cow"" onChange=""submit();"">" & vbCr)  
'   Response.Write("<option value=""0"">"&Str.Item("strChoose")&"</option>" & vbCr)
'   CreateSelectList_  aStandardCattleFarm,nStandardCattleFarm,arrStandardCattleFarm
'	    
''   Response.Write("<option value=""178920"">"&arrStandardFarm(5,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179033"">"&arrStandardFarm(12,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179034"">"&arrStandardFarm(13,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178921"">"&arrStandardFarm(6,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179035"">"&arrStandardFarm(14,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179036"">"&arrStandardFarm(15,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178922"">"&arrStandardFarm(7,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179037"">"&arrStandardFarm(16,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179038"">"&arrStandardFarm(17,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178923"">"&arrStandardFarm(8,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179039"">"&arrStandardFarm(18,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179040"">"&arrStandardFarm(19,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178924"">"&arrStandardFarm(9,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179041"">"&arrStandardFarm(20,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179042"">"&arrStandardFarm(21,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178925"">"&arrStandardFarm(10,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179043"">"&arrStandardFarm(22,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""179044"">"&arrStandardFarm(23,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178926"">"&arrStandardFarm(11,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
'   Response.Write("</td>" & vbCr)
'   Response.Write("<td align=""right"">" & vbCr)
'   Response.Write("<select name=""CopyRef_Pig"" onChange=""submit();"">" & vbCr)  
'   Response.Write("<option value=""0"">"&Str.Item("strChoose")&"</option>" & vbCr)  
'   CreateSelectList_  aStandardPigFarm,nStandardPigFarm,arrStandardPigFarm
''   Response.Write("<option value=""178910"">"&arrStandardFarm(1,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178912"">"&arrStandardFarm(2,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178916"">"&arrStandardFarm(3,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178917"">"&arrStandardFarm(4,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178908"">"&arrStandardFarm(0,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178914"">"&arrStandardFarm(7,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
'   Response.Write("</td>" & vbCr)
''   Response.Write("<td align=""right"">" & vbCr)
''   Response.Write("<select name=""CopyRef_Plant"" onChange=""submit();"">" & vbCr)   
''   Response.Write("<option value=""0"">"&Str.Item("strChoose")&"</option>" & vbCr)   
'''   Response.Write("<option value=""178905"">"&Str.Item("strPlant1Repr")&"</option>" & vbCr)  
'''   Response.Write("<option value=""178906"">"&Str.Item("strPlant2Repr")&"</option>" & vbCr) 
''   Response.Write("<option value=""178907"">"&arrStandardFarm(0,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178908"">"&arrStandardFarm(1,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178930"">"&arrStandardFarm(21,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178931"">"&arrStandardFarm(22,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178909"">"&arrStandardFarm(2,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178916"">"&arrStandardFarm(9,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178917"">"&arrStandardFarm(10,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178932"">"&arrStandardFarm(23,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("<option value=""178929"">"&arrStandardFarm(20,dictStandardFarm.Item("FarmOwnerName"))&"</option>" & vbCr)   
''   Response.Write("</td>" & vbCr)
'   Response.Write("</tr>" & vbCr)
' end if
end if

   Response.Write("<tr>" & vbCr)
   Response.Write("<td>" & vbCr)
   Response.write("<a href=""ShowFarm.asp?show=116106"">Northern Pig production</a>" & vbCr)
   Response.Write("</td>" & vbCr)
   Response.Write("</tr>" & vbCr)
   Response.Write("</table>" & vbCr)
   Response.Write("</form>" & vbCr)


end sub
'**************************************************************************************************
%>