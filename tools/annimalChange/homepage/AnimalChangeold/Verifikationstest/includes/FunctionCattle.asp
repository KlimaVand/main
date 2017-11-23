<%
'**************************************************************************************************
' Author: Poul Lassen, Danish Institute of Agricultural Sciences
' Description: 
'**************************************************************************************************
function CalcCattleFeedEnergy(intCattleRace, intCattleAgeGroup, fltProductionLevel, fltProductionLevel2)
   dim returnValue
   
   select case CInt(intCattleRace)
      case 1 'Tunge racer
         select case CInt(intCattleAgeGroup)
            case 1 'Malkekøer
               returnValue = 1635 + 0.4*fltProductionLevel + 0.0000167*fltProductionLevel^2
            case 2 'Opdræt
               returnValue = (1683 + 2.19*(fltProductionLevel-700))/0.8905
            case 3 'Ungtyre
               returnValue = 1971 + (1100 - fltProductionLevel) * 1.09
            case else
               returnValue = 0
         end select   
      case 2 'Jersey
         select case CInt(intCattleAgeGroup)
            case 1 'Malkekøer
               returnValue = 1192 + 0.4*fltProductionLevel + 0.0000167*fltProductionLevel^2
            case 2 'Opdræt
               returnValue = (1236 + 1.83*(fltProductionLevel-500))/0.9155
            case 3 'Ungtyre
               returnValue = 1266 + (900 - fltProductionLevel) * 0.55
            case else
               returnValue = 0
         end select   
      Case 3 'Ammekøer
           returnValue = ((fltProductionLevel/200+1.5)*1.1)*365+0.4*((fltProductionLevel2-279)*9+1243)+101+88

	  case else
         returnValue = 0
   end select
'   Response.Write("<h3>returnValue: "&returnValue&"</h3>")
   CalcCattleFeedEnergy = returnValue
end function
'**************************************************************************************************
function CalcCattleFeedDryMatter(intCattleRace, intCattleAgeGroup, fltProductionLevel, fltProductionLevel2, fltEnergy)
   dim returnValue
   
   select case CInt(intCattleRace)
      case 1 'Tunge racer
         select case CInt(intCattleAgeGroup)
            case 1 'Malkekøer
               returnValue = fltEnergy*(1.10 + (8500-fltProductionLevel)*0.00002)
            case 2 'Opdræt
               returnValue = fltEnergy*1.35 + (fltProductionLevel-700)*0.00025
            case 3 'Ungtyre
               returnValue = fltEnergy*1.05
            case else
               returnValue = 0
         end select   
      case 2 'Jersey
         select case CInt(intCattleAgeGroup)
            case 1 'Malkekøer
               returnValue = fltEnergy*(1.10 + (7500-fltProductionLevel)*0.00002)
            case 2 'Opdræt
               returnValue = fltEnergy*1.35 + (fltProductionLevel-500)*0.00025
            case 3 'Ungtyre
               returnValue = fltEnergy*1.05
            case else
               returnValue = 0
         end select   
      Case 3 'Ammekøer
               returnValue = fltEnergy*1.5
	  case else
         returnValue = 0
   end select
   CalcCattleFeedDryMatter = returnValue
end function
'**************************************************************************************************
function CalcCattleFeedNitrogen(intCattleRace, intCattleAgeGroup, fltEnergy, fltGrazing)
   dim returnValue
   
   select case CInt(intCattleRace)
      case 1 'Tunge racer
         select case CInt(intCattleAgeGroup)
            case 1 'Malkekøer
               returnValue = fltEnergy*0.028
            case 2 'Opdræt
               returnValue = fltEnergy*0.025 + 10*(fltGrazing/fltEnergy)
            case 3 'Ungtyre
               returnValue = fltEnergy*0.022
            case else
               returnValue = 0
         end select   
      case 2 'Jersey
         select case CInt(intCattleAgeGroup)
            case 1 'Malkekøer
               returnValue = fltEnergy*0.028
            case 2 'Opdræt
               returnValue = fltEnergy*0.025 + 10*(fltGrazing/fltEnergy)
            case 3 'Ungtyre
               returnValue = fltEnergy*0.022
            case else
               returnValue = 0
         end select   
      Case 3 'Ammekøer
               returnValue = fltEnergy*0.031
      case else
         returnValue = 0
   end select
   CalcCattleFeedNitrogen = returnValue
