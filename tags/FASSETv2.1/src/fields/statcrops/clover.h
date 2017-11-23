/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <statcrop.h>

class static_clover: public staticCrop
{
public:
	static_clover();
   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	virtual int NumOfProtectStrat_();
	virtual int NitrogenLevels_();
   virtual double NitrogenAmount(int JB_NO,double free_N,int N_level);
	virtual double ExpectedYield_(int JB_NO,int N_level,int PV_strat);
	virtual double PhosphorAmount_();
	virtual double PotassiumAmount_();
	virtual double ExtraNRelease_();

};
