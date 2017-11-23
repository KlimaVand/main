using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace simpleModel
{
    class main
    {
        static void Main(string[] args)
        {
            simpleModel test = new simpleModel();
            string text = System.IO.File.ReadAllText(args[0]);
            string[] data = text.Split('\r');
            double N_total = Convert.ToDouble(data[0].Split('\t')[1]); 
            double Norg_share = Convert.ToDouble(data[1].Split('\t')[1]); 

       
            double CN2550 = Convert.ToDouble(data[2].Split('\t')[1]); 
            double a1 = Convert.ToDouble(data[3].Split('\t')[1]); 
            double a2 = Convert.ToDouble(data[4].Split('\t')[1]); 
            double a3 = Convert.ToDouble(data[5].Split('\t')[1]);
            double C25 = Convert.ToDouble(data[6].Split('\t')[1]); 
            double CN25 = Convert.ToDouble(data[7].Split('\t')[1]);
            double C2550 = Convert.ToDouble(data[8].Split('\t')[1]); 
            double Nr = Convert.ToDouble(data[9].Split('\t')[1]); 
            double Nc = Convert.ToDouble(data[10].Split('\t')[1]); 
         
            double Yp = Convert.ToDouble(data[11].Split('\t')[1]); 
            double Clay_5 = Convert.ToDouble(data[12].Split('\t')[1]); 
            double NRE = Convert.ToDouble(data[13].Split('\t')[1]); 
            double NRR =Convert.ToDouble(data[14].Split('\t')[1]);
            double NPCT = Convert.ToDouble(data[15].Split('\t')[1]); 
            double EP_season = Convert.ToDouble(data[16].Split('\t')[1]); 
            double P_season = Convert.ToDouble(data[17].Split('\t')[1]);
            double Temp = Convert.ToDouble(data[18].Split('\t')[1]); ;
            double C_resid = Convert.ToDouble(data[19].Split('\t')[1]); ; ;
            double D = Convert.ToDouble(data[20].Split('\t')[1]); ; ;
            double fx = Convert.ToDouble(data[21].Split('\t')[1]); ; ;
            double RTlogic = Convert.ToDouble(data[22].Split('\t')[1]); ; ;
            double ypl = Convert.ToDouble(data[23].Split('\t')[1]); ; ;
            Console.WriteLine(test.calculate(N_total, Norg_share, CN2550, a1, a2, a3, C25, CN25, C2550, Nr, Nc, Yp, Clay_5, NRE, NRR, NPCT, EP_season, P_season, Temp, ypl,C_resid, D, fx, RTlogic));
        }
    }
}