end function
'**************************************************************************************************
function CalcCattleProductionMilkNitrogen(intCattleRace, intCattleAgeGroup, fltProductionLevel, fltProductionLevel2)
   dim returnValue
   
   select case CInt(intCattleRace)
      case 1 'Tunge racer
         select case CInt(intCattleAgeGroup)
            case 1 'Malkekøer
               returnValue = fltProductionLevel*0.00518
            case else
               returnValue = 0
         end select   
      case 2 'Jersey
         select case CInt(intCattleAgeGroup)
            case 1 'Malkekøer
               returnValue = fltProductionLevel*0.00488
            case else
               returnValue = 0
         end select   
      Case 3 'Ammekøer
               returnValue = ((fltProductionLevel2-279)*9+1243)*0.00518
      case else
         returnValue = 0
   end select
   CalcCattleProductionMilkNitrogen = returnValue
end function
'**************************************************************************************************
function CalcCattleProductionGrowthNitrogen(intCattleRace, intCattleAgeGroup, fltProductionLevel, fltProductionLevel2)
   dim returnValue
   
   select case CInt(intCattleRace)
      case 1 'Tunge racer
         select case CInt(intCattleAgeGroup)
            case 1 'Malkekøer
               returnValue = 2
            case 2 'Opdræt
               returnValue = fltProductionLevel*365*0.000025
            case 3 'Ungtyre
               returnValue = fltProductionLevel*365*0.000025
            case else
               returnValue = 0
         end select   
      case 2 'Jersey
         select case CInt(intCattleAgeGroup)
            case 1 'Malkekøer
               returnValue = 1
            case 2 'Opdræt
               returnValue = fltProductionLevel*365*0.000025
            case 3 'Ungtyre
               returnValue = fltProductionLevel*365*0.000025
            case else
               returnValue = 0
         end select   
      Case 3 'Ammekøer
               returnValue = 2
     case else
         returnValue = 0
   end select
   CalcCattleProductionGrowthNitrogen = returnValue
end function
'**************************************************************************************************
function CalcCattleManureNitrogen(fltFeedNitrogen, fltMilkNitrogen, fltGrowthNitrogen)
   dim returnValue
   returnValue = fltFeedNitrogen - fltMilkNitrogen - fltGrowthNitrogen
   CalcCattleManureNitrogen = returnValue
end function
'**************************************************************************************************
function CalcCattleManureNitrogenOnField(fltFeedEnergy, fltFeedEnergyGrazing, fltManureNitrogen)
   dim returnValue
   returnValue = fltFeedEnergyGrazing/fltFeedEnergy*fltManureNitrogen
   CalcCattleManureNitrogenOnField = returnValue
end function
'**************************************************************************************************
function CalcCattleManureNitrogenInFaeces(intCattleType, fltFeedNitrogen, fltFeedDryMatter, fltFeedEnergy, fltFeedEnergyGrazing, fltManureNitrogen)
   dim returnValue
      select case CInt(intCattleType)
      case 7 'Ammekøer
          returnValue = fltManureNitrogen/fltFeedEnergy *(0.36 * fltFeedEnergy - 0.12 * fltFeedEnergyGrazing)
	  Case Else
          returnValue = ((0.04*fltFeedNitrogen*1000/365) + (((fltFeedDryMatter/365)^2*1.8)/6.25) + (((fltFeedDryMatter/365)*20)/6.25))*365/1000 
	  End select
   CalcCattleManureNitrogenInFaeces = returnValue
end function
'**************************************************************************************************
sub CalcCattleHerdTotal()
   dim currentRace, currentAgeGroup, currentType, currentAnimalYear, currentGrazing, currentProductionLevel, currentProductionLevel2, currentEffectivity, currentStable
   dim h, dictCattleHerd
	 
'   nCattleHerd = GetCattleHerdList(aFarmNumber,aScenarioID,arrCattleHerd)
   set dictCattleHerd = CreateObject("Scripting.Dictionary")
   nCattleHerd = CallSP ("GetCattleHerdList " & Session("farmNumber")& "," & Session("scenarioNumber") , arrCattleHerd, dictCattleHerd)

'   Dim arrParameter
   redim arrParameter(1)

	 arrParameter(0) = Session("farmNumber")
	 arrParameter(1) = Session("scenarioNumber")
     returnValue = CallSPReturn ("DeleteCattleHerdTotal",arrParameter)
