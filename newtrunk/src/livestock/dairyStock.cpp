
#include "../base/common.h"
#include "dairyStock.h"
#include "../base/IndicatorOutput.h"
#include "../building/buildings.h"
#include "../base/nixcbits.h"
#include "../base/settings.h"

dairyStock::dairyStock(const dairyStock & a) {
    maxNumPerCohort = 0;
    grouplist       = new linkList<cattleGroup>;

    for (int i = 0; i < a.grouplist -> NumOfNodes(); i++) {
        cattleGroup * p = new cattleGroup(*(a.grouplist -> ElementAtNumber(i)));

        grouplist -> InsertLast(p);
    }

    pointyCalves = new linkList<cattle>;

    for (int i = 0; i < a.pointyCalves -> NumOfNodes(); i++) {
        cattle * p = new cattle(*(a.pointyCalves -> ElementAtNumber(i)));

        pointyCalves -> InsertLast(p);
    }

    oldies = new linkList<cattle>;

    for (int i = 0; i < a.oldies -> NumOfNodes(); i++) {
        cattle * p = new cattle(*(a.oldies -> ElementAtNumber(i)));

        oldies -> InsertLast(p);
    }

    theStock = new linkList<animal>;

    for (int i = 0; i < theStock -> NumOfNodes(); i++) {
        animal * p = new animal(*(theStock -> ElementAtNumber(i)));

        theStock -> InsertLast(p);
    }

    IDnumber = a.IDnumber;
}

dairyStock::dairyStock(string       aName,
                       const int    aIndex,
                       const base * aOwner):
    animalStock(aName,
                aIndex,
                aOwner)
{
    maxNumPerCohort = 0;
    grouplist       = new linkList<cattleGroup>;
    pointyCalves    = new linkList<cattle>;
    oldies          = new linkList<cattle>;

    delete theStock;

    theStock = nullptr;
    IDnumber = 0;
}

dairyStock::~dairyStock() {
    if (pointyCalves -> NumOfNodes()) {
        theMessage -> FatalError("dairyStock::pointy calves should be empty.");
    }

    delete pointyCalves;    // should be empty

    // call destructors for all groups and animals
    delete grouplist;

    if (oldies -> NumOfNodes()) {
        theMessage -> FatalError("dairyStock::oldies should be empty.");
    }

    delete oldies;
}

/*
 * ReceivePlan read the plan for all animals
 */
void dairyStock::ReceivePlan(string fileExtension)

// Calculates number of animals to be imported + herd structure
{
    string     fileName = "cattle." + fileExtension;
    commonData data;

    data.setCritical(false);

    if (!data.readFile(fileName)) {
        data.setCritical(true);
        data.readFile("cattle.fnn");
    }

    // Get number of animals produced per year
    double numAnimals;
    double numberPerGroup = 0;

    // Note that this next part breaks convention that a class should read its own parameters
    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        cattleGroup * agroup = grouplist -> ElementAtNumber(i);
        string        name   = agroup -> GetAlias();

        numAnimals = 0;

        data.FindItem(name, &numAnimals);

        if (numAnimals > 0.0) {
            int numCohorts = 0;

            if (numAnimals <= maxNumPerCohort) {
                numCohorts = int(numAnimals);
            } else {
                numCohorts = maxNumPerCohort;
            }

            agroup -> SetnumberOfCohorts(numCohorts);

            numberPerGroup = numAnimals / numCohorts;

            agroup -> SetnumberPerCohort(numberPerGroup);

            cout << "dairyStock::ReceivePlan - number per group " << i << " " << numberPerGroup << endl;
        } else {
            agroup -> SetnumberOfCohorts(0);
            agroup -> SetnumberPerCohort(0);
        }
    }

    if (GetTotalAnimals() == 0.0)                        // this is initial call - no animals present
    {
        SeedGroups();
    } else {
        for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
            cattleGroup * agroup = grouplist -> ElementAtNumber(i);
            string        name   = agroup -> GetAlias();

            data.FindItem(name, &numAnimals);

            int numberToCull;

            if (agroup -> GetnumberInstances()
                    < agroup -> GetnumberOfCohorts())    // if too many animals then remove excess immediately
            {
                numberToCull = agroup -> GetnumberInstances() - agroup -> GetnumberOfCohorts();

                agroup -> CullAnimals(numberToCull);
            }

            // agroup->SetNumber(numAnimals);
        }
    }
}

