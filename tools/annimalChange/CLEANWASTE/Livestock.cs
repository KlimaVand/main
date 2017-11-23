using System;
using System.Collections.Generic;
using System.Xml;
public class livestock
{
    public struct ManureRecipient
    {
        int StorageType;
        public void setStorageType(int aType) { StorageType = aType; }
        public int GetStorageType() { return StorageType; }
        public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
        {
            writer.WriteStartElement("ManureRecipient");

            writer.WriteStartElement("StorageType");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteEndElement();
            writer.WriteValue("StorageType");
            writer.WriteStartElement("Value");
            writer.WriteValue(StorageType);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteEndElement();
            tabFile.Write("ManureRecipient" + '\n');
            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Nbalance" + "\t");
            tabFile.Write("StorageType" + "\t");


        }
    }
    public struct housingRecord
    {
        int HousingType;
        double propTime;
        string NameOfHousing;
        public List<ManureRecipient> Recipient;
        public void SetNameOfHousing(string aName) { NameOfHousing = aName; }
        public void SetHousingType(int aVal) { HousingType = aVal; }
        public void SetpropTime(double aVal) { propTime = aVal; }
        public int GetHousingType() { return HousingType; }
        public double GetpropTime() { return propTime; }
        public List<ManureRecipient> GetManureRecipient() { return Recipient; }
        public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
        {
            writer.WriteStartElement("housingRecord");

            writer.WriteStartElement("HousingType");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("HousingType");
            writer.WriteValue("Ninput");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(HousingType);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("propTime");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("propTime");
            writer.WriteValue("Ninput");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(propTime);
            writer.WriteEndElement();
            writer.WriteEndElement();


            tabFile.Write("housingRecord"+'\n');

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
            tabFile.Write("propTime" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(propTime.ToString() + "\n");



            for (int i=0; i < Recipient.Count; i++)
                Recipient[i].Write(writer,tabFile);
            writer.WriteEndElement();
        }
    }
    public struct item
    {
        int feedCode;
        double amount;
        public void SetfeedCode(int aVal) { feedCode = aVal; }
        public void Setamount(double aVal) { amount = aVal; }
        public int GetfeedCode() { return feedCode; }
        public double Getamount() { return amount; }
    }

    string path;

    //characteristics of livestock
    bool isRuminant;
    bool isDairy; // true if this is a milk-producing animal
    double mu_base;   //energy intake level below which there is no reduction in energy utilisation
    double mu_b; //
    //input data
    double avgNumberOfAnimal;

    double avgProductionMilk;
    double avgProductionMeat;
    List<housingRecord> housingDetails;
    //parameters
    int identity;
    int speciesGroup;
    int LivestockType; //finding parameter for this type
    double liveweight;
    double urineProp;
    string name;
    double growthNconc;
    double growthCconc;
    double milkNconc;
    double milkCconc;
    double age;
    double maintenanceEnergyCoeff;
    double growthEnergyDemandCoeff;
    double milkEnergyDemandCoeff;
    bool housedDuringGrazing;
    //other variables
    double energyIntake;
    double energyDemand;
    double energyUseForMaintenance;
    double energyUseForGrowth;
    double energyUseForMilk;
    double energyUseForGrazing;
    double energyFromRemobilisation;
    double maintenanceEnergyDeficit;
    double growthEnergyDeficit;
    double milkEnergyDeficit;
    double DMintake;
    double Nintake;
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
    double urineC;
    double urineN;
    double faecalC;
    double faecalN;
    double CH4C;
    double CO2C;
    double mexp=0; //Mass of manure from species group sg 
    double cman=0;// Mass of manure from species group sg
    double nman = 0;
    double vsg=0;//Annual production of manure from species group sg and store type s
    public double getCH4C() { return CH4C; }
    public double getCO2C() { return CO2C; }
    public double getCmanExp() { return mexp * cman * vsg; }
    public double getNmanExp() { return mexp * nman * vsg; }
    public double timeOnPasture;
    public void SetavgProductionMilk(double anavgProductionMilk) { avgProductionMilk = anavgProductionMilk; }
    public void SetavgProductionMeat(double anavgProductionMeat) { avgProductionMeat = anavgProductionMeat; }
    public void Setname(string aname) { name = aname; }
    public void Setidentity(int aValue) { identity = aValue; }
    public void SetspeciesGroup(int aValue) { speciesGroup = aValue; }
    public double GetMilkC() { return milkC; }
    public double GetMeatC() { return growthC; }
    public double GetMilkN() { return milkN; }
    public double GetGrowthN() { return growthN; }
    public bool GethousedDuringGrazing() { return housedDuringGrazing; }
    public void SetisRuminant(bool aVal){isRuminant=aVal;}
    List<feedItem> feedRation;
    public void SetfeedRation(List<feedItem> afeedRation) { feedRation = afeedRation; }

