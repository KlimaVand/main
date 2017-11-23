/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef POTATO_H
   #define POTATO_H

#include "staticCrop.h"
#include "../../base/message.h"

class staticPotato: public staticCrop
{
	enum {Num_of_pr_str=3,Nitrogen_levels=12};

   double Nopt(int JB_NO);
   double Ymin(int JB_NO);
   double Ymax(int JB_NO);

public:
   staticPotato();
	virtual double PVstratFactor(int PV_strat);
   void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	
	
	virtual double NitrogenAmount(int JB_NO,double free_N,int N_level);
	virtual double ExpectedYield_(int JB_NO,int N_level,int PV_strat);


   
};

#endif

