<%
'**************************************************************************************************
' Author: Bjørn Molt Petersen, Danish Institute of Agricultural Sciences
' Written: 2003-2004
' Function: The function returns a floating point number giving the total estimated 
' denitrification (kg N) using Finn P. Vinthers method (version 2.0)
' Parameters: 
' intSoilCode is an integer according to the danish JB classification system
' intFarmType is an integer: 
'    1 = cash crop farm (planteavler)
'    2 = pig farm (svineavler)
'    3 = dairy farm (kvægavler)
' fltFertiliserN is a double, giving the amount of N from mineral fertiliser in kg
' fltManureN is a floating point number, giving the amount of total N from manure in kg
' fltNFixation is a floating point number, giving the total amount of N fixatet from the atmosphere in kg
'**************************************************************************************************
function DenitrificationPrRotation(intSoilCode, intFarmType, fltFertiliserN, fltManureNincorp, fltManureNspread, fltNFixation)
   
   'dim fltCinSoil - later '
'Response.write "<br> Soilcode: " & intSoilCode
'Response.write "<br> intFarmType: " & intFarmType
'Response.write "<br> fltFertiliserN: " & fltFertiliserN
'Response.write "<br> fltManureNincorp: " & fltManureNincorp
'Response.write "<br> fltManureNspread: " &fltManureNspread 
'Response.write "<br> fltNFixation: " & fltNFixation
'Response.write "<br> Soilcode: " & intSoilCode
   dim intOrgLevel
   '1 = low, 2 = medium, 3 = high
   intOrgLevel = intFarmType'Iflg. aftale med Finn Vinther' bedriftstype: intFarmType
'   intOrgLevel = 2'oprindelig indstilling
   'Change to dependency on organic matter level later!!!
   dim fltBackgr
   dim fltRatio
   fltBackgr = 0
   fltRatio = 5 
   if intOrgLevel = 1 or intOrgLevel = 2 then
      if intSoilCode  = 1 then fltBackgr = 0.0'0.0
      if intSoilCode  = 2 then fltBackgr = 0.5'0.5
      if intSoilCode  = 3 then fltBackgr = 1.4'1
      if intSoilCode  = 4 then fltBackgr = 2.8'3
      if intSoilCode  = 5 then fltBackgr = 4.8'6
      if intSoilCode  = 6 then fltBackgr = 7.3'6
      if intSoilCode  = 7 then fltBackgr = 10.2'12
      if intSoilCode >= 8 then fltBackgr = 14.0

      if intSoilCode  = 1 then fltRatio = 0.0'0.0
      if intSoilCode  = 2 then fltRatio = 0.5'0.5
      if intSoilCode  = 3 then fltRatio = 1.5'1.5
      if intSoilCode  = 4 then fltRatio = 2.5'3.0
      if intSoilCode  = 5 then fltRatio = 3.0'4.0
      if intSoilCode  = 6 then fltRatio = 4.0'4.0
      if intSoilCode  = 7 then fltRatio = 5.0'5.0
      if intSoilCode >= 8 then fltRatio = 6.0'5.0
   end if

'   if intOrgLevel = 2 then
'      if intSoilCode  = 1 then fltBackgr = 0.3'0.3
'      if intSoilCode  = 2 then fltBackgr = 1.0'1
'      if intSoilCode  = 3 then fltBackgr = 2.3'2
'      if intSoilCode  = 4 then fltBackgr = 4.5'5
'      if intSoilCode  = 5 then fltBackgr = 7.5'9
'      if intSoilCode  = 6 then fltBackgr = 10.5'9
'      if intSoilCode  = 7 then fltBackgr = 14.0'18
'      if intSoilCode >= 8 then fltBackgr = 20.0
'
'      if intSoilCode  = 1 then fltRatio = 0.3'0.3
'      if intSoilCode  = 2 then fltRatio = 1.0'1.0
'      if intSoilCode  = 3 then fltRatio = 2.0'2.0
'      if intSoilCode  = 4 then fltRatio = 3.5'3.5
'      if intSoilCode  = 5 then fltRatio = 4.0'5.0
'      if intSoilCode  = 6 then fltRatio = 5.0'5.0
'      if intSoilCode  = 7 then fltRatio = 6.0'6.0
'      if intSoilCode >= 8 then fltRatio = 7.0'6.0
'   end if
'
   if intOrgLevel = 3 then
      if intSoilCode  = 1 then fltBackgr = 0.8'1
      if intSoilCode  = 2 then fltBackgr = 1.8'2
      if intSoilCode  = 3 then fltBackgr = 3.3'3
      if intSoilCode  = 4 then fltBackgr = 6.6'6
      if intSoilCode  = 5 then fltBackgr = 10.8'13
      if intSoilCode  = 6 then fltBackgr = 14.4'13
      if intSoilCode  = 7 then fltBackgr = 18.4'24
      if intSoilCode >= 8 then fltBackgr = 27

      if intSoilCode  = 1 then fltRatio = 0.5'0.5
      if intSoilCode  = 2 then fltRatio = 1.5'1.5
      if intSoilCode  = 3 then fltRatio = 2.5'2.5
      if intSoilCode  = 4 then fltRatio = 4.5'4.0
      if intSoilCode  = 5 then fltRatio = 5.0'6.0
      if intSoilCode  = 6 then fltRatio = 6.0'6.0
      if intSoilCode  = 7 then fltRatio = 7.0'7.0
      if intSoilCode >= 8 then fltRatio = 8.0'7.0
   end if

   dim fltFert
   fltfert = fltFertiliserN*0.008*(fltRatio+1.0)

   dim fltMan
   fltMan =  0.7*fltManureNincorp*0.025*(fltRatio+1.5)+0.5*fltManureNspread*0.025*(fltRatio+1.5)'algoritme før:'fltManureNincorp*0.025*(fltRatio+1.5)+fltManureNspread*0.025*(fltRatio+0.75)

   dim fltFix
   fltFix = fltNFixation*0.4*0.025*(fltRatio+1.5)

   DenitrificationPrRotation = fltBackgr+fltfert+fltMan+fltFix
