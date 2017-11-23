#ifndef LUCERNE_H
	#define LUCERNE_H
#include <statcrop.h>

class static_lucerne_estab: public staticCrop
{
public:
	static_lucerne_estab();
   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList (fieldOrder * PP,int JB_NO,double reduction);
	virtual int NumOfProtectStrat_();
	virtual int NitrogenLevels_();
	virtual int FallowCrop();
	virtual double ExtraNRelease_();
};

#endif

