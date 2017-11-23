/*
 * Created 8/99 by Nick Hutchings
 * Describes the floor of an animal house, mainly in relation to the accumulation of dung and urine
 * Differs from manurestore in that several manure types can be stored on a floor
 * (essential to represent systems where there is separation of liquid and solids)
 */

#include "../base/common.h"
#include "floorstore.h"
#include "dungheap.h"
#include "instore.h"
#include "../base/controlParameters.h"

#ifdef _STANDALONE

#include "../base/OutputControl.h"
#include "../base/message.h"

#endif

#include "../base/IndicatorOutput.h"
#include "../base/commonData.h"

/*
 * Default Constructor
 */
floorstore::floorstore():
    base()
{
    ManureToID        = 0;
    CO2Emissionfactor = N2Emissionfactor = N2OEmissionfactor = 0.0;
    CH4Emissionfactor = AreaPartition = proportionFouled = WashingWater = 0.0;
    cleaningInterval  = 0.0;
    StrawAdded        = 0.0;
    excretaPartition  = 1.0;
    ventilated        = false;
    theOnFloorStores  = new linkList<instore>;
    theNextStores     = new linkList<manurestore>;
    volumeToArea      = 0.0;
    NH3Emissionfactor = 0.0;
    timeUncleaned     = 0.0;
    area              = 0.0;
}

floorstore::~floorstore() {
    delete theOnFloorStores;
}

floorstore::floorstore(string        aName,
                       const int     aIndex,
                       const base *  aOwner,
                       commonData *& runData,
                       commonData *& original,
                       int           numberOfFloors):
    base(aName,
         aIndex,
         aOwner)
{
    ventilated    = false;
    area          = 0.0;
    AreaPartition = 1;
    timeUncleaned = 0;

    string  sectionName(aName);

    sectionName = sectionName + "(" + conv.IntToString(aIndex) + ")";

    runData -> setCritical(true);
    runData -> FindSection(Name, Index);
    runData -> FindItem("Alias", &Alias);
    runData -> setCritical(false);
    original -> setCritical(true);

    // string sectionNameOrg(aName);
    int    indexInOld;
    string place;

    original -> findSection(aName, 0, "Alias", Alias, &place, &indexInOld);
    original -> FindSection(place, indexInOld);

    if (numberOfFloors > 1) {
        original -> FindItem("AreaPartition", &AreaPartition);
        runData -> FindItem("AreaPartition", &AreaPartition);
        original -> FindItem("ExcretaPartition", &excretaPartition);
        runData -> FindItem("ExcretaPartition", &excretaPartition);
    } else {
        AreaPartition    = 1.0;
        excretaPartition = 1.0;
    }

    cout << sectionName << " " << Name << endl;

    original -> FindItem("ManureToID", &ManureToID);
    runData -> FindItem("ManureToID", &ManureToID);
    original -> FindItem("ManureToName", &manureToName);
    runData -> FindItem("ManureToName", &manureToName);
    original -> FindItem("CleaningInterval", &cleaningInterval);
    runData -> FindItem("CleaningInterval", &cleaningInterval);
    original -> FindItem("VolumeToArea", &volumeToArea);
    runData -> FindItem("VolumeToArea", &volumeToArea);
    original -> FindItem("Manuretype", &manuretype);
    runData -> FindItem("Manuretype", &manuretype);

    double velocityConst = 0.0;

    switch (theControlParameters -> GetvolatModel()) {
        case 0 :
            original -> FindItem("NH3Emissionfactor", &NH3Emissionfactor);
            runData -> FindItem("NH3Emissionfactor", &NH3Emissionfactor);
            original -> FindItem("N2Emissionfactor", &N2Emissionfactor);
            runData -> FindItem("N2Emissionfactor", &N2Emissionfactor);
            original -> FindItem("N2OEmissionfactor", &N2OEmissionfactor);
            runData -> FindItem("N2OEmissionfactor", &N2OEmissionfactor);
            original -> FindItem("CO2Emissionfactor", &CO2Emissionfactor);
            runData -> FindItem("CO2Emissionfactor", &CO2Emissionfactor);
            original -> FindItem("CH4Emissionfactor", &CH4Emissionfactor);
            runData -> FindItem("CH4Emissionfactor", &CH4Emissionfactor);

            break;

        case 1 :
            original -> FindItem("ProportionFouled", &proportionFouled);
            runData -> FindItem("ProportionFouled", &proportionFouled);
            original -> FindItem("VelocityConstant", &velocityConst);
            runData -> FindItem("VelocityConstant", &velocityConst);
    }

    double postCleanDepth = 0.0;

    original -> FindItem("PostCleanDepth", &postCleanDepth);
    runData -> FindItem("PostCleanDepth", &postCleanDepth);
    original -> FindItem("WashingWater", &WashingWater);
    runData -> FindItem("WashingWater", &WashingWater);
    original -> FindItem("StrawAdded", &StrawAdded);
    runData -> FindItem("StrawAdded", &StrawAdded);

    string inhouse      = sectionName;
    int    placeSection = (int) inhouse.find(".");

    inhouse.resize(placeSection);

    theOnFloorStores = new linkList<instore>;
    theNextStores    = new linkList<manurestore>;

    if (manuretype == "SLURRY") {

        // store information on where to send manure in the urine fraction
        // currently use urine to store information necessary to calculate ammonia volatilisation
        instore * urine = new instore(GetName(), Index, this, runData, original);

        urine -> SetvelocityConstant(velocityConst);
        theOnFloorStores -> InsertLast(urine);

        // string manureType;
        instore * dung = new instore(GetName(), Index, this, runData, original);

        theOnFloorStores -> InsertLast(dung);
    } else if (manuretype.find("FYM") != string::npos) {
        instore * farmyard = new instore(GetName(), Index, this, runData, original);

        theOnFloorStores -> InsertLast(farmyard);
    } else if (manuretype == "SEPARATED")    // need separate as liquid and solid stored separately
    {
        theMessage -> FatalError("FloorStore: attempt to use Separated manure system - is not supported");

        // store information on where to send liquid manure in the urine fraction
        instore * urine = new instore(GetName(), Index, this, runData, original);

        theOnFloorStores -> InsertLast(urine);

        // store information on where to send solid manure in the urine fraction
        string manureType;

        original -> FindItem("Manuretype", &manureType);
        runData -> FindItem("Manuretype", &manureType);

        instore * dung = new instore(GetName(), Index, this, manureType);

        theOnFloorStores -> InsertLast(dung);
    } else {
        theMessage -> FatalError("FloorStore: unknown Manure");
    }

    Nbudget.SetNames(sectionName, "N");
    Cbudget.SetNames(sectionName, "C");
}

