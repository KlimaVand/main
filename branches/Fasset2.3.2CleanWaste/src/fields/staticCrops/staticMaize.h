/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef MAIZE_H
   #define MAIZE_H

#include "staticCrop.h"

class staticMaize: public staticCrop
{
protected:
	enum {Num_of_pr_str=2,Nitrogen_levels=12};
   bool underSown;                              // med udlæg

   virtual double Nopt(int JB_NO);
   virtual double Ymin(int JB_NO);
   virtual double Ymax(int JB_NO);
   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);

public:
   staticMaize();

   virtual void SetUnderSown(bool uS){underSown=uS;};

	virtual double PVstratFactor(int PV_strat);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	
	
	virtual double NitrogenAmount(int JB_NO,double free_N,int N_level);
	virtual double ExpectedYield_(int JB_NO,int N_level,int PV_strat);
   virtual double ExpectedStrawYield_(int JB_NO,int N_level,int PV_strat);

   
};

#endif

