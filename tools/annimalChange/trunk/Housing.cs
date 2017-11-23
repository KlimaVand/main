using System;
using System.Collections.Generic;
using System.Xml;
public class housing
{
    //inputs
    int HousingType;
    string Name;
    //parameters
    
    double feedWasteFactor;
    double HousingRefTemp;
    double EFNH3ref;
    double EFNH3housingTier2;
    double beddingFactor;

    //other variables
    double beddingDM;
    double Cinput;
    double CO2C;
    double CtoStorage;
    double FibreCToStorage;
    double propTimeThisHouse;
    double meanTemp;
    double timeOnPasture;
    double beddingC;
    string parens;
    livestock theLivestock;
    double NTanInstore;
    double proportionDegradable; 
    double proportionNondegradable;
    double proportionTAN;
    double proportionP;
    double feedWasteC = 0;
    double NWasteFeed = 0;
    double Nbedding = 0;
    double NtanInhouse = 0;
    double NorgInHouse = 0;
    double faecalN = 0;
    double NNH3housing = 0;
    double NfedInHousing = 0;
    double Ninput = 0;
    double Nout = 0;
    double NLost = 0;
    double Nbalance = 0;
    feedItem wasteFeed = null;
    double waterInput;
    double washingWater;
    double spiltWater;
    double inputP;
    double Pbedding;
    double PWasteFeed;
    double Poutput;

    double operatingCosts;
    double variableOperatingCostFactor;
    double fixedOperatingCostFactor;
    double investmentCosts;
    double variableInvestmentCostFactor;
    double fixedInvestmentCostFactor;
    double lifetime;
    double annualInvestmentCosts;
/**
* get water Input
*/
    public double getwaterInput() { return waterInput; }
/**
* Set water Input
*/
    public void SetwaterInput(double newValue) { waterInput = newValue; }
/**
* get washing Water
*/
    public double getwashingWater() { return washingWater; }
/**
* Set washing Water
*/
    public void SetwashingWater(double newValue) { washingWater = newValue; }
/**
* get spilt Water
*/
    public double getspiltWater() { return spiltWater; }
/**
* Set spilt Water
*/
    public void SetspiltWater(double newValue) { spiltWater = newValue; }
/**
* get input P
*/
    public double getinputP() { return inputP; }
/**
* Set input P
*/
    public void SetinputP(double newValue) { inputP = newValue; }
/**
* get bedding
*/
    public double getPbedding() { return Pbedding; }
/**
* get spilt FeedP
*/
    public double getPWasteFeed() { return PWasteFeed; }
/**
* get output
*/
    public double getPoutput() { return Poutput; }
/**
* Do Water
*/
    public void DoWater(){}

