using System.Collections.Generic;
using System.Xml;
using System;
using System.Diagnostics;
using System.IO;
using System.Xml.Linq;

public class GlobalVars
{
     private static GlobalVars instance;
     public struct avgCFom
     {
         public string parants;
         public double amounts;
  
     }; 
    public List<avgCFom> allAvgCFom=new List<avgCFom>();
    public void addAvgCFom(double amount,string parant)
    {
        avgCFom tmp;
        tmp.parants = parant;
        tmp.amounts = amount;
        allAvgCFom.Add(tmp);
    }
    public struct avgNFom
    {
        public string parants;
        public double amounts;

    };
    public List<avgNFom> allAvgNFom = new List<avgNFom>();
    public void addAvgNFom(double amount, string parant)
    {
        avgNFom tmp;
        tmp.parants = parant;
        tmp.amounts = amount;
        allAvgNFom.Add(tmp);
    }

     public bool header;
     public bool headerLivestock;
     public bool Ctoolheader;
     private Stopwatch sw;
     System.IO.StreamWriter Seyda;
     private GlobalVars()
     {
         header = false;
         headerLivestock = false;
         Ctoolheader = false;
     }
     public static GlobalVars Instance
     {
         get
         {
             if (instance == null)
             {
                 instance = new GlobalVars();
                 instance.sw = new Stopwatch();
                 instance.sw.Start();
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
         public string name;
         public string parens;
         public void Write()
         {
             GlobalVars.Instance.writeStartTab("GrazedItem");
             GlobalVars.Instance.writeInformationToFiles("name", "Name", "-", name, parens);
             if (GlobalVars.Instance.getRunFullModel() == true)
             {
                 GlobalVars.Instance.writeInformationToFiles("fieldDMgrazed", "FieldDMgrazed", "kg", fieldDMgrazed, parens);
                 GlobalVars.Instance.writeInformationToFiles("ruminantDMgrazed", "ruminantDMgrazed", "kg", ruminantDMgrazed, parens);
             }
             else
             {
                 GlobalVars.Instance.writeInformationToFiles("fieldDMgrazed", "FieldDMgrazed", "kg", fieldDMgrazed, parens);
                 GlobalVars.Instance.writeInformationToFiles("ruminantDMgrazed", "ruminantDMgrazed", "kg", ruminantDMgrazed, parens);
             }


             GlobalVars.Instance.writeEndTab();
         }
     }
     public grazedItem[] grazedArray = new grazedItem[maxNumberFeedItems];
     public product[] allFeedAndProductsUsed = new product[maxNumberFeedItems];
     public product[] allFeedAndProductsProduced = new product[maxNumberFeedItems];
     public product[] allFeedAndProductsPotential = new product[maxNumberFeedItems];
     public product[] allFeedAndProductTradeBalance = new product[maxNumberFeedItems];
     public product[] allFeedAndProductFieldProduction = new product[maxNumberFeedItems];
     private product[] allUnutilisedGrazableFeed = new product[maxNumberFeedItems];

    //constants
     private double humification_const;
     private double alpha;
     private double rgas;
     private double CNhum;
     private double tor;
     private double Eapp;
     private double CO2EqCH4;
     private double CO2EqN2O;
     private double CO2EqsoilC;
     private double IndirectNH3N2OFactor;
     private double IndirectNO3N2OFactor;
     private double defaultBeddingCconc;
     private double defaultBeddingNconc;
     private double maxToleratedErrorYield; 
     private double maxToleratedErrorGrazing;
     private int maximumIterations;
     private double EFNO3_IPCC;
     private double digestEnergyToME = 0.81;
     private int minimumTimePeriod;
     private int adaptationTimePeriod;
     private List<int> theInventorySystems;
     private int currentInventorySystem;
     private int currentEnergySystem;
     bool strictGrazing;
     public bool logFile;
     public bool logScreen;
     public int verbocity;
     public int reuseCtoolData;
     public System.IO.StreamWriter logFileStream;

    private int zoneNr;
     public double getHumification_const() { return humification_const; }
    public double getalpha() { return alpha; }
    public double getrgas() { return rgas; }
    public double getCNhum() { return CNhum; }
    public double gettor() { return tor; }
    public double getEapp() { return Eapp; }
    public double GetCO2EqCH4() { return CO2EqCH4; }
    public double GetCO2EqN2O() { return CO2EqN2O; }
    public double GetCO2EqsoilC() { return CO2EqsoilC; }
    public double GetIndirectNH3N2OFactor() { return IndirectNH3N2OFactor; }
    public double GetIndirectNO3N2OFactor() { return IndirectNO3N2OFactor; }
    public int GetminimumTimePeriod() { return minimumTimePeriod; }
    public int GetadaptationTimePeriod() { return adaptationTimePeriod; }
    public bool GetstrictGrazing() { return strictGrazing; }
    public int GetmaximumIterations() { return maximumIterations; }
    public double GetdigestEnergyToME(){return digestEnergyToME;}
    public int GetZone() { return zoneNr; }
    public void SetZone(int zone) { zoneNr=zone; }
    public double getdefaultBeddingCconc() { return defaultBeddingCconc; }
    public double getdefaultBeddingNconc() { return defaultBeddingNconc; }
    public int getcurrentInventorySystem() { return currentInventorySystem; }
    public int getcurrentEnergySystem() { return currentEnergySystem; }
    public double getEFNO3_IPCC() { return EFNO3_IPCC; }
    public double getmaxToleratedErrorYield() { return maxToleratedErrorYield; }
    public double getmaxToleratedErrorGrazing() { return maxToleratedErrorGrazing; }

    public void setcurrentInventorySystem(int aVal) { currentInventorySystem = aVal; }
    public void setcurrentEnergySystem(int aVal) { currentEnergySystem = aVal; }
    public void SetstrictGrazing(bool aVal) { strictGrazing = aVal; }
    public double Getrho() { return 3.1415926 * 2.0 / (365.0 * 24.0 * 3600.0); }
    private bool stopOnError;
    private bool pauseBeforeExit;
    public void setPauseBeforeExit(bool stop) { pauseBeforeExit = stop; }
    public bool getPauseBeforeExit() { return pauseBeforeExit; }
    public void setstopOnError(bool stop) { stopOnError = stop; }
    public bool getstopOnError() { return stopOnError; }
    static string parens;
    public void reset(string aParens) {
        instance=null;
        parens = aParens;
    }
    //private int[] errorCodes = new int[100];
    private bool RunFullModel;//forces exit with error if energy requirements not met
    public void setRunFullModel(bool aVal){RunFullModel=aVal;}
    public bool getRunFullModel() { return RunFullModel; }
    public double GetECM(double litres, double percentFat, double percentProtein)
    {
        double retVal = litres * (0.383 * percentFat + 0.242 * percentProtein + 0.7832) / 3.1138;
        return retVal;
    }

    public struct zoneSpecificData
    {
   
        private string debugFileName;
        private System.IO.StreamWriter debugfile;


        public void SetdebugFileName(string aName) { debugFileName = aName; }


        public double[] airTemp;
        private double[] droughtIndex;
        public double[] Precipitation;
        public double[] PotentialEvapoTrans;
        public int[] rainDays;
        private int numberRainyDaysPerYear;
        private double Ndeposition;

        //data read from parameters.xml, fertilisers or manure tag
        public struct fertiliserData
        {
            public int manureType; 
            public int speciesGroup; //livestock type for this manure (not used for fertilisers)
            public double fertManNH3EmissionFactor; //NH3 emission factor for field-applied manure or fertiliser (read from EFNH3)
            public double EFNH3FieldTier2; //Tier 2 NH3 emission for fertiliser (read from EFNH3FieldTier2)
            public double fertManNH3EmissionFactorHousingRefTemperature;
            public string name;
        }

        public struct C_ToolData
        {
            public double initialC;
            public double InitialFOMCtoN;
            public double InitialFOM;
            public double InitialCtoN;
            public double pHUMupperLayer;
            public double pHUMlowerLayer;

        }

        public struct soilLayerData
        {
            public double z_lower;
            public double fieldCapacity;
        }

        public struct soilWaterData
        {
            public double drainageConstant;
            public List<soilLayerData> thesoilLayerData;
        }
        public struct soilData
        {
            public double N2Factor;
            public string name;
            public double ClayFraction;
            public double SandFraction;
            public double maxSoilDepth;
            public double dampingDepth;
           
            public double thermalDiff;
            public double GetdampingDepth() { return dampingDepth; }
            public List<C_ToolData> theC_ToolData;
            public soilWaterData thesoilWaterData;

            public double CalcDampingDepth(double thermalDiff, double rho)
            {
                return Math.Sqrt(3600 * 24 * 2.0 * thermalDiff / rho);
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
        double burntResidueN2OEmissionFactor;
        double burntResidueNH3EmissionFactor;
        double burntResidueNOxEmissionFactor;
        double burntResidueCOEmissionFactor;
        double burntResidueBlackCEmissionFactor;
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
        public double GetburntResidueN2OEmissionFactor() { return burntResidueN2OEmissionFactor; }
        public double GetburntResidueNH3EmissionFactor() { return burntResidueNH3EmissionFactor; }
        public double GetburntResidueNOxEmissionFactor() { return burntResidueNOxEmissionFactor; }
        public double GetburntResidueCOEmissionFactor() { return burntResidueCOEmissionFactor; }
        public double GetburntResidueBlackCEmissionFactor() { return burntResidueBlackCEmissionFactor; }
        public double GetaverageAirTemperature() { return averageAirTemperature; }
        public int GetairtemperatureOffset() { return airtemperatureOffset; }
        public double GetairtemperatureAmplitude() { return airtemperatureAmplitude; }
        public int GetgrazingMidpoint() { return grazingMidpoint; }
        public void SetaverageYearsToSimulate(double aVal) { averageYearsToSimulate = aVal; }
        public double GetaverageYearsToSimulate() { return averageYearsToSimulate; }
        public void SetNdeposition(double aVal) { Ndeposition = aVal; }
        public double GetNdeposition() { return Ndeposition; }
        

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
                MeanDroughtIndex += droughtIndex[monthCount - 1] * time.GetDaysInMonth(monthCount);
            }
            double startMonthAmount = ((time.GetDaysInMonth(startMonth) - startDay) / time.GetDaysInMonth(startMonth)) * droughtIndex[startMonth - 1];
            MeanDroughtIndex += startMonthAmount;
            double endMonthAmount = ((time.GetDaysInMonth(endMonth) - endDay) / time.GetDaysInMonth(endMonth)) * droughtIndex[endMonth - 1];
            MeanDroughtIndex += endMonthAmount;
            int numbersOfDays = 0;
            for (int i = (startMonth) + 1; i <= endCount - 1; i++)
            {
                int monthCount = i;
                if (i > 12)
                    monthCount -= 12;
                numbersOfDays += time.GetDaysInMonth(monthCount);
            }
            numbersOfDays += (time.GetDaysInMonth(endMonth) - endDay);
            numbersOfDays += (time.GetDaysInMonth(startMonth) - startDay);

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
                MeanTemperature += airTemp[monthCount - 1] * time.GetDaysInMonth(monthCount);
            }
            double startMonthAmount = ((time.GetDaysInMonth(startMonth) - startDay) / time.GetDaysInMonth(startMonth)) * airTemp[startMonth - 1];
            MeanTemperature += startMonthAmount;
            double endMonthAmount = ((time.GetDaysInMonth(endMonth) - endDay) / time.GetDaysInMonth(endMonth)) * airTemp[endMonth - 1];
            MeanTemperature += endMonthAmount;
            int numbersOfDays = 0;
            for (int i = (startMonth) + 1; i <= endCount - 1; i++)
            {
                int monthCount = i;
                if (i > 12)
                    monthCount -= 12;
                numbersOfDays += time.GetDaysInMonth(monthCount);
            }
            numbersOfDays += (time.GetDaysInMonth(endMonth) - endDay);
            numbersOfDays += (time.GetDaysInMonth(startMonth) - startDay);

            return MeanTemperature / numbersOfDays;
        }

        public double GetTemperatureSum(double temperature, double baseTemp)
        {
            double Tsum = 0;
            if (temperature > baseTemp)
                Tsum = temperature - baseTemp;
            return Tsum;
        }

        public double GetPeriodTemperatureSum(int startDay, int startMonth, int startYear, int endDay, int endMonth, int endYear, double baseTemp)
        {
            timeClass time = new timeClass();
            double Tsum = 0;
            int endCount = endMonth;
            if (endYear > startYear)
                endCount += 12;
            for (int i = (startMonth) + 1; i <= endCount - 1; i++)
            {
                int monthCount = i;
                if (i > 12)
                    monthCount -= 12;
                Tsum += GetTemperatureSum(airTemp[monthCount - 1], baseTemp) * time.GetDaysInMonth(monthCount);
            }
            Tsum += GetTemperatureSum(airTemp[startMonth - 1], baseTemp) * (time.GetDaysInMonth(startMonth) - startDay);
            Tsum += GetTemperatureSum(airTemp[endMonth - 1], baseTemp) * (time.GetDaysInMonth(endMonth) - endDay);
            return Tsum;
        }

        public double GetPropLeaching(int startDay, int startMonth, int endDay, int endMonth)
        {
            timeClass time = new timeClass();
            double PropLeaching = 0;
            int numbersOfDays = 0;
            int endMonthCount = endMonth;
            int startMonthCount = startMonth;
            if (((endMonth == startMonth)&&(endDay<startDay))||(endMonth<startMonth))
                    endMonthCount = 12 + endMonth;
            for (int i = startMonthCount + 1; i <= endMonthCount - 1; i++)
            {
                int monthCount=i;
                if (i > 12)
                    monthCount -= 13;
                else
                    monthCount -= 1;
                if (droughtIndex[monthCount]==0)
                    PropLeaching += time.GetDaysInMonth(monthCount);
                numbersOfDays += time.GetDaysInMonth(monthCount);
                //GlobalVars.Instance.log(i + " " + monthCount + " " + PropLeaching.ToString());
            }
            double startMonthAmount = 0;
            if (droughtIndex[startMonth-1]== 0)
                startMonthAmount = time.GetDaysInMonth(startMonth) - startDay + 1;
            PropLeaching += startMonthAmount;
            numbersOfDays += (time.GetDaysInMonth(startMonth) - startDay + 1);
            double endMonthAmount = 0;
            if (droughtIndex[endMonth - 1] == 0)
                endMonthAmount = time.GetDaysInMonth(endMonth) - endDay;
            PropLeaching += endMonthAmount;
            numbersOfDays += time.GetDaysInMonth(endMonth) - endDay;

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
                monthCount -= 12;
            else
                monthCount -= 1;
            if (droughtIndex[monthCount]==0)
                PropFieldCapacity += time.GetDaysInMonth(monthCount);
        }
        double startMonthAmount=0;
        if (droughtIndex[cropData.GetStartMonth() - 1] == 0)
            startMonthAmount = time.GetDaysInMonth(cropData.GetStartMonth()) - cropData.GetStartDay() + 1;
        PropFieldCapacity += startMonthAmount;
        double endMonthAmount = 0;
        if (droughtIndex[cropData.GetEndMonth() - 1] == 0)
            endMonthAmount = cropData.GetEndDay();
        PropFieldCapacity += endMonthAmount;
        int numbersOfDays=0;
        for (int i = startMonthCount + 1; i <= endMonthCount; i++)
        {
            int monthCount = i;
            if (i > 12)
                monthCount -= 12;
            else
                monthCount -= 1;
            numbersOfDays += time.GetDaysInMonth(monthCount);//Jonas - is this correct?
        }
        numbersOfDays += cropData.GetEndDay();
        numbersOfDays += (time.GetDaysInMonth(cropData.GetStartMonth()) - cropData.GetStartDay());
        double ret_val = PropFieldCapacity / numbersOfDays;

        return ret_val;
 }

        //!Generate temperature model parameters from monthly temperature data
    public void CalcTemperatureParameters()
    {
        double minTemp = 300;
        double maxTemp = -300;
        int maxMonth = 0;
        averageAirTemperature = 0;
        for (int i = 1; i <= 12; i++)
        {
            averageAirTemperature += airTemp[i - 1];
            if (airTemp[i - 1] > maxTemp)
            {
                maxTemp = airTemp[i - 1];
                maxMonth = i;
            }
            if (airTemp[i - 1] < minTemp)
                minTemp = airTemp[i - 1];
        }
        averageAirTemperature /= 12.0;
        airtemperatureAmplitude = (maxTemp - minTemp) / 2;
        if ((maxMonth > 3) && (maxMonth <= 9))
            airtemperatureOffset = 245;//Northern hemisphere
        else
            airtemperatureOffset = 65;//Southern hemisphere
    }

        public void readZoneSpecificData(int zone_nr, int currentFarmType)
        {
            FileInformation AEZParamFile = new FileInformation(GlobalVars.Instance.getParamFilePath());
            //get zone-specific constants
            string basePath = "AgroecologicalZone(" + zone_nr.ToString() + ")";
            AEZParamFile.setPath(basePath);
            airTemp = new double[12];
            droughtIndex = new double[12];
            Precipitation = new double[12];
            Precipitation = new double[12];
            PotentialEvapoTrans = new double[12];
            rainDays = new int[12];
            double cumulativePrecip = 0;
            numberRainyDaysPerYear = AEZParamFile.getItemInt("Value", basePath + ".NumberRaindays(-1)");
            AEZParamFile.setPath(basePath);
            bool monthlyData = AEZParamFile.getItemBool("MonthlyAirTemp");
            AEZParamFile.PathNames.Add("Month");
            int max = 0; int min = 99;
            AEZParamFile.getSectionNumber(ref min, ref max);
            if ((max - min + 1) != 12)
            {
                string messageString = "Error - number of months in parameters.xml is not 12";
                GlobalVars.instance.Error(messageString);
            }
            AEZParamFile.Identity.Add(-1);
            AEZParamFile.Identity.Add(-1);
            AEZParamFile.PathNames.Add( "DroughtIndex");
            for (int i = min; i <= max; i++)
            {
                AEZParamFile.Identity[1] = i;
                droughtIndex[i - 1] = AEZParamFile.getItemDouble("Value");
            }
            if (monthlyData == true)
            {
                AEZParamFile.PathNames[2] = "AirTemperature";
                for (int i = min; i <= max; i++)
                {
                    AEZParamFile.Identity[1] = i;
                    airTemp[i-1] = AEZParamFile.getItemDouble("Value");
                    averageAirTemperature += airTemp[i - 1];
                }
                averageAirTemperature /= 12.0;
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
                    cumulativePrecip += Precipitation[i - 1];
                }
                AEZParamFile.PathNames[2] = "PotentialEvapoTrans";
                for (int i = min; i <= max; i++)
                {
                    AEZParamFile.Identity[1] = i;
                    PotentialEvapoTrans[i - 1] = AEZParamFile.getItemDouble("Value");
                }
                int checkdays = 0;
                for (int i = 0; i < 12; i++)
                {
                    rainDays[i] = (int)Math.Round(numberRainyDaysPerYear * (Precipitation[i] / cumulativePrecip));
                    checkdays += rainDays[i];
                }
                CalcTemperatureParameters();
            }
            else
            {
                AEZParamFile.setPath(basePath + ".AverageAirTemperature(-1)");
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
            string tempPath = basePath + ".CropResidues(-1).EFN2O(-1)";
            residueN2OEmissionFactor = AEZParamFile.getItemDouble("Value", tempPath);
            tempPath = basePath + ".CropResidues(-1).EFN2O_burning(-1)";
            burntResidueN2OEmissionFactor = AEZParamFile.getItemDouble("Value", tempPath);
            tempPath = basePath + ".CropResidues(-1).EFNOx_burning(-1)";
            burntResidueNOxEmissionFactor = AEZParamFile.getItemDouble("Value", tempPath);
            tempPath = basePath + ".CropResidues(-1).EFNH3_burning(-1)";
            burntResidueNH3EmissionFactor = AEZParamFile.getItemDouble("Value", tempPath);
            tempPath = basePath + ".CropResidues(-1).EFBlackC_burning(-1)";
            burntResidueBlackCEmissionFactor = AEZParamFile.getItemDouble("Value", tempPath);
            tempPath = basePath + ".CropResidues(-1).EFCO_burning(-1)";
            burntResidueCOEmissionFactor = AEZParamFile.getItemDouble("Value", tempPath);
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
                string RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").ManureApplicationTechnique" + '(' + j.ToString() + ").Name";
                newappData.name = AEZParamFile.getItemString("Name", RecipientPath);
                RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").ManureApplicationTechnique" + '(' + j.ToString() + ").NH3ReductionFactor(-1)";
                newappData.NH3EmissionReductionFactor = AEZParamFile.getItemDouble("Value", RecipientPath);
                themanureAppData.Add(newappData);
            }
            AEZParamFile.setPath("AgroecologicalZone(" + zone_nr.ToString() + ").SoilType");
            int maxSoil = 0, minSoil = 99;
            AEZParamFile.getSectionNumber(ref minSoil, ref maxSoil);
            thesoilData = new List<soilData>();
            AEZParamFile.Identity.Add(-1);
            for (int j = minSoil; j <= maxSoil; j++)
            {
                AEZParamFile.setPath("AgroecologicalZone(" + zone_nr.ToString() + ").SoilType");
                if(AEZParamFile.doesIDExist(j))
                {

                soilData newsoilData = new soilData();
                string RecipientStub = "AgroecologicalZone(" + zone_nr.ToString() + ").SoilType" + '(' + j.ToString() + ").";
                string RecipientPath = RecipientStub;
                newsoilData.name = AEZParamFile.getItemString("Name", RecipientPath);
                RecipientPath = RecipientStub + "N2Factor(-1)";
                newsoilData.N2Factor = AEZParamFile.getItemDouble("Value", RecipientPath);
                RecipientPath = RecipientStub + "SandFraction(-1)";
                newsoilData.SandFraction = AEZParamFile.getItemDouble("Value", RecipientPath);
                RecipientPath = RecipientStub + "ClayFraction(-1)";
                newsoilData.ClayFraction = AEZParamFile.getItemDouble("Value", RecipientPath);
                

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
                    RecipientPath = RecipientStub + "InitialFOMinput(-1)";
                    newC_ToolData.InitialFOM = AEZParamFile.getItemDouble("Value", RecipientPath);
                    RecipientPath = RecipientStub + "InitialFOMCtoN(-1)";
                    newC_ToolData.InitialFOMCtoN = AEZParamFile.getItemDouble("Value", RecipientPath);
                    RecipientPath = RecipientStub + "InitialCtoN(-1)";
                    newC_ToolData.InitialCtoN = AEZParamFile.getItemDouble("Value", RecipientPath);
                    RecipientPath = RecipientStub + "pHUMupperLayer(-1)";
                    newC_ToolData.pHUMupperLayer = AEZParamFile.getItemDouble("Value", RecipientPath);
                    RecipientPath = RecipientStub + "pHUMlowerLayer(-1)";
                    newC_ToolData.pHUMlowerLayer = AEZParamFile.getItemDouble("Value", RecipientPath);
                    newsoilData.theC_ToolData.Add(newC_ToolData);
                }
                RecipientStub = "AgroecologicalZone(" + zone_nr.ToString() + ").SoilType(" + j.ToString() + ").SoilWater(-1)";

                AEZParamFile.setPath(RecipientStub);
                newsoilData.thesoilWaterData = new soilWaterData();
                AEZParamFile.Identity.Add(-1);
                AEZParamFile.setPath(RecipientStub + ".drainageConst(-1)");
                newsoilData.thesoilWaterData = new soilWaterData();
                newsoilData.thesoilWaterData.thesoilLayerData = new List<soilLayerData>();

                newsoilData.thesoilWaterData.drainageConstant = AEZParamFile.getItemDouble("Value");

                AEZParamFile.setPath(RecipientStub + ".layerClass");
                min = 99; max = 0;
                AEZParamFile.getSectionNumber(ref min, ref max);
                for (int index = min; index <= max; index++)
                {
                    soilLayerData anewsoilLayer = new soilLayerData();
                    string temp = RecipientStub + ".layerClass(" + index.ToString() + ").z_lower(-1)";
                    anewsoilLayer.z_lower = AEZParamFile.getItemDouble("Value", temp);
                    //temp = RecipientStub + ".layerClass(" + index.ToString() + ").fieldCapacity(-1)";
                   // anewsoilLayer.fieldCapacity = AEZParamFile.getItemDouble("Value", temp);
                    newsoilData.thesoilWaterData.thesoilLayerData.Add(anewsoilLayer);
                }
                newsoilData.maxSoilDepth = newsoilData.thesoilWaterData.thesoilLayerData[newsoilData.thesoilWaterData.thesoilLayerData.Count-1].z_lower;
                thesoilData.Add(newsoilData);
            }
            }
            AEZParamFile.setPath("AgroecologicalZone(" + zone_nr.ToString() + ").Fertiliser(-1).EFN2O(-1)");
            fertiliserN20EmissionFactor = AEZParamFile.getItemDouble("Value");
            AEZParamFile.setPath("AgroecologicalZone(" + zone_nr.ToString() + ").Fertiliser(-1).FertiliserType");
            int maxFert = 0, minFert = 99;
            AEZParamFile.getSectionNumber(ref minFert, ref maxFert);
            theFertManData = new List<fertiliserData>();
           
