using System.Collections.Generic;
using System.Xml;
using System;

public class GlobalVars
{
     private static GlobalVars instance;
     private GlobalVars()
     { 
            
     }
     public static GlobalVars Instance
     {
         get
         {
             if (instance == null)
             {
                 instance = new GlobalVars();
             }
             return instance;
         }
     }
     public struct grazedItem
     {
         public double urineC;
         public double urineN;
         public double faecesC;
         public double faecesN;
         public double ruminantDMgrazed;
         public double fieldDMgrazed;
     }
     public grazedItem[] grazedArray = new grazedItem[maxNumberFeedItems];
     public product[] allFeedAndProductsUsed = new product[maxNumberFeedItems];
     public product[] allFeedAndProductsProduced = new product[maxNumberFeedItems];
     public product[] allFeedAndProductTradeBalance = new product[maxNumberFeedItems];

    //constants
     private double humification_const;
     private double alpha;
     private double rgas;
     private double CNhum;
     private double tor;
     private double Eapp;
     private double CO2EqCH4;
     private double CO2EqN2O;
     private double IndirectNH3N2OFactor;
     private double IndirectNO3N2OFactor;
     private double defaultBeddingCconc;
     private double defaultBeddingNconc;
     private double maxToleratedErrorYield; 
     private double maxToleratedErrorGrazing;
    private double EFNO3_IPCC;
     private  double fert_EFNH3_IPCC;
     private   double man_EFNH3_IPCC;
     private int minimumTimePeriod;
     private List<int> theInventorySystems;
     private int currentInventorySystem;
     private int currentEnergySystem;
     bool IsIterating;


     public double getHumification_const() { return humification_const; }
    public double getalpha() { return alpha; }
    public double getrgas() { return rgas; }
    public double getCNhum() { return CNhum; }
    public double gettor() { return tor; }
    public double getEapp() { return Eapp; }
    public double GetCO2EqCH4() { return CO2EqCH4; }
    public double GetCO2EqN2O() { return CO2EqN2O; }
    public double GetIndirectNH3N2OFactor() { return IndirectNH3N2OFactor; }
    public double GetIndirectNO3N2OFactor() { return IndirectNO3N2OFactor; }
    public int GetminimumTimePeriod() { return minimumTimePeriod; }
    public bool GetIsIterating() { return IsIterating; }

    public double getdefaultBeddingCconc() { return defaultBeddingCconc; }
    public double getdefaultBeddingNconc() { return defaultBeddingNconc; }
    public int getcurrentInventorySystem() { return currentInventorySystem; }
    public int getcurrentEnergySystem() { return currentEnergySystem; }
    public double getEFNO3_IPCC() { return EFNO3_IPCC; }
    public double getfert_EFNH3_IPCC(){return fert_EFNH3_IPCC;}
    public double getman_EFNH3_IPCC(){return man_EFNH3_IPCC;}
    public double getmaxToleratedErrorYield() { return maxToleratedErrorYield; }
    private double getmaxToleratedErrorGrazing() { return maxToleratedErrorGrazing; }

    public void setcurrentInventorySystem(int aVal) { currentInventorySystem = aVal; }
    public void setcurrentEnergySystem(int aVal) { currentEnergySystem = aVal; }
    public void SetIsIterating(bool aVal) { IsIterating = aVal; }
    public double Getrho() { return 3.1415926 * 2.0 / (365.0 * 24.0 * 3600.0); }
    private bool stopOnError;
    private bool pauseBeforeExit;
    public void setPauseBeforeExit(bool stop) { pauseBeforeExit = stop; }
    public bool getPauseBeforeExit() { return pauseBeforeExit; }
    public void setstopOnError(bool stop) { stopOnError = stop; }
    public bool getstopOnError() { return stopOnError; }
    private int[] errorCodes = new int[100];
    private bool strict;//forces exit with error if energy requirements not met
    public void setStrict(bool aVal){strict=aVal;}
    public bool getStrict() { return strict; }

    public struct zoneSpecificData
    {
        private XmlWriter writer;
        private System.IO.StreamWriter tabFile;
        private string debugFileName;
        private System.IO.StreamWriter debugfile;
        bool debugIt;

        public void SetdebugFileName(string aName) { debugFileName = aName; }
        public void SetdebugIt(bool aVal) { debugIt = aVal; }
        public bool GetdebugIt() { return debugIt; }

        private double[] airTemp;
        private double[] droughtIndex;
        private double[] Precipitation;
        public struct fertiliserData
        {
            public int storageType;
            public int speciesGroup;
            public double fertManNH3EmissionFactor;
            public double fertManNH3EmissionFactorHousingRefTemperature;
            public string name;
        }

        public struct C_ToolData
        {
            public double initialC;
            public double initialFOMn;
            public double propHUM;
            public double propROM;
            public double InitialCtoN;
            public double InitialFOMCtoN;
        }

        public struct soilData
        {
            public double N2Factor;
            public string name;
            public double ClayFraction;
            public double maxSoilDepth;
            public double dampingDepth;

            public double thermalDiff;
            public double GetdampingDepth() { return dampingDepth; }
            public List<C_ToolData> theC_ToolData;

            public double CalcDampingDepth(double thermalDiff, double rho)
            {
                return Math.Sqrt(2.0 * thermalDiff / rho);
            }
        }

        public struct manureAppData
        {
            public double NH3EmissionReductionFactor;
            public string name;
        }

