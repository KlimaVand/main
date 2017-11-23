
#ifdef NITROSCAPE

/*
 * PALM_UNIT  -name fasset\
 *          -functions {c++ fasset}\
 *          -object_files {fasset.o libFarm.a}\
 *          -parallel mpi\
 *          -minproc 1\
 *          -maxproc 16
 */

/*
 * PALM_SPACE  -name intall\
 *           -shape (nb_eco)\
 *           -element_size PL_INTEGER
 */

/*
 * PALM_SPACE  -name fl3\
 *           -shape (3)\
 *           -element_size PL_REAL
 */

/*
 * PALM_SPACE  -name fl6\
 *           -shape (6)\
 *           -element_size PL_REAL
 */

/*
 * PALM_SPACE  -name fl2x4\
 *           -shape (2,4)\
 *           -element_size PL_REAL
 */

/*
 * PALM_SPACE  -name fl12all\
 *           -shape (nb_eco,12)\
 *           -element_size PL_REAL
 */

/*
 * PALM_SPACE  -name FarmToLinkerS\
 *           -shape (1)\
 *           -element_size PL_AUTO_SIZE\
 *           -items { {itk intall} {itkdet fl12all} {em fl3} }
 *
 */

/*
 * PALM_SPACE  -name LinkerToFarmS\
 *           -shape (nb_eco)\
 *           -element_size PL_AUTO_SIZE\
 *           -items { {swc fl6} {prod fl2x4} }
 */

/*
 * PALM_OBJECT  -name LinkerToFarm\
 *            -space LinkerToFarmS\
 *            -localisation REPLICATED_ON_ALL_PROCS\
 *            -intent IN
 */

/*
 * PALM_OBJECT  -name FarmToLinker\
 *            -space FarmToLinkerS\
 *            -tag ON\
 *            -localisation REPLICATED_ON_ALL_PROCS\
 *            -intent OUT
 */

#include <stdio.h>
#include <stdlib.h>

// FASSET includes

#include "../base/settings.h"
#include "../base/common.h"
#include "../base/bstime.h"
#include "../products/products.h"
#include "../base/message.h"
#include "../base/climate.h"
#include "../base/controlParameters.h"
#include "../technics/technics.h"
#include "../tools/fileAccess.h"
#include "../Nitroscape/pixiData.h"
#include "../base/linklist.h"
#include "../Nitroscape/MMSClass.h"
#include "../Nitroscape/ecosystinterface.h"

#ifdef NITROSCAPE
bsTime              theTime;
products *          theProducts;
message *           theMessage;
legislate *         theLegislation;
IndicatorOutput *   theOutput;
technics *          theTechnics;
buildings *         theBuildings;
climate *           theClimate;
controlParameters * theControlParameters;
manureManager *     theManure_manager;
settings *          globalSettings;
string           InputDirectory, OutputDirectory, ClimateDirectory, ClimateFileName, baseOutputDir;
bool                verbosity = false;
pixiData *          thePixiData;

// NJH number of farms - could be replaced by Palm variable
int    numOfFarms;
string orgPath;

// FASSET definitions
void SetEcoStatus(MMSClass *            theMMS,
                  patchInterfaceClass * apatchInterfaceClass,
                  double                ReadyForHarvestOrLater,
                  double                IrrigationDemand,
                  float                 prod[4][2]);

#ifndef PALM
void GetEcoStatus(ecosystinterfaceClass * anecosystinterfaceClass,
                  float                   prod[4][2]);

void CheckTillage(MMSClass *              aMMS,
                  patchInterfaceClass *   thePatchInterface,
                  ecosystinterfaceClass * anecosystinterfaceClass);

void CheckSowing(MMSClass *              aMMS,
                 patchInterfaceClass *   thePatchInterface,
                 ecosystinterfaceClass * anecosystinterfaceClass);

