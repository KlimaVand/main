<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
Private Const NUMBER_FEED_COL = 11
Private Const NUMBER_FEED_ROW = 6
Private Const MIN_CONC = 0.016       ' Minimum concentrate of kg N/FE in feed (cereal)
Private Const MAX_CONC = 0.057       ' Maximum concentrate of kg N/FE in feed (soya)
'Private Const MIN_CONC = 0.008       ' Minimum concentrate of kg N/FE in feed IB
'Private Const MAX_CONC = 0.5       ' Maximum concentrate of kg N/FE in feed IB
'Private Const ANDEL_GROVFODER = 1   ' Maximum andel grovfoder IB


'**************************************************************************************************
function GetSoldManureTotal(nStorageType)
   dim returnValue, s
   
   for s = 1 to nStorageType
      returnValue = returnValue + GetNitrogenStableBeforeLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), s)
      returnValue = returnValue - GetNitrogenStableLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), s)
      returnValue = returnValue - GetNitrogenStorageLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), s)
      returnValue = returnValue - GetNitrogenSpreadLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), s)
      returnValue = returnValue - GetNitrogenFieldUse(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), s)
   next
   GetSoldManureTotal = returnValue
end function
'**************************************************************************************************
sub InitTableFeed(byref arrFeed, byref aBoughtFeedN, byref aSoldFeedN, byref HighEnergyConc, ByRef BoughtBeddingN, ByRef SoldBeddingN)
   dim concGrazingN, concRoughageN
   dim grazingYieldFE, grazingYieldDM, grazingYieldN, grazingConsumptionFE, grazingConsumptionN
   dim herdNeedFE, herdNeedDM, herdNeedN, fieldYieldFE, fieldYieldDM, totalConsumptionFE,totalConsumptionN
   dim internalGrazingFE, internalGrazingN
   dim roughageYieldFE, roughageYieldN, roughageConsumptionFE, roughageConsumptionN
   dim roughageDiffFE, roughageDiffN
   dim cerealYieldFE, cerealYieldN, cerealConsumptionFE, cerealConsumptionN
   dim Sold_FE_cereal, Sold_N_cereal, Buy_FE_feed, Buy_N_feed
   dim supplyFE, supplyN, supplyConsumptionFE, supplyConsumptionN
   dim PigNeedFE, PigNeedN
   Dim dictStrawNNeed, X, arrStrawNNeed, StrawDMNeed, StrawFENeed, StrawNNeed
   Dim cerealYieldDM, arrStrawSold, dictStrawSold
   Dim concCHG



   Dim StrawBeddingNeedN
   Dim StrawByeFE, StrawByeDM, StrawBuyN, StrawSoldFE, StrawSoldDM, StrawSoldN 
   Dim ByeDMPrFE, ByeNPrFE, arrStrawProduced, dictStrawProduced
   Dim i, j, a, b


   ReDim preserve arrParameter(2)    
   arrParameter(2) = 312
   a = CallSPReturn ("GetFarmTotal",arrParameter)
   arrParameter(2) = 412
   b = CallSPReturn ("GetFarmTotal",arrParameter)
   
   StrawBeddingNeedN  = Round(a + b)

   set dictStrawProduced = CreateObject("Scripting.Dictionary")
   X = CallSP ("GetSummarizedStrawBedding_Feed " & Session("farmNumber")& "," & Session("scenarioNumber") , arrStrawProduced, dictStrawProduced)
   set dictStrawSold = CreateObject("Scripting.Dictionary")
   X = CallSP ("GetSummarizedStrawIncorp_Sold " & Session("farmNumber")& "," & Session("scenarioNumber") , arrStrawSold, dictStrawSold)