/*
 *      DailyUpdate is the daily execution of cattle
 */
void dairyStock::DailyUpdate(void) {
    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        cattleGroup * agroup = grouplist -> ElementAtNumber(i);

        agroup -> DailyUpdate();
    }

    CheckBirth();
    GiveIndicator(environmentalIndicator);
}

// NJH Mar 2000

/*
 * Initializes a new dairyStock
 */
void dairyStock::InitCattle(commonData *& file,
                            int           anAnimalModel) {
    file -> FindSection("DairyStock");
    file -> FindItem("MaxNumPerCohort", &maxNumPerCohort);
	if(maxNumPerCohort==0)
		theMessage->FatalError("dairyStock::InitCattle MaxNumPerCohort in livestock.dat should not be 0");
    if (file -> FindSection(GetName())) {
        string namestr;

        namestr = (string) GetName() + "." + "Group";

        int first, num;

        file -> getSectionsNumbers(namestr, &first, &num);    // count the number of sections associated with this instance
        file -> setCritical(true);

        for (int inx = first; inx <= (num); inx++) {
            cattleGroup * acattlegroup = new cattleGroup(namestr, inx, this);

            acattlegroup -> SetanimalModel(anAnimalModel);
            acattlegroup -> ReadParameters(file);
            grouplist -> InsertLast(acattlegroup);
        }
    }
}

/*
 * Check to see if any calves are to be born
 */
void dairyStock::CheckBirth() {
    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        grouplist -> ElementAtNumber(i) -> CheckBirth();
    }
}

;

/*
 * Check to see if animals have out-aged their age class and move them if necessary
 * Cast animals (those moving out of the oldest age class) are left in oldies linkList
 */
void dairyStock::CheckAge() {
    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        cattleGroup * agroup = grouplist -> ElementAtNumber(i);

        if (agroup -> GetnumberOfAnimals() > 0) {
            agroup -> CheckAge(oldies);
        }

        // this code enables animals to migrate upwards in age categories. Not implemented yet
        // pass information over to the next age category

        if (oldies -> NumOfNodes() > 0) {
            Cull(oldies);
        }

        oldies -> ForgetAll();
    }
}

/*
 * Creates a set of Cattle in each cattle group stored in grouplist
 */
void dairyStock::SeedGroups() {
    double startAge = 0.0;
    double endAge   = 0.0;

    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        startAge = grouplist -> ElementAtNumber(i) -> GetminAge();
        endAge   = grouplist -> ElementAtNumber(i) -> GetmaxAge();

        if (grouplist -> ElementAtNumber(i) -> GetnumberOfCohorts() > 0) {
            grouplist -> ElementAtNumber(i) -> SeedGroup(startAge, endAge, &IDnumber);
        }
    }
}

/*
 * Sell cast animals, delete instances and record sale
 */
void dairyStock::Cull(linkList<cattle> * victims) {
    linkList<cattle>::PS aNode;
    victims -> PeekHead(aNode);

    while (aNode) {
        cattle * tempcow   = (cattle *) aNode -> element;
        string   outstring = aNode -> element -> GetAlias();

        if (globalSettings -> DetailsData.getHerdDetails()) {
            if (!tempcow -> GetisYoung()) {
                outstring += " - animal sold by dairystock";
            } else {
                outstring += " - calf sold by dairystock";
            }

            theMessage -> LogEventWithTime(outstring);
        }

        animalProduct * anAnimalProduct =
            new animalProduct(*(animalProduct *) theProducts -> GetProductElement("DAIRY"));

        tempcow -> loadAnimalProduct(anAnimalProduct);
        theProducts -> AddProduct(anAnimalProduct);

        delete anAnimalProduct;

        if (tempcow -> GetCalf()) {
            linkList<cattle> * tempList = new linkList<cattle>;

            tempList -> InsertLast(tempcow -> GetCalf());
            Cull(tempList);
            tempcow -> SetCalf(nullptr);

            delete tempList;
        }

        victims -> Remove(aNode);
        victims -> PeekHead(aNode);
    }

    victims -> ForgetAll();
}

/*
 * Return total number of animals
 */
double dairyStock::GetTotalAnimals() {
    double ret_val = 0.0;

    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        cattleGroup * aGroup = grouplist -> ElementAtNumber(i);

        ret_val += aGroup -> GetnumberOfAnimals();
    }

    return ret_val;
}

;

/*
 * Recruit new animals if necessary - currently only works for calves
 */
