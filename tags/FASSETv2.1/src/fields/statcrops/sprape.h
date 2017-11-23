/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef SPRAPE_H
#define SPRAPE_H

#include <statcrop.h>
#include <message.h>

class static_sprape: public staticCrop
{
	enum {Num_of_pr_str=3,Nitrogen_levels=12};

   virtual double Nopt(int JB_NO);
   virtual double Ymin(int JB_NO);
   virtual double Ymax(int JB_NO);
   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);

public:
   static_sprape();
	virtual double PVstratFactor(int PV_strat);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	virtual int NumOfProtectStrat_();
	virtual int NitrogenLevels_();
	virtual double NitrogenAmount(int JB_NO,double free_N,int N_level);
	virtual double ExpectedYield_(int JB_NO,int N_level,int PV_strat);
	virtual double PhosphorAmount_();
	virtual double PotassiumAmount_();
	virtual double ExtraNRelease_();
   virtual double NUseSlurrySpring(){return 0.55;};
   virtual double NUseSlurryAutumn(){return 0.45;};
   virtual int SlyrryPossibleThisPeriod(period * p);
};

#endif

