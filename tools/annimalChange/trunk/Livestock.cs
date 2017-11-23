using System;
using System.Collections.Generic;
using System.Xml;
public class livestock
{
    public struct ManureRecipient
    {
        int StorageType;
        string parens;
    /**
* set Parens
*/
        public void setParens(string aParens){parens=aParens;}
    /**
* set Storage Type
*/
        public void setStorageType(int aType) { 
            StorageType = aType;
        }
    /**
* Get Storage Type
*/
        public int GetStorageType() { return StorageType; }
    /**
* Write
*/
        public void Write()
        {
            GlobalVars.Instance.writeStartTab("ManureRecipient");
            GlobalVars.Instance.writeInformationToFiles("StorageType", "Type of manure store", "-", StorageType, parens);
            GlobalVars.Instance.writeEndTab();
        }
    }
    public struct housingRecord
    {
        int HousingType;
        double propTime;
        string NameOfHousing;
        string parens;
        public List<ManureRecipient> Recipient;
    /**
* Set Name Of Housing
*/
        public void SetNameOfHousing(string aName) { NameOfHousing = aName; }
    /**
* Set Housing Type
*/
        public void SetHousingType(int aVal) { HousingType = aVal; }
    /**
* Set prop Time
*/
        public void SetpropTime(double aVal) { propTime = aVal; }
    /**
* Get Housing Type
*/
        public int GetHousingType() { return HousingType; }
    /**
* Get Housing Name
*/
        public string GetHousingName() { return NameOfHousing; }
    /**
* Get prop Time
*/
        public double GetpropTime() { return propTime; }
    /**
* Get Manure Recipient
*/
        public List<ManureRecipient> GetManureRecipient() { return Recipient; }
    /**
* set Parens
*/
        public void setParens(string aParens) { 
            parens = aParens; 
        }
    /**
* Write
*/
        public void Write()
        {
            GlobalVars.Instance.writeStartTab("housingRecord");
            GlobalVars.Instance.writeInformationToFiles("HousingType", "Type of housing", "-", HousingType,parens);
            GlobalVars.Instance.writeInformationToFiles("propTime", "Proportion of time spent in house", "-", propTime, parens);
            for (int i=0; i < Recipient.Count; i++)
                Recipient[i].Write();
            GlobalVars.Instance.writeEndTab();
        }
    }
    public struct item
    {
        int feedCode;
        double amount;
    /**
* Set feedCode
*/
        public void SetfeedCode(int aVal) { feedCode = aVal; }
    /**
* Set amount
*/
        public void Setamount(double aVal) { amount = aVal; }
    /**
* Get feedCode
*/
        public int GetfeedCode() { return feedCode; }
    /**
* Get amount
*/
        public double Getamount() { return amount; }
    }

    string path;

    //characteristics of livestock
    bool isRuminant;
    bool isDairy; // true if this is a milk-producing animal
    double Energy_base;  ///< energy intake level below which there is no reduction in energy utilisation, EA_base,g
    double mu_slope;  ///< rate of reduction in energy utilisation, µA_slope
    //input data
    double avgNumberOfAnimal;///<number of animals in category, averaged over year, Z_A_g

    double avgProductionMilk;
    double avgProductionMeat;///<change in liveweight, kg/yr, LA_change,g 
    double avgProductionECM;
    double efficiencyProteinMilk;
    List<housingRecord> housingDetails;
    //parameters
    int identity;
    private string parens;
    int speciesGroup;
    int LivestockType; //finding parameter for this type
    double liveweight;
    double startWeight;
    double endWeight;
    double duration;
    double urineProp;
    string name;
    double growthNconc;
    double growthPconc;
    double growthCconc;
    double milkNconc;
    double milkCconc;
    double milkPconc;
    double milkFat;
    double age;
    double maintenanceEnergyCoeff;
    double growthEnergyDemandCoeff;///<metabolisable energy required for weight gain, ME/kg, eA_prod,g
    double milkAdjustmentCoeff;
    bool housedDuringGrazing;
    double mortalityCoefficient;
    double entericTier2MCF;
    double nitrateEfficiency;
    //other variables
    double energyIntake;///<annual intake of available energy, MJ per year, E_A_int,g
    double energyDemand;
    double energyUseForMaintenance;
    double energyUseForGrowth;
    double energyUseForMilk;
    double energyUseForGrazing;
    double energyFromRemobilisation;
    double maintenanceEnergyDeficit;
    double growthEnergyDeficit;
    double concentrateEnergy;

    double DMintake;///<DM intake, kg per year, I_A_g
    double DMgrazed;
    double FE;
    double concentrateDM;
    double Nintake; ///< nitrogen intake in kg/yr, NA_int,g
    double diet_ash;
    double diet_fibre;
    double diet_fat;
    double diet_NFE;
    double digestibilityDiet;
    double diet_nitrate;//kg/kg
    double Cintake;
    double energyLevel;
    double milkN;
    double milkC;
    double growthN;
    double growthC;
    double mortalitiesN;
    double mortalitiesP;
    double mortalitiesC;
    double urineC;
    double faecalC;
    double urineN;
    double faecalN;
    double NexcretionToPasture;
    double CexcretionToPasture;
    double CH4C;
    double CO2C;
    double grazedN = 0;
    double grazedDM = 0;
    double mexp=0; //Mass of manure from species group sg 
    double cman=0;// Mass of manure from species group sg
    double nman = 0;
    double vsg=0;//Annual production of manure from species group sg and store type s
    bool proteinLimited;

    double waterIntake;
    double waterMetabolic;
    double waterExhaled;
    double waterExcreta;
    double Pintake;
    double growthP;
    double milkP;
    double excretaP;

    //economics
    double salePriceMeat;
    double salePriceMilk;
    double nonFeedCosts;
/**
* ge twater Intake
*/
    public double getwaterIntake() { return waterIntake; }
/**
* Set water Intake
*/
    public void SetwaterIntake(double newValue) { waterIntake = newValue; }
/**
* get water Metabolic
*/
    public double getwaterMetabolic() { return waterMetabolic; }
/**
* Set water Metabolic
*/
    public void SetwaterMetabolic(double newValue) { waterMetabolic = newValue; }
/**
* get water Exhaled
*/
    public double getwaterExhaled() { return waterExhaled; }
/**
* Se twater Exhaled
*/
    public void SetwaterExhaled(double newValue) { waterExhaled = newValue; }
/**
* get water Excreta
*/
    public double getwaterExcreta() { return waterExcreta; }
/**
* Set water Excreta
*/
    public void SetwaterExcreta(double newValue) { waterExcreta = newValue; }
/**
* get Pintake
*/
    public double getPintake() { return Pintake; }
/**
* Set Pintake
*/
    public void SetPintake(double newValue) { Pintake = newValue; }
/**
* get milk P
*/
    public double getmilkP() { return milkP; }
/**
* get milk P
*/
    public double getgrowthP() { return growthP; }
/**
* Set milk P
*/
    public void SetmilkP(double newValue) { milkP = newValue; }
/**
* get excreta P
*/
    public double getexcretaP() { return excretaP; }
/**
* get sale Price Meat
*/
    public double getsalePriceMeat() { return salePriceMeat; }
/**
* Set sale Price Meat
*/
    public void SetsalePriceMeat(double newValue) { salePriceMeat = newValue; }
/**
    * get sale Price Milk
    */
public double getsalePriceMilk() { return salePriceMilk; }
/**
* get non feed costs
*/
    public double getnonFeedCosts() { return nonFeedCosts; }
    /**
    * Set sale Price Milk
        */
    public void SetsalePriceMilk(double newValue) { salePriceMilk = newValue; }
/**
* Do Water
*/
    public void DoWater(){}

   // feedItem milk;  //used to hold milk produced
/**
* get CH4C
*/
    public double getCH4C() { return CH4C; }
/**
* get CO2C
*/
    public double getCO2C() { return CO2C; }
/**
*get CmanExp
*/
    public double getCmanExp() { return mexp * cman * vsg; }
/**
* get NmanExp
*/
    public double getNmanExp() { return mexp * nman * vsg; }
    public double timeOnPasture;
/**
* Set avg Production Milk
*/
    public void SetavgProductionMilk(double anavgProductionMilk) { avgProductionMilk = anavgProductionMilk; }
/**
* Set avg Production Meat
*/
    public void SetavgProductionMeat(double anavgProductionMeat) { avgProductionMeat = anavgProductionMeat; }
/**
* Set name
*/
    public void Setname(string aname) { name = aname; }
/**
* Set identity
*/
    public void Setidentity(int aValue) { identity = aValue; }
/**
* Set species Group
*/
    public void SetspeciesGroup(int aValue) { speciesGroup = aValue; }
/**
* Get Milk C
*/
    public double GetMilkC() { return milkC; }
/**
* Get Growth C
*/
    public double GetGrowthC() { return growthC; }
/**
* Get Milk N
*/
    public double GetMilkN() { return milkN; }
/**
* Get Growth N
*/
    public double GetGrowthN() { return growthN; }
/**
* Get Mortalities N
*/
    public double GetMortalitiesN() { return mortalitiesN; }
/**
* Get Mortalities C
*/
    public double GetMortalitiesC() { return mortalitiesC; }
/**
* Get mortality Coefficient
*/
    public double GetmortalityCoefficient() {return mortalityCoefficient;}
/**
* Get avg Production Milk
*/
    public double GetavgProductionMilk() { return avgProductionMilk; }
/**
* Get avg Production Meat
*/
    public double GetavgProductionMeat() { return avgProductionMeat; }
/**
* Get housed During Grazing
*/
    public bool GethousedDuringGrazing() { return housedDuringGrazing; }
/**
* Get grazed N
*/
    public double GetgrazedN() { return grazedN; }
/**
* Get grazed DM
*/
    public double GetgrazedDM() { return grazedDM; }
/**
* Get N excretion To Pasture
*/
    public double GetNexcretionToPasture() { return NexcretionToPasture; }
/**
* Get C excretion To Pasture
*/
    public double GetCexcretionToPasture() { return CexcretionToPasture; }
/**
* Set isRuminant
*/
    public void SetisRuminant(bool aVal) { isRuminant = aVal; }

