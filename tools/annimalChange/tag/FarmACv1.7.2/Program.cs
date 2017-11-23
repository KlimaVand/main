using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Xml;
using System.IO;
using System.Xml.Linq;
using System.Diagnostics;
namespace AnimalChange
{
    class Program
    {
        //args[0] and args[1] are farm number and scenario number respectively
        //if args[2] is 1, the energy demand of livestock must be met (0 is used when reporting current energy balance of livestock to users)
        //if args[3] is 1, the crop model requires the modelled production of grazed DM to match the expected production. If =0, grazed DM will be imported if there is a deficit
        //if args[4] is -1, the model will spinup for the years in the spinupYearsBaseLine parameter then run a baseline scenario and generate a Ctool data transfer file
        //if args[4] is a positive integer and the spinupYearsNonBaseLine parameter is zero, the model will read Ctool data from the Ctool data transfer file. If spinupYearsNonBaseLine is not zero, the model will spin up for spinupYearsNonBaseLine years then run the scenario
        // So to just calculate the animal production, set the farm file in system.xml to "farm.xml" and the args to <farm number> <scenario number> "0" "0" "-1"
        static void Main(string[] args)
        {
            Stopwatch timer = new Stopwatch();
            timer.Start();
            string[] system = new string[2];
            system[0] = "system.xml";
            system[1] = "systemAlternative.xml";
            FileInformation settings = new FileInformation(system);
            settings.setPath("CommonSettings(-1)");
            bool logFile=settings.getItemBool("logFile",false);
            bool logScreen = settings.getItemBool("logScreen",false);
            int verbocity = settings.getItemInt("verbosityLevel",false);
            if (verbocity == -1)
                verbocity = 5;
            int minSetting = 99, maxSetting = 0;
            settings.setPath("settings");
            settings.getSectionNumber(ref minSetting, ref maxSetting);
            string outputDir;
            for (int settingsID = minSetting; settingsID <= maxSetting; settingsID++)
            {
                GlobalVars.Instance.reset(settingsID.ToString());
                GlobalVars.Instance.logFile=logFile;
                GlobalVars.Instance.logScreen=logScreen;
                GlobalVars.Instance.verbocity = verbocity;
                if (args.Length != 5 && args.Length != 0)
                {
                    GlobalVars.Instance.log("missing arguments in arg list", 5);
                    Environment.Exit(0);
                }
                if (args.Length == 5)
                {
                    if (args[2].CompareTo("1") == 0) //if args[2] = 1, the full model is run
                        GlobalVars.Instance.setRunFullModel(true);
                    else if (args[2].CompareTo("0") == 0)
                        GlobalVars.Instance.setRunFullModel(false);   //used when reporting current energy balance of livestock to users
                    else
                    {
                        GlobalVars.Instance.log("invalid input", 5);
                        Environment.Exit(1);
                    }

                    if (args[3].CompareTo("1") == 0)
                        GlobalVars.Instance.SetstrictGrazing(true);
                    else if (args[3].CompareTo("0") == 0)
                        GlobalVars.Instance.SetstrictGrazing(false);
                    else
                    {
                        GlobalVars.Instance.log("invalid input", 5);
                        Environment.Exit(1);
                    }
    
                    GlobalVars.Instance.reuseCtoolData=Convert.ToInt32(args[4]);
                    
                }
                else
                {
                    GlobalVars.Instance.SetstrictGrazing(true);
                    GlobalVars.Instance.setRunFullModel(true);
                    if (settings.Identity.Count == 1)
                    settings.Identity.RemoveAt(0);
                    if (settings.doesIDExist(settingsID))
                    {
                        GlobalVars.Instance.reuseCtoolData =-1;
                        settings.Identity.Add(settingsID);
                        GlobalVars.Instance.reuseCtoolData = settings.getItemInt("Adaptation", false);

                    }
                }

                if (settings.Identity.Count == 1)
                    settings.Identity.RemoveAt(0);
                if (settings.doesIDExist(settingsID))
                {
                    settings.setPath("settings(" + settingsID.ToString() + ")");
                    if (!Directory.Exists(settings.getItemString("outputDir"))) 
                    {
                        Directory.CreateDirectory(settings.getItemString("outputDir"));
                    }
                    
                    if (args.Length != 0 && args[0].CompareTo("-1") != 0)
                        GlobalVars.Instance.seterrorFileName(settings.getItemString("outputDir") + "error" + "_" + args[0] + "_" + args[1] + ".txt");
                    else
                        GlobalVars.Instance.seterrorFileName(settings.getItemString("outputDir") + "error.txt");


                    //GlobalVars.Instance.setHandOverData(settings.getItemString("outputDir") + "dataCtool" + "_" + args[0] + "_" + args[1] + ".txt");
                    outputDir=settings.getItemString("outputDir");
                    GlobalVars.Instance.logFileStream = new System.IO.StreamWriter(outputDir + "\\log.txt");
                    string[] file=fileName(args, settings, "constants");
                    GlobalVars.Instance.setConstantFilePath(file);
                    //GlobalVars.Instance.setSimplesoilModelFileName(settings.getItemString("simplesoilModel"));
      
     

                    string farmName = settings.getItemString("farm");
                    //this code section expects the 'farm' tag in system.xml to consist of a full file name (path + filename.xxx). 
                    //The model will look for input in path + filename + "_" + args[0] + "_" + args[1].xxx
                    //note that the farm file in system.xml must be called "farm.xml" for this to work
                    if (args.Length != 0 && args[0].CompareTo("-1") != 0) 
                    {
                        string[] names = farmName.Split('.');
                        farmName = names[0] + "_" + args[0] + "_" + args[1] + "." + names[1];

                    }
                    //Alternative
                    string[] farmNames = new string[2];
                    farmNames[0] = farmName;
                    farmNames[1] = farmName.Split('.')[0] + "Alternative.xml";
                    GlobalVars.Instance.setFarmtFilePath(farmNames);
                    GlobalVars.Instance.log("Begin simulation of:", 5);
                    GlobalVars.Instance.log(farmName, 5);
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
                    if ((GlobalVars.Instance.reuseCtoolData==-1) && (max!=min))
                        GlobalVars.Instance.Error("Model called in baseline mode, when more than one scenario is present in the system.dat file");
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
                                if(FarmTyp>3)
                                    GlobalVars.Instance.Error("Farmtype not supported");
                                GlobalVars.Instance.theZoneData.readZoneSpecificData(zoneNr, FarmTyp);
                                double Ndep = farmInformation.getItemDouble("Value", newPath + ".NDepositionRate(-1)");
                                GlobalVars.Instance.theZoneData.SetNdeposition(Ndep);
                                newPath = newPath + ".SelectedScenario";
                                farmInformation.setPath(newPath);
                                int minScenario = 99, maxScenario = 0;
                                farmInformation.getSectionNumber(ref minScenario, ref maxScenario);
                                for (int settingsnr = minScenario; settingsnr <= maxScenario; settingsnr++)
                                {
                                    int InventorySystem = 0;
                                    int energySystem = 0;
                                    double areaWeightedDuration = 0;
                                    double farmArea = 0;

                                    int ScenarioNr = 0;
                                    if(args.Length == 0)
                                    {
                                        //Console.WriteLine(GlobalVars.Instance.getFarmFilePath());
                                        string[] tmp=GlobalVars.Instance.getFarmFilePath()[0].Split('_');
                                        ScenarioNr = Convert.ToInt32(tmp[tmp.Length-1].Split('.')[0]);
                                    }
                                    else
                                        ScenarioNr = Convert.ToInt32(args[1]);
                                    if (args.Length == 0)
                                    {
                                        GlobalVars.Instance.setReadHandOverData(settings.getItemString("outputDir") + "dataCtool" + "_" + farmNr + "_" + GlobalVars.Instance.reuseCtoolData + ".txt");
                                        GlobalVars.Instance.setWriteHandOverData(settings.getItemString("outputDir") + "dataCtool" + "_" + farmNr + "_" + ScenarioNr.ToString() + ".txt");
                                    }
                                    else
                                    {
                                        GlobalVars.Instance.setReadHandOverData(settings.getItemString("outputDir") + "dataCtool" + "_" + args[0] + "_" + args[4] + ".txt");
                                        GlobalVars.Instance.setWriteHandOverData(settings.getItemString("outputDir") + "dataCtool" + "_" + args[0] + "_" + args[1] + ".txt");
                                    }

                                    GlobalVars.Instance.openSeyda(settings.getItemString("outputDir"), ScenarioNr.ToString());
                                    
                                    if (farmInformation.doesIDExist(settingsnr))
                                    {

                                        string outputName;
                                        if ((GlobalVars.Instance.reuseCtoolData==-1) && (args.Length == 0))
                                            outputName=settings.getItemString("outputDir") + "outputFarm" + farmNr.ToString() + "BaselineScenarioNr" + ScenarioNr.ToString() + ".xml";
                                        else
                                            outputName = settings.getItemString("outputDir") + "outputFarm" + farmNr.ToString() + "ScenarioNr" + ScenarioNr.ToString() + ".xml";
                                       
                                        //XmlWriter writer = XmlWriter.Create(outputName);
                                        XmlWriter writer = GlobalVars.Instance.OpenOutputXML(outputName);
                                        
                                        outputName = outputName.Split('.')[0];
                                        //outputName = outputName + ".xls";
                                        GlobalVars.Instance.OpenOutputTabFile(outputName, settings.getItemString("outputDir"));
                                        GlobalVars.Instance.OpenDebugFile();

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
                                        GlobalVars.Instance.OpenCtoolFile();
                                        List<CropSequenceClass> rotationList = new List<CropSequenceClass>();
                                        int minRotation = 99, maxRotation = 0;
                                        //if (GlobalVars.Instance.getRunFullModel())
                                        //{
                                        //do cropped area first
                                        if(GlobalVars.Instance.GetstrictGrazing() == true)
                                        {
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
                                            if (GlobalVars.Instance.reuseCtoolData != -1)
                                            {
                                                double[] oldArea = new double[20];
                                                double[] newArea = new double[20];
                                                for (int i = 0; i < 20; i++)
                                                {
                                                    oldArea[i] = 0;
                                                    newArea[i] = 0;
                                                }
                                                for (int i = 0; i < rotationList.Count; i++)
                                                {
                                                    int soilNr=rotationList[i].GetsoiltypeNo();
                                                    newArea[soilNr] += rotationList[i].getArea();
                                                }
                                                string[] lines=null;
                                                try
                                                {
                                                    lines = System.IO.File.ReadAllLines(GlobalVars.Instance.getReadHandOverData());
                                                }
                                                catch
                                                {
                                                    GlobalVars.Instance.Error("could not find " + GlobalVars.Instance.getReadHandOverData());
                                                }
                                                
                                                for (int j = 0; j < lines.Length; j++)
                                                {
                                                    string[] tmp=lines[j].Split('\t');
                                                    
                                                    int soilNr = Convert.ToInt32(tmp[0]);
                                                    oldArea[soilNr] += Convert.ToDouble(tmp[9]);
                                                }
                                                for (int j = 0; j < 20; j++)
                                                {
                                                    if(oldArea[j]!= newArea[j])
                                                    {
                                                        GlobalVars.Instance.Error("area for soil type "+j.ToString()+" donot match");
                                                    }
                                                }
                                              
                                            }
                                                if (farmArea > 0)
                                                    areaWeightedDuration /= farmArea;
                                                else
                                                    areaWeightedDuration = 1;
                                        }
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

                                        if (!GlobalVars.Instance.getRunFullModel()) //only called when only the livestock excretion is needed
                                        {
                                            GlobalVars.Instance.CalcAllFeedAndProductsPotential(rotationList);
                                            //write output to xml file
                                            GlobalVars.Instance.writeGrazedItems();
                                            for (int i = 0; i < listOfHousing.Count; i++)
                                            {
                                                listOfHousing[i].Write();
                                            }

                                            for (int i = 0; i < listOfManurestores.Count; i++)
                                            {
                                                manureStore amanurestore2 = listOfManurestores[i];
                                                amanurestore2.Write();
                                            }
                                            GlobalVars.Instance.OpenLivestockFile();
                                            for (int i = 0; i < listOfLivestock.Count; i++)
                                            {
                                                livestock anAnimal = listOfLivestock[i];
                                                anAnimal.Write();
                                                anAnimal.WriteLivestockFile();
                                            }
                                            GlobalVars.Instance.CloseLivestockFile();
                                            GlobalVars.Instance.Write(false);
                                            GlobalVars.Instance.CloseOutputTabFile();
                                            GlobalVars.Instance.CloseOutputXML();
                                            GlobalVars.Instance.log(GlobalVars.Instance.getRunFullModel().ToString(), 5);
                                        }

                                        else
                                        {
                                            GlobalVars.Instance.OpenPlantFile();
                                            
                                            for (int rotationID = 0; rotationID < rotationList.Count; rotationID++)
                                            {

                                                //rotationList[rotationID].getGrazedFeedItems();
                                                rotationList[rotationID].CalcModelledYield();
                                                rotationList[rotationID].CheckYields();
                                                // rotationList[rotationID].getAllFeedItems();
                                                GlobalVars.Instance.log("Seq " + rotationID.ToString() + " DM " + rotationList[rotationID].GetDMYield().ToString(), 5);
                                                GlobalVars.Instance.log("Seq " + rotationID.ToString() + " C " + rotationList[rotationID].getCHarvested().ToString(), 5);
                                                
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
                                                    listOfHousing[i].Write();
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
                                          //if (((listOfHousing.Count() == 0) && (listOfLivestock.Count > 0))) //there are grazing animals that are not housed
                                            {
                                                for (int i = 0; i < listOfLivestock.Count; i++)
                                                {
                                                    livestock anAnimal = listOfLivestock[i];
                                                    if (anAnimal.GethousingDetails().Count == 0)
                                                    {
                                                        anAnimal.Write();
                                                        GlobalVars.Instance.writeEndTab();
                                                    }
                                                }
                                            }
                                            

                                            GlobalVars.Instance.OpenPlantFile();

                                            for (int i = 0; i < rotationList.Count; i++)
                                            {
                                                CropSequenceClass rotation = rotationList[i];
                                                rotation.Write();
                                            }
                                            GlobalVars.Instance.ClosePlantFile();

                                            GlobalVars.Instance.CalcAllFeedAndProductsPotential(rotationList);
                                            GlobalVars.Instance.Write(true);
                                            GlobalVars.Instance.writeStartTab("ExpectedYield");
                                            for (int i = 0; i < rotationList.Count; i++)
                                                rotationList[i].calculateExpectedYield("ExpectedYield0_CropSequenceClass" + i.ToString());
                                            GlobalVars.Instance.writeEndTab();
                                            GlobalVars.Instance.CloseOutputXML();
                                            GlobalVars.Instance.CloseOutputTabFile();
                                            GlobalVars.Instance.logFileStream.Close();
                                            GlobalVars.Instance.OpenLivestockFile();
                                            for (int i = 0; i < listOfLivestock.Count; i++)
                                            {
                                                livestock anAnimal = listOfLivestock[i];
                                                anAnimal.WriteLivestockFile();

                                            }
                                            GlobalVars.Instance.CloseLivestockFile();
                                            GlobalVars.Instance.CloseCtoolFile();
                                            XmlWriter writerCtool;
                                            writerCtool = XmlWriter.Create(outputName + "CtoolFile.xml");
                                            XElement fileCtool = new XElement("file");
                                            for(int i=0;i<rotationList.Count();i++)
                                            {
                                                XElement rotation = new XElement("rotation");
                                                rotation.Add(rotationList[i].node);
                                                fileCtool.Add(rotation);
                                         
                                            }
                                            fileCtool.Save(writerCtool);
                                            writerCtool.Close();

                                            if (GlobalVars.Instance.reuseCtoolData == -1)
                                            {
                                                //Jonas - replace 20 with variable name
                                                double [] rotarea = new double[20];
                                                double[] fomcLayer1 = new double[20];
                                                double[] fomcLayer2 = new double[20];
                                                double[] humcLayer1 = new double[20];
                                                double[] humcLayer2 = new double[20];
                                                double[] romcLayer1 = new double[20];
                                                double[] romcLayer2 = new double[20];
                                                double[] FOMn = new double[20];
                                                double[] rotresidualMineralN = new double[20];
                                                for (int soilNo = 0; soilNo < 20; soilNo++)
                                                {
                                                    rotarea[soilNo] = 0;
                                                    fomcLayer1[soilNo] = 0;
                                                    fomcLayer2[soilNo] = 0;
                                                    humcLayer1[soilNo] = 0;
                                                    humcLayer2[soilNo] = 0;
                                                    romcLayer1[soilNo] = 0;
                                                    romcLayer2[soilNo] = 0;
                                                    FOMn[soilNo] = 0;
                                                    rotresidualMineralN[soilNo] = 0;
                                                }
                                                for (int i = 0; i < rotationList.Count; i++)
                                                {
                                                    CropSequenceClass rotation = rotationList[i];
                                                    int soiltypeNo=rotation.GetsoiltypeNo();
                                                    rotarea[soiltypeNo] += rotation.getArea();
                                                    fomcLayer1[soiltypeNo] += rotation.aModel.fomc[0] * rotation.getArea();
                                                    fomcLayer2[soiltypeNo] += rotation.aModel.fomc[1] * rotation.getArea();
                                                    humcLayer1[soiltypeNo] += rotation.aModel.humc[0] * rotation.getArea();
                                                    humcLayer2[soiltypeNo] += rotation.aModel.humc[1] * rotation.getArea();
                                                    romcLayer1[soiltypeNo] += rotation.aModel.romc[0] * rotation.getArea();
                                                    romcLayer2[soiltypeNo] += rotation.aModel.romc[1] * rotation.getArea();
                                                    FOMn[soiltypeNo] += rotation.aModel.FOMn * rotation.getArea();
                                                    rotresidualMineralN[soiltypeNo] += rotation.GetResidualSoilMineralN();
                                                }
                                                System.IO.StreamWriter extraCtoolData = new System.IO.StreamWriter(GlobalVars.Instance.getWriteHandOverData());
                                                for (int soilNo = 0; soilNo < 20; soilNo++)
                                                {
                                                    if (rotarea[soilNo] > 0)
                                                    {
                                                        extraCtoolData.WriteLine(soilNo.ToString() + '\t' + (fomcLayer1[soilNo] / rotarea[soilNo]).ToString() + '\t' + (fomcLayer2[soilNo] / rotarea[soilNo]).ToString()
                                                            + '\t' + (humcLayer1[soilNo] / rotarea[soilNo]).ToString() + '\t' + (humcLayer2[soilNo] / rotarea[soilNo]).ToString()
                                                            + '\t' + (romcLayer1[soilNo] / rotarea[soilNo]).ToString() + '\t' + (romcLayer2[soilNo] / rotarea[soilNo]).ToString()
                                                            + '\t' + FOMn[soilNo] / rotarea[soilNo] + '\t' + rotresidualMineralN[soilNo] / rotarea[soilNo] + '\t' + rotarea[soilNo]);
                                                    }
                                                }
                                                extraCtoolData.Close();
                                                GlobalVars.Instance.ClosePlantFile();
                                            }
                                        }
                                        GlobalVars.Instance.closeSeyda();
                                        GlobalVars.Instance.CloseDebugFile();
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
                Console.WriteLine("Finished after running " + (settingsID+1).ToString() + " scenarios");
                timer.Stop();
                // Get the elapsed time as a TimeSpan value.
                TimeSpan ts = timer.Elapsed;

                // Format and display the TimeSpan value. 
                string elapsedTime = String.Format("{0:00}:{1:00}:{2:00}.{3:00}",
                    ts.Hours, ts.Minutes, ts.Seconds,
                    ts.Milliseconds / 10);
                Console.WriteLine("RunTime (hrs:mins:secs) " + elapsedTime);

                //Console.WriteLine("Time elapsed = ");
                //
            }

        }

        static string[] fileName(string[] args, FileInformation settings, string file)
        {
            string[] names =new string[2];
            if (args.Length != 0)
            {
                List<string> tmpPath = new List<string>(settings.PathNames);
                List<int> tmpID = new List<int>(settings.Identity);
                settings.Identity.Clear();
                settings.PathNames.Clear();
                settings.setPath("CommonSettings(-1)");
                string alternativePath = settings.getItemString("alternativePath");
                settings.PathNames = tmpPath;
                settings.Identity = tmpID;
                string constants = settings.getItemString(file);
                string[] constantsList = constants.Split('\\');
                string fileName = constantsList[constantsList.Length - 1];
                if (args[0] == "-1")
                    alternativePath += "\\" + fileName;
                else
                    alternativePath += "\\" + args[0] + "\\" + fileName;
                if (File.Exists(alternativePath))
                {
                    names[0] = alternativePath;
                }
                else
                {
                    names[0] = constants;
                }
                alternativePath = alternativePath.Split('.')[0] + "Alternative.xml";
                if (File.Exists(alternativePath))
                {
                    names[1] = alternativePath;
                }
                else
                {
                    names[1] = constants.Split('.')[0] + "Alternative.xml"; 
                }
            }
            else
            {
                names[0] = settings.getItemString(file);
                names[1] = settings.getItemString(file).Split('.')[0] + "Alternative.xml"; ;
            }
            return names;
          

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
                GlobalVars.Instance.OpenLivestockFile();
                for (int i = 0; i < rotationList.Count; i++)
                {
                    CropSequenceClass rotation = rotationList[i];
                    rotation.Write();
                }
                
               
                GlobalVars.Instance.ClosePlantFile();
                GlobalVars.Instance.CloseLivestockFile();
                GlobalVars.Instance.CalcAllFeedAndProductsPotential(rotationList);
                GlobalVars.Instance.Write(true);
                GlobalVars.Instance.writeStartTab("ExpectedYield");
                for (int i = 0; i < rotationList.Count; i++)
                {


                    rotationList[i].calculateExpectedYield("ExpectedYield(0)_crop"+i.ToString());
                
                }
                GlobalVars.Instance.writeEndTab();
                GlobalVars.Instance.CloseOutputXML();
                GlobalVars.Instance.CloseOutputTabFile();
        }
            
    }

}
