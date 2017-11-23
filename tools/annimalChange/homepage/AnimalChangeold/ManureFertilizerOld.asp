<%
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
%>
<!-- #include file="includes/RotationModel2.asp" -->
<%
Dim totalManureNContent,totalManureShareOfNNeed,arrManureNContent,arrNNeed, arrManureFertilizerDelivery
Dim ObjectCoeff   ' matrix of objective function coefficients NumberOfFieldPlanRotations * NumberOfManureTypes
                  ' -10000 indicates that it is an impossible combination of manure and crop
Dim arrResult   ' Result array
Dim arrA  ' array of ManureTypes
Dim arrB  ' array of fields(FieldPlanRotations)
Dim arrDelivery  ' array of deliveryTypes for each result in arrResult
Dim i, j, k, test,totalNDelivered,totalNLoss
Dim nManureType, arrManureType
'**************************************************************************************************
sub CreateTabManure()
dim c
InitFarmNDB
totalManureNContent=Round(GetManure_N_Content(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrManureNContent),2)
totalManureShareOfNNeed=Round(GetManureShareOfN_Need(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrNNeed),2)
if totalManureNContent<totalManureShareOfNNeed then c=1
if totalManureNContent>totalManureShareOfNNeed then c=2
if totalManureNContent=totalManureShareOfNNeed then c=3
select case c
case 1
	Redim ObjectCoeff(Ubound(arrManureNContent,2),Ubound(arrNNeed,2)-1)
	Redim arrResult(Ubound(arrManureNContent,2),Ubound(arrNNeed,2)-1)
	Redim arrDelivery(Ubound(arrManureNContent,2),Ubound(arrNNeed,2)-1)
	Redim arrA(Ubound(arrManureNContent,2))
	Redim arrB(Ubound(arrNNeed,2)-1)
case 2
	Redim ObjectCoeff(Ubound(arrManureNContent,2)-1,Ubound(arrNNeed,2))
	Redim arrResult(Ubound(arrManureNContent,2)-1,Ubound(arrNNeed,2))
	Redim arrDelivery(Ubound(arrManureNContent,2)-1,Ubound(arrNNeed,2))
	Redim arrA(Ubound(arrManureNContent,2)-1)
	Redim arrB(Ubound(arrNNeed,2))
case 3
	Redim ObjectCoeff(Ubound(arrManureNContent,2)-1,Ubound(arrNNeed,2)-1)
	Redim arrResult(Ubound(arrManureNContent,2)-1,Ubound(arrNNeed,2)-1)
	Redim arrDelivery(Ubound(arrManureNContent,2)-1,Ubound(arrNNeed,2)-1)
	Redim arrA(Ubound(arrManureNContent,2)-1)
	Redim arrB(Ubound(arrNNeed,2)-1)
case else
end select
	
    InitArray ObjectCoeff,-10000
    InitArray arrResult,-1
	FillArrays c
	   


   Response.Write("<h3>Fordeling af organisk gødning</h3><br/>" & vbCr) 
   if totalManureNContent<>0 then
     Response.Write("  <table border=""0"" cellpadding=""3"" cellspacing=""0"">" & vbCr)
     CreateTableManureContent 
	 if totalManureShareOfNNeed<>0 then
	  CreateTableManureShareOfNNeed
	  CalculateManureDelivery
	  GetManureFertilizerDelivery CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrManureFertilizerDelivery
      CloseFarmNDB
	  CreateTableManureDelivery
	 else
	  GetManureFertilizerDelivery CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrManureFertilizerDelivery
	  CloseFarmNDB
      CreateTableManureDelivery
	  response.write("<tr><td>Der er intet gødningsbehov at opfylde (ingen marker)</td></tr></table>" & vbCr) 
     end if
   else
    response.write("<tr><td><br/>Der er ingen gødning at fordele (ingen dyr)</td></tr>" & vbCr)
   end if

   Response.Write("<br/>" & vbCr)   
   Response.Write("<br/>" & vbCr)   
end sub
'**************************************************************************************************
sub FillArrays(c)
dim degree,delType,a,b

for a=0 to Ubound(arrManureNContent,2)-1
 arrA(a)=Round(arrManureNContent(2,a),2)
 for b=0 to Ubound(arrNNeed,2)-1
    arrB(b)=Round(arrNNeed(2,b),2)
    degree=GetUtilizationDegree(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrNNeed(0,b),arrNNeed(1,b),arrManureNContent(0,a),true,delType)
    if not IsNull(degree) then 
	  ObjectCoeff(a,b)=degree
	  arrDelivery(a,b)=delType
	end if
		select case c
		case 1
		   ObjectCoeff(Ubound(arrManureNContent,2),b)=0
		   arrA(Ubound(arrManureNContent,2))=totalManureShareOfNNeed-totalManureNContent
		case 2
		   ObjectCoeff(a,Ubound(arrNNeed,2))=0
		   arrB(Ubound(arrNNeed,2))=totalManureNContent-totalManureShareOfNNeed
		case else
		end select
 next
next
end sub 
'**************************************************************************************************
sub CalculateManureDelivery()
dim a,b,aLoss,m
	   test=calcRotation(arrA,arrB,ObjectCoeff,arrResult)
       DeleteManureFertilizerDelivery CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))
	   for a=0 to Ubound(arrA)
	    for b=0 to Ubound(arrB)
	     if arrResult(a,b)<>0 and ObjectCoeff(a,b)>0 then 
			 aLoss=GetLoss(arrManureNContent(0,a), arrDelivery(a,b))
			 InsertUpdateManureFertilizerDelivery CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrNNeed(0,b), arrNNeed(1,b),arrManureNContent(0,a), arrDelivery(a,b), arrResult(a,b),Round(aLoss*arrResult(a,b)/100,2)
		 end if
		next
	   next
	   