    List<feedItem> feedRation;
/**
* Set feed Ration
*/
    public void SetfeedRation(List<feedItem> afeedRation) { feedRation = afeedRation; }
/**
* Set housing Detail
*/
    public void SethousingDetail(int houseID, double proportion)
    {
        housingRecord ahousingRecord = new housingRecord();
        ahousingRecord.SetHousingType(houseID);
        ahousingRecord.SetpropTime(proportion);
        housingDetails.Add(ahousingRecord);
    }
/**
* Get isRuminant
*/
    public bool GetisRuminant() { return isRuminant; }
/**
*/
    public double Getliveweight() { return liveweight; }
/**
* Get DMintake
*/
    public double GetDMintake() { return DMintake; }
/**
* Get DMgrazed
*/
    public double GetDMgrazed() { return DMgrazed; }
/**
* Get Cintake
*/
    public double GetCintake() { return Cintake; }
/**
* Get Nintake
*/
    public double GetNintake() { return Nintake; }
/**
* Get urineC
*/
    public double GeturineC() { return urineC; }
/**
* Get faecal C
*/
    public double GetfaecalC() { return faecalC; }
/**
* Get Fibre
*/
    public double GetFibre() { return diet_fibre; }
/**
* Get urine N
*/
    public double GeturineN() { return urineN; }
/**
* Get faecal N
*/
    public double GetfaecalN() { return faecalN; }
/**
* Get Excreted N
*/
    public double GetExcretedN() { return faecalN + urineN; }
/**
* Get AvgNumber Of Animal
*/
    public double GetAvgNumberOfAnimal(){return avgNumberOfAnimal;}
/**
* Get time On Pasture
*/
    public double GettimeOnPasture() { return timeOnPasture; }
/**
* Get feed Ration
*/
    public List<feedItem> GetfeedRation() { return feedRation; }
/**
* Get name
*/
    public string Getname() { return name; }
/**
* Get identity
*/
    public int Getidentity() { return identity; }
/**
* Get species Group
*/
    public int GetspeciesGroup() { return speciesGroup; }
/**
* Get housing Details
*/
    public List<housingRecord> GethousingDetails() { return housingDetails; }
/**
* Constructor
*/
    public livestock()
    {
    }
/**
* Constructor
*/
    public livestock(string aPath, int id, int zoneNr, int AnimalNr, string aParens)
    {
        parens = aParens;
        FileInformation livestockFile =new FileInformation(GlobalVars.Instance.getFarmFilePath());
        identity = id;
        path = aPath+"("+id.ToString()+")";
        livestockFile.setPath(path);
        feedRation = new List<feedItem>();
        urineProp = 0;
        DMintake =0;
        DMgrazed = 0;
        energyDemand = 0;
        energyIntake = 0;
        diet_ash = 0;
        diet_nitrate = 0;
        digestibilityDiet = 0;
        timeOnPasture = 0;
        proteinLimited = false;
        name = livestockFile.getItemString("NameOfAnimals");
        avgNumberOfAnimal = livestockFile.getItemDouble("NumberOfAnimals");
        housingDetails = new List<housingRecord>();
        if (avgNumberOfAnimal > 0)
        {
            LivestockType = livestockFile.getItemInt("LivestockType");
            speciesGroup = livestockFile.getItemInt("Species_group");


            FileInformation paramFile = new FileInformation(GlobalVars.Instance.getParamFilePath());

            //read livestock parameters from constants.xml
            string basePath = "AgroecologicalZone(" + zoneNr.ToString() + ").Livestock";
            int min = 99, max = 0;
            paramFile.setPath(basePath);
            paramFile.getSectionNumber(ref min, ref max);
            bool gotit = false;
            int livestockID = 0;
            for (int i = min; i <= max; i++)
            {
                if (paramFile.doesIDExist(i))
                {
                    string testPath = basePath + "(" + i.ToString() + ").LivestockType(0)";
                    int testLivestockType = paramFile.getItemInt("Value", testPath);
                    testPath = basePath + "(" + i.ToString() + ").SpeciesGroup(0)";
                    int testspeciesGroup = paramFile.getItemInt("Value", testPath);
                    if ((testLivestockType == LivestockType) && (testspeciesGroup == speciesGroup))
                    {
                        livestockID = i;
                        gotit = true;
                        break;
                    }
                    paramFile.setPath(basePath);
                }
            }
            if (gotit == false)
            {
                string messageString = ("Livestock " + name + " Species " + speciesGroup.ToString() + ", Livestocktype  " + LivestockType.ToString() + " not found in parameters.xml");
                GlobalVars.Instance.Error(messageString);
            }
            basePath = "AgroecologicalZone(" + zoneNr.ToString() + ").Livestock(" + Convert.ToInt32(livestockID) + ")";
            //paramFile.setPath(basePath + ".SpeciesGroup(0)");
            //speciesGroup = paramFile.getItemInt("Value");
            paramFile.setPath(basePath + ".efficiencyProteinMilk(0)");
            efficiencyProteinMilk = paramFile.getItemDouble("Value");
            
            paramFile.setPath(basePath + ".isRuminant(0)");
            isRuminant = paramFile.getItemBool("Value");
            paramFile.setPath(basePath + ".isDairy(0)");
            isDairy = paramFile.getItemBool("Value");
            paramFile.setPath(basePath + ".growthNconc(0)");
            growthNconc = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".growthCconc(0)");
            growthCconc = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".growthPconc(0)");
            growthPconc = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".urineProp(0)");
            urineProp = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".maintenanceEnergyCoeff(0)");
            maintenanceEnergyCoeff = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".growthEnergyDemandCoeff(0)");
            growthEnergyDemandCoeff = paramFile.getItemDouble("Value");
            if (isDairy)
            {
                paramFile.setPath(basePath + ".milkAdjustmentCoeff(0)");
                milkAdjustmentCoeff = paramFile.getItemDouble("Value");
                paramFile.setPath(basePath + ".milkFat(0)");
                milkFat = paramFile.getItemDouble("Value");
            }
            paramFile.setPath(basePath + ".Liveweight(0)");
            liveweight = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".Age(0)");
            age = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".HousedDuringGrazing(-1)");
            housedDuringGrazing = paramFile.getItemBool("Value");
            paramFile.setPath(basePath + ".Mortality(0)");
            mortalityCoefficient = paramFile.getItemDouble("Value");
            entericTier2MCF = paramFile.getItemDouble("Value", basePath + ".entericTier2MCF(-1)");
            paramFile.setPath(basePath + ".growthSalePrice(0)");
            salePriceMeat = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".nonFeedCosts(0)");
            nonFeedCosts = paramFile.getItemDouble("Value");

            if (isRuminant)
            {
                paramFile.setPath(basePath + ".mu_b(0)");
                mu_slope = paramFile.getItemDouble("Value");
                paramFile.setPath(basePath + ".mu_base(0)");
                Energy_base = paramFile.getItemDouble("Value");
                paramFile.setPath(basePath + ".nitrateEfficiency(0)");
                nitrateEfficiency = paramFile.getItemDouble("Value");
            }
            //back to reading user input
            if (isDairy)
            {
                //avgProductionMilk = livestockFile.getItemDouble("avgProductionMilk");
                paramFile.setPath(basePath + ".weightGainDairy(0)");
                avgProductionMeat = paramFile.getItemDouble("Value");
                avgProductionMeat /= GlobalVars.avgNumberOfDays;
                paramFile.setPath(basePath + ".milkNconc(0)");
                milkNconc = paramFile.getItemDouble("Value");
                paramFile.setPath(basePath + ".milkPconc(0)");
                milkPconc = paramFile.getItemDouble("Value");
                paramFile.setPath(basePath + ".milkCconc(0)");
                milkCconc = paramFile.getItemDouble("Value");
                paramFile.setPath(basePath + ".milkSalePrice(0)");
                salePriceMilk = paramFile.getItemDouble("Value");
            }
            else
            {
                if (speciesGroup == 1)
                    avgProductionMeat = livestockFile.getItemDouble("avgProductionMeat");
                else
                if (speciesGroup == 2)
                {
/*                    double startWeight = livestockFile.getItemDouble("ProductionLevel");
                    double endWeight = livestockFile.getItemDouble("ProductionLevel2");
                    */
                }
            }

            //housedDuringGrazing = true;
            
            string housingPath = path + ".Housing";
            min = 99;
            max = 0;
            livestockFile.setPath(housingPath);
            livestockFile.getSectionNumber(ref min, ref max);
            if (max > 0)
            {
                double testPropTime = 0;
                for (int i = min; i <= max; i++)
                {
                    if (livestockFile.doesIDExist(i))
                    {
                        housingRecord newHouse = new housingRecord();
                        newHouse.setParens(parens + "_housingRecord" + i.ToString());
                        livestockFile.Identity.Add(i);
                        newHouse.SetHousingType(livestockFile.getItemInt("HousingType"));
                        newHouse.SetNameOfHousing(livestockFile.getItemString("NameOfHousing"));
                        if (newHouse.GetHousingName() != "None")
                        {
                            newHouse.SetpropTime(livestockFile.getItemDouble("PropTime"));
                            testPropTime += newHouse.GetpropTime();
                            int maxManureRecipient = 0, minManureRecipient = 99;
                            newHouse.Recipient = new List<ManureRecipient>();
                            string RecipientPath = housingPath + '(' + i.ToString() + ").ManureRecipient";
                            livestockFile.setPath(RecipientPath);
                            livestockFile.getSectionNumber(ref minManureRecipient, ref maxManureRecipient);
                            for (int j = minManureRecipient; j <= maxManureRecipient; j++)
                            {
                                if (livestockFile.doesIDExist(j))
                                {
                                    ManureRecipient newRecipient = new ManureRecipient();
                                    newRecipient.setParens(parens + "_ManureRecipientI" + i.ToString() + "_ManureRecipientJ" + j.ToString());
                                    livestockFile.Identity.Add(j);
                                    newRecipient.setStorageType(livestockFile.getItemInt("StorageType"));
                                    newHouse.Recipient.Add(newRecipient);
                                    livestockFile.Identity.RemoveAt(livestockFile.Identity.Count - 1);
                                }
                            }
                            //Hack to remove surplus housing generated for pigs by user interface
                            if ((newHouse.GetHousingType() != 0) && (speciesGroup == 2))
                                livestockFile.Identity.RemoveAt(livestockFile.Identity.Count - 1);
                            housingDetails.Add(newHouse);
                        }
                        else
                        {
                            testPropTime = 1.0;
                            livestockFile.Identity.RemoveAt(livestockFile.Identity.Count - 1);
                        }
                    }
                }
                if (testPropTime != 1.0)
                {
                    string messageString = ("Sum of proportions of time in different housing does not equal 1.0 ");
                    GlobalVars.Instance.Error(messageString);
                }
            }
            //read livestock input variables from input xml file
            string feeditemPath = path + ".itemFed";
            min = 99;
            max = 0;
            livestockFile.setPath(feeditemPath);
            livestockFile.getSectionNumber(ref min, ref max);
            for (int i = min; i <= max; i++)
            {
                if (livestockFile.doesIDExist(i))
                {
                    //find the feed code for the first feed item

                    feedItem newFeedItem = new feedItem(feeditemPath, i, true,parens+"_"+i.ToString());
                    if (newFeedItem.GetFeedCode() == 926)
                        Console.Write("");
                    //if there is no housing or corralling, all feed is fed at pasture
                    if ((housingDetails.Count == 0) && (newFeedItem.GetisGrazed() == false))
                        newFeedItem.SetfedAtPasture(true);
                    feedRation.Add(newFeedItem);
                }
            }
        } //end if average number of animals >0
    }


