/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <grass1.h>

class static_seedgrass: public static_grass_estab
{
public:
	virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
   virtual double ExpectedYield_(int JB_NO,int N_level,int PV_strat);
};

