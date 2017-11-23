#ifndef LUCERNE_H
	#define LUCERNE_H
#include "staticCrop.h"

class staticLucerneEstablished: public staticCrop
{
public:
	staticLucerneEstablished();
   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList (fieldOrder * PP,int JB_NO);
	virtual int NumOfProtectStrat_();
	virtual int NitrogenLevels_();
	virtual int FallowCrop();
	virtual double ExtraNRelease_();
};

#endif