'Response.write "<br> arrStrawProduced "
'PrintArrayInTable2Nina arrStrawProduced

   set dictStrawNNeed = CreateObject("Scripting.Dictionary")
   X = CallSP ("GetStrawNNeed " & Session("farmNumber")& "," & Session("scenarioNumber") , arrStrawNNeed, dictStrawNNeed)
   ' Hvis halm købes er det fodermiddel kode 781 som har 4% crude protein og 4.38 kg DM/FE
   ' N er 4 * 4.38 / 625 kg N/FE
   ByeDMPrFE = 4.38
   ByeNPrFE = 4 * 4.38 / 625

   Dim arrPStraw, dictPStraw
   ReDim arrPStraw(3,3)  'Første index er anvendelse 0-Foder, 1-Strøelse, 2-Foder/Strøelse, 3-Købt
                         'Andet index refererer til N, FE, DM og FEPrN
   set dictPStraw = CreateObject("Scripting.Dictionary")
   dictPStraw.Add CStr("N"),0
   dictPStraw.Add CStr("FE"),1
   dictPStraw.Add CStr("DM"),2
   dictPStraw.Add CStr("FEPrN"),3

   InitArray arrPStraw,0

   StrawNNeed = Round(arrStrawNNeed(0,dictStrawNNeed.Item("Straw")),2)

   For i=0 To UBound(arrStrawProduced)
     Select Case arrStrawProduced(i,dictStrawProduced.Item("UseStraw"))
       Case 1 ' strøelse og foder
         arrPStraw(2,dictPStraw.Item("N")) = Round(arrStrawProduced(i,dictStrawProduced.Item("Straw_KgN")),2)
         arrPStraw(2,dictPStraw.Item("FE")) = Round(arrStrawProduced(i,dictStrawProduced.Item("Straw_FE")),2)
         arrPStraw(2,dictPStraw.Item("DM")) = Round(arrStrawProduced(i,dictStrawProduced.Item("Straw_KgDrymatter")),2)
         If arrPStraw(2,dictPStraw.Item("N")) > 0 Then 
		 arrPStraw(2,dictPStraw.Item("FEPrN")) = CDbl(arrPStraw(2,dictPStraw.Item("FE"))/arrPStraw(2,dictPStraw.Item("N")))
		 Else 
         arrPStraw(2,dictPStraw.Item("FEPrN")) = 0
		 End If
		 
		 Case 2  ' kun strøelse 
         arrPStraw(1,dictPStraw.Item("N")) = Round(arrStrawProduced(i,dictStrawProduced.Item("Straw_KgN")),2)
         arrPStraw(1,dictPStraw.Item("FE")) = Round(arrStrawProduced(i,dictStrawProduced.Item("Straw_FE")),2)
         arrPStraw(1,dictPStraw.Item("DM")) = Round(arrStrawProduced(i,dictStrawProduced.Item("Straw_KgDrymatter")),2)

		 If arrPStraw(1,dictPStraw.Item("N")) > 0 Then 
		 arrPStraw(1,dictPStraw.Item("FEPrN")) = CDbl(arrPStraw(1,dictPStraw.Item("FE")))/CDbl(arrPStraw(1,dictPStraw.Item("N")))
		 Else 
         arrPStraw(1,dictPStraw.Item("FEPrN")) = 0
		 End If
		 
	   Case 4   ' kun foder
         arrPStraw(0,dictPStraw.Item("N")) = Round(arrStrawProduced(i,dictStrawProduced.Item("Straw_KgN")),2)
         arrPStraw(0,dictPStraw.Item("FE")) = Round(arrStrawProduced(i,dictStrawProduced.Item("Straw_FE")),2)
         arrPStraw(0,dictPStraw.Item("DM")) = Round(arrStrawProduced(i,dictStrawProduced.Item("Straw_KgDrymatter")),2)
		 If arrPStraw(0,dictPStraw.Item("N")) > 0 Then 
		 arrPStraw(0,dictPStraw.Item("FEPrN")) = CDbl(arrPStraw(0,dictPStraw.Item("FE"))/arrPStraw(0,dictPStraw.Item("N")))
		 Else 
         arrPStraw(0,dictPStraw.Item("FEPrN")) = 0
		 End If
		 
	   Case Else
	 End Select
   Next 
'Response.write "<br> arrPStraw "
'PrintArrayInTable2Nina arrPStraw

' Brug af transportproblem til afstemning af strøelse
  Dim ObjectCoeff, arrResult, arrA, arrB
	Dim SumStrawProduced, SumStrawNeed, test
	
	
	Redim ObjectCoeff(3,2)
	Redim arrResult(3,2)
	Redim arrA(3)
	Redim arrB(2)
	
    InitArray ObjectCoeff,-10000
    InitArray arrResult,-1
    SumStrawProduced = CDbl(arrPStraw(0,dictPStraw.Item("N"))) + CDbl(arrPStraw(1,dictPStraw.Item("N"))) + CDbl(arrPStraw(2,dictPStraw.Item("N")))
    SumStrawNeed = CDbl(StrawNNeed) + CDbl(StrawBeddingNeedN)

    StrawSoldN = 0 
    StrawBuyN  = 0
    
	If CDbl(SumStrawProduced) >= CDbl(SumStrawNeed) Then
	  StrawSoldN = CDbl(SumStrawProduced) - CDbl(SumStrawNeed)
	Else 
	  StrawBuyN = CDbl(SumStrawNeed) - CDbl(SumStrawProduced)
	  arrPStraw(3,dictPStraw.Item("N")) = CDbl(StrawBuyN)
	End If
	
'Response.write "<br> arrA "
'PrintArrayInTableNina arrA
'Response.write "<br> arrB "
'PrintArrayInTableNina arrB

 For i=0 To UBound(arrA)
   arrA(i) = CDbl(FormatNumber(arrPStraw(i,dictPStraw.Item("N")),2))
'   Response.write "<br> Data type "& i & " " & VarType(arrA(i))
 Next


 arrB(0)=FormatNumber(StrawNNeed,2)     ' Behov til foder
 arrB(1)=FormatNumber(StrawBeddingNeedN,2)  ' Behov til strøelse
 arrB(2)=FormatNumber(StrawSoldN,2)     ' Solgt strøelse
 
' Response.write "<br> arrA "
'PrintArrayInTableNina arrA
'Response.write "<br> arrB "
'PrintArrayInTableNina arrB

 ObjectCoeff(0,0)=CDbl(2)
 ObjectCoeff(0,2)=CDbl(1)
 ObjectCoeff(1,1)=CDbl(2)
 ObjectCoeff(1,2)=CDbl(1)
 ObjectCoeff(2,0)=CInt(2)
 ObjectCoeff(2,1)=CInt(3)
 ObjectCoeff(2,2)=CInt(1)
 ObjectCoeff(3,0)=CInt(1)
 ObjectCoeff(3,1)=CInt(1)
 ObjectCoeff(3,2)=CInt(1)



