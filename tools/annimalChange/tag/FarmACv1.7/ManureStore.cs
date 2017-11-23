using System;
using System.Collections.Generic;
using System.Xml;
public class manureStore
{
    string path;
    //inputs

    //parameters
    string name;
    int storageType;
    int speciesGroup;
    double b1;
    double lnArr;
    double StorageRefTemp;
    double meanTemp;
    double MCF; //needs to be AEZ specific
    double Bo;//needs to be AEZ specific
    double EFStoreNH3;
    double EFStoreN20;
    double EFStoreN2;
    double Lambda;
    double propGasCapture;

    //other variables
    string parens;
    int identity;
    manure theManure;
    housing theHousing;
    int manureTypeStored;
    double tstore=0;
    double CdegradationRate;
    double Cinput=0;
    double CCH4ST=0;
    double CCO2ST = 0;
    double CdegST;
    double NTanInstore;
    double NorgInstore;
    double NDegOrgOut;
    double ohmOrg;
    double ohmTAN;
    double NRunOffOrg;
    double NHUM;
    double NRunoffHum;
    double NorgOutStore;
    double NTanOutstore;
    double NTANLost;
    double NrunoffTan;
    double totalNstoreNH3;
    double totalNstoreN2;
    double totalNstoreN20;
    double Ninput = 0;
    double Nout = 0;
    double NLost = 0;
    double Nbalance = 0;
    double biogasCH4C = 0;
    double biogasCO2C = 0;

    public double GetCCH4ST() { return CCH4ST; }
    public double GetCCO2ST() { return CCO2ST; }
    public double GettotalNstoreNH3() { return totalNstoreNH3; }
    public double GettotalNstoreN2() { return totalNstoreN2; }
    public double GettotalNstoreN20() { return totalNstoreN20; }
    public double GetrunoffN() { return NRunoffHum + NRunOffOrg; }
    public double GetbiogasCH4C() { return biogasCH4C; }
    public double GetbiogasCO2C() { return biogasCO2C; }

    public double GetManureC()
    {
        double retVal = 0;
        retVal = theManure.GetdegC() + theManure.GethumicC() + theManure.GetnonDegC();
        return retVal;
    }
    public double GetManureN()
    {
        double retVal = 0;
        retVal = theManure.GetTAN()+ theManure.GetorganicN();
        return retVal;
    }

    private manureStore()
    {

    }
    public manureStore(string aPath, int id, int zoneNr, string aParens)
    {
        string parens = aParens;
        FileInformation manureStoreFile = new FileInformation(GlobalVars.Instance.getFarmFilePath());
        identity = id;
        path=aPath+'('+id.ToString()+')';

        manureStoreFile.setPath(path);
        name=manureStoreFile.getItemString("NameOfStorage");
        storageType = manureStoreFile.getItemInt("StorageType");
        speciesGroup = manureStoreFile.getItemInt("SpeciesGroup");
        getParameters(zoneNr);
    }
    public manureStore(int manureStorageType, int livestockSpeciesGroup, int zoneNr, string aParens)
    {
        storageType = manureStorageType;
        speciesGroup = livestockSpeciesGroup;
        getParameters(zoneNr);
        parens = aParens;
    }

