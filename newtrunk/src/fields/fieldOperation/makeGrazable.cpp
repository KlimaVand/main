/*
 * NJH Jan 2001 - enables or disables fields for grazing
 */

#include "../../base/common.h"
#include "makeGrazable.h"

makeGrazable::makeGrazable() {}

makeGrazable::makeGrazable(const makeGrazable & h):
    harvestFields(h)
{}

makeGrazable::~makeGrazable() {}

makeGrazable * makeGrazable::clone() const {
    makeGrazable * h = new makeGrazable(*this);

    return h;
}

void makeGrazable::DefinemakeGrazableOper(operationNames op,
        string                                           crop,
        string                                           cropNm,
        int                                              yr,
        int                                              mon,
        int                                              d,
        double                                           ar,
        int                                              fieldn,
        double                                           dist,
        bool                                             makePossible) {
    fieldOperationFields::DefineOp(op, crop, cropNm, yr, mon, d, ar, fieldn, dist);
}