        public List<fertiliserData> theFertManData;
        public List<soilData> thesoilData;
        public List<manureAppData> themanureAppData;
        double urineNH3EmissionFactor;
        double manureN20EmissionFactor;
        double fertiliserN20EmissionFactor;
        double residueN2OEmissionFactor;
        double soilN2OEmissionFactor;
        double manureN2Factor;
        double averageAirTemperature;
        int airtemperatureOffset;
        double airtemperatureAmplitude;
        int grazingMidpoint;
        double averageYearsToSimulate;
        public double geturineNH3EmissionFactor(){return urineNH3EmissionFactor;}
        public double getmanureN20EmissionFactor(){return manureN20EmissionFactor;}
        public double getfertiliserN20EmissionFactor(){return fertiliserN20EmissionFactor;}
//        public double GetfertManNH3EmissionFactorHousingRefTemperature() { return fertManNH3EmissionFactorHousingRefTemperature; }
        public double getresidueN2OEmissionFactor(){return residueN2OEmissionFactor;}
        public double getsoilN2OEmissionFactor() { return soilN2OEmissionFactor; }
        public double GetaverageAirTemperature() { return averageAirTemperature; }
        public int GetairtemperatureOffset() { return airtemperatureOffset; }
        public double GetairtemperatureAmplitude() { return airtemperatureAmplitude; }
        public int GetgrazingMidpoint() { return grazingMidpoint; }
        public void SetaverageYearsToSimulate(double aVal) { averageYearsToSimulate = aVal; }
        public double GetaverageYearsToSimulate() { return averageYearsToSimulate; }

        public XmlWriter OpenOutputXML(string outputName)
        {
            writer = XmlWriter.Create(outputName);
            writer.WriteStartDocument();
            return writer;
        }

        public void CloseOutputXML()
        {
            writer.WriteEndDocument();
            writer.Close();

        }

        public XmlWriter GetXMLWriter(){return writer;}

        public System.IO.StreamWriter OpenOutputTabFile(string outputName)
        {
            tabFile = new System.IO.StreamWriter(outputName);
          //  tabFile.WriteStartDocument();
            return tabFile;
        }

        public void CloseOutputTabFile()
        {

            tabFile.Close();

        }

        public System.IO.StreamWriter GetTabFileWriter() { return tabFile; }

        public void OpenDebugFile(string afilename)
        {
            SetdebugFileName(afilename);
            debugfile = new System.IO.StreamWriter(debugFileName);
        }
        public void CloseDebugFile()
        {
            try
            {
                debugfile.Close();
            }
            catch
            { }
        }

        public void WriteToDebug(string aString)
        {
            debugfile.Write(aString);
        }

        public void WriteLineToDebug(string aString)
        {
            debugfile.WriteLine(aString);
        }

        public double GetMeanDroughtIndex(CropClass cropData)
        {
            double MeanDroughtIndex = 0;
            int startDay = cropData.GetStartDay();
            int startMonth = cropData.GetStartMonth();
            int startYear = cropData.GetStartYear();
            int endDay = cropData.GetEndDay();
            int endMonth = cropData.GetEndMonth();
            int endYear = cropData.GetEndYear();
            MeanDroughtIndex = GetMeanDroughtIndex(startDay, startMonth, startYear, endDay, endMonth, endYear);
            return MeanDroughtIndex;
        }

        public double GetMeanDroughtIndex(int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear)
        {
            timeClass time = new timeClass();
            double MeanDroughtIndex = 0;
            int endCount = endMonth;
            if (endYear > startYear)
                endCount += 12;
            for (int i = (startMonth) + 1; i <= endCount - 1; i++)
            {
                int monthCount = i;
                if (i > 12)
                    monthCount -= 12;
                MeanDroughtIndex += droughtIndex[monthCount - 1] * time.tabDaysPerMonth[monthCount - 1];
            }
            double startMonthAmount = ((time.tabDaysPerMonth[startMonth - 1] - startDay) / time.tabDaysPerMonth[startMonth - 1]) * droughtIndex[startMonth - 1];
            MeanDroughtIndex += startMonthAmount;
            double endMonthAmount = ((time.tabDaysPerMonth[endMonth - 1] - endDay) / time.tabDaysPerMonth[endMonth - 1]) * droughtIndex[endMonth - 1];
            MeanDroughtIndex += endMonthAmount;
            int numbersOfDays = 0;
            for (int i = (startMonth) + 1; i <= endCount - 1; i++)
            {
                int monthCount = i;
                if (i > 12)
                    monthCount -= 12;
                numbersOfDays += time.tabDaysPerMonth[monthCount - 1];
            }
            numbersOfDays += (time.tabDaysPerMonth[endMonth-1] - endDay);
            numbersOfDays += (time.tabDaysPerMonth[startMonth-1] - startDay);

            return MeanDroughtIndex / numbersOfDays;
        }
        public double GetMeanTemperature(CropClass cropData)
        {
            double MeanTemperature = 0;
            int startDay = cropData.GetStartDay();
            int startMonth = cropData.GetStartMonth();
            int startYear = cropData.GetStartYear();
            int endDay = cropData.GetEndDay();
            int endMonth = cropData.GetEndMonth();
            int endYear = cropData.GetEndYear();
            MeanTemperature = GetMeanTemperature(startDay, startMonth, startYear, endDay, endMonth, endYear);
            return MeanTemperature;
        }

        public double GetMeanTemperature(int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear)
        {
            timeClass time = new timeClass();
            double MeanTemperature = 0;
            int endCount = endMonth;
            if (endYear > startYear)
                endCount += 12;
            for (int i = (startMonth) + 1; i <= endCount - 1; i++)
            {
                int monthCount = i;
                if (i > 12)
                    monthCount -= 12;
                MeanTemperature += airTemp[monthCount - 1] * time.tabDaysPerMonth[monthCount - 1];
            }
            double startMonthAmount = ((time.tabDaysPerMonth[startMonth - 1] - startDay) / time.tabDaysPerMonth[startMonth - 1]) * airTemp[startMonth - 1];
            MeanTemperature += startMonthAmount;
            double endMonthAmount = ((time.tabDaysPerMonth[endMonth - 1] - endDay) / time.tabDaysPerMonth[endMonth - 1]) * airTemp[endMonth - 1];
            MeanTemperature += endMonthAmount;
            int numbersOfDays = 0;
            for (int i = (startMonth) + 1; i <= endCount - 1; i++)
            {
                int monthCount = i;
                if (i > 12)
                    monthCount -= 12;
                numbersOfDays += time.tabDaysPerMonth[monthCount - 1];
            }
            numbersOfDays += (time.tabDaysPerMonth[endMonth-1] - endDay);
            numbersOfDays += (time.tabDaysPerMonth[startMonth-1] - startDay);

            return MeanTemperature / numbersOfDays;
        }

