/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef BEET_H
#define BEET_H

#include "staticCrop.h"
#include "../../base/message.h"

class staticBeet: public staticCrop
{
	enum {Num_of_pr_str=3,Nitrogen_levels=12};

   double Nopt(int JB_NO);
   double Ymin(int JB_NO);
   double Ymax(int JB_NO);

public:
   staticBeet();
	virtual double PVstratFactor(int PV_strat);
   void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	
	
	virtual double NitrogenAmount(int JB_NO,double free_N,int N_level);
	virtual double ExpectedYield_(int JB_NO,int N_level,int PV_strat);
   virtual double NUseSlurryAutumn(){return 0.45;};                // VERIFY !!!
   
#ifdef MEASCOPE
   void SpecifiedPeriods(fieldOrder * PP,int JB_NO,char * crop_id);
#endif
};

#endif

