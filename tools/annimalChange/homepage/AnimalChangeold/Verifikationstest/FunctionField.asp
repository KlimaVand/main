<%
'**************************************************************************************************
' Author: Margit Jørgensen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
'**************************************************************************************************
'sub CalcFieldPlanTotal(aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation)
'   InsertUpdateFieldPlanTotal PARAMETER_FE_CERNAL, aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation
'   InsertUpdateFieldPlanTotal PARAMETER_FE_STRAW, aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation
'   InsertUpdateFieldPlanTotal PARAMETER_FE_SECONDCROP, aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation
'   InsertUpdateFieldPlanTotal PARAMETER_N_CERNAL, aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation
'   InsertUpdateFieldPlanTotal PARAMETER_N_STRAW, aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation
'   InsertUpdateFieldPlanTotal PARAMETER_N_SECONDCROP, aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation
'   InsertUpdateFieldPlanTotal PARAMETER_P_CERNAL, aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation
'   InsertUpdateFieldPlanTotal PARAMETER_P_STRAW, aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation
'   InsertUpdateFieldPlanTotal PARAMETER_P_SECONDCROP, aFarmNumber, aScenarioID, aRotationID, aSoilTypeID, aIrrigation
'end sub
'**************************************************************************************************
sub CalculateManure(NPercent, scale,arrA,arrB, warning, TotalBoughtManure, TotalSoldManure)

' Bemærk at arrayet arrCalcSoldManure vender omvendt af arrSoldManure!!

Dim ObjectCoeff   ' matrix of objective function coefficients NumberOfFieldPlanRotations * NumberOfManureTypes
                  ' -10000 indicates that it is an impossible combination of manure and crop
Dim arrResult   ' Result array
'Dim arrA  ' array of ManureTypes
'Dim arrB  ' array of fields(FieldPlanRotations)
Dim arrDelivery  ' array of deliveryTypes for each result in arrResult
Dim i, j, k,totalNDelivered,totalNLoss', test
dim c, a, b, f
dim arrCalcSoldManure, totalCalcSoldManure
dim test, test2, aLoss
dim totalNNeed, FertEqNorm, BoughtFertilizer, FertEqProd, RotationNNeed
dim ExcessManure, fert, SumCalcSoldManure
dim arrHGdistribute, FertEqDist, GrazingConc, EnergyGrazed, NGrazed
Dim arrNPercent,dictNPercent, X 
Dim arrParameter
Dim arrManureNorm, dictManureNorm, totalManureNContentNorm
Dim arrManureNContent, arrManureNContent2, dictManureNContent
Dim arrSoldManure, dictSoldManure, totalSM
Dim arrBoughtManure, dictBoughtManure, totalBM
Dim SalesOrderList, dictSalesOrderList, SalesOrderTotal
Dim arrNNeed, dictNNeed, totalFertManureNContent
Dim DE, Reduktion, animalUnitKorr, animalUnit2

Dim arranimal, dictanimal, nanimal
Dim NprDE, animalUnitPrha, Test_Reduktion
Dim tid0,tid1,tid2,tid3,tid4

'Response.write "<br> NPercent " & NPercent
'tid0=Timer

	InitFarmNDB

   'Tabel som bestemmer indkøbt handelsgødning.   
   set dictNPercent = CreateObject("Scripting.Dictionary")
   X = CallSP ("GetNNeedPercent " & Session("farmNumber") & "," & Session("scenarioNumber"), arrNPercent, dictNPercent)
'response.write "<br> X " & X
   If UBound(arrNPercent)= -1 Then
     NPercent=100
   Else 
     NPercent = arrNPercent(0,0)
   End If
'   Response.write "<br> NPercent " & NPercent

'Initialisering af arrays
   set dictManureNorm = CreateObject("Scripting.Dictionary")
   X = CallSP ("GetManureNorm " & Session("farmNumber") & "," & Session("scenarioNumber"), arrManureNorm, dictManureNorm)
   totalManureNContentNorm = Round(SumDict(arrManureNorm,dictManureNorm,"Manure_N"),1)
'   Response.write "<br> totalManureNContentNorm "  & totalManureNContentNorm
'totalManureNContentNorm = GetManureNorm(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrManureNorm)

   set dictManureNContent = CreateObject("Scripting.Dictionary")
   X = CallSP ("GetManure_N_Content " & Session("farmNumber") & "," & Session("scenarioNumber"), arrManureNContent, dictManureNContent)
   totalManureNContent = Round(SumDict(arrManureNContent,dictManureNContent,"Manure_N"),1)
'   Response.write "<br> totalManureNContent "  & totalManureNContent
' totalManureNContent = GetManure_N_Content(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrManureNContent)
   arrManureNContent2 = arrManureNContent

   set dictSoldManure = CreateObject("Scripting.Dictionary")
   X = CallSP ("GetSoldManure " & Session("farmNumber") & "," & Session("scenarioNumber"), arrSoldManure, dictSoldManure)
   totalSM = Round(SumDict(arrSoldManure, dictSoldManure,"SoldManure"),1)
'   Response.write "<br> totalSM "  & totalSM

'totalSM = GetSoldManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrSoldManure)
   ReDim arrCalcSoldManure(UBound(arrSoldManure,2),UBound(arrSoldManure,1))
   For i=0 To UBound(arrSoldManure)
     For j=0 To UBound(arrSoldManure,2)
     arrCalcSoldManure(j,i) = arrSoldManure(i,j)
	 Next
   Next
   
   totalCalcSoldManure = totalSM
'   Response.write "<br> totalCalcSoldManure "  & totalCalcSoldManure
'totalCalcSoldManure = GetSoldManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrCalcSoldManure)

   set dictBoughtManure = CreateObject("Scripting.Dictionary")
'   X = CallSP ("GetBoughtManureExGrazingManure " & Session("farmNumber") & "," & Session("scenarioNumber"), arrBoughtManure, dictBoughtManure)
   X = CallSP ("GetBoughtManure " & Session("farmNumber") & "," & Session("scenarioNumber"), arrBoughtManure, dictBoughtManure)
   totalBM = Round(SumDict(arrBoughtManure, dictBoughtManure,"BoughtManure"),1)
'   Response.write "<br> totalBM "  & totalBM

'totalBM = GetBoughtManure(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrBoughtManure)

'Response.write "<br> arrCalcSoldManure " 
'PrintArrayInTable2Nina arrCalcSoldManure
'Response.write "<br> arrManureNContent " & totalManureNContent
'PrintArrayInTable2Nina arrManureNContent
'Response.write "<br> arrManureNorm " 
'PrintArrayInTable2Nina arrManureNorm
'Response.write "<br> arrSoldManure " 
'PrintArrayInTable2Nina arrSoldManure
'Response.write "<br> arrBoughtManure " 
'PrintArrayInTable2Nina arrBoughtManure

'Response.write "<br> totalCalcSoldManure "  & totalCalcSoldManure

'***********************************************************************
'Undersøger om det som ønsket solgt faktisk kan sælges: 
for i=0 to Ubound(arrSoldManure) 
test=1
  for j=0 to Ubound(arrManureNContent)
     if CInt(arrManureNContent(j,dictManureNContent.Item("StorageID")))=CInt(arrSoldManure(i,dictSoldManure.Item("SoldManureType"))) then
     test=0
       if CDbl(arrManureNContent(j,dictManureNContent.Item("Manure_N")))< CDbl(arrSoldManure(i,dictSoldManure.Item("SoldManure"))) then