/**
* these functions calculate energy demands using a specific energy system. the value calculated here should actually be read from file
*/
    double dailymaintenanceEnergy() //MJ per animal
    {
        double maintenanceEnergy = 0;
        switch (GlobalVars.Instance.getcurrentEnergySystem())
        {
            case 1:
            case 2:
                if (speciesGroup == 1)
                {
                    double efficiencyMaintenance = 0.02 * energyIntake / DMintake + 0.5;
                    double dailyEnergyIntake = energyIntake / GlobalVars.avgNumberOfDays;
                    if (age < 6.0)
                        maintenanceEnergy = maintenanceEnergyCoeff * (0.26 * Math.Pow(liveweight, 0.75) * Math.Exp(-0.03 * age)) / efficiencyMaintenance
                            + 0.09 * (energyIntake / GlobalVars.avgNumberOfDays);//SRC (1990) eq 1.21, minus EGRAZE and ECOLD
                    else
                        maintenanceEnergy = maintenanceEnergyCoeff * (0.26 * Math.Pow(liveweight, 0.75) * Math.Exp(-0.03 * 6.0)) / efficiencyMaintenance
                            + 0.09 * (energyIntake / GlobalVars.avgNumberOfDays);//SRC (1990) eq 1.21, minus EGRAZE and ECOLD
                }
                if (speciesGroup == 2)
                {
                    maintenanceEnergy = 0.44 * Math.Pow(liveweight, 0.75);
                }
                if (speciesGroup == 3)
                {
                    double efficiencyMaintenance = 0.02 * energyIntake / DMintake + 0.5;
                    double dailyEnergyIntake = energyIntake / GlobalVars.avgNumberOfDays;
                    if (age < 6.0)
                        maintenanceEnergy = maintenanceEnergyCoeff * (0.26 * Math.Pow(liveweight, 0.75) * Math.Exp(-0.03 * age)) / efficiencyMaintenance
                            + 0.09 * (energyIntake / GlobalVars.avgNumberOfDays);//SRC (1990) eq 1.21, minus EGRAZE and ECOLD
                    else
                        maintenanceEnergy = maintenanceEnergyCoeff * (0.26 * Math.Pow(liveweight, 0.75) * Math.Exp(-0.03 * 6.0)) / efficiencyMaintenance
                            + 0.09 * (energyIntake / GlobalVars.avgNumberOfDays);//SRC (1990) eq 1.21, minus EGRAZE and ECOLD
                }
                break;
      
            default: 
                    string messageString=("Energy system for " + name + " not found");
                    GlobalVars.Instance.Error(messageString);
                    break;

        }
        return maintenanceEnergy;
    }
/**
* daily Growth Energy Per kg
*/
    double dailyGrowthEnergyPerkg() ///<energy required for growth, MJ per kg
    {
        double growthEnergyPerkg = 0;///<efficiency of use of ME for growth, kA_growth, no units
        switch (GlobalVars.Instance.getcurrentEnergySystem())
        {
            case 1:
            case 2:
                if (speciesGroup == 1)
                {
                    double efficiencyGrowth = 0.042 * energyIntake / DMintake + 0.006;//SCA 1990 1.38,
                    growthEnergyPerkg = growthEnergyDemandCoeff / efficiencyGrowth;
                }
                if (speciesGroup==2)
                    growthEnergyPerkg= 47.0;
                break;

            default:
                string messageString=("Energy system for livestock not found");
                GlobalVars.Instance.Error(messageString);
                break;

        }
        return growthEnergyPerkg;
    }
/**
* daily Milk Energy Perk g
*/
    double dailyMilkEnergyPerkg()//MJ per kg
    {
        double milkEnergyPerkg = 0;
        double milkEnergyContentPerkg=0;///<energy content of energy-corrected milk, eA_milk, MJ/kg
        switch (speciesGroup)
        {
            case 1: milkEnergyContentPerkg = GlobalVars.Instance.GetECM(1, milkFat/10, milkNconc * 6.38 * 100) * 3.054;//Australian standards state 3.054 MJ/kg ECM
                break;
            case 2: break;
            case 3: milkEnergyContentPerkg = 0.0328 * milkFat + 0.0025 * 42 /*assume 6 weeks for day of lactation*/ + 2.203;
                break;
        }
        switch (GlobalVars.Instance.getcurrentEnergySystem())
        {
            case 1:
            case 2: double efficiencyMilk = (0.02 * energyIntake / DMintake + 0.4);//SCA 1990 1.48
                milkEnergyPerkg = milkAdjustmentCoeff * milkEnergyContentPerkg / efficiencyMilk; // milkAdjustmentCoeff is  Multiplier to increase (>1) or decrease (<1) the energy requirement per unit of milk produced
                break;
            default: 
                string messageString=("Energy system for livestock not found");
                GlobalVars.Instance.Error(messageString);
                break;
        }
        return milkEnergyPerkg;
    }
/**
* daily Energy Remobilisation
*/
    double dailyEnergyRemobilisation(double weightLoss)//MJ ME/day
    {
        double energyRemobilisation = weightLoss * growthEnergyDemandCoeff * 0.84;
        return energyRemobilisation;
    }
/**
* daily Weight Loss
*/
    double dailyWeightLoss(double remobilisedEnergy)//MJ ME/day
    {
        double weightLoss = remobilisedEnergy / (growthEnergyDemandCoeff * 0.84);
        return weightLoss;
    }
/**
* daily Energy For Grazing
*/
    double dailyEnergyForGrazing() //MJ ME/day
    {
        double retVal = 0;
        return retVal;
    }
/**
* calc Energy Level
*/
    void calcEnergyLevel()
    {
        energyLevel=energyIntake/(dailymaintenanceEnergy() * GlobalVars.avgNumberOfDays);
    }
/**
* daily Endogenous Faecal Protein
*/
    double dailyEndogenousFaecalProtein()//g per animal/day
    {
        double endogenousFaecalProtein = 0;
        endogenousFaecalProtein = 15.2 * DMintake / GlobalVars.avgNumberOfDays;
        return endogenousFaecalProtein;
    }