void CheckAddManure(MMSClass *              aMMS,
                    patchInterfaceClass *   thePatchInterface,
                    ecosystinterfaceClass * anecosystinterfaceClass);

void CheckFertilisation(MMSClass *              aMMS,
                        patchInterfaceClass *   thePatchInterface,
                        ecosystinterfaceClass * anecosystinterfaceClass);

void CheckHarvest(MMSClass *              aMMS,
                  patchInterfaceClass *   thePatchInterface,
                  ecosystinterfaceClass * anecosystinterfaceClass);

void CheckGrazed(MMSClass *              aMMS,
                 patchInterfaceClass *   apatchInterfaceClass,
                 ecosystinterfaceClass * anecosystinterfaceClass);

// end FASSET definitions
#endif PALM

ecosystinterfaceClass * findEcoModel(int  ecoNr,
        linkList<ecosystinterfaceClass> * theEcosystems) {
    ecosystinterfaceClass * anecosystinterfaceClass = NULL;

    for (int e = 0; e < theEcosystems -> NumOfNodes(); e++) {
        if (theEcosystems -> ElementAtNumber(e) -> GetIndex() == ecoNr) {
            anecosystinterfaceClass = theEcosystems -> ElementAtNumber(e);

            break;
        }
    }

    if (!anecosystinterfaceClass) {
        cout << "extern \"C\" int main():ecosystem interface not found" << endl;

        exit(99);
    }

    return anecosystinterfaceClass;
}