'	  response.write "<br> kommer hertil "& arrManureNContent(j,dictManureNContent.Item("Manure_N")) & " " &  arrSoldManure(i,dictSoldManure.Item("SoldManure"))
	  arrCalcSoldManure(dictSoldManure.Item("SoldManure"),i)=arrManureNContent(j,dictManureNContent.Item("Manure_N"))
       arrManureNContent(j,dictManureNContent.Item("Manure_N")) = arrManureNContent(j,dictManureNContent.Item("Manure_N")) - arrCalcSoldManure(dictSoldManure.Item("SoldManure"),i)
	   Else 
       arrManureNContent(j,dictManureNContent.Item("Manure_N")) = arrManureNContent(j,dictManureNContent.Item("Manure_N")) - arrSoldManure(i,dictSoldManure.Item("SoldManure"))
       end if  

     end if
  next
if test=1 then 
   arrCalcSoldManure(dictSoldManure.Item("SoldManure"),i)=0
end if
next
'Response.write "<br> arrCalcSoldManure " &  SumDict2(arrCalcSoldManure, dictSoldManure,"SoldManure")
SumCalcSoldManure = Round(SumDict2(arrCalcSoldManure, dictSoldManure,"SoldManure"),1)
'Response.write "<br> SumCalcSoldManure " & SumCalcSoldManure
'PrintArrayInTable2Nina arrCalcSoldManure
'Response.write "<br> arrManureNContent " & totalManureNContent
'PrintArrayInTable2Nina arrManureNContent

'**************************************************************
redim arrParameter(1)
arrParameter(0) = CLng(Session("farmNumber"))
arrParameter(1) = CInt(Session("scenarioNumber"))

  Set dictanimal = CreateObject("Scripting.Dictionary")
   
   nanimal = CallSP("GetAnimalUnit "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")), arranimal, dictanimal)
   animalUnit = arranimal(0,dictanimal.item("DE"))
   totalArea = CallSPReturn ("GetTotalArea",arrParameter)
   Set dictanimal = Nothing
   
if animalUnit = 0 then
   NprDE = 100'??
else
   NprDE = totalManureNContent / animalUnit
end if

  
  Test_Reduktion = False 
  While Test_Reduktion = False    

'Finder handelsgødningsækvivalent af HG-norm + købt - solgt -- i CalcFertEqDict ganges N-indholdet med udnyttelsesgraden for hver gødn.type

FertEqNorm = CalcFertEqDict(arrManureNorm, dictManureNorm, "Manure_N", "N_UtilizationDegree") + CalcFertEqDict(arrBoughtManure, dictBoughtManure, "BoughtManure", "N_UtilizationDegree") - CalcFertEqDict2(arrCalcSoldManure, dictSoldManure, "SoldManure", "N_UtilizationDegree")
'response.write "<br> FertEqNorm "& FertEqNorm

'Finder total N behov på marker
totalNNeed = CallSPReturn("GetNitrogenFieldNeed", arrParameter)
'response.write "<br> totalNNeed "& totalNNeed

'totalNNeed = GetTotalNNeed(CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))) 
'response.write "<br> totalNNeed "& totalNNeed
totalNNeed = NPercent / 100 * totalNNeed
'response.write "<br> totalNNeed "& totalNNeed


'************************************************************************************
' Her skal totalManureNContent fratrækkes kvælstofreduktion som følge af efterafgrøder.
' DE udregnes som den på dyreholdet som er tastet, hertil lægges en dyreenhed pr 100 kg N importeret 
' og der fratrækkes en dyreenhed pr den i farmN bestemte kg N pr dyreenhed for export af egen gødning.
   

'   response.write "<br> totalManureNContent "& totalManureNContent

'   response.write "<br> NprDE "& NprDE

'   response.write "<br> animalUnit "& animalUnit
'if NprDE = 0 then NprDE = 100' sikring mod "division by zero"
   animalUnitKorr = animalUnit - SumCalcSoldManure/NprDE + totalBM / 100

'   response.write "<br> animalUnitKorr "& animalUnitKorr
if totalArea<>0 then
   animalUnitPrha = animalUnitKorr / totalArea
else
   animalUnitPrha = animalUnitKorr
end if

' response.write "<br> animalUnitPrha "& animalUnitPrha

  redim preserve arrParameter(2)
  arrParameter(2) = CDbl(animalUnitPrha)
  Reduktion = CallSPReturn("fnGetNormReduction", arrParameter)
'  response.write "<br> Reduktion som følge af efterafgrøder (kg N totalt) "& Reduktion & " "

  totalNNeed =   totalNNeed - Reduktion
'response.write "<br> totalNNeed "& totalNNeed
'***************************************************************************************



'Response.write "<br> FertEqNorm " & FertEqNorm
   set dictSalesOrderList = CreateObject("Scripting.Dictionary")
   SalesOrderTotal = CallSP ("GetSalesOrderList ", SalesOrderList, dictSalesOrderList)



ReDim arrHGdistribute(UBound(SalesOrderList),UBound(SalesOrderList,2)+1)
dictSalesOrderList.Add "N", 3

For i=0 To UBound(SalesOrderList)
  For j=0 To UBound(SalesOrderList,2)
    arrHGdistribute(i,j) = SalesOrderList(i,j)
  Next
  arrHGdistribute(i,dictSalesOrderList.Item("N")) = 0
Next



'SalesOrderTotal = GetSalesOrderList(SalesOrderList)
'arrHGdistribute = SalesOrderList
'Initialiserer vektor med id og mængde af hver type HG som skal fordeles samt udnyttelsesgrad
'Response.write "<br> arrCalcSoldManure " 
'PrintArrayInTable2Nina arrCalcSoldManure


'*****************************************************************************
'Afstemmer HG ækvivalent med behov ifølge regler og bestemmer købt handelsgødning. 
if CDbl(FertEqNorm) < CDbl(totalNNeed) then 
    BoughtFertilizer = totalNNeed - FertEqNorm
    if CDbl(totalManureNContentNorm-SumCalcSoldManure+totalBM) > 0 then 
    MeanUtilization = FertEqNorm/(totalManureNContentNorm-SumCalcSoldManure+totalBM)
    else 
    MeanUtilization = 0
    end if
'    response.write "<br> MeanUtilization "& MeanUtilization
else 
    ExcessManure = FertEqNorm-totalNNeed
'    response.write "<br> ExcessManure begyndelse "& ExcessManure
    BoughtFertilizer = 0
    for i=0 to Ubound(SalesOrderList)-1 ' Ubound(SalesOrderList) svarer til handelsgødning

        for j=0 to Ubound(arrManureNContent)
          if CInt(arrManureNContent(j,dictManureNContent.Item("StorageID"))) = CInt(SalesOrderList(i,dictSalesOrderList.Item("StorageID"))) then
	    fert=arrManureNContent(j,dictManureNContent.Item("Manure_N"))*SalesOrderList(i,dictSalesOrderList.Item("N_UtilizationDegree"))/100

'	    response.write "<br> fert "& fert
	      if CDbl(ExcessManure) > Cdbl(fert) then 
	         test = 0
	         for k=0 to Ubound(arrCalcSoldManure,2)
		       if CInt(arrManureNContent(j,dictManureNContent.Item("StorageID"))) = CInt(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k)) then 
		         test = 1
		         arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k) = arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k) + arrManureNContent(j,dictManureNContent.Item("Manure_N"))
		         arrManureNContent(j,dictManureNContent.Item("Manure_N")) = 0
		       end if
		     next
             if test = 0 then
		       Redim preserve arrCalcSoldManure(ubound(arrCalcSoldManure),ubound(arrCalcSoldManure,2)+1)
                     arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),ubound(arrCalcSoldManure,2)) = arrManureNContent(j,dictManureNContent.Item("StorageID"))
                     arrCalcSoldManure(dictSoldManure.Item("SoldManure"),ubound(arrCalcSoldManure,2)) = arrManureNContent(j,dictManureNContent.Item("Manure_N"))
                     arrCalcSoldManure(dictSoldManure.Item("N_UtilizationDegree"),ubound(arrCalcSoldManure,2)) = SalesOrderList(i,dictSalesOrderList.Item("N_UtilizationDegree"))
		             arrManureNContent(j,dictManureNContent.Item("Manure_N")) = 0
		     end if
		     ExcessManure = ExcessManure - fert