'	 Response.write "<br> fltRatio: " & fltRatio
'	 Response.write "<br> fltBackgr: " & fltBackgr
'	 Response.write "<br> fltfert: " & fltfert
'	 Response.write "<br> fltMan: " & fltMan
'	 Response.write "<br> fltFix: " & fltFix
end function

'**************************************************************************************************
' Author: Bjørn Molt Petersen, Danish Institute of Agricultural Sciences 
' Written: 2003-2004
' Function: The function returns a floating point number giving the total estimated 
' soil N change (kg N/ha) using Bjørn Molt Petersens method
' Parameters: 
' intSoilCode is an integer according to the danish JB classification system
' intFarmType is an integer according to the below: 
'    1 = cash crop farm (planteavler)
'    2 = pig farm (svineavler)
'    3 = dairy farm (kvægavler)
' fltCropC is a floating point number, giving the total amount of crop derived C added to the soil
' fltManureC is a floating point number, giving the total amount of C from animal manure
'**************************************************************************************************
function SoilChange_old(intSoilCode, intFarmType,fltTotalCarbonFromCrops,fltTotalCarbonFromManure)'fltTotalCarbonFromCrops og fltTotalCarbonFromManure findes ved stored procedures
   dim fltClay,fltR
Response.write "<br><br> Her starter SoilChange. " 
Response.write "<br> fltTotalCarbonFromCrops: " & fltTotalCarbonFromCrops
Response.write "<br> fltTotalCarbonFromManure: " & fltTotalCarbonFromManure

  
   fltClay = 10
   if intSoilCode = 1 then fltClay = 2.5
   if intSoilCode = 2 then fltClay = 2.5
   if intSoilCode = 3 then fltClay = 7.5
   if intSoilCode = 4 then fltClay = 7.5
   if intSoilCode = 5 then fltClay = 12.5
   if intSoilCode = 6 then fltClay = 12.5
   if intSoilCode = 7 then fltClay = 20
   if intSoilCode = 8 then fltClay = 35

   fltR = 3.09+2.67*exp(-0.079*fltClay)
   fltR = 1/(1+fltR)
   
   dim fltCinSoil

   fltCinSoil = TotalCarbonInSoil(intSoilCode, intFarmType)
'   Response.write "<br> fltCinSoil " & fltCinSoil
'   Response.write "<br> fltTotalCarbonFromCrops " & fltTotalCarbonFromCrops
'   Response.write "<br> fltTotalCarbonFromManure " & fltTotalCarbonFromManure
'   Response.write "<br> konstant " & 0.63*fltCinSoil*CN_factor(intSoilCode, intFarmType)
   SoilChange = 90*(fltR*fltTotalCarbonFromCrops+(fltR+0.08)*fltTotalCarbonFromManure)-0.63*fltCinSoil*CN_factor(intSoilCode, intFarmType)
'   SoilChange = 100*(fltR*fltTotalCarbonFromCrops+(fltR+0.116)*fltTotalCarbonFromManure)-0.63*fltCinSoil*CN_factor(intSoilCode, intFarmType)
   ' FJERN DETTE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   Response.write "<br> SoilChange: " & SoilChange
'   if SoilChange>30 then 
'      SoilChange=30
'   end if
'   if SoilChange<-30 then 
'      SoilChange=-30
'   end if

'   Response.write "<br> SoilChange: " & SoilChange
'Response.write "<br> fltCinSoil: " & fltCinSoil


end function

'**************************************************************************************************
' Author: Bjørn Molt Petersen, Danish Institute of Agricultural Sciences
' Written: 2003-2004
' Function: The function returns a floating point number giving the presumed total C content of the 
' soil, according to the LCA soil types again based on "Kvadratnettet"
' Parameters: 
' intSoilCode is an integer according to the danish JB classification system
' intFarmType is an integer according to the below:  
'    1 = cash crop farm (planteavler)
'    2 = pig farm (svineavler)
'    3 = dairy farm (kvægavler)
'**************************************************************************************************
function TotalCarbonInSoil(intSoilCode, intFarmType)
   dim returnValue
   returnValue=120
   if intSoilCode>4 then
      if intFarmType = 1 then returnValue = 125.0 '106.3
      if intFarmType = 2 then returnValue = 132.2 '113.9
      if intFarmType = 3 then returnValue = 136.1 '117.8
   else
      if intFarmType = 1 then returnValue = 143.8 '115.8
      if intFarmType = 2 then returnValue = 145.1 '117.1
      if intFarmType = 3 then returnValue = 179.0 '151.1
   end if
   TotalCarbonInSoil = returnValue
end function