extern "C"
int main() {

    // NJH call to this class constructor leads to input data being read
    // from system.dat (must be in current working directory
    // NJH read number of farms - could be replaced by Palm variable
    float prod[4][2];

    linkList<MMSClass> * theFarms;

    theFarms = new linkList<MMSClass>;

    // following two lines are only needed if simulating ecosystem here

#ifndef PALM
    linkList<ecosystinterfaceClass> * theEcosystems;

    theEcosystems = new linkList<ecosystinterfaceClass>;
#endif PALM

    fileAccess hd;
    string     orgPath = hd.getCurrentPath();
    commonData aBase;

    aBase.setCritical(true);
    aBase.readFile("system.dat");

    string PixiDataPath;

    aBase.FindItem("PixiDataPath", &PixiDataPath);

    thePixiData = new pixiData(PixiDataPath);

    int first, num;

    aBase.getSectionsNumbers("run", &first, &num);
    aBase.setCritical(false);

    int startFarm = first;    // first farm to simulate
    int stopFarm  = num;      // last farm to simulate

    if (aBase.FindItem("JumpStart", &startFarm)) {
        aBase.FindItem("JumpStop", &stopFarm);

        if ((startFarm >= 0) && (startFarm <= num)) {
            first = startFarm;
        } else {
            cout << "JumpStart less than zero or greater than number of possible runs" << endl;
            cout << "Program will terminate after keypress" << endl;

#ifndef __BCplusplus__
            char dum;

            cin >> dum;
#else
            getch();
#endif

            exit(99);
        }
    }

    numOfFarms = num;

    for (int farmNo = first; farmNo <= numOfFarms; farmNo++) {
        aBase.setCritical(false);

        bool found = aBase.FindSection("run", farmNo);

        aBase.setCritical(true);

        if (found) {
            if ((farmNo >= startFarm) && (farmNo <= stopFarm))    // if farm is to be simulated
            {
                cout << "starting on farm nr " << farmNo << endl;

                MMSClass * aMMS = new MMSClass(farmNo);

                theFarms -> InsertLast(aMMS);                     // add to linklist

                //
                ////initialise MMS, giving names of directories as string
                // PALM_Write(PL_OUT," creation of MMS ");
                //
                aMMS -> initialise_MMS(farmNo, orgPath);

                // PALM_Write(PL_OUT," ok ");
                // MMS needs to know the initial status of the ECO
                // assume no crops present

#ifndef PALM
                eventControl *    localEventControl = aMMS -> getEeventControl();
                linkList<field> * fieldsList        = localEventControl -> getCropRotation() -> GetFields();

                for (int i = 0; i < fieldsList -> NumOfNodes(); i++) {
                    cloneList<patch> * patchList = fieldsList -> ElementAtNumber(i) -> getPatchList();

                    for (int j = 0; j < patchList -> NumOfNodes(); j++) {
                        patch *                 apatch                  = patchList -> ElementAtNumber(j);
                        ecosystinterfaceClass * anecosystinterfaceClass =
                            new ecosystinterfaceClass(apatch -> getEcoModel(), apatch -> getSoilFileName(),
                                                      apatch -> GetArea());

                        anecosystinterfaceClass -> OutputPlantDetails(false);    // comment out if no plant details needed
                        anecosystinterfaceClass -> UpdateStatus();
                        theEcosystems -> InsertLast(anecosystinterfaceClass);

                        // get product details from ecosystem
                        GetEcoStatus(anecosystinterfaceClass, prod);

                        // send to MMS
                        SetEcoStatus(aMMS, patchList -> ElementAtNumber(j) -> getPatchInterface(), 0.0, 0.0, prod);
                    }
                }

                // end of section that only works for one farm
#endif PALM

            }
        }
    }

    //
    // for(e=0;e<nb_eco;e++){
    // SetEcoStatus(theMMS,e, 1.0,0.0, prod);
    // }
    //
    int day = 0;

    numOfFarms = stopFarm - startFarm + 1;

#ifndef PALM
    bool finished = false;

    // bsTime * ShouldStopTime = new bsTime(1, 8, mySystemData->getStartYear());
    while (!finished)    // MMSClass reads the start and end year from system.dat
    {
        day++;
#else
    for (day = 0; day < 365; day++) {
#endif

        // PALM_Write(PL_OUT,"#################################");
        // PALM_Write(PL_OUT,"              Day: %d",day);
        // PALM_Write(PL_OUT,"#################################");
        //
        // PALM_Write(PL_OUT,"********  IN   **************");
        //
        // sprintf(cl_space,"LinkerToFarmS");
        // sprintf(cl_object,"LinkerToFarm");
        // /* allocation in memory*/
        ////                size=PALM_Space_get_size(cl_space);
        ////                buffer=malloc(size);
        //
        //
        // /*c getting input fromLinker*/
        // il_tag=PL_NO_TAG;
        // il_time=PL_NO_TIME;
        //
        // /*                il_err=PALM_Get(cl_space,cl_object, &il_time,&il_tag, buffer);*/
        // /*Unpacking swc  in LinkerToFarm*/
        //
        // for(e=0;e<nb_eco;e++){
        // il_pos=e;
        // PALM_Write(PL_OUT,"eco pixel: %d\n",il_pos);
        // il_err=PALM_Unpack(buffer,cl_space,cl_item_swc,&il_pos,swc);
        // /********************
        // soil water content
        // ********************/
        // PALM_Write(PL_OUT,"soil water content %f, %f, %f, %f, %f, %f \n",swc[0],swc[1],swc[2],swc[3],swc[4],swc[5]);
        //
        // /*c Unpacking prod in LinkerToFarm*/
        ////                        il_err=PALM_Unpack(buffer,cl_space,cl_item_prod,&il_pos,prod);
        //
        // /************************
        // prod see FARM_IO
        // ************************/
        // for(i=0;i<2;i++){
        // PALM_Write(PL_OUT,"production : type: %f, amount: %f, %%n: %f, %%c %f",prod[0][i],prod[1][i],prod[2][i],prod[3][i]);
        // }
        //
        //// tells farm model the status of each ECO. Two dummy parameters currently used for ReadyForHarvestOrLater and IrrigationDemand
        //// SetEcoStatus is defined in this file because preprocessing of information is required before calling MMSClass function
        // delete if using ecosystem interface

        /*
         * prod[0][0]=0;
         * prod[0][1]=1;
         * prod[1][0]=prod[1][1]=10;
         * prod[2][0]=prod[2][1]=prod[3][0]=prod[3][1]=20;
         */
        for (int count = 0; count < theFarms -> NumOfNodes(); count++) {
            MMSClass * aMMS = theFarms -> ElementAtNumber(count);

            //
            ////The manure management and livestock module can now perform its daily function
            //
            cout << "Farm no " << aMMS -> GetfarmNo() << " day " << day << " date " << theTime << endl;

            if ((theTime.GetDay() == 29) && (theTime.GetMonth() == 4)) {
                cout << "here";
            }

            aMMS -> DoMMSDaily();

#ifndef PALM
            if (day == 366) {
                cout << "d" << endl;
            }

            finished = aMMS -> GetfinishedRun();
#endif notPALM

#ifndef PALM
            eventControl *    localEventControl = aMMS -> getEeventControl();
            linkList<field> * fieldsList        = localEventControl -> getCropRotation() -> GetFields();

            for (int i = 0; i < fieldsList -> NumOfNodes(); i++) {
                cloneList<patch> * patchList = fieldsList -> ElementAtNumber(i) -> getPatchList();

                for (int j = 0; j < patchList -> NumOfNodes(); j++) {
                    int                     ecoNum                  = patchList -> ElementAtNumber(j) -> getEcoModel();
                    ecosystinterfaceClass * anecosystinterfaceClass = findEcoModel(ecoNum, theEcosystems);

                    anecosystinterfaceClass -> SetECOGlobals();
                    CheckTillage(aMMS, patchList -> ElementAtNumber(j) -> getPatchInterface(), anecosystinterfaceClass);
                    CheckSowing(aMMS, patchList -> ElementAtNumber(j) -> getPatchInterface(), anecosystinterfaceClass);
                    CheckAddManure(aMMS, patchList -> ElementAtNumber(j) -> getPatchInterface(),
                                   anecosystinterfaceClass);
                    CheckFertilisation(aMMS, patchList -> ElementAtNumber(j) -> getPatchInterface(),
                                       anecosystinterfaceClass);
                    CheckHarvest(aMMS, patchList -> ElementAtNumber(j) -> getPatchInterface(), anecosystinterfaceClass);
                    CheckGrazed(aMMS, patchList -> ElementAtNumber(j) -> getPatchInterface(), anecosystinterfaceClass);
                    anecosystinterfaceClass -> DoDailyEco();
                    anecosystinterfaceClass -> OutputPlantDetails(false);

                    // get product details from ecosystem
                    GetEcoStatus(anecosystinterfaceClass, prod);

                    /*
                     *          for (int houseNo=0; houseNo<aMMS->GetNumberOfHouses();houseNo++)
                     * {
                     * cout <<"House " << houseNo << " house NH3 " << aMMS->GetMMSHouseAmmonia() << " house N2O " << aMMS->GetMMSHouseN2O()
                     * << " house NO " << aMMS->GetMMSHouseNO() << endl;
                     * }
                     * for (int storeNo=0; storeNo<aMMS->GetNumberOfStores();storeNo++)
                     * {
                     * cout << "Store " << storeNo << " store NH3 " << aMMS->GetMMSStoreN2O(storeNo) << " store N2O " << aMMS->GetMMSStoreN2O(storeNo)
                     * << " store NO " << aMMS->GetMMSStoreNO(storeNo) << endl;
                     * }
                     */
#endif PALM

                    // send to MMS
                    SetEcoStatus(aMMS, patchList -> ElementAtNumber(j) -> getPatchInterface(), 0.0, 0.0, prod);
                    ecoNum++;
                }
            }

            // end of section that only works for one farm
        }
    }

    // tidy up memory

#ifndef PALM
    for (int count = 0; count < theFarms -> NumOfNodes(); count++) {
        MMSClass *        aMMS              = theFarms -> ElementAtNumber(count);
        eventControl *    localEventControl = aMMS -> getEeventControl();
        linkList<field> * localFieldList    = localEventControl -> getCropRotation() -> GetFields();

        for (int i = 0; i < localFieldList -> NumOfNodes(); i++) {
            cloneList<patch> * patchList = localFieldList -> ElementAtNumber(i) -> getPatchList();

            for (int j = 0; j < patchList -> NumOfNodes(); j++) {
                int                     ecoNum                  = patchList -> ElementAtNumber(j) -> getEcoModel();
                ecosystinterfaceClass * anecosystinterfaceClass = findEcoModel(ecoNum, theEcosystems);

                anecosystinterfaceClass -> ClosePlantDetails();
            }
        }
    }

    delete theEcosystems;
#endif PALM

    delete theFarms;

    cout << "\n succesfully finish" << endl;

    return 0;
}

void SetEcoStatus(MMSClass *            theMMS,
                  patchInterfaceClass * apatchInterfaceClass,
                  double                ReadyForHarvestOrLater,
                  double                IrrigationDemand,
                  float                 prod[4][2]) {
    linkList<harvInfoClass> * theHarvestProducts = new linkList<harvInfoClass>;

    for (int i = 0; i < 2; i++) {
        harvInfoClass * aharvInfoClass = new harvInfoClass();

        aharvInfoClass -> SetproductCode(int(prod[0][i]));
        aharvInfoClass -> SetamountDM(prod[1][i]);    // kg per ha
        aharvInfoClass -> SetamountN(prod[1][i] * prod[2][i] / 100.0);
        aharvInfoClass -> SetamountC(prod[1][i] * prod[3][i] / 100.0);
        theHarvestProducts -> InsertLast(aharvInfoClass);
    }

    apatchInterfaceClass -> SetEcoStatus(ReadyForHarvestOrLater, IrrigationDemand, theHarvestProducts);
}

#ifndef PALM

// Get status variables from ecosystem
void GetEcoStatus(ecosystinterfaceClass * anecosystinterfaceClass,
                  float                   prod[4][2]) {
    linkList<harvInfoClass> * theHarvestProducts = anecosystinterfaceClass -> GetharvestStuff();

    for (int harv = 0; harv < 2; harv++) {
        harvInfoClass * aharvInfoClass = theHarvestProducts -> ElementAtNumber(harv);

        prod[0][harv] = aharvInfoClass -> GetproductCode();
        prod[1][harv] = aharvInfoClass -> GetamountDM() * 10.0;    // convert from grams per metre square to kg per ha

        if (prod[1][harv] > 0) {
            prod[2][harv] = 100.0 * aharvInfoClass -> GetamountN() / aharvInfoClass -> GetamountDM();
            prod[3][harv] = 100.0 * aharvInfoClass -> GetamountC() / aharvInfoClass -> GetamountDM();
        } else {
            prod[2][harv] = 0.0;
            prod[3][harv] = 0.0;
        }
    }
}

void CheckTillage(MMSClass *              aMMS,
                  patchInterfaceClass *   apatchInterfaceClass,
                  ecosystinterfaceClass * anecosystinterfaceClass) {
    double theType, theDepth, theFractionIncorporated;

    theType                 = apatchInterfaceClass -> GettheType() + 1.0;
    theDepth                = apatchInterfaceClass -> GettheDepth();
    theFractionIncorporated = apatchInterfaceClass -> GettheFractionIncorporated();

    apatchInterfaceClass -> SettheType(-1);

    if (theType != 0) {
        anecosystinterfaceClass -> Tillage((theType - 1.0), theDepth, theFractionIncorporated);
    }
}

void CheckSowing(MMSClass *              aMMS,
                 patchInterfaceClass *   apatchInterfaceClass,
                 ecosystinterfaceClass * anecosystinterfaceClass) {
    double thecropID, amountSeedDM, seedN, seedC;

    thecropID    = (double) apatchInterfaceClass -> GetthecropID();
    amountSeedDM = apatchInterfaceClass -> GetamountSeedDM();
    seedN        = apatchInterfaceClass -> GetseedN();
    seedC        = apatchInterfaceClass -> GetseedC();

    apatchInterfaceClass -> SetamountSeedDM(0.0);

    if (amountSeedDM > 0.0) {
        anecosystinterfaceClass -> Sow(thecropID, amountSeedDM, seedN, seedC);
    }
}

void CheckAddManure(MMSClass *              aMMS,
                    patchInterfaceClass *   apatchInterfaceClass,
                    ecosystinterfaceClass * anecosystinterfaceClass) {
    double theManureType      = 0,
           theManureamount    = 0,
           theManureDepth     = 0,
           theManureTotalN    = 0,
           theManureNH4N      = 0,
           theManureNO3N      = 0,
           theManurepercentDM = 0,
           theManureC         = 0;

    if (apatchInterfaceClass -> GetdecompAmount() > 0.0) {
        theManureamount    = apatchInterfaceClass -> GetdecompAmount();
        theManureDepth     = 0.0;    // apatchInterfaceClass->GettheDepth();
        theManureNH4N      = apatchInterfaceClass -> GetdecompNH4_N();
        theManureNO3N      = apatchInterfaceClass -> GetdecompNO3_N();
        theManureTotalN    = theManureNH4N + theManureNO3N + apatchInterfaceClass -> GetdecompOrg_N();
        theManureC         = apatchInterfaceClass -> GetdecompC();
        theManurepercentDM = apatchInterfaceClass -> GetdecompPercentDM();
        theManureType      = apatchInterfaceClass -> GetdecompType();

        apatchInterfaceClass -> SetdecompAmount(0.0);
    }

    if (theManureamount > 0.0) {
        anecosystinterfaceClass -> AddOrganic(theManureType, theManureamount, theManureDepth, theManureTotalN,
                theManureNH4N, theManureNO3N, theManurepercentDM, theManureC);
    }
}

void CheckFertilisation(MMSClass *              aMMS,
                        patchInterfaceClass *   apatchInterfaceClass,
                        ecosystinterfaceClass * anecosystinterfaceClass) {
    double theFertAmmonium, theFertNitrate;

    theFertAmmonium = apatchInterfaceClass -> GettheNitrate();
    theFertNitrate  = apatchInterfaceClass -> GettheAmmonium();

    if ((theFertAmmonium > 0.0) || (theFertNitrate > 0.0)) {
        anecosystinterfaceClass -> SetFertilisation(theFertAmmonium, theFertNitrate);
    }
}

void CheckHarvest(MMSClass *              aMMS,
                  patchInterfaceClass *   apatchInterfaceClass,
                  ecosystinterfaceClass * anecosystinterfaceClass) {
    double harvestCode, propGrainHarvested, propStrawHarvested;

    harvestCode        = (double) apatchInterfaceClass -> GetharvestCode();
    propGrainHarvested = apatchInterfaceClass -> GetpropGrainHarvested();
    propStrawHarvested = apatchInterfaceClass -> GetpropStrawHarvested();

    apatchInterfaceClass -> SetharvestCode(0);

    if (harvestCode > 0) {
        anecosystinterfaceClass -> HarvestOperations(harvestCode, propGrainHarvested, propStrawHarvested);
    }
}

void CheckGrazed(MMSClass *              aMMS,
                 patchInterfaceClass *   apatchInterfaceClass,
                 ecosystinterfaceClass * anecosystinterfaceClass) {
    double grazedDM = (double) apatchInterfaceClass -> GetdryMatterGrazed();

    if (grazedDM > 0.0) {
        anecosystinterfaceClass -> Grazing(grazedDM);
    }
}
#endif PALM

#endif

#endif

