<%
'**************************************************************************************************
' Author: Nina Detlefsen, Danish Institute of Agricultural Sciences
' Description: Test af ny LP-model
'**************************************************************************************************

Sub LPRotation(CropString, Yield, arrCropAndArea2,dictFieldPlan, NumberCrops, NProd, xAarP2, xAarP1, xAar, MIPStatus)
response.write "<br> NumberCrops " & NumberCrops &" " & CropString
Dim index, h,i,j,f,k
Dim TotalNoHa

TotalNoHa = 0
For i=0 To UBound(arrCropAndArea2)
  TotalNoHa = TotalNoHa + arrCropAndArea2(i,dictFieldPlan.Item("Area"))
next


Server.ScriptTimeOut = 24000
MIPStatus = "-1"

' Finder ud af om 4-kombinationen er tilladt (1) eller ikke (0).
  Dim dictAllowedComb,X, arrAllowedComb


While Not(CInt(MIPStatus)=6)
'    response.write "<br> MIPStatus " & MIPStatus

  set dictAllowedComb = CreateObject("Scripting.Dictionary")
  set dictAllowed3Comb = CreateObject("Scripting.Dictionary")


	X = CallSP ("Get4Combinations '" & CropString &"'", arrAllowedComb, dictAllowedComb)

'    response.write "<br> Get4Combinations " & X

'  response.write "<br> arrAllowedComb " 
'  Print2 arrAllowedComb, dictAllowedComb

  For i=0 To UBound(arrAllowedComb)
     If arrAllowedComb(i,dictAllowedComb.item("Allowed")) = 1 Then 
     arrAllowedComb(i,dictAllowedComb.item("UB")) = TotalNoHa
	 End If 
  Next


If CInt(MIPStatus) > CInt(-1) Then
  For i=0 To UBound(arrAllowedComb)
     arrAllowedComb(i,dictAllowedComb.item("Allowed")) = 1
     arrAllowedComb(i,dictAllowedComb.item("UB")) = TotalNoHa
  Next
End if

'  response.write "<br> arrAllowedComb " 
'  Print2 arrAllowedComb, dictAllowedComb
	
' Finder ud af om 3-kombinationen er tilladt (1) eller ikke (0).

  Dim dictAllowed3Comb, arrAllowed3Comb
	X = CallSP ("Get3Combinations '" & CropString &"'", arrAllowed3Comb, dictAllowed3Comb)

If CInt(MIPStatus) > CInt(-1) Then
  For i=0 To UBound(arrAllowed3Comb)
     arrAllowed3Comb(i,dictAllowed3Comb.item("Allowed")) = 1
  Next
End if

'  response.write "<br> arrAllowed3Comb " 
'  Print2 arrAllowed3Comb, dictAllowed3Comb

  Dim UBP2
  ReDim UBP2(NumberCrops^2-1,2)
  index=0
    For i=0 To UBound(arrCropAndArea2,1)
      For j=0 To UBound(arrCropAndArea2,1)
	    UBP2(index,0)=arrCropAndArea2(i,dictFieldPlan.Item("Crop"))
        UBP2(index,1)=arrCropAndArea2(j,dictFieldPlan.Item("Crop"))
        UBP2(index,2)=CDbl(arrCropAndArea2(j,dictFieldPlan.Item("Area"))) 'CDbl(TotalNoHa) 
	    index=index+1
	  Next
	Next


Dim NoGraesID
NoGraesID=0
ReDim graesid(2)
For f=0 To NumberCrops-1
 If IsGraesID(arrCropAndArea2(f,dictFieldPlan.Item("Crop"))) Then 
   NoGraesID=NoGraesID+1
'  response.write "<br> NoGraesID " &NoGraesID
 End If
Next


if NoGraesID > 0 then

  
  For i=0 To UBound(UBP2,1)
    If IsGraesID(UBP2(i,0)) And UBP2(i,0)=UBP2(i,1) Then 
	  UBP2(i,2)= Int(UBP2(i,2)/2)+1
