using System;

public class NixCSharpBits
{
    const double PI = 3.1415926;
    const double VonKarman = 0.41;
    const double StephanBoltzmann = 5.67E-8; //W⋅m−2⋅K−4
    const double N_to_protein = 6.25;
    const double StandardAirPressure = 101325;      // Pa
    const double GrossEnergyinDryMatter = 18.4; //MJ kg-1
    const double HeatCapacityFreezingWater = 334400.0;    // J/kg
    const double HeatCapacityWater = 4192.0;      // J/kgC
    const double HeatCapacityIce = 2000.0;      // J/kgC
    const double HeatCapacitySolid = 750.0;       // J/kgC
    const double WaterDensity = 1000.0;      // kg/m3
    const double SpecificGasConstant = 287.058; // J/(kg*K)

    public double GetgravitationalAcceleration()
    { return 9.81; }

    public double GetdynamicViscosityAir()
    { return 1.72 * 1E-5; }

    public double GetGasConstant()
    { return 8.31; }   // K-1 mol-1}

    public double  GetCconcInOrgmatter()
        {return 0.52;}

    public double GetStandardAirPressure()
    { return StandardAirPressure; }

    public double GetFrictionVelocity(double height,
                           double displacementHt,
                           double zeroPlane,
                           double windVelocity)
    {return (windVelocity * VonKarman) / (Math.Log((height - displacementHt) / zeroPlane));}

    /*
     *  CUBIC.C - Solve a cubic polynomial
     *  public domain by Ross Cottrell
     */
    //! Returns the roots of a cubic function with form ax3 + bx2 + cx + d. There maybe one or three valid roots.
    /*!
     \param a double coefficient of cubic term
    \param b double coefficient of quadratic term
    \param c double coefficient of linear term
      \param d double constant term
    \param solutions double number of valid roots
    \param x array of double containing the roots
*/
    public void SolveCubic(double a, double b, double c, double d, ref int solutions, ref double[] x)
    {
        double a1 = b / a,
                    a2 = c / a,
                    a3 = d / a;
        double Q = (a1 * a1 - 3.0 * a2) / 9.0;
        double R = (2.0 * a1 * a1 * a1 - 9.0 * a1 * a2 + 27.0 * a3) / 54.0;
        double R2_Q3 = R * R - Q * Q * Q;
        double theta;

        if (R2_Q3 <= 0)
        {
            solutions = 3;
            theta = Math.Acos(R / Math.Sqrt(Q * Q * Q));
            x[0] = -2.0 * Math.Sqrt(Q) * Math.Cos(theta / 3.0) - a1 / 3.0;
            x[1] = -2.0 * Math.Sqrt(Q) * Math.Cos((theta + 2.0 * PI) / 3.0) - a1 / 3.0;
            x[2] = -2.0 * Math.Sqrt(Q) * Math.Cos((theta + 4.0 * PI) / 3.0) - a1 / 3.0;
        }
        else
        {
            solutions = 1;

            double inputOne = Math.Sqrt(R2_Q3) + Math.Abs(R);
            double inputTwo = 1 / 3.0;

            x[0] = Math.Pow(inputOne, inputTwo);
            x[0] += Q / x[0];
            x[0] *= (R < 0.0) ? 1 : -1;
            x[0] -= a1 / 3.0;
        }
    }
//Returns the specific heat capacity of air in Joules
    public double GetspecificHeatCapAir(double waterVapourPressure, double absoluteTemperture)
    {
        double Celsius = absoluteTemperture - 273.13;
        double ret_val = 1000 * (1.007 * Celsius - 0.026) + (GetdensityWaterVapour(waterVapourPressure, absoluteTemperture) * (2501 + 1.84 * Celsius));
        return 1005.0;
    }

    // J per K per kg - specific heat capacity of dry air
    public double GetdensityDryAir(double airPressure, double airTemperature)    // air pressure in Pa, temperature in K
    {
            double ret_val = 0;
            if (airTemperature < 173) //temperature has been input in Celsius
                airTemperature += 273.3;
            if (airPressure < 150.0) //air pressure has been input in kPa not Pa
                airPressure *= 1000.0;

            ret_val=airPressure / (SpecificGasConstant * airTemperature);
            return ret_val;
        }

