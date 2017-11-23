using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace SimpleModel
{
   public class SimpleModel
   {
        public SimpleModel()
        {

        }
        public double calculate(double N_org,double N_fert,double CN3060,double a1,double a2,double a3,double C_30,double CN30, double C3060, double Nr,double Nc, double C_10,double Yp, double Clay_10, double NRE, double NRR, double NPCT,double  EP_season,double P_season)
        {
            double Ns=0.01*(0.3*C_30/CN30+0.3*C3060/CN3060)*10000*1.5*1000;
            double Nu=a1*Ns+a2*Nr+a3*Nc;
            double Yn=0.08*Nu;
            double N_total=N_org+N_fert;
            double F_w=0;
            double EPfactor=0;
                double EpP = EP_season / P_season;
                if (EpP > 1.5)
                {
                    EPfactor=0.9;
                }
                else
                {
                    EPfactor=1;
                }
            if (C_10 < 3)
                {
                    F_w = EPfactor + (1 - EPfactor) * C_10 / 3;
                }
                else
                {
                    F_w = 1;
                }
            double F_e=0;
                double Dexter=Clay_10/C_10;
                if(Dexter>=10)
                {
                   F_e=Math.Max(1-(Dexter-10)*0.005,0.95);
                }
                else
                {
                   F_e= 1;
                }
            double NUE = NRE * NRR / (NPCT * 10);
            double F_h = 0;
            if (C_10 > 3)
            {
                F_h = 1;
            }
            else
            {
                F_h = 0.95 + C_10 / 3 * 0.05;
            }
            double Yx = (F_h * F_e * F_w) * Yp;
            double NUT=(Yx-Yn)/NUE;
            if(N_total<NUT)
            {
                return Yn +NUE*N_total;
            }
            else
            {
               
               

                return (F_h * F_e * F_w) * Yp;
            }
        }
   }
}
