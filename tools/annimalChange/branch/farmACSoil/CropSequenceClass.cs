using System;
using System.Collections.Generic;
using System.Xml;
using simplesoilModel;

public class CropSequenceClass
{
    //inputs
    string name;
    string soilType;
    int FarmType;
    double area;
    //parameters 

    //other variables to be output

    //other
    int lengthOfSequence; //length of rotation in years
    double startsoilMineralN;
    ctool2 startSoil;
    string path;
    int identity;
    int repeats;
    List<CropClass> theCrops = new List<CropClass>();
    
    public void Setname(string aname) { name = aname; }
    public void Setidentity(int aValue) { identity = aValue; }
    public string Getname() { return name; }
    public List<CropClass> GettheCrops() { return theCrops; }
    public int Getidentity() { return identity; }
    double initialSoilC = 0;
    double initialSoilN = 0;
    double finalSoilC = 0;
    double finalSoilN = 0;
    double soilCO2Emission = 0;
    double oldsoilCO2Emission = 0;
    double Cleached = 0;
    double oldCleached = 0;
    double CinputToSoil = 0;
    double CdeltaSoil = 0;
    double Cbalance = 0;
    //double orgNleached = 0;
    double NinputToSoil = 0;
    double NdeltaSoil = 0;
    double Nbalance = 0;
    double Ninput;
    double NLost;
    ctool2 aModel;
    simpleSoil thesoilWaterModel; 
    simpleSoil startWaterModel;
    private double fieldCapacity = 0;
    private string Parens;
    public double GetinitialSoilC() { return initialSoilC; }
    public double GetCStored() { return aModel.GetCStored() * area; }
    public double GetNStored() { return aModel.GetNStored() * area ; }
    public double GetinitialSoilN() { return initialSoilN; }
    public double GetsoilCO2Emission() { return soilCO2Emission ; }
    public double GetCdeltaSoil(){return CdeltaSoil;}
    public double GetCleached() { return Cleached; }
    public double GettheNitrateLeaching()
    {
        return GettheNitrateLeaching(theCrops.Count);
    }

    public double GettheNitrateLeaching(int maxCrops) 
    {
        double Nleached = 0;
        for (int i = 0; i < maxCrops; i++)
            Nleached += theCrops[i].GetnitrateLeaching() * area;
        return Nleached;
    }
    public int Getrepeats() { return repeats; }
    public double GetstartsoilMineralN() { return startsoilMineralN * area; }
    public int GetlengthOfSequence() { return lengthOfSequence; }
    public double GetNinput() { return Ninput; }
    public double GetNlost() { return NLost; }

