
#ifndef barleyPea_H
#define barleyPea_H

#include "staticCrop.h"

class staticBarleyPea:
    public staticCrop
{
    protected:
        enum {
            Num_of_pr_str = 3, Nitrogen_levels = 1, Sand_max_N = 0, Clay_max_N = 0
        };

    public:
        staticBarleyPea();

        virtual int SpeciesCanGiveUsableStraw() {
            return 0;
        }

        virtual double PVstratFactor(int PV_strat);

        virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,
                int                                                       JB_NO);

        virtual void SpecifiedPlan(fieldOrder * PP,
                                   int          JB_NO,
                                   string       crop_id);

        // virtual int NumOfProtectStrat_();
        // virtual int NitrogenLevels_();
        virtual double NitrogenAmount(int    JB_NO,
                                      double free_N,
                                      int    N_level);

        virtual double ExpectedYield_(int JB_NO,
                                      int N_level,
                                      int PV_strat);

        // virtual double PhosphorAmount_();
        // virtual double PotassiumAmount_();
        // virtual double ExtraNRelease_();
};
#endif