nManureType = GetUserLossList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrManureType)

for m = 0 to Ubound(arrManureType,2)-1
	InsertUpdateBalanceStorage CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrManureType(0,m), 4, arrManureType(1,m)'updating "BalanceStorage": Tab udbringning/afgræsning - pr. StorageType der er bragt gødning ud på
next

end sub 
'**************************************************************************************************
sub CreateTableManureContent()
   dim s
  
 
'   Response.Write("  <table border=""0"" cellpadding=""3"" cellspacing=""0"">" & vbCr)
   
   
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;Gødningstype<br/><br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;N-indhold<br/>&nbsp;&nbsp;efter tab<br/>&nbsp;&nbsp;i stald<br/>&nbsp;&nbsp;og lager<br/>&nbsp;&nbsp;(Kg N)</th>" & vbCr)
      Response.Write("    </tr>" & vbCr)

   for s = 0 to Ubound(arrManureNContent,2)-1
         Response.Write("    <tr>" & vbCr)   
         Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrManureNContent(1,s)&"</td>" & vbCr)
         Response.Write("      <td align=""left"">&nbsp;&nbsp;"&FormatNumber((arrManureNContent(2,s)),1)&"</td>" & vbCr)
         Response.Write("    </tr>" & vbCr)
   next
      Response.Write("    <tr>" & vbCr)   
      Response.Write("      <td align=""left"">&nbsp;&nbsp;Ialt</td>" & vbCr)
      Response.Write("      <td align=""left"">&nbsp;&nbsp;"&FormatNumber(totalManureNContent,1)&"</td>" & vbCr)
      Response.Write("    </tr>" & vbCr)

      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td colspan=""10""><hr/></td>" & vbCr)
      Response.Write("    </tr>" & vbCr)
   
'   Response.Write("  </table>" & vbCr)
end sub
'**************************************************************************************************
sub CreateTableManureShareOfNNeed()
   dim f,fields
   
 
'   Response.Write("  <table border=""0"" cellpadding=""3"" cellspacing=""0"">" & vbCr)
   
   
      Response.Write("    <tr>" & vbCr)
'      Response.Write("      <td align=""left"" colspan=""4""><h4>Sædskifte navn: "&arrRotation(1,r)&"</td>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;antal marker<br/>&nbsp;&nbsp;der kan fordeles på</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;del af N behov<br/>&nbsp;&nbsp;der kan dækkes<br/>&nbsp;&nbsp;med organisk<br/>&nbsp;&nbsp;gødning<br/>&nbsp;&nbsp;(Kg N)</th>" & vbCr)
      Response.Write("    </tr>" & vbCr)

   for f = 0 to Ubound(arrNNeed,2)-1
   if arrNNeed(2,f)<>0 then fields=fields+1
   next

       Response.Write("    <tr>" & vbCr)   
       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&fields&"</td>" & vbCr)
       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&FormatNumber(totalManureShareOfNNeed,1)&"</td>" & vbCr)
       Response.Write("    </tr>" & vbCr)

      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td colspan=""10""><hr/></td>" & vbCr)
      Response.Write("    </tr>" & vbCr)
   
'   Response.Write("  </table>" & vbCr)
end sub
'**************************************************************************************************
sub CreateTableManureDelivery()
dim f,a,b 
 

      Response.Write("    <tr>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;Sædskifte<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;Forfrugt<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;Afgrøde</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;tildelt type<br/>&nbsp;&nbsp;organisk gødning<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;Udbringnings-<br/>&nbsp;&nbsp;teknik<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;Kg N<br/>&nbsp;&nbsp;udbragt<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;Kg N<br/>&nbsp;&nbsp;udbringningstab<br/></th>" & vbCr)
      Response.Write("    </tr>" & vbCr)
      
	  
      for a = 0 to Ubound(arrManureFertilizerDelivery,2)-1
      Response.Write("    <tr>" & vbCr)

'		   case 2,4,5,6,7,8 
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrManureFertilizerDelivery(0,a)&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrManureFertilizerDelivery(2,a)&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrManureFertilizerDelivery(4,a)&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrManureFertilizerDelivery(6,a)&"</td>" & vbCr)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrManureFertilizerDelivery(8,a)&"</td>" & vbCr)
               totalNDelivered=totalNDelivered+arrManureFertilizerDelivery(9,a)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&FormatNumber(arrManureFertilizerDelivery(9,a),1)&"</td>" & vbCr)
               totalNLoss=totalNLoss+arrManureFertilizerDelivery(10,a)
		       Response.Write("      <td align=""left"">&nbsp;&nbsp;"&FormatNumber(arrManureFertilizerDelivery(10,a),1)&"</td>" & vbCr)
       
       Response.Write("    </tr>" & vbCr)
       next
      Response.Write("    <tr>" & vbCr)
      Response.Write("      <td colspan=""10""><hr/></td>" & vbCr)
      Response.Write("    </tr>" & vbCr)
      Response.Write("    <tr>" & vbCr)
'      Response.Write("      <td align=""left"" colspan=""4""><h4>Sædskifte navn: "&arrRotation(1,r)&"</td>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;<br/></th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;Ialt:</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&FormatNumber(totalNDelivered,1)&"</th>" & vbCr)
      Response.Write("      <th align=""left"">&nbsp;&nbsp;"&FormatNumber(totalNLoss,1)&"</th>" & vbCr)
      Response.Write("    </tr>" & vbCr)
   
   Response.Write("  </table>" & vbCr)
   
'   Response.Write("</form>" & vbCr)
   CreateUpdateFormManure
end sub
'**************************************************************************************************
sub CreateUpdateFormManure()
   Response.Write("<form name=""update"" action=""Dummy.asp"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************
%>
