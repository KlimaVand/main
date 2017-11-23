/*
 * Constructed by Nick Hutchings
 * Used to manage groups of cattle (includes management info + list of animals)
 */

#include "../base/common.h"
#include "cattleGroup.h"
#include "../base/nixcbits.h"
#include "../base/settings.h"

/*
 *  Constructor with arguments. Also call base(string aName, const int aIndex = -1, const base * aOwner = nullptr);
 */
cattleGroup::cattleGroup(string       aName,
                         const int    aIndex,
                         const base * aOwner):
    base(aName,
         aIndex,
         aOwner)
{
    data = new commonData();

    data -> setCritical(true);
    data -> readFile("animals.dat");

    // group= new linkList<cattle>;
    group          = new linkList<cattle>;
    feedPlanList   = new cloneList<diet>;
    theCurrentDiet = nullptr;    // holds current menu - needed when buying cattle

    // Alias;
    numberOfCohorts  = 0;
    numberPerCohort  = 0;
    maxAge           = 0;          // age at which animals move to the next group (in years)
    minAge           = 0;          // age at which animals move into the group (in years)
    code             = 0;
    LivestockUnits   = 0.0;
    areFemale        = false;
    areYoung         = false;      // true if animals are young (directs info flow in to indicators)
    theanimalSection = nullptr;    // holds pointer to relevant animal section in housing
    animalModel      = 0;
}

linkList<cattle> * cattleGroup::GetGroupClone() {
    linkList<cattle> * newlist;

    newlist = new linkList<cattle>;

    for (int i = 0; i < group -> NumOfNodes(); i++) {
        cattle * p = new cattle(*(group -> ElementAtNumber(i)));

        newlist -> InsertLast(p);
    }

    return newlist;
}

cloneList<diet> * cattleGroup::GetFeedPlanListClone() {
    cloneList<diet> * newlist;

    newlist = new cloneList<diet>;

    for (int i = 0; i < feedPlanList -> NumOfNodes(); i++) {
        diet * p = new diet(*(feedPlanList -> ElementAtNumber(i)));

        newlist -> InsertLast(p);
    }

    return newlist;
}

cattleGroup::cattleGroup(const cattleGroup & a) {
    group            = GetGroupClone();
    feedPlanList     = GetFeedPlanListClone();
    theCurrentDiet   = new diet(*a.theCurrentDiet);    // holds current menu - needed when buying cattle
    theanimalSection = new animalSection(*a.theanimalSection);

    // Alias;
    numberOfCohorts = a.numberOfCohorts;
    numberPerCohort = a.numberPerCohort;
    maxAge          = a.maxAge;      // age at which animals move to the next group (in years)
    minAge          = a.minAge;      // age at which animals move into the group (in years)
    code            = a.code;
    LivestockUnits  = a.LivestockUnits;
    areFemale       = a.areFemale;
    areYoung        = a.areYoung;    // true if animals are young (directs info flow in to indicators)

    // holds pointer to relevant animal section in housing
    animalModel = a.animalModel;
}

/*
 *  Destructor
 */
cattleGroup::~cattleGroup() {
    group -> Reset();

    delete feedPlanList;
    delete group;
    delete data;
}

/*
 *      DailyUpdate is called daily
 */
void cattleGroup::DailyUpdate(void) {
    for (int i = 0; i < group -> NumOfNodes(); i++) {
        cattle * tempcattle = group -> ElementAtNumber(i);

        tempcattle -> DailyUpdate();

        if (tempcattle -> GetCalf()) {
            tempcattle -> GetCalf() -> DailyUpdate();
        }

        // tempcattle->DoDailyIndicators();
        tempcattle -> checkBalances(false);
    }
}

/*
 * Check N budget for the animals
 */
void cattleGroup::checkBalances(bool show) {
    for (int i = 0; i < group -> NumOfNodes(); i++) {
        cattle * tempcattle = group -> ElementAtNumber(i);

        tempcattle -> checkBalances(show);
    }
}

/*
 * Read parameters for group
 */
