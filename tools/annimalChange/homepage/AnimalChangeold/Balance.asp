<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
Private Const NUMBER_MANURE_COL        = 9
Private Const LOSS_MANURE_N_FIELD      = 0.07

dim arrManure
dim arrFeedBalance, arrFeedBalance1, nFeedBalance, dictFeedBalance
dim nStorageType, arrStorageType
Dim dictStorageType, dictRotation
Dim arrParameter, returnValue
'dim dictManureNContent, nManureNContent, arrManureNContent
version = Session("version")


'**************************************************************************************************
sub CreateTabBalance()
   dim NPercent,dummy1, dummy2, dummy3, dummy4, HighEnergyConc
   dim internalGrazingFE, internalGrazingN
   dim dictManure, nManure, arrManure
time16=now
   ReDim arrParameter(1)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))

'   CalculateManure version, NPercent, scale,arrA,arrB,warning, dummy1, dummy2

   InitFarmNDB
   CreateLanguageDictionary
   
   set dictFeedBalance = CreateObject("Scripting.Dictionary")
   nFeedBalance = CallSP ("getAllFeedplansModelled "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber"))&"" , arrFeedBalance1, dictFeedBalance)
   

   set dictStorageType = CreateObject("Scripting.Dictionary")
   nStorageType = CallSP ("getAllStorageTypesList " , arrStorageType, dictStorageType)
 '  nStorageType = CallSP ("GetStorageTypeList " , arrStorageType, dictStorageType)
 '  nStorageType = GetStorageTypeList(arrStorageType)

   set dictManureNContent = CreateObject("Scripting.Dictionary")
   nManureNContent = CallSP ("GetModelledManure " & Session("farmNumber") & "," & Session("scenarioNumber"), arrManureNContent, dictManureNContent)

   set dictManure = CreateObject("Scripting.Dictionary")
   nManure = CallSP ("GetManure_N_Content " & Session("farmNumber") & "," & Session("scenarioNumber"), arrManure, dictManure)
'   nRotation  = GetRotationList(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrRotation)

'   reDim arrManure(nStorageType,NUMBER_MANURE_COL-1)
   reDim arrFeedBalance(NUMBER_FEED_ROW,NUMBER_FEED_COL-1)

'   InitTableManure
'   InitTableFeed arrFeedBalance, dummy1, dummy2, HighEnergyConc, dummy3, dummy4,internalGrazingFE
'
'   Response.Write("<h3 style=""color: red;"">"&Str.Item("strModelHasToBeRun")&"</h3><br/>" & vbCr) 
   Response.Write("<h3>"&Str.Item("strMANURE")&"</h3>" & vbCr)   
   CreateTableManure

'   Response.Write("<hr/>" & vbCr)   
   Response.Write("<h3>"&Str.Item("strFEED")&"</h3>" & vbCr)
   CreateTableFeed HighEnergyConc
   CloseFarmNDB
	 CreateUpdateFormBalance
time17=now
end sub
'**************************************************************************************************
sub CreateTableManure()
   dim r,c, a, b
   ReDim arrParameter(1)
   arrParameter(0) = CLng(Session("farmNumber"))
   arrParameter(1) = CInt(Session("scenarioNumber"))
   
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strPRODUCTION")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strBOUGHT")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strSOLD")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strHOUSINGUSE")&"</th>" & vbCr)
'   Response.Write("      <th align=""right"">&nbsp;&nbsp;"&Str.Item("strInternalExchge")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&Str.Item("strMANURETYPE")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strKg_toerstof")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strKg_toerstof")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strKg_toerstof")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strKg_toerstof")&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""right"">&nbsp;&nbsp;"&Str.Item("strFE")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
'   a = CallSPReturn ("GetNitrogenFieldNeed",arrParameter)
'response.write arrManureNContent(0,dictManureNContent.Item("StorageName"))
    
   for r = 0 to nManureNContent-1
