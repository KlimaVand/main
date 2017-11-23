using System;
using System.Xml;
using System.Xml.Linq;
using System.Collections.Generic;

public class ctool2
{
    //Parameters to output
    double Th_diff = 0;
    double FOMdecompositionrate = 0;
    double HUMdecompositionrate = 0;
    double Clayfraction = 0;
    double tF = 0;
    double ROMificationfraction = 0;
    double ROMdecompositionrate = 0;
    double CtoNHUM = 10.0;
    double CtoNROM = 10.0;


    //Other variables to output
    int numberOfLayers = 0;

    //other variables
    //! fomc = fresh organic matter C, kg/ha
    //! humc = humic organic matter C, kg/ha
    //! romc = resistant organic matter C, kg/ha
    public double[] fomc;
    public double[] humc;
    public double[] romc;
    //! FOMn = N in fresh organic matter, kg N/ha
    public double FOMn = 0;

    double maxSoilDepth = 0;
    double timeStep = 1/365.25;
    double fCO2 = 0;
    double totalCO2Emission = 0;
    double CInput = 0;
    double FOMNInput = 0;
    double Nlost = 0;
    double offset = 0;
    double amplitude = 0;
    double dampingDepth = 0;
    string parens;
    private bool pauseBeforeExit = false;

    double FOMcInput = 0;
    double FOMcCO2 = 0;
    double FOMcToHUM = 0;
    double HUMcInput = 0;
    double HUMcCO2 = 0;
    double HUMcToROM = 0;
    double ROMcInput = 0;
    double ROMcCO2 = 0;

/**
* Destructor
*/
    ~ctool2()
    {
        
    }
/**
* Constructor
*/
    public ctool2(string aParens)
    {
        parens = aParens;
    }
/**
* Get Clay fraction
*/
    public double GetClayfraction() { return Clayfraction; }
/**
* Get FOM n
*/
    public double GetFOMn() { return FOMn; }
/**
* Set pause Before Exit
*/
    public void SetpauseBeforeExit(bool aVal) { pauseBeforeExit = aVal; }
/**
* Get CN
*/
    double CN(double cn)
    {

        return Math.Min(56.2 * Math.Pow(cn, -1.69), 1);
    }
/**
* Get FOM
*/
    public double GetFOM()
    {
        double retVal = 0;
        for (int i = 0; i < numberOfLayers; i++)
            retVal += fomc[i];
        return retVal;
    }
/**
* Get HUM
*/
    public double GetHUM()
    {
        double retVal = 0;
        for (int i = 0; i < numberOfLayers; i++)
            retVal += humc[i];
        return retVal;
    }
/**
* Get ROM
*/
    public double GetROM()
    {
        double retVal = 0;
        for (int i = 0; i < numberOfLayers; i++)
            retVal += romc[i];
        return retVal;
    }
/**
* Get OrgC
*/
    public double GetOrgC(int layer)
    {
        double retVal = 0;
        retVal = fomc[layer] + humc[layer] + romc[layer];
        return retVal;
    }

/**
     * @param ClayFraction 
     * @param offsetIn number of days to move 
     * @param amplitudeIn array [month, layer] of fresh organic matter input (kg/ha)
     * @param maxSoilDepthIn array [month, layer] of humic organic matter input (kg/ha)
     * @param dampingDepthIn array [month] of N input in fresh organic matter (kg/ha)
     * @param initialC array [month] of depth of cultivation (m) (not used yet)
     * @param initialFOMn mean air temperature for the agroecological zone (Celcius)
     * @param propHUM change in carbon in the soil over the period (kg) 
     * @param propROM mineralisation of soil N over the period (kg) (negative if N is immobilised)
     * @param parameterFileName N leached from the soil in organic matter (kg)
     * @param errorFileName
     */
    public void Initialisation(int soilTypeNo, double ClayFraction, double offsetIn, double amplitudeIn, double maxSoilDepthIn, double dampingDepthIn,
        double initialC, string[] parameterFileName, string errorFileName, double InitialCtoN, double pHUMupperLayer, double pHUMLowerLayer,
        ref double residualMineralN)
    {
        amplitude = amplitudeIn;
        maxSoilDepth = maxSoilDepthIn;
        dampingDepth = dampingDepthIn;
        Th_diff = 0.35E-6;
        residualMineralN = 0;
   
        FileInformation ctoolInfo = new FileInformation(parameterFileName);
        ctoolInfo.setPath("constants(0).C-Tool(-1).timeStep(-1)");
        timeStep = ctoolInfo.getItemDouble("Value"); //one day pr year

        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "NumOfLayers";
        numberOfLayers = ctoolInfo.getItemInt("Value");

        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "FOMdecompositionrate";
        FOMdecompositionrate = ctoolInfo.getItemDouble("Value");
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "HUMdecompositionrate";
        HUMdecompositionrate = ctoolInfo.getItemDouble("Value");
        this.Clayfraction = ClayFraction;
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "transportCoefficient";
        tF = ctoolInfo.getItemDouble("Value");
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "ROMdecompositionrate";
        ROMdecompositionrate = ctoolInfo.getItemDouble("Value");
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "fCO2";
        fCO2 = ctoolInfo.getItemDouble("Value");
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "ROMificationfraction";
        ROMificationfraction = ctoolInfo.getItemDouble("Value");
        fCO2 = 0.628;
        ROMificationfraction = 0.012; 

        fomc = new double[numberOfLayers];
        humc = new double[numberOfLayers];
        romc = new double[numberOfLayers];

        double CNfactor = CN(InitialCtoN);
        double fractionCtopsoil = 0.47;

        int NonBaselinespinupYears=0;
        if (GlobalVars.Instance.reuseCtoolData != -1)
        {
            FileInformation file = new FileInformation(GlobalVars.Instance.getConstantFilePath());
            file.setPath("constants(0).spinupYearsNonBaseLine(-1)");
            NonBaselinespinupYears = file.getItemInt("Value");
        }
        if ((GlobalVars.Instance.reuseCtoolData != -1)&&(NonBaselinespinupYears==0))
        {
            Console.WriteLine("handover Data from "+ GlobalVars.Instance.getReadHandOverData());
            string[] lines=null;
            try
            {
                lines = System.IO.File.ReadAllLines(GlobalVars.Instance.getReadHandOverData());
            }
            catch
            {
                GlobalVars.Instance.Error("could not find " + GlobalVars.Instance.getReadHandOverData());
            }
            bool gotit=false;
            for (int j = 0; j < lines.Length; j++)
            {
                string[] data = lines[j].Split('\t');
                if (soilTypeNo == Convert.ToDouble(data[0]))
                {
                    fomc[0] = Convert.ToDouble(data[1]);
                    fomc[1] = Convert.ToDouble(data[2]);
                    humc[0] = Convert.ToDouble(data[3]);
                    humc[1] = Convert.ToDouble(data[4]);
                    romc[0] = Convert.ToDouble(data[5]);
                    romc[1] = Convert.ToDouble(data[6]);
                    FOMn = Convert.ToDouble(data[7]);
                    residualMineralN = Convert.ToDouble(data[8]);
                    gotit=true;
                }
            }
            if (!gotit)
                GlobalVars.Instance.Error("could not find soil carbon data for soil type " + soilTypeNo.ToString());
            // file.WriteLine(fomc[0].ToString() + '\t' + fomc[1].ToString() + '\t' + humc[0].ToString() + '\t' + humc[1].ToString() + '\t' + humc[0].ToString() + '\t' + humc[1].ToString() + '\t' + FOMn);
            //file.Close();
        }
        else
        {
            humc[0] = initialC * pHUMupperLayer * CNfactor * fractionCtopsoil;
            romc[0] = initialC * fractionCtopsoil - humc[0];
            humc[1] = initialC * pHUMLowerLayer * CNfactor * (1 - fractionCtopsoil);
            romc[1] = initialC * (1 - fractionCtopsoil) - humc[1];
            fomc[0] = initialC * 0.05;
            FOMn = fomc[0]/10.0;
        }
        CInput = humc[0] + humc[1] + romc[0] + romc[1];
        CInput = initialC;
        HUMcInput = humc[0] + humc[1];
        ROMcInput = romc[0] + romc[1];
    }
/**
* Copy constructor
*/
    public ctool2(ctool2 C_ToolToCopy)
    {
        Th_diff = C_ToolToCopy.Th_diff;
        FOMdecompositionrate = C_ToolToCopy.FOMdecompositionrate;
        HUMdecompositionrate = C_ToolToCopy.HUMdecompositionrate;
        Clayfraction = C_ToolToCopy.Clayfraction;
        tF = C_ToolToCopy.tF;
        ROMificationfraction = C_ToolToCopy.ROMificationfraction;
        ROMdecompositionrate = C_ToolToCopy.ROMdecompositionrate;
        
        maxSoilDepth = C_ToolToCopy.maxSoilDepth;
        fCO2 = C_ToolToCopy.fCO2;
        CInput = C_ToolToCopy.CInput;
        FOMn = C_ToolToCopy.FOMn;
        CtoNHUM = C_ToolToCopy.CtoNHUM;
        CtoNROM = C_ToolToCopy.CtoNROM;
        FOMNInput = C_ToolToCopy.FOMNInput;
        Nlost = C_ToolToCopy.Nlost;
        offset = C_ToolToCopy.offset;
        amplitude = C_ToolToCopy.amplitude;
        dampingDepth = C_ToolToCopy.dampingDepth;

        numberOfLayers = C_ToolToCopy.numberOfLayers;
        fomc = new double[numberOfLayers];
        humc = new double[numberOfLayers];
        romc = new double[numberOfLayers];
        for (int j = 0; j < numberOfLayers; j++)
        {
            fomc[j] = C_ToolToCopy.fomc[j];
            romc[j] = C_ToolToCopy.romc[j];
            humc[j] = C_ToolToCopy.humc[j];
        }

    }
/**
* Copy Ctool
*/
    public void CopyCTool(ctool2 C_ToolToCopy)
    {
        Th_diff = C_ToolToCopy.Th_diff;
        FOMdecompositionrate = C_ToolToCopy.FOMdecompositionrate;
        HUMdecompositionrate = C_ToolToCopy.HUMdecompositionrate;
        Clayfraction = C_ToolToCopy.Clayfraction;
        tF = C_ToolToCopy.tF;
        ROMificationfraction = C_ToolToCopy.ROMificationfraction;
        ROMdecompositionrate = C_ToolToCopy.ROMdecompositionrate;
        
        maxSoilDepth = C_ToolToCopy.maxSoilDepth;
        fCO2 = C_ToolToCopy.fCO2;
        CInput = C_ToolToCopy.CInput;
        FOMn = C_ToolToCopy.FOMn;
        CtoNHUM = C_ToolToCopy.CtoNHUM;
        CtoNROM = C_ToolToCopy.CtoNROM;
        FOMNInput = C_ToolToCopy.FOMNInput;
        Nlost = C_ToolToCopy.Nlost;
        offset = C_ToolToCopy.offset;
        amplitude = C_ToolToCopy.amplitude;
        dampingDepth = C_ToolToCopy.dampingDepth;

        numberOfLayers = C_ToolToCopy.numberOfLayers;
        fomc = new double[numberOfLayers];
        humc = new double[numberOfLayers];
        romc = new double[numberOfLayers];
        for (int j = 0; j < numberOfLayers; j++)
        {
            fomc[j] = C_ToolToCopy.fomc[j];
            romc[j] = C_ToolToCopy.romc[j];
            humc[j] = C_ToolToCopy.humc[j];
        }

    }
/**
* reload C_Tool
*/
    public void reloadC_Tool(ctool2 original)
    {
        for (int j = 0; j < numberOfLayers; j++)
        {
            fomc[j] = original.fomc[j];
            romc[j] = original.romc[j];
            humc[j] = original.humc[j];
        }
        FOMn = original.FOMn;
    }
/**
* Get numbers Of Layers
*/
    public int GetnumOfLayers() { return numberOfLayers; }


/**
* Get C Stored
*/
    public double GetCStored()
    {
        double Cstored = 0;
        for (int j = 0; j < numberOfLayers; j++)
            Cstored += fomc[j] + humc[j] + romc[j];
        return Cstored;
    }
/**
* Get FOM C Stored
*/
    public double GetFOMCStored()
    {
        double FOMCstored = 0;
        for (int j = 0; j < numberOfLayers; j++)
            FOMCstored += fomc[j];
        return FOMCstored;
    }
/**
* Get HUM C Stored
*/
    public double GetHUMCStored()
    {
        double HUMCstored = 0;
        for (int j = 0; j < numberOfLayers; j++)
            HUMCstored += humc[j];
        return HUMCstored;
    }
/**
* Get ROM C Stored
*/
    public double GetROMCStored()
    {
        double ROMCstored = 0;
        for (int j = 0; j < numberOfLayers; j++)
            ROMCstored += romc[j];
        return ROMCstored;
    }
/**
* Get N Stored
*/
    public double GetNStored()
    {
        double Nstored = FOMn;
        for (int j = 0; j < numberOfLayers; j++)
            Nstored += (romc[j]/CtoNROM) + (humc[j]/CtoNHUM);
        return Nstored;
    }
/**
* Get HUM n
*/
    public double GetHUMn() { return GetHUMCStored() / CtoNHUM; }
/**
* Get ROM n
*/
    public double GetROMn() { return GetROMCStored() / CtoNROM; }
/**
* Get C Details
*/
    public void GetCDetails()
    {
        for (int j = 0; j < numberOfLayers; j++)
            GlobalVars.Instance.log(j.ToString() + " " + fomc[j].ToString() + " " + humc[j].ToString() + " " +romc[j].ToString(),5);
    }


/**
* Check C Balance
*/
    public void CheckCBalance()
    {
        double Cstored = GetCStored();
        double CBalance = CInput - (Cstored + totalCO2Emission);
        double diff = CBalance / CInput;
        if (Math.Abs(diff) > 0.05)
        {
            double errorPercent = 100 * diff;
           /* System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
            file.WriteLine("Error; C balance in C-Tool");
            file.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
            file.Close();*/
            string messageString=("Error; C balance in C-Tool\n");
            messageString+=("Percentage error = " + errorPercent.ToString("0.00") + "%");
            GlobalVars.Instance.Error(messageString);
        }

    }

/**
     * @param startMonth first month of period
     * @param endMonth last month of period
     * @param FOM_Cin array [month, layer] of fresh organic matter input (kg/ha)
     * @param HUM_Cin array [month, layer] of humic organic matter input (kg/ha)
     * @param FOMnin array [month] of N input in fresh organic matter (kg/ha)
     * @param cultivation array [month] of depth of cultivation (m) (not used yet)
     * @param meanTemperature mean air temperature for the agroecological zone (Celcius)
     * @param Cchange change in carbon in the soil over the period (kg) 
     * @param Nmin mineralisation of soil N over the period (kg) (negative if N is immobilised)
     * @param Nleached N leached from the soil in organic matter (kg)
     */
    public XElement Dynamics(bool writeOutput, int julianDay, long startDay, long endDay, double[,] FOM_Cin, double[,] HUM_Cin, double[] FOMnIn, 
        double[] cultivation, double[] meanTemperature, double droughtIndex, ref double Cchange, ref double CO2Emission,
        ref double Cleached, ref double Nmin, ref double Nleached,  int CropSeqID)
    {
        Cchange = 0;
        Nmin = 0;
        FOMNInput = 0;
        double FOMCInput = 0;
        double HUMCInput = 0;
        double FOMnmineralised = 0;
        double CStart = GetCStored();
        double NStart = GetNStored();
        double startHUM = GetHUMCStored();
        double startROM = GetROMCStored();
        double FOMCO2 = 0;
        double HUMCO2 = 0;
        double ROMCO2 = 0;
        CO2Emission = 0;
        Nleached = 0;
        Cleached = 0;
        long iterations = endDay - startDay+1;
        double balance = 0;
        
            //startDay, endDay
        XElement ctoolData = new XElement("ctool");
        if ((GlobalVars.Instance.Ctoolheader == false)&&(writeOutput))
        {
            GlobalVars.Instance.writeCtoolFile("CropSeqID", "CropSeqID", "CropSeqID", CropSeqID, parens, 0);
            GlobalVars.Instance.writeCtoolFile("startDay", "startDay", "day", startDay.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("endDay", "endDay", "day", endDay.ToString(), parens, 0);

            GlobalVars.Instance.writeCtoolFile("FOMCStoredStart", "Initial C FOM", "MgC/ha", GetFOMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMCStoredStart", "Initial C HUM", "MgC/ha", GetHUMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("ROMCStoredStart", "Initial C ROM", "MgC/ha", GetROMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("FOMnStoredStart", "Initial N FOM", "MgN/ha", GetFOMn().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMnStoredStart", "Initial N HUM", "MgN/ha", GetHUMn().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("ROMnStoredStart", "Initial N ROM", "MgN/ha", GetROMn().ToString(), parens, 0);

            GlobalVars.Instance.writeCtoolFile("FOMCInput", "FOM_C_input", "MgC/ha/period", FOMCInput.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMCInput", "HUM_C_input", "MgC/ha/period", HUMCInput.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("CO2Emission", "CO2_C_emission", "MgC/ha/period", CO2Emission.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("balance", "balance", "MgC/ha/period", balance.ToString(), parens, 0);


            GlobalVars.Instance.writeCtoolFile("FOMCStoredEnd", "Final_C_FOM", "MgC/ha", GetFOMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMCStoredEnd", "Final_C_HUM", "MgC/ha", GetHUMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("ROMCStoredEnd", "Final_C_ROM", "MgC/ha", GetROMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("SoilOrganicCarbon=soc", "Final_C_Total", "MgC/ha", GetCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("FOMnStoredEnd", "Final_N_FOM", "MgN/ha", GetFOMn().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMnStoredEnd", "Final_N_HUM", "MgN/ha", GetHUMn().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("ROMnStoredEnd", "Final_N_ROM", "MgN/ha", GetROMn().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("finalN til SoilOrganicNitrogen=son", "Final_N_Total", "MgN/ha", 0.ToString(), parens, 0);

            GlobalVars.Instance.writeCtoolFile("FOMNInput", "FOMNin", "MgN/ha/period", FOMNInput.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMNInput", "HUMNin", "MgN/ha/period", 0.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("Nmin", "Nmin", "MgN/ha/period", Nmin.ToString(), parens, 0);

            GlobalVars.Instance.writeCtoolFile("Org_N_leached", "Org_N_leached", "MgN/ha/period", Nleached.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("NStart", "NStart", "MgN/ha", NStart.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("Nend", "Nend", "MgN/ha", 0.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("FOMnmineralised", "FOMnmineralised", "MgN/ha/period", FOMnmineralised, parens, 1);
 
            GlobalVars.Instance.Ctoolheader = true;
        }
        double min = 999999;
        double max = 0;
        for (int j = 0; j < 12; j++)
        {
            if (meanTemperature[j] < min)
                min = meanTemperature[j];

            if (meanTemperature[j] > max)
                max = meanTemperature[j];
        }
        amplitude = (max - min) / 2;
        if (writeOutput)
        {
          //  GlobalVars.Instance.writerCtool;

            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("CropSeqID", "CropSeqID", "CropSeqID", CropSeqID, parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("startDay", "startDay", "day", startDay.ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("endDay", "endDay", "day", endDay.ToString(), parens, 0));

            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetFOMCStored", "Initial C FOM", "MgC/ha", GetFOMCStored().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetHUMCStored", "Initial C HUM", "MgC/ha", GetHUMCStored().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetROMCStored", "Initial C ROM", "MgC/ha", GetROMCStored().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetFOMn", "Initial N FOM", "MgN/ha", GetFOMn().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetHUMn", "Initial N HUM", "MgN/ha", GetHUMn().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetROMn", "Initial N ROM", "MgN/ha", GetROMn().ToString(), parens, 0));
        }

        double cumFOMnmineralised = 0;
        double cumhumificationAmount = 0;
        double totFOMCO2 = 0;

        //double totTransportOut = 0;
        for (int i = 0; i < iterations; i++)
        {
			if (julianDay >= 365)
                julianDay = 1;
            double JulianAsDouble=(double)julianDay;
            int month = (int) Math.Floor(JulianAsDouble / 30.4166)+1;
            /*double tmp = (i) / 12;
            int year = (int)Math.Floor(tmp) + 1;*/
            double juliandayCtool = month * 30.4166;
            double FOMtransportIn=0;
            double FOMtransportOut=0;
            double HUMtransportIn=0;
            double HUMtransportOut=0;
            double ROMtransportIn=0;
            double ROMtransportOut = 0;
            double startFOM = GetFOMCStored();
            double newFOM = 0;
            double cumFOMCO2 = 0;
            double newHUM = 0;
            double newROM = 0;
            double FOMmineralised = 0;
            for (int j = 0; j < numberOfLayers; j++)
            {
                FOMCInput += FOM_Cin[i, j];
                FOMcInput += FOM_Cin[i, j];
                HUMCInput += HUM_Cin[i, j];
                HUMcInput += HUM_Cin[i, j];
                CInput += FOMCInput + HUMCInput;
                layerDynamics(julianDay, j, meanTemperature[month-1], droughtIndex, FOMtransportIn, ref FOMtransportOut, 
                    ref FOMCO2, HUMtransportIn, ref HUMtransportOut, ref HUMCO2, ROMtransportIn, ref ROMtransportOut, ref ROMCO2,
                    ref newHUM, ref newROM);
                CO2Emission+=FOMCO2 + HUMCO2 + ROMCO2;
                FOMcCO2 += FOMCO2;
                HUMcCO2 += HUMCO2;
                ROMcCO2 += ROMCO2;
                FOMcToHUM += newHUM;
                HUMcToROM += newROM;
                cumFOMCO2 += FOMCO2;
                cumhumificationAmount += newHUM;
                FOMmineralised += FOMCO2 + newHUM;
                FOMtransportIn = FOMtransportOut;
                HUMtransportIn = HUMtransportOut;
                ROMtransportIn = ROMtransportOut;
                fomc[j] += FOM_Cin[i, j];
                humc[j] += HUM_Cin[i, j];
                newFOM += FOM_Cin[i, j];
            }
            totFOMCO2 += cumFOMCO2;
            //last value of C transport out equates to C leaving the soil
            double FOMntransportOut = (FOMtransportOut * FOMn) / GetFOMCStored();
           // totTransportOut += FOMntransportOut + ROMtransportOut + HUMtransportOut;
//            Nleached += FOMntransportOut + HUMtransportOut / CtoNHUM + ROMtransportOut / CtoNROM;
            Nleached = 0;
            if (startFOM > 0)
                FOMnmineralised = FOMmineralised * FOMn / startFOM;
            else
                FOMnmineralised = 0;
            cumFOMnmineralised += FOMnmineralised;
            FOMNInput += FOMnIn[i];
            double test = FOMCInput / FOMNInput;
            FOMn += FOMnIn[i] - FOMnmineralised ;
            test= GetFOMCStored() / FOMn;
            julianDay++;
        }
        double CEnd = GetCStored();
        Cchange = CEnd - CStart;
        double Nend = GetNStored();
        Nmin = NStart + FOMNInput - Nend - Nleached;

        balance = FOMcInput - (FOMcCO2  + GetFOMCStored() + FOMcToHUM);
        balance = HUMcInput + FOMcToHUM - (HUMcCO2 + GetHUMCStored() + HUMcToROM);
        balance = ROMcInput + HUMcToROM - ROMcCO2 - GetROMCStored();
        balance = CStart + FOMCInput + HUMCInput - (CO2Emission + CEnd);
        double HUMNInput = HUMCInput / CtoNHUM;
       // CheckCBalance();
        if (writeOutput)
        {
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("FOMCInput", "FOM_C_input", "", FOMCInput.ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("HUMCInput", "HUM_C_input", "", HUMCInput.ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("CO2Emission", "CO2_C_emission", "", CO2Emission.ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("balance", "balance", "", balance.ToString(), parens, 0));


            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetFOMCStored", "Final_C_FOM", "", GetFOMCStored().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetHUMCStored", "Final_C_HUM", "", GetHUMCStored().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetROMCStored", "Final_C_ROM", "", GetROMCStored().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetCStored", "Final_C_Total", "", GetCStored().ToString(), parens, 0));

            double finalN = GetFOMn() + GetHUMn() + GetROMn();

            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetFOMn", "Final_N_FOM", "", GetFOMn().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetHUMn", "Final_N_HUM", "", GetHUMn().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetROMn", "Final_N_ROM", "", GetROMn().ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("finalN", "Final_N_Total", "", finalN.ToString(), parens, 0));

            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("FOMNInput", "FOMNin", "", FOMNInput.ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("HUMNInput", "HUMNin", "", HUMNInput.ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("Nmin", "Nmin", "", Nmin.ToString(), parens, 0));

            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("Org_N_leached", "Org_N_leached", "", Nleached.ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("NStart", "NStart", "", NStart.ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("Nend", "Nend", "", Nend.ToString(), parens, 0));

            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("FOMnmineralised", "FOMnmineralised", "", FOMnmineralised, parens, 1));
                
        }
        return ctoolData;
    }
/**
     * Calculate Damping Depth
     */
    double CalcDampingDepth(double k, double rho)
    {
        return Math.Sqrt(2.0 * k / rho);
    }
/**
     * Get Temperature
     */
    public double Temperature(double avgTemperature, double day, double depth, double amplitude, double offset)
    {
        double rho = 3.1415926 * 2.0 / (365.0 * 24.0 * 3600.0);
        double Th_diff = 0.35E-6;
        double dampingDepth = CalcDampingDepth(Th_diff, rho);
        double retVal = avgTemperature + amplitude * Math.Exp(-depth / dampingDepth) * Math.Sin(rho * (day + offset) * 24.0 * 3600.0 - depth / dampingDepth);
        return retVal;
    }
/**
 * layer Dynamics
 */
    public void layerDynamics(int JulianDay, int layerNo, double temperature, double droughtCoefficient, double FOMtransportIn, 
        ref double FOMtransportOut, ref double FOMCO2, double HUMtransportIn, ref double HUMtransportOut, ref double HUMCO2,
        double ROMtransportIn, ref double ROMtransportOut, ref double ROMCO2, ref double newHUM, ref double newROM)
    {
        double CO2=0;
        double fomcStart = fomc[layerNo];
        double humcStart = humc[layerNo];
        double romcStart = romc[layerNo];
        bool zeroRatesForDebugging = false; //use true to help when debugging
        //tF = 0;
        double depthInLayer = (100.0) / numberOfLayers * layerNo + (100.0) / numberOfLayers/2;
  
        double temp =Temperature(temperature, JulianDay, depthInLayer, amplitude, offset);
        double tempCofficent = temperatureCoefficent(temp);
        double temporaryCoefficient = tempCofficent *(1 - droughtCoefficient);
        if (zeroRatesForDebugging)
            FOMdecompositionrate = 0;
        //do FOM
        double FomAfterDecom = rk4decay(fomc[layerNo], timeStep, FOMdecompositionrate, temporaryCoefficient);
        double remainingDegradedFOM=fomc[layerNo]-FomAfterDecom;
        FOMtransportOut = remainingDegradedFOM * tF;
        remainingDegradedFOM -= FOMtransportOut;
        
        //Jonas - this following calculation could be moved to Initialisation, since it need only be calculated once
        double Rfraction = R(Clayfraction);
        double humification = 1 / (Rfraction + 1);
        newHUM = remainingDegradedFOM * humification;
        FOMCO2 = remainingDegradedFOM * (1 - humification);
        CO2+=FOMCO2;
        double test = (fomc[layerNo] - FomAfterDecom) - (FOMtransportOut + newHUM + FOMCO2);
        fomc[layerNo] = FomAfterDecom + FOMtransportIn;
        if (layerNo == (numberOfLayers-1))
            fomc[layerNo] += FOMtransportOut;
        if (zeroRatesForDebugging)
            HUMdecompositionrate = 0;
        //do HUM
        double HumAfterDecom = rk4decay(humc[layerNo], timeStep, HUMdecompositionrate, temporaryCoefficient);
       
        double degradedHUM = humc[layerNo] - HumAfterDecom;
        newROM = ROMificationfraction* degradedHUM;
        HUMCO2 = fCO2 * degradedHUM;
        HUMtransportOut = degradedHUM * (1 - fCO2 - ROMificationfraction);
        CO2+=HUMCO2;
        double test2 = (humc[layerNo] - HumAfterDecom) - (HUMCO2 + HUMtransportOut + newROM);
        humc[layerNo] = HumAfterDecom + newHUM + HUMtransportIn;
        if (layerNo == (numberOfLayers-1))
            humc[layerNo] += HUMtransportOut;
        if (zeroRatesForDebugging)
            ROMdecompositionrate = 0;
        romc[layerNo] += newROM;
        //do ROM
        double RomAfterDecom = rk4decay(romc[layerNo], timeStep, ROMdecompositionrate, temporaryCoefficient);
      
        double degradedROM = romc[layerNo] - RomAfterDecom;

        ROMCO2= fCO2 * degradedROM;
        ROMtransportOut = degradedROM * (1 - fCO2);
        romc[layerNo] = RomAfterDecom + ROMtransportIn;
        double balance1 = fomcStart + FOMtransportIn - (fomc[layerNo] + FOMCO2 + FOMtransportOut + newHUM);
        double balance2 = humcStart + HUMtransportIn + newHUM - (humc[layerNo] + HUMCO2 + HUMtransportOut + newROM);
        double balance3 = romcStart + ROMtransportIn + newROM- (romc[layerNo] + ROMCO2 + ROMtransportOut);
        if (layerNo == (numberOfLayers-1))
            romc[layerNo] += ROMtransportOut;
    }
/**
* Calculate R
*/
    private double R(double Clayfraction)
    {
        return 1.67 * (1.85 + 1.6 * Math.Exp(-7.86 * Clayfraction));
    }
/**
* temperature Coefficent
*/
    private double temperatureCoefficent(double temperature)
    {
	    return 7.24*Math.Exp(-3.432+0.168*temperature*(1-0.5*temperature/36.9)); 
    }
/**
* 
*/
    private  double func (double amount,double coeff)
    {
        return amount*-coeff;
    }
/**
* runka kutta
*/
    private  double  rk4decay ( double u0,double dt,double k, double temporaryCoefficient)
    {
        double coeff = k * temporaryCoefficient;
        double f1 = func(u0, coeff);
        double f2 = func(u0 + dt * f1 / 2,coeff);
        double f3 = func(u0 + dt * f2 / 2, coeff);
        double f4 = func(u0 + dt * f3, coeff);
        double retVal = u0 + dt * ( f1 + 2.0 * f2 + 2.0 * f3 + f4 ) / 6.0;
        return retVal;
    }
/**
* Get C to N Factor
*/
    public double GetCtoNFactor(double CtoNratio)
    {
        double retVal = Math.Min(56.2 * Math.Pow(CtoNratio,- 1.69), 1.0);
        return retVal;
    }
/**
* Write output
*/
    public void Write()
    {

        GlobalVars.Instance.writeStartTab("Ctool2");
        GlobalVars.Instance.writeInformationToFiles("timeStep", "Timestep", "Day", timeStep, parens);
        GlobalVars.Instance.writeInformationToFiles("numberOfLayers", "Number of soil layers", "-", numberOfLayers, parens);
        GlobalVars.Instance.writeInformationToFiles("FOMdecompositionrate", "FOM decomposition rate", "per day", FOMdecompositionrate, parens);
        GlobalVars.Instance.writeInformationToFiles("HUMdecompositionrate", "HUM decomposition rate", "per day", HUMdecompositionrate, parens);
        GlobalVars.Instance.writeInformationToFiles("ROMdecompositionrate", "ROM decomposition rate", "per day", ROMdecompositionrate, parens);
        GlobalVars.Instance.writeInformationToFiles("ROMificationfraction", "ROMification rate", "per day", ROMificationfraction, parens);
        GlobalVars.Instance.writeInformationToFiles("fCO2", "fCO2", "-", fCO2, parens);
        GlobalVars.Instance.writeInformationToFiles("GetFOMCStored", "GetFOMCStored", "-", GetFOMCStored(), parens);
        GlobalVars.Instance.writeInformationToFiles("GetHUMCStored", "GetHUMCStored", "-", GetHUMCStored(), parens);
        GlobalVars.Instance.writeInformationToFiles("GetROMCStored", "GetROMCStored", "-", GetROMCStored(), parens);
        GlobalVars.Instance.writeInformationToFiles("GetFOMn", "GetFOMn", "-", GetFOMn(), parens);
        GlobalVars.Instance.writeInformationToFiles("GetHUMn", "GetHUMn", "-", GetHUMn(), parens);
        GlobalVars.Instance.writeInformationToFiles("GetROMn", "GetROMn", "-", GetROMn(), parens);
        GlobalVars.Instance.writeEndTab();
       
    }
}