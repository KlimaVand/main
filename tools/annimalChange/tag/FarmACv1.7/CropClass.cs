using System;
using System.Collections.Generic;
using System.Xml;

//simulates the crop processes
public class CropClass
{
    public  List<manure> theManureApplied;
    public string Parens;
    public struct fertRecord
    {
        public string Name;
        public string Parens;
        public double Namount;
        public int Month_applied;
        public int dayOfApplication;
        public timeClass applicdate;
        public string Applic_techniqueS;
        public int Applic_techniqueI;
        public int ManureType;
        public int speciesGroup;
        public string Units;
        public bool ReadFertManApplication(FileInformation cropInformation, timeClass startTime, timeClass endTime)
        {
            bool aVal = false;
            Name = cropInformation.getItemString("Name");
            Units = cropInformation.getItemString("Unit");
            Namount = cropInformation.getItemDouble("Value");
            int applicationYear = startTime.GetYear();
            int month_applied = cropInformation.getItemInt("Month_applied");
            //only have month of application, so need to set a sensible day in month
            if (month_applied < startTime.GetMonth())
                applicationYear++;
            if (month_applied == startTime.GetMonth())
                SetapplicDate(startTime.GetDay(), month_applied, applicationYear); //earliest possible day
            else if (month_applied == endTime.GetMonth())
            {
                int applicationDay = Math.Max(15, endTime.GetDay());
                SetapplicDate(applicationDay, month_applied, applicationYear); //last possible day
            }
            else
                SetapplicDate(15, month_applied, applicationYear);//some day in the middle of the month
            if ((GetDate().getLongTime() < startTime.getLongTime()) || (GetDate().getLongTime() > endTime.getLongTime()))
                aVal = false;
            else
                aVal = true;
            return aVal;
        }
        public void Write(string theParens)
        {
            Parens = theParens;
            GlobalVars.Instance.writeStartTab("fertRecord");

            GlobalVars.Instance.writeInformationToFiles("Namount", "Total N applied", "kg/ha", Namount,Parens);
            GlobalVars.Instance.writeInformationToFiles("Applic_techniqueS", "Application technique", "-", Applic_techniqueS, Parens);
            GlobalVars.Instance.writeInformationToFiles("Applic_techniqueI", "Application technique ID", "-", Applic_techniqueI, Parens);
            GlobalVars.Instance.writeInformationToFiles("ManureType", "Manure category", "-", ManureType, Parens);
            GlobalVars.Instance.writeInformationToFiles("speciesGroup", "Species category", "-", speciesGroup, Parens);
            GlobalVars.Instance.writeInformationToFiles("Month_applied", "Month applied", "month", Month_applied, Parens);
            GlobalVars.Instance.writeInformationToFiles("dayOfApplication", "Day applied", "Day in month", dayOfApplication, Parens);

            GlobalVars.Instance.writeEndTab();

        }
        // Copy constructor. 
    public fertRecord(fertRecord theCropToBeCopied)
    {
        Name = theCropToBeCopied.Name;
        Parens = theCropToBeCopied.Parens;
        Namount = theCropToBeCopied.Namount;
        Month_applied = theCropToBeCopied.Month_applied;
        dayOfApplication = theCropToBeCopied.dayOfApplication;
        if (theCropToBeCopied.applicdate != null)
            applicdate = new timeClass(theCropToBeCopied.applicdate);
        else
            applicdate = null;
        Applic_techniqueS = theCropToBeCopied.Applic_techniqueS;
        Applic_techniqueI = theCropToBeCopied.Applic_techniqueI;
        ManureType = theCropToBeCopied.ManureType;
        speciesGroup = theCropToBeCopied.speciesGroup;
        Units = theCropToBeCopied.Units;
    }
        public void setParens(string aParen) { Parens = aParen; }
        public double getNamount() { return Namount; }
        public string getName() { return Name; }
        public int getspeciesGroup() { return speciesGroup; }
        public int getManureType() { return ManureType; }
        public int GetMonth_applied() { return Month_applied; }
        public void SetdayOfApplication(int aDay) { dayOfApplication = aDay; }
        public int GetdayOfApplication() { return dayOfApplication; }
        public void SetNamount(double aVal) { Namount = aVal; }
        public void SetapplicDate(int day, int month, int year) 
        {
            applicdate.setDate(day, month, year);
        }
        public timeClass GetDate() { return applicdate; }
        public long GetRelativeDay(long startDay)//timeClass startDate) 
        {
            long retVal = applicdate.getLongTime() - startDay;// startDate.getLongTime();
      
            return retVal;
        }
    }
    // Copy constructor. 
    public CropClass(CropClass theCropToBeCopied)
    {
        NCrop = theCropToBeCopied.NCrop;
        name = theCropToBeCopied.name;
        identity = theCropToBeCopied.identity;
        area = theCropToBeCopied.area;
        theStartDate = new timeClass(theCropToBeCopied.theStartDate);
       
        theEndDate = new timeClass(theCropToBeCopied.theEndDate);
        if (theCropToBeCopied.residueToNext!=null)
            residueToNext = new GlobalVars.product(theCropToBeCopied.residueToNext);
        isIrrigated = theCropToBeCopied.isIrrigated;
        fertiliserApplied=new List<fertRecord>();
        for (int i = 0; i < theCropToBeCopied.fertiliserApplied.Count; i++)
            fertiliserApplied.Add(new fertRecord( theCropToBeCopied.fertiliserApplied[i]));
        manureApplied=new List<fertRecord>();
        for (int i = 0; i < theCropToBeCopied.manureApplied.Count; i++)
            manureApplied.Add(new fertRecord(theCropToBeCopied.manureApplied[i]));
        propAboveGroundResidues[0] = theCropToBeCopied.propAboveGroundResidues[0];
        propAboveGroundResidues[1] = theCropToBeCopied.propAboveGroundResidues[1];
        propBelowGroundResidues = theCropToBeCopied.propBelowGroundResidues;
        CconcBelowGroundResidues = theCropToBeCopied.CconcBelowGroundResidues;
        CtoNBelowGroundResidues = theCropToBeCopied.CtoNBelowGroundResidues;
        NDepositionRate = theCropToBeCopied.NDepositionRate;
        urineNH3EmissionFactor = theCropToBeCopied.urineNH3EmissionFactor;
        manureN2OEmissionFactor = theCropToBeCopied.manureN2OEmissionFactor;
        fertiliserN2OEmissionFactor = theCropToBeCopied.fertiliserN2OEmissionFactor;
        soilN2OEmissionFactor = theCropToBeCopied.soilN2OEmissionFactor;
        soilN2Factor = theCropToBeCopied.soilN2Factor;
        cropResidueN2OEmissionFactor = theCropToBeCopied.cropResidueN2OEmissionFactor;
        harvestMethod = theCropToBeCopied.harvestMethod;
        MaximumRootingDepth = theCropToBeCopied.MaximumRootingDepth;
        NfixationFactor = theCropToBeCopied.NfixationFactor;
        duration = theCropToBeCopied.duration;
        permanent = theCropToBeCopied.permanent;

        theProducts = new List<GlobalVars.product>();
        for (int i = 0; i < theCropToBeCopied.theProducts.Count; i++)
        {
            GlobalVars.product aProduct=new  GlobalVars.product(theCropToBeCopied.theProducts[i]);
            theProducts.Add(aProduct);
        }
        CFixed = theCropToBeCopied.CFixed;
        Nfixed = theCropToBeCopied.Nfixed;
        nAtm = theCropToBeCopied.nAtm;
        manureNH3emission = theCropToBeCopied.manureNH3emission;
        fertiliserNH3emission = theCropToBeCopied.fertiliserNH3emission;
        urineNH3emission = theCropToBeCopied.urineNH3emission;
        surfaceResidueC = theCropToBeCopied.surfaceResidueC;
        subsurfaceResidueC = theCropToBeCopied.subsurfaceResidueC;
        surfaceResidueN = theCropToBeCopied.surfaceResidueN;
        subsurfaceResidueN = theCropToBeCopied.subsurfaceResidueN;
        manureFOMCsurface = new double[theCropToBeCopied.manureFOMCsurface.Length];
        for (int i = 0; i < theCropToBeCopied.manureFOMCsurface.Length; i++)
        {
            manureFOMCsurface[i] = theCropToBeCopied.manureFOMCsurface[i];
        }
        manureHUMCsurface = new double[theCropToBeCopied.manureHUMCsurface.Length];
        for (int i = 0; i < theCropToBeCopied.manureHUMCsurface.Length; i++)
        {
            manureHUMCsurface[i] = theCropToBeCopied.manureHUMCsurface[i];
        }
        manureFOMCsubsurface = new double[theCropToBeCopied.manureFOMCsubsurface.Length];
        for (int i = 0; i < theCropToBeCopied.manureFOMCsubsurface.Length; i++)
        {
            manureFOMCsubsurface[i] = theCropToBeCopied.manureFOMCsubsurface[i];
        }
        manureHUMCsubsurface = new double[theCropToBeCopied.manureHUMCsubsurface.Length];
        for (int i = 0; i < theCropToBeCopied.manureHUMCsubsurface.Length; i++)
        {
            manureHUMCsubsurface[i] = theCropToBeCopied.manureHUMCsubsurface[i];
        }
        manureFOMNsurface = new double[theCropToBeCopied.manureFOMNsurface.Length];
        for (int i = 0; i < theCropToBeCopied.manureFOMNsurface.Length; i++)
        {
            manureFOMNsurface[i] = theCropToBeCopied.manureFOMNsurface[i];
        }
        manureHUMNsurface = new double[theCropToBeCopied.manureHUMNsurface.Length];
        for (int i = 0; i < theCropToBeCopied.manureHUMNsurface.Length; i++)
        {
            manureHUMNsurface[i] = theCropToBeCopied.manureHUMNsurface[i];
        }
        manureTAN = new double[theCropToBeCopied.manureTAN.Length];
        for (int i = 0; i < theCropToBeCopied.manureTAN.Length; i++)
        {
            manureTAN[i] = theCropToBeCopied.manureTAN[i];
        }
        fertiliserN = new double[theCropToBeCopied.fertiliserN.Length];
        for (int i = 0; i < theCropToBeCopied.fertiliserN.Length; i++)
            fertiliserN[i] = theCropToBeCopied.fertiliserN[i];
        nitrificationInhibitor = new double[theCropToBeCopied.nitrificationInhibitor.Length];
        for (int i = 0; i < theCropToBeCopied.nitrificationInhibitor.Length; i++)
            nitrificationInhibitor[i] = theCropToBeCopied.nitrificationInhibitor[i];
        droughtFactorPlant = new double[theCropToBeCopied.droughtFactorPlant.Length];
        for (int i = 0; i < theCropToBeCopied.droughtFactorPlant.Length; i++)
            droughtFactorPlant[i] = theCropToBeCopied.droughtFactorPlant[i];
        droughtFactorSoil = new double[theCropToBeCopied.droughtFactorSoil.Length];
        for (int i = 0; i < theCropToBeCopied.droughtFactorSoil.Length; i++)
            droughtFactorSoil[i] = theCropToBeCopied.droughtFactorSoil[i];
        drainage = new double[theCropToBeCopied.drainage.Length];
        for (int i = 0; i < theCropToBeCopied.drainage.Length; i++)
            drainage[i] = theCropToBeCopied.drainage[i];
        transpire = new double[theCropToBeCopied.transpire.Length];
        for (int i = 0; i < theCropToBeCopied.transpire.Length; i++)
            transpire[i] = theCropToBeCopied.transpire[i];
        evaporation = new double[theCropToBeCopied.evaporation.Length];
        for (int i = 0; i < theCropToBeCopied.evaporation.Length; i++)
            evaporation[i] = theCropToBeCopied.evaporation[i];
        irrigationWater = new double[theCropToBeCopied.irrigationWater.Length];
        for (int i = 0; i < theCropToBeCopied.irrigationWater.Length; i++)
            irrigationWater[i] = theCropToBeCopied.irrigationWater[i];
        soilWater = new double[theCropToBeCopied.soilWater.Length];
        for (int i = 0; i < theCropToBeCopied.soilWater.Length; i++)
            soilWater[i] = theCropToBeCopied.soilWater[i];

  
        fertiliserC = theCropToBeCopied.fertiliserC;
        urineC = theCropToBeCopied.urineC;
        faecalC = theCropToBeCopied.faecalC;
        harvestedC = theCropToBeCopied.harvestedC;
        harvestedDM = theCropToBeCopied.harvestedDM;
        storageProcessingCLoss = theCropToBeCopied.storageProcessingCLoss;
        storageProcessingNLoss = theCropToBeCopied.storageProcessingNLoss;
        mineralNavailable = theCropToBeCopied.mineralNavailable;
        residueN = theCropToBeCopied.residueN;
        excretaNInput = theCropToBeCopied.excretaNInput;
        excretaCInput = theCropToBeCopied.excretaCInput;
        fertiliserNinput = theCropToBeCopied.fertiliserNinput;
        harvestedN = theCropToBeCopied.harvestedN;
        totalManureNApplied = theCropToBeCopied.totalManureNApplied;
        N2ONemission = theCropToBeCopied.N2ONemission;
        N2Nemission = theCropToBeCopied.N2Nemission;
        cropNuptake = theCropToBeCopied.cropNuptake;
        manureFOMn = theCropToBeCopied.manureFOMn;
        manureHUMn = theCropToBeCopied.manureHUMn;
        urineNasFertilizer = theCropToBeCopied.urineNasFertilizer;
        faecalN = theCropToBeCopied.faecalN;
        OrganicNLeached = theCropToBeCopied.OrganicNLeached;
        soilNMineralisation = theCropToBeCopied.soilNMineralisation;
        mineralNFromLastCrop = theCropToBeCopied.mineralNFromLastCrop;
        mineralNToNextCrop = theCropToBeCopied.mineralNToNextCrop;
        fertiliserN2OEmission = theCropToBeCopied.fertiliserN2OEmission;
        manureN2OEmission = theCropToBeCopied.manureN2OEmission;
        cropResidueN2O = theCropToBeCopied.cropResidueN2O ;
        soilN2OEmission = theCropToBeCopied.soilN2OEmission;
        proportionLeached = theCropToBeCopied.proportionLeached;
        mineralNreserve = theCropToBeCopied.mineralNreserve;
        nitrateLeaching = theCropToBeCopied.nitrateLeaching;
        cropSequenceName = theCropToBeCopied.cropSequenceName;
        droughtSusceptability = theCropToBeCopied.droughtSusceptability;
        totalTsum = theCropToBeCopied.totalTsum;
        maxLAI= theCropToBeCopied.maxLAI;
    }
    public double NCrop;
    string name;
    int identity;
    //inputs
    double area;
    public timeClass theStartDate;
    timeClass theEndDate;
    bool isIrrigated;

    string cropSequenceName;
    int cropSequenceNo;
    public List<fertRecord> fertiliserApplied;
    public List<fertRecord> manureApplied;
    
    //parameters
    double [] propAboveGroundResidues = new double[2];
    double propBelowGroundResidues;
    double CconcBelowGroundResidues;
    double CtoNBelowGroundResidues;
    double NDepositionRate;
    double urineNH3EmissionFactor;
    double manureN2OEmissionFactor;
    double fertiliserN2OEmissionFactor;
    double soilN2OEmissionFactor;
    double soilN2Factor;
    double cropResidueN2OEmissionFactor;
    string harvestMethod;
    double MaximumRootingDepth;
    double NfixationFactor;
    double baseTemperature;
    long duration = 0;
    bool permanent = false;
    double droughtSusceptability = 1.0;
    bool zeroGasEmissionsDebugging = false;
    bool zeroLeachingDebugging = false;
    //bool continues = false;
    double[] potentialEvapoTrans = new double[366];
    double[] precipitation = new double[366];
    double[] temperature = new double[366];
    double[] Tsum = new double[366];
    double[] potEvapoTrans;
    double[] evaporation;
    double[] drainage;
    double[] transpire;
    double[] soilWater;
    double[] irrigationWater;
    double[] droughtFactorPlant;
    double[] droughtFactorSoil;

    List<GlobalVars.product> theProducts = new List<GlobalVars.product>();