        public double GetPropLeaching(int startDay, int startMonth, CropClass cropData)
        {
            timeClass time = new timeClass();
            double PropLeaching = 0;
            int endMonthCount = cropData.GetEndMonth();
            if (cropData.GetEndYear() > cropData.GetStartYear())
                endMonthCount += 12;
            int startMonthCount = startMonth;
            if (startMonth <cropData.GetStartMonth())
                startMonthCount += 12;
            for (int i = startMonthCount + 1; i <= endMonthCount - 1; i++)
            {
                int monthCount=i;
                if (i > 12)
                    monthCount -= 13;
                else
                    monthCount -= 1;
                if (droughtIndex[monthCount]==0)
                    PropLeaching += time.tabDaysPerMonth[monthCount];
            }
            double startMonthAmount = 0;
            if (droughtIndex[startMonth-1]== 0)
                startMonthAmount = time.tabDaysPerMonth[startMonth-1] - startDay + 1;
            PropLeaching += startMonthAmount;
            double endMonthAmount = 0;
            if (droughtIndex[cropData.GetEndMonth() - 1] == 0)
                endMonthAmount = time.tabDaysPerMonth[cropData.GetEndMonth() - 1] - cropData.GetEndDay();
            PropLeaching += endMonthAmount;
            int numbersOfDays = 0;
            for (int i = startMonthCount + 1; i <= endMonthCount - 1; i++)
            {
                int monthCount = i;
                if (i > 12)
                    monthCount -= 13;
                else
                    monthCount -= 1;
                numbersOfDays += time.tabDaysPerMonth[monthCount];
            }
            numbersOfDays += cropData.GetEndDay();
            numbersOfDays += (time.tabDaysPerMonth[startMonth-1] - startDay);

            return PropLeaching / numbersOfDays;
        }

    public double GetPropFieldCapacity(CropClass cropData)
    {
        timeClass time=new timeClass();
        double PropFieldCapacity = 0;
        int startMonthCount = cropData.GetStartMonth();
        int endMonthCount = cropData.GetEndMonth();
        if (cropData.GetEndYear() > cropData.GetStartYear())
            endMonthCount += 12;
        for (int i = startMonthCount + 1; i <= endMonthCount - 1; i++)
        {
            int monthCount=i;
            if (i > 12)
                monthCount -= 13;
            else
                monthCount -= 1;
            if (droughtIndex[monthCount]==0)
                PropFieldCapacity += time.tabDaysPerMonth[monthCount];
        }
        double startMonthAmount=0;
        if (droughtIndex[cropData.GetStartMonth() - 1] == 0)
            startMonthAmount = time.tabDaysPerMonth[cropData.GetStartMonth() - 1] - cropData.GetStartDay() + 1;
        PropFieldCapacity += startMonthAmount;
        double endMonthAmount = 0;
        if (droughtIndex[cropData.GetEndMonth() - 1] == 0)
            endMonthAmount = cropData.GetEndDay();
        PropFieldCapacity += endMonthAmount;
        int numbersOfDays=0;
        for (int i = startMonthCount + 1; i <= endMonthCount - 1; i++)
        {
            int monthCount = i;
            if (i > 12)
                monthCount -= 13;
            else
                monthCount -= 1;
            numbersOfDays += time.tabDaysPerMonth[monthCount];
        }
        numbersOfDays += cropData.GetEndDay();
        numbersOfDays += (time.tabDaysPerMonth[cropData.GetStartMonth() - 1] - cropData.GetStartDay());

        return PropFieldCapacity / numbersOfDays;
 }

