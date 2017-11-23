using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestCTool
{
    class TestCTool
    {
        static void Main(string[] args)
        {
            ctool2 aModel = new ctool2();
            double initialC = 12;//kg C per sq metre
            double initialFOMn = initialC * 0.05 / 10;
            aModel.Initialisation(initialC,initialFOMn,0.4,0.55);
            double initialN = aModel.GetNStored();
            aModel.CheckCBalance();

            int startMonth=1;
            int endMonth=12;
            double[,] FOMin = new double[12,4];
            double[,] HUMin = new double[12,4];
            double[] cultivation = new double[12];
            double[] fomnIn = new double[12];
            double meanTemperature = 12.0;
            for (int i = 0; i < 12; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    FOMin[i,j] = 0;
                    HUMin[i, j] = 0;
                }
                fomnIn[i] = 0;
            }
            FOMin[6,0] = 0.03;
            fomnIn[6] = 0.003;
            double initCStored = aModel.GetCStored();
            for (int j = 0; j < 100; j++)
            {
                double Cchange = 0;
                double Nmin = 0;
                double Nleached = 0;
                double CO2Emission = 0;
                for (int i = 0; i < 12; i++)
                {
                    aModel.Dynamics(startMonth, endMonth, FOMin, HUMin, fomnIn, cultivation, meanTemperature, ref Cchange, ref CO2Emission,
                        ref Nmin, ref Nleached);
                    aModel.CheckCBalance();
                    //aModel.CheckNBalance();
                }
                Console.WriteLine(j.ToString() + " Cch " + Cchange.ToString() + " Nmin " + Nmin.ToString() + " FOMn " + aModel.GetFOMn().ToString());
            }
            Console.WriteLine("Initial C " + initCStored.ToString() + " Initial N " + initialN);
            double CStored = aModel.GetCStored();
            double NStored = aModel.GetNStored();
            Console.WriteLine("Final C " + CStored.ToString() + " Final N " + NStored);
            aModel.GetCDetails();
        }
    }
}
