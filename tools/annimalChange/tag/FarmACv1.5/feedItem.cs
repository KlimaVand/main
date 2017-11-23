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
    bool isGrazed;
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
    public void SetisGrazed(bool aVal) { isGrazed = aVal; }
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
    public bool GetisGrazed() { return isGrazed; }
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
        isGrazed=false;
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
        if ((feedCode > 1000) && (feedCode < 2000))
        {
            feedCode -= 1000;
        }
        string aString = feedFile.getItemString("Grazed");
        if (aString == "true")
            isGrazed = true;
        else
            isGrazed = false;

        //Hack to allow grazed crop residues to refer to alternative feed, until interface fixed
        if (feedCode == 310)
        {
            feedCode = 925;
            isGrazed = true;
        }
        if (feedCode == 312)
        {
            feedCode = 926;
            isGrazed = true;
        }
        if (feedCode == 254)
        {
            feedCode = 934;
            isGrazed = true;
        }
        //end of hack
        //need a more elegant method of detecting incorporation
        bool checkIncorp = name.Contains("incorporated");
        if (name.Contains("incorporated"))
            feedCode-=2000;
        GetStandardFeedItem(feedCode);
        if (checkIncorp)//get standard feeditem has renamed the product, so add incorporated again, so it will be recognised later
            name += " incorporated";
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
            isGrazed = afeedItem.isGrazed;
            feedCode = afeedItem.feedCode;
            StoreProcessFactor = afeedItem.StoreProcessFactor;
            fedAtPasture = afeedItem.fedAtPasture;
        }

    public void GetStandardFeedItem(int targetFeedCode)
    {
        DateTime start = DateTime.Now;
       
        FileInformation file=new FileInformation(GlobalVars.Instance.getfeeditemFilePath());
        file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem");
        int min = 99; int max = 0;
        file.getSectionNumber(ref min, ref max);
        XmlReader data = XmlReader.Create(GlobalVars.Instance.getfeeditemFilePath());
        bool found = false;
        for(int i=min;i<max;i++)
        {
            file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem");
            if(file.doesIDExist(i))
            {
            file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem("+i.ToString()+")");
            int StandardFeedCode = file.getItemInt("FeedCode");
            if (StandardFeedCode == targetFeedCode)
            {
                found = true;
                name = file.getItemString("Name");
                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").Main(-1)");
                isMain = file.getItemBool("Value");
                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").Fibre_concentration(-1)");
                fibre_conc = file.getItemDouble("Value");
                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").NFE_concentration(-1)");
                nitrogenfreeExtract_conc = file.getItemDouble("Value");
                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").CrudeProtein_concentration(-1)");
                double CrudeProtein_concentration = file.getItemDouble("Value");
                SetN_conc(CrudeProtein_concentration / GlobalVars.NtoCrudeProtein);
                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").Fat_concentration(-1)");
                fat_conc = file.getItemDouble("Value");
       
                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").Energy_concentration(-1)");
                energy_conc = file.getItemDouble("Value"); 
                energy_conc *= GlobalVars.Instance.GetdigestEnergyToME();

                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").Ash_concentration(-1)");
                ash_conc = file.getItemDouble("Value");
                SetC_conc((1.0 - Getash_conc()) * 0.46);

                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").Nitrate_concentration(-1)");
                nitrate_conc = file.getItemDouble("Value");
     
                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").DMDigestibility(-1)");
                DMdigestibility = file.getItemDouble("Value");
     
                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").Bedding_material(-1)");
                beddingMaterial =file.getItemBool("Value");

                file.setPath("AgroecologicalZone(" + GlobalVars.Instance.GetZone() + ").feedItem(" + i.ToString() + ").processStorageLoss(-1)");
                StoreProcessFactor = file.getItemDouble("Value");
                
                break;
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
        isGrazed = false;
        beddingMaterial = false;
    }
    public void Write(string theParens)
    {
        parens = theParens + "feedCode" + feedCode.ToString();
        GlobalVars.Instance.writeStartTab("FeedItem");
        GlobalVars.Instance.writeInformationToFiles("name", "Name", "-", name,parens);
        GlobalVars.Instance.writeInformationToFiles("amount", "Amount", "kg DM", amount, parens);
        GlobalVars.Instance.writeInformationToFiles("feedCode", "Feed code", "-", feedCode, parens);
        GlobalVars.Instance.writeInformationToFiles("ash_conc", "Ash concentration", "kg/kg DM", ash_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("C_conc", "C concentration", "kg/kg DM", C_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("N_conc", "N concentration", "kg/kg DM", N_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("nitrogenfreeExtract_conc", "NFE concentration", "kg/kg DM", nitrogenfreeExtract_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("fibre_conc", "Fibre concentration", "kg/kg DM", fibre_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("fat_conc", "Fat concentration", "kg/kg DM", fat_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("energy_conc", "Energy concentration", "ME/kg DM", energy_conc, parens);
        GlobalVars.Instance.writeInformationToFiles("DMdigestibility", "DM digestibility", "kg/kg DM", DMdigestibility, parens);
        GlobalVars.Instance.writeInformationToFiles("isGrazed", "Fed at pasture", "-", isGrazed, parens);
        GlobalVars.Instance.writeInformationToFiles("beddingMaterial", "Can be used for bedding", "-", beddingMaterial, parens);
        GlobalVars.Instance.writeEndTab();

        
    }
    public void AdjustAmount(double factor)
    {
        amount *= factor;
    }
}

