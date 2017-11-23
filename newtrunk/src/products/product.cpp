/*
 *
 * Revision 10/7 JB added corrected price calculations, check for
 * units, check for storages
 *
 */

#include "product.h"

#ifdef _STANDALONE

#include "../base/message.h"

#endif

product::product():
    base()
{
    InitVariables();
}

product::product(string       aName,
                 const int    aIndex,
                 const base * aOwner):
    base(aName,
         aIndex,
         aOwner)
{
    InitVariables();
}

product::product(const product & aproduct):
    base(aproduct)
{
    storable     = false;
    amount       = aproduct.amount;
    mainUnitName = aproduct.mainUnitName;
    aPrice       = aproduct.aPrice;
    ObjType      = aproduct.ObjType;
}

product::~product() {}

void product::InitVariables() {
    amount       = 0.0;
    mainUnitName = -1;
    aPrice       = nullptr;
    ObjType      = productObj;
}

ostream & operator <<(ostream &       os,
                      const product & aproduct) {
    os << setiosflags(ios::left) << setw(40) << "name " << aproduct.GetName() << endl;    // added LKK
    os << setiosflags(ios::left) << setw(40) << "amount " << aproduct.amount << endl;
    os << setiosflags(ios::left) << setw(40) << "mainUnitName " << aproduct.mainUnitName << endl;
    os << setiosflags(ios::left) << setw(40) << "aPrice " << aproduct.aPrice << endl;
    os << setiosflags(ios::left) << setw(40) << "ObjType " << aproduct.ObjType << endl;

    return os;
}

/*
 *
 * Function purpose: returns name of product that can be transformed to this class
 *
 */
string product::DerivedFrom() const {
    return "NONE";
}

product & product::operator = (const product & aProduct) {
    product * someProduct;

    someProduct = (product *) (&aProduct);

    string tmp = someProduct -> GetName();

    Name         = tmp;
    amount       = someProduct -> GetAmount();
    mainUnitName = someProduct -> GetmainUnitName();
    aPrice       = someProduct -> GetaPrice();
    ObjType      = someProduct -> GetObjType();
    Index        = someProduct -> GetIndex();

    return *this;
}

/*
 *
 * Needs cleaning up !!!
 *
 */
product & product::operator +(const product & aProduct) {
    product * someProduct;

    someProduct = (product *) (&aProduct);

    double aAmount       = someProduct -> GetAmount();
    int    aMainUnitName = someProduct -> GetmainUnitName();

    if (amount == 0) {
        mainUnitName = aMainUnitName;
        Name         = someProduct -> Name;
        aPrice       = someProduct -> GetaPrice();
    } else {
        if (mainUnitName != aMainUnitName) {
            string WarningString;

            WarningString = Name;

            WarningString.append(" + ");

            // if (someProduct -> GetName().length() == 0) {
            WarningString += (someProduct -> GetName()) + " : ";

            // } else {
            // WarningString += " : ";
            // }
            WarningString += GetUnitString(mainUnitName) + " + " + GetUnitString(someProduct -> GetmainUnitName());

            theMessage -> FatalError("product::operator+ - products with different units added: ",
                                     WarningString);
        }
    }

    amount += aAmount;

    return *this;
}

product & product::operator -(const product & aProduct) {
    product * someProduct;

    someProduct = (product *) (&aProduct);

    double aAmount = someProduct -> GetAmount();

    if (amount == 0) {
        aPrice = someProduct -> GetaPrice();
    } else if ((aAmount != 0) && (aPrice != someProduct -> GetaPrice())) {
        aPrice = nullptr;    // a composite item has no price??           JB
    }

    if (mainUnitName != someProduct -> GetmainUnitName()) {
        theMessage -> FatalError("product::operator- - products with different units subtracted");
    }

    amount -= aAmount;

    return *this;
}

product & product::operator *(const double & aFactor) {
    amount *= aFactor;

    return *this;
}

void product::showstats(ostream * os) {
    *os << "productname  " << Name << endl;
    *os << "amount       " << amount << endl;
    *os << "main unit    " << GetUnitString(mainUnitName) << endl;
}

product * product::clone() const {
    product * aProduct;

    aProduct    = new product;
    (*aProduct) = (*this);

    return aProduct;
}

int product::GetUnitType(string aMainUnitType) {
    bool found   = false;
    int  typeRes = -1;

    if (aMainUnitType == "kwh") {
        found   = true;
        typeRes = kwh;
    } else if (aMainUnitType == "tfi") {
        found   = true;
        typeRes = tfi;
    } else if (aMainUnitType == "t") {
        found   = true;
        typeRes = t;
    } else if (aMainUnitType == "kg") {
        found   = true;
        typeRes = kg;
    } else if (aMainUnitType == "g") {
        found   = true;
        typeRes = g;
    } else if (aMainUnitType == "m3") {
        found   = true;
        typeRes = m3;
    } else if (aMainUnitType == "l") {
        found   = true;
        typeRes = l;
    } else if (aMainUnitType == "ha") {
        found   = true;
        typeRes = ha;
    } else if (aMainUnitType == "m2") {
        found   = true;
        typeRes = m2;
    } else if (aMainUnitType == "m") {
        found   = true;
        typeRes = m;
    } else if (aMainUnitType == "mm") {
        found   = true;
        typeRes = mm;
    } else if (aMainUnitType == "sfu") {
        found   = true;
        typeRes = sfu;
    } else if (aMainUnitType == "day") {
        found   = true;
        typeRes = day;
    } else if (aMainUnitType == "hour") {
        found   = true;
        typeRes = hour;
    } else if (aMainUnitType == "kr") {
        found   = true;
        typeRes = kr;
    } else if (aMainUnitType == "fraction") {
        found   = true;
        typeRes = fraction;
    } else if (aMainUnitType == "pcs") {
        found   = true;
        typeRes = pcs;
    }

    if (!found) {
        theMessage -> WarningWithDisplay("product::GetUnitType - the unit type <", aMainUnitType,
                                         "> does not exist");

        return -1;
    } else {
        return typeRes;
    }
}

string product::GetUnitString(int aMainUnit) {

    return conv.GetUnitTypeIntToString(aMainUnit);
}

void product::ReadParameters(commonData *& file,
                             string        sectionName) {
    if (file -> FindSection(sectionName, Index)) {
        string aUnit;
        string name;

        file -> FindItem("productname", &name);

        Name = name;

        file -> FindItem("mainunitname", &aUnit);
        file -> FindItem("amount", &amount);

        mainUnitName = GetUnitType(aUnit);
    }
}
