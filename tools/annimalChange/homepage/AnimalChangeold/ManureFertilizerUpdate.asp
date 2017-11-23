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
<%
dim aFarmNumber, aScenario, aNPercent, d, arrParameter, X
dim arrParameterInsert, aLoss, aRotationID, arrFieldPlanRotation, dictFieldPlanRotation
dim aAmountList, aDeliveryList, aManureList,nFieldPlanRotation
dim arrAmountList, arrDeliveryList, arrManureList, m, aUtilDegree, amount
dim arrParameterUtil, arrMonth, aMonthList, arrParameterDel,del,monthvalue, aArea

aFarmNumber       = Session("farmNumber")
aScenario = Session("scenarioNumber")
del=false
'nFieldPlanRotation =request.form("hiddenNFieldPlanRotation") 

Response.write request.form
response.write "<br/><br/>"

InitFarmNDB
     set dictFieldPlanRotation = CreateObject("Scripting.Dictionary")
     nFieldPlanRotation = CallSP ("GetFieldplanRotationsForManure " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) ,arrFieldPlanRotation,dictFieldPlanRotation)

ReDim arrParameter(1)
ReDim arrParameterDel(2)
 	  arrParameter(0) = aFarmNumber
      arrParameter(1) = aScenario
 	  arrParameterDel(0) = aFarmNumber
      arrParameterDel(1) = aScenario
      arrParameterDel(2) = CInt(arrFieldPlanRotation(d,dictFieldPlanRotation.Item("RotationID")))
redim arrParameterUtil(0)

       X = CallSPReturn("DeleteManureFertilizerDelivery", arrParameter)
for d = 0 to cint(nFieldPlanRotation)-1
Redim arrParameterInsert(10)
aManureList = request.form("hiddenManureList_"&d&"")
arrManureList = split(aManureList, ",")
aDeliveryList = request.form("hiddenDeliveryList_"&d&"")
arrDeliveryList = split(aDeliveryList, ",")
aAmountList = request.form("hiddenAmountList_"&d&"")
aMonthList = request.form("hiddenMonthList_"&d&"")
response.write "<br/>aAmountList "&aAmountList
response.write "<br/>aDeliveryList "&aDeliveryList
response.write "<br/>aMonthList "&aMonthList

arrAmountList = split(aAmountList, ";")
arrMonth = split(aMonthList, ",")
response.write "<br/>ubound(arrManureList)-1 "& ubound(arrManureList)-1 

aRotationID = request.form("hiddenRotationID_"&d&"")
  
 	  arrParameterInsert(0) = aFarmNumber
      arrParameterInsert(1) = aScenario
'	  if del=false then
'		   X = CallSPReturn("DeleteManureFertilizerDelivery_1", arrParameterDel)
'		   del=true
'	  end if
'       X = CallSPReturn("DeleteManureFertilizerDelivery", arrParameter)
	for m = 0 to ubound(arrManureList)-1 
	if ubound(arrAmountList)<>ubound(arrManureList) then
	  amount = 0
   else
      amount =(arrAmountList(m))
   end if
	if ubound(arrMonth)<>ubound(arrManureList) then
	  monthvalue = 1
   else
      monthvalue =(arrMonth(m))
   end if
   
'    if (ubound(arrManureList)=ubound(arrDeliveryList))and (ubound(arrAmountList)=ubound(arrMonth)) then
' response.write "Ok<br/>"
            arrParameter(0) = arrManureList(m)
            arrParameter(1) = arrDeliveryList(m)
            aLoss = 0'CallSPReturn("GetLoss", arrParameter)
			arrParameterUtil(0) = arrManureList(m)
			aUtilDegree =  0'CallSPReturn("GetManureUtilizationDegree", arrParameterUtil)

	        arrParameterInsert(2) = CInt(arrFieldPlanRotation(d,dictFieldPlanRotation.Item("RotationID")))
			arrParameterInsert(3) = CInt(arrFieldPlanRotation(d,dictFieldPlanRotation.Item("FieldPlanRotationID")))
			arrParameterInsert(4) = arrManureList(m)'CInt(arrHGdistribute(a,dictSalesOrderList("StorageID")))
			arrParameterInsert(5) = arrDeliveryList(m)'CInt(arrDelivery(a,b))
            if Cdbl(amount) >= 0 then
'			response.write "amount > 0<br/>"
			arrParameterInsert(6) = Cdbl(amount) '* CInt(arrFieldPlanRotation(d,dictFieldPlanRotation.Item("Area")))'Cdbl(arrResult(a,b))
			arrParameterInsert(7) = 0'cstr(CDbl(Round(aLoss*arrAmountList(m)/100,2)))
            arrParameterInsert(8) = 0'cstr(Cdbl(arrAmountList(m)*aUtilDegree/100))'Cdbl(arrHGdistribute(a,dictSalesOrderList("N_UtilizationDegree"))*arrResult(a,b))/100''Decide how this is going to be calculated
            arrParameterInsert(9) = Cint(monthvalue)'Cdbl(arrHGdistribute(a,dictSalesOrderList("N_UtilizationDegree"))*arrResult(a,b))/100''Decide how this is going to be calculated
            arrParameterInsert(10) = m
'			response.write "InsertUpdateManureFertilizerDelivery " &aFarmNumber&","&aScenario&","&CInt(arrFieldPlanRotation(d,dictFieldPlanRotation.Item("RotationID")))&","&CInt(arrFieldPlanRotation(d,dictFieldPlanRotation.Item("FieldPlanRotationID")))&","& arrManureList(m)&","&arrDeliveryList(m)&","&Cdbl(arrAmountList(m))&",0,0,"&Cint(arrMonth(m))
			X = CallSPReturn("InsertUpdateManureFertilizerDelivery", arrParameterInsert)
			response.write "<br/>m: "&m
			end if
'	end if

	next
'	       redim preserve arrParameterInsert(3)
'		   if m < ubound(arrManureList) then
'		   X = CallSPReturn("DeleteManureFertilizerDelivery_1", arrParameterInsert)
'		   end if
'del=false
next
CloseFarmNDB
Session("ShowResult")=False
Response.Redirect("data.asp?TB="&session("tabNumber")&"")
Response.End
%>