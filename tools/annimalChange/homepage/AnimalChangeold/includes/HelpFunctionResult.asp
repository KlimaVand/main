<%
'**************************************************************************************************
' Author: Margit Jorgensen
' Description: Include file used by Result.asp and CalculateResult.asp
'**************************************************************************************************
sub InitTableInput(farmNumber,scenarioID)
dim boughtFertilizer, pigs, BoughtPigs, a,b,c,d
dim arrResult,y,x

redim arrResult(8,1)

 ReDim arrParameter(3)
   arrParameter(0) = farmNumber
   arrParameter(1) = scenarioID
   arrParameter(2) = 1
   arrParameter(3) = 415
   a = CallSPReturn ("GetPigHerdTotal",arrParameter)
   arrParameter(2) = 3
   arrParameter(3) = 414
   b = CallSPReturn ("GetPigHerdTotal",arrParameter)
   arrParameter(2) = 3
   arrParameter(3) = 415
   c = CallSPReturn ("GetPigHerdTotal",arrParameter)
   arrParameter(2) = 2
   arrParameter(3) = 414
   d = CallSPReturn ("GetPigHerdTotal",arrParameter)
   pigs =a-b+c-d 

   if pigs < 0 then 
   BoughtPigs=-pigs
   else 
   BoughtPigs=0
   end if
   arrInput(0,0)  = ""&Str.Item("strMINFERT")&"" 
   ReDim preserve arrParameter(1)
   arrInput(0,1) = CallSPReturn ("GetFertilizer",arrParameter)
   arrResult(0,0) = 11'parameter for kunstgødning i resultat tabellen
   arrResult(0,1) = arrResult(0,0)
   arrInput(1,0)  = ""&Str.Item("strMANUREBUY")&""
   arrInput(1,1)  = TotalBoughtManure
   arrResult(1,0) = 12'parameter for købt husdyrgødning i resultat tabellen
   arrResult(1,1) = arrInput(1,1)
   arrInput(2,0)  = ""&Str.Item("strFEEDBUY")&""
   arrInput(2,1)  = boughtFeed
	 arrResult(2,0) = 13'parameter for købt foder i resultat tabellen
	 arrResult(2,1) = arrInput(2,1)
   arrInput(3,0)  = ""&Str.Item("strSTRAWBUY")&""
   arrInput(3,1)  = BoughtBeddingN
	 arrResult(3,0) = 14'parameter for købt strøelse i resultat tabellen
	 arrResult(3,1) = arrInput(3,1)
   arrInput(4,0)  = ""&Str.Item("strANIMBUY")&""
   arrInput(4,1)  = BoughtPigs
	 arrResult(4,0) = 15'parameter for købte grise i resultat tabellen
	 arrResult(4,1) = arrInput(4,1)
   arrInput(5,0)  = ""&Str.Item("strSEED")&""
   arrInput(5,1) = CallSPReturn ("GetTotalNInSeed",arrParameter)
	 arrResult(5,0) = 16'parameter for udsæd i resultat tabellen
	 arrResult(5,1) = arrInput(5,1)
   arrInput(6,0)  = ""&Str.Item("strNFIX")&""
   arrInput(6,1)  = GetFixation(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)
	 arrResult(6,0) = 17'parameter for N-fixering i resultat tabellen
	 arrResult(6,1) = arrInput(6,1)
   arrInput(7,0)  = ""&Str.Item("strATMOSDEP")&""
   arrInput(7,1)  = 15*totalArea
	 arrResult(7,0) = 18'parameter for atmosfærisk afsætning i resultat tabellen
	 arrResult(7,1) = arrInput(7,1)
   arrInput(8,0)  = ""
   arrInput(8,1)  = null
   arrInput(9,0)  = ""&Str.Item("strTOTINPUT")&""
   arrInput(9,1)  = arrInput(0,1)+arrInput(1,1)+arrInput(2,1)+arrInput(3,1)+arrInput(4,1)+arrInput(5,1)+arrInput(6,1)+arrInput(7,1)
	 arrResult(8,0) = 19'parameter for input ialt i resultat tabellen
	 arrResult(8,1) = arrInput(9,1)
   redim preserve arrParameter(3)
	 for y = 0 to ubound(arrResult)
	   arrParameter(2)=arrResult(y,0)
     if not isnull(arrResult(y,1)) and (arrResult(y,1)<>"") then
	    arrParameter(3)=arrResult(y,1)
		 else 
	    arrParameter(3)=0
		 end if 
     X = CallSPReturn ("InsertUpdateResult " , arrParameter)
	 next
