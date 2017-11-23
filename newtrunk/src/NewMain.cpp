
#ifndef NITROSCAPE

#ifdef __BCplusplus__

#include "../base/settings.h"
#include "../base/common.h"
#include "../base/bstime.h"
#include "../products/products.h"
#include "../base/message.h"
#include "../legislation/legislation.h"
#include "../building/buildings.h"
#include "../base/climate.h"
#include "../event/eventControl.h"
#include "../base/controlParameters.h"
#include "../technics/technics.h"
#include "../manager/manureManager.h"
#include "../tools/fileAccess.h"
#include "../base/commonData.h"

#endif

#ifndef __BCplusplus__

#include "base/common.h"
#include "base/bstime.h"
#include "products/products.h"
#include "base/message.h"
#include "legislation/legislation.h"
#include "building/buildings.h"
#include "base/climate.h"
#include "event/eventControl.h"
#include "base/controlParameters.h"
#include "technics/technics.h"
#include "manager/manureManager.h"
#include "tools/fileAccess.h"
#include "base/commonData.h"
#include "tools/compare.h"

#endif

#include <time.h>

using namespace std;

#ifndef NITROSCAPE
bsTime              theTime;
products *          theProducts;
message *           theMessage;
IndicatorOutput *   theOutput;
legislate *         theLegislation;
technics *          theTechnics;
buildings *         theBuildings;
climate *           theClimate;
controlParameters * theControlParameters;
manureManager *     theManure_manager;
settings *          globalSettings;
livestock *    theLivestock;
// static fstream * fs; //scenario log file @@@
void Display()
{
    int NW = theMessage -> NumOfWarnings();
    if (NW > 0)
    {
        cout << endl << endl << "The simulation has resulted in " << NW;    // BMP �
        if (NW > 1)
            cout << " warnings.";
        else
            cout << " warning.";
        cout << " See 'WARNINGS.TXT'";
    }
    else
        cout << endl << endl << "Simulation completed.";
    cout << endl << endl;
}