'For i=0 To UBound(arrA)
'  Response.write "<br> Data type "& i & " " & VarType(arrA(i))
'Next


    test = calcRotation(arrA,arrB,ObjectCoeff,arrResult)

    do until test = true
	   for a=0 to Ubound(arrA)
	    for b=0 to Ubound(arrB)
	     if arrResult(a,b)<>0 and ObjectCoeff(a,b)<-1000 then 
    	       StrawBuyN = StrawBuyN + Round(arrResult(a,b),2)
			   StrawSoldN = StrawSoldN + Round(arrResult(a,b),2)
               arrA(a) = arrA(a)' - Round(arrResult(a,b),2)
			   arrB(b) = arrB(b)' - Round(arrResult(a,b),2)
    	       arrA(Ubound(arrA)) = arrA(Ubound(arrA)) + Round(arrResult(a,b),2)
    	       arrB(Ubound(arrB)) = arrB(Ubound(arrB)) + Round(arrResult(a,b),2)
         End If
        next
       next
    test = calcRotation(arrA,arrB,ObjectCoeff,arrResult)
    loop
'Response.write "<br> arrResult "
'PrintArrayInTable2Nina arrResult

    arrPStraw(3,dictPStraw.Item("N")) = Round(StrawBuyN,2)
    arrPStraw(3,dictPStraw.Item("FE")) = Round(StrawBuyN / ByeNPrFE,2)
    arrPStraw(3,dictPStraw.Item("DM")) = Round(StrawBuyN/ ByeNPrFE*ByeDMPrFE,2)
    If arrPStraw(3,dictPStraw.Item("N")) > 0 Then
    arrPStraw(3,dictPStraw.Item("FEPrN")) = Round(arrPStraw(3,dictPStraw.Item("FE"))/arrPStraw(3,dictPStraw.Item("N")),2)
	Else 
    arrPStraw(3,dictPStraw.Item("FEPrN")) = 0
	End If
	
StrawDMNeed = 0
StrawFENeed = 0

For i=0 To UBound(arrA)
  If arrResult(i,0) > 0 Then
    StrawDMNeed = StrawDMNeed + arrResult(i,0) * arrPStraw(i,dictPStraw.Item("DM"))/arrPStraw(i,dictPStraw.Item("N"))
    StrawFENeed = StrawFENeed + arrResult(i,0) * arrPStraw(i,dictPStraw.Item("FEPrN"))
  End If 
Next 

Dim StrawFeedNProduced, StrawFeedFEProduced, StrawFeedNBought
Dim StrawFeedFEBought, StrawFeedNSold, StrawFeedFESold

StrawFeedNProduced = arrResult(0,0) + arrResult(1,0) + arrResult(2,0) + arrResult(0,2)
StrawFeedNBought = arrResult(3,0)
StrawFeedFEProduced = arrResult(0,0) * arrPStraw(0,dictPStraw.Item("FEPrN")) + arrResult(1,0) * arrPStraw(1,dictPStraw.Item("FEPrN")) + arrResult(2,0) * arrPStraw(2,dictPStraw.Item("FEPrN")) + arrResult(0,2) * arrPStraw(0,dictPStraw.Item("FEPrN"))
StrawFeedFEBought = arrResult(3,0) * arrPStraw(3,dictPStraw.Item("FEPrN"))

StrawFeedNSold = arrResult(0,2)
StrawFeedFESold = arrResult(0,2) * arrPStraw(0,dictPStraw.Item("FEPrN"))

BoughtBeddingN = arrResult(3,1)
SoldBeddingN = arrResult(1,2) + arrResult(2,2)
'	 response.write "<br> BoughtBeddingN " & BoughtBeddingN
'     response.write "<br> SoldBeddingN " & SoldBeddingN
'	 response.write "<br> StrawFeedNSold " & StrawFeedNSold
'     response.write "<br> StrawFeedNBought " & StrawFeedNBought

SoldBeddingN = SoldBeddingN + arrStrawSold(0,dictStrawSold.Item("Straw_KgN_Sold"))
'	 response.write "<br> Efter eksternt salg " 
'     response.write "<br> SoldBeddingN " & SoldBeddingN



    Dim dictGrazingYield, arrGrazingYield2
	set dictGrazingYield = CreateObject("Scripting.Dictionary")
	X = CallSP ("GetSummarizedGrazingYield_ny " & Session("farmNumber")& "," & Session("scenarioNumber") , arrGrazingYield2, dictGrazingYield)
    Dim dictNonGrazingYield, arrNonGrazingYield2
	set dictNonGrazingYield = CreateObject("Scripting.Dictionary")
	X = CallSP ("GetSummarizedNonGrazedYield " & Session("farmNumber")& "," & Session("scenarioNumber") , arrNonGrazingYield2, dictNonGrazingYield)
    Dim dictCerealYield, arrCerealYield2
	set dictCerealYield = CreateObject("Scripting.Dictionary")
	X = CallSP ("GetSummarizedCerealYield " & Session("farmNumber")& "," & Session("scenarioNumber") , arrCerealYield2, dictCerealYield)
    Dim dictNonCerealYield, arrNonCerealYield2
	set dictNonCerealYield = CreateObject("Scripting.Dictionary")
	X = CallSP ("GetSummarizedNonCerealYield " & Session("farmNumber")& "," & Session("scenarioNumber") , arrNonCerealYield2, dictNonCerealYield)
    Dim dictSecondCropYield, arrSecondCropYield
	set dictSecondCropYield = CreateObject("Scripting.Dictionary")
	X = CallSP ("GetSummarizedSecondCropYield " & Session("farmNumber")& "," & Session("scenarioNumber") , arrSecondCropYield, dictSecondCropYield)