end sub
'**************************************************************************************************
'sub InitTableInput()
'dim boughtFertilizer, pigs, BoughtPigs, a,b,c,d
''PrintArrayInTable2 arrRotation
' '  boughtFertilizer = GetBoughtFertilizer(arrRotation)
' ReDim arrParameter(3)
'   arrParameter(0) = CLng(Session("farmNumber"))
'   arrParameter(1) = CInt(Session("scenarioNumber"))
'   arrParameter(2) = 1
'   arrParameter(3) = 415
'   a = CallSPReturn ("GetPigHerdTotal",arrParameter)
''   a=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 1, 415) - 
'   arrParameter(2) = 3
'   arrParameter(3) = 414
'   b = CallSPReturn ("GetPigHerdTotal",arrParameter)
''   b=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 414) + 
'   arrParameter(2) = 3
'   arrParameter(3) = 415
'   c = CallSPReturn ("GetPigHerdTotal",arrParameter)
''   c=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 415) - 
'   arrParameter(2) = 2
'   arrParameter(3) = 414
'   d = CallSPReturn ("GetPigHerdTotal",arrParameter)
''   d=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 414)
'   pigs =a-b+c-d 
'
'   if pigs < 0 then 
'   BoughtPigs=-pigs
'   else 
'   BoughtPigs=0
'   end if
'   arrInput(0,0)  = ""&Str.Item("strMINFERT")&"" 
'   ReDim preserve arrParameter(1)
'   arrInput(0,1)  = CallSPReturn ("GetFertilizer",arrParameter)/totalArea
''   arrInput(0,1)  = GetFertilizer(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
'   arrInput(1,0)  = ""&Str.Item("strMANUREBUY")&""
'   arrInput(1,1)  = TotalBoughtManure/totalArea
'   arrInput(0,2)  = round(arrInput(0,1),1)-(((meanUtilization)*round(arrInput(1,1),1)+round(arrInput(0,1),1))-((meanUtilization)*round(arrInput(1,1),1)+round(arrInput(0,1),1))*(normPercent/100))
'   reducedFertilizer = round(arrInput(0,1),1)-round(arrInput(0,2),1)
'   arrInput(1,2)  = TotalBoughtManure/totalArea
'   arrInput(2,0)  = ""&Str.Item("strFEEDBUY")&""
'   arrInput(2,1)  = boughtFeed/totalArea
'   arrInput(2,2)  = boughtFeed/totalArea
'   arrInput(3,0)  = ""&Str.Item("strSTRAWBUY")&""
'   arrInput(3,1)  = BoughtBeddingN/totalArea
'   arrInput(3,2)  = BoughtBeddingN/totalArea
'   arrInput(4,0)  = ""&Str.Item("strANIMBUY")&""
'   arrInput(4,1)  = BoughtPigs/totalArea
'   arrInput(4,2)  = BoughtPigs/totalArea
'   arrInput(5,0)  = ""&Str.Item("strSEED")&""
'   arrInput(5,1)  = CallSPReturn ("GetTotalNInSeed",arrParameter)/totalArea
'   arrInput(5,2)  = arrInput(5,1)
''   arrInput(5,1)  = GetTotalNInSeed(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
'   arrInput(6,0)  = ""&Str.Item("strNFIX")&""
'   arrInput(6,1)  = GetFixation(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)/totalArea
'   arrInput(6,2)  = arrInput(6,1)
'   arrInput(7,0)  = ""&Str.Item("strATMOSDEP")&""
'   arrInput(7,1)  = 15'*totalArea
'   arrInput(7,2)  = arrInput(7,1)
'   arrInput(8,0)  = ""
'   arrInput(8,1)  = null
'   arrInput(8,2)  = null
'   arrInput(9,0)  = ""&Str.Item("strTOTINPUT")&""
''   Response.write "<br> Test: " & arrInput(2,1)
'   arrInput(9,1)  = arrInput(0,1)+arrInput(1,1)+arrInput(2,1)+arrInput(3,1)+arrInput(4,1)+arrInput(5,1)+arrInput(6,1)+arrInput(7,1)
'   arrInput(9,2)  = arrInput(9,1)-(reducedFertilizer)
''   Response.write "<br> Test2: " & arrInput(9,1)
'end sub
'**************************************************************************************************
sub InitTableOutput(farmNumber,scenarioID)
dim pigs, SoldPigs, nMilk, arrMilk, dictMilk, a, b, c, d
dim arrResult,y,x

redim arrResult(8,1)
 
ReDim arrParameter(3)
   arrParameter(0) = farmNumber
   arrParameter(1) = scenarioID
   arrParameter(2) = 1
   arrParameter(3) = 415
   a = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   a=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 1, 415) - 
   arrParameter(2) = 3
   arrParameter(3) = 414
   b = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   b=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 414) + 
   arrParameter(2) = 3
   arrParameter(3) = 415
   c = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   c=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 415) - 
   arrParameter(2) = 2
   arrParameter(3) = 414
   d = CallSPReturn ("GetPigHerdTotal",arrParameter)