'      	    response.write "<br> ExcessManure opdateret "& ExcessManure
		   else 
             test = 0
		     for k=0 to Ubound(arrCalcSoldManure,2)
		        if CInt(arrManureNContent(j,dictManureNContent.Item("StorageID"))) = CInt(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k)) then 
		          test = 1
		          arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k) = arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k) + ExcessManure/SalesOrderList(i,dictSalesOrderList.Item("N_UtilizationDegree"))*100
		          arrManureNContent(j,dictManureNContent.Item("Manure_N")) = arrManureNContent(j,dictManureNContent.Item("Manure_N")) - ExcessManure/SalesOrderList(i,dictSalesOrderList.Item("N_UtilizationDegree"))*100
		        end if
		     next
             if test = 0 then
		       Redim preserve arrCalcSoldManure(ubound(arrCalcSoldManure),ubound(arrCalcSoldManure,2)+1)
                     arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),ubound(arrCalcSoldManure,2)) = arrManureNContent(j,dictManureNContent.Item("StorageID"))
                     arrCalcSoldManure(dictSoldManure.Item("SoldManure"),ubound(arrCalcSoldManure,2)) = ExcessManure/SalesOrderList(i,dictSalesOrderList.Item("N_UtilizationDegree"))*100
                     arrCalcSoldManure(dictSoldManure.Item("N_UtilizationDegree"),ubound(arrCalcSoldManure,2)) = SalesOrderList(i,dictSalesOrderList.Item("N_UtilizationDegree"))
		             arrManureNContent(j,dictManureNContent.Item("Manure_N")) = arrManureNContent(j,dictManureNContent.Item("Manure_N")) - ExcessManure/SalesOrderList(i,dictSalesOrderList.Item("N_UtilizationDegree"))*100
		     end if
             ExcessManure = 0 
          end if
	  end if
	next
     next

'Response.write "<br> arrCalcSoldManure " 
'PrintArrayInTable2Nina arrCalcSoldManure
'Response.write "<br> arrBoughtManure " 
'PrintArrayInTable2Nina arrBoughtManure

FertEqNorm = CalcFertEqDict(arrManureNorm, dictManureNorm, "Manure_N", "N_UtilizationDegree") + CalcFertEqDict(arrBoughtManure, dictBoughtManure, "BoughtManure", "N_UtilizationDegree") - CalcFertEqDict2(arrCalcSoldManure, dictSoldManure, "SoldManure", "N_UtilizationDegree")

ExcessManure = FertEqNorm-totalNNeed
'response.write "<br> ExcessManure " & ExcessManure

'  If Round(ExcessManure,2) > 0 Then
'  Response.write "<br> Overskydende indkøbt gødning skal sælges:  " 
'     for j=0 to Ubound(arrBoughtManure)
'	    fert=arrBoughtManure(j,dictBoughtManure.Item("BoughtManure"))*arrBoughtManure(j,dictBoughtManure.Item("N_UtilizationDegree"))/100
''		response.write "<br> fert " & fert
'        If CDbl(fert) > ExcessManure Then 
'		  fert = ExcessManure
'		End If
'			
'		test = 0
'	         for k=0 to Ubound(arrCalcSoldManure,2)
'		        if CInt(arrBoughtManure(j,dictBoughtManure.Item("BoughtManureType"))) = CInt(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k)) then 
'		          test = 1
'		          arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k) = arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k) + fert * 100 / arrBoughtManure(j,dictBoughtManure.Item("N_UtilizationDegree"))
'                  ExcessManure=ExcessManure-fert
'		        end if
'		     next
'             if test = 0 then
'		       Redim preserve arrCalcSoldManure(ubound(arrCalcSoldManure),ubound(arrCalcSoldManure,2)+1)
'                     arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),ubound(arrCalcSoldManure,2)) = arrBoughtManure(j,dictBoughtManure.Item("BoughtManureType"))
'                     arrCalcSoldManure(dictSoldManure.Item("SoldManure"),ubound(arrCalcSoldManure,2)) = fert * 100 / arrBoughtManure(j,dictBoughtManure.Item("N_UtilizationDegree"))
'                     arrCalcSoldManure(dictSoldManure.Item("N_UtilizationDegree"),ubound(arrCalcSoldManure,2)) = arrBoughtManure(j,dictBoughtManure.Item("N_UtilizationDegree"))
'                     ExcessManure=ExcessManure-fert
'		     end if
'          
'      Next
'
'  End If
'


SumCalcSoldManure =  Round(SumDict2(arrCalcSoldManure, dictSoldManure,"SoldManure"),1)
'Response.write "<br> SumCalcSoldManure "  & SumCalcSoldManure & " " & totalManureNContentNorm-SumCalcSoldManure+totalBM 
if CDbl(totalManureNContentNorm-SumCalcSoldManure+totalBM) <> 0 then 
MeanUtilization = FertEqNorm/(totalManureNContentNorm-SumCalcSoldManure+totalBM)
else 
MeanUtilization = 0 
end if
'    response.write "<br> MeanUtilization "& MeanUtilization
end if 

'    response.write "<br> MeanUtilization "& MeanUtilization

'***********************************************************************************************
'Response.write "<br> Efter norm afstemning arrCalcSoldManure " 
'PrintArrayInTable2Nina arrCalcSoldManure
'Response.write "<br> BoughtFertilizer " & BoughtFertilizer
 
   

' Fylde arrHGdistribute med det HG som skal fordeles: 
for i=0 to ubound(arrHGdistribute)
   for j=0 to ubound(arrManureNContent2)
     if CInt(arrHGdistribute(i,dictSalesOrderList.Item("StorageID"))) = CInt(arrManureNContent2(j,dictManureNContent.Item("StorageID"))) then 
       arrHGdistribute(i,dictSalesOrderList.Item("N")) = arrHGdistribute(i,dictSalesOrderList.Item("N")) + arrManureNContent2(j,dictManureNContent.Item("Manure_N")) 
     end if
   next
   for j=0 to ubound(arrBoughtManure)
     If Cint(arrHGdistribute(i,dictSalesOrderList.Item("StorageID"))) = CInt(arrBoughtManure(j,dictBoughtManure.Item("BoughtManureType"))) and (CInt(arrBoughtManure(j,dictBoughtManure.Item("BoughtManureType")))<>3) then
       arrHGdistribute(i,dictSalesOrderList.Item("N")) = arrHGdistribute(i,dictSalesOrderList.Item("N")) + arrBoughtManure(j,dictBoughtManure.Item("BoughtManure"))
       arrHGdistribute(i,dictSalesOrderList.Item("N_UtilizationDegree")) = arrBoughtManure(j,dictBoughtManure.Item("N_UtilizationDegree"))
     end if
   next
   for j=0 to ubound(arrCalcSoldManure,2)
     if CInt(arrHGdistribute(i,dictSalesOrderList.Item("StorageID"))) = CInt(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),j)) then
       arrHGdistribute(i,dictSalesOrderList.Item("N")) = arrHGdistribute(i,dictSalesOrderList.Item("N")) - arrCalcSoldManure(dictSoldManure.Item("SoldManure"),j)
     end if
   next
next
arrHGdistribute(ubound(arrHGdistribute),dictSalesOrderList.Item("N")) = BoughtFertilizer
arrHGdistribute(ubound(arrHGdistribute),dictSalesOrderList.Item("N_UtilizationDegree")) = 100

'Finder handelsgødnignsækvivalent af HG som skal fordeles samt købt handelsgødning. 
'FertEqDist = CalcFertEq(arrHGdistribute,1,2) 
FertEqDist = CalcFertEqDict(arrHGdistribute, dictSalesOrderList, "N", "N_UtilizationDegree")
'Response.write "<br>FertEqDist " & FertEqDist
 