/**
* daily Faecal Protein
*/
    double dailyFaecalProtein()//g per animal per day - RedNex equation
    {
        double dailyDMI=DMintake/GlobalVars.avgNumberOfDays;
        double dailyNintake=1000*Nintake/GlobalVars.avgNumberOfDays;
        double faecalProtein = 0;
        if (dailyDMI < 5)
        {
            double faecalProtAtFiveKgDMI = 6.25 * (6.3 * 5 + 0.17 * 5.0 * (dailyNintake/dailyDMI) - 31.0);
            faecalProtein = (dailyDMI / 5.0) * faecalProtAtFiveKgDMI;
        }
        else
            faecalProtein = 6.25 * (6.3 * dailyDMI + 0.17 * dailyNintake - 31.0);
        //NOTE
        faecalProtein=(0.04*dailyNintake + (20 * dailyDMI/6.25) + (1.8*dailyDMI*dailyDMI)/6.25);  //Dk equation - gN/day
        faecalProtein *= 6.25;  //convert to protein
        return faecalProtein;
    }
/**
* daily Endogenous Urinary Protein
*/
    double dailyEndogenousUrinaryProtein()//g per animal/day
    {
        double endogenousUrinaryProtein = 0;
        switch (speciesGroup)
        {
            case 1:
                endogenousUrinaryProtein = 16.1 * Math.Log(liveweight) - 42.2;
                break;
            case 3:
                endogenousUrinaryProtein = 0.147 * liveweight + 3.375;
                break;
            default:
                string messageString1 = ("Protein system for livestock not found");
                GlobalVars.Instance.Error(messageString1);
                break;
        }
        return endogenousUrinaryProtein;
    }
/**
* dailym aintenance Protein
*/
    double dailymaintenanceProtein() //g per animal/day
    {
        double maintenanceProtein = 0;
        double endogenousUrinaryProtein = 0;
        double endogenousFaecalProtein = 0;
        switch (GlobalVars.Instance.getcurrentEnergySystem())
        {
            case 1:
            case 2:
                double efficiencyMaintenance = 0.7; //from Australian feeding standards
                endogenousUrinaryProtein= dailyEndogenousUrinaryProtein();
                endogenousFaecalProtein = dailyEndogenousFaecalProtein();
                maintenanceProtein = (endogenousUrinaryProtein + endogenousFaecalProtein) / efficiencyMaintenance;
                break;

            default:
                string messageString2 = ("Protein system for livestock not found");
                GlobalVars.Instance.Error(messageString2);
                break;

        }
        return maintenanceProtein;
    }
/**
* daily Milk Protein Per kg
*/
    double dailyMilkProteinPerkg(double dailyProteinAvailableForProduction)//g per kg
    {
        double milkProteinPerkg = 0;
        double milkProteinContentPerkg = 0;
        milkProteinContentPerkg = 1000.0 * milkNconc * 6.38; 
        switch (GlobalVars.Instance.getcurrentEnergySystem())
        {
            case 1:
            case 2: double dailyME = energyIntake / GlobalVars.avgNumberOfDays;
                    double Nmet = dailyProteinAvailableForProduction / 6.25;
                    
                    milkProteinPerkg = milkProteinContentPerkg / efficiencyProteinMilk;
                break;
            default:
                string messageString = ("Protein system for livestock not found");
                GlobalVars.Instance.Error(messageString);
                break;
        }

        return milkProteinPerkg;
    }
/**
* daily Growth Prote in Per kg
*/
    double dailyGrowthProteinPerkg() //g per kg
    {
        double growthProteinPerkg = 0;
        switch (GlobalVars.Instance.getcurrentEnergySystem())
        {
            case 1:
            case 2:
                if (speciesGroup == 1)
                {
                    double efficiencyGrowth = 0.7;
                    growthProteinPerkg = 1000.0 * growthNconc * 6.25 / efficiencyGrowth;
                }
                if (speciesGroup == 2) //need to make efficiency dependent on amino acids in diet
                {
                    double efficiencyGrowth = 0.7;
                    growthProteinPerkg = 1000.0 * growthNconc * 6.25 / efficiencyGrowth;
                }
                break;

            default:
                string messageString = ("Protein system for livestock not found");
                GlobalVars.Instance.Error(messageString);
                break;

        }
        return growthProteinPerkg;
    }
/**
* daily Protein Remobilisation
*/
    double dailyProteinRemobilisation(double weightLoss)//MJ ME/day
    {
        double proteinRemobilisation = 0;
        //if (isDairy)
            proteinRemobilisation = 0.7 * weightLoss * growthNconc;
        return proteinRemobilisation;
    }
/**
* Get maintenance Energy
*/
    public double GetmaintenanceEnergy()//MJ ME per year
    {
        double maintenanceEnergy = dailymaintenanceEnergy() * GlobalVars.avgNumberOfDays;
        return maintenanceEnergy;
    }
/**
*Get Growth Energy
*/
    public double GetGrowthEnergy()//MJ ME per year
    {
        double growthEnergy = avgProductionMeat * dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays;
        return growthEnergy;
    }
/**
*Get Milk Energy
*/
    public double GetMilkEnergy()//MJ ME per year
    {
        double milkEnergy = 0;
        if (isDairy)
            milkEnergy= avgProductionMilk * dailyMilkEnergyPerkg() * GlobalVars.avgNumberOfDays;
        return milkEnergy;
    }
/**
*Get maintenance Protein
*/
    public double GetmaintenanceProtein()//kg protein per year
    {
        double maintenanceProtein = dailymaintenanceProtein() * GlobalVars.avgNumberOfDays/1000.0;
        return maintenanceProtein;
    }
/**
*Get Growth Protein
*/
    public double GetGrowthProtein()//kg protein per year
    {
        double growthProtein = avgProductionMeat * dailyGrowthProteinPerkg() * GlobalVars.avgNumberOfDays / 1000.0;
        return growthProtein;
    }
/**
*Calc Energy Demand
*/
    public void CalcEnergyDemand()//MJ per year
    {
        calcEnergyLevel();
        energyDemand = GetmaintenanceEnergy() + GetGrowthEnergy() + GetMilkEnergy();
        energyIntake *= Getmu(energyLevel);
    }
