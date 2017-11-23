using System;
using System.Collections.Generic;
using System.Xml;

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
        public string Applic_techniqueS;
        public int Applic_techniqueI;
        public int manureType;
        public int speciesGroup;
        public string Units;
        public void Write(string theParens)
        {
            Parens = theParens;
            GlobalVars.Instance.writeStartTab("fertRecord");

            GlobalVars.Instance.writeInformationToFiles("Namount", "Total N applied", "kg/ha", Namount,Parens);
            GlobalVars.Instance.writeInformationToFiles("Applic_techniqueS", "Application technique", "-", Applic_techniqueS, Parens);
            GlobalVars.Instance.writeInformationToFiles("Applic_techniqueI", "Application technique ID", "-", Applic_techniqueI, Parens);
            GlobalVars.Instance.writeInformationToFiles("manureType", "Storage category", "-", manureType, Parens);
            GlobalVars.Instance.writeInformationToFiles("speciesGroup", "Species category", "-", speciesGroup, Parens);
            GlobalVars.Instance.writeInformationToFiles("Month_applied", "Month applied", "month", Month_applied, Parens);
            GlobalVars.Instance.writeInformationToFiles("dayOfApplication", "Day applied", "Day in month", dayOfApplication, Parens);

            GlobalVars.Instance.writeEndTab();


        }
        public void setParens(string aParen) { Parens = aParen; }
        public double getNamount() { return Namount; }
        public string getName() { return Name; }
        public int getspeciesGroup() { return speciesGroup; }
        public int getstorageType() { return manureType; }
        public int GetMonth_applied() { return Month_applied; }
        public void SetdayOfApplication(int aDay) { dayOfApplication = aDay; }
        public int GetdayOfApplication() { return dayOfApplication; }
        public void SetNamount(double aVal) { Namount = aVal; }
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
        isIrrigated = theCropToBeCopied.isIrrigated;
        fertiliserApplied=new List<fertRecord>();
        for (int i = 0; i < theCropToBeCopied.fertiliserApplied.Count; i++)
        {
            fertiliserApplied.Add( theCropToBeCopied.fertiliserApplied[i]);
        }
        manureApplied=new List<fertRecord>();
        for (int i = 0; i < theCropToBeCopied.manureApplied.Count; i++)
        {
            manureApplied.Add(theCropToBeCopied.manureApplied[i]);
        }
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
 
  
        fertiliserC = theCropToBeCopied.fertiliserC;
        urineC = theCropToBeCopied.urineC;
        faecalC = theCropToBeCopied.faecalC;
        harvestedC = theCropToBeCopied.harvestedC;
        storageProcessingCLoss = theCropToBeCopied.storageProcessingCLoss;
        storageProcessingNLoss = theCropToBeCopied.storageProcessingNLoss;
        mineralNavailable = theCropToBeCopied.mineralNavailable;
        residueN = theCropToBeCopied.residueN;
        excretaNInput = theCropToBeCopied.excretaNInput;
        fertiliserNinput = theCropToBeCopied.fertiliserNinput;
        harvestedN = theCropToBeCopied.harvestedN;
        totalManureNApplied = theCropToBeCopied.totalManureNApplied;
        N2ONemission = theCropToBeCopied.N2ONemission;
        N2Nemission = theCropToBeCopied.N2Nemission;
        cropNuptake = theCropToBeCopied.cropNuptake;
        manureFOMn = theCropToBeCopied.manureFOMn;
        manureHUMn = theCropToBeCopied.manureHUMn;
        urineN = theCropToBeCopied.urineN;
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
    List<fertRecord> manureApplied;

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
    long duration = 0;
    bool permanent = false;
    //bool continues = false;

    List<GlobalVars.product> theProducts = new List<GlobalVars.product>();

    //other variables
    //variables to output
    double CFixed = 0;//carbon fixed
    double surfaceResidueC;
    double subsurfaceResidueC;
    double urineC = 0;
    double faecalC = 0;
    double harvestedC = 0;
    double storageProcessingCLoss = 0;
    double storageProcessingNLoss = 0;

    double NyieldMax = 0;
    double maxCropNuptake = 0;
    double NavailFact = 0;
    double Nfixed = 0;
    double nAtm = 0;
    double manureNH3emission = 0;
    double fertiliserNH3emission = 0;
    double urineNH3emission = 0;
    double surfaceResidueN;
    double subsurfaceResidueN;
    double surfaceResidueDM = 0;
    double leachedManureTAN;
    double leachedFertiliserN;
    double leachedUrineN;
    double leachedOtherN;
    double fertiliserC = 0;
    double mineralNavailable = 0;
    double residueN = 0;
    double excretaNInput = 0;
    double fertiliserNinput = 0;
    double harvestedN = 0;
    double totalManureNApplied = 0;
    double N2ONemission = 0;
    double N2Nemission = 0;
    double cropNuptake = 0;
    double manureFOMn = 0;
    double manureHUMn = 0;
    double urineN = 0;
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
    //other variables not for output
    public double [] manureFOMCsurface;
    double [] manureHUMCsurface;
    double [] manureFOMCsubsurface;
    double [] manureHUMCsubsurface;
    double[] manureFOMNsurface;
    double[] manureHUMNsurface;
    double[] manureTAN;

    public void Setname(string aname) { name = aname; }
    public void Setidentity(int aValue) { identity = aValue; }
    public void setArea(double aVal) { area = aVal; }
    public void setsoilN2Factor(double aVal) { soilN2Factor = aVal; }
    public void Setduration(long aVal) { duration = aVal; }
    public void SetcropSequenceName(string aVal) { cropSequenceName = aVal; }
    public void SetcropSequenceNo(int aVal) { cropSequenceNo = aVal; }
    public void SetpreviousPhaseCropNuptake(double aVal) { previousPhaseCropNuptake = aVal; }
    public double getMineralNFromLastCrop(){return mineralNFromLastCrop;}
    public double getLeachedFertiliserN(){return leachedFertiliserN;}
    public double getLeachedUrineN() { return leachedUrineN; }
    public double getLeachedOtherN() { return leachedOtherN; }
    public double getCropResidueN2O() { return cropResidueN2O; }
    public double getNFix() { return Nfixed; }
    public double getnAtm() { return nAtm; }
    public string Getname() { return name; }
    public double getSurfaceResidueDM(){return surfaceResidueDM;}
    public int Getidentity() { return identity; }
    public double getArea() { return area; }
    public void SetlengthOfSequence(double aVal) { lengthOfSequence = aVal; }
    private double GetpropBelowGroundResidues() { return propBelowGroundResidues; }
    private double GetCconcBelowGroundResidues() { return CconcBelowGroundResidues; }
    public double GetstorageProcessingCLoss() { return storageProcessingCLoss; }
    public double GetstorageProcessingNLoss() { return storageProcessingNLoss; }
    public double GetharvestedC() { return harvestedC; }
    public double GetsurfaceResidueC() { return surfaceResidueC; }
    public double GetsubsurfaceResidueC() { return subsurfaceResidueC; }
    public double GetsurfaceResidueN() { return surfaceResidueN; }
    public double GetsubsurfaceResidueN() { return subsurfaceResidueN; }
    public double GeturineNH3emission() { return urineNH3emission; }
    public double GetFertiliserC() { return fertiliserC; }
    public double GetUnutilisedGrazableC() { return unutilisedGrazableC; }
    public double GetManureC() 
    {
        double retVal = 0;
        int numDays = manureHUMCsurface.Length;
        for (int i = 0; i < numDays; i++)
            retVal += manureHUMCsurface[i] + manureFOMCsurface[i] + manureHUMCsubsurface[i] + manureFOMCsubsurface[i];
        return retVal;
    }
    public double GetUrineN() { return urineN; }
    public double GetUrineC() { return urineC; }
    public double GetfaecalC() { return faecalC; }
    public double GetfaecalN() { return faecalN; }
    public double GetresidueN() { return residueN; }
    public double GettotalManureNApplied() { return totalManureNApplied; }
    public double GetexcretaNInput() { return excretaNInput; }
    public double GetmanureNH3Nemission() { return manureNH3emission; }
    public double GetN2ONemission() { return N2ONemission; }
    public double GetN2Nemission() { return N2Nemission; }
    public double GetfertiliserNH3Nemission() { return fertiliserNH3emission; }
    public double GetleachedManureTAN() { return leachedManureTAN; }
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
    public void CalcProportionLeached()
    {
        proportionLeached = GlobalVars.Instance.theZoneData.GetPropFieldCapacity(this);
    }

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
        theStartDate.setDate(Start_day, Start_month, Start_year);
        int End_day = cropInformation.getItemInt("End_day");
        int End_month = cropInformation.getItemInt("End_month");
        int End_year = cropInformation.getItemInt("End_year");
        theEndDate.setDate(End_day, End_month, End_year);
        duration = theEndDate.getLongTime() - theStartDate.getLongTime() + 1;
        if (duration < 0)
        {
            GlobalVars.Instance.Error("negative duration");
        }
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
        if (length!=-1)
            permanent = true;
        int minFertiliserApplied = 99, maxFertiliserApplied = 0;
        cropInformation.PathNames.Add("Fertilizer_applied");
        cropInformation.getSectionNumber(ref minFertiliserApplied, ref maxFertiliserApplied);

        for (int i = minFertiliserApplied; i <= maxFertiliserApplied; i++)
        {
            fertRecord newFertRecord = new fertRecord();
            cropInformation.Identity.Add(i);
            newFertRecord.Name = cropInformation.getItemString("Name");
            newFertRecord.Units = cropInformation.getItemString("Unit");
            newFertRecord.Namount = cropInformation.getItemDouble("Value");
            newFertRecord.Month_applied = cropInformation.getItemInt("Month_applied");
            cropInformation.PathNames.Add("Applic_technique_Fertilizer");
            cropInformation.Identity.Add(-1);
            newFertRecord.Applic_techniqueS = cropInformation.getItemString("NameS");
            newFertRecord.Applic_techniqueI = cropInformation.getItemInt("NameI");
            fertiliserApplied.Add(newFertRecord);
            cropInformation.PathNames.RemoveAt(cropInformation.PathNames.Count - 1);
            cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
            cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
        }

        int minManure_applied = 99, maxManure_applied = 0;
        cropInformation.PathNames[cropInformation.PathNames.Count - 1] = "Manure_applied";
        cropInformation.getSectionNumber(ref minManure_applied, ref maxManure_applied);

        for (int i = minManure_applied; i <= maxManure_applied; i++)
        {
            fertRecord newFertRecord = new fertRecord();
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
            if (GetEndYear()>GetStartYear()) //crop period straddles 1 Jan
            {
                if ((newFertRecord.GetMonth_applied() < GetStartMonth()) && (newFertRecord.GetMonth_applied() > GetEndMonth()))
                    inPeriod=false;
            }
            else //crop period is within one calendar year
            {
                if ((newFertRecord.GetMonth_applied() < GetStartMonth()) || (newFertRecord.GetMonth_applied() > GetEndMonth()))
                    inPeriod=false;
                }
            if (inPeriod==false)
            {
  
                string messageString=("Error - manure applied outside the period of this crop\n");
                messageString = messageString + ("Crop sequence name = " + cropSequenceName + "\n");
                messageString = messageString + ("Crop sequence number = " + cropSequenceNo + "\n");
                messageString = messageString + ("Crop name = " + name);
                GlobalVars.Instance.Error(messageString);
            }
            newFertRecord.manureType = cropInformation.getItemInt("StorageType");
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
        getParameters(index, zoneNr, path);
    }
    public void UpdateParens(string aParent, int ID)
    {
        identity = ID;
        Parens = aParent + "_" + theStartDate.GetYear().ToString();

        for (int i = 0; i < fertiliserApplied.Count; i++)
        {
            fertiliserApplied[i].setParens( Parens + "_" + i.ToString());
        }
        for (int i = 0; i < manureApplied.Count; i++)
        {
            manureApplied[i].setParens(Parens + "_" + i.ToString());
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
        double fedAtPastureProduction = 0;
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

                string temp = path + "(" + index + ")" + ".Product" + "(" + i.ToString() + ").Potential_yield(-1)";
                anExample.Potential_yield = cropInformation.getItemDouble("Value", temp);
                temp = path + "(" + index + ")" + ".Product" + "(" + i.ToString() + ").Expected_yield(-1)";
                anExample.Expected_yield = cropInformation.getItemDouble("Value", temp);
                totProduction += anExample.Potential_yield;
                if (anExample.composition.GetfedAtPasture() == true)
                {
                    anExample.Grazed_yield = anExample.Expected_yield;
                    fedAtPastureProduction += anExample.Expected_yield;
                }
                else
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
        if (name != "Bare soil")
        {
            tmpPath = basePath + ".UrineNH3EF(-1)";
            cropParamFile.setPath(tmpPath);
            urineNH3EmissionFactor = cropParamFile.getItemDouble("Value");

            cropParamFile.Identity.RemoveAt(cropParamFile.Identity.Count - 1);

            cropParamFile.PathNames[cropParamFile.PathNames.Count - 1] = "Crop";
            int min = 99, max = 0;
            cropParamFile.getSectionNumber(ref min, ref max);
            bool gotit = false;
            string aname ="None";
            for (int j = min; j <= max; j++)
            {
                tmpPath = basePath + ".Crop" + "(" + j.ToString() + ")";
                cropParamFile.setPath(tmpPath);
                aname = cropParamFile.getItemString("Name");
                Console.WriteLine(aname + " " + name);
                if (aname == name)
                {
                    gotit = true;
                    break;
                }
            }
            if (gotit == false)
            {
              
                string message1=("Error - could not find crop in parameter file\n");
                message1 += message1 + ("Crop source = " + cropPath + "(" + cropIdentityNo.ToString() + ")\n");
                message1+=("Crop name = " + name);
                GlobalVars.Instance.Error(message1);
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
            cropParamFile.getSectionNumber(ref min, ref max);
            cropParamFile.Identity.Add(-1);
            for (int i = min; i <= max; i++)
            {
                cropParamFile.Identity[cropParamFile.PathNames.Count - 1] = i;
                string harvestMethodName = cropParamFile.getItemString("Name");
                if (harvestMethodName == harvestMethod)
                {
                    cropParamFile.PathNames.Add("PropAboveGroundResidues");
                    cropParamFile.Identity.Add(-1);
                    if (harvestMethodName=="Grazing")
                        propAboveGroundResidues[1] = cropParamFile.getItemDouble("Value");
                    else
                        propAboveGroundResidues[0] = cropParamFile.getItemDouble("Value");
                    break;
                }
            }
        }
    }

    public void Calcwaterlimited_yield(double droughtIndex)
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
                NyieldMax += theProducts[k].composition.GetN_conc() * theProducts[k].Potential_yield * (1 - droughtIndex);
                theProducts[k].SetwaterLimited_yield(theProducts[k].Potential_yield * (1 - droughtIndex));
            }
            theProducts[k].SetExpectedYield(theProducts[k].GetwaterLimited_yield());
        }
        maxCropNuptake=CalculateCropNUptake();
    }


    public double CalculateCropNUptake()
    {
        double uptakeN = 0;
        double NinProduct = 0;
        double NinSurfaceResidues = 0;
        double NinSubsurfaceResidues = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].composition.GetfedAtPasture()) //grazed crop
            {
                NinProduct = theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc();
                NinSurfaceResidues = theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * propAboveGroundResidues[1];
                NinSubsurfaceResidues = theProducts[i].GetExpectedYield() * (GetCconcBelowGroundResidues() * GetpropBelowGroundResidues()) / CtoNBelowGroundResidues;
                uptakeN += NinProduct + NinSurfaceResidues + NinSubsurfaceResidues;
            }
            else
            {
                //ungrazed part of crop
                NinProduct = theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc();
                NinSurfaceResidues = theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * propAboveGroundResidues[0];
                NinSubsurfaceResidues = theProducts[i].GetExpectedYield() * (GetCconcBelowGroundResidues() * GetpropBelowGroundResidues()) / CtoNBelowGroundResidues;
                uptakeN += NinProduct + NinSurfaceResidues + NinSubsurfaceResidues;
            }
        }
        return uptakeN;
    }

       public void CalculateHarvestedYields()
    {
        harvestedC = 0;
        harvestedN = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].composition.GetfedAtPasture()) //grazed crop
            {
                if (GlobalVars.Instance.GetstrictGrazing())
                {
                    harvestedC += theProducts[i].Grazed_yield * theProducts[i].composition.GetC_conc();
                    harvestedN += theProducts[i].Grazed_yield * theProducts[i].composition.GetN_conc();
                }
                else
                {
                    if (theProducts[i].GetExpectedYield() > theProducts[i].GetGrazed_yield())
                    {
                        //theProducts[i].SetExpectedYield(theProducts[i].GetGrazed_yield());
                        harvestedC += theProducts[i].GetGrazed_yield() * theProducts[i].composition.GetC_conc();
                        harvestedN += theProducts[i].GetGrazed_yield() * theProducts[i].composition.GetN_conc();
                    }
                    else
                    {
                        harvestedC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc();
                        harvestedN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc();
                    }
                }
            }
            else
            {
                if ((theProducts[i].Harvested == "Harvested") || (theProducts[i].Harvested == "Stubbleburning"))
                {
                    harvestedC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc();
                    harvestedN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc();
                }
            }
        }
    }

    public void CalculateCFixed()
    {
        CFixed = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            double CFixedThisCrop = 0;
            double CaboveGroundResidues = 0;
            double CbelowGroundResidues = 0;
            double CHarvestable=0;
            if (theProducts[i].composition.GetfedAtPasture())  //
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
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].composition.GetfedAtPasture()) //grazed crop
            {
                surfaceResidueC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc() * propAboveGroundResidues[1];
                surfaceResidueN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * propAboveGroundResidues[1];
                surfaceResidueDM += theProducts[i].GetExpectedYield() * propAboveGroundResidues[1];
                if (theProducts[i].Expected_yield >= theProducts[i].Grazed_yield)//yield in excess of grazed is added to surface residues
                {
                    unutilisedGrazableC+=(theProducts[i].Expected_yield - theProducts[i].Grazed_yield) * theProducts[i].composition.GetC_conc();
                    surfaceResidueC += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield) * theProducts[i].composition.GetC_conc();
                    unutilisedGrazableN += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield) * theProducts[i].composition.GetN_conc();
                    surfaceResidueN += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield) * theProducts[i].composition.GetN_conc();
                    unutilisedGrazableDM += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield);
                    surfaceResidueDM += (theProducts[i].Expected_yield - theProducts[i].Grazed_yield);
                }
            }
            else //ungrazed crop
            {
                if ((theProducts[i].Harvested == "Harvested")||(theProducts[i].Harvested == "Stubbleburning"))
                {
                    surfaceResidueC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc() * propAboveGroundResidues[0];
                    surfaceResidueN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * propAboveGroundResidues[0];
                    surfaceResidueDM += theProducts[i].GetExpectedYield() * propAboveGroundResidues[0];
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


    public void CalculateCropResidueBurning()
    {
        double DMburnt = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            if ((theProducts[i].Harvested == "Burnt")||(theProducts[i].Harvested == "Stubbleburning"))
            {
                DMburnt = surfaceResidueDM;
                burntResidueC = surfaceResidueC;
                burntResidueN = surfaceResidueN;
                surfaceResidueN = 0.0;
                surfaceResidueC = 0.0;
            }
        }
        burningBlackC = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueBlackCEmissionFactor();
        burningCOC = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueCOEmissionFactor();
        burningCO2C = burntResidueC - (burningBlackC + burningCOC);

        burningN2ON = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueN2OEmissionFactor();
        burningNH3N = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueNH3EmissionFactor();
        burningNOxN = DMburnt * GlobalVars.Instance.theZoneData.GetburntResidueNOxEmissionFactor();
        burningOtherN = burntResidueN - (burningN2ON + burningNH3N + burningNOxN);        
    }

    public void CalculateExcretaNInput()
    {
        excretaNInput = 0;
        urineN = 0;
        faecalN = 0;
        urineNH3emission = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            int feedCode = theProducts[i].composition.GetFeedCode();
            if (theProducts[i].composition.GetfedAtPasture())
            {
                for (int j = 0; j < GlobalVars.Instance.getmaxNumberFeedItems(); j++)
                {
                    if (theProducts[i].composition.GetFeedCode() == j)
                    {
                        double proportion = area * theProducts[i].Grazed_yield / GlobalVars.Instance.grazedArray[j].fieldDMgrazed;
                        urineN += proportion * GlobalVars.Instance.grazedArray[j].urineN / area;
                        urineNH3emission += urineNH3EmissionFactor* urineN;
                        faecalN += proportion * GlobalVars.Instance.grazedArray[j].faecesN / area;
                    }
                }
            }
        }
        double leachingProportion = 0;
        leachedUrineN = 0;
        if (urineN > 0.0)
        {
            leachingProportion = GlobalVars.Instance.theZoneData.GetPropLeaching(theStartDate.GetDay(), theStartDate.GetMonth(), theEndDate.GetDay(), theEndDate.GetMonth());
            leachedUrineN = leachingProportion * urineN;
        }
        excretaNInput = urineN + faecalN;
    }

    public void CalculateExcretaCInput()
    {
        urineC = 0;
        faecalC = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            int feedCode = theProducts[i].composition.GetFeedCode();
            //double DMgrazed = Proportion_grazed * theProducts[i].Expected_yield * (1 - propAboveGroundResidues[1]);
            if (theProducts[i].composition.GetfedAtPasture())
            {
                for (int j = 0; j < GlobalVars.Instance.getmaxNumberFeedItems(); j++)
                {
                    if (theProducts[i].composition.GetFeedCode() == j)
                    {
                        double proportion = area * theProducts[i].Grazed_yield/ GlobalVars.Instance.grazedArray[j].fieldDMgrazed ;
                        urineC += proportion * GlobalVars.Instance.grazedArray[j].urineC/area;
                        faecalC += proportion * GlobalVars.Instance.grazedArray[j].faecesC/area;
                    }
                }
            }
        }
    }

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
        leachedManureTAN = 0;
        double totManureCapplied = 0;
        manure aManure;// = new manure() ;
        for (int i = 0; i < manureApplied.Count; i++)
        {
            double amountTotalN = manureApplied[i].getNamount() * area;
            totalManureNApplied += manureApplied[i].getNamount();
            int storageType = manureApplied[i].getstorageType();
            int speciesGroup = manureApplied[i].getspeciesGroup();
            string applicType = manureApplied[i].Applic_techniqueS;

            if (lockit == false)
            {
                aManure = GlobalVars.Instance.theManureExchange.TakeManure(amountTotalN, lengthOfSequence, storageType, speciesGroup);
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
                int tmpType = GlobalVars.Instance.theZoneData.theFertManData[j].storageType;
                int tmpSpecies = GlobalVars.Instance.theZoneData.theFertManData[j].speciesGroup;
                if ((tmpType == storageType) && (tmpSpecies == speciesGroup))
                {
                    if (GlobalVars.Instance.getcurrentInventorySystem() == 1)
                    {
                        NH3EmissionFactor = GlobalVars.Instance.theZoneData.theFertManData[j].EFNH3FieldTier2;
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
            manureTAN[manureApplied[i].GetdayOfApplication()] += aManure.GetTAN();
            aManure.SetTAN(aManure.GetTAN() - tmpNH3emission);
            if ((duration - manureApplied[i].GetdayOfApplication()) < 30)  //assumes that mineral N applied close to end of crop period is not used by this crop
            {
                mineralNreserve = aManure.GetTAN();
                aManure.SetTAN(0);
            }
            else
            {
                int startDay = 15;
                if (manureApplied[i].GetMonth_applied() == GetStartMonth())
                {
                    if (GetStartDay() > 15)
                        startDay = GetStartDay();
                }
                double leachingProportion = GlobalVars.Instance.theZoneData.GetPropLeaching(startDay, manureApplied[i].GetMonth_applied(), theEndDate.GetDay(), theEndDate.GetMonth());
                leachedManureTAN += leachingProportion * aManure.GetTAN();
            }
            manureFOMCsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GetdegC() + aManure.GetnonDegC();
            manureHUMCsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GethumicC();
            manureFOMNsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GetorganicN() - aManure.GethumicN();
            manureHUMNsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GethumicN();
            totManureCapplied += aManure.GetdegC() + aManure.GetnonDegC() + aManure.GethumicC();
            Console.WriteLine(manureFOMNsurface[manureApplied[i].GetdayOfApplication() - 1].ToString());
        }
    }

    public void CalculateFertiliserInput(ref double fertiliserC, ref double fertiliserN)
    {
        fertiliserN = 0;
        fertiliserC = 0;
        leachedFertiliserN = 0;
        fertiliserNH3emission = 0;
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
            double amount = Napplied/Nconc;
            fertiliserN += Napplied;
            fertiliserC += amount * Cconc;             

            double NH3EmissionFactor = 0;

            int maxFert = 0;
            maxFert = GlobalVars.Instance.theZoneData.theFertManData.Count;
            for (int j = 0; j < maxFert; j++)
            {
                string tmpName = GlobalVars.Instance.theZoneData.theFertManData[j].name;
                if (tmpName == fertilizerName)
                {
                    NH3EmissionFactor =  GlobalVars.Instance.theZoneData.theFertManData[j].fertManNH3EmissionFactor;
                    break;
                }
            }
                double tmpNH3emission = NH3EmissionFactor * Napplied;
            fertiliserNH3emission += tmpNH3emission;
            //Napplied now refers to N entering soil
            Napplied -= tmpNH3emission;
            if ((duration - fertiliserApplied[i].GetdayOfApplication()) <30)  //assumes that mineral N applied close to end of crop period is not used by this crop
            {
                mineralNreserve = Napplied;
                fertiliserN -= Napplied;
                fertiliserApplied[i].SetNamount(0);
            }
            else
            {
                int startDay = 15;
                if (fertiliserApplied[i].GetMonth_applied() == GetStartMonth())
                {
                    if (GetStartDay() > 15)
                        startDay = GetStartDay();
                }
                double leachingProportion = GlobalVars.Instance.theZoneData.GetPropLeaching(startDay, fertiliserApplied[i].GetMonth_applied(), theEndDate.GetDay(), theEndDate.GetMonth());
                leachedFertiliserN+= leachingProportion * Napplied;
            }
        }        
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
        CalculateCFixed();
        CalculateCropResidues();
        CalculateHarvestedYields();
        CalculateCropResidueBurning();
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

    public double GetNfixation(double soilMineralNSupply)
    {
        double retVal = 0;
        if ((soilMineralNSupply < maxCropNuptake)&&(NfixationFactor>=0))
            retVal = (maxCropNuptake - soilMineralNSupply) * NfixationFactor;
        return retVal;
    }

    //Jonas - this function is messy. It is called CalcAvailableN but also has carbon in it too. Needs to be tidied
    public void CalcAvailableN(double surplusMineralN, double thesoilNMineralisation)
    {
        bool debugStopDenitrification = false;
        mineralNFromLastCrop = surplusMineralN;
        soilNMineralisation = thesoilNMineralisation;
        double manureOrgN = GetmanureOrgN();
        double totmanureTAN = GetmanureTAN();
        double soilNSupply = mineralNFromLastCrop + soilNMineralisation;
        
        double fertiliserN = 0;
        CalculateFertiliserInput(ref fertiliserC, ref fertiliserN);
        nAtm = NDepositionRate * (getEndLongTime()-getStartLongTime()+1)/365;
        //note tha N2O emissions are currently calculated after removal of NH3 emissions and without humic N
        switch (GlobalVars.Instance.getcurrentInventorySystem())
        {
            case 1:
                N2ONemission = CalculateN20(fertiliserN, (totmanureTAN + manureOrgN), 0, soilNSupply);
                break;
        }
        if (debugStopDenitrification)
            N2ONemission = 0;
        N2Nemission = N2ONemission * soilN2Factor;
        double mineralNSupply = fertiliserN + totmanureTAN + urineN + soilNSupply + nAtm;
        double otherDenitrification = 0;
        double totalOtherN = urineN + soilNSupply + nAtm;
        if (totalOtherN>0)
            otherDenitrification = (N2ONemission + N2Nemission) * (urineN + soilNSupply + nAtm) / totalOtherN;
        if (soilNSupply > 0)
            leachedOtherN = proportionLeached * ((soilNSupply + nAtm) - (otherDenitrification + urineNH3emission));
        else
            leachedOtherN = proportionLeached * (nAtm - (otherDenitrification + urineNH3emission));
        nitrateLeaching = leachedManureTAN + leachedUrineN + leachedOtherN + leachedFertiliserN;
        double mineralNLoss = fertiliserNH3emission + manureNH3emission + urineNH3emission + N2ONemission + N2Nemission + nitrateLeaching;
        mineralNavailable = mineralNSupply - mineralNLoss;
        Nfixed = GetNfixation(mineralNavailable);
        mineralNavailable += Nfixed;
        if (mineralNavailable < 0.0)
        {
            string messageString = ("Error - insufficient mineral N available to satisfy immobilisation in soil\n");
            messageString += " Crop sequence name " + cropSequenceName + " \n";
            messageString += " Crop number " + identity + " \n";
            messageString += " Crop start year " + GetStartYear().ToString();
            GlobalVars.Instance.Error(messageString);
        }
        cropNuptake = CalculateCropNUptake();

        Console.WriteLine("Min N " + mineralNSupply.ToString("0.00") + " Nfix " + Nfixed.ToString("0.00") + " soil min " + soilNMineralisation.ToString("0.00")
            + " surplus " + mineralNFromLastCrop.ToString("0.00") + " leaching " + nitrateLeaching.ToString("0.00")
            + " avail " + mineralNavailable.ToString("0.00"));
        Console.WriteLine("Manure NH3N " + manureNH3emission.ToString());
    }

    public void getGrazedFeedItems()
    {
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].composition.GetfedAtPasture())
            {
                int feedCode = theProducts[i].composition.GetFeedCode();
                GlobalVars.Instance.grazedArray[feedCode].fieldDMgrazed += theProducts[i].Grazed_yield * area / lengthOfSequence;
                GlobalVars.Instance.grazedArray[feedCode].name = theProducts[i].composition.GetName();
            }
        }     
    }
    
     public double GetDMYield()
    {
        double DMYield = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            DMYield += theProducts[i].GetExpectedYield();
        }
        return DMYield;
    }
    public bool expect()
    {
        int numberOfMatching = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            double diff = theProducts[i].GetModelled_yield() - theProducts[i].GetExpectedYield();
            double relative_diff = System.Math.Abs(diff / theProducts[i].GetModelled_yield());
            double threshold = GlobalVars.Instance.getmaxToleratedErrorYield();
            if (relative_diff < threshold)
                numberOfMatching++;
        }
        if (numberOfMatching == theProducts.Count)
            return true;
        else
            return false;
    }
    public bool CalcModelledYield(ref double surplusMineralN, bool final)
    {
        bool retVal = false;
        if (theProducts.Count > 2)
        {

           string messageString=("Error - too many products in crop");
           GlobalVars.Instance.Error(messageString);
        }

        if (theProducts.Count > 0) // not bare soil
        {
            NavailFact = 0;
            if (cropNuptake <= mineralNavailable)
                NavailFact = 1.0;
            else
            {
                NavailFact = mineralNavailable / maxCropNuptake;
            }
            Console.WriteLine("NavailFact " + NavailFact.ToString());

            for (int i = 0; i < theProducts.Count; i++)
            {
                theProducts[i].SetModelled_yield(theProducts[i].GetwaterLimited_yield() * NavailFact);
                Console.WriteLine("expected yield " + theProducts[i].Modelled_yield.ToString());
            }
            if (expect())
            {
                cropNuptake = CalculateCropNUptake();
                for (int i = 0; i < theProducts.Count; i++)
                {

                    if ((theProducts[i].composition.GetfedAtPasture()) && (GlobalVars.Instance.GetstrictGrazing()))
                    {
                        double diff_grazed = theProducts[i].GetModelled_yield() - theProducts[i].Grazed_yield;
                        double rel_diff_grazed = diff_grazed / theProducts[i].Grazed_yield;
                        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();

                        if ((rel_diff_grazed < 0.0) && (Math.Abs(rel_diff_grazed) > tolerance))
                        {

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
                
                
                
                if (cropNuptake > mineralNavailable)
                {
                    for (int i = 0; i < theProducts.Count; i++)
                    {
                        theProducts[i].SetExpectedYield((mineralNavailable / cropNuptake) * theProducts[i].GetExpectedYield());
                    }
                    surplusMineralN = 0;
                }
                else
                    surplusMineralN = mineralNavailable - cropNuptake;
                surplusMineralN += mineralNreserve; //mineral reserve is available too late to be leached
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
        else
        {
            surplusMineralN = mineralNavailable;
            surplusMineralN += mineralNreserve; //mineral reserve is available too late to be leached
            mineralNToNextCrop = surplusMineralN;
            retVal = true;
        }
        return retVal;
    }

    public int CheckYields(string RotationName)
    {
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].GetModelled_yield() == 0)
            {
                    string messageString=("Error - modelled yield is zero\n");
                    messageString+=("Rotation name = " + RotationName+"\n");
                    messageString+=("Crop product = " + theProducts[i].composition.GetName());
                    GlobalVars.Instance.Error(messageString);
            }
            else
            {
                double expected = theProducts[i].Expected_yield;
                double modelled=theProducts[i].GetModelled_yield();
                if (Double.IsNaN(modelled)) //this should never happen..
                {
                    string messageString = "Error; modelled yield has not been calculated\n";
                    messageString += "Rotation name = " + RotationName+"\n";
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
                    if (diff>0)
                        messageString = "Error; modelled yield exceeds expected yield by more than the permitted margin\n";
                    else
                        messageString = "Error; expected yield exceeds modelled yield by more than the permitted margin\n";
                    if (errorPercent < 0)
                        errorPercent *= -1.0;
                    messageString += "Rotation name = " + RotationName+"\n";
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
                    if ((theProducts[i].Harvested == "Harvested") || ((theProducts[i].composition.GetfedAtPasture())) || (theProducts[i].Harvested == "Stubbleburning"))
                    {
                        if (!theProducts[i].composition.GetfedAtPasture())
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
                        //theProducts[i].composition.Setamount(theProducts[i].GetGrazed_yield());
                        theProducts[i].composition.Setamount(theProducts[i].composition.Getamount() * area/lengthOfSequence);
                        double temp = theProducts[i].composition.Getamount() * theProducts[i].composition.GetN_conc();
                        GlobalVars.Instance.AddProductProduced(theProducts[i].composition);
                        //reset value to per ha basis (for debugging)
                        theProducts[i].composition.Setamount(theProducts[i].composition.Getamount()/area);
                    }
                }
            }
        }
        return 0;
    }

    public void AdjustDates(int firstYear)
    {
        SetEndYear(GetEndYear() - firstYear + 1);
        SetStartYear(GetStartYear()-firstYear+1);

        for (int i = 0; i < fertiliserApplied.Count; i++)
        {
            int monthOfApplication;
            if (fertiliserApplied[i].GetMonth_applied() < GetStartMonth())
                monthOfApplication = fertiliserApplied[i].GetMonth_applied() + 12 - GetStartMonth();
            else
                monthOfApplication = fertiliserApplied[i].GetMonth_applied() - GetStartMonth();
            fertiliserApplied[i].SetdayOfApplication((int)Math.Round(monthOfApplication * 30.416 + 15));
            if (fertiliserApplied[i].GetdayOfApplication() > duration)
                fertiliserApplied[i].SetdayOfApplication((int)duration);
        }
        for (int i = 0; i < manureApplied.Count; i++)
        {
            int monthOfApplication;
            if (manureApplied[i].GetMonth_applied() < GetStartMonth())
                monthOfApplication = manureApplied[i].GetMonth_applied() + 12 - GetStartMonth();
            else
                monthOfApplication = manureApplied[i].GetMonth_applied() - GetStartMonth();
            manureApplied[i].SetdayOfApplication((int)Math.Round(monthOfApplication * 30.416 + 15));
            if (manureApplied[i].GetdayOfApplication() > duration)
                manureApplied[i].SetdayOfApplication((int)duration);
        }
    }

    public bool CheckCropCBalance(string rotationName, int cropNo)
    {
        bool retVal = false;
        double manureCinput = GetManureC();
        double Cinp = CFixed;// +urineC + faecalC + fertiliserC + manureCinput;
        double Cout = surfaceResidueC + subsurfaceResidueC + harvestedC + burntResidueC;
        double Cbalance = Cinp - Cout;
        if (Cinp != 0) //not bare soil
        {
            double diff = Cbalance / Cinp;
            double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
            if (Math.Abs(diff) > tolerance)
            {
                double errorPercent = 100 * diff;
                string messageString=("Error; Crop C balance error is more than the permitted margin\n");
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
        double Ninp = CalculateCropNUptake();
        //CalculateCropResidueN();
        double Nout = surfaceResidueN + subsurfaceResidueN + harvestedN + burntResidueN;
        double Nbalance = Ninp - Nout;
       // Console.WriteLine("crop " + name + " Ninp " + Ninp.ToString() + " Nsurface " + surfaceResidueN.ToString() + " Nsubsurf " + subsurfaceResidueN.ToString() +
         //   " harvestedN" + harvestedN.ToString());
        if (Ninp != 0) //not bare soil
        {
            double diff = Nbalance / Ninp;
            double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
            if (Math.Abs(diff) > tolerance)
            {               
                    double errorPercent = 100 * diff;                 
                    string messageString=("Error; Crop N balance error is more than the permitted margin\n");
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
        Parens += "_" + theStartDate.GetYear().ToString();
        GlobalVars.Instance.writeInformationToFiles("Identity", "Identity", "-", identity,Parens);
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
        GlobalVars.Instance.writeInformationToFiles("unutilisedGrazableDM", "Unutilised grazable DM", "kg/ha", unutilisedGrazableDM, Parens);

        //        GlobalVars.Instance.writeInformationToFiles("NyieldMax", "??", "??", NyieldMax);
//potential and water limited yield

        GlobalVars.Instance.writeInformationToFiles("CFixed", "C fixed", "kgC/ha", CFixed, Parens);
        GlobalVars.Instance.writeInformationToFiles("surfaceResidueC", "C in surface residues", "kgC/ha", surfaceResidueC, Parens);
        GlobalVars.Instance.writeInformationToFiles("subsurfaceResidueC", "C in subsurface residues", "kgC/ha", subsurfaceResidueC, Parens);
        GlobalVars.Instance.writeInformationToFiles("urineC", "C in urine", "kgC/ha", urineC, Parens);
        GlobalVars.Instance.writeInformationToFiles("faecalC", "C in faeces", "kgC/ha", faecalC, Parens);
        GlobalVars.Instance.writeInformationToFiles("storageProcessingCLoss", "C lost during processing or storage", "kgC/ha", storageProcessingCLoss, Parens);
        GlobalVars.Instance.writeInformationToFiles("fertiliserC", "Emission of CO2 from fertiliser", "kgC/ha", fertiliserC, Parens);
        GlobalVars.Instance.writeInformationToFiles("harvestedC", "Harvested C", "kgC/ha", harvestedC, Parens);
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
        GlobalVars.Instance.writeInformationToFiles("leachedManureTAN", "Leached manure TAN", "kgN/ha", leachedManureTAN, Parens);
        GlobalVars.Instance.writeInformationToFiles("leachedFertiliserN", "Leached fertiliser N", "kgN/ha", leachedFertiliserN, Parens);
        GlobalVars.Instance.writeInformationToFiles("leachedUrineN", "Leached urine N", "kgN/ha", leachedUrineN, Parens);
        GlobalVars.Instance.writeInformationToFiles("leachedOtherN", "Leached other N", "kgN/ha", leachedOtherN, Parens);
        GlobalVars.Instance.writeInformationToFiles("mineralNavailable", "Mineral N available", "kgN/ha", mineralNavailable, Parens);
        GlobalVars.Instance.writeInformationToFiles("harvestedN", "N harvested (N yield)", "kgN/ha", harvestedN, Parens);
        GlobalVars.Instance.writeInformationToFiles("storageProcessingNLoss", "N2 emission from mineralised N", "kgN/ha", storageProcessingNLoss, Parens);
        GlobalVars.Instance.writeInformationToFiles("N2ONemission", "N2O emission", "kgN/ha", N2ONemission, Parens);
        GlobalVars.Instance.writeInformationToFiles("N2Nemission", "N2 emission", "kgN/ha", N2Nemission, Parens);
        GlobalVars.Instance.writeInformationToFiles("urineN", "Urine N", "kgN/ha", urineN, Parens);
        GlobalVars.Instance.writeInformationToFiles("faecalN", "Faecal N", "kgN/ha", faecalN, Parens);
        GlobalVars.Instance.writeInformationToFiles("burningN2ON", "N2O emission from burned crop residues", "kgN/ha", burningN2ON, Parens);
        GlobalVars.Instance.writeInformationToFiles("burningNH3N", "NH3 emission from burned crop residues", "kgN/ha", burningNH3N, Parens);
        GlobalVars.Instance.writeInformationToFiles("burningNOxN", "NOx emission from burned crop residues", "kgN/ha", burningNOxN, Parens);
        GlobalVars.Instance.writeInformationToFiles("burningOtherN", "N2 emission from burned crop residues", "kgN/ha", burningOtherN, Parens);
        GlobalVars.Instance.writeInformationToFiles("OrganicNLeached", "Leached organic N", "kgN/ha", OrganicNLeached, Parens);
        GlobalVars.Instance.writeInformationToFiles("soilNMineralisation", "Soil mineralised N", "kgN/ha", soilNMineralisation, Parens);
        GlobalVars.Instance.writeInformationToFiles("mineralNToNextCrop", "Mineral N to next crop", "kgN/ha", mineralNToNextCrop, Parens);
        GlobalVars.Instance.writeInformationToFiles("fertiliserN2OEmission", "N2O emission from fertiliser N", "kgN/ha", fertiliserN2OEmission, Parens);
        GlobalVars.Instance.writeInformationToFiles("manureN2OEmission", "N2O emission from manure N", "kgN/ha", manureN2OEmission, Parens);
        GlobalVars.Instance.writeInformationToFiles("cropResidueN2O", "N2O emission from crop residue N", "kgN/ha", cropResidueN2O, Parens);
        GlobalVars.Instance.writeInformationToFiles("soilN2OEmission", "N2O emission from mineralised N", "kgN/ha", soilN2OEmission, Parens);
        GlobalVars.Instance.writeInformationToFiles("proportionLeached", "Proportion of other N leached", "kgN/ha", proportionLeached, Parens);
        GlobalVars.Instance.writeInformationToFiles("mineralNreserve", "Mineral N applied near end of crop perios", "kgN/ha", mineralNreserve, Parens);
        GlobalVars.Instance.writeInformationToFiles("nitrateLeaching", "Nitrate N leaching", "kgN/ha", nitrateLeaching, Parens);
        GlobalVars.Instance.writeInformationToFiles("unutilisedGrazableN", "N in unutilised grazable DM", "kg/ha", unutilisedGrazableN, Parens);

        for (int i = 0; i < theProducts.Count; i++)
        {
            theProducts[i].Write(Parens+"_" + i.ToString());
        }
        for (int i = 0; i < fertiliserApplied.Count; i++)
        {
            fertiliserApplied[i].Write(Parens + "_" + i.ToString());
        }
        for (int i = 0; i < manureApplied.Count; i++)
        {
            manureApplied[i].Write(Parens + "_" + i.ToString());
        }

        GlobalVars.Instance.writeEndTab();

    }

}