'   if arrManure(r,1)<> 0 or arrManure(r,7)<> 0 then
      Response.Write("    <tr>" & vbCr)  
      Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrManureNContent(r,dictManureNContent.Item("StorageName"))&"</td>" & vbCr)
      Response.Write("      <td></td><td align=""left"">"&FormatNumber(arrManureNContent(r,dictManureNContent.Item("AmountProduced")),1)&"</td>" & vbCr)
      Response.Write("      <td></td><td align=""left"">"&FormatNumber(arrManureNContent(r,dictManureNContent.Item("Bought")),1)&"</td>" & vbCr)
      Response.Write("      <td></td><td align=""left"">"&FormatNumber(arrManureNContent(r,dictManureNContent.Item("Sold")),1)&"</td>" & vbCr)
      Response.Write("      <td></td><td align=""left"">"&FormatNumber(arrManureNContent(r,dictManureNContent.Item("Amount")),1)&"</td>" & vbCr)
      Response.Write("    </tr>" & vbCr) 
   next   
   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"" colspan=""8""><hr/></th>" & vbCr)
   Response.Write("    </tr>" & vbCr)   
   Response.Write("  </table>" & vbCr)

end sub
'**************************************************************************************************
sub CreateTableFeed1(HighEnergyConc)
   dim r,c
   
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">"&Str.Item("strFIELDPROD")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">"&Str.Item("strBOUGHT")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">"&Str.Item("strSOLD")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">"&Str.Item("strHOUSINGUSE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strInternalExchge")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&Str.Item("strFeedtype")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strKGN")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strKGN")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strKGN")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFE")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strKGN")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">"&Str.Item("strFE")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
    
   for r = 0 to NUMBER_FEED_ROW-1
      Response.Write("    <tr>" & vbCr)  
      Response.Write("      <td align=""left"">"&Str.Item(""&"str"&arrFeedBalance(r,0)&"")&"</td>" & vbCr)
      for c = 1 to NUMBER_FEED_COL-2
         Response.Write("      <td align=""right"">&nbsp;&nbsp;"&FormatNumber(arrFeedBalance(r,c),0,-1,0,0))
	 if (r=0 or r=1) and c=9 and arrFeedBalance(r,9)<>0 then Response.write "*"
	 Response.write ("</td>" & vbCr)
      next
      Response.Write("    </tr>" & vbCr)  
   next   

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&Str.Item("strIaltTOTAL")&"</th>" & vbCr)
   for c = 1 to NUMBER_FEED_COL-2
      Response.Write("      <th align=""right"">&nbsp;&nbsp;"&FormatNumber(arrFeedBalance(NUMBER_FEED_ROW,c),0,-1,0,0)&"</th>" & vbCr)
   next
   Response.Write("    </tr>" & vbCr)   

   
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <td align=""left"" colspan=""10""><hr/></td>" & vbCr)
   Response.Write("    </tr>" & vbCr)   

   Response.Write("  </table>" & vbCr)    