'**************************************************************************************************
' Author: Bjørn Molt Petersen, Danish Institute of Agricultural Sciences
' Written: 2003-2004
' Function: The function returns a floating point number giving the factor for organic matter 
' turnover which depends on the C/N ratio. The function is dereived from data given by Springob
' intSoilCode is an integer according to the danish JB classification system
' intFarmType is an integer according to the below:  
'    1 = cash crop farm (planteavler)
'    2 = pig farm (svineavler)
'    3 = dairy farm (kvægavler)
'**************************************************************************************************
function CN_factor(intSoilCode, intFarmType)
   dim returnValue,CN_ratio
   CN_ratio=11
   returnValue=1
   if intSoilCode<=4 then
      if intFarmType = 1 then CN_ratio = 12.4
      if intFarmType = 2 then CN_ratio = 12.3
      if intFarmType = 3 then CN_ratio = 15.4
   end if
   returnValue=61.651*(CN_ratio^-1.7314)
   if returnValue>1 then returnValue=1
   CN_factor = returnValue
end function
'**************************************************************************************************
function GetFixationPrFieldPlanRotation(intCropID,aArea)

select case intCropID
	case 14'Græsmarksfrøbælgplanter
		GetFixationPrFieldPlanRotation=aArea*200.0
	case 27,28'Grønærter,Grønærter/vårsæd
		GetFixationPrFieldPlanRotation=aArea*(18.0+35.0)'bidrag fra afgrøden + bidrag fra efterafgrøden (kløvergræs)'bemærkn. Margit 25-10-06: efterafgrøde er her forstået som udlæg dvs. efterfølgende års afgrøde
	case 10,11'Ærter,Bælgsæd ikke ærter
		GetFixationPrFieldPlanRotation=aArea*126.0
	case 12'Lucerne fabrik
		GetFixationPrFieldPlanRotation=aArea*375.0
	case 18'Sædskiftekløvergræs
		GetFixationPrFieldPlanRotation=aArea*105.0
	case else
		GetFixationPrFieldPlanRotation=0.0
end select

end function
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Written: 2004
' Function: The function returns a floating point number giving the total estimated 
' fixation (kg N)
' Parameters: 
' intCropID is an integer giving the ID of the crop
'**************************************************************************************************
 function GetFixation(aFarmNumber,aScenarioID,arrRotation)
   dim aFixation, totalFixation, i, j, temp
   dim nFieldPlanRotation, arrFieldPlanRotation, dictFieldPlanRotation
	 dim arrParameter, rotationArea
 

	 redim arrParameter(2)
	 arrParameter(0)=aFarmNumber
	 arrParameter(1)=aScenarioID
   for i=0 to Ubound(arrRotation)
'   redim preserve arrParameter(2)
	 arrParameter(2) = CInt(arrRotation(i,0))
'   set dictFieldPlanRotation = CreateObject("Scripting.Dictionary")
'   nFieldPlanRotation = CallSP ("GetFieldPlanRotationList " & aFarmNumber & "," & aScenarioID & "," & arrRotation(i,0), arrFieldPlanRotation, dictFieldPlanRotation)

'	   nFieldPlanRotation	 = GetFieldPlanRotationList(aFarmNumber,aScenarioID,arrRotation(i,0),arrFieldPlanRotation)
	   aFixation=0 
'       for j=0 to nFieldPlanRotation-1
'		  temp = GetFixationPrFieldPlanRotation(arrFieldPlanRotation(j,dictFieldPlanRotation.item("Crop")),Csng(arrFieldPlanRotation(j,dictFieldPlanRotation.item("Area"))))
		  temp =  CallSPReturn ("fnGetFixationMeanPrRotation",arrParameter)
			rotationArea =   CallSPReturn ("GetTotalAreaRotation",arrParameter)
		  totalFixation=totalFixation+(temp*rotationArea)
'	   next
'       totalFixation=totalFixation+aFixation
   next
   GetFixation = totalFixation
 end function
'**************************************************************************************************
' Author: Bjørn Molt Petersen, Danish Institute of Agricultural Sciences
' Written: 2003-2004
' Function: The function returns a floating point number giving the total estimated 
' N leaching calculated by the difference method 
' Parameters:
' fltFarmBalance is a floating point number containing the total farm N balance. It can be 
' calculated by the function "FarmNBalance"
' fltDeltaNinSoil is a floating point number containing the yearly soil N change. It can be 
' calculated by the function "SoilChange"
'**************************************************************************************************
function NitrogenLeaching(fltFarmBalance, fltDeltaNinSoil,aArea)
   dim fltRes
   fltRes=fltFarmBalance-fltDeltaNinSoil
'	   response.write "<br/>FarmBalance:    " &fltFarmBalance
'     response.write "<br/>DeltaNInSoilBeforeCorrection:    " & fltDeltaNinSoil

'   if fltRes<(12*aArea) then 'korrektionsfaktoren ganges med arealet, hvis det drejer sig om totaltal
'      dim fltCorrection
'      fltCorrection=fltRes-12*aArea
'      fltDeltaNinSoil=fltDeltaNinSoil+fltCorrection
'      fltRes=12*aArea
'   end if
   NitrogenLeaching=fltRes