    //other variables
    //variables to output
    double CFixed = 0;//carbon fixed
    double surfaceResidueC;
    double subsurfaceResidueC;
    double urineC = 0;
    double faecalC = 0;
    double grazedC = 0;
    double harvestedC = 0;
    double harvestedDM = 0;
    double storageProcessingCLoss = 0;
    double storageProcessingNLoss = 0;
    double storageProcessingDMLoss = 0;

    double NyieldMax = 0;
    double maxCropNuptake = 0;
    double modelledCropNuptake = 0;
    double NavailFact = 0;
    double Nfixed = 0;
    double nAtm = 0;
    double manureNH3emission = 0;
    double fertiliserNH3emission = 0;
    double urineNH3emission = 0;
    double surfaceResidueN;
    double subsurfaceResidueN;
    double surfaceResidueDM = 0;
    double fertiliserC = 0;
    double mineralNavailable = 0;
    double residueN = 0;
    double excretaNInput = 0;
    double excretaCInput = 0;
    double fertiliserNinput = 0;
    double harvestedN = 0;
    double totalManureNApplied = 0;
    double N2ONemission = 0;
    double N2Nemission = 0;
    double cropNuptake = 0;
    double manureFOMn = 0;
    double manureHUMn = 0;
    double urineNasFertilizer = 0;
    double faecalN = 0;
    double burntResidueC = 0;
    double burntResidueN = 0;
    double burningN2ON = 0;
    double burningNH3N = 0;
    double burningNOxN = 0;
    double burningCOC = 0;
    double burningCO2C = 0;
    double burningBlackC = 0;
    double burningOtherN = 0;
    double OrganicNLeached = 0;
    double soilNMineralisation = 0;
    double mineralNFromLastCrop = 0;
    double mineralNToNextCrop = 0;
    double fertiliserN2OEmission = 0;
    double manureN2OEmission;
    double cropResidueN2O = 0;
    double soilN2OEmission = 0;
    double proportionLeached = 0;
    double mineralNreserve = 0;
    double nitrateLeaching = 0;
    double lengthOfSequence = 0;
    double previousPhaseCropNuptake = 0;
    double unutilisedGrazableDM = 0;
    double unutilisedGrazableC = 0;
    double unutilisedGrazableN = 0;
    double LAI = 0;
    double totalTsum = 0;
    double maxLAI = 5.0;

    //other variables not for output
    public double [] manureFOMCsurface;
    double [] manureHUMCsurface;
    double [] manureFOMCsubsurface;
    double [] manureHUMCsubsurface;
    double[] manureFOMNsurface;
    double[] manureHUMNsurface;
    double[] manureTAN;
    double[] fertiliserN;
    //! nitrification inhibitor - 1 at time of application
    double[] nitrificationInhibitor;

    GlobalVars.product residueFromPrevious;
    GlobalVars.product residueToNext;

    double cropdroughtIndex;
    public double GetcropdroughtIndex() { return cropdroughtIndex; }
    public void SetcropdroughtIndex(double val) { cropdroughtIndex = val; }
    public double Gettranspire(int index) { return transpire[index]; }
    public double GetIrrigationWater(int index) { return irrigationWater[index]; }
    public void SetsoilWater(int index, double val) { soilWater[index] = val; }
    public void SetdroughtFactorPlant(int index, double val) { droughtFactorPlant[index] = val; }
    public double GetdroughtFactorPlant(int index) { return droughtFactorPlant[index]; }
    public double GetdroughtFactorSoil(int index) { return droughtFactorSoil[index]; }
    public void SetdroughtFactorSoil(int index, double val) { droughtFactorSoil[index] = val; }

    public void Setname(string aname) { name = aname; }
    public void Setidentity(int aValue) { identity = aValue; }
    public void setArea(double aVal) { area = aVal; }
    public void setsoilN2Factor(double aVal) { soilN2Factor = aVal; }
    public void Setduration(long aVal) { duration = aVal; }
    public void SetcropSequenceName(string aVal) { cropSequenceName = aVal; }
    public void SetcropSequenceNo(int aVal) { cropSequenceNo = aVal; }
    public void SetpreviousPhaseCropNuptake(double aVal) { previousPhaseCropNuptake = aVal; }
    public void SetnitrificationInhibitor(double aVal) { nitrificationInhibitor[0] = aVal; }
    public double getMineralNFromLastCrop(){return mineralNFromLastCrop;}
    public double getCropResidueN2O() { return cropResidueN2O; }
    public double getNFix() { return Nfixed; }
    public double getnAtm() { return nAtm; }
    public string Getname() { return name; }
    public double getSurfaceResidueDM(){return surfaceResidueDM;}
    public int Getidentity() { return identity; }
    public double getArea() { return area; }
    public void SetlengthOfSequence(double aVal) { lengthOfSequence = aVal; }
    public double GetpropBelowGroundResidues() { return propBelowGroundResidues; }
    public double GetCconcBelowGroundResidues() { return CconcBelowGroundResidues; }
    public double GetstorageProcessingCLoss() { return storageProcessingCLoss; }
    public double GetstorageProcessingNLoss() { return storageProcessingNLoss; }
    public double GetstorageProcessingDMLoss() { return storageProcessingDMLoss; }
    public double GetharvestedC() { return harvestedC; }
    public double GetharvestedDM() { return harvestedDM; }
    public double GetgrazedC() { return grazedC; }
    public double GetsurfaceResidueC() { return surfaceResidueC; }
    public double GetsubsurfaceResidueC() { return subsurfaceResidueC; }
    public double GetsurfaceResidueN() { return surfaceResidueN; }
    public double GetsubsurfaceResidueN() { return subsurfaceResidueN; }
    public double GeturineNH3emission() { return urineNH3emission; }
    public double GetFertiliserC() { return fertiliserC; }
    public double GetnitrificationInhibitor()  { return nitrificationInhibitor[duration-1]; }
    public double GetUnutilisedGrazableC() { return unutilisedGrazableC; }
    public double GetUnutilisedGrazableDM() { return unutilisedGrazableDM; }
    public double GetpropAboveGroundResidues(int index) { return propAboveGroundResidues[index]; }
    public double GetMaxpropAboveGroundResidues() 
    {
        double retVal = propAboveGroundResidues[0];
        if (propAboveGroundResidues[1] > retVal)
            retVal = propAboveGroundResidues[1];

        return retVal; 
    }