void cattleGroup::ReadParameters(commonData *& file) {
    file -> setCritical(true);
    file -> FindSection(GetName(), Index);
    file -> FindItem("Alias", &Alias);
    file -> FindItem("Code", &code);
    file -> FindItem("LivestockUnits", &LivestockUnits);

    //
    {
        file -> FindItem("minAge", &minAge);
        file -> FindItem("maxAge", &maxAge);
    }

    int dum;

    file -> FindItem("Young", &dum);

    if (dum == 1) {
        areYoung = true;
    } else {
        areYoung = false;
    }

    file -> FindItem("Sex", &dum);

    if (dum == 1) {
        areFemale = true;
    } else {
        areFemale = false;
    }
}

/*
 * Check to see if any calves are to be born
 */
void cattleGroup::CheckBirth() {
    for (int i = 0; i < group -> NumOfNodes(); i++) {
        cattle * tempcattle = group -> ElementAtNumber(i);

        if (tempcattle -> GetisFemale()) {
            tempcattle -> CheckBirth();
        }
    }
}

/*
 * See if any animals have out-aged their group and if so, extract them
 */
void cattleGroup::CheckAge(linkList<cattle> * oldies) {
    linkList<cattle>::PS aNode;
    group -> PeekHead(aNode);

    while (aNode) {

        // if (((aNode->element->Getage()/theTime.daysInYear(0))>maxAge)&&(!aNode->element->isLactating()))
        if ((aNode -> element -> Getage() / theTime.daysInYear(0) > maxAge)) {
            oldies -> InsertLast(aNode -> element);    // animals in oldies are in order oldest .... youngest
            group -> Release(aNode);
            group -> PeekTail(aNode);
        } else {
            group -> OneStep(aNode);
        }
    }
}

;

/*
 *  Adds an Cattle to the group
 */
void cattleGroup::AddAnimal(cattle * anAnimal) {
    group -> InsertLast(anAnimal);
}

;

/*
 * Creates an number of animals spaced at even age increments
 */
void cattleGroup::SeedGroup(double startAge,
                            double endAge,
                            int *  IDnumber) {

    // open database file of cattle parameters
    if (numberOfCohorts > 0) {
        double   increment  = (endAge - startAge) * theTime.daysInYear(0) / numberOfCohorts;
        double   currentAge = ((int) (startAge * theTime.daysInYear(0))) + 1;
        cattle * tempcow;

        for (int i = 0; i < numberOfCohorts; i++) {
            tempcow = buyNewAnimal(currentAge, IDnumber, false);

            cout << tempcow -> GeturineN() << endl;

            group -> InsertLast(tempcow);

            currentAge += increment;
        }
    }
}

;

/*
 * Culls animals (oldest go first)
 */
void cattleGroup::CullAnimals(int numberToCull) {
    if (group -> NumOfNodes() < numberToCull) {
        theMessage -> FatalError("cattlegroup: more animals to cull than are currently present");
    }

    int i = numberToCull;

    // linkList <cattle>::PS aNode;
    linkList<cattle>::PS aNode;

    while (i > 0) {
        group -> PeekTail(aNode);

        cattle * tempcow = (cattle *) aNode -> element;

        CullAnimalFromGroup(tempcow);
        i--;
    }
}

/*
 * Culls specific a animal. If animal is in this group then returns true, otherwise false
 */
bool cattleGroup::CullAnimalFromGroup(cattle * theVictim) {
    linkList<cattle>::PS aNode;
    group -> PeekHead(aNode);

    int i = group -> NumOfNodes();

    while ((aNode) && (i > 0)) {
        if ((cattle *) aNode -> element == theVictim) {
            CullAnimal(theVictim, false);
            group -> Remove(aNode);

            return true;
        }

        i--;
    }

    return false;
}

/*
 * Recruits animals to group if numbers are lacking. If recruitment list is not nullptr
 * then it will attempt to recruit from this list, otherwise it will buy animals
 */
