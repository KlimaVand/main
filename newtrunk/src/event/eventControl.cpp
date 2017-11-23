
#include "../base/common.h"
#include "eventControl.h"
#include "../base/climate.h"
#include "../base/settings.h"
#include "../base/controlParameters.h"
#include "../tools/fileAccess.h"

#ifndef __BCplusplus__

#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

#endif

eventControl::eventControl():
    base()
{
    InitVariables();
}

eventControl::~eventControl() {
    if (theOperational_manager) {
        delete theOperational_manager;
    }

    if (!WithoutFields) {
        if (theCropRotation) {
            delete theCropRotation;
        }
    }

    if (theLivestock) {
        delete theLivestock;
    }
}

/*
 * Only called by constructor, to avoid redundant code
 */
void eventControl::InitVariables() {
    theLivestock           = nullptr;
    theCropRotation        = new cropRotation;
    theOperational_manager = new operationsManager();    // added by NJH

    Nbudget.SetNames("farm", "N");    // ???!!!
}

void eventControl::Initialize(bsTime * stopTime,
                              string   inputDir,
                              string   climateDir,
                              string   climateFilename,
                              string   outputPath) {
    string filenameCSV;

#ifndef __BCplusplus__
    filenameCSV = outputPath + "/INDICATX.XLS";
#else
    filenameCSV = outputPath + "\\INDICATX.XLS";
#endif

    theOutput -> Initialize(filenameCSV);

    bool Irrigation            = false;
    int  daysBetweenIrrigation = 0;
    int  irrigationDelay       = 0;

    // inserted by NJH Oct 2007
    bool SelltheStrawProducts       = 0;
    bool SelltheNonRoughageProducts = 0;

    WithoutFields = false;

    bool ScaleBuildings = false;

    //
    bool       pigfarm     = false;
    bool       dairyfarm   = false;
    bool       beeffarm    = false;
    bool       poultryfarm = false;
    int        FingerFlow  = 0;
    fileAccess hd;

    hd.changeDir(globalSettings -> getInputDirectory());

    commonData data;

    data.setCritical(false);
    data.readFile("farm.dat");
    data.FindSection("farm");
    data.FindItem("Irrigation", &Irrigation);
    data.FindItem("daysBetweenIrrigation", &daysBetweenIrrigation);
    data.FindItem("irrigationDelay", &irrigationDelay);
    data.FindItem("FingerFlow", &FingerFlow);

    double MinFieldArea = 0;

    data.FindItem("MinFieldArea", &MinFieldArea);
    data.FindItem("SelltheStrawProducts", &SelltheStrawProducts);
    data.FindItem("SelltheNonRoughageProducts", &SelltheNonRoughageProducts);
    data.FindItem("WithoutFields", &WithoutFields);
    data.FindItem("ScaleBuildings", &ScaleBuildings);

    //

#ifdef TUPLE
    string FarmID = theFarmData -> FarmDatas.FarmID;

    theControlParameters -> SetFarmID(FarmID);
#endif

    data.FindSection("livestock");
    data.FindItem("pigfarm", &pigfarm);
    data.FindItem("dairyfarm", &dairyfarm);
    data.FindItem("beeffarm", &beeffarm);
    data.FindItem("poultryfarm", &poultryfarm);

   
    theLivestock = new livestock;
    

    theProducts -> InitializeProducts("RunProducts.dat", "StandardProducts.dat");    // ,(dairyfarm || beeffarm));

    // theProducts->InitializePrices(stopTime);
    theProducts -> InitializeFeed("FeedTable2004.dat");
    theClimate -> InitClimate(climateDir, climateFilename);
	theProducts->InitializePrices(stopTime);
    // change back to input directory, just in case the climate directory was elsewhere
    hd.changeDir(inputDir);

    if (!WithoutFields) {
        theCropRotation -> Initialize(WithoutFields, Irrigation, daysBetweenIrrigation, irrigationDelay, FingerFlow,
                                      MinFieldArea);
    }

     theProducts->UpdateProductPtr();
    hd.changeDir(globalSettings -> getInputDirectory());
    theTechnics -> Initialize("operatio.dat", "techeqp.dat");
    theBuildings -> InitializeAll("RunBuildings.dat", pigfarm, dairyfarm, beeffarm, ScaleBuildings);

    if (theLivestock) {
        theLivestock -> Initialize("livestock.dat", pigfarm, dairyfarm, beeffarm, theBuildings);
    }

    theOperational_manager -> Initialize(theLivestock, theCropRotation, theBuildings);    // added by NJH 1.5.00

    // inserted by NJH Oct 2007
    theProducts -> SetSelltheStrawProducts(SelltheStrawProducts);
    theProducts -> SetSelltheNonRoughageProducts(SelltheNonRoughageProducts);
}

void eventControl::ReceivePlan(string fileExtension,
                               string inputDir, int yearNumber) {
    string fileName; 
	fileAccess hd;
	int index;
	string name="Fields." + fileExtension;
	if(!hd.fileExists(name))
	{
		fileName="Fields.fnn";
		index=yearNumber;
	}
	else
	{
		fileName = "Fields." + fileExtension;
		index=0;
	}
    theCropRotation -> ReceivePlan(fileName,index);

    if (theLivestock) {
        theLivestock -> ReceivePlan(fileExtension);
    }

    theOperational_manager -> ReceivePlan(fileExtension, inputDir);

    // scale buildings after call to theOperational_manager->ReceivePlan, so know which buildings are in use
    if ((theLivestock)) {
        theBuildings -> ScaleBuildings();
    }
}