'  response.write "<br>GrazingYield"
'  print2 arrGrazingYield2, dictGrazingYield

'  response.write "<br>NonGrazingYield"
'  print2 arrNonGrazingYield2, dictNonGrazingYield

'  response.write "<br>CerealYield"
'  print2 arrCerealYield2, dictCerealYield
  
'  response.write "<br>NonCerealYield"
'  print2 arrNonCerealYield2, dictNonCerealYield
  
'  response.write "<br>SecondCropYield"
'  print2 arrSecondCropYield, dictSecondCropYield


'dim a
'a = dictGrazingYield.Keys
'for i = 0 to dictGrazingYield.Count -1
'  response.write a(i)&" -- "
'  response.write arrGrazingYield2(0,dictGrazingYield.Item(a(i)))&"<br/>"
'next
'  response.write "<br> ----------------<br>"
'  response.write "<br> NonGrazingYield<br>"
'a = dictNonGrazingYield.Keys
'for i = 0 to dictNonGrazingYield.Count -1
'  response.write a(i)&" -- "
'  response.write arrNonGrazingYield2(0,dictNonGrazingYield.Item(a(i)))&"<br/>"
'next
'  response.write "<br> ----------------<br>"
'  response.write "<br> CerealYield<br>"
'a = dictCerealYield.Keys
'for i = 0 to dictCerealYield.Count -1
'  response.write a(i)&" -- "
'  response.write arrCerealYield2(0,dictCerealYield.Item(a(i)))&"<br/>"
'next
'  response.write "<br> ----------------<br>"
'  response.write "<br>NonCerealYield <br>"
'a = dictNonCerealYield.Keys
'for i = 0 to dictNonCerealYield.Count -1
'  response.write a(i)&" -- "
'  response.write arrNonCerealYield2(0,dictNonCerealYield.Item(a(i)))&"<br/>"
'next
'  response.write "<br> ----------------<br>"



   grazingYieldFE       = CDbl(arrGrazingYield2(0,dictGrazingYield.Item("Cernal_FE")))
   grazingYieldDM       = CDbl(arrGrazingYield2(0,dictGrazingYield.Item("CernalKgDryMatter")))

'   response.write "<br> grazingYieldFE " &grazingYieldFE

   grazingYieldN        = CDbl(arrGrazingYield2(0,dictGrazingYield.Item("CernalKgN"))) 
'   If grazingYieldN="" Then grazingYieldN = 0

   arrParameter(2) = PARAMETER_FE_GRAZING
   grazingConsumptionFE = CallSPReturn ("GetFarmTotal",arrParameter)

'   grazingConsumptionFE = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_FE_GRAZING)
  arrParameter(2) = PARAMETER_FE
   herdNeedFE = CallSPReturn ("GetFarmTotal",arrParameter)
'   herdNeedFE         = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_FE)
   arrParameter(2) = PARAMETER_TS
   herdNeedDM = CallSPReturn ("GetFarmTotal",arrParameter)
'   herdNeedDM         = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_TS)
   arrParameter(2) = PARAMETER_N
   herdNeedN = CallSPReturn ("GetFarmTotal",arrParameter)
'   herdNeedN          = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_N)
'   response.write "<br> concHerdDM før halm " & FormatNumber(herdNeedDM/herdNeedFE,4)

   If grazingYieldFE> 0 Then 
      herdNeedFE = herdNeedFE - StrawFENeed - grazingConsumptionFE ' Besætningens behov efter halm
      herdNeedDM = herdNeedDM - StrawDMNeed - grazingConsumptionFE * grazingYieldDM/grazingYieldFE' Besætningens behov efter halm
   Else 
   herdNeedFE = herdNeedFE - StrawFENeed  ' Besætningens behov efter halm
   herdNeedDM = herdNeedDM - StrawDMNeed
   End If 
   herdNeedN = herdNeedN - StrawNNeed    ' Besætningens behov efter halm

   fieldYieldFE       = CDbl(arrNonGrazingYield2(0,dictNonGrazingYield.Item("Cernal_FE")))  + CDbl(arrNonCerealYield2(0,dictNonCerealYield.Item("Cernal_FE"))) + CDbl(arrSecondCropYield(0,dictSecondCropYield("SecondCrop_FE")))