'********************************* Effektiviteter
   dim percentFE,percentFEBought,percentN,percentNBought,milkPrFE,effStable,nMilk,arrMilk, dictMilk
   dim TotalCattleFE, TotalCattleN, NeffSupplement, NeffPig, AddOn, Nneed, a, b
   ReDim preserve arrParameter(2)
   
   TotalCattleFE = arrFeedBalance(0,7)+arrFeedBalance(1,7)+arrFeedBalance(2,7)+arrFeedBalance(3,7)+arrFeedBalance(4,7)
   TotalCattleN = arrFeedBalance(0,8)+arrFeedBalance(1,8)+arrFeedBalance(2,8)+arrFeedBalance(3,8)+arrFeedBalance(4,8)

   set dictMilk = CreateObject("Scripting.Dictionary")
   nMilk = CallSP ("CalcTotalMilkkg " & CLng(Session("farmNumber")) & "," & CInt(Session("scenarioNumber")), arrMilk, dictMilk)
   nMilk = arrMilk(0, dictMilk.item("SumMilkkg"))

   
 '  nMilk = CalcTotalMilkkg(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrMilk) 
   
   if TotalCattleFE>0 then
   percentFE = (arrFeedBalance(0,7)+arrFeedBalance(1,7)+arrFeedBalance(2,7))/TotalCattleFE*100
   percentFEBought = arrFeedBalance(4,7)/TotalCattleFE*100
   milkPrFE = arrMilk(0,0)/TotalCattleFE*100   ' total kg mælk 

   else 
   percentFE = 0
   percentFEBought = 0
   milkPrFE = 0
   end if
   
   If IsNull(arrMilk(0,0)) Then 
   arrMilk(0,0)=0
   End If

   if TotalCattleN>0 then
   percentN = (arrFeedBalance(0,8)+arrFeedBalance(1,8)+arrFeedBalance(2,8))/TotalCattleN*100
   percentNBought = arrFeedBalance(4,8)/TotalCattleN*100
   
   arrParameter(2) = PARAMETER_GROWTH_N
   a = CallSPReturn ("GetFarmTotal",arrParameter)
 '  arrParameter(2) = PARAMETER_MILK_N
 '  b = CallSPReturn ("GetFarmTotal",arrParameter)
 '  response.write "<br>b " & b
   effStable = (a+arrMilk(0,0)*0.00518)/TotalCattleN*100
   
'   effStable = (GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_GROWTH_N)+GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), PARAMETER_MILK_N))/TotalCattleN*100
   else 
   percentN = 0
   percentNBought = 0
   effStable = 0
   end if

   if arrFeedBalance(3,7) + arrFeedBalance(4,7) > 0 then
   NeffSupplement =  (arrFeedBalance(3,8) + arrFeedBalance(4,8))/(arrFeedBalance(3,7) + arrFeedBalance(4,7))
   else 
   NeffSupplement = 0
   end if

   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