/**
*Calc Maximum Production
*/
    public bool CalcMaximumProduction()//calculate daily production permitted by energy available
    {
        bool retVal = true;
        energyUseForMaintenance=0;
        energyUseForGrowth=0;
        energyUseForMilk=0;
        energyFromRemobilisation=0;
        maintenanceEnergyDeficit = 0;
       
        growthEnergyDeficit = 0;
        double proteinFromRemobilisation = 0;
        double initialEnergy = liveweight * growthEnergyDemandCoeff;
        calcEnergyLevel();
        //energyIntake is in MJ per animal per year
        double energyAvail = energyIntake * Getmu(energyLevel);///<EA_potprod
        double proteinSupply = Nintake * 6.25;
        double dailyMEintake=energyIntake/365;
        double protEnergyRatio =(proteinSupply*1000/365)/dailyMEintake;
        double faecalProtein = dailyFaecalProtein() * GlobalVars.avgNumberOfDays/1000.0; //kg per year
        faecalN = faecalProtein / 6.25;
        energyUseForMaintenance = dailymaintenanceEnergy() * GlobalVars.avgNumberOfDays;
        energyAvail -= energyUseForMaintenance;
        energyFromRemobilisation = 0;
        if (avgProductionMeat < 0.0)
        {
            energyFromRemobilisation = dailyEnergyRemobilisation(avgProductionMeat) * GlobalVars.avgNumberOfDays;
            proteinFromRemobilisation = dailyProteinRemobilisation(avgProductionMeat) * GlobalVars.avgNumberOfDays;
            liveweight-=avgProductionMeat;
        }
        energyAvail += energyFromRemobilisation;
        proteinSupply += proteinFromRemobilisation;
        double proteinAvailableForProduction = proteinSupply - faecalProtein;
        if ((energyAvail<0)||(proteinAvailableForProduction<0))//feeding below maintenance for either energy or protein
        {
            double weightlLoss=0;
            if (avgProductionMeat>0.0)//wanted growth but not enough energy or protein
                avgProductionMeat=0;
            if (energyAvail<0)//remoblise energy
            {
                double remobilisationForMaintenance = 0;///<energy gained from remobilised tissue, MJ/d, EA_remob is annual form
                remobilisationForMaintenance=Math.Abs(energyAvail);
                energyFromRemobilisation+=remobilisationForMaintenance;
                weightlLoss=dailyWeightLoss(remobilisationForMaintenance /GlobalVars.avgNumberOfDays);
                double associatedProteinRemob=dailyProteinRemobilisation(weightlLoss) * GlobalVars.avgNumberOfDays;
                proteinAvailableForProduction+=associatedProteinRemob;
                proteinFromRemobilisation += associatedProteinRemob;
                avgProductionMeat = -weightlLoss;
                liveweight -= weightlLoss;
                energyAvail = 0.0;
                if (liveweight < 0)
                {
                    if (GlobalVars.Instance.getRunFullModel())
                    {
                        string messageString = name + " - liveweight has fallen below zero!";
                        GlobalVars.Instance.Error(messageString);
                        retVal = false;
                    }
                    retVal = false;
                }
            }
            if (proteinAvailableForProduction<0.0) //need to remobilise protein
            {
                weightlLoss=Math.Abs(proteinAvailableForProduction)/(dailyGrowthProteinPerkg() * GlobalVars.avgNumberOfDays);
                liveweight -= avgProductionMeat;
                proteinLimited = true;
                if (liveweight < 0)
                {
                    if (GlobalVars.Instance.getRunFullModel())
                    {
                        string messageString = name + " - liveweight has fallen below zero!";
                        GlobalVars.Instance.Error(messageString);
                        retVal = false;
                    }
                    retVal = false;
                }
                else
                    proteinAvailableForProduction=0;
            }
        }
        if (isDairy)
        {
            energyUseForGrowth = 0;
            if (avgProductionMeat > 0)//these animals are growing
            {
                energyUseForGrowth = avgProductionMeat * dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays;
                double proteinRequiredForGrowth=GetGrowthProtein();
                if ((energyAvail < energyUseForGrowth)||(proteinAvailableForProduction < proteinRequiredForGrowth))  //need to reduce growth
                {
                    if (energyAvail < energyUseForGrowth)//reduce growth to match energy available
                    {
                        growthEnergyDeficit = -1 * (energyUseForGrowth - energyAvail);
                        avgProductionMeat = energyAvail / (dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays);///<LA_Elimit
                        energyUseForMilk = 0;
                        avgProductionMilk = 0;
                        energyAvail = 0;
                        proteinRequiredForGrowth=avgProductionMeat * dailyGrowthProteinPerkg() * GlobalVars.avgNumberOfDays;
                    }
                    if (proteinAvailableForProduction < proteinRequiredForGrowth)//reduce growth to match protein available
                    {
                        avgProductionMeat=proteinAvailableForProduction/(dailyGrowthProteinPerkg() * GlobalVars.avgNumberOfDays);//reduce growth to match available protein
                        proteinAvailableForProduction=0;
                        avgProductionMilk = 0;
                        proteinLimited = true;
                    }                    
                }
                else //enough energy and protein for milk production
                {
                    energyUseForMilk = energyAvail - energyUseForGrowth;
                    proteinAvailableForProduction -= proteinRequiredForGrowth; //enough protein to satisfy growth demand of dairy animals
                }
            }
            else //growth is zero or less
            {
                energyUseForMilk = energyAvail;
            }
            double thedailyMilkEnergyPerkg = dailyMilkEnergyPerkg();
            double energyLimitedMilk= energyUseForMilk / (thedailyMilkEnergyPerkg * GlobalVars.avgNumberOfDays);
            double dailyproteinAvailableForProduction = 1000 * proteinAvailableForProduction / GlobalVars.avgNumberOfDays;
            double proteinLimitedMilk = dailyproteinAvailableForProduction / dailyMilkProteinPerkg(dailyproteinAvailableForProduction) ;
            if (energyLimitedMilk < proteinLimitedMilk)
            {
                avgProductionMilk = energyLimitedMilk;
            }
            else
            {
                avgProductionMilk = proteinLimitedMilk;
                proteinLimited = true;
            }
            double milkDMcontent = 0.14;//approx value from feed tables
//            milk.Setenergy_conc(thedailyMilkEnergyPerkg / milkDMcontent);
  //          milk.Setamount(avgProductionMilk * milkDMcontent * avgNumberOfAnimal * GlobalVars.avgNumberOfDays);
    //        GlobalVars.Instance.AddProductProduced(milk);
            if (avgProductionMilk > 0.0)
            {
                double percentMilkProtein = (avgProductionMilk * milkNconc * 6.23 * 100.0) / avgProductionMilk;
                avgProductionECM = GlobalVars.Instance.GetECM(avgProductionMilk, (milkFat / 10.0), percentMilkProtein);
            }
            else
                avgProductionECM = 0;
            retVal = true;
        }
        else //these are meat animals
        {
            energyUseForMilk = 0;
         
            energyUseForGrowth = energyAvail - maintenanceEnergyDeficit;
            double energyLimitedGrowth=energyAvail / (dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays);
            double proteinLimitedGrowth=1000 * proteinAvailableForProduction / (dailyGrowthProteinPerkg() * GlobalVars.avgNumberOfDays);
            if (avgProductionMeat >= 0)
            {
                if (energyLimitedGrowth < proteinLimitedGrowth)
                {
                    avgProductionMeat = energyLimitedGrowth;
                }
                else
                {
                    avgProductionMeat = proteinLimitedGrowth;
                    proteinLimited = true;
                }
            }
            retVal = true;
        }
        return retVal;
    }
    /**
    * Get mu - calculate the reduction in energy availability with increasing intake rate (µA_g)
    */
    double Getmu(double energyLevel)
    {
        double mu = 1;
        //if (energyLevel > Energy_base)
          //  mu = 1 - mu_slope * (energyLevel - Energy_base);
        return mu;
    }
/**
* intake
*/
    public void intake()
    {
        concentrateEnergy = 0;
        for (int k = 0; k < feedRation.Count; k++)
        {
            feedItem anItem = feedRation[k];
            double amount = anItem.Getamount();
            DMintake += GlobalVars.avgNumberOfDays * amount;
            energyIntake += GlobalVars.avgNumberOfDays * amount * anItem.Getenergy_conc();
            diet_ash += GlobalVars.avgNumberOfDays * amount * anItem.Getash_conc();
            Nintake += GlobalVars.avgNumberOfDays * amount * anItem.GetN_conc();
            Cintake += GlobalVars.avgNumberOfDays * amount * anItem.GetC_conc();
            Pintake += GlobalVars.avgNumberOfDays * amount * anItem.GetP_conc();
            diet_fat += GlobalVars.avgNumberOfDays * amount * anItem.Getfat_conc();
            diet_fibre += GlobalVars.avgNumberOfDays * amount * anItem.Getfibre_conc();
            diet_NFE += GlobalVars.avgNumberOfDays * amount * anItem.GetnitrogenfreeExtract_conc();
            diet_nitrate += GlobalVars.avgNumberOfDays * amount * anItem.GetNitrate_conc();
            digestibilityDiet += amount * anItem.GetDMdigestibility();
        }
        digestibilityDiet /= (DMintake/ GlobalVars.avgNumberOfDays);
        
        for (int j = 0; j < feedRation.Count; j++)
        {
            if ((feedRation[j].GetisGrazed())||(feedRation[j].GetfedAtPasture()))
                DMgrazed += feedRation[j].Getamount() * GlobalVars.avgNumberOfDays;
        }
        timeOnPasture = DMgrazed / GetDMintake();
        concentrateDM = GetConcentrateDM() * GlobalVars.avgNumberOfDays;
        concentrateEnergy = GetConcentrateEnergy() * GlobalVars.avgNumberOfDays;
        FE = (0.75 * 1000 * (energyIntake / DMintake) - 1883) / 7720;
    }
/**
* enteric Methane
*/
    public double entericMethane()
    {
        double numDays = GlobalVars.avgNumberOfDays;
        double methane = 0; //initially in grams
        switch (GlobalVars.Instance.getcurrentInventorySystem())
        {
            case 1:
                double grossEnergyIntake = 18.45 * DMintake;
                methane = grossEnergyIntake * entericTier2MCF / 55.65;//1.13
                break;
            case 2:
            double dailyFibre =diet_fibre/numDays;
            double dailyCP = Nintake * GlobalVars.NtoCrudeProtein / numDays;
            double dailyNFE = diet_NFE / numDays;
            double dailyFat = diet_fat / numDays;
            double tot = dailyCP + dailyFat + dailyFibre + dailyNFE;
            double test = DMintake / numDays;
                
          methane =  63 + 79 * dailyFibre + 10 * dailyNFE
                + 26 * dailyCP - 212 * dailyFat;
          methane /= 1000.0;
          methane *= numDays;
          break;
        }
        double methane_reduction = 0;
        if (diet_nitrate > 0)
        {
            double mol_nitrate = diet_nitrate / 62; //mols of nitrate
            double mol_methane = methane / 16; //mols of methane
            methane_reduction = nitrateEfficiency * mol_nitrate / mol_methane;
            if (methane_reduction > 1)
                methane_reduction = 1.0;
        }
        methane *= (1-methane_reduction);
        return methane;
    }
/**
* Do Carbon
*/
    public void DoCarbon()
    {
        milkC = GlobalVars.avgNumberOfDays * avgProductionMilk * milkCconc;
        double totalGrowthC = GlobalVars.avgNumberOfDays * avgProductionMeat * growthCconc;
        mortalitiesC = mortalityCoefficient / 2 * totalGrowthC;
        growthC =  totalGrowthC - mortalitiesC;
        double ashConc =diet_ash/DMintake;
        if (isRuminant)
            faecalC = Cintake * (1 - Getmu(energyLevel) * digestibilityDiet)/(1-ashConc);
        else
            faecalC = Cintake * (1 - digestibilityDiet) / (1 - ashConc);
        urineC = urineProp * Cintake;
        CH4C = entericMethane() * 12 / 16;
        CO2C=Cintake - (milkC + growthC + mortalitiesC + faecalC + urineC + CH4C);
        CexcretionToPasture = timeOnPasture * (faecalC + urineC);
    }