'   d=GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 414)
   pigs =a-b+c-d 
 
 
 
 
' pigs = GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 1, 415) - GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 414) + GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 3, 415) - GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 414)
 
 
 
   set dictMilk = CreateObject("Scripting.Dictionary")
   nMilk = CallSP ("CalcTotalMilkkg " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")), arrMilk, dictMilk)
   nMilk = arrMilk(0, dictMilk.item("SumMilkkg"))
 
' nMilk = CalcTotalMilkkg(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrMilk) 
   If IsNull(arrMilk(0, dictMilk.item("SumMilkkg"))) Then 
   arrMilk(0, dictMilk.item("SumMilkkg"))=0
   End If
   arrMilk(0, dictMilk.item("SumMilkkg"))=arrMilk(0, dictMilk.item("SumMilkkg"))*0.00518
   if pigs > 0 then 
   SoldPigs=pigs
   else 
   SoldPigs=0
   end if
   arrOutput(0,0)  = ""&Str.Item("strCASHCROP")&""
   arrOutput(0,1)  = GetSoldCrop
   arrOutput(1,0)  = ""&Str.Item("strANIMSOLD")&""
   arrOutput(1,1)  = SoldPigs
   arrOutput(2,0)  = ""&Str.Item("strMEAT")&""
   ReDim preserve arrParameter(2)
   arrParameter(2) = PARAMETER_GROWTH_N
   arrOutput(2,1)  = CallSPReturn ("GetFarmTotal",arrParameter)
'   arrOutput(2,1)  = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_GROWTH_N)
   arrOutput(3,0)  = ""&Str.Item("strMILK")&""
   arrOutput(3,1)  = arrMilk(0, dictMilk.item("SumMilkkg"))
   arrOutput(4,0)  = ""&Str.Item("strMANURESOLD")&""
   arrOutput(4,1)  = TotalSoldManure ' GetSoldManureTotal(8)
   arrOutput(5,0)  = ""&Str.Item("strFEEDSOLD")&""
   arrOutput(5,1)  = soldFeed
   arrOutput(6,0)  = ""&Str.Item("strSoldStraw")&""
   arrOutput(6,1)  = SoldBeddingN
   arrOutput(7,0)  = ""&Str.Item("strSoldPigMeat")&""

   ReDim  preserve arrParameter(3)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
   arrParameter(2) = 2
   arrParameter(3) = 415
   a = CallSPReturn ("GetPigHerdTotal",arrParameter)
   ReDim  preserve arrParameter(2)
   arrParameter(2) = 413
   b = CallSPReturn ("GetFarmTotal",arrParameter)
   arrOutput(7,1)  = a+b

'   arrOutput(7,1)  = GetPigHerdTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 2, 415)+GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 413)
   arrOutput(8,0)  = ""&Str.Item("strPlantUptake")&""
   arrOutput(8,2)  = arrOutput(0,1)+arrOutput(5,1)+arrOutput(6,1)'null

   arrOutput(9,0) = ""&Str.Item("strOUTPUTALL")&""
   arrOutput(9,1) = arrOutput(0,1)+arrOutput(1,1)+arrOutput(2,1)+arrOutput(3,1)+arrOutput(4,1)+arrOutput(5,1)+arrOutput(6,1)+arrOutput(7,1)
end sub
'**************************************************************************************************
sub InitTableSurplus(farmNumber,scenarioID)
   dim aSoilChange, aBalance, NLeaching
   dim nNLES,meanNLES_N,meanNLES_mg,arrNLES, remainder,deltaD,deltaU

   surplus = arrInput(9,1)- arrOutput(9,1)
		 
   ReDim  arrParameter(1)
   arrParameter(0) = farmNumber
   arrParameter(1) = scenarioID
   
   arrSurplus(0,0)  = ""&Str.Item("strNH3HOUSE")&""
   arrSurplus(0,1)  = CallSPReturn ("GetNitrogenStableLossTotal",arrParameter)
   arrSurplus(0,2)  = arrSurplus(0,1)
   arrSurplus(0,3)  = arrSurplus(0,1)
'   arrSurplus(0,1)  = GetNitrogenStableLossTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(1,0)  = ""&Str.Item("strNH3STORE")&""
   arrSurplus(1,1)  = CallSPReturn ("GetNitrogenStorageLossTotal",arrParameter)
   arrSurplus(1,2)  = arrSurplus(1,1)
   arrSurplus(1,3)  = arrSurplus(1,1)
'   arrSurplus(1,1)  = GetNitrogenStorageLossTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(2,0)  = ""&Str.Item("strDENITRIF")&", " & Str.Item("storage")&""
   arrSurplus(2,1)  = CallSPReturn ("GetNitrogenStorageDenitrificationTotal",arrParameter)
   arrSurplus(2,2)  = arrSurplus(2,1)
   arrSurplus(3,0)  = ""&Str.Item("strFieldSurplus")&""
   arrSurplus(3,1)  = surplus-arrSurplus(0,1)-arrSurplus(1,1)-arrSurplus(2,1)
   arrSurplus(3,2)  = arrSurplus(3,1)
   arrSurplus(3,3)  = arrSurplus(3,1)
'   arrSurplus(2,1) = GetNitrogenStorageDenitrificationTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(4,0)  = ""&Str.Item("strNH3GRAZING")&""
   arrSurplus(4,1)  = LOSS_MANURE_N_FIELD * CallSPReturn ("fnGetBoughtGrazingN ",arrParameter)
  ReDim  preserve arrParameter(2)
   arrParameter(2)  = PARAMETER_MANURE_N_FIELD
   arrSurplus(4,1)  = arrSurplus(4,1) + LOSS_MANURE_N_FIELD * CallSPReturn ("GetFarmTotal",arrParameter)
   arrSurplus(4,2)  = arrSurplus(4,1)
   arrSurplus(4,3)  = arrSurplus(4,1)
'   arrSurplus(3,1)  = 0.07*GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_MANURE_N_FIELD)
  ReDim  preserve arrParameter(1)
'  Response.write "<br> arrParameter "& arrParameter(0) & " " & arrParameter(1)
   arrSurplus(5,0)  = ""&Str.Item("strNH3SPREAD")&""
   arrSurplus(5,1)  = CallSPReturn ("GetTotalLossManure",arrParameter)
   arrSurplus(5,2)  = arrSurplus(5,1)
   arrSurplus(5,3)  = arrSurplus(5,1)
'  Response.write "<br> arrSurplus "& arrSurplus(4,1)
'   arrSurplus(4,1)  = GetTotalLossManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(6,0) = ""&Str.Item("strNH3MIN")&""
   arrSurplus(6,1) = 0.03 * CallSPReturn ("GetFertilizer",arrParameter)
   arrSurplus(6,2) = arrSurplus(6,1)
   arrSurplus(6,3) = arrSurplus(6,1)
'   arrSurplus(5,1) = 0.03*GetFertilizer(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
   arrSurplus(7,0) = ""&Str.Item("strDENITRIF")&", " & Str.Item("strsoil")&""
   arrSurplus(7,1) = (Denitrification(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),CInt(Session("farmType")),arrRotation))'*totalArea'der skal ikke ganges med arealet her
  ' arrSurplus(6,0) = ""&Str.Item("strDENITRIF")&" " & Str.Item("storage")&""
  ' arrSurplus(6,1) = GetNitrogenStorageDenitrificationTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")))