        // kg per cubic meter - density of air
        public double GetdensityAir(double airPressure, double temperature, double vapourPressure)
    {
            double ret_val = 0;
            airPressure /= 1000.0;
            vapourPressure /= 1000.0;
            ret_val = GetdensityDryAir(airPressure, temperature) * (1 - (vapourPressure * (1 - 0.622) / airPressure));
        return ret_val;
    }

    public double GetdensityWaterVapour(double waterVapourPressure, double absoluteTemperture)
    {
        double ret_val = 0;
        ret_val = 0.0022 * waterVapourPressure / absoluteTemperture;
        return ret_val;
    }

        /*public double GetsaturatedVapourPressure(double Tdew, double humidity, double Tmin, double Tmax)
    {
            double ret_val = 0;
            if (Tdew != -99)
            {
                ret_val = 0.6108 * Math.Exp((17.27 * Tdew) / (Tdew + 237.3));
            }
            if (humidity!=0)
            { 
                ret_val = humidity / 100.0 * ((e(Tmin) + e(Tmax)) / 2);
            }
            return ret_val;
        }*/
    // air temperature in Cel
    public double GetsaturatedWaterVapourPressure(double airTemperature)    // returns value in Pa
{
        if (airTemperature>173) //temperature has been input in Kelvin
            airTemperature -= 273.3;
        double ret_val;

        ret_val = 610.78 * Math.Exp((17.269 * airTemperature) / (airTemperature + 237.3));
        if (airTemperature < 0.0)
            ret_val = -4.86 + 0.855 * ret_val + 0.000244 * Math.Pow(ret_val, 2);
   
    return ret_val;

}

    public double GetSkyTemperature(double airTemperature)
    {
        double ret_val = 0;
        ret_val = 0.0552 * Math.Pow(airTemperature, 1.5); //Swinbank 1963 Long wave radiation from clear skies, Quart. J. Roy. Meteorol. Soc., 89
        return ret_val;
    }

    public double GetSkyEmissivity(double theairTemperature, double thewaterVapourPressure)  //Brutsaert, W. H. (1975), On a derivable formula for long-wave radiation from clear skies, Water Resour.Res., 11, 742 – 744, doi:10.1029/WR011i005p00742.
    {
        double ret_val = 1.24 * Math.Pow((thewaterVapourPressure/100) / theairTemperature, (1.0/ 7.0));
        return ret_val;
    }

public double GetlongWaveDown(double theairTemperature, double thewaterVapourPressure)
{
        double ret_val= GetSkyEmissivity(theairTemperature,thewaterVapourPressure) * StephanBoltzmann * Math.Pow(theairTemperature, 4);
        return ret_val;
}

public double GetlongWaveUp(double emissivity,
                     double aTemperature)
{
    return emissivity * StephanBoltzmann * Math.Pow(aTemperature, 4);
}

/*
 * Returns root of a quadratic
 */
public bool SolveQuadratic(bool posRoot,double a,double b,double c, ref double x)
{
    if ((b * b - 4 * a * c) < 0)
    {
        return false;
    }
    else if (posRoot)
    {
        x = (-b + Math.Sqrt(b * b - 4 * a * c)) / (2 * a);
    }
    else
    {
        x = (-b - Math.Sqrt(b * b - 4 * a * c)) / (2 * a);
    }

    return true;
}

// in kJ per kg
public double GetLatentHeatVaporisationWater(double temperature)
{
    return -0.0000614342 * Math.Pow(temperature, 3) + 0.00158927 * Math.Pow(temperature, 2) - 2.36418 * temperature + 2500.79;
}


public double GetWindAtHeight(double origHt,
                       double origWind,
                       double newHt,
                       double roughnessLength)
{
    return origWind * Math.Log(newHt / roughnessLength) / Math.Log(origHt / roughnessLength);
}

public double GetGrossEnergyinDryMatter()
{
    return 18.4;
}

    //MJ per kg

public double GetNetLongWave(double airTemperature, double waterVapourPressure,double surfaceTemp,double emissivity)
    {
        double longWaveDown = GetlongWaveDown(airTemperature,waterVapourPressure);    // longwave radiation input to surface, Watts per square metre
        double longWaveUp = GetlongWaveUp(emissivity,surfaceTemp);    // longwave radiation output of surface, Watts per square metre; note small error here, if surface temperature not equal to air temperature
        double longWave = longWaveDown - longWaveUp;    // net longwave energy exchange, Watts per square metre
        return longWave;
    }




}
