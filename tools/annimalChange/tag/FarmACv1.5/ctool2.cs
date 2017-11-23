using System;
using System.Xml;
using System.Xml.Linq;

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
    double[] fomc;
    double[] humc;
    double[] romc;
    double layerThickness=0;
    double maxSoilDepth = 0;
    double timeStep = 0;
    double fCO2 = 0;
    double totalCO2Emission = 0;
    double CInput = 0;
    double FOMn = 0;
    double FOMNInput = 0;
    double Nlost = 0;
    int offset = 0;
    double amplitude = 0;
    double dampingDepth = 0;
    string parens;
    private bool pauseBeforeExit = false;

    public ctool2(string aParens)
    {
        parens = aParens;
    }

    public double GetFOMn() { return FOMn; }
    public void SetpauseBeforeExit(bool aVal) { pauseBeforeExit = aVal; }
    double CN(double cn)
    {

        return Math.Min(56.2 * Math.Pow(cn, -1.69), 1);
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
    public void Initialisation(double ClayFraction, int offsetIn, double amplitudeIn, double maxSoilDepthIn, double dampingDepthIn,
        double initialC, string parameterFileName, string errorFileName, double InitialCtoN, double pHUMupperLayer, double pHUMLowerLayer)
    {

        offset = offsetIn;
        amplitude = amplitudeIn;
        maxSoilDepth = maxSoilDepthIn;
        dampingDepth = dampingDepthIn;
        Th_diff = 0.35E-6;
        
        FileInformation ctoolInfo = new FileInformation(parameterFileName);
        ctoolInfo.setPath("constants(0).C-Tool(-1).timeStep(-1)");
        timeStep = ctoolInfo.getItemDouble("Value"); //one day pr year
        numberOfLayers = 2;
        layerThickness = maxSoilDepth / numberOfLayers;
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "FOMdecompositionrate";
        FOMdecompositionrate = ctoolInfo.getItemDouble("Value");
        //FOMdecompositionrate = 0;
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "HUMdecompositionrate";
        HUMdecompositionrate = ctoolInfo.getItemDouble("Value");
        //HUMdecompositionrate = 0;
        this.Clayfraction = ClayFraction;
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "transportCoefficient";
        tF = ctoolInfo.getItemDouble("Value");
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "ROMdecompositionrate";
        ROMdecompositionrate = ctoolInfo.getItemDouble("Value");
       //ROMdecompositionrate = 0;
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "fCO2";
        fCO2 = ctoolInfo.getItemDouble("Value");
        ctoolInfo.PathNames[ctoolInfo.PathNames.Count - 1] = "ROMificationfraction";
        ROMificationfraction = ctoolInfo.getItemDouble("Value");

        fomc = new double[numberOfLayers];
        humc = new double[numberOfLayers];
        romc = new double[numberOfLayers];

        double CNfactor = CN(InitialCtoN);
        double fractionCtopsoil = 0.47;
        humc[0] = initialC * pHUMupperLayer * CNfactor * fractionCtopsoil;
        romc[0] = initialC * fractionCtopsoil - humc[0];
        humc[1] = initialC * pHUMLowerLayer * CNfactor * (1 - fractionCtopsoil);
        romc[1] = initialC * (1 -fractionCtopsoil) - humc[1];

        CInput = humc[0] + humc[1] + romc[0] + romc[1];
        CInput = initialC;
        FOMn = 0;
    }
    public ctool2(ctool2 C_ToolToCopy)
    {
        Th_diff = C_ToolToCopy.Th_diff;
        FOMdecompositionrate = C_ToolToCopy.FOMdecompositionrate;
        HUMdecompositionrate = C_ToolToCopy.HUMdecompositionrate;
        Clayfraction = C_ToolToCopy.Clayfraction;
        tF = C_ToolToCopy.tF;
        ROMificationfraction = C_ToolToCopy.ROMificationfraction;
        ROMdecompositionrate = C_ToolToCopy.ROMdecompositionrate;
        layerThickness = C_ToolToCopy.layerThickness;
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
    
    public void CopyCTool(ctool2 C_ToolToCopy)
    {
        Th_diff = C_ToolToCopy.Th_diff;
        FOMdecompositionrate = C_ToolToCopy.FOMdecompositionrate;
        HUMdecompositionrate = C_ToolToCopy.HUMdecompositionrate;
        Clayfraction = C_ToolToCopy.Clayfraction;
        tF = C_ToolToCopy.tF;
        ROMificationfraction = C_ToolToCopy.ROMificationfraction;
        ROMdecompositionrate = C_ToolToCopy.ROMdecompositionrate;
        layerThickness = C_ToolToCopy.layerThickness;
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

    public int GetnumOfLayers() { return numberOfLayers; }

    public double GetlayerThickness() { return layerThickness; }

    public double GetCStored()
    {
        double Cstored = 0;
        for (int j = 0; j < numberOfLayers; j++)
            Cstored += fomc[j] + humc[j] + romc[j];
        return Cstored;
    }

    public double GetFOMCStored()
    {
        double FOMCstored = 0;
        for (int j = 0; j < numberOfLayers; j++)
            FOMCstored += fomc[j];
        return FOMCstored;
    }

    public double GetHUMCStored()
    {
        double HUMCstored = 0;
        for (int j = 0; j < numberOfLayers; j++)
            HUMCstored += humc[j];
        return HUMCstored;
    }

    public double GetROMCStored()
    {
        double ROMCstored = 0;
        for (int j = 0; j < numberOfLayers; j++)
            ROMCstored += romc[j];
        return ROMCstored;
    }

    public double GetNStored()
    {
        double Nstored = FOMn;
        for (int j = 0; j < numberOfLayers; j++)
            Nstored += (romc[j]/CtoNROM) + (humc[j]/CtoNHUM);
        return Nstored;
    }

    public double GetHUMn() { return GetHUMCStored() / CtoNHUM; }

    public double GetROMn() { return GetROMCStored() / CtoNROM; }

    public void GetCDetails()
    {
        for (int j = 0; j < numberOfLayers; j++)
            GlobalVars.Instance.log(j.ToString() + " " + fomc[j].ToString() + " " + humc[j].ToString() + " " +romc[j].ToString(),5);
    }



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
        double[] cultivation, double meanTemperature, double droughtIndex, ref double Cchange, ref double CO2Emission, 
        ref double Cleached, ref double Nmin, ref double Nleached)
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
            GlobalVars.Instance.writeCtoolFile("startDay", "startDay", "day", startDay.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("endDay", "endDay", "day", endDay.ToString(), parens, 0);

            GlobalVars.Instance.writeCtoolFile("FOMCStored", "Initial C FOM", "kgC/ha", GetFOMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMCStored", "Initial C HUM", "kgC/ha", GetHUMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("ROMCStored", "Initial C ROM", "kgC/ha", GetROMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("FOMn", "Initial N FOM", "kgN/ha", GetFOMn().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMn", "Initial N HUM", "kgN/ha", GetHUMn().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("ROMn", "Initial N ROM", "kgN/ha", GetROMn().ToString(), parens, 0);

            GlobalVars.Instance.writeCtoolFile("FOMCInput", "FOM_C_input", "kgC/ha/period", FOMCInput.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMCInput", "HUM_C_input", "kgC/ha/period", HUMCInput.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("CO2Emission", "CO2_C_emission", "kgC/ha/period", CO2Emission.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("balance", "balance", "kgC/ha/period", balance.ToString(), parens, 0);


            GlobalVars.Instance.writeCtoolFile("FOMCStored", "Final_C_FOM", "kgC/ha", GetFOMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMCStored", "Final_C_HUM", "kgC/ha", GetHUMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("ROMCStored", "Final_C_ROM", "kgC/ha", GetROMCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("CStored", "Final_C_Total", "kgC/ha", GetCStored().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("FOMn", "Final_N_FOM", "kgN/ha", GetFOMn().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMn", "Final_N_HUM", "kgN/ha", GetHUMn().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("ROMn", "Final_N_ROM", "kgN/ha", GetROMn().ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("finalN", "Final_N_Total", "kgN/ha", 0.ToString(), parens, 0);

            GlobalVars.Instance.writeCtoolFile("FOMNInput", "FOMNin", "kgN/ha/period", FOMNInput.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("HUMNInput", "HUMNin", "kgN/ha/period", 0.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("Nmin", "Nmin", "kgN/ha/period", Nmin.ToString(), parens, 0);

            GlobalVars.Instance.writeCtoolFile("Org_N_leached", "Org_N_leached", "kgN/ha/period", Nleached.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("NStart", "NStart", "kgN/ha", NStart.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("Nend", "Nend", "kgN/ha", 0.ToString(), parens, 0);
            GlobalVars.Instance.writeCtoolFile("FOMnmineralised", "FOMnmineralised", "kgN/ha/period", FOMnmineralised, parens, 1);
            GlobalVars.Instance.Ctoolheader = true;
        }

        if (writeOutput)
        {
          //  GlobalVars.Instance.writerCtool;

            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("startDay", "startDay", "day", startDay.ToString(), parens, 0));
            ctoolData.Add(GlobalVars.Instance.writeCtoolFile("endDay", "endDay", "day", endDay.ToString(), parens, 0));

             ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetFOMCStored", "Initial C FOM", "", GetFOMCStored().ToString(), parens, 0));
             ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetHUMCStored", "Initial C HUM", "", GetHUMCStored().ToString(), parens, 0));
             ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetROMCStored", "Initial C ROM", "", GetROMCStored().ToString(), parens, 0));
             ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetFOMn", "Initial N FOM", "", GetFOMn().ToString(), parens, 0));
             ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetHUMn", "Initial N HUM", "", GetHUMn().ToString(), parens, 0));
             ctoolData.Add(GlobalVars.Instance.writeCtoolFile("GetROMn", "Initial N ROM", "", GetROMn().ToString(), parens, 0));
        }
           
        for (int i = 0; i < iterations; i++)
        {
            if (julianDay > 365)
                julianDay = 1;
            double FOMtransportIn=0;
            double FOMtransportOut=0;
            double HUMtransportIn=0;
            double HUMtransportOut=0;
            double ROMtransportIn=0;
            double ROMtransportOut = 0;
            double startFOMN = FOMn;
            double startFOM = GetFOMCStored();
            double cumFOMCO2 = 0;
            for (int j = 0; j < numberOfLayers; j++)
            {
                fomc[j] += FOM_Cin[i,j];
                humc[j] += HUM_Cin[i,j];
                FOMCInput += FOM_Cin[i, j];
                HUMCInput += HUM_Cin[i, j];
                CInput += FOMCInput + HUMCInput;
                layerDynamics(julianDay, j, meanTemperature, droughtIndex, FOMtransportIn, ref FOMtransportOut, ref FOMCO2, HUMtransportIn, 
                    ref HUMtransportOut, ref HUMCO2, ROMtransportIn, ref ROMtransportOut, ref ROMCO2);
                CO2Emission+=FOMCO2 + HUMCO2 + ROMCO2;
                cumFOMCO2 += FOMCO2;
                FOMtransportIn = FOMtransportOut;
                HUMtransportIn=HUMtransportOut;
                ROMtransportIn = ROMtransportOut;
            }
            //last value of C transport out equates to C leaving the soil
            double FOMntransportOut = (FOMtransportOut * FOMn) / GetFOMCStored();
            Cleached += FOMtransportOut + HUMtransportOut + ROMtransportOut;
            Nleached += FOMntransportOut + HUMtransportOut/CtoNHUM + ROMtransportOut/CtoNROM;
            if (startFOM != 0)
                FOMnmineralised = (cumFOMCO2 / startFOM) * FOMn;
            else
                FOMnmineralised = 0;
            FOMNInput += FOMnIn[i];
            
            FOMn += FOMnIn[i] - (FOMnmineralised + FOMntransportOut);
            julianDay++;
        }
        double CEnd = GetCStored();
        Cchange = CEnd - CStart;
        double Nend = GetNStored();
        Nmin = NStart + FOMNInput - Nend - Nleached;
      
            balance = CStart + FOMCInput + HUMCInput - (CO2Emission + CEnd);
            double HUMNInput = HUMCInput / CtoNHUM;
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

    public void layerDynamics(int JulianDay, int layerNo, double temperature, double droughtCoefficient, double FOMtransportIn, ref double FOMtransportOut, ref double FOMCO2,
    double HUMtransportIn, ref double HUMtransportOut, ref double HUMCO2, double ROMtransportIn, ref double ROMtransportOut, ref double ROMCO2)
    {
        double CO2=0;
        bool zeroRatesForDebugging = false; //use true to help when debugging
        //F = 0;
        double depthInLayer =  layerThickness* layerNo + (layerThickness) / 2;
        double temp = GlobalVars.Instance.Temperature(temperature, dampingDepth, JulianDay, depthInLayer, amplitude, offset);
        double tempCofficent = temperatureCoefficent(temp);
        double temporaryCoefficient = tempCofficent * (1 - droughtCoefficient);
        if (zeroRatesForDebugging)
            FOMdecompositionrate = 0;
        //do FOM
        double FomAfterDecom = rk4decay(0, fomc[layerNo], timeStep, @func, FOMdecompositionrate, temporaryCoefficient);
        FomAfterDecom = Math.Exp(-FOMdecompositionrate * timeStep * tempCofficent) * fomc[layerNo];
        double remainingDegradedFOM=fomc[layerNo]-FomAfterDecom;
        FOMtransportOut = remainingDegradedFOM * tF * timeStep;
        remainingDegradedFOM-=FOMtransportOut;
        //Jonas - this following calculation could be moved to Initialisation, since it need only be calculated once
        double Rfraction = R(Clayfraction);
        double humification = 1 / (Rfraction + 1);
        double humificationAmount = remainingDegradedFOM * humification;
        FOMCO2 = remainingDegradedFOM * (1 - humification);
        CO2+=FOMCO2;
        fomc[layerNo] = FomAfterDecom +FOMtransportIn;
        if (zeroRatesForDebugging)
            HUMdecompositionrate = 0;
        //do HUM
        double HumAfterDecom = rk4decay(0, humc[layerNo], timeStep, @func, HUMdecompositionrate, temporaryCoefficient);
        HumAfterDecom = Math.Exp(-HUMdecompositionrate * timeStep * tempCofficent) * humc[layerNo];
        double degradedHUM = humc[layerNo] - HumAfterDecom;
        double newROM = ROMificationfraction*degradedHUM;
        HUMCO2 = fCO2 * degradedHUM;
        //HUMCO2 = (1.0 - ROMificationfraction) * degradedHUM;
        HUMtransportOut = degradedHUM - (newROM + HUMCO2); ;
        CO2+=HUMCO2;
        humc[layerNo] = HumAfterDecom + humificationAmount + HUMtransportIn;
        if (zeroRatesForDebugging)
            ROMdecompositionrate = 0;
        //do ROM
        double RomAfterDecom = rk4decay(0, romc[layerNo], timeStep, @func, ROMdecompositionrate, temporaryCoefficient);
        RomAfterDecom = Math.Exp(-ROMdecompositionrate * timeStep * tempCofficent) * romc[layerNo];
        double degradedROM = romc[layerNo] - RomAfterDecom;

        ROMCO2= fCO2 * degradedROM;
        ROMtransportOut = degradedROM - ROMCO2;
        romc[layerNo] = RomAfterDecom + newROM + ROMtransportIn;
    }

    private double R(double Clayfraction)
    {
        return 1.67 * (1.85 + 1.6 * Math.Exp(-7.86 * Clayfraction));
    }

    private double temperatureCoefficent(double temperature)
    {
	    return 7.24*Math.Exp(-3.432+0.168*temperature*(1-0.5*temperature/36.9)); 
    }
    private  double func (double time,double amount,double k,double tempCoefficient)
    {
        return amount*-k*tempCoefficient*time;
    }
    private  double  rk4decay ( double t0,double u0,double dt,Func<double,double, double,double,double> f ,double k, double temporaryCoefficient)
    {
        double f1 = f ( t0,u0,k,temporaryCoefficient );
        double f2 = f(t0 + dt / 2, u0 + dt * f1 / 2, k, temporaryCoefficient);
        double f3 = f(t0 + dt / 2, u0 + dt * f2 / 2, k, temporaryCoefficient);
        double f4 = f(t0 + dt, u0 + dt * f3, k, temporaryCoefficient);
        return u0 + dt * ( f1 + 2.0 * f2 + 2.0 * f3 + f4 ) / 6.0;
    }

    public double GetCtoNFactor(double CtoNratio)
    {
        double retVal = Math.Min(56.2 * Math.Pow(CtoNratio,- 1.69), 1.0);
        return retVal;
    }
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
        GlobalVars.Instance.writeEndTab();
       
    }
}