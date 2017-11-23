using System;
using System.Collections.Generic;
using System.Xml;
class farmBalanceClass
{
    double entericCH4CO2Eq = 0.0;
    double manureCH4CO2Eq = 0;
    double manureN2OCO2Eq = 0;
    double fieldN2OCO2Eq = 0;
    double directGHGEmissionCO2Eq = 0;

    double housingNH3CO2Eq = 0;
    double manurestoreNH3CO2Eq = 0;
    double fieldmanureNH3CO2Eq = 0;
    double fieldfertNH3CO2Eq =0;
    double leachedNCO2Eq = 0;
    double indirectGHGCO2Eq = 0;
    //!carbon fixation by crops (kg)
    double carbonFromPlants = 0;
    double initialSoilC = 0;
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
    double soilCO2Emission = 0;
    //!C lost from stored plant products
    double processStorageCloss = 0;
    //!C in organic matter leached from soil
    double soilCleached = 0;
    //!continuity check for C
    double Cbalance=0;

    //! N input via N fixation in crops (kg)
    double nFix = 0;
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
    //!N exported in animal manure
    double Nmanexp = 0;
    //total N export
    double NExport = 0;
    // N losses and change in N stored in soil
    double NDeltaSoil = 0;
    //!total N lost
    double NLost = 0;
    //!N lost as NH3 from housing
    double housingNLoss = 0;
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
    //NO3-N leaching from soil
    double Nleaching = 0;
    //!initial soil nitrogen
    double initialNSoil = 0;
    //!final soil nitrogen
    double finalNSoil = 0;
    //!nitrous oxide emission from fertiliser
    double fertiliserN2OEmission = 0;
   //!nitrous oxide emissions from crop residues
    double cropResidueN2O = 0;
    //!total nitrous oxide emissions from soil
    double soilN2OEmission = 0;
    //!leaching of organic nitrogen
    double organicNLeached = 0;
    //!runoff from manure storage
    double runoffN = 0;
    //!residual soil mineral N at end of crop sequence
    double residualSoilMineralN = 0;
    //!continuity check
    double Nbalance = 0;


    public farmBalanceClass()
    {
    }

