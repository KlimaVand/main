using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FarmACtestsetupData
{
    class main
    {
        public static void Main(string[] args)
        {
            string[] dirs = Directory.GetFiles(args[0], "farm*");
            System.IO.StreamWriter fileOutput = new System.IO.StreamWriter(args[0]+"output.xls");
            fileOutput.WriteLine("FarmNumber" + '\t' + "FarmDescription" + '\t' + "AgroEcologicalZone" + '\t' + "Scenario Name" + '\t' + "Scenario No" + '\t' + 
                "No Livestock types" + '\t' + " No Houses types" + '\t' + "No ManureRecipient types" + '\t' + "No Rotations" + '\t' + "No Crops types" + '\t' + "No Grazed" + '\t' + "No SoilsType");

            for(int i=0;i<dirs.Count();i++)
            {
                Console.WriteLine(dirs[i]);
                string[] fileName= new string[2];
                fileName[0] = dirs[i];
                fileName[1] = dirs[i];
                FileInformation file = new FileInformation(fileName);
                file.setPath("Farm");
                int min = 99999999, max = 0;
                file.getSectionNumber(ref min, ref max);

                for (int farmNr = min; farmNr <= max; farmNr++)
                {
                    if (file.doesIDExist(farmNr))
                    {
                        string newPath = "Farm(" + farmNr.ToString() + ")";
                        file.setPath(newPath);
                        int farmNumber = file.getItemInt("FarmNumber");
                        int agronr = file.getItemInt("AgroEcologicalZone");
                        string readString = file.getItemString("FarmDescription");
                        string farmDescription = "";
                        int k = 0;
                        while (k < readString.Length)
                        {
                            if (((int)readString[k]) >= 32)
                            {
                                farmDescription += readString[k];
                            }
                            else
                                break;
                            k += 1;                            
                        }
                        fileOutput.Write(farmNumber.ToString() + '\t');
                        fileOutput.Write(farmDescription + '\t');
                        fileOutput.Write(agronr.ToString() + '\t');
                        newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario";
                        file.setPath(newPath);
                        int minSelectedScenario = 99999999, maxSelectedScenario = 0;
                        file.getSectionNumber(ref minSelectedScenario, ref maxSelectedScenario);
                        for (int ScenarioNR = minSelectedScenario; ScenarioNR <= maxSelectedScenario; ScenarioNR++)
                        {
                            newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ")";
                            file.setPath(newPath);
                            string Scenario = file.getItemString("Name");

                            fileOutput.Write(Scenario.ToString() + '\t');
                            fileOutput.Write(ScenarioNR.ToString() + '\t');

                            newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Livestock";
                            file.setPath(newPath);
                            int minLivestock = 99999999, maxLivestock = 0;
                            file.getSectionNumber(ref minLivestock, ref maxLivestock);
                            string livestocks = "";
                            for (int livestockNR = minLivestock; livestockNR <= maxLivestock; livestockNR++)
                            {
                                newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Livestock("+livestockNR+")";
                                file.setPath(newPath);
                                string livestockName = file.getItemString("NameOfAnimals");
                                if (!livestocks.Contains(livestockName))
                                    livestocks += "&&&&&&" + file.getItemString("NameOfAnimals");
                            }

                            int allLivestock = CountStringOccurrences(livestocks,"&&&&&&");
                            fileOutput.Write(allLivestock.ToString() + '\t');
                            string nrOfHouses = "";
                            string nrStorageName = "";
                            for (int livestockNR = minLivestock; livestockNR <= maxLivestock; livestockNR++)
                            {
                                newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Livestock(" + livestockNR + ").Housing" ;
                                file.setPath(newPath);
                                int minHousing= 99999999, maxHousing = 0;
                                file.getSectionNumber(ref minHousing, ref maxHousing);

                                for (int housingNR = minHousing; housingNR <= maxHousing; housingNR++)
                                {
                                    newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Livestock(" + livestockNR + ").Housing(" + housingNR+")";
                                    file.setPath(newPath);
                                    string NameOfHousing = file.getItemString("NameOfHousing");
                                    if (!nrOfHouses.Contains(NameOfHousing))
                                        nrOfHouses += "&&&&&&" + file.getItemString("NameOfHousing");

                                    newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Livestock(" + livestockNR + ").Housing(" + housingNR + ").ManureRecipient";
                                    file.setPath(newPath);

                                    int minManureRecipient = 99999999, maxManureRecipient = 0;
                                    file.getSectionNumber(ref minManureRecipient, ref maxManureRecipient);
                                    for (int ManureRecipientNR = minManureRecipient; ManureRecipientNR <= maxManureRecipient; ManureRecipientNR++)
                                    {
                                        newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Livestock(" + livestockNR + ").Housing(" + housingNR + ").ManureRecipient(" + ManureRecipientNR+")";
                                        file.setPath(newPath);
                                        string StorageName = file.getItemString("StorageName");
                                        if (!nrStorageName.Contains(StorageName))
                                            nrStorageName += "&&&&&&" + file.getItemString("StorageName");
                                    }
                                }
                            }
                            int houses = CountStringOccurrences(nrOfHouses, "&&&&&&");
                            fileOutput.Write(houses.ToString() + '\t');
                            int nrStorage = CountStringOccurrences(nrStorageName, "&&&&&&");
                            fileOutput.Write(nrStorage.ToString() + '\t');

                            newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Rotation";
                            file.setPath(newPath);
                            int minRotation = 99999999, maxRotation = 0;
                            file.getSectionNumber(ref minRotation, ref maxRotation);
                            string RotationNames="";
                            string CropsNames = "";
                            int Grazed = 0;
                            string soils = "";
                            for (int RotationNR = minRotation; RotationNR <= maxRotation; RotationNR++)
                            {
                                newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Rotation";
                                file.setPath(newPath);
                                if (file.doesIDExist(RotationNR))
                                {
                                    newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Rotation(" + RotationNR + ")";
                                    file.setPath(newPath);

                                    string NameOfCrop = file.getItemString("NameOfRotation");
                                    if (!RotationNames.Contains(NameOfCrop))
                                        RotationNames += "&&&&&&" + file.getItemString("NameOfRotation");

                                   
                                    string SoilType = file.getItemString("SoilType");
                                    if (!soils.Contains(SoilType))
                                        soils += "&&&&&&" + file.getItemString("SoilType");


                                    newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Rotation(" + RotationNR + ").Crop";
                                    file.setPath(newPath);


                                    int minCrops = 99999999, maxCrops = 0;
                                    file.getSectionNumber(ref minCrops, ref maxCrops);

                                    for (int CropNR = minCrops; CropNR <= maxCrops; CropNR++)
                                    {
                                        newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Rotation(" + RotationNR + ").Crop(" + CropNR + ")";
                                        file.setPath(newPath);
                                        string Crop = file.getItemString("NameOfCrop");
                                        if (!CropsNames.Contains(Crop))
                                            CropsNames += "&&&&&&" + file.getItemString("SoilType");

                                        newPath = "Farm(" + farmNr.ToString() + ").SelectedScenario(" + ScenarioNR + ").Rotation(" + RotationNR + ").Crop(" + CropNR + ").Product(-1)";
                                        file.setPath(newPath);
                                        bool Graze = file.getItemBool("Grazed");
                                        if (Graze == true)
                                        {
                                            Grazed++;
                                        }
                                    }
                                }
                            }
                            int count = CountStringOccurrences(RotationNames, "&&&&&&");
                            fileOutput.Write(count.ToString() + '\t');

                            count = CountStringOccurrences(CropsNames, "&&&&&&");
                            fileOutput.Write(count.ToString() + '\t');

                            fileOutput.Write(Grazed.ToString() + '\t');

                            count =CountStringOccurrences(soils,"&&&&&&");
                            fileOutput.WriteLine(count.ToString() + '\t');
                        }
                    }
                }
            }
            fileOutput.Close();
        }
        public static int CountStringOccurrences(string text, string pattern)
        {
            // Loop through all instances of the string 'text'.
            int count = 0;
            int i = 0;
            while ((i = text.IndexOf(pattern, i)) != -1)
            {
                i += pattern.Length;
                count++;
            }
            return count;
        }
    }
}