void cattleGroup::AutoRecruit(linkList<cattle> * recruitmentList,
                              int *              IDnumber) {

    // see if need to recruit animals
    // int numToGet=GetnumberInstances(); njh removed mar 07
    // njh added mar 07
    int numToGet = 0;

    if (GetnumberInstances() < numberOfCohorts) {
        numToGet = numberOfCohorts - GetnumberInstances();
    }

    // end of add
    while (numToGet > 0) {
        if (recruitmentList) {    // only normally present for first age group (contains calves)
            for (int i = 0; i < recruitmentList -> NumOfNodes(); i++) {
                linkList<cattle>::PS aNode;
                recruitmentList -> PeekHead(aNode);

                if ((aNode -> element -> Getage() > GetminAge()) && (aNode -> element -> Getage() <= GetmaxAge())) {
                    if (((GetareFemale()) && (aNode -> element -> GetisFemale()))
                            ||    // check if sex of candidate is ok for group
                            ((!GetareFemale()) && (!aNode -> element -> GetisFemale()))) {
                        aNode -> element -> SetisYoung(areYoung);

                        *IDnumber = *IDnumber + 1;

                        aNode -> element -> SetIndex(*IDnumber);

                        if (theanimalSection) {
                            aNode -> element -> SetanimalSection(theanimalSection);
                        } else {
                            theMessage -> FatalError("cattle::No current animal section for recruited animals");
                        }

                        AddAnimal(aNode -> element);                           // add animal to group

                        string outstring = GetAlias();

                        outstring += " - animal recruited by cattle group";

                        theMessage -> LogEventWithTime(outstring);
                        recruitmentList -> Release(aNode);                     // release node from recruitment list
                        numToGet--;
                    }
                }
            }
        }

        if (numToGet > 0)                                                      // cannot recruit enough - buy some
        {
            double   currentAge = minAge * 365 + 1;
            cattle * tempcow    = buyNewAnimal(currentAge, IDnumber, true);    // , CattlefileStream);

            if (theanimalSection) {
                tempcow -> SetanimalSection(theanimalSection);

                if (tempcow -> GetCalf()) {
                    tempcow -> GetCalf() -> SetanimalSection(theanimalSection);
                }
            } else {
                theMessage -> FatalError("cattlegroup::No current animal section for bought animals");
            }

            group -> InsertLast(tempcow);
            numToGet--;
        }
    }

    if (numberOfCohorts
            < GetnumberInstances())    // Too many animals - time for the oldest to move on to pastures new......
    {
        int theNoOfVictims = GetnumberInstances() - numberOfCohorts;

        CullAnimals(theNoOfVictims);
    }
}

;

/*
 * Returns the total N contained in the group
 */
double cattleGroup::GetCattleGroupN() {
    double ret_val = 0.0;

    for (int i = 0; i < group -> NumOfNodes(); i++) {
        cattle * tempcattle = group -> ElementAtNumber(i);

        ret_val += tempcattle -> GetCattleN();
    }

    return ret_val;
}

/*
 * Returns the total C contained in the group
 */
double cattleGroup::GetCattleGroupC() {
    double ret_val = 0.0;

    for (int i = 0; i < group -> NumOfNodes(); i++) {
        cattle * tempcattle = group -> ElementAtNumber(i);

        ret_val += tempcattle -> GetCattleC();
    }

    return ret_val;
}

/*
 * Checks to see if any calves should be weaned and if so, punts them into pointycalves
 * Note: Up to another function (currently in grazing manager) to set suckling to false
 * and therefore prompt weaning.
 */
void cattleGroup::CheckWean(linkList<cattle> * calves,
                            bool               stopLactation) {
    for (int i = 0; i < group -> NumOfNodes(); i++) {
        cattle * tempcattle = group -> ElementAtNumber(i);

        if (tempcattle -> GetisFemale()) {
            cattle * aCalf = tempcattle -> GetCalf();

            if (aCalf) {
                if (!aCalf -> GetisSuckling()) {
                    tempcattle -> WeanCalf();
                    aCalf -> Setmother(nullptr);
                    aCalf -> SetfeedPlanList(
                        nullptr);    // calf uses same feedplanlist as mother, so set to nullptr to avoid double delete
                    CullAnimal(aCalf, true);

                    if (stopLactation) {
                        tempcattle -> Setlactating(false);
                    }
                }
            }
        }
    }
}