floorstore::floorstore(floorstore & s):
    base(s)
{
    ManureToID       = 0;
    AreaPartition    = StrawAdded = proportionFouled = 0.0;
    WashingWater     = 0.0;
    ventilated       = false;
    theNextStores    = new linkList<manurestore>;
    theOnFloorStores = new linkList<instore>;

    for (int i = 0; i < s.theOnFloorStores -> NumOfNodes(); i++) {
        instore * p = new instore(*(s.theOnFloorStores -> ElementAtNumber(i)));

        theOnFloorStores -> InsertLast(p);
    }

    Nbudget           = s.Nbudget;
    Cbudget           = s.Cbudget;
    excretaPartition  = s.excretaPartition;
    area              = s.area;
    cleaningInterval  = s.cleaningInterval;
    timeUncleaned     = s.timeUncleaned;
    volumeToArea      = s.volumeToArea;
    NH3Emissionfactor = s.NH3Emissionfactor;
    manuretype        = s.manuretype;
    N2Emissionfactor  = s.N2Emissionfactor;
    N2OEmissionfactor = s.N2OEmissionfactor;
    CO2Emissionfactor = s.CO2Emissionfactor;
    CH4Emissionfactor = s.CH4Emissionfactor;
}

void floorstore::DailyUpdate() {
    timeUncleaned += 1.0;
}

/*
 * Volatize from the floorstore
 */