'   fieldYieldFE       = CDbl(arrGrazingYield2(0,dictGrazingYield.Item("Cernal_FE")))  + CDbl(arrNonGrazingYield2(0,dictNonGrazingYield.Item("Cernal_FE")))  + CDbl(arrNonCerealYield2(0,dictNonCerealYield.Item("Cernal_FE"))) + CDbl(arrSecondCropYield(0,dictSecondCropYield("SecondCrop_FE")))
   ' FE i Produceret grovfoder incl afgræsning
   
   fieldYieldDM       = CDbl(arrNonGrazingYield2(0,dictNonGrazingYield.Item("CernalKgDryMatter")))  + CDbl(arrNonCerealYield2(0,dictNonCerealYield.Item("CernalKgDryMatter"))) + CDbl(arrSecondCropYield(0,dictSecondCropYield("SecondCropKgDryMatter"))) 
'   fieldYieldDM       = CDbl(arrGrazingYield2(0,dictGrazingYield.Item("CernalKgDryMatter"))) + CDbl(arrNonGrazingYield2(0,dictNonGrazingYield.Item("CernalKgDryMatter")))  + CDbl(arrNonCerealYield2(0,dictNonCerealYield.Item("CernalKgDryMatter"))) + CDbl(arrSecondCropYield(0,dictSecondCropYield("SecondCropKgDryMatter"))) 
   ' DM i Produceret grovfoder incl afgræsning
   
   roughageYieldFE       = CDbl(arrNonGrazingYield2(0,dictNonGrazingYield.Item("Cernal_FE"))) + CDbl(arrNonCerealYield2(0,dictNonCerealYield.Item("Cernal_FE"))) + CDbl(arrSecondCropYield(0,dictSecondCropYield("SecondCrop_FE")))
   
   roughageYieldN        = CDbl(arrNonGrazingYield2(0,dictNonGrazingYield.Item("CernalKgN")))  + CDbl(arrNonCerealYield2(0,dictNonCerealYield.Item("CernalKgN")))  + CDbl(arrSecondCropYield(0,dictSecondCropYield("SecondCropKgN")))

   cerealYieldFE       = CDbl(arrCerealYield2(0,dictCerealYield.Item("Cernal_FE")))

   cerealYieldN        = CDbl(arrCerealYield2(0,dictCerealYield.Item("CernalKgN")))

   cerealYieldDM       = CDbl(arrCerealYield2(0,dictCerealYield.Item("CernalKgDryMatter")))
  
'  response.write "<br> cerealYieldFE " & cerealYieldFE
'  response.write "<br> cerealYieldDM " & cerealYieldDM
   
'  response.write "<br> grazingYieldFE " & grazingYieldFE
'  response.write "<br> grazingYieldN " & grazingYieldN
'   response.write "<br> fieldYieldFE " & fieldYieldFE
'   response.write "<br> fieldYieldDM " & fieldYieldDM
'   response.write "<br> roughageYieldFE " & roughageYieldFE
'   response.write "<br> roughageYieldN " & roughageYieldN

   Dim concHerdDM, concFieldDM, concCerealDM
   If herdNeedFE>0 Then
   concHerdDM = herdNeedDM/herdNeedFE
   Else 
   concHerdDM = 0.9
   End If
   If fieldYieldFE>0 Then
   concFieldDM = fieldYieldDM/fieldYieldFE
   Else 
   concFieldDM = 1.5
   End If
   If cerealYieldFE>0 Then
   concCerealDM = 0.95
   'concCerealDM = cerealYieldDM/cerealYieldFE
   Else 
   concCerealDM = 0.95
   End If
   
'   response.write "<br> concHerdDM " & FormatNumber(concHerdDM,4)
'   response.write "<br> concFieldDM " & FormatNumber(concFieldDM,4)
'   response.write "<br> concCerealDM " & FormatNumber(concCerealDM,4)


   totalConsumptionFE = CalcUsedFE(concHerdDM, concFieldDM, concCerealDM, herdNeedFE, HighEnergyConc) 'grovfoder ex. afgræsning
'   response.write "<br> totalConsumptionFE " & totalConsumptionFE
   If grazingYieldFE> 0 Then 
      totalConsumptionFE = totalConsumptionFE + grazingConsumptionFE ' total grovfoder
      herdNeedFE = herdNeedFE + grazingConsumptionFE ' Besætningens behov efter halm
      herdNeedDM = herdNeedDM + grazingConsumptionFE * grazingYieldDM/grazingYieldFE' Besætningens behov efter halm
   End If 





   if grazingYieldFE<>0 then
     concGrazingN         = grazingYieldN/grazingYieldFE
   else
     concGrazingN  = 0
   end if
   if roughageYieldFE<>0 then
     concRoughageN         = roughageYieldN/roughageYieldFE
   else
     concRoughageN  = 0.03
   end If
'   response.write "<br> concHerdN " & FormatNumber(herdNeedN/herdNeedFE,4)
'   response.write "<br> concGrazingN " & FormatNumber(concGrazingN,4)
'   response.write "<br> concRoughageN " & FormatNumber(concRoughageN,4)

   internalGrazingFE = grazingYieldFE - grazingConsumptionFE

   if  internalGrazingFE < 0 then
     grazingConsumptionN  = grazingYieldFE*concGrazingN + (grazingConsumptionFE-grazingYieldFE)*concRoughageN
   else 
     grazingConsumptionN  = grazingConsumptionFE*concGrazingN
   end if

   
   roughageConsumptionFE = totalConsumptionFE - grazingConsumptionFE
   roughageDiffFE = roughageYieldFE + internalGrazingFE - roughageConsumptionFE
   roughageDiffN  = roughageDiffFE*concRoughageN
   'hvis grovfoder købes, så købes grønpiller med en N-konc på 43,2 gN pr FE
