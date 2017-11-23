<%
'**************************************************************************************************
' Author: Nina Detlefsen, Danish Institute of Agricultural Sciences
' Description: Test af ny LP-model
'**************************************************************************************************

Sub LPRotation(CropString, Yield, arrCropAndArea2, NumberCrops, NProd, xAarP2, xAarP1, xAar, MIPStatus)

Dim Tid0, Tid1, Tid2, Tid3, Tid4, Tid5, Tid6
Dim index, h
Server.ScriptTimeOut = 2400

Tid0 = Timer

' Finder ud af om 4-kombinationen er tilladt (1) eller ikke (0).
  Dim dictAllowedComb,X, arrAllowedComb
'  Dim strSQL  
  set dictAllowedComb = CreateObject("Scripting.Dictionary")
	X = CallSP ("Get4Combinations '" & CropString &"'", arrAllowedComb, dictAllowedComb)
'  response.write "<br/>" & arrAllowedComb(0,dictAllowedComb.Item("Allowed"))
'  response.write "<br/>" & dictAllowedComb.Item("Allowed") & "<br/>"
dim a,i
a = dictAllowedComb.Keys

  response.write " <br> "

For j = 0 To UBound(arrAllowedComb)
for i = 0 To dictAllowedComb.Count -1
  response.write a(i)&" -- "
  response.write arrAllowedComb(j,dictAllowedComb.Item(a(i)))&" -- "
Next
  response.write " <br> "
Next


	Tid1=Timer

	
' Finder ud af om 3-kombinationen er tilladt (1) eller ikke (0).

  Dim dictAllowed3Comb, arrAllowed3Comb
  set dictAllowed3Comb = CreateObject("Scripting.Dictionary")
	X = CallSP ("Get3Combinations '" & CropString &"'", arrAllowed3Comb, dictAllowed3Comb)


  Dim UBP2
  ReDim UBP2(NumberCrops^2-1,2)
  index=0
    For i=0 To UBound(arrCropAndArea2,1)
      For j=0 To UBound(arrCropAndArea2,1)
	    UBP2(index,0)=arrCropAndArea2(i,dictFieldPlan.Item("Crop"))
        UBP2(index,1)=arrCropAndArea2(j,dictFieldPlan.Item("Crop"))
        UBP2(index,2)=CDbl(1000.0) 'CDbl(arrCropAndArea2(j,dictFieldPlan.Item("Area")))
	    index=index+1
	  Next
	Next
Response.write " <br> UBP2 "
PrintArrayInTable2 UBP2
	
Response.write " <br> arrCropAndArea2 "
PrintArrayInTable2 arrCropAndArea2

Tid2 = Timer

Dim NoGraesID
NoGraesID=0
ReDim graesid(2)
For f=0 To NumberCrops-1
 If IsGraesID(arrCropAndArea2(f,dictFieldPlan.Item("Crop"))) Then 
   NoGraesID=NoGraesID+1
 End If
Next
Response.write "<br> NoGraesID " &NoGraesID

  response.write " <br> "

'arrAllowedComb(0,dictAllowedComb.Item("Allowed"))
if NoGraesID > 0 then

' UB for Græs efter græs skal være halvdelen af arealet
'  For f=0 To UBound(arrCropAndArea2,1)
'    If IsGraesID(arrCropAndArea2(f,dictFieldPlan.Item("Crop"))) Then 
'      For i=0 to UBound(arrAllowedComb,1)
'	    If arrCropAndArea2(f,dictFieldPlan.Item("Crop")) = arrAllowedComb(i,dictAllowedComb.Item("CropID_3")) And arrCropAndArea2(f,dictFieldPlan.Item("Crop")) = arrAllowedComb(i,dictAllowedComb.Item("CropID_4")) Then
'          arrAllowedComb(i,dictAllowedComb.Item("UB")) = Int(arrCropAndArea2(f,dictFieldPlan.Item("Area"))/2)+1
'          Response.write "<br> UB change  " & arrAllowedComb(i,dictAllowedComb.Item("CropID_3")) & arrAllowedComb(i,dictAllowedComb.Item("CropID_3")) & arrAllowedComb(i,dictAllowedComb.Item("UB"))	
'        End If
'      Next
'    End If  
'  Next
  
  For i=0 To UBound(UBP2,1)
    If IsGraesID(UBP2(i,0)) And UBP2(i,0)=UBP2(i,1) Then 
	  UBP2(i,2)= Int(UBP2(i,2)/2)+1
	End If
  Next 
  