void floorstore::Volatize(double   airVelocity,
                          double   waterEvapThisFloor,
                          double   temperature,
                          double * volatAmmonia,
                          double * N2Emission,
                          double * N2OEmission,
                          double * CH4Emission,
                          double * CO2Emission) {

#ifdef STRICT_BALANCES
    EndBudget(false);
#endif

    int numSources = 0;

    if (manuretype == "SLURRY") {
        numSources = 1;
    }

    // else
    // theMessage->FatalError("Volatilisation module for separated manure not yet made");
    double totalWater = 0.0;

    for (int i = 0; i < numSources; i++) {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

        if (ainstore -> GetManure()) {
            totalWater += ainstore -> GetManure() -> Getwater();
        }
    }

    for (int i = 0; i < numSources; i++) {
        double tempvolatAmmonia = 0.0,
               tempWaterEvap    = 0.0,
               tempN2Emission   = 0.0,
               tempN2OEmission  = 0.0,
               tempCH4Emission  = 0.0,
               tempCO2Emission  = 0.0;

        // handle emission from different manures appropriately
        {
            instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

            if (ainstore -> GetManure()) {
                double areaSoiled = ainstore -> GetManure() -> GetAmount() * volumeToArea;

                tempWaterEvap = waterEvapThisFloor * ainstore -> GetManure() -> Getwater() / totalWater;

                if (areaSoiled > 0.0) {
                    if (areaSoiled > area * proportionFouled) {
                        areaSoiled = area * proportionFouled;
                    }

                    double duration = 24.0;    // duration in hours

                    ainstore -> floorVolatize(airVelocity, tempWaterEvap, temperature, areaSoiled, duration,
                                              &tempvolatAmmonia, &tempN2Emission, &tempN2OEmission, &tempCH4Emission,
                                              &tempCO2Emission);

                    *volatAmmonia += tempvolatAmmonia;
                    *N2Emission   += tempN2Emission;
                    *N2OEmission  += tempN2OEmission;
                    *CH4Emission  += tempCH4Emission;
                    *CO2Emission  += tempCO2Emission;
                }
            }
        }
    }

    Nbudget.AddOutput(*volatAmmonia + *N2Emission + *N2OEmission);
    Cbudget.AddOutput(*CH4Emission + *CO2Emission);

#ifdef STRICT_BALANCES
    EndBudget(false);
#endif

}

/*
 * Volatize ammonia from the floorstore - static model
 */
void floorstore::Volatize(double * volatAmmonia,
                          double * N2Emission,
                          double * N2OEmission,
                          double * CH4Emission,
                          double * CO2Emission) {
    double tempvolatAmmonia = 0.0,
           tempN2Emission   = 0.0,
           tempN2OEmission  = 0.0,
           tempCH4Emission  = 0.0,
           tempCO2Emission  = 0.0;

    for (int i = 0; i < theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

        ainstore -> Volatize(&tempvolatAmmonia, &tempN2Emission, &tempN2OEmission, &tempCH4Emission, &tempCO2Emission);

        *volatAmmonia += tempvolatAmmonia;
        *N2Emission   += tempN2Emission;
        *N2OEmission  += tempN2OEmission;
        *CH4Emission  += tempCH4Emission;
        *CO2Emission  += tempCO2Emission;
    }
}

/*
 * Receive manure from animals
 */
void floorstore::RcvManure(manure * aUrine,
                           manure * aFaeces) {
    manure * aManure = new manure();

    *aManure + *aUrine;
    *aManure + *aFaeces;
    Nbudget.AddInput(aManure -> GetAmount() * aManure -> GetAllN().n);
    Cbudget.AddInput(aManure -> GetAmount() * aManure -> GetC_content());

    if ((manuretype == "SLURRY") || (manuretype == "SEPARATED")) {
        if (manuretype == "SEPARATED") {
            theMessage -> FatalError("floorstore::RcvManure: SEPARATED not supported. Talk to nick");
        }

        instore * ainstore = theOnFloorStores -> ElementAtNumber(0);

        // add urine to urine store
        ainstore -> RcvManure(aUrine);

        // calculate area covered by urine
        double newArea = ainstore -> GetAmountManure() * volumeToArea;

        if (newArea > area) {
            ainstore -> SetsurfaceArea(area);
        } else {
            ainstore -> SetsurfaceArea(newArea);
        }

        ainstore = theOnFloorStores -> ElementAtNumber(1);

        // add dung to dung store (area = 0.0)
        ainstore -> RcvManure(aFaeces);
    } else {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(0);

        ainstore -> RcvManure(aManure);

        // if (!ainstore->GetnextStore())//must be solid manure stored on floor - record amount produced here
        // theOutput->AddIndicator(1973,"19.73 Solid manure produced","t",aManure->GetAmount());
    }

    delete aManure;
}

/*
 * Receive bedding
 */
