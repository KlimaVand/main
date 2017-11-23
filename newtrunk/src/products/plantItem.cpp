
#include "../base/common.h"
#include "plantItem.h"

#ifdef _STANDALONE

#include "../base/message.h"

#endif

plantItem::plantItem():
    decomposable()
{
    InitVariables();
}

plantItem::plantItem(string       aName,
                     const int    aIndex,
                     const base * aOwner):
    decomposable(aName,
                 aIndex,
                 aOwner)
{
    InitVariables();
}

plantItem::plantItem(const plantItem & aplantItem):
    decomposable(aplantItem)
{
    tradeable = aplantItem.tradeable;

    if (aplantItem.tradeName.length() > 0) {
        tradeName = aplantItem.tradeName;
    }
}

plantItem::~plantItem() {}

void plantItem::InitVariables() {
    tradeable = 0;

    tradeName.resize(0);

    ObjType = plantItemObj;
}

product & plantItem::operator = (const product & somePlantItem) {
    plantItem * aPlantItem;

    aPlantItem = (plantItem *) (&somePlantItem);

    decomposable::operator = (somePlantItem);

    tradeable = aPlantItem -> GetTradeable();
    tradeName = aPlantItem -> GetTradeName();

    return *this;
}

product & plantItem::operator +(const product & somePlantItem) {
    plantItem * aPlantItem;

    aPlantItem = (plantItem *) (&somePlantItem);
    tradeable  = tradeable && aPlantItem -> GetTradeable();

    decomposable::operator +(somePlantItem);

    return *this;
}

product & plantItem::operator -(const product & somePlantItem) {
    plantItem * aPlantItem;

    aPlantItem = (plantItem *) (&somePlantItem);
    tradeable  = tradeable && aPlantItem -> GetTradeable();

    if (tradeName != aPlantItem -> GetTradeName()) {
        theMessage -> WarningWithDisplay("plantItem::operator- - plants with different tradenames subtracted");
    }

    decomposable::operator -(somePlantItem);

    return *this;
}

product * plantItem::clone() const {
    plantItem * aProduct = new plantItem(*this);

    return aProduct;
}

void plantItem::ReadParameters(commonData * data,
                               string       sectionName) {
    decomposable::ReadParameters(data, sectionName);

    if (data -> FindSection(sectionName, Index)) {
        data -> FindItem("tradename", &tradeName);
        data -> FindItem("tradeable", &tradeable);
    }
}