'  response.write "<br> UBP2(i,2) " &UBP2(i,2)
	End If
  Next 
  
'  For i=0 To UBound(arrAllowedComb)
'     If IsGraesID(arrAllowedComb(i,dictAllowedComb.item("CropID_1"))) And IsGraesID(arrAllowedComb(i,dictAllowedComb.item("CropID_2"))) Then 
'     arrAllowedComb(i,dictAllowedComb.item("UB")) = arrCropAndArea2(Int(index/NumberCrops^3)+1,dictFieldPlan.Item("Area"))/(NumberCrops^2) + 1
'	 End If 
'  Next



' Først sikres mod at græsmarker pløjes op hvert år
	for i=0 to UBound(arrAllowedComb,1)
   	  If ((Not(IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_1"))))) And (IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_2")))) And (Not(IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_3")))))) Or ((Not(IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_2"))))) And (IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_3")))) And (Not(IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_4")))))) Then
        arrAllowedComb(i,dictAllowedComb.Item("Allowed")) = 0
        arrAllowedComb(i,dictAllowedComb.Item("UB")) = 0
 '     response.write "<br> 4 comb "&arrAllowedComb(i,dictAllowedComb.Item("Allowed")) &" " & arrAllowedComb(i,dictAllowedComb.Item("UB"))
	  End If
    next

	for i=0 to UBound(arrAllowed3Comb,1)
   	  If (Not(IsGraesID(arrAllowed3Comb(i,dictAllowed3Comb.Item("CropID_1")))) And (IsGraesID(arrAllowed3Comb(i,dictAllowed3Comb.Item("CropID_2")))) And Not(IsGraesID(arrAllowed3Comb(i,dictAllowed3Comb.Item("CropID_3"))))) Then
        arrAllowed3Comb(i,dictAllowed3Comb.Item("Allowed")) = 0
 '     response.write "<br> 3 comb " & arrAllowed3Comb(i,dictAllowed3Comb.Item("CropID_1")) & " " & arrAllowed3Comb(i,dictAllowed3Comb.Item("CropID_2")) & " " & arrAllowed3Comb(i,dictAllowed3Comb.Item("CropID_3")) & " " & arrAllowed3Comb(i,dictAllowed3Comb.Item("Allowed"))
	  End If
	next

end if

'  response.write "<br> arrAllowedComb " 
'  Print2 arrAllowedComb, dictAllowedComb

'  response.write "<br> arrAllowed3Comb " 
'  Print2 arrAllowed3Comb, dictAllowed3Comb

'Response.write "<br> Her skal kald til LP solver sendes med arrCropAndArea, Yield, ProductionYears, CropReq, CropComb"


dim matVal, ColStart, RowInd, elemmat, elemcols, elemrow, qgtype, mgcols
dim dRHS, dObj, sRowType, dLB, dUB

elemmat=3*(NumberCrops^2+NumberCrops^3)+3*NumberCrops^4*(NProd-1)+2*NumberCrops^4
elemcols=NumberCrops^2+NumberCrops^3+NProd*NumberCrops^4
elemrow=3*NumberCrops+NumberCrops^2+NumberCrops^3+NProd*NumberCrops+(NProd-1)*NumberCrops^3

'Response.write "<br> NumberCrops "& NumberCrops
'Response.write "<br> NProd "& NProd

'Response.write "<br> rows "& elemrow
'Response.write "<br> columns "& elemcols
'Response.write "<br> elemmat "& elemmat

Redim matVal(elemmat-1)
Redim ColStart(elemcols)
Redim RowInd(elemmat-1)
Redim dObj(elemcols-1)
Redim dRHS(elemrow-1)
Redim sRowType(elemrow-1)
Redim dLB(elemcols-1)
Redim dUB(elemcols-1)
ReDim qgtype(elemcols-1)
ReDim mgcols(elemcols-1)

dim y

'ColStart genereres 
index=0
for i=0 to NumberCrops^2+NumberCrops^3+(NProd-1)*NumberCrops^4-1
  ColStart(index) = 3*i
  index=index+1
