
#include "../base/common.h"
#include "legislation.h"
#include "../products/product.h"
#include "../products/products.h"
#include "../products/premium.h"
#include "../base/commonData.h"

legislate::legislate():
    base()
{
    InitVariables();
}

legislate::legislate(string aName,
                     int    aIndex,
                     base * aOwner):
    base(aName,
         aIndex,
         aOwner)
{
    InitVariables();
}

legislate::~legislate() {
    delete fieldAttribList;
    delete taxList;
}

void legislate::InitVariables() {
    fieldAttribList = new linkList<product>;
    taxList         = new linkList<taxLegislation>;
    simulationYear  = 0;
    GreenPrecent    = 0.0;
    Fallow          = 0.05;
    Nprice          = -5.0;
    Pprice          = -8.0;
    Kprice          = -3.0;
    Nmax            = 10000;
}

void legislate::Initialize(string LawApplyFileName) {
    commonData data;

    data.readFile(LawApplyFileName);

    if (fieldAttribList) {
        delete fieldAttribList;
    }

    fieldAttribList = theProducts -> GetSubsetofProductList(premiumObj);

    InitializeTax();
    data.FindSection("general");
    data.FindItem("GreenPrecent", &GreenPrecent);
    data.FindItem("Fallow", &Fallow);
    data.FindItem("Nprice", &Nprice);
    data.FindItem("Pprice", &Pprice);
    data.FindItem("Kprice", &Kprice);
    data.FindItem("Nmax", &Nmax);
}

void legislate::InitializeTax() {
    commonData * data = new commonData();

    data -> setCritical(true);

    int first, num;

    data -> getSectionsNumbers("legislat", &first, &num);

    for (int index = first; index < (first + num); index++) {
        taxLegislation * Alegislation = new taxLegislation("legislat", index, this);

        Alegislation -> ReadParameters(data);
        taxList -> InsertLast(Alegislation);
    }

    taxLegislation * Alegislation = new taxLegislation(0, 0, 0, 1000, "legislat", -1, this);    // default tax

    taxList -> InsertLast(Alegislation);

    delete data;
}

double legislate::GetCurrentTax(int type) {
    for (int i = 0; i < taxList -> NumOfNodes(); i++) {
        if (((taxList -> ElementAtNumber(i) -> getType()) == (type + 1))
                && (taxList -> ElementAtNumber(i) -> taxThisYear(simulationYear))) {
            return taxList -> ElementAtNumber(i) -> getTax();
        }
    }

    return 0.0;    // default element at end of taxlist
}

double legislate::GetCurrentLevel(int type) {
    for (int i = 0; i < taxList -> NumOfNodes(); i++) {
        if (((taxList -> ElementAtNumber(i) -> getType()) == (type + 1))
                && (taxList -> ElementAtNumber(i) -> taxThisYear(simulationYear))) {
            return taxList -> ElementAtNumber(i) -> getLevel();
        }
    }

    return 0.0;    // default element at end of taxlist
}

void legislate::Update() {
    simulationYear++;
}