'     Response.write "<br> returnValue " & returnValue
'   DeleteCattleHerdTotal aFarmNumber,aScenarioID   

   redim preserve arrParameter(2)
	 arrParameter(2) = 3
     returnValue = CallSPReturn ("DeleteStable",arrParameter)
     returnValue = CallSPReturn ("DeleteStorage",arrParameter)
     returnValue = CallSPReturn ("DeleteStorageNorm",arrParameter)
 '    Response.write "<br> returnValue " & returnValue

'   DeleteStable          aFarmNumber,aScenarioID,3
'   DeleteStorage         aFarmNumber,aScenarioID,3
'   DeleteStorageNorm aFarmNumber,aScenarioID,3

   for h = 0 to nCattleHerd-1
      currentRace            = arrCattleHerd(h,dictCattleHerd.item("CattleRace"))
      currentAgeGroup        = arrCattleHerd(h,dictCattleHerd.item("CattleAgeGroup"))
      currentType            = arrCattleHerd(h,dictCattleHerd.item("CattleType"))
      currentAnimalYear      = arrCattleHerd(h,dictCattleHerd.item("AnimalYear"))
      currentGrazing         = arrCattleHerd(h,dictCattleHerd.item("Grazing"))
      currentProductionLevel = arrCattleHerd(h,dictCattleHerd.item("ProductionLevel"))
      currentProductionLevel2= arrCattleHerd(h,dictCattleHerd.item("ProductionLevel2"))
      currentEffectivity     = arrCattleHerd(h,dictCattleHerd.item("Effectivity"))
      currentStable          = arrCattleHerd(h,dictCattleHerd.item("Stable"))
'Response.write "<br> current... " & currentRace & " - " & currentAgeGroup & " - " & currentType & " - " & currentAnimalYear & " - " & currentGrazing & " - " & currentProductionLevel & " - " & currentProductionLevel2 & " - " & currentEffectivity & " - " & currentStable

      CalcFeedEnergyGrazing currentType,currentAnimalYear,currentGrazing
      CalcFeedEnergy currentRace,currentAgeGroup,currentType,currentAnimalYear,currentProductionLevel,currentProductionLevel2,currentEffectivity
      CalcFeedDryMatter currentRace,currentAgeGroup,currentType,currentAnimalYear,currentProductionLevel,currentProductionLevel2 
      CalcFeedNitrogen currentRace,currentAgeGroup,currentType,currentAnimalYear,currentGrazing,currentEffectivity
      CalcFeedPhosphorus currentType
      CalcProductionMilkNitrogen currentRace,currentAgeGroup,currentType,currentAnimalYear,currentProductionLevel,currentProductionLevel2
      CalcProductionGrowthNitrogen currentRace,currentAgeGroup,currentType,currentAnimalYear,currentProductionLevel,currentProductionLevel2
      CalcManureNitrogen currentType,currentAnimalYear
      CalcManureNitrogenOnField currentType,currentAnimalYear,currentGrazing
      CalcManureNitrogenFaecesUrine currentType,currentAnimalYear,currentGrazing
      
      CalcManureStableStorage currentType, currentAnimalYear, currentStable, currentGrazing
   Next
  set dictCattleHerd = Nothing
  
end sub
'**************************************************************************************************
sub CalcFeedEnergyGrazing(cattleType, animalYear, grazing)
   dim feedEnergyGrazing
   
   feedEnergyGrazing = animalYear*grazing
'   Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0)= aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = cattleType
	 arrParameter(3) = PARAMETER_FE_GRAZING
	 arrParameter(4) = CSng(feedEnergyGrazing)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'   IncrementCattleHerdTotal aFarmNumber, aScenarioID, cattleType, PARAMETER_FE_GRAZING, CSng(feedEnergyGrazing)
end sub
'**************************************************************************************************
sub CalcFeedEnergy(cattleRace, cattleAgeGroup, cattleType, animalYear, productionLevel,productionLevel2, effectivity)

   feedEnergyAnimalYear = CalcCattleFeedEnergy(cattleRace, cattleAgeGroup, productionLevel,productionLevel2)
 '  feedEnergyTotalNorm = animalYear*feedEnergyAnimalYear
'   feedEnergyTotal = animalYear*feedEnergyAnimalYear*100/effectivity
   feedEnergyTotal = animalYear*feedEnergyAnimalYear
'   Response.write "<br> feedEnergyTotal: " & feedEnergyTotal
 '  Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = cattleType
	 arrParameter(3) = PARAMETER_FE
	 arrParameter(4) = CSng(feedEnergyTotal)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'   IncrementCattleHerdTotal aFarmNumber, aScenarioID, cattleType, PARAMETER_FE, CSng(feedEnergyTotal)