end function
'**************************************************************************************************
' Author: Margit Jorgensen, Danish Institute of Agricultural Sciences
' Written: 2004
' Function: The function returns a floating point number giving the total sum of 
' denitrification (Kg N) for all rotations.
' Parameters: 
' aFarmNumber is a long integer
' aScenario is an integer
' aFarmType is an integer
'**************************************************************************************************
function Denitrification(aFarmNumber,aScenarioID,aFarmType,arrRotation)
   dim aDenitrification, aFixation, aGrazingManure, aManureNSpread, aManureNIncorp, aFertilizerBeforeLoss 
   Dim i,j, temp, tempDenitri
   dim nFieldPlanRotation, arrFieldPlanRotation, dictFieldPlanRotation
   Dim nManureNIncorp, arrManureNIncorp, dictManureNIncorp, nManureNSpread, arrManureNSpread, dictManureNSpread
   Dim nFertilizerBeforeLoss, arrFertilizerBeforeLoss, dictFertilizerBeforeLoss
   dim aFertilizerDeliveryLoss, EnergyGrazed, NGrazed, aGrazingFE, GrazingConc
   dim totalFixation,rotationArea
	 dim aBoughtGrazingManure
   Dim arrParameter
   redim arrParameter(2)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
'   response.write " <br> I funktionen Denitrification  "

   aFertilizerDeliveryLoss=0.03

'   for i=0 to nRotation-1
   for i=0 to Ubound(arrRotation)
'   aFertilizerBeforeLoss = 0
'   aManureNIncorp = 0
'   aManureNSpread = 0
'   aGrazingFE = 0
'   aGrazingManure = 0
'   response.write " <br> Ubound(arrRotation)  " & Ubound(arrRotation)

   set dictFieldPlanRotation = CreateObject("Scripting.Dictionary")
   nFieldPlanRotation = CallSP ("GetFieldPlanRotationList " & aFarmNumber & "," & aScenarioID & "," & arrRotation(i,0), arrFieldPlanRotation, dictFieldPlanRotation)
'   response.write "<br> arrFieldPlanRotation "& UBound(arrFieldPlanRotation)
'   print2 arrFieldPlanRotation, dictFieldPlanRotation
   If UBound(arrFieldPlanRotation) > -1 Then 
   'nFieldPlanRotation    = GetFieldPlanRotationList(aFarmNumber,aScenarioID,arrRotation(i,0),arrFieldPlanRotation)

   set dictFertilizerBeforeLoss = CreateObject("Scripting.Dictionary")
   nFertilizerBeforeLoss = CallSP ("GetFertilizerBeforeLoss1 " & aFarmNumber & "," & aScenarioID & "," & arrRotation(i,0), arrFertilizerBeforeLoss, dictFertilizerBeforeLoss)
   aFertilizerBeforeLoss = arrFertilizerBeforeLoss(0,dictFertilizerBeforeLoss.item("Fertilizer"))
'       aFertilizerBeforeLoss = GetFertilizerBeforeLoss(aFarmNumber,aScenarioID,arrRotation(i,0)) 
 
   set dictManureNIncorp = CreateObject("Scripting.Dictionary")
   nManureNIncorp = CallSP ("GetManureNIncorp " & aFarmNumber & "," & aScenarioID & "," & arrRotation(i,0), arrManureNIncorp, dictManureNIncorp)

   aManureNIncorp = arrManureNIncorp(0,dictManureNIncorp.item("ManureNIncorp"))
'	   aManureNIncorp = GetManureNIncorp(aFarmNumber,aScenarioID,arrRotation(i,0))
'  response.write "<br>  aManureNIncorp" & aManureNIncorp 
 
   set dictManureNSpread = CreateObject("Scripting.Dictionary")
   nManureNSpread = CallSP ("GetManureNSpread " & aFarmNumber & "," & aScenarioID & "," & arrRotation(i,0), arrManureNSpread, dictManureNSpread)
   aManureNSpread = arrManureNSpread(0,dictManureNSpread.item("ManureNSpread"))	   
'   aManureNSpread = GetManureNSpread(aFarmNumber,aScenarioID,arrRotation(i,0))

'  response.write "<br>  aManureNSpread" & aManureNSpread 
    arrParameter(2) = CInt(arrRotation(i,0))
   aGrazingFE = CallSPReturn("fnGetGrazingFE_PrRotation", arrParameter)

	   
'	   aGrazingFE = GetTotalGrazingManure(aFarmNumber,aScenarioID,arrRotation(i,0))
 'response.write "<br>  aGrazingFE" & aGrazingFE 

   redim preserve arrParameter(2)

	 arrParameter(2) = 309
     NGrazed = CallSPReturn ("GetFarmTotal",arrParameter)
     arrParameter(2) = 301
     EnergyGrazed = CallSPReturn ("GetFarmTotal",arrParameter)

'       NGrazed = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 309)
'       EnergyGrazed = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 301)

       if EnergyGrazed = 0 then
          GrazingConc = 0
       else 
          GrazingConc = NGrazed/EnergyGrazed
       end if
'   response.write "<br>  GrazingConc" & GrazingConc 

       aGrazingManure = aGrazingFE*GrazingConc
       
'   response.write "<br>  aGrazingManure "&arrRotation(i,0) &"-"& aGrazingManure 
       arrParameter(2) = CInt(arrRotation(i,0))
       aBoughtGrazingManure = CallSPReturn ("fnGetBoughtGrazingNPrRotation", arrParameter)
			 
			 
'   response.write "<br>  aBoughtGrazingManure "&arrRotation(i,0) &"-"& aBoughtGrazingManure 
       aManureNSpread = aManureNSpread+aGrazingManure+aBoughtGrazingManure
