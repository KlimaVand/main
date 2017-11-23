/*
 * This class represent animal that are produced
 */

#include "../base/common.h"
#include "animalProduct.h"

#ifdef _STANDALONE

#include <message.h>

#endif

animalProduct::animalProduct():
    liveMatter()
{
    InitVariables();
}

animalProduct::animalProduct(string       aName,
                             const int    aIndex,
                             const base * aOwner):
    liveMatter(aName,
               aIndex,
               aOwner)
{
    InitVariables();
}

animalProduct::animalProduct(const animalProduct & aanimalProduct):
    liveMatter(aanimalProduct)
{
    animalPrHa  = aanimalProduct.animalPrHa;
    startWeight = aanimalProduct.startWeight;
    endWeight   = aanimalProduct.endWeight;
}

animalProduct::~animalProduct() {}

void animalProduct::InitVariables() {
    animalPrHa  = 0.0;
    startWeight = 0.0;
    endWeight   = 0.0;
    ObjType     = animalObj;
}

ostream & operator <<(ostream &             os,
                      const animalProduct & aanimalProduct) {
    os << (product &) aanimalProduct;
    os << setiosflags(ios::left) << setw(40) << "startWeight " << aanimalProduct.startWeight << endl;
    os << setiosflags(ios::left) << setw(40) << "endWeight " << aanimalProduct.endWeight << endl;
    os << setiosflags(ios::left) << setw(40) << "animalPrHa " << aanimalProduct.animalPrHa << endl;

    return os;
}

product & animalProduct::operator = (const product & someanimalProduct) {
    animalProduct * aanimalProduct;

    aanimalProduct = (animalProduct *) (&someanimalProduct);

    liveMatter::operator = (someanimalProduct);

    startWeight = aanimalProduct -> GetStartWeight();
    endWeight   = aanimalProduct -> GetEndWeight();
    animalPrHa  = aanimalProduct -> GetAnimalPrHa();

    return *this;
}

product & animalProduct::operator +(const product & someanimalProduct) {
    animalProduct * ananimalProduct = (animalProduct *) (&someanimalProduct);
    double          aAmount         = ananimalProduct -> GetAmount();
    double          astartWeight    = ananimalProduct -> GetStartWeight();
    double          aendWeight      = ananimalProduct -> GetEndWeight();

    if (aAmount != 0) {
        startWeight = (startWeight * amount + astartWeight * aAmount) / (amount + aAmount);
        endWeight   = (endWeight * amount + aendWeight * aAmount) / (amount + aAmount);
    }

    liveMatter::operator +(someanimalProduct);

    return *this;
}

product & animalProduct::operator -(const product & someanimalProduct) {
    liveMatter::operator -(someanimalProduct);

    if (Name != someanimalProduct.GetName()) {
        theMessage -> WarningWithDisplay(
            "animalProduct::operator-: different animal products should not be subtracted");
    }

    return *this;
}

void animalProduct::showstats(ostream * os) {
    liveMatter::showstats(os);

    *os << "startWeight        " << startWeight << endl;
    *os << "endWeight          " << endWeight << endl;
    *os << "animalPrHa         " << animalPrHa << endl;
}

product * animalProduct::clone() const {
    animalProduct * aProduct = new animalProduct(*this);

    return aProduct;
}

/*
 * Read the parameter from file
 */
void animalProduct::ReadParameters(commonData * data,
                                   string       sectionName) {
    product::ReadParameters(data, sectionName);    // do not use liveMatter's function - has irrelevant paramters

    if (data -> FindSection(sectionName, Index)) {
        double N_content;

        data -> FindItem("orgN_content", &N_content);

        orgN_content = N_content;

        data -> FindItem("startWeight", &startWeight);
        data -> FindItem("endWeight", &endWeight);
        data -> FindItem("C_content", &C_content);
    }
}