<%
sub LPRotationModel(arrCropAndArea2, dictFieldPlan)
Dim f,PriceCrop,PriceN, NProd, h,Yield, cropString
'Response.write "<br> NumberCrops "& NumberCrops 
Dim i,j,k	
'PriceCrop=4
PriceN=4.61
NProd=5


'    ReDim arrCropOrder(1,NumberCrops-1)
	redim arrCrop(NumberCrops)
	for i=0 to NumberCrops-1
	  arrCrop(i)=Cint(arrCropAndArea2(i,dictFieldPlan.Item("Crop")))
'response.write "<br> TEst af udl : " & arrCropAndArea2(i,dictFieldPlan.Item("Crop")) & " " & arrCropAndArea2(i,dictFieldPlan.Item("Area"))& " " & arrCropAndArea2(i,dictFieldPlan.Item("UseUdl"))

	next

	CropString=mid(join(arrCrop,","),1,len(join(arrCrop,","))-1)
'   response.write  "<br> CropString "& CropString

    Dim dictYield, arrYield2, X, nYield
	set dictYield = CreateObject("Scripting.Dictionary")
	nYield = CallSP ("GetYieldAndN_LP " & "'" & CropString & "'", arrYield2, dictYield)


'	nYield = GetCropDefaultPriceList(aSoilTypeID,aIrrigation,CropString,2,aYieldLevel,arrYield)
'    nYield = GetYieldAndN_LP(CropString,arrYield)
'  PrintArrayInTable2 arrYield
'   response.write  "<br> nYield "& nYield

ReDim Yield(NumberCrops-1,NumberCrops-1,NumberCrops-1)

if nYield<>0 then	
  For i=0 To UBound(arrCropAndArea2)    ' Forfrugt
    For j=0 To UBound(arrCropAndArea2)  ' Afgrøden
	  For k=0 To Ubound(arrYield2)
        If CInt(arrYield2(k,dictYield.Item("CropID")))=CInt(arrCropAndArea2(j,dictFieldPlan.Item("Crop"))) And CInt(arrYield2(k,dictYield.Item("Crop1ID")))=CInt(arrCropAndArea2(i,dictFieldPlan.Item("Crop"))) Then
		  For h=0 To NumberCrops-1
		     Yield(h,i,j)=formatnumber(CDbl(arrYield2(k,dictYield.Item("Kr_FE")))*CDbl(arrYield2(k,dictYield.Item("CernalFE_PrHa")))-PriceN*CDbl(arrYield2(k,dictYield.Item("NitrogenNeed"))),1)
'             response.write  "<br> Yield("&h&","&i&","&j&")  = "& Yield(h,i,j)
		  Next
		End If
	  Next
	Next
  Next
      
	   
End If 

    Dim dictYield2
	set dictYield2 = CreateObject("Scripting.Dictionary")
	nYield = CallSP ("GetYieldAndN_LP2 " & "'" & CropString & "'", arrYield2, dictYield2)

'	nYield = GetYieldAndN_LP2(CropString,arrYield)


'  PrintArrayInTable2 arrYield
'   response.write  "<br> X "& X

if nYield<>0 then	
'   response.write  "<br> Test2"

  For i=0 To UBound(arrCropAndArea2)    ' Forfrugt2
    For j=0 To UBound(arrCropAndArea2)  ' Forfrugt1
	 For h=0 To UBound(arrCropAndArea2) ' Afgrøden
	  For k=0 To Ubound(arrYield2)
        If CInt(arrYield2(k,dictYield2.Item("CropID")))=CInt(arrCropAndArea2(h,dictFieldPlan.Item("Crop"))) And CInt(arrYield2(k,dictYield2.Item("Crop1ID")))=CInt(arrCropAndArea2(j,dictFieldPlan.Item("Crop"))) And CInt(arrYield2(k,dictYield2.Item("Crop2ID")))=CInt(arrCropAndArea2(i,dictFieldPlan.Item("Crop"))) Then