/*
 * return the sum of all Cattle
 */
double cattleGroup::GetnumberOfAnimals() {
    double numberOfAnimals = 0.0;

    for (int i = 0; i < group -> NumOfNodes(); i++) {
        cattle * tempcattle = group -> ElementAtNumber(i);

        numberOfAnimals += tempcattle -> GetNumber();
    }

    return numberOfAnimals;
}

;

/*
 * return the sum of all Cattle
 */
double cattleGroup::GetDE() {
    double aVal = 0.0;

    for (int i = 0; i < group -> NumOfNodes(); i++) {
        cattle * tempcattle = group -> ElementAtNumber(i);

        aVal += tempcattle -> GetDE();
    }

    return aVal;
}

;

/*
 * if input is economicIndicator the indicat.xls will have values and number from the group
 */
void cattleGroup::GiveIndicator(int indicatorType) {

    // double Number = 0.0;
    if ((theTime.GetMonth() == 1) && (theTime.GetDay() == 1) && (theTime.GetYear() == 1988)) {
        for (int i = 0; i < group -> NumOfNodes(); i++) {

            // cattle * tempcattle = group->ElementAtNumber(i);

            /*
             *           if (indicatorType==environmentalIndicator)
             * tempcattle->GiveIndicator(indicatorType);
             */

            // Number += tempcattle->GetNumber();
        }
    }
}

/*
 * Create a new Cattle
 */