void floorstore::RcvBedding(plantItem * aBeddingMaterial) {
    Nbudget.AddInput(aBeddingMaterial -> GetAmount() * aBeddingMaterial -> GetAllN().n);
    Cbudget.AddInput(aBeddingMaterial -> GetAmount() * aBeddingMaterial -> GetC_content());

    // save old name before changing to name of manure
    // cout<<"t t"<<manuretype<<endl;
    string oldName = aBeddingMaterial -> GetName();

    if (manuretype == "SLURRY") {

        // add straw to dung
        instore * ainstore = theOnFloorStores -> ElementAtNumber(1);

        ainstore -> ReceiveBedding(aBeddingMaterial);
    } else {
        aBeddingMaterial -> Setname(manuretype);    // change name and get info, otherwise manure is rejected by storage
        GetaStore(0) -> ReceiveBedding(aBeddingMaterial);
    }

    aBeddingMaterial -> Setname(oldName);    // change name back or funny things could happen to budgetting later!!
}

/*
 * Clean the floor of all manure - manure sent to the relevant store
 */
manure * floorstore::CleanFloor(double numberOfAnimals) {

    // EndBudget(false);
    // currently only slurry and FYM, so one store
    manurestore * ptrToNextStore = theNextStores -> ElementAtNumber(0);
    string        nextManureName = ptrToNextStore -> getManureName();

    // make a manure from water
    manure * water = new manure(nextManureName, 0, nullptr);    // is this deleted later??

    water -> SetmainUnitName(t);    // otherwise product the later addition to manure will get upset over the units
    water -> SetpH(ptrToNextStore -> GetManure() -> GetpH());    // water does not affect pH
    water -> Setamount(WashingWater * numberOfAnimals / 1000.0);

    manure * manureToAdd = new manure();

    *manureToAdd + *water;

    delete water;

    for (int i = 0; i < theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = theOnFloorStores -> ElementAtNumber(i);
        manure *  aManure  = ainstore -> GetManure();

        aManure = (manure *) ainstore -> GetFromManureStore(10000000.0, aManure, false);

        // add urine and faeces (if slurry)
        aManure -> SetmainUnitName(t);    // otherwise product the later addition to manure will get upset over the units
        aManure -> Setname(nextManureName);
        *manureToAdd + *aManure;

        // aManure->Setamount(0.0);
    }

    if (ptrToNextStore)    // there is a store to receive the manure
    {
        ptrToNextStore -> RcvManure(manureToAdd);

        timeUncleaned = 0;

        Nbudget.AddOutput(manureToAdd -> GetAmount() * manureToAdd -> GetAllN().n);
        Cbudget.AddOutput(manureToAdd -> GetAmount() * manureToAdd -> GetC_content());

        if (!ptrToNextStore -> Getexternal()) {
            manureToAdd -> Setamount(0.0);
        }
    }

#ifdef STRICT_BALANCES
    EndBudget(false);
#endif

    if (manureToAdd -> GetAmount() == 0.0) {
        delete manureToAdd;

        return nullptr;
    } else {
        return manureToAdd;
    }
}

/*
 * Return total amount of N in store
 */
double floorstore::GetTotalNStored() {
    double CurrentAmount = 0;

    for (int i = 0; i < theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

        CurrentAmount += ainstore -> GetTotalNStored();
    }

    return CurrentAmount;
}

/*
 * Return total amount of C in store
 */
double floorstore::GetTotalCStored() {
    double CurrentAmount = 0;

    for (int i = 0; i < theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

        CurrentAmount += ainstore -> GetTotalCStored();
    }

    return CurrentAmount;
}

/*
 * Return total amount of DM in store
 */
double floorstore::GetTotalDMStored() {
    double CurrentAmount = 0;

    for (int i = 0; i < theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

        CurrentAmount += ainstore -> GetTotalDMStored();
    }

    return CurrentAmount;
}

/*
 * Get total amount of manure in store
 */
double floorstore::GetManureAmount() {
    double CurrentAmount = 0;

    for (int i = 0; i < theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

        CurrentAmount += ainstore -> GetAmountManure();
    }

    return CurrentAmount;
}

manure * floorstore::GetManure() {
    manure * CurrentAmount = theOnFloorStores -> ElementAtNumber(0) -> GetManure();

    for (int i = 1; i < theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

        *CurrentAmount = *ainstore -> GetManure() + *CurrentAmount;
    }

    return CurrentAmount;
}

/*
 * Get some manure from store
 */