'     response.write "<br> roughageDiffN " & FormatNumber(roughageDiffN,4)
'   If roughageDiffFE < 0 Then 
'   roughageDiffN  = roughageDiffFE*0.0432
'   End If 
'      response.write "<br> roughageDiffN " & FormatNumber(roughageDiffN,4)

   
'      response.write "<br> TEst " & roughageYieldN-roughageDiffN
   if  internalGrazingFE < 0 then
     roughageConsumptionN  = roughageYieldN-roughageDiffN
   else 
     roughageConsumptionN  = roughageYieldN-roughageDiffN + internalGrazingFE*(concGrazingN-concRoughageN)
   end if

   totalConsumptionN   = grazingConsumptionN + roughageConsumptionN
'      response.write "<br> Grovfoder N " & totalConsumptionN

   CalcSoldBoughtFeed herdNeedFE,herdNeedN,totalConsumptionFE,totalConsumptionN,cerealYieldFE,cerealYieldN,Sold_FE_cereal,Sold_N_cereal,Buy_FE_feed,Buy_N_feed

  
   cerealConsumptionFE = cerealYieldFE - Sold_FE_cereal 
    if cerealYieldFE<>0 then
	cerealConsumptionN  = cerealYieldN*cerealConsumptionFE/cerealYieldFE
	else
    cerealConsumptionN=0
	end if
   supplyFE            = Buy_FE_feed
   supplyN             = Buy_N_feed
   supplyConsumptionFE = supplyFE
   supplyConsumptionN  = supplyN

'hvis der er for lidt kvælstof i kvægfodringen så forsøger vi at sætte N konc i indkøbt grovfoder op.
'response.write "<br> TEst af ind i løkken " & totalConsumptionN+StrawNNeed+cerealConsumptionN+supplyConsumptionN &" " &  GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_N)

   arrParameter(2) = PARAMETER_N
   a =  CallSPReturn ("GetFarmTotal",arrParameter)

   If FormatNumber(totalConsumptionN+StrawNNeed+cerealConsumptionN+supplyConsumptionN,1) <> FormatNumber(a,1) Then
'     response.write "<br> roughageDiffN " & roughageDiffN &" roughageDiffFE "& roughageDiffFE
    concCHG = 0 
   If roughageDiffFE < 0 Then 
      concCHG = (a-(totalConsumptionN+StrawNNeed+cerealConsumptionN+supplyConsumptionN))/roughageDiffFE
      concRoughageN = concRoughageN - concCHG
'   response.write "<br> concRoughageN " & concRoughageN
        If concRoughageN > 0.0432 Then
           concRoughageN = 0.0432
		End If 

   roughageDiffN  = roughageDiffFE*concRoughageN
   End If 
'      response.write "<br> roughageDiffN " & roughageDiffN

   
'      response.write "<br> internalGrazingFE (concGrazingN-concRoughageN) " & internalGrazingFE & " " & concGrazingN & " " & concRoughageN
   if  internalGrazingFE < 0 then
     roughageConsumptionN  = roughageYieldN-roughageDiffN
   else 
     roughageConsumptionN  = roughageYieldN-roughageDiffN + internalGrazingFE*(concGrazingN-concRoughageN-concCHG)
   end if