end sub
'**************************************************************************************************
sub CalcFeedDryMatter(cattleRace, cattleAgeGroup, cattleType, animalYear, productionLevel,productionLevel2)
   feedDryMatterAnimalYear = CalcCattleFeedDryMatter(cattleRace, cattleAgeGroup, productionLevel, productionLevel2, feedEnergyAnimalYear)
   feedDryMatterTotal = animalYear*feedDryMatterAnimalYear
 '  Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = cattleType
	 arrParameter(3) = PARAMETER_TS
	 arrParameter(4) = CSng(feedDryMatterTotal)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'   IncrementCattleHerdTotal aFarmNumber, aScenarioID, cattleType, PARAMETER_TS, CSng(feedDryMatterTotal)
end sub
'**************************************************************************************************
sub CalcFeedNitrogen(cattleRace, cattleAgeGroup, cattleType, animalYear, grazing, effectivity)
   feedNitrogenAnimalYear = CalcCattleFeedNitrogen(cattleRace, cattleAgeGroup, feedEnergyAnimalYear, grazing)*100/effectivity
   feedNitrogenAnimalYearNorm = CalcCattleFeedNitrogen(cattleRace, cattleAgeGroup, feedEnergyAnimalYear, 0)
   feedNitrogenTotal = animalYear*feedNitrogenAnimalYear
 '  Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = cattleType
	 arrParameter(3) = PARAMETER_N
	 arrParameter(4) = CSng(feedNitrogenTotal)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'   IncrementCattleHerdTotal aFarmNumber, aScenarioID, cattleType, PARAMETER_N, CSng(feedNitrogenTotal)
end sub
'**************************************************************************************************
sub CalcFeedPhosphorus(cattleType)
 ' Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = cattleType
	 arrParameter(3) = PARAMETER_P
	 arrParameter(4) = 0.0
     returnValue = CallSPReturn ("InsertUpdateCattleHerdTotal",arrParameter)
'   InsertUpdateCattleHerdTotal aFarmNumber, aScenarioID,cattleType, PARAMETER_P, 0.0
end sub
'**************************************************************************************************
sub CalcProductionMilkNitrogen(cattleRace, cattleAgeGroup, cattleType, animalYear, productionLevel, productionLevel2)
   productionMilkNitrogenAnimalYear = CalcCattleProductionMilkNitrogen(cattleRace, cattleAgeGroup, productionLevel, productionLevel2)
   productionMilkNitrogenTotal = animalYear*productionMilkNitrogenAnimalYear
 '  Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = cattleType
	 arrParameter(3) = PARAMETER_MILK_N
	 arrParameter(4) = CSng(productionMilkNitrogenTotal)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'   IncrementCattleHerdTotal aFarmNumber, aScenarioID, cattleType, PARAMETER_MILK_N, CSng(productionMilkNitrogenTotal)
end sub
'**************************************************************************************************
sub CalcProductionGrowthNitrogen(cattleRace, cattleAgeGroup, cattleType, animalYear, productionLevel,productionLevel2)
   productionGrowthNitrogenAnimalYear = CalcCattleProductionGrowthNitrogen(cattleRace, cattleAgeGroup, productionLevel,productionLevel2)
   productionGrowthNitrogenTotal = animalYear*productionGrowthNitrogenAnimalYear
 '  Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = cattleType
	 arrParameter(3) = PARAMETER_GROWTH_N
	 arrParameter(4) = CSng(productionGrowthNitrogenTotal)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'   IncrementCattleHerdTotal aFarmNumber, aScenarioID, cattleType, PARAMETER_GROWTH_N, CSng(productionGrowthNitrogenTotal)
end sub
'**************************************************************************************************
sub CalcManureNitrogen(cattleType, animalYear)
   manureNitrogenAnimalYear = CalcCattleManureNitrogen(feedNitrogenAnimalYear, productionMilkNitrogenAnimalYear, productionGrowthNitrogenAnimalYear)
   manureNitrogenAnimalYearNorm = CalcCattleManureNitrogen(feedNitrogenAnimalYearNorm, productionMilkNitrogenAnimalYear, productionGrowthNitrogenAnimalYear)
   manureNitrogenTotal = animalYear*manureNitrogenAnimalYear
   manureNitrogenTotalNorm = animalYear*manureNitrogenAnimalYearNorm
