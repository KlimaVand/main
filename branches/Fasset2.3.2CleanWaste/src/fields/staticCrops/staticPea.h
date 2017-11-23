/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef PEA_H
#define PEA_H

#include "staticCrop.h"

class staticPea: public staticCrop
{
protected:
	enum {Num_of_pr_str=3,Nitrogen_levels=1,Sand_max_N=0,Clay_max_N=0};

   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);

public:
	staticPea();

	virtual double PVstratFactor(int PV_strat);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	
	
	virtual double NitrogenAmount(int JB_NO,double free_N,int N_level);
	virtual double ExpectedYield_(int JB_NO,int N_level,int PV_strat);
	
	
	
};

#endif

