/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef SPBARL_H
   #define SPBARL_H

#include "staticCrop.h"

class staticSbarley: public staticCrop
{
protected:
	enum {Num_of_pr_str=2,Nitrogen_levels=12};
   bool underSown;                              // med udlæg

   virtual double Nopt(int JB_NO);
   virtual double Ymin(int JB_NO);
   virtual double Ymax(int JB_NO);
   virtual void HarvestPlan(fieldOrder * PP,int JB_NO,char * crop_id);

public:
   staticSbarley();
   virtual int ImplementationDoesGiveStraw() {return gatheringStraw;};
   virtual void SetUnderSown(bool uS){underSown=uS;};
   virtual int SpeciesCanGiveUsableStraw(){return 1;};
	virtual double PVstratFactor(int PV_strat);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	virtual int NumOfProtectStrat_();
	virtual int NitrogenLevels_();
	virtual double NitrogenAmount(int JB_NO,double free_N,int N_level);
	virtual double ExpectedYield_(int JB_NO,int N_level,int PV_strat);
   virtual double ExpectedStrawYield_(int JB_NO,int N_level,int PV_strat);
	virtual double PhosphorAmount_();
	virtual double PotassiumAmount_();
	virtual double ExtraNRelease_();
   virtual double NUseSlurrySpring(){return 0.55;};
   virtual int SlyrryPossibleThisPeriod(period * p);
};

#endif