'             response.write  "<br> Yield("&i&","&j&","&h&")  = "& Yield(i,j,h)
		     Yield(i,j,h)=formatnumber(Yield(i,j,h)+CDbl(arrYield2(k,dictYield2.Item("Kr_FE")))*CDbl(arrYield2(k,dictYield2.Item("CernalFE_PrHa")))-PriceN*CDbl(arrYield2(k,dictYield2.Item("NitrogenNeed"))),1)
'             response.write  "<br> Yield("&i&","&j&","&h&")  = "& Yield(i,j,h)
		End If
	  Next
	 Next
	Next
  Next
      
	   
End If 


Dim arrParameter
redim arrParameter(2)

	 arrParameter(0)= aFarmNumber
	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aRotationID

   returnValue = CallSPReturn ("DeleteFieldPlanRotation",arrParameter)


'DeleteFieldPlanRotation aFarmNumber, aScenarioID, aRotationID


Dim MIPStatus
ReDim xAarP2(NumberCrops-1,NumberCrops-1)
ReDim xAarP1(NumberCrops-1,NumberCrops-1,NumberCrops-1)
ReDim xAar(NProd-1,NumberCrops-1,NumberCrops-1,NumberCrops-1,NumberCrops-1)

LPRotation CropString, Yield, arrCropAndArea2,dictFieldPlan, NumberCrops, NProd, xAarP2, xAarP1, xAar, MIPStatus

'Response.write "<br> MIPStatus i LPRotationModel.asp " & MIPStatus



Dim y,l, udbytte, ulovlig
ulovlig = 0
For y=0 To NProd-1
  For i=0 To NumberCrops-1
    For j=0 To NumberCrops-1
      For k=0 To NumberCrops-1
        For l=0 To NumberCrops-1
		  If xAar(y,i,j,k,l) > 0 And Yield(j,k,l) < -5000 Then
            ulovlig = 1
		  Response.write "<br>  Yield("&j&","&k&","&l&") = "& Yield(j,k,l)
		  Response.write "<br>  xAar("&y&","&i&","&j&","&k&","&l&") = "& xAar(y,i,j,k,l)
		  End if
		Next
      Next
    Next
  Next
Next

' ulovlig = 0
 Response.write "<br>  ulovlig " & ulovlig

'Response.write "<br>  Test af GetPreviousCropGroupID " & GetPreviousCropGroupID(arrCropAndArea(0,0))
Dim UdbytteMUdl, UdbytteUUdl, arr, dictarr
redim arrParameter(12)

	if MIPStatus = "6" And ulovlig = 0 then   
	   aFieldPlanRotationID=1
	   for l=0 to NumberCrops-1 ' afgrøden
	    for k=0 to NumberCrops-1 ' forfrugt
		  udbytte=0
          For j=0 To NumberCrops-1 '2 forfrugt
		    For i=0 To NumberCrops-1 ' 3 forfrugt
			  udbytte = udbytte+xAar(1,i,j,k,l)
			Next
		  Next
  	arrParameter(0)=Session("farmNumber")
	arrParameter(1)=aScenarioID
	arrParameter(2)=aRotationID
	set dictarr = CreateObject("Scripting.Dictionary")
	nYield = CallSP ("GetPreviousCropGroupID " & CInt(arrCropAndArea2(k,dictFieldPlan.Item("Crop"))), arr, dictarr)
	arrParameter(4)=arr(0,dictarr.item("PreviousCropID"))
	set dictarr = Nothing 
    Response.write "<br>  Test af GetPreviousCropGroupID " & arrParameter(4)