'  Response.write "<br> Denitrification "& Denitrification(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),CInt(Session("farmType")),arrRotation)
  aBalance = arrInput(9,1) - arrOutput(9,1) - arrSurplus(0,1) - arrSurplus(1,1) - arrSurplus(2,1) - arrSurplus(4,1) - arrSurplus(5,1) - arrSurplus(6,1) - arrSurplus(7,1)

  aSoilChange = GetSoilChange(arrRotation,totalArea)
  NLeaching = NitrogenLeaching(aBalance, aSoilChange, totalArea) 
  nNLES = CalcN_Les(meanNLES_N,meanNLES_mg,meanRunOff,arrNLES)
	remainder = NLeaching-meanNLES_N*totalArea
'	deltaD
'   Response.write "<br> aSoilChange "& aSoilChange & " tot " & totalArea
'   Response.write "<br> NLeaching "& NLeaching

   arrSurplus(8,0) = ""&Str.Item("strSOILCHANGE")&""
      if not IsNull(aSoilChange) then
         arrSurplus(8,1) = aSoilChange
      else 
         arrSurplus(8,1) = null
      end if
'  arrSurplus(8,0) = ""&Str.Item("strNLEACH")&""
'      if not IsNull(NLeaching) then
'         arrSurplus(8,1) = NLeaching
'      else 
'         arrSurplus(8,1) = null
'      end if
  arrSurplus(9,0) = Str.Item("strNLEACH")& " ("&Str.Item("strkg N/ha")&")"
      if not IsNull(meanNLES_N) then
         arrSurplus(9,1) = meanNLES_N*totalArea
      else 
         arrSurplus(9,1) = null
      end if
  arrSurplus(10,0) = ""&Str.Item("strRemainder")&""
  arrSurplus(11,0) = ""&Str.Item("strNLEACH")& " ("&Str.Item("strMgNO3/l")&")"