    public CropSequenceClass(string aPath, int aID, int zoneNr, int currentFarmType, string aParens)
    {
        Parens = aParens;
        path = aPath;
        identity = aID;
        FarmType = currentFarmType;
        FileInformation rotation = new FileInformation(GlobalVars.Instance.getFarmFilePath());
        path += "(" + identity.ToString() + ")";
        rotation.setPath(path);
        name = rotation.getItemString("NameOfRotation");
        area = rotation.getItemDouble("Area");
        soilType = rotation.getItemString("SoilType");
        string crop = path + ".Crop";
        rotation.setPath(crop);
        int min = 99; int max = 0;
        rotation.getSectionNumber(ref min, ref max);
        for (int i = min; i <= max; i++)
        {
            if (rotation.doesIDExist(i))
            {
                CropClass aCrop = new CropClass(crop, i, zoneNr, name);
                 theCrops.Add(aCrop);
            }
        }
        for (int i = 0; i < theCrops.Count; i++)
        {
            CropClass aCrop = theCrops[i];
            theCrops[i].CalcProportionLeached();
            if (i == theCrops.Count - 1) //true if this is the last (or only) crop
            {
                //check to ensure that the end of the crop sequence is exactly one or more years after it start
                long adjustedStartTime;
                long adjustedEndTimeThisCrop;
                if (theCrops.Count == 1) //only one crop
                {
                    adjustedStartTime = aCrop.getStartLongTime();
                    adjustedEndTimeThisCrop = aCrop.getEndLongTime();
                }
                else
                {
                    adjustedStartTime = theCrops[0].getStartLongTime();
                    adjustedEndTimeThisCrop = theCrops[i ].getEndLongTime();
                }
                long numDays = adjustedEndTimeThisCrop - adjustedStartTime +1;
                if (numDays<365)
                {
                    string messageString = ("Error - cropping sequence number " + identity.ToString() + " is less than one year");
                    GlobalVars.Instance.Error(messageString);
                }
                long mod = numDays % 365;
                if (Math.Abs(mod) > 1)
                {
                    string messageString=("Error - gap at end of cropping sequence number " + identity.ToString());
                    GlobalVars.Instance.Error(messageString);
                }
            }
        }
        lengthOfSequence = calculatelengthOfSequence();  //calculate length of sequence in years
        int cropsPerSequence = theCrops.Count;
        List<CropClass> CopyOfPlants = new List<CropClass>();
        int initialCropCount = theCrops.Count;
        for (int i = 0; i < initialCropCount; i++)
        {
            double duration = theCrops[i].CalcDuration();
            if (duration == 0)
            {
                string messageString = ("Error - crop number " + i.ToString() + " in sequence " + name);
                messageString += (": duration of crop cannot be zero");
                GlobalVars.Instance.Error(messageString);
            }
            if ((duration>366) && (duration % 365 != 0))
            {
                string messageString = ("Error - crop number " + i.ToString() + " in sequence " + name);
                messageString += (": crops lasting more than one year must last an exact number of years");
                GlobalVars.Instance.Error(messageString);
            }
            int durationInYears = (int) duration / 365;
            if (durationInYears > 1)     //need to clone for one or more years, if crop persists for more than one year
            {
                CropClass aCrop = theCrops[i];
                if ((aCrop.GetStartDay() == 1) && (aCrop.GetStartMonth() == 1))
                    aCrop.SetEndYear(aCrop.GetStartYear());
                else
                    aCrop.SetEndYear(aCrop.GetStartYear() + 1);
                aCrop.CalcDuration();
                aCrop.CalcProportionLeached();
                for (int j = 1; j < durationInYears; j++)
                {
                    {
                        aCrop = new CropClass(theCrops[i]);
                        aCrop.SetStartYear(j + theCrops[i].GetStartYear());
                        aCrop.SetEndYear(j + theCrops[i].GetEndYear());
                        aCrop.AlignDates(j);
                        theCrops.Add(aCrop);
                        //Console.WriteLine(i.ToString() + " " + aCrop.GetStartYear().ToString() + " " + aCrop.GetEndYear().ToString());
                    }
                }
            }
        }
       
        AdjustDates(theCrops[0].GetStartYear());    //this converts from calendar year to zero base e.g. 2010 to 0, 2011 to 1 etc
        lengthOfSequence = calculatelengthOfSequence();  //calculate length of sequence in years
        int length = GlobalVars.Instance.GetminimumTimePeriod();
        timeClass newStartDate = new timeClass(theCrops[theCrops.Count - 1].GettheEndDate());
        newStartDate.incrementOneDay();
        repeats = (int) Math.Ceiling(((double) length)/((double) lengthOfSequence));//number of times to repeat this sequence of crops
        for (int j = 0; j < repeats-1; j++)
        {
            for (int i = 0; i < theCrops.Count; i++)
            {
                CropClass newClass = new CropClass(theCrops[i]);
                int timeIncr = newStartDate.GetYear() - newClass.theStartDate.GetYear();
                newClass.theStartDate.SetDate(newStartDate.GetDay(), newStartDate.GetMonth(), newStartDate.GetYear());
                newClass.theEndDate.SetDate(newClass.theEndDate.GetDay(), newClass.theEndDate.GetMonth(), newClass.theEndDate.GetYear()
                    +timeIncr);
                newClass.AlignDates(timeIncr);
/*                Console.WriteLine("crop " + i + " syear " + newClass.GettheStartDate().GetYear() + " eyer " + newClass.GettheEndDate().GetYear());
                if (newClass.fertiliserApplied.Count > 0)
                    Console.WriteLine(i + " fert " + newClass.fertiliserApplied[0].GetDate().GetYear());*/
                CopyOfPlants.Add(newClass);
                newStartDate.SetDate(newClass.theEndDate.GetDay(), newClass.theEndDate.GetMonth(), newClass.theEndDate.GetYear());
                newStartDate.incrementOneDay();
            }
        }
        for (int i = 0; i < CopyOfPlants.Count; i++)//adjust crop start and end dates so they run sequentially
        {
            CropClass acrop = CopyOfPlants[i];
            theCrops.Add(acrop);
        }
        for (int i = 0; i < theCrops.Count; i++)
        {
            theCrops[i].UpdateParens(Parens + "_" + (i+1).ToString(),i);
        }
        for (int i = 0; i < theCrops.Count; i++)
        {
            CropClass aCrop = theCrops[i];
            //Console.WriteLine(i.ToString() + " " + aCrop.GetStartYear().ToString() + " " + aCrop.GetEndYear().ToString());
            aCrop.setArea(area);
        }
        lengthOfSequence = calculatelengthOfSequence();  //recalculate length of sequence in years
        thesoilWaterModel = new simpleSoil();
        getparameters(zoneNr);
        for (int i = 0; i < theCrops.Count; i++)
        {
            CropClass aCrop = theCrops[i];
            aCrop.SetlengthOfSequence(lengthOfSequence);
        }

        aModel = new ctool2(Parens+"_1");
        int soilIndex=-1;
        for (int i = 0; i < GlobalVars.Instance.theZoneData.thesoilData.Count; i++)
        {
            if(GlobalVars.Instance.theZoneData.thesoilData[i].name.CompareTo(soilType)==0)
                soilIndex=i;
        }
        if(soilIndex==-1)
        {
            string messageString=("Error - could not find soil type " + soilType + " in parameter file\n");
            messageString+=("Crop sequence name = " + name);
            GlobalVars.Instance.Error(messageString);
        }
        switch (FarmType)
        {
            case 1:
            case 2: 
            case 3: 
                break;
            default:
                    string messageString = ("Error - could not find C-Tool data for farm type " + FarmType + " in parameter file\n");
                    messageString += ("Crop sequence name = " + name);
                    GlobalVars.Instance.Error(messageString);
                    break;
        }
        double initialC = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].initialC;
        double initialFOMn = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].initialFOMn;
        double propHUM = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].propHUM;
        double propROM = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].propROM;

        double InitialCtoN = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].InitialCtoN;
        double InitialFOMCtoN = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType - 1].InitialFOMCtoN;
        double InitialFOMinput = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType - 1].InitialFOM;
        double ClayFraction = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].ClayFraction;
        double maxSoilDepth = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].maxSoilDepth;
        double dampingDepth = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].GetdampingDepth();
        initialFOMn = initialC * (1 - (propHUM + propROM)) / InitialFOMCtoN;
        double averageAirTemperature = GlobalVars.Instance.theZoneData.GetaverageAirTemperature();
        int offset = GlobalVars.Instance.theZoneData.GetairtemperatureOffset();
        double amplitude = GlobalVars.Instance.theZoneData.GetairtemperatureAmplitude();
        aModel.Initialisation(ClayFraction, offset, amplitude, maxSoilDepth, dampingDepth, initialC, initialFOMn, propHUM, propROM, 
            GlobalVars.Instance.getConstantFilePath(), GlobalVars.Instance.GeterrorFileName());
        double mineralNFromSpinup = 0;
        spinup(ref mineralNFromSpinup, InitialFOMinput, InitialFOMCtoN, averageAirTemperature, GlobalVars.Instance.theZoneData.GetdebugIt());
        theCrops[0].SetsoilNMineralisation(mineralNFromSpinup);
        double currentRootingDepth = 0;
        double currentLAI = 0;
        if (theCrops[0].Getpermanent())
        {
            currentRootingDepth = theCrops[0].GetMaximumRootingDepth();
            currentLAI = 3.0;
        }
        else
        {
            currentRootingDepth = 0;
            currentLAI = 0;
        }
        thesoilWaterModel.Initialise2(zoneNr, soilIndex, theCrops[0].GetMaximumRootingDepth(), currentRootingDepth, currentLAI);
        fieldCapacity = 1000.0 * thesoilWaterModel.getsoilFieldCapacity() * maxSoilDepth;
        for (int i = 0; i < theCrops.Count; i++)
            theCrops[i].CalculateClimate();
    }

    public void getparameters(int zoneNR)
    {
        double soilN2Factor = 0;
        bool gotit = false;
        int max = GlobalVars.Instance.theZoneData.thesoilData.Count;
        for (int i = 0; i < max; i++)
        {
            string soilname = GlobalVars.Instance.theZoneData.thesoilData[i].name;
            if (soilname == soilType)
            {
                soilN2Factor = GlobalVars.Instance.theZoneData.thesoilData[i].N2Factor;
                for (int j = 0; j < theCrops.Count; j++)
                {
                    CropClass aCrop = theCrops[j];
                    aCrop.setsoilN2Factor(soilN2Factor);
                }
                gotit = true;
                break;
            }
        }
        if (gotit == false)
        {
    
            string messageString=("Error - could not find soil type " + soilType + " in parameter file\n");
            messageString+=("Crop sequence name = " + name);
            GlobalVars.Instance.Error(messageString);
        }
    }
    //!Adjust the crop dates so that the first year is year 1 rather than calendar year
    private void AdjustDates(int firstYear)
    {
        for (int i = 0; i < theCrops.Count; i++)
            theCrops[i].AdjustDates(firstYear);
    }
    
    public double getArea()    { return area;}
    
    public double getCFixed()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += theCrops[i].getCFixed() * area;
        }

        return result;
    }
    public double getCHarvested()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += theCrops[i].GetharvestedC() * area;
        }

        return result;
    }
    public double getCropResidueCarbon()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += (theCrops[i].GetsurfaceResidueC() + theCrops[i].GetsurfaceResidueC()) * area;
        }
        return result;
    }

    public double getBurntResidueCO2C()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += theCrops[i].GetburningCO2C() * area;
        }
        return result;
    }

    public double getBurntResidueCOC()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += theCrops[i].GetburningCOC() * area;
        }
        return result;
    }

    public double getBurntResidueBlackC()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += theCrops[i].GetburningBlackC() * area;
        }
        return result;
    }

    public double getBurntResidueN()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += theCrops[i].GetburntResidueN() * area;
        }
        return result;
    }

    public double getBurntResidueN2ON()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += theCrops[i].GetburningN2ON() * area;
        }
        return result;
    }

    public double getBurntResidueNH3N()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += theCrops[i].GetburningNH3N() * area;
        }
        return result;
    }

    public double getBurntResidueOtherN()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += theCrops[i].GetburningOtherN() * area;
        }
        return result;
    }

    public double getBurntResidueNOxN()
    {
        double result = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            result += theCrops[i].GetburningNOxN() * area;
        }
        return result;
    }

    public double getProcessStorageLossCarbon()
    {
        double retVal = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            retVal += theCrops[i].GetstorageProcessingCLoss() * area;
        }
        return retVal;
    }

    public double getProcessStorageLossNitrogen()
    {
        double retVal = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            retVal += theCrops[i].GetstorageProcessingNLoss() * area;
        }
        return retVal;
    }

    public double GetDMYield()
    {
        double retVal = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            retVal += theCrops[i].GetDMYield()* area;
        }
        return retVal;
    }

    public double GetCropNinputToSoil()
    {
        double retVal = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            retVal += theCrops[i].GetresidueN() * area;
        }
        return retVal;
    }

    public double GetFertiliserNapplied()
    {
        return GetFertiliserNapplied(theCrops.Count);
    }

    public double GetFertiliserNapplied(int maxCrops)
    {
        double retVal = 0;
        for (int i = 0; i < maxCrops; i++)
            retVal += theCrops[i].GetFertiliserNapplied() * area;
        return retVal;
    }

    public double GetManureNapplied()
    {
        return GetManureNapplied(theCrops.Count);
    }
    
    public double GetManureNapplied(int maxCrops)
    {
        double retVal = 0;
        for (int i = 0; i < maxCrops; i++)
            retVal += theCrops[i].GetManureNapplied() * area;
        return retVal;
    }

    public double GetfertiliserN2ONEmissions()
    {
        return GetfertiliserN2ONEmissions(theCrops.Count);
    }

    public double GetfertiliserN2ONEmissions(int maxCrops) 
    {
        double fertiliserN2OEmission = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            fertiliserN2OEmission += theCrops[i].GetfertiliserN2ONEmission() * area;
        }
        return fertiliserN2OEmission; 
    }

    public double GetmanureN2ONEmissions()
    {
        return GetmanureN2ONEmissions(theCrops.Count);
    }

    public double GetmanureN2ONEmissions(int maxCrops) 
    {
        double manureN2OEmission = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            manureN2OEmission += theCrops[i].GetmanureN2ONEmission() * area;
        }
        return manureN2OEmission; 
    }

    public double GetcropResidueN2ON()
    {
        return GetcropResidueN2ON(theCrops.Count);
    }

    public double GetcropResidueN2ON(int maxCrops) 
    {
        double cropResidueN2O = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            cropResidueN2O += theCrops[i].GetcropResidueN2ON() * area;
        }
        return cropResidueN2O; 
    }

    public double GetsoilN2ONEmissions()
    {
        return GetsoilN2ONEmissions(theCrops.Count);
    }

    public double GetsoilN2ONEmissions(int maxCrops)
    {
        double soilN2OEmission = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            soilN2OEmission += theCrops[i].GetsoilN2ONEmission() * area;
        }
        return soilN2OEmission;
    }
    public double GetsoilN2NEmissions()
    {
        return GetsoilN2NEmissions(theCrops.Count);
    }

    public double GetsoilN2NEmissions(int maxCrops)
    {
        double soilN2Emission = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            soilN2Emission += theCrops[i].GetN2Nemission() * area;
        }
        return soilN2Emission;
    }

    public double GetNH3NmanureEmissions()
    {
        return GetNH3NmanureEmissions(theCrops.Count);
    }

    public double GetNH3NmanureEmissions(int maxCrops)
    {
        double manureNH3Emissions = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            manureNH3Emissions += theCrops[i].GetmanureNH3Nemission() * area;
        }
        return manureNH3Emissions;
    }

    public double GetfertiliserNH3Nemissions()
    {
        return GetfertiliserNH3Nemissions(theCrops.Count);
    }

    public double GetfertiliserNH3Nemissions(int maxCrops)
    {
        double fertiliserNH3emissions = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            fertiliserNH3emissions += theCrops[i].GetfertiliserNH3Nemission() * area;
        }
        return fertiliserNH3emissions;
    }
    public double GeturineNH3emissions()
    {
        return GeturineNH3emissions(theCrops.Count);
    }

    public double GeturineNH3emissions(int maxCrops)
    {
        double urineNH3emissions = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            urineNH3emissions += theCrops[i].GeturineNH3emission() * area;
        }
        return urineNH3emissions;
    }


    public void DoCropInputs(bool lockit)
    {
        for (int i = 0; i < theCrops.Count; i++)
            theCrops[i].DoCropInputs(lockit);
           
   }
    public int calculatelengthOfSequence()
    {
        long firstDate = 999999999;
        long lastDate = -99999999;
        for (int i = 0; i < theCrops.Count; i++)
        {
            CropClass acrop = theCrops[i];
            long astart = acrop.getStartLongTime();
            if (astart < firstDate)
                firstDate = astart;
            long anend = acrop.getEndLongTime();
            if (anend > lastDate)
                lastDate = anend;
           // Console.WriteLine(i.ToString() +" Crop start " + acrop.GetStartYear() + " end " + acrop.GetEndYear());
        }
        long period = lastDate - firstDate;
        double temp = ((double)period) / ((double)365);
        int retVal = (int)Math.Ceiling(temp);
        return retVal;   
    }
    public void Write()
    {
        GlobalVars.Instance.writeStartTab("CropSequenceClass");

        GlobalVars.Instance.writeInformationToFiles("name", " Name", "-", name, Parens);
        GlobalVars.Instance.writeInformationToFiles("identity", "Identity", "-", identity, Parens);
        GlobalVars.Instance.writeInformationToFiles("soilType", "Soil type", "-", soilType, Parens);
        GlobalVars.Instance.writeInformationToFiles("area", "area", "-", area, Parens);

        for (int i = 0; i < theCrops.Count; i++)
        {
            theCrops[i].Write();
        }

        int year =calculatelengthOfSequence();
        
        double tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].getCFixed();
        }
        GlobalVars.Instance.writeInformationToFiles("CFixed", "C fixed", "kgC/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetsurfaceResidueC();
        }
        GlobalVars.Instance.writeInformationToFiles("surfaceResidueC", "C in surface residues", "kgC/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetsubsurfaceResidueC();
        }
        GlobalVars.Instance.writeInformationToFiles("subsurfaceResidueC", "C in subsurface residues", "kgC/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetUrineC();
        }
        GlobalVars.Instance.writeInformationToFiles("urineC", "C in urine", "kgC/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetfaecalC();
        }
        GlobalVars.Instance.writeInformationToFiles("faecalC", "C in faeces", "kgC/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetstorageProcessingCLoss();
        }
        GlobalVars.Instance.writeInformationToFiles("storageProcessingCLoss", "C lost during processing or storage", "kgC/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetFertiliserC();
        }
        GlobalVars.Instance.writeInformationToFiles("fertiliserC", "Emission of CO2 from fertiliser", "kgC/ha/yr", tmp / year, Parens); tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetharvestedC();
        }
        GlobalVars.Instance.writeInformationToFiles("harvestedC", "Harvested C", "kgC/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetburntResidueC();
        }
        GlobalVars.Instance.writeInformationToFiles("burntResidueC", "C in burned crop residues", "kg/ha", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetUnutilisedGrazableC();
        }
        GlobalVars.Instance.writeInformationToFiles("unutilisedGrazableC", "C in unutilised grazable DM", "kg/ha", tmp / year, Parens);
        //N budget
        double Ninput = 0;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].getNFix();
        }
        GlobalVars.Instance.writeInformationToFiles("Nfixed", "N fixed", "kgN/ha/yr", tmp / year, Parens);
        Ninput += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].getnAtm();
        }
        GlobalVars.Instance.writeInformationToFiles("nAtm", "N from atmospheric deposition", "kgN/ha/yr", tmp / year, Parens);
        Ninput += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetFertiliserNapplied();
        }
        GlobalVars.Instance.writeInformationToFiles("fertiliserNinput", "Input of N in fertiliser", "kgN/ha/yr", tmp / year, Parens);
        Ninput += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetUrineN();
        }
        GlobalVars.Instance.writeInformationToFiles("urineN", "Urine N", "kgN/ha/yr", tmp / year, Parens);
        Ninput += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetfaecalN();
        }
        GlobalVars.Instance.writeInformationToFiles("faecalN", "Faecal N", "kgN/ha/yr", tmp / year, Parens);
        Ninput += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetexcretaNInput();
        }
        GlobalVars.Instance.writeInformationToFiles("excretaNInput", "Input of N in excreta", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GettotalManureNApplied();
        }
        GlobalVars.Instance.writeInformationToFiles("totalManureNApplied", "Total N applied in manure", "kgN/ha/yr", tmp / year, Parens);
        Ninput += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetharvestedN();
        }
        GlobalVars.Instance.writeInformationToFiles("harvestedN", "N harvested (N yield)", "kgN/ha/yr", tmp / year, Parens);
        /*        
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
                {
                    tmp += theCrops[i].getSurfaceResidueDM();
                }
                GlobalVars.Instance.writeInformationToFiles("surfaceResidueDM", "Surface residue dry matter", "kg/ha", tmp / year, Parens);*/
        double Nlost = 0;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetmanureNH3Nemission();
        }
        GlobalVars.Instance.writeInformationToFiles("manureNH3emission", "NH3-N from manure application", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetfertiliserNH3Nemission();
        }
        GlobalVars.Instance.writeInformationToFiles("fertiliserNH3emission", "NH3-N from fertiliser application", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GeturineNH3emission();
        }
        GlobalVars.Instance.writeInformationToFiles("urineNH3emission", "NH3-N from urine deposition", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetstorageProcessingNLoss();
        }
        GlobalVars.Instance.writeInformationToFiles("storageProcessingNLoss", "N2 emission from mineralised N", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetN2ONemission();
        }
        GlobalVars.Instance.writeInformationToFiles("N2ONemission", "N2O emission", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetfertiliserN2ONEmission();
        }
        GlobalVars.Instance.writeInformationToFiles("fertiliserN2OEmission", "N2O emission from fertiliser", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].getCropResidueN2O();
        }
        GlobalVars.Instance.writeInformationToFiles("cropResidueN2O", "N2O emission from crop residues", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetsoilN2ONEmission();
        }
        GlobalVars.Instance.writeInformationToFiles("soilN2OEmission", "N2O emission from mineralised N", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetN2Nemission();
        }
        GlobalVars.Instance.writeInformationToFiles("N2Nemission", "N2 emission", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetburningN2ON();
        }
        GlobalVars.Instance.writeInformationToFiles("burningN2ON", "N2O emission from burned crop residues", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetburningNH3N();
        }
        GlobalVars.Instance.writeInformationToFiles("burningNH3N", "NH3 emission from burned crop residues", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetburningNOxN();
        }
        GlobalVars.Instance.writeInformationToFiles("burningNOxN", "NOx emission from burned crop residues", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetburningOtherN();
        }
        GlobalVars.Instance.writeInformationToFiles("burningOtherN", "N2 emission from burned crop residues", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetOrganicNLeached();
        }
        GlobalVars.Instance.writeInformationToFiles("OrganicNLeached", "Leached organic N", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
/*        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetmineralNToNextCrop();
        }
        GlobalVars.Instance.writeInformationToFiles("mineralNToNextCrop", "Mineral N to next crop", "kgN/ha/yr", tmp / year, Parens);
 */
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetfertiliserN2ONEmission();
        }
        GlobalVars.Instance.writeInformationToFiles("fertiliserN2OEmission", "N2O emission from fertiliser N", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetmanureN2ONEmission();
        }
        GlobalVars.Instance.writeInformationToFiles("manureN2OEmission", "N2O emission from manure N", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].getCropResidueN2O();
        }
        GlobalVars.Instance.writeInformationToFiles("cropResidueN2O", "N2O emission from crop residue N", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetsoilN2ONEmission();
        }
        GlobalVars.Instance.writeInformationToFiles("soilN2OEmission", "N2O emission from mineralised N", "kgN/ha/yr", tmp / year, Parens);
