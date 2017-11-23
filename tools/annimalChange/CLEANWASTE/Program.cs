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
        static void Main(string[] args)
        {
            if (args.Length != 4)
            {
                Console.WriteLine("missing arguments in arg list");
                Environment.Exit(0);
            }
            if (args[2].CompareTo("1")==0)
                GlobalVars.Instance.setStrict(true);
            else
                GlobalVars.Instance.setStrict(false);
           
            if (args[3].CompareTo("1")==0)
                GlobalVars.Instance.SetIsIterating(true);
            else
                GlobalVars.Instance.SetIsIterating(false);

            FileInformation settings = new FileInformation("system.xml");
            int minSetting = 99, maxSetting = 0;
            settings.setPath("settings");
            settings.getSectionNumber(ref minSetting, ref maxSetting);
            for (int settingsID = minSetting; settingsID <= maxSetting; settingsID++)
            {
                settings.setPath("settings("+ settingsID.ToString()+")");
                if (args[0].CompareTo("-1") != 0)
                    GlobalVars.Instance.seterrorFileName(settings.getItemString("outputDir") + "error" + "_" + args[0] + "_" + args[1] + ".txt");
                else
                    GlobalVars.Instance.seterrorFileName(settings.getItemString("outputDir") + "error.txt");
                GlobalVars.Instance.setConstantFilePath(settings.getItemString("constants"));

                GlobalVars.Instance.theZoneData.SetdebugIt(false);
                if (GlobalVars.Instance.theZoneData.GetdebugIt())
                    GlobalVars.Instance.theZoneData.OpenDebugFile(settings.getItemString("outputDir") + "debug.txt");
                
                string farmName = settings.getItemString("farm");
                if (args[0].CompareTo("-1") != 0) //this code expects the 'farm' tag in system.xml to consist of a full file name (path + filename.xxx). The model will look for input in path + filename + "_" + args[0] + "_" + args[1].xxx
                {
                    string[] names = farmName.Split('.');
                    farmName = names[0] + "_" + args[0] + "_" + args[1] + "." + names[1];
              
                }
                GlobalVars.Instance.setFarmtFilePath(farmName);
                Console.WriteLine("Begin simulation of:");
                Console.WriteLine(farmName);
                GlobalVars.Instance.setFeeditemFilePath(settings.getItemString("feedItems"));
                GlobalVars.Instance.setParamFilePath(settings.getItemString("parameters"));
                GlobalVars.Instance.setfertManFilePath(settings.getItemString("fertAndManure"));
                GlobalVars.Instance.setPauseBeforeExit(Convert.ToBoolean(settings.getItemString("pauseBeforeExit")));
                GlobalVars.Instance.setstopOnError(Convert.ToBoolean(settings.getItemString("stopOnError")));
                
                GlobalVars.Instance.readGlobalConstants();


                FileInformation farmInformation = new FileInformation(GlobalVars.Instance.getFarmFilePath(), GlobalVars.Instance.GeterrorFileName());
                farmInformation.setPath("Farm");
                int min = 99999999, max = 0;

                farmInformation.getSectionNumber(ref min, ref max);
                for (int farmNr = min; farmNr <= max; farmNr++)
                {
                    //Stuff from files
                    if (farmInformation.doesIDExist(farmNr))
                    {
                        string newPath = "Farm(" + farmNr.ToString() + ")";
                        farmInformation.setPath(newPath);
                        int zoneNr = farmInformation.getItemInt("AgroEcologicalZone");
                        int FarmTyp = farmInformation.getItemInt("FarmType");

                        GlobalVars.Instance.theZoneData.readZoneSpecificData(zoneNr, FarmTyp);
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
                                XmlWriter writer = GlobalVars.Instance.theZoneData.OpenOutputXML(outputName);
                                outputName = outputName.Split('.')[0];
                                outputName = outputName + ".xls";
                                System.IO.StreamWriter tabFile = GlobalVars.Instance.theZoneData.OpenOutputTabFile(outputName);
                                //writer.WriteStartDocument();
                                writer.WriteStartElement("Farm");
                                tabFile.Write("Farm\t");
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

                                //do cropped area first
                                string RotationPath = newPath + "(" + ScenarioNr.ToString() + ").Rotation";
                                farmInformation.setPath(RotationPath);

                                int minRotation = 99, maxRotation = 0;

                                farmInformation.getSectionNumber(ref minRotation, ref maxRotation);
                                List<CropSequenceClass> rotationList = new List<CropSequenceClass>();
                                for (int rotationID = minRotation; rotationID <= maxRotation; rotationID++)
                                {
                                    if (farmInformation.doesIDExist(rotationID))
                                    {
                                        CropSequenceClass anExample = new CropSequenceClass(RotationPath, rotationID, zoneNr, FarmTyp);
                                        areaWeightedDuration += anExample.getArea() * anExample.GetlengthOfSequence();
                                        anExample.getGrazedFeedItems();
                                        anExample.getAllFeedItems();
                                        farmArea += anExample.getArea();
                                        rotationList.Add(anExample);
                                    }
                                }
                                if (farmArea > 0)
                                    areaWeightedDuration /= farmArea;
                                else
                                    areaWeightedDuration = 1;
                                GlobalVars.Instance.theZoneData.SetaverageYearsToSimulate(areaWeightedDuration);


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
                                        livestock anAnimal = new livestock(LivestockPath, LiveStockID, zoneNr, animalNr);
                                        anAnimal.GetAllFeedItemsUsed();
                                        listOfLivestock.Add(anAnimal);
                                        animalNr++;
                                    }
                                }

                                if (!GlobalVars.Instance.getStrict())
                                {
                                    Console.Write(GlobalVars.Instance.getStrict());
                                    for (int i = 0; i < listOfLivestock.Count; i++)
                                    {
                                        livestock anAnimal = listOfLivestock[i];
                                        anAnimal.intake();
                                        anAnimal.CalcMaximumProduction();
                                        anAnimal.DoRuminant();
                                        anAnimal.Write(writer, tabFile);

                                    }
                                }
                                    /*                                    writer.WriteEndDocument();
                                                                        writer.Close();
                                                                        tabFile.Close();*/
                                    //Environment.Exit(0);


                                    //calculate composition of bedding material
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
                                            manureStore amanurestore = new manureStore(ManureStoragePath, ManureStorageID, zoneNr);
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
                                            housing aHouse = new housing(housingType, anAnimalCategory, j, zoneNr);
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
                                            if ((anAnimal.GettimeOnPasture() > 0) && (maxRotation == 0))
                                            {
                                                System.IO.StreamWriter file = new System.IO.StreamWriter(GlobalVars.Instance.GeterrorFileName());
                                                file.WriteLine("Error - livestock are indicated as grazing but there are no fields to graze");
                                                file.Close();
                                                Console.WriteLine("Error - livestock are indicated as grazing but there are no fields to graze");
                                                if (GlobalVars.Instance.getPauseBeforeExit())
                                                    Console.Read();
                                                Environment.Exit(0);
                                            }
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



                                    if (!GlobalVars.Instance.getStrict())
                                    {
                                        //write output to xml file
                                        for (int i = 0; i < listOfHousing.Count; i++)
                                        {
                                            listOfHousing[i].Write(writer, tabFile);
                                        }


                                        for (int i = 0; i < listOfManurestores.Count; i++)
                                        {
                                            manureStore amanurestore2 = listOfManurestores[i];
                                            amanurestore2.Write(writer, tabFile);
                                        }
                                        writer.WriteEndDocument();
                                        writer.Close();
                                        tabFile.Close();
                                        Environment.Exit(0);
                                    }
                                

                                for (int rotationID = 0; rotationID <rotationList.Count; rotationID++)
                                {
                                        rotationList[rotationID].CalcModelledYield();
                                        rotationList[rotationID].CheckYields();
                                }

                                GlobalVars.Instance.CheckGrazingData();

                                farmBalanceClass theBalances = new farmBalanceClass();
                                theBalances.DoFarmBalances(rotationList, listOfLivestock, listOfHousing, listOfManurestores, writer, tabFile);

                               
                                //write output to xml file
                                for (int i = 0; i < listOfHousing.Count; i++)
                                {
                                    listOfHousing[i].Write(writer,tabFile);
                                }

                                
                               for (int i = 0; i < listOfManurestores.Count; i++)
                               {
                                   manureStore amanurestore2 = listOfManurestores[i];
                                   amanurestore2.Write(writer,tabFile);
                               }
                               for (int i = 0; i < listOfLivestock.Count; i++)
                               {
                                   livestock anAnimal = listOfLivestock[i];
                                   anAnimal.Write(writer,tabFile);

                               }
                     
                               for (int i = 0; i < rotationList.Count; i++)
                               {
                                   CropSequenceClass rotation = rotationList[i];
                                   rotation.Write(writer, tabFile);

                               }
                               GlobalVars.Instance.Write(writer, tabFile);
                             
                                
                                GlobalVars.Instance.theZoneData.CloseOutputXML();
                                GlobalVars.Instance.theZoneData.CloseOutputTabFile();
                            }
                        }
                    }
                }
                GlobalVars.Instance.theZoneData.CloseDebugFile();
            }
            

        }
    }
}