'Response.write "<br> Efter Fylde arrHGdistribute " 
'Response.write "<br> arrManureNContent " 
'PrintArrayInTable2Nina arrManureNContent
'Response.write "<br> arrBoughtManure " 
'PrintArrayInTable2Nina arrBoughtManure
'Response.write "<br> arrCalcSoldManure " 
'PrintArrayInTable2Nina arrCalcSoldManure
'Response.write "<br> arrHGdistribute " 
'PrintArrayInTable2Nina arrHGdistribute

'************************************************************************************************


'Response.write "<br> FertEqDist " & FertEqDist
'response.write "<br> MeanUtilization "& MeanUtilization

'Finder det totale N behov efter at N behov opfyldt af N afsat under afgræsning er trukket fra.
redim preserve arrParameter(2)
'arrParameter(0) = CLng(Session("farmNumber"))
'arrParameter(1) = CInt(Session("scenarioNumber"))
arrParameter(2) = CInt(309)
NGrazed = CallSPReturn("GetFarmTotal", arrParameter)

arrParameter(2) = CInt(301)
EnergyGrazed = CallSPReturn("GetFarmTotal", arrParameter)

'NGrazed = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 309)
'EnergyGrazed = GetFarmTotal(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), 301)

if EnergyGrazed = 0 then
  GrazingConc = 0
else 
  GrazingConc = NGrazed/EnergyGrazed
end if
'response.write "<br> GrazingConc "& GrazingConc
'Response.write "<br> GrazingConc*MeanUtilization " & 10000*GrazingConc '& " " & GrazingConc & " "& MeanUtilization

   set dictNNeed = CreateObject("Scripting.Dictionary")
'   X = CallSP ("GetFieldNNeedExGrazing " & Session("farmNumber") & "," & Session("scenarioNumber") & ",'" & Cstr(replace(Round(10000*GrazingConc*MeanUtilization,1),",","."))&"'", arrNNeed, dictNNeed)
   X = CallSP ("GetFieldNNeedExGrazing " & Session("farmNumber") & "," & Session("scenarioNumber"), arrNNeed, dictNNeed)

   totalManureShareOfNNeed = Round(SumDict(arrNNeed,dictNNeed,"NNeed"),1)
' N-behov efter at afgræsning er fratrukket.


'Response.write "<br> totalManureShareOfNNeed " & totalManureShareOfNNeed
'Response.write "<br> I FunctionField arrHGdistribute "
'PrintArrayInTable2Nina arrHGdistribute

'Response.write "<br> I FunctionField arrNNeed "
'PrintArrayInTable2Nina arrNNeed

'Finder hvor stor en del af N behovet efter at N opfyldt af N afsat under afgræsning er trukket fra.
if totalManureShareOfNNeed > 0 then
scale = (FertEqDist) / totalManureShareOfNNeed
else 
scale = -1
end if
'Response.write "<br> scale " & scale

for i=0 to ubound(arrNNeed)
   arrNNeed(i,dictNNeed.Item("NNeed")) = Round(arrNNeed(i,dictNNeed.Item("NNeed")) * scale,2)
next
  
'Response.write "<br> I FunctionField arrNNeed efter scalering" 
'PrintArrayInTable2Nina arrNNeed


totalFertManureNContent = FertEqDist
totalManureNContent = SumDict(arrHGdistribute,dictSalesOrderList,"N")
'Response.write "<br> totalFertManureNContent: " & totalFertManureNContent 
'Response.write "<br> totalManureNContent: " & totalManureNContent 

redim preserve arrParameter(1)
'arrParameter(0) = CLng(Session("farmNumber"))
'arrParameter(1) = CInt(Session("scenarioNumber"))
X = CallSPReturn("DeleteFertilizer", arrParameter)
'Response.write "<br> DeleteFertilizer X " & X 
X = CallSPReturn("DeleteManureFertilizerDelivery", arrParameter)
'Response.write "<br> DeleteManureFertilizerDelivery X " & X 
tid0=Timer


'DeleteFertilizer CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))
'DeleteManureFertilizerDelivery CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))

	
	Redim ObjectCoeff(Ubound(arrHGdistribute),Ubound(arrNNeed))
	Redim arrResult(Ubound(arrHGdistribute),Ubound(arrNNeed))
	Redim arrDelivery(Ubound(arrHGdistribute),Ubound(arrNNeed))
	Redim arrA(Ubound(arrHGdistribute))
	Redim arrB(Ubound(arrNNeed))

    InitArray ObjectCoeff,-10000
    InitArray arrResult,-1

'Response.write "<br> arrA "
'PrintArrayInTableNina arrA
'Response.write "<br> arrB "
'PrintArrayInTableNina arrB
'Response.write "<br> ObjectCoeff "
'PrintArrayInTable2Nina ObjectCoeff
'Response.write "<br> arrDelivery "
'PrintArrayInTable2Nina arrDelivery
'Response.write "<br> arrResult "
'PrintArrayInTable2Nina arrResult
if totalManureShareOfNNeed > 0 and totalManureNContent>0 then  
FillArrays c,f,arrA,arrB,arrHGdistribute,arrNNeed,ObjectCoeff,arrDelivery,totalManureShareOfNNeed,totalManureNContent,BoughtFertilizer,arrCalcSoldManure,arrBoughtManure,SalesOrderList,scale,dictNNeed,dictSalesOrderList
'tid1=Timer
'if Cint(Session("userType"))=50 then
'Response.write "<br> Efter Fill arrays"
'Response.write "<br> arrA "
'PrintArrayInTableNina arrA
'Response.write "<br> arrB "
'PrintArrayInTableNina arrB
'Response.write "<br> ObjectCoeff "
'PrintArrayInTable2Nina ObjectCoeff
'Response.write "<br> arrDelivery "
'PrintArrayInTable2Nina arrDelivery
'Response.write "<br> arrResult "
'PrintArrayInTable2Nina arrResult
'end if
	   
    test = calcRotation(arrA,arrB,ObjectCoeff,arrResult)
'    Response.write "<br> test " & test 
 '   Response.write "<br> arrResult "
 '   PrintArrayInTable2Nina arrResult

while test = False
if Ubound(arrNNeed)<>-1 then
	   for a=0 to Ubound(arrA)-1
	    for b=0 to Ubound(arrB)
	     if arrResult(a,b)<>0 and ObjectCoeff(a,b)<-1000 then 
'               Response.write "<br> a, b " & a & b 
	       BoughtFertilizer = BoughtFertilizer + Round(arrResult(a,b),2)
	       warning=1
'               Response.write "<br> BoughtFertilizer " & BoughtFertilizer
               arrA(a) = arrA(a) - Round(arrResult(a,b),2)
    	       arrA(Ubound(arrA)) = arrA(Ubound(arrA)) + Round(arrResult(a,b),2)
	          

                 test2=0
	         for k=0 to Ubound(arrCalcSoldManure,2)
		    if CInt(arrHGdistribute(a,dictSalesOrderList.Item("StorageID"))) = CInt(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k)) then 
		       test2 = 1
		       arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k) = arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k) + arrResult(a,b)*100/arrHGdistribute(a,dictSalesOrderList.Item("N_UtilizationDegree"))
		       arrHGdistribute(a,dictSalesOrderList.Item("N")) = arrHGdistribute(a,dictSalesOrderList.Item("N")) - arrResult(a,b)*100/arrHGdistribute(a,dictSalesOrderList.Item("N_UtilizationDegree"))
               arrHGdistribute(ubound(arrHGdistribute),dictSalesOrderList.Item("N")) = arrHGdistribute(ubound(arrHGdistribute),dictSalesOrderList.Item("N")) + arrResult(a,b)

		     end if
		 next
                if test2 = 0 then
		     Redim preserve arrCalcSoldManure(ubound(arrCalcSoldManure),ubound(arrCalcSoldManure,2)+1)
                     arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),ubound(arrCalcSoldManure,2))= CInt(arrHGdistribute(a,dictSalesOrderList.Item("StorageID")))