'	arrParameter(4)=GetPreviousCropGroupID(CInt(arrCropAndArea2(k,dictFieldPlan.Item("Crop"))))
    arrParameter(5)=CInt(arrCropAndArea2(k,dictFieldPlan.Item("Crop")))
    arrParameter(6)=CInt(arrCropAndArea2(l,dictFieldPlan.Item("Crop")))
    arrParameter(8)=1
    arrParameter(9)=0
    arrParameter(10)=0
    Select Case arrCropAndArea2(l,dictFieldPlan.Item("UseStraw"))
     Case 0
    arrParameter(11)=0
	 Case 1,2,4
    arrParameter(11)=3
     Case 3
    arrParameter(11)=1
	End Select
	
  ' Kender hvor mange ha af afgrøde l med forfrugt k 
  ' Skal derefter finde hvor mange ha med afgrøder som kræver udlæg som kommer efter
          UdbytteUUdl = 0
	      For i=0 To NumberCrops-1 ' afgrøden året efter
		  UdbytteMUdl = 0
            For j=0 To NumberCrops-1 '3 forfrugt
 'Response.write "<br> Afgrøde + afgrøde året efter " & arrCropAndArea2(l,dictFieldPlan.Item("Crop")) & " " & arrCropAndArea2(i,dictFieldPlan.Item("Crop"))& " " & xAar(2,j,k,l,i)
			   If (IsUdl(CInt(arrCropAndArea2(i,dictFieldPlan.Item("Crop"))))) And (CBool(arrCropAndArea2(l,dictFieldPlan.Item("UseUdl")))) then
 'Response.write "<br> Test af Cbool 1 " & (arrCropAndArea2(l,dictFieldPlan.Item("UseUdl")))
               UdbytteMUdl = UdbytteMUdl + xAar(2,j,k,l,i)
			   Else 
 'Response.write "<br> Test af Cbool 2 " & (arrCropAndArea2(l,dictFieldPlan.Item("UseUdl")))
               UdbytteUUdl = UdbytteUUdl + xAar(2,j,k,l,i)
			   End if
			Next
    If UdbytteMUdl > 0 Then 
    arrParameter(3)=aFieldPlanRotationID
    arrParameter(7)=UdbytteMUdl
	arrParameter(12)=CInt(arrCropAndArea2(i,dictFieldPlan.Item("Crop")))

    returnValue=CallSPReturn("InsertUpdateFieldPlanRotation ", arrParameter)
	 aFieldPlanRotationID=aFieldPlanRotationID+1
    End If
    
		  
		  Next
    If UdbytteUUdl > 0 Then
    arrParameter(3)=aFieldPlanRotationID
    arrParameter(7)=UdbytteUUdl
	If CInt(arrCropAndArea2(l,dictFieldPlan.Item("Crop")))=CInt(15) Or CInt(arrCropAndArea2(l,dictFieldPlan.Item("Crop")))= CInt(21) Then 
'Response.write "<br> Test af if "
	arrParameter(12)=CInt(arrCropAndArea2(l,dictFieldPlan.Item("Crop")))
	Else 
	arrParameter(12)=0
    End If

	returnValue=CallSPReturn("InsertUpdateFieldPlanRotation ", arrParameter)
	 aFieldPlanRotationID=aFieldPlanRotationID+1
    End If

'Response.write "<br> k,l,udbytte, UdbytteMUdl, UdbytteUUdl  : " & k & " " & l & " " & udbytte & " "& UdbytteMUdl & " "& UdbytteUUdl
		next
	   next
	end if


end sub 

'**************************************************************************
Sub PrintArrayInTable2(anArray)
    Dim r,c
  Response.Write "<br><table border>" & Chr(13)
    For r = 0 to UBound(anArray,2)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(anArray,1)
        If isnull(anArray(c,r)) Then
		Response.Write "<td> - </td>" & Chr(13)
        Else 
		Response.Write "<td>" & CStr(anArray(c,r)) & "</td>" & Chr(13)
		End if
      Next
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)
  End Sub
'**************************************************************************
  Sub PrintArrayInTable(anArray)
    Dim c
  Response.Write "<br><table border>" & Chr(13)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(anArray,1)
        Response.Write "<td>" & CStr(anArray(c)) & "</td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)
    Response.Write "</table>" & Chr(13)
  End Sub
'**************************************************************************
function IsUdl(id)
Dim returnValue   
 Select Case id
 Case 12,13,14,18,19,29,32
'   response.write "<br> Case 1 " & id
   returnValue = True
 Case Else
'   response.write "<br> Case 2 " & id
    returnValue = False
 End Select 
   IsUdl = returnValue
end Function
'***************************************************************************
%>