        public void readZoneSpecificData(int zone_nr, int currentFarmType)
        {
            FileInformation AEZParamFile = new FileInformation(GlobalVars.Instance.getParamFilePath(), GlobalVars.Instance.GeterrorFileName());
            //get zone-specific constants
            string basePath = "AgroecologicalZone(" + zone_nr.ToString() + ")";
            AEZParamFile.setPath(basePath);
            airTemp = new double[12];
            droughtIndex = new double[12];
            Precipitation = new double[12];
            bool monthlyData = AEZParamFile.getItemBool("MonthlyAirTemp");
            if (monthlyData == true)
            {
                AEZParamFile.PathNames.Add("Month");
                int max = 0; int min = 99;
                AEZParamFile.getSectionNumber(ref min, ref max);
                AEZParamFile.Identity.Add(-1);
                AEZParamFile.PathNames.Add("AirTemperature");
                AEZParamFile.Identity.Add(-1);
                for (int i = min; i <= max; i++)
                {
                    AEZParamFile.Identity[1] = i;
                    airTemp[i-1] = AEZParamFile.getItemDouble("Value");
                }
                AEZParamFile.PathNames[2] = "DroughtIndex"; 
                for (int i = min; i <= max; i++)
                {
                    AEZParamFile.Identity[1] = i;
                    droughtIndex[i - 1] = AEZParamFile.getItemDouble("Value");
                }
                AEZParamFile.PathNames[2] = "Precipitation"; 
                for (int i = min; i <= max; i++)
                {
                    AEZParamFile.Identity[1] = i;
                    Precipitation[i - 1] = AEZParamFile.getItemDouble("Value");
                }
            }
            else
            {
                AEZParamFile.PathNames.Add("AverageAirTemperature");
                AEZParamFile.Identity.Add(-1);
                averageAirTemperature = AEZParamFile.getItemDouble("Value");
                AEZParamFile.PathNames[1]="AirTemperatureMaxDay";

                int temp = AEZParamFile.getItemInt("Value");
                airtemperatureOffset = temp + 94;
                AEZParamFile.PathNames[1] = "AirTemperaturAmplitude";
       
                airtemperatureAmplitude = AEZParamFile.getItemDouble("Value");
            }

            AEZParamFile.setPath(basePath + ".GrazingMidpoint(-1)");
            grazingMidpoint = AEZParamFile.getItemInt("Value");
            
            AEZParamFile.setPath(basePath + ".UrineNH3EF(-1)");
            urineNH3EmissionFactor = AEZParamFile.getItemDouble("Value");

            AEZParamFile.setPath(basePath + ".Manure(-1).EFN2O(-1)");
            manureN20EmissionFactor = AEZParamFile.getItemDouble("Value");
            AEZParamFile.setPath(basePath + ".Manure(-1).N2Factor(-1)");
            manureN2Factor = AEZParamFile.getItemDouble("Value");
            AEZParamFile.setPath(basePath + ".CropResidues(-1).EFN2O(-1)");
            residueN2OEmissionFactor = AEZParamFile.getItemDouble("Value");
            AEZParamFile.setPath(basePath + ".MineralisedSoilN(-1).EFN2O(-1)");
            soilN2OEmissionFactor = AEZParamFile.getItemDouble("Value");

            AEZParamFile.setPath("AgroecologicalZone(" + zone_nr.ToString() + ").ManureApplicationTechnique");
            int maxApp = 0, minApp = 99;
            AEZParamFile.getSectionNumber(ref minApp, ref maxApp);
            themanureAppData = new List<manureAppData>();
            AEZParamFile.Identity.Add(-1);
            for (int j = minApp; j <= maxApp; j++)
            {
                AEZParamFile.Identity[1] = j;
                manureAppData newappData = new manureAppData();
                newappData.name = AEZParamFile.getItemString("Name");
                string RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").ManureApplicationTechnique" + '(' + j.ToString() + ").NH3ReductionFactor(-1)";
                newappData.NH3EmissionReductionFactor = AEZParamFile.getItemDouble("Value", RecipientPath);
                themanureAppData.Add(newappData);
                break;
            }

            AEZParamFile.setPath("AgroecologicalZone(" + zone_nr.ToString() + ").SoilType");
            int maxSoil = 0, minSoil = 99;
            AEZParamFile.getSectionNumber(ref minSoil, ref maxSoil);
            thesoilData = new List<soilData>();
            AEZParamFile.Identity.Add(-1);
            for (int j = minSoil; j <= maxSoil; j++)
            {
                AEZParamFile.Identity[1] = j;
                soilData newsoilData = new soilData();
                string RecipientStub = "AgroecologicalZone(" + zone_nr.ToString() + ").SoilType" + '(' + j.ToString() + ").";
                string RecipientPath = RecipientStub;
                newsoilData.name = AEZParamFile.getItemString("Name", RecipientPath);
                RecipientPath = RecipientStub + "N2Factor(-1)";
                newsoilData.N2Factor = AEZParamFile.getItemDouble("Value", RecipientPath);
                RecipientPath = RecipientStub + "ClayFraction(-1)";
                newsoilData.ClayFraction = AEZParamFile.getItemDouble("Value", RecipientPath);
                RecipientPath = RecipientStub + "MaxSoilDepth(-1)";
                newsoilData.maxSoilDepth = AEZParamFile.getItemDouble("Value", RecipientPath);
                RecipientPath = RecipientStub + "ThermalDiffusivity(-1)";
                newsoilData.thermalDiff = AEZParamFile.getItemDouble("Value", RecipientPath);
                newsoilData.dampingDepth = newsoilData.CalcDampingDepth(newsoilData.thermalDiff, GlobalVars.Instance.Getrho());
 
                RecipientStub = "AgroecologicalZone(" + zone_nr.ToString() + ").SoilType(" + j.ToString() + ").C-Tool";
                AEZParamFile.setPath(RecipientStub);
                int maxHistory = 0, minHistory = 99;
                AEZParamFile.getSectionNumber(ref minHistory, ref maxHistory);
                newsoilData.theC_ToolData = new List<C_ToolData>();
                AEZParamFile.Identity.Add(-1);
                for (int k = minHistory; k <= maxHistory; k++)
                {
                    AEZParamFile.Identity[1] = k;
                    C_ToolData newC_ToolData = new C_ToolData();
                    RecipientStub = "AgroecologicalZone(" + zone_nr.ToString() + ").SoilType(" + j.ToString() + ").C-Tool" + '(' + k.ToString() + ").";
                    RecipientPath = RecipientStub + "InitialC(-1)";
                    newC_ToolData.initialC = AEZParamFile.getItemDouble("Value", RecipientPath);
                    RecipientPath = RecipientStub + "propHUM(-1)";
                    newC_ToolData.propHUM = AEZParamFile.getItemDouble("Value",RecipientPath);
                    RecipientPath = RecipientStub + "propHUM(-1)";
                    newC_ToolData.propHUM = AEZParamFile.getItemDouble("Value", RecipientPath);
                    RecipientPath = RecipientStub + "propROM(-1)";
                    newC_ToolData.propROM = AEZParamFile.getItemDouble("Value", RecipientPath);
                    RecipientPath = RecipientStub + "InitialCtoN(-1)";
                    newC_ToolData.InitialCtoN = AEZParamFile.getItemDouble("Value", RecipientPath);
                    RecipientPath = RecipientStub + "InitialFOMCtoN(-1)";
                    newC_ToolData.InitialFOMCtoN = AEZParamFile.getItemDouble("Value", RecipientPath);
               
                    newsoilData.theC_ToolData.Add(newC_ToolData);
                }
                thesoilData.Add(newsoilData);
            }

            AEZParamFile.setPath("AgroecologicalZone(" + zone_nr.ToString() + ").Fertiliser(-1).EFN2O(-1)");
            fertiliserN20EmissionFactor = AEZParamFile.getItemDouble("Value");
            AEZParamFile.setPath("AgroecologicalZone(" + zone_nr.ToString() + ").Fertiliser(-1).FertiliserType");

            int maxFert = 0, minFert = 99;
            AEZParamFile.getSectionNumber(ref minFert, ref maxFert);
            theFertManData = new List<fertiliserData>();
            AEZParamFile.Identity.Add(-1);
            for (int j = minFert; j < maxFert; j++)
            {
                AEZParamFile.Identity[2] = j;
                fertiliserData newfertData = new fertiliserData();
                newfertData.name = AEZParamFile.getItemString("Description");
                string RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Fertiliser(-1).FertiliserType" + '(' + j.ToString() + ").EFNH3(-1)";
                newfertData.fertManNH3EmissionFactor = AEZParamFile.getItemDouble("Value", RecipientPath);
                newfertData.fertManNH3EmissionFactorHousingRefTemperature = 0;
                theFertManData.Add(newfertData);
            }