    public void SethousingDetail(int houseID, double proportion)
    {
        housingRecord ahousingRecord = new housingRecord();
        ahousingRecord.SetHousingType(houseID);
        ahousingRecord.SetpropTime(proportion);
        housingDetails.Add(ahousingRecord);
    }
    public bool GetisRuminant() { return isRuminant; }
    public double Getliveweight() { return liveweight; }
    public double GetDMintake() { return DMintake; }
    public double GetCintake() { return Cintake; }
    public double GetNintake() { return Nintake; }
    public double GeturineC() { return urineC; }
    public double GetfaecalC() { return faecalC; }
    public double GetFibre() { return diet_fibre; }
    public double GeturineN() { return urineN; }
    public double GetfaecalN() { return faecalN; }
    public double GetAvgNumberOfAnimal(){return avgNumberOfAnimal;}
    public double GettimeOnPasture() { return timeOnPasture; }
    public List<feedItem> GetfeedRation() { return feedRation; }
    
    public string Getname() { return name; }
    public int Getidentity() { return identity; }
    public int GetspeciesGroup() { return speciesGroup; }
    
    public List<housingRecord> GethousingDetails() { return housingDetails; }
    public livestock()
    {
    }
    public livestock(string aPath, int id, int zoneNr, int AnimalNr)
    {
        FileInformation livestockFile =new FileInformation(GlobalVars.Instance.getFarmFilePath());
        identity = id;
        path = aPath+"("+id.ToString()+")";
        livestockFile.setPath(path);
        feedRation = new List<feedItem>();
        urineProp = 0;
        DMintake =0;
        energyDemand = 0;
        energyIntake = 0;
        diet_ash = 0;
        diet_nitrate = 0;
        digestibilityDiet = 0;
        timeOnPasture = 0;
        name = livestockFile.getItemString("NameOfAnimals");
        avgNumberOfAnimal = livestockFile.getItemDouble("NumberOfAnimals");
        LivestockType= livestockFile.getItemInt("LivestockType");
        speciesGroup= livestockFile.getItemInt("Species_group");
  
        
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
                string testPath=basePath +  "(" + i.ToString() + ").LivestockType(-1)";
                int testLivestockType = paramFile.getItemInt("Value", testPath);
                testPath = basePath + "(" + i.ToString() + ").SpeciesGroup(-1)";
                int testspeciesGroup = paramFile.getItemInt("Value");
                if ((testLivestockType == LivestockType) && (testspeciesGroup == speciesGroup))
                {
                    livestockID = i;
                    gotit = true;
                }
            }
        }
        if (gotit==false)
        {
            System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.WriteLine("livestocktype not found in constants.xml");
            file.Close();
            Console.WriteLine("livestocktype not found in constants.xml");
            if (GlobalVars.Instance.getPauseBeforeExit())
                Console.Read();
            Environment.Exit(0);
        }
        basePath = "AgroecologicalZone(" + zoneNr.ToString() + ").Livestock(" + Convert.ToInt32(livestockID) + ")";
        //paramFile.setPath(basePath + ".SpeciesGroup(0)");
        //speciesGroup = paramFile.getItemInt("Value");
        paramFile.setPath(basePath + ".isRuminant(0)");
        isRuminant = paramFile.getItemBool("Value");
        paramFile.setPath(basePath + ".isDairy(0)");
        isDairy = paramFile.getItemBool("Value");
        paramFile.setPath(basePath + ".growthNconc(0)");
        growthNconc = paramFile.getItemDouble("Value"); //26%/6.25
        paramFile.setPath(basePath + ".growthCconc(0)");
        growthCconc = paramFile.getItemDouble("Value");
        paramFile.setPath(basePath + ".urineProp(0)");
        urineProp = paramFile.getItemDouble("Value");
        paramFile.setPath(basePath + ".maintenanceEnergyCoeff(0)");
        maintenanceEnergyCoeff = paramFile.getItemDouble("Value");
        paramFile.setPath(basePath + ".growthEnergyDemandCoeff(0)");
        growthEnergyDemandCoeff = paramFile.getItemDouble("Value");
        if (isDairy)
        {
            paramFile.setPath(basePath + ".milkEnergyDemandCoeff(0)");
            milkEnergyDemandCoeff = paramFile.getItemDouble("Value");
        }
        paramFile.setPath(basePath + ".Liveweight(0)");
        liveweight = paramFile.getItemDouble("Value");
        paramFile.setPath(basePath + ".Age(0)");
        age = paramFile.getItemDouble("Value");
        paramFile.setPath(basePath + ".HousedDuringGrazing(-1)");
        housedDuringGrazing = paramFile.getItemBool("Value");
        if (isRuminant)
        {
            paramFile.setPath(basePath + ".mu_b(0)");
            mu_b = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".mu_base(0)");
            mu_base = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".milkNconc(0)");
            milkNconc = paramFile.getItemDouble("Value");
            paramFile.setPath(basePath + ".milkCconc(0)");
            milkCconc = paramFile.getItemDouble("Value");
        }
        //back to reading user input
        if (isDairy)
        {
            avgProductionMilk = livestockFile.getItemDouble("avgProductionMilk");
            paramFile.setPath(basePath + ".weightGainDairy(0)");
            avgProductionMeat = paramFile.getItemDouble("Value");
            avgProductionMeat /= GlobalVars.avgNumberOfDays;
        }
        else
        {
            avgProductionMeat = livestockFile.getItemDouble("avgProductionMeat");
        }

        //housedDuringGrazing = true;
        housingDetails = new List<housingRecord>();
        string housingPath = path + ".Housing";
        min = 99;
        max = 0;
        livestockFile.setPath(housingPath);
        livestockFile.getSectionNumber(ref min, ref max);
        double testPropTime = 0;
        for (int i = min; i <= max; i++)
        {
            housingRecord newHouse =new housingRecord();
            livestockFile.Identity.Add(i);
            newHouse.SetHousingType(livestockFile.getItemInt("HousingType"));
            newHouse.SetNameOfHousing(livestockFile.getItemString("NameOfHousing"));
            newHouse.SetpropTime(livestockFile.getItemDouble("PropTime"));
            testPropTime += newHouse.GetpropTime();
            int maxManureRecipient=0, minManureRecipient=99;
            newHouse.Recipient=new List<ManureRecipient>();
            string RecipientPath=housingPath+'('+i.ToString()+").ManureRecipient";
            livestockFile.setPath(RecipientPath);
            livestockFile.getSectionNumber(ref minManureRecipient, ref maxManureRecipient);
            for (int j = minManureRecipient; j <= maxManureRecipient; j++)
            {
                if(livestockFile.doesIDExist(j))
                {
                    ManureRecipient newRecipient = new ManureRecipient();
                    livestockFile.Identity.Add(j);
                    newRecipient.setStorageType(livestockFile.getItemInt("StorageType"));
                    newHouse.Recipient.Add(newRecipient);
                    livestockFile.Identity.RemoveAt(livestockFile.Identity.Count - 1);
                }
            }
            housingDetails.Add(newHouse);
            livestockFile.Identity.RemoveAt(livestockFile.Identity.Count-1);
        }
        if (testPropTime !=1.0)
        {
          System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.WriteLine("Sum of proportions of time in different housing does not equal 1.0 ");
            file.WriteLine("program terminated");
            file.Close();
            Console.WriteLine("Sum of proportions of time in different housing does not equal 1.0 ");
            Console.WriteLine("program terminated");
            if (GlobalVars.Instance.getPauseBeforeExit())
                Console.ReadLine();
            Environment.Exit(0);
        }

        //read livestock input variables from input xml file
        string feeditemPath = path + ".itemFed";
        min = 99;
        max = 0;
        livestockFile.setPath(feeditemPath);
        livestockFile.getSectionNumber(ref min, ref max);
        for (int i = min; i <= max; i++)
        {
            if(livestockFile.doesIDExist(i))
            {
                //find the feed code for the first feed item
                feedItem newFeedItem = new feedItem(feeditemPath, i, true);
                if (newFeedItem.GetfedAtPasture() == true)
                    GlobalVars.Instance.SetGrazedDM(avgNumberOfAnimal * newFeedItem.Getamount(), newFeedItem.GetFeedCode());
                feedRation.Add(newFeedItem);
            }
        }
    }

    //these functions calculate energy demands using a specific energy system. the value calculated here should actually be read from file
    double dailymaintenanceEnergy() //MJ per animal
    {
        double maintenanceEnergy = 0;
        switch (GlobalVars.Instance.getcurrentEnergySystem())
        {
            case 1:
            case 2: 
                double efficiencyMaintenance = 0.02 * energyIntake / DMintake + 0.5;
                if (age<6.0)
                    maintenanceEnergy = ((maintenanceEnergyCoeff * 0.28* Math.Pow(liveweight, 0.75) * Math.Exp(-0.03*age)) / efficiencyMaintenance) + 0.09 * (energyIntake / GlobalVars.avgNumberOfDays);//SRC (1990) eq 1.21, minus EGRAZE and ECOLD
                else
                    maintenanceEnergy = ((maintenanceEnergyCoeff * 0.28* Math.Pow(liveweight, 0.75) * Math.Exp(-0.03*6.0)) / efficiencyMaintenance) + 0.09 * (energyIntake / GlobalVars.avgNumberOfDays);//SRC (1990) eq 1.21, minus EGRAZE and ECOLD
                    break;
      
            default: System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                    file.WriteLine("Energy system for livestock not found");
                    file.WriteLine("program terminated");
                    file.Close();
                    Console.WriteLine("Energy system for livestock not found");
                    Console.WriteLine("program terminated");
                    if (GlobalVars.Instance.getPauseBeforeExit())
                        Console.ReadLine();
                    Environment.Exit(0);
                    break;

        }
        return maintenanceEnergy;
    }

    double dailyGrowthEnergyPerkg() //MJ per kg
    {
        double growthEnergyPerkg = 0;
        switch (GlobalVars.Instance.getcurrentEnergySystem())
        {
            case 1:
            case 2: double efficiencyGrowth = 0.042 * energyIntake / DMintake + 0.006;//SCA 1990 1.38,
                growthEnergyPerkg = growthEnergyDemandCoeff / efficiencyGrowth;
                break;

            default: System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                file.WriteLine("Energy system for livestock not found");
                file.WriteLine("program terminated");
                file.Close();
                Console.WriteLine("Energy system for livestock not found");
                Console.WriteLine("program terminated");
                if (GlobalVars.Instance.getPauseBeforeExit())
                    Console.ReadLine();
                Environment.Exit(0);
                break;

        }
        return growthEnergyPerkg;
    }
    double dailyMilkEnergyPerkg()//MJ per kg
    {
        double milkEnergyPerkg = 0;
        switch (GlobalVars.Instance.getcurrentEnergySystem())
        {
            case 1:
            case 2: double efficiencyMilk = (0.02 * energyIntake / DMintake + 0.4);//SCA 1990 1.48
                milkEnergyPerkg = milkEnergyDemandCoeff / efficiencyMilk;
                break;
            default: System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                file.WriteLine("Energy system for livestock not found");
                file.WriteLine("program terminated");
                file.Close();
                Console.WriteLine("Energy system for livestock not found");
                Console.WriteLine("program terminated");
                if (GlobalVars.Instance.getPauseBeforeExit())
                    Console.ReadLine();
                Environment.Exit(0);
                break;
        }

        return milkEnergyPerkg;
    }

    double dailyEnergyRemobilisation()//MJ ME/day
    {
        double remobilisedEnergy = 0;
        if (avgProductionMeat<0)
            remobilisedEnergy=growthEnergyDemandCoeff * avgProductionMeat * 0.8;
        return remobilisedEnergy;
    }
    double dailyEnergyForGrazing() //MJ ME/day
    {
        double retVal = 0;
        return retVal;
    }
    void calcEnergyLevel()
    {
        energyLevel=energyIntake/(dailymaintenanceEnergy() * GlobalVars.avgNumberOfDays);
    }

    public double GetmaintenanceEnergy()//MJ ME per year
    {
        double maintenanceEnergy = dailymaintenanceEnergy() * GlobalVars.avgNumberOfDays;
        return maintenanceEnergy;
    }
    public double GetGrowthEnergy()//MJ ME per year
    {
        double growthEnergy =avgProductionMeat * dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays;
        return growthEnergy;
    }
    public double GetMilkEnergy()//MJ ME per year
    {
        double milkEnergy = avgProductionMilk * dailyMilkEnergyPerkg() * GlobalVars.avgNumberOfDays;
        return milkEnergy;
    }

    public void CalcEnergyDemand()//MJ per year
    {
        calcEnergyLevel();
        energyDemand = GetmaintenanceEnergy() + GetGrowthEnergy() + GetMilkEnergy();
        energyIntake *= Getmu(energyLevel);
    }

    public bool CalcMaximumProduction()//calculate daily production permitted by energy available
    {
        bool retVal = true;
        energyUseForMaintenance=0;
        energyUseForGrowth=0;
        energyUseForMilk=0;
        energyFromRemobilisation=0;
        maintenanceEnergyDeficit = 0;
        milkEnergyDeficit = 0;
        growthEnergyDeficit = 0;
        double initialEnergy = liveweight * growthEnergyDemandCoeff;
        calcEnergyLevel();
        double energyAvail = energyIntake * Getmu(energyLevel);//energyIntake is in MJ per animal per year
        energyUseForMaintenance = dailymaintenanceEnergy() * GlobalVars.avgNumberOfDays;
        energyAvail -= energyUseForMaintenance;
        if (energyAvail<0)
        {
            energyFromRemobilisation=dailyEnergyRemobilisation();
            energyAvail += energyFromRemobilisation;
            if (energyAvail < 0)
            {
                if (GlobalVars.Instance.getStrict())
                {
                    string message = name + " - Insufficient energy for maintenance";
                    System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                    file.WriteLine(message);
                    file.WriteLine("program terminated");
                    file.Close();
                    Console.WriteLine(message);
                    retVal = false;
                    Environment.Exit(0);
                }
                else
                {
                    maintenanceEnergyDeficit = energyAvail;
                    retVal = false;
                }
            }
            else
            {
                energyFromRemobilisation = energyAvail;
                energyAvail = 0;
            }
        }
        if (avgProductionMilk > 0) //these are dairy
        {
            double targetEnergyUseForMilk = dailyMilkEnergyPerkg() * GlobalVars.avgNumberOfDays * avgProductionMilk;
            if (avgProductionMeat > 0)//these animals are growing
            {
                energyUseForGrowth = avgProductionMeat * dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays;
                if (energyAvail < energyUseForGrowth) //assume positive growth is obligatory for dairy
                {
                    if (GlobalVars.Instance.getStrict())
                    {
                        string message = name + " - Insufficient energy for growth of dairy animals";
                        System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                        file.WriteLine(message);
                        file.WriteLine("program terminated");
                        file.Close();
                        Console.WriteLine(message);
                        retVal = false;
                        Environment.Exit(0);
                    }
                    else
                    {
                        energyUseForMilk = 0;
                        growthEnergyDeficit = -1*(energyUseForGrowth - energyAvail);
                        avgProductionMilk = 0;
                        retVal = false;
                    }
                    
                }
                energyUseForMilk = energyAvail - energyUseForGrowth;
                avgProductionMilk = energyUseForMilk / (dailyMilkEnergyPerkg() * GlobalVars.avgNumberOfDays);
                retVal = true;
            }
            else//there will be remobilised energy available
            {
                energyUseForMilk = energyAvail + energyFromRemobilisation + maintenanceEnergyDeficit;
                avgProductionMilk = energyAvail / (dailyMilkEnergyPerkg() * GlobalVars.avgNumberOfDays) 
                    + energyFromRemobilisation/GetMilkEnergy();
                energyFromRemobilisation = dailyEnergyRemobilisation();//reset remobilisation energy - some may have been used for maintenance
                retVal = true;
            }
            if (energyUseForMilk < targetEnergyUseForMilk)
                milkEnergyDeficit = -1*(targetEnergyUseForMilk - energyUseForMilk);
        }
        else //these are meat animals
        {
            double targetEnergyUseForGrowth = dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays * avgProductionMeat;
            energyUseForMilk = 0;
            milkEnergyDeficit = 0;
            energyUseForGrowth = energyAvail - maintenanceEnergyDeficit;
            if (energyUseForGrowth < targetEnergyUseForGrowth)
                growthEnergyDeficit = -1 * (targetEnergyUseForGrowth - energyUseForGrowth);
            avgProductionMeat = energyAvail / (dailyGrowthEnergyPerkg() * GlobalVars.avgNumberOfDays);
            retVal = true;
        }

        double final = (liveweight + avgProductionMeat) * growthEnergyDemandCoeff;
        double test1 =  energyUseForMilk + energyUseForMaintenance + energyUseForGrowth;
        return retVal;
    }
    double Getmu(double energyLevel)
    {
        double mu = 1;
        if (energyLevel > mu_base)
            mu = 1 - mu_b * (energyLevel - mu_base);
        return mu;
    }

    public void intake()
    {
        for (int k = 0; k < feedRation.Count; k++)
        {
            feedItem anItem = feedRation[k];
            DMintake += GlobalVars.avgNumberOfDays * anItem.Getamount();
            energyIntake += GlobalVars.avgNumberOfDays * anItem.Getamount() * anItem.Getenergy_conc();
            diet_ash += GlobalVars.avgNumberOfDays * anItem.Getamount() * anItem.Getash_conc();
            Nintake += GlobalVars.avgNumberOfDays * anItem.Getamount() * anItem.GetN_conc();
            Cintake += GlobalVars.avgNumberOfDays * anItem.Getamount() * anItem.GetC_conc();
            diet_fat += GlobalVars.avgNumberOfDays * anItem.Getamount() * anItem.Getfat_conc();
            diet_fibre += GlobalVars.avgNumberOfDays * anItem.Getamount() * anItem.Getfibre_conc();
            diet_NFE += GlobalVars.avgNumberOfDays * anItem.Getamount() * anItem.GetnitrogenfreeExtract_conc();
            diet_nitrate += GlobalVars.avgNumberOfDays * anItem.Getamount() * anItem.GetNitrate_conc();
            digestibilityDiet += anItem.Getamount() * anItem.GetDMdigestibility();
        }
        digestibilityDiet /= (DMintake/ GlobalVars.avgNumberOfDays);
        double sum = 0;
        for (int j = 0; j < feedRation.Count; j++)
        {
            if (feedRation[j].GetfedAtPasture())
                sum += feedRation[j].Getamount() * GlobalVars.avgNumberOfDays;
        }

        timeOnPasture =sum/ GetDMintake();

    }
    public double entericMethane()
    {
        double numDays = GlobalVars.avgNumberOfDays;
        double methane = 0; //initially in grams
        switch (GlobalVars.Instance.getcurrentInventorySystem())
        {
            case 1:
                double grossEnergyIntake = 18.4 * DMintake;
                double MCF = 0.06;
                methane = grossEnergyIntake * MCF / 55.65;//1.13
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
            methane_reduction = mol_nitrate / mol_methane;
            if (methane_reduction > 1)
                methane_reduction = 1.0;
        }
        methane *= (1-methane_reduction);
        return methane;
    }
    public void DoCarbon()
    {
        milkC = GlobalVars.avgNumberOfDays * avgProductionMilk * milkCconc;
        growthC = GlobalVars.avgNumberOfDays * avgProductionMeat * growthCconc;
        double ashConc =diet_ash/DMintake;
        faecalC = Cintake * (1 - Getmu(energyLevel) * digestibilityDiet)/(1-ashConc);
        urineC = urineProp * Cintake;
        CH4C = entericMethane() * 12 / 16;
        CO2C=Cintake - (milkC + growthC + faecalC+urineC+CH4C);
    }

    public void DoNitrogen()
    {
        milkN = GlobalVars.avgNumberOfDays * avgProductionMilk * milkNconc;
        growthN =GlobalVars.avgNumberOfDays * avgProductionMeat * growthNconc;
        double ashConc = diet_ash / DMintake;
        faecalN = (Nintake * (1 - Getmu(energyLevel) * digestibilityDiet)) / (1 - ashConc);
        urineN = Nintake - (milkN + growthN + faecalN);
    }
    public void DoRuminant()
    {
        intake();
        CalcMaximumProduction();
        CalcEnergyDemand();
        DoCarbon();
        DoNitrogen();
        GetExcretaDeposition();
    }
    public void Write(XmlWriter writer,System.IO.StreamWriter tabFile)
    {
        writer.WriteStartElement("LiveStock");

        writer.WriteStartElement("name");
        writer.WriteStartElement("Description");
        writer.WriteValue("Name ");
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

        writer.WriteStartElement("LivestockType");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("LivestockType");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(LivestockType);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("liveweight");
        writer.WriteStartElement("Description");
        writer.WriteValue("Liveweight of animals");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("liveweight");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(liveweight);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("isRuminant");
        writer.WriteStartElement("Description");
        writer.WriteValue("Is this a ruminant");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("None");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("isRuminant");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(isRuminant);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("mu_b");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("mu_b");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(mu_b);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("urineProp");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("urineProp");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(urineProp);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("growthCconc");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("growthCconc");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(growthCconc);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("milkNconc");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("milkNconc");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(milkNconc);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("milkCconc");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("milkCconc");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(milkCconc);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("maintenanceEnergyCoeff");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("maintenanceEnergyCoeff");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(maintenanceEnergyCoeff);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("growthEnergyDemandCoeff");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("growthEnergyDemandCoeff");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(growthEnergyDemandCoeff);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("milkEnergyDemandCoeff");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("milkEnergyDemandCoeff");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(milkEnergyDemandCoeff);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("housedDuringGrazing");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("housedDuringGrazing");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(housedDuringGrazing);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("energyIntake");
        writer.WriteStartElement("Description");
        writer.WriteValue("Energy intake");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("MJ/yr");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("energyIntake");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(energyIntake);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("energyUseForGrowth");
        writer.WriteStartElement("Description");
        writer.WriteValue("Energy used for growth");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("MJ/");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("energyUseForGrowth");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(energyUseForGrowth);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("energyUseForMilk");
        writer.WriteStartElement("Description");
        writer.WriteValue("Energy used for milk production");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("MJ/yr");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("energyUseForMilk");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(energyUseForMilk);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("energyFromRemobilisation");
        writer.WriteStartElement("Description");
        writer.WriteValue("Energy supplied by remobilisation");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("MJ/yr");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("energyFromRemobilisation");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(energyFromRemobilisation);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("energyUseForMaintenance");
        writer.WriteStartElement("Description");
        writer.WriteValue("");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("MJ/yr");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("energyUseForMaintenance");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(energyUseForMaintenance);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("maintenanceEnergyDeficit");
        writer.WriteStartElement("Description");
        writer.WriteValue("Deficit in energy required for maintenance");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("MJ/yr");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("maintenanceEnergyDeficit");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(maintenanceEnergyDeficit);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("growthEnergyDeficit");
        writer.WriteStartElement("Description");
        writer.WriteValue("Deficit in energy required for growth");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("MJ/yr");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("growthEnergyDeficit");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(growthEnergyDeficit);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("milkEnergyDeficit");
        writer.WriteStartElement("Description");
        writer.WriteValue("Deficit in energy required for milk production");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("MJ/yr");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("milkEnergyDeficit");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(milkEnergyDeficit);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("DMintake");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("DMintake");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(DMintake);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("diet_ash");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("diet_ash");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(diet_ash);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("diet_fibre");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("diet_fibre");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(diet_fibre);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("diet_fat");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("diet_fat");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(diet_fat);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("diet_NFE");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("diet_NFE");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(diet_NFE);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("digestibilityDiet");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("digestibilityDiet");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(digestibilityDiet);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Cintake");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("Cintake");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(Cintake);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("energyLevel");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("energyLevel");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(energyLevel);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("milkN");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("milkN");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(milkN);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("milkC");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("milkC");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(milkC);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("growthN");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("growthN");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(growthN);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("growthC");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("growthC");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(growthC);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("urineC");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("urineC");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(urineC);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("urineN");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("urineN");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(urineN);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("faecalC");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("faecalC");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(faecalC);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("faecalN");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("faecalN");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(faecalN);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CH4C");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("CH4C");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(CH4C);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("CO2C");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("CO2C");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(CO2C);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("avgNumberOfAnimal");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("avgNumberOfAnimal");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(avgNumberOfAnimal);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("avgNumberOfAnimal");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("avgNumberOfAnimal");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(avgNumberOfAnimal);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("avgProductionMilk");
        writer.WriteStartElement("Description");
        writer.WriteValue("Average daily milk production");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg/day");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("avgProductionMilk");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(avgProductionMilk);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("avgProductionMilk");
        writer.WriteStartElement("Description");
        writer.WriteValue("Average yearly milk production");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg/yr");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("avgProductionMilk");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        double temp = avgProductionMilk * GlobalVars.avgNumberOfDays;
        writer.WriteValue(temp);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("avgProductionMeat");
        writer.WriteStartElement("Description");
        writer.WriteValue("Average daily weight change");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("kg/day");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("avgProductionMeat");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(avgProductionMeat);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("LivestockType");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("LivestockType");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(LivestockType);
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

        writer.WriteStartElement("growthNconc");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("growthNconc");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(growthNconc);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("growthCconc");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("growthCconc");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(growthCconc);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("mu_b");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("mu_b");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(mu_b);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("milkNconc");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("milkNconc");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(milkNconc);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("growthNconc");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("growthNconc");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(growthNconc);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("milkCconc");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("milkCconc");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(milkCconc);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("milkNconc");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("milkNconc");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(milkNconc);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("growthNconc");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("growthNconc");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(growthNconc);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("urineProp");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("urineProp");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(urineProp);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("maintenanceEnergyCoeff");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("maintenanceEnergyCoeff");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(maintenanceEnergyCoeff);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("milkEnergyDemandCoeff");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("milkEnergyDemandCoeff");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(milkEnergyDemandCoeff);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("growthEnergyDemandCoeff");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("growthEnergyDemandCoeff");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(growthEnergyDemandCoeff);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("housedDuringGrazing");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("housedDuringGrazing");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(housedDuringGrazing);
        writer.WriteEndElement();
        writer.WriteEndElement();


            tabFile.Write("LiveStock"+"\n");

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
            tabFile.Write("speciesGroup" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(speciesGroup.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("LivestockType" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(LivestockType.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("liveweight" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(liveweight.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("isRuminant" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(isRuminant.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("mu_b" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(mu_b.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("urineProp" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(urineProp.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("growthCconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(growthCconc.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("milkNconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(milkNconc.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("milkCconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(milkCconc.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("maintenanceEnergyCoeff" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(maintenanceEnergyCoeff.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("maintenanceEnergyCoeff" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(milkEnergyDemandCoeff.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("housedDuringGrazing" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(housedDuringGrazing.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("Energy intake" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("MJ/yr" + "\t");
            tabFile.Write("energyIntake" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(energyIntake.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("Energy used for maintenance" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("MJ/yr" + "\t");
            tabFile.Write("energyUseForMaintenance" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(energyUseForMaintenance.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("Energy used for growt" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("MJ/yr" + "\t");
            tabFile.Write("energyUseForGrowth" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(energyUseForGrowth.ToString() + "\n");



            tabFile.Write("Description" + "\t");
            tabFile.Write("Energy used for milk production" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("MJ/yr" + "\t");
            tabFile.Write("energyUseForMilk" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(energyUseForMilk.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("Energy supplied by remobilisation" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("MJ/yr" + "\t");
            tabFile.Write("energyFromRemobilisation" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(energyFromRemobilisation.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("Deficit in energy required for maintenance" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("MJ/yr" + "\t");
            tabFile.Write("maintenanceEnergyDeficit" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(maintenanceEnergyDeficit.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("Deficit in energy required for growth" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("MJ/yr" + "\t");
            tabFile.Write("growthEnergyDeficit" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(growthEnergyDeficit.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("Deficit in energy required for milk production" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("MJ/yr" + "\t");
            tabFile.Write("milkEnergyDeficit" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(milkEnergyDeficit.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("DMintake" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(DMintake.ToString() + "\n");



            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("diet_ash" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(diet_ash.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("diet_fibre" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(diet_fibre.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("diet_fat" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(diet_fat.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("diet_NFE" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(diet_NFE.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("digestibilityDiet" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(digestibilityDiet.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Cintake" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Cintake.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("energyLevel" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(energyLevel.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("milkN" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(milkN.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("milkC" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(milkC.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("growthN" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(growthN.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("growthC" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(growthC.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("urineC" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(urineC.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("urineN" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(urineN.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("faecalC" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(faecalC.ToString() + "\n");


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
            tabFile.Write("CH4C" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(CH4C.ToString() + "\n");


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
            tabFile.Write("avgNumberOfAnimal" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(avgNumberOfAnimal.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("Average daily milk production" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("kg/day" + "\t");
            tabFile.Write("avgNumberOfAnimal" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(avgProductionMilk.ToString() + "\n");



            tabFile.Write("Description" + "\t");
            tabFile.Write("Average daily weight change" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("kg/day" + "\t");
            tabFile.Write("avgNumberOfAnimal" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(avgProductionMeat.ToString() + "\n");



            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("LivestockType" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(LivestockType.ToString() + "\n");



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
            tabFile.Write("growthNconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(growthNconc.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("growthCconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(growthCconc.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("mu_b" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(mu_b.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("milkNconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(milkNconc.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("growthNconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(growthNconc.ToString() + "\n");
            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("growthNconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(growthNconc.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("milkCconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(milkCconc.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("milkNconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(milkNconc.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("growthNconc" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(growthNconc.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("urineProp" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(urineProp.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("maintenanceEnergyCoeff" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(maintenanceEnergyCoeff.ToString() + "\n");
            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("milkEnergyDemandCoeff" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(milkEnergyDemandCoeff.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("growthEnergyDemandCoeff" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(growthEnergyDemandCoeff.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("housedDuringGrazing" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(housedDuringGrazing.ToString() + "\n");



            for (int i = 0; i < housingDetails.Count; i++)
                housingDetails[i].Write(writer, tabFile);
        writer.WriteEndElement();
    }
    public void GetExcretaDeposition()
    {
        double[] DM = new double[GlobalVars.Instance.getmaxNumberFeedItems()];
        for (int i = 0; i < GlobalVars.Instance.getmaxNumberFeedItems(); i++)
            DM[i] = 0;

        double sum = 0;
        for (int j = 0; j < feedRation.Count; j++)
        {
            if (feedRation[j].GetfedAtPasture())
            {
                int feedCode=feedRation[j].GetFeedCode();
                double temp = avgNumberOfAnimal *  GlobalVars.Instance.GetavgNumberOfDays() * feedRation[j].Getamount();
                DM[feedCode]+= temp ;
                sum += temp;
            }
        }
        for (int i = 0; i < GlobalVars.Instance.getmaxNumberFeedItems(); i++)
        {
            if (DM[i]>0)
            {
                double theUrineN = timeOnPasture * avgNumberOfAnimal * urineN * DM[i] / sum;
                double theUrineC = timeOnPasture * avgNumberOfAnimal * urineC * DM[i] / sum;
                double theFaecalN = timeOnPasture * avgNumberOfAnimal * faecalN * DM[i] / sum;
                double theFaecalC = timeOnPasture * avgNumberOfAnimal * faecalC * DM[i] / sum;
                GlobalVars.Instance.grazedArray[i].urineC +=  theUrineC * GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate();
                GlobalVars.Instance.grazedArray[i].urineN += theUrineN * GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate(); ;
                GlobalVars.Instance.grazedArray[i].faecesC += theFaecalC * GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate(); ;
                GlobalVars.Instance.grazedArray[i].faecesN += theFaecalN * GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate(); ;
                GlobalVars.Instance.grazedArray[i].ruminantDMgrazed += DM[i] * GlobalVars.Instance.theZoneData.GetaverageYearsToSimulate(); ;
            }
        }
    }
    public void GetAllFeedItemsUsed()
    {
        for (int i = 0; i < GlobalVars.Instance.getmaxNumberFeedItems(); i++)
        {
            for (int j = 0; j < feedRation.Count; j++)
                if ((feedRation[j].GetFeedCode() == i)&&(!feedRation[j].GetfedAtPasture()))
                {
                    feedItem afeedItem = new feedItem();
                    afeedItem.setFeedCode(i);
                    afeedItem.AddFeedItem(feedRation[j], false);
                    afeedItem.Setamount(avgNumberOfAnimal * GlobalVars.Instance.GetavgNumberOfDays()  * feedRation[j].Getamount());
                    GlobalVars.Instance.allFeedAndProductsUsed[i].composition.AddFeedItem(afeedItem, false);
                    break;
                }
        }
    }

    public bool CheckLivestockCBalance()
    {
        bool retVal = false;
        double Cout = urineC + growthC + faecalC + milkC;
        double CLost = CH4C + CO2C;
        double Cbalance = Cintake - (Cout + CLost);
        double diff = Cbalance / Cintake;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
           
                double errorPercent = 100 * diff;
                System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                file.WriteLine("Error; Livestock C balance error is more than the permitted margin");
                file.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                file.Close();
                Console.WriteLine("Error; Livestock C balance error is more than the permitted margin");
                Console.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                if (GlobalVars.Instance.getPauseBeforeExit())
                    Console.Read();
                Environment.Exit(0);
       
        }
        return retVal;
    }

    public bool CheckLivestockNBalances()
    {
        bool retVal = false;
        double Nout = urineN + growthN + faecalN + milkN;
        double Nbalance = Nintake - Nout;
        double diff = Nbalance / Nintake;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
            
                double errorPercent = 100 * diff;
                System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                file.WriteLine("Error; Livestock N balance error is more than the permitted margin");
                file.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                file.Close();
                Console.WriteLine("Error; Livestock N balance error is more than the permitted margin");
                Console.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                if (GlobalVars.Instance.getPauseBeforeExit())
                    Console.Read();
                Environment.Exit(0);
           
        }
        return retVal;
    }

}
