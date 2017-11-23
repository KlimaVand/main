
#include "../base/common.h"
#include "building.h"
#include "../base/commonData.h"

building::building():
    base()
{
    Area     = 0.0;
    location = 0;
    scalable = false;
}

building::~building() {}

building::building(string        aName,
                   const int     aIndex,
                   const base *  aOwner,
                   commonData *& runData,
                   commonData *& original):
    base(aName,
         aIndex,
         aOwner)
{
    string Alias;

    scalable = false;
    Area     = 0.0;
    location = 0;

    original -> setCritical(true);
    original -> FindSection("all");
    original -> FindItem("scalable", &scalable);
    runData -> setCritical(false);
    runData -> FindSection("all");
    runData -> FindItem("scalable", &scalable);

    // string sectionName(aName);
    // sectionName = sectionName + "(" + conv.IntToString(aIndex) + ")";
    runData -> setCritical(true);
    runData -> FindSection(Name, Index);
    runData -> FindItem("Alias", &Alias);
    runData -> setCritical(false);
    original -> setCritical(true);

    int    indexInOld;
    string place;

    original -> findSection(aName, 0, "Alias", Alias, &place, &indexInOld);
    original -> FindSection(place, indexInOld);

    // original->FindItem("Area",&Area);
    if (!scalable) {
        runData -> FindItem("Area", &Area);
    }
}

building::building(const building & b):
    base(b)
{
    location = 0;
    Area     = b.Area;
    scalable = b.scalable;
}

/*
 * Does nothing
 */
void building::DailyUpdate() {

    // GiveIndicator();
}

ostream & operator <<(ostream &  os,
                      building * b) {

    // os << (base*) &b << endl;   // << isn't overloaded for base
    // os << setiosflags(ios::left) << setw(40) << "Name " << b->GetName() << "(" << b->Index << ")" << endl;
    return os;
}
