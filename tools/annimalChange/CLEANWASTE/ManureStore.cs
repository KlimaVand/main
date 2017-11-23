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

    //other variables
    int identity;
    manure theManure;
    housing theHousing;
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

    public double GetCCH4ST() { return CCH4ST; }
    public double GetCCO2ST() { return CCO2ST; }
    public double GettotalNstoreNH3() { return totalNstoreNH3; }
    public double GettotalNstoreN2() { return totalNstoreN2; }
    public double GettotalNstoreN20() { return totalNstoreN20; }
    public double GetrunoffN() { return NRunoffHum + NRunOffOrg; }

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
    public manureStore(string aPath, int id, int zoneNr)
    {
        FileInformation manureStoreFile = new FileInformation(GlobalVars.Instance.getFarmFilePath());
        identity = id;
        path=aPath+'('+id.ToString()+')';

        manureStoreFile.setPath(path);
        name=manureStoreFile.getItemString("NameOfStorage");
        storageType = manureStoreFile.getItemInt("StorageType");
        speciesGroup = manureStoreFile.getItemInt("SpeciesGroup");
        getParameters(zoneNr);
    }
    public manureStore(int manureStorageType, int livestockSpeciesGroup, int zoneNr)
    {
        storageType = manureStorageType;
        speciesGroup = livestockSpeciesGroup;
        getParameters(zoneNr);
    }

    public void getParameters(int zoneNr)
    {
        FileInformation manureParamFile = new FileInformation(GlobalVars.Instance.getParamFilePath());
        manureParamFile.setPath("AgroecologicalZone("+zoneNr.ToString()+").ManureStorage");
        int maxManure = 0, minManure = 99;
        manureParamFile.getSectionNumber(ref minManure, ref maxManure);
        manureParamFile.Identity.Add(-1);
        bool found = false;
        int num=0;
        for (int i = minManure; i <= maxManure; i++)
        {
            manureParamFile.Identity[manureParamFile.Identity.Count - 1] = i;
            int tmpStorageType = manureParamFile.getItemInt("StorageType");
            int tmpSpeciesGroup = manureParamFile.getItemInt("SpeciesGroup");
            name = manureParamFile.getItemString("Name");
  //          Console.WriteLine(i.ToString()," ",storageType.ToString(), " ", tmpStorageType.ToString(), " ", speciesGroup.ToString(), " ", tmpSpeciesGroup.ToString());
            if (storageType == tmpStorageType & speciesGroup == tmpSpeciesGroup)
            {
                found = true;
                num = i;
                break;
            }
        }
        if (found == false)
        {
          System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.WriteLine("could not match StorageType and SpeciesGroup at ManureStore. Was trying to find StorageType" + storageType.ToString() + " and speciesGroup " + speciesGroup.ToString());
            file.WriteLine("model terminated");
            file.Close();
            Console.WriteLine("could not match StorageType and SpeciesGroup at ManureStore. Was trying to find StorageType" + storageType.ToString() + " and speciesGroup " + speciesGroup.ToString());
            Console.WriteLine("model terminated");
            if (GlobalVars.Instance.getPauseBeforeExit())
                Console.Read();
            Environment.Exit(0);
        }
        string RecipientPath = "AgroecologicalZone("+zoneNr.ToString()+").ManureStorage" + '(' + num.ToString() + ").StoresSolid(-1)";
        bool StoresSolid;
        string tempString = manureParamFile.getItemString("Value",RecipientPath);
        if (tempString == "True")
            StoresSolid = true;
        else
            StoresSolid = false;
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "b1";
        b1 = manureParamFile.getItemDouble("Value");
//        manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "lnArr";
        lnArr = manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "meanTemp";
        meanTemp = manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "ohmOrg";
        ohmOrg = manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "ohmTAN";
        ohmTAN = manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "Bo";
        Bo = manureParamFile.getItemDouble("Value");
        manureParamFile.PathNames[manureParamFile.PathNames.Count - 1] = "MCF";
        MCF = manureParamFile.getItemDouble("Value");

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
        theManure = new manure();
        theManure.SetisSolid(StoresSolid);
        //indicate the type of manure
        theManure.SetspeciesGroup(speciesGroup);
        theManure.SetstorageType(storageType);
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
        theManure.SetnonDegC(theManure.GetnonDegC() + amanure.GetnonDegC());
        theManure.SetTAN(theManure.GetTAN() + amanure.GetTAN());
        theManure.SetorganicN(theManure.GetorganicN() + amanure.GetorganicN());
        tstore += ((1 - proportionOfYearGrazing) * (theManure.GetdegC() + theManure.GetnonDegC()) / (2 * (theManure.GetdegC() + theManure.GetnonDegC())));
    }

    public void UpdateManureExchange()
    {
        manure manureToManureExchange = new manure(theManure);
        manureToManureExchange.IncreaseManure(GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate());
        GlobalVars.Instance.theManureExchange.AddToManureExchange(manureToManureExchange);
    }
    public void DoManurestore()
    {
        double temp = theManure.GetnonDegC() + theManure.GetdegC() + theManure.GetTAN() + theManure.GetorganicN();
        if (temp > 0.0)
        {
            DoCarbon();
            DoNitrogen();
            UpdateManureExchange();
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
            double fT=Math.Exp(((-1.22*100000)/rgas)*((1/(meanTemp + GlobalVars.absoluteTemp))-(1/(15.0 + GlobalVars.absoluteTemp))));
            double km = 0.39;
            double VS = (theManure.GetdegC() + theManure.GetnonDegC())/0.46;
            CCH4ST = km * fT * VS * Bo * 0.67;
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
            totalNstoreN20 = EFStoreN20 * (NTanInstore + NorgInstore); //1.64 - not quite..
            totalNstoreNH3 = EFStoreNH3 * (NTanInstore + NorgInstore);//1.65 - not quite..
            totalNstoreN2 = Lambda * totalNstoreN20;//1.66
            NHUM = 0;
            NrunoffTan = NTanInstore  * (1- Math.Exp(-ohmTAN * tstore));
            NTanOutstore = NTanInstore + totalOrgNdegradation - (totalNstoreN20 + totalNstoreN2 + totalNstoreNH3 + NrunoffTan);
            if (NTanOutstore < 0.0)
            {
                Write(GlobalVars.Instance.theZoneData.GetXMLWriter(),GlobalVars.Instance.theZoneData.GetTabFileWriter());
                GlobalVars.Instance.theZoneData.CloseOutputXML();
                string message1 = "Error; N losses from manure storage " + name + " exceed TAN present";
                string message2 = "";
                GlobalVars.Instance.WriteError(message1, message2);
                Environment.Exit(0);
            }
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
    public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
    {
        writer.WriteStartElement("ManureStore");

        writer.WriteStartElement("name");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("name");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(name);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Ninput");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("Ninput");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(Ninput);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("totalNstoreNH3");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("totalNstoreNH3");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(totalNstoreNH3);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("totalNstoreN2");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("totalNstoreN2");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(totalNstoreN2);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("totalNstoreN20");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("totalNstoreN20");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(totalNstoreN20);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NTANLost");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("NTANLost");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NTANLost);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NDegOrgOut");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("NDegOrgOut");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NDegOrgOut);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NHUM");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("NHUM");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NHUM);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NorgOutStore");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("NorgOutStore");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NorgOutStore);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NRunoffHum");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("NRunoffHum");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NRunoffHum);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NRunOffOrg");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("NRunOffOrg");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NRunOffOrg);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NTanInstore");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("NTanInstore");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NTanInstore);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NLost");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
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
        writer.WriteStartElement("name");
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
        writer.WriteStartElement("name");
        writer.WriteValue("Nbalance");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(Nbalance);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("meanTemp");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("meanTemp");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(meanTemp);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("MCF");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("MCF");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(MCF);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Bo");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("Bo");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(Bo);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Cinput");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("Cinput");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(Cinput);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("lnArr");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("lnArr");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(lnArr);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("identity");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("identity");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(identity);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Cinput");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("Cinput");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(Cinput);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("speciesGroup");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("speciesGroup");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(speciesGroup);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("tstore");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("tstore");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(tstore);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CCH4ST");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("CCH4ST");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(CCH4ST);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CCO2ST");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("CCO2ST");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(CCO2ST);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("b1");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("b1");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(b1);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Cinput");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("Cinput");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(Cinput);
        writer.WriteEndElement();
        writer.WriteEndElement();

        
       




        tabFile.Write("ManureStore"+"\n");
        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("name" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(name.ToString() + "\n");

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
        tabFile.Write("totalNstoreNH3" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(totalNstoreNH3.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("totalNstoreN2" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(totalNstoreN2.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("totalNstoreN20" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(totalNstoreN20.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NTANLost" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NTANLost.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NDegOrgOut" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NDegOrgOut.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NHUM" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NHUM.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NorgOutStore" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NorgOutStore.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NRunoffHum" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NRunoffHum.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NRunOffOrg" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NRunOffOrg.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NTanInstore" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NTanInstore.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NorgInstore" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NorgInstore.ToString() + "\n");

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


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("meanTemp" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(meanTemp.ToString() + "\n");



        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("MCF" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(MCF.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Bo" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(Bo.ToString() + "\n");


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
        tabFile.Write("lnArr" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(lnArr.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("identity" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(identity.ToString() + "\n");

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
        tabFile.Write("speciesGroup" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(speciesGroup.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("tstore" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(tstore.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("CCH4ST" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(CCH4ST.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("CCO2ST" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(CCO2ST.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("b1" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(b1.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Cinput" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(Cinput.ToString() + "\n");



        theManure.Write(writer, tabFile);

        writer.WriteEndElement();
    }
    public bool CheckManureStoreCBalance()
    {
        bool retVal = false;
        double Cout = GetManureC();
        double CLost = CCH4ST + CCO2ST;
        double Cbalance = Cinput - (Cout + CLost);
        double diff = Cbalance / Cinput;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
                double errorPercent = 100 * diff;
                string message1 = "Error; Manure storage C balance error is more than the permitted margin";
                string message2 = "Percentage error = " + errorPercent.ToString("0.00") + "%";
                GlobalVars.Instance.WriteError(message1, message2);
                Environment.Exit(0);
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
            Write(GlobalVars.Instance.theZoneData.GetXMLWriter(),GlobalVars.Instance.theZoneData.GetTabFileWriter());
            GlobalVars.Instance.theZoneData.CloseOutputXML();

                double errorPercent = 100 * diff;
                System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                file.WriteLine("Error; Manure storage N balance error is more than the permitted margin");
                file.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                file.Close();
                Console.WriteLine("Error; Manure storage N balance error is more than the permitted margin");
                Console.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                if (GlobalVars.Instance.getPauseBeforeExit())
                    Console.Read();
                Environment.Exit(0);
          
        }
        return retVal;
    }
}
