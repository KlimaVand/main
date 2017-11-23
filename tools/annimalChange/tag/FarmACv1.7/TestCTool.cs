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
            double Cchange = 0;
            double Nchange = 0;
            for (int j = 0; j < 100; j++)
            {
                for (int i = 0; i < 12; i++)
                {
                    aModel.Dynamics(startMonth, endMonth, FOMin, HUMin, fomnIn, cultivation, meanTemperature, ref Cchange, ref Nchange);
                    aModel.CheckCBalance();
                }
                Console.WriteLine(j.ToString() + " " + Cchange.ToString());
            }
            Console.WriteLine("Initial C " + initCStored.ToString());
            double CStored = aModel.GetCStored();
            Console.WriteLine("Final C " + CStored.ToString());
            aModel.GetCDetails();
        }
    }
}
