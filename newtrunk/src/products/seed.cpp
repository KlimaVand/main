
#include "../base/common.h"
#include "seed.h"

#ifdef _STANDALONE

#include "../base/message.h"

#endif

seed::seed():
    liveMatter()
{
    InitVariables();
}

seed::seed(string       aName,
           const int    aIndex,
           const base * aOwner):
    liveMatter(aName,
               aIndex,
               aOwner)
{
    InitVariables();
}

seed::seed(const seed & aseed):
    liveMatter(aseed)
{
    standardDensity = aseed.standardDensity;
}

seed::~seed() {}

void seed::InitVariables() {
    liveMatter::InitVariables();

    standardDensity = 0.0;
    ObjType         = seedObj;
}

ostream & operator <<(ostream &    os,
                      const seed & aseed) {
    os << setiosflags(ios::left) << " Amount applied " << (aseed.amount) << " kg/ha " << "standardDensity "
       << aseed.standardDensity << " kg/ha ";

    return os;
}

product & seed::operator = (const product & someSeed) {
    seed * aSeed;

    aSeed = (seed *) (&someSeed);

    liveMatter::operator = (someSeed);

    standardDensity = aSeed -> GetStandardDensity();

    return *this;
}

product & seed::operator +(const product & someSeed) {
    seed * aSeed;

    aSeed = (seed *) (&someSeed);

    double aAmount          = aSeed -> GetAmount();
    double astandardDensity = aSeed -> GetStandardDensity();

    if (amount != 0) {
        standardDensity = (standardDensity * amount + aAmount * astandardDensity) / (amount + aAmount);
    } else {
        standardDensity = astandardDensity;
    }

    liveMatter::operator +(someSeed);

    return *this;
}

product & seed::operator -(const product & someSeed) {
    liveMatter::operator -(someSeed);

    return *this;
}

product * seed::clone() const {
    seed * aProduct = new seed(*this);

    return (product *) aProduct;
}

void seed::ReadParameters(commonData * data,
                          string       sectionName) {
    product::ReadParameters(data, sectionName);

    if (data -> FindSection(sectionName, Index)) {
        data -> FindItem("orgN_content", &orgN_content.n);
        data -> FindItem("dryMatter", &dryMatter);
        data -> FindItem("C_content", &C_content);
        data -> FindItem("standardDensity", &standardDensity);
    }
}