'   Response.write "<br> manureNitrogenTotalNorm: " & manureNitrogenTotalNorm
'   Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = cattleType
	 arrParameter(3) = PARAMETER_MANURE_N
	 arrParameter(4) = CSng(manureNitrogenTotal)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'   IncrementCattleHerdTotal aFarmNumber, aScenarioID, cattleType, PARAMETER_MANURE_N, CSng(manureNitrogenTotal)
end sub
'**************************************************************************************************
sub CalcManureNitrogenOnField(cattleType, animalYear, grazing)
   manureNitrogenOnFieldAnimalYear = CalcCattleManureNitrogenOnField(feedEnergyAnimalYear, grazing, manureNitrogenAnimalYear)
   manureNitrogenOnFieldTotal = animalYear*manureNitrogenOnFieldAnimalYear
'   Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = cattleType
	 arrParameter(3) = PARAMETER_MANURE_N_FIELD
	 arrParameter(4) = CSng(manureNitrogenOnFieldTotal)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'   IncrementCattleHerdTotal aFarmNumber, aScenarioID, cattleType, PARAMETER_MANURE_N_FIELD, CSng(manureNitrogenOnFieldTotal)
end sub
'**************************************************************************************************
sub CalcManureNitrogenFaecesUrine(cattleType, animalYear, grazing)
   manureNitrogenFaecesAnimalYear = CalcCattleManureNitrogenInFaeces(cattleType, feedNitrogenAnimalYear, feedDryMatterAnimalYear, feedEnergyAnimalYear, grazing, manureNitrogenAnimalYear)

   manureNitrogenFaecesTotal = animalYear*manureNitrogenFaecesAnimalYear
 '  Response.write "<br> manureNitrogenFaecesTotal: " & manureNitrogenFaecesTotal

'   Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = cattleType
	 arrParameter(3) = PARAMETER_MANURE_N_FAECES
	 arrParameter(4) = CSng(manureNitrogenFaecesTotal)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'   IncrementCattleHerdTotal aFarmNumber, aScenarioID, cattleType, PARAMETER_MANURE_N_FAECES, CSng(manureNitrogenFaecesTotal)

   manureNitrogenUrineAnimalYear = manureNitrogenAnimalYear - manureNitrogenFaecesAnimalYear
   manureNitrogenUrineTotal = animalYear*manureNitrogenUrineAnimalYear
'   Response.write "<br> manureNitrogenUrineTotal: " & manureNitrogenUrineTotal
	 arrParameter(3) = PARAMETER_MANURE_N_URINE
	 arrParameter(4) = CSng(manureNitrogenUrineTotal)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'   IncrementCattleHerdTotal aFarmNumber, aScenarioID, cattleType, PARAMETER_MANURE_N_URINE, CSng(manureNitrogenUrineTotal)
end sub
'**************************************************************************************************
sub CalcManureStableStorage(aCattleTypeID, animalYear, aStableID, aGrazing)
   dim nStable, arrStable, dictStable
   dim stableNitrogen, stableNitrogenLoss, stableNitrogenAfterLoss
   dim storageNitrogen, storageNitrogenLossAmmonia, storageNitrogenLossDenitrification, storageNitrogenAfterLoss, stableBeddingNitrogen, stableNitrogenNormAfterLoss, storageNitrogenNormAfterLoss, stableNitrogenNorm, stableBeddingNitrogenNorm
   dim s, GrazingPart
 '  Response.write "<br> aCattleTypeID,aStableID " & aCattleTypeID &" " &aStableID
   
   set dictStable = CreateObject("Scripting.Dictionary")

   nStable = CallSP ("GetCattleStableList "&aCattleTypeID & ", " & aStableID, arrStable, dictStable)
'   nStable = GetCattleStableList(aCattleTypeID,aStableID,arrStable)
   for s = 0 to nStable-1
      GrazingPart = aGrazing/feedEnergyAnimalYear
    '  Response.write "<br> GrazingPart: " & GrazingPart


      stableBeddingNitrogen = arrStable(s,4)*animalYear*0.85*0.005*(1-GrazingPart)
      stableBeddingNitrogenNorm = arrStable(s,4)*animalYear*0.85*0.005
      if isnull(stableBeddingNitrogen) then 
      stableBeddingNitrogen = 0
      end if
      if isnull(stableBeddingNitrogenNorm) then 
      stableBeddingNitrogenNorm = 0
      end if
    '  Response.write "<br> Strøelse: " & stableBeddingNitrogen
     ' Response.write "<br> StrøelseNorm: " & stableBeddingNitrogenNorm

