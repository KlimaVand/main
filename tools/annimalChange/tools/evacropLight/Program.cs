using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Xml.Linq;
using System.Diagnostics;
using simplesoilModel;
namespace AnimalChange
{
    class Program
    {
        
        static void Main(string[] args)
        {

            simpleSoil test = new simpleSoil();
            double[] orgc=new double[2];
            //organic C content of the topsoil and subsoil
            orgc[0]=74000.0;
            orgc[1]=121000.0;
            string[] param= new string[2];
            //replace by local folder name
            //param[0] = @"D:\main\tools\annimalChange\tag\FarmACv1.8\inputFiles\parameters.xml";
            //param[1] = @"D:\main\tools\annimalChange\tag\FarmACv1.8\inputFiles\parameters.xml";
            param[0] = @"C:\workspace\tools\annimalChange\tag\FarmACv1.8\inputFiles\parameters.xml";
            param[1] = @"C:\workspace\tools\annimalChange\tag\FarmACv1.8\inputFiles\parameters.xml";
            GlobalVars.Instance.setParamFilePath(param);
            // zone number, farm type
            GlobalVars.Instance.theZoneData.readZoneSpecificData(1, 1);
            GlobalVars.Instance.theZoneData.OpenDebugFile("C:/trash/output.txt");


            //zone number, soil type, max rooting depth (m), current rooting depth, LAI
            double maxRootDepth = 1.0;
            double currentRootDepth = 0;
            double LAI = 0;
            double soilDrought=0;
            double plantDrought=0;
            //GlobalVars.Instance.OpenDebugFile();
            test.Initialise2(1, 1, maxRootDepth, currentRootDepth, LAI, orgc);
            test.WriteDebugHeader();
            for (int month = 0; month < 11; month++)
            {
                double temperature = 0;
                double precip = 0;
                double potEvap = 0;
                if (month <= 1)
                {
                    temperature = -2;
                    precip = 3.0;
                    potEvap = 0;
                    LAI=0;
                    currentRootDepth=0;
                }
                if ((month>1)&&(month<5))
                {
                    temperature = 6;
                    precip = 1.0;
                    potEvap = 2;
                    LAI=2.0;
                    currentRootDepth=0.5;
                }
                if (month>8)
                {
                    temperature = 6;
                    precip = 1.0;
                    potEvap = 2;
                    LAI=0.0;
                    currentRootDepth=0.75;
                }

                if (((month >= 5) && (month <= 8)))
                {
                    temperature = 12;
                    precip = 0.0;
                    potEvap = 4.0;
                    LAI=4.0;
                    currentRootDepth=0;
                }
                for (int day = 0; day <= 31; day++)
                {
                    test.dailyRoutine(potEvap, precip, 0, temperature, LAI, currentRootDepth, ref plantDrought, ref soilDrought);
                    test.WriteDebug(31 * month + day);
                }

            }
        }
            
    }

}