product * floorstore::GetFromManureStore(double RequestedAmount,
        product *                               p,
        bool                                    checkAccess) {
    product * aManure     = nullptr;
    product * manureFound = new manure();

    for (int i = 0; i < theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

        aManure = ainstore -> GetFromManureStore(RequestedAmount, p, checkAccess);

        if (aManure) {
            *manureFound + *aManure;

            delete aManure;

            aManure = nullptr;
        }
    }

    if (manureFound -> GetAmount() == 0.0) {
        delete manureFound;

        manureFound = nullptr;
    }

    return manureFound;
}

product * floorstore::TakeAllManureFromFloor(bool checkAccess) {
    product * aManure     = nullptr;
    product * manureFound = new manure();

    for (int i = 0; i < theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

        aManure = ainstore -> EmptyManureStore(checkAccess);

        if (aManure) {
            *manureFound + *aManure;

            delete aManure;

            aManure = nullptr;
        }
    }

    if (manureFound -> GetAmount() == 0.0) {
        delete manureFound;

        manureFound = nullptr;
    } else {
        Nbudget.AddOutput(manureFound -> GetAmount() * ((manure *) manureFound) -> GetAllN().n);
        Cbudget.AddOutput(manureFound -> GetAmount() * ((manure *) manureFound) -> GetC_content());
    }

    return manureFound;
}

/*
 * Check if all N is accounted for
 */
void floorstore::EndBudget(bool show) {
    if (show) {
        cout << "Balance for " << GetName() << ".(" << GetIndex() << ")" << endl;
    }

    // N budget
    double NRemaining = GetTotalNStored();

    if (!Nbudget.Balance(NRemaining)) {
        if (show) {
            cout << Nbudget << "NRemaining " << NRemaining << endl;
        }
    }

    // C budget
    double CRemaining = GetTotalCStored();

    if (!Cbudget.Balance(CRemaining)) {
        if (show) {
            cout << Cbudget;
            cout << "CRemaining " << CRemaining << endl;
        }
    }

    for (int i = 0; i < theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = (instore *) theOnFloorStores -> ElementAtNumber(i);

        ainstore -> EndBudget(show);
    }
}

/*
 * Scale floor to number of animals present
 */
void floorstore::ScaleSize(double annualManure) {
    if (theNextStores -> NumOfNodes()
            > 0)    // check to see if the manure is to be sent to another store. If so, scale that store.
    {
        manurestore * aStore = theNextStores -> ElementAtNumber(0);

        aStore -> Scale(annualManure);
    }

    // if ((instore *) aStore->GetnextStore())
    // ((instore *) aStore->GetnextStore())->ScaleInstore(annualManure);
}

/*
 * Operator << for output
 */
ostream & operator <<(ostream &          os,
                      const floorstore & s) {
    os << setiosflags(ios::left) << setw(40) << "Floor details for " << s.GetName() << endl;
    os << setiosflags(ios::left) << setw(40) << "excreta partition " << s.excretaPartition << endl;
    os << setiosflags(ios::left) << setw(40) << "manure type " << s.manuretype << endl;
    os << setiosflags(ios::left) << setw(40) << "floor area " << s.area << endl;
    os << setiosflags(ios::left) << setw(40) << "cleaning interval " << s.cleaningInterval << endl;

    for (int i = 0; i < s.theOnFloorStores -> NumOfNodes(); i++) {
        instore * ainstore = (instore *) s.theOnFloorStores -> ElementAtNumber(i);

        os << *ainstore;
    }

    return os;
}

string floorstore::getmanureName() {
    if (!theOnFloorStores -> NumOfNodes() == 1) {
        theMessage -> FatalError("floorstore::getmanureName - more than one manure type stored");
    }

    return theOnFloorStores -> ElementAtNumber(0) -> getManureName();
}

void floorstore::LinkManureStores(manurestore * aManureStore) {
    cout << aManureStore -> GetName() << endl;
    cout << manureToName << endl;
    cout << ManureToID << endl;
    cout << aManureStore -> GetIndex() << endl;

#ifdef NITROSCAPE
    if ((manureToName == (aManureStore -> GetName()) ) && ((aManureStore -> getmanureStoreID()) == ManureToID)) {
        theNextStores -> InsertLast(aManureStore);
    }
#endif

#ifndef NITROSCAPE
    if (manureToName == aManureStore -> GetName() && (aManureStore -> GetIndex() == ManureToID)) {
        theNextStores -> InsertLast(aManureStore);
    }
#endif

}
