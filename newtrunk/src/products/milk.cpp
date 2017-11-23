
#include "../base/common.h"
#include "milk.h"

#ifdef _STANDALONE

#include "../base/message.h"

#endif

// needs updating wrt protein, urea
milk::milk():
    product()
{
    InitVariables();
}

milk::milk(string       aName,
           const int    aIndex,
           const base * aOwner):
    product(aName,
            aIndex,
            aOwner)
{
    InitVariables();
}

milk::milk(const milk & amilk):
    product(amilk)
{
    fat       = amilk.fat;
    protein   = amilk.protein;
    urea      = amilk.urea;
    dryMatter = amilk.dryMatter;
}

milk::~milk() {}

void milk::InitVariables() {
    fat       = 0.0;
    dryMatter = 0.0;
    protein   = 0.0;
    urea      = 0.0;
    ObjType   = milkObj;
}

product & milk::operator = (const product & someMilk) {
    milk * aMilk;

    aMilk = (milk *) (&someMilk);

    product::operator = (someMilk);

    fat       = aMilk -> Getfat();
    dryMatter = aMilk -> GetdryMatter();

    return *this;
}

product & milk::operator +(const product & someMilk) {
    milk * aMilk;

    aMilk = (milk *) (&someMilk);

    double aAmount    = aMilk -> GetAmount();
    double afat       = aMilk -> Getfat();
    double adryMatter = aMilk -> GetdryMatter();

    if (amount != 0) {
        fat       = (fat * amount + afat * aAmount) / (amount + aAmount);
        dryMatter = (dryMatter * amount + adryMatter * aAmount) / (amount + aAmount);
    } else {
        fat       = afat * aAmount;
        dryMatter = adryMatter * aAmount;
    }

    product::operator +(someMilk);

    return *this;
}

product & milk::operator -(const product & someMilk) {
    milk * aMilk;

    aMilk = (milk *) (&someMilk);

    if ((fat != aMilk -> Getfat()) || (dryMatter != aMilk -> GetdryMatter())) {
        theMessage -> Warning("milk::operator- - milk with different composition subtracted");
    }

    product::operator -(someMilk);

    return *this;
}

product * milk::clone() const {
    milk * aProduct = new milk(*this);

    return (product *) aProduct;
}

void milk::ReadParameters(commonData * data,
                          string       sectionName) {
    product::ReadParameters(data, sectionName);

    if (data -> FindSection(sectionName, Index)) {
        data -> FindItem("fat", &fat);
        data -> FindItem("drymatter", &dryMatter);
    }
}