Response.write " <br> UBP2 "
PrintArrayInTable2 UBP2


' Først sikres mod at græsmarker pløjes op hvert år
	for i=0 to UBound(arrAllowedComb,1)
   	  If ((Not(IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_1"))))) And (IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_2")))) And (Not(IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_3")))))) Or ((Not(IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_2"))))) And (IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_3")))) And (Not(IsGraesID(arrAllowedComb(i,dictAllowedComb.Item("CropID_4")))))) Then
        arrAllowedComb(i,dictAllowedComb.Item("Allowed")) = 0
        arrAllowedComb(i,dictAllowedComb.Item("UB")) = 0

	  End If
    next

	for i=0 to UBound(arrAllowed3Comb,1)
   	  If (Not(IsGraesID(arrAllowed3Comb(i,dictAllowed3Comb.Item("CropID_1"))))) And (IsGraesID(arrAllowed3Comb(i,dictAllowed3Comb.Item("CropID_2")))) And (Not(IsGraesID(arrAllowed3Comb(i,dictAllowed3Comb.Item("CropID_3"))))) Then
        arrAllowed3Comb(i,dictAllowed3Comb.Item("Allowed")) = 0
	  End If
	next

end if

Tid3 = Timer
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
  index=index+1
next
for y=0 to (NProd-1)
  for i=0 to NumberCrops-1
    for h=0 to NumberCrops-1
      for j=0 to NumberCrops-1
        for k=0 to NumberCrops-1
        dObj(index) = Yield(h,j,k)
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
  dUB(index) = UBP2(index,2)
  index=index+1
Next

for i=0 to UBound(arrAllowed3Comb,1)
  if arrAllowed3Comb(i,dictAllowed3Comb.Item("Allowed"))=0 then
     dUB(index) = 0.0
  else 
     dUB(index) = 1000.0
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


Response.write " <br> Colstart "
PrintArrayInTable ColStart
Response.write " <br> matVal "
PrintArrayInTable matVal
Response.write " <br> RowInd "
PrintArrayInTable RowInd
Response.write " <br> dRHS "
PrintArrayInTable dRHS
Response.write " <br> dObj "
PrintArrayInTable dObj
Response.write " <br> sRowType "
PrintArrayInTable sRowType

Response.write " <br> dLB "
PrintArrayInTable dLB
Response.write " <br> dUB "
PrintArrayInTable dUB

Response.write " <br> qgtype "
PrintArrayInTable qgtype

Response.write " <br> mgcols "
PrintArrayInTable mgcols
Tid4 = Timer

dim oSoapClient

'Kræver at SoapToolkit 3.0 er installeret på serveren
set oSoapClient = server.CreateObject("MSSOAP.SoapClient30")

oSoapClient.ClientProperty("ServerHTTPRequest") = True
'oSoapClient.MSSoapInit("http://172.23.173.223/FarmN/webservice/FarmNOptimizer.asmx?WSDL")
oSoapClient.MSSoapInit("http://localhost/Webservice/FarmNOptimizer.asmx?WSDL")
oSoapClient.ConnectorProperty("Timeout") = 60000000

Dim result, LPStatus, ObjVal
'løser problemet som LP
'result = oSoapClient.OptimizeLP(sRowType, dRHS, dObj, dLB, dUB, ColStart, RowInd, matVal, LPStatus, ObjVal)
'Response.Write("<br>LPStatus " & LPStatus & "<br> " )
'Response.Write("ObjVal " & ObjVal & "<br> " )

'PrintArrayInTable result

Dim resultIP
'Løser problemet som IP
resultIP = oSoapClient.OptimizeGlobal(sRowType, dRHS, dObj, dLB, dUB, ColStart, RowInd, matVal, qgtype, mgcols, MIPStatus, ObjVal)
Response.Write("MIPStatus med Global  " & MIPStatus & "<br> ")
Response.Write("ObjVal " & ObjVal & "<br> " )
'PrintArrayInTable resultIP
Tid5 = Timer

set oSoapClient = nothing

'Response.write "<br> NumberCrops,NProd " & NumberCrops & " " & NProd
 ConvertResult xAarP2,xAarP1,xAar,resultIP,NumberCrops,NProd
Tid6=Timer

Response.write "<br> Tid 0-1 i LProtation " & Tid1-Tid0 

Response.write "<br> Tid 1-2 i LProtation " & Tid2-Tid1 
Response.write "<br> Tid 2-3 i LProtation " & Tid3-Tid2 
Response.write "<br> Tid 3-4 i LProtation " & Tid4-Tid3 
Response.write "<br> Tid 4-5 i LProtation " & Tid5-Tid4 
Response.write "<br> Tid 5-6 i LProtation " & Tid6-Tid5 

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
Dim l
Response.write "<br>  xAar " 
  Response.write "<br> "
For y=0 To NProd-1
  For i=0 To NumberCrops-1
    For j=0 To NumberCrops-1
      For k=0 To NumberCrops-1
        For l=0 To NumberCrops-1
		  If xAar(y,i,j,k,l) > 0 Then
		  Response.write "  xAar("&y&","&i&","&j&","&k&","&l&") = "& xAar(y,i,j,k,l)
		  End if
		Next
      Next
    Next
  Next
  Response.write "<br> "
Next

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

  Sub PrintArrayInTable2(anArray)
    Dim r,c
  Response.Write "<br><table border>" & Chr(13)
    For r = 0 to UBound(anArray,2)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(anArray,1)
        Response.Write "<td>" & CStr(anArray(c,r)) & "</td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)
  End Sub
'----------------------------------------------------------------
  Sub PrintArrayInTable(anArray)
    Dim c, k, rows, MaxCols
	MaxCols=20
	
	rows = Int((UBound(anArray,1)+1)/MaxCols)
    If (UBound(anArray,1)+1) Mod MaxCols > 0 Then rows = rows + 1
  
  Response.Write "<br><table border>" & Chr(13)
	  
	 For k=0 To rows-2
	  Response.Write "<tr>" & Chr(13)
      For c = k*MaxCols to (k+1)*MaxCols-1
        Response.Write "<td>" & CStr(c) & "</td>" & Chr(13)		
      Next
      Response.Write "</tr>" & Chr(13)

	  Response.Write "<tr>" & Chr(13)
      For c = k*MaxCols to (k+1)*MaxCols-1
        Response.Write "<td>" & CStr(anArray(c)) & "</td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)
      
	Next
	
    k = rows-1

	  Response.Write "<tr>" & Chr(13)
      For c = k*MaxCols to UBound(anArray,1)
        Response.Write "<td>" & CStr(c) & "</td>" & Chr(13)		
      Next
      Response.Write "</tr>" & Chr(13)

	  Response.Write "<tr>" & Chr(13)
      For c = k*MaxCols to UBound(anArray,1)
        Response.Write "<td>" & CStr(anArray(c)) & "</td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)

    Response.Write "</table>" & Chr(13)
  End Sub
'----------------------------------------------------------------

function IsGraesID(id)
Dim returnValue   
 If id = 19 Or id = 18 Or id = 12 Or id = 32 Then 
   returnValue = True
 Else 
   returnValue = False
 End if
   IsGraesID = returnValue
end function

%>