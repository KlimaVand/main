
#include "staticCrop.h"

class staticGrazingGrass:
    public staticCrop
{
    public:
        staticGrazingGrass();

        virtual void SpecifiedPlan(fieldOrder * PP,
                                   int          JB_NO,
                                   string       crop_id);

        virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,
                int                                                       JB_NO);

        // virtual int NumOfProtectStrat_();
        // virtual int NitrogenLevels_();
        // virtual int FallowCrop();
        // virtual double ExtraNRelease_();
};