    public void getParameters(int zoneNr)
    {
        FileInformation manureParamFile = new FileInformation(GlobalVars.Instance.getParamFilePath());
        manureParamFile.setPath("AgroecologicalZone("+zoneNr.ToString()+").ManureStorage");
        int maxManure = 0, minManure = 99;
        manureParamFile.getSectionNumber(ref minManure, ref maxManure);

        bool found = false;
        int num=0;
        //GlobalVars.Instance.log("ind " + " Req " + " test" + " sg ");
        for (int i = minManure; i <= maxManure; i++)
        {
            if (manureParamFile.doesIDExist(i))
            {
                manureParamFile.Identity.Add(i);
                int tmpStorageType = manureParamFile.getItemInt("StorageType");
                int tmpSpeciesGroup = manureParamFile.getItemInt("SpeciesGroup");
                name = manureParamFile.getItemString("Name");
                //  GlobalVars.Instance.log(i.ToString()+ " " + storageType.ToString()+ " "+ tmpStorageType.ToString()+ " "+ speciesGroup.ToString()+ " "+ tmpSpeciesGroup.ToString());
                if (storageType == tmpStorageType & speciesGroup == tmpSpeciesGroup)
                {
                    found = true;
                    num = i;
                    break;
                }
                manureParamFile.Identity.RemoveAt(manureParamFile.Identity.Count - 1);
            }
        }
        if (found == false)
        {
          string messageString=("could not match StorageType and SpeciesGroup at ManureStore. Was trying to find StorageType" + storageType.ToString() + " and speciesGroup " + speciesGroup.ToString());
          GlobalVars.Instance.Error(messageString);
        }
        string RecipientPath = "AgroecologicalZone("+zoneNr.ToString()+").ManureStorage" + '(' + num.ToString() + ").StoresSolid(-1)";
        bool StoresSolid;
        string tempString = manureParamFile.getItemString("Value",RecipientPath);
        if (tempString == "true")
            StoresSolid = true;
        else
            StoresSolid = false;
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "b1";
        b1 = manureParamFile.getItemDouble("Value");
//        manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "lnArr";
        lnArr = manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "ohmOrg";
        ohmOrg = manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "ohmTAN";
        ohmTAN = manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "Bo";
        Bo = manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "MCF";
        MCF = manureParamFile.getItemDouble("Value");
        //    double propGasCapture;
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "PropGasCapture";
        propGasCapture = manureParamFile.getItemDouble("Value");

        if (GlobalVars.Instance.getcurrentInventorySystem() == 1)
        {
            manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "EFNH3storageIPCC";
            EFStoreNH3 = manureParamFile.getItemDouble("Value");
            manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "EFN2OstorageIPCC";
            EFStoreN20 = manureParamFile.getItemDouble("Value");
        }
        else
        {
            manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "EFNH3storageRef";
            EFStoreNH3 = manureParamFile.getItemDouble("Value");
            manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "EFN2OstorageRef";
            EFStoreN20 = manureParamFile.getItemDouble("Value");
            manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "StorageRefTemp";
            StorageRefTemp = manureParamFile.getItemDouble("Value");
        }
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "lambda_m";
        Lambda = manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "TypeStored";
        manureTypeStored = manureParamFile.getItemInt("Value");
        theManure = new manure();
        theManure.SetisSolid(StoresSolid);
        //indicate the type of manure
        theManure.SetspeciesGroup(speciesGroup);
        FileInformation file = new FileInformation(GlobalVars.Instance.getfertManFilePath());
        file.setPath("AgroecologicalZone("+GlobalVars.Instance.GetZone().ToString()+").manure");
        int min = 99; int max = 0;
        file.getSectionNumber(ref min, ref max);
  
        //int itemNr = 0;
        bool gotit = false;
        int j = min;
        while ((j <= max) && (gotit == false))
        {
            if(file.doesIDExist(j))
            {
        
                file.Identity.Add(j);
                int StoredTypeFile = file.getItemInt("ManureType");
                int SpeciesGroupFile = file.getItemInt("SpeciesGroup");
                string manureName = file.getItemString("Name");
                if (StoredTypeFile == manureTypeStored && SpeciesGroupFile == speciesGroup)
                {
                    //itemNr = j;
                    theManure.SetmanureType(manureTypeStored);
                    theManure.Setname(manureName);
                    gotit = true;
                }
                j++;
                file.Identity.RemoveAt(file.Identity.Count-1);
            }
        }
        if (gotit == false)
        {
            string messageString = "Error - manure type not found for manure storage " + name;
            GlobalVars.Instance.Error(messageString);
        }
       // theManure.SetmanureType(itemNr);
    }
    public void Setname(string aname) { name = aname; }
    public void Setidentity(int aValue) { identity = aValue; }
    public void SetspeciesGroup(int aValue) { speciesGroup = aValue; }
    public void SetstorageType(int aValue) { storageType = aValue; }
    public void SettheHousing(housing ahouse){theHousing=ahouse;}
    public string Getname() { return name; }
    public int Getidentity() { return identity; }
    public int GetstorageType() { return storageType; }
    public int GetspeciesGroup() { return speciesGroup; }
    public housing GettheHousing() { return theHousing; }
    public bool GetStoresSolid() { return theManure.GetisSolid(); }

    public void Addmanure(manure amanure, double proportionOfYearGrazing)
    {
        theManure.SetDM(theManure.GetDM() + amanure.GetDM() );
        theManure.SetdegC(theManure.GetdegC()  + amanure.GetdegC());
        Cinput += amanure.GetdegC();
        Cinput += amanure.GetnonDegC();
        theManure.SetnonDegC(theManure.GetnonDegC() + amanure.GetnonDegC());
        theManure.SetTAN(theManure.GetTAN() + amanure.GetTAN());
        theManure.SetorganicN(theManure.GetorganicN() + amanure.GetorganicN());
        tstore += ((1 - proportionOfYearGrazing) * (theManure.GetdegC() + theManure.GetnonDegC()) / (2 * (theManure.GetdegC() + theManure.GetnonDegC())));
    }

