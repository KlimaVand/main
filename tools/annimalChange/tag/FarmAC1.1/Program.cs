using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
namespace AnimalChange
{
    class Program
    {
        //args[0] and args[1] are farm number and scenario number respectively
        //if args[2] is 1, the energy demand of livestock must be met (0 is used when reporting current energy balance of livestock to users)
        //if args[3] is 1, the crop model requires the modelled production of grazed DM to match the expected production. If =0, grazed DM will be imported if there is a deficit
        static void Main(string[] args)
        {
            
            FileInformation settings = new FileInformation("system.xml");
            int minSetting = 99, maxSetting = 0;
            settings.setPath("settings");
            settings.getSectionNumber(ref minSetting, ref maxSetting);
            string outputDir;
            for (int settingsID = minSetting; settingsID <= maxSetting; settingsID++)
            {
                GlobalVars.Instance.reset(settingsID.ToString());
                if (args.Length != 4 && args.Length != 0)
                {
                    Console.WriteLine("missing arguments in arg list");
                    Environment.Exit(0);
                }
                if (args.Length == 4)
                {
                    if (args[2].CompareTo("1") == 0) //if args[2] = 1, the full model is run
                        GlobalVars.Instance.setRunFullModel(true);
                    else if (args[2].CompareTo("0") == 0)
                        GlobalVars.Instance.setRunFullModel(false);   //used when reporting current energy balance of livestock to users
                    else
                    {
                        Console.WriteLine("invalid input");
                        Environment.Exit(1);
                    }

                    if (args[3].CompareTo("1") == 0)
                        GlobalVars.Instance.SetstrictGrazing(true);
                    else if (args[3].CompareTo("0") == 0) 
                        GlobalVars.Instance.SetstrictGrazing(false);
                    else
                    {
                        Console.WriteLine("invalid input");
                        Environment.Exit(1);
                    }
                }
                else
                    GlobalVars.Instance.setRunFullModel(true);

                if (settings.Identity.Count == 1)
                    settings.Identity.RemoveAt(0);
                if (settings.doesIDExist(settingsID))
                {
                    settings.setPath("settings(" + settingsID.ToString() + ")");
                    if (args.Length != 0 && args[0].CompareTo("-1") != 0)
                        GlobalVars.Instance.seterrorFileName(settings.getItemString("outputDir") + "error" + "_" + args[0] + "_" + args[1] + ".txt");
                    else
                        GlobalVars.Instance.seterrorFileName(settings.getItemString("outputDir") + "error.txt");
                    
                    outputDir=settings.getItemString("outputDir");

                    string file=fileName(args, settings, "constants");
                    GlobalVars.Instance.setConstantFilePath(file);
                    //GlobalVars.Instance.setSimplesoilModelFileName(settings.getItemString("simplesoilModel"));
                    GlobalVars.Instance.theZoneData.SetdebugIt(false);
                    if (GlobalVars.Instance.theZoneData.GetdebugIt())
                        GlobalVars.Instance.theZoneData.OpenDebugFile(settings.getItemString("outputDir") + "debug.txt");

                    string farmName = settings.getItemString("farm");
                    if (args.Length != 0 && args[0].CompareTo("-1") != 0) //this code expects the 'farm' tag in system.xml to consist of a full file name (path + filename.xxx). The model will look for input in path + filename + "_" + args[0] + "_" + args[1].xxx
                    {
                        string[] names = farmName.Split('.');
                        farmName = names[0] + "_" + args[0] + "_" + args[1] + "." + names[1];

                    }
                    GlobalVars.Instance.setFarmtFilePath(farmName);
                    Console.WriteLine("Begin simulation of:");
                    Console.WriteLine(farmName);
                    file = fileName(args, settings, "feedItems");
                    GlobalVars.Instance.setFeeditemFilePath(file);
                    file = fileName(args, settings, "parameters");
                    GlobalVars.Instance.setParamFilePath(file);
                    file = fileName(args, settings, "fertAndManure");
                    GlobalVars.Instance.setfertManFilePath(file);
                    GlobalVars.Instance.setPauseBeforeExit(Convert.ToBoolean(settings.getItemString("pauseBeforeExit")));
                    GlobalVars.Instance.setstopOnError(Convert.ToBoolean(settings.getItemString("stopOnError")));
                    try
                    {
                        GlobalVars.Instance.readGlobalConstants();
                    }
                    catch
                    {
                        GlobalVars.Instance.Error("constant file not found", "program",false);
                    }

                    FileInformation farmInformation = new FileInformation(GlobalVars.Instance.getFarmFilePath());
                    farmInformation.setPath("Farm");
                    int min = 99999999, max = 0;

                    farmInformation.getSectionNumber(ref min, ref max);
                    for (int farmNr = min; farmNr <= max; farmNr++)
                    {
                        //Stuff from files
                        try
                        {
                            if (farmInformation.doesIDExist(farmNr))
                            {
                                string newPath = "Farm(" + farmNr.ToString() + ")";
                                farmInformation.setPath(newPath);
                                int zoneNr = farmInformation.getItemInt("AgroEcologicalZone");
                                GlobalVars.Instance.SetZone(zoneNr);
                                int FarmTyp = farmInformation.getItemInt("FarmType");

                                GlobalVars.Instance.theZoneData.readZoneSpecificData(zoneNr, FarmTyp);
                                double Ndep = farmInformation.getItemDouble("Value", newPath + ".NDepositionRate(-1)");
                                GlobalVars.Instance.theZoneData.SetNdeposition(Ndep);
                                newPath = newPath + ".SelectedScenario";
                                farmInformation.setPath(newPath);
                                int minScenario = 99, maxScenario = 0;
                                farmInformation.getSectionNumber(ref minScenario, ref maxScenario);
                                for (int ScenarioNr = minScenario; ScenarioNr <= maxScenario; ScenarioNr++)
                                {
                                    int InventorySystem = 0;
                                    int energySystem = 0;
                                    double areaWeightedDuration = 0;
                                    double farmArea = 0;
                                    if (farmInformation.doesIDExist(ScenarioNr))
                                    {

                                        string outputName = settings.getItemString("outputDir") + "outputFarm" + farmNr.ToString() + "ScenarioNr" + ScenarioNr.ToString() + ".xml";

                                        //XmlWriter writer = XmlWriter.Create(outputName);
                                        XmlWriter writer = GlobalVars.Instance.OpenOutputXML(outputName);
                                        outputName = outputName.Split('.')[0];
                                        //outputName = outputName + ".xls";
                                        GlobalVars.Instance.OpenOutputTabFile(outputName, settings.getItemString("outputDir"));

                                        //writer.WriteStartDocument();
                                        GlobalVars.Instance.writeStartTab("Farm");

                                        GlobalVars.Instance.initialiseExcretaExchange();
                                        GlobalVars.Instance.initialiseFeedAndProductLists();
                                        string ScenarioPath = newPath + "(" + ScenarioNr.ToString() + ")";
                                        farmInformation.setPath(ScenarioPath);
                                        farmInformation.Identity.Add(-1);
                                        farmInformation.PathNames.Add("InventorySystem");
                                        InventorySystem = farmInformation.getItemInt("Value");
                                        GlobalVars.Instance.setcurrentInventorySystem(InventorySystem);
                                        farmInformation.PathNames.Add("EnergySystem");
                                        energySystem = farmInformation.getItemInt("Value");
                                        GlobalVars.Instance.setcurrentEnergySystem(energySystem);

                                        List<CropSequenceClass> rotationList = new List<CropSequenceClass>();
                                        int minRotation = 99, maxRotation = 0;
                                        //if (GlobalVars.Instance.getRunFullModel())
                                        //{
                                        //do cropped area first
                                        string RotationPath = newPath + "(" + ScenarioNr.ToString() + ").Rotation";
                                        farmInformation.setPath(RotationPath);

                                        farmInformation.getSectionNumber(ref minRotation, ref maxRotation);
                                        for (int rotationID = minRotation; rotationID <= maxRotation; rotationID++)
                                        {
                                            if (farmInformation.doesIDExist(rotationID))
                                            {
                                                CropSequenceClass anExample = new CropSequenceClass(RotationPath, rotationID, zoneNr, FarmTyp, "farmnr" + farmNr.ToString() + "_ScenarioNr" + ScenarioNr.ToString() + "_CropSequenceClass" + rotationID.ToString());
                                                areaWeightedDuration += anExample.getArea() * anExample.GetlengthOfSequence();
                                                anExample.getGrazedFeedItems();
                                                farmArea += anExample.getArea();
                                                rotationList.Add(anExample);
                                            }
                                        }
                                        if (farmArea > 0)
                                            areaWeightedDuration /= farmArea;
                                        else
                                            areaWeightedDuration = 1;
                                        GlobalVars.Instance.theZoneData.SetaverageYearsToSimulate(areaWeightedDuration);
                                        //}

                                        // temporary location
                                        List<manureStore> listOfManurestores = new List<manureStore>();
                                        List<housing> listOfHousing = new List<housing>();

                                        //start of livestock section
                                        string LivestockPath = newPath + "(" + ScenarioNr.ToString() + ").Livestock";
                                        farmInformation.setPath(LivestockPath);

                                        List<livestock> listOfLivestock = new List<livestock>();
                                        //read the livestock details from file
                                        int minLivestock = 99, maxLivestock = 0;

                                        farmInformation.getSectionNumber(ref minLivestock, ref maxLivestock);
                                        int animalNr = 0;
                                        for (int LiveStockID = minLivestock; LiveStockID <= maxLivestock; LiveStockID++)
                                        {

                                            if (farmInformation.doesIDExist(LiveStockID))
                                            {
                                                livestock anAnimal = new livestock(LivestockPath, LiveStockID, zoneNr, animalNr, "farmnr" + farmNr.ToString() + "_ScenarioNr" + ScenarioNr.ToString() + "_livestock" + LiveStockID.ToString());
                                                anAnimal.GetAllFeedItemsUsed();
                                                listOfLivestock.Add(anAnimal);
                                                animalNr++;
                                            }
                                        }

                                        //calculate composition of bedding material
                                        if (GlobalVars.Instance.getRunFullModel())
                                            GlobalVars.Instance.CalcbeddingMaterial(rotationList);

                                        //read details of any manure stores that do not receive manure from livestock on the farm
                                        string ManureStoragePath = newPath + "(" + ScenarioNr.ToString() + ").ManureStorage";
                                        farmInformation.setPath(ManureStoragePath);
                                        //
                                        int minManureStorage = 99, maxManureStorage = 0;

                                        farmInformation.getSectionNumber(ref minManureStorage, ref maxManureStorage);
                                        for (int ManureStorageID = minManureStorage; ManureStorageID <= maxManureStorage; ManureStorageID++)
                                        {
                                            if (farmInformation.doesIDExist(ManureStorageID))
                                            {
                                                manureStore amanurestore = new manureStore(ManureStoragePath, ManureStorageID, zoneNr, "farmnr" + farmNr.ToString() + "_ScenarioNr" + ScenarioNr.ToString() + "_manureStore" + ManureStorageID.ToString());
                                                listOfManurestores.Add(amanurestore);
                                            }
                                        }

                                        //get details of animal housing (for each livestock category)
                                        for (int i = 0; i < listOfLivestock.Count(); i++)
                                        {
                                            livestock anAnimalCategory = listOfLivestock[i];
                                            for (int j = 0; j < anAnimalCategory.GethousingDetails().Count(); j++)
                                            {
                                                int housingType = anAnimalCategory.GethousingDetails()[j].GetHousingType();
                                                double proportionOfTime = anAnimalCategory.GethousingDetails()[j].GetpropTime();
                                                housing aHouse = new housing(housingType, anAnimalCategory, j, zoneNr, "farmnr" + farmNr.ToString() + "_ScenarioNr" + ScenarioNr.ToString() + "_housingi" + i.ToString() + "_housingj" + j.ToString());
                                                listOfHousing.Add(aHouse);
                                                //storage for manure produced in housing is initiated in the housing module
                                                for (int k = 0; k < aHouse.GetmanurestoreDetails().Count; k++)
                                                {
                                                    manureStore aManureStore = aHouse.GetmanurestoreDetails()[k].GettheStore();
                                                    aManureStore.SettheHousing(aHouse);
                                                    listOfManurestores.Add(aManureStore);
                                                }
                                            }
                                        }

                                        for (int i = 0; i < listOfLivestock.Count; i++)
                                        {
                                            livestock anAnimal = listOfLivestock[i];
                                            if (anAnimal.GetisRuminant())
                                            {
                                                anAnimal.DoRuminant();
                                                if ((GlobalVars.Instance.getRunFullModel()) &&
                                                        ((anAnimal.GettimeOnPasture() > 0) && (maxRotation == 0)))
                                                {
                                                    string messageString = ("Error - livestock are indicated as grazing but there are no fields to graze");
                                                    GlobalVars.Instance.Error(messageString);
                                                }
                                            }
                                            else if (anAnimal.GetspeciesGroup() == 2)
                                            {
                                                anAnimal.DoPig();
                                            }

                                            if (!GlobalVars.Instance.getRunFullModel())   //only called when only the livestock excretion is needed
                                            {
                                                Console.Write(GlobalVars.Instance.getRunFullModel());
                                                GlobalVars.Instance.writeGrazedItems();
                                            }

                                        }
                                        for (int i = 0; i < listOfHousing.Count; i++)
                                        {
                                            housing ahouse = listOfHousing[i];
                                            ahouse.DoHousing();
                                        }

                                        GlobalVars.Instance.theManureExchange = new GlobalVars.theManureExchangeClass();
                                        for (int i = 0; i < listOfManurestores.Count; i++)
                                        {
                                            manureStore amanurestore2 = listOfManurestores[i];
                                            amanurestore2.DoManurestore();
                                        }

                                        if (!GlobalVars.Instance.getRunFullModel())
                                        {
                                            //write output to xml file
                                            for (int i = 0; i < listOfHousing.Count; i++)
                                            {
                                                listOfHousing[i].Write();
                                            }


                                            for (int i = 0; i < listOfManurestores.Count; i++)
                                            {
                                                manureStore amanurestore2 = listOfManurestores[i];
                                                amanurestore2.Write();
                                            }
                                            for (int i = 0; i < listOfLivestock.Count; i++)
                                            {
                                                livestock anAnimal = listOfLivestock[i];
                                                anAnimal.Write();

                                            }
                                            GlobalVars.Instance.Write(false);
                                            GlobalVars.Instance.CloseOutputTabFile();
                                            GlobalVars.Instance.CloseOutputXML();
                                            Console.WriteLine(GlobalVars.Instance.getRunFullModel());
                                           
                                        }

                                        else
                                        {
                                            GlobalVars.Instance.OpenPlantFile();
                                            for (int rotationID = 0; rotationID < rotationList.Count; rotationID++)
                                            {
                                                rotationList[rotationID].CalcModelledYield();
                                                rotationList[rotationID].CheckYields();
                                                // rotationList[rotationID].getAllFeedItems();
                                                Console.WriteLine("Seq " + rotationID.ToString() + " DM " + rotationList[rotationID].GetDMYield().ToString());
                                                Console.WriteLine("Seq " + rotationID.ToString() + " C " + rotationList[rotationID].getCHarvested().ToString());
                                            }
                                            GlobalVars.Instance.ClosePlantFile();
                                            GlobalVars.Instance.CheckGrazingData();

                                            //Begin output routines

                                            farmBalanceClass theBalances = new farmBalanceClass("farmnr" + farmNr.ToString() + "_ScenarioNr" + ScenarioNr.ToString() + "FarmBalance_1");
                                            theBalances.DoFarmBalances(rotationList, listOfLivestock, listOfHousing, listOfManurestores);

                                            //write output to xml file
                                            if (GlobalVars.Instance.getRunFullModel() == false)
                                            {
                                                for (int i = 0; i < listOfHousing.Count; i++)
                                                {
                                                    listOfHousing[i].Write();
                                                }
                                            }
                                            for (int i = 0; i < listOfManurestores.Count; i++)
                                            {
                                                manureStore amanurestore2 = listOfManurestores[i];
                                                if (GlobalVars.Instance.getRunFullModel() == false)
                                                    amanurestore2.Write();
                                                else
                                                {
                                                    amanurestore2.Write();
                                                    GlobalVars.Instance.writeEndTab();
                                                    GlobalVars.Instance.writeEndTab();
                                                }
                                            }
                                            if (GlobalVars.Instance.getRunFullModel() == false)
                                            {
                                                for (int i = 0; i < listOfLivestock.Count; i++)
                                                {
                                                    livestock anAnimal = listOfLivestock[i];
                                                    anAnimal.Write();
                                                }
                                            }
                                            GlobalVars.Instance.OpenPlantFile();
                                            for (int i = 0; i < rotationList.Count; i++)
                                            {
                                                CropSequenceClass rotation = rotationList[i];
                                                rotation.Write();
                                            }
                                            GlobalVars.Instance.ClosePlantFile();
                                            GlobalVars.Instance.Write(true);
                                            GlobalVars.Instance.writeStartTab("ExpectedYield");
                                            for (int i = 0; i < rotationList.Count; i++)
                                            {
                                                GlobalVars.Instance.writeStartTab("CropSequenceClass");

                                                rotationList[i].calculateExpectedYield();
                                                GlobalVars.Instance.writeEndTab();
                                            }
                                            GlobalVars.Instance.writeEndTab();
                                            GlobalVars.Instance.CloseOutputXML();
                                            GlobalVars.Instance.CloseOutputTabFile();
                                        }
                                    }//end of scenario exists
                                    long ticks = DateTime.UtcNow.Ticks;
                                    System.IO.File.WriteAllText(outputDir + "done" + farmNr.ToString() + "ScenarioNr" + ScenarioNr.ToString() + ".txt", ticks.ToString());
                                }//end of scenario
                            }//end of farm exists
                        }
                        catch (Exception e)
                        {

                            GlobalVars.Instance.Error(e.Message, e.StackTrace, false);
                        }
                    }
                    GlobalVars.Instance.theZoneData.CloseDebugFile();
                }

            }

            

        }