/**
* Do Nitrogen
*/
    public void DoNitrogen()
    {
        milkN = GlobalVars.avgNumberOfDays * avgProductionMilk * milkNconc;
        double totalGrowthN = GlobalVars.avgNumberOfDays * avgProductionMeat * growthNconc;
        mortalitiesN = mortalityCoefficient / 2 * totalGrowthN;
        growthN = totalGrowthN - mortalitiesN;
        urineN = Nintake - (milkN + growthN + mortalitiesN + faecalN);
        if (urineN < 0.0)
        {
            Write();
            string message1 = "Error; urine N for " + name + " has gone negative";
            GlobalVars.Instance.Error(message1);
        }
        NexcretionToPasture = timeOnPasture * (faecalN + urineN);
    }
/**
    * Do Phosphorus
    */
    public void DoPhosphorus()
    {
        milkP = GlobalVars.avgNumberOfDays * avgProductionMilk * milkPconc;
        double totalGrowthP = GlobalVars.avgNumberOfDays * avgProductionMeat * growthPconc;
        mortalitiesP = mortalityCoefficient / 2 * totalGrowthP;
        growthP = totalGrowthP - mortalitiesP;
        switch (LivestockType)
        {
            case 1:
                //urine P based on B. G. Geisert, G. E. Erickson, T. J. Klopfenstein, C. N. Macken, M. K. Luebbe,  and J. C. MacDonald
                //urineP = 0.00755 * liveweight;
                excretaP = Pintake - (milkP + growthP + mortalitiesP);
                if (excretaP < 0.0)
                {
                    Write();
                    string message1 = "Error; faecal P for " + name + " has gone negative";
                    GlobalVars.Instance.Error(message1);
                }
                break;
            case 2:
            case 3:
                excretaP = Pintake - (milkP + growthP + mortalitiesP);
                break;
        }
    }
/**
* Do Ruminant
*/
    public void DoRuminant()
    {
        intake();
        CalcMaximumProduction();
        CalcEnergyDemand();
        DoCarbon();
        DoNitrogen();
        DoPhosphorus();
        GetExcretaDeposition();
    }
/**
* Get Excreta Deposition
*/
    public void GetExcretaDeposition()
    {
        double[] DM = new double[GlobalVars.Instance.getmaxNumberFeedItems()];
        for (int i = 0; i < GlobalVars.Instance.getmaxNumberFeedItems(); i++)
            DM[i] = 0;

        double sum = 0;
        for (int j = 0; j < feedRation.Count; j++)
        {
            if (feedRation[j].GetisGrazed())
            {
                int feedCode=feedRation[j].GetFeedCode();
                double temp = avgNumberOfAnimal *  GlobalVars.Instance.GetavgNumberOfDays() * feedRation[j].Getamount();
                grazedN += feedRation[j].Getamount() * feedRation[j].GetN_conc() * GlobalVars.Instance.GetavgNumberOfDays();
                grazedDM += feedRation[j].Getamount() * GlobalVars.Instance.GetavgNumberOfDays();
                DM[feedCode]+= temp ;
                sum += temp;
            }
        }
        double excretaN = 0;
        for (int i = 0; i < GlobalVars.Instance.getmaxNumberFeedItems(); i++)
        {
            if (DM[i]>0)
            {
                double theUrineN = timeOnPasture * avgNumberOfAnimal * urineN * DM[i] / sum;
                double theUrineC = timeOnPasture * avgNumberOfAnimal * urineC * DM[i] / sum;
                double theFaecalN = timeOnPasture * avgNumberOfAnimal * faecalN * DM[i] / sum;
                double theFaecalC = timeOnPasture * avgNumberOfAnimal * faecalC * DM[i] / sum;
                double theRuminantGrazedDM = DM[i];
                GlobalVars.Instance.grazedArray[i].urineC += theUrineC; 
                GlobalVars.Instance.grazedArray[i].urineN += theUrineN;
                GlobalVars.Instance.grazedArray[i].faecesC += theFaecalC;
                GlobalVars.Instance.grazedArray[i].faecesN += theFaecalN;
                GlobalVars.Instance.grazedArray[i].ruminantDMgrazed += theRuminantGrazedDM;
                excretaN += theFaecalN + theUrineN;
            }
        }
        if ((excretaN==0) && (housingDetails.Count == 0))
        {
            string message1 = "Error; animals are fed at pasture only but no pasture is consumed. Livestock name = " + name;
            GlobalVars.Instance.Error(message1);
        }
    }
/**
* Get All Feed Items Used
*/
    public void GetAllFeedItemsUsed()
    {
        for (int i = 0; i < GlobalVars.Instance.getmaxNumberFeedItems(); i++)
        {
            for (int j = 0; j < feedRation.Count; j++)
                 if (feedRation[j].GetFeedCode() == i) 
                 {
                    feedItem afeedItem = new feedItem(feedRation[j]);
                    afeedItem.setFeedCode(i);
                    afeedItem.AddFeedItem(feedRation[j], false);
                    afeedItem.Setamount(avgNumberOfAnimal * GlobalVars.Instance.GetavgNumberOfDays() * feedRation[j].Getamount());
/*                    if ((afeedItem.GetisGrazed())&&(GlobalVars.Instance.GetstrictGrazing()))
                   {
                        afeedItem.Setname(afeedItem.GetName() + ", grazed");
                        afeedItem.setFeedCode(afeedItem.GetFeedCode() + 1000);
                        GlobalVars.Instance.allFeedAndProductsUsed[i+1000].composition.AddFeedItem(afeedItem, false);
                    }
                    else*/
                        GlobalVars.Instance.allFeedAndProductsUsed[i].composition.AddFeedItem(afeedItem, false);
                    //break;
                }
        }
    }
/**
* Check Livestock C Balance
*/
    public bool CheckLivestockCBalance()
    {
        bool retVal = false;
        double Cout = urineC + growthC + faecalC + milkC + mortalitiesC;
        double CLost = CH4C + CO2C;
        double Cbalance = Cintake - (Cout + CLost);
        double diff = Cbalance / Cintake;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
           
                double errorPercent = 100 * diff;
               
                string messageString=("Error; Livestock C balance error is more than the permitted margin of "
                    + tolerance.ToString() +"\n");
                messageString+=("Percentage error = " + errorPercent.ToString("0.00") + "%");
                GlobalVars.Instance.Error(messageString);
        }
        return retVal;
    }
/**
* Check Livestock N Balances
*/
    public bool CheckLivestockNBalances()
    {
        bool retVal = false;
        double Nout = urineN + growthN + faecalN + milkN + mortalitiesN;
        double Nbalance = Nintake - Nout;
        double diff = Nbalance / Nintake;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
                double errorPercent = 100 * diff;
                string messageString = ("Error; Livestock N balance error is more than the permitted margin of "
                    + tolerance.ToString() + "\n");
                messageString += ("Percentage error = " + errorPercent.ToString("0.00") + "%");
                GlobalVars.Instance.Error(messageString);  
        }
        return retVal;
    }