'   Dim arrParameter
   redim preserve arrParameter(4)

'	 arrParameter(0) = aFarmNumber
'	 arrParameter(1) = aScenarioID
	 arrParameter(2) = aCattleTypeID
	 arrParameter(3) = PARAMETER_BEDDING_N
	 arrParameter(4) = CSng(stableBeddingNitrogen)
     returnValue = CallSPReturn ("IncrementCattleHerdTotal",arrParameter)
'      IncrementCattleHerdTotal aFarmNumber, aScenarioID, aCattleTypeID, PARAMETER_BEDDING_N, CSng(stableBeddingNitrogen)

'      Response.write "<br> manureNitrogenTotal : " & manureNitrogenTotal & " manureNitrogenOnFieldTotal " & manureNitrogenOnFieldTotal

      stableNitrogen = (manureNitrogenTotal - manureNitrogenOnFieldTotal)*arrStable(s,3)/100 + stableBeddingNitrogen
      stableNitrogenNorm = (manureNitrogenTotalNorm)*arrStable(s,3)/100 + stableBeddingNitrogenNorm
'      Response.write "<br> stableNitrogen i storageID: " & arrStable(s,1) & " " & stableNitrogen

      stableNitrogenLoss = stableNitrogen*arrStable(s,7)/100

      stableNitrogenAfterLoss = stableNitrogen - stableNitrogenLoss
      stableNitrogenNormAfterLoss = stableNitrogenNorm*(100-arrStable(s,7))/100

      redim preserve arrParameter(7)
 	 arrParameter(2) = 3
	 arrParameter(3) = aStableID
	 arrParameter(4) = 0
	 arrParameter(5) = 0
	 arrParameter(6) = stableNitrogenLoss
	 arrParameter(7) = stableNitrogenAfterLoss
     returnValue = CallSPReturn ("IncrementStable",arrParameter)

'      IncrementStable  aFarmNumber,aScenarioID,3,aStableID,0,0,stableNitrogenLoss,stableNitrogenAfterLoss
      
      storageNitrogen = stableNitrogenAfterLoss
      storageNitrogenLossAmmonia = storageNitrogen*arrStable(s,9)/100
    '  Response.write "<br> storageNitrogenLossAmmonia i storageID: " & arrStable(s,1) & " " & storageNitrogenLossAmmonia
      storageNitrogenLossDenitrification = storageNitrogen*arrStable(s,10)/100
     ' Response.write "<br> storageNitrogenLossDenitrification i storageID: " & arrStable(s,1) & " " & storageNitrogenLossDenitrification
      storageNitrogenAfterLoss = storageNitrogen - storageNitrogenLossAmmonia - storageNitrogenLossDenitrification
     ' Response.write "<br> storageNitrogenAfterLoss i storageID: " & arrStable(s,1) & " " & storageNitrogenAfterLoss
      storageNitrogenNormAfterLoss = stableNitrogenNormAfterLoss * (100- arrStable(s,9) - arrStable(s,10))/100

      redim preserve arrParameter(14)
	 arrParameter(3) = arrStable(s,1)
	 arrParameter(4) = stableNitrogen
	 arrParameter(5) = stableNitrogenLoss
	 arrParameter(6) = stableNitrogenAfterLoss
	 arrParameter(7) = storageNitrogenLossAmmonia
	 arrParameter(8) = storageNitrogenLossDenitrification
	 arrParameter(9) = storageNitrogenAfterLoss
	 arrParameter(10) = 0
	 arrParameter(11) = 0
	 arrParameter(12) = 0
	 arrParameter(13) = 0
	 arrParameter(14) = 0  


     returnValue = CallSPReturn ("IncrementStorage",arrParameter)
'      IncrementStorage aFarmNumber, aScenarioID, 3, arrStable(s,1), stableNitrogen, stableNitrogenLoss, stableNitrogenAfterLoss, storageNitrogenLossAmmonia, storageNitrogenLossDenitrification, storageNitrogenAfterLoss, 0,0,0,0,0  

	  redim preserve arrParameter(4)
	 arrParameter(3) = arrStable(s,1)
	 arrParameter(4) = storageNitrogenNormAfterLoss
     returnValue = CallSPReturn ("IncrementStorageNorm",arrParameter)

'      IncrementStorageNorm aFarmNumber,aScenarioID,3, arrStable(s,1), storageNitrogenNormAfterLoss
   next
end sub
'**************************************************************************************************
%>