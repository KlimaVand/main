using System;
using System.Collections.Generic;
using System.Xml;
class farmBalanceClass
{
    //Jonas put these under Herd
    //import of N in animal feed
    double liveFeedImportN = 0;
    //N in grazed feed
    double liveGrazedN = 0;
    //input of excretal N to housing
    double liveToHousingN = 0;
    //deposition of N by grazing livestock
    double liveToFieldN = 0;

    //Jonas put these under Housing
    //excretal N deposited in housing
    double houseInFromAnimalsN = 0;
    //Gaseous loss of N from housing
    double houseLossN = 0;
    //N from housing to storage
    double houseToStorageN = 0;

    //Jonas put these under ManureStorage
    //N input to storage from excreta deposited in housing (minus NH3 emission)
    double storageFromHouseN = 0;
    //N input to storage in bedding
    double storageFromBeddingN = 0;
    //N input to storage in wasted feed
    double storageFromFeedWasteN = 0;
    //N lost in gaseous emissions from storage
    double storageGaseousLossN = 0;
    //N lost in runoff from storage
    double storageRunoffN = 0;

    //N in imported manure
    double manureImportN = 0;
    //N in exported manure
    double manureExportN = 0;

    //Jonas put these in Fields
    //N in manure applied to fields
    double manureToFieldN = 0;
    //N in gaseous emissions in the field
    double fieldGaseousLossN = 0;
    //N in nitrate leaching
    double fieldNitrateLeachedN = 0;
    //N removed by grazing animals
    double grazedN = 0;
    //Change in soil N storage
    double changeSoilN = 0;
    //N in plant material harvested for consumption by livestocl
    double fieldharvestedConsumedN = 0;
    double entericCH4CO2Eq = 0.0;
    double manureCH4CO2Eq = 0;
    double manureN2OCO2Eq = 0;
    double fieldN2OCO2Eq = 0;
    double directGHGEmissionCO2Eq = 0;
    double soilCO2Eq = 0;
    double housingNH3CO2Eq = 0;
    double manurestoreNH3CO2Eq = 0;
    double fieldmanureNH3CO2Eq = 0;
    double fieldfertNH3CO2Eq =0;
    double leachedNCO2Eq = 0;
    double indirectGHGCO2Eq = 0;
    //!carbon fixation by crops (kg)
    double carbonFromPlants = 0;
    //! carbon imported in livestock manure (kg)
    double Cmanimp = 0;
    //!carbon imported in animal feed (kg)
    double CPlantProductImported = 0;
    //!carbon in imported bedding (kg)
    double CbeddingReq = 0;
    //!Carbon exported in milk (kg)
    double Cmilk = 0;
    //!Carbon exported in meat (kg)
    double Cmeat = 0;
    //!Carbon exported in dead animals (kg)
    double Cmortalities = 0;
    //Carbon exported in manure  (kg)
    double Cmanexp = 0;
    //!Carbon ín sold crop products (kg)
    double Csold = 0;
    //!total carbon loss to environment
    double CLost = 0;
    //!Carbon lost as methane from livestock
    double livestockCH4C = 0;
    //!Carbon lost as carbon dioxide from livestock
    double livestockCO2C = 0;
    double livstockCLoss = 0;
    //!Carbon lost as carbon dioxide from urea hydrolysis
    double housingCLoss = 0;
    //!Carbon lost as methane from manure storage
    double manurestoreCH4C = 0;
    //!Carbon lost as carbon dioxide from manure storage
    double manurestoreCO2C = 0;
    //!change in soil C
    double CDeltaSoil = 0;
    //!emissions of CO2 from soil
    double soilCO2_CEmission = 0;
    //!C lost from stored plant products
    double processStorageCloss = 0;
    //!C in organic matter leached from soil
    double soilCleached = 0;
    //!CO-C from burnt crop residues
    double burntResidueCOC = 0;
    //!Black C from burnt crop residues
    double burntResidueBlackC = 0;
    //!CO2-C from burnt crop residues
    double burntResidueCO2C = 0;
    //!C in CH4 from biogas reactor
    double biogasCH4C = 0;
    //!C in CO2 from biogas reactor
    double biogasCO2C = 0;
    //!continuity check for C
    double Cbalance=0;

    //! N input via N fixation in crops (kg)
    double nFix = 0;
    //!N lost from stored plant products
    double processStorageNloss = 0;
    //! N input via atmospheric deposition
    double Natm = 0;
    //!N input in N fertilisers (kg)
    double nFert = 0;//1.134 where are the import of fertiliser??//1.134
    //!N input in imported bedding (kg)
    double Nbedding = 0;
    //! carbon imported in livestock manure (kg)
    double Nmanimp = 0;
    //! N imported in animal feed (kg)
    double NPlantProductImported = 0;
    //! N sold in crop products
    double Nsold = 0;
    //!N exported in milk
    double Nmilk = 0;
    //! N exported in animal growth
    double NGrowth = 0;
    //! N exported in animal mortalities
    double Nmortalities = 0;
    //!N exported in animal manure
    double Nmanexp = 0;
    //total N export
    double NExport = 0;
    // N losses and change in N stored in soil
    double NDeltaSoil = 0;
    //!total N lost
    double NLost = 0;
    //!N lost as NH3 from housing
    double housingNH3Loss = 0;
    //!N2O-N emission from stored manure
    double manureN2Emission = 0;
    //!N2-N emission from stored manure
    double manureN2OEmission = 0;
    //!NH3-N emission from stored manure
    double manureNH3Emission = 0;
    double manurestoreNLoss = 0;
    double fieldNLoss = 0;
    //N2-N emission from soil
    double fieldN2Emission = 0;
    //N2O-N emission from soil
    double fieldN2OEmission = 0;
    //NH3-N-N emission from fertiliser
    double fertNH3NEmission = 0;
    //NH3-N emission from field-applied manure
    double fieldmanureNH3Emission = 0;
    //NH3-N emission from urine deposited in the field
    double fieldUrineNH3Emission = 0;
    //NO3-N leaching from soil
    double Nleaching = 0;
    //N excreted in housing
    double NexcretedHousing = 0;
    //N excreted during grazing
    double NexcretedField = 0;
    //N fed in housing
    double NfedInHousing = 0;
    //N from grazed
    double NinGrazedFeed = 0;
    //!Change in mineral N in soil
    double changeInMinN;
    //!nitrous oxide emission from fertiliser
    double fertiliserN2OEmission = 0;
   //!nitrous oxide emissions from crop residues
    double cropResidueN2O = 0;
    //!leaching of organic nitrogen
    double organicNLeached = 0;
    //!N2O-N in gases from burnt crop residues
    double burntResidueN2ON = 0;
    //!NH3N in gases from burnt crop residues
    double burntResidueNH3N = 0;
    //!NOX in gases from burnt crop residues
    double burntResidueNOxN = 0;
    //!N in other gases from burnt crop residues
    double burntResidueOtherN = 0;
    //!runoff from manure storage
    double runoffN = 0;
    //!residual soil mineral N at end of crop sequence
    double residualSoilMineralN = 0;
    //!total losses from pocess/storage of crop products, housing and manure storage
    double totalHouseStoreNloss = 0;
    //! total losses from fields
    double totalFieldNlosses = 0;
    //!change in total N storage (organic and inorganic)
    double changeAllSoilNstored = 0;
    //!farm N surplus (kg/ha/yr)
    double totalFarmNSurplus = 0;
    //!continuity check
    double Nbalance = 0;