/**
* write
*/
    public void Write()
    {
        double numofDaysInYear = GlobalVars.avgNumberOfDays;
        GlobalVars.Instance.writeStartTab("LiveStock");
        GlobalVars.Instance.writeInformationToFiles("nameLiveStock", "Name", "-", name, parens);
        GlobalVars.Instance.writeInformationToFiles("waterIntake", "waterIntake", "-", waterIntake, parens);
        GlobalVars.Instance.writeInformationToFiles("waterMetabolic", "waterMetabolic", "-", waterMetabolic, parens);
        GlobalVars.Instance.writeInformationToFiles("waterExhaled", "waterExhaled", "-", waterExhaled, parens);
        GlobalVars.Instance.writeInformationToFiles("waterExcreta", "waterExcreta", "-", waterExcreta, parens);
        GlobalVars.Instance.writeInformationToFiles("Pintake", "Pintake", "-", Pintake, parens);
        GlobalVars.Instance.writeInformationToFiles("meatP", "growthP", "-", growthP, parens);
        GlobalVars.Instance.writeInformationToFiles("milkP", "milkP", "-", milkP, parens);
        GlobalVars.Instance.writeInformationToFiles("excretaP", "excretaP", "-", excretaP, parens);
        GlobalVars.Instance.writeInformationToFiles("salePriceMeat", "salePriceMeat", "-", salePriceMeat, parens);
        GlobalVars.Instance.writeInformationToFiles("salePriceMilk", "salePriceMilk", "-", salePriceMilk, parens);
        GlobalVars.Instance.writeInformationToFiles("speciesGroup", "Species identifier", "-", speciesGroup, parens);
        GlobalVars.Instance.writeInformationToFiles("LivestockType", "Livestock type", "", LivestockType, parens);
        GlobalVars.Instance.writeInformationToFiles("liveweight", "Liveweight", "kg", liveweight, parens);
        GlobalVars.Instance.writeInformationToFiles("isRuminant", "Is a ruminant", "-", isRuminant, parens);
        GlobalVars.Instance.writeInformationToFiles("avgNumberOfAnimal", "Annual average number of animals", "-", avgNumberOfAnimal, parens);

        GlobalVars.Instance.writeInformationToFiles("DMintake", "Intake of DM", "kg/day", DMintake / numofDaysInYear, parens);
        GlobalVars.Instance.writeInformationToFiles("energyIntake", "Intake of energy", "MJ/day", energyIntake / numofDaysInYear, parens);
        GlobalVars.Instance.writeInformationToFiles("energyUseForGrowth", "Energy used for growth", "MJ/day", energyUseForGrowth / numofDaysInYear, parens);
        GlobalVars.Instance.writeInformationToFiles("energyUseForMilk", "Energy used for milk production", "MJ/day", energyUseForMilk / numofDaysInYear, parens);
        GlobalVars.Instance.writeInformationToFiles("energyFromRemobilisation", "Energy supplied by remobilisation", "MJ/day", energyFromRemobilisation / numofDaysInYear, parens);
        GlobalVars.Instance.writeInformationToFiles("energyUseForMaintenance", "Energy used for maintenance", "MJ/day", energyUseForMaintenance / numofDaysInYear, parens);
        GlobalVars.Instance.writeInformationToFiles("maintenanceEnergyDeficit", "Maintenance energy deficit", "MJ/day", maintenanceEnergyDeficit / numofDaysInYear, parens);
        GlobalVars.Instance.writeInformationToFiles("growthEnergyDeficit", "Growth energy deficit", "MJ/day", growthEnergyDeficit / numofDaysInYear, parens);
        //GlobalVars.Instance.writeInformationToFiles("milkEnergyDeficit", "Deficit in energy required for milk production", "MJ", milkEnergyDeficit);

        GlobalVars.Instance.writeInformationToFiles("diet_ash", "Ash in diet", "kg", diet_ash, parens);
        GlobalVars.Instance.writeInformationToFiles("diet_fibre", "Fibre in diet", "kg", diet_fibre, parens);
        GlobalVars.Instance.writeInformationToFiles("diet_fat", "Fat in diet", "kg", diet_fat, parens);
        GlobalVars.Instance.writeInformationToFiles("diet_NFE", "NFE  in diet", "kg", diet_NFE, parens);
        GlobalVars.Instance.writeInformationToFiles("digestibilityDiet", "Diet DM digestibility", "kg/kg", digestibilityDiet, parens);

        GlobalVars.Instance.writeInformationToFiles("Cintake", "Intake of C", "kg", Cintake, parens);
        GlobalVars.Instance.writeInformationToFiles("milkC", "C in milk", "kg", milkC, parens);
        GlobalVars.Instance.writeInformationToFiles("growthC", "C in growth", "kg", growthC, parens);
        GlobalVars.Instance.writeInformationToFiles("urineCLiveStock", "C in urine", "kg", urineC, parens);
        GlobalVars.Instance.writeInformationToFiles("faecalCLiveStock", "C in faeces", "kg", faecalC, parens);
        GlobalVars.Instance.writeInformationToFiles("CH4C", "CH4-C emitted", "kg", CH4C, parens);
        GlobalVars.Instance.writeInformationToFiles("CO2C", "CO2-C emitted", "kg", CO2C, parens);
        //GlobalVars.Instance.writeInformationToFiles("energyLevel", "??", "??", energyLevel);
        GlobalVars.Instance.writeInformationToFiles("Nintake", "Intake of N", "kg", Nintake, parens);
        GlobalVars.Instance.writeInformationToFiles("milkN", "N in milk", "kg", milkN, parens);
        GlobalVars.Instance.writeInformationToFiles("growthN", "N in growth", "kg", growthN, parens);
        GlobalVars.Instance.writeInformationToFiles("mortalitiesN", "N in mortalities", "kg", mortalitiesN, parens);
        GlobalVars.Instance.writeInformationToFiles("urineN", "N in urine", "kg", urineN, parens);
        GlobalVars.Instance.writeInformationToFiles("faecalN", "N in faeces", "kg", faecalN, parens);

        GlobalVars.Instance.writeInformationToFiles("avgDailyProductionMilk", "Average daily milk production", "kg/day", avgProductionMilk, parens);
        double temp = avgProductionMilk * GlobalVars.avgNumberOfDays;
        GlobalVars.Instance.writeInformationToFiles("avgProductionMilk", "Average yearly milk production", "kg", temp, parens);
        if (avgProductionMilk > 0.0)
        {
            double percentMilkProtein = (milkN * 6.23 * 100.0) / (avgProductionMilk * GlobalVars.avgNumberOfDays);
            avgProductionECM = GlobalVars.Instance.GetECM(avgProductionMilk, (milkFat / 10.0), percentMilkProtein);
        }
        else
            avgProductionECM = 0;
        GlobalVars.Instance.writeInformationToFiles("avgProductionECM", "Average energy-corrected milk production", "kg/day", avgProductionECM * 365.0, parens);
        GlobalVars.Instance.writeInformationToFiles("avgDailyProductionECM", "Average daily energy-corrected milk production", "kg/day", avgProductionECM, parens);
        GlobalVars.Instance.writeInformationToFiles("avgProductionMeat", "Average weight change", "g/day", avgProductionMeat * 1000.0, parens);
        //GlobalVars.Instance.writeInformationToFiles("housedDuringGrazing", "??", "??", housedDuringGrazing);
        for (int i = 0; i < housingDetails.Count; i++)
            housingDetails[i].Write();
        if (!GlobalVars.Instance.getRunFullModel())
            GlobalVars.Instance.writeEndTab();
    }
/**
* Do PigEnergy
*/
    public void DoPigEnergy()
    {
        double dailyenergyIntake = energyIntake/GlobalVars.avgNumberOfDays;
        double dailydiet_ash = diet_ash/GlobalVars.avgNumberOfDays;
        double dailydiet_fat = diet_fat/GlobalVars.avgNumberOfDays;
    }

/**
* Return pig maintenance energy requirement (MJ/day)
*/
    public double pigMaintenanceEnergy()
    {
        double retVal = 0.44 * Math.Pow(liveweight, 0.75);
        return retVal;
    }


/**
* return change in weight in kg. energyAvailable is in MJ 
*/
    public double GetPigGrowth(double energyAvailable)
    {
        double retVal = energyAvailable / 44.35;
        return retVal;
    }
/**
* pig Lactation Energy
*/
    public double pigLactationEnergy(double numPiglets, double birthWt, double weanedWt, double duration)
    {
        double retVal = 0;
        double growthRate = (weanedWt - birthWt) / duration;
        retVal = 4.184 * numPiglets * (6.83 * ((weanedWt - birthWt) / duration) - 125);
        return retVal;
    }
/**
* Do Pig
*/
    public void DoPig()
    {
        intake();
        switch (LivestockType)
        {
            case 1:
                break;
            case 2:
            case 3:
                DoGrowingPigs();
                break;
        }
    }
/**
* Do Growing Pigs
*/
    public void DoGrowingPigs()
    {
        double FE = (0.75 * 1000 * (energyIntake / DMintake) - 1883) / 7720;
        double FEIntake = DMintake * FE;
        if (LivestockType == 2)
        {
            startWeight = 30.0;
            duration = 76.0;
        }
        if (LivestockType == 3)
        {
            startWeight = 7.0;
            duration = 42.0;
        }
        avgProductionMeat = FEIntake / (2.85 * GlobalVars.avgNumberOfDays);
        CalcMaximumPigProduction();
        if ((startWeight>0)&&(duration>0))
            endWeight=startWeight + duration*avgProductionMeat;
        DoCarbon();
        faecalN = Nintake * (1 - digestibilityDiet);  //in kg/yr, assumes protein digestib = DM digestib
        DoNitrogen();
        DoPhosphorus();
        double FEperKgPigProduced = (duration * FEIntake / GlobalVars.avgNumberOfDays) / (endWeight - startWeight);
        double ProteinperFE = 6.25*Nintake/FEIntake ;
        double NperPigProduced = duration * (faecalN + urineN) / GlobalVars.avgNumberOfDays;
    }