'                     response.write "<br> arrCalcSoldManure (type) " & arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),ubound(arrCalcSoldManure,2))
					 if arrHGdistribute(a,dictSalesOrderList.Item("N_UtilizationDegree"))=0 then arrHGdistribute(a,dictSalesOrderList.Item("N_UtilizationDegree"))=1
					 arrCalcSoldManure(dictSoldManure.Item("SoldManure"),ubound(arrCalcSoldManure,2))= arrResult(a,b)*100/arrHGdistribute(a,dictSalesOrderList.Item("N_UtilizationDegree"))
'                     response.write "<br> arrResult " & arrResult(a,b)
'					 response.write "<br> arrHGdistribute N_UtilizationDegree" & arrHGdistribute(a,dictSalesOrderList.Item("N_UtilizationDegree"))
'                     response.write "<br> arrCalcSoldManure (manure) " & arrCalcSoldManure(dictSoldManure.Item("SoldManure"),ubound(arrCalcSoldManure,2))
					 arrCalcSoldManure(dictSoldManure.Item("N_UtilizationDegree"),ubound(arrCalcSoldManure,2))= arrHGdistribute(a,dictSalesOrderList.Item("N_UtilizationDegree"))
        		     arrHGdistribute(a,dictSalesOrderList.Item("N"))  = arrHGdistribute(a,dictSalesOrderList.Item("N")) - arrResult(a,b)*100/arrHGdistribute(a,dictSalesOrderList.Item("N_UtilizationDegree"))
  	                 arrHGdistribute(ubound(arrHGdistribute),dictSalesOrderList.Item("N")) =arrHGdistribute(ubound(arrHGdistribute),dictSalesOrderList.Item("N")) +arrResult(a,b)
		 end if
	     end if
            next
	   next

    test = calcRotation(arrA,arrB,ObjectCoeff,arrResult)
'    Response.write "<br> test   " & test 
'    Response.write "<br> arrResult "
'    PrintArrayInTable2Nina arrResult

end if
Wend


end if
'    Response.write "<br> test efter loop" & test 
'    Response.write "<br> arrResult "
'    PrintArrayInTable2Nina arrResult

if totalManureShareOfNNeed = 0 and totalManureNContent > 0 then  
	     for a=0 to Ubound(arrHGdistribute,2)
             test2=0
	         for k=0 to Ubound(arrCalcSoldManure,2)
		    If CInt(arrHGdistribute(a,dictSalesOrderList.Item("StorageID"))) = CInt(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k)) then 
		       test2 = 1
		       arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k) = arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k) + arrHGdistribute(a,dictSalesOrderList.Item("N"))
		       arrHGdistribute(a,dictSalesOrderList.Item("N")) = 0
		     end if
		 next
                 if test2 = 0 then
		     Redim preserve arrCalcSoldManure(ubound(arrCalcSoldManure),ubound(arrCalcSoldManure,2)+1)
                     arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),ubound(arrCalcSoldManure,2))= CInt(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k))
                     arrCalcSoldManure(dictSoldManure.Item("SoldManure"),ubound(arrCalcSoldManure,2))= arrHGdistribute(a,dictSalesOrderList.Item("N"))
                     arrCalcSoldManure(dictSoldManure.Item("N_UtilizationDegree"),ubound(arrCalcSoldManure,2))= arrHGdistribute(a,dictSalesOrderList.Item("N_UtilizationDegree"))
		     arrHGdistribute(1,a) = 0
	         end if
	     next
end if


totalBM = Round(SumDict(arrBoughtManure, dictBoughtManure,"BoughtManure"),1)

SumCalcSoldManure = Round(SumDict2(arrCalcSoldManure, dictSoldManure,"SoldManure"),1)

animalUnit2 = animalUnit - SumCalcSoldManure/NprDE + totalBM / 100

  If CDbl(animalUnitKorr)=CDbl(animalUnit2) Then
     Test_Reduktion = True
  End If

Wend
tid2=Timer

'redim preserve arrParameter(3)
'arrParameter(2) = 1'parameter for N-behov minus normreduktion
'arrParameter(3) = totalNNeed'Det justerede N-behov

'   X = CallSPReturn ("InsertUpdateResult " , arrParameter)
'response.write "<br>totalNNeed = "&   totalNNeed


InsertUpdateManureDelivery arrA,arrB,ObjectCoeff,arrResult,arrDelivery,arrHGdistribute,arrNNeed,dictNNeed,dictSalesOrderList
InsertUpdateManureFertilizer BoughtFertilizer,arrCalcSoldManure,arrBoughtManure,SalesOrderList, dictSoldManure, dictBoughtManure, dictSalesOrderList

' Inserted og beregnet N-Less pr record i fieldplanrotation 


TotalBoughtManure = totalBM
TotalSoldManure =  SumDict2(arrCalcSoldManure, dictSoldManure,"SoldManure")
'  response.write "<br> NprDE "& NprDE
  response.write "<br> Reduktion som følge af efterafgrøder (kg N totalt) "& round(Reduktion,2) & " "
tid3=Timer
'response.write "<br>Tid0 1  = "&   tid1-tid0
'response.write "<br>Tid1 2  = "&   tid2-tid1
'response.write "<br>Tid2 3  = "&   tid3-tid2

end sub
'**************************************************************************************************
sub FillArrays(c,f,arrA,arrB,arrHGdistribute,arrNNeed,ObjectCoeff,arrDelivery,totalManureShareOfNNeed,totalManureNContent,BoughtFertilizer,arrCalcSoldManure,arrBoughtManure,SalesOrderList,scale,dictNNeed,dictSalesOrderList)
dim degree,delType,a,b,k,fert,SumA,SumB
Dim arrUtilDegree, dictUtilDegree, X
'Dim test
for a=0 to Ubound(arrHGdistribute)
 arrA(a)=Round(arrHGdistribute(a,dictSalesOrderList.Item("N"))*arrHGdistribute(a,dictSalesOrderList.Item("N_UtilizationDegree"))/100,2)
' test=0
 for b=0 to Ubound(arrNNeed)
    arrB(b)=Round(arrNNeed(b,dictNNeed.Item("NNeed")),2)
'    response.write "<br> arrB("&b&") "& arrB(b)
   set dictUtilDegree = CreateObject("Scripting.Dictionary")
   X = CallSP ("GetUtilizationDegree " & Session("farmNumber") & "," & Session("scenarioNumber") & "," & arrNNeed(b,dictNNeed.Item("RotationID")) & "," & arrNNeed(b,dictNNeed.Item("FieldPlanRotationID")) & "," & arrHGdistribute(a,dictSalesOrderList.Item("StorageID")), arrUtilDegree, dictUtilDegree)
'    Response.write "<br> arrUtilDegree "
'    PrintArrayInTable2Nina arrUtilDegree
   If UBound(arrUtilDegree) <> -1 Then
  '   Response.write "<br> I løkken som sætter koef "

     degree = CInt(arrUtilDegree(0,dictUtilDegree.Item("UtilizationDegree")))
     delType = CInt(arrUtilDegree(0,dictUtilDegree.Item("DeliveryID")))
	 fert =  CBool(arrUtilDegree(0,dictUtilDegree.Item("OrganicFertilizer")))

	'degree =	GetUtilizationDegree(CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),arrNNeed(b,dictNNeed.Item("RotationID")),arrNNeed(b,dictNNeed.Item("FieldPlanRotationID")),arrHGdistribute(a,dictSalesOrderList.Item("StorageID")),delType,fert)

      if fert = true then 