    public void UpdateManureExchange()
    {
        manure manureToManureExchange = new manure(theManure);
        //manureToManureExchange.IncreaseManure(GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate());
        GlobalVars.Instance.theManureExchange.AddToManureExchange(manureToManureExchange);
    }
    public void DoManurestore()
    {
        double temp = theManure.GetnonDegC() + theManure.GetdegC() +theManure.GetTAN() + theManure.GetorganicN();
        if (temp > 0.0)
        {
            temp = theManure.GetnonDegC() + theManure.GetdegC();
            DoCarbon();
            DoNitrogen();
            UpdateManureExchange();
            temp = theManure.GetnonDegC() + theManure.GetdegC();
        }
    }
    public void DoCarbon()
    {
        Cinput = GetManureC();
        double tor = GlobalVars.Instance.gettor();
        double rgas = GlobalVars.Instance.getrgas();

        if (GlobalVars.Instance.getcurrentInventorySystem() == 1)
        {
            CCH4ST = (1/ GlobalVars.Instance.getalpha()) * (theManure.GetdegC() + theManure.GetnonDegC()) * (Bo * 0.67 * MCF);//1.46
           // double fT=Math.Exp(((-1.22*100000)/rgas)*((1/(meanTemp + GlobalVars.absoluteTemp))-(1/(15.0 + GlobalVars.absoluteTemp))));
            double km = 0.39;
            double VS1 = (theManure.GetdegC() + theManure.GetnonDegC()) / 0.46;
            double VS = (1-GettheHousing().gettheLivestock().GettimeOnPasture())*GettheHousing().gettheLivestock().GetAvgNumberOfAnimal() *(GettheHousing().gettheLivestock().GetfaecalC() + GettheHousing().gettheLivestock().GeturineC()) / 0.46;
            //            CCH4ST = km * fT * VS * Bo * 0.67;
            CCH4ST = MCF * VS * Bo * 0.67;
//            CCH4ST = MCF * fT * VS * Bo * 0.67;
            CCO2ST = (CCH4ST * (1 - tor)) / tor;//1.47
            CdegST = CCH4ST + CCO2ST;
            double Cdegradation=CdegST * (1 + GlobalVars.Instance.getHumification_const());
            if (Cdegradation>theManure.GetdegC())
            {
                Cdegradation-=theManure.GetdegC();
                 theManure.SetdegC(0.0);
                theManure.SetnonDegC(theManure.GetnonDegC()-Cdegradation);
            }
            else
                theManure.SetdegC(theManure.GetdegC() - Cdegradation);
        }
        else
        {
            CdegradationRate = b1 * Math.Pow(Math.E, lnArr - GlobalVars.Instance.getEapp() * (1 / (rgas * (meanTemp + GlobalVars.absoluteTemp)))); //1.48
            CdegST = theManure.GetdegC() * (1 - Math.Pow(Math.E, -CdegradationRate * tstore * GlobalVars.avgNumberOfDays));//1.49
            CCH4ST = CdegST * tor;//1.51
            CCO2ST = (CdegST * (1 - tor));//1.52
            theManure.SetdegC(theManure.GetdegC() - CdegST * (1 + GlobalVars.Instance.getHumification_const()));
        }
        //disable until can get workng properly
        //theManure.SethumicC(GlobalVars.Instance.getHumification_const() * CdegST);
        biogasCH4C = propGasCapture * CCH4ST;
        CCH4ST -= biogasCH4C;
        biogasCO2C = propGasCapture * CCO2ST;
        CCO2ST -= biogasCO2C;
    }
    public double getNitrogen()
    {
        return totalNstoreNH3+totalNstoreN2+totalNstoreN20;
    }
    public void DoNitrogen()
    {
        EFStoreN2 = Lambda * EFStoreN20;//1.66
        NTanInstore = theManure.GetTAN();
        NorgInstore = theManure.GetorganicN();
        double totalOrgNdegradation = 0; //only used if GlobalVars.Instance.getcurrentInventorySystem() = 1

        if (GlobalVars.Instance.getcurrentInventorySystem() == 1)
        {
            totalOrgNdegradation = (CdegST / Cinput) * NorgInstore;
            NRunOffOrg =NorgInstore*(1- Math.Pow(Math.E, -ohmOrg));
            NorgOutStore = NorgInstore - (NRunOffOrg + totalOrgNdegradation);
        }
        else
        {
            if (CdegradationRate > 0)
            {
                NDegOrgOut = NorgInstore * Math.Exp(-(CdegradationRate + ohmOrg) * tstore * GlobalVars.avgNumberOfDays);
                NRunOffOrg = (ohmOrg / (ohmOrg + CdegradationRate)) * NorgInstore * (1 - Math.Pow(Math.E, -(ohmOrg + CdegradationRate * tstore * GlobalVars.avgNumberOfDays))); //1.59
                //disable until can get workng properly
                /*    NHUM = (GlobalVars.Instance.getHumification_const() / GlobalVars.Instance.getCNhum()) * CdegradationRate
                        * (Math.Pow(Math.E, -(ohmOrg + CdegradationRate) * tstore) - Math.Pow(Math.E, -ohmOrg * tstore * GlobalVars.avgNumberOfDays));
                    NRunoffHum = (CdegradationRate / (ohmOrg + CdegradationRate));*/
            }
            else
            {
                NDegOrgOut = NorgInstore * Math.Exp(-ohmOrg * tstore * GlobalVars.avgNumberOfDays);
                NRunOffOrg = NorgInstore - NDegOrgOut;
                NHUM = 0;
                NRunoffHum = 0;
            }
            NorgOutStore = NDegOrgOut + NHUM;
        }

        if (GlobalVars.Instance.getcurrentInventorySystem() == 1)
        {
            totalNstoreN20 = EFStoreN20 * NTanInstore; //1.64 - not quite..
            double mineralisedN = 0.1 * NorgInstore;  // from Guidebook
            totalNstoreNH3 = EFStoreNH3 * (NTanInstore + mineralisedN);//1.65 - not quite..
            totalNstoreN2 = Lambda * totalNstoreN20;//1.66
            NHUM = 0;
            NrunoffTan = NTanInstore  * (1- Math.Exp(-ohmTAN * tstore));
            NTanOutstore = NTanInstore + totalOrgNdegradation - (totalNstoreN20 + totalNstoreN2 + totalNstoreNH3 + NrunoffTan);
        }
        else
        {
            double CN = theManure.GetdegC() / NorgInstore;
            double StorageRefTemp = 0;
            double EFNH3ref = 0;
            double KHø = 1 - 1.69 + 1447.7 / (meanTemp + GlobalVars.absoluteTemp);
            double KHref = 1 - 1.69 + 1447.7 / (StorageRefTemp + GlobalVars.absoluteTemp);
            EFStoreNH3 = KHref / KHø * EFNH3ref; //1.67
            double EFsum = EFStoreNH3 + EFStoreN20 + EFStoreN2;

            NTanOutstore = ((CdegradationRate * (1 / CN - GlobalVars.Instance.getHumification_const() / GlobalVars.Instance.getCNhum())
                * theManure.GetOrgDegC()) / ((EFsum + ohmTAN) - (ohmOrg + CdegradationRate / CN)))
                * Math.Pow(Math.E, -(ohmOrg + CdegradationRate / CN) * tstore);//1.63
            //NTanOutstore += NTanInstore - (theManure.GetdegC() * (1 / CN - tau / GlobalVars.Instance.getCNhum()) * theManure.GetOrgDegC()) / ((EFsum + ohmTAN) - (ohmOrg + theManure.GetdegC() / CN)) * Math.Pow(Math.E, -(EFsum + ohmOrg) * tstore);//1.63
            NTANLost = NorgInstore + NTanInstore - (NTanInstore + NrunoffTan + NTanOutstore);//1.68
            NrunoffTan = ohmTAN / (ohmTAN + EFStoreNH3 + EFStoreN20 + EFStoreN2) * NTANLost;
            totalNstoreNH3 = NTANLost * EFStoreNH3 / (ohmTAN + EFStoreN2 + EFStoreN20 + EFStoreNH3);
            totalNstoreN2 = NTANLost * EFStoreN2 / (ohmTAN + EFStoreN2 + EFStoreN20 + EFStoreNH3);
            totalNstoreN20 = NTANLost * EFStoreN20 / (ohmTAN + EFStoreN2 + EFStoreN20 + EFStoreNH3);
        }
        theManure.SethumicN(NHUM);
        theManure.SetorganicN(NorgOutStore);
        theManure.SetTAN(NTanOutstore);
    }
    public void Write()
    {
        if (GlobalVars.Instance.getRunFullModel())
            theHousing.Write();
        GlobalVars.Instance.writeStartTab("ManureStore");
        GlobalVars.Instance.writeInformationToFiles("name", "Name", "-", name, parens);
        GlobalVars.Instance.writeInformationToFiles("identity", "ID", "-", identity, parens);
        GlobalVars.Instance.writeInformationToFiles("Cinput", "C input", "kg", Cinput, parens);
        GlobalVars.Instance.writeInformationToFiles("CCH4ST", "CH4-C emitted", "kg", CCH4ST, parens);
        GlobalVars.Instance.writeInformationToFiles("CCO2ST", "CO2-C emitted", "kg", CCO2ST, parens);

        GlobalVars.Instance.writeInformationToFiles("Ninput", "N input", "kg", Ninput, parens);
        GlobalVars.Instance.writeInformationToFiles("NTanInstore", "TAN input to storage", "kg", NTanInstore, parens);
        GlobalVars.Instance.writeInformationToFiles("totalNstoreNH3", "NH3-N emitted", "kg", totalNstoreNH3, parens);
        GlobalVars.Instance.writeInformationToFiles("totalNstoreN2", "N2-N emitted", "kg", totalNstoreN2, parens);
        GlobalVars.Instance.writeInformationToFiles("totalNstoreN20", "N2O-N emitted", "kg", totalNstoreN20, parens);
        GlobalVars.Instance.writeInformationToFiles("NTANLost", "Total TAN lost", "kg", NTANLost, parens);
        GlobalVars.Instance.writeInformationToFiles("NDegOrgOut", "Degradable N ex storage", "kg", NDegOrgOut, parens);
        GlobalVars.Instance.writeInformationToFiles("NHUM", "Humic N ex storage", "kg", NHUM, parens);
        GlobalVars.Instance.writeInformationToFiles("NorgOutStore", "Organic N ex storage", "kg", NorgOutStore, parens);
        GlobalVars.Instance.writeInformationToFiles("NRunoffHum", "Humic N in runoff", "kg", NRunoffHum, parens);
        GlobalVars.Instance.writeInformationToFiles("NrunoffTan", "TAN in runoff", "kg", NrunoffTan, parens);
        GlobalVars.Instance.writeInformationToFiles("NRunOffOrg", "Organic N in runoff", "kg", NRunOffOrg, parens);
        GlobalVars.Instance.writeInformationToFiles("NLost", "Total N lost", "kg", NLost, parens);
        //GlobalVars.Instance.writeInformationToFiles("Nout", "Total N ", "kg", Nout);
        //GlobalVars.Instance.writeInformationToFiles("Nbalance", "??", "??", Nbalance);
        
        theManure.Write("");
      //  if (writeEndTab)
            GlobalVars.Instance.writeEndTab();
    }
    public bool CheckManureStoreCBalance()
    {
        bool retVal = false;
        double Cout = GetManureC() + biogasCO2C + biogasCH4C;
        double CLost = CCH4ST + CCO2ST;
        double Cbalance = Cinput - (Cout + CLost);
        double diff = Cbalance / Cinput;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
                double errorPercent = 100 * diff;
                string message1 = "Error; Manure storage C balance error is more than the permitted margin\n";
                string message2 =message1+ "Percentage error = " + errorPercent.ToString("0.00") + "%";
                GlobalVars.Instance.Error(message2);
             
        }
        return retVal;
    }
    public bool CheckManureStoreNBalance()
    {
        bool retVal = false;
        Ninput = NTanInstore + NorgInstore;
        Nout = GetManureN();
        NLost = NRunOffOrg + NrunoffTan+ totalNstoreN2+totalNstoreN20+totalNstoreNH3;
        Nbalance = Ninput - (Nout + NLost);
        double diff = Nbalance / Ninput;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
            Write();
            GlobalVars.Instance.CloseOutputXML();

                double errorPercent = 100 * diff;
       
                string messageString= ("Error; Manure storage N balance error is more than the permitted margin\n");
                messageString = messageString+("Percentage error = " + errorPercent.ToString("0.00") + "%");
        
                GlobalVars.Instance.Error(messageString);
          
        }
        return retVal;
    }
}