        static string fileName(string[] args, FileInformation settings, string file)
        {
            if (args.Length == 4)
            {
                List<string> tmpPath = new List<string>(settings.PathNames);
                List<int> tmpID = new List<int>(settings.Identity);
                settings.Identity.Clear();
                settings.PathNames.Clear();
                settings.setPath("CommonSettings(-1)");
                string alternativePath = settings.getItemString("alternativePath");
                settings.PathNames=tmpPath;
                settings.Identity = tmpID;
                string constants = settings.getItemString(file);
                string[] constantsList = constants.Split('\\');
                string fileName = constantsList[constantsList.Length - 1];
                alternativePath += "\\" + args[0] + "\\" + fileName;
                if (File.Exists(alternativePath))
                {
                    return alternativePath;
                }
                else
                {
                    return constants;
                }
            }
            else
                return settings.getItemString(file);
          

        }

        public void writeOutput(List<manureStore> listOfManurestores, List<housing> listOfHousing, List<livestock> listOfLivestock,
            List<CropSequenceClass> rotationList)
        {
                            //write output to xml file
                if (GlobalVars.Instance.getRunFullModel() == false)
                {
                    for (int i = 0; i < listOfHousing.Count; i++)
                    {
                        listOfHousing[i].Write();
                    }
                }
                for (int i = 0; i < listOfManurestores.Count; i++)
                {
                    manureStore amanurestore2 = listOfManurestores[i];
                    if (GlobalVars.Instance.getRunFullModel() == false)
                        amanurestore2.Write();
                    else
                    {
                        amanurestore2.Write();
                        GlobalVars.Instance.writeEndTab();
                        GlobalVars.Instance.writeEndTab();
                    }
                }
                if (GlobalVars.Instance.getRunFullModel() == false)
                {
                    for (int i = 0; i < listOfLivestock.Count; i++)
                    {
                        livestock anAnimal = listOfLivestock[i];
                        anAnimal.Write();
                    }
                }
                GlobalVars.Instance.OpenPlantFile();
                for (int i = 0; i < rotationList.Count; i++)
                {
                    CropSequenceClass rotation = rotationList[i];
                    rotation.Write();
                }
                GlobalVars.Instance.ClosePlantFile();

                GlobalVars.Instance.Write(true);
                GlobalVars.Instance.writeStartTab("ExpectedYield");
                for (int i = 0; i < rotationList.Count; i++)
                {
                    GlobalVars.Instance.writeStartTab("CropSequenceClass");

                    rotationList[i].calculateExpectedYield();
                    GlobalVars.Instance.writeEndTab();
                }
                GlobalVars.Instance.writeEndTab();
                GlobalVars.Instance.CloseOutputXML();
                GlobalVars.Instance.CloseOutputTabFile();
        }
            
    }

}
