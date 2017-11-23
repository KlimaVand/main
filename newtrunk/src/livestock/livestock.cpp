
#include "../base/common.h"
#include "livestock.h"
#include "swineStock.h"
#include "dairyStock.h"
#include "../base/IndicatorOutput.h"

livestock::livestock():
    base()
{
    theStockList = new linkList<animalStock>;
    Name         = "livestock";
}

livestock::~livestock() {
    delete theStockList;
}

/*
 * Initialize a live stock
 *  @param filename is the filename where is variable is stored
 *  @param pigfarm symbolize if SwineStock section should be read or not
 *  @param dairyfarm symbolize if DairyStock section should be read or not
 *  @param beeffarm symbolize if BeefStock section should be read or not
 *  @param beeffarm symbolize if BeefStock section should be read or not
 *  @param theBuildings is where the SwineStock is stored
 *  @param filenameCattle filename where there is extra cattle information
 */
void livestock::Initialize(string      filename,
                           bool        pigfarm,
                           bool        dairyfarm,
                           bool        beeffarm,
                           buildings * theBuildings) {
    int          animalModel;
    commonData * data = new commonData();

    data -> setCritical(true);
    data -> readFile(filename);
    data -> FindItem("AnimalModel", &animalModel);
	 
    if (pigfarm) {
        int first, num;

        data -> setCritical(false);

        string InstanceName = "SwineStock";

        data -> getSectionsNumbers(InstanceName, &first, &num);

        for (int inx = first; inx <= (num); inx++) {
            data -> setCritical(true);
            data -> FindSection(InstanceName, inx);

            swineStock * aSwineStock = new swineStock("SwineStock", 0, this);

            aSwineStock -> InitSwines(data, theBuildings);
            theStockList -> InsertLast((animalStock *) aSwineStock);
        }

        data -> setCritical(false);

        /*
         * if (FindSection("SwineStock"))
         * {
         * swineStock * aSwineStock = new swineStock("SwineStock",0,this);
         * aSwineStock->InitSwines(file,theBuildings);
         * theStockList->InsertLast((animalStock*) aSwineStock);
         * }
         */
    }

    if (dairyfarm) {
        if (data -> FindSection("DairyStock")) {
            dairyStock * aDairyStock = new dairyStock("DairyStock", 0, this);

            aDairyStock -> InitCattle(data, animalModel);
            theStockList -> InsertLast((animalStock *) aDairyStock);
        } else {
            theMessage -> FatalError("livestock::Initialize - section DairyStock not found");
        }
    }

    if (beeffarm) {
        if (data -> FindSection("BeefStock")) {
            dairyStock * aDairyStock = new dairyStock("BeefStock", 0, this);

            aDairyStock -> InitCattle(data, animalModel);
            theStockList -> InsertLast((animalStock *) aDairyStock);
        }
    }

    delete data;
}

void livestock::InformBuildings() {
    for (int i = 0; i < theStockList -> NumOfNodes(); i++) {
        animalStock * aanimalStock = theStockList -> ElementAtNumber(i);

        aanimalStock -> InformBuildings();
    }
}

void livestock::checkBalances(bool show) {
    for (int i = 0; i < theStockList -> NumOfNodes(); i++) {
        (theStockList -> ElementAtNumber(i)) -> checkBalances(show);
    }
}

/*
 * update each animal stock in theStockList
 */
void livestock::ReceivePlan(string fileExtension) {
    cout << theStockList -> NumOfNodes() << endl;

    for (int i = 0; i < theStockList -> NumOfNodes(); i++) {
        theStockList -> ElementAtNumber(i) -> ReceivePlan(fileExtension);
    }
}

double livestock::GetDE() {
    double animalUnits = 0.0;

    for (int i = 0; i < theStockList -> NumOfNodes(); i++) {
        animalUnits += theStockList -> ElementAtNumber(i) -> GetDE();
    }

    return animalUnits;
}

void livestock::DailyUpdate() {
    theStockList -> ForEach(UpdateAnimals);
	for (int i=0; i < theCattleonField.size(); i++)
	{
		theCattleonField[i]->DailyUpdate();
	}
}

void livestock::GiveIndicator(int indicatorType) {
    for (int i = 0; i < theStockList -> NumOfNodes(); i++) {
        (theStockList -> ElementAtNumber(i)) -> GiveIndicator(indicatorType);
    }
}

void livestock::FlushIndicator(indicatorTypes indicatorType) {
    for (int i = 0; i < theStockList -> NumOfNodes(); i++) {
        (theStockList -> ElementAtNumber(i)) -> FlushIndicator(indicatorType);
    }
}