    public void DoFarmBalances(List<CropSequenceClass> rotationList, List<livestock> listOfLivestock, List<housing> listOfHousing,
        List<manureStore> listOfManurestores, XmlWriter writer, System.IO.StreamWriter tabFile)
    {
        //Farm balances
        //C balance
        //C inputs
        GlobalVars.Instance.CalculateTradeBalance();

        int minRotation = 1;
        int maxRotation = rotationList.Count;
        for (int rotationID = minRotation; rotationID <= maxRotation; rotationID++)
        {
            carbonFromPlants += rotationList[rotationID - 1].getCFixed(); //1.100
        }
        carbonFromPlants /= GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        for (int i = 0; i < GlobalVars.Instance.theManureExchange.GetmanuresImported().Count; i++)
        {
            Cmanimp += GlobalVars.Instance.theManureExchange.GetmanuresImported()[i].GetTotalC();
        }
        Cmanimp /= GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        GlobalVars.product compositeProductImported = GlobalVars.Instance.GetPlantProductImports();
        CPlantProductImported = compositeProductImported.composition.Getamount() * compositeProductImported.composition.GetC_conc();
      //  CPlantProductImported /= GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate(); 
        CbeddingReq = GlobalVars.Instance.GetthebeddingMaterial().Getamount() *
            GlobalVars.Instance.GetthebeddingMaterial().GetC_conc();
        CPlantProductImported += CbeddingReq;

        double cInput = carbonFromPlants + CPlantProductImported + Cmanimp;

        //C outputs
        GlobalVars.product compositeProductExported = GlobalVars.Instance.GetPlantProductExports();
        GlobalVars.Instance.PrintPlantProducts();
        Csold = compositeProductExported.composition.Getamount() * compositeProductExported.composition.GetC_conc();
        Csold /= GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();

        for (int i = 0; i < listOfLivestock.Count; i++)
        {
            Cmilk += listOfLivestock[i].GetMilkC() * listOfLivestock[i].GetAvgNumberOfAnimal(); //1.113
            Cmeat += listOfLivestock[i].GetMeatC() * listOfLivestock[i].GetAvgNumberOfAnimal();//1.114
        }

        for (int i = 0; i < GlobalVars.Instance.theManureExchange.GetmanuresExported().Count; i++)
        {
            Cmanexp += GlobalVars.Instance.theManureExchange.GetmanuresExported()[i].GetTotalC();
            Cmanexp /= GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        }
        double Cexport = Csold + Cmilk + Cmeat + Cmanexp;//1.116
        for (int i = 0; i < listOfLivestock.Count; i++)
        {
            livestock anAnimalCategory = listOfLivestock[i];
            anAnimalCategory.CheckLivestockCBalance();
            livestockCH4C += anAnimalCategory.getCH4C() * listOfLivestock[i].GetAvgNumberOfAnimal();
            livestockCO2C += anAnimalCategory.getCO2C() * listOfLivestock[i].GetAvgNumberOfAnimal();
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

        double manurestoreCLoss = 0;
        for (int i = 0; i < listOfManurestores.Count; i++)
        {
            manureStore amanurestore2 = listOfManurestores[i];
            amanurestore2.CheckManureStoreCBalance();
            manurestoreCO2C += amanurestore2.GetCCO2ST();
            manurestoreCH4C += amanurestore2.GetCCH4ST();
        }
        manurestoreCLoss = manurestoreCH4C + manurestoreCO2C;
        CLost += manurestoreCLoss;

        soilCO2Emission = 0;
        processStorageCloss = 0;
        for (int rotationID = minRotation; rotationID <= maxRotation; rotationID++)
        {
            rotationList[rotationID - 1].CheckRotationCBalance();
            //double initialSoilC=rotationList[rotationID - 1].GetinitialSoilC();
            CDeltaSoil += rotationList[rotationID - 1].GetCdeltaSoil();
            CDeltaSoil /= GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            soilCO2Emission += rotationList[rotationID - 1].GetsoilCO2Emission();
            soilCO2Emission /= GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            soilCleached += rotationList[rotationID - 1].GetCleached();
            soilCleached /= GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            processStorageCloss += rotationList[rotationID - 1].getProcessStorageLossCarbon();
            processStorageCloss /= GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        }
        CLost += processStorageCloss + soilCO2Emission + soilCleached;
        
        Cbalance = cInput - (Cexport + CLost + CDeltaSoil);//1.117
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        double diff = 0;
        if (cInput > 0)  //check absolute error, if no C input
            diff = Cbalance / cInput;
        else
            diff = Cbalance;
        if (Math.Abs(diff) > tolerance)
        {
            double errorPercent = 100 * diff;

            System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.WriteLine("Error; C balance at farm scale deviates by more than the permitted margin");
            file.WriteLine("Percentage error = " + errorPercent.ToString("0.00") + "%");
            file.Close();
            Console.WriteLine("Error; C balance at farm scale deviates by more than the permitted margin");
            Console.WriteLine("Percentage error = " + errorPercent.ToString("0.00") + "%");
            if (GlobalVars.Instance.getPauseBeforeExit() && rotationList.Count != 0)
                Console.Read();
            if ((rotationList.Count != 0)&&(GlobalVars.Instance.getstopOnError()))
                Environment.Exit(0);
            else
            {
                Console.Write("there is no soil");
                Console.Read();
            }

        }


        //Other C flows
        //NOT CURRENTLY USED

        double CfeedHome = 0;// Cfeedreq - CfeedImp;//1.119
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
        for (int i = 0; i < rotationList.Count; i++)
        {
            nFix += rotationList[i].getNFix()/GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();//1.132
        }
        for (int i = 0; i < rotationList.Count; i++)
        {
            Natm += rotationList[i].getNAtm() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();//1.133
        }

        for (int i = 0; i < rotationList.Count; i++)
        {
            nFert += rotationList[i].getFertiliserNapplied() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        }

        //need to be checked by Nic since there is one loop but sum in papir. Papir also talkes about substraction but formular does not
        for (int i = 0; i < listOfHousing.Count; i++)
        {
            Nbedding += listOfHousing[i].getBeddingN(); //1.102
        }

        for (int i = 0; i < GlobalVars.Instance.theManureExchange.GetmanuresImported().Count; i++)
        {
            Nmanimp += GlobalVars.Instance.theManureExchange.GetmanuresImported()[i].GetTotalN() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        }
        NPlantProductImported = compositeProductImported.composition.Getamount() * compositeProductImported.composition.GetN_conc();
        double Ninput = Natm + nFert + Nbedding + Nmanimp + NPlantProductImported;

        //Nexport
        Nsold = compositeProductExported.composition.Getamount() * compositeProductExported.composition.GetN_conc() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        for (int i = 0; i < listOfLivestock.Count; i++)
        {
            Nmilk += listOfLivestock[i].GetMilkN() * listOfLivestock[i].GetAvgNumberOfAnimal(); //1.113
            NGrowth += listOfLivestock[i].GetGrowthN() * listOfLivestock[i].GetAvgNumberOfAnimal();//1.114
            listOfLivestock[i].CheckLivestockNBalances();
            //Nmanexp += listOfLivestock[i].getNmanExp();//1.115
        }
        for (int i = 0; i < GlobalVars.Instance.theManureExchange.GetmanuresExported().Count; i++)
        {
            Nmanexp += GlobalVars.Instance.theManureExchange.GetmanuresExported()[i].GetTotalN() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
        }
        NExport = Nmanexp + Nsold + Nmilk + NGrowth;

        for (int i = 0; i < listOfHousing.Count; i++)
        {
            housing ahouse = listOfHousing[i];
            ahouse.CheckHousingNBalance();
            housingNLoss += ahouse.GetNNH3housing();
        }

        NLost += housingNLoss;
        for (int i = 0; i < listOfManurestores.Count; i++)
        {
            manureStore amanurestore2 = listOfManurestores[i];
            amanurestore2.CheckManureStoreNBalance();
            manureN2Emission = amanurestore2.GettotalNstoreN2();
            manureN2OEmission = amanurestore2.GettotalNstoreN20();
            manureNH3Emission = amanurestore2.GettotalNstoreNH3();
            manurestoreNLoss += manureN2Emission + manureN2OEmission + manureNH3Emission;
            runoffN += amanurestore2.GetrunoffN();
        }
        NLost += manurestoreNLoss;

        double startsoilMineralN = 0;
        for (int rotationID = minRotation; rotationID <= maxRotation; rotationID++)
        {
            rotationList[rotationID - 1].CheckRotationNBalance();
            NDeltaSoil += (rotationList[rotationID - 1].GetNStored() - rotationList[rotationID - 1].GetinitialSoilN()) / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            fieldN2Emission += rotationList[rotationID - 1].GetN2NEmission() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            fertiliserN2OEmission += rotationList[rotationID - 1].GetfertiliserN2ONEmissions() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            cropResidueN2O += rotationList[rotationID - 1].GetcropResidueN2ON() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            fieldN2OEmission += rotationList[rotationID - 1].GetN2ONemission() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            fertNH3NEmission += rotationList[rotationID - 1].GetFertNH3NEmission() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            fieldmanureNH3Emission += rotationList[rotationID - 1].GetManureNH3NEmission() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            Nleaching += rotationList[rotationID - 1].GettheNitrateLeaching() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            organicNLeached += rotationList[rotationID - 1].GetOrganicNLeached() / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
            residualSoilMineralN += rotationList[rotationID - 1].GetResidualSoilMineralN();
            startsoilMineralN += rotationList[rotationID - 1].GetstartsoilMineralN();
            // No processStorageNloss;
        }
        fieldNLoss = fertNH3NEmission + fieldmanureNH3Emission + fieldN2Emission + fieldN2OEmission + Nleaching + organicNLeached;
        NLost += fieldNLoss;
        double totalFarmNSurplus = Ninput - NExport; //1.137
        double changeInMinN=(residualSoilMineralN-startsoilMineralN) / GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();

        Nbalance = Ninput - (NExport + NLost + NDeltaSoil + changeInMinN);//1.117
        if (Ninput > 0)
            diff = Nbalance / Ninput;
        else
            diff = Nbalance;
        if (Math.Abs(diff) > tolerance)
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
            Console.WriteLine(outstring1);
            Console.WriteLine(outstring2);
            Console.Write(outstring3);
            if (GlobalVars.Instance.getPauseBeforeExit() && rotationList.Count != 0)
                Console.Read();
            if ((rotationList.Count != 0)&&(GlobalVars.Instance.getstopOnError()))
            {
                WriteFarmBalances(writer, tabFile);
                writer.WriteEndDocument();
                writer.Close();
                tabFile.Close();
                Environment.Exit(0);
            }
            else
            {
                Console.Write("there is no soil");
                Console.Read();
            }

        }
        double Arot = 0;
        for (int i = 0; i < rotationList.Count; i++)
        {
            Arot = rotationList[i].getArea();
        }
        double Nsurp = totalFarmNSurplus / Arot;//1,138


        //do GHG budget
        entericCH4CO2Eq = CliveCH4 * GlobalVars.Instance.GetCO2EqCH4();
        manureCH4CO2Eq = manurestoreCH4C * GlobalVars.Instance.GetCO2EqCH4();
        manureN2OCO2Eq = manureN2OEmission * GlobalVars.Instance.GetCO2EqN2O();
        fieldN2OCO2Eq = fieldN2OEmission * GlobalVars.Instance.GetCO2EqN2O();
        directGHGEmissionCO2Eq = entericCH4CO2Eq + manureCH4CO2Eq + manureN2OCO2Eq + fieldN2OCO2Eq + CDeltaSoil;

        housingNH3CO2Eq = housingNLoss * GlobalVars.Instance.GetIndirectNH3N2OFactor() * GlobalVars.Instance.GetCO2EqN2O();
        manurestoreNH3CO2Eq = manureNH3Emission * GlobalVars.Instance.GetIndirectNH3N2OFactor() * GlobalVars.Instance.GetCO2EqN2O();
        fieldmanureNH3CO2Eq = fieldmanureNH3Emission * GlobalVars.Instance.GetIndirectNH3N2OFactor() * GlobalVars.Instance.GetCO2EqN2O();
        fieldfertNH3CO2Eq = fertNH3NEmission * GlobalVars.Instance.GetIndirectNH3N2OFactor() * GlobalVars.Instance.GetCO2EqN2O();
        leachedNCO2Eq = Nleaching * GlobalVars.Instance.GetIndirectNO3N2OFactor() * GlobalVars.Instance.GetCO2EqN2O();
        indirectGHGCO2Eq = housingNH3CO2Eq + manurestoreNH3CO2Eq + fieldmanureNH3CO2Eq + fieldfertNH3CO2Eq + leachedNCO2Eq;
        WriteFarmBalances(writer, tabFile);
    }
    public void WriteFarmBalances(XmlWriter writer, System.IO.StreamWriter tabFile)
        {
        //writing output.

        writer.WriteStartElement("FarmBalance");
        writer.WriteStartElement("FarmCBalance");

        writer.WriteStartElement("carbonFromPlants");
        writer.WriteStartElement("Value");
        writer.WriteValue(carbonFromPlants);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C fixed from atmosphere");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Cmanimp");
        writer.WriteStartElement("Value");
        writer.WriteValue(Cmanimp);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in imported manure");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CPlantProductImported");
        writer.WriteStartElement("Value");
        writer.WriteValue(CPlantProductImported);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in imported feed");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CbeddingReq");
        writer.WriteStartElement("Value");
        writer.WriteValue(CbeddingReq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in imported bedding");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Cmilk");
        writer.WriteStartElement("Value");
        writer.WriteValue(Cmilk);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in exported milk");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Cmeat");
        writer.WriteStartElement("Value");
        writer.WriteValue(Cmeat);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in exported meat");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Csold");
        writer.WriteStartElement("Value");
        writer.WriteValue(Csold);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in crop products sold");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CLost");
        writer.WriteStartElement("Value");
        writer.WriteValue(CLost);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C lost to the environment");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("livestockCH4C");
        writer.WriteStartElement("Value");
        writer.WriteValue(livestockCH4C);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in enteric methane emissions");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("livestockCO2C");
        writer.WriteStartElement("Value");
        writer.WriteValue(livestockCO2C);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in CO2 emitted by livestock");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("housingCLoss");
        writer.WriteStartElement("Value");
        writer.WriteValue(housingCLoss);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in CO2 emitted from animal housing");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("manurestoreCH4C");
        writer.WriteStartElement("Value");
        writer.WriteValue(manurestoreCH4C);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in methane emitted by manure");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("manurestoreCO2C");
        writer.WriteStartElement("Value");
        writer.WriteValue(manurestoreCO2C);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in CO2 emitted by manure");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("processStorageCloss");
        writer.WriteStartElement("Value");
        writer.WriteValue(processStorageCloss);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in CO2 lost from stored crop products");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("soilCO2Emission");
        writer.WriteStartElement("Value");
        writer.WriteValue(soilCO2Emission);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("C in CO2 emitted by the soil");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CDeltaSoil");
        writer.WriteStartElement("Value");
        writer.WriteValue(CDeltaSoil);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Change in C stored in the soil");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Cbalance");
        writer.WriteStartElement("Value");
        writer.WriteValue(Cbalance);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Net C balance (should be zero)");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteEndElement();

        writer.WriteStartElement("FarmNBalance");
        writer.WriteStartElement("Nmanimp");
        writer.WriteStartElement("Value");
        writer.WriteValue(Nmanimp);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N in imported manure");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("nFix");
        writer.WriteStartElement("Value");
        writer.WriteValue(nFix);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N fixation");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Natm");
        writer.WriteStartElement("Value");
        writer.WriteValue(Natm);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N deposited from atmosphere");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("nFert");
        writer.WriteStartElement("Value");
        writer.WriteValue(nFert);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N in fertiliser");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Nbedding");
        writer.WriteStartElement("Value");
        writer.WriteValue(Nbedding);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N in imported bedding");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NPlantProductImported");
        writer.WriteStartElement("Value");
        writer.WriteValue(NPlantProductImported);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N in imported crop products");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Nsold");
        writer.WriteStartElement("Value");
        writer.WriteValue(Nsold);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N sold in crop products");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Nmilk");
        writer.WriteStartElement("Value");
        writer.WriteValue(Nmilk);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N exported in milk");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NGrowth");
        writer.WriteStartElement("Value");
        writer.WriteValue(NGrowth);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N exported in meat");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Nmanexp");
        writer.WriteStartElement("Value");
        writer.WriteValue(Nmanexp);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N in exported manure");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NExport");
        writer.WriteStartElement("Value");
        writer.WriteValue(NExport);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Total amount of N exported");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("housingNLoss");
        writer.WriteStartElement("Value");
        writer.WriteValue(housingNLoss);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N lost in NH3 emission from housing");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("manureN2Emission");
        writer.WriteStartElement("Value");
        writer.WriteValue(manureN2Emission);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N lost in N2 emission from manure storage");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("manureN2OEmission");
        writer.WriteStartElement("Value");
        writer.WriteValue(manureN2OEmission);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N lost in N2O emission from manure storage");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("manureNH3Emission");
        writer.WriteStartElement("Value");
        writer.WriteValue(manureNH3Emission);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N lost in NH3 emission from manure storage");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("runoffN");
        writer.WriteStartElement("Value");
        writer.WriteValue(runoffN);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N lost in runoff from manure storage");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("fieldN2Emission");
        writer.WriteStartElement("Value");
        writer.WriteValue(fieldN2Emission);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Emission of N2 from the field");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("fieldN2OEmission");
        writer.WriteStartElement("Value");
        writer.WriteValue(fieldN2OEmission);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Emission of N2O from the field");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("fertNH3NEmission");
        writer.WriteStartElement("Value");
        writer.WriteValue(fertNH3NEmission);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N lost via NH3 emission from fertilisers");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("fieldmanureNH3Emission");
        writer.WriteStartElement("Value");
        writer.WriteValue(fieldmanureNH3Emission);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N lost as NH3 from field-applied manure");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Nleaching");
        writer.WriteStartElement("Value");
        writer.WriteValue(Nleaching);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N lost via NO3 leaching from soil");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("organicNLeached");
        writer.WriteStartElement("Value");
        writer.WriteValue(organicNLeached);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N lost via leaching of organic N from soil");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg C");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NDeltaSoil");
        writer.WriteStartElement("Value");
        writer.WriteValue(NDeltaSoil);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Change in N stored in soil");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Nbalance");
        writer.WriteStartElement("Value");
        writer.WriteValue(Nbalance);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N balance (should be zero)");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg N");
        writer.WriteEndElement();
        writer.WriteEndElement();
        
        writer.WriteEndElement();


        writer.WriteStartElement("FarmDirectGHG");
        writer.WriteStartElement("entericCH4CO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(entericCH4CO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Enteric methane emissions");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("manureCH4CO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(manureCH4CO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Manure methane emissions");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("manureN2OCO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(manureN2OCO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Manure N2O emissions");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("fieldN2OCO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(fieldN2OCO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Field N2O emissions");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CDeltaSoil");
        writer.WriteStartElement("Value");
        writer.WriteValue(CDeltaSoil);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Change in C stored in soil");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("directGHGEmissionCO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(directGHGEmissionCO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Total GHG emissions");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteEndElement();

        writer.WriteStartElement("FarmIndirectGHG");
        writer.WriteStartElement("housingNH3CO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(housingNH3CO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Housing NH3 emissions");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("manurestoreNH3CO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(manurestoreNH3CO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Manure storage NH3 emissions");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();
        
        writer.WriteStartElement("fieldmanureNH3CO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(fieldmanureNH3CO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("NH3 emissions from field-applied manure");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();
        
        writer.WriteStartElement("fieldfertNH3CO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(fieldfertNH3CO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("NH3 emissions from fertilisers");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();
        
        writer.WriteStartElement("leachedNCO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(leachedNCO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("N2O emissions resulting from leaching of N");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();
        
        writer.WriteStartElement("indirectGHGCO2Eq");
        writer.WriteStartElement("Value");
        writer.WriteValue(indirectGHGCO2Eq);
        writer.WriteEndElement();
        writer.WriteStartElement("Description");
        writer.WriteValue("Total indirect emissions");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg CO2 equivalents");
        writer.WriteEndElement();
        writer.WriteEndElement();
        writer.WriteEndElement();


        writer.WriteEndElement();

            tabFile.Write("FarmBalance"+"\t");
        tabFile.Write("FarmCBalance"+"\t");

        tabFile.Write("carbonFromPlants"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(carbonFromPlants.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C fixed from atmosphere"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("Cmanimp"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Cmanimp.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in imported manure"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("CPlantProductImported"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(CPlantProductImported.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in imported feed"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("CbeddingReq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(CbeddingReq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in imported bedding"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
                

        tabFile.Write("Cmilk"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Cmilk.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in exported milk"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("Cmeat"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Cmeat.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in exported meat"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("Csold"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Csold.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in crop products sold"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("CLost"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(CLost.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C lost to the environment"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("livestockCH4C"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(livestockCH4C.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in enteric methane emissions"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("livestockCO2C"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(livestockCO2C.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in CO2 emitted by livestock"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("housingCLoss"+"\t");
        tabFile.Write("Value"+"\t");
        tabFile.Write(housingCLoss + "\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in CO2 emitted from animal housing"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("manurestoreCH4C"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(manurestoreCH4C.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in methane emitted by manure"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("manurestoreCO2C"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(manurestoreCO2C.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in CO2 emitted by manure"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("processStorageCloss"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(processStorageCloss.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in CO2 lost from stored crop products"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("soilCO2Emission"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(soilCO2Emission.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("C in CO2 emitted by the soil"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("CDeltaSoil"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(CDeltaSoil.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Change in C stored in the soil"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("Cbalance"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Cbalance.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Net C balance (should be zero)"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        

        tabFile.Write("FarmNBalance"+"\t");
        tabFile.Write("Nmanimp"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Nmanimp.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N in imported manure"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("nFix"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(nFix.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N fixation"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("Natm"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Natm.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N deposited from atmosphere"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("nFert"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(nFert.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N in fertiliser"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("Nbedding"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Nbedding.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N in imported bedding"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("NPlantProductImported"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(NPlantProductImported.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N in imported crop products"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("Nsold"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Nsold.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N sold in crop products"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("Nmilk"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Nmilk.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N exported in milk"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("NGrowth"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(NGrowth.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N exported in meat"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("Nmanexp"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Nmanexp.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N in exported manure"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("NExport"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(NExport.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Total amount of N exported"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("housingNLoss"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(housingNLoss.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N lost in NH3 emission from housing"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("manureN2Emission"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(manureN2Emission.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N lost in N2 emission from manure storage"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("manureN2OEmission"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(manureN2OEmission.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N lost in N2O emission from manure storage"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("manureNH3Emission"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(manureNH3Emission.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N lost in NH3 emission from manure storage"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("runoffN"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(runoffN.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N lost in runoff from manure storage"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("fieldN2Emission"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(fieldN2Emission.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Emission of N2 from the field"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("fieldN2OEmission"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(fieldN2OEmission.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Emission of N2O from the field"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("fertNH3NEmission"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(fertNH3NEmission.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N lost via NH3 emission from fertilisers"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("fieldmanureNH3Emission"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(fieldmanureNH3Emission.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N lost as NH3 from field-applied manure"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("Nleaching"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Nleaching.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N lost via NO3 leaching from soil"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("organicNLeached"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(organicNLeached.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N lost via leaching of organic N from soil"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg C"+"\t");
        
        

        tabFile.Write("NDeltaSoil"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(NDeltaSoil.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Change in N stored in soil"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        

        tabFile.Write("Nbalance"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(Nbalance.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N balance (should be zero)"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg N"+"\t");
        
        
        
        


        tabFile.Write("FarmDirectGHG"+"\t");
        tabFile.Write("entericCH4CO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(entericCH4CO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Enteric methane emissions"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        

        tabFile.Write("manureCH4CO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(manureCH4CO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Manure methane emissions"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        

        tabFile.Write("manureN2OCO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(manureN2OCO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Manure N2O emissions"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        

        tabFile.Write("fieldN2OCO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(fieldN2OCO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Field N2O emissions"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        

        tabFile.Write("CDeltaSoil"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(CDeltaSoil.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Change in C stored in soil"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        

        tabFile.Write("directGHGEmissionCO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(directGHGEmissionCO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Total GHG emissions"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        

        

        tabFile.Write("FarmIndirectGHG"+"\t");
        tabFile.Write("housingNH3CO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(housingNH3CO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Housing NH3 emissions"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        

        tabFile.Write("manurestoreNH3CO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(manurestoreNH3CO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Manure storage NH3 emissions"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        
        
        tabFile.Write("fieldmanureNH3CO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(fieldmanureNH3CO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("NH3 emissions from field-applied manure"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        
        
        tabFile.Write("fieldfertNH3CO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(fieldfertNH3CO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("NH3 emissions from fertilisers"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        
        
        tabFile.Write("leachedNCO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(leachedNCO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("N2O emissions resulting from leaching of N"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        
        
        tabFile.Write("indirectGHGCO2Eq"+"\t");
        tabFile.Write("Value"+"\t");
         tabFile.Write(indirectGHGCO2Eq.ToString()+"\n");
        
        tabFile.Write("Description"+"\t");
         tabFile.Write("Total indirect emissions"+"\t");
        
        tabFile.Write("Units"+"\t");
         tabFile.Write("kg CO2 equivalents"+"\t");
        
        
        
    }
}