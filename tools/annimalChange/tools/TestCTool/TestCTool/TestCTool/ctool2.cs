using System;

public class ctool2
{

    double offset = 0;
    double amplitude = 0;
    int numberOfLayers = 0;
    //
    double FOMdecompositionrate = 0;
    double HUMdecompositionrate = 0;
    double Clayfraction = 0;
    double tF = 0;
    double ROMificationfraction =0;
    double ROMdecompositionrate = 0;
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
    double CtoNHUM = 10.0;
    double CtoNROM = 10.0;
    double FOMNInput = 0;
    double Nlost = 0;

    public ctool2()
    {
    }

    public double GetFOMn() { return FOMn; }

    public void Initialisation(double initialC, double initialFOMn, double propHUM, double propROM)
    {
        offset = 0;
        maxSoilDepth = 1.0;
        amplitude = 0;
        numberOfLayers = 4;
        timeStep = 0.083;
        layerThickness=maxSoilDepth/numberOfLayers;
        //
        FOMdecompositionrate =1.44;
        HUMdecompositionrate = 0.03;
        Clayfraction = 0.2;
        tF = 0.03;
        ROMdecompositionrate = 4.63*Math.Pow(10-0,-4.0);
        fCO2 = 0.628;
        fomc = new double[numberOfLayers];
        humc = new double[numberOfLayers];
        romc = new double[numberOfLayers];
        for (int j = 0; j < numberOfLayers; j++)
        {
            fomc[j] = (1 - (propHUM + propROM))* initialC/numberOfLayers;
            romc[j] = propROM * initialC/numberOfLayers;
            humc[j] = propHUM * initialC/numberOfLayers;
        }
        CInput = initialC;
        FOMn = initialFOMn;
    }

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
            Console.WriteLine(j.ToString() + " " + fomc[j].ToString() + " " + humc[j].ToString() + " " +romc[j].ToString());
    }



    public void CheckCBalance()
    {
        double Cstored = GetCStored();
        double CBalance = CInput - (Cstored + totalCO2Emission);
        double diff = CBalance / CInput;
        if (Math.Abs(diff) > 0.05)
        {
            double errorPercent = 100 * diff;
            //if (GlobalVars.Instance.getstopOnError())
            {
                Console.WriteLine("Error; C balance in C-Tool");
                Console.Write("Percentage error = " + errorPercent.ToString("0.00") + "%");
                Console.Read();
                Environment.Exit(0);
            }
        }

    }

    /**
     * @param startMonth first month of period
     * @param endMonth last month of period
     * @param FOMin array [month, layer] of fresh organic matter input (kg/ha)
     * @param HUMin array [month, layer] of humic organic matter input (kg/ha)
     * @param FOMnin array [month] of N input in fresh organic matter (kg/ha)
     * @param cultivation array [month] of depth of cultivation (m) (not used yet)
     * @param meanTemperature mean air temperature for the agroecological zone (Celcius)
     * @param Cchange change in carbon in the soil over the period (kg) 
     * @param Nmin mineralisation of soil N over the period (kg) (negative if N is immobilised)
     * @param Nleached N leached from the soil in organic matter (kg)
     */
    public void Dynamics(int startMonth, int endMonth, double[,] FOMin, double[,] HUMin, double [] FOMnIn, 
        double[] cultivation, double meanTemperature, ref double Cchange, ref double CO2Emission, ref double Nmin, ref double Nleached)
    {
        Cchange = 0;
        Nmin = 0;
        FOMNInput = 0;
        double FOMnmineralised = 0;
        double CStart = GetCStored();
        double NStart = GetNStored();
        double startHUM = GetHUMCStored();
        double startROM = GetROMCStored();
        double FOMCO2 = 0;
        double HUMCO2 = 0;
        double ROMCO2 = 0;

        int iterations = endMonth-startMonth;

        for (int i = 0; i < iterations; i++)
        {
            int month = i % 12 + 1;
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
                fomc[j] += FOMin[month,j];
                humc[j] += HUMin[month,j];
                CInput += FOMin[month,j] + HUMin[month,j];
                layerDynamics(month, j, meanTemperature, FOMtransportIn, ref FOMtransportOut, ref FOMCO2, HUMtransportIn, 
                    ref HUMtransportOut, ref HUMCO2, ROMtransportIn, ref ROMtransportOut, ref ROMCO2);
                totalCO2Emission += FOMCO2 + HUMCO2 + ROMCO2;
                cumFOMCO2 += FOMCO2;
                FOMtransportIn = FOMtransportOut;
                HUMtransportIn=HUMtransportOut;
                ROMtransportIn = ROMtransportOut;
            }
            //last value of C transport out equates to C leaving the soil
            totalCO2Emission += FOMtransportOut + HUMtransportOut + ROMtransportOut;
            double FOMntransportOut=(FOMtransportOut * FOMn)/GetFOMCStored();
            Nleached += FOMntransportOut + HUMtransportOut/CtoNHUM + ROMtransportOut/CtoNROM;
            FOMnmineralised = (cumFOMCO2/startFOM) * FOMn;
            FOMNInput += FOMnIn[i];
            FOMn += FOMnIn[i] - (FOMnmineralised + FOMntransportOut);
//            NInput += FOMnIn[i];
        }
        CO2Emission = totalCO2Emission;
        double CEnd = GetCStored();
        Cchange = CEnd - CStart;
        double Nend = GetNStored();
        Nmin = NStart + FOMNInput - Nend - Nleached;
        //Nlost += Nleached;
    }