void dairyStock::AutoRecruit() {
    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        if (i == 0) {
            grouplist -> ElementAtNumber(0) -> AutoRecruit(pointyCalves, &IDnumber);    // ,CattlefileStream);
        } else {
            grouplist -> ElementAtNumber(i) -> AutoRecruit(nullptr, &IDnumber);         // ,CattlefileStream);
        }
    }

    if (pointyCalves -> NumOfNodes() > 0) {
        Cull(pointyCalves);
    }
}

/*
 * Give extra information in indicat.xml
 */
void dairyStock::GiveIndicator(int indicatorType) {
    if (indicatorType == environmentalIndicator) {
        double val = GetHerdN();

        theOutput -> AddStateIndicator(3360, "33.60 N bound in cattle", "kg N", val);

        val = GetHerdC();

        theOutput -> AddStateIndicator(4360, "43.60 C bound in cattle", "kg C", val);

        for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
            grouplist -> ElementAtNumber(i) -> GiveIndicator(indicatorType);
        }
    }

    if (indicatorType == economicIndicator) {
        for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
            grouplist -> ElementAtNumber(i) -> GiveIndicator(indicatorType);
        }

        // include these groups?

        /*
         *    for(int i=0;i<pointyCalves->NumOfNodes();i++)
         * {
         * val+=pointyCalves->ElementAtNumber(i)->GetDE();
         * }
         * for(int i=0;i<oldies->NumOfNodes();i++)
         * {
         * val+=oldies->ElementAtNumber(i)->GetDE();
         * }
         */
    }
}

;

/*
 *  Check to see if any calves are to be weaned and put them into pointyCalves
 */
void dairyStock::CheckWean(bool stopLactation) {
    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        grouplist -> ElementAtNumber(i) -> CheckWean(pointyCalves, stopLactation);
    }
}

;

/*
 * Calculate the Nitrogen amount in group, pointy and oldies
 */
double dairyStock::GetHerdN() {
    double val = 0.0;

    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        val += grouplist -> ElementAtNumber(i) -> GetCattleGroupN();
    }

    for (int i = 0; i < pointyCalves -> NumOfNodes(); i++) {
        val += pointyCalves -> ElementAtNumber(i) -> GetCattleN();
    }

    for (int i = 0; i < oldies -> NumOfNodes(); i++) {
        val += oldies -> ElementAtNumber(i) -> GetCattleN();
    }

    return val;
}

/*
 * Calculate the Carbon amount in group, pointy and oldies
 */
double dairyStock::GetHerdC() {
    double val = 0.0;

    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        val += grouplist -> ElementAtNumber(i) -> GetCattleGroupC();
    }

    for (int i = 0; i < pointyCalves -> NumOfNodes(); i++) {
        val += pointyCalves -> ElementAtNumber(i) -> GetCattleC();
    }

    for (int i = 0; i < oldies -> NumOfNodes(); i++) {
        val += oldies -> ElementAtNumber(i) -> GetCattleC();
    }

    return val;
}

/*
 * Calculate the DE amount in group, pointy and oldies
 */
double dairyStock::GetDE() {
    double val = 0.0;

    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        val += grouplist -> ElementAtNumber(i) -> GetDE();
    }

    for (int i = 0; i < pointyCalves -> NumOfNodes(); i++) {
        val += pointyCalves -> ElementAtNumber(i) -> GetDE();
    }

    for (int i = 0; i < oldies -> NumOfNodes(); i++) {
        val += oldies -> ElementAtNumber(i) -> GetDE();
    }

    return val;
}

/*
 * Sells Old animal and buy new
 */
void dairyStock::theSorting(bool canCull) {

    // GiveIndicator(environmentalIndicator);
    if (canCull) {
        CheckAge();
        Cull(oldies);
    }

    AutoRecruit();
}

void dairyStock::dairyStockcheckBalances(bool show) {
    for (int i = 0; i < grouplist -> NumOfNodes(); i++) {
        grouplist -> ElementAtNumber(i) -> checkBalances(show);
    }

    for (int i = 0; i < pointyCalves -> NumOfNodes(); i++) {
        pointyCalves -> ElementAtNumber(i) -> checkBalances(show);
    }

    for (int i = 0; i < oldies -> NumOfNodes(); i++) {
        oldies -> ElementAtNumber(i) -> checkBalances(show);
    }
}

void dairyStock::InformBuildings() {}