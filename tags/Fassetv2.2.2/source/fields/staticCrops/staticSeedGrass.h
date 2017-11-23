/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "staticGrassEstablished.h"

class staticSeedGrass: public staticGrassEstablished
{
public:
	virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
   virtual double ExpectedYield_(int JB_NO,int N_level,int PV_strat);
};