next
for i=0 to NumberCrops^4-1
  ColStart(index) = 3*(NumberCrops^2+NumberCrops^3+(NProd-1)*NumberCrops^4) + 2*i
  index=index+1
next
ColStart(index) = elemmat

'matVal genereres
index=0
for i=0 to NumberCrops^2+NumberCrops^3-1
  matVal(index) = 1.0
  index=index+1
  matVal(index) = -1.0
  index=index+1
  matVal(index) = 1.0
  index=index+1
next
for i=0 to (NProd-1)*NumberCrops^4-1
  matVal(index) = 1.0
  index=index+1
  matVal(index) = 1.0
  index=index+1
  matVal(index) = -1.0
  index=index+1
next
for i=0 to NumberCrops^4-1
  matVal(index) = 1.0
  index=index+1
  matVal(index) = 1.0
  index=index+1
next

' RowInd genereres
index=0
for i=0 to NumberCrops-1
  for j=0 to NumberCrops-1
    RowInd(index) = int(index/(3*NumberCrops))
    index=index+1
    RowInd(index) = int(index/3)+NumberCrops
    index=index+1
    RowInd(index) = NumberCrops + NumberCrops^2 + NumberCrops^3 + j
    index=index+1
  next
next
for i=0 to NumberCrops^2-1
  for j=0 to NumberCrops-1
    RowInd(index) = int(index/(3*NumberCrops))
    index=index+1
    RowInd(index) = int(index/3)+NumberCrops
    index=index+1
    RowInd(index) = 2*NumberCrops + NumberCrops^2 + NumberCrops^3 + j
    index=index+1
  next
next
'for prod år 0
for i=0 to NumberCrops-1
 for k=0 to NumberCrops^2-1
  for j=0 to NumberCrops-1
    RowInd(index) = int(index/(3*NumberCrops))
    index=index+1
    RowInd(index) = 3*NumberCrops + NumberCrops^2 + NumberCrops^3 + j
    index=index+1
    RowInd(index) = 3*NumberCrops  + NProd*NumberCrops + int(index/3) - i*(NumberCrops^3)
    index=index+1
  next
 next
next
'for prod år > 0 < NProd -1
for y=1 to NProd-2
 for i=0 to NumberCrops-1
  for k=0 to NumberCrops^2-1
    for j=0 to NumberCrops-1
      RowInd(index) = 3*NumberCrops + NumberCrops^2 + NumberCrops^3 + y*NumberCrops + j
      index=index+1
      RowInd(index) = int(index/(3*NumberCrops))+ (NProd+2)*NumberCrops
      index=index+1
   '   RowInd(index) = 3*NumberCrops + NumberCrops^2 + NumberCrops^3 + NProd*NumberCrops + NumberCrops^3 + i
      RowInd(index) = 3*NumberCrops + NumberCrops^2 + NumberCrops^3 + NProd*NumberCrops + y*NumberCrops^3 + k*NumberCrops + j
      index=index+1
    next
  Next
 Next 
next
'for prod år NProd -1
for y=NProd-1 to NProd-1
  for i=0 to NumberCrops^3-1
    for j=0 to NumberCrops-1
      RowInd(index) = 3*NumberCrops + NumberCrops^2 + NumberCrops^3 + y*NumberCrops + j
      index=index+1
      RowInd(index) = 3*NumberCrops + NumberCrops^2 + NumberCrops^3 + NProd*NumberCrops + (y-1)*NumberCrops^3 + i
      index=index+1
    next
  next
next


' dRHS genereres
index=0
for i=0 to NumberCrops-1
    dRHS(index) = arrCropAndArea2(i,dictFieldPlan.Item("Area"))
    index=index+1
next
for i=0 to NumberCrops^2+NumberCrops^3-1
    dRHS(index) = 0
    index=index+1
next
for j=0 to 1+NProd
for i=0 to NumberCrops-1
    dRHS(index) = arrCropAndArea2(i,dictFieldPlan.Item("Area"))
    index=index+1
next
next
for i=0 to (NProd-1)*NumberCrops^3-1
    dRHS(index) = 0
    index=index+1