'  response.write "<br>  aManureNSpread" & aManureNSpread 

'       aFixation=0 
'       for j=0 to UBound(arrFieldPlanRotation)
'          temp = GetFixationPrFieldPlanRotation(arrFieldPlanRotation(j,14),Csng(arrFieldPlanRotation(j,15)))
'		  aFixation=aFixation+temp
'       next

			 aFixation = CallSPReturn ("fnGetFixationMeanPrRotation",arrParameter)
	     rotationArea =   CallSPReturn ("GetTotalAreaRotation",arrParameter)

' Fixering aFixation er kg N/ha pr sædskifte
'	  response.write "<br>  DenitrificationPrRotation(soiltype, aFarmType, aFertilizerBeforeLoss, aManureNIncorp, aManureNSpread, aFixation)"
'	   response.write "<br> DenitrificationPrRotation("&arrRotation(i,2)&", "&aFarmType&", "&aFertilizerBeforeLoss&", "&aManureNIncorp&", "&aManureNSpread&", "&aFixation&")"
       tempDenitri = DenitrificationPrRotation(arrRotation(i,2), aFarmType, aFertilizerBeforeLoss, aManureNIncorp, aManureNSpread, aFixation)
' tempDenitri er i enheden kg N/ha pr sædskifte
'       totalFixation=totalFixation+aFixation'aFixation skal ganges med sædskiftets areal
       aDenitrification = aDenitrification + (tempDenitri*rotationArea)'tempDenitri skal ganges med sædskiftets areal
'   response.write "<br> tempDenitri " & tempDenitri 
   set dictManureNSpread = Nothing
   set dictManureNIncorp = Nothing 
   set dictFertilizerBeforeLoss = Nothing
   set dictFieldPlanRotation = Nothing 
   End If 

   next
   
   Denitrification = aDenitrification
