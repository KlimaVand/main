using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AnimalChange
{
    class Program
    {
        static void Main(string[] args)
        {
            string[] lines = System.IO.File.ReadAllLines("farms.txt");
            for (int l = 0; l < lines.Count(); l++)
            {
                FileInformation fil = new FileInformation(lines[l]);
                string[] tmp=lines[l].Split('_');

                string farmnr = tmp[1];
                string scenarioNr = tmp[2].Split('.')[0];
                fil.setPath("Farm");
                int max = 0;
                int min = 9999999;
                fil.getSectionNumber(ref min, ref max);
                System.IO.StreamWriter output = new System.IO.StreamWriter("output_" + farmnr + "_" + scenarioNr+".xls");
                for (int i = min; i <= max; i++)
                {
                    //  System.IO.StreamWriter output = new System.IO.StreamWriter("output1ForFarmNr" + i.ToString() + ".xls");
                    fil.setPath("Farm(" + i.ToString() + ")");
                    output.WriteLine("LIVESTOCK");
                    //output.WriteLine("FarmOwnerName: " + fil.getItemString("FarmOwnerName"));
                    //output.WriteLine("FarmDescription: " + fil.getItemString("FarmDescription"));
                    fil.setPath("Farm(" + i.ToString() + ").SelectedScenario");
                    int maxSelectedScenario = 0;
                    int minSelectedScenario = 99;
                    fil.getSectionNumber(ref minSelectedScenario, ref maxSelectedScenario);
                    for (int s = minSelectedScenario; s <= maxSelectedScenario; s++)
                    {

                        fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock");
                        int maxRotation = 0;
                        int minRotation = 99;
                        fil.getSectionNumber(ref minRotation, ref maxRotation);
                        for (int r = minRotation; r <= maxRotation; r++)
                        {
                            fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock(" + r.ToString() + ")");
                            output.Write("Livestock" + "\t");
                            output.WriteLine("Group" + "\t");
                            output.Write(fil.getItemString("NameOfAnimals") + "\t");
                            output.WriteLine(fil.getItemString("Identity"));
                            fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock(" + r.ToString() + ").itemFed");
                            int maxitemFed = 0;
                            int minitemFed = 99;
                            fil.getSectionNumber(ref minitemFed, ref maxitemFed);
                            output.Write("\t" + "\t" + "Feed name" + "\t");
                            output.Write("Amount" + "\t");
                            output.WriteLine("Units" + "\t");
                            for (int h = minitemFed; h <= maxitemFed; h++)
                            {
                                fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock(" + r.ToString() + ").itemFed");
                                if (fil.doesIDExist(h))
                                {
                                    fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock(" + r.ToString() + ").itemFed(" + h.ToString() + ")");
                                    output.Write("\t" + "\t" + fil.getItemString("Name") + "\t");
                                    output.Write(fil.getItemString("Amount") + "\t");
                                    output.WriteLine(fil.getItemString("Unit") + "\t");
                                }

                            }

                        }
                    }

                }
                fil.setPath("Farm");
                max = 0;
                min = 9999999;
                fil.getSectionNumber(ref min, ref max);
                fil.setPath("Farm(" + min.ToString() + ")");

                for (int i = min; i <= max; i++)
                {
                    // System.IO.StreamWriter output = new System.IO.StreamWriter("output2ForFarmNr" + i.ToString() + ".xls");
                    fil.setPath("Farm(" + i.ToString() + ")");
                    output.WriteLine("HOUSING AND MANURE STORAGE");
                    //output.WriteLine("FarmOwnerName: " + fil.getItemString("FarmOwnerName"));
                    //output.WriteLine("FarmDescription: " + fil.getItemString("FarmDescription"));
                    fil.setPath("Farm(" + i.ToString() + ").SelectedScenario");
                    int maxSelectedScenario = 0;
                    int minSelectedScenario = 99;
                    fil.getSectionNumber(ref minSelectedScenario, ref maxSelectedScenario);
                    for (int s = minSelectedScenario; s <= maxSelectedScenario; s++)
                    {

                        fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock");
                        int maxRotation = 0;
                        int minRotation = 99;
                        fil.getSectionNumber(ref minRotation, ref maxRotation);
                        for (int r = minRotation; r <= maxRotation; r++)
                        {
                            fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock(" + r.ToString() + ")");
                            output.Write("Livestock" + "\t");
                            output.Write("Group" + "\t");
                            output.Write("Housing" + "\t");
                            output.Write("Manure_storage" + "\t");
                            output.Write("Housing" + "\t");
                            output.WriteLine("Manure_storage" + "\t");
                            output.Write(fil.getItemString("NameOfAnimals") + "\t");
                            output.Write(fil.getItemString("Identity") + "\t");
                            fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock(" + r.ToString() + ").Housing");
                            int maxHousing = 0;
                            int minHousing = 99;
                            fil.getSectionNumber(ref minHousing, ref maxHousing);
                            for (int h = minHousing; h <= maxHousing; h++)
                            {
                                fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock(" + r.ToString() + ").Housing(" + h.ToString() + ")");
                                string nameOfHouse = fil.getItemString("NameOfHousing");
                                fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock(" + r.ToString() + ").Housing(" + h.ToString() + ").ManureRecipient");
                                int maxManureRecipient = 0;
                                int minManureRecipient = 99;
                                fil.getSectionNumber(ref minManureRecipient, ref maxManureRecipient);
                                for (int mr = minManureRecipient; mr <= maxManureRecipient; mr++)
                                {
                                    fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Livestock(" + r.ToString() + ").Housing(" + h.ToString() + ").ManureRecipient(" + mr.ToString() + ")");
                                    //output.Write("\t" + "\t" + "Housing" + "\t");
                                    //output.Write("ManureRecipient" + "\t");
                                    //output.WriteLine("ManureRecipient" + "\t");
                                    output.Write(nameOfHouse + "\t");
                                    output.Write(fil.getItemString("StorageName") + "\t");
                                    output.Write(fil.getItemString("StorageName") + "\t");
                                }
                                output.WriteLine();
                            }

                        }
                    }

                }



                fil.setPath("Farm");
                max = 0;
                min = 9999999;
                fil.getSectionNumber(ref min, ref max);

                for (int i = min; i <= max; i++)
                {
                    // System.IO.StreamWriter output = new System.IO.StreamWriter("output3ForFarmNr"+i.ToString()+".xls");
                    fil.setPath("Farm(" + i.ToString() + ")");
                    output.WriteLine("FarmOwnerName: " + fil.getItemString("FarmOwnerName"));
                    output.WriteLine("FarmDescription: " + fil.getItemString("FarmDescription"));
                    output.WriteLine("CROPPING AND FERTILISATION");
                    fil.setPath("Farm(" + i.ToString() + ").SelectedScenario");
                    int maxSelectedScenario = 0;
                    int minSelectedScenario = 99;
                    fil.getSectionNumber(ref minSelectedScenario, ref maxSelectedScenario);
                    for (int s = minSelectedScenario; s <= maxSelectedScenario; s++)
                    {

                        fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation");
                        int maxRotation = 0;
                        int minRotation = 99;
                        fil.getSectionNumber(ref minRotation, ref maxRotation);
                        for (int r = minRotation; r <= maxRotation; r++)
                        {
                            fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation");
                            if (fil.doesIDExist(r))
                            {
                                fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ")");
                                output.WriteLine("Crop sequence");
                                output.WriteLine(fil.getItemInt("Identity"));
                                output.Write("Name" + '\t');
                                output.Write("Area (ha)" + '\t');
                                output.WriteLine("Soil Type");
                                output.Write(fil.getItemString("NameOfRotation") + '\t');
                                output.Write(fil.getItemString("Area") + '\t');
                                output.WriteLine(fil.getItemString("SoilType"));

                                fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop");
                                int maxCrop = 0;
                                int minCrop = 99;
                                fil.getSectionNumber(ref minCrop, ref maxCrop);
                                for (int c = minCrop; c <= maxCrop; c++)
                                {
                                    fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop");
                                    if (fil.doesIDExist(c))
                                    {
                                        fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop(" + c.ToString() + ")");
                                        output.Write("Crop" + '\t');
                                        output.Write("Start:day" + '\t');
                                        output.Write("Start:month" + '\t');
                                        output.Write("End:day" + '\t');
                                        output.WriteLine("End:month" + '\t');

                                        output.Write(fil.getItemString("NameOfCrop") + '\t');
                                        output.Write(fil.getItemString("Start_day") + '\t');
                                        output.Write(fil.getItemString("Start_month") + '\t');
                                        output.Write(fil.getItemString("End_day") + '\t');
                                        output.WriteLine(fil.getItemString("End_month"));
                                        fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop(" + c.ToString() + ").Fertilizer_applied");
                                        int maxFertilizer = 0;
                                        int minFertilizer = 99;
                                        fil.getSectionNumber(ref minFertilizer, ref maxFertilizer);
                                        for (int f = minFertilizer; f <= maxFertilizer; f++)
                                        {
                                            fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop(" + c.ToString() + ").Fertilizer_applied(" + f.ToString() + ")");
                                            output.Write('\t' + "Fertiliser" + '\t');
                                            output.Write("Month_applied" + '\t');
                                            output.Write("Amount" + '\t');
                                            output.Write("Units" + '\t');
                                            output.WriteLine("Application_method");
                                            output.Write('\t' + fil.getItemString("Name") + '\t');
                                            output.Write(fil.getItemString("Month_applied") + '\t');
                                            output.Write(fil.getItemString("Value") + '\t');
                                            output.Write(fil.getItemString("Unit") + '\t');
                                            fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop(" + c.ToString() + ").Fertilizer_applied(" + f.ToString() + ").Applic_technique_Fertilizer(-1)");
                                            output.WriteLine(fil.getItemString("NameS"));
                                        }
                                        fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop(" + c.ToString() + ").Manure_applied");
                                        int maxManure = 0;
                                        int minManure = 99;
                                        fil.getSectionNumber(ref minManure, ref maxManure);
                                        for (int f = minManure; f <= maxManure; f++)
                                        {
                                            fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop(" + c.ToString() + ").Manure_applied(" + f.ToString() + ")");
                                            output.Write('\t' + "Manure" + '\t');
                                            output.Write("Month_applied" + '\t');
                                            output.Write("Amount" + '\t');
                                            output.Write("Units" + '\t');
                                            output.WriteLine("Application method");
                                            output.Write('\t' + fil.getItemString("Name") + '\t');
                                            output.Write(fil.getItemString("Month_applied") + '\t');
                                            output.Write(fil.getItemString("Value") + '\t');
                                            output.Write(fil.getItemString("Unit") + '\t');
                                            fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop(" + c.ToString() + ").Manure_applied(" + f.ToString() + ").Applic_technique_Manure(-1)");
                                            output.WriteLine(fil.getItemString("NameS"));
                                        }
                                        fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop(" + c.ToString() + ").Product");
                                        int maxProduct = 0;
                                        int minProduct = 99;
                                        fil.getSectionNumber(ref minProduct, ref maxProduct);
                                        if (maxProduct > 0)
                                            output.WriteLine('\t' + "Product_1" + '\t' + "Fate" + '\t' + "Product_2" + '\t' + "Fate");
                                        //output.WriteLine();
                                        for (int f = minProduct; f <= maxProduct; f++)
                                        {
                                            fil.setPath("Farm(" + i.ToString() + ").SelectedScenario(" + s.ToString() + ").Rotation(" + r.ToString() + ").Crop(" + c.ToString() + ").Product(" + f.ToString() + ")");
                                            output.Write('\t' + fil.getItemString("Name") + '\t');
                                            output.Write(fil.getItemString("Harvested") + '\t');
                                        }
                                        output.WriteLine();
                                    }
                                }
                            }
                        }
                    }
                    // output.Close();
                }
                output.Close();
            }
        }
    }
}