'      if not IsNull(meanNLES_N*totalArea-NLeaching) then
'         arrSurplus(9,1) = meanNLES_N*totalArea-NLeaching
      if not IsNull(remainder) and meanNLES_N<>0 then
			'den første måde at fordele rest på
'         arrSurplus(10,1) = remainder
''				 arrSurplus(6,2) = arrSurplus(6,1)-(arrSurplus(6,1)/(arrSurplus(6,1)+aSoilChange)*remainder)
'				 arrSurplus(7,2) = arrSurplus(7,1)+(Cdbl(arrSurplus(7,1))/(Cdbl(arrSurplus(7,1))+Cdbl(meanNLES_N*totalArea))*Cdbl(remainder))
'				 arrSurplus(7,3) = arrSurplus(7,2)
'				 arrSurplus(8,2) = aSoilChange+(meanNLES_N*totalArea/(arrSurplus(7,1)+meanNLES_N*totalArea)*remainder*0.5)
'				 arrSurplus(8,3) = arrSurplus(8,2)
'				 arrSurplus(9,2) = (meanNLES_N*totalArea)+(meanNLES_N*totalArea/(arrSurplus(7,1)+meanNLES_N*totalArea)*remainder*0.5)
'				 arrSurplus(9,3) = arrSurplus(9,2)
       'den nye måde at fordele rest på
         arrSurplus(10,1) = remainder
         arrSurplus(11,1) = meanNLES_mg*totalArea
				 arrOutput(8,2) = arrOutput(8,2)+ remainder*SURPLUSCORRECTION_HARVEST
				 surplusCorr = surplus-remainder*SURPLUSCORRECTION_HARVEST
         arrSurplus(3,2)  = surplusCorr-arrSurplus(0,1)-arrSurplus(1,1)-arrSurplus(2,1)
         arrSurplus(3,3)  = arrSurplus(3,2)
				 arrOutput(9,2) = arrOutput(8,2)+arrOutput(1,1)+arrOutput(2,1)+arrOutput(3,1)+arrOutput(4,1)+arrOutput(7,1)
'				 arrSurplus(6,2) = arrSurplus(6,1)-(arrSurplus(6,1)/(arrSurplus(6,1)+aSoilChange)*remainder)
				 arrSurplus(7,2) = arrSurplus(7,1)+ remainder*SURPLUSCORRECTION_DENITR
				 arrSurplus(7,3) = arrSurplus(7,2)
				 arrSurplus(8,2) = arrSurplus(8,1)+ remainder*SURPLUSCORRECTION_SOIL
				 arrSurplus(8,3) = arrSurplus(8,2)
				 arrSurplus(9,2) = arrSurplus(9,1)+ remainder*SURPLUSCORRECTION_NLES
				 arrSurplus(11,2) = 100*arrSurplus(9,2)*4.43/meanRunOff
				 arrSurplus(11,3) = arrSurplus(11,2)
				 arrSurplus(9,3) = arrSurplus(9,2)
      else 
         arrSurplus(10,1) = null
      end if
end sub
'**************************************************************************************************
%>