    double[] TANtoThisStore = new double[2];
    double[] organicNtoThisStore = new double[2];
    double[] PtoThisStore = new double[2];
/**
        * Set proportion Degradable
        */
    public void SetproportionDegradable(double aValue) { proportionDegradable = aValue; }
/**
* Set proportion Non-degradable
*/
    public void SetproportionNondegradable(double aValue) { proportionNondegradable = aValue; }
/**
* Set proportion TAN
*/
    public void SetproportionTAN(double aValue) { proportionTAN = aValue; }
/**
* Get proportion Degradable
*/
    public double GetproportionDegradable() { return proportionDegradable; }
/**
* Get proportion Non-degradable
*/
    public double GetproportionNondegradable() { return proportionNondegradable; }
/**
* Get proportion TAN
*/
    public double GetproportionTAN() { return proportionTAN; }
/**
* Set Name
*/
    public void SetName(string aName) { Name = aName; }
/**
* get Prop Time This House
*/
    public double getPropTimeThisHouse() { return propTimeThisHouse; }
/**
* Get Name
*/
    public string GetName() { return Name; }
/**
* get bedding DM
*/
    public double getbeddingDM(){return beddingDM;}
/**
* get Urine C
*/
    public double getUrineC() { return theLivestock.GeturineC(); }
/**
* get Faeces C
*/
    public double getFaecesC() { return theLivestock.GetfaecalC(); }
/**
* get Urine N
*/
    public double getUrineN() { return NtanInhouse; }
/**
* get Faeces N
*/
    public double getFaecesN() { return faecalN; }
/**
* get Bedding C
*/
    public double getBeddingC() { return beddingC; }
/**
* get Bedding N
*/
    public double getBeddingN() { return Nbedding; }
/**
* get Feed Waste C
*/
    public double getFeedWasteC() { return feedWasteC; }
/**
* get Feed Waste N
*/
    public double getFeedWasteN() { return NWasteFeed; }
/**
* Get CO2C
*/
    public double GetCO2C() { return CO2C; }
/**
* Get NNH3 housing
*/
    public double GetNNH3housing() { return NNH3housing; }
/**
* Get N fed In Housing
*/
    public double GetNfedInHousing() { return NfedInHousing; }
/**
* Get N input In Excreta
*/
    public double GetNinputInExcreta() {return (NtanInhouse + faecalN);}
/**
* get Manure Carbon
*/
    public double getManureCarbon()
    {
        double returnVal = 0;
        for (int i = 0; i < manurestoreDetails.Count; i++)
        {
            returnVal += manurestoreDetails[i].manureToStorage.GetTotalC();
        }
        return returnVal;
    }
/**
* get Manure Nitrogen
*/
    public double getManureNitrogen()
    {
        double returnVal = 0;
        for (int i = 0; i < manurestoreDetails.Count; i++)
        {
            returnVal += manurestoreDetails[i].manureToStorage.GetTotalN();
        }
        return returnVal;
    }
    List<feedItem> feedInHouse;
    List<GlobalVars.manurestoreRecord> manurestoreDetails = new List<GlobalVars.manurestoreRecord>();
    public List<GlobalVars.manurestoreRecord> GetmanurestoreDetails() { return manurestoreDetails; }
    public livestock gettheLivestock() { return theLivestock; }
    public double getTimeOnPasture(){return timeOnPasture;}

    public double GetAnnualInvestmentCosts()
    {
        double retVal = 0;
        investmentCosts= fixedInvestmentCostFactor + variableInvestmentCostFactor * theLivestock.GetAvgNumberOfAnimal();
        double interestProportion = GlobalVars.Instance.theZoneData.GetinterestRate()/100;
        retVal = investmentCosts * (Math.Pow((1 + interestProportion), lifetime) * interestProportion) /
            (Math.Pow((1 + interestProportion), lifetime) - 1);
        return retVal;
    }

    public void CalcOperatingCosts()
    {
        operatingCosts = fixedOperatingCostFactor * investmentCosts + variableOperatingCostFactor * theLivestock.GetAvgNumberOfAnimal();
    }