'   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <td align=""left"">"& Str.Item("strGrovfoderAfgræning") &"</td>" & vbCr)
''   Response.Write("      <td align=""right""><i>-999</i></td>" & vbCr)
'   Response.Write("      <td align=""right"">"&FormatNumber(percentFE,1,-1,0,0)&"</td>" & vbCr)
'   Response.Write("      <td align=""left"">%</td>" & vbCr)
'   Response.Write("      <td>&nbsp;</td>" & vbCr)
'   Response.Write("      <td align=""left"">"&Str.Item("strBoughtFE")&"</td>" & vbCr)
''   Response.Write("      <td align=""right""><i>-999</i></td>" & vbCr)
'   Response.Write("      <td align=""right"">"&FormatNumber(percentFEBought,1,-1,0,0)&"</td>" & vbCr)
'   Response.Write("      <td align=""left"">%</td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)   
'   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <td align=""left"">"&Str.Item("strNFEEDNGRAZE")&"</td>" & vbCr)
''   Response.Write("      <td align=""right""><i>-999</i></td>" & vbCr)
'   Response.Write("      <td align=""right"">"&FormatNumber(percentN,1,-1,0,0)&"</td>" & vbCr)
'   Response.Write("      <td align=""left"">%</td>" & vbCr)
'   Response.Write("      <td>&nbsp;</td>" & vbCr)
'   Response.Write("      <td align=""left"">"&Str.Item("strBoughtN")&"</td>" & vbCr)
''   Response.Write("      <td align=""right""><i>-999</i></td>" & vbCr)
'   Response.Write("      <td align=""right"">"&FormatNumber(percentNBought,1,-1,0,0)&"</td>" & vbCr)
'   Response.Write("      <td align=""left"">%</td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)   
''   Response.Write("    <tr>" & vbCr)   
''   Response.Write("      <td align=""left"">"&Str.Item("strPFEEDNGRAZE")&"</td>" & vbCr)
''   Response.Write("      <td align=""right"">0</td>" & vbCr)
''   Response.Write("      <td align=""left"">%</td>" & vbCr)
''   Response.Write("      <td align=""left"">"&Str.Item("strBOUGHT")&"</td>" & vbCr)
''   Response.Write("      <td align=""right"">0</td>" & vbCr)
''   Response.Write("      <td align=""left"">%</td>" & vbCr)
''   Response.Write("    </tr>" & vbCr)   
'   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <td align=""left"">"&Str.Item("strFEEffektivitet")&"</td>" & vbCr)
'   Response.Write("      <td align=""right"">"& vbCr)
'   if isnull(milkPrFE) then 
'   Response.Write(" - ")
'   else 
'   Response.write( FormatNumber(milkPrFE,1,-1,0,0) )
'   end if
'   Response.write("      </td>" & vbCr)
'   Response.Write("      <td align=""left"">"&Str.Item("strKg. mælk") & " " &  Str.Item("strpr. FE") & "</td>" & vbCr)
'   Response.Write("      <td colspan=""3"">&nbsp;</td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)   
'   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <td align=""left"">"&Str.Item("strNEffektivitet")&"</td>" & vbCr)
'   Response.Write("      <td align=""right"">"&FormatNumber(effStable,1,-1,0,0)&"</td>" & vbCr)
'   Response.Write("      <td align=""left"">%</td>" & vbCr)
'   Response.Write("      <td colspan=""3"">&nbsp;</td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)   
'   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <td align=""left"">"&Str.Item("strNEffektivitetSupplement")&"</td>" & vbCr)
'   Response.Write("      <td align=""right"">"&FormatNumber(NeffSupplement*1000,1,-1,0,0)&"</td>" & vbCr)
'   Response.Write("      <td align=""left"">g/"&Str.Item("strFE")&"</td>" & vbCr)
'   Response.Write("      <td colspan=""3"">&nbsp;</td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)   
''   Response.Write("    <tr>" & vbCr)   
''   Response.Write("      <td align=""left"">P: Effektivitet i stalden</td>" & vbCr)
''   Response.Write("      <td align=""right"">0</td>" & vbCr)
''   Response.Write("      <td align=""left"">%</td>" & vbCr)
''   Response.Write("      <td colspan=""3"">&nbsp;</td>" & vbCr)
''   Response.Write("    </tr>" & vbCr)  


   arrParameter(2) = 407
   AddOn = CallSPReturn ("GetFarmTotal",arrParameter)
'   AddOn = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 407)
   arrParameter(2) = 404
   Nneed = CallSPReturn ("GetFarmTotal",arrParameter)
'   Nneed = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 404)

   if Nneed > 0 then
   NeffPig = AddOn/Nneed*100

'   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <td align=""left"">"&Str.Item("strNEffektivitetPig")&"</td>" & vbCr)
'   Response.Write("      <td align=""right"">"&FormatNumber(NeffPig,1,-1,0,0)&"</td>" & vbCr)
'   Response.Write("      <td align=""left"">%</td>" & vbCr)
'   Response.Write("      <td colspan=""3"">&nbsp;</td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)   
   
   else 
   NeffPig = 0
   end if


'   Response.Write("    <tr>" & vbCr)   
'   Response.Write("      <td align=""left"" colspan=""7""><hr/></td>" & vbCr)
'   Response.Write("    </tr>" & vbCr)   

   Response.Write("  </table>" & vbCr) 
 '****************Advarsel hvis intern omsætning er forskellig fra 0.
   if FormatNumber(arrFeedBalance(0,9),1,-1,0,0) <> 0 then
      Response.Write("<b>&nbsp;&nbsp;*&nbsp; "& Str.Item("strWarningTurnover") & "</b><br>" )
   end if
   '****************Advarsel hvis N indhold i foder er for højt for kvæg.
   arrParameter(2) = PARAMETER_N
   a = CallSPReturn ("GetFarmTotal",arrParameter)
   arrParameter(2) = 404
   b = CallSPReturn ("GetFarmTotal",arrParameter)
   
   if FormatNumber(arrFeedBalance(NUMBER_FEED_ROW,8),1,-1,0,0) <> FormatNumber(a+b,1,-1,0,0) then
      Response.Write("&nbsp;&nbsp; "& Str.Item("strKvælstofbehov")& FormatNumber(a+b,0,-1,0,0)&" " & Str.Item("strKGNBARE")& "<br>" )
   end if