    //farm milk production
    double farmMilkProduction = 0;
    //farm meat production
    double farmMeatProduction = 0;
    //average milk production per head
    double avgProductionMilkPerHead = 0;

    //!farm area (ha)
    double agriculturalArea = 0;
    string parens;
    public farmBalanceClass(string aParens)
    {
        parens = aParens;
    }

    public double GetAgriculturalArea(List<CropSequenceClass> therotationList)
    {
        double area = 0;
        for (int i = 0; i < therotationList.Count; i++)
        {
            area += therotationList[i].getArea();
        }
        return area;
    }

    public void DoFarmBalances(List<CropSequenceClass> rotationList, List<livestock> listOfLivestock, List<housing> listOfHousing,
        List<manureStore> listOfManurestores)
    {
        //Farm balances
        //C balance
        //C inputs
        GlobalVars.Instance.CalculateTradeBalance();

        int minRotation = 1;
        int maxRotation = rotationList.Count;
        for (int rotationID = minRotation; rotationID <= maxRotation; rotationID++)
        {
            carbonFromPlants += rotationList[rotationID - 1].getCFixed() / rotationList[rotationID - 1].GetlengthOfSequence(); //1.100
        }
        for (int i = 0; i < GlobalVars.Instance.theManureExchange.GetmanuresImported().Count; i++)
        {
            Cmanimp += GlobalVars.Instance.theManureExchange.GetmanuresImported()[i].GetTotalC();
        }
        
        GlobalVars.product compositeProductImported = GlobalVars.Instance.GetPlantProductImports();
        CPlantProductImported = compositeProductImported.composition.Getamount() * compositeProductImported.composition.GetC_conc();
        CbeddingReq = GlobalVars.Instance.GetthebeddingMaterial().Getamount() *
            GlobalVars.Instance.GetthebeddingMaterial().GetC_conc();
        CPlantProductImported += CbeddingReq;

        double cInput = carbonFromPlants + CPlantProductImported + Cmanimp;

        //C outputs
        GlobalVars.product compositeProductExported = GlobalVars.Instance.GetPlantProductExports();
        GlobalVars.Instance.PrintPlantProducts();
        Csold = compositeProductExported.composition.Getamount() * compositeProductExported.composition.GetC_conc();

        for (int i = 0; i < listOfLivestock.Count; i++)
        {
            Cmilk += listOfLivestock[i].GetMilkC() * listOfLivestock[i].GetAvgNumberOfAnimal(); //1.113
            Cmeat += listOfLivestock[i].GetGrowthC() * listOfLivestock[i].GetAvgNumberOfAnimal();//1.114
            Cmortalities += listOfLivestock[i].GetMortalitiesC() * listOfLivestock[i].GetAvgNumberOfAnimal();
        }

        for (int i = 0; i < GlobalVars.Instance.theManureExchange.GetmanuresExported().Count; i++)
        {
            Cmanexp += GlobalVars.Instance.theManureExchange.GetmanuresExported()[i].GetTotalC();
        }
        double Cexport = Csold + Cmilk + Cmeat + Cmanexp + Cmortalities;//1.116
        //variables used for debugging
        double LivestockCconsumption = 0;
        double LivestockUrineC = 0;
        double LivestockFaecalC = 0;
        double LivestockGrowthC = 0;
        double LivestockMilkC = 0;
        double LivestockMortalityC = 0;
        double excretalCtoPasture = 0;
        //end of variables for debugging
        for (int i = 0; i < listOfLivestock.Count; i++)
        {
            livestock anAnimalCategory = listOfLivestock[i];
            anAnimalCategory.CheckLivestockCBalance();
            livestockCH4C += anAnimalCategory.getCH4C() * listOfLivestock[i].GetAvgNumberOfAnimal();
            livestockCO2C += anAnimalCategory.getCO2C() * listOfLivestock[i].GetAvgNumberOfAnimal();
            LivestockCconsumption += anAnimalCategory.GetCintake() * listOfLivestock[i].GetAvgNumberOfAnimal();
            LivestockUrineC += anAnimalCategory.GeturineC() * listOfLivestock[i].GetAvgNumberOfAnimal();
            LivestockFaecalC += anAnimalCategory.GetfaecalC() * listOfLivestock[i].GetAvgNumberOfAnimal();
            LivestockGrowthC += anAnimalCategory.GetGrowthC() * listOfLivestock[i].GetAvgNumberOfAnimal();
            LivestockMilkC += anAnimalCategory.GetMilkC() * listOfLivestock[i].GetAvgNumberOfAnimal();
            LivestockMortalityC += anAnimalCategory.GetMortalitiesC() * listOfLivestock[i].GetAvgNumberOfAnimal();
            excretalCtoPasture+=anAnimalCategory.GetCexcretionToPasture()* listOfLivestock[i].GetAvgNumberOfAnimal();
       }
        livstockCLoss = livestockCH4C + livestockCO2C;
        CLost += livstockCLoss;

        for (int i = 0; i < listOfHousing.Count; i++)
        {
            housing ahouse = listOfHousing[i];
            ahouse.CheckHousingCBalance();
            housingCLoss += ahouse.GetCO2C();
        }
        CLost += housingCLoss;
        biogasCH4C=0;
        biogasCO2C=0;
        double manurestoreCLoss = 0;
        
        for (int i = 0; i < listOfManurestores.Count; i++)
        {
            manureStore amanurestore2 = listOfManurestores[i];
            amanurestore2.CheckManureStoreCBalance();
            manurestoreCO2C += amanurestore2.GetCCO2ST();
            manurestoreCH4C += amanurestore2.GetCCH4ST();
            biogasCH4C += amanurestore2.GetbiogasCH4C();
            biogasCO2C += amanurestore2.GetbiogasCO2C();
        }
        manurestoreCLoss = manurestoreCH4C + manurestoreCO2C;
        Cexport += biogasCO2C + biogasCH4C;
        CLost += manurestoreCLoss;

        soilCO2_CEmission = 0;
        processStorageCloss = 0;
        burntResidueCOC = 0;
        burntResidueCO2C = 0;
        burntResidueBlackC = 0;
        //variables used for debugging
        double cropResidueC = 0;
        double grazedHerbageC = 0;
        double CinputToSoil = 0;
        //end of debug variables
        for (int rotationID = minRotation; rotationID <= maxRotation; rotationID++)
        {
            rotationList[rotationID - 1].CheckRotationCBalance();
            CDeltaSoil += rotationList[rotationID - 1].GetCdeltaSoil() / rotationList[rotationID - 1].GetlengthOfSequence();
            soilCO2_CEmission += rotationList[rotationID - 1].GetsoilCO2_CEmission() / rotationList[rotationID - 1].GetlengthOfSequence();
            soilCleached += rotationList[rotationID - 1].GetCleached() / rotationList[rotationID - 1].GetlengthOfSequence();
            processStorageCloss += rotationList[rotationID - 1].getProcessStorageLossCarbon() / rotationList[rotationID - 1].GetlengthOfSequence();
            burntResidueCOC += rotationList[rotationID - 1].getBurntResidueCOC() / rotationList[rotationID - 1].GetlengthOfSequence();
            burntResidueBlackC += rotationList[rotationID - 1].getBurntResidueBlackC() / rotationList[rotationID - 1].GetlengthOfSequence();
            burntResidueCO2C += rotationList[rotationID - 1].getBurntResidueCO2C() / rotationList[rotationID - 1].GetlengthOfSequence();
            CinputToSoil+=rotationList[rotationID - 1].GetCinputToSoil()/ rotationList[rotationID - 1].GetlengthOfSequence();
            grazedHerbageC += rotationList[rotationID - 1].getGrazedC() / rotationList[rotationID - 1].GetlengthOfSequence();
            cropResidueC += rotationList[rotationID - 1].getCropResidueCarbon() / rotationList[rotationID - 1].GetlengthOfSequence();
        }
        //cropResidueC = CinputToSoil-excretalCtoPasture;

        double burntResidueC = burntResidueBlackC + burntResidueCO2C + burntResidueCOC;

        CLost += processStorageCloss + soilCO2_CEmission + soilCleached + burntResidueC;
        
        Cbalance = cInput - (Cexport + CLost + CDeltaSoil);//1.117
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        double diff = 0;
        if (cInput > 0)  //check absolute error, if no C input
            diff = Cbalance / cInput;
        else
            diff = Cbalance;
        if (Math.Abs(diff) > 2 * tolerance)
        {
            double errorPercent = 100 * diff;
            string tmp="Error; C balance at farm scale deviates by more than the permitted margin.\n";
            tmp += ("Percentage error = " + errorPercent.ToString("0.00") + "%"); ;
            GlobalVars.Instance.Error(tmp);
            if (GlobalVars.Instance.getPauseBeforeExit() && rotationList.Count != 0)
                Console.Read();
            if ((rotationList.Count != 0)&&(GlobalVars.Instance.getstopOnError()))
                throw new System.ArgumentException("farmFail", "farmFail");
            else
            {
                Console.Write("there is no soil");
                Console.Read();
            }
          

        }

        Console.Write("");
        //Other C flows
        //NOT CURRENTLY USED

        double CliveCO2 = 0;
        double CliveCH4 = 0;

        for (int i = 0; i < listOfLivestock.Count; i++)
        {
            CliveCO2 += listOfLivestock[i].getCO2C(); //1.120
            CliveCH4 += listOfLivestock[i].getCH4C();//1.121

        }

        double CInhouseExcreta = 0;
        for (int i = 0; i < listOfHousing.Count; i++)
        {
            CInhouseExcreta = listOfHousing[i].getPropTimeThisHouse() * (1 - listOfHousing[i].getTimeOnPasture()) * (listOfHousing[i].getFaecesC() + listOfHousing[i].getUrineC()); //1.122

        }

        double CInhouseExcretaBedding = 0;
        for (int i = 0; i < listOfHousing.Count; i++)
        {
            CInhouseExcretaBedding = listOfHousing[i].getBeddingC();//1.123


        }
        double foodWaste = 0;
        for (int i = 0; i < listOfHousing.Count; i++)
        {
            foodWaste = listOfHousing[i].getFeedWasteC();//1.124
        }

        double Cmanure = 0;
        for (int i = 0; i < listOfHousing.Count; i++)
        {
            Cmanure = listOfHousing[i].getManureCarbon();//1.125
        }

        //N balance 
        // N inputs
        //N fixation

      



        for (int i = 0; i < rotationList.Count; i++)
        {
            nFix += rotationList[i].getNFix() / rotationList[i].GetlengthOfSequence();//1.132
        }
        // N deposition from atmosphere
        for (int i = 0; i < rotationList.Count; i++)
        {
            Natm += rotationList[i].getNAtm() / rotationList[i].GetlengthOfSequence();//1.133
        }
        //Fertiliser N
        for (int i = 0; i < rotationList.Count; i++)
        {
            nFert += rotationList[i].getFertiliserNapplied() / rotationList[i].GetlengthOfSequence();
        }

        //Imported bedding N
        //N fed in housing
        for (int i = 0; i < listOfHousing.Count; i++)
        {
            Nbedding += listOfHousing[i].getBeddingN(); //1.102
            NfedInHousing += listOfHousing[i].GetNfedInHousing();
        }
        //Imported manure N
        for (int i = 0; i < GlobalVars.Instance.theManureExchange.GetmanuresImported().Count; i++)
        {
            Nmanimp += GlobalVars.Instance.theManureExchange.GetmanuresImported()[i].GetTotalN();// / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        }
        manureImportN = Nmanimp;
        NPlantProductImported = compositeProductImported.composition.Getamount() * compositeProductImported.composition.GetN_conc();
        double Ninput = Natm + nFert + nFix + Nmanimp + NPlantProductImported;
        
        liveFeedImportN = NPlantProductImported-Nbedding;

        //Nexport
        //N exported in crop products
        double temp = compositeProductExported.composition.Getamount();/// GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        Nsold = compositeProductExported.composition.Getamount() * compositeProductExported.composition.GetN_conc();

        double livestockNintake = 0;
        double livestockNexcreted = 0;
        for (int i = 0; i < listOfLivestock.Count; i++)
        {
            Nmilk += listOfLivestock[i].GetMilkN() * listOfLivestock[i].GetAvgNumberOfAnimal(); //1.113
            NGrowth += listOfLivestock[i].GetGrowthN() * listOfLivestock[i].GetAvgNumberOfAnimal();//1.114
            Nmortalities += listOfLivestock[i].GetMortalitiesN() * listOfLivestock[i].GetAvgNumberOfAnimal();//1.114
            listOfLivestock[i].CheckLivestockNBalances();
            livestockNintake += listOfLivestock[i].GetNintake() * listOfLivestock[i].GetAvgNumberOfAnimal();
            NinGrazedFeed += listOfLivestock[i].GetgrazedN() * listOfLivestock[i].GetAvgNumberOfAnimal();
            livestockNexcreted += listOfLivestock[i].GetExcretedN() * listOfLivestock[i].GetAvgNumberOfAnimal();
            liveToFieldN += listOfLivestock[i].GetNexcretionToPasture() * listOfLivestock[i].GetAvgNumberOfAnimal();
        }
        fieldharvestedConsumedN = livestockNintake-NinGrazedFeed;
        liveGrazedN = NinGrazedFeed;

        //N from grazed feed
        grazedN = NinGrazedFeed;
        NfedInHousing = livestockNintake - NinGrazedFeed;
        for (int i = 0; i < GlobalVars.Instance.theManureExchange.GetmanuresExported().Count; i++)
        {
            Nmanexp += GlobalVars.Instance.theManureExchange.GetmanuresExported()[i].GetTotalN();// / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        }
        manureExportN = Nmanexp;

        NExport = Nmanexp + Nsold + Nmilk + NGrowth + Nmortalities;

        for (int i = 0; i < listOfHousing.Count; i++)
        {
            housing ahouse = listOfHousing[i];
            ahouse.CheckHousingNBalance();
            housingNH3Loss += ahouse.GetNNH3housing();
            NexcretedHousing += ahouse.GetNinputInExcreta();
            storageFromFeedWasteN += ahouse.getFeedWasteN();
            storageFromBeddingN += ahouse.getBeddingN();
        }

        houseInFromAnimalsN = NexcretedHousing;
        storageFromHouseN = NexcretedHousing + storageFromBeddingN + storageFromFeedWasteN;
        houseLossN = housingNH3Loss;
        houseToStorageN = houseInFromAnimalsN - houseLossN;

        //N excreted during grazing
        NexcretedField = livestockNexcreted - NexcretedHousing;
        double manureNexStorage = 0;
        NLost += housingNH3Loss;
        for (int i = 0; i < listOfManurestores.Count; i++)
        {
            manureStore amanurestore2 = listOfManurestores[i];
            amanurestore2.CheckManureStoreNBalance();
            manureN2Emission += amanurestore2.GettotalNstoreN2();
            manureN2OEmission += amanurestore2.GettotalNstoreN20();
            manureNH3Emission += amanurestore2.GettotalNstoreNH3();
            runoffN += amanurestore2.GetrunoffN();
            manureNexStorage += amanurestore2.GetManureN();
        }
        manurestoreNLoss = manureN2Emission + manureN2OEmission + manureNH3Emission;
        storageGaseousLossN = manurestoreNLoss;
        storageRunoffN = runoffN;

        NLost += manurestoreNLoss + runoffN;

        manureToFieldN = manureNexStorage + manureImportN - manureExportN;

        double DMYield = 0;
        double startsoilMineralN = 0;
        burntResidueN2ON = 0;
        burntResidueNH3N = 0;
        burntResidueNOxN = 0;
        fieldUrineNH3Emission = 0;
        for (int rotationID = minRotation; rotationID <= maxRotation; rotationID++)
        {
            rotationList[rotationID - 1].CheckRotationNBalance();
            NDeltaSoil += (rotationList[rotationID - 1].GetNStored() - rotationList[rotationID - 1].GetinitialSoilN()) / rotationList[rotationID - 1].GetlengthOfSequence();
            fieldN2Emission += rotationList[rotationID - 1].GetN2NEmission() / rotationList[rotationID - 1].GetlengthOfSequence();
            fertiliserN2OEmission += rotationList[rotationID - 1].GetfertiliserN2ONEmissions() / rotationList[rotationID - 1].GetlengthOfSequence();
            cropResidueN2O += rotationList[rotationID - 1].GetcropResidueN2ON() / rotationList[rotationID - 1].GetlengthOfSequence();
            fieldN2OEmission += rotationList[rotationID - 1].GetN2ONemission() / rotationList[rotationID - 1].GetlengthOfSequence();
            fertNH3NEmission += rotationList[rotationID - 1].GetFertNH3NEmission() / rotationList[rotationID - 1].GetlengthOfSequence();
            fieldmanureNH3Emission += rotationList[rotationID - 1].GetManureNH3NEmission() / rotationList[rotationID - 1].GetlengthOfSequence();
            fieldUrineNH3Emission += rotationList[rotationID - 1].GeturineNH3emissions() / rotationList[rotationID - 1].GetlengthOfSequence();
            Nleaching += rotationList[rotationID - 1].GettheNitrateLeaching() / rotationList[rotationID - 1].GetlengthOfSequence();
            organicNLeached += rotationList[rotationID - 1].GetOrganicNLeached() / rotationList[rotationID - 1].GetlengthOfSequence();
            burntResidueN2ON += rotationList[rotationID - 1].getBurntResidueN2ON ()/ rotationList[rotationID - 1].GetlengthOfSequence();
            burntResidueNH3N += rotationList[rotationID - 1].getBurntResidueNH3N() / rotationList[rotationID - 1].GetlengthOfSequence();
            burntResidueNOxN += rotationList[rotationID - 1].getBurntResidueNOxN() / rotationList[rotationID - 1].GetlengthOfSequence();
            burntResidueOtherN += rotationList[rotationID - 1].getBurntResidueOtherN() / rotationList[rotationID - 1].GetlengthOfSequence();
            residualSoilMineralN += rotationList[rotationID - 1].GetResidualSoilMineralN();
            startsoilMineralN += rotationList[rotationID - 1].GetstartsoilMineralN();
            DMYield += rotationList[rotationID - 1].GetDMYield() / rotationList[rotationID - 1].GetlengthOfSequence();
            processStorageNloss += rotationList[rotationID - 1].getProcessStorageLossNitrogen() / rotationList[rotationID - 1].GetlengthOfSequence();
        }
        double burntResidueN = burntResidueN2ON + burntResidueNH3N + burntResidueNOxN + burntResidueOtherN;
        fieldGaseousLossN = fertNH3NEmission + fieldmanureNH3Emission + +fieldUrineNH3Emission + fieldN2Emission + fieldN2OEmission
            + burntResidueN + processStorageNloss;

        fieldNLoss = fertNH3NEmission + fieldmanureNH3Emission + fieldUrineNH3Emission + fieldN2Emission + fieldN2OEmission 
            + Nleaching + burntResidueN + organicNLeached + processStorageNloss;
        fieldNitrateLeachedN = organicNLeached + Nleaching;
        changeSoilN = NDeltaSoil;

        NLost += fieldNLoss;
        totalFarmNSurplus = Ninput - NExport; //1.137
        totalFieldNlosses = fieldNLoss - processStorageNloss;
        totalHouseStoreNloss = housingNH3Loss + manurestoreNLoss + processStorageNloss;
        changeInMinN=(residualSoilMineralN-startsoilMineralN) / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        changeAllSoilNstored = NDeltaSoil + changeInMinN;

        Nbalance = Ninput - (NExport + NLost + NDeltaSoil + changeInMinN);//1.117
        if (Ninput > 0)
            diff = Nbalance / Ninput;
        else
            diff = Nbalance;
        if (Math.Abs(diff) > 2 *tolerance)
        {
            double errorPercent = 100 * diff;

            System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            string outstring1 = "Error; N balance at farm scale deviates by more than the permitted margin";
            string outstring2 = "Percentage error = " + errorPercent.ToString("0.00") + "%";
            string outstring3 = "Absolute error = " + Nbalance.ToString();
            file.WriteLine(outstring1);
            file.WriteLine(outstring2);
            file.Write(outstring3);
            file.Close();
            GlobalVars.Instance.log(outstring1, 5);
            GlobalVars.Instance.log(outstring2, 5);
            Console.Write(outstring3);
            if (GlobalVars.Instance.getPauseBeforeExit() && rotationList.Count != 0)
                Console.Read();
            if ((rotationList.Count != 0)&&(GlobalVars.Instance.getstopOnError()))
            {
                WriteFarmBalances();
                GlobalVars.Instance.CloseOutputXML();
                GlobalVars.Instance.CloseOutputTabFile();
                throw new System.ArgumentException("farm Failed", "farm Failed");
            }
            else
            {
                Console.Write("there is no soil");
                Console.Read();
            }

        }
        agriculturalArea= GetAgriculturalArea(rotationList);


        //do GHG budget
        entericCH4CO2Eq = livestockCH4C * GlobalVars.Instance.GetCO2EqCH4();
        manureCH4CO2Eq = manurestoreCH4C * GlobalVars.Instance.GetCO2EqCH4();
        manureN2OCO2Eq = manureN2OEmission * GlobalVars.Instance.GetCO2EqN2O();
        fieldN2OCO2Eq = fieldN2OEmission * GlobalVars.Instance.GetCO2EqN2O();
        soilCO2Eq = -1 * CDeltaSoil * GlobalVars.Instance.GetCO2EqsoilC();
        directGHGEmissionCO2Eq = entericCH4CO2Eq + manureCH4CO2Eq + manureN2OCO2Eq + fieldN2OCO2Eq + soilCO2Eq;

        housingNH3CO2Eq = housingNH3Loss * GlobalVars.Instance.GetIndirectNH3N2OFactor() * GlobalVars.Instance.GetCO2EqN2O();
        manurestoreNH3CO2Eq = manureNH3Emission * GlobalVars.Instance.GetIndirectNH3N2OFactor() * GlobalVars.Instance.GetCO2EqN2O();
        fieldmanureNH3CO2Eq = fieldmanureNH3Emission * GlobalVars.Instance.GetIndirectNH3N2OFactor() * GlobalVars.Instance.GetCO2EqN2O();
        fieldfertNH3CO2Eq = fertNH3NEmission * GlobalVars.Instance.GetIndirectNH3N2OFactor() * GlobalVars.Instance.GetCO2EqN2O();
        leachedNCO2Eq = Nleaching * GlobalVars.Instance.GetIndirectNO3N2OFactor() * GlobalVars.Instance.GetCO2EqN2O();
        indirectGHGCO2Eq = housingNH3CO2Eq + manurestoreNH3CO2Eq + fieldmanureNH3CO2Eq + fieldfertNH3CO2Eq + leachedNCO2Eq;

        double numDairy = 0;
        for (int i = 0; i < listOfLivestock.Count; i++)
        {
            livestock anAnimalCategory = listOfLivestock[i];
            farmMilkProduction += anAnimalCategory.GetavgProductionMilk() * listOfLivestock[i].GetAvgNumberOfAnimal() * GlobalVars.avgNumberOfDays;
            farmMeatProduction += anAnimalCategory.GetavgProductionMeat() * listOfLivestock[i].GetAvgNumberOfAnimal() * GlobalVars.avgNumberOfDays/1000.0;
            if (anAnimalCategory.GetavgProductionMilk() > 0)
                numDairy += listOfLivestock[i].GetAvgNumberOfAnimal();
        }
        if (numDairy > 0)
            avgProductionMilkPerHead = farmMilkProduction / numDairy;
        else
            avgProductionMilkPerHead = 0;

        WriteFarmBalances();
    }
    public void WriteFarmBalances()
        {
            double Nsurp = 0;
            if (agriculturalArea > 0)
                Nsurp = totalFarmNSurplus / agriculturalArea;//1,138

        //writing output.
        GlobalVars.Instance.writeStartTab("FarmBalance");

        GlobalVars.Instance.writeStartTab("Herd");
        GlobalVars.Instance.writeInformationToFiles("liveFeedImportN", "Imported livestock feed", "kgN/yr", liveFeedImportN, parens);
        GlobalVars.Instance.writeInformationToFiles("liveGrazedN", "Grazed", "kgN/yr", liveGrazedN, parens);
        GlobalVars.Instance.writeInformationToFiles("liveToHousingN", "Deposited in housing", "kgN/yr", liveToHousingN, parens);
        GlobalVars.Instance.writeInformationToFiles("liveToFieldN", "Deposited in field", "kgN/yr", liveToFieldN, parens);
        GlobalVars.Instance.writeEndTab();

        GlobalVars.Instance.writeStartTab("housing");
        GlobalVars.Instance.writeInformationToFiles("houseInFromAnimalsN", "Input from livestock", "kgN/yr", houseInFromAnimalsN, parens);
        GlobalVars.Instance.writeInformationToFiles("houseLossN", "Gaseous loss", "kgN/yr", houseLossN, parens);
        GlobalVars.Instance.writeInformationToFiles("houseToStorageN", "Sent to storage", "kgN/yr", houseToStorageN, parens);
        GlobalVars.Instance.writeEndTab();

        GlobalVars.Instance.writeStartTab("ManureStorage");
        GlobalVars.Instance.writeInformationToFiles("storageFromHouseN", "Input from housing", "kgN/yr", storageFromHouseN, parens);
        GlobalVars.Instance.writeInformationToFiles("storageFromBeddingN", "Bedding", "kgN/yr", storageFromBeddingN, parens);
        GlobalVars.Instance.writeInformationToFiles("storageFromFeedWasteN", "Feed wastage", "kgN/yr", storageFromFeedWasteN, parens);
        GlobalVars.Instance.writeInformationToFiles("storageGaseousLossN", "Gaseous loss", "kgN/yr", storageGaseousLossN, parens);
        GlobalVars.Instance.writeInformationToFiles("storageRunoffN", "Runoff", "kgN/yr", storageRunoffN, parens);
        GlobalVars.Instance.writeInformationToFiles("manureImportN", "Imported", "kgN/yr", manureImportN, parens);
        GlobalVars.Instance.writeInformationToFiles("manureExportN", "Exported", "kgN/yr", manureExportN, parens);
        GlobalVars.Instance.writeEndTab();

        GlobalVars.Instance.writeStartTab("Fields");
        GlobalVars.Instance.writeInformationToFiles("manureToFieldN", "Manure", "kgN/yr", manureToFieldN, parens);
        GlobalVars.Instance.writeInformationToFiles("fieldGaseousLossN", "Gaseous loss", "kgN/yr", fieldGaseousLossN, parens);
        GlobalVars.Instance.writeInformationToFiles("fieldNitrateLeachedN", "Nitrate leaching", "kgN/yr", fieldNitrateLeachedN, parens);
        GlobalVars.Instance.writeInformationToFiles("fieldharvestedConsumedN", "Harvested mechanically", "kgN/yr", fieldharvestedConsumedN, parens);
        GlobalVars.Instance.writeInformationToFiles("grazedN", "Grazed", "kgN/yr", grazedN, parens);
        GlobalVars.Instance.writeInformationToFiles("changeSoilN", "Change in soil storage", "kgN/yr", changeSoilN, parens);
        GlobalVars.Instance.writeEndTab();

        GlobalVars.Instance.writeStartTab("FarmCBalance");
        GlobalVars.Instance.writeInformationToFiles("carbonFromPlants", "C fixed from atmosphere", "kg C/yr", carbonFromPlants, parens);
        GlobalVars.Instance.writeInformationToFiles("Cmanimp", "C in imported manure", "kg C/yr", Cmanimp, parens);
        GlobalVars.Instance.writeInformationToFiles("CPlantProductImported", "C in imported feed", "kg C/yr", CPlantProductImported, parens);
        GlobalVars.Instance.writeInformationToFiles("CbeddingReq", "C in imported bedding", "kg C/yr", CbeddingReq, parens);
        GlobalVars.Instance.writeInformationToFiles("Cmilk", "C in exported milk", "kg C/yr", Cmilk, parens);
        GlobalVars.Instance.writeInformationToFiles("Cmeat", "C in exported meat", "kg C/yr", Cmeat, parens);
        GlobalVars.Instance.writeInformationToFiles("Cmortalities", "C in mortalities", "kg C/yr", Cmortalities, parens);
        GlobalVars.Instance.writeInformationToFiles("Csold", "C in crop products sold", "kg C/yr", Csold, parens);
        GlobalVars.Instance.writeInformationToFiles("Cmanexp", "C in exported manure", "kg C/yr", Cmanexp, parens);
        GlobalVars.Instance.writeInformationToFiles("livestockCH4C", "C in enteric methane emissions", "kg C/yr", livestockCH4C, parens);
        GlobalVars.Instance.writeInformationToFiles("livestockCO2C", "C in CO2 emitted by livestock", "kg C/yr", livestockCO2C, parens);
        GlobalVars.Instance.writeInformationToFiles("housingCLoss", "C in CO2 emitted from animal housing", "kg C/yr", housingCLoss, parens);
        GlobalVars.Instance.writeInformationToFiles("manurestoreCH4C", "C in methane emitted by manure", "kg C/yr", manurestoreCH4C, parens);
        GlobalVars.Instance.writeInformationToFiles("manurestoreCO2C", "C in CO2 emitted by manure", "kg C/yr", manurestoreCO2C, parens);
        GlobalVars.Instance.writeInformationToFiles("biogasCH4C", "C in biogas methane", "kg C/yr", biogasCH4C, parens);
        GlobalVars.Instance.writeInformationToFiles("biogasCO2C", "C in biogas CO2", "kg C/yr", biogasCO2C, parens);
        GlobalVars.Instance.writeInformationToFiles("processStorageCloss", "C in CO2 lost from stored crop products", "kg C/yr", processStorageCloss, parens);
        GlobalVars.Instance.writeInformationToFiles("soilCO2_CEmission", "C in CO2 emitted by the soil", "kg C/yr", soilCO2_CEmission, parens);
        GlobalVars.Instance.writeInformationToFiles("soilCleached", "C in organic matter leached from the soil", "kg C/yr", soilCleached, parens);
        GlobalVars.Instance.writeInformationToFiles("burntResidueCOC", "CO-C from burning crop residues", "kg C/yr", burntResidueCOC, parens);
        GlobalVars.Instance.writeInformationToFiles("burntResidueCO2C", "CO2-C in gases from burning crop residues", "kg C/yr", burntResidueCO2C, parens);
        GlobalVars.Instance.writeInformationToFiles("burntResidueBlackC", "Black carbon in gases from burning crop residues", "kg C/yr", burntResidueBlackC, parens);
        GlobalVars.Instance.writeInformationToFiles("CDeltaSoil", "Change in C stored in the soil", "kg C/yr", CDeltaSoil, parens);
        GlobalVars.Instance.writeInformationToFiles("CLost", "C lost to the environment", "kg C/yr", CLost, parens);
        GlobalVars.Instance.writeInformationToFiles("Cbalance", "Net C balance (should be about zero)", "kg C/yr", Cbalance, parens);
        if (agriculturalArea > 0)
        {
            GlobalVars.Instance.writeStartTab("PerUnitArea");
            GlobalVars.Instance.writeInformationToFiles("carbonFromPlants", "C fixed from atmosphere", "kg C/ha/yr", carbonFromPlants / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Cmanimp", "C in imported manure", "kg C/ha/yr", Cmanimp / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("CPlantProductImported", "C in imported feed", "kg C/ha/yr", CPlantProductImported / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("CbeddingReq", "C in imported bedding", "kg C/ha/yr", CbeddingReq / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Cmilk", "C in exported milk", "kg C/ha/yr", Cmilk / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Cmeat", "C in exported meat", "kg C/ha/yr", Cmeat / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Cmortalities", "C in mortalities", "kg C/ha/yr", Cmortalities / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Csold", "C in crop products sold", "kg C/ha/yr", Csold / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("soilCO2_CEmission", "C in CO2 emitted by the soil", "kg C/ha/yr", soilCO2_CEmission / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("soilCleached", "C in organic matter leached from the soil", "kg C/ha/yr", soilCleached / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("burntResidueCOC", "CO-C from burning crop residues", "kg C/ha/yr", burntResidueCOC / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("burntResidueCO2C", "CO2-C in gases from burning crop residues", "kg C/ha/yr", burntResidueCO2C / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("burntResidueBlackC", "Black carbon in gases from burning crop residues", "kg C/ha/yr", burntResidueBlackC / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("CDeltaSoil", "Change in C stored in the soil", "kg C/ha/yr", CDeltaSoil / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Cbalance", "Net C balance (should be about zero)", "kg C/ha/yr", Cbalance / agriculturalArea, parens);
            GlobalVars.Instance.writeEndTab();
        }
        GlobalVars.Instance.writeEndTab();
        GlobalVars.Instance.writeStartTab("FarmNBalance");
        GlobalVars.Instance.writeInformationToFiles("Nmanimp", "N in imported manure", "kg N/yr", Nmanimp, parens);
        GlobalVars.Instance.writeInformationToFiles("nFix", "N fixation", "kg N/yr", nFix, parens);
        GlobalVars.Instance.writeInformationToFiles("Natm", "N deposited from atmosphere", "kg N/yr", Natm, parens);
        GlobalVars.Instance.writeInformationToFiles("nFert", "N in fertiliser", "kg N/yr", nFert, parens);
        GlobalVars.Instance.writeInformationToFiles("Nbedding", "N in imported bedding", "kg N/yr", Nbedding, parens);
        GlobalVars.Instance.writeInformationToFiles("NPlantProductImported", "N in imported crop products", "kg N/yr", NPlantProductImported, parens);
        GlobalVars.Instance.writeInformationToFiles("processStorageNloss", "N in NH3 lost from stored crop products", "kg N/yr", processStorageNloss, parens);
        GlobalVars.Instance.writeInformationToFiles("Nsold", "N sold in crop products", "kg N/yr", Nsold, parens);
        GlobalVars.Instance.writeInformationToFiles("Nmilk", "N sold in milk", "kg N/yr", Nmilk, parens);
        GlobalVars.Instance.writeInformationToFiles("NGrowth", "N exported in meat", "kg N/yr", NGrowth, parens);
        GlobalVars.Instance.writeInformationToFiles("Nmortalities", "N in mortalities", "kg N/yr", Nmortalities, parens);
        GlobalVars.Instance.writeInformationToFiles("Nmanexp", "N in exported manure", "kg N/yr", Nmanexp, parens);
        GlobalVars.Instance.writeInformationToFiles("NExport", "Total amount of N exported", "kg N/yr", NExport, parens);
        GlobalVars.Instance.writeInformationToFiles("housingNH3Loss", "N lost in NH3 emission from housing", "kg N/yr", housingNH3Loss, parens);
        GlobalVars.Instance.writeInformationToFiles("manureN2Emission", "N lost in N2 emission from manure storage", "kg N/yr", manureN2Emission, parens);
        GlobalVars.Instance.writeInformationToFiles("manureN2OEmission", "N lost in N2O emission from manure storage", "kg N/yr", manureN2OEmission, parens);
        GlobalVars.Instance.writeInformationToFiles("manureNH3Emission", "N lost in NH3 emission from manure storage", "kg N/yr", manureNH3Emission, parens);
        GlobalVars.Instance.writeInformationToFiles("fieldUrineNH3Emission", "N lost in NH3 emission from urine deposited in field", "kg N/yr", fieldUrineNH3Emission, parens);
        GlobalVars.Instance.writeInformationToFiles("runoffN", "N lost in runoff from manure storage", "kg N/yr", runoffN, parens);
        GlobalVars.Instance.writeInformationToFiles("fieldN2Emission", "Emission of N2 from the field", "kg N/yr", fieldN2Emission, parens);
        GlobalVars.Instance.writeInformationToFiles("fieldN2OEmission", "Emission of N2O from the field", "kg N/yr", fieldN2OEmission, parens);
        GlobalVars.Instance.writeInformationToFiles("fertNH3NEmission", "N lost via NH3 emission from fertilisers", "kg N/yr", fertNH3NEmission, parens);
        GlobalVars.Instance.writeInformationToFiles("fieldmanureNH3Emission", "N lost as NH3 from field-applied manure", "kg N/yr", fieldmanureNH3Emission, parens);
        GlobalVars.Instance.writeInformationToFiles("Nleaching", "N lost via NO3 leaching from soil", "kg N/yr", Nleaching, parens);
        GlobalVars.Instance.writeInformationToFiles("organicNLeached", "N lost via leaching of organic N from soil", "kg N/yr", organicNLeached, parens);
        GlobalVars.Instance.writeInformationToFiles("burningN2ON", "N2O in gases from burning crop residues", "kg N/yr", burntResidueN2ON, parens);
        GlobalVars.Instance.writeInformationToFiles("burningNH3N", "NH3 in gases from burning crop residues", "kg N/yr", burntResidueNH3N, parens);
        GlobalVars.Instance.writeInformationToFiles("burningNOxN", "NOx in gases from burning crop residues", "kg N/yr", burntResidueNOxN, parens);
        GlobalVars.Instance.writeInformationToFiles("burningOtherN", "N in other gases from burning crop residues", "kg N/yr", burntResidueOtherN, parens);
        GlobalVars.Instance.writeInformationToFiles("NDeltaSoil", "Change in N stored in soil", "kg N/yr", NDeltaSoil, parens);
        GlobalVars.Instance.writeInformationToFiles("NDeltaMineral", "Change in N stored in mineral form in soil", "kg N/yr", changeInMinN, parens);
        GlobalVars.Instance.writeInformationToFiles("totalHouseStoreNloss", "Total N losses from product storage, housing and manure storage", "kg N/yr", totalHouseStoreNloss, parens);
        GlobalVars.Instance.writeInformationToFiles("totalFieldNlosses", "Total N losses from fields", "kg N/yr", totalFieldNlosses, parens);
        GlobalVars.Instance.writeInformationToFiles("changeAllSoilNstored", "Change in N stored in organic and mineral form in soil", "kg N/yr", changeAllSoilNstored, parens);

        GlobalVars.Instance.writeInformationToFiles("Nsurplus", "N surplus", "kg N/yr", totalFarmNSurplus, parens);
        GlobalVars.Instance.writeInformationToFiles("Nbalance", "N balance (should be about zero)", "kg N/yr", Nbalance, parens);
        if (agriculturalArea > 0)
        {
            GlobalVars.Instance.writeStartTab("PerUnitArea");
            GlobalVars.Instance.writeInformationToFiles("Nmanimp", "N in imported manure", "kg N/ha/yr", Nmanimp / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("nFix", "N fixation", "kg N/ha/yr", nFix / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Natm", "N deposited from atmosphere", "kg N/ha/yr", Natm / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("nFert", "N in fertiliser", "kg N/ha/yr", nFert / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Nbedding", "N in imported bedding", "kg N/ha/yr", Nbedding / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("NPlantProductImported", "N in imported crop products", "kg N/ha/yr", NPlantProductImported / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Nsold", "N sold in crop products", "kg N/ha/yr", Nsold / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Nmilk", "N sold in milk", "kg N/ha/yr", Nmilk / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("NGrowth", "N exported in meat", "kg N/ha/yr", NGrowth / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Nmortalities", "N in mortalities", "kg N/ha/yr", Nmortalities / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Nmanexp", "N in exported manure", "kg N/ha/yr", Nmanexp / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("NExport", "Total amount of N exported", "kg N/ha/yr", NExport / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("housingNH3Loss", "N lost in NH3 emission from housing", "kg N/ha/yr", housingNH3Loss / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("manureN2Emission", "N lost in N2 emission from manure storage", "kg N/ha/yr", manureN2Emission / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("manureN2OEmission", "N lost in N2O emission from manure storage", "kg N/ha/yr", manureN2OEmission / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("manureNH3Emission", "N lost in NH3 emission from manure storage", "kg N/ha/yr", manureNH3Emission / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("runoffN", "N lost in runoff from manure storage", "kg N/ha/yr", runoffN / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("fieldN2Emission", "Emission of N2 from the field", "kg N/ha/yr", fieldN2Emission / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("fieldN2OEmission", "Emission of N2O from the field", "kg N/ha/yr", fieldN2OEmission / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("fertNH3NEmission", "N lost via NH3 emission from fertilisers", "kg N/ha/yr", fertNH3NEmission / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("fieldmanureNH3Emission", "N lost as NH3 from field-applied manure", "kg N/ha/yr", fieldmanureNH3Emission / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Nleaching", "N lost via NO3 leaching from soil", "kg N/ha/yr", Nleaching / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("organicNLeached", "N lost via leaching of organic N from soil", "kg N/ha/yr", organicNLeached / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("burningN2ON", "N2O in gases from burning crop residues", "kg N/ha/yr", burntResidueN2ON / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("burningNH3", "NH3 in gases from burning crop residues", "kg N/ha/yr", burntResidueNH3N / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("burningNOx", "NOx in gases from burning crop residues", "kg N/ha/yr", burntResidueNOxN / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("burningOtherN", "N in other gases from burning crop residues", "kg N/ha/yr", burntResidueOtherN / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("NDeltaSoil", "Change in N stored in soil", "kg N/ha/yr", NDeltaSoil / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("NDeltaMineral", "Change in N stored in mineral form in soil", "kg N/ha/yr", changeInMinN / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("Nsurplus", "N surplus", "kg N/ha/yr", totalFarmNSurplus / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("NexcretedHousing", "N excreted in housing", "kg N/ha/yr", NexcretedHousing / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("NexcretedField", "N excreted in field", "kg N/ha/yr", NexcretedField / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("NinGrazedFeed", "N in grazed feed", "kg N/ha/yr", NinGrazedFeed / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("NfedInHousing", "N fed in housing", "kg N/ha/yr", NfedInHousing / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("totalHouseStoreNlossInFarmBalance", "Total N losses from product storage, housing and manure storage", "kg N/ha/yr", totalHouseStoreNloss / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("totalFieldNlossesInFarmBalance", "Total N losses from fields", "kg N/ha/yr", totalFieldNlosses / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("changeAllSoilNstoredInFarmBalance", "Change in N stored in organic and mineral form in soil", "kg N/ha/yr", changeAllSoilNstored / agriculturalArea, parens);
            GlobalVars.Instance.writeInformationToFiles("NbalanceInFarmBalance", "N balance (should be about zero)", "kg N/ha/yr", Nbalance / agriculturalArea, parens);
            GlobalVars.Instance.writeEndTab();
        }
        GlobalVars.Instance.writeEndTab();
        GlobalVars.Instance.writeStartTab("FarmDirectGHG");
        GlobalVars.Instance.writeInformationToFiles("entericCH4CO2Eq", "Enteric methane emissions", "kg CO2 equivalents/yr", entericCH4CO2Eq, parens);
        GlobalVars.Instance.writeInformationToFiles("manureCH4CO2Eq", "Manure methane emissions", "kg CO2 equivalents/yr", manureCH4CO2Eq, parens);
        GlobalVars.Instance.writeInformationToFiles("manureN2OCO2Eq", "Manure N2O emissions", "kg CO2 equivalents/yr", manureN2OCO2Eq, parens);
        GlobalVars.Instance.writeInformationToFiles("fieldN2OCO2Eq", "Field N2O emissions", "kg CO2 equivalents/yr", fieldN2OCO2Eq, parens);
        GlobalVars.Instance.writeInformationToFiles("soilCO2Eq", "Change in C stored in soil", "kg CO2 equivalents/yr", soilCO2Eq, parens);
        GlobalVars.Instance.writeInformationToFiles("directGHGEmissionCO2Eq", "Total GHG emissions", "kg CO2 equivalents/yr", directGHGEmissionCO2Eq, parens);
        GlobalVars.Instance.writeEndTab();
        GlobalVars.Instance.writeStartTab("FarmIndirectGHG");
        GlobalVars.Instance.writeInformationToFiles("housingNH3CO2Eq", "Housing NH3 emissions", "kg CO2 equivalents/yr", housingNH3CO2Eq, parens);
        GlobalVars.Instance.writeInformationToFiles("manurestoreNH3CO2Eq", "Manure storage NH3 emissions", "kg CO2 equivalents/yr", manurestoreNH3CO2Eq, parens);
        GlobalVars.Instance.writeInformationToFiles("fieldmanureNH3CO2Eq", "NH3 emissions from field-applied manure", "kg CO2 equivalents/yr", fieldmanureNH3CO2Eq, parens);
        GlobalVars.Instance.writeInformationToFiles("fieldfertNH3CO2Eq", "NH3 emissions from fertilisers", "kg CO2 equivalents/yr", fieldfertNH3CO2Eq, parens);
        GlobalVars.Instance.writeInformationToFiles("leachedNCO2Eq", "N2O emissions resulting from leaching of N", "kg CO2 equivalents/yr", leachedNCO2Eq, parens);
        GlobalVars.Instance.writeInformationToFiles("indirectGHGCO2Eq", "Total indirect emissions", "kg CO2 equivalents/yr", indirectGHGCO2Eq, parens);
        GlobalVars.Instance.writeEndTab();
        GlobalVars.Instance.writeEndTab();

        GlobalVars.Instance.writeStartTab("Indicators");
        GlobalVars.Instance.writeInformationToFiles("FarmMilkProduction", "Total farm milk production", "kg/yr", farmMilkProduction, parens);
        GlobalVars.Instance.writeInformationToFiles("FarmMeatProduction", "Total farm meat production", "kg/yr", farmMeatProduction, parens);
        GlobalVars.Instance.writeInformationToFiles("FarmMilkProductionPerHead", "Farm milk production per head", "kg/yr", avgProductionMilkPerHead, parens);
        GlobalVars.Instance.writeInformationToFiles("MilkProductionPerUnitArea", "Milk production per unit area", "kg/ha/yr", farmMilkProduction/agriculturalArea, parens);
        GlobalVars.Instance.writeEndTab();
    }
}