            string tmpPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Manure(-1).ManureType";
            AEZParamFile.setPath(tmpPath);

            int maxMan = 0, minMan= 99;
            AEZParamFile.getSectionNumber(ref minMan, ref maxMan);
            AEZParamFile.Identity.Add(-1);
            for (int j = minMan; j < maxMan; j++)
            {
                //AEZParamFile.setPath(basePath);
                AEZParamFile.Identity[2] = j;
                fertiliserData newfertData = new fertiliserData();

                newfertData.storageType= AEZParamFile.getItemInt("StorageType");
                newfertData.speciesGroup= AEZParamFile.getItemInt("SpeciesGroup");
                newfertData.name = AEZParamFile.getItemString("Name");
                string RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Manure(-1).ManureType" + '(' + j.ToString() + ").EFNH3FieldRef(-1)";
                newfertData.fertManNH3EmissionFactor = AEZParamFile.getItemDouble("Value", RecipientPath);
                RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Manure(-1).ManureType" + '(' + j.ToString() + ").EFNH3FieldRefTemperature(-1)";
                newfertData.fertManNH3EmissionFactorHousingRefTemperature = AEZParamFile.getItemDouble("Value", RecipientPath);
                theFertManData.Add(newfertData);
                break;
            }
           
        }
         
    }
    public zoneSpecificData theZoneData;
    public void readGlobalConstants()
    {
        FileInformation constants = new FileInformation(GlobalVars.Instance.getConstantFilePath(), GlobalVars.Instance.GeterrorFileName());
        constants.setPath("constants(0)");
        constants.Identity.Add(-1);
        constants.PathNames.Add("humification_const");
        humification_const=constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "alpha";
        alpha = constants.getItemDouble("Value");

        constants.PathNames[constants.PathNames.Count - 1] = "rgas";
        rgas = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "CNhum";
        CNhum = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "tor";
        tor = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "Eapp";
        Eapp = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "CO2EqCH4";
        CO2EqCH4 = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "CO2EqN2O";
        CO2EqN2O = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "IndirectNH3N2OFactor";
        IndirectNH3N2OFactor = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "IndirectNO3N2OFactor";
        IndirectNO3N2OFactor = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "defaultBeddingCconc";
        defaultBeddingCconc = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "defaultBeddingNconc";
        defaultBeddingNconc = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "ErrorToleranceYield";
        maxToleratedErrorYield = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "ErrorToleranceGrazing";
        maxToleratedErrorGrazing = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "EFNO3_IPCC";
        EFNO3_IPCC = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "minimumTimePeriod";
        minimumTimePeriod = constants.getItemInt("Value");
        
        List<int> theInventorySystems = new List<int>();
        constants.setPath("constants(0).InventorySystem");
        int maxInvSysts = 0, minInvSysts = 99;
        constants.getSectionNumber(ref minInvSysts, ref maxInvSysts);
        constants.Identity.Add(-1);
        for (int i = minInvSysts; i <= maxInvSysts; i++)
        {
            constants.Identity[constants.Identity.Count - 1] = i;
            //theInventorySystems.Add(constants.getItemInt("Value");
            theInventorySystems.Add(i);
        }
     }

     private string constantFilePath;
     public void setConstantFilePath(string path)
     {
         constantFilePath = path;
     }
     public string getConstantFilePath()
     {
         return constantFilePath;
     }

     private string ParamFilePath;
     public void setParamFilePath(string path)
     {
         ParamFilePath = path;
     }
     public string getParamFilePath()
     {
         return ParamFilePath;
     }
     private string farmFilePath;
     public void setFarmtFilePath(string path)
     {
         farmFilePath = path;
     }
     public string getFarmFilePath()
     {
         return farmFilePath;
     }
     private string feeditemPath;
     public void setFeeditemFilePath(string path)
     {
         feeditemPath = path;
     }
     public string getfeeditemFilePath()
     {
         return feeditemPath;
     }
     private string fertManPath;
     public void setfertManFilePath(string path)
     {
         fertManPath = path;
     }
     public string getfertManFilePath()
     {
         return fertManPath;
     }
     private string errorFileName="error.xml";
     public void seterrorFileName(string path)
     {
         errorFileName = path;
     }
     public string GeterrorFileName() { return errorFileName; }
     public const int totalNumberLivestockCategories = 1;
    public const int totalNumberHousingCategories = 1;
    public const int totalNumberSpeciesGroups = 1;
    public const int totalNumberStorageTypes = 1;
    public const double avgNumberOfDays = 365;
    public const double NtoCrudeProtein = 6.25;
    public const double absoluteTemp = 273.15;
    public const int maxNumberFeedItems = 2000;
    public int getmaxNumberFeedItems() { return maxNumberFeedItems; }
    public double GetavgNumberOfDays() { return avgNumberOfDays; }
    public List<housing> listOfHousing = new List<housing>();

    public List<manureStore> listOfManurestores = new List<manureStore>();

    public class product
    {
        public double Modelled_yield;
        public double Expected_yield;
        public double Potential_yield;
        public double waterLimited_yield;
        public double Grazed_yield;
        public bool Harvested;
        public feedItem composition;
        public int usableForBedding;
        public string Units;
        public product()
        {
        }
        public product(product aProduct)
        {
            Modelled_yield = aProduct.Modelled_yield;
            Expected_yield = aProduct.Expected_yield;
            waterLimited_yield = aProduct.waterLimited_yield;
            Grazed_yield = aProduct.Grazed_yield;
            Potential_yield = aProduct.Potential_yield;
            Harvested = aProduct.Harvested;
            usableForBedding = aProduct.usableForBedding;
            Units = aProduct.Units;
            composition = new feedItem(aProduct.composition);
        }
        public void SetExpectedYield(double aVal) { Expected_yield = aVal; }
        public double GetExpectedYield() { return Expected_yield; }
        public void SetModelled_yield(double aVal) { Modelled_yield = aVal; }
        public void SetwaterLimited_yield(double aVal) { waterLimited_yield = aVal; }
        public double GetModelled_yield() { return Modelled_yield; }
        public double GetwaterLimited_yield() { return waterLimited_yield; }
        public double GetPotential_yield() { return Potential_yield; }
        public void AddExpectedYield(double aVal) { Expected_yield += aVal; }
        public void AddActualAmount(double aVal) { composition.Setamount(composition.Getamount() + aVal); }
        public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
        {
            writer.WriteStartElement("product");

            writer.WriteStartElement("Modelled_yield");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("Modelled_yield");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Modelled_yield);
            writer.WriteEndElement();
            writer.WriteEndElement();
            writer.WriteStartElement("name");

            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("Units");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Units);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("Expected_yield");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("Expected_yield");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Expected_yield);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("Harvested");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("Harvested");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Harvested);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("usableForBedding");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("usableForBedding");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(usableForBedding);
            writer.WriteEndElement();
            writer.WriteEndElement();
            if(composition!=null)
                composition.Write(writer, tabFile);
            
            writer.WriteEndElement();

            tabFile.Write("product"+"\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Modelled_yield" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Modelled_yield.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Expected_yield" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Expected_yield.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Harvested" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Harvested.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("usableForBedding" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(usableForBedding.ToString() + "\n");
           

           
        }
    }
   
    public feedItem thebeddingMaterial = new feedItem();
    public feedItem GetthebeddingMaterial() { return thebeddingMaterial; }

    //need to calculate these values
    public void CalcbeddingMaterial(List<CropSequenceClass> rotationList)
    {
        thebeddingMaterial.Setfibre_conc(0.1); //guess
        double tmp1 = 0;
        double tmp2 = 0;
        double tmp3 = 0;
        //double tmp4 = 0;
        for (int i = 0; i < rotationList.Count; i++)
        {

            CropSequenceClass arotation = rotationList[i];
            for (int j = 0; j < arotation.GettheCrops().Count; j++)
            {
                CropClass acrop = arotation.GettheCrops()[j];
                for (int k = 0; k < acrop.GettheProducts().Count; k++)
                {
                    product aproduct = acrop.GettheProducts()[k];
                    tmp1 += aproduct.usableForBedding * (acrop.getArea() * aproduct.GetExpectedYield()) * (1 - aproduct.composition.GetC_conc()) * aproduct.composition.Getash_conc();
                    tmp3 += aproduct.usableForBedding * (acrop.getArea() * aproduct.GetExpectedYield()) * (1 - aproduct.composition.GetN_conc()) * aproduct.composition.Getash_conc();
                    tmp2 += aproduct.usableForBedding * (acrop.getArea() * aproduct.GetExpectedYield());
                }
            }
        }
        if (tmp2 > 0.0 && tmp3 > 0.0)
        {
            thebeddingMaterial.SetC_conc(tmp1 / tmp2);
            thebeddingMaterial.SetN_conc(tmp3 / tmp2);
        }
        else
        {
            thebeddingMaterial.SetC_conc(GlobalVars.Instance.getdefaultBeddingCconc());
            thebeddingMaterial.SetN_conc(GlobalVars.Instance.getdefaultBeddingNconc());
        }
    }
    public struct manurestoreRecord
    {
        manureStore theStore;
        double propYearGrazing;
        public void SetpropYearGrazing(double aVal) { propYearGrazing = aVal; }
        public manure manureToStorage;
        public bool GetStoresSolid() { return manureToStorage.GetisSolid(); }
        public void SetmanureToStorage(manure amanureToStorage) { manureToStorage = amanureToStorage; }
        public double GetpropYearGrazing() { return propYearGrazing; }
        public manure GetmanureToStorage() { return manureToStorage; }
        public manureStore GettheStore() { return theStore; }
        public void SettheStore(manureStore aStore) { theStore = aStore; }
        public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
        {
            writer.WriteStartElement("manurestoreRecord");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("propYearGrazing");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(propYearGrazing);
            writer.WriteEndElement();
            
            tabFile.Write("manurestoreRecord" + "\n");
            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("propYearGrazing" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(propYearGrazing.ToString() + "\n");

            theStore.Write(writer, tabFile);
            manureToStorage.Write(writer, tabFile);
            writer.WriteEndElement();
        }
    }

    //the theManureExchangeClass is used to keep track of the manure generated on the farm and the manure that must be imported 
    public class theManureExchangeClass
    {
        private List<manure> manuresStored;
        private List<manure> manuresImported;
        public List<manure> GetmanuresImported() { return manuresImported; }
        public List<manure> GetmanuresExported() { return manuresStored; }
        public theManureExchangeClass()
        {
            manuresStored = new List<manure>();
            manuresImported = new List<manure>();
        }
        public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
        {
            writer.WriteStartElement("theManureExchangeClass");

            for (int i = 0; i < manuresStored.Count; i++)
            {
                manuresStored[i].Write(writer, tabFile);
            }
            for (int i = 0; i < manuresImported.Count; i++)
            {
                manuresImported[i].Write(writer, tabFile);
            }
            writer.WriteEndElement();


        }
        //! adds manure to the list of manures available
        public void AddToManureExchange(manure aManure)
        {
            bool gotit = false;
            for (int i = 0; i < manuresStored.Count; i++)
            {
                if (manuresStored[i].isSame(aManure)) //add this manure to an existing manure
                {
                    gotit = true;
                    manuresStored[i].AddManure(aManure);
                    continue;
                }
            }
            if (gotit == false)
                manuresStored.Add(aManure);
        }

        public manure TakeManure(double amountN, int storageType, int speciesGroup)
        {
            bool gotit = false;
            double amountNFound=amountN;
            double amountNNeeded = amountN;
            manure aManure = new manure();
            aManure.SetstorageType(storageType);
            aManure.SetspeciesGroup(speciesGroup);
            int i = 0;
            while ((i < manuresStored.Count)&&(gotit==false))
            {
                if (manuresStored[i].isSame(aManure))
                {
                    gotit = true;
                    manuresStored[i].TakeManure(ref amountNFound, ref aManure);
                    amountNNeeded = amountN - amountNFound;
                }
                else
                    i++;
            }
            //if cannot find this manure or there is none left
            if ((gotit == false) || (amountNNeeded >0))
            {
                i = 0;
                gotit = false;

                while ((i < manuresImported.Count)&&(gotit==false))
                {
                    if (manuresImported[i].isSame(aManure))  //there is already some of this manure that will be imported
                    {
                     //  manure anotherManure = new manure(manuresImported[i]);
                        double proportion = amountNNeeded / manuresImported[i].GetTotalN();
                       /*if (proportion < 1.0)
                        //{
                            System.IO.StreamWriter files = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                            string messageString = "Code not implemented " ;
                            files.WriteLine(messageString);
                            Console.WriteLine(messageString);
                            if (GlobalVars.Instance.getPauseBeforeExit())
                                Console.Read();
   */
                            aManure.SetTAN(manuresImported[i].GetTAN() * proportion);
                            aManure.SetorganicN(manuresImported[i].GetorganicN() * proportion);
                            aManure.SethumicN(manuresImported[i].GethumicN() * proportion);
                            aManure.SethumicC(manuresImported[i].GethumicC() * proportion);
                            aManure.SetnonDegC(manuresImported[i].GetnonDegC() * proportion);
                            aManure.SetdegC(manuresImported[i].GetdegC() * proportion);
                            manuresImported[i].AddManure(aManure);
                            //and for all the other components of manure...
                            gotit = true;
                    }
                    else
                        i++;
                }
                if (gotit!=true)
                    {
                        //find a standard manure of this type
                        
                        FileInformation file = new FileInformation(GlobalVars.instance.getfertManFilePath());
                        file.setPath("manure");
                        int min = 99; int max = 0;
                        file.getSectionNumber(ref min, ref max);
                        file.Identity.Add(-1);
                        int itemNr=0;
                        i = min;
                        while ((i <= max)&&(gotit==false))
                        {
                            file.Identity[0]=i;
                            int StorageTypeFile=file.getItemInt("StorageType");
                            int SpeciesGroupFile = file.getItemInt("SpeciesGroup");
                            if (StorageTypeFile == storageType && SpeciesGroupFile == speciesGroup)
                            {
                                itemNr = i;
                                gotit = true;
                            }
                            i++;
                        }
                        if (gotit == false)
                        {
                            System.IO.StreamWriter files = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                            string messageString = "problem finding manure to import: species group = " + speciesGroup.ToString()
                                + " and storage type = " + storageType.ToString();
                            files.WriteLine(messageString);
                            files.Close();
                            Console.WriteLine(messageString);
                            if (GlobalVars.Instance.getPauseBeforeExit())
                                Console.Read();
                            
                            Environment.Exit(0);
                        }

                        manure anotherManure = new manure("manure", itemNr, amountNNeeded);
                        manuresImported.Add(anotherManure);
                        aManure.AddManure(anotherManure);
                    }              
            }
            return aManure;
        }
    }
    public theManureExchangeClass theManureExchange;
    public void initialiseExcretaExchange()
    {
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            grazedArray[i].ruminantDMgrazed = 0;  //kg
            grazedArray[i].fieldDMgrazed = 0;  //kg
            grazedArray[i].urineC = 0;  //kg
            grazedArray[i].urineN = 0;  //kg
            grazedArray[i].faecesC = 0;  //kg
            grazedArray[i].faecesN = 0;  //kg
        }
    }
    public void SetGrazedDM(double amount, int feedCode)
    {
        grazedArray[feedCode].ruminantDMgrazed += amount;
    }

    public void initialiseFeedAndProductLists()
    {
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            feedItem aproduct = new feedItem();
            allFeedAndProductsUsed[i] = new product();
            allFeedAndProductsUsed[i].composition = aproduct ;
            allFeedAndProductsUsed[i].composition.setFeedCode(i);
            aproduct = new feedItem();
            allFeedAndProductsProduced[i] = new product();
            allFeedAndProductsProduced[i].composition=aproduct;
            allFeedAndProductsProduced[i].composition.setFeedCode(i);
            aproduct = new feedItem();
            allFeedAndProductTradeBalance[i] = new product();
            allFeedAndProductTradeBalance[i].composition=aproduct;
            allFeedAndProductTradeBalance[i].composition.setFeedCode(i);
       }
    }

    public void CalculateTradeBalance()
    {        
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            if (allFeedAndProductsUsed[i].composition.Getamount() > 0)
            {
                if (allFeedAndProductsProduced[i].composition.Getamount() == 0)
                    allFeedAndProductTradeBalance[i].composition.GetStandardFeedItem(i);
            }
            allFeedAndProductTradeBalance[i].composition.AddFeedItem(allFeedAndProductsProduced[i].composition, true);
            allFeedAndProductTradeBalance[i].composition.SubtractFeedItem(allFeedAndProductsUsed[i].composition, true);
        }
    }
    public product GetPlantProductImports()
    {
        product aProduct = new product();
        aProduct.composition = new feedItem();
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            product productToAdd = new product();
            productToAdd.composition = new feedItem();
            if (allFeedAndProductTradeBalance[i].composition.Getamount() < 0)
            {
                productToAdd.composition.AddFeedItem(allFeedAndProductTradeBalance[i].composition, true);
                productToAdd.composition.Setamount(allFeedAndProductTradeBalance[i].composition.Getamount() * -1.0);
                aProduct.composition.AddFeedItem(productToAdd.composition, true);
 //               Console.WriteLine(productToAdd.composition.GetFeedCode().ToString() + " " + productToAdd.composition.Getamount().ToString());
            }
        }
        return aProduct;
    }
    
    public product GetPlantProductExports()
    {
        product aProduct = new product();
        aProduct.composition = new feedItem();
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            product productToAdd = new product();
            productToAdd.composition = new feedItem();
            if (allFeedAndProductTradeBalance[i].composition.Getamount() >0)
            {
                productToAdd.composition.AddFeedItem(allFeedAndProductTradeBalance[i].composition, true);
                aProduct.composition.AddFeedItem(productToAdd.composition, true);
            }
        }
        return aProduct;
    }

    public void PrintPlantProducts()
    {
        Console.WriteLine("Feedcode  allFeedAndProductTradeBalance   allFeedAndProductsProduced   allFeedAndProductTradeBalance");
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
           if (allFeedAndProductTradeBalance[i].composition.Getamount() != 0)
            {
                Console.WriteLine(allFeedAndProductTradeBalance[i].composition.GetFeedCode().ToString() + " " +
                    allFeedAndProductsProduced[i].composition.Getamount().ToString() + " " +
                    allFeedAndProductsUsed[i].composition.Getamount().ToString() + " " +
                     allFeedAndProductTradeBalance[i].composition.Getamount());
            }
        }
    }

    public int CheckGrazingData()
    {
        double diff = 0;
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            if (grazedArray[i].fieldDMgrazed > 0)
            {
                double expectedDM =grazedArray[i].fieldDMgrazed;
                double actualDM=grazedArray[i].ruminantDMgrazed ;
                diff = (actualDM - expectedDM) / expectedDM;
                double tolerance = GlobalVars.Instance.getmaxToleratedErrorGrazing();
                if (Math.Abs(diff) > tolerance)
                {
                    
                        double errorPercent = 100 * diff;
                      
                        System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                        file.WriteLine("Error; difference between grazed DM input and modelled grazed DM yield differ by more than the permitted margin");
                        file.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                        file.Close();
                        Console.WriteLine("Error; difference between grazed DM input and modelled grazed DM yield differ by more than the permitted margin");
                        Console.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                        if (GlobalVars.Instance.getPauseBeforeExit()) 
                            Console.Read();
                        Environment.Exit(0);
                   
                }
            }
        }
        return 0;
    }

    public double Temperature(double avgTemperature, double dampingDepth, int day, double depth, double amplitude, int offset)
    {
        double retVal=0;
        double rho = 3.1415926 * 2.0 / (365.0 * 24.0 * 3600.0);
        if (dampingDepth==0)
            retVal = avgTemperature + amplitude * Math.Sin(rho * (day + offset) * 24.0 * 3600.0);
        else
            retVal = avgTemperature + amplitude * Math.Exp(-depth / dampingDepth) * Math.Sin(rho * (day + offset) * 24.0 * 3600.0 - depth / dampingDepth);
        return retVal;
    }

    public double GetDegreeDays(int startDay, int endDay, double basetemperature, double averageTemperature, double amplitude, int offset)
    {
        double retVal = 0;
        for (int i = startDay; i <= endDay; i++)
            retVal += Temperature(averageTemperature, 0.0, i, 0.0, amplitude, offset) - basetemperature;
        return retVal;
    }
    public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
    {
        theManureExchange.Write(writer, tabFile);

        for (int i=0; i < maxNumberFeedItems; i++)
        {
            if (allFeedAndProductsUsed[i].composition.Getamount() > 0)
            {
                allFeedAndProductsUsed[i].composition.Write(writer, tabFile);
                allFeedAndProductsProduced[i].composition.Write(writer, tabFile);
            }
        }
    }
    public void WriteError(string message1, string message2)
    {
        System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
        file.WriteLine(message1);
        file.Write(message2);
        file.Close();
        Console.WriteLine(message1);
        Console.Write(message2);
        if (GlobalVars.Instance.getPauseBeforeExit())
            Console.Read();
    }

}