'   response.write "<br> Denitrification " & Denitrification 
end function
'**************************************************************************************************
'function GetBoughtFertilizer(arrRotation)
'   dim aBoughtFertilizer, tempA
'   dim i
'   
'   for i = 0 to Ubound(arrRotation)
'      tempA = GetFertilizerBeforeLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation(i,0))
'      aBoughtFertilizer = aBoughtFertilizer+tempA
'   next
'   
'GetBoughtFertilizer = aBoughtFertilizer
'end function
'**************************************************************************************************
function GetSoldCrop()
    dim arrSoldCrop, dictSoldCrop, nSoldCrop

   set dictSoldCrop = CreateObject("Scripting.Dictionary")
   nSoldCrop = CallSP ("GetSummarizedSoldCrop " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")), arrSoldCrop, dictSoldCrop)
'   GetSummarizedSoldCrop CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrSoldCrop

GetSoldCrop	= arrSoldCrop(0,dictSoldCrop.item("CernalKgN"))
end function
'**************************************************************************************************
function GetSoilChange(arrRotation,aArea)
dim r, temp, aSoilChange
dim totalCarbonManure, totalGrazingFE, grazingConversion, totalCarbonCrop, fractionCatchCrops
dim testArr, RotArea, NGrazed, EnergyGrazed, GrazingConc, dictDMPot, dictDMStraw, arrDMPot
Dim FieldPlanNo, arrFieldPlanList, dictFieldPlanList, j, DMPot, DMStraw, nDMPot, nDMStraw, arrDMStraw
Dim ntotalCarbonManure,arrtotalCarbonManure, dicttotalCarbonManure
Dim ngrazingConversion, arrgrazingConversion, dictgrazingConversion
dim nFarm, arrFarm, dictFarm, postalCode
dim aBoughtGrazingManure, aGrazingManure


Dim arrParameter
redim arrParameter(2)
arrParameter(0) = CLng(Session("farmNumber"))
arrParameter(1) = CInt(Session("scenarioNumber"))

   set dictFarm = CreateObject("Scripting.Dictionary")
   nFarm = CallSPStr ("GetFarmData " & CLng(Session("farmNumber")), arrFarm, dictFarm)	   
   postalCode = arrFarm(0,dictFarm.Item("FarmOwnerZipCode"))
 ' obs alle beregninger i denne funktion er pr ha. 
'   response.write " <br><br> I funktionen GetSoilChange  "
aSoilChange=0
   for r = 0 to Ubound(arrRotation)
'   Response.write " <br> Rotations ID "& r & " " & arrRotation(r,0)
   set dictFieldPlanList = CreateObject("Scripting.Dictionary")
   FieldPlanNo = CallSP ("GetFieldPlanRotationList " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & "," & arrRotation(r,0), arrFieldPlanList, dictFieldPlanList)
'   response.write "<br> arrFieldPlanList "& UBound(arrFieldPlanList)
'   print2 arrFieldPlanList, dictFieldPlanList
  If UBound(arrFieldPlanList) > -1 Then
  
  totalCarbonCrop = 0
  For j=0 To Ubound(arrFieldPlanList)

   set dictDMPot = CreateObject("Scripting.Dictionary")
   nDMPot = CallSP ("GetPotentialDMDeposition " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & "," & arrRotation(r,0) & "," & arrFieldPlanList(j,11), arrDMPot, dictDMPot)
   DMPot = arrDMPot(0,dictDMPot.item("PotentialDMDeposition"))
   
'   DMPot = GetPotentialDMDeposition(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrRotation(r,0), arrFieldPlanList(j,11)) 
   set dictDMStraw = CreateObject("Scripting.Dictionary")
   nDMStraw = CallSP ("GetStrawDMRemoved " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & "," & arrRotation(r,0) & "," & arrFieldPlanList(j,11), arrDMStraw, dictDMStraw)
'  response.write "<br>  arrRotation(r,0) arrFieldPlanList(j,11) "&  CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & " " & arrRotation(r,0) & " " & arrFieldPlanList(j,11)

'  response.write "<br> j & arrDMStraw( "& j & " " & arrDMStraw(0,dictDMStraw.item("StrawDMRemoved"))
   
   DMStraw = arrDMStraw(0,dictDMStraw.item("StrawDMRemoved"))
'     DMStraw = GetStrawDMRemoved(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrRotation(r,0), arrFieldPlanList(j,11)) 

'     Response.write "<br>Test at GetStrawDMRemoved " & GetStrawDMRemoved(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrRotation(r,0), arrFieldPlanList(j,11)) 
'     Response.write "<br>Test at GetPotentialDMDeposition " & GetPotentialDMDeposition(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrRotation(r,0), arrFieldPlanList(j,11)) 
     totalCarbonCrop = totalCarbonCrop + 0.45*(DMPot-DMStraw)*arrFieldPlanList(j,15)
   set dictDMPot = Nothing
   set dictDMStraw = Nothing
  Next
  
'  Response.write "<br> C from crops, total: " &totalCarbonCrop  
   arrParameter(2) = CInt(arrRotation(r,0))
   RotArea = CallSPReturn("GetTotalAreaRotation", arrParameter)
   fractionCatchCrops = CallSPReturn("fnGetFractionCatchCrop", arrParameter) 

'   RotArea = GetTotalAreaRotation(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation(r,0))
'  Response.write "<br> RotArea: " &RotArea  
 If RotArea>0 Then
 totalCarbonCrop = totalCarbonCrop/RotArea
 End If
 
' Response.write "<br> C from crops, per ha: " &totalCarbonCrop  

   set dicttotalCarbonManure = CreateObject("Scripting.Dictionary")
   ntotalCarbonManure = CallSP ("GetTotalCarbonFromManure_new " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")) & "," & arrRotation(r,0), arrtotalCarbonManure, dicttotalCarbonManure)
   totalCarbonManure = arrtotalCarbonManure(0,dicttotalCarbonManure.item("CarbonFromManure"))
   
'   totalCarbonManure = GetTotalCarbonFromManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation(r,0))
'   response.write "<br> totalCarbonManure før afgræsning " & totalCarbonManure
   
   totalGrazingFE = CallSPReturn("fnGetGrazingFE_PrRotation", arrParameter)
	 aBoughtGrazingManure = CallSPReturn ("fnGetBoughtGrazingNPrRotation", arrParameter)
'   totalGrazingFE = GetTotalGrazingManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation(r,0))
    ' totalGrazingFE er: Mængden af FE som afgræsses på Sædskiftesiden

   set dictgrazingConversion = CreateObject("Scripting.Dictionary")
   ngrazingConversion = CallSP ("GetConversionFactorForStorageType 3", arrgrazingConversion, dictgrazingConversion)
   grazingConversion = arrgrazingConversion(0,dictgrazingConversion.item("ConversionFactor"))
   
'   grazingConversion = GetConversionFactorForStorageType(3)
'   response.write "<br> totalGrazing " & totalGrazingFE
'   response.write "<br> grazingConversion " & grazingConversion ' C/N forhold i frisk kvæg afsætning
   	 arrParameter(2) = 309
     NGrazed = CallSPReturn ("GetFarmTotal",arrParameter)
'     NGrazed = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 309) ' N afsat under afgræsning på kvægsiden
 	 arrParameter(2) = 301
     EnergyGrazed = CallSPReturn ("GetFarmTotal",arrParameter)
'    EnergyGrazed = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 301) 'FE spist under afgræsning på kvægsiden

   if EnergyGrazed = 0 then
     GrazingConc = 0
   else 
     GrazingConc = NGrazed/EnergyGrazed  ' N/FE afgræsset på kvægsiden
   end if
	 aGrazingManure = totalGrazingFE*GrazingConc
'   response.write "<br>  egen græsning "& aGrazingManure 
	 aGrazingManure = aGrazingManure+aBoughtGrazingManure
'   response.write "<br>  købt græsning "& aGrazingManure 

'   response.write "<br> test ((totalGrazing*GrazingConc*grazingConversion)/(1000*RotArea))"&((totalGrazingFE*GrazingConc*grazingConversion)/(1000*RotArea))
''''''oprindelige fejlagtige beregning,- bliver for lille
'   totalCarbonManure = (aGrazingManure*grazingConversion/(1000*RotArea))+totalCarbonManure

   totalCarbonManure = (aGrazingManure*grazingConversion/1000)+totalCarbonManure
'   response.write "<br> carbon i græsningsafsat gødn. " & (aGrazingManure*grazingConversion/1000)
'   response.write "<br> totalCarbonManure " & totalCarbonManure
' Konverterer fra kg N/ha til tons C pr ha pr. sædskifte

' OLD!!!   totalCarbonCrop = GetTotalCarbonFromCrops(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation(r,0)) 
'  response.write "<br> totalCarbonCrop " & totalCarbonCrop & " " & CLng(Session("farmNumber")) & " " & CInt(Session("scenarioNumber"))& " " & arrRotation(r,0) 
	 
'   GetTotalCarbonFromCrops_Test CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),testArr
'	   response.write "<br/>CarbonCrop/area:     " & totalCarbonCrop & "="
'	   response.write "<br/><br/>CarbonTop/area:     " & testArr(0,0)
'	   response.write "<br/>+CarbonRoot/area:     " & testArr(1,0)
'	   response.write "<br/>+CarbonStrawLeft/area:     " & testArr(2,0)
'		 response.write "<br/><br/>CarbonManure/area:     " & totalCarbonManure & "="
'		 response.write "<br/><br/>CarbonDeliveredManure/area:     " & GetTotalCarbonFromManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation(i,0))
'		 response.write "<br/>+CarbonGrazingManure/area:     " & (totalGrazing*grazingConversion)/1000

'   Response.write " <br> Rotations ID "& r & " " & arrRotation(r,0)

' response.write "<br/>SoilChange(SoilType,FarmType,TotalCarbonCrop,totalCarbonManure)     " 
' response.write "<br/>SoilChange("&arrRotation(r,2)&", "&Cint(Session("farmType"))&", "&TotalCarbonCrop&", "&totalCarbonManure&")"
' Response.write "<br> Areal af rotationen: " & RotArea
'  response.write "<br> SoilChange("&arrRotation(r,2)&",,"&Cint(Session("farmType"))&",,"&TotalCarbonCrop&",,"&totalCarbonManure&")"
  temp = SoilChange(arrRotation(r,2), Cint(Session("farmType")), totalCarbonCrop,totalCarbonManure,postalCode,fractionCatchCrops)
  aSoilChange=aSoilChange+temp*RotArea
'  response.write "<br> temp og aSoilChange " & temp & " " & aSoilChange 
   set dictFieldPlanList = Nothing 
End If 

   next
 
' **********Denne funktion returnerer total SoilChange IKKE pr ha. 
'  response.write "<br> aSoilChange "&aSoilChange
GetSoilChange = aSoilChange
set dictFarm = Nothing

end function
'**************************************************************************************************
'sub Calculate()
'   dim aDenitrification, aBoughtFertilizer, tempA, tempB, aIncorpFertilizer, aFertilizerDeliveryLoss
'   dim i,totalCarbonManure,totalCarbonCrop,aSoilChange,r,temp
'   dim totalGrazing,grazingConversion
'   dim aNInSeed, nArea, arrArea,totalLoss
'   dim outputResult,InputResult
'   dim aBalance,Nleaching,aSoldCrop
'	dim nFarmTotal, arrFarmTotal
'	dim nStorage, arrStorage
'	dim nRotation, arrRotation
'
'	aFarmNumber = Session("farmNumber")
'	aScenarioID = Session("scenarioNumber")
'	InitFarmNDB
'	nFarmTotal = GetFarmTotalList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),3,arrFarmTotal)
'	nStorage   = GetStorageList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrStorage)
'	nRotation  = GetRotationList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)

   
 '  aDenitrification  = Denitrification(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),CInt(Session("farmType")))
 '  for i = 0 to nRotation-1
 '     tempA = GetFertilizerBeforeLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation(0,i))
 '     aBoughtFertilizer = aBoughtFertilizer+tempA
 '     tempB = GetFertilizerAfterLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation(0,i),0.03) 
 '     aIncorpFertilizer = aIncorpFertilizer+tempB
 '  next
 '  aFertilizerDeliveryLoss=aBoughtFertilizer-aIncorpFertilizer

'   totalCarbonManure=GetTotalCarbonFromManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
'   totalGrazing=GetTotalGrazingManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
'   grazingConversion=GetConversionFactor(3)
'   totalCarbonManure=((totalGrazing*grazingConversion)/1000)+totalCarbonManure
'   totalCarbonCrop=GetTotalCarbonFromCrops(CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))) 
'   for r = 0 to nRotation-1
'   Response.write " Rotations ID "& r
'     temp=SoilChange(arrRotation(2,0), arrRotation(0,0),TotalCarbonCrop,totalCarbonManure)'
'	 aSoilChange=aSoilChange+temp
'   next

'   nArea 				= CalcSumArea(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrArea)
   
'   aSoilChange=aSoilChange/(nRotation)*arrArea(0,0)
   
   
'   inputResult			= CalcSumResult(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),1)
'   outputResult			= CalcSumResult(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),2)
   
'   aBalance				= inputResult-outputResult
'   NLeaching			= NitrogenLeaching(aBalance, aSoilChange, arrArea(0,0))
   
'   aNInSeed				= GetTotalNInSeed(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   
   
'   totalLoss			= GetTotalLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))

'   GetSummarizedSoldCrop CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrSoldCrop
'   aSoldCrop			= arrSoldCrop(4,0)

'end sub
'**************************************************************************************************
' Author: Bjørn Molt Petersen, Danish Institute of Agricultural Sciences
' Written: 2006
' Function: The function returns a floating point number giving the presumed decay of the 
' soil, according to the C-TOOL www model
' Parameters: 
' intSoilCode is an integer according to the danish JB classification system
' intFarmType is an integer according to the below:  
'    1 = cash crop farm (planteavler)
'    2 = pig farm (svineavler)
'    3 = dairy farm (kvægavler)
' intPostalCode is an integer with the postal code
'**************************************************************************************************
function TotalDecayInSoil(intSoilCode, intFarmType, intPostalCode)
   dim returnValue
   returnValue=70
   if intPostalCode > 4999 then' Jylland - ER DET KORREKT?!
      if intFarmType < 3 then
         if intSoilCode = 1 then returnValue = 50
         if intSoilCode = 2 then returnValue = 50.5
         if intSoilCode = 3 then returnValue = 57.6
         if intSoilCode = 4 or intSoilCode = 11 or intSoilCode = 12  then returnValue = 69.6   
         if intSoilCode = 5 then returnValue = 89.9
         if intSoilCode = 6 then returnValue = 86.6
         if intSoilCode = 7 or intSoilCode = 8 or intSoilCode = 9  or intSoilCode = 10 then returnValue  = 106.5
      end if
      if intFarmType = 3 then
         if intSoilCode = 1 then returnValue = 59.2
         if intSoilCode = 2 then returnValue = 63.9
         if intSoilCode = 3 then returnValue = 68.8
         if intSoilCode = 4 or intSoilCode = 11 or intSoilCode = 12 then returnValue = 84
         if intSoilCode = 5 then returnValue = 111.6
         if intSoilCode = 6 then returnValue = 108.1
         if intSoilCode = 7 or intSoilCode = 8 or intSoilCode = 9  or intSoilCode = 10 then returnValue = 132.2
      end if
   end if
   if intPostalCode < 5000 then' Øerne - ER DET KORREKT?!
     if intFarmType < 3 then
         if intSoilCode = 1 then returnValue = 48.3
         if intSoilCode = 2 then returnValue = 47.3
         if intSoilCode = 3 then returnValue = 55.7
         if intSoilCode = 4 or intSoilCode = 11 or intSoilCode = 12  then returnValue = 68.2
         if intSoilCode = 5 then returnValue = 85.8
         if intSoilCode = 6 then returnValue = 81.6
         if intSoilCode = 7 or intSoilCode = 8 or intSoilCode = 9  or intSoilCode = 10 then returnValue = 96.1
      end if
      if intFarmType = 3 then
         if intSoilCode = 1 then returnValue = 58.6
         if intSoilCode = 2 then returnValue = 62.4
         if intSoilCode = 3 then returnValue = 68.3
         if intSoilCode = 4 or intSoilCode = 11 or intSoilCode = 12  then returnValue = 84.7
         if intSoilCode = 5 then returnValue = 110.5
         if intSoilCode = 6 then returnValue = 106.1
         if intSoilCode = 7 or intSoilCode = 8 or intSoilCode = 9  or intSoilCode = 10 then returnValue = 124
      end if
   end if
   TotalDecayInSoil = returnValue
end function


'**************************************************************************************************
' Author: Bjørn Molt Petersen, Danish Institute of Agricultural Sciences 
' Written: 2006
' Function: The function returns a floating point number giving the total estimated 
' soil N change (kg N/ha) 
' Parameters: 
' intSoilCode is an integer according to the danish JB classification system
' intFarmType is an integer according to the below: 
'    1 = cash crop farm (planteavler)
'    2 = pig farm (svineavler)
'    3 = dairy farm (kvægavler)
' fltCropC is a floating point number, giving the total amount of crop derived C added to the soil
' fltManureC is a floating point number, giving the total amount of C from animal manure
' intPostalCode is an integer with the postal code
'**************************************************************************************************
function SoilChange(intSoilCode, intFarmType,fltTotalCarbonFromCrops,fltTotalCarbonFromManure, intPostalCode,fractionCatchCrops)
'fltTotalCarbonFromCrops og fltTotalCarbonFromManure findes ved stored procedures
   dim fltClay,fltR,arr
'Response.write "<br> Her starter SoilChange. " 
'Response.write "<br> fltTotalCarbonFromCrops: " & fltTotalCarbonFromCrops
'Response.write "<br> fltTotalCarbonFromManure: " & fltTotalCarbonFromManure

redim arr(0)
arr(0)= intSoilCode 
'   fltClay = 10
'   if intSoilCode = 1 then fltClay = 2.5
'   if intSoilCode = 2 then fltClay = 2.5
'   if intSoilCode = 3 then fltClay = 7.5
'   if intSoilCode = 4 then fltClay = 7.5
'   if intSoilCode = 5 then fltClay = 12.5
'   if intSoilCode = 6 then fltClay = 12.5
'   if intSoilCode = 7 then fltClay = 20
'   if intSoilCode > 7 then fltClay = 35

'lerprocent hentes fra SoilType:

fltClay =  Cdbl(CallSPReturn ("GetClayRatioPrSoilType",arr))
'response.write "<br>"&fltClay
   fltR = 3.09+2.67*exp(-0.079*fltClay)
   fltR = 1/(1+fltR)
   
   dim soilDecay
	 fltTotalCarbonFromCrops = fltTotalCarbonFromCrops + 0.72*fractionCatchCrops ' fraction is 0-1: areaCatchCrop/totalArea
   soilDecay = TotalDecayInSoil(intSoilCode, intFarmType, intPostalCode)
   SoilChange = 82*(fltR*fltTotalCarbonFromCrops+(fltR+0.08)*fltTotalCarbonFromManure-0.01*soilDecay)-4
'   Response.write "<br> SoilChange: " & SoilChange

end function
'**************************************************************************************************
%>