using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using System.IO;
using System.Xml.XPath;
public class feedItem
{
    double amount;
    string name;
    int feedCode;
    int ID;
    string path;
    bool isMain;
    double energy_conc;
    double ash_conc;
    double C_conc;
    double N_conc;
    double nitrogenfreeExtract_conc;
    double fibre_conc;
    double fat_conc;
    double nitrate_conc;
    double DMdigestibility;
    bool fedAtPasture;
    bool beddingMaterial;
    double StoreProcessFactor;
    public void setFeedCode(int aFeedCode) { feedCode = aFeedCode; }
    public int GetFeedCode(){return feedCode;}
    public void Setamount(double anamount) { amount = anamount; }
    public void SetC_conc(double aVal) { C_conc = aVal; }
    public void SetN_conc(double aVal) { N_conc = aVal; }
    public void Setfibre_conc(double aVal) { fibre_conc = aVal; }
    public void SetnitrogenfreeExtract_conc(double aVal) { nitrogenfreeExtract_conc = aVal; }
    public void SetDMdigestibility(double aVal) { DMdigestibility = aVal; }
    public void Setash_conc(double aVal) { ash_conc = aVal; }
    public void Setnitrate_conc(double aVal) { nitrate_conc = aVal; }
    public void Setenergy_conc(double aVal) { energy_conc = aVal; }
    public void Setname(string aString) { name = aString; }
    public void SetisMain(bool aVal) { isMain = aVal; }
    public void SetfedAtPasture(bool aVal) { fedAtPasture = aVal; }

    public double Getamount() { return amount; }
    public string GetName() { return name; }
    public double Getenergy_conc() { return energy_conc; }
    public double Getash_conc() { return ash_conc; }
    public double GetC_conc() { return C_conc; }
    public double GetN_conc() { return N_conc; }
    public double GetNitrate_conc() { return nitrate_conc; }
    public double GetnitrogenfreeExtract_conc() { return nitrogenfreeExtract_conc; }
    public double Getfibre_conc() { return fibre_conc; }
    public double Getfat_conc() { return fat_conc; }
    public double GetDMdigestibility() { return DMdigestibility; }
    public double GetStoreProcessFactor() { return StoreProcessFactor; }
    public bool GetfedAtPasture() { return fedAtPasture; }
    public bool GetbeddingMaterial() { return beddingMaterial; }
    public bool GetisMain() { return isMain; }
    public void Addamount(double aVal){amount += aVal;}
    private string Unit;
    string parens;
    public feedItem()
    {
        name = "None";
        path = "None";
        amount=0;
        feedCode=0;
        isMain=false;
        energy_conc=0;
        ash_conc=0;
        C_conc=0;
        N_conc=0;
        nitrogenfreeExtract_conc=0;
        fibre_conc=0;
        fat_conc=0;
        nitrate_conc = 0;
        DMdigestibility=0;
        fedAtPasture=false;
        beddingMaterial=false;
        StoreProcessFactor=0;
        Unit = "kg/day";
    }
    public feedItem(string feeditemPath, int id, bool getamount, string aParens)
    {
        parens = aParens;
        ID = id;
        path = feeditemPath + '(' + id + ')';
        FileInformation feedFile = new FileInformation(GlobalVars.Instance.getFarmFilePath());
        feedFile.setPath(path);
        if (getamount == true)
        {
            amount = feedFile.getItemDouble("Amount");
            Unit = feedFile.getItemString("Unit");
        }
        name = feedFile.getItemString("Name");
        feedCode = feedFile.getItemInt("FeedCode");
        string aString = feedFile.getItemString("Grazed");
        if (aString == "true")
           fedAtPasture = true;
        else
           fedAtPasture = false;
        GetStandardFeedItem(feedCode);
    }