'        test = 1
	    ObjectCoeff(a,b)=degree
	    arrDelivery(a,b)=delType
      else 
         if arrHGdistribute(a,dictSalesOrderList.Item("StorageID"))=9 then
           ObjectCoeff(a,b)=degree
	       arrDelivery(a,b)=delType
		 Else 
		   ObjectCoeff(a,b)=-10000
           arrDelivery(a,b)=-1
         end if
      end If
	End If
    If UBound(arrUtilDegree) = -1 Then
        ObjectCoeff(a,b)=-10000
        arrDelivery(a,b)=-1
	End If 
	       
   set dictUtilDegree = Nothing
 next
next
'Response.write "<br> arrA i Fill arrays"
'PrintArrayInTableNina arrA
'Response.write "<br> arrB i Fill arrays "
'PrintArrayInTableNina arrB

    For a = 0 to UBound(arrA)
      SumA = SumA + arrA(a)
    Next
    For b = 0 to UBound(arrB)
      SumB = SumB + arrB(b)
    Next

'Response.write "<br> SumA " & SumA & "  SumB " & SumB  

if SumA<SumB then
 arrA(UBound(arrA)) = Round(arrA(UBound(arrA)),2) + Round(SumB-SumA,2)
else
 arrB(UBound(arrB)) = Round(arrB(UBound(arrB)),2) + Round(SumA-SumB,2)
end if

'Response.write "<br> arrA i Fill arrays 2 "
'PrintArrayInTableNina arrA
'Response.write "<br> arrB i Fill arrays  2 "
'PrintArrayInTableNina arrB

end sub 
'**************************************************************************************************
sub InsertUpdateManureDelivery(arrA,arrB,ObjectCoeff,arrResult,arrDelivery,arrHGdistribute,arrNNeed,dictNNeed,dictSalesOrderList)
dim a,b,aLoss,m,test
Dim nManureType, arrManureType
Dim arrParameter, X
ReDim arrParameter(1)
Dim arrParameterInsert(8)

       arrParameterInsert(0) = CLng(Session("farmNumber"))
	   arrParameterInsert(1) = CInt(Session("scenarioNumber"))
  
 	  arrParameter(0) = CLng(Session("farmNumber"))
      arrParameter(1) = CInt(Session("scenarioNumber"))
       X = CallSPReturn("DeleteManureFertilizerDelivery", arrParameter)
   
'       DeleteManureFertilizerDelivery CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))
	   for a=0 to Ubound(arrA)
	    for b=0 to Ubound(arrB)
		' Hvis der er noget gødning som udbringes eller hvis afgrøden har norm 0 og a=9 er handelsgødning 
		' så indsættes en rekord i ManureFertilizer tabellen.
'	     if (arrResult(a,b)>0.01 And ObjectCoeff(a,b)>0) Or (CInt(a)=CInt(9) And CInt(arrB(b))=CInt(0)) then 
	     if (arrResult(a,b)>0.01 And ObjectCoeff(a,b)>0) then 
            arrResult(a,b)=round(arrResult(a,b)*100/arrHGdistribute(a,dictSalesOrderList("N_UtilizationDegree")),2)

            arrParameter(0) = arrHGdistribute(a,dictSalesOrderList("StorageID"))
            arrParameter(1) = arrDelivery(a,b)
            aLoss = CallSPReturn("GetLoss", arrParameter)

	        arrParameterInsert(2) = CInt(arrNNeed(b,dictNNeed.Item("RotationID")))
			arrParameterInsert(3) = CInt(arrNNeed(b,dictNNeed.Item("FieldPlanRotationID")))
			arrParameterInsert(4) = CInt(arrHGdistribute(a,dictSalesOrderList("StorageID")))
			arrParameterInsert(5) = CInt(arrDelivery(a,b))
			arrParameterInsert(6) = Cdbl(arrResult(a,b))
			arrParameterInsert(7) = CDbl(Round(aLoss*arrResult(a,b)/100,2))
            arrParameterInsert(8) = Cdbl(arrHGdistribute(a,dictSalesOrderList("N_UtilizationDegree"))*arrResult(a,b))/100
            X = CallSPReturn("InsertUpdateManureFertilizerDelivery", arrParameterInsert)


'			 aLoss=GetLoss(arrHGdistribute(0,a), arrDelivery(a,b))

'			 InsertUpdateManureFertilizerDelivery CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrNNeed(0,b), arrNNeed(1,b),arrHGdistribute(0,a), arrDelivery(a,b), arrResult(a,b),Round(aLoss*arrResult(a,b)/100,2),(arrHGdistribute(2,a)*arrResult(a,b))/100
		 end if
	     if arrResult(a,b)<>0 and ObjectCoeff(a,b)=0 then 
            arrParameter(0) = 9
            arrParameter(1) = 5
            aLoss = CallSPReturn("GetLoss", arrParameter)
'			 aLoss=GetLoss(9, 5)

			arrParameterInsert(2) = CInt(arrNNeed(b,dictNNeed.Item("RotationID")))
			arrParameterInsert(3) = CInt(arrNNeed(b,dictNNeed.Item("FieldPlanRotationID")))
			arrParameterInsert(4) = CInt(9)
			arrParameterInsert(5) = CInt(5)
			arrParameterInsert(6) = Cdbl(arrResult(a,b))
			arrParameterInsert(7) = CDbl(Round(aLoss*arrResult(a,b)/100,2))
            arrParameterInsert(8) = Cdbl(arrResult(a,b))

            X = CallSPReturn("InsertUpdateManureFertilizerDelivery", arrParameterInsert)
			 
'			 InsertUpdateManureFertilizerDelivery CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrNNeed(0,b), arrNNeed(1,b),9, 5, arrResult(a,b),Round(aLoss*arrResult(a,b)/100,2),arrResult(a,b)
		 end if
		next
	   next
	   

end sub 
'**************************************************************************************************
sub InsertUpdateManureFertilizer(BoughtFertilizer,arrCalcSoldManure,arrBoughtManure,SalesOrderList, dictSoldManure, dictBoughtManure, dictSalesOrderList)
dim i,k,j,test
Dim arrParameter, X
Dim arrParameterInsert(4)
ReDim arrParameter(1)
'Response.write "<br> arrCalcSoldManure "
'PrintArrayInTable2Nina arrCalcSoldManure

       arrParameterInsert(0) = CLng(Session("farmNumber"))
	   arrParameterInsert(1) = CInt(Session("scenarioNumber"))

 	  arrParameter(0) = CLng(Session("farmNumber"))
      arrParameter(1) = CInt(Session("scenarioNumber"))
       X = CallSPReturn("DeleteFertilizer", arrParameter)
       X = CallSPReturn("DeleteCalcBoughtSoldManure", arrParameter)

ReDim preserve arrParameter(2)
      arrParameter(2) = CDbl(BoughtFertilizer)
       X = CallSPReturn("InsertFertilizer", arrParameter)


'DeleteFertilizer   CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))
'InsertFertilizer   CLng(Session("farmNumber")),CInt(Session("scenarioNumber")),BoughtFertilizer
'Response.write "<br> I funktionen InsertUpdateManureFertilizer  "

'DeleteCalcBoughtSoldManure   CLng(Session("farmNumber")),CInt(Session("scenarioNumber"))

for i=0 to Ubound(SalesOrderList)
   for k=0 to Ubound(arrCalcSoldManure,2)
     if cint(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k)) = cint(SalesOrderList(i,dictSalesOrderList.Item("StorageID"))) then
        test = 0 
	for j = 0 to ubound(arrBoughtManure)
	  if cint(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k)) = cint(arrBoughtManure(j,dictBoughtManure.Item("BoughtManureType"))) then
	
	        arrParameterInsert(2) = Cint(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k))
			arrParameterInsert(3) = CDbl(arrBoughtManure(j,dictBoughtManure.Item("BoughtManure")))
			arrParameterInsert(4) = CDbl(arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k))
            X = CallSPReturn("InsertUpdateBoughtSoldManure", arrParameterInsert)
 'Response.write "<br> InsertUpdateBoughtSoldManure X " & X 