next

'dObj genereres 
index=0
for i=0 to NumberCrops^2+NumberCrops^3-1
  dObj(index) = 0.0
'response.write "<br> dObj("&index&") " & dObj(index) 
  index=index+1
next
for y=0 to (NProd-1)
  for i=0 to NumberCrops-1
    for h=0 to NumberCrops-1
      for j=0 to NumberCrops-1
        for k=0 to NumberCrops-1
        dObj(index) = CDbl(Yield(h,j,k))
'response.write "<br> dObj("&index&") " & dObj(index) &" Yield("&h&","&j&","&k&")" 
		index=index+1
        next
      next
    next
  next
next

'sRowType genereres 
for i=0 to Ubound(sRowType)
  sRowType(i) = "E"
next

'dLB genereres 
for i=0 to Ubound(dLB)
  dLB(i) = 0.0
next

'dUB genereres 
index=0
for i=0 to NumberCrops^2-1
  dUB(index) = CDbl(UBP2(index,2))
  index=index+1
Next

for i=0 to UBound(arrAllowed3Comb,1)
  if arrAllowed3Comb(i,dictAllowed3Comb.Item("Allowed"))=0 then
     dUB(index) = 0.0
  else 
     dUB(index) = TotalNoHa
  end If
  index=index+1
next

for y=0 to (NProd-1)
  for i=0 to UBound(arrAllowedComb,1)
    dUB(index) = CDbl(arrAllowedComb(i,dictAllowedComb.Item("UB")))
'    if arrAllowedComb(i,dictAllowedComb.Item("Allowed"))=0 then
'             dUB(index) = 0.0
'    else 
'             dUB(index) = 1000.0
'    end if
    index=index+1
  next
next

ReDim qgtype(elemcols-1)
ReDim mgcols(elemcols-1)

'qgtype genereres 
for i=0 to Ubound(qgtype)
  qgtype(i) = "I"
Next

'mgcols genereres 
for i=0 to Ubound(mgcols)
  mgcols(i) = i
Next

  set dictAllowedComb = nothing
  set dictAllowed3Comb = nothing

'Response.write " <br> Colstart "
'PrintArrayInTable ColStart
'Response.write " <br> matVal "
'PrintArrayInTable matVal
'Response.write " <br> RowInd "
'PrintArrayInTable RowInd
'Response.write " <br> dRHS "
'PrintArrayInTable dRHS
'Response.write " <br> dObj "
'PrintArrayInTable dObj
'Response.write " <br> sRowType "
'PrintArrayInTable sRowType

'Response.write " <br> dLB "
'PrintArrayInTable dLB
'Response.write " <br> dUB "
'PrintArrayInTable dUB

'Response.write " <br> qgtype "
'PrintArrayInTable qgtype

'Response.write " <br> mgcols "
'PrintArrayInTable mgcols

dim oSoapClient
Dim tid1,tid0
'Kræver at SoapToolkit 3.0 er installeret på serveren
set oSoapClient = server.CreateObject("MSSOAP.SoapClient30")
tid0=Timer
oSoapClient.ClientProperty("ServerHTTPRequest") = True
oSoapClient.MSSoapInit("http://172.23.173.223/FarmN/webservice/FarmNOptimizer.asmx?WSDL")
'oSoapClient.MSSoapInit("http://localhost/Webservice/FarmNOptimizer.asmx?WSDL")
'oSoapClient.ConnectorProperty("Timeout") = 60000000

Dim result, LPStatus, ObjVal
'løser problemet som LP
'result = oSoapClient.OptimizeLP(sRowType, dRHS, dObj, dLB, dUB, ColStart, RowInd, matVal, LPStatus, ObjVal)
'Response.Write("<br>LPStatus " & LPStatus & "<br> " )
'Response.Write("ObjVal " & ObjVal & "<br> " )

'PrintArrayInTable result