'      response.write "<br> roughageConsumptionN " & roughageConsumptionN

   totalConsumptionN   = grazingConsumptionN + roughageConsumptionN

   CalcSoldBoughtFeed herdNeedFE,herdNeedN,totalConsumptionFE,totalConsumptionN,cerealYieldFE,cerealYieldN,Sold_FE_cereal,Sold_N_cereal,Buy_FE_feed,Buy_N_feed

  
   cerealConsumptionFE = cerealYieldFE - Sold_FE_cereal 
    if cerealYieldFE<>0 then
	cerealConsumptionN  = cerealYieldN*cerealConsumptionFE/cerealYieldFE
	else
    cerealConsumptionN=0
	end if
   supplyFE            = Buy_FE_feed
   supplyN             = Buy_N_feed
   supplyConsumptionFE = supplyFE
   supplyConsumptionN  = supplyN

   End If 

   ' Grazing
   arrFeed(0,0)  = "Afgræsning"                             ' Text
   arrFeed(0,1)  = grazingYieldFE                           ' Produced energy (FE) on field
   arrFeed(0,2)  = grazingYieldN                            ' Produced nitrogen (Kg N) on field
   arrFeed(0,3)  = 0                                        ' Bought energy (FE)
   arrFeed(0,4)  = 0                                        ' Bought nitrogen (Kg N)
   arrFeed(0,5)  = 0                                        ' Sold energy (FE)
   arrFeed(0,6)  = 0                                        ' Sold nitrogen (Kg N)
   arrFeed(0,7)  = grazingConsumptionFE                     ' Energy consumption herd
   arrFeed(0,8)  = grazingConsumptionN                      ' Nitrogen consumption herd
   arrFeed(0,9)  = -internalGrazingFE                       ' Internal turnover
   arrFeed(0,10) = 0                                        ' Balance

   ' Other roughage   
   arrFeed(1,0)  = "Grovfoder"                               ' Text
   arrFeed(1,1)  = roughageYieldFE                           ' Produced energy (FE) on field
   arrFeed(1,2)  = roughageYieldN                            ' Produced nitrogen (Kg N) on field
   if roughageDiffFE < 0 then                                ' Bought energy (FE)
      arrFeed(1,3) = -roughageDiffFE
   else
      arrFeed(1,3) = 0
   end if 
   if roughageDiffFE < 0 then                                ' Bought nitrogen (Kg N)
      arrFeed(1,4) = -roughageDiffN
   else
      arrFeed(1,4) = 0
   end if
   if roughageDiffFE > 0 then                                ' Sold energy (FE)
      arrFeed(1,5) = roughageDiffFE
   else
      arrFeed(1,5) = 0
   end if
   if roughageDiffFE > 0 then                                ' Sold nitrogen (Kg N)
      arrFeed(1,6) = roughageDiffN
   else
      arrFeed(1,6) = 0
   end if
   arrFeed(1,7)  = roughageConsumptionFE                     ' Energy consumption herd
   arrFeed(1,8)  = roughageConsumptionN                      ' Nitrogen consumption herd
   arrFeed(1,9)  = internalGrazingFE                         ' Internal turnover
   arrFeed(1,10) = 0                                         ' Balance
   
   arrFeed(2,0)  = "Foderhalm"     ' Text
   arrFeed(2,1)  = StrawFeedFEProduced ' Produced energy (FE) on field
   arrFeed(2,2)  = StrawFeedNProduced  ' Produced nitrogen (Kg N) on field
   arrFeed(2,3)  = StrawFeedFEBought   ' Bought energy (FE)
   arrFeed(2,4)  = StrawFeedNBought    ' Bought nitrogen (Kg N)
   arrFeed(2,5)  = StrawFeedFESold     ' Sold energy (FE)
   arrFeed(2,6)  = StrawFeedNSold      ' Sold nitrogen (Kg N)
   arrFeed(2,7)  = StrawFENeed         ' Energy consumption herd
   arrFeed(2,8)  = StrawNNeed          ' Nitrogen consumption herd
   arrFeed(2,9)  = 0                   ' Internal turnover
   arrFeed(2,10) = 0                   ' Balance
   

   ' Cereal  
   arrFeed(3,0)  = "Korn" ' Text
   arrFeed(3,1)  = cerealYieldFE       ' Produced energy (FE) on field
   arrFeed(3,2)  = cerealYieldN        ' Produced nitrogen (Kg N) on field
   arrFeed(3,3)  = 0                   ' Bought energy (FE)
   arrFeed(3,4)  = 0                   ' Bought nitrogen (Kg N)
   arrFeed(3,5)  = Sold_FE_cereal      ' Sold energy (FE)
   arrFeed(3,6)  = Sold_N_cereal       ' Sold nitrogen (Kg N)
   arrFeed(3,7)  = cerealConsumptionFE ' Energy consumption herd
   arrFeed(3,8)  = cerealConsumptionN  ' Nitrogen consumption herd
   arrFeed(3,9)  = 0                   ' Internal turnover
   arrFeed(3,10) = 0                   ' Balance
   
   
   ' Supplementary
   arrFeed(4,0)  = "Tilskudsfoder"     ' Text
   arrFeed(4,1)  = 0                   ' Produced energy (FE) on field
   arrFeed(4,2)  = 0                   ' Produced nitrogen (Kg N) on field
   arrFeed(4,3)  = supplyFE            ' Bought energy (FE)
   arrFeed(4,4)  = supplyN             ' Bought nitrogen (Kg N)
   arrFeed(4,5)  = 0                   ' Sold energy (FE)
   arrFeed(4,6)  = 0                   ' Sold nitrogen (Kg N)
   arrFeed(4,7)  = supplyConsumptionFE ' Energy consumption herd
   arrFeed(4,8)  = supplyConsumptionN  ' Nitrogen consumption herd
   arrFeed(4,9)  = 0                   ' Internal turnover
   arrFeed(4,10) = 0                   ' Balance
   
   arrParameter(2) = 402
   PigNeedFE = CallSPReturn ("GetFarmTotal",arrParameter)
'   PigNeedFE         = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 402)
   arrParameter(2) = 404
   PigNeedN = CallSPReturn ("GetFarmTotal",arrParameter)