'Response.write "<br> Efter InsertUpdateBoughtSoldManure "
'PrintArrayInTableNina arrParameterInsert
		 
'		 InsertUpdateBoughtSoldManure CLng(Session("farmNumber")), CInt(Session("scenarioNumber")), arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k), 	arrBoughtManure(j,dictBoughtManure.Item("BoughtManure")), arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k)

'	     Response.write "<br> Inserter både købt og solgt "
	     test = 1
	  end if
	next
	if test = 0 then
'	     Response.write "<br> Inserter kun  solgt "
	        arrParameterInsert(2) = Cint(arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k))
			arrParameterInsert(3) = CDbl(0)
			arrParameterInsert(4) = CDbl(arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k))
            X = CallSPReturn("InsertUpdateBoughtSoldManure", arrParameterInsert)
 'Response.write "<br> InsertUpdateBoughtSoldManure X " & X 

'Response.write "<br> Efter InsertUpdateBoughtSoldManure "
'PrintArrayInTableNina arrParameterInsert

'	InsertUpdateBoughtSoldManure CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),k), 0, arrCalcSoldManure(dictSoldManure.Item("SoldManure"),k)
	end if
     end if
   next
   for k=0 to Ubound(arrBoughtManure)
     if arrBoughtManure(k,dictBoughtManure.Item("BoughtManureType")) = SalesOrderList(i,dictSalesOrderList.Item("StorageID")) then
        test = 0 
	for j = 0 to ubound(arrCalcSoldManure,2)
	  if arrCalcSoldManure(dictSoldManure.Item("SoldManureType"),j) = arrBoughtManure(k,dictBoughtManure.Item("BoughtManureType")) then
	     test = 1
	  end if
	next
	if test = 0 then
'	     Response.write "<br> Inserter kun  købt "
	        arrParameterInsert(2) = Cint(arrBoughtManure(k,dictBoughtManure.Item("BoughtManureType")))
			arrParameterInsert(3) = CDbl(arrBoughtManure(k,dictBoughtManure.Item("BoughtManure")))
			arrParameterInsert(4) = CDbl(0)
            X = CallSPReturn("InsertUpdateBoughtSoldManure", arrParameterInsert)

	
'	InsertUpdateBoughtSoldManure CLng(Session("farmNumber")),CInt(Session("scenarioNumber")), arrBoughtManure(k,dictBoughtManure.Item("BoughtManureType")), arrBoughtManure(k,dictBoughtManure.Item("BoughtManure")), 0
	end if
     end if
   next

next

end sub 
'**************************************************************************************************
'function GetFertEquivManure_N_Content(arrManureNContent)
'   dim index,totalFertNContent
'   
'   
'   for index=0 to Ubound(arrManureNContent ,2 ) 
'      totalFertNContent=totalFertNContent+Round(arrManureNContent(2,index)*arrManureNContent(3,index)/100,2)
'   next 
'   
'   GetFertEquivManure_N_Content = totalFertNContent  
'end function
'**************************************************************************************************

'function CalcFertEq(anArr, i, j)
'dim k,total

'  for k=0 to Ubound(anArr,2) -1  
'     total = total + Round(anArr(i,k) * anArr(j,k)/100 , 2)
'  next
  
'  CalcFertEq =  total

'end function
'**************************************************************************************************

function CalcFertEqDict(anArr, anDict, strN, strPercent)
dim k,total

  for k=0 to Ubound(anArr)  
     total = total + Round(CDbl(anArr(k,anDict.Item(""&strN&""))) * CDbl(anArr(k,anDict.Item(""&strPercent&"")))/100 , 2)
  next
  
  CalcFertEqDict =  total

end function
'**************************************************************************************************

function CalcFertEqDict2(anArr, anDict, strN, strPercent)
dim k,total

  for k=0 to Ubound(anArr,2)  
     total = total + Round(CDbl(anArr(anDict.Item(""&strN&""),k)) * CDbl(anArr(anDict.Item(""&strPercent&""),k))/100 , 2)
  next
  
  CalcFertEqDict2 =  total

end function

'**************************************************************************************************
function CalcN_Les_Nina(meanNLES_N,meanNLES_mg,meanRunOff,arrNLES)
dim arrRotation,aRotationID,nRotation,dictRotation,r
dim arrFieldPlanRotation,aFieldPlanRotation,nFieldPlanRotation,dictFieldPlanRotation,f
dim NLes,arrParameter,RunOff,NLESmg,aSecondCrop,aPreCropSecondCrop,Area,TotalNLES_N,totalArea
dim totalRunOff
redim arrParameter(1)
   arrParameter(0)=CLng(Session("farmNumber"))
   arrParameter(1)=CInt(Session("scenarioNumber"))
   totalArea = CallSPReturn ("GetTotalArea",arrParameter)
	 if totalArea = 0 then totalArea = 1
'   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
   set dictRotation = CreateObject("Scripting.Dictionary")
   nRotation = CallSP("GetRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")),arrRotation,dictRotation)
redim preserve arrParameter(3)
for r = 0 to nRotation-1
    aRotationID = arrRotation(r,dictRotation.Item("RotationID"))
    arrParameter(2)=aRotationID
'    Response.Write("      <th align=""left"">"&arrRotation(r,dictRotation.Item("RotationName"))&"<br/></th>" & vbCr)
'    Response.Write("    <tr>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCARRYOVER")&"</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAfterCrop")&"</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCROP")&"</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAfterCrop")&"</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;N-LES<br />&nbsp;&nbsp;Kg.N/Ha</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;N-LES<br />&nbsp;&nbsp;mg NO<sub>3</sub>/l</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;Afstrømning<br />&nbsp;&nbsp;mm</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;Areal<br />&nbsp;&nbsp;Ha</th>" & vbCr)
'    Response.Write("    </tr>" & vbCr)
		set dictFieldPlanRotation = CreateObject("Scripting.Dictionary")
		nFieldPlanRotation = CallSP("GetFieldPlanRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber"))&","&aRotationID,arrFieldPlanRotation,dictFieldPlanRotation)
		for f=0 to nFieldPlanRotation-1
			aSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID"))
			aPreCropSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropSecondCrop"))
			aFieldPlanRotation = arrFieldPlanRotation(f,dictFieldPlanRotation.Item("FieldPlanRotationID"))
			arrParameter(3)=aFieldPlanRotation
			NLes=CallSPReturn("CalcN_LesPrFieldPlanRotation ",arrParameter)
			Area=Cdbl(arrFieldPlanRotation(f,dictFieldPlanRotation.Item("Area")))
			TotalNLES_N=TotalNLES_N+(NLes*Area)
			RunOff=CallSPReturn("GetRunOff ",arrParameter)
			totalRunOff=totalRunOff+(RunOff*Area)
			if RunOff<>0 then 
			  NLESmg=100*NLes*4.43/RunOff'NLES (KgN/Ha)-> 1000000*NLES (mg/Ha). RunOff (liter/squareMeter)-> 10000*RunOff (liter/Ha)
			else
			  NLESmg=100*NLes*4.43/1
			end if
'			Response.Write("    <tr>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("PreCropOriginalName")))
'			if CInt(aPreCropSecondCrop)>CInt(0) And  Not(CInt(aPreCropSecondCrop)=CInt(15)) And Not(CInt(aPreCropSecondCrop)=CInt(21)) then
'			  Response.write(" "&Str.Item("strUnderSown"))
'			end if
'			Response.Write("</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("PreCrop_AfterCrop_Name"))&"</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("CropName")))
'			if CInt(aSecondCrop)>CInt(0) And  Not(CInt(aSecondCrop)=CInt(15)) And Not(CInt(aSecondCrop)=CInt(21)) then
'			  Response.write(" "&Str.Item("strUnderSown"))
'			end if
'			Response.Write("</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("Crop_AfterCrop_Name"))&"</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(NLes,2)&"</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(NLesmg,2)&"</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(RunOff,2)&"</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Area&"</td>" & vbCr)
'			Response.Write("    </tr>" & vbCr)
	  next