    public feedItem(feedItem afeedItem)
        {
            energy_conc = afeedItem.energy_conc ;
            ash_conc = afeedItem.ash_conc;
            C_conc = afeedItem.C_conc;
            N_conc = afeedItem.N_conc;
            nitrogenfreeExtract_conc = afeedItem.nitrogenfreeExtract_conc;
            fibre_conc = afeedItem.fibre_conc;
            fat_conc = afeedItem.fat_conc;
            DMdigestibility = afeedItem.DMdigestibility;
            amount = afeedItem.amount;
            name = afeedItem.name;
            fedAtPasture = afeedItem.fedAtPasture;
            feedCode = afeedItem.feedCode;
            StoreProcessFactor = afeedItem.StoreProcessFactor;
        }

    public void GetStandardFeedItem(int targetFeedCode)
    {
        DateTime start = DateTime.Now;

        XmlReader data = XmlReader.Create(GlobalVars.Instance.getfeeditemFilePath());
        bool found = false;
        while (data.Read())
        {
            if (data.NodeType == XmlNodeType.Element)
            {
                XElement el = XNode.ReadFrom(data) as XElement;
                IEnumerable<XElement> node = el.Elements();
                long size= node.LongCount();
                for (int i = 0; i < size; i++)
                {
                    IEnumerable<XElement> zones = node.ElementAt(i).Elements();
                    int zone = Convert.ToInt32(zones.ElementAt(0).Value);
                    if (GlobalVars.Instance.GetZone() == zone)
                    {
                        long sizeofItem = zones.LongCount();
                        for (int j = 0; j < (sizeofItem-3); j++)
                        {
                            IEnumerable<XElement> feeditem = zones.ElementAt(3+j).Elements();
                            int StandardFeedCode = Convert.ToInt32(feeditem.ElementAt(2).Value);
                            if (StandardFeedCode == targetFeedCode)
                            {
                                found = true;
                                name = Convert.ToString(feeditem.ElementAt(1).Value);
                                IEnumerable<XElement> main = feeditem.ElementAt(3).Elements();
                                isMain = Convert.ToBoolean(main.ElementAt(1).Value);
                                IEnumerable<XElement> Fibre_concentration = feeditem.ElementAt(4).Elements();


                                fibre_conc = Convert.ToDouble(Fibre_concentration.ElementAt(1).Value);
                                IEnumerable<XElement> NFE_concentrationFile = feeditem.ElementAt(5).Elements();
                                nitrogenfreeExtract_conc = Convert.ToDouble(NFE_concentrationFile.ElementAt(1).Value);
                                IEnumerable<XElement> CrudeProtein_concentrationFile = feeditem.ElementAt(6).Elements();
                                double CrudeProtein_concentration = Convert.ToDouble(CrudeProtein_concentrationFile.ElementAt(1).Value);
                                SetN_conc(CrudeProtein_concentration / GlobalVars.NtoCrudeProtein);
                                IEnumerable<XElement> Fat_concentration = feeditem.ElementAt(7).Elements();
                                fat_conc = Convert.ToDouble(Fat_concentration.ElementAt(1).Value);
                                IEnumerable<XElement> Energy_concentration = feeditem.ElementAt(8).Elements();
                                energy_conc = Convert.ToDouble(Energy_concentration.ElementAt(1).Value);
                                energy_conc *= GlobalVars.Instance.GetdigestEnergyToME();
                                IEnumerable<XElement> Ash = feeditem.ElementAt(9).Elements();
                                ash_conc = Convert.ToDouble(Ash.ElementAt(1).Value);
                                SetC_conc((1.0 - Getash_conc()) * 0.46);
                                IEnumerable<XElement> Nitrate = feeditem.ElementAt(10).Elements();
                                nitrate_conc = Convert.ToDouble(Nitrate.ElementAt(1).Value);
                                IEnumerable<XElement> DMDigestibility = feeditem.ElementAt(11).Elements();
                                DMdigestibility = Convert.ToDouble(DMDigestibility.ElementAt(1).Value);
                                IEnumerable<XElement> Bedding_material = feeditem.ElementAt(12).Elements();
                                beddingMaterial = Convert.ToBoolean(Bedding_material.ElementAt(1).Value);
                                IEnumerable<XElement> storeProcessFactor = feeditem.ElementAt(13).Elements();
                                StoreProcessFactor = Convert.ToDouble(storeProcessFactor.ElementAt(1).Value);
                                break;
                            }
                        }
                    }
                }
            }
        }
    
        if (found == false)
        {
            string messageString=("could not find feeditem \n");
            messageString+=(feedCode);
            GlobalVars.Instance.Error(messageString);
        }
 
        TimeSpan timeItTook = DateTime.Now - start;

}
    public int AddFeedItem(feedItem afeedItem, bool pooling)
    {
        if ((afeedItem.GetFeedCode() != feedCode) && (pooling != true))
        {
              string messageString=("Error; attempt to combine two different feed items");
               GlobalVars.Instance.Error(messageString);
        }
        double donorAmount = afeedItem.Getamount();
        name = afeedItem.GetName();
        feedCode = afeedItem.GetFeedCode();
        if (donorAmount != 0)
        {
            energy_conc = (energy_conc * amount + donorAmount * afeedItem.Getenergy_conc()) / (amount + donorAmount);
            ash_conc = (ash_conc * amount + donorAmount * afeedItem.Getash_conc()) / (amount + donorAmount);
            C_conc = (C_conc * amount + donorAmount * afeedItem.GetC_conc()) / (amount + donorAmount);
            N_conc = (N_conc * amount + donorAmount * afeedItem.GetN_conc()) / (amount + donorAmount);
            nitrogenfreeExtract_conc = (nitrogenfreeExtract_conc * amount + donorAmount * afeedItem.GetnitrogenfreeExtract_conc()) / (amount + donorAmount);
            fibre_conc = (fibre_conc * amount + donorAmount * afeedItem.Getfibre_conc()) / (amount + donorAmount);
            fat_conc = (fat_conc * amount + donorAmount * afeedItem.Getfat_conc()) / (amount + donorAmount);
            DMdigestibility = (DMdigestibility * amount + donorAmount * afeedItem.GetDMdigestibility()) / (amount + donorAmount);
            amount += donorAmount;
        }
        return 0;
    }
    //Jonas - this is dangerous code. The feedItem concentrations could go negative. I have no immediate solution
    public int SubtractFeedItem(feedItem afeedItem, bool pooling)
    {
        if ((afeedItem.GetFeedCode() != feedCode) && (pooling != true))
        {
            string messageString=("Error; attempt to subtract two different feed items");
            GlobalVars.Instance.Error(messageString);
        }
        double donorAmount = afeedItem.Getamount();
        amount -= donorAmount;
        return 0;
    }

