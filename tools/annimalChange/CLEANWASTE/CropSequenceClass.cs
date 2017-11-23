using System;
using System.Collections.Generic;
using System.Xml;

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
    ctool2 aModel;

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

    public CropSequenceClass(string aPath, int aID, int zoneNr, int currentFarmType)
    {
        path = aPath;
        identity = aID;
        FarmType = currentFarmType;
        FileInformation rotation = new FileInformation(GlobalVars.Instance.getFarmFilePath(), GlobalVars.Instance.GeterrorFileName());
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
                CropClass aCrop = new CropClass(crop, i, zoneNr);
                aCrop.SetcropSequenceName(name);
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
                long mod = numDays % 365;
                if (Math.Abs(mod) > 1)
                {
                    System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                    file.Write("Error - gap at end of cropping sequence");
                    file.Close();
                    Console.WriteLine("Error - gap at end of cropping sequence");
                    Console.WriteLine("Crop sequence name = " + name);
                    if (GlobalVars.Instance.getPauseBeforeExit())
                        Console.Read();
                    Environment.Exit(0);
                }
            }
        }
        lengthOfSequence = calculatelengthOfSequence();  //calculate length of sequence in years
        int cropsPerSequence = theCrops.Count;
        List<CropClass> CopyOfPlants = new List<CropClass>();
        if ((cropsPerSequence==1)&&(lengthOfSequence>1))    //need to clone for one or more years, if crop persists for more than one year
        {
         //   theCrops[0].SetEndYear(theCrops[0].GetStartYear() + 1); //now describes first year only
            if ((theCrops[0].GetStartDay() == 1) && (theCrops[0].GetStartMonth() == 1))
                theCrops[0].SetEndYear(theCrops[0].GetStartYear());
            else
                theCrops[0].SetEndYear(theCrops[0].GetStartYear() + 1);
            theCrops[0].CalcDuration();
            theCrops[0].CalcProportionLeached();
            for (int i = 1; i < lengthOfSequence; i++)
            {
                CropClass aCrop = new CropClass(theCrops[0]);
                theCrops.Add(aCrop);
                if ((aCrop.GetStartDay()==1)&&(aCrop.GetStartMonth()==1))
                    aCrop.SetEndYear(i + aCrop.GetStartYear());
                else
                    aCrop.SetEndYear(i + aCrop.GetStartYear() + 1);
                aCrop.SetStartYear(i + aCrop.GetStartYear());
                aCrop.CalcDuration();
                Console.WriteLine(i.ToString() + " " + aCrop.GetStartYear().ToString() + " " +aCrop.GetEndYear().ToString());
            }

        }
        int length = GlobalVars.Instance.GetminimumTimePeriod();
        repeats = (int) Math.Ceiling(((double) length)/((double) lengthOfSequence));
        for (int j = 0; j < repeats-1; j++)
        {
            for (int i = 0; i < theCrops.Count; i++)
            {
                CopyOfPlants.Add(new CropClass(theCrops[i]));
            }
        }
        int k = 1;
        int cropCount = 1;
        for (int i = 0; i < CopyOfPlants.Count; i++)
        {
            CropClass acrop = CopyOfPlants[i];
            theCrops.Add(acrop);
            acrop.SetStartYear(k * lengthOfSequence + acrop.GetStartYear());
            acrop.SetEndYear(k * lengthOfSequence + acrop.GetEndYear());
            if (cropCount == cropsPerSequence)
            {
                k++;
                cropCount = 0;
            }
            cropCount++;
                //   CopyOfPlants[i].Setname(CopyOfPlants[i].Getname() + i.ToString());
        }
        AdjustDates(theCrops[0].GetStartYear());
        for (int i = 0; i < theCrops.Count; i++)
        {
            CropClass aCrop = theCrops[i];
            aCrop.setArea(area);
        }
        lengthOfSequence = calculatelengthOfSequence();  //recalculate length of sequence in years
        getparameters(zoneNr);
        aModel = new ctool2();
        int soilIndex=-1;
        for (int i = 0; i < GlobalVars.Instance.theZoneData.thesoilData.Count; i++)
        {
            if(GlobalVars.Instance.theZoneData.thesoilData[i].name.CompareTo(soilType)==0)
                soilIndex=i;
        }
        if(soilIndex==-1)
        {
            System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.Write("Error - could not find soil type " + soilType + " in parameter file");
            file.Close();
            Console.WriteLine("Error - could not find soil type " + soilType + " in parameter file");
            Console.WriteLine("Crop sequence name = " + name);
            if (GlobalVars.Instance.getPauseBeforeExit())
                Console.Read();
            Environment.Exit(0);
        }
        double initialC = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].initialC;
        double initialFOMn = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].initialFOMn;
        double propHUM = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].propHUM;
        double propROM = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].propROM;

        double InitialCtoN = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].InitialCtoN;
        double InitialFOMCtoN = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].theC_ToolData[FarmType-1].InitialFOMCtoN;
        double ClayFraction = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].ClayFraction;
        double maxSoilDepth = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].maxSoilDepth;
        double dampingDepth = GlobalVars.Instance.theZoneData.thesoilData[soilIndex].GetdampingDepth();
        initialFOMn = initialC * (1 - (propHUM + propROM)) / 10;
        int offset = GlobalVars.Instance.theZoneData.GetairtemperatureOffset();
        double amplitude = GlobalVars.Instance.theZoneData.GetairtemperatureAmplitude();
        aModel.Initialisation(ClayFraction, offset, amplitude, maxSoilDepth, dampingDepth, initialC, initialFOMn, propHUM, propROM, 
            GlobalVars.Instance.getConstantFilePath(), GlobalVars.Instance.GeterrorFileName());
        double mineralNFromSpinup = 0;
        spinup(ref mineralNFromSpinup, GlobalVars.Instance.theZoneData.GetdebugIt());
        theCrops[0].SetsoilNMineralisation(mineralNFromSpinup);
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
            System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.Write("Error - could not find soil type " + soilType + " in parameter file");
            file.Close();
            Console.WriteLine("Error - could not find soil type " + soilType + " in parameter file");
            Console.WriteLine("Crop sequence name = " + name);
            if (GlobalVars.Instance.getPauseBeforeExit())
                Console.Read();
            Environment.Exit(0);
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

    public double getProcessStorageLossCarbon()
    {
        double retVal = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            retVal += theCrops[i].GetstorageProcessingCLoss() * area;
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
        }
        long period = lastDate - firstDate;
        double temp = ((double)period) / ((double)365);
        int retVal = (int)Math.Ceiling(temp);
        return retVal;   
    }
    public void Write(XmlWriter writer, System.IO.StreamWriter tabFile)
    {
        writer.WriteStartElement("CropSequenceClass");

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

        writer.WriteStartElement("soilType");
        writer.WriteStartElement("Description");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("Units");
        writer.WriteValue("??");
        writer.WriteEndElement();
        writer.WriteStartElement("name");
        writer.WriteValue("soilType");
        writer.WriteEndElement();
        writer.WriteStartElement("Value");
        writer.WriteValue(soilType);
        writer.WriteEndElement();
        writer.WriteEndElement();

        tabFile.Write("CropSequenceClass" + '\n');
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
        tabFile.Write("identity" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(identity.ToString() + "\n");

        tabFile.Write("Description" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("Units" + "\t");
        tabFile.Write("??" + "\t");
        tabFile.Write("soilType" + "\t");
        tabFile.Write("Value" + "\t");
        tabFile.Write(soilType.ToString() + "\n");
        for (int i = 0; i < theCrops.Count; i++)
        {
            theCrops[i].Write(writer, tabFile);
        }
        writer.WriteEndElement();

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
    public void getAllFeedItems()
    {
        for (int i = 0; i < theCrops.Count; i++)
            theCrops[i].getAllFeedItems();
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
        for (int i = 0; i < theCrops.Count; i++)
        {
            double meanTemperature = GlobalVars.Instance.theZoneData.GetMeanTemperature(theCrops[i]);
            double meanDroughtFactor= GlobalVars.Instance.theZoneData.GetMeanDroughtIndex(theCrops[i]);
            theCrops[i].Calcwaterlimited_yield(meanDroughtFactor);
            if (GlobalVars.Instance.GetIsIterating() == false)
            {
                theCrops[i].DoCropInputs(false);
                RunCropCTool(false, i, meanTemperature, meanDroughtFactor, ref CropCinputToSoil, ref CropNinputToSoil, ref ManCinputToSoil, ref ManNinputToSoil, ref CropsoilCO2Emission, ref CropCleached, ref mineralisedN);
                theCrops[i].CalcAvailableN(surplusMineralN, mineralisedN);
                theCrops[i].CalcModelledYield(ref surplusMineralN, true);
            }
            else
            {
                bool doneOnce = false;
                startSoil.CopyCTool(aModel);
                oldsoilCO2Emission = soilCO2Emission;
                oldCleached = Cleached;
                double oldsurplusMineralN = surplusMineralN;
                bool gotit = false;
                int count=0;
                while ((gotit==false)||(doneOnce==false))
                {
                    count++;
                    Console.WriteLine("seq " + identity.ToString() + "crop " + i.ToString() + " loop " + count.ToString());
                    if (doneOnce)
                    {
                        resetC_Tool();
                        surplusMineralN = oldsurplusMineralN;
                        theCrops[i].DoCropInputs(true);
                    }
                    else
                        theCrops[i].DoCropInputs(false);
                    RunCropCTool(false, i, meanTemperature, meanDroughtFactor, ref CropCinputToSoil, ref CropNinputToSoil, ref ManCinputToSoil, ref ManNinputToSoil, ref CropsoilCO2Emission, ref CropCleached, ref mineralisedN);
                    theCrops[i].CalcAvailableN(surplusMineralN, mineralisedN);
                    gotit=theCrops[i].CalcModelledYield(ref surplusMineralN, true);
                    doneOnce = true;
                }
                count = 0;
                CinputToSoil += (CropCinputToSoil + ManCinputToSoil) * area;
                NinputToSoil += (CropNinputToSoil + ManNinputToSoil) * area;
                soilCO2Emission += CropsoilCO2Emission * area;
                Cleached += CropCleached * area;
               theCrops[i].CalculateHarvestedYields();
                CheckRotationNBalance(i + 1);
            }
        }
    }

    
    public bool CheckRotationCBalance()
    {
        bool retVal = true;
        double harvestedC = 0;
        double fixedC = 0;
        double manureC = 0;
        double faecalC=0;
        double urineC = 0;
        for (int i = 0; i < theCrops.Count; i++)
        {
            retVal = theCrops[i].CheckCropCBalance(name,i+1);
            if (retVal == false)
                break;
            harvestedC += theCrops[i].GetharvestedC() * area;
            fixedC += theCrops[i].getCFixed() * area;
            manureC += theCrops[i].GetManureC() * area;
            faecalC += theCrops[i].GetfaecalC() * area;
            urineC += theCrops[i].GetUrineC() * area;
        }
        finalSoilC = GetCStored();
        CdeltaSoil =  finalSoilC - initialSoilC;
        double diff = CinputToSoil- (soilCO2Emission + Cleached+ CdeltaSoil );
        double errorPercent = 100 * diff / initialSoilC;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
         {
             string outstring1 = "Error; soil C balance is greater than the permitted margin";
             string outstring2 = "Crop sequence name = " + name;
             string outstring3 = "Percentage error = " + errorPercent.ToString("0.00") + "%";

             System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
             file.WriteLine(outstring1);
             file.WriteLine(outstring2);
             file.WriteLine(outstring3);
             file.Close();
             Console.WriteLine(outstring1);
             Console.WriteLine(outstring2);
             Console.Write(outstring3);
             Write(GlobalVars.Instance.theZoneData.GetXMLWriter(), GlobalVars.Instance.theZoneData.GetTabFileWriter());
             GlobalVars.Instance.theZoneData.CloseOutputXML();

             if (GlobalVars.Instance.getPauseBeforeExit())
                 Console.ReadKey();
             Environment.Exit(0);
         }
         double Charvested = getCHarvested();
         double Cfixed = getCFixed();
        Cbalance=(Cfixed + manureC + faecalC + urineC - (soilCO2Emission + Cleached + CdeltaSoil + Charvested));
         diff =  Cbalance/ initialSoilC;
         errorPercent = 100 * diff;
         if (Math.Abs(diff) > tolerance) 
         {
             string outstring1 = "Error; crop sequence C balance is greater than the permitted margin";
             string outstring2 = "Crop sequence name = " + name;
             string outstring3 = "Percentage error = " + errorPercent.ToString("0.00") + "%";

             System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
             file.WriteLine(outstring1);
             file.WriteLine(outstring2);
             file.WriteLine(outstring3);
             file.Close();
             Console.WriteLine(outstring1);
             Console.WriteLine(outstring2);
             Console.Write(outstring3);
             Write(GlobalVars.Instance.theZoneData.GetXMLWriter(), GlobalVars.Instance.theZoneData.GetTabFileWriter());
             GlobalVars.Instance.theZoneData.CloseOutputXML();

             if (GlobalVars.Instance.getPauseBeforeExit())
                 Console.ReadKey();
             Environment.Exit(0);
         }
         return retVal;
    }

    public void CheckRotationNBalance()
    {
        CheckRotationNBalance(theCrops.Count);
    }

    public void CheckRotationNBalance(int maxCrops)
    {
        double Ninp = 0;
        for (int i = 0; i < maxCrops; i++)
            theCrops[i].CheckCropNBalance(name,i+1);
        double NAtm = getNAtm(maxCrops);
        double ManureNapplied = GetManureNapplied(maxCrops);
        double excretaNInput = GetexcretaNInput(maxCrops);
        double FertiliserNapplied = GetFertiliserNapplied(maxCrops);
        Ninp = NAtm + ManureNapplied+excretaNInput+FertiliserNapplied + startsoilMineralN * area;
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
        double NLost = NH3NmanureEmissions + fertiliserNH3Nemissions + urineNH3emissions + N2ONEmission + N2NEmission 
                + orgNleached + nitrateLeaching;
        Nbalance = Ninp - (NLost + harvestedN + NdeltaSoil + residualMineralN);
        double diff = Nbalance / Ninp;
        double errorPercent = 100 * diff;
        double tolerance = GlobalVars.Instance.getmaxToleratedErrorYield();
        if (Math.Abs(diff) > tolerance)
        {
            string outstring1 = "Error; crop sequence N balance is greater than the permitted margin";
            string outstring2 = "Crop sequence name = " + name;
            string outstring3 = "Percentage error = " + errorPercent.ToString("0.00") + "%";

            System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.WriteLine(outstring1);
            file.WriteLine(outstring2);
            file.WriteLine(outstring3);
            file.Close();
            Console.WriteLine(outstring1);
            Console.WriteLine(outstring2);
            Console.Write(outstring3);
            Write(GlobalVars.Instance.theZoneData.GetXMLWriter(), GlobalVars.Instance.theZoneData.GetTabFileWriter());
            GlobalVars.Instance.theZoneData.CloseOutputXML();

            if (GlobalVars.Instance.getPauseBeforeExit())
                Console.ReadKey();
            Environment.Exit(0);
        }
/*        double Charvested = getCHarvested();
        double Cfixed = getCFixed();
        Cbalance = (Cfixed - (soilCO2Emission + Cleached + CdeltaSoil + Charvested));
        diff = Cbalance / initialSoilC;
        errorPercent = 100 * diff;
        if (errorPercent > 0.1)
        {
            string outstring1 = "Error; crop sequence C balance is greater than the permitted margin";
            string outstring2 = "Crop sequence name = " + name;
            string outstring3 = "Percentage error = " + errorPercent.ToString("0.00") + "%";

            System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.WriteLine(outstring1);
            file.WriteLine(outstring2);
            file.WriteLine(outstring3);
            file.Close();
            Console.WriteLine(outstring1);
            Console.WriteLine(outstring2);
            Console.Write(outstring3);
            Write(GlobalVars.Instance.theZoneData.GetXMLWriter(), GlobalVars.Instance.theZoneData.GetTabFileWriter());
            GlobalVars.Instance.theZoneData.CloseOutputXML();

            if (GlobalVars.Instance.getPauseBeforeExit())
                Console.ReadKey();
            Environment.Exit(0);
        }*/
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

        public void GetAverageCropData()
    {
        //CropClass averageCropClass = new CropClass();
        for (int i = 0; i < theCrops.Count; i++)
        {
        }
    }

    
   public void spinup(ref double soilNmineralised, bool diagnostics)
    {            
        int startDay=1;
        int endDay=365;
        double[,] FOMin = new double[365,aModel.GetnumOfLayers()];
        double[,] HUMin = new double[365, aModel.GetnumOfLayers()];
        double[] cultivation = new double[365];
        double[] fomnIn = new double[365];
        double meanTemperature = 12.0;
        int spinupYears = 50;
        double Cchange = 0;
        double Nleached = 0;
        double CO2Emission = 0;
        double Cleached = 0;
        double FOMinput = 4000.0;
        double FOMnInput = 80.0;
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
    double FOMCsubsurface = theCrops[cropNo].GetsubsurfaceResidueC(); //Fresh plant OM carbon input to at depth in the soil (e.g. roots)
    double urineCO2Emission = theCrops[cropNo].GetUrineC(); //assume urine C is all emitted
    double faecalC = theCrops[cropNo].GetfaecalC();
    ManCinputToSoil += urineCO2Emission + faecalC;
    CropCinputToSoil = FOMCsurface + FOMCsubsurface;
    double tempCleached = 0;

    double faecalN = theCrops[cropNo].GetfaecalN();
    ManNinputToSoil += faecalN;
    double FOMNsurface = theCrops[cropNo].GetsurfaceResidueN(); //Fresh organic N added to soli surface
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
}

}
