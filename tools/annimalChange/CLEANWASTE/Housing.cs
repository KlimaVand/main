using System;
using System.Collections.Generic;
using System.Xml;
public class housing
{
    //inputs
    int HousingType;
    string Name;
    double propTime;
    //parameters
    
    double feedWasteFactor;
    double HousingRefTemp;
    double EFNH3ref;
    
    //other variables
    double beddingDM;
    double Cinput;
    double CO2C;
    double CtoStorage;
    double FibreCToStorage;
    double propTimeThisHouse;
    double DBedding;
    double DWaste;
    double meanTemp;
    double nTanin;
    double timeOnPasture;
    double beddingC;
    livestock theLivestock;
    double NTanInstore;
    double proportionDegradable; 
    double proportionNondegradable;
    double proportionTAN;
    double feedWasteC=0;
    double NWasteFeed = 0;
    double Nbedding = 0;
    double NtanInhouse = 0;
    double NorgInHouse = 0;
    double faecalN = 0;
    double NNH3housing = 0;
    double Ninput = 0;
    double Nout = 0;
    double NLost = 0;
    double Nbalance = 0;

    double[] TANtoThisStore = new double[2];
    double[] organicNtoThisStore = new double[2];

    public void SetproportionDegradable(double aValue) { proportionDegradable = aValue; }
    public void SetproportionNondegradable(double aValue) { proportionNondegradable = aValue; }
    public void SetproportionTAN(double aValue) { proportionTAN = aValue; }
    public double GetproportionDegradable() { return proportionDegradable; }
    public double GetproportionNondegradable() { return proportionNondegradable; }
    public double GetproportionTAN() { return proportionTAN; }
    public void SetName(string aName) { Name = aName; }
    public double getPropTimeThisHouse() { return propTimeThisHouse; }
    public string GetName() { return Name; }
    public double getDbedding(){return DBedding;}
    public double getUrineC() { return theLivestock.GeturineC(); }
    public double getFaecesC() { return theLivestock.GetfaecalC(); }
    public double getUrineN() { return NtanInhouse; }
    public double getFaecesN() { return faecalN; }
    public double getBeddingC() { return beddingC; }
    public double getBeddingN() { return Nbedding; }
    public double getFeedWasteC() { return feedWasteC; }
    public double GetCO2C() { return CO2C; }
    public double GetNNH3housing() { return NNH3housing; }
    public double getManureCarbon()
    {
        double returnVal = 0;
        for (int i = 0; i < manurestoreDetails.Count; i++)
        {
            returnVal += manurestoreDetails[i].manureToStorage.GetTotalC();
        }
        return returnVal;
    }
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
    private housing()
    {
    }
    public housing(int aHousingType, livestock aLivestock, int houseIndex, int zoneNr)
    {
        theLivestock = aLivestock;
        timeOnPasture = aLivestock.GettimeOnPasture();
        HousingType = aHousingType;
        FileInformation paramFile = new FileInformation(GlobalVars.Instance.getParamFilePath());

        paramFile.setPath("AgroecologicalZone("+zoneNr.ToString()+").Housing");
        int minHouse = 99, maxHouse = 0;
        paramFile.getSectionNumber(ref minHouse, ref maxHouse);
        int tmpHousingType = -1;
        paramFile.Identity.Add(0);
        bool found = false;
        int num = 0;
        for (int i = minHouse; i <= maxHouse; i++)
        {
            paramFile.Identity[paramFile.Identity.Count-1]=i;
            tmpHousingType = paramFile.getItemInt("HousingType");
            if (tmpHousingType == HousingType)
            {
                found = true;
                num = i;
                break;
            }
        }
        if (found ==false)
        {
          System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.WriteLine("could not link houses");
            file.WriteLine("program terminated");
            file.Close();
            Console.WriteLine("could not link houses");
            Console.WriteLine("program terminated");
            if (GlobalVars.Instance.getPauseBeforeExit())
                Console.Read();
            Environment.Exit(0);
        }
        Name = paramFile.getItemString("Name");
        paramFile.PathNames.Add("feedWasteFactor");
        paramFile.Identity.Add(-1);
        feedWasteFactor = paramFile.getItemDouble("Value");
        /*paramFile.PathNames[paramFile.PathNames.Count - 1] = "meanTemp";
        meanTemp = paramFile.getItemDouble("Value");*/
        paramFile.PathNames[paramFile.PathNames.Count - 1] = "HousingRefTemp";
        HousingRefTemp = paramFile.getItemDouble("Value");
        paramFile.PathNames[paramFile.PathNames.Count - 1] = "EFNH3housingRef";
        EFNH3ref = paramFile.getItemDouble("Value");
        paramFile.PathNames[paramFile.PathNames.Count - 1] = "ProportionDegradable";
        proportionDegradable = paramFile.getItemDouble("Value");
        paramFile.PathNames[paramFile.PathNames.Count - 1] = "ProportionNondegradable";
        proportionNondegradable = paramFile.getItemDouble("Value");
        paramFile.PathNames[paramFile.PathNames.Count - 1] = "ProportionTAN";
        proportionTAN = paramFile.getItemDouble("Value");
        GlobalVars.manurestoreRecord amanurestoreRecord = new GlobalVars.manurestoreRecord();
        int numManureStores = theLivestock.GethousingDetails()[houseIndex].GetManureRecipient().Count;
        for (int j = 0; j < numManureStores; j++)
        {
            manureStore aStore = new manureStore(theLivestock.GethousingDetails()[houseIndex].GetManureRecipient()[j].GetStorageType(), theLivestock.GetspeciesGroup(), zoneNr);
            amanurestoreRecord.SettheStore(aStore);
            manure amanureToStore = new manure();
            amanurestoreRecord.SetmanureToStorage(amanureToStore);
            manurestoreDetails.Add(amanurestoreRecord);
        }
        feedInHouse = aLivestock.GetfeedRation();
    }
    public void SettheLivestock(livestock someLivestock)
    {
        theLivestock=someLivestock;
    }
    public double GetCtoStorage() { return CtoStorage; }

