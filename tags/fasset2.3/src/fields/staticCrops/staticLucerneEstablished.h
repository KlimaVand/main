#ifndef LUCERNE_H
	#define LUCERNE_H
#include "staticCrop.h"

class staticLucerneEstablished: public staticCrop
{
public:
	staticLucerneEstablished();
   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList (fieldOrder * PP,int JB_NO,double reduction);
	
	

	
};

#endif