int modified()
{
    // base aBase;
    bsTime stopTime;
    string InputDirectory1;
    string OutputDirectory1;
    string ClimateDirectory;
    string ClimateFileName;
    // ScenarioFileName,
    string EconomicIndicatorDate      = "1/1";
    string EnvironmentalIndicatorDate = "1/1";
    int    StartYear                  = 0;
    int    StopYear                   = 0;
    int    day                        = 1;
    int    month                      = 8;
    int    ClimateFileType            = -1;
    int    EpotCalculationMethod      = 0;
    int    VolatModel                 = 0;
    int    singleField                = -1;
    int    fertiliserUnit             = 0;
    // bool SellPlantProducts=false;
    bool UseReductionParameters  = false;    // Default determined by theControlParameters
    bool MeasuredSoilTemperature = false;    // Default determined by theControlParameters
    bool AirToSoilTemperature    = false;    // Default determined by theControlParameters
    bool WriteSoilTemperature    = false;    // Default determined by theControlParameters
    bool SimpleTemperatureModel  = false;    // Default determined by theControlParameters
    // WriteDetails,                  // Default determined by theControlParameters
    bool GrassWeed        = false;    // Default determined by theControlParameters
    bool Organic          = false;    // Default determined by theControlParameters
    bool ForcedOperations = false;    // Default determined by theControlParameters
    bool ForcedHarvest    = false;
    bool fieldsFlexSize   = false;    // When true, fields are allowed to shrimp or expand
    bool uniSizeFields    = false;    // When true, all fields are 1 ha big
    int  index;
    // Used to read info that is common  for all runs in system.dat. Can be overwritten later in the individual run definitions
    commonData * data = new commonData();
    data -> setCritical(true);
    data -> readFile("system.dat");
    char the_path[256];
#ifdef VS2008
    _getcwd(the_path, 255);
#else
    getcwd(the_path, 255);
#endif
    cout << "Reading system.dat from " << string(the_path) << endl;
    int firstScenarie = 0;
    int num           = 0;
    int offset        = 0;
    data -> getSectionsNumbers("run", &firstScenarie, &num);
    data -> setCritical(false);
    data -> FindItem("JumpStart", &firstScenarie);
    data -> FindItem("JumpStop", &num);
    data -> FindItem("offset", &offset);
    bool remove = false;
    data -> FindItem("removeOldData", &remove);
    for (index = firstScenarie; index <= num; index++)
    {
        data -> setCritical(true);
        data -> FindSection("run", index);
        data -> FindItem("StartYear", &StartYear);
        data -> FindItem("StopYear", &StopYear);
        data -> FindItem("InputDirectory", &InputDirectory1);
        data -> FindItem("OutputDirectory", &OutputDirectory1);
        // Although some compilers may accept \ in an include path,
        // they are all required to accept /.
        // so / should be  a valid path separator in Windows too
        // root/path is equivalent to root//path
        OutputDirectory1 = OutputDirectory1 + "/";
        InputDirectory1 = InputDirectory1 + "/";
        globalSettings = new settings();
        globalSettings -> setInputDir(InputDirectory1);
        globalSettings -> setOutputDir(OutputDirectory1);
        globalSettings -> run = index;
        if (remove)
        {
            fileAccess remove;
            string     path = globalSettings -> getOutputDirectory();
            remove.deleteEveryFileInFolder(path);
        }
        theControlParameters = new controlParameters;
        theControlParameters -> SetoperationOffset(offset);
        data -> FindItem("ClimateFileName", &ClimateFileName);
        data -> setCritical(false);
        if (!data -> FindItem("ClimateDirectory", &ClimateDirectory))
        {
            string instring = ClimateFileName;
            int    loc      = instring.find_last_of("/\\");    // separate directory and file names
            if (loc > 0)
            {
                ClimateDirectory = instring.substr(0, loc);
                ClimateFileName  = instring.substr(loc + 1);
            }
            else
            {
                cout << instring << endl;
                exit(99);
            }
        }
        if (!data -> FindItem("ClimateFileType", &ClimateFileType))
        {
            int size = ClimateFileName.size();
            if (ClimateFileName.substr(size - 3, size) == "CLM")
                ClimateFileType = 0;
            else if (ClimateFileName.substr(size - 3, size) == "clm")
                ClimateFileType = 0;
            else if (ClimateFileName.substr(size - 3, size) == "KML")
                ClimateFileType = 1;
            else if (ClimateFileName.substr(size - 3, size) == "kml")
                ClimateFileType = 1;
            else if (ClimateFileName.substr(size - 3, size) == "dat")
                ClimateFileType = 5;
            else if (ClimateFileName.substr(size - 3, size) == "csv")
                ClimateFileType = 8;
        }
        data -> FindItem("EnvironmentalIndicator", &EnvironmentalIndicatorDate);
        data -> FindItem("EpotCalculationMethod", &EpotCalculationMethod);
        data -> FindItem("EconomicIndicatorDate", &EconomicIndicatorDate);
        data -> FindItem("VolatModel", &VolatModel);
        theControlParameters -> SetvolatModel(VolatModel);
        if (data -> FindItem("SimpleTemperatureModel", &SimpleTemperatureModel))
            theControlParameters -> SetSimpleTemperatureModel(SimpleTemperatureModel);
        if (data -> FindItem("AirToSoilTemperature", &AirToSoilTemperature))
            theControlParameters -> SetAirToSoilTemperature(AirToSoilTemperature);
        if (data -> FindItem("uniSizeFields", &uniSizeFields))
            theControlParameters -> SetFieldsUniSize(uniSizeFields);
        if (data -> FindItem("fieldsFlexSize", &fieldsFlexSize))
            theControlParameters -> SetFieldsFlexSize(fieldsFlexSize);
        if (data -> FindItem("SingleField", &singleField))
        {
            theControlParameters -> SetSingleField(singleField);
            globalSettings -> DetailsData.PlantDetails = true;
            globalSettings -> DetailsData.SoilDetails  = true;
        }
        if (data -> FindItem("fertiliserUnit", &fertiliserUnit))
            theControlParameters -> SetFertiliserUnit(fertiliserUnit);
        if (data -> FindItem("MeasuredSoilTemperature", &MeasuredSoilTemperature))
            theControlParameters -> SetMeasuredSoilTemperature(MeasuredSoilTemperature);
        if (data -> FindItem("WriteSoilTemperature", &WriteSoilTemperature))
            theControlParameters -> SetWriteSoilTemperature(WriteSoilTemperature);
        if (data -> FindItem("ForcedOperations", &ForcedOperations))
            theControlParameters -> SetForcedOperations(ForcedOperations);
        data -> FindItem("ForcedOperations", &ForcedHarvest);
        theControlParameters -> SetForcedHarvest(ForcedHarvest);
        double HarvestWaste = 0.05;
        data -> FindItem("HarvestWaste", &HarvestWaste);
        if (data -> FindItem("HarvestWaste", &HarvestWaste))
        {
            theControlParameters -> SetHarvestWaste(HarvestWaste);        // 0.05 - 0.10 may be a sensible range
        }
        double StrawLeft = 0.45;
        data -> FindItem("StrawLeft", &StrawLeft);
        theControlParameters -> SetStrawLeft(StrawLeft);
        if (data -> FindItem("ForcedOperations", &ForcedOperations))
            theControlParameters -> SetUseReductionParameters(UseReductionParameters);
        if (data -> FindItem("ForcedOperations", &ForcedOperations))
            theControlParameters -> SetGrassWeed(GrassWeed);
        if (data -> FindItem("ForcedOperations", &ForcedOperations))
            theControlParameters -> SetOrganicFarm(Organic);
		string AlternativePath;
		if (data->FindItem("AlternativePath", &AlternativePath))
			globalSettings->setInputDirAlternative(AlternativePath);
        data -> FindItem("StartMonth", &month);
        theTime.SetTime(day, month, StartYear);
        cout << "Simulation starts: " << theTime << endl;
        stopTime.SetTime(day, month, StopYear);                           // JB added this
        cout << "Simulation stops:  " << stopTime << endl;
        string FN1, FN2;
        // chdir(OutputDirectory);
        FN1               = OutputDirectory1 + "warnings.txt";
        FN2               = OutputDirectory1 + "logfile.txt";
        theProducts       = new products;
        theOutput         = new IndicatorOutput;
        theMessage        = new message;
        theTechnics       = new technics("technics", 0);                            // BMP
        theClimate        = new climate(ClimateFileType, EpotCalculationMethod);    // BMP
        theBuildings      = new buildings;
        theManure_manager = new manureManager();
        eventControl * aEventControl = new eventControl;
        theMessage -> InitMessage(FN1, FN2);
        globalSettings -> readParameter();
        cout << globalSettings -> version << endl;
        cout << "Starting Simulation" << endl;
        aEventControl -> Simulation(index, stopTime, InputDirectory1, OutputDirectory1, ClimateDirectory,ClimateFileName, EconomicIndicatorDate,
                                    EnvironmentalIndicatorDate);
        Display();
        delete theProducts;
        delete theOutput;
        if (index == num)
            delete data;
        delete theTechnics;
        delete theClimate;
        delete theManure_manager;
        delete aEventControl;
        delete theControlParameters;
        delete globalSettings;
        delete theMessage;
    }
    return 0;
}

double diffclock(clock_t clock1,
                 clock_t clock2)
{
    double diffticks = clock1 - clock2;
    double diffms    = (diffticks * 1000) / CLOCKS_PER_SEC;
    return diffms;
}

int main()      // NJH, 28.2.07
{
    int        times = 1;
    fileAccess hd;
    string     path  = hd.getCurrentPath();
    clock_t    begin = clock();
    for (int i = 0; i < times; i++)
    {
        hd.changeDir(path);
        modified();
    }
    clock_t end = clock();
    cout << "Time elapsed: " << double(diffclock(end, begin)) / times << " ms" << endl;
    return 0;
}
#endif

#endif

