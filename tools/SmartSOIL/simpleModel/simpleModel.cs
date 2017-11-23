using System;
namespace simpleModel
{
    public class simpleModel
    {
        public simpleModel()
        {
        }
        public double calculate(double N_total, double Norg_share, double CN2550, double a1, double a2, double a3, double C25, double CN25, double C2550, double Nr, double Nc, double Yp, double Clay_5, double NRE, double NRR, double NPCT, double EP_season, double P_season, double Temp, double C_resid,double ypl, double D, double fx, double RTlogic)
//… og en integer variabel:  integer RTlogic  (som kan/skal antage værdien 0, 1 eller 2)    eller skal den bare være double..?


        {
         double C5 = 0;
         double fn5 = 1 + (fx - 1) * (Math.Min(ypl / 20, 1) * (1 - Math.Max((D - 5) / 20, 0)));  
            if (RTlogic >0)
                {
                    C5 = fn5 * C25;
                }
                else
                {
                    C5 = C25;
                }
         double Ns=0.01*(0.25*C25/CN25+0.25*C2550/CN2550)*10000*1.5*1000;
           double Corr_Temp =7.24*Math.Exp(-3.432+0.168*Temp*(1-0.5*Temp/36.9));
            double Nu= Corr_Temp* a1*Ns+a2*Nr+a3*Nc;
            double Yn = Nu/NPCT/10 ;
            
              double F_w=0;
               double EPfactor=0;
                double EpP = EP_season / (P_season+0.1);
                if (EpP > 1.2)
                {
                    EPfactor = Math.Max(0.7, 1 - (EpP - 1.2) * 0.2);
                }
                else
                {
                    EPfactor=1;
                }
            double F_w1=0;
            if (C25 < 3)
                {
                    F_w1 = Math.Min(1, EPfactor + (1 - EPfactor) * C25 / 3 * 1.5);
                }
                else
                {
                    F_w1 = 1;
                }
               double F_w2=0;
               double Surf_OM = C_resid * RTlogic*0.5;
             if (Surf_OM < 3)
             { 
             F_w2= EPfactor + (1-EPfactor)*Surf_OM/3;
              }  
              else
                {
                    F_w2 = 1;
                }
             F_w = Math.Min(F_w1, F_w2);



               double F_e=0;
                double Dexter=Clay_5/C5;
                if(Dexter>=10)
                {
                   F_e=Math.Max(1-(Dexter-10)*0.005,0.95);
                }
                else
                {
                   F_e= 1;
                }
         
            double F_h = 0;
            if (C5 > 3)
            {
                F_h = 1;
            }
            else
            {
                F_h = 0.95 + C5 / 3 * 0.05;
            }


         double F_till=0;
         if (RTlogic > 0)
             F_till = 1 - RTlogic * 0.05;
         else
         F_till = 1.0;



            double Yx = (F_h * F_e * F_w*F_till) * Yp;
            double N_org = Norg_share * N_total;
            double N_fert = N_total - N_org;
        
             return Math.Min(Yn+(NRE*(N_fert+NRR*N_org))/(10*NPCT), Yx);
            
        }
    }
}