next
     meanNLES_N = TotalNLES_N/totalArea
	 meanRunOff = TotalRunOff/totalArea
	 if meanRunOff = 0 then meanRunOff = 1
	 meanNLES_mg = 100*meanNLES_N*4.43/meanRunOff
'   Response.Write("    </tr>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strMeanOnFarm")&"</th>" & vbCr)
''   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(meanNLES_N,2)&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(meanNLES_mg,2)&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(meanRunOff,2)&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'   Response.Write("    </tr>" & vbCr)
'   Response.Write("  </table>" & vbCr)
end function
'**************************************************************************************************
function CalcN_Les(meanNLES_N,meanNLES_mg,meanRunOff,arrNLES)
dim arrRotation,aRotationID,nRotation,dictRotation,r
dim arrFieldPlanRotation,aFieldPlanRotation,nFieldPlanRotation,dictFieldPlanRotation,f
dim NLes,arrParameter,RunOff,NLESmg,aSecondCrop,aPreCropSecondCrop,Area,TotalNLES_N,totalArea
dim totalRunOff
redim arrParameter(1)
   arrParameter(0)=CLng(Session("farmNumber"))
   arrParameter(1)=CInt(Session("scenarioNumber"))
   totalArea = CallSPReturn ("GetTotalArea",arrParameter)
	 if totalArea = 0 then totalArea = 1
'   Response.Write("  <table class=""data"" border=""0"">" & vbCr)
   set dictRotation = CreateObject("Scripting.Dictionary")
   nRotation = CallSP("GetRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber")),arrRotation,dictRotation)
redim preserve arrParameter(3)
for r = 0 to nRotation-1
    aRotationID = arrRotation(r,dictRotation.Item("RotationID"))
    arrParameter(2)=aRotationID
'    Response.Write("      <th align=""left"">"&arrRotation(r,dictRotation.Item("RotationName"))&"<br/></th>" & vbCr)
'    Response.Write("    <tr>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCARRYOVER")&"</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAfterCrop")&"</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strCROP")&"</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Str.Item("strAfterCrop")&"</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;N-LES<br />&nbsp;&nbsp;Kg.N/Ha</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;N-LES<br />&nbsp;&nbsp;mg NO<sub>3</sub>/l</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;Afstrømning<br />&nbsp;&nbsp;mm</th>" & vbCr)
'    Response.Write("      <th align=""left"">&nbsp;&nbsp;Areal<br />&nbsp;&nbsp;Ha</th>" & vbCr)
'    Response.Write("    </tr>" & vbCr)
		set dictFieldPlanRotation = CreateObject("Scripting.Dictionary")
		nFieldPlanRotation = CallSP("GetFieldPlanRotationList "&CLng(Session("farmNumber"))&","&CInt(Session("scenarioNumber"))&","&aRotationID,arrFieldPlanRotation,dictFieldPlanRotation)
		for f=0 to nFieldPlanRotation-1
			aSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("SecondCropID"))
			aPreCropSecondCrop = arrFieldPlanRotation(f,dictFieldPlanRotation("PreCropSecondCrop"))
			aFieldPlanRotation = arrFieldPlanRotation(f,dictFieldPlanRotation.Item("FieldPlanRotationID"))
			arrParameter(3)=aFieldPlanRotation
			NLes=CallSPReturn("CalcN_LesPrFieldPlanRotation ",arrParameter)
			Area=Cdbl(arrFieldPlanRotation(f,dictFieldPlanRotation.Item("Area")))
			TotalNLES_N=TotalNLES_N+(NLes*Area)
			RunOff=CallSPReturn("GetRunOff ",arrParameter)
			totalRunOff=totalRunOff+(RunOff*Area)
			if RunOff<>0 then 
			  NLESmg=100*NLes*4.43/RunOff'NLES (KgN/Ha)-> 1000000*NLES (mg/Ha). RunOff (liter/squareMeter)-> 10000*RunOff (liter/Ha)
			else
			  NLESmg=100*NLes*4.43/1
			end if
'			Response.Write("    <tr>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("PreCropOriginalName")))
'			if CInt(aPreCropSecondCrop)>CInt(0) And  Not(CInt(aPreCropSecondCrop)=CInt(15)) And Not(CInt(aPreCropSecondCrop)=CInt(21)) then
'			  Response.write(" "&Str.Item("strUnderSown"))
'			end if
'			Response.Write("</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("PreCrop_AfterCrop_Name"))&"</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("CropName")))
'			if CInt(aSecondCrop)>CInt(0) And  Not(CInt(aSecondCrop)=CInt(15)) And Not(CInt(aSecondCrop)=CInt(21)) then
'			  Response.write(" "&Str.Item("strUnderSown"))
'			end if
'			Response.Write("</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&arrFieldPlanRotation(f,dictFieldPlanRotation.item("Crop_AfterCrop_Name"))&"</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(NLes,2)&"</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(NLesmg,2)&"</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Round(RunOff,2)&"</td>" & vbCr)
'			Response.Write("      <td align=""left"">&nbsp;&nbsp;"&Area&"</td>" & vbCr)
'			Response.Write("    </tr>" & vbCr)
	  next
next
     meanNLES_N = TotalNLES_N/totalArea
	 meanRunOff = TotalRunOff/totalArea
	 if meanRunOff = 0 then meanRunOff = 1
	 meanNLES_mg = 100*meanNLES_N*4.43/meanRunOff
'   Response.Write("    </tr>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"" colspan=""2"">&nbsp;&nbsp;"&Str.Item("strMeanOnFarm")&"</th>" & vbCr)
''   Response.Write("      <th align=""left"">&nbsp;&nbsp;</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(meanNLES_N,2)&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(meanNLES_mg,2)&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;&nbsp;"&Round(meanRunOff,2)&"</th>" & vbCr)
'   Response.Write("      <th align=""left"">&nbsp;</th>" & vbCr)
'   Response.Write("    </tr>" & vbCr)
'   Response.Write("  </table>" & vbCr)
end function
'**************************************************************************************************

  Sub PrintArrayInTable2Nina(anArray)
    Dim r,c
  Response.Write "<br><table border>" & Chr(13)
    For r = 0 to UBound(anArray,2)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(anArray,1)
        Response.Write "<td> &nbsp;" & CStr(anArray(c,r)) & " </td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)
    Next
    Response.Write "</table>" & Chr(13)
  End Sub

'**************************************************************************************************
   Sub PrintArrayInTableNina(anArray)
    Dim r,c
  Response.Write "<br><table border>" & Chr(13)
      Response.Write "<tr>" & Chr(13)
      For c = 0 to UBound(anArray)
        Response.Write "<td> &nbsp; " & CStr(anArray(c)) & "</td>" & Chr(13)
      Next
      Response.Write "</tr>" & Chr(13)
    Response.Write "</table>" & Chr(13)
  End Sub


'**************************************************************************************************

'  function Sum(anArray,k)
'    Dim r, total

'    For r = 0 to UBound(anArray,2)
'      total = total + anArray(k,r)
'    Next
'    Sum = total
'  End function

'**************************************************************************************************

  function SumDict(anArray,anDict,str)
    Dim r, total

    For r = 0 to UBound(anArray)
      total = total + CDbl(anArray(r,anDict.Item(""&str&"")))
    Next
    SumDict = total
  End function
'**************************************************************************************************

  function SumDict2(anArray,anDict,str)
    Dim r, total

    For r = 0 to UBound(anArray,2)
      total = total + CDbl(anArray(anDict.Item(""&str&""),r))
    Next
    SumDict2 = total
  End function

%>