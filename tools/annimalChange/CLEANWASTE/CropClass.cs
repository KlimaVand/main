using System;
using System.Collections.Generic;
using System.Xml;

public class CropClass
{
    public  List<manure> theManureApplied;
    
    public struct fertRecord
    {
        public string Name;
        public double Namount;
        public int Month_applied;
        public int dayOfApplication;
        public string Applic_techniqueS;
        public int Applic_techniqueI;
        public int storageType;
        public int speciesGroup;
        public string Units;
        public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
        {
            writer.WriteStartElement("fertRecord");

            writer.WriteStartElement("Namount");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("Namount");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Namount);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("Applic_techniqueS");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("Applic_techniqueS");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Applic_techniqueS);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("Applic_techniqueI");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("Applic_techniqueI");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Applic_techniqueI);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("storageType");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("storageType");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(storageType);
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

            writer.WriteStartElement("Month_applied");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("Month_applied");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Month_applied);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("Applic_techniqueS");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("Applic_techniqueS");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Applic_techniqueS);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("Applic_techniqueI");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("Applic_techniqueI");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(Applic_techniqueI);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteStartElement("dayOfApplication");
            writer.WriteStartElement("Description");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("Units");
            writer.WriteValue("??");
            writer.WriteEndElement();
            writer.WriteStartElement("name");
            writer.WriteValue("dayOfApplication");
            writer.WriteEndElement();
            writer.WriteStartElement("Value");
            writer.WriteValue(dayOfApplication);
            writer.WriteEndElement();
            writer.WriteEndElement();

            writer.WriteEndElement();

            tabFile.Write("fertRecord"+'\n');
            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Namount" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Namount.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Applic_techniqueS" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Applic_techniqueS.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Applic_techniqueI" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Applic_techniqueI.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("storageType" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(storageType.ToString() + "\n");

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
            tabFile.Write("Units" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Units.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Month_applied" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Month_applied.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Applic_techniqueS" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Applic_techniqueS.ToString() + "\n");


            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Applic_techniqueI" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(Applic_techniqueI.ToString() + "\n");

            tabFile.Write("Description" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("Units" + "\t");
            tabFile.Write("??" + "\t");
            tabFile.Write("dayOfApplication" + "\t");
            tabFile.Write("Value" + "\t");
            tabFile.Write(dayOfApplication.ToString() + "\n");



        }
        public double getNamount() { return Namount; }
        public string getName() { return Name; }
        public int getspeciesGroup() { return speciesGroup; }
        public int getstorageType() { return storageType; }
        public int GetMonth_applied() { return Month_applied; }
        public void SetdayOfApplication(int aDay) { dayOfApplication = aDay; }
        public int GetdayOfApplication() { return dayOfApplication; }
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
//        Proportion_grazed = theCropToBeCopied.Proportion_grazed;
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
        PropSecondaryProduct = theCropToBeCopied.PropSecondaryProduct;
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
        //Proportion_grazed = theCropToBeCopied.Proportion_grazed;

        theProducts = new List<GlobalVars.product>();
        for (int i = 0; i < theCropToBeCopied.theProducts.Count; i++)
        {
            GlobalVars.product aProduct=new  GlobalVars.product(theCropToBeCopied.theProducts[i]);
            theProducts.Add(aProduct);
        }
        CFixed = theCropToBeCopied.CFixed;
        nFix = theCropToBeCopied.nFix;
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
    public List<fertRecord> fertiliserApplied;
    List<fertRecord> manureApplied;

    //parameters
    double [] propAboveGroundResidues = new double[2];
    double propBelowGroundResidues;
    double CconcBelowGroundResidues;
    double PropSecondaryProduct;
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

    double NyieldMax = 0;
    double maxCropNuptake = 0;
    double NavailFact = 0;
    double nFix = 0;
    double nAtm = 0;
    double manureNH3emission = 0;
    double fertiliserNH3emission = 0;
    double urineNH3emission = 0;
    double surfaceResidueN;
    double subsurfaceResidueN;

    double leachedManureTAN;
    double leachedFertiliserN;
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
    public void SetcropSequenceName(string aVal){cropSequenceName=aVal;}
    public double getNFix() { return nFix; }
    public double getnAtm() { return nAtm; }
    public string Getname() { return name; }
    public int Getidentity() { return identity; }
    public double getArea() { return area; }
    private double GetpropBelowGroundResidues() { return propBelowGroundResidues; }
    private double GetCconcBelowGroundResidues() { return CconcBelowGroundResidues; }
   // public double getProportion_grazed(){return Proportion_grazed;}
    public double GetstorageProcessingCLoss() { return storageProcessingCLoss; }
    public double GetharvestedC() { return harvestedC; }
    public double GetsurfaceResidueC() { return surfaceResidueC; }
    public double GetsubsurfaceResidueC() { return subsurfaceResidueC; }
    public double GetsurfaceResidueN() { return surfaceResidueN; }
    public double GetsubsurfaceResidueN() { return subsurfaceResidueN; }
    public double GeturineNH3emission() { return urineNH3emission; }
    public double GetManureC() 
    {
        double retVal = 0;
        int numDays = manureHUMCsurface.Length;
        for (int i = 0; i < numDays; i++)
            retVal += manureHUMCsurface[i] + manureFOMCsurface[i] + manureHUMCsubsurface[i] + manureFOMCsubsurface[i];
        return retVal;
    }
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
    public double GetharvestedN() { return harvestedN; }
    public double GetmineralNToNextCrop() { return mineralNToNextCrop; }
    
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
    public double GetproportionLeached() { return proportionLeached; }
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

    public CropClass(string path, int index, int zoneNr)
    {
        fertiliserApplied = new List<fertRecord>();
        manureApplied = new List<fertRecord>();
        theStartDate = new timeClass();
        theEndDate = new timeClass();

        FileInformation cropInformation = new FileInformation(GlobalVars.Instance.getFarmFilePath(), GlobalVars.Instance.GeterrorFileName());
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
        manureFOMCsurface = new double[duration];
        manureHUMCsurface = new double[duration];
        manureFOMCsubsurface = new double[duration];
        manureHUMCsubsurface = new double[duration];
        manureFOMNsurface = new double[duration];
        manureHUMNsurface = new double[duration];
        manureTAN = new double[duration];
        string tempString = cropInformation.getItemString("Irrigation");
        if (tempString == "False")
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
                System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                file.WriteLine("Error - manure applied outside the period of this crop");
                file.WriteLine("Crop sequence name = " + cropSequenceName);
                file.WriteLine("Crop name = " + name);
                file.Close();
                Console.WriteLine("Error - manure applied outside the period of this crop");
                Console.WriteLine("Crop sequence name = " + cropSequenceName);
                Console.WriteLine("Crop name = " + name);
                if (GlobalVars.Instance.getPauseBeforeExit())
                    Console.Read();
                Environment.Exit(0);
            }
            newFertRecord.storageType = cropInformation.getItemInt("StorageType");
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

    public void GetCropInformation(string path, int index, int zoneNr)
    {
        FileInformation cropInformation = new FileInformation(GlobalVars.Instance.getFarmFilePath(), GlobalVars.Instance.GeterrorFileName());
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
                feedItem aComposition = new feedItem(cropPath, i, false);
                anExample.composition = aComposition;
                anExample.Harvested = cropInformation.getItemBool("Harvested");

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
                theProducts.Add(anExample);
                cropInformation.PathNames.RemoveAt(cropInformation.PathNames.Count - 1);
                cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
                cropInformation.Identity.RemoveAt(cropInformation.Identity.Count - 1);
            }
        }
        if ((totProduction== 0)&&(name!="Bare soil"))
        {
            System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.WriteLine("Error - total potential production of a crop cannot be zero");
            file.WriteLine("Crop source = " + path);
            file.WriteLine("Crop name = " + name);
            file.Close();
            Console.WriteLine("Error - total potential production of a crop cannot be zero");
            Console.WriteLine("Crop source = " + path);
            Console.WriteLine("Crop name = " + name);
            if (GlobalVars.Instance.getPauseBeforeExit())
                Console.Read();
            Environment.Exit(0);
        }
       // Proportion_grazed = fedAtPastureProduction / totProduction;
        if (theProducts.Count > 1)
            PropSecondaryProduct = theProducts[1].GetExpectedYield() / theProducts[0].GetExpectedYield() ;
        else
            PropSecondaryProduct = 0;
    }

    public void getParameters(int cropIdentityNo, int zoneNR, string cropPath)
    {
        identity = cropIdentityNo;
        FileInformation cropParamFile = new FileInformation(GlobalVars.Instance.getParamFilePath(), GlobalVars.Instance.GeterrorFileName());
        string basePath = "AgroecologicalZone(" + zoneNR + ")";
        string tmpPath = basePath + ".NDepositionRate(-1)";
        cropParamFile.setPath(tmpPath);
        NDepositionRate = cropParamFile.getItemDouble("Value");
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
                System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                file.WriteLine("Error - could not find crop in parameter file");
                file.WriteLine("Crop source = " + cropPath + "(" + cropIdentityNo.ToString()+ ")");
                file.WriteLine("Crop name = " + name);
                file.Close();
                Console.WriteLine("Crop source = " + cropPath + "(" + cropIdentityNo.ToString() + ")");
                Console.WriteLine("Error - could not find crop in parameter file");
                Console.WriteLine("Crop name = " + name);
                if (GlobalVars.Instance.getPauseBeforeExit())
                    Console.Read();
                Environment.Exit(0);
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

            cropParamFile.Identity.RemoveAt(cropParamFile.Identity.Count - 1);
            cropParamFile.PathNames[cropParamFile.PathNames.Count - 1] = "HarvestMethod";
            cropParamFile.getSectionNumber(ref min, ref max);
            cropParamFile.Identity.Add(-1);
            for (int i = min; i <= max; i++)
            {
                cropParamFile.Identity[cropParamFile.PathNames.Count - 1] = i;
                string harvestMethodName = cropParamFile.getItemString("Name");
                int x;
                if (name == "Grass")
                    x = 0;
                if (harvestMethodName == harvestMethod)
                {
                    cropParamFile.PathNames.Add("PropAboveGroundResidues");
                    cropParamFile.Identity.Add(-1);
                    propAboveGroundResidues[i - 1] = cropParamFile.getItemDouble("Value");
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

    public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
    {
        writer.WriteStartElement("CropClass");

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

        writer.WriteStartElement("area");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("area");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(area);
        writer.WriteEndElement();
        writer.WriteEndElement();


        writer.WriteStartElement("theStartDate");
        theStartDate.Write(writer);
        writer.WriteEndElement();
        writer.WriteStartElement("theEndDate");
        theEndDate.Write(writer);
        writer.WriteEndElement();


        writer.WriteStartElement("isIrrigated");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("isIrrigated");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(isIrrigated);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Proportion_grazed");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("Proportion_grazed");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        //writer.WriteValue(Proportion_grazed);
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

        writer.WriteStartElement("fertiliserN2OEmission");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("fertiliserN2OEmission");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(fertiliserN2OEmission);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("cropResidueN2O");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("cropResidueN2O");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(cropResidueN2O);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("soilN2OEmission");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("soilN2OEmission");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(soilN2OEmission);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("area");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("area");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(area);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("NyieldMax");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("NyieldMax");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(NyieldMax);
        writer.WriteEndElement();
        writer.WriteEndElement();

        writer.WriteStartElement("Start_month");
        writer.WriteValue(GetStartMonth());
        writer.WriteEndElement();
        writer.WriteStartElement("End_month");
        writer.WriteValue(GetEndMonth());
        writer.WriteEndElement();

        writer.WriteStartElement("Proportion_grazed");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("Proportion_grazed");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        //writer.WriteValue(Proportion_grazed);
        writer.WriteEndElement();
        writer.WriteEndElement();

        for (int i = 0; i < theProducts.Count; i++)
        {
            theProducts[i].Write(writer, tabFile);
        }
        for (int i = 0; i < fertiliserApplied.Count; i++)
        {
            fertiliserApplied[i].Write(writer,tabFile);
        }
        for (int i = 0; i < manureApplied.Count; i++)
        {
            manureApplied[i].Write(writer,tabFile);
        }
       
        writer.WriteEndElement();
        
        tabFile.Write("CropClass".ToString()+"\n");
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
        tabFile.Write("area" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(area.ToString() + "\n");
     
        tabFile.Write("theStartDate"+"\n");
        theStartDate.Write(tabFile);
        
        tabFile.Write("theEndDate".ToString()+"\n");
        theEndDate.Write(tabFile);

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("isIrrigated" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(isIrrigated.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Proportion_grazed" + "\t");
        tabFile.Write("Value" + "\t");
        //tabFile.Write(Proportion_grazed.ToString() + "\n");

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
        tabFile.Write("fertiliserN2OEmission" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(fertiliserN2OEmission.ToString() + "\n");


        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("cropResidueN2O" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(cropResidueN2O.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("soilN2OEmission" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(soilN2OEmission.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("area" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(area.ToString() + "\n");



        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("NyieldMax" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(NyieldMax.ToString() + "\n");
        

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("GetStartMonth()" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(GetStartMonth().ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("GetEndMonth()" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(GetEndMonth().ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Proportion_grazed" + "\t");
        tabFile.Write("Value" + "\t");
        //tabFile.Write(Proportion_grazed.ToString() + "\n");
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
                harvestedC += theProducts[i].Grazed_yield * theProducts[i].composition.GetC_conc();
                harvestedN += theProducts[i].Grazed_yield * theProducts[i].composition.GetN_conc();
            }
            else
            {
                if (theProducts[i].Harvested == true)
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

    public void CalculateCropResidueC()
    {
        surfaceResidueC = 0;
        subsurfaceResidueC = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            double propHarvested = 0;
            if (theProducts[i].composition.GetfedAtPasture()) //grazed crop
            {
                propHarvested = theProducts[i].Grazed_yield / theProducts[i].Expected_yield;
                surfaceResidueC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc() * (propAboveGroundResidues[1] + (1 - propHarvested));
            }
            else //ungrazed crop
            {
                if (theProducts[i].Harvested == true)
                    propHarvested = 1.0;
                surfaceResidueC += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetC_conc() * (propAboveGroundResidues[0] + (1 - propHarvested));
            }
            subsurfaceResidueC += theProducts[i].GetExpectedYield() * (GetCconcBelowGroundResidues() * GetpropBelowGroundResidues());
        }
    }

    public void CalculateCropResidueN()
    {
        int x;
        surfaceResidueN = 0;
        subsurfaceResidueN = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            double propHarvested = 0;
            if (theProducts[i].composition.GetfedAtPasture())
            {
                //grazed part of crop
                propHarvested = theProducts[i].Grazed_yield / theProducts[i].Expected_yield;
                surfaceResidueN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * (propAboveGroundResidues[1] + (1 - propHarvested));
            }
            else //ungrazed part of crop
            {
                if (theProducts[i].Harvested == true)
                    propHarvested = 1.0;
                surfaceResidueN += theProducts[i].GetExpectedYield() * theProducts[i].composition.GetN_conc() * (propAboveGroundResidues[0] + (1 - propHarvested));
            }
            subsurfaceResidueN += theProducts[i].GetExpectedYield() * GetCconcBelowGroundResidues() * GetpropBelowGroundResidues()/CtoNBelowGroundResidues;
        }
        residueN = surfaceResidueN + subsurfaceResidueN;
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
                        urineNH3emission += urineNH3EmissionFactor * urineN;
                        faecalN += proportion * GlobalVars.Instance.grazedArray[j].faecesN / area;
                    }
                }
            }
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
        manureFOMCsurface = new double [duration];
        manureHUMCsurface = new double[duration];
        manureFOMCsubsurface = new double[duration];
        manureHUMCsubsurface = new double[duration];
        manureFOMNsurface = new double[duration];
        manureHUMNsurface = new double[duration];
        manureTAN = new double[duration];
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
                aManure = GlobalVars.Instance.theManureExchange.TakeManure(amountTotalN, storageType, speciesGroup);
                aManure.DivideManure(1 / area);
                manure anextraManure = new manure(aManure);
                if (theManureApplied == null)
                    theManureApplied = new List<manure>();
                theManureApplied.Add(anextraManure);
            }
            else
                aManure = theManureApplied[i];
            if (GlobalVars.Instance.getcurrentInventorySystem() == 1)
                NH3EmissionFactor = GlobalVars.Instance.getman_EFNH3_IPCC();
            else
            {
                int maxManure = 0;
                maxManure = GlobalVars.Instance.theZoneData.theFertManData.Count;
                for (int j = 0; j < maxManure; j++)
                {
                    int tmpType = GlobalVars.Instance.theZoneData.theFertManData[j].storageType;
                    int tmpSpecies = GlobalVars.Instance.theZoneData.theFertManData[j].speciesGroup;
                    if ((tmpType == storageType) && (tmpSpecies == speciesGroup))
                    {
                        double refEFNH3 = GlobalVars.Instance.theZoneData.theFertManData[j].fertManNH3EmissionFactor;
                        double HousingRefTemp = GlobalVars.Instance.theZoneData.theFertManData[j].fertManNH3EmissionFactorHousingRefTemperature;
                        double actualTemp = GlobalVars.Instance.Temperature(GlobalVars.Instance.theZoneData.GetaverageAirTemperature(),
                            0.0, manureApplied[i].GetdayOfApplication(), 0.0, GlobalVars.Instance.theZoneData.GetairtemperatureAmplitude(), GlobalVars.Instance.theZoneData.GetairtemperatureOffset());
                        double KHtheta = Math.Pow(10, -1.69 + 1447.7 / (actualTemp + GlobalVars.absoluteTemp));
                        double KHref = Math.Pow(10, -1.69 + 1447.7 / (HousingRefTemp + GlobalVars.absoluteTemp));
                        NH3EmissionFactor = (KHref / KHtheta) * refEFNH3;
                        break;
                    }
                }
            }

            double NH3ReductionFactor = 0;
            int maxApps = GlobalVars.Instance.theZoneData.themanureAppData.Count;
            for (int j = 0; j < maxApps; j++)
            {
                string tmpName = GlobalVars.Instance.theZoneData.themanureAppData[j].name;
                if (tmpName == applicType)
                {
                    NH3ReductionFactor = GlobalVars.Instance.theZoneData.themanureAppData[j].NH3EmissionReductionFactor;
                    break;
                }
            }

            double tmpNH3emission = NH3EmissionFactor * (1 - NH3ReductionFactor) * aManure.GetTAN();
            manureNH3emission += tmpNH3emission;
            aManure.SetTAN(aManure.GetTAN() - tmpNH3emission);
            if ((duration - manureApplied[i].GetdayOfApplication()) < 14)  //assumes that mineral N applied close to end of crop period is not used by this crop
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
                double leachingProportion = GlobalVars.Instance.theZoneData.GetPropLeaching(startDay, manureApplied[i].GetMonth_applied(), this);
                leachedManureTAN += leachingProportion * aManure.GetTAN();
            }
            manureTAN[manureApplied[i].GetdayOfApplication()] += aManure.GetTAN();
            manureFOMCsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GetdegC() + aManure.GetnonDegC();
            manureHUMCsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GethumicC();
            manureFOMNsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GetorganicN() - aManure.GethumicN();
            manureHUMNsurface[manureApplied[i].GetdayOfApplication() - 1] += aManure.GethumicN();
            totManureCapplied += aManure.GetdegC() + aManure.GetnonDegC() + aManure.GethumicC();
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
            cropInformation.setPath("fertiliser");
            int max = 0;
            int min = 99;

            cropInformation.getSectionNumber(ref min, ref max);
            cropInformation.Identity.Add(min);
            for (int j = min; j < max; j++)
            {
                cropInformation.Identity[0] = min;
                string fertManName = cropInformation.getItemString("Name");
                if (fertManName.CompareTo(fertilizerName) == 0)
                    break;
            }

            cropInformation.PathNames.Add("Cconcentration");
            cropInformation.Identity.Add(-1);
            double Cconc = cropInformation.getItemDouble("Value");
            cropInformation.PathNames[1] = "Nconcentration";
            double Nconc = cropInformation.getItemDouble("Value");
            double amount = Napplied/Nconc;
            fertiliserN += Napplied;
            fertiliserC += amount * Cconc;             

            double NH3EmissionFactor = 0;

/*            if (GlobalVars.Instance.getcurrentInventorySystem() == 1)
                NH3EmissionFactor = GlobalVars.Instance.getfert_EFNH3_IPCC();
            else
            {*/
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
            //}
                double tmpNH3emission = NH3EmissionFactor * Napplied;
            fertiliserNH3emission += tmpNH3emission;
            //Napplied now refers to N entering soil
            Napplied -= tmpNH3emission;
            if ((duration - fertiliserApplied[i].GetdayOfApplication()) < 14)  //assumes that mineral N applied close to end of crop period is not used by this crop
            {
                mineralNreserve = Napplied;
                Napplied = 0;
            }
            else
            {
                int startDay = 15;
                if (fertiliserApplied[i].GetMonth_applied() == GetStartMonth())
                {
                    if (GetStartDay() > 15)
                        startDay = GetStartDay();
                }
                double leachingProportion = GlobalVars.Instance.theZoneData.GetPropLeaching(startDay, fertiliserApplied[i].GetMonth_applied(), this);
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
        //need to update this section
        CalculateExcretaCInput();
        CalculateExcretaNInput();
        CalculateCFixed();
        CalculateHarvestedYields();
        CalculateCropResidueC();
        CalculateManureInput(lockit);
        CalculateCropResidueN();
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
        if ((soilMineralNSupply < NyieldMax)&&(NfixationFactor>=0))
            retVal = (NyieldMax - soilMineralNSupply) * NfixationFactor;
        return retVal;
    }

    //Jonas - this function is messy. It is called CalcAvailableN but also has carbon in it too. Needs to be tidied
    public void CalcAvailableN(double surplusMineralN, double thesoilNMineralisation)
    {
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
        N2Nemission = N2ONemission * soilN2Factor;
        double mineralNSupply = fertiliserN + totmanureTAN + urineN + soilNSupply + nAtm;
        double fixedN = GetNfixation(mineralNSupply);
        mineralNSupply += fixedN;
        double otherDenitrification = 0;
        double totalOtherN = urineN + soilNSupply + nAtm;
        if (totalOtherN>0)
            otherDenitrification = (N2ONemission + N2Nemission) * (urineN + soilNSupply + nAtm) / totalOtherN;
        leachedOtherN = proportionLeached * ((urineN + soilNSupply + nAtm) - (otherDenitrification + urineNH3emission));
        nitrateLeaching = leachedManureTAN + leachedOtherN + leachedFertiliserN;
        double mineralNLoss = fertiliserNH3emission + manureNH3emission + urineNH3emission + N2ONemission + N2Nemission + leachedManureTAN
            + leachedOtherN + leachedFertiliserN;
        mineralNavailable = mineralNSupply - mineralNLoss;
      Console.WriteLine("Min N " + mineralNSupply.ToString() + " soil min " + soilNMineralisation.ToString() + " surplus " + mineralNFromLastCrop.ToString()
            + " leaching " + nitrateLeaching.ToString() + " avail " + mineralNavailable.ToString());
    }

    public void getGrazedFeedItems()
    {
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].composition.GetfedAtPasture())
            {
                int feedCode = theProducts[i].composition.GetFeedCode();
                GlobalVars.Instance.grazedArray[feedCode].fieldDMgrazed += theProducts[i].Grazed_yield  * area;
            }
        }     
    }

    public void getAllFeedItems()
    {
        for (int i = 0; i < theProducts.Count; i++)
        {
            int feedCode = theProducts[i].composition.GetFeedCode();
            GlobalVars.Instance.allFeedAndProductsProduced[feedCode].AddExpectedYield(theProducts[i].Expected_yield
                * (1 - propAboveGroundResidues[1])*area);
            GlobalVars.Instance.allFeedAndProductsProduced[feedCode].composition.AddFeedItem(theProducts[i].composition,false);
        }
    }
    public bool expect()
    {
        int numberOfMatching = 0;
        for (int i = 0; i < theProducts.Count; i++)
        {
            if (theProducts[i].composition.GetfedAtPasture())
            {
                double diff_grazed = theProducts[i].GetModelled_yield() - theProducts[i].Grazed_yield;
                double rel_diff_grazed = diff_grazed / theProducts[i].Grazed_yield;
                double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
                
                if ((rel_diff_grazed<0.0)&&(Math.Abs(rel_diff_grazed) >tolerance))
                {
                    System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                    file.Write("Error - modelled production lower than expected production for grazed feed item");
                    string outstring1 = "Error; modelled yield has not been calculated";
                    //                string outstring2 = "Rotation name = " + RotationName;
                    string outstring3 = "Crop product = " + theProducts[i].composition.GetName();
                    string outstring4 = "Crop start year " + GetStartYear().ToString();

                    file.WriteLine(outstring1);
                    //              file.WriteLine(outstring2);
                    file.WriteLine(outstring3);
                    file.Write(outstring4);
                    file.Close();

                    Console.WriteLine(outstring1);
                    //            Console.WriteLine(outstring2);
                    Console.WriteLine(outstring3);
                    Console.Write(outstring4);
                    if (GlobalVars.Instance.getPauseBeforeExit())
                        Console.ReadKey();
                    Environment.Exit(0);
                }
            }
            double diff = theProducts[i].GetModelled_yield() - theProducts[i].GetExpectedYield();
            double relative_diff = System.Math.Abs(diff / theProducts[i].GetModelled_yield());
            double threshold =GlobalVars.Instance.getmaxToleratedErrorYield();
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
          System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.Write("Error - too many products in crop");
            file.Close();
            Console.Write("Error - too many products in crop");
            if (GlobalVars.Instance.getPauseBeforeExit())
                 Console.Read();
            Environment.Exit(0);
        }

        if (theProducts.Count > 0) // not bare soil
        {
            cropNuptake = CalculateCropNUptake();
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
                if (GlobalVars.Instance.GetIsIterating())
                {
                    for (int i = 0; i < theProducts.Count; i++)
                    {
                        double diff = theProducts[i].GetModelled_yield() - theProducts[i].GetExpectedYield();
                        if (diff < 0)
                            theProducts[i].SetExpectedYield(theProducts[i].GetExpectedYield() + diff / 2);
                        else
                            theProducts[i].SetExpectedYield(theProducts[i].GetExpectedYield() - diff / 2);
                    }
                }
                retVal = false;
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
                
                  System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                    file.WriteLine("Error - modelled yield is zero");
                    file.WriteLine("Rotation name = " + RotationName);
                    file.WriteLine("Crop product = " + theProducts[i].composition.GetName());
                    file.Close();
                    Console.WriteLine("Error - modelled yield is zero");
                    Console.WriteLine("Rotation name = " + RotationName);
                    Console.WriteLine("Crop product = " + theProducts[i].composition.GetName());
                    if (GlobalVars.Instance.getPauseBeforeExit())
                        Console.Read();
                    Environment.Exit(0);         
            }
            else
            {
                double expected = theProducts[i].Expected_yield;
                double modelled=theProducts[i].GetModelled_yield();
                if (Double.IsNaN(modelled)) //this should never happen..
                {
                    string outstring1 = "Error; modelled yield has not been calculated";
                    string outstring2 = "Rotation name = " + RotationName;
                    string outstring3 = "Crop product = " + theProducts[i].composition.GetName();
                    string outstring4 = "Crop start year " + GetStartYear().ToString();

                    System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                    file.WriteLine(outstring1);
                    file.WriteLine(outstring2);
                    file.WriteLine(outstring3);
                    file.Write(outstring4);
                    file.Close();

                    Console.WriteLine(outstring1);
                    Console.WriteLine(outstring2);
                    Console.WriteLine(outstring3);
                    Console.Write(outstring4);
                    if (GlobalVars.Instance.getPauseBeforeExit())
                        Console.ReadKey();
                    Environment.Exit(0);
                }

                double diff = (modelled - expected) / modelled;
                double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
                if (Math.Abs(diff) > tolerance)
                {
                    double errorPercent = 100 * diff;
                    string outstring1;
                    if (diff>0)
                        outstring1 = "Error; modelled yield exceeds expected yield by more than the permitted margin";
                    else
                        outstring1 = "Error; expected yield exceeds modelled yield by more than the permitted margin";
                    if (errorPercent < 0)
                        errorPercent *= -1.0;
                    string outstring2 = "Rotation name = " + RotationName;
                    string outstring3 = "Crop product = " + theProducts[i].composition.GetName();
                    string outstring4 = "Crop start year " + GetStartYear().ToString();
                    string outstring5 ="Percentage error = " + errorPercent.ToString("0.00") + "%";
                    string outstring6="Expected yield= " +expected.ToString() + " Modelled yield= " + modelled.ToString();
                    
                        System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                        file.WriteLine(outstring1);
                        file.WriteLine(outstring2);
                        file.WriteLine(outstring3);
                        file.WriteLine(outstring4);
                        file.WriteLine(outstring5);
                        file.Write(outstring6);
                        file.Close();
     
                        Console.WriteLine(outstring1);
                        Console.WriteLine(outstring2);
                        Console.WriteLine(outstring3);
                        Console.WriteLine(outstring4);
                        Console.WriteLine(outstring5);
                        Console.Write(outstring6);
                        Write(GlobalVars.Instance.theZoneData.GetXMLWriter(), GlobalVars.Instance.theZoneData.GetTabFileWriter());
                        GlobalVars.Instance.theZoneData.CloseOutputXML();

                        if (GlobalVars.Instance.getPauseBeforeExit())
                            Console.ReadKey();
                        Environment.Exit(0);
                    
                }
                else
                {
                    //accept the modelled yield as valid and add to allFeedAndProductsProduced
                    double productProcessingLossFactor = theProducts[i].composition.GetStoreProcessFactor();
                    if ((theProducts[i].Harvested == true)&&(!theProducts[i].composition.GetfedAtPasture()))
                    {
                        theProducts[i].composition.Setamount(theProducts[i].GetExpectedYield());
                        double originalC = theProducts[i].composition.Getamount() * theProducts[i].composition.GetC_conc();
                        double tempCLoss = productProcessingLossFactor * originalC;
                        theProducts[i].composition.SetC_conc((originalC - tempCLoss) / theProducts[i].composition.Getamount());
                        storageProcessingCLoss += tempCLoss;
                        double temp2 = theProducts[i].composition.GetC_conc() * theProducts[i].composition.Getamount();
                        theProducts[i].composition.Setamount(theProducts[i].composition.Getamount() * area);
                        double temp = theProducts[i].composition.Getamount() * theProducts[i].composition.GetN_conc();
                        GlobalVars.Instance.allFeedAndProductsProduced[theProducts[i].composition.GetFeedCode()].composition.AddFeedItem(theProducts[i].composition, false);
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
        double Cout = surfaceResidueC + subsurfaceResidueC + harvestedC;
        double Cbalance = Cinp - Cout;
        if (Cinp != 0) //not bare soil
        {
            double diff = Cbalance / Cinp;
            double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
            if (Math.Abs(diff) > tolerance)
            {

                double errorPercent = 100 * diff;
                System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                file.WriteLine("Error; Crop C balance error is more than the permitted margin");
                file.WriteLine("Crop name " + name);
                file.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                file.Close();
                Console.WriteLine("Error; Crop C balance error is more than the permitted margin");
                Console.WriteLine("Rotation name " + rotationName);
                Console.WriteLine("Crop no " + cropNo.ToString() + " Crop name " + name);
                Console.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");


                if (GlobalVars.Instance.getPauseBeforeExit())
                    Environment.Exit(0);

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
        double Ninp =cropNuptake;
        double Nout = surfaceResidueN + subsurfaceResidueN + harvestedN;
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
                    System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                    file.WriteLine("Error; Crop N balance error is more than the permitted margin");
                    file.WriteLine("Crop name " + name);
                    file.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                    file.Close();
                    Console.WriteLine("Error; Crop N balance error is more than the permitted margin");
                    Console.WriteLine("Rotation name " + rotationName.ToString());
                    Console.WriteLine("Crop no " + cropNo.ToString() + " Crop name " + name);
                    Console.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                    if (GlobalVars.Instance.getPauseBeforeExit())
                        Console.Read();
                    Environment.Exit(0);
                
            }
        }
    }

    public void CalcDuration()
    {
        duration = theEndDate.getLongTime() - theStartDate.getLongTime() + 1;
    }

}
