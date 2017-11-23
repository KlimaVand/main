/*
 * Revision: JB added support for several species
 */

#include "../../base/common.h"
#include "silageFields.h"

silageFields::silageFields() {
    for (int i = 0; i < MaxPlants; i++) {
        plantItems[i] = nullptr;
    }
}

silageFields::silageFields(const silageFields & h):
    fieldOperationFields(h)
{
    for (int i = 0; i < MaxPlants; i++) {
        if (h.plantItems[i]) {
            plantItems[i] = new plantItem(*h.plantItems[i]);
        } else {
            plantItems[i] = nullptr;
        }
    }
}

silageFields::~silageFields() {
    for (int i = 0; i < MaxPlants; i++) {
        if (plantItems[i]) {
            delete plantItems[i];
        }
    }
}

silageFields * silageFields::clone() const {
    silageFields * h = new silageFields(*this);

    return h;
}

void silageFields::DefinesilageFieldsOper(operationNames op,
        string                                           crop,
        string                                           cropNm,
        int                                              yr,
        int                                              mon,
        int                                              d,
        double                                           ar,
        int                                              fieldn,
        double                                           dist,
        plantItem *                                      thePlantItem) {
    fieldOperationFields::DefineOp(op, crop, cropNm, yr, mon, d, ar, fieldn, dist);

    for (int i = 0; i < MaxPlants; i++) {
        plantItems[i] = nullptr;
    }

    if (thePlantItem) {
        plantItems[0] = new plantItem(*thePlantItem);
    }
}

/*
 * is not implemented
 */
bool silageFields::CalcCost(linkList<operation> * aOperationList,    // A HACK !!!!!
                            linkList<techEquip> * aTechFarmList,
                            calcLPType            mode,
                            double *              cost,
                            bool                  update) {
    theMessage -> Warning("Currently no cost information for silage making");

    return true;
}
