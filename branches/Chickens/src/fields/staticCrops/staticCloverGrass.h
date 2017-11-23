/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CloverGrass_H
#define CloverGrass_H

#include "staticCrop.h"

class staticCloverGrass: public staticCrop
{
protected:
	bool permanent;
	enum {Num_of_pr_str=3,Nitrogen_levels=1,Sand_max_N=0,Clay_max_N=0};

public:
	staticCloverGrass(bool permanentGrass);

	virtual double PVstratFactor(int PV_strat);
   void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	
	
	virtual double NitrogenAmount(int JB_NO,double free_N,int N_level);
	virtual double ExpectedYield_(int JB_NO,int N_level,int PV_strat);
	
	
	
};

#endif