/*
 * Handle the daily updates for climate, indicate, operationsManager, Livestock, CropRotation, buildigns
 */
void eventControl::DailyUpdate() {
	theClimate->Update();    // Load today's climate

	if ((theTime.GetDay() == 14) && (theTime.GetMonth() == 4) && (theTime.GetYear() == 1952))
	{
        cout << "" << flush;
    }

    theOutput -> DailyUpdate();
    theOperational_manager -> daily_update();

    if (theLivestock) {
        theLivestock -> DailyUpdate();
    }

    if (!WithoutFields) {
        theCropRotation -> DailyUpdate();
    }

    theBuildings -> DailyUpdate();

    if (globalSettings -> DetailsData.getOperationalDetails()) {
        theOperational_manager -> ShowOpsDetails(false);    // NJH added this November 2006
    }

#ifndef NITROSCAPE
    if ((theTime.GetDay() == 5) || (theTime.GetDay() == 15) || (theTime.GetDay() == 25)) {
        cout << "." << flush;
    }
#endif

    // cout << endl;

    /*
     *      filehandle = theMessage->GiveDebugHandle();
     * filehandle << endl ;
     * theMessage->CloseDebugHandle();
     */
}

void eventControl::GiveIndicator(indicatorTypes indicatorType,
                                 int            yearNumber) {
    if ((indicatorType == environmentalIndicator) && theLivestock) {
        theLivestock -> FlushIndicator(indicatorType);
    }

    // theOutput->AddStateIndicator("98.91 current year"," ",theTime.GetYear());
    theOutput -> FlushIndicator(indicatorType, yearNumber);
}

void eventControl::Simulation(int    runNumber,
                              bsTime stopTime,
                              string inputDir,
                              string outputDir,
                              string climateDir,
                              string climateFilename,
                              string economicIndicatorDate,
                              string environmentalIndicatorDate) {
    int    yearNumber = 0;
    bsTime economicIndicatorTime, environmentalIndicatorTime, yearStopTime;

    economicIndicatorTime.SetTime(economicIndicatorDate);    // default 1/9
    environmentalIndicatorTime.SetTime(environmentalIndicatorDate);    // default 1/9

    if (economicIndicatorTime <= theTime) {
        economicIndicatorTime.AddOneYear();    // added by JB
    }

    if (environmentalIndicatorTime <= theTime) {
        environmentalIndicatorTime.AddOneYear();
    }

    cout << "Input directory    " << inputDir << endl;
    cout << "Output directory   " << outputDir << endl;
    cout << "Climate directory  " << climateDir << endl;
    cout << "Climate file       " << climateFilename << endl;

#ifdef VS2008
    if (_chdir(outputDir.c_str()) != 0)
#else
    if (chdir(outputDir.c_str()) != 0)
#endif

#if defined(__BCplusplus__) || defined(__MINGW32__)
    	 mkdir(outputDir.c_str());
#endif

#if defined(unix) || defined(CYGWIN)
    mkdir(outputDir.c_str(), '-p');
#endif


#ifdef VS2008
    if (_chdir(inputDir.c_str()) != 0)
#else
    if (chdir(inputDir.c_str()) != 0)
#endif

    theMessage -> FatalError("Event:: input directory ", inputDir, " not found!");

    Initialize(&stopTime, inputDir, climateDir, climateFilename, outputDir);

    bool onceOnly = false;

    // Yearly outerloop begins
    while ((theTime < stopTime) && (!onceOnly)) {
        cout << endl;

        if (runNumber <= 9) {
            cout << " Run " << "0" << runNumber;
        } else {
            cout << " Run " << runNumber;
        }

        if (yearNumber <= 9) {
            cout << " Year " << "0" << yearNumber;
        } else {
            cout << " Year " << yearNumber;
        }

        // Initialize modules with results from fixed plan
        char fileExtension[5];

        fileExtension[0] = '\0';

        if (yearNumber < 100) {
            sprintf(fileExtension, "f%02d", yearNumber);
        } else {
            sprintf(fileExtension, "f%03d", yearNumber);
        }

        cout << endl << "Receiving fixed production plans" << endl;

        ReceivePlan(fileExtension, inputDir,yearNumber);

        cout << endl << "Simulating production ...................................." << endl
             << "Progress              ";

        // innerloop of production
        yearStopTime = theTime;

        yearStopTime.AddOneYear();

        while ((theTime < yearStopTime) && (!onceOnly)) {
            onceOnly = false;    // enable if you want to pass through the daily loop only once per year

            // theProducts->SellPlantItems(); !!!???
            DailyUpdate();

            // if (!theLivestock) theProducts->SellPlantItems();  //sell plant products only if no animals present!! NJH April 2001
            theTime.AddOneDay();

            if (theTime == economicIndicatorTime) {
                theTechnics -> YearlyCalc();
                GiveIndicator(economicIndicator, yearNumber);
                theTechnics -> Reset();
            }

            if (theTime == environmentalIndicatorTime) {
                GiveIndicator(environmentalIndicator, yearNumber);
            }
        }

        // Reset before next year
        theProducts -> resetProducts();

        // theProducts->YearlyUpdate();
        yearNumber++;
        economicIndicatorTime.AddOneYear();
        environmentalIndicatorTime.AddOneYear();
    }

    EndSimulation(true);

    // Above line creates memory problems sometimes - see soilProfile::GetMaximumDepth. Removed for now - BMP 11. November 2006 ??? !!!!
}

void eventControl::EndSimulation(bool show) {
    if (theLivestock) {
        theLivestock -> checkBalances(show);
    }

    theBuildings -> EndBudget(show);
    theCropRotation -> EndBudget();
}
