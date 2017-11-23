
#include "staticCrop.h"

class staticFallowPermanent:
    public staticCrop
{
    public:
        staticFallowPermanent();

        virtual void SpecifiedPlan(fieldOrder * PP,
                                   int          JB_NO,
                                   string       crop_id);

        virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,
                int                                                       JB_NO);
};
