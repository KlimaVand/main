using System;

public class ALFAM
{
    public double maxTime = 168.0;
    public double b_Nmx0 = -6.5757;
    public double b_sm1Nmx = 0.0971;
    public double b_atNmx = 0.0221;
    public double b_wsNmx = 0.0409;
    public double b_mt1Nmx = -0.156;
    public double b_mdmNmx = 0.1024;
    public double b_mtanNmx = -0.1888;
    public double b_ma0Nmx = 3.5691;
    public double b_ma1Nmx = 3.0198;
    public double b_ma2Nmx = 3.1592;
    public double b_ma3Nmx = 2.2702;
    public double b_ma4Nmx = 2.9582;
    public double b_mrNmx = -0.00433;
    public double b_mi0Nmx = 2.4291;
    public double b_met1Nmx = -0.6382;
    public double b_met2Nmx = -0.5485;
    public double b_Km0 = 0.037;
    public double b_sm1Km = 0.0974;
    public double b_atKm = -0.0409;
    public double b_wsKm = -0.0517;
    public double b_mt1Km = 1.3567;
    public double b_mdmKm = 0.1614;
    public double b_mtanKm = 0.1011;
    public double b_mrKm = 0.0175;
    public double b_met1Km = 0.3888;
    public double b_met2Km = 0.7024;
    public double applicRate = 0;
    public double exposureTime = 0;
    public double km = 0;
    public double Nmax = 0;
    public double TAN = 0;

    public ALFAM()
    {
    }

    /*
     * manureType: 0 = cattle, 1 = pig
     */
    public void initialise(int soilWet,
                           double aveAirTemp,
                           double aveWindspeed,
                           int manureType,
                           double initDM,
                           double initTAN,
                           double appRate,
                           int appMeth,
                            double anExposureTime)
    {
        TAN = initTAN;
        applicRate = appRate;
        exposureTime = anExposureTime;

        switch (appMeth)
        {
            case 1:    // broadcast
                Nmax = Math.Exp(b_Nmx0 + b_sm1Nmx * soilWet + b_atNmx * aveAirTemp + b_wsNmx * aveWindspeed
                           + b_mt1Nmx * manureType + b_mdmNmx * initDM + b_mtanNmx * TAN + b_ma0Nmx + b_mrNmx * appRate
                           + b_mi0Nmx + b_met2Nmx);
                km = Math.Exp(b_Km0 + b_sm1Km * soilWet + b_atKm * aveAirTemp + b_wsKm * aveWindspeed + b_mt1Km * manureType
                         + b_mdmKm * initDM + b_mtanKm * TAN + b_mrKm * appRate + b_met2Km);

                break;

            case 2:    // trailing hose
                Nmax = Math.Exp(b_Nmx0 + b_sm1Nmx * soilWet + b_atNmx * aveAirTemp + b_wsNmx * aveWindspeed
                           + b_mt1Nmx * manureType + b_mdmNmx * initDM + b_mtanNmx * TAN + b_ma1Nmx + b_mrNmx * appRate
                           + b_mi0Nmx + b_met2Nmx);
                km = Math.Exp(b_Km0 + b_sm1Km * soilWet + b_atKm * aveAirTemp + b_wsKm * aveWindspeed + b_mt1Km * manureType
                         + b_mdmKm * initDM + b_mtanKm * TAN + b_mrKm * appRate + b_met2Km);

                break;

            case 3:    // trailing shoe
                Nmax = Math.Exp(b_Nmx0 + b_sm1Nmx * soilWet + b_atNmx * aveAirTemp + b_wsNmx * aveWindspeed
                           + b_mt1Nmx * manureType + b_mdmNmx * initDM + b_mtanNmx * TAN + b_ma2Nmx + b_mrNmx * appRate
                           + b_mi0Nmx + b_met2Nmx);
                km = Math.Exp(b_Km0 + b_sm1Km * soilWet + b_atKm * aveAirTemp + b_wsKm * aveWindspeed + b_mt1Km * manureType
                         + b_mdmKm * initDM + b_mtanKm * TAN + b_mrKm * appRate + b_met2Km);

                break;

            case 4:    // open slot
                Nmax = Math.Exp(b_Nmx0 + b_sm1Nmx * soilWet + b_atNmx * aveAirTemp + b_wsNmx * aveWindspeed
                           + b_mt1Nmx * manureType + b_mdmNmx * initDM + b_mtanNmx * TAN + b_ma3Nmx + b_mrNmx * appRate
                           + b_mi0Nmx + b_met2Nmx);
                km = Math.Exp(b_Km0 + b_sm1Km * soilWet + b_atKm * aveAirTemp + b_wsKm * aveWindspeed + b_mt1Km * manureType
                         + b_mdmKm * initDM + b_mtanKm * TAN + b_mrKm * appRate + b_met2Km);

                break;

            case 5:    // closed slot
                Nmax = Math.Exp(b_Nmx0 + b_sm1Nmx * soilWet + b_atNmx * aveAirTemp + b_wsNmx * aveWindspeed
                           + b_mt1Nmx * manureType + b_mdmNmx * initDM + b_mtanNmx * TAN + b_ma4Nmx + b_mrNmx * appRate
                           + b_mi0Nmx + b_met2Nmx);
                km = Math.Exp(b_Km0 + b_sm1Km * soilWet + b_atKm * aveAirTemp + b_wsKm * aveWindspeed + b_mt1Km * manureType
                         + b_mdmKm * initDM + b_mtanKm * TAN + b_mrKm * appRate + b_met2Km);

                break;
        }
    }

    //returns proportion of TAN volatilised. Different from FASSET implementation
    public double ALFAM_volatilisation()
    {
      double ret_val = Nmax* exposureTime / (exposureTime+ km);
      return ret_val;
    }

    public int GetALFAMApplicCode(int OpCode)
    {
        switch (OpCode)
        {
            case 7:    // SpreadingLiquidManure
                return 1;
                break;

            case 8:    // ClosedSlotInjectingLiquidManure
                return 5;
                break;

            case 9:    // SpreadingSolidManure
                return 1;
                break;

            case 35:    // OpenSlotInjectingLiquidManure
                return 4;
                break;

            case 36:    // TrailingHoseSpreadingLiquidManure
                return 2;
                break;

            case 37:    // TrailingShoeSpreadingLiquidManure
                return 3;
                break;

            default:
                string theMessage="ALFAM: application method code not found";
                break;
        }

        return 0;
    }
}