public void layerDynamics(int month,int layerNo, double temperature, double FOMtransportIn, ref double FOMtransportOut, ref double FOMCO2,
    double HUMtransportIn, ref double HUMtransportOut, ref double HUMCO2, double ROMtransportIn, ref double ROMtransportOut, ref double ROMCO2)
{
    double CO2=0;
    double day=month * 30.4 + 15;
    int JulianDay = (int) Math.Round(day);
    double depthInLayer =  layerThickness* layerNo + (layerThickness) / 2;
    double tempCofficent = temperatureCoefficent(Temperature(temperature, JulianDay, depthInLayer, amplitude, offset));

    //do FOM
    double FomAfterDecom = rk4decay(0, fomc[layerNo], timeStep, @func, FOMdecompositionrate, tempCofficent);
    double remainingDegradedFOM=fomc[layerNo]-FomAfterDecom;
    FOMtransportOut = remainingDegradedFOM * tF;
    remainingDegradedFOM-=FOMtransportOut;
    double Rfraction = R(Clayfraction);
    double humification = 1 / (Rfraction + 1);
    double humificationAmount = remainingDegradedFOM * humification;
    FOMCO2 = remainingDegradedFOM * (1 - humification);
    CO2+=FOMCO2;
    fomc[layerNo] = FomAfterDecom +FOMtransportIn;
    
    //do HUM
    double HumAfterDecom = rk4decay(0, humc[layerNo], timeStep, @func, HUMdecompositionrate, tempCofficent);
    double degradedHUM=humc[layerNo]-HumAfterDecom;
    double newROM = ROMificationfraction*degradedHUM;
    HUMCO2= fCO2 * degradedHUM;
    HUMtransportOut = degradedHUM - (newROM+HUMCO2);;
    CO2+=HUMCO2;
    humc[layerNo] = HumAfterDecom + humificationAmount + HUMtransportIn;
    
    //do ROM
    double RomAfterDecom = rk4decay(0, romc[layerNo], timeStep, @func, ROMdecompositionrate, tempCofficent);
    double degradedROM=romc[layerNo]-RomAfterDecom;
    ROMCO2= fCO2 * degradedROM;
    ROMtransportOut = degradedROM -ROMCO2;
    romc[layerNo] = RomAfterDecom + newROM + ROMtransportIn;
}
    private double R(double Clayfraction)
    {
        return 1.67 * (1.85 + 1.6 * Math.Exp(-7.86 * Clayfraction));
    }

    
    private double Temperature(double avgTemperature,int day,double depth,double amplitude,double offset)
    {
    double rho=3.1415926*2.0/(365.0*24.0*3600.0);
    double Th_diff = 0.35E-6;
    double dampingDepth = CalcDampingDepth(Th_diff, rho);
    double retVal = avgTemperature + amplitude * Math.Exp(-depth / dampingDepth) * Math.Sin(rho * (day + offset) * 24.0 * 3600.0 - depth / dampingDepth);
        return retVal;
    }

    private double CalcDampingDepth(double k, double rho)
    {

        return Math.Sqrt(2.0 * k / rho);

    }
    private double temperatureCoefficent(double temperature)
    {
	    return 7.24*Math.Exp(-3.432+0.168*temperature*(1-0.5*temperature/36.9)); 
    }
    private  double func (double time,double amount,double k,double tempCoefficient)
    {
        return amount*-k*tempCoefficient*time;
    }
    private  double  rk4decay ( double t0,double u0,double dt,Func<double,double, double,double,double> f ,double k, double tempCoefficient)
    {
        double f1 = f ( t0,          u0,k,tempCoefficient );
        double f2 = f(t0 + dt / 2, u0 + dt * f1 / 2, k, tempCoefficient);
        double f3 = f(t0 + dt / 2, u0 + dt * f2 / 2, k, tempCoefficient);
        double f4 = f(t0 + dt, u0 + dt * f3, k, tempCoefficient);
        return u0 + dt * ( f1 + 2.0 * f2 + 2.0 * f3 + f4 ) / 6.0;
    }

    public double GetCtoNFactor(double CtoNratio)
    {
        double retVal = Math.Min(56.2 * Math.Pow(CtoNratio,- 1.69), 1.0);
        return retVal;
    }
}