    public void initialize()
    {
        amount = 0;
        fedAtPasture = false;
        beddingMaterial = false;
    }
    public void Write(string theParens)
    {
        parens = theParens + feedCode.ToString();
        GlobalVars.Instance.writeStartTab("FeedItem");
        GlobalVars.Instance.writeInformationToFiles("name", "Name", "-", name,parens);
        GlobalVars.Instance.writeInformationToFiles("amount", "Amount", "kg", amount, parens);
        GlobalVars.Instance.writeInformationToFiles("feedCode", "Feed code", "-", feedCode, parens);
        GlobalVars.Instance.writeInformationToFiles("ash_conc", "Ash concentration", "kg/kg", ash_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("C_conc", "C concentration", "kg/kg", C_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("N_conc", "N concentration", "kg/kg", N_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("nitrogenfreeExtract_conc", "NFE concentration", "kg/kg", nitrogenfreeExtract_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("fibre_conc", "Fibre concentration", "kg/kg", fibre_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("fat_conc", "Fat concentration", "kg/kg", fat_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("energy_conc", "Energy concentration", "ME/kg", energy_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("DMdigestibility", "DM digestibility", "kg/kg", DMdigestibility, parens);
        GlobalVars.Instance.writeInformationToFiles("fedAtPasture", "Fed at pasture", "-", fedAtPasture, parens);
        GlobalVars.Instance.writeInformationToFiles("beddingMaterial", "Can be used for bedding", "-", beddingMaterial, parens);
        GlobalVars.Instance.writeEndTab();

        
    }
    public void AdjustAmount(double factor)
    {
        amount *= factor;
    }
}

