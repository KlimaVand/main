/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef STATCROP_H
   #define STATCROP_H

#include <fiop.h>
#include <fiopan.h>
#include <fiopst.h>
#include <fiopmi.h>
#include <fiopso.h>
#include <fioppc.h>
#include <fiopml.h>
#include <fiopir.h>
#include <fiopha.h>
#include <fiopgr.h>
#include <fldordr.h>
#include <pfertil.h>
#include <pwater.h>
#include <pchemic.h>
#include <pplantit.h>
#include <period.h>
#include <clonlist.h>

class staticCrop
{
private:
	staticCrop& operator=(const staticCrop& s); // Dissable the compilers generation of default assignment operator.
	staticCrop(const staticCrop& s);            // Dissable the compilers generation of default copy constructor.

protected:
	enum {Num_of_pr_str=3,Nitrogen_levels=12,id_len=9,max_list_len=80};
	typedef fieldOperationFields *FOPT;
	cloneList<fieldOperationFields> * LP;
	int day,month,OpListItems,irrigation,conventional;
   bool gatheringStraw;
	typedef char str[20];
	str cropName;
	plantItem * aPlantItem1;
   plantItem * aPlantItem2;

	virtual void AddOpList(fieldOperationFields * action);
   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);

public:
	staticCrop();
	virtual ~staticCrop(){};
   void SetAdjustArea(cloneList<fieldOperationFields> * OP,fieldOrder * PP);
   char * GetSeedName(char * crop_id);
   virtual int SpeciesCanGiveUsableStraw(){return 0;};
   virtual int ImplementationDoesGiveStraw(){return 0;};
   char * GetCropName(){return cropName;};
	static void SetSorting(int year,int month,int day);
	static int SortFunc(fieldOperationFields *PP);
	static void Show(fieldOperationFields *PP);
   virtual void SetGatheringStraw(bool gS){gatheringStraw=gS;};
	virtual void InitCrop(int convent,int irrigt);
	virtual double PVstratFactor(int PV_strat);
	virtual void ResetOpList();
//	virtual void BasicOpList(fieldOrder * PP,int JB_NO,double reduction,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	virtual void ShowOpList(cloneList<fieldOperationFields> * OP);
	virtual int NumOfProtectStrat_();
	virtual int NitrogenLevels_();
	virtual int FallowCrop();
	virtual double NitrogenAmount(int JB_NO,double free_N,int N_level);
	virtual double ExpectedYield_(int JB_NO,
											int N_level,
											int PV_strat);
	virtual double  ExpectedStrawYield_(int JB_NO,int N_level,int PV_strat);
	virtual double PhosphorAmount_();
	virtual double PotassiumAmount_();
	virtual double ExtraNRelease_();
   virtual double NUseSlurrySpring(){return 0.0;};
   virtual double NUseSlurryAutumn(){return 0.0;};
   virtual int SlyrryPossibleThisPeriod(period * p) {return 0;};
   virtual void CorrectCropId(char * crop_id);
#ifdef MEASCOPE
	virtual void SpecifiedPeriods(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual void GetMineralFertilizer(fieldOrder * PP, int mincount, fertilizer *aFertilizer1,fertilizer *aFertilizer2,fertilizer *aFertilizer3);
#endif
};

#endif

