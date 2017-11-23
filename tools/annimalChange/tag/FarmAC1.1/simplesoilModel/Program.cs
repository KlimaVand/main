using System;
using System.Collections.Generic;
using System.Linq;
using System.Xml;
using System.Text;

namespace simplesoilModel
{
    class Program
    {
        static void Main(string[] args)
        {
            FileInformation settings = new FileInformation("system.xml");
            int minSetting = 99, maxSetting = 0;
            settings.setPath("settings");
            settings.getSectionNumber(ref minSetting, ref maxSetting);
            for (int settingsID = minSetting; settingsID <= maxSetting; settingsID++)
            {
                settings.setPath("settings(" + settingsID.ToString() + ")");
                GlobalVars.Instance.setSimplesoilModelFileName(settings.getItemString("simplesoilModel"));
/*                string outputName = settings.getItemString("outputDir") + "outputSoil" + ".xml";
                XmlWriter writer = GlobalVars.Instance.OpenOutputXML(outputName);
                outputName = outputName.Split('.')[0];
                outputName = outputName + ".xls";
                GlobalVars.Instance.OpenOutputTabFile(outputName);*/
                GlobalVars.Instance.theZoneData.OpenDebugFile(settings.getItemString("outputDir") + "debug.txt");
                simpleSoil aSoil = new simpleSoil();
                aSoil.Initialise(0.0, 0);
                aSoil.WriteDebugHeader();
                for (int i = 1; i <= 365; i++)
                {
                    aSoil.dailyRoutine(1.0, 2.0, 10.0, 0.0, 0.0);
                    aSoil.checkBudget();
                    double waterIn = aSoil.getwaterIn();
                    double waterOut = aSoil.getwaterOut();
                    double waterInSystem = aSoil.getwaterInSystem();
                    Console.WriteLine("In " + waterIn.ToString() + " Out " + waterOut.ToString() + " In system " + waterInSystem.ToString());
                    aSoil.WriteDebug(i);
                }
//                aSoil.Write();
                GlobalVars.Instance.theZoneData.CloseDebugFile();
/*                GlobalVars.Instance.CloseOutputXML();
                GlobalVars.Instance.CloseOutputTabFile();*/
            }
        }
    }
}
