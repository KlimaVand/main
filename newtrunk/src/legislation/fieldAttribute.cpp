
#include "../base/common.h"
#include "fieldAttribute.h"

fieldAttrib::fieldAttrib():
    base()
{
    isgreen              = false;
    contributionThisYear = 0.0;
    contributionIndex    = 0;
    contributionList     = '\0';
    boundary             = 0;
    fieldName[0]         = '\0';
}

fieldAttrib::fieldAttrib(string aName,
                         int    aIndex,
                         base * aOwner):
    base(aName,
         aIndex,
         aOwner)
{
    isgreen              = false;
    contributionThisYear = 0.0;
    contributionIndex    = 0;
    contributionList     = '\0';
    boundary             = 0;
    fieldName[0]         = '\0';
}

fieldAttrib::~fieldAttrib() {
    delete[] contributionList;
}

void fieldAttrib::Initialize(string   afieldName,
                             bool     aisgreen,
                             double * acontributionList,
                             int      aboundary) {
    boundary             = aboundary;
    isgreen              = aisgreen;
    fieldName            = afieldName;
    contributionList     = acontributionList;
    contributionThisYear = acontributionList[contributionIndex];
}

bool fieldAttrib::Update() {
    if ((contributionIndex + 1) < boundary) {
        contributionIndex++;

        contributionThisYear = contributionList[contributionIndex];

        return true;
    } else {
        return false;
    }
}
