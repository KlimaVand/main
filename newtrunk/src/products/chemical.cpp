
#include "../base/common.h"
#include "chemical.h"

#ifdef _STANDALONE

#include <message.h>

#endif

chemical::chemical():
    product()
{
    InitVariables();
}

chemical::chemical(string       aName,
                   const int    aIndex,
                   const base * aOwner):
    product(aName,
            aIndex,
            aOwner)
{
    InitVariables();
}

chemical::chemical(const chemical & achemical):
    product(achemical)
{
    type                   = achemical.type;
    activeSubstancePerUnit = achemical.activeSubstancePerUnit;
    amountPerTFI           = achemical.amountPerTFI;
}

chemical::~chemical() {}

void chemical::InitVariables() {
    type                   = 0;
    activeSubstancePerUnit = 0.0;
    amountPerTFI           = 0.0;
    ObjType                = chemicalObj;
}

ostream & operator <<(ostream &        os,
                      const chemical & achemical) {
    os << (product &) achemical;
    os << setiosflags(ios::left) << setw(40) << "type " << achemical.type << endl;
    os << setiosflags(ios::left) << setw(40) << "activeSubstancePerUnit " << achemical.activeSubstancePerUnit << endl;
    os << setiosflags(ios::left) << setw(40) << "amountPerTFI " << achemical.amountPerTFI << endl;

    return os;
}

product & chemical::operator = (const product & someChemical) {
    chemical * aChemical;

    aChemical = (chemical *) (&someChemical);

    product::operator = (someChemical);

    type                   = aChemical -> Gettype();
    activeSubstancePerUnit = aChemical -> GetactiveSubstancePerUnit();    // denne var kommenteret før??   JB
    amountPerTFI           = aChemical -> GetAmountPerTFI();

    return *this;
}

product & chemical::operator +(const product & someChemical) {
    chemical * aChemical;

    aChemical = (chemical *) (&someChemical);

    string tmp1 = someChemical.GetName();
    string tmp  = GetName();

    if (tmp1 != tmp) {
        theMessage -> FatalError("chemical::operator+ - chemicals should not be added");
    }

    if (type != aChemical -> Gettype()) {
        theMessage -> FatalError("chemical::operator+ - chemicals with different types added");
    }

    if ((activeSubstancePerUnit != aChemical -> GetactiveSubstancePerUnit())
            || (amountPerTFI != aChemical -> GetAmountPerTFI())) {
        theMessage -> FatalError("chemical::operator+ - chemicals with different composition added");
    }

    product::operator +(someChemical);

    return *this;
}

product & chemical::operator -(const product & someChemical) {
    chemical * aChemical;

    aChemical = (chemical *) (&someChemical);

    if (aChemical -> GetName() != Name) {
        theMessage -> FatalError("chemical::operator- - chemicals should not be subtracted");
    }

    if (type != aChemical -> Gettype()) {
        theMessage -> FatalError("chemical::operator- - chemicals with different types subtracted");
    }

    if ((activeSubstancePerUnit != aChemical -> GetactiveSubstancePerUnit())
            || (amountPerTFI != aChemical -> GetAmountPerTFI())) {
        theMessage -> FatalError("chemical::operator- - chemicals with different composition subtracted");
    }

    product::operator -(someChemical);

    return *this;
}

void chemical::showstats(ostream * os) {
    product::showstats(os);

    *os << "activeSubstancePerUnit          " << activeSubstancePerUnit << endl;
    *os << "amountPerTFI                    " << amountPerTFI << endl;
}

product * chemical::clone() const {
    chemical * aProduct = new chemical(*this);

    return (product *) aProduct;
}

// double chemical::GetByTFI(void) {
// return amount / amountPerTFI;
// }
void chemical::SetByTFI(double aTFI) {
    amount = aTFI * amountPerTFI;
}

int chemical::GetChemType(string aChemType) {


    return conv.GetChemType(aChemType);
}

void chemical::ReadParameters(commonData * data,
                              string       sectionName) {
    product::ReadParameters(data, sectionName);

    if (data -> FindSection(sectionName, Index)) {
        string aChemType;

        data -> FindItem("type", &aChemType);
        data -> FindItem("activeSubstancePerUnit", &activeSubstancePerUnit);
        data -> FindItem("amountPerTFI", &amountPerTFI);

        if (amountPerTFI <= 0) {
            theMessage -> FatalError("Chemical product <", Name, "> has illegal value in field amountPerTFI");
        }

        Settype(GetChemType(aChemType));
    }
}