            for (int j = minFert; j <= maxFert; j++)
            {
                AEZParamFile.setPath("AgroecologicalZone(" + zone_nr.ToString() + ").Fertiliser(-1).FertiliserType");
                if (AEZParamFile.doesIDExist(j))
                {
                    AEZParamFile.Identity.Add(j);
                    fertiliserData newfertData = new fertiliserData();
                    string RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Fertiliser(-1).FertiliserType" + '(' + j.ToString() + ")";
                    newfertData.name = AEZParamFile.getItemString("Name", RecipientPath);
                    RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Fertiliser(-1).FertiliserType" + '(' + j.ToString() + ").EFNH3(-1)";
                    newfertData.fertManNH3EmissionFactor = AEZParamFile.getItemDouble("Value", RecipientPath);
                    newfertData.fertManNH3EmissionFactorHousingRefTemperature = 0;
                    theFertManData.Add(newfertData);
                
                }
            }
            string tmpPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Manure(-1).ManureType";
            AEZParamFile.setPath(tmpPath);
            int maxMan = 0, minMan= 99;
            AEZParamFile.getSectionNumber(ref minMan, ref maxMan);
            AEZParamFile.Identity.Add(-1);
            for (int j = minMan; j <= maxMan; j++)
            {
                tmpPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Manure(-1).ManureType(-1)";
                AEZParamFile.setPath(tmpPath);
                AEZParamFile.Identity[2] = j;
                fertiliserData newfertData = new fertiliserData();
                newfertData.manureType = AEZParamFile.getItemInt("StorageType");
                newfertData.speciesGroup= AEZParamFile.getItemInt("SpeciesGroup");
                newfertData.name = AEZParamFile.getItemString("Name");
                string RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Manure(-1).ManureType" + '(' + j.ToString() + ").EFNH3FieldRef(-1)";
                newfertData.fertManNH3EmissionFactor = AEZParamFile.getItemDouble("Value", RecipientPath);
                RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Manure(-1).ManureType" + '(' + j.ToString() + ").EFNH3FieldRefTemperature(-1)";
                newfertData.fertManNH3EmissionFactorHousingRefTemperature = AEZParamFile.getItemDouble("Value", RecipientPath);
                RecipientPath = "AgroecologicalZone(" + zone_nr.ToString() + ").Manure(-1).ManureType" + '(' + j.ToString() + ").EFNH3FieldTier2(-1)";
                newfertData.EFNH3FieldTier2 = AEZParamFile.getItemDouble("Value", RecipientPath);
                theFertManData.Add(newfertData);
            }         
        }         
    }
    public zoneSpecificData theZoneData;
    public void readGlobalConstants()
    {
        FileInformation constants = new FileInformation(GlobalVars.Instance.getConstantFilePath());
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
        constants.PathNames[constants.PathNames.Count - 1] = "CO2EqsoilC";
        CO2EqsoilC = constants.getItemDouble("Value");
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

        constants.PathNames[constants.PathNames.Count - 1] = "maximumIterations";
        maximumIterations = constants.getItemInt("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "EFNO3_IPCC";
        EFNO3_IPCC = constants.getItemDouble("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "minimumTimePeriod";
        minimumTimePeriod = constants.getItemInt("Value");
        constants.PathNames[constants.PathNames.Count - 1] = "adaptationTimePeriod";
        adaptationTimePeriod = constants.getItemInt("Value");
        
        List<int> theInventorySystems = new List<int>();
        constants.setPath("constants(0).InventorySystem");
        int maxInvSysts = 0, minInvSysts = 99;
        constants.getSectionNumber(ref minInvSysts, ref maxInvSysts);
        constants.Identity.Add(-1);
        for (int i = minInvSysts; i <= maxInvSysts; i++)
        {
            constants.Identity[constants.Identity.Count - 1] = i;
            theInventorySystems.Add(constants.getItemInt("Value"));
            //theInventorySystems.Add(i);
        }
     }

     private string[] constantFilePath;
     public void setConstantFilePath(string[] path)
     {
         constantFilePath = path;
     }
     public string[] getConstantFilePath()
     {
         return constantFilePath;
     }

     private string[] ParamFilePath;
     public void setParamFilePath(string[] path)
     {
         ParamFilePath = path;
     }
     public string[] getParamFilePath()
     {
         return ParamFilePath;
     }
     private string[] farmFilePath;
     public void setFarmtFilePath(string[] path)
     {
         farmFilePath = path;
     }
     public string[] getFarmFilePath()
     {
         return farmFilePath;
     }
     private string[] feeditemPath;
     public void setFeeditemFilePath(string[] path)
     {
         feeditemPath = path;
     }
     public string[] getfeeditemFilePath()
     {
         return feeditemPath;
     }
     private string[] fertManPath;
     public void setfertManFilePath(string[] path)
     {
         fertManPath = path;
     }
     public string[] getfertManFilePath()
     {
         return fertManPath;
     }
     private string simplesoilModel = "simplesoilModel.xml";
     public void setSimplesoilModelFileName(string path)
     {
         simplesoilModel = path;
     }
     public string getSimplesoilModelFileName() { return simplesoilModel; }
     private string writeHandOverData = "simplesoilModel.xml";
     public void setWriteHandOverData(string path)
     {
         writeHandOverData = path;
     }
     public string getWriteHandOverData() { return writeHandOverData; }
     private string ReadHandOverData = "simplesoilModel.xml";
     public void setReadHandOverData(string path)
     {
         ReadHandOverData = path;
     }
     public string getReadHandOverData() { return ReadHandOverData; }
        
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
        public string Harvested;
        public feedItem composition;
        public int usableForBedding;
        public string Units;
        public bool burn;
        public double ResidueGrazingAmount;
        public product()
        {
            Modelled_yield = 0;
            Expected_yield = 0;
            waterLimited_yield = 0;
            Grazed_yield = 0;
            Potential_yield = 0;
            Harvested = "";
            usableForBedding = 0;
            Units = "";
            burn = false;
            ResidueGrazingAmount = 0;
            composition = new feedItem();
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
            burn = aProduct.burn;
            ResidueGrazingAmount = aProduct.ResidueGrazingAmount; 
            composition = new feedItem(aProduct.composition);
        }
        public void SetExpectedYield(double aVal) { Expected_yield = aVal; }
        public double GetExpectedYield() { return Expected_yield; }
        public void SetModelled_yield(double aVal) { Modelled_yield = aVal; }
        public void SetwaterLimited_yield(double aVal) { waterLimited_yield = aVal; }
        public void SetGrazed_yield(double aVal) { Grazed_yield = aVal; }
        public double GetModelled_yield() { return Modelled_yield; }
        public double GetwaterLimited_yield() { return waterLimited_yield; }
        public double GetPotential_yield() { return Potential_yield; }
        public double GetGrazed_yield() { return Grazed_yield; }
        public void AddExpectedYield(double aVal) { Expected_yield += aVal; }
        public void AddActualAmount(double aVal) { composition.Setamount(composition.Getamount() + aVal); }
        public void Write(string theParens)
        {
            GlobalVars.Instance.writeStartTab("product");
            parens = theParens + "_FeedCode" + composition.GetFeedCode().ToString();
            GlobalVars.Instance.writeInformationToFiles("Name", "Name", "kg/ha", composition.GetName(), parens);
            GlobalVars.Instance.writeInformationToFiles("Potential_yield", "Potential yield", "kg/ha", Potential_yield, parens);
            GlobalVars.Instance.writeInformationToFiles("waterLimited_yield", "Expected yield", "kg/ha", waterLimited_yield, parens);
            GlobalVars.Instance.writeInformationToFiles("Modelled_yield", "Modelled yield", "kg/ha", Modelled_yield, parens);
            GlobalVars.Instance.writeInformationToFiles("Expected_yield", "Expected yield", "kg/ha", Expected_yield, parens);
            GlobalVars.Instance.writeInformationToFiles("Grazed_yield", "Grazed yield", "kg/ha", Grazed_yield, parens);
            GlobalVars.Instance.writeInformationToFiles("Harvested", "Is harvested", "-", Harvested, parens);
            GlobalVars.Instance.writeInformationToFiles("usableForBedding", "Usable for bedding", "-", usableForBedding,parens);

            if(composition!=null)
                composition.Write(parens);

            GlobalVars.Instance.writeEndTab();
           
        }
   
     

        public void WritePlantFile(string theParens,int i, int count)
        {

            parens = theParens + "_FeedCode" + composition.GetFeedCode().ToString();
            if (GlobalVars.Instance.header == false)
            {
                GlobalVars.Instance.writePlantFile("Name", "Name", "kg/ha", composition.GetName(), parens, 0);
                GlobalVars.Instance.writePlantFile("Potential_yield", "Potential yield", "kg/ha", Potential_yield, parens, 0);
                GlobalVars.Instance.writePlantFile("waterLimited_yield", "Expected yield", "kg/ha", waterLimited_yield, parens, 0);
                GlobalVars.Instance.writePlantFile("Modelled_yield", "Modelled yield", "kg/ha", Modelled_yield, parens, 0);
                GlobalVars.Instance.writePlantFile("Expected_yield", "Expected yield", "kg/ha", Expected_yield, parens, 0);
                GlobalVars.Instance.writePlantFile("Grazed_yield", "Grazed yield", "kg/ha", Grazed_yield, parens, 0);
                GlobalVars.Instance.writePlantFile("Harvested", "Is harvested", "-", Harvested, parens, 0);
                if (i == (count - 1))
                GlobalVars.Instance.writePlantFile("usableForBedding", "Usable for bedding", "-", usableForBedding, parens, 1);
                else
                    GlobalVars.Instance.writePlantFile("usableForBedding", "Usable for bedding", "-", usableForBedding, parens, 0);
                
            }
            else if (GlobalVars.Instance.header == true)
            {
                GlobalVars.Instance.writePlantFile("Name", "Name", "kg/ha", composition.GetName(), parens, 0);
                GlobalVars.Instance.writePlantFile("Potential_yield", "Potential yield", "kg/ha", Potential_yield, parens, 0);
                GlobalVars.Instance.writePlantFile("waterLimited_yield", "Expected yield", "kg/ha", waterLimited_yield, parens, 0);
                GlobalVars.Instance.writePlantFile("Modelled_yield", "Modelled yield", "kg/ha", Modelled_yield, parens, 0);
                GlobalVars.Instance.writePlantFile("Expected_yield", "Expected yield", "kg/ha", Expected_yield, parens, 0);
                GlobalVars.Instance.writePlantFile("Grazed_yield", "Grazed yield", "kg/ha", Grazed_yield, parens, 0);
                GlobalVars.Instance.writePlantFile("Harvested", "Is harvested", "-", Harvested, parens, 0);
                if(i==(count-1))
                GlobalVars.Instance.writePlantFile("usableForBedding", "Usable for bedding", "-", usableForBedding, parens, 1);
                else
                    GlobalVars.Instance.writePlantFile("usableForBedding", "Usable for bedding", "-", usableForBedding, parens, 0);
            }
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
        if (rotationList != null)
        {
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
        }
        if (tmp2 > 0.0 && tmp3 > 0.0)
        {
            thebeddingMaterial.SetC_conc(tmp1 / tmp2);
            thebeddingMaterial.SetN_conc(tmp3 / tmp2);
        }
        if (thebeddingMaterial.GetC_conc()==0) //no bedding found on farm or field model not called
        {
            thebeddingMaterial.SetC_conc(GlobalVars.Instance.getdefaultBeddingCconc());
            thebeddingMaterial.SetN_conc(GlobalVars.Instance.getdefaultBeddingNconc());
            thebeddingMaterial.Setname("default beddding");
            thebeddingMaterial.setFeedCode(999);
        }
    }
    public struct manurestoreRecord
    {
        manureStore theStore;
        double propYearGrazing;
        public void SetpropYearGrazing(double aVal) { propYearGrazing = aVal; }
        public manure manureToStorage;
        public void SetmanureToStorage(manure amanureToStorage) { manureToStorage = amanureToStorage; }
        public double GetpropYearGrazing() { return propYearGrazing; }
        public manure GetmanureToStorage() { return manureToStorage; }
        public manureStore GettheStore() { return theStore; }
        public void SettheStore(manureStore aStore) { theStore = aStore; }
        public void Write()
        {
            GlobalVars.Instance.writeStartTab("manurestoreRecord");
            theStore.Write();
            manureToStorage.Write(theStore.Getname().ToString());
            GlobalVars.Instance.writeEndTab();
        }
    }

    //the theManureExchangeClass is used to keep track of the manure generated on the farm and the manure that must be imported 
    public class theManureExchangeClass
    {
        private List<manure> manuresStored;
        private List<manure> manuresProduced;
        private List<manure> manuresImported;
        public List<manure> GetmanuresImported() { return manuresImported; }
        public List<manure> GetmanuresExported() { return manuresStored; }
        public theManureExchangeClass()
        {
            manuresStored = new List<manure>();
            manuresProduced = new List<manure>();
            manuresImported = new List<manure>();
        }
        public void Write()
        {
            GlobalVars.Instance.writeStartTab("theManureExchangeClass");
            GlobalVars.Instance.writeStartTab("producedManure");

            for (int i = 0; i < manuresProduced.Count; i++)
            {
                manuresProduced[i].Write("Produced");
            }
            GlobalVars.Instance.writeEndTab();
            GlobalVars.Instance.writeStartTab("exportedManure");
            for (int i = 0; i < manuresStored.Count; i++)
            {
                if (manuresStored[i].GetTotalN()>0)
                        manuresStored[i].Write("Exported");
            }
            GlobalVars.Instance.writeEndTab();
            GlobalVars.Instance.writeStartTab("importedManure");
            for (int i = 0; i < manuresImported.Count; i++)
            {
                manuresImported[i].Write("Imported");
            }
            GlobalVars.Instance.writeEndTab();
            GlobalVars.Instance.writeEndTab();
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
                    manure newManure = new manure(aManure);
                    manuresProduced[i].AddManure(newManure);
                    continue;
                }
            }
            if (gotit == false)
            {
                manuresStored.Add(aManure);
                manure newManure = new manure(aManure);
                manuresProduced.Add(newManure);
            }
        }

        public manure TakeManure(double amountN, double lengthOfSequence, int manureType, int speciesGroup)
        {
            bool gotit = false;
            double amountNFound = amountN / lengthOfSequence;
            double amountNNeeded = amountN / lengthOfSequence;
            amountN /= lengthOfSequence;
            manure aManure = new manure();
            aManure.SetmanureType(manureType);
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
                         double proportion=0;
                        if(manuresImported[i].GetTotalN()!=0)
                            proportion = amountNNeeded / manuresImported[i].GetTotalN();
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
                        
                        FileInformation file = new FileInformation(GlobalVars.Instance.getfertManFilePath());
                        file.setPath("AgroecologicalZone("+GlobalVars.Instance.GetZone().ToString()+").manure");
                        int min = 99; int max = 0;
                        file.getSectionNumber(ref min, ref max);
                        file.Identity.Add(-1);
                        int itemNr=0;
                        i = min;
                        while ((i <= max)&&(gotit==false))
                        {
                            file.Identity[1]=i;
                            int ManureType = file.getItemInt("ManureType");
                            int SpeciesGroupFile = file.getItemInt("SpeciesGroup");
                            if (ManureType == manureType)
                            {
                                if ((ManureType > 12) || (SpeciesGroupFile == speciesGroup))
                                {
                                    itemNr = i;
                                    gotit = true;
                                }
                            }
                            i++;
                        }
                        if (gotit == false)
                        {
                           
                            string messageString = "problem finding manure to import: species group = " + speciesGroup.ToString()
                                + " and storage type = " + manureType.ToString();
                            GlobalVars.Instance.Error(messageString);
                        }

                        manure anotherManure = new manure("manure", itemNr, amountNNeeded, parens+"_"+i.ToString());
                        manuresImported.Add(anotherManure);
                        aManure.AddManure(anotherManure);
                        aManure.Setname(anotherManure.Getname());
                    }              
            }
            aManure.IncreaseManure(lengthOfSequence);
            return aManure;
        }

    }//end of manure exchange
    private XmlWriter writer;
 //   public XElement writerCtool;
    //public XmlWriter writerCtool;
    private System.IO.StreamWriter tabFile;
    private System.IO.StreamWriter DebugFile;
    private System.IO.StreamWriter PlantFile;
    private System.IO.StreamWriter livestockFile;
    private System.IO.StreamWriter CtoolFile;
    private string plantfileName;
    private string CtoolfileName;
    private string DebugfileName;
    private string livestockfileName;
    string PlantHeaderName;
    string PlantHeaderUnits;
    string CtoolHeaderName;
    string CtoolHeaderUnits;
    string livestockHeaderName;
    string livestockHeaderUnits;
    public XmlWriter OpenOutputXML(string outputName)
    {
        writer = XmlWriter.Create(outputName);
        writer.WriteStartDocument();
        return writer;
    }

    public void CloseOutputXML()
    {
        try
        {
            writer.WriteEndDocument();
            writer.Close();
        }
        catch
        {
        }
   

    }
    public void writeStartTab(string name)
    {
     
        tabFile.Write(name + "\n");
        writer.WriteStartElement(name);

    }
    public void writeEndTab()
    {        
        writer.WriteEndElement();
    }
    public void writeInformationToFiles(string name, string Description, string Units, bool value, string parens)
    {
        writeInformationToFiles(name, Description, Units, Convert.ToString(value), parens);
    }
    public void writeInformationToFiles(string name, string Description, string Units, double value, string parens)
    {
        writeInformationToFiles(name, Description, Units, Convert.ToString(Math.Round(value, 4)), parens);
    }
    public void writeInformationToFiles(string name, string Description, string Units, int value, string parens)
    {
        writeInformationToFiles(name, Description, Units, Convert.ToString(value), parens);
    }
  /*  public void writeSinglItem(string name, double value)
    {
       
        writer.WriteStartElement(name);
        writer.WriteValue(value);
        writer.WriteEndElement();

        tabFile.Write(name + "\t");
        tabFile.Write(value + "\n");
    }*/
    public void writeInformationToFiles(string name, string Description, string Units, string value,string parens)
    {
        writer.WriteStartElement(name);
        writer.WriteStartElement("Description");
        writer.WriteValue(Description);
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue(Units);
        writer.WriteEndElement();
        writer.WriteStartElement("Name");
        writer.WriteValue(name);
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(value);
        writer.WriteEndElement();
        writer.WriteStartElement("StringUI");
        writer.WriteValue(name + parens);
        writer.WriteEndElement();
        writer.WriteEndElement();

        tabFile.Write("Description" + "\t");
        tabFile.Write(Description + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write(Units + "\t");
        tabFile.Write("Name" + "\t");
        tabFile.Write(name + parens + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(value + "\n");
       
    }
    public void writePlantFile(string name, string Description, string Units, bool value, string parens, int specielPlant)
    {
        writePlantFile(name, Description, Units, Convert.ToString(value), parens, specielPlant);
    }
    public void writePlantFile(string name, string Description, string Units, double value, string parens, int specielPlant)
    {
        writePlantFile(name, Description, Units, Convert.ToString(Math.Round(value, 4)), parens, specielPlant);
    }
    public void writePlantFile(string name, string Description, string Units, int value, string parens, int specielPlant)
    {
        writePlantFile(name, Description, Units, Convert.ToString(value), parens, specielPlant);
    }
    public void writePlantFile(string name, string Description, string Units, string value, string parens, int specielPlant)
     {
     if (header == false)
        {
            if (specielPlant == 0)
            {

                PlantHeaderName += name + "\t";
                PlantHeaderUnits += Units + "\t"; ;
            }
            if (specielPlant == 1)
            {
                PlantHeaderName += name;
                PlantHeaderUnits += Units ;
                PlantFile.Write(PlantHeaderName + "\n");
                PlantFile.Write(PlantHeaderUnits + "\n");
            }
        }
        else
        {
            if (specielPlant == 0)
            {
                PlantFile.Write(value + "\t");
            }
            if (specielPlant == 1)
            {
                PlantFile.Write(value + "\n");
            }
        }
}
    public void writeLivestockFile(string name, string Description, string Units, bool value, string parens, int specielPlant)
    {
        writeLivestockFile(name, Description, Units, Convert.ToString(value), parens, specielPlant);
    }
    public void writeLivestockFile(string name, string Description, string Units, double value, string parens, int specielPlant)
    {
        writeLivestockFile(name, Description, Units, Convert.ToString(Math.Round(value, 4)), parens, specielPlant);
    }
    public void writeLivestockFile(string name, string Description, string Units, int value, string parens, int specielPlant)
    {
        writeLivestockFile(name, Description, Units, Convert.ToString(value), parens, specielPlant);
    }
    public void writeLivestockFile(string name, string Description, string Units, string value, string parens, int specielPlant)
    {
        if (headerLivestock == false)
        {
            if (specielPlant == 0)
            {

                livestockHeaderName += name + "\t";
                livestockHeaderUnits += Units + "\t"; ;
            }
            if (specielPlant == 1)
            {
                livestockFile.Write(livestockHeaderName + name + "\n");
                livestockFile.Write(livestockHeaderUnits + Units + "\n");
            }
        }
        else
        {
            if (specielPlant == 0)
            {

                livestockFile.Write(value + "\t");
            }
            if (specielPlant == 1)
            {
                livestockFile.Write(value + "\n");
            }
        }
    }


    public XElement writeCtoolFile(string name, string Description, string Units, bool value, string parens, int specielPlant)
    {
        return writeCtoolFile(name, Description, Units, Convert.ToString(value), parens, specielPlant);
    }
    public XElement writeCtoolFile(string name, string Description, string Units, double value, string parens, int specielPlant)
    {
        return writeCtoolFile(name, Description, Units, Convert.ToString(Math.Round(value, 4)), parens, specielPlant);
    }
    public XElement writeCtoolFile(string name, string Description, string Units, int value, string parens, int specielPlant)
    {
        return writeCtoolFile(name, Description, Units, Convert.ToString(value), parens, specielPlant);
    }
    public XElement writeCtoolFile(string name, string Description, string Units, string value, string parens, int specielPlant)
    {
        
        if (Ctoolheader == false)
        {
            if (specielPlant == 0)
            {

                CtoolHeaderName += name + "\t";
                CtoolHeaderUnits += Units + "\t"; ;
            }
            if (specielPlant == 1)
            {
                CtoolHeaderName += name;
                CtoolHeaderUnits += Units;
                CtoolFile.Write(CtoolHeaderName + "\n");
                CtoolFile.Write(CtoolHeaderUnits + "\n");
            }
            return null;
        }
        else
        {
            if (specielPlant == 0)
            {

                CtoolFile.Write(value + "\t");
            }
            if (specielPlant == 1)
            {
                CtoolFile.Write(value + "\n");
            }
            return writeInformationToCtoolFiles(name, Description, Units, value, parens);
        }
    }
    public XElement writeInformationToCtoolFiles(string name, string Description, string Units, string value, string parens)
    {
        XElement node = new XElement(name);

        XElement DescriptionNode = new XElement("Description");
        DescriptionNode.Value = Description;
        node.Add(DescriptionNode);

        DescriptionNode = new XElement("Units");
        DescriptionNode.Value = Units;
        node.Add(DescriptionNode);

        DescriptionNode = new XElement("name");
        DescriptionNode.Value = name;
        node.Add(DescriptionNode);
        DescriptionNode = new XElement("value");
        DescriptionNode.Value = value;
        node.Add(DescriptionNode);

        DescriptionNode = new XElement("StringUI");
        DescriptionNode.Value = name + parens;
        node.Add(DescriptionNode);
        return node;
        
    }
    public void OpenOutputTabFile(string outputName, string output)
    {
        string tabfileName = outputName + ".xls";
        tabFile = new System.IO.StreamWriter(tabfileName);
        plantfileName = outputName + "plantfile.xls";
        if (File.Exists(plantfileName))
            File.Delete(plantfileName);
        livestockfileName = outputName + "livetockfile.xls";
        CtoolfileName = outputName + "CtoolFile.xls";
        DebugfileName = outputName + "Debug.xls";
    }
    static bool usedPlant = false;
    public void OpenPlantFile()
    {
        if (usedPlant == false)
            PlantFile = new System.IO.StreamWriter(plantfileName);
        else
            PlantFile=File.AppendText(plantfileName);
        usedPlant = true;
    }
    public void ClosePlantFile()
    {
        try
        {
        PlantFile.Close();  
        }
        catch
        {
        }
    }
    public void OpenDebugFile()
    {
        DebugFile = new System.IO.StreamWriter(DebugfileName);
       
    }
    public void CloseDebugFile()
    {
        try
        {
            DebugFile.Write(headerDebug);
            DebugFile.Write(dataDebug);
            DebugFile.Close();
            if (headerDebug==null)
                File.Delete(DebugfileName);
        }
        catch
        {
        }
    }
    private bool DebugHeader=true;
    private string headerDebug;
    private string dataDebug;
    public void WriteDebugFile(string name, int value,char seperater)
    {
        WriteDebugFile(name, value.ToString(),seperater);
    }
    public void WriteDebugFile(string name, double value,char seperater)
    {
        WriteDebugFile(name, value.ToString(),seperater);
    }
    public void WriteDebugFile(string name, string value,char seperater)
    {
        if(DebugHeader==true)
        {
            headerDebug += name + seperater;
        }
        if (seperater == '\n')
            DebugHeader = false;
        dataDebug += value + seperater;
    }
    
    public void OpenCtoolFile()
    {
        CtoolFile = new System.IO.StreamWriter(CtoolfileName);
/*        if (usedCtoolFile == false)
            CtoolFile = new System.IO.StreamWriter(CtoolfileName);
        else
            CtoolFile = File.AppendText(CtoolfileName);
        usedCtoolFile = true;
      */ 
     
    }
    public void CloseCtoolFile()
    {
        CtoolFile.Close();
      
    }
    public void OpenLivestockFile()
    {
        headerLivestock = false;
        livestockFile = new System.IO.StreamWriter(livestockfileName);
    }
    public void CloseLivestockFile()
    {
        try
        {
            livestockFile.Close();
        }
        catch
        {
        }
    }
    public void CloseOutputTabFile()
    {
        try
        {
            tabFile.Close();
        }
        catch
        {
        }

    }

    public System.IO.StreamWriter GetTabFileWriter() { return tabFile; }
    public void Error(string erroMsg, string stakTrace="",bool withExeption = true)
    {
        try
        {
            if (withExeption == true)
            {
                CloseOutputXML();
                CloseOutputTabFile();
                ClosePlantFile();
                CloseLivestockFile();
                CloseCtoolFile();
                CloseDebugFile();
                logFileStream.Close();
            }
        }
        catch(Exception e)
        {
            Console.WriteLine(e.Message);
        }
        if (!erroMsg.Contains("farm Fail"))
        {
            Console.WriteLine(GlobalVars.Instance.GeterrorFileName());
            System.IO.StreamWriter files = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            files.WriteLine(erroMsg + " " + stakTrace);
            files.Close();
            Console.WriteLine(erroMsg + " " + stakTrace);
            sw.Stop();
            Console.WriteLine("RunTime (hrs:mins:secs) " + sw.Elapsed);
            if (GlobalVars.Instance.getPauseBeforeExit())
                Console.Read();
        }
        
        if (withExeption == true)
            throw new System.ArgumentException("farm Fail", "farm Fail");
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
/*    public void SetGrazedDM(double amount, int feedCode, string theName)
    {
        grazedArray[feedCode].ruminantDMgrazed += amount;
        grazedArray[feedCode].name = theName;
    }
    */
    public void writeGrazedItems()// writes the consumed and produced DM for all grazed products/feeditems
    {
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            if ((grazedArray[i].fieldDMgrazed>0.0)||(grazedArray[i].ruminantDMgrazed>0.0))
            grazedArray[i].Write();
        }
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
            allFeedAndProductsPotential[i] = new product();
            allFeedAndProductsPotential[i].composition = aproduct;
            allFeedAndProductsPotential[i].composition.setFeedCode(i);
            aproduct = new feedItem();
            allFeedAndProductFieldProduction[i] = new product();
            allFeedAndProductFieldProduction[i].composition = aproduct;
            allFeedAndProductFieldProduction[i].composition.setFeedCode(i);
            aproduct = new feedItem();
            allFeedAndProductTradeBalance[i] = new product();
            allFeedAndProductTradeBalance[i].composition=aproduct;
            allFeedAndProductTradeBalance[i].composition.setFeedCode(i);
            aproduct = new feedItem();            
            allUnutilisedGrazableFeed[i] = new product();
            allUnutilisedGrazableFeed[i].composition = aproduct;
            allUnutilisedGrazableFeed[i].composition.setFeedCode(i);
       }
    }

    public void AddProductProduced(feedItem anItem)
    {
        allFeedAndProductsProduced[anItem.GetFeedCode()].composition.Setname(anItem.GetName());
        allFeedAndProductsProduced[anItem.GetFeedCode()].composition.AddFeedItem(anItem, false);
    }

    public void AddGrazableProductUnutilised(feedItem anItem)
    {
        allUnutilisedGrazableFeed[anItem.GetFeedCode()].composition.Setname(anItem.GetName());
        allUnutilisedGrazableFeed[anItem.GetFeedCode()].composition.AddFeedItem(anItem, false);
    }

    public void CalculateTradeBalance()
    {        
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            product feedItemUsed = allFeedAndProductsUsed[i];
            product feedItemProduced = allFeedAndProductsProduced[i];
            if ((feedItemUsed.composition.Getamount() > 0) || (feedItemProduced.composition.Getamount() > 0))
            {
                if (feedItemUsed.composition.Getamount() > 0)
                {
                    if (feedItemProduced.composition.Getamount() == 0)
                    {
                        if (feedItemUsed.composition.GetFeedCode() == 999)
                        {
                            allFeedAndProductTradeBalance[i].composition.AddFeedItem(allFeedAndProductsUsed[999].composition, true,true);
                            allFeedAndProductTradeBalance[i].composition.Setamount(0);
                        }
                        else
                            allFeedAndProductTradeBalance[i].composition.GetStandardFeedItem(i);
                    }
                }
                if (feedItemProduced.composition.Getamount()>0)
                    allFeedAndProductTradeBalance[i].composition.AddFeedItem(feedItemProduced.composition, true);
                if (feedItemUsed.composition.Getamount() > 0)
                    allFeedAndProductTradeBalance[i].composition.SubtractFeedItem(feedItemUsed.composition, true);
            }
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
                double N = productToAdd.composition.Getamount() * productToAdd.composition.GetN_conc();

                GlobalVars.Instance.log(productToAdd.composition.GetFeedCode().ToString() + " " + productToAdd.composition.Getamount().ToString("0.0") + " " + N.ToString("0.0"), 5);
            }
        }
        return aProduct;
    }

    public feedItem GetBeddingImported()
    {
        feedItem beddingItem = new feedItem();
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
          if((allFeedAndProductTradeBalance[i].composition.GetbeddingMaterial())&& (allFeedAndProductTradeBalance[i].composition.Getamount()<0))
              beddingItem.AddFeedItem(allFeedAndProductTradeBalance[i].composition,true,true);          
        }
        beddingItem.Setamount(beddingItem.Getamount() * -1);
        return beddingItem;
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

    public void CalcAllFeedAndProductsPotential(List<CropSequenceClass> list)
    {
        for (int i = 0; i < list.Count; i++)
        {
            List<CropClass> crops=list[i].GettheCrops();
            int yearsInSequence = list[i].GetlengthOfSequence();
            for (int j = 0; j < crops.Count; j++)
            {
                List<product> products = crops[j].GettheProducts();
                for (int k = 0; k < products.Count; k++)
                {
                    int feedCode=products[k].composition.GetFeedCode();
                    if (products[k].Potential_yield > 0)
                    {
                        allFeedAndProductsPotential[feedCode].composition.Addamount(crops[j].getArea() * products[k].Potential_yield / yearsInSequence);
                        allFeedAndProductsPotential[feedCode].composition.Setname(products[k].composition.GetName());
                    }
                    if (products[k].Modelled_yield > 0)
                    {
                        allFeedAndProductFieldProduction[feedCode].composition.Addamount(crops[j].getArea() * products[k].Modelled_yield / yearsInSequence);
                        allFeedAndProductFieldProduction[feedCode].composition.Setname(products[k].composition.GetName());
                    }
                }
                
            }
        }
    }

    public void GetRoughageExchange(ref double importedRoughageDM, ref double exportedRoughageDM)
    {
        importedRoughageDM=0;
        exportedRoughageDM=0;
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            int feedCode = allFeedAndProductTradeBalance[i].composition.GetFeedCode();
            double amount =allFeedAndProductTradeBalance[i].composition.Getamount();
            if ((feedCode>=400)&&(feedCode<700))
            {
                if (amount>0)
                    exportedRoughageDM+=amount;
                if (amount<0)
                    importedRoughageDM-=amount;
            }
        }
    }
    
    public void PrintPlantProducts()
    {
        double totDM = 0;
        double totN = 0;
        double totC = 0;
        GlobalVars.Instance.log("Fdcode FdAndProdProduced FdAndProdUsed  FdAndProdTradeBalance",5);
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            if ((allFeedAndProductsProduced[i].composition.Getamount() != 0)||(allFeedAndProductsUsed[i].composition.Getamount()!=0))
            {
                totDM += allFeedAndProductsProduced[i].composition.Getamount();
                totN += allFeedAndProductsProduced[i].composition.Getamount() * allFeedAndProductsProduced[i].composition.GetN_conc();
                totC += allFeedAndProductsProduced[i].composition.Getamount() * allFeedAndProductsProduced[i].composition.GetC_conc();

                GlobalVars.Instance.log(allFeedAndProductTradeBalance[i].composition.GetFeedCode().ToString() + " " +
                    allFeedAndProductsProduced[i].composition.Getamount().ToString("0.0") + " " +
                    allFeedAndProductsUsed[i].composition.Getamount().ToString("0.0") + " " +
                     allFeedAndProductTradeBalance[i].composition.Getamount().ToString("0.0"), 5);
            }
        }
        GlobalVars.Instance.log("Tot DM produced " + totDM.ToString("0.0"), 5);
        GlobalVars.Instance.log("Tot C produced " + totC.ToString("0.0"), 5);
        GlobalVars.Instance.log("Tot N produced " + totN.ToString("0.0"), 5);
    }

    public int CheckGrazingData()
    {
        double diff = 0;
        for (int i = 0; i < maxNumberFeedItems; i++)
        {
            if ((grazedArray[i].fieldDMgrazed > 0)||(grazedArray[i].ruminantDMgrazed>0.0))
            {
                double expectedDM =grazedArray[i].fieldDMgrazed;
                double actualDM=grazedArray[i].ruminantDMgrazed ;
                if (expectedDM == 0.0)
                {
                    string messageString = "Error; grazed feed item not produced on farm.\n feed code = " + i.ToString();
                    GlobalVars.instance.Error(messageString);

                }
                else
                {
                    diff = (expectedDM - actualDM)/ expectedDM;
                    double tolerance = GlobalVars.Instance.getmaxToleratedErrorGrazing();
                    if ((tolerance<=1) && (Math.Abs(diff) > tolerance))
                    {
                        double errorPercent = 100 * diff;
                        string productName = grazedArray[i].name;
                        string messageString = "Error; consumed grazed DM and modelled grazed DM " +
                            productName + " differ by more than the permitted margin.\n Percentage error = " + errorPercent.ToString("0.00") + "%";
                        GlobalVars.instance.Error(messageString);
                    }
                }
            }
        }
        return 0;
    }

    public double Temperature(double avgTemperature, double dampingDepth, int day, double depth, double amplitude, int offset)
    {
        double retVal = 0;
        double rho = 3.1415926 * 2.0 / 365.0;
        if (dampingDepth == 0)
            retVal = avgTemperature + amplitude * Math.Sin(rho * (day + offset));
        else
            retVal = avgTemperature + amplitude * Math.Exp(-depth / dampingDepth) * Math.Sin(rho * (day + offset) - depth / dampingDepth);
        return retVal;
    }


    public double GetDegreeDays(int startDay, int endDay, double basetemperature, double averageTemperature, double amplitude, int offset)
    {
        double retVal = 0;
        for (int i = startDay; i <= endDay; i++)
            retVal += Temperature(averageTemperature, 0.0
                , i, 0.0, amplitude, offset) - basetemperature;
        return retVal;
    }
    public void Write(bool fullModelRun)
    {
        theManureExchange.Write();
        if (!fullModelRun)
        {
             GlobalVars.Instance.writeStartTab("PotentialBalance");
             for (int i = 0; i < maxNumberFeedItems; i++)
             {
                 if ((allFeedAndProductsUsed[i].composition.Getamount() > 0)||(allFeedAndProductsPotential[i].composition.Getamount() > 0))
                 {
                     GlobalVars.Instance.writeStartTab("FeedItem");
                     GlobalVars.Instance.writeInformationToFiles("name", "Name", "-", allFeedAndProductsUsed[i].composition.GetName(), parens);
                     GlobalVars.Instance.writeInformationToFiles("Used", "Used", "kg", allFeedAndProductsUsed[i].composition.Getamount(), parens);
                     GlobalVars.Instance.writeInformationToFiles("Potential", "Potential", "kg", allFeedAndProductsPotential[i].composition.Getamount(), parens);
                     GlobalVars.Instance.writeInformationToFiles("Expected", "Expected", "kg",allFeedAndProductsProduced[i].composition.Getamount(), parens);
                     GlobalVars.Instance.writeEndTab();
                 }
             }
             GlobalVars.Instance.writeEndTab();
         }
        
        if (fullModelRun)
        {
            GlobalVars.Instance.writeStartTab("FeedAndProductsUsed");
            for (int i = 0; i < maxNumberFeedItems; i++)
            {
                if (allFeedAndProductsUsed[i].composition.Getamount() > 0)
                {
                    allFeedAndProductsUsed[i].composition.Write("Used");
                }
            }
            writer.WriteEndElement();
            GlobalVars.Instance.writeStartTab("FeedAndProductsProduced");
            for (int i = 0; i < maxNumberFeedItems; i++)
            {
                if (allFeedAndProductsProduced[i].composition.Getamount() > 0)
                {
                    allFeedAndProductsProduced[i].composition.Write("Produced");
                }
            }
            writer.WriteEndElement();
            GlobalVars.Instance.writeStartTab("FeedAndProductTradeBalance");
            for (int i = 0; i < maxNumberFeedItems; i++)
            {
                if ((allFeedAndProductsUsed[i].composition.Getamount() > 0) || (allFeedAndProductsProduced[i].composition.Getamount() > 0))
                {
                    allFeedAndProductTradeBalance[i].composition.Write("Balance");
                }
            }
            writer.WriteEndElement();
            GlobalVars.Instance.writeStartTab("FeedAndProductsPotential");
            for (int i = 0; i < maxNumberFeedItems; i++)
            {
                if (allFeedAndProductsPotential[i].composition.Getamount() > 0)
                    allFeedAndProductsPotential[i].composition.Write("Potential");
            }
            writer.WriteEndElement();
            GlobalVars.Instance.writeStartTab("FeedAndProductsField");
            for (int i = 0; i < maxNumberFeedItems; i++)
            {
                if (allFeedAndProductFieldProduction[i].composition.Getamount() > 0)
                    allFeedAndProductFieldProduction[i].composition.Write("Field");
            }
            writer.WriteEndElement();

            GlobalVars.Instance.writeStartTab("allUnutilisedGrazableFeed");
            for (int i = 0; i < maxNumberFeedItems; i++)
            {
                if (allUnutilisedGrazableFeed[i].composition.Getamount() > 0)
                    allUnutilisedGrazableFeed[i].composition.Write("UnutilisedGrazableFeed");
            }
            writer.WriteEndElement();
        }
    }
    public void log(string informatio, int level)
    {
        if (level <= verbocity)
        {
            if (logScreen)
                Console.WriteLine(informatio);
            if (logFile)
                logFileStream.WriteLine(informatio);
        }

    }
    public void writeSeyda(string information, string units, double amount)
    {
        
        writeSeyda(information, units,Convert.ToString(amount));

    }
    public void writeSeyda(string information, string units, string amount)
    {
        Seyda.WriteLine(information + '\t' + units + '\t' + amount);

    }
    public void openSeyda(string outputDir, string scenarioNr)
    {
        Seyda = new System.IO.StreamWriter(outputDir + "Seyda" + scenarioNr + ".xls");
     }
    public void closeSeyda()
    {
        Seyda.Close();
    }


}