cattle * cattleGroup::buyNewAnimal(double currentAge,
                                   int *  IDnumber,
                                   bool   logit) {
    cattle * tempcow = new dk_cattle(GetAlias().c_str(), *IDnumber, this);

    *IDnumber = *IDnumber + 1;

    /*
     *    if (animalModel==1)
     * tempcow= new uk_cattle(GetName(),*IDnumber,this);
     */
    if (animalModel != 2) {
        theMessage -> FatalError("current animal Model is not supported at this point");
    }

    // tempcow= new dk_cattle(GetName(),*IDnumber,this);
    if (areFemale) {
        tempcow -> SetisFemale(true);
    } else {
        tempcow -> SetisFemale(false);
    }

    tempcow -> ReadParameters(data);

    // these two factors will be later reset by operations manager but we need to make an initial guess
    // if (tempcow->GetIndex()==29)
    // cout <<"";
    if (tempcow -> GetisFemale()) {
        if (currentAge < 2 * 365) {                                    // 1st year heifers + 2nd year heifers
            tempcow -> Setlactating(false);
            tempcow -> SetlactationNumber(0);

            if (currentAge
                    <= (2 * 365 - tempcow -> GetgestationPeriod()))    // 1st year heifers + non-pregnant 2nd year heifers
            {
                tempcow -> Setpregnant(false);
                tempcow -> SetdaysFromConception(0.0);
                tempcow -> SetdaysFromParturition(0.0);
            } else {                                          // 2nd year heifers, pregnant
                tempcow -> Setpregnant(true);

                // take one day off, otherwise there is a risk that time since conception = gestation period
                tempcow -> SetdaysFromConception(currentAge - ((2 * 365.0) - tempcow -> GetgestationPeriod()));

                if (tempcow -> GetdaysFromConception() == tempcow -> GetgestationPeriod()) {
                    tempcow -> SetdaysFromConception(tempcow -> GetgestationPeriod()
                                                     - 1);    // avoid immediate birth - causes problems
                }

                tempcow -> SetdaysFromParturition(0.0);
                tempcow -> SetlactationNumber(0);
            }
        } else if ((GetAlias() == "DAIRY") || (GetAlias() == "SUCKLER"))    // only FBEEF3 fall out from the if tree
        {
            int lactationNumber = int((currentAge / 365.0) - 1);

            tempcow -> SetlactationNumber(lactationNumber);
            tempcow -> SetCalf(nullptr);

            double partOfYear = (int) (currentAge) % 365;

            tempcow -> SetdaysFromParturition(partOfYear);

            double conceptionTime    = 0;                                   // days between parturition and conception
            double lengthOfLactation = 0;

            if (GetAlias() == "DAIRY") {
                conceptionTime    = 49;
                lengthOfLactation = 315;
            } else if (GetAlias() == "SUCKLER") {
                conceptionTime    = 98;
                lengthOfLactation = 225;
            } else {
                theMessage -> FatalError("cattleGroup::buyNewAnimal - ");
            }

            if (partOfYear <= conceptionTime)                               // assume conception after 7 weeks
            {
                tempcow -> Setpregnant(false);
                tempcow -> Setlactating(true);
                tempcow -> SetdaysFromConception(0.0);

                if (GetAlias() == "SUCKLER") {
                    if (animalModel == 2) {
                        cattle * tempcalf = new dk_cattle("CALF", 0, this);

                        tempcalf -> SetisFemale(true);
                        tempcow -> SetCalf(tempcalf);
                        tempcalf -> Setcode(2);
                        tempcalf -> ReadParameters(data);
                        tempcalf -> Initialise(code, int(partOfYear), LivestockUnits, numberPerCohort,
                                               true);                       // , CattlefileStream);
                        tempcalf -> SetAlias("CALF");
                    }
                }
            } else if (partOfYear >= lengthOfLactation)    // assume dry
            {
                tempcow -> Setpregnant(false);
                tempcow -> Setlactating(false);
                tempcow -> SetdaysFromConception(partOfYear - conceptionTime);
            } else {
                tempcow -> Setpregnant(true);
                tempcow -> Setlactating(true);
                tempcow -> SetdaysFromConception(partOfYear - conceptionTime);

                if (GetAlias() == "SUCKLER") {
                    if (animalModel == 2) {
                        cattle * tempcalf = new dk_cattle("CALF", 0, this);

                        tempcalf -> SetisFemale(true);
                        tempcow -> SetCalf(tempcalf);
                        tempcalf -> Setcode(2);
                        tempcalf -> ReadParameters(data);
                        tempcalf -> Initialise(code, int(partOfYear), LivestockUnits, numberPerCohort,
                                               true);      // , CattlefileStream);
                        tempcalf -> SetAlias("CALF");
                    }
                }
            }
        }
    }

    tempcow -> SetAlias(Alias);
    tempcow -> Initialise(code, int(currentAge), LivestockUnits, numberPerCohort, areYoung);    // , CattlefileStream);

    if (tempcow -> isPregnant() &&!tempcow -> GetFoetus()) {
        theMessage -> FatalError(
            "cattleGroup::buyNewAnimal - logic error as there is a pregnant animal without a foetus");
    }

    if (logit) {
        animalProduct * anAnimalProduct =
            new animalProduct(*(animalProduct *) theProducts -> GetProductElement(Alias));

        tempcow -> loadAnimalProduct(anAnimalProduct);
        theProducts -> SubtractProduct(anAnimalProduct);

        delete anAnimalProduct;

        if (globalSettings -> DetailsData.getHerdDetails()) {
            string outstring = GetAlias();

            outstring += " - animal bought by cattle group";

            theMessage -> LogEventWithTime(outstring);
        }
    }

    return tempcow;
}

/*
 * Cull an animal. Delete animal if deleteAnimal is true, otherwise it will be done elsewhere
 */
void cattleGroup::CullAnimal(cattle * theVictim,
                             bool     deleteAnimal) {
    theVictim -> checkBalances(false);

    animalProduct * anAnimalProduct;

    if (globalSettings -> DetailsData.getHerdDetails()) {
        string outstring = GetAlias();

        outstring += " - calf sold by cattle group";

        theMessage -> LogEventWithTime(outstring);
    }

    anAnimalProduct = new animalProduct(
        *(animalProduct *) theProducts -> GetProductElement(theVictim -> GetAlias()));

    theVictim -> loadAnimalProduct(anAnimalProduct);

    // anAnimalProduct->Setamount(anAnimalProduct->GetAmount()*anAnimalProduct->GetEndWeight());  //convert from number to mass in kg
    theProducts -> AddProduct(anAnimalProduct);

    delete anAnimalProduct;

    if (deleteAnimal) {
        delete theVictim;
    }
}