'*****************Advarsel hvis concentrationen FE/DM er højere i marken end behovet er i kvægbesætningen.    
   if HighEnergyConc > 0 then
      Response.Write("&nbsp;&nbsp; "& Str.Item("strHighEnergyConc")& "<br>" )
   end if

end sub
'**************************************************************************************************
sub InitTableManure()
   dim r,c,difference, arrDummy, Dummy, dictDummy,arrFarmScenario
   ReDim arrFarmScenario(1)
   arrFarmScenario(0) = CLng(Session("farmNumber"))
   arrFarmScenario(1) = CInt(Session("scenarioNumber"))
   
   ' Text 
   for r = 0 to nStorageType-1
      arrManure(r,0) = arrStorageType(r,dictStorageType.Item("StorageName"))
   next
   arrManure(nStorageType,0) = Str.Item("strIaltTOTAL")
   ' Produced manure nitrogen
   ReDim preserve arrParameter(2)
   for r = 0 to nStorageType-1
      arrParameter(2) = arrStorageType(r,dictStorageType.Item("StorageID"))
      arrManure(r,1) = CallSPReturn ("GetNitrogenStableBeforeLoss",arrParameter)
   next
   arrParameter(2) = PARAMETER_MANURE_N_FIELD
   ' Nitrogen loss from stable
   for r = 0 to nStorageType-1
      arrParameter(2) = arrStorageType(r,dictStorageType.Item("StorageID"))
      arrManure(r,2) = CallSPReturn ("GetNitrogenStableLoss",arrParameter)
 '	  arrManure(r,2) = GetNitrogenStableLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrStorageType(r,dictStorageType.Item("StorageID")))
   next
   ' Nitrogen loss from storage
   for r = 0 to nStorageType-1
      arrParameter(2) = arrStorageType(r,dictStorageType.Item("StorageID"))
      arrManure(r,3) = CallSPReturn ("GetNitrogenStorageLoss",arrParameter)
'      arrManure(r,3) = GetNitrogenStorageLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrStorageType(r,dictStorageType.Item("StorageID")))
   next
   ' Nitrogen loss from field
   for r = 0 to nStorageType-1
      arrParameter(2) = arrStorageType(r,dictStorageType.Item("StorageID"))
      arrManure(r,4) = CallSPReturn ("GetNitrogenSpreadLoss",arrParameter)
'      arrManure(r,4) = GetNitrogenSpreadLoss(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrStorageType(r,dictStorageType.Item("StorageID")))
   next
   ' Nitrogen in bought manure
   for r = 0 to nStorageType-1
    set dictDummy = CreateObject("Scripting.Dictionary")
    Dummy = CallSP ("GetManure " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrStorageType(r,dictStorageType.Item("StorageID")) & ", ""BoughtManure""" , arrDummy, dictDummy)