'   PigNeedN          = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 404)

   arrFeed(5,0)  = "Grisefoder"     ' Text
   arrFeed(5,1)  = 0                   ' Produced energy (FE) on field
   arrFeed(5,2)  = 0                   ' Produced nitrogen (Kg N) on field
   arrFeed(5,3)  = PigNeedFE            ' Bought energy (FE)
   arrFeed(5,4)  = PigNeedN             ' Bought nitrogen (Kg N)
   arrFeed(5,5)  = 0                   ' Sold energy (FE)
   arrFeed(5,6)  = 0                   ' Sold nitrogen (Kg N)
   arrFeed(5,7)  = PigNeedFE ' Energy consumption herd
   arrFeed(5,8)  = PigNeedN  ' Nitrogen consumption herd
   arrFeed(5,9)  = 0                   ' Internal turnover
   arrFeed(5,10) = 0                   ' Balance

   ' Total
   dim r,c
   arrFeed(NUMBER_FEED_ROW,0) = "I alt"
   for r = 0 to NUMBER_FEED_ROW-1
      for c = 1 to NUMBER_FEED_COL-1
         arrFeed(NUMBER_FEED_ROW,c) = arrFeed(NUMBER_FEED_ROW,c) + arrFeed(r,c)
      next
   next
   
   aBoughtFeedN = arrFeed(NUMBER_FEED_ROW,4)
   aSoldFeedN   = arrFeed(NUMBER_FEED_ROW,6)

end sub
'**************************************************************************************************
function CalcUsedFE(concHerdDM, concFieldDM, concCerealDM, HerdFE, byref HighEnergyConc)
 Dim ANDEL_GROVFODER
'   response.write "<br> concHerdDM " & concHerdDM
'   response.write "<br> concFieldDM " & concFieldDM
'   response.write "<br> concCerealDM " & concCerealDM
'   response.write "<br> HerdFE " & HerdFE
 HighEnergyConc = 0
 If concCerealDM <= concHerdDM <= concFieldDM And HerdFE > 0 Then 
    ANDEL_GROVFODER = 1 - (concFieldDM - concHerdDM)/(concFieldDM - concCerealDM)
 '   HighEnergyConc = 0
 End If
 If concHerdDM < concCerealDM And HerdFE > 0 Then
    ANDEL_GROVFODER = 0
    HighEnergyConc = -1
 End If
 If concHerdDM > concFieldDM And HerdFE > 0 Then 
    ANDEL_GROVFODER = 1
    HighEnergyConc = 1
 End If 
   


'   response.write "<br> ANDEL_GROVFODER " & ANDEL_GROVFODER

      CalcUsedFE = ANDEL_GROVFODER * HerdFE

'      response.write "<br> CalcUsedFE " & CalcUsedFE

'  response.write "<br> HighEnergyConc " & HighEnergyConc


end function
'**************************************************************************************************

sub CalcSoldBoughtFeed(herdNeedFE,herdNeedN,totalConsumptionFE,totalConsumptionN,cerealYieldFE,cerealYieldN,Sold_FE_cereal,Sold_N_cereal,Buy_FE_feed,Buy_N_feed)
   dim concN,concCerealN,a,concBuy

   if totalConsumptionFE <> herdNeedFE then 
     concN = (herdNeedN - totalConsumptionN)/(herdNeedFE-totalConsumptionFE)
   else 
     concN = 0
   end if
   if cerealYieldFE > 0 then
     concCerealN = cerealYieldN/cerealYieldFE
   else 
     concCerealN = MIN_CONC
   end if
'   concN=0.1
'   concCerealN=0.023
'   response.write "<br> concN " & FormatNumber(concN,4) 
'   response.write "<br> concCerealN " & FormatNumber(concCerealN,4) 
'   response.write "<br> MIN_CONC " & MIN_CONC 
'   response.write "<br> MAX_CONC " & MAX_CONC 

   if concN < MIN_CONC then           ' Tilfældet hvor concentration af kvæstof er mindre end det som er muligt
     Sold_FE_cereal =  cerealYieldFE - ( herdNeedFE-totalConsumptionFE )
     Sold_N_cereal = Sold_FE_cereal*concCerealN
       if Sold_FE_cereal < 0 then 
          Buy_FE_feed = - Sold_FE_cereal
          Buy_N_feed = Buy_FE_feed*MIN_CONC
	  Sold_FE_cereal = 0
          Sold_N_cereal = 0
       else 
          Buy_FE_feed = 0
	  Buy_N_feed = 0
       end if
   else                               ' Tilfældet hvor koncentration af kvælstof i rest foderet er større end minimum
                                      ' Der er ikke taget højde for tilfældet hvor koncentrationen er større end max
									  ' Da bliver ikke alle foderenheder dækket.
       if concCerealN <= concN then   'Finder fordeling af eget korn og 
         a = (concN - MAX_CONC)/(concCerealN - MAX_CONC)
	 concBuy = MAX_CONC
       else
         a = (concN - MIN_CONC) / (concCerealN - MIN_CONC)
	 concBuy = MIN_CONC
       end if
'       response.write "<br> a  " & a 
       
	   if a < 0 then                  ' Hvis concN > MAX_CONC bliver a < 0
	     a = 0
	   end if

       Sold_FE_cereal =  cerealYieldFE - a* ( herdNeedFE-totalConsumptionFE )
       Sold_N_cereal = Sold_FE_cereal*concCerealN
       Buy_FE_feed = (1-a) * ( herdNeedFE-totalConsumptionFE )
	   Buy_N_feed = Buy_FE_feed*concBuy
       
	   if Sold_FE_cereal < 0 then
	     Buy_FE_feed = Buy_FE_feed - Sold_FE_cereal
		 Buy_N_feed = Buy_N_feed - Sold_N_cereal
	     Sold_FE_cereal = 0
		 Sold_N_cereal = 0
       end if
   end if
   
end sub


%>