    public void DoEconomics()
    {
        annualInvestmentCosts = GetAnnualInvestmentCosts();
        CalcOperatingCosts();
    }
/**
* Constructor 
*/
    private housing()
    {
    }
/**
* Constructor 
*/
    public housing(int aHousingType, livestock aLivestock, int houseIndex, int zoneNr, string aParens)
    {
        parens = aParens;
        theLivestock = aLivestock;
        timeOnPasture = aLivestock.GettimeOnPasture();
        HousingType = aHousingType;
        feedInHouse = new List<feedItem>();
        FileInformation paramFile = new FileInformation(GlobalVars.Instance.getParamFilePath());

        if (HousingType != 0)
        {
            string basePath = "AgroecologicalZone(" + zoneNr.ToString() + ").Housing";
            paramFile.setPath(basePath);
            int minHouse = 99, maxHouse = 0;
            paramFile.getSectionNumber(ref minHouse, ref maxHouse);
            int tmpHousingType = -1;

            bool found = false;
            int num = 0;
            for (int i = minHouse; i <= maxHouse; i++)
            {
                if (paramFile.doesIDExist(i))
                {
                    paramFile.Identity.Add(i);
                    tmpHousingType = paramFile.getItemInt("HousingType");
                    if (tmpHousingType == HousingType)
                    {
                        found = true;
                        num = i;
                        basePath += "(" + num.ToString() + ")";
                        break;
                    }
                    paramFile.Identity.RemoveAt(paramFile.Identity.Count - 1);
                }
            }
            if (found == false)
            {

                string messageString = aLivestock.Getname() + " could not link housing and manure storage";
                GlobalVars.Instance.Error(messageString);
            }
            Name = paramFile.getItemString("Name");
            paramFile.PathNames.Add("feedWasteFactor");
            paramFile.Identity.Add(-1);
            feedWasteFactor = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "beddingFactor";
            beddingFactor = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "HousingRefTemp";
            HousingRefTemp = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "EFNH3housingRef";
            EFNH3ref = paramFile.getItemDouble("Value");
            EFNH3housingTier2 = paramFile.getItemDouble("Value", basePath + ".EFNH3housingTier2(-1)");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "ProportionDegradable";
            proportionDegradable = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "ProportionNondegradable";
            proportionNondegradable = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "ProportionTAN";
            proportionTAN = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "ProportionP";
            proportionP = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "fixedInvestmentCostFactor";
            fixedInvestmentCostFactor = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "variableInvestmentCostFactor";
            variableInvestmentCostFactor = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "fixedOperatingCostFactor";
            fixedOperatingCostFactor = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "variableOperatingCostFactor";
            variableOperatingCostFactor = paramFile.getItemDouble("Value");
            paramFile.PathNames[paramFile.PathNames.Count - 1] = "lifetime";
            lifetime = paramFile.getItemDouble("Value");
            GlobalVars.manurestoreRecord amanurestoreRecord = new GlobalVars.manurestoreRecord();
            int numManureStores = theLivestock.GethousingDetails()[houseIndex].GetManureRecipient().Count;
            for (int j = 0; j < numManureStores; j++)
            {

                manureStore aStore = new manureStore(theLivestock.GethousingDetails()[houseIndex].GetManureRecipient()[j].GetStorageType(),
                    theLivestock.GetspeciesGroup(), zoneNr, parens + "_manureStore" + (j + 1).ToString());
                amanurestoreRecord.SettheStore(aStore);
                manure amanureToStore = new manure();
                amanurestoreRecord.SetmanureToStorage(amanureToStore);
                manurestoreDetails.Add(amanurestoreRecord);

            }
        }
        for (int i = 0; i < aLivestock.GetfeedRation().Count; i++)
        {
            feedItem aFeedItem = aLivestock.GetfeedRation()[i];
            if (!aFeedItem.GetisGrazed())
                feedInHouse.Add(aFeedItem);
            else feedInHouse.Add(null);
            //        feedInHouse = aLivestock.GetfeedRation();
        }
    }
/**
* Set theLivestock 
*/
    public void SettheLivestock(livestock someLivestock)
    {
        theLivestock=someLivestock;
    }
/**
* Get C to Storage
*/
    public double GetCtoStorage() { return CtoStorage; }
/**
* Do Housing
*/
    public void DoHousing()
    {
        propTimeThisHouse = 0;
        for (int i = 0; i < theLivestock.GethousingDetails().Count; i++)
        {
            if (theLivestock.GethousingDetails()[i].GetHousingType() == HousingType)
                propTimeThisHouse = theLivestock.GethousingDetails()[i].GetpropTime();
        }
        beddingDM = propTimeThisHouse * (1 - theLivestock.GettimeOnPasture()) * theLivestock.GetAvgNumberOfAnimal()
            * GlobalVars.avgNumberOfDays * beddingFactor;//1.28  
        //GlobalVars.Instance.GetthebeddingMaterial().Setamount(beddingDM);
        feedItem bedding = new feedItem(GlobalVars.Instance.GetthebeddingMaterial());
        bedding.Setamount(beddingDM);
        GlobalVars.Instance.allFeedAndProductsUsed[bedding.GetFeedCode()].composition.AddFeedItem(bedding, false, true);

        double amount = 0;
        for (int i = 0; i < theLivestock.GetfeedRation().Count; i++)
        {
            
            if (!theLivestock.GetfeedRation()[i].GetisGrazed())
            {
                feedInHouse[i].Setamount(propTimeThisHouse * theLivestock.GetfeedRation()[i].Getamount());
                feedInHouse[i].SetC_conc(theLivestock.GetfeedRation()[i].GetC_conc());
                feedInHouse[i].SetN_conc(theLivestock.GetfeedRation()[i].GetN_conc());
                amount += feedInHouse[i].Getamount();
            }
        }

        int daysOnPasture = (int) Math.Round(theLivestock.GettimeOnPasture() * 365);
        if ((daysOnPasture > 0) && (theLivestock.GethousedDuringGrazing() == false))
            meanTemp=GetMeanTemperature(daysOnPasture);
        else
            meanTemp=GetMeanTemperature(0);
        //DWaste = propTimeThisHouse *  amount * theLivestock.GetAvgNumberOfAnimal() * GlobalVars.avgNumberOfDays;
        DoCarbon();
        DoNitrogen();
        DoPhosphorus();
        for (int i = 0; i < manurestoreDetails.Count; i++)
        {
            GlobalVars.manurestoreRecord amanurestoreRecord = manurestoreDetails[i];
            amanurestoreRecord.GettheStore().Addmanure(amanurestoreRecord.GetmanureToStorage(), amanurestoreRecord.GetpropYearGrazing());
        }
        DoEconomics();
    }

/**
* Get Mean Temperature
*/
    private double GetMeanTemperature(int daysOnPasture)
    {
        int startDay=1;
        int endDay=365;
        double retVal = 0;
        if (daysOnPasture == 0)
            retVal = GlobalVars.Instance.GetDegreeDays(startDay, endDay, 0.0, GlobalVars.Instance.theZoneData.GetaverageAirTemperature(),
                GlobalVars.Instance.theZoneData.GetairtemperatureAmplitude(), GlobalVars.Instance.theZoneData.GetairtemperatureOffset());
        else
        {
            int midpoint = GlobalVars.Instance.theZoneData.GetgrazingMidpoint();
            //Jonas - this division will loose data
            int halfPoint = midpoint - daysOnPasture / 2;
            if (halfPoint < 0)
            {
                startDay = midpoint + daysOnPasture / 2;
                endDay = 365 - startDay;
                retVal = GlobalVars.Instance.GetDegreeDays(startDay, endDay, 0.0, GlobalVars.Instance.theZoneData.GetaverageAirTemperature(),
                    GlobalVars.Instance.theZoneData.GetairtemperatureAmplitude(), GlobalVars.Instance.theZoneData.GetairtemperatureOffset());
            }
            else
            {
                startDay = 1;
                endDay = midpoint - daysOnPasture / 2;
                retVal = GlobalVars.Instance.GetDegreeDays(startDay, endDay, 0.0, GlobalVars.Instance.theZoneData.GetaverageAirTemperature(),
                    GlobalVars.Instance.theZoneData.GetairtemperatureAmplitude(), GlobalVars.Instance.theZoneData.GetairtemperatureOffset());
                startDay = midpoint + daysOnPasture / 2;
                endDay = 365;
                retVal += GlobalVars.Instance.GetDegreeDays(startDay, endDay, 0.0, GlobalVars.Instance.theZoneData.GetaverageAirTemperature(),
                    GlobalVars.Instance.theZoneData.GetairtemperatureAmplitude(), GlobalVars.Instance.theZoneData.GetairtemperatureOffset());
            }
        }
        retVal /= (365 - daysOnPasture);

        return retVal;
    }
/**
* register Feed Waste
*/
    void registerFeedWaste()
    {
        wasteFeed = new feedItem();
        for (int i = 0; i < GlobalVars.Instance.getmaxNumberFeedItems(); i++)
        {
            for (int j = 0; j < feedInHouse.Count; j++)
                if(feedInHouse[j]!=null)
                if (feedInHouse[j].GetFeedCode() == i)
                {
                    feedItem afeedItem = new feedItem();
                    afeedItem.setFeedCode(i);
                    afeedItem.AddFeedItem(feedInHouse[j], false);
                    double amountConsumedPerYear=theLivestock.GetAvgNumberOfAnimal() * GlobalVars.Instance.GetavgNumberOfDays() 
                        * feedInHouse[j].Getamount();
                    double wastedAmountPerYear =feedWasteFactor * amountConsumedPerYear/(1-feedWasteFactor) ;
                    afeedItem.Setamount(wastedAmountPerYear);
                    GlobalVars.Instance.allFeedAndProductsUsed[i].composition.AddFeedItem(afeedItem, false);
                    wasteFeed.AddFeedItem(afeedItem,true,false);
                    break;
                }
        }
    }
/**
* Do Carbon
*/
    void DoCarbon()
    {
        if (theLivestock.GettimeOnPasture() < 1.0)
        {
            double amount = 0;
            double C_conc = 0;
            double fibre_conc = 0;
            for (int i = 0; i < feedInHouse.Count; i++)
            {
                if (feedInHouse[i] != null)
                {
                    amount += feedInHouse[i].Getamount();
                    C_conc += feedInHouse[i].GetC_conc() * feedInHouse[i].Getamount();
                    fibre_conc += feedInHouse[i].Getfibre_conc() * feedInHouse[i].Getamount();
                }
            }
            C_conc /= amount;
            fibre_conc /= amount;
            beddingC = beddingDM * GlobalVars.Instance.GetthebeddingMaterial().GetC_conc();
            //feedWasteFactor = 0;
            registerFeedWaste();
            feedWasteC = wasteFeed.Getamount() * wasteFeed.GetC_conc();
            double urineC = theLivestock.GeturineC() * theLivestock.GetAvgNumberOfAnimal();
            double faecalC = theLivestock.GetfaecalC() * theLivestock.GetAvgNumberOfAnimal();
            Cinput = propTimeThisHouse * (1 - theLivestock.GettimeOnPasture()) * (faecalC + urineC) + beddingC + feedWasteC;
            CO2C = propTimeThisHouse * (1 - theLivestock.GettimeOnPasture()) * urineC;
            CtoStorage = Cinput - CO2C;
            //        FibreCToStorage = (1 - theLivestock.GettimeOnPasture()) * theLivestock.GetCintake() * (theLivestock.GetFibre() / (theLivestock.GetDMintake()))
            //          + beddingC * GlobalVars.Instance.GetthebeddingMaterial().Getfibre_conc();
            FibreCToStorage = ((1 - theLivestock.GettimeOnPasture()) * theLivestock.GetCintake() + feedWasteC) * fibre_conc
                + beddingC * GlobalVars.Instance.GetthebeddingMaterial().Getfibre_conc();
            double nonDegC;
            double DegC;
            GlobalVars.manurestoreRecord amanurestoreRecord;
            switch (manurestoreDetails.Count)
            {
                case 0:
                    string messageString=("Error - No manure storage destinations");
                    GlobalVars.Instance.Error(messageString);
                    break;
                case 1:
                    amanurestoreRecord = manurestoreDetails[0];
                    nonDegC = FibreCToStorage;
                    DegC = CtoStorage - FibreCToStorage;
                    amanurestoreRecord.GetmanureToStorage().SetdegC(DegC);
                    amanurestoreRecord.GetmanureToStorage().SetnonDegC(nonDegC);
                    amanurestoreRecord.SetpropYearGrazing(theLivestock.GettimeOnPasture());
                    break;
                case 2:
                    bool gotSolid = false;
                    bool gotLiquid = false;
                    for (int i = 0; i < manurestoreDetails.Count; i++)
                    {
                        amanurestoreRecord = manurestoreDetails[i];
                        if (amanurestoreRecord.GettheStore().GetStoresSolid())
                        {
                            nonDegC = proportionNondegradable * FibreCToStorage;
                            DegC = proportionDegradable * (CtoStorage - FibreCToStorage);
                            if (gotSolid)
                            {
                                messageString = ("Error - two manure storage destinations receive solid manure");
                                GlobalVars.Instance.Error(messageString);
                                break;
                            }
                            else
                                gotSolid = true;
                        }
                        else
                        {
                            nonDegC = (1 - proportionNondegradable) * FibreCToStorage;
                            DegC = (1 - proportionDegradable) * (CtoStorage - FibreCToStorage);
                            if (gotLiquid)
                            {
                                messageString = ("Error - two manure storage destinations receive liquid manure");
                                GlobalVars.Instance.Error(messageString);
                                break;
                            }
                            else
                                gotLiquid = true;
                        }
                        amanurestoreRecord.GetmanureToStorage().SetdegC(DegC);
                        amanurestoreRecord.GetmanureToStorage().SetnonDegC(nonDegC);
                        amanurestoreRecord.SetpropYearGrazing(theLivestock.GettimeOnPasture());
                        //send C to manure store
                    }
                    break;
                default:
                    messageString=("Error - too manure storage destinations");
                    GlobalVars.Instance.Error(messageString);
                    break;
            }
        }
    }
/**
* Do Nitrogen
*/
    public void DoNitrogen()
    {
        if (theLivestock.GettimeOnPasture() < 1.0)
        {
            NtanInhouse = propTimeThisHouse * (1 - theLivestock.timeOnPasture) * theLivestock.GeturineN() * theLivestock.GetAvgNumberOfAnimal(); //1.37
            double amount = 0;
            double N_conc = 0;
            TANtoThisStore[0] = 0;
            organicNtoThisStore[0] = 0;
            TANtoThisStore[1] = 0;
            organicNtoThisStore[1] = 0;

            for (int i = 0; i < feedInHouse.Count; i++)
            {
                if (feedInHouse[i] != null)
                {
                    amount += feedInHouse[i].Getamount();
                    N_conc += feedInHouse[i].GetN_conc() * feedInHouse[i].Getamount();
                }
            }
            N_conc /= amount;
            NfedInHousing = N_conc * amount * GlobalVars.avgNumberOfDays * theLivestock.GetAvgNumberOfAnimal();
            NWasteFeed = wasteFeed.Getamount() * wasteFeed.GetN_conc();
            Nbedding = beddingDM * GlobalVars.Instance.GetthebeddingMaterial().GetN_conc();
            faecalN = propTimeThisHouse * (1 - theLivestock.timeOnPasture) * theLivestock.GetfaecalN() * theLivestock.GetAvgNumberOfAnimal();
            NorgInHouse = Nbedding + NWasteFeed + faecalN;
            Ninput = NorgInHouse + NtanInhouse;

            if (GlobalVars.Instance.getcurrentInventorySystem() == 1)//For IPCC, NH3 emissions from housing are included in manure storage emissions
            {
                NNH3housing = EFNH3housingTier2* NtanInhouse;
                NTanInstore = NtanInhouse - NNH3housing;
            }
            else
            {
                double KHtheta = Math.Pow(10, -1.69 + 1447.7 / (meanTemp + GlobalVars.absoluteTemp));
                double KHref = Math.Pow(10, -1.69 + 1447.7 / (HousingRefTemp + GlobalVars.absoluteTemp));

                double EFNH3theta = (KHref / KHtheta) * EFNH3ref;

                if (GlobalVars.Instance.getcurrentInventorySystem() == 2)
                    NNH3housing = propTimeThisHouse * (1 - theLivestock.timeOnPasture) * EFNH3theta * NtanInhouse;
                else
                    NNH3housing = propTimeThisHouse * (1 - theLivestock.timeOnPasture) * EFNH3theta * NtanInhouse;

                NTanInstore = NtanInhouse - NNH3housing;
            }
            GlobalVars.manurestoreRecord amanurestoreRecord;
            switch (manurestoreDetails.Count)
            {
                case 1:
                    amanurestoreRecord = manurestoreDetails[0];
                    TANtoThisStore[0] = NTanInstore;
                    organicNtoThisStore[0] = NorgInHouse;
                    amanurestoreRecord.GetmanureToStorage().SetTAN(TANtoThisStore[0]);
                    amanurestoreRecord.GetmanureToStorage().SetorganicN(organicNtoThisStore[0]);
                    break;
                case 2:
                    for (int i = 0; i < manurestoreDetails.Count; i++)
                    {
                        amanurestoreRecord = manurestoreDetails[i];
                        if (amanurestoreRecord.GettheStore().GetStoresSolid())
                        {
                            TANtoThisStore[i] = proportionTAN * NTanInstore;
                            organicNtoThisStore[i] = proportionDegradable * NorgInHouse;
                        }
                        else
                        {
                            TANtoThisStore[i] = (1 - proportionTAN) * NTanInstore;
                            organicNtoThisStore[i] = (1 - proportionDegradable) * NorgInHouse;
                        }
                        amanurestoreRecord.GetmanureToStorage().SetTAN(TANtoThisStore[i]);
                        amanurestoreRecord.GetmanureToStorage().SetorganicN(organicNtoThisStore[i]);
                    }
                    break;
                default:
                    string messageString=("Error - too many manure storage destinations");
                    
                    GlobalVars.Instance.Error(messageString);
                    break;
            }
        }
    }
/**
    * Do Nitrogen
    */
    public void DoPhosphorus()
    {
        if (theLivestock.GettimeOnPasture() < 1.0)
        {
            double amount = 0;
            double P_conc = 0;
            double PfedInHousing = 0;
            PtoThisStore[0] = 0;
            PtoThisStore[1] = 0;

            for (int i = 0; i < feedInHouse.Count; i++)
            {
                if (feedInHouse[i] != null)
                {
                    amount += feedInHouse[i].Getamount();
                    P_conc += feedInHouse[i].GetP_conc() * feedInHouse[i].Getamount();
                }
            }
            P_conc /= amount;
            PfedInHousing = P_conc * amount * GlobalVars.avgNumberOfDays * theLivestock.GetAvgNumberOfAnimal();
            PWasteFeed = wasteFeed.Getamount() * wasteFeed.GetP_conc();
            Pbedding = beddingDM * GlobalVars.Instance.GetthebeddingMaterial().GetP_conc();
            inputP = propTimeThisHouse * (1 - theLivestock.timeOnPasture) * theLivestock.getexcretaP() * theLivestock.GetAvgNumberOfAnimal(); //
            inputP+= Pbedding + PWasteFeed;
            Poutput = inputP;
            GlobalVars.manurestoreRecord amanurestoreRecord;
            switch (manurestoreDetails.Count)
            {
                case 1:
                    amanurestoreRecord = manurestoreDetails[0];
                    amanurestoreRecord.GetmanureToStorage().Setphosphorus(inputP);
                    break;
                case 2:
                    for (int i = 0; i < manurestoreDetails.Count; i++)
                    {
                        amanurestoreRecord = manurestoreDetails[i];
                        if (amanurestoreRecord.GettheStore().GetStoresSolid())
                            PtoThisStore[i] = proportionP * inputP;
                        else
                            PtoThisStore[i] = (1 - proportionP) * inputP;
                        amanurestoreRecord.GetmanureToStorage().Setphosphorus(PtoThisStore[i]);
                    }
                    break;
                default:
                    string messageString = ("Error - too many manure storage destinations");

                    GlobalVars.Instance.Error(messageString);
                    break;
            }
        }
    }
/**
    * Write
    */
    public void Write()
    {
        if (GlobalVars.Instance.getRunFullModel())
            theLivestock.Write();
        GlobalVars.Instance.writeStartTab("Housing");
        for (int i = 0; i < feedInHouse.Count; i++)
        {

            if (feedInHouse[i] != null)
            feedInHouse[i].Write(parens+"_");
        }

        GlobalVars.Instance.writeInformationToFiles("Name", "Name", "-", Name, parens);
        GlobalVars.Instance.writeInformationToFiles("HousingType", "Type of housing", "-", HousingType, parens);
        GlobalVars.Instance.writeInformationToFiles("waterInput", "waterInput", "-", waterInput, parens);
        GlobalVars.Instance.writeInformationToFiles("washingWater", "washingWater", "-", washingWater, parens);
        GlobalVars.Instance.writeInformationToFiles("spiltWater", "spiltWater", "-", spiltWater, parens);
        GlobalVars.Instance.writeInformationToFiles("inputP", "inputP", "kg", inputP, parens);
        GlobalVars.Instance.writeInformationToFiles("Pbedding", "Pbedding", "kg", Pbedding, parens);
        GlobalVars.Instance.writeInformationToFiles("spiltFeedP", "spiltFeedP", "kg", PWasteFeed, parens);
        GlobalVars.Instance.writeInformationToFiles("Poutput", "Poutput", "kg", Poutput, parens);

        GlobalVars.Instance.writeInformationToFiles("HousingType", "Type of housing", "-", HousingType, parens);

        GlobalVars.Instance.writeInformationToFiles("Cinput", "Total C input to housing", "kg", Cinput, parens);
        GlobalVars.Instance.writeInformationToFiles("CO2C", "CO2-C emitted from housing", "kg", CO2C, parens);
        GlobalVars.Instance.writeInformationToFiles("CtoStorage", "C sent to manure storage", "kg", CtoStorage, parens);
        //GlobalVars.Instance.writeInformationToFiles("HousingRefTemp", "Reference temperature for housing NH3 emissions", "Celsius", HousingRefTemp);

        GlobalVars.Instance.writeInformationToFiles("Ninput", "Total N entering the housing", "kg", Ninput, parens);
        GlobalVars.Instance.writeInformationToFiles("NtanInhouse", "TAN entering the housing", "kg", NtanInhouse, parens);
        GlobalVars.Instance.writeInformationToFiles("NorgInHouse", "Organic N entering the housing", "kg", NorgInHouse, parens);
        GlobalVars.Instance.writeInformationToFiles("faecalNInHouseing", "Faecal N deposited", "kg", faecalN, parens);
        GlobalVars.Instance.writeInformationToFiles("Nbedding", "N added in bedding", "kg", Nbedding, parens);
        GlobalVars.Instance.writeInformationToFiles("NWasteFeed", "N added in waste feed", "kg", NWasteFeed, parens);
        GlobalVars.Instance.writeInformationToFiles("NNH3housing", "NH3-N emitted from housing", "kg", NNH3housing, parens);
        GlobalVars.Instance.writeInformationToFiles("NLost", "Total N lost", "kg", NLost, parens);
        GlobalVars.Instance.writeInformationToFiles("Nout", "N leaving housing", "kg", Nout, parens);
        GlobalVars.Instance.writeInformationToFiles("operatingCosts", "operatingCosts", GlobalVars.Instance.theZoneData.Getcurrency(), operatingCosts, parens);
        GlobalVars.Instance.writeInformationToFiles("annualInvestmentCosts", "annualInvestmentCosts", GlobalVars.Instance.theZoneData.Getcurrency(), annualInvestmentCosts, parens);
        if (!GlobalVars.Instance.getRunFullModel())
            GlobalVars.Instance.writeEndTab();
    }
/**
* Check Housing C Balance
*/
    public bool CheckHousingCBalance()
    {
        bool retVal = false;
        double Cout = getManureCarbon();
        double CLost = CO2C;
        double Cbalance = Cinput - (Cout + CLost);
        double diff = Cbalance / Cinput;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
            double errorPercent = 100 * diff;
            string messageString=("Error; Housing C balance error is more than the permitted margin\n");
            messageString+=("Percentage error = " + errorPercent.ToString("0.00") + "%");
            GlobalVars.Instance.Error(messageString);
        }
        return retVal;
    }
/**
* Check Housing N Balance
*/
    public bool CheckHousingNBalance()
    {
        bool retVal = false;
        Nout = getManureNitrogen();
        NLost = NNH3housing;
        Nbalance = Ninput - (Nout + NLost);
        double diff = Nbalance / Ninput;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
                double errorPercent = 100 * diff;

                string messageString =("Error; Housing N balance error is more than the permitted margin\n");
                messageString+=("Percentage error = " + errorPercent.ToString("0.00") + "%");
                GlobalVars.Instance.Error(messageString);
        }
        return retVal;
    }
  
}