'	  Dummy = GetManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrStorageType(r,dictStorageType.Item("StorageID")), "BoughtManure", arrDummy)
      if ubound(arrDummy)<>-1 then
			  arrManure(r,5) = arrDummy(0,dictDummy.Item("BoughtManure"))
			else
			  arrManure(r,5) = 0
		  end if
   Set dictDummy = nothing
   next
	 
   arrManure(2,5) =  CallSPReturn ("fnGetBoughtGrazingN ",arrFarmScenario)
   arrManure(2,4) = (arrManure(2,5)+arrManure(2,1))*LOSS_MANURE_N_FIELD ' Special case for grazing
   ' Nitrogen need on field
   for r = 0 to nStorageType-1
      arrParameter(2) = arrStorageType(r,dictStorageType.Item("StorageID"))
      arrManure(r,7) = CallSPReturn ("GetNitrogenFieldUse",arrParameter)
   next
   ' Nitrogen in sold manure
   for r = 0 to nStorageType-1
      set dictDummy = CreateObject("Scripting.Dictionary")
      Dummy = CallSP ("GetManure " & Session("farmNumber")& "," & Session("scenarioNumber") & "," & arrStorageType(r,dictStorageType.Item("StorageID")) & ", ""SoldManure""" , arrDummy, dictDummy)
      if ubound(arrDummy)<>-1 then
			  arrManure(r,6) = arrDummy(0,dictDummy.Item("SoldManure"))
			else
			  arrManure(r,6) = 0
		  end if
   Set dictDummy = nothing
	
   next
   ' Balance
   for r = 0 to nStorageType-1
      arrManure(r,8) = 0
   Next
   
   '   for r = 0 to nStorageType-1
'     For c = 0 To UBound(arrManure,2)
'	   response.write " <br> arrManure("&r&","&c&") = "& arrManure(r,c)
'     Next
'   Next
 
   ' Total
   for r = 0 to nStorageType-1
      for c = 1 to NUMBER_MANURE_COL-1
         arrManure(nStorageType,c) = CDbl(arrManure(nStorageType,c)) + CDbl(arrManure(r,c))
      next
   next
  ' Response.write "<br> arrManure(2,1): " & arrManure(2,1) 

  ' Response.write "<br> arrManure(nStorageType,7): " & arrManure(nStorageType,7) 
end sub
'**************************************************************************************************
sub CreateTableFeed(HighEnergyConc)
   dim r,c, bought, sold
   
   Response.Write("  <table class=""data"" border=""0"">" & vbCr)

   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strFIELDPROD")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strBOUGHT")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strSOLD")&"</th>" & vbCr)
   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strHOUSINGUSE")&"</th>" & vbCr)
'   Response.Write("      <th align=""right"">&nbsp;&nbsp;"&Str.Item("strInternalExchge")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   Response.Write("    <tr>" & vbCr)   
   Response.Write("      <th align=""left"">"&Str.Item("strFeedtype")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strKg_toerstof")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strKg_toerstof")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strKg_toerstof")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strKg_toerstof")&"</th>" & vbCr)
   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""right"">&nbsp;&nbsp;"&Str.Item("strFE")&"</th>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   for r=0 to nFeedBalance-1
   bought = arrFeedBalance1(r,3)
   sold = arrFeedBalance1(r,4)
   if bought < 0 then
	 sold = sold + abs(bought)
     bought = 0
   end if 
   Response.Write("    <tr>" & vbCr)
   Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFeedBalance1(r,0)&"</td>" & vbCr)
   Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(arrFeedBalance1(r,1),1)&"</td>" & vbCr)
   Response.Write("      <td align=""left"">&nbsp;&nbsp;</td>" & vbCr)
   Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(arrFeedBalance1(r,2),1)&"</td>" & vbCr)
   Response.Write("      <td align=""left"">&nbsp;&nbsp;</td>" & vbCr)
   Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(arrFeedBalance1(r,3),1)&"</td>" & vbCr)
   Response.Write("      <td align=""left"">&nbsp;&nbsp;</td>" & vbCr)
   Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(arrFeedBalance1(r,4),1)&"</td>" & vbCr)
   Response.Write("      <td align=""left"">&nbsp;&nbsp;</td>" & vbCr)
   Response.Write("    </tr>" & vbCr)
   next
   Response.Write("  </table>" & vbCr) 
end sub
'**************************************************************************************************
sub CreateUpdateFormBalance()
   Response.Write("<form name=""update"" action=""Dummy.asp"" method=""post"">" & vbCr)
   Response.Write("<input type=""hidden"" name=""saveUpdates"" value=""false"">" & vbCr)
   Response.Write("</form>" & vbCr)
end sub
'**************************************************************************************************

%>
