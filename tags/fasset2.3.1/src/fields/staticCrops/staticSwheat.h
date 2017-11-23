/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef SPWHT_H
#define SPWHT_H

#include "staticCrop.h"
#include "../../base/message.h"
#include "../../base/typer.h"

class staticSwheat: public staticCrop
{
protected:
	enum {Num_of_pr_str=3,Nitrogen_levels=12};

   double Nopt(int JB_NO);
   double Ymin(int JB_NO);
   double Ymax(int JB_NO);

public:
	staticSwheat();

	virtual double PVstratFactor(int PV_strat);
   cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
   virtual void BasicOpList(fieldOrder * PP,int JB_NO,double reduction,char * crop_id);
	
	
	virtual double NitrogenAmount(int soil_type,double free_N,int N_level);
	virtual double ExpectedYield_(int soil_type,int N_level,int PV_strat);
   
};

#endif