    public double GetUtilisedDM()
    {
        double retVal = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].composition.GetisGrazed())
                retVal+=theProducts[i].GetGrazed_yield();
            else if (theProducts[i].Harvested=="Harvested")
                retVal += theProducts[i].GetModelled_yield();
        }
        return retVal;
    }

    public double GetManureC() 
    {
        double retVal = 0;
        int numDays = manureHUMCsurface.Length;
        for (int i = 0; i < numDays; i++)
            retVal += manureHUMCsurface[i] + manureFOMCsurface[i] + manureHUMCsubsurface[i] + manureFOMCsubsurface[i];
        return retVal;
    }
    public double GetUrineN() { return urineNasFertilizer; }
    public double GetUrineC() { return urineC; }
    public double GetfaecalC() { return faecalC; }
    public double GetfaecalN() { return faecalN; }
    public double GetresidueN() { return residueN; }
    public double GettotalManureNApplied() { return totalManureNApplied; }
    public double GetexcretaNInput() { return excretaNInput; }
    public double GetexcretaCInput() { return excretaCInput; }
    public double GetmanureNH3Nemission() { return manureNH3emission; }
    public double GetN2ONemission() { return N2ONemission; }
    public double GetN2Nemission() { return N2Nemission; }
    public double GetfertiliserNH3Nemission() { return fertiliserNH3emission; }
    public double GetmineralNavailable() { return mineralNavailable; }
    public double GetfertiliserN2ONEmission() { return fertiliserN2OEmission; }
    public double GetmanureN2ONEmission() { return manureN2OEmission; }
    public double GetcropResidueN2ON() { return cropResidueN2O; }
    public double GetsoilN2ONEmission() { return soilN2OEmission; }

    public double GetburningN2ON() { return burningN2ON; }
    public double GetburningNH3N() { return burningNH3N; }
    public double GetburningNOxN() { return burningNOxN; }
    public double GetburningOtherN() { return burningOtherN; }
    public double GetburningCOC() { return burningCOC; }
    public double GetburningCO2C() { return burningCO2C; }
    public double GetburningBlackC() { return burningBlackC; }
    public double GetburntResidueC() { return burntResidueC; }
    public double GetburntResidueN() { return burntResidueN; }
    public double GetharvestedN() { return harvestedN; }
    public double GetmineralNToNextCrop() { return mineralNToNextCrop; }
    //public bool GetContinues() { return continues; }
    public double GetCropNuptake() { return cropNuptake; }

    public List<GlobalVars.product> GettheProducts() { return theProducts; }
    public List<fertRecord> GetfertiliserApplied() { return fertiliserApplied; }
    public List<fertRecord> GetmanureApplied() { return manureApplied; }
    public int GetStartDay() { return theStartDate.GetDay(); }
    public int GetEndDay() { return theEndDate.GetDay(); }
    public int GetStartMonth() { return theStartDate.GetMonth(); }
    public int GetEndMonth() { return theEndDate.GetMonth(); }
    public int GetEndYear() { return theEndDate.GetYear(); }
    public int GetStartYear() { return theStartDate.GetYear(); }
    public void SetStartYear(int aVal) { theStartDate.SetYear(aVal); }
    public void SetEndYear(int aVal) { theEndDate.SetYear(aVal); }
    public bool Getpermanent() {return permanent;}

    public double GetMaximumRootingDepth() { return MaximumRootingDepth; }
    public double GetmanureFOMCsurface(int day) { return manureFOMCsurface[day]; }
    public double GetmanureFOMNsurface(int day) { return manureFOMNsurface[day]; }
    public double GetmanureHUMCsurface(int day) { return manureHUMCsurface[day]; }
    public long getDuration() { return duration; }
    public double GetOrganicNLeached() {return OrganicNLeached; }
    public double GetSoilNMineralisation() { return soilNMineralisation; }
    
    public double GetproportionLeached() { return proportionLeached; }
    public double GetMineralNreserve() { return mineralNreserve; }
    
    public void SetnitrateLeaching(double aVal){nitrateLeaching=aVal;}
    public double GetnitrateLeaching() { return nitrateLeaching; } 
    public double GetTotalmanureHUMNsurface()
    {
        double retVal = 0;
        for (int i = 0; i < manureHUMNsurface.Length; i++)
            retVal += manureHUMNsurface[i];
        return retVal;
    }

    public GlobalVars.product GetresidueToNext() { return residueToNext; }

    public double GetTotalmanureFOMNsurface()
    {
        double retVal = 0;
        for (int i = 0; i < manureFOMNsurface.Length; i++)
            retVal += manureFOMNsurface[i];
        return retVal;
    }
    public double GetFertiliserNapplied()
    {
        double retVal = 0;
        for (int i = 0; i < fertiliserApplied.Count; i++)
            retVal += fertiliserApplied[i].getNamount();
        return retVal;
    }
    public double GetManureNapplied()
    {
        double retVal = 0;
        for (int i = 0; i < manureApplied.Count; i++)
            retVal += manureApplied[i].getNamount();
        return retVal;
    }
    public void SetpotEvapoTrans(int index, double val)
    {
        potEvapoTrans[index] = val;
    }
    public void Setevaporation(int index, double val)
    {
        evaporation[index] = val;
    }
    public void Setdrainage(int index, double val)
    {
        drainage[index] = val;
    }

    public void Settranspire(int index, double val)
    {
        transpire[index] = val;
    }
    public void SetOrganicNLeached(double aVal) { OrganicNLeached = aVal; }
    public void SetsoilNMineralisation(double aVal) { soilNMineralisation = aVal; }
    public void setsoilN2OEmissionFactor(double aVal) { soilN2OEmissionFactor = aVal; }

    private CropClass(){}

    public long getStartLongTime() { return theStartDate.getLongTime(); }
    public long getEndLongTime(){return theEndDate.getLongTime();}

    public double getCFixed(){return CFixed;}
    public double getNCrop(){return NCrop;}

    public CropClass(int lastCropMonth, int lastCropYear, int nextCropMonth, int nextCropYear)
    {
        fertiliserApplied = new List<fertRecord>();
        manureApplied = new List<fertRecord>();
        theManureApplied = new List<manure>(); ;
        theStartDate = new timeClass();
        theStartDate.setDate(15, lastCropMonth, lastCropYear);
        theEndDate = new timeClass();
        theEndDate.setDate(15, nextCropMonth, nextCropYear);
        duration = theEndDate.getLongTime() - theStartDate.getLongTime() + 1;
        manureFOMCsurface = new double[duration];
        manureHUMCsurface = new double[duration];
        manureFOMCsubsurface = new double[duration];
        manureHUMCsubsurface = new double[duration];
        manureFOMNsurface = new double[duration];
        manureHUMNsurface = new double[duration];
        manureTAN = new double[duration];
        drainage = new double[duration];
        evaporation = new double[duration];
        potEvapoTrans = new double[duration];
        transpire = new double[duration];
        droughtFactorPlant = new double[duration];
        droughtFactorSoil = new double[duration];
        fertiliserN = new double[duration];
        nitrificationInhibitor = new double[duration];
        soilWater = new double[duration];
        irrigationWater = new double[duration];
    }

    public CropClass(string path, int index, int zoneNr, string theCropSeqName)
    {
        fertiliserApplied = new List<fertRecord>();
        manureApplied = new List<fertRecord>();
        theStartDate = new timeClass();
        theEndDate = new timeClass();

        cropSequenceName = theCropSeqName;
        cropSequenceNo = index;
        FileInformation cropInformation = new FileInformation(GlobalVars.Instance.getFarmFilePath());
        cropInformation.setPath(path+"("+index+")");
        name = cropInformation.getItemString("NameOfCrop");
        int Start_day = cropInformation.getItemInt("Start_day");
        int Start_month = cropInformation.getItemInt("Start_month");
        int Start_year = cropInformation.getItemInt("Start_year");
        if (!theStartDate.setDate(Start_day, Start_month, Start_year))
        {
            GlobalVars.Instance.Error("Incorrect date for start date for " + name);
        }
        int End_day = cropInformation.getItemInt("End_day");
        int End_month = cropInformation.getItemInt("End_month");
        int End_year = cropInformation.getItemInt("End_year");
        if (!theEndDate.setDate(End_day, End_month, End_year))
        {
            GlobalVars.Instance.Error("Incorrect date for end date for " + name);
        }
        duration = theEndDate.getLongTime() - theStartDate.getLongTime() + 1;
        if (duration <= 0)
        {
            GlobalVars.Instance.Error("negative duration");
        }
        drainage = new double[duration];
        evaporation = new double[duration];
        potEvapoTrans = new double[duration];
        transpire = new double[duration];
        soilWater = new double[duration];
        droughtFactorPlant = new double[duration];
        droughtFactorSoil = new double[duration];
        irrigationWater = new double[duration];
        fertiliserN = new double[duration];
        nitrificationInhibitor = new double[duration];
        manureFOMCsurface = new double[duration];
        manureHUMCsurface = new double[duration];
        manureFOMCsubsurface = new double[duration];
        manureHUMCsubsurface = new double[duration];
        manureFOMNsurface = new double[duration];
        manureHUMNsurface = new double[duration];
        manureTAN = new double[duration];
        string tempString = cropInformation.getItemString("Irrigation");
        if (tempString == "false")
            isIrrigated = false;
        else
            isIrrigated = true;
        if (name != "Bare soil")
            GetCropInformation(path, index, zoneNr);
        int length = name.IndexOf("Permanent");
        if (length != -1)
        {
            permanent = true;
            droughtSusceptability = 0.5;
        }
        //temporary hack for Ib
        if (GlobalVars.Instance.GetZone() == 1)
        {
            if ((name.IndexOf("Grass") != -1) || (name.IndexOf("grass") != -1) || (name.IndexOf("clover") != -1) || (name.IndexOf("Clover") != -1))
                permanent = true;
        }
        int minFertiliserApplied = 99, maxFertiliserApplied = 0;
        cropInformation.PathNames.Add("Fertilizer_applied");
        cropInformation.getSectionNumber(ref minFertiliserApplied, ref maxFertiliserApplied);

        for (int i = minFertiliserApplied; i <= maxFertiliserApplied; i++)
        {
            if (cropInformation.doesIDExist(i))
            {
                fertRecord newFertRecord = new fertRecord();
                newFertRecord.applicdate = new timeClass();
                cropInformation.Identity.Add(i);
                newFertRecord.Name = cropInformation.getItemString("Name");
                newFertRecord.Units = cropInformation.getItemString("Unit");
                newFertRecord.Namount = cropInformation.getItemDouble("Value");
                newFertRecord.Month_applied = cropInformation.getItemInt("Month_applied");
                if (newFertRecord.ReadFertManApplication(cropInformation, theStartDate, theEndDate) == false)
                {

                    string messageString = ("Error - fertiliser applied outside the period of this crop\n");
                    messageString = messageString + ("Crop sequence name = " + cropSequenceName + "\n");
                    messageString = messageString + ("Crop sequence number = " + cropSequenceNo + "\n");
                    messageString = messageString + ("Crop name = " + name);
                    GlobalVars.Instance.Error(messageString);
                }
                cropInformation.PathNames.Add("Applic_technique_Fertilizer");
                cropInformation.Identity.Add(-1);
                newFertRecord.Applic_techniqueS = cropInformation.getItemString("NameS");
                newFertRecord.Applic_techniqueI = cropInformation.getItemInt("NameI");
                fertiliserApplied.Add(newFertRecord);
                cropInformation.PathNames.RemoveAt(cropInformation.PathNames.Count - 1);
                cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
                cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
            }
        }

        int minManure_applied = 99, maxManure_applied = 0;
        cropInformation.PathNames[cropInformation.PathNames.Count - 1] = "Manure_applied";
        cropInformation.getSectionNumber(ref minManure_applied, ref maxManure_applied);

        for (int i = minManure_applied; i <= maxManure_applied; i++)
        {
            if (cropInformation.doesIDExist(i))
            {
                fertRecord newFertRecord = new fertRecord();
                newFertRecord.applicdate = new timeClass();
                cropInformation.Identity.Add(i);
                newFertRecord.Name = cropInformation.getItemString("Name");
                newFertRecord.Units = cropInformation.getItemString("Unit");
                newFertRecord.Namount = cropInformation.getItemDouble("Value");
                newFertRecord.Month_applied = cropInformation.getItemInt("Month_applied");
                //only have month of application, so need to set a sensible day in month
                if (newFertRecord.Month_applied == Start_month)
                    newFertRecord.dayOfApplication = Start_day; //earliest possible day
                else if (newFertRecord.Month_applied == End_month)
                    newFertRecord.dayOfApplication = End_day;//last possible day
                else
                    newFertRecord.dayOfApplication = 15;//some day in the middle of the month
                bool inPeriod = true;
                if (GetEndYear() > GetStartYear()) //crop period straddles 1 Jan
                {
                    if ((newFertRecord.GetMonth_applied() < GetStartMonth()) && (newFertRecord.GetMonth_applied() > GetEndMonth()))
                        inPeriod = false;
                    else

                        if ((newFertRecord.GetMonth_applied() >= GetStartMonth()) && (newFertRecord.GetMonth_applied() <= 12))
                            newFertRecord.SetapplicDate(newFertRecord.dayOfApplication, newFertRecord.Month_applied, GetStartYear());
                        else
                            newFertRecord.SetapplicDate(newFertRecord.dayOfApplication, newFertRecord.Month_applied, GetEndYear());
                }
                else //crop period is within one calendar year
                {
                    if ((newFertRecord.GetMonth_applied() < GetStartMonth()) || (newFertRecord.GetMonth_applied() > GetEndMonth()))
                        inPeriod = false;
                    else
                        newFertRecord.SetapplicDate(newFertRecord.dayOfApplication, newFertRecord.Month_applied, GetStartYear());
                }
                if (inPeriod == false)
                {

                    string messageString = ("Error - manure applied outside the period of this crop\n");
                    messageString = messageString + ("Crop sequence name = " + cropSequenceName + "\n");
                    messageString = messageString + ("Crop sequence number = " + cropSequenceNo + "\n");
                    messageString = messageString + ("Crop name = " + name);
                    GlobalVars.Instance.Error(messageString);
                }

                newFertRecord.ManureType = cropInformation.getItemInt("StorageType");
                newFertRecord.speciesGroup = cropInformation.getItemInt("SpeciesGroup");
                cropInformation.PathNames.Add("Applic_technique_Manure");
                cropInformation.Identity.Add(-1);
                newFertRecord.Applic_techniqueS = cropInformation.getItemString("NameS");
                newFertRecord.Applic_techniqueI = cropInformation.getItemInt("NameI");
                manureApplied.Add(newFertRecord);
                cropInformation.PathNames.RemoveAt(cropInformation.PathNames.Count - 1);
                cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
                cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
            }
        }
        totalTsum = GlobalVars.Instance.theZoneData.GetPeriodTemperatureSum(Start_day, Start_month, Start_year, End_day, End_month, End_year, baseTemperature);
        if (totalTsum <= 0)
        {
            string messageString = ("Error - total Tsum is zero or less\n");
            messageString = messageString + ("Crop sequence name = " + cropSequenceName + "\n");
            messageString = messageString + ("Crop sequence number = " + cropSequenceNo + "\n");
            messageString = messageString + ("Crop name = " + name);
            GlobalVars.Instance.Error(messageString);
        }
        getParameters(index, zoneNr, path);
    }
    public void UpdateParens(string aParent, int ID)
    {
        identity = ID;

        Parens = aParent;
        for (int i = 0; i < fertiliserApplied.Count; i++)
        {
            fertiliserApplied[i].setParens( Parens + "_" + i.ToString());
        }
        for (int i = 0; i < manureApplied.Count; i++)
        {
            manureApplied[i].setParens(Parens + "_" + i.ToString());
        }

    }

    public void GetBareSoilResidues(string path, int index, int zoneNr)
    {
        FileInformation cropInformation = new FileInformation(GlobalVars.Instance.getFarmFilePath());
        cropInformation.setPath(path + "(" + index + ").Product(-1)");
        cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
        int minProduct = 99, maxProduct = 0;
        cropInformation.getSectionNumber(ref minProduct, ref maxProduct);
        double isGrazedProduction = 0;
        for (int i = minProduct; i <= maxProduct; i++)
        {
            if (cropInformation.doesIDExist(i) == true)
            {
                cropInformation.Identity.Add(i);
                string cropPath = path + "(" + index + ")" + ".Product";
                GlobalVars.product anExample = new GlobalVars.product();
                feedItem aComposition = new feedItem();
                anExample.composition = aComposition;
                anExample.Harvested = cropInformation.getItemString("Harvested");
                string temp = path + "(" + index + ")" + ".Product" + "(" + i.ToString() + ").Expected_yield(-1)";
                anExample.Expected_yield = cropInformation.getItemDouble("Value", temp);
                if (anExample.composition.GetisGrazed() == true)
                {
                    anExample.Grazed_yield = anExample.Expected_yield;
                    isGrazedProduction += anExample.Expected_yield;
                }
                if (anExample.Expected_yield > 0)
                    theProducts.Add(anExample);
                else
                {
                    string messageString = ("Error - grazed yield of grazed residue crop must be greater than zero \n");
                    messageString = messageString + ("Crop sequence name = " + cropSequenceName + "\n");
                    messageString = messageString + ("Crop sequence number = " + cropSequenceNo + "\n");
                    messageString = messageString + ("Crop name = " + name);
                    GlobalVars.Instance.Error(messageString);
                }
                cropInformation.PathNames.RemoveAt(cropInformation.PathNames.Count - 1);
                cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
                cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
            }
        }
    }
    public void AddProductsWithResidue(List<GlobalVars.product> list)
    {
        for (int i = 0; i < list.Count; i++)
        {
            theProducts.Add(list[i]);
        }
    }
    public void GetCropInformation(string path, int index, int zoneNr)
    {
        FileInformation cropInformation = new FileInformation(GlobalVars.Instance.getFarmFilePath());
        cropInformation.setPath(path + "(" + index + ")");
        cropInformation.Identity.Add(-1);
        cropInformation.PathNames.Add("HarvestMethod");
        harvestMethod = cropInformation.getItemString("Value");
        cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
        cropInformation.PathNames[cropInformation.PathNames.Count - 1] = "Product";
        int minProduct = 99, maxProduct = 0;
        cropInformation.getSectionNumber(ref minProduct, ref maxProduct);
        double totProduction = 0;
        double isGrazedProduction = 0;
        for (int i = minProduct; i <= maxProduct; i++)
        {
            if(cropInformation.doesIDExist(i)==true)
            {
                cropInformation.Identity.Add(i);
                string cropPath = path + "(" + index + ")" + ".Product";
                GlobalVars.product anExample = new GlobalVars.product();
                feedItem aComposition = new feedItem(cropPath, i, false,Parens+"_"+i.ToString());
                anExample.composition = aComposition;
                anExample.Harvested = cropInformation.getItemString("Harvested");

                /*bool checkIncorp = anExample.composition.GetName().Contains("Incorporated");
                if (checkIncorp)
                    anExample.Harvested = "Incorporated";
                */
                string temp = path + "(" + index + ")" + ".Product" + "(" + i.ToString() + ").Potential_yield(-1)";
                anExample.Potential_yield = cropInformation.getItemDouble("Value", temp);
                if (anExample.Potential_yield<=0)
                {
                    string messageString = ("Error - potential production of a crop product cannot be zero or less than zero\n");
                    messageString += ("Crop source = " + path + "\n");
                    messageString += ("Crop name = " + name);
                    GlobalVars.Instance.Error(messageString);
                }
                temp = path + "(" + index + ")" + ".Product" + "(" + i.ToString() + ").Expected_yield(-1)";
                anExample.Expected_yield = cropInformation.getItemDouble("Value", temp);
                totProduction += anExample.Potential_yield;
                if (anExample.composition.GetisGrazed() == true || anExample.Harvested.Contains("Residue"))
                {
                    anExample.Grazed_yield = anExample.Expected_yield;
                    isGrazedProduction += anExample.Expected_yield;
                }
                else
                    if (!anExample.Harvested.Contains("Incorporated"))
                        anExample.composition.AdjustAmount(1+anExample.composition.GetStoreProcessFactor());
                if (anExample.Potential_yield >0)
                    theProducts.Add(anExample);
                cropInformation.PathNames.RemoveAt(cropInformation.PathNames.Count - 1);
                cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
                cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
            }
        }

        if ((totProduction== 0)&&(name!="Bare soil"))
        {

            string messageString=("Error - total potential production of a crop cannot be zero\n");
            messageString+=("Crop source = " + path+"\n");
            messageString += ("Crop name = " + name);
            GlobalVars.Instance.Error(messageString);
        }
    }

    public void getParameters(int cropIdentityNo, int zoneNR, string cropPath)
    {
        identity = cropIdentityNo;
        FileInformation cropParamFile = new FileInformation(GlobalVars.Instance.getParamFilePath());
        string basePath = "AgroecologicalZone(" + zoneNR + ")";
        string tmpPath;
        NDepositionRate = GlobalVars.Instance.theZoneData.GetNdeposition();
        tmpPath = basePath + ".UrineNH3EF(-1)";
        cropParamFile.setPath(tmpPath);
        if (zeroGasEmissionsDebugging)
            urineNH3EmissionFactor = 0;
        else
            urineNH3EmissionFactor = cropParamFile.getItemDouble("Value");

        cropParamFile.Identity.RemoveAt(cropParamFile.Identity.Count - 1);
        if (name != "Bare soil")
        {

            cropParamFile.PathNames[cropParamFile.PathNames.Count - 1] = "Crop";
            int min = 99, max = 0;
            cropParamFile.getSectionNumber(ref min, ref max);
            bool gotit = false;
            string aname ="None";
            for (int j = min; j <= max; j++)
            {
               
                if (cropParamFile.doesIDExist(j))
                {
                    tmpPath = basePath + ".Crop" + "(" + j.ToString() + ")";
                    cropParamFile.setPath(tmpPath);
                    aname = cropParamFile.getItemString("Name");
         
                    if (aname == name)
                    {
                        gotit = true;
                        break;
                    }
                    cropParamFile.Identity.RemoveAt(cropParamFile.Identity.Count - 1);
                }
            }
            if (gotit == false)
            {
              
                string message1=("Error - could not find crop in parameter file\n");
                message1 += message1 + ("Crop source = " + cropPath + "(" + cropIdentityNo.ToString() + ")\n");
                message1+=("Crop name = " + name);
                GlobalVars.Instance.Error(message1);
            }
            if (GlobalVars.Instance.GetZone() == 7)
            {
                cropParamFile.setPath(tmpPath + ".MaxLAI(-1)");
                maxLAI = cropParamFile.getItemDouble("Value");
            }
            cropParamFile.setPath(tmpPath + ".NavailMax(-1)");
            cropParamFile.setPath(tmpPath + ".NfixationFactor(-1)");
            NfixationFactor = cropParamFile.getItemDouble("Value");
            
            cropParamFile.setPath(tmpPath + ".PropBelowGroundResidues(-1)");
            propBelowGroundResidues = cropParamFile.getItemDouble("Value");
            cropParamFile.setPath(tmpPath + ".BelowGroundCconc(-1)");
            CconcBelowGroundResidues = cropParamFile.getItemDouble("Value");
            cropParamFile.setPath(tmpPath + ".BelowGroundCtoN(-1)");
            CtoNBelowGroundResidues = cropParamFile.getItemDouble("Value");
            cropParamFile.setPath(tmpPath + ".MaximumRootingDepth(-1)");
            MaximumRootingDepth = cropParamFile.getItemDouble("Value");

           // continues = cropParamFile.getItemBool("Value", tmpPath + ".Continues(-1)");

            cropParamFile.Identity.RemoveAt(cropParamFile.Identity.Count - 1);
            cropParamFile.PathNames[cropParamFile.PathNames.Count - 1] = "HarvestMethod";
            max = -999;
            min=999;
            cropParamFile.getSectionNumber(ref min, ref max);
            cropParamFile.Identity.Add(-1);
            for (int i = min; i <= max; i++)
            {
            //    if (cropParamFile.doesIDExist(i))
                {
                    cropParamFile.Identity[cropParamFile.PathNames.Count - 1] = i;
                    string harvestMethodName = cropParamFile.getItemString("Name");
                    if (harvestMethodName == harvestMethod)
                    {
                        cropParamFile.PathNames.Add("PropAboveGroundResidues");
                        cropParamFile.Identity.Add(-1);
                        if (harvestMethodName == "Grazing")
                            propAboveGroundResidues[1] = cropParamFile.getItemDouble("Value");
                        else
                            propAboveGroundResidues[0] = cropParamFile.getItemDouble("Value");
                        break;
                    }
                }
            }
        }
    }

    public void Calcwaterlimited_yield(double droughtIndex)
    {
        if (Getname() != "Bare soil")
        {
            for (int k = 0; k < theProducts.Count; k++)
            {
                if (isIrrigated)
                {
                    NyieldMax += theProducts[k].composition.GetN_conc() * theProducts[k].Potential_yield;
                    theProducts[k].SetwaterLimited_yield(theProducts[k].Potential_yield);
                }
                else
                {
                    NyieldMax += theProducts[k].composition.GetN_conc() * theProducts[k].Potential_yield * (1 - droughtSusceptability * droughtIndex);
                    theProducts[k].SetwaterLimited_yield(theProducts[k].Potential_yield * (1 - droughtSusceptability * droughtIndex));
                }
                theProducts[k].SetExpectedYield(theProducts[k].GetwaterLimited_yield());
            }
            maxCropNuptake = CalculateCropNUptake();
        }
    }


    public double CalculateCropNUptake()
    {
        double uptakeN = 0;
        double NinProduct = 0;
        double NinSurfaceResidues = 0;
        double NinSubsurfaceResidues = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].composition.GetisGrazed()) //grazed crop
            {
                NinProduct += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc();
                NinSurfaceResidues += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * propAboveGroundResidues[1];
                NinSubsurfaceResidues += theProducts[i].GetExpectedYield() * (GetCconcBelowGroundResidues() * GetpropBelowGroundResidues()) / CtoNBelowGroundResidues;
            }
            else
            {
                //ungrazed part of crop
                NinProduct += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc();
                NinSurfaceResidues += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * propAboveGroundResidues[0];
                NinSubsurfaceResidues += theProducts[i].GetExpectedYield() * (GetCconcBelowGroundResidues() * GetpropBelowGroundResidues()) / CtoNBelowGroundResidues;
            }
        }
        uptakeN += NinProduct + NinSurfaceResidues + NinSubsurfaceResidues;
        return uptakeN;
    }

    public double CalculateMaxCropNUptake()
    {
        double uptakeN = 0;
        double NinProduct = 0;
        double NinSurfaceResidues = 0;
        double NinSubsurfaceResidues = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].composition.GetisGrazed()) //grazed crop
            {
                NinProduct += theProducts[i].GetPotential_yield() * theProducts[i].composition.GetN_conc();
                NinSurfaceResidues += theProducts[i].GetPotential_yield() * theProducts[i].composition.GetN_conc() * propAboveGroundResidues[1];
                NinSubsurfaceResidues += theProducts[i].GetPotential_yield() * (GetCconcBelowGroundResidues() * GetpropBelowGroundResidues()) / CtoNBelowGroundResidues;
            }
            else
            {
                //ungrazed part of crop
                NinProduct += theProducts[i].GetPotential_yield() * theProducts[i].composition.GetN_conc();
                NinSurfaceResidues += theProducts[i].GetPotential_yield() * theProducts[i].composition.GetN_conc() * propAboveGroundResidues[0];
                NinSubsurfaceResidues += theProducts[i].GetPotential_yield() * (GetCconcBelowGroundResidues() * GetpropBelowGroundResidues()) / CtoNBelowGroundResidues;
            }
        }
        uptakeN += NinProduct + NinSurfaceResidues + NinSubsurfaceResidues;
        return uptakeN;
    }

       public void CalculateHarvestedYields()
    {
        if (residueFromPrevious == null)
        {
            harvestedC = 0;
            harvestedN = 0;
            harvestedDM = 0;
            grazedC = 0;
        }
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].composition.GetisGrazed()) //grazed crop
            {
                if (GlobalVars.Instance.GetstrictGrazing())
                {
                    double grazedProductC = theProducts[i].Grazed_yield * theProducts[i].composition.GetC_conc();
                    harvestedDM = theProducts[i].Grazed_yield;
                    harvestedC += grazedProductC;
                    grazedC += grazedProductC;
                    harvestedN += theProducts[i].Grazed_yield * theProducts[i].composition.GetN_conc();
                }
                else
                {
                    if (theProducts[i].GetExpectedYield() > theProducts[i].GetGrazed_yield() && theProducts[i].GetGrazed_yield()!=0)
                    {
                        //theProducts[i].SetExpectedYield(theProducts[i].GetGrazed_yield());
                        harvestedDM += theProducts[i].GetGrazed_yield();
                        harvestedC += theProducts[i].GetGrazed_yield() * theProducts[i].composition.GetC_conc();
                        harvestedN += theProducts[i].GetGrazed_yield() * theProducts[i].composition.GetN_conc();
                    }
                    else
                    {
                        harvestedDM += theProducts[i].GetExpectedYield();
                        harvestedC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc();
                        harvestedN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc();
                    }
                }
            }
            else
            {
                if ((theProducts[i].Harvested == "Harvested") || (theProducts[i].Harvested == "Burnt stubble"))
                {
                    harvestedDM += theProducts[i].GetExpectedYield();
                    harvestedC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc();
                    harvestedN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc();
                }
            }
        }
    }

    public void CalculateCFixed()
    {
        CFixed = 0;
        if (Getname() != "Bare soil")
        {
            for (int i = 0; i < theProducts.Count; i++)
            {
                double CFixedThisCrop = 0;
                double CaboveGroundResidues = 0;
                double CbelowGroundResidues = 0;
                double CHarvestable = 0;
                if (theProducts[i].composition.GetisGrazed())  //
                    CaboveGroundResidues = theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc() * propAboveGroundResidues[1];
                else
                    CaboveGroundResidues = theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc() * propAboveGroundResidues[0];
                CFixedThisCrop += CaboveGroundResidues;
                CbelowGroundResidues = theProducts[i].GetExpectedYield() * propBelowGroundResidues * GetCconcBelowGroundResidues();
                CFixedThisCrop += CbelowGroundResidues;
                CHarvestable = theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc();
                CFixedThisCrop += CHarvestable;
                CFixed += CFixedThisCrop;
            }
        }
    }

    public void CalculateCropResidues()
    {
        surfaceResidueC = 0;
        subsurfaceResidueC = 0;
        surfaceResidueN = 0;
        subsurfaceResidueN = 0;
        surfaceResidueDM = 0;
        unutilisedGrazableC = 0;
        unutilisedGrazableN = 0;
        unutilisedGrazableDM = 0;
        if (Getname() != "Bare soil")
        {
            for (int i = 0; i < theProducts.Count; i++)
            {
                if (theProducts[i].composition.GetisGrazed()) //grazed crop
                {
                    surfaceResidueC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc() * propAboveGroundResidues[1];
                    surfaceResidueN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * propAboveGroundResidues[1];
                    surfaceResidueDM += theProducts[i].GetExpectedYield() * propAboveGroundResidues[1];
                    if (theProducts[i].Expected_yield >= theProducts[i].Grazed_yield)//yield in excess of grazed is added to surface residues
                    {
                        unutilisedGrazableC += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield) * theProducts[i].composition.GetC_conc();
                        surfaceResidueC += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield) * theProducts[i].composition.GetC_conc();
                        unutilisedGrazableN += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield) * theProducts[i].composition.GetN_conc();
                        surfaceResidueN += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield) * theProducts[i].composition.GetN_conc();
                        unutilisedGrazableDM += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield);
                        surfaceResidueDM += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield);
                    }
                }
                else //ungrazed crop
                {
                    if ((theProducts[i].Harvested == "Harvested") || (theProducts[i].Harvested == "Burnt stubble"))
                    {
                        surfaceResidueC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc() * propAboveGroundResidues[0];
                        surfaceResidueN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * propAboveGroundResidues[0];
                        surfaceResidueDM += theProducts[i].GetExpectedYield() * propAboveGroundResidues[0];
                    }
                    else if (theProducts[i].Harvested.Contains("Residue"))
                    {
                        residueToNext = new GlobalVars.product(theProducts[i]);
                        //add surface residues to amount to carry over to bare soil
                        residueToNext.SetModelled_yield(theProducts[i].GetExpectedYield() * (1 + propAboveGroundResidues[0]));
                        residueToNext.SetExpectedYield(theProducts[i].GetExpectedYield() * (1 + propAboveGroundResidues[0]));
                    }
                    else
                    {
                        surfaceResidueC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc() * (propAboveGroundResidues[0] + 1);
                        surfaceResidueN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * (propAboveGroundResidues[0] + 1);
                        surfaceResidueDM += theProducts[i].GetExpectedYield() * (propAboveGroundResidues[0] + 1);
                    }
                }
                subsurfaceResidueC += theProducts[i].GetExpectedYield() * (GetCconcBelowGroundResidues() * GetpropBelowGroundResidues());
                subsurfaceResidueN += theProducts[i].GetExpectedYield() * GetCconcBelowGroundResidues() * GetpropBelowGroundResidues() / CtoNBelowGroundResidues;
            }
        }
    }


    public void CalculateCropResidueBurning()
    {
        double DMburnt = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            if ((theProducts[i].Harvested == "Burnt") || (theProducts[i].Harvested == "Burnt stubble"))
            {
                DMburnt = surfaceResidueDM;
                burntResidueC = surfaceResidueC;
                burntResidueN = surfaceResidueN;
                surfaceResidueN = 0.0;
                surfaceResidueC = 0.0;
            }
        }
        if (!zeroGasEmissionsDebugging)
        {
            burningBlackC = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueBlackCEmissionFactor();
            burningCOC = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueCOEmissionFactor();
            burningCO2C = burntResidueC - (burningBlackC + burningCOC);

            burningN2ON = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueN2OEmissionFactor();
            burningNH3N = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueNH3EmissionFactor();
            burningNOxN = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueNOxEmissionFactor();
        }
        burningOtherN = burntResidueN - (burningN2ON + burningNH3N + burningNOxN);
    }

    public void CalculateExcretaNInput()
    {
        excretaNInput = 0;
        urineNasFertilizer = 0;
        faecalN = 0;
        int feedCode;
        urineNH3emission = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            feedCode = theProducts[i].composition.GetFeedCode();
            if (theProducts[i].composition.GetisGrazed())
            {
                for (int j = 0; j < GlobalVars.Instance.getmaxNumberFeedItems(); j++)
                {
                    if (theProducts[i].composition.GetFeedCode() == j)
                    {
                        double grazedDM=GlobalVars.Instance.grazedArray[j].fieldDMgrazed;
                        if (grazedDM > 0)
                        {
                            double proportion = area * theProducts[i].Grazed_yield / grazedDM;
                            urineNasFertilizer += proportion * GlobalVars.Instance.grazedArray[j].urineN / area;
                            urineNH3emission += urineNH3EmissionFactor * urineNasFertilizer;
                            faecalN += proportion * GlobalVars.Instance.grazedArray[j].faecesN / area;
                        }
                    }
                }
            }
        }
        if (residueFromPrevious!=null)
        {
            feedCode = residueFromPrevious.composition.GetFeedCode();
            for (int j = 0; j < GlobalVars.Instance.getmaxNumberFeedItems(); j++)
            {
                if (residueFromPrevious.composition.GetFeedCode() == j)
                {
                    double grazedDM = GlobalVars.Instance.grazedArray[j].fieldDMgrazed;
                    if (grazedDM > 0)
                    {
                        double proportion = area * residueFromPrevious.Grazed_yield / grazedDM;
                        urineNasFertilizer += proportion * GlobalVars.Instance.grazedArray[j].urineN / area;
                        urineNH3emission += urineNH3EmissionFactor * urineNasFertilizer;
                        faecalN += proportion * GlobalVars.Instance.grazedArray[j].faecesN / area;
                    }
                }
            }
        }
        excretaNInput = urineNasFertilizer + faecalN;
    }

    public void CalculateExcretaCInput()
    {
        excretaCInput = 0;
        urineC = 0;
        faecalC = 0;
        int feedCode;
        for (int i = 0; i < theProducts.Count; i++)
        {
            feedCode = theProducts[i].composition.GetFeedCode();
            if (theProducts[i].composition.GetisGrazed())
            {
                for (int j = 0; j < GlobalVars.Instance.getmaxNumberFeedItems(); j++)
                {
                    if (theProducts[i].composition.GetFeedCode() == j)
                    {
                        if (GlobalVars.Instance.grazedArray[j].fieldDMgrazed > 0)
                        {
                            double proportion = area * theProducts[i].Grazed_yield / GlobalVars.Instance.grazedArray[j].fieldDMgrazed;
                            urineC += proportion * GlobalVars.Instance.grazedArray[j].urineC / area;
                            faecalC += proportion * GlobalVars.Instance.grazedArray[j].faecesC / area;
                        }
                    }
                }
            }
        }
        if (residueFromPrevious != null)
        {
            feedCode = residueFromPrevious.composition.GetFeedCode();
            for (int j = 0; j < GlobalVars.Instance.getmaxNumberFeedItems(); j++)
            {
                if (residueFromPrevious.composition.GetFeedCode() == j)
                {
                    double grazedDM = GlobalVars.Instance.grazedArray[j].fieldDMgrazed;
                    if (grazedDM > 0)
                    {
                        double proportion = area * residueFromPrevious.Grazed_yield / GlobalVars.Instance.grazedArray[j].fieldDMgrazed;
                        urineC += proportion * GlobalVars.Instance.grazedArray[j].urineC / area;
                        faecalC += proportion * GlobalVars.Instance.grazedArray[j].faecesC / area;
                    }
                }
            }
        }
        excretaCInput = urineC + faecalC;
    }

    static int tmp = 0;
    public void CalculateManureInput(bool lockit)
    {
        manureNH3emission = 0;
        //need to modify this to allow manure OM to be placed at different depths in the soil
        for (int i = 0; i < duration; i++)
        {
            manureFOMCsurface[i] = 0;
            manureHUMCsurface[i] = 0;
            manureFOMCsubsurface[i] = 0;
            manureHUMCsubsurface[i] = 0;
            manureFOMNsurface[i] = 0;
            manureHUMNsurface[i] = 0;
            manureTAN[i] = 0;
        }
        double NH3EmissionFactor=0;
        double totManureCapplied = 0;
        manure aManure;// = new manure() ;
        tmp++;
        for (int i = 0; i < manureApplied.Count; i++)
        {
            double amountTotalN = manureApplied[i].getNamount() * area;
            totalManureNApplied += manureApplied[i].getNamount();
            int ManureType = manureApplied[i].getManureType();
            int speciesGroup = manureApplied[i].getspeciesGroup();
            string applicType = manureApplied[i].Applic_techniqueS;

            if (lockit == false)
            {
                aManure = GlobalVars.Instance.theManureExchange.TakeManure(amountTotalN, lengthOfSequence, ManureType, speciesGroup);
                aManure.DivideManure(1 / area);
                manure anextraManure = new manure(aManure);
                if (theManureApplied == null)
                    theManureApplied = new List<manure>();
                theManureApplied.Add(anextraManure);
            }
            else
                aManure = theManureApplied[i];
            int maxManure = 0;
            maxManure = GlobalVars.Instance.theZoneData.theFertManData.Count;
            bool gotit = false;
            for (int j = 0; j < maxManure; j++)
            {
                int tmpType = GlobalVars.Instance.theZoneData.theFertManData[j].manureType;  //type of manure storage
                int tmpSpecies = GlobalVars.Instance.theZoneData.theFertManData[j].speciesGroup;
                if ((tmpType == ManureType) && ((tmpSpecies == speciesGroup) || (tmpType == 12)))
                {
                    if (GlobalVars.Instance.getcurrentInventorySystem() == 1)
                    {
                        NH3EmissionFactor = GlobalVars.Instance.theZoneData.theFertManData[j].EFNH3FieldTier2;//Tier 2 EF is proportion of total N
                        //Revised NH3 emission
                        double temp = aManure.GetTotalN() / aManure.GetTAN();
                        NH3EmissionFactor *= temp;
                        //end revised NH3 emission
                        gotit = true;
                        break;
                    }
                    else
                    {
                        double refEFNH3 = GlobalVars.Instance.theZoneData.theFertManData[j].fertManNH3EmissionFactor;
                        double HousingRefTemp = GlobalVars.Instance.theZoneData.theFertManData[j].fertManNH3EmissionFactorHousingRefTemperature;
                        double actualTemp = GlobalVars.Instance.Temperature(GlobalVars.Instance.theZoneData.GetaverageAirTemperature(),
                            0.0, manureApplied[i].GetdayOfApplication(), 0.0, GlobalVars.Instance.theZoneData.GetairtemperatureAmplitude(), GlobalVars.Instance.theZoneData.GetairtemperatureOffset());
                        double KHtheta = Math.Pow(10, -1.69 + 1447.7 / (actualTemp + GlobalVars.absoluteTemp));
                        double KHref = Math.Pow(10, -1.69 + 1447.7 / (HousingRefTemp + GlobalVars.absoluteTemp));
                        NH3EmissionFactor = (KHref / KHtheta) * refEFNH3;
                        gotit = true;
                        break;
                    }
                }
            }
            if (!gotit)
            {
                string messageString = ("Error - unable to find ammonia emission factor for a manure\n");
                messageString += " Manure name " + aManure.Getname() + " \n";
                messageString += " Crop sequence name " + cropSequenceName + " \n";
                messageString += " Crop start year " + GetStartYear().ToString();
                GlobalVars.Instance.Error(messageString);
            }
            if (zeroGasEmissionsDebugging)
                NH3EmissionFactor = 0;

            double NH3ReductionFactor = 0;
            int maxApps = GlobalVars.Instance.theZoneData.themanureAppData.Count;
            gotit = false;
            for (int j = 0; j < maxApps; j++)
            {
                string tmpName = GlobalVars.Instance.theZoneData.themanureAppData[j].name;
                if (tmpName == applicType)
                {
                    NH3ReductionFactor = GlobalVars.Instance.theZoneData.themanureAppData[j].NH3EmissionReductionFactor;
                    gotit = true;
                    break;
                }
            }
            if (!gotit)
            {
                string messageString = ("Error - unable to find ammonia emission reduction factor for a manure or fertiliser application method\n");
                messageString += " Application method name " + applicType + " \n";
                messageString += " Crop sequence name " + cropSequenceName + " \n";
                messageString += " Crop start year " + GetStartYear().ToString();
                GlobalVars.Instance.Error(messageString);
            }
            double tmpNH3emission = NH3EmissionFactor * (1 - NH3ReductionFactor) * aManure.GetTAN();
            manureNH3emission += tmpNH3emission;
/*            Console.WriteLine(i + " " + aManure.GetTAN());
            Console.WriteLine(theStartDate.GetDay());
            Console.WriteLine(theStartDate.GetMonth());
            Console.WriteLine(theStartDate.GetYear());*/
            int dayNo= (int) manureApplied[i].GetRelativeDay(getStartLongTime());
            //if (dayNo < 0)
              //  Console.WriteLine("");
//            if (manureApplied[i].GetdayOfApplication()>duration-1)
  //              Console.WriteLine("seq " + cropSequenceName + " " + name);
//            manureTAN[manureApplied[i].GetdayOfApplication()] += aManure.GetTAN();
            if (tmpNH3emission <= aManure.GetTAN())
                aManure.SetTAN(aManure.GetTAN() - tmpNH3emission);
            else //this should rarely happen and only if using Tier 2 
            {
                tmpNH3emission -= aManure.GetTAN();
                aManure.SetTAN(0.0);
                aManure.SetorganicN(aManure.GetorganicN() - tmpNH3emission);
            }
            manureTAN[dayNo] += aManure.GetTAN();
            /*manureFOMCsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GetdegC() + aManure.GetnonDegC();
            manureHUMCsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GethumicC();
            manureFOMNsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GetorganicN() - aManure.GethumicN();
            manureHUMNsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GethumicN();
            */
            manureFOMCsurface[dayNo] += aManure.GetdegC() + aManure.GetnonDegC();
            manureHUMCsurface[dayNo] += aManure.GethumicC();
            manureFOMNsurface[dayNo] += aManure.GetorganicN() - aManure.GethumicN();
            manureHUMNsurface[dayNo] += aManure.GethumicN();
            totManureCapplied += aManure.GetdegC() + aManure.GetnonDegC() + aManure.GethumicC();
            GlobalVars.Instance.log(manureFOMNsurface[dayNo].ToString(), 5);
        }
    }

    public void CalculateFertiliserInput()
    {
        double fertiliserNin = 0;
        //double fertiliserC = 0;
        fertiliserNH3emission = 0;
        fertiliserNinput = 0;
        FileInformation cropInformation = new FileInformation(GlobalVars.Instance.getfertManFilePath());
        for (int i = 0; i < fertiliserApplied.Count; i++)
        {
             
            double Napplied = fertiliserApplied[i].getNamount();
            fertiliserNinput += Napplied;
            string fertilizerName = fertiliserApplied[i].getName();
            cropInformation.setPath("AgroecologicalZone("+GlobalVars.Instance.GetZone().ToString()+").fertiliser");
            int max = 0;
            int min = 99;
            cropInformation.getSectionNumber(ref min, ref max);
            cropInformation.Identity.Add(min);
            for (int j = min; j < max; j++)
            {
                cropInformation.Identity[1] = min;
                string fertManName = cropInformation.getItemString("Name");
                if (fertManName.CompareTo(fertilizerName) == 0)
                    break;
            }
            cropInformation.PathNames.Add("Cconcentration");
            cropInformation.Identity.Add(-1);
            double Cconc = cropInformation.getItemDouble("Value");
            cropInformation.PathNames[2] = "Nconcentration";
            double Nconc = cropInformation.getItemDouble("Value");
            double amount = 0;
            if (Nconc>0)
                amount=Napplied/Nconc;
            fertiliserNin += Napplied;
            fertiliserC += amount * Cconc;             

            double NH3EmissionFactor = 0;

            int maxFert = 0;
            maxFert = GlobalVars.Instance.theZoneData.theFertManData.Count;
            bool found=false;
            for (int j = 0; j < maxFert; j++)
            {
                string tmpName = GlobalVars.Instance.theZoneData.theFertManData[j].name;             
                if (tmpName == fertilizerName)
                {
                    NH3EmissionFactor = GlobalVars.Instance.theZoneData.theFertManData[j].fertManNH3EmissionFactor;
                    found = true;
                    break;
                }
            }
            if (zeroGasEmissionsDebugging)
                NH3EmissionFactor = 0;

            if (found == false)
                GlobalVars.Instance.Error("NH3 not found in parameter file for " + fertilizerName);
            double tmpNH3emission = NH3EmissionFactor * Napplied;
            fertiliserNH3emission += tmpNH3emission;
            int applicDay = (int)(fertiliserApplied[i].GetDate().getLongTime() - theStartDate.getLongTime());
            if ((applicDay < 0) || (applicDay > duration))
            {
                string messageString = ("Error - Fertiliser applied outside crop period\n");
                messageString += " Crop sequence name " + cropSequenceName + " \n";
                messageString += " Crop number " + identity + " \n";
                messageString += " Crop start year " + GetStartYear().ToString();
                GlobalVars.Instance.Error(messageString);
            }
            //Napplied now refers to N entering soil
            fertiliserN[applicDay]+= Napplied - tmpNH3emission; //this is what enters the soil
            if (fertilizerName == "Nitrification inhibitor")
            {
                if (Napplied > 1.0)
                {
                    string messageString = ("Error - nitrification inhibitor efficiency must be <=1.0\n");
                    messageString += " Crop sequence name " + cropSequenceName + " \n";
                    messageString += " Crop number " + identity + " \n";
                    messageString += " Crop start year " + GetStartYear().ToString();
                    GlobalVars.Instance.Error(messageString);
                }
                else
                    nitrificationInhibitor[applicDay] = Napplied;
            }

        }        
    }

    public double CalculateNitrificationInhibitor()
    {
        double retVal = 0;
        for (int i = 0; i < duration; i++)
        {
            if (i > 0)
            {
                if (nitrificationInhibitor[i] < nitrificationInhibitor[i - 1])  //no new inhibitor applied
                {
                    double temp = temperature[i];
                    double degRate = Math.Log(2) / (168 * Math.Exp(-0.084 * temp));//from F.M. Kelliher a,b,*, T.J. Clough b, H. Clark c, G. Rys d, J.R. Sedcole b
                    nitrificationInhibitor[i] = nitrificationInhibitor[i - 1] * Math.Exp(-degRate);
                    if (nitrificationInhibitor[i] < 0.000001)
                        nitrificationInhibitor[i] = 0;
                }
            }
            retVal += nitrificationInhibitor[i];
        }
        retVal /= duration;
        return retVal;
    }

    public double CalculateN20(double fertN, double manureN, double residN, double soilN)
    {
        double N2OEmission = 0;
        soilN2OEmissionFactor = GlobalVars.Instance.theZoneData.getsoilN2OEmissionFactor(); ;
        manureN2OEmissionFactor = GlobalVars.Instance.theZoneData.getmanureN20EmissionFactor();
        fertiliserN2OEmissionFactor = GlobalVars.Instance.theZoneData.getfertiliserN20EmissionFactor();
        cropResidueN2OEmissionFactor = GlobalVars.Instance.theZoneData.getresidueN2OEmissionFactor();
        fertiliserN2OEmission = fertiliserN2OEmissionFactor * fertN;
        manureN2OEmission = manureN2OEmissionFactor * manureN;
        cropResidueN2O = cropResidueN2OEmissionFactor * residN;
        soilN2OEmission = soilN2OEmissionFactor * soilN;
        N2OEmission = fertiliserN2OEmission + manureN2OEmission + cropResidueN2O + soilN2OEmission;
        return N2OEmission;
    }

    public void DoCropInputs(bool lockit)
    {
        if (!lockit)
        {
            CalculateExcretaCInput();
            CalculateExcretaNInput();
            CalculateManureInput(lockit);
        }
        if (Getname() != "Bare soil")
        {
            CalculateCFixed();
            CalculateCropResidues();
            CalculateCropResidueBurning();
        }
        CalculateHarvestedYields();
    }

    public void HandleBareSoilResidues(GlobalVars.product someResidueFromPrevious)
    {
        if ((Getname() == "Bare soil") && (someResidueFromPrevious!=null))
        {
            double DMburnt = 0;
            double grazedDM = 0;
            residueFromPrevious = new GlobalVars.product(someResidueFromPrevious);
            //This is to keep 
            //residueFromPrevious.SetExpectedYield(residueFromPrevious.GetModelled_yield();
            //retrieve the DM in the residue from the previous crop
            double remainingResidueDM = residueFromPrevious.GetModelled_yield();
            if (remainingResidueDM > 0)
            {
                if (residueFromPrevious.GetGrazed_yield() > 0)
                {
                    residueFromPrevious.composition.SetisGrazed(true);
                    grazedDM = residueFromPrevious.GetGrazed_yield();
                    harvestedC = residueFromPrevious.GetGrazed_yield() * residueFromPrevious.composition.GetC_conc();
                    grazedC = harvestedC;
                    harvestedN = residueFromPrevious.GetGrazed_yield() * residueFromPrevious.composition.GetN_conc();
                    harvestedDM = grazedDM;
                    GlobalVars.product aProduct = new GlobalVars.product(residueFromPrevious);
                    //convert to yield from yield per ha
                    aProduct.composition.Setamount((aProduct.GetModelled_yield() - aProduct.GetGrazed_yield())*area);
                    GlobalVars.Instance.AddGrazableProductUnutilised(aProduct.composition);
                }
                remainingResidueDM -= grazedDM;
                unutilisedGrazableDM += remainingResidueDM;
                unutilisedGrazableC += remainingResidueDM * someResidueFromPrevious.composition.GetC_conc();
                unutilisedGrazableN += remainingResidueDM * someResidueFromPrevious.composition.GetN_conc();

                if (residueFromPrevious.Harvested.Contains("burnt")) //burn the DM remaining after grazing of residues
                {
                    DMburnt = remainingResidueDM;
                    if (DMburnt >= 0)
                    {
                        burntResidueC = DMburnt * residueFromPrevious.composition.GetC_conc();
                        burntResidueN = DMburnt * residueFromPrevious.composition.GetN_conc();
                        surfaceResidueN = 0.0;
                        surfaceResidueC = 0.0;
                    }
                }
                else if (residueFromPrevious.Harvested.Contains("harvested")) //
                {
                    harvestedC += remainingResidueDM * residueFromPrevious.composition.GetC_conc();
                    harvestedN += remainingResidueDM * residueFromPrevious.composition.GetN_conc();
                   // theProducts[1].SetExpectedYield(remainingResidueDM);//otherwise the residues will be harvested with zero mass
                    string messageString = ("Error - attempt to harvest crop residue on bare soil - function not implemented\n");
                    messageString += " Crop sequence name " + cropSequenceName + " \n";
                    messageString += " Crop number " + identity + " \n";
                    messageString += " Crop start year " + GetStartYear().ToString();
                    GlobalVars.Instance.Error(messageString);
                }
                else
                //return the residue to the soil surface
                {
                    surfaceResidueC = remainingResidueDM * residueFromPrevious.composition.GetC_conc();
                    surfaceResidueN = remainingResidueDM * residueFromPrevious.composition.GetN_conc();
                }
                /*                    else if (theProducts[2].Harvested == "Incorporated")
                                    {
                                    }*/

            }
            residueFromPrevious.composition.Setamount(grazedDM);//so this is logged as produced
            burningBlackC = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueBlackCEmissionFactor();
            burningCOC = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueCOEmissionFactor();
            burningCO2C = burntResidueC - (burningBlackC + burningCOC);

            burningN2ON = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueN2OEmissionFactor();
            burningNH3N = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueNH3EmissionFactor();
            burningNOxN = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueNOxEmissionFactor();
            burningOtherN = burntResidueN - (burningN2ON + burningNH3N + burningNOxN);
           // theProducts.Clear();
        }

    }

    public double GetmanureOrgN()
    {
        double retVal = 0;
        for (int i = 0; i < manureFOMNsurface.Length; i++)
            retVal += manureFOMNsurface[i];
        for (int i = 0; i < manureHUMNsurface.Length; i++)
            retVal += manureHUMNsurface[i];
        return retVal;
    }

    public double GetmanureTAN()
    {
        double retVal = 0;
        for (int i = 0; i < manureTAN.Length; i++)
            retVal += manureTAN[i];
        return retVal;
    }

    public double GetNfixation(double deficit)
    {
        double retVal = 0;
        if ((deficit>0)&&(NfixationFactor>=0))
            retVal = deficit * NfixationFactor;
        return retVal;
    }

    //Jonas - this function is messy. It is called CalcAvailableN but also has carbon in it too. Needs to be tidied
    public void CalcAvailableN(ref double surplusMineralN, double thesoilNMineralisation, ref double relGrowth)
    {
        mineralNFromLastCrop = surplusMineralN;
        soilNMineralisation = thesoilNMineralisation;
        double manureOrgN = GetmanureOrgN();
        double totmanureTAN = GetmanureTAN();
        double soilNSupply = mineralNFromLastCrop + soilNMineralisation;
        
        //double fertiliserN = 0;
        //CalculateFertiliserInput(ref fertiliserC, ref fertiliserN);
        nAtm = (NDepositionRate/365) * (getEndLongTime() - getStartLongTime() + 1);
        //note tha N2O emissions are currently calculated after removal of NH3 emissions and without humic N
        double evenNSupply = urineNasFertilizer + soilNMineralisation + nAtm - urineNH3emission;
        relGrowth = 0;
        CalculateLeachingAndUptake(mineralNFromLastCrop, evenNSupply, ref relGrowth, ref surplusMineralN);//calculate leaching and modelled crop uptake

        GlobalVars.Instance.log("Crop " + name + " rel growth " + relGrowth.ToString("0.00") + " Nfix " + Nfixed.ToString("0.00") + " soil min " + soilNMineralisation.ToString("0.00")
            + " fromlast " + mineralNFromLastCrop.ToString("0.00") + " surplus " +surplusMineralN.ToString("0.00") + " leaching " + nitrateLeaching.ToString("0.00")
            + " avail " + mineralNavailable.ToString("0.00"), 5);
        //GlobalVars.Instance.log("Manure NH3N " + manureNH3emission.ToString());
    }

    public void getGrazedFeedItems()
    {
        for (int i = 0; i < theProducts.Count; i++)
        {
            if ((theProducts[i].composition.GetisGrazed())||(theProducts[i].Harvested.Contains("Residue")))
            {
                int feedCode = theProducts[i].composition.GetFeedCode();
                GlobalVars.Instance.grazedArray[feedCode].fieldDMgrazed += theProducts[i].Grazed_yield * area/lengthOfSequence;
                GlobalVars.Instance.grazedArray[feedCode].name = theProducts[i].composition.GetName();
            }
        }
    }
    
     public double GetDMYield()
    {
        double DMYield = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            DMYield += theProducts[i].GetModelled_yield();
        }
        return DMYield;
    }
    public bool expect()
    {
        int numberOfMatching = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].GetModelled_yield() > 0)
            {
                double diff = theProducts[i].GetModelled_yield() - theProducts[i].GetExpectedYield();
                double relative_diff = System.Math.Abs(diff / theProducts[i].GetModelled_yield());
                double threshold = 0.1 * GlobalVars.Instance.getmaxToleratedErrorYield();
                if (relative_diff < threshold)
                    numberOfMatching++;
            }
            else
            {
                theProducts[i].SetExpectedYield(0);
                theProducts[i].SetModelled_yield(0);
                numberOfMatching++;
            }
        }
        if (numberOfMatching == theProducts.Count)
        {
            if (mineralNavailable < 0.0)
            {
                string messageString = ("Error - insufficient mineral N available to satisfy immobilisation in soil\n");
                messageString += " Crop sequence name " + cropSequenceName + " \n";
                messageString += " Crop number " + identity + " \n";
                messageString += " Crop start year " + GetStartYear().ToString();
                GlobalVars.Instance.Error(messageString);
                return false;
            }
            else
                return true;
        }
        else
            return false;
    }
    public bool CalcModelledYield(ref double surplusMineralN, ref double relGrowth, bool final)
    {
        bool retVal = false;
        if (theProducts.Count > 2)
        {
           string messageString=("Error - too many products in crop");
           GlobalVars.Instance.Error(messageString);
        }

        if (Getname() != "Bare soil")
        {
            GlobalVars.Instance.log("relGrowth " + relGrowth.ToString(), 5);

            for (int i = 0; i < theProducts.Count; i++)
            {
                if (theProducts[i].GetGrazed_yield() > theProducts[i].GetPotential_yield())
                {
                    string messageString = ("Error - grazed yield " + theProducts[i].GetGrazed_yield().ToString() +
                            " required is greater than the potential yield " + theProducts[i].GetwaterLimited_yield().ToString());
                    GlobalVars.Instance.Error(messageString);
                }

                theProducts[i].SetModelled_yield(theProducts[i].GetPotential_yield() * relGrowth);
                GlobalVars.Instance.log("expected yield " + theProducts[i].Modelled_yield.ToString(), 5);
            }
            if (expect())
            {
                for (int i = 0; i < theProducts.Count; i++)
                {
                    bool residueGrazed = false;
                    if (theProducts[i].Harvested.Contains("Residue"))
                        residueGrazed = true;
                    if (((theProducts[i].composition.GetisGrazed())|| (residueGrazed)) && (GlobalVars.Instance.GetstrictGrazing()))
                    {
                        double diff_grazed = theProducts[i].GetModelled_yield() - theProducts[i].Grazed_yield;
                        FileInformation constantFile = new FileInformation(GlobalVars.Instance.getConstantFilePath());
                        constantFile.setPath("constants(0).absoluteGrazedDMtolerance(-1)");
                        double absoluteGrazedDMtolerance = constantFile.getItemDouble("Value");
                        if (Math.Abs(diff_grazed) > absoluteGrazedDMtolerance)
                        {
                            double rel_diff_grazed = 0;
                            if (theProducts[i].Grazed_yield > 0)
                                rel_diff_grazed = diff_grazed / theProducts[i].Grazed_yield;
                            double tolerance = GlobalVars.Instance.getmaxToleratedErrorGrazing();
                            if ((rel_diff_grazed < 0.0) && (Math.Abs(rel_diff_grazed) > tolerance))
                            {
                                WritePlantFile();
                                string messageString = ("Error - modelled production lower than required production for grazed feed item \n");
                                messageString += " Modelled yield " + theProducts[i].GetModelled_yield().ToString() + " Required yield " + theProducts[i].Grazed_yield.ToString();
                                messageString += " Crop sequence name " + cropSequenceName + " \n";
                                messageString += "Crop number " + identity + " \n";
                                messageString += "Crop product = " + theProducts[i].composition.GetName() + " \n";
                                messageString += "Crop start year " + GetStartYear().ToString();
                                GlobalVars.Instance.Error(messageString);
                            }
                        }
                    }
                    theProducts[i].SetExpectedYield(theProducts[i].GetModelled_yield());
                }
                cropNuptake = CalculateCropNUptake();
                mineralNToNextCrop = surplusMineralN;
                //DoCropInputs(true);
                retVal = true;
            }
            else
            {
                for (int i = 0; i < theProducts.Count; i++)
                {
                    double newExpectedYield = 0;
                    double diff = theProducts[i].GetModelled_yield() - theProducts[i].GetExpectedYield();
                    if (diff < 0)
                        newExpectedYield = theProducts[i].GetExpectedYield() + diff / 2;
                    else
                        newExpectedYield = theProducts[i].GetExpectedYield() - diff / 2;
                    if (newExpectedYield <= 0.0)
                    {
                        newExpectedYield = 0.0001;
                        retVal = true;
                    }
                    theProducts[i].SetExpectedYield(newExpectedYield);
                }
            }
        }
        else  //bare soil
        {
            mineralNToNextCrop = surplusMineralN;
            retVal = true;
        }
    return retVal;
}

    public int CheckYields(string RotationName)
    {
        if (residueFromPrevious != null)
        {
            residueFromPrevious.composition.Setamount(residueFromPrevious.composition.Getamount() * area / lengthOfSequence);
//            double temp = residueFromPrevious.composition.Getamount() * theProducts[i].composition.GetN_conc();
            GlobalVars.Instance.AddProductProduced(residueFromPrevious.composition);
            //reset value to per ha basis (for debugging)
            residueFromPrevious.composition.Setamount(residueFromPrevious.composition.Getamount() / area);
        }
        else
        {
            for (int i = 0; i < theProducts.Count; i++)
            {
                if (theProducts[i].GetModelled_yield() == 0)
                {
                    string messageString = ("Error - modelled yield is zero\n");
                    messageString += ("Rotation name = " + RotationName + "\n");
                    messageString += ("Crop product = " + theProducts[i].composition.GetName());
                    GlobalVars.Instance.Error(messageString);
                }
                else
                {
                    double expected = theProducts[i].Expected_yield;
                    double modelled = theProducts[i].GetModelled_yield();
                    if (Double.IsNaN(modelled)) //this should never happen..
                    {
                        string messageString = "Error; modelled yield has not been calculated\n";
                        messageString += "Rotation name = " + RotationName + "\n";
                        messageString += "Crop product = " + theProducts[i].composition.GetName() + "\n";
                        messageString += "Crop start year " + GetStartYear().ToString();
                        GlobalVars.Instance.Error(messageString);
                    }

                    double diff = (modelled - expected) / modelled;
                    double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
                    if (Math.Abs(diff) > tolerance)
                    {
                        double errorPercent = 100 * diff;
                        string messageString;
                        if (diff > 0)
                            messageString = "Error; modelled yield exceeds expected yield by more than the permitted margin of "
                            + tolerance.ToString() + "\n";
                        else
                            messageString = "Error; expected yield exceeds modelled yield by more than the permitted margin"
                            + tolerance.ToString() +"\n";
                        if (errorPercent < 0)
                            errorPercent *= -1.0;
                        messageString += "Rotation name = " + RotationName + "\n";
                        messageString += "Crop product = " + theProducts[i].composition.GetName() + "\n";
                        messageString += "Crop start year " + GetStartYear().ToString() + "\n";
                        messageString += "Percentage error = " + errorPercent.ToString("0.00") + "%" + "\n";
                        messageString += "Expected yield= " + expected.ToString() + " Modelled yield= " + modelled.ToString() + "\n";
                        Write();

                        GlobalVars.Instance.Error(messageString);

                    }
                    else
                    {
                        //accept the modelled yield as valid and add to allFeedAndProductsProduced
                        double productProcessingLossFactor = theProducts[i].composition.GetStoreProcessFactor();
                        if ((theProducts[i].Harvested == "Harvested") || ((theProducts[i].composition.GetisGrazed()))
                            || (theProducts[i].Harvested == "Burnt stubble"))
                        //                        || (theProducts[i].Harvested == "Residue") || (theProducts[i].Harvested == "Stubbleburning"))
                        {
                            if (!theProducts[i].composition.GetisGrazed())
                            {
                                theProducts[i].composition.Setamount(theProducts[i].GetExpectedYield());
                                double originalC = theProducts[i].composition.Getamount() * theProducts[i].composition.GetC_conc();
                                double tempCLoss = productProcessingLossFactor * originalC;
                                storageProcessingCLoss += tempCLoss;
                                double originalN = theProducts[i].composition.Getamount() * theProducts[i].composition.GetN_conc();
                                double tempNLoss = productProcessingLossFactor * originalN;
                                storageProcessingNLoss += tempNLoss;
                                theProducts[i].composition.AdjustAmount(1 - productProcessingLossFactor);
                                theProducts[i].composition.SetC_conc((originalC - tempCLoss) / theProducts[i].composition.Getamount());
                                double temp2 = theProducts[i].composition.GetC_conc() * theProducts[i].composition.Getamount();
                            }
                            else
                                theProducts[i].composition.Setamount(theProducts[i].GetGrazed_yield());
                            //multiply by crop area to obtain yield from yield per ha
                            theProducts[i].composition.Setamount(theProducts[i].composition.Getamount() * area / lengthOfSequence);
                            double temp = theProducts[i].composition.Getamount() * theProducts[i].composition.GetN_conc();
                            GlobalVars.Instance.AddProductProduced(theProducts[i].composition);
                            if (theProducts[i].GetGrazed_yield() > 0)
                            {
                                GlobalVars.product aProduct = new GlobalVars.product(theProducts[i]);
                                aProduct.composition.Setamount(aProduct.GetModelled_yield() - aProduct.GetGrazed_yield());
                                GlobalVars.Instance.AddGrazableProductUnutilised(aProduct.composition);
                            }
                                
                            //reset yield value back to per ha basis (for debugging)
                            theProducts[i].composition.Setamount(theProducts[i].composition.Getamount() / area);
                        }
                    }
                }
            }
        }
        return 0;
    }

    public void AdjustDates(int firstYear)
    {
        //Console.WriteLine(" init start yr " + GetStartYear().ToString() + " init end " + GetEndYear().ToString());
        SetEndYear(GetEndYear() - firstYear + 1);
        SetStartYear(GetStartYear()-firstYear+1);
        //Console.WriteLine(" fin start yr " + GetStartYear().ToString() + " fin end " + GetEndYear().ToString());

        for (int i = 0; i < fertiliserApplied.Count; i++)
        {
            int monthOfApplication;
            int yearOfApplication;
            if (fertiliserApplied[i].GetMonth_applied() < GetStartMonth())
            {
                monthOfApplication = fertiliserApplied[i].GetMonth_applied() + 12 - GetStartMonth();
                yearOfApplication = GetEndYear();
            }
            else
            {
                monthOfApplication = fertiliserApplied[i].GetMonth_applied() - GetStartMonth();
                yearOfApplication = GetStartYear();
            }
            fertiliserApplied[i].SetdayOfApplication((int)Math.Round(monthOfApplication * 30.416 + 15));
            if (fertiliserApplied[i].GetdayOfApplication() > duration)
                fertiliserApplied[i].SetdayOfApplication((int)duration);
            fertiliserApplied[i].SetapplicDate(fertiliserApplied[i].GetdayOfApplication(), fertiliserApplied[i].GetMonth_applied(), yearOfApplication);
        }
        for (int i = 0; i < manureApplied.Count; i++)
        {
            int monthOfApplication;
            int yearOfApplication;
            if (manureApplied[i].GetMonth_applied() < GetStartMonth())
            {
                monthOfApplication = manureApplied[i].GetMonth_applied() + 12 - GetStartMonth();
                yearOfApplication = GetEndYear();
            }
            else
            {
                monthOfApplication = manureApplied[i].GetMonth_applied() - GetStartMonth();
                yearOfApplication = GetStartYear();
            }
            manureApplied[i].SetdayOfApplication((int)Math.Round(monthOfApplication * 30.416 + 15));
            if (manureApplied[i].GetdayOfApplication() > duration)
                manureApplied[i].SetdayOfApplication((int)duration);
            manureApplied[i].SetapplicDate(manureApplied[i].GetdayOfApplication(), manureApplied[i].GetMonth_applied(), yearOfApplication);
        }
    }

    public bool CheckCropCBalance(string rotationName, int cropNo)
    {
        bool retVal = false;
        double manureCinput = GetManureC();
        double Cinp = CFixed;
        double residueCfromPrevious = 0;
        if (residueFromPrevious != null)
        {
            residueCfromPrevious = residueFromPrevious.GetModelled_yield() * residueFromPrevious.composition.GetC_conc();
            Cinp += residueCfromPrevious;
        }
        double residueCtoNext = 0;
        if (residueToNext != null)
            residueCtoNext = residueToNext.GetModelled_yield() * residueToNext.composition.GetC_conc();
        double Cout = surfaceResidueC + subsurfaceResidueC + harvestedC + burntResidueC + residueCtoNext;
        double Cbalance = Cinp - Cout;
        if (Cinp != 0) //not bare soil
        {
            double diff = Cbalance / Cinp;
            double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
            if (Math.Abs(diff) > tolerance)
            {
                double errorPercent = 100 * diff;
                string messageString=("Error; Crop C balance error is more than the permitted margin of "
                        + tolerance.ToString() +"\n");
                messageString+=("Crop name " + name+"\n");
                messageString+=("Percentage error = " + errorPercent.ToString("0.00") + "%");
                GlobalVars.Instance.Error(messageString);
            }
            else
                return true;
        }
        else
            retVal = true;
        return retVal;
    }

    public void CheckCropNBalance(string rotationName, int cropNo)
    {
        double Ninp = 0;
        double residueNfromPrevious = 0;
        if (Getname() != "Bare soil")
            Ninp = CalculateCropNUptake();
        else
        {
            if (residueFromPrevious != null)
            {
                residueNfromPrevious = residueFromPrevious.GetModelled_yield() * residueFromPrevious.composition.GetN_conc();
                Ninp += residueNfromPrevious;
            }
        }
        double residueNtoNext = 0;
        if (residueToNext!=null)
            residueNtoNext = residueToNext.GetModelled_yield() * residueToNext.composition.GetN_conc();
        double Nout = surfaceResidueN + subsurfaceResidueN + harvestedN + burntResidueN + residueNtoNext;
        double Nbalance = Ninp - Nout;
       // GlobalVars.Instance.log("crop " + name + " Ninp " + Ninp.ToString() + " Nsurface " + surfaceResidueN.ToString() + " Nsubsurf " + subsurfaceResidueN.ToString() +
         //   " harvestedN" + harvestedN.ToString());
        if (Ninp != 0) //not bare soil
        {
            double diff = Nbalance / Ninp;
            double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
            if (Math.Abs(diff) > tolerance)
            {               
                    double errorPercent = 100 * diff;                 
                    string messageString=("Error; Crop N balance error is more than the permitted margin of "
                        + tolerance.ToString() +"\n");
                    messageString+=("Crop name " + name+"\n");
                    messageString+=("Percentage error = " + errorPercent.ToString("0.00") + "%");
                    GlobalVars.Instance.Error(messageString);
           }
        }
    }

    public double  CalcDuration()
    {
        return duration = theEndDate.getLongTime() - theStartDate.getLongTime() + 1;
    }

    public void Write()
    {
        GlobalVars.Instance.writeStartTab("CropClass");
        
            GlobalVars.Instance.writeInformationToFiles("Identity", "Identity", "-", identity, Parens);
            GlobalVars.Instance.writeInformationToFiles("name", "Crop name", "-", name, Parens);
            GlobalVars.Instance.writeInformationToFiles("area", "Area", "ha", area, Parens);
            GlobalVars.Instance.writeStartTab("theStartDate");
            theStartDate.Write();
            GlobalVars.Instance.writeEndTab();
            GlobalVars.Instance.writeStartTab("theEndDate");
            theEndDate.Write();
            GlobalVars.Instance.writeEndTab();
            GlobalVars.Instance.writeInformationToFiles("isIrrigated", "Is irrigated", "-", isIrrigated, Parens);
            GlobalVars.Instance.writeInformationToFiles("fertiliserN2OEmission", "N2O emission from fertiliser", "kgN/ha", fertiliserN2OEmission, Parens);
            GlobalVars.Instance.writeInformationToFiles("cropResidueN2O", "N2O emission from crop residues", "kgN/ha", cropResidueN2O, Parens);
            GlobalVars.Instance.writeInformationToFiles("soilN2OEmission", "N2O emission from mineralised N", "kgN/ha", soilN2OEmission, Parens);
            GlobalVars.Instance.writeInformationToFiles("unutilisedGrazableDM", "Unutilised grazable DM", "kg DM/ha", unutilisedGrazableDM, Parens);

            //        GlobalVars.Instance.writeInformationToFiles("NyieldMax", "??", "??", NyieldMax);
            //potential and water limited yield

            GlobalVars.Instance.writeInformationToFiles("CFixed", "C fixed", "kgC/ha", CFixed, Parens);
            GlobalVars.Instance.writeInformationToFiles("surfaceResidueC", "C in surface residues", "kgC/ha", surfaceResidueC, Parens);
            GlobalVars.Instance.writeInformationToFiles("subsurfaceResidueC", "C in subsurface residues", "kgC/ha", subsurfaceResidueC, Parens);

            GlobalVars.Instance.writeInformationToFiles("urineCCropClass", "C in urine", "kgC/ha", urineC, Parens);
            double amount=0;
            for(int i=0;i<manureFOMCsurface.Length;i++)
            {
                amount+=manureFOMCsurface[i];
            }
            GlobalVars.Instance.writeInformationToFiles("manureFOMCsurface", "manureFOMCsurface", "kgC/ha", amount, Parens);
            GlobalVars.Instance.writeInformationToFiles("faecalCCropClass", "C in faeces", "kgC/ha", faecalC, Parens);
            GlobalVars.Instance.writeInformationToFiles("storageProcessingCLoss", "C lost during processing or storage", "kgC/ha", storageProcessingCLoss, Parens);
            GlobalVars.Instance.writeInformationToFiles("fertiliserC", "Emission of CO2 from fertiliser", "kgC/ha", fertiliserC, Parens);
            GlobalVars.Instance.writeInformationToFiles("harvestedC", "Harvested C", "kgC/ha", harvestedC, Parens);
            GlobalVars.Instance.writeInformationToFiles("harvestedDM", "Harvested DM", "kg DM/ha", harvestedDM, Parens);
            GlobalVars.Instance.writeInformationToFiles("burntResidueC", "C in burned crop residues", "kg/ha", burntResidueC, Parens);
            GlobalVars.Instance.writeInformationToFiles("unutilisedGrazableC", "C in unutilised grazable DM", "kg/ha", unutilisedGrazableC, Parens);

            GlobalVars.Instance.writeInformationToFiles("NyieldMax", "Maximum N yield", "kgN/ha", NyieldMax, Parens);
            GlobalVars.Instance.writeInformationToFiles("NavailFact", "N availability factor", "-", NavailFact, Parens);
            GlobalVars.Instance.writeInformationToFiles("maxCropNuptake", "Maximum crop N uptake", "kgN/ha", maxCropNuptake, Parens);
            GlobalVars.Instance.writeInformationToFiles("cropNuptake", "Crop N uptake", "kgN/ha", cropNuptake, Parens);
            GlobalVars.Instance.writeInformationToFiles("Nfixed", "N fixed", "kgN/ha", Nfixed, Parens);
            GlobalVars.Instance.writeInformationToFiles("nAtm", "N from atmospheric deposition", "kgN/ha", nAtm, Parens);
            GlobalVars.Instance.writeInformationToFiles("fertiliserNinput", "Input of N in fertiliser", "kgN/ha", fertiliserNinput, Parens);
            GlobalVars.Instance.writeInformationToFiles("excretaNInput", "Input of N in excreta", "kgN/ha", excretaNInput, Parens);
            GlobalVars.Instance.writeInformationToFiles("totalManureNApplied", "Total N applied in manure", "kgN/ha", totalManureNApplied, Parens);
            GlobalVars.Instance.writeInformationToFiles("mineralNFromLastCrop", "N2O emission from mineralised N", "kgN/ha", mineralNFromLastCrop, Parens);
            GlobalVars.Instance.writeInformationToFiles("manureNH3emission", "NH3-N from manure application", "kgN/ha", manureNH3emission, Parens);
            GlobalVars.Instance.writeInformationToFiles("surfaceResidueN", "N in surface residues", "kgN/ha", surfaceResidueN, Parens);
            GlobalVars.Instance.writeInformationToFiles("subsurfaceResidueN", "N in subsurface residues", "kgN/ha", subsurfaceResidueN, Parens);

            GlobalVars.Instance.writeInformationToFiles("surfaceResidueDM", "Surface residue dry matter", "kg/ha", surfaceResidueDM, Parens);
            GlobalVars.Instance.writeInformationToFiles("fertiliserNH3emission", "NH3-N from fertiliser application", "kgN/ha", fertiliserNH3emission, Parens);
            GlobalVars.Instance.writeInformationToFiles("urineNH3emission", "NH3-N from urine deposition", "kgN/ha", urineNH3emission, Parens);
            GlobalVars.Instance.writeInformationToFiles("harvestedN", "N harvested (N yield)", "kgN/ha", harvestedN, Parens);
            GlobalVars.Instance.writeInformationToFiles("storageProcessingNLoss", "N2 emission during product processing/storage", "kgN/ha", storageProcessingNLoss, Parens);
            GlobalVars.Instance.writeInformationToFiles("N2Nemission", "N2 emission", "kgN/ha", N2Nemission, Parens);
            GlobalVars.Instance.writeInformationToFiles("urineNCropClass", "Urine N", "kgN/ha", urineNasFertilizer, Parens);
            GlobalVars.Instance.writeInformationToFiles("faecalNCropClass", "Faecal N", "kgN/ha", faecalN, Parens);
            GlobalVars.Instance.writeInformationToFiles("burningN2ON", "N2O emission from burned crop residues", "kgN/ha", burningN2ON, Parens);
            GlobalVars.Instance.writeInformationToFiles("burningNH3N", "NH3 emission from burned crop residues", "kgN/ha", burningNH3N, Parens);
            GlobalVars.Instance.writeInformationToFiles("burningNOxN", "NOx emission from burned crop residues", "kgN/ha", burningNOxN, Parens);
            GlobalVars.Instance.writeInformationToFiles("burningOtherN", "N2 emission from burned crop residues", "kgN/ha", burningOtherN, Parens);
            GlobalVars.Instance.writeInformationToFiles("OrganicNLeached", "Leached organic N", "kgN/ha", OrganicNLeached, Parens);
            GlobalVars.Instance.writeInformationToFiles("mineralNToNextCrop", "Mineral N to next crop", "kgN/ha", mineralNToNextCrop, Parens);
            GlobalVars.Instance.writeInformationToFiles("fertiliserN2OEmission", "N2O emission from fertiliser N", "kgN/ha", fertiliserN2OEmission, Parens);
            GlobalVars.Instance.writeInformationToFiles("manureN2OEmission", "N2O emission from manure N", "kgN/ha", manureN2OEmission, Parens);
            GlobalVars.Instance.writeInformationToFiles("cropResidueN2O", "N2O emission from crop residue N", "kgN/ha", cropResidueN2O, Parens);
            GlobalVars.Instance.writeInformationToFiles("soilN2OEmission", "N2O emission from mineralised N", "kgN/ha", soilN2OEmission, Parens);
            GlobalVars.Instance.writeInformationToFiles("N2ONemission", "N2O emission", "kgN/ha", N2ONemission, Parens);
            GlobalVars.Instance.writeInformationToFiles("soilNMineralisation", "Soil mineralised N", "kgN/ha", soilNMineralisation, Parens);
            GlobalVars.Instance.writeInformationToFiles("mineralNavailable", "Mineral N available", "kgN/ha", mineralNavailable, Parens);
            GlobalVars.Instance.writeInformationToFiles("proportionLeached", "Proportion of other N leached", "kgN/ha", proportionLeached, Parens);
            GlobalVars.Instance.writeInformationToFiles("mineralNreserve", "Mineral N applied near end of crop perios", "kgN/ha", mineralNreserve, Parens);
            GlobalVars.Instance.writeInformationToFiles("nitrateLeaching", "Nitrate N leaching", "kgN/ha", nitrateLeaching, Parens);
            GlobalVars.Instance.writeInformationToFiles("unutilisedGrazableN", "N in unutilised grazable DM", "kg/ha", unutilisedGrazableN, Parens);

            GlobalVars.Instance.writeInformationToFiles("cumulativepotEvapoTrans", "cumulativepotEvapoTrans", "mm", GetcumulativepotEvapoTrans(), Parens);
            GlobalVars.Instance.writeInformationToFiles("cumulativePrecipitation", "cumulativePrecipitation", "mm", GetcumulativePrecipitation(), Parens);
            GlobalVars.Instance.writeInformationToFiles("cumulativeIrrigation", "cumulativeIrrigation", "mm", GetcumulativeIrrigation(), Parens);
            GlobalVars.Instance.writeInformationToFiles("cumulativeEvaporation", "cumulativeEvaporation", "mm", GetcumulativeEvaporation(), Parens);
            GlobalVars.Instance.writeInformationToFiles("cumulativeTranspiration", "cumulativeTranspiration", "mm", GetcumulativeTranspiration(), Parens);
            GlobalVars.Instance.writeInformationToFiles("cumulativeDrainage", "cumulativeDrainage", "kg/ha", GetcumulativeDrainage(), Parens);
            GlobalVars.Instance.writeInformationToFiles("AverageDroughtIndex", "AverageDroughtIndex", "kg/ha", GetAverageDroughtIndexPlant(), Parens);

        
        for (int i = 0; i < theProducts.Count; i++)
        {
            theProducts[i].Write(Parens + "_theProducts" + i.ToString());
        }
        for (int i = 0; i < fertiliserApplied.Count; i++)
        {
            fertiliserApplied[i].Write(Parens + "_fertiliserApplied" + i.ToString());
        }
        for (int i = 0; i < manureApplied.Count; i++)
        {
            manureApplied[i].Write(Parens + "_manureApplied" + i.ToString());
        }

        GlobalVars.Instance.writeEndTab();

    }
    public void WritePlantFile()
    {
        int times = 1;
        if (GlobalVars.Instance.header == false)
            times = 2;
        for (int j = 0; j < times; j++)
        {
            GlobalVars.Instance.writePlantFile("Identity", "Identity", "-", identity, Parens,0);
            GlobalVars.Instance.writePlantFile("name", "Crop name", "-", name, Parens, 0);
            GlobalVars.Instance.writePlantFile("area", "Area", "ha", area, Parens, 0);
            GlobalVars.Instance.writePlantFile("isIrrigated", "Is irrigated", "-", isIrrigated, Parens, 0);
            GlobalVars.Instance.writePlantFile("unutilisedGrazableDM", "Unutilised grazable DM", "kg/ha", unutilisedGrazableDM, Parens, 0);

            //        GlobalVars.Instance.writeInformationToFiles("NyieldMax", "??", "??", NyieldMax);
            //potential and water limited yield

            GlobalVars.Instance.writePlantFile("CFixed", "C fixed", "kgC/ha", CFixed, Parens, 0);
            GlobalVars.Instance.writePlantFile("surfaceResidueC", "C in surface residues", "kgC/ha", surfaceResidueC, Parens, 0);
            GlobalVars.Instance.writePlantFile("subsurfaceResidueC", "C in subsurface residues", "kgC/ha", subsurfaceResidueC, Parens, 0);
           
            GlobalVars.Instance.writePlantFile("urineCCropClass", "C in urine", "kgC/ha", urineC, Parens, 0);
            double amount = 0;
            for (int i = 0; i < manureFOMCsurface.Length; i++)
            {
                amount += manureFOMCsurface[i];
            }
            GlobalVars.Instance.writePlantFile("manureFOMCsurface", "manureFOMCsurface", "kgC/ha", amount, Parens,0);
            GlobalVars.Instance.writePlantFile("faecalCCropClass", "C in faeces", "kgC/ha", faecalC, Parens, 0);
            GlobalVars.Instance.writePlantFile("storageProcessingCLoss", "C lost during processing or storage", "kgC/ha", storageProcessingCLoss, Parens, 0);
            GlobalVars.Instance.writePlantFile("fertiliserC", "Emission of CO2 from fertiliser", "kgC/ha", fertiliserC, Parens, 0);
            GlobalVars.Instance.writePlantFile("harvestedC", "Harvested C", "kgC/ha", harvestedC, Parens, 0);
            GlobalVars.Instance.writePlantFile("harvestedDM", "Harvested DM", "kgC/ha", harvestedDM, Parens, 0);
            GlobalVars.Instance.writePlantFile("burntResidueC", "C in burned crop residues", "kg/ha", burntResidueC, Parens, 0);
            
            if (residueToNext!=null)
                GlobalVars.Instance.writePlantFile("residueCtoNext", "C in residues sent to next crop", "kg/ha", residueToNext.GetModelled_yield() * residueToNext.composition.GetC_conc(), Parens, 0);
            else
                GlobalVars.Instance.writePlantFile("residueCtoNext", "C in residues sent to next crop", "kg/ha", 0, Parens, 0);
            GlobalVars.Instance.writePlantFile("unutilisedGrazableC", "C in unutilised grazable DM", "kg/ha", unutilisedGrazableC, Parens, 0);

            GlobalVars.Instance.writePlantFile("NyieldMax", "Maximum N yield", "kgN/ha", NyieldMax, Parens, 0);
            GlobalVars.Instance.writePlantFile("NavailFact", "N availability factor", "-", NavailFact, Parens, 0);
            GlobalVars.Instance.writePlantFile("maxCropNuptake", "Maximum crop N uptake", "kgN/ha", maxCropNuptake, Parens, 0);
            GlobalVars.Instance.writePlantFile("cropNuptake", "Crop N uptake", "kgN/ha", cropNuptake, Parens, 0);
            GlobalVars.Instance.writePlantFile("mineralNavailable", "Mineral N available", "kgN/ha", mineralNavailable, Parens, 0);
            GlobalVars.Instance.writePlantFile("soilNMineralisation", "Soil mineralised N", "kgN/ha", soilNMineralisation, Parens, 0);
            GlobalVars.Instance.writePlantFile("Nfixed", "N fixed", "kgN/ha", Nfixed, Parens, 0);
            GlobalVars.Instance.writePlantFile("nAtm", "N from atmospheric deposition", "kgN/ha", nAtm, Parens, 0);
            GlobalVars.Instance.writePlantFile("fertiliserNinput", "Input of N in fertiliser", "kgN/ha", fertiliserNinput, Parens, 0);
            GlobalVars.Instance.writePlantFile("totalManureNApplied", "Total N applied in manure", "kgN/ha", totalManureNApplied, Parens, 0);
            GlobalVars.Instance.writePlantFile("urineNCropClass", "Urine N", "kgN/ha", urineNasFertilizer, Parens, 0);
            GlobalVars.Instance.writePlantFile("faecalNCropClass", "Faecal N", "kgN/ha", faecalN, Parens, 0);
            GlobalVars.Instance.writePlantFile("mineralNFromLastCrop", "N2O emission from mineralised N", "kgN/ha", mineralNFromLastCrop, Parens, 0);
            GlobalVars.Instance.writePlantFile("surfaceResidueN", "N in surface residues", "kgN/ha", surfaceResidueN, Parens, 0);
            GlobalVars.Instance.writePlantFile("subsurfaceResidueN", "N in subsurface residues", "kgN/ha", subsurfaceResidueN, Parens, 0);
            GlobalVars.Instance.writePlantFile("excretaNInput", "Input of N in excreta", "kgN/ha", excretaNInput, Parens, 0);
            GlobalVars.Instance.writePlantFile("manureNH3emission", "NH3-N from manure application", "kgN/ha", manureNH3emission, Parens, 0);
            GlobalVars.Instance.writePlantFile("fertiliserNH3emission", "NH3-N from fertiliser application", "kgN/ha", fertiliserNH3emission, Parens, 0);
            GlobalVars.Instance.writePlantFile("urineNH3emission", "NH3-N from urine deposition", "kgN/ha", urineNH3emission, Parens, 0);
            GlobalVars.Instance.writePlantFile("harvestedN", "N harvested (N yield)", "kgN/ha", harvestedN, Parens, 0);
            GlobalVars.Instance.writePlantFile("storageProcessingNLoss", "N2 emission during product processing/storage", "kgN/ha", storageProcessingNLoss, Parens, 0);
            GlobalVars.Instance.writePlantFile("manureN2OEmission", "N2O emission from manure N", "kgN/ha", manureN2OEmission, Parens, 0);
            GlobalVars.Instance.writePlantFile("soilN2OEmission", "N2O emission from mineralised N", "kgN/ha", soilN2OEmission, Parens, 0);
            GlobalVars.Instance.writePlantFile("fertiliserN2OEmission", "N2O emission from fertiliser", "kgN/ha", fertiliserN2OEmission, Parens, 0);
            GlobalVars.Instance.writePlantFile("cropResidueN2O", "N2O emission from crop residues", "kgN/ha", cropResidueN2O, Parens, 0);
            GlobalVars.Instance.writePlantFile("burningN2ON", "N2O emission from burned crop residues", "kgN/ha", burningN2ON, Parens, 0);
            GlobalVars.Instance.writePlantFile("N2Nemission", "N2 emission", "kgN/ha", N2Nemission, Parens, 0);
            GlobalVars.Instance.writePlantFile("burningNH3N", "NH3 emission from burned crop residues", "kgN/ha", burningNH3N, Parens, 0);
            GlobalVars.Instance.writePlantFile("burningNOxN", "NOx emission from burned crop residues", "kgN/ha", burningNOxN, Parens, 0);
            GlobalVars.Instance.writePlantFile("burningOtherN", "N2 emission from burned crop residues", "kgN/ha", burningOtherN, Parens, 0);
            GlobalVars.Instance.writePlantFile("Irrigation", "Irrigation", "mm", GetcumulativeIrrigation(), Parens, 0);
            GlobalVars.Instance.writePlantFile("N2ONemission", "N2O emission", "kgN/ha", N2ONemission, Parens, 0);
            GlobalVars.Instance.writePlantFile("nitrateLeaching", "Nitrate N leaching", "kgN/ha", nitrateLeaching, Parens, 0);
            GlobalVars.Instance.writePlantFile("mineralNToNextCrop", "Mineral N to next crop", "kgN/ha", mineralNToNextCrop, Parens, 0);
            GlobalVars.Instance.writePlantFile("Precipitation", "Cum precipitation", "mm", GetcumulativePrecipitation(), Parens, 0);
            GlobalVars.Instance.writePlantFile("Drainage", "Drainage", "mm", GetcumulativeDrainage(), Parens, 0);
            GlobalVars.Instance.writePlantFile("surfaceResidueDM", "Surface residue dry matter", "kg/ha", surfaceResidueDM, Parens, 0);
            double temp = 0;
            for (int I = 0; I < theProducts.Count; I++)
                temp += theProducts[I].Modelled_yield * theProducts[I].composition.GetStoreProcessFactor();

            GlobalVars.Instance.writePlantFile("storageProcessingDMLoss", "DM lost during processing/storage", "kg DM/ha", temp, Parens, 0);
            if (theProducts.Count == 0 && GlobalVars.Instance.header == true)
            {
                GlobalVars.Instance.writePlantFile("unutilisedGrazableN", "N in unutilised grazable DM", "kg/ha", unutilisedGrazableN, Parens, 1);

            }
            else
                GlobalVars.Instance.writePlantFile("unutilisedGrazableN", "N in unutilised grazable DM", "kg/ha", unutilisedGrazableN, Parens, 0);

            if(GlobalVars.Instance.header==false)
                for (int i = 0; i < 2; i++)
                {
                    GlobalVars.product tmp = new GlobalVars.product();
                    tmp.WritePlantFile(Parens + "_theProducts" + i.ToString(), i, 2);
                }
            if (GlobalVars.Instance.header == true)
           for (int i = 0; i < theProducts.Count; i++)
            {
                theProducts[i].WritePlantFile(Parens + "_theProducts" + i.ToString(), i, theProducts.Count);
            }
           GlobalVars.Instance.header = true;
            
        }
    }

    
    double GetLAI() { return LAI; }
    public double GetpotentialEvapoTrans(int day) { return potentialEvapoTrans[day]; }
    public double Getprecipitation(int day) { return precipitation[day]; }
    public double Gettemperature(int day) { return temperature[day]; }
    public bool GetisIrrigated() { return isIrrigated; }
    public timeClass GettheStartDate() { return theStartDate; }
    public timeClass GettheEndDate() { return theEndDate; }

    public void Setirrigation(int index, double val)
    {
        irrigationWater[index] = val;
    }
    
    public double CalculateLAI(double dayNo)
    {
        if (permanent)
            LAI = maxLAI;
        else
        {
            double maxDM = 0;
            double currentDM = 0;
            double timeIntoCrop = dayNo / duration;
            for (int i = 0; i < theProducts.Count; i++)
            {
                maxDM += theProducts[i].GetPotential_yield();
                currentDM += theProducts[i].GetExpectedYield() * timeIntoCrop;
            }
            if (maxDM == 0)
                LAI = 0;
            else
                LAI = maxLAI * currentDM / maxDM;
        }
        return LAI;
    }

    public double CalculateRootingDepth(double dayNo)
    {
        double rootingDepth = 0;
        if (permanent)
            rootingDepth = MaximumRootingDepth;
        else
        {
            double maxDM = 0;
            double currentDM = 0;
            double timeIntoCrop = dayNo / duration;
            for (int i = 0; i < theProducts.Count; i++)
            {
                maxDM += theProducts[i].GetPotential_yield();
                currentDM += theProducts[i].GetExpectedYield() * timeIntoCrop;
            }
            if (maxDM == 0)
                rootingDepth = 0;
            else
                rootingDepth = MaximumRootingDepth * currentDM / maxDM;
        }
        return rootingDepth;
    }
    public double CalculatedroughtFactorPlant()
    {
        double cumDroughtIndex = 0;
        for (int i = 0; i < duration; i++)
        {
            cumDroughtIndex += droughtFactorPlant[i];
        }
        cropdroughtIndex = cumDroughtIndex / (double)duration;
        return cropdroughtIndex;
    }


    public double CalculatedroughtFactorSoil()
    {
        double cumDroughtIndex = 0;
        for (int i = 0; i < duration; i++)
        {
            cumDroughtIndex += droughtFactorSoil[i];
        }
        cropdroughtIndex = cumDroughtIndex / (double)duration;
        return cropdroughtIndex;
    }


    public double GetcumulativePrecipitation()
    {
        double cumPrecip = 0;
        for (int i = 0; i < (int)duration; i++)
            cumPrecip += precipitation[i];
        return cumPrecip;
    }

    public double GetcumulativeDrainage()
    {
        double cum = 0;
        for (int i = 0; i < (int)duration; i++)
            cum += drainage[i];
        return cum;
    }

    public double GetcumulativeIrrigation()
    {
        double cum = 0;
        for (int i = 0; i < (int)duration; i++)
            cum += irrigationWater[i];
        return cum;
    }

    public double GetcumulativepotEvapoTrans()
    {
        double cum = 0;
        for (int i = 0; i < (int)duration; i++)
            cum += potentialEvapoTrans[i];
        return cum;
    }

    public double GetcumulativeEvaporation()
    {
        double cum = 0;
        for (int i = 0; i < (int)duration; i++)
            cum += evaporation[i];
        return cum;
    }

    public double GetcumulativeTranspiration()
    {
        double cum = 0;
        for (int i = 0; i < (int)duration; i++)
            cum += transpire[i];
        return cum;
    }

    public double GetAverageDroughtIndexPlant()
    {
        double averageDroughtIndex = 0;
        for (int i = 0; i < duration; i++)
            averageDroughtIndex += droughtFactorPlant[i];
        averageDroughtIndex /= duration;
        return averageDroughtIndex;
    }
    public double GetAverageDroughtIndexSoil()
    {
        double averageDroughtIndex = 0;
        for (int i = 0; i < duration; i++)
            averageDroughtIndex += droughtFactorSoil[i];
        averageDroughtIndex /= duration;
        return averageDroughtIndex;
    }

    public void CalculateClimate()
    {
        for (int i = 0; i < 366; i++)
        {
            potentialEvapoTrans[i] = 0;
            precipitation[i] = 0;
            temperature[i] = 0;
            Tsum[i] = 0;
        }
        timeClass clockit = new timeClass(theStartDate);
        int k = 0;
        int rainfreeDays = 0;
        double cumPrecip = 0;
        while (clockit.getLongTime() < theEndDate.getLongTime() + 1)
        {
            potentialEvapoTrans[k] = GlobalVars.Instance.theZoneData.PotentialEvapoTrans[clockit.GetMonth()-1];
            if (potentialEvapoTrans[k] > 20)
            {
                string messageString = ("Error; potential evapotranspiration for month \n");
                messageString += (k.ToString() + " is unrealistically high at " + potentialEvapoTrans[k].ToString());
                GlobalVars.Instance.Error(messageString);
            }
            int maxRainfreeDays = (int)clockit.GetDaysInMonth(clockit.GetMonth());
            if (GlobalVars.Instance.theZoneData.rainDays[clockit.GetMonth()-1] > 0)
            {
                double temp = clockit.GetDaysInMonth(clockit.GetMonth()) / GlobalVars.Instance.theZoneData.rainDays[clockit.GetMonth() - 1];
                maxRainfreeDays = (int) Math.Round(temp);
            }
            cumPrecip += GlobalVars.Instance.theZoneData.Precipitation[clockit.GetMonth() - 1] / (double)clockit.GetDaysInMonth(clockit.GetMonth());
            if (rainfreeDays >= maxRainfreeDays)
            {
                precipitation[k] = cumPrecip;
                cumPrecip = 0;
                rainfreeDays = 0;
            }
            else
                rainfreeDays++;
            //            Console.WriteLine(" k " + k + " precip " + precipitation[k].ToString("F3"));
            temperature[k] = GlobalVars.Instance.theZoneData.airTemp[clockit.GetMonth()-1];
            Tsum[k] = GlobalVars.Instance.theZoneData.GetTemperatureSum(temperature[k], baseTemperature);
            /*Console.WriteLine(name + " month " + clockit.GetMonth().ToString() + " k " + k.ToString() + " ppt " + precipitation[k].ToString("0.00")
                + " potevap " + potentialEvapoTrans[k].ToString("0.00"));*/
            k++;
            clockit.incrementOneDay();
        }
        if (cumPrecip>0)
            precipitation[k-1] += cumPrecip;
        double temp2 = GetcumulativepotEvapoTrans();
        double temp1 = GetcumulativePrecipitation();
    }
    public void CalculateLeachingAndUptake(double mineralNFromLastCrop, double evenNsupply, ref double relGrowth, 
        ref double mineralNsurplus)
    {
        double averagNitrificationInhibition=CalculateNitrificationInhibitor();
        double maxCropNuptake = CalculateMaxCropNUptake();
        if (zeroGasEmissionsDebugging)
        {
            soilN2OEmissionFactor = 0;
            manureN2OEmissionFactor = 0;
            fertiliserN2OEmissionFactor = 0;
            cropResidueN2OEmissionFactor = 0;
        }
        else
        {
            soilN2OEmissionFactor = (1-averagNitrificationInhibition) * GlobalVars.Instance.theZoneData.getsoilN2OEmissionFactor(); ;
            manureN2OEmissionFactor = (1 - averagNitrificationInhibition) * GlobalVars.Instance.theZoneData.getmanureN20EmissionFactor();
            fertiliserN2OEmissionFactor = (1 - averagNitrificationInhibition) * GlobalVars.Instance.theZoneData.getfertiliserN20EmissionFactor();
            cropResidueN2OEmissionFactor = (1 - averagNitrificationInhibition) * GlobalVars.Instance.theZoneData.getresidueN2OEmissionFactor();
        }

        modelledCropNuptake = 0;
        nitrateLeaching = 0;
        manureN2OEmission = 0;
        fertiliserN2OEmission = 0;
        Nfixed = 0;

        soilN2OEmission = soilN2OEmissionFactor * evenNsupply;
        N2Nemission = soilN2OEmission * soilN2Factor;
        double Ninp = mineralNFromLastCrop + evenNsupply;
        evenNsupply -= soilN2OEmission + N2Nemission;
        double Nout = 0;
        double cumdrainage = 0;
        evenNsupply /= duration; //added evenly over duration

        mineralNavailable= mineralNFromLastCrop;
        double N2ON = 0;
        double N2N = 0;
        for (int i = 0; i < duration; i++)
        {
            double NleachingToday = 0;
            double fixationToday = 0;
            if ((!zeroLeachingDebugging)&&(mineralNavailable>0))
                NleachingToday=(1 - nitrificationInhibitor[i]) * mineralNavailable * drainage[i] / soilWater[i];
            cumdrainage += drainage[i];
            double NuptakeToday = 0;
            if (mineralNavailable>0)
                NuptakeToday=mineralNavailable * (1 - droughtFactorPlant[i]);
            if (NuptakeToday < 0)
                Console.Write("");
            double maxDailyCropNuptake = 0;
            if ((Tsum[i] > 0)&&(maxCropNuptake>0))
                maxDailyCropNuptake = (Tsum[i] / totalTsum) * maxCropNuptake;
            if (NuptakeToday > maxDailyCropNuptake)
                NuptakeToday = maxDailyCropNuptake;
          /*  Console.WriteLine("i " + i.ToString() + " drain " + drainage[i].ToString("0.00") + " leach " + 
                NleachingToday.ToString("0.00") + " min " + mineralNavailable.ToString("0.00") + " Nup " + NuptakeToday.ToString("0.00"));*/
            if (mineralNavailable > 0)
            {
                if ((NleachingToday + NuptakeToday) > mineralNavailable)
                {
                    NleachingToday *= mineralNavailable / (NleachingToday + NuptakeToday);
                    NuptakeToday *= mineralNavailable / (NleachingToday + NuptakeToday);
                }
            }
            nitrateLeaching += NleachingToday;
            modelledCropNuptake += NuptakeToday;
            if (((maxDailyCropNuptake - NuptakeToday) > 0) && (NfixationFactor > 0))
            {
                fixationToday = GetNfixation(maxDailyCropNuptake - NuptakeToday) * (1 - droughtFactorPlant[i]);
                Ninp += fixationToday;
            }
            modelledCropNuptake += fixationToday;
            Nfixed += fixationToday;
            Nout += NuptakeToday + NleachingToday + fixationToday;
            mineralNavailable += evenNsupply - (NleachingToday + NuptakeToday);
            if (fertiliserN[i] > 0)
            {
                Ninp += fertiliserN[i];
                N2ON = fertiliserN2OEmissionFactor * fertiliserN[i];
                fertiliserN2OEmission += N2ON;
                N2N = N2ON * soilN2Factor;
                N2Nemission += N2N;
                mineralNavailable += fertiliserN[i] - (N2ON + N2N);
            }
            if (manureTAN[i] > 0)
            {
                Ninp += manureTAN[i];
                N2ON = manureN2OEmissionFactor * manureTAN[i];
                manureN2OEmission += N2ON;
                N2N = N2ON * soilN2Factor;
                N2Nemission += N2N;
                mineralNavailable += manureTAN[i] - (N2ON + N2N);
            }
        }
        if (modelledCropNuptake > maxCropNuptake)
        {
            mineralNavailable += modelledCropNuptake - maxCropNuptake;
            modelledCropNuptake = maxCropNuptake;
        }
        if (maxCropNuptake > 0) //not bare soil
            relGrowth = modelledCropNuptake / maxCropNuptake;
        else
            relGrowth = 0;
//        if (nitrateLeaching > 0)
  //          Console.Write("drain " + cumdrainage.ToString());
        GlobalVars.Instance.log("Cum potevap " + GetcumulativepotEvapoTrans().ToString("F6") + " Cum ppt " + GetcumulativePrecipitation().ToString("F6")
            + " evap " + GetcumulativeEvaporation().ToString("F6") + " ave drought " + GetAverageDroughtIndexPlant() + " cum drain " + GetcumulativeDrainage().ToString("F6"),5);
        mineralNsurplus = mineralNavailable;
        N2ONemission = fertiliserN2OEmission + manureN2OEmission + soilN2OEmission;
        Nout += N2ONemission + N2Nemission;
        double balance = Ninp - (Nout + mineralNavailable);
    }


}