/**
* calculate daily production permitted by energy available
*/
    public bool CalcMaximumPigProduction()
    {
        bool retVal = true;
        energyUseForMaintenance = 0;
        energyUseForGrowth = 0;
        energyUseForMilk = 0;
        energyFromRemobilisation = 0;
        maintenanceEnergyDeficit = 0;
        
        growthEnergyDeficit = 0;
        double proteinFromRemobilisation = 0;
        double energyAvail = energyIntake;//energyIntake is in MJ per animal per year
        double proteinSupply = Nintake * 6.25;
        faecalN =  Nintake * (1 - digestibilityDiet);  //in kg/yr, assumes protein digestib = DM digestib
        double faecalProtein = faecalN * 6.25;
        energyUseForMaintenance = dailymaintenanceEnergy() * GlobalVars.avgNumberOfDays;
        energyAvail -= energyUseForMaintenance;
        energyFromRemobilisation = 0;
        if (avgProductionMeat < 0.0)
        {
            energyFromRemobilisation = dailyEnergyRemobilisation(avgProductionMeat) * GlobalVars.avgNumberOfDays;
            proteinFromRemobilisation = dailyProteinRemobilisation(avgProductionMeat) * GlobalVars.avgNumberOfDays;
            liveweight -= avgProductionMeat;
        }
        energyAvail += energyFromRemobilisation;
        proteinSupply += proteinFromRemobilisation;
        double proteinAvailableForProduction = proteinSupply - faecalProtein;
        if ((energyAvail < 0) || (proteinAvailableForProduction < 0))//feeding below maintenance for either energy or protein
        {
            double weightlLoss = 0;
            if (avgProductionMeat > 0.0)//wanted growth but not enough energy or protein
                avgProductionMeat = 0;
            if (energyAvail < 0)//remoblise energy
            {
                double remobilisationForMaintenance = 0;
                remobilisationForMaintenance = Math.Abs(energyAvail);
                energyFromRemobilisation += remobilisationForMaintenance;
                weightlLoss = dailyWeightLoss(remobilisationForMaintenance / GlobalVars.avgNumberOfDays);
                double associatedProteinRemob = dailyProteinRemobilisation(weightlLoss) * GlobalVars.avgNumberOfDays;
                proteinAvailableForProduction += associatedProteinRemob;
                proteinFromRemobilisation += associatedProteinRemob;
                avgProductionMeat = -weightlLoss;
                liveweight -= weightlLoss;
                energyAvail = 0.0;
                if (liveweight < 0)
                {
                    if (GlobalVars.Instance.getRunFullModel())
                    {
                        string messageString = name + " - liveweight has fallen below zero!";
                        GlobalVars.Instance.Error(messageString);
                        retVal = false;
                    }
                    retVal = false;
                }
            }
            if (proteinAvailableForProduction < 0.0) //need to remobilise protein
            {
                weightlLoss = Math.Abs(proteinAvailableForProduction) / (dailyGrowthProteinPerkg() * GlobalVars.avgNumberOfDays);
                liveweight -= avgProductionMeat;
                if (liveweight < 0)
                {
                    if (GlobalVars.Instance.getRunFullModel())
                    {
                        string messageString = name + " - liveweight has fallen below zero!";
                        GlobalVars.Instance.Error(messageString);
                        retVal = false;
                    }
                    retVal = false;
                }
                else
                    proteinAvailableForProduction = 0;
            }
        }
        if (isDairy)
        {
            energyUseForGrowth = 0;
            if (avgProductionMeat > 0)//these animals are growing
            {
                energyUseForGrowth = avgProductionMeat * dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays;
                double proteinRequiredForGrowth = GetGrowthProtein();
                if ((energyAvail < energyUseForGrowth) || (proteinAvailableForProduction < proteinRequiredForGrowth))  //need to reduce growth
                {
                    if (energyAvail < energyUseForGrowth)//reduce growth to match energy available
                    {
                        growthEnergyDeficit = -1 * (energyUseForGrowth - energyAvail);
                        avgProductionMeat = energyAvail / (dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays);
                        energyUseForMilk = 0;
                        avgProductionMilk = 0;
                        energyAvail = 0;
                        proteinRequiredForGrowth = avgProductionMeat * dailyGrowthProteinPerkg() * GlobalVars.avgNumberOfDays;
                    }
                    if (proteinAvailableForProduction < proteinRequiredForGrowth)//reduce growth to match protein available
                    {
                        avgProductionMeat = proteinAvailableForProduction / (dailyGrowthProteinPerkg() * GlobalVars.avgNumberOfDays);//reduce growth to match available protein
                        proteinAvailableForProduction = 0;
                        avgProductionMilk = 0;
                    }
                }
                else //enough energy and protein for milk production
                {
                    energyUseForMilk = energyAvail - energyUseForGrowth;
                    proteinAvailableForProduction -= proteinRequiredForGrowth; //enough protein to satisfy growth demand of dairy animals
                }
            }
            else //growth is zero or less
                energyUseForMilk = energyAvail;
            double energyLimitedMilk = energyUseForMilk / (dailyMilkEnergyPerkg() * GlobalVars.avgNumberOfDays);
            double dailyproteinAvailableForProduction = 1000 * proteinAvailableForProduction / GlobalVars.avgNumberOfDays;
            double proteinLimitedMilk = dailyproteinAvailableForProduction / dailyMilkProteinPerkg(dailyproteinAvailableForProduction);
            if (energyLimitedMilk < proteinLimitedMilk)
                avgProductionMilk = energyLimitedMilk;
            else
                avgProductionMilk = proteinLimitedMilk;
            retVal = true;
        }
        else //these are meat animals
        {
            energyUseForMilk = 0;
            energyUseForGrowth = energyAvail - maintenanceEnergyDeficit;
            double energyLimitedGrowth = energyAvail / (dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays);
            double proteinLimitedGrowth = 1000 * proteinAvailableForProduction / (dailyGrowthProteinPerkg() * GlobalVars.avgNumberOfDays);
            if (avgProductionMeat >= 0)
            {
                if (energyLimitedGrowth < proteinLimitedGrowth)
                    avgProductionMeat = energyLimitedGrowth;
                else
                    avgProductionMeat = proteinLimitedGrowth;
            }
            retVal = true;
        }
        return retVal;
    }
/**
* Get Concentrate DM
*/
    public double GetConcentrateDM()
    {
        double retVal = 0;
        for (int i = 0; i < feedRation.Count; i++)
        {
            feedItem afeedItem = feedRation[i];
            if ((afeedItem.GetFeedCode() < 348) || ((afeedItem.GetFeedCode() >= 908) && afeedItem.GetFeedCode() <= 911))
                retVal += afeedItem.Getamount();
        }
        return retVal;
    }
/**
* Get Concentrate Energy
*/
    public double GetConcentrateEnergy()
    {
        double retVal = 0;
        for (int i = 0; i < feedRation.Count; i++)
        {
            feedItem afeedItem = feedRation[i];
            if ((afeedItem.GetFeedCode() < 348) || ((afeedItem.GetFeedCode() >= 908) && afeedItem.GetFeedCode() <= 911))
                
                retVal += afeedItem.Getamount() * afeedItem.Getenergy_conc();
        }
        return retVal;
    }
/**
* Write Livestock File
*/
    public void WriteLivestockFile()
    {
        int times = 1;
        if (GlobalVars.Instance.headerLivestock == false)
            times = 2;
        for (int j = 0; j < times; j++)
        {
            GlobalVars.Instance.writeLivestockFile("name", "name", "-", name, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("avgNumberOfAnimal", "avgNumberOfAnimal", "-", avgNumberOfAnimal, "livestock", 0);

            GlobalVars.Instance.writeLivestockFile("diet_ash", "diet_ash", "kg/day", diet_ash / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("diet_fibre", "diet_fibre", "kg/day", diet_fibre / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("diet_fat", "diet_fat", "kg/day", diet_fat / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("diet_NFE", "diet_NFE", "kg/day", diet_NFE / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("diet_nitrate", "diet_nitrate", "kg/day", diet_nitrate / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("digestibilityDiet", "digestibilityDiet", "-", digestibilityDiet, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("energyIntake", "energyIntake", "MJ ME/day", energyIntake / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("energyUseForMaintenance", "energyUseForMaintenance", "MJ ME/day", energyUseForMaintenance / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("energyUseForGrowth", "energyUseForGrowth", "MJ ME/day", energyUseForGrowth / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("energyUseForMilk", "energyUseForMilk", "MJ ME/day", energyUseForMilk / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("energyUseForGrazing", "energyUseForGrazing", "MJ ME/day", energyUseForGrazing / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("energyFromRemobilisation", "energyFromRemobilisation", "MJ ME/day", energyFromRemobilisation / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("maintenanceEnergyDeficit", "maintenanceEnergyDeficit", "MJ ME/day", maintenanceEnergyDeficit / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("growthEnergyDeficit", "growthEnergyDeficit", "MJ ME/day", growthEnergyDeficit / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("energyLevel", "energyLevel", "-", energyLevel, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("DMintake", "DMintake", "kg/day", DMintake / GlobalVars.avgNumberOfDays, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("concentrateDM", "concentrateDM", "kg/day", concentrateDM, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("grazedDM", "grazedDM", "kg/day", grazedDM / GlobalVars.avgNumberOfDays, "livestock", 0);
            if (proteinLimited)
                GlobalVars.Instance.writeLivestockFile("proteinLimited", "proteinLimited", "-", 1, "livestock", 0);
            else
                GlobalVars.Instance.writeLivestockFile("proteinLimited", "proteinLimited", "-", 0, "livestock", 0);

            GlobalVars.Instance.writeLivestockFile("Cintake", "Cintake", "kg/yr", Cintake, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("milkC", "milkC", "kg/yr", milkC, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("growthC", "growthC", "kg/yr", growthC, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("mortalitiesC", "mortalitiesC", "kg/yr", mortalitiesC, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("urineC", "urineC", "kg/yr", urineC, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("faecalC", "faecalC", "kg/yr", faecalC, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("CexcretionToPasture", "CexcretionToPasture", "kg/yr", CexcretionToPasture, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("LiveCH4C", "LiveCH4C", "kg/yr", CH4C, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("LiveCO2C", "LiveCO2C", "kg/yr", CO2C, "livestock", 0);

            GlobalVars.Instance.writeLivestockFile("Nintake", "Nintake", "kg/yr", Nintake, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("milkN", "milkN", "kg/yr", milkN, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("growthN", "growthN", "kg/yr", growthN, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("mortalitiesN", "mortalitiesN", "kg/yr", mortalitiesN, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("urineN", "urineN", "kg/yr", urineN, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("faecalN", "faecalN", "kg/yr", faecalN, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("NexcretionToPasture", "NexcretionToPasture", "kg/yr", NexcretionToPasture, "livestock", 0);

            GlobalVars.Instance.writeLivestockFile("avgProductionMeat", "avgProductionMeat", "g/day", avgProductionMeat*1000, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("avgProductionMilk", "avgProductionMilk", "kg/day", avgProductionMilk, "livestock", 0);
            GlobalVars.Instance.writeLivestockFile("avgProductionECM", "avgProductionECM", "kg/day", avgProductionECM, "livestock", 1);
            GlobalVars.Instance.headerLivestock = true;

        }
    }
}