    public void DoHousing()
    {
        propTimeThisHouse = 0;
        for (int i = 0; i < theLivestock.GethousingDetails().Count; i++)
        {
            if (theLivestock.GethousingDetails()[i].GetHousingType() == HousingType)
                propTimeThisHouse = theLivestock.GethousingDetails()[i].GetpropTime();
        }
        DBedding = propTimeThisHouse * (1 - theLivestock.GettimeOnPasture()) * theLivestock.GetAvgNumberOfAnimal()
            * GlobalVars.avgNumberOfDays * beddingDM;//1.28  
        GlobalVars.Instance.GetthebeddingMaterial().Setamount(DBedding);

        double amount = 0;
        for (int i = 0; i < theLivestock.GetfeedRation().Count; i++)
        {
            if (!theLivestock.GetfeedRation()[i].GetfedAtPasture())
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
        for (int i = 0; i < manurestoreDetails.Count; i++)
        {
            GlobalVars.manurestoreRecord amanurestoreRecord = manurestoreDetails[i];
            amanurestoreRecord.GettheStore().Addmanure(amanurestoreRecord.GetmanureToStorage(), amanurestoreRecord.GetpropYearGrazing());
        }
    }


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
    void registerFeedWaste()
    {
        for (int i = 0; i < GlobalVars.Instance.getmaxNumberFeedItems(); i++)
        {
            for (int j = 0; j < feedInHouse.Count; j++)
                if (feedInHouse[j].GetFeedCode() == i)
                {
                    feedItem afeedItem = new feedItem();
                    afeedItem.setFeedCode(i);
                    afeedItem.AddFeedItem(feedInHouse[j], false);
                    afeedItem.Setamount(feedWasteFactor * theLivestock.GetAvgNumberOfAnimal() * GlobalVars.Instance.GetavgNumberOfDays() 
                        * feedInHouse[j].Getamount());
                    GlobalVars.Instance.allFeedAndProductsUsed[i].composition.AddFeedItem(afeedItem, false);
                    break;
                }
        }
    }

    void DoCarbon()
    {
        if (theLivestock.GettimeOnPasture() < 1.0)
        {
            double amount = 0;
            double C_conc = 0;
            double fibre_conc = 0;
            for (int i = 0; i < feedInHouse.Count; i++)
            {
                amount += feedInHouse[i].Getamount();
                C_conc += feedInHouse[i].GetC_conc() * feedInHouse[i].Getamount();
                fibre_conc += feedInHouse[i].Getfibre_conc() * feedInHouse[i].Getamount();
            }
            C_conc /= amount;
            fibre_conc /= amount;
            beddingC = DBedding * GlobalVars.Instance.GetthebeddingMaterial().GetC_conc();
            //feedWasteFactor = 0;
            registerFeedWaste();
            feedWasteC = feedWasteFactor * GlobalVars.avgNumberOfDays * theLivestock.GetAvgNumberOfAnimal() *
                (1 - theLivestock.GettimeOnPasture()) * amount * C_conc / (1 - theLivestock.GettimeOnPasture());
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
                    System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                    file.WriteLine("Error - No manure storage destinations");
                    file.Close();
                    Console.WriteLine("Error - no manure storage destinations");
                    if (GlobalVars.Instance.getPauseBeforeExit())
                        Console.Read();
                    Environment.Exit(0);
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
                    for (int i = 0; i < manurestoreDetails.Count; i++)
                    {
                        amanurestoreRecord = manurestoreDetails[i];
                        if (amanurestoreRecord.GettheStore().GetStoresSolid())
                        {
                            nonDegC = proportionNondegradable * FibreCToStorage;
                            DegC = proportionDegradable * (CtoStorage - FibreCToStorage);
                        }
                        else
                        {
                            nonDegC = (1 - proportionNondegradable) * FibreCToStorage;
                            DegC = (1 - proportionDegradable) * (CtoStorage - FibreCToStorage);
                        }
                        amanurestoreRecord.GetmanureToStorage().SetdegC(DegC);
                        amanurestoreRecord.GetmanureToStorage().SetnonDegC(nonDegC);
                        amanurestoreRecord.SetpropYearGrazing(theLivestock.GettimeOnPasture());
                        //send C to manure store
                    }
                    break;
                default:
                    System.IO.StreamWriter afile = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                    afile.WriteLine("Error - too many manure storage destinations");
                    afile.Close();
                    Console.WriteLine("Error - too many manure storage destinations");
                    if (GlobalVars.Instance.getPauseBeforeExit())
                        Console.Read();
                    Environment.Exit(0);
                    break;
            }
        }
    }
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
                amount += feedInHouse[i].Getamount();
                N_conc += feedInHouse[i].GetN_conc() * feedInHouse[i].Getamount();
            }
            N_conc /= amount;
            NWasteFeed = propTimeThisHouse * feedWasteFactor * GlobalVars.avgNumberOfDays
                * theLivestock.GetAvgNumberOfAnimal() * (1 - theLivestock.GettimeOnPasture()) * amount * N_conc / (1 - theLivestock.GettimeOnPasture());

            Nbedding = DBedding * GlobalVars.Instance.GetthebeddingMaterial().GetN_conc();
            faecalN = propTimeThisHouse * (1 - theLivestock.timeOnPasture) * theLivestock.GetfaecalN() * theLivestock.GetAvgNumberOfAnimal();
            NorgInHouse = Nbedding + NWasteFeed + faecalN;
            Ninput = NorgInHouse + NtanInhouse;

            if (GlobalVars.Instance.getcurrentInventorySystem() == 1)//For IPCC, NH3 emissions from housing are included in manure storage emissions
            {
                NNH3housing = 0;
                NTanInstore = NtanInhouse;
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
                    System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                    file.WriteLine("Error - too many manure storage destinations");
                    file.Close();
                    Console.WriteLine("Error - too many manure storage destinations");
                    if (GlobalVars.Instance.getPauseBeforeExit())
                        Console.Read();
                    Environment.Exit(0);
                    break;
            }
        }
    }
    public void Write(XmlWriter writer,System.IO.StreamWriter tabFile)
    {

        writer.WriteStartElement("Housing");


        writer.WriteStartElement("Name");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("Name");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(Name);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("HousingRefTemp");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("HousingRefTemp");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(HousingRefTemp);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Ninput");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("Ninput");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(Ninput);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NtanInhouse");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("NtanInhouse");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NtanInhouse);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NorgInHouse");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("NorgInHouse");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NorgInHouse);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("faecalN");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("faecalN");
        writer.WriteEndElement(); ;
        writer.WriteStartElement("Value");
        writer.WriteValue(faecalN);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Nbedding");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("Nbedding");
        writer.WriteEndElement(); 
        writer.WriteStartElement("Value");
        writer.WriteValue(Nbedding);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NWasteFeed");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("NWasteFeed");
        writer.WriteEndElement(); 
        writer.WriteStartElement("Value");
        writer.WriteValue(NWasteFeed);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("proportionDegradable");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("proportionDegradable");
        writer.WriteEndElement(); 
        writer.WriteStartElement("Value");
        writer.WriteValue(proportionDegradable);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("proportionNondegradable");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("proportionNondegradable");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(proportionNondegradable);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("proportionTAN");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("proportionTAN");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(proportionTAN);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("feedWasteFactor");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("feedWasteFactor");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(feedWasteFactor);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("beddingDM");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("beddingDM");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(beddingDM);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("feedWasteFactor");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("feedWasteFactor");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(feedWasteFactor);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NNH3housing");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("NNH3housing");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NNH3housing);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CO2C");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("CO2C");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(CO2C);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CtoStorage");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("CtoStorage");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(CtoStorage);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("FibreCToStorage");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("FibreCToStorage");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(FibreCToStorage);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("propTimeThisHouse");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("propTimeThisHouse");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(propTimeThisHouse);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("HousingType");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("HousingType");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(HousingType);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Cinput");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("Cinput");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(Cinput);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NTanInstore");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue("NTanInstore");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NTanInstore);
        writer.WriteEndElement();
        writer.WriteEndElement();

            for (int i = 0; i < 2; i++)
            {
                writer.WriteStartElement("TANtoThisStore");

                writer.WriteStartElement("Value");
                writer.WriteStartElement("Description");
                writer.WriteValue("??");
                writer.WriteEndElement();
                writer.WriteStartElement("Units");
                writer.WriteValue("??");
                writer.WriteEndElement();
                writer.WriteStartElement("Name");
                writer.WriteValue("id");
                writer.WriteEndElement();
                writer.WriteStartElement("Value");
                writer.WriteValue(i);
                writer.WriteEndElement();
                writer.WriteEndElement();

                writer.WriteStartElement("TANtoThisStore");
                writer.WriteStartElement("Description");
                writer.WriteValue("??");
                writer.WriteEndElement();
                writer.WriteStartElement("Units");
                writer.WriteValue("??");
                writer.WriteEndElement();
                writer.WriteStartElement("Name");
                writer.WriteValue("TANtoThisStore");
                writer.WriteEndElement();
                writer.WriteStartElement("Value");
                writer.WriteValue(TANtoThisStore[i]);
                writer.WriteEndElement();
                writer.WriteEndElement();


                writer.WriteStartElement("id");
                writer.WriteStartElement("Description");
                writer.WriteValue("??");
                writer.WriteEndElement();
                writer.WriteStartElement("Units");
                writer.WriteValue("??");
                writer.WriteEndElement();
                writer.WriteStartElement("Name");
                writer.WriteValue("id");
                writer.WriteEndElement();
                writer.WriteStartElement("Value");
                writer.WriteValue(i);
                writer.WriteEndElement();
                writer.WriteEndElement();

                writer.WriteStartElement("organicNtoThisStore");
                writer.WriteStartElement("Description");
                writer.WriteValue("??");
                writer.WriteEndElement();
                writer.WriteStartElement("Units");
                writer.WriteValue("??");
                writer.WriteEndElement();
                writer.WriteStartElement("Name");
                writer.WriteValue("organicNtoThisStore");
                writer.WriteEndElement();

                writer.WriteStartElement("Value");
                writer.WriteValue(organicNtoThisStore[i]);
                writer.WriteEndElement();
                writer.WriteEndElement();

                writer.WriteEndElement();
            }


            writer.WriteStartElement("NLost");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Name");
            writer.WriteValue("NLost");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(NLost);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("Nout");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Name");
            writer.WriteValue("Nout");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Nout);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("Nbalance");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Name");
            writer.WriteValue("Nbalance");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Nbalance);
            writer.WriteEndElement();
            writer.WriteEndElement();

        tabFile.Write("Housing"+"\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Name" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(Name.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("HousingRefTemp" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(HousingRefTemp.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Ninput" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(Ninput.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NtanInhouse" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NtanInhouse.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NorgInHouse" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NorgInHouse.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("faecalN" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(faecalN.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Nbedding" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(Nbedding.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NWasteFeed" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NWasteFeed.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("proportionDegradable" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(proportionDegradable.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("proportionNondegradable" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(proportionNondegradable.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("proportionTAN" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(proportionTAN.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("feedWasteFactor" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(feedWasteFactor.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("beddingDM" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(beddingDM.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("feedWasteFactor" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(feedWasteFactor.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NNH3housing" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NNH3housing.ToString() + "\n");


        
        /*            tabFile.Write("Ninput");
                    tabFile.Write(Ninput);
            
                    tabFile.Write("Ninput");
                    tabFile.Write(Ninput);
            
                    tabFile.Write("Ninput");
                    tabFile.Write(Ninput);
            
                */
        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("CO2C" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(CO2C.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("CtoStorage" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(CtoStorage.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("FibreCToStorage" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(FibreCToStorage.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("propTimeThisHouse" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(propTimeThisHouse.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("HousingType" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(HousingType.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Cinput" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(Cinput.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NTanInstore" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NTanInstore.ToString() + "\n");

        

        for (int i = 0; i < 2; i++)
        {
            tabFile.Write("TANtoThisStore"+"\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Identity" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(i.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(TANtoThisStore[i].ToString() + "\n");

            

            
            

            tabFile.Write("organicNtoThisStore"+"\n");
            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Identity" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(i.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(organicNtoThisStore[i].ToString() + "\n");

            
            
        }

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NLost" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NLost.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Nout" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(Nout.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Nbalance" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(Nbalance.ToString() + "\n");


        
        for (int i = 0; i < manurestoreDetails.Count; i++)
        {
            manurestoreDetails[i].Write(writer, tabFile);
        }
        theLivestock.Write(writer, tabFile);
        for (int i = 0; i < feedInHouse.Count; i++)
        {
            feedInHouse[i].Write(writer, tabFile);
        }
        writer.WriteEndElement();

      
    }
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
                System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                file.WriteLine("Error; Housing C balance error is more than the permitted margin");
                file.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                file.Close();
                Console.WriteLine("Error; Housing C balance error is more than the permitted margin");
                Console.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                if (GlobalVars.Instance.getPauseBeforeExit())
                    Console.Read();
                Environment.Exit(0);
        }
        return retVal;
    }
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
                System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                file.WriteLine("Error; Housing N balance error is more than the permitted margin");
                file.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                file.Close();
                Console.WriteLine("Error; Housing N balance error is more than the permitted margin");
                Console.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                if (GlobalVars.Instance.getPauseBeforeExit())
                    Console.Read();
                Environment.Exit(0);
        }
        return retVal;
    }
  
}