/*        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetproportionLeached();
        }
        GlobalVars.Instance.writeInformationToFiles("proportionLeached", "Proportion of other N leached", "kgN/ha/yr", tmp / year, Parens);*/
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            tmp += theCrops[i].GetnitrateLeaching();
        }
        GlobalVars.Instance.writeInformationToFiles("nitrateLeaching", "Nitrate N leaching", "kgN/ha/yr", tmp / year, Parens);
        Nlost += tmp;
        double nitrateLeaching = tmp;

        double DeltaSoilN = (finalSoilN - initialSoilN)/area;

        GlobalVars.Instance.writeInformationToFiles("DeltaSoilN", "Change in soil N", "kgN/ha/yr", DeltaSoilN/ year, Parens);
        GlobalVars.Instance.writeInformationToFiles("Ninput", "N input", "kgN/ha/yr", Ninput / year, Parens);
        GlobalVars.Instance.writeInformationToFiles("NLost", "N lost", "kgN/ha/yr", Nlost / year, Parens);

        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
            tmp += theCrops[i].GetmineralNavailable();
        GlobalVars.Instance.writeInformationToFiles("mineralNavailable", "Mineral N available", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
            tmp += theCrops[i].GetUnutilisedGrazableC();
        GlobalVars.Instance.writeInformationToFiles("unutilisedGrazableN", "N in unutilised grazable DM", "kg/ha", tmp / year, Parens);

        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
            tmp += theCrops[i].getMineralNFromLastCrop();
        GlobalVars.Instance.writeInformationToFiles("mineralNFromLastCrop", "Mineral N from last crop", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
            tmp += theCrops[i].GetCropNuptake();
        GlobalVars.Instance.writeInformationToFiles("cropNuptake", "Crop N uptake", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
            tmp += theCrops[i].GetsurfaceResidueN();
        GlobalVars.Instance.writeInformationToFiles("surfaceResidueN", "N in surface residues", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
            tmp += theCrops[i].GetsubsurfaceResidueN();
        GlobalVars.Instance.writeInformationToFiles("subsurfaceResidueN", "N in subsurface residues", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
            tmp += theCrops[i].GetSoilNMineralisation();
        GlobalVars.Instance.writeInformationToFiles("soilNMineralisation", "Soil mineralised N", "kgN/ha/yr", tmp / year, Parens);
        tmp = 0;
        for (int i = 0; i < theCrops.Count; i++)
            tmp += theCrops[i].GetcumulativePrecipitation();
        GlobalVars.Instance.writeInformationToFiles("precipitation", "precipitation", "mm/yr", tmp / year, Parens);
        for (int i = 0; i < theCrops.Count; i++)
            tmp += theCrops[i].GetcumulativeTranspiration();
        GlobalVars.Instance.writeInformationToFiles("evapotranspiration", "evapotranspiration", "mm/yr", tmp / year, Parens);
        for (int i = 0; i < theCrops.Count; i++)
            tmp += theCrops[i].GetcumulativeDrainage();
        GlobalVars.Instance.writeInformationToFiles("drainage", "drainage", "mm/yr", tmp / year, Parens);
        tmp = 100 * nitrateLeaching / tmp;
        GlobalVars.Instance.writeInformationToFiles("nitrateNconc", "nitrate N concentration", "mg/litre", tmp / year, Parens);
        GlobalVars.Instance.writeEndTab();

    }
    public double getNFix()
    {
        return getNFix(theCrops.Count);
    }

    public double getNFix(int maxCrops)
    {
        double nFix = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            nFix += theCrops[i].getNFix() * area;
        }
        return nFix;
    }

    public double getNAtm()
    {
        return getNAtm(theCrops.Count);
    }

    public double getNAtm(int maxCrops)
    {
        double nAtm = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            nAtm += theCrops[i].getnAtm() * area;
        }
        return nAtm;
    }

    public double getManureNapplied()
    {
        return getManureNapplied(theCrops.Count);
    }

    public double getManureNapplied(int maxCrops)
    {
        double manureN = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            for (int j = 0; j < theCrops[i].GetmanureApplied().Count; j++)
                manureN += theCrops[i].GetmanureApplied()[j].getNamount() * area;
        }
        return manureN;
    }

    public double getFertiliserNapplied()
    {
        return getFertiliserNapplied(theCrops.Count);
    }

    public double getFertiliserNapplied(int maxCrops)
    {
        double fertiliserN = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            for (int j = 0; j < theCrops[i].GetfertiliserApplied().Count; j++)
                fertiliserN += theCrops[i].GetfertiliserApplied()[j].getNamount() * area;
        }
        return fertiliserN;
    }

    public double getNharvested()
    {
        return getNharvested(theCrops.Count);
    }

    public double getNharvested(int maxCrops)
    {
        double Nharvested = 0;
        for (int i = 0; i < theCrops.Count; i++)
            Nharvested += theCrops[i].GetharvestedN() * area;
        return Nharvested;
    }

    public double GetResidualSoilMineralN()
    {
        return GetResidualSoilMineralN(theCrops.Count);
    }

    public double GetResidualSoilMineralN(int maxCrops)
    {
        double retVal=0;
        retVal = theCrops[maxCrops - 1].GetmineralNToNextCrop() * area;
        return retVal;
    }
   public void getGrazedFeedItems()
    {
        for (int i = 0; i < theCrops.Count; i++)
            theCrops[i].getGrazedFeedItems();
    }

    public int CheckYields()
    {
        int retVal = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            retVal = theCrops[i].CheckYields(name);
            if (retVal > 0)
                break;
        }
        return retVal;
    }
    public void CalcModelledYield()
    {
        double surplusMineralN = startsoilMineralN;
        double CropCinputToSoil = 0;
        double CropNinputToSoil = 0;
        double CropsoilCO2Emission = 0;
        double CropCleached = 0;
        double ManCinputToSoil = 0;
        double ManNinputToSoil = 0;
        double mineralisedN = 0;
        startWaterModel = new simpleSoil(thesoilWaterModel);
        for (int i = 0; i < theCrops.Count; i++)
            theCrops[i].Calcwaterlimited_yield(0);//sets expected yield to potential yield
        CalculateWater();  //this runs the soil water model for this crop
        for (int i = 0; i < theCrops.Count; i++)
        {
            double meanDroughtFactor = 0;
            double meanTemperature = GlobalVars.Instance.theZoneData.GetMeanTemperature(theCrops[i]);
            meanDroughtFactor = GlobalVars.Instance.theZoneData.GetMeanDroughtIndex(theCrops[i]);
            theCrops[i].Calcwaterlimited_yield(meanDroughtFactor);
            
            bool doneOnce = false;
            startSoil.CopyCTool(aModel);  //take copy, in case we need to reset C-Tool to start of crop
//            startWaterModel.reload_simpleSoil(thesoilWaterModel); //take copy, in case we need to reset soil water model to start of crop
            oldsoilCO2Emission = soilCO2Emission;
            oldCleached = Cleached;
            double oldsurplusMineralN = surplusMineralN;
            bool gotit = false;
            int count = 0;
            while ((gotit == false) || (doneOnce == false))//iterate for each crop, until the crop yield stabilises (note special treatment of grazed crops)
            {
                count++;
                if (count > GlobalVars.Instance.GetmaximumIterations())
                {
                    string messageString = "Error; Crop production iterations exceeds maximum\n";
                    messageString += "Crop sequence name = " + name + "\n";
                    messageString += "Crop name = " + name + " crop number " + i.ToString();
                    Write();
                    GlobalVars.Instance.Error(messageString);
                }

                Console.WriteLine("seq " + identity.ToString() + "crop " + i.ToString() + " loop " + count.ToString());
                if (doneOnce)
                {
                    resetC_Tool();
                    surplusMineralN = oldsurplusMineralN;
                    theCrops[i].DoCropInputs(true);
                }
                else
                {
                    theCrops[i].DoCropInputs(false);
                }
                RunCropCTool(false, i, meanTemperature, meanDroughtFactor, ref CropCinputToSoil, ref CropNinputToSoil, ref ManCinputToSoil, ref ManNinputToSoil, 
                    ref CropsoilCO2Emission, ref CropCleached, ref mineralisedN);
                theCrops[i].CalcAvailableN(ref surplusMineralN, mineralisedN);
                gotit = theCrops[i].CalcModelledYield(ref surplusMineralN, true);
                doneOnce = true;
            }
            count = 0;
            theCrops[i].getCFixed();
            theCrops[i].DoCropInputs(true);
            resetC_Tool();
            RunCropCTool(false, i, meanTemperature, meanDroughtFactor, ref CropCinputToSoil, ref CropNinputToSoil, ref ManCinputToSoil, ref ManNinputToSoil, ref CropsoilCO2Emission, ref CropCleached, ref mineralisedN);
            CinputToSoil += (CropCinputToSoil + ManCinputToSoil) * area;
            NinputToSoil += (CropNinputToSoil + ManNinputToSoil) * area;
            soilCO2Emission += CropsoilCO2Emission * area;
            Cleached += CropCleached * area;
            CheckRotationCBalance(i + 1);
            CheckRotationNBalance(i + 1);
        }
    }

    public void CalculateWater()
    {
        for (int i = 0; i < theCrops.Count; i++)
        {
            double cumevap = 0;
            double cumtranspire = 0;
            double irrigationThreshold = 0.1;
            double irrigationMinimum = 20.0;
            timeClass clockit = new timeClass(theCrops[i].GettheStartDate());
            int k = 0;
            double cropDuration = theCrops[i].getDuration();
            while (k<cropDuration)
            {
                double LAI = theCrops[i].CalculateLAI(k);
                double rootingDepth = theCrops[i].CalculateRootingDepth(k);
                double precip = theCrops[i].Getprecipitation(k);
                double potevapotrans = theCrops[i].GetpotentialEvapoTrans(k);
                double airTemp = theCrops[i].Gettemperature(k);
                double SMD = thesoilWaterModel.getSMD(rootingDepth, rootingDepth);
                if (SMD > 0)
                    Console.Write("");
                double fieldCapacityToRootingDepth = thesoilWaterModel.getFieldCapacity(rootingDepth, rootingDepth);
                double droughtFactor = SMD / fieldCapacityToRootingDepth;
                theCrops[i].SetdroughtFactor(k, droughtFactor);
                if ((theCrops[i].GetisIrrigated()) && (droughtFactor > irrigationThreshold) && ((SMD-precip) > irrigationMinimum))
                    thesoilWaterModel.setirrigate(true);
                else
                    thesoilWaterModel.setirrigate(false);
                if (k == 100)
                    Console.Write(" ");

                thesoilWaterModel.dailyRoutine(potevapotrans, precip, airTemp, LAI, rootingDepth);

                double evap = thesoilWaterModel.GetsnowEvap() + thesoilWaterModel.Getevap();
                double transpire = thesoilWaterModel.Gettranspire();
                double drainage = thesoilWaterModel.Getdrainage();
                double evapoTrans = evap + transpire;
                SMD = thesoilWaterModel.getSMD(rootingDepth, rootingDepth);
                fieldCapacityToRootingDepth = thesoilWaterModel.getFieldCapacity(rootingDepth, rootingDepth);
                double irrigation = thesoilWaterModel.GetirrigationWater();
                double waterInSoil = thesoilWaterModel.getwaterInSystem();
                theCrops[i].SetsoilWater(k,waterInSoil);
                droughtFactor = SMD / fieldCapacityToRootingDepth;
                theCrops[i].SetdroughtFactor(k, droughtFactor);
                
                cumevap += evap;
                cumtranspire += transpire;
                theCrops[i].Setdrainage(k, drainage);
                theCrops[i].Setevaporation(k, evap);
                theCrops[i].Settranspire(k, transpire);
                theCrops[i].Setirrigation(k, irrigation);
                Console.WriteLine("Crop " + i + " k " + k + " precip " + precip.ToString("F3") + " evap " + evap.ToString("F3")
                    + " drought " + (SMD / fieldCapacity).ToString("F3") + " drain " + drainage.ToString("F3") 
                    + " transpire " + transpire.ToString("F3") + " irr " + irrigation.ToString("F3"));
                k++;
                clockit.incrementOneDay();
            }
            theCrops[i].CalculateDroughtFactor();
            //Console.WriteLine("crop " + i + " cumevap " + cumevap + " cumtranspire " + cumtranspire);
        }
    }
    public bool CheckRotationCBalance()
    {
        return CheckRotationCBalance(theCrops.Count);
    }

    public bool CheckRotationCBalance(int maxCrops)
    {
        bool retVal = true;
        double harvestedC = 0;
        double fixedC = 0;
        double manureC = 0;
        double faecalC=0;
        double urineC = 0;
        double burntC = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            retVal = theCrops[i].CheckCropCBalance(name,i+1);
            if (retVal == false)
                break;
            harvestedC += theCrops[i].GetharvestedC() * area;
            fixedC += theCrops[i].getCFixed() * area;
            manureC += theCrops[i].GetManureC() * area;
            faecalC += theCrops[i].GetfaecalC() * area;
            urineC += theCrops[i].GetUrineC() * area;
            burntC += theCrops[i].GetburntResidueC() * area;
        }
        finalSoilC = GetCStored();
        CdeltaSoil =  finalSoilC - initialSoilC;
        double diff = CinputToSoil- (soilCO2Emission + Cleached+ CdeltaSoil );
        double errorPercent = 100 * diff / initialSoilC;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
         {
             string messageString = "Error; soil C balance is greater than the permitted margin\n";
             messageString += "Crop sequence name = " + name+"\n";
             messageString += "Percentage error = " + errorPercent.ToString("0.00") + "%";
             Write();
             GlobalVars.Instance.Error(messageString);
         }
         double Charvested = getCHarvested();
         double Cfixed = getCFixed();
         Cbalance=(Cfixed + manureC + faecalC + urineC - (soilCO2Emission + Cleached + CdeltaSoil + Charvested + burntC));
         diff =  Cbalance/ initialSoilC;
         errorPercent = 100 * diff;
         if (Math.Abs(diff) > tolerance) 
         {
             string messageString = "Error; crop sequence C balance is greater than the permitted margin" + "\n"; ;
             messageString += "Crop sequence name = " + name + "\n"; ;
             messageString += "Percentage error = " + errorPercent.ToString("0.00") + "%";
             Write();
             GlobalVars.Instance.Error(messageString);
         }
         return retVal; 
    }

    public void CheckRotationNBalance()
    {
        CheckRotationNBalance(theCrops.Count);
    }

    public void CheckRotationNBalance(int maxCrops)
    {
        for (int i = 0; i < maxCrops; i++)
            theCrops[i].CheckCropNBalance(name,i+1);
        double NAtm = getNAtm(maxCrops);
        double ManureNapplied = GetManureNapplied(maxCrops);
        double excretaNInput = GetexcretaNInput(maxCrops);
        double FertiliserNapplied = GetFertiliserNapplied(maxCrops);
        double fixedN = getNFix();
        Ninput = NAtm + ManureNapplied + excretaNInput + FertiliserNapplied + fixedN + startsoilMineralN * area;
        finalSoilN = GetNStored();
        NdeltaSoil = finalSoilN - initialSoilN;
        double residualMineralN = GetResidualSoilMineralN(maxCrops);
        double NH3NmanureEmissions = GetNH3NmanureEmissions(maxCrops);
        double fertiliserNH3Nemissions = GetfertiliserNH3Nemissions(maxCrops);
        double urineNH3emissions = GeturineNH3emissions(maxCrops);
        double N2ONEmission = GetN2ONemission(maxCrops);
        double N2NEmission = GetN2NEmission(maxCrops);
        double orgNleached = GetOrganicNLeached(maxCrops);
        double nitrateLeaching = GettheNitrateLeaching(maxCrops);
        double harvestedN = getNharvested(maxCrops);
        double burntN = getBurntResidueN();
        NLost = NH3NmanureEmissions + fertiliserNH3Nemissions + urineNH3emissions + N2ONEmission + N2NEmission + burntN
                + orgNleached + nitrateLeaching;
        Nbalance = Ninput - (NLost + harvestedN + NdeltaSoil + residualMineralN);
        double diff = Nbalance / Ninput;
        double errorPercent = 100 * diff;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
            string messageString = "Error; crop sequence N balance is greater than the permitted margin\n";
            messageString += "Crop sequence name = " + name+"\n";
            messageString += "Percentage error = " + errorPercent.ToString("0.00") + "%";
            Write();
            GlobalVars.Instance.Error(messageString);
        }
    }


    public double GettotalManureNApplied()
    {
        return GettotalManureNApplied(theCrops.Count);
    }

    public double GettotalManureNApplied(int maxCrops)
    {
        double retVal = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            retVal += theCrops[i].GettotalManureNApplied() * area;
        }
        return retVal;
    }

    public double GetexcretaNInput()
    {
        return GetexcretaNInput(theCrops.Count);
    }

    public double GetexcretaNInput(int maxCrops)
    {
        double retVal = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            retVal += theCrops[i].GetexcretaNInput() * area;
        }
        return retVal;
    }

    public double GetAtmosphericDep()
    {
        return GetAtmosphericDep( theCrops.Count);
    }

    public double GetAtmosphericDep(int maxCrops)
    {
        double retVal = 0;
        for (int i = 0; i <maxCrops; i++)
        {
            retVal += theCrops[i].getnAtm() * area;
        }
        return retVal;
    }

    public double GetManureNH3NEmission()
    {
            return GetManureNH3NEmission( theCrops.Count);
    }

    public double GetManureNH3NEmission(int maxCrops)
    {
        double retVal = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            retVal += theCrops[i].GetmanureNH3Nemission() * area;
        }
        return retVal;
    }

    public double GetFertNH3NEmission()
    {
        return GetFertNH3NEmission(theCrops.Count);
    }
   
     public double GetFertNH3NEmission(int maxCrops)
    {
        double retVal = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            retVal += theCrops[i].GetfertiliserNH3Nemission() * area;
        }
        return retVal;
    }

    public double GetOrganicNLeached()
    {
        return GetOrganicNLeached(theCrops.Count);
    }

    public double GetOrganicNLeached(int maxCrops)
    {
        double retVal = 0;
        for (int i = 0; i < maxCrops; i++)
        {
            retVal += theCrops[i].GetOrganicNLeached() * area;
        }
        return retVal;
    }

    public double GetN2NEmission()
    {
            return GetN2NEmission(theCrops.Count);
    }

    public double GetN2NEmission(int maxCrops)
    {
        double retVal = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            retVal += theCrops[i].GetN2Nemission() * area;
        }
        return retVal;
    }
    
    public double GetN2ONemission()
    {
        return GetN2ONemission(theCrops.Count);
    }

    public double GetN2ONemission(int maxCrops)
    {
        double retVal = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            retVal += theCrops[i].GetN2ONemission() * area;
        }
        return retVal;
    }


        public void spinup(ref double soilNmineralised, double initialFOMinput, double InitialFOMCtoN, double meanTemperature, bool diagnostics)
    {            
        int startDay=1;
        int endDay=365;
        double[,] FOMin = new double[365,aModel.GetnumOfLayers()];
        double[,] HUMin = new double[365, aModel.GetnumOfLayers()];
        double[] cultivation = new double[365];
        double[] fomnIn = new double[365];
        int spinupYears = 50;
        double Cchange = 0;
        double Nleached = 0;
        double CO2Emission = 0;
        double Cleached = 0;
        double FOMinput = initialFOMinput;
        double FOMnInput = initialFOMinput / InitialFOMCtoN;
        double CinputSpin = 0;
        for (int i = 0; i < 365; i++)
        {
            for (int j = 0; j < aModel.GetnumOfLayers(); j++)
            {
                FOMin[i,j] = 0;
                HUMin[i, j] = 0;
            }
            fomnIn[i] = 0;
            FOMin[i, 0] = FOMinput / 365.0;
            fomnIn[i] = FOMnInput / 365.0;
        }
        double initCStored = aModel.GetCStored();//value in kgC/ha
        double Nmin = 0;
        for (int j = 0; j < spinupYears; j++)
        {
            double tempCchange = 0;
            double tempNleached = 0;
            double tempCO2Emission = 0;
            double tempCleached = 0;
            if (GlobalVars.Instance.theZoneData.GetdebugIt())
                GlobalVars.Instance.theZoneData.WriteToDebug(j.ToString() + "\t");
            double droughtFactor = GlobalVars.Instance.theZoneData.GetMeanDroughtIndex(1, 1, 1, 31, 12, 1);
            aModel.Dynamics(1,startDay, endDay, FOMin, HUMin, fomnIn, cultivation, meanTemperature,droughtFactor,
                    ref tempCchange, ref tempCO2Emission, ref tempCleached, ref Nmin, ref tempNleached, diagnostics);
            if (diagnostics)
                Console.WriteLine(j.ToString() + " " + aModel.GetFOMCStored().ToString() + " " + aModel.GetHUMCStored().ToString() + " " + aModel.GetROMCStored().ToString() +
                " " + aModel.GetCStored().ToString());
            if (GlobalVars.Instance.theZoneData.GetdebugIt())
                GlobalVars.Instance.theZoneData.WriteLineToDebug("");
            Cchange += tempCchange;
            Nleached += tempNleached;
            CO2Emission += tempCO2Emission;
            Cleached += tempCleached;
        }
        double deltaC = aModel.GetCStored() - initCStored;
        CinputSpin = FOMinput * spinupYears;
        double diff = CinputSpin - (CO2Emission + Cleached + deltaC);
        if ((Nmin < 0)&&(Math.Abs(Nmin)<0.0001))
            Nmin = 0;
        soilNmineralised = Nmin;
        startsoilMineralN = Nmin;
        initialSoilC = GetCStored();//value in kgC per crop sequence
        initialSoilN = GetNStored();//value in kgN per crop sequence
        if (diagnostics)
        {
            Console.WriteLine("kg C/ha at start of spinup " + initCStored.ToString() + " kg C/ha at end of spinup " + initialSoilC.ToString());
            Console.WriteLine("kg N/ha min at end of spinup " + soilNmineralised.ToString());
        }
        startSoil = new ctool2(aModel);
    }

    public void resetSoilWaterModel()
    {
        thesoilWaterModel.reload_simpleSoil(startWaterModel);
    }

   public void resetC_Tool()
   {
       aModel.reloadC_Tool(startSoil);
   }
   public void RunCropCTool(bool diagnostics, int cropNo, double meanTemperature, double droughtFactor, ref double CropCinputToSoil, ref double CropNinputToSoil,
       ref double ManCinputToSoil, ref double ManNinputToSoil, ref double CropsoilCO2Emission, ref double CropCleached, ref double Nmin)
{
    double Nin = GetNStored(); //N in soil at start of crop period
    ManCinputToSoil = 0;
    ManNinputToSoil = 0;
    if (diagnostics)
    {
        Console.WriteLine(theCrops[cropNo].Getname().ToString());
        Console.WriteLine("N in " + Nin.ToString());
    }
    long startDay = theCrops[cropNo].getStartLongTime();
    long stopDay = theCrops[cropNo].getEndLongTime();

    long lastDay = theCrops[cropNo].getDuration();
    double[,] FOMin = new double[lastDay, aModel.GetnumOfLayers()];
    double[,] HUMin = new double[lastDay, aModel.GetnumOfLayers()];
    double[] cultivation = new double[lastDay];
    double[] fomnIn = new double[lastDay];
    double FOMCsurface = theCrops[cropNo].GetsurfaceResidueC(); //Fresh plant OM carbon input to surface of soil (e.g. leaf and stem litter, unharvested above-ground OM)
    double FOMCsubsurface = theCrops[cropNo].GetsubsurfaceResidueC(); //Fresh plant OM carbon input below the soil surface (e.g. roots)
    double urineCO2Emission = theCrops[cropNo].GetUrineC(); //assume urine C is all emitted
    double faecalC = theCrops[cropNo].GetfaecalC();
    ManCinputToSoil += urineCO2Emission + faecalC;
    CropCinputToSoil = FOMCsurface + FOMCsubsurface;
    double tempCleached = 0;

    double faecalN = theCrops[cropNo].GetfaecalN();
    ManNinputToSoil += faecalN;
    double FOMNsurface = theCrops[cropNo].GetsurfaceResidueN(); //Fresh organic N added to soil surface
    double FOMNsubsurface = theCrops[cropNo].GetsubsurfaceResidueN();//Fresh organic N added below soil surface
    CropNinputToSoil = FOMNsurface + FOMNsubsurface + faecalN;
    //distribute below ground OM using a triangular function
    double maxDepthThisCrop = theCrops[cropNo].GetMaximumRootingDepth();
    int numOfLayers = aModel.GetnumOfLayers();
    double OMdepthDistribCoeff = 2 / ((double)numOfLayers);
    double OMtimeDistCoeff = 2 / (double)theCrops[cropNo].getDuration();
                
    double acc2 = 0;
    double oldDayCum = 0;
    for (int j = 0; j < lastDay; j++)
    {
        double manureFOMCsurface = theCrops[cropNo].GetmanureFOMCsurface(j);  //Fresh OM carbon input to surface of soil
        double manureHUMCsurface = theCrops[cropNo].GetmanureHUMCsurface(j); //Humic OM carbon input to surface of soil
        double manureFOMnsurface = theCrops[cropNo].GetmanureFOMNsurface(j); //Fresh OM nitrogen input to surface of soil
        double manureHUMnsurface = theCrops[cropNo].GetmanureHUMCsurface(j) / GlobalVars.Instance.getCNhum(); //Humic nitrogen input below surface of soil
        double faecalCToday = faecalC / lastDay;   //distribute faecal C evenly over whole period
        ManCinputToSoil += manureFOMCsurface + manureHUMCsurface;
        double faecalNToday = faecalN / lastDay;
        ManNinputToSoil += manureFOMnsurface + manureHUMnsurface;

        //distribuute the organic matter inputs in crop residues and roots over time
        double propThisDay = 0;
        if (theCrops[cropNo].Getpermanent() == false)
        {
            double newDayCum = (((double)j + 1) / 2) * OMtimeDistCoeff * (((double)j + 1) / (double)theCrops[cropNo].getDuration());
            propThisDay = newDayCum - oldDayCum;
            oldDayCum = newDayCum;
        }
        else
            propThisDay=1/(double)theCrops[cropNo].getDuration();

        FOMin[j, 0] = propThisDay * FOMCsurface;
        double oldDepthCum = 1.0;
        double acc1 = 0;
        //distribute C and N inputs over time and soil depth
        for (int k = aModel.GetnumOfLayers() - 1; k >= 0; k--)
        {
            double newDepthCum = (((double)k) / 2) * OMdepthDistribCoeff * (((double)k) / (double)numOfLayers);
            double propThisLayer = oldDepthCum - newDepthCum;
            FOMin[j, k] += FOMCsubsurface * propThisDay * propThisLayer;
            HUMin[j, k] = 0;
            oldDepthCum = newDepthCum;
            acc1 += propThisLayer;
        }
        FOMin[j, 0] += manureFOMCsurface + faecalCToday;
        HUMin[j, 0] += manureHUMCsurface;
        //set fresh OM nitrogen inputs over time
        fomnIn[j] = (FOMNsurface +FOMNsubsurface) * propThisDay + manureFOMnsurface + faecalNToday;
        acc2 += propThisDay;
    }
    double Cchange = 0;
    Nmin = 0;
    double orgNleached = 0;
    double CO2Emission = 0;
    long JDay = theCrops[cropNo].getStartLongTime();

    while (JDay > 365)
        JDay = JDay - 365;
    aModel.Dynamics((int)JDay, theCrops[cropNo].getStartLongTime(), theCrops[cropNo].getEndLongTime(), FOMin, HUMin, fomnIn, cultivation,
        meanTemperature, droughtFactor, ref Cchange, ref CO2Emission, ref tempCleached, ref Nmin, ref orgNleached, false);

    CropsoilCO2Emission = CO2Emission + urineCO2Emission;
    CropCleached = tempCleached;
    theCrops[cropNo].SetOrganicNLeached(orgNleached);
    theCrops[cropNo].SetsoilNMineralisation(Nmin);
    Nin += area * (FOMNsurface + theCrops[cropNo].GetTotalmanureHUMNsurface() + theCrops[cropNo].GetTotalmanureFOMNsurface() +
        FOMNsubsurface + faecalN);
    double Nout = GetNStored();
    double balance = Nin - (Nout + area * (orgNleached + Nmin));

    if (diagnostics)
    {
        Console.WriteLine("orgN leached " + (area * orgNleached).ToString() + " Nmin " + (area * Nmin).ToString());
        Console.WriteLine("N out " + Nout.ToString() + " bal " + balance.ToString());
    }
    double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
    double diff = balance / Nin;
    if (Math.Abs(diff) > tolerance)
    {
        string messageString = "Error; crop sequence soil C-Tool N balance is greater than the permitted margin\n";
        messageString += "Crop sequence name = " + name + "\n";
        Write();
        GlobalVars.Instance.Error(messageString);
    }
    if ((Nmin < 0)&&(Math.Abs(Nmin)<0.0001))
        Nmin = 0;
}


   public void extraoutput(XmlWriter writer, System.IO.StreamWriter tabFile)
   {
       for (int i = 0; i < theCrops.Count; i++)
       {
           writer.WriteStartElement("Crop");
           writer.WriteStartElement("Identity");
           writer.WriteValue(theCrops[i].Getidentity());
           for (int j = 0; j < theCrops[i].GettheProducts().Count; j++)
           {
               writer.WriteStartElement("Product");
               writer.WriteStartElement("Identity");
               writer.WriteValue(theCrops[i].GettheProducts()[j].GetExpectedYield());
               writer.WriteEndElement();
               writer.WriteEndElement();
           }
           writer.WriteEndElement();
            writer.WriteEndElement();
               
       }
   }
   public void calculateExpectedYield()
    {
        List<int> ID=new List<int>();
        List<double> ExpectedYeld0=new List<double>();
        List<double> ExpectedYeld1 = new List<double>();
        List<int> NumberOfPlants = new List<int>();
        for (int i = 0; i < theCrops.Count; i++)
        {
            int newID = -1;
            for (int j = 0; j < ID.Count; j++)
            {
                if (ID[j] == theCrops[i].Getidentity())
                {
                    newID = j;
                }
            }
            if (newID != -1)
            {
                if (theCrops[newID].GettheProducts().Count >= 1)
                ExpectedYeld0[newID] += theCrops[i].GettheProducts()[0].GetExpectedYield();
                if (theCrops[newID].GettheProducts().Count == 2)
                ExpectedYeld1[newID] += theCrops[i].GettheProducts()[1].GetExpectedYield();
                NumberOfPlants[newID] += 1;
            }
            else
            {
                ID.Add(theCrops[i].Getidentity());
                if(theCrops[i].GettheProducts().Count>=1)
                    ExpectedYeld0.Add(theCrops[i].GettheProducts()[0].GetExpectedYield());
                else
                    ExpectedYeld0.Add(-1);

                if (theCrops[i].GettheProducts().Count == 2)
                    ExpectedYeld1.Add(theCrops[i].GettheProducts()[1].GetExpectedYield());
                else
                    ExpectedYeld1.Add(-1);
                NumberOfPlants.Add(1);
            }
        }
        GlobalVars.Instance.writeStartTab("CropSequenceClass");
        for (int i = 0; i < ID.Count; i++)
        {
            GlobalVars.Instance.writeStartTab("Crop");
            GlobalVars.Instance.writeSinglItem("Identity", ID[i]);
            if(ExpectedYeld0[i]!=-1)
            {
                GlobalVars.Instance.writeStartTab("product");
               GlobalVars.Instance.writeSinglItem("ExpectedYield", ExpectedYeld0[i] / NumberOfPlants[i]);
               GlobalVars.Instance.writeEndTab();
            }
            if (ExpectedYeld1[i] != -1)
            {
                GlobalVars.Instance.writeStartTab("product");
                GlobalVars.Instance.writeSinglItem("ExpectedYield", ExpectedYeld1[i] / NumberOfPlants[i]);
                GlobalVars.Instance.writeEndTab();
            }
            GlobalVars.Instance.writeEndTab();
        }
        GlobalVars.Instance.writeEndTab();


    }
}