Dim resultIP
Dim MIP2
MIP2 = MIPStatus
'Løser problemet som IP
resultIP = oSoapClient.OptimizeGlobalG(sRowType, dRHS, dObj, dLB, dUB, ColStart, RowInd, matVal, qgtype, mgcols, MIPStatus, ObjVal)
Response.Write("<br>MIPStatus med Global i løkken " & MIPStatus)
'Response.Write("<br>ObjVal " & ObjVal & "<br> " )
'PrintArrayInTable resultIP
tid1=Timer
'response.write "<br> Tid i oSoapClient " & tid1-tid0

set oSoapClient = Nothing
'response.flush

If CInt(MIP2) = CInt(MIPStatus) Then MIPStatus=6


Wend

'Response.write "<br> NumberCrops,NProd " & NumberCrops & " " & NProd
 ConvertResult xAarP2,xAarP1,xAar,resultIP,NumberCrops,NProd


'Response.write " <br> xAarP2 "
'    For j=0 To NumberCrops-1
'      For k=0 To NumberCrops-1
'        If xAarP2(j,k) > 0  Then 
'		  Response.write " <br> xAarP2("&j&","&k&") = "& xAarP2(j,k)
'		End If 
'      Next
'    Next

'Response.write "<br>  xAarP1 " 
'  For i=0 To NumberCrops-1
'    For j=0 To NumberCrops-1
'      For k=0 To NumberCrops-1
'        If xAarP1(i,j,k) > 0  Then 
'		  Response.write " <br> xAarP1("&i&","&j&","&k&") = "& xAarP1(i,j,k)
'		End If 
'      Next
'    Next
'  Next
'Dim l
'Response.write "<br>  xAar " 
'  Response.write "<br> "
'For y=0 To NProd-1
'  For i=0 To NumberCrops-1
'    For j=0 To NumberCrops-1
'      For k=0 To NumberCrops-1
'        For l=0 To NumberCrops-1
'		  If xAar(y,i,j,k,l) > 0 Then
'		  Response.write "  xAar("&y&","&i&","&j&","&k&","&l&") = "& xAar(y,i,j,k,l)
'		  End if
'		Next
'      Next
'    Next
'  Next
'  Response.write "<br> "
'Next

End Sub


'--------------------------------------------------

  Sub  ConvertResult(xAarP2,xAarP1,xAar,result,NumberCrops,NProd)
  Dim i,j,k,l,y, index
  index=0
  For i=0 To NumberCrops-1
    For j=0 To NumberCrops-1
	  xAarP2(i,j) = Clng(result(index))/1000
	  index=index+1
	Next
  Next

  For i=0 To NumberCrops-1
    For j=0 To NumberCrops-1
      For k=0 To NumberCrops-1
        xAarP1(i,j,k) = Clng(result(index))/1000
 	    index=index+1
      Next
    Next
  Next
  
For y=0 To NProd-1
  For i=0 To NumberCrops-1
    For j=0 To NumberCrops-1
      For k=0 To NumberCrops-1
        For l=0 To NumberCrops-1
		  xAar(y,i,j,k,l) = Clng(result(index))/1000
		  index=index+1
		Next
      Next
    Next
  Next
Next

  End sub


'----------------------------------------------------------------

function IsGraesID(id)
Dim returnValue   
 If id = 19 Or id = 18 Or id = 12 Then 
   returnValue = True
 Else 
   returnValue = False
 End if
   IsGraesID = returnValue
end function

'**************************************************************************************************
Sub Print2(anArray,dict)
    Dim a,r,c
  a = dict.Keys

  Response.Write "<br><table border>" & Chr(13)

Response.Write "<tr>" & Chr(13)


for c = 0 to UBound(a)
  response.write "<th> " & a(c) & " </th>" & Chr(13)
next

Response.Write "</tr>" & Chr(13)	

	For r = 0 to UBound(anArray)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(a)
        If isnull(anArray(r,dict.Item(a(c)))) Then
		Response.Write "<td> - </td>" & Chr(13)
        Else 
		Response.Write "<td>" & CStr(anArray(r,dict.Item(a(c)))) & "</td>" & Chr(13)
		End if
      Next
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)

End Sub


%>