using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.Xml.Linq;
using CToolModel;
using System.IO;
namespace CToolModel
{
    class Program
    {

        static public void Main()
        {
            string[] mode = null;
            try
            {
                mode = System.IO.File.ReadAllLines(@"mode.txt");

            }
            catch
            {
                Console.WriteLine("cannot read the mode.txt file");
                Environment.Exit(0);
            }
            string[] runsenariosInformation=new string[0];
            try
            {
                runsenariosInformation = System.IO.File.ReadAllLines("runsenarios.txt");
            }
            catch
            {
                Console.WriteLine("cannot read the runsenarios.txt file");
                Environment.Exit(0);
            }
            for (int i = 0; i < (runsenariosInformation.Length / 4); i++)
            {
                string text = null;
                try
                {
                    text = System.IO.File.ReadAllText(runsenariosInformation[i*4].Split('\t')[1]);
                }
                catch
                {
                    Console.WriteLine("cannot read the input.txt file");
                    Environment.Exit(0);
                }
                string[] data = text.Split('\r');

                double PLoweLayer = Convert.ToDouble(data[1].Split('\t')[1]);
                double offset = Convert.ToDouble(data[2].Split('\t')[1]);
                double depth = Convert.ToDouble(data[3].Split('\t')[1]);
                double PupperLayer = Convert.ToDouble(data[4].Split('\t')[1]);
                double C14percent = Convert.ToDouble(data[5].Split('\t')[1]);

                double startAmountOfCarbon = Convert.ToDouble(data[6].Split('\t')[1]);

                double CNratio = Convert.ToDouble(data[7].Split('\t')[1]);

                double extraCarbon = Convert.ToDouble(data[8].Split('\t')[1]);




                double HUMdecompositionratePlant = Convert.ToDouble(data[11].Split('\t')[1]);
                double FOMdecompositionratePlant = Convert.ToDouble(data[13].Split('\t')[1]);
                double ClayfractionPlant = Convert.ToDouble(data[14].Split('\t')[1]);
                double tFPlant = Convert.ToDouble(data[15].Split('\t')[1]);

                double ROMfractionPlant = Convert.ToDouble(data[17].Split('\t')[1]);
                double ROMdecompositionratePlant = Convert.ToDouble(data[18].Split('\t')[1]);


                double HUMdecompositionrateManure = Convert.ToDouble(data[21].Split('\t')[1]);
                double HUMfractionManure = Convert.ToDouble(data[22].Split('\t')[1]);

                double FOMdecompositionrateManure = Convert.ToDouble(data[24].Split('\t')[1]);
                double ClayfractionManure = Convert.ToDouble(data[25].Split('\t')[1]);
                double tFManure = Convert.ToDouble(data[26].Split('\t')[1]);

                double ROMfractionManure = Convert.ToDouble(data[28].Split('\t')[1]);
                double ROMdecompositionrateManure = Convert.ToDouble(data[29].Split('\t')[1]);




                double HUMdecompositionratePlantC14 = Convert.ToDouble(data[11].Split('\t')[1]);
                double FOMdecompositionratePlantC14 = Convert.ToDouble(data[34].Split('\t')[1]);
                double ClayfractionPlantC14 = Convert.ToDouble(data[35].Split('\t')[1]);
                double tFPlantC14 = Convert.ToDouble(data[36].Split('\t')[1]);

                double ROMfractionPlantC14 = Convert.ToDouble(data[38].Split('\t')[1]);
                double ROMdecompositionratePlantC14 = Convert.ToDouble(data[39].Split('\t')[1]);
                double DecayRateC14Plant = Convert.ToDouble(data[40].Split('\t')[1]);

                double HUMdecompositionrateManureC14 = Convert.ToDouble(data[43].Split('\t')[1]);
                double HUMfractionManureC14 = Convert.ToDouble(data[44].Split('\t')[1]);
                double FOMdecompositionrateManureC14 = Convert.ToDouble(data[46].Split('\t')[1]);
                double ClayfractionManureC14 = Convert.ToDouble(data[47].Split('\t')[1]);
                double tFManureC14 = Convert.ToDouble(data[48].Split('\t')[1]);

                double ROMfractionManureC14 = Convert.ToDouble(data[50].Split('\t')[1]);
                double ROMdecompositionrateManureC14 = Convert.ToDouble(data[51].Split('\t')[1]);
                double DecayRateC14Manuer = Convert.ToDouble(data[52].Split('\t')[1]);

                double FOMfractionPlantTopLayer = Convert.ToDouble(data[54].Split('\t')[1]);
                double FOMfractionPlantLowerLayer = Convert.ToDouble(data[55].Split('\t')[1]);
                double FOMfractionPlantTopLayerC14 = Convert.ToDouble(data[56].Split('\t')[1]);
                double FOMfractionPlantLowerLayerC14 = Convert.ToDouble(data[57].Split('\t')[1]);
                CTool test = new CTool();

                string[] lines = null;
                try
                {
                    
                    lines = System.IO.File.ReadAllLines(runsenariosInformation[i * 4 + 1].Split('\t')[1]);

                }
                catch
                {
                    Console.WriteLine("cannot read the data.txt file");
                    Environment.Exit(0);
                }
                List<CTool.Data> allData = new List<CTool.Data>();
                for (int j = 1; j < lines.Count(); j++)
                {
                    string[] items = lines[j].Split('\t');
                    CTool.Data item = new CTool.Data();
                    string tmp = items[0];
                    if (tmp.CompareTo("") != 0)
                    {
                        item.year = Convert.ToInt32(items[0]);
                        item.UpperLayerC = Convert.ToDouble(items[1]);
                        item.LowerLayerC = Convert.ToDouble(items[2]);
                        item.ManureC = Convert.ToDouble(items[3]);
                        item.C14PlantProcent = Convert.ToDouble(items[4]);
                        item.C14Manure = Convert.ToDouble(items[5]);
                        allData.Add(item);
                    }
                }
                List<double> TemperatureData = new List<double>();
                string[] TemperatureLines = null;
                try
                {
                    TemperatureLines = System.IO.File.ReadAllLines(runsenariosInformation[i * 4 + 2].Split('\t')[1]);
                }
                catch
                {
                    Console.WriteLine("cannot read the temperature.txt file");
                    Environment.Exit(0);
                }

                for (int j = 0; j < TemperatureLines.Count(); j++)
                {
                    TemperatureData.Add(Convert.ToDouble(TemperatureLines[j]));
                }
               
                Directory.SetCurrentDirectory(runsenariosInformation[i * 4 + 3].Split('\t')[1]);
                
                test.Main(TemperatureData, allData, DecayRateC14Manuer, ROMdecompositionrateManureC14, ROMfractionManureC14, tFManureC14, ClayfractionManureC14, FOMdecompositionrateManureC14, HUMfractionManureC14, HUMdecompositionrateManureC14, DecayRateC14Plant, ROMdecompositionratePlantC14, ROMfractionPlantC14, tFPlantC14, ClayfractionPlantC14, FOMdecompositionratePlantC14, ROMdecompositionrateManure, ROMfractionManure, tFManure, ClayfractionManure, FOMdecompositionrateManure, HUMfractionManure, HUMdecompositionrateManure, ROMfractionPlant, tFPlant, ClayfractionPlant, FOMdecompositionratePlant, PLoweLayer, offset, depth, PupperLayer, C14percent, startAmountOfCarbon, CNratio, extraCarbon, HUMdecompositionratePlant, ROMdecompositionratePlant, ROMfractionPlantC14, HUMdecompositionratePlantC14, Convert.ToInt32(mode[0]), FOMfractionPlantTopLayer, FOMfractionPlantLowerLayer, FOMfractionPlantTopLayerC14, FOMfractionPlantLowerLayerC14);
            }
        }
    }
}
