/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef GRASSESTAB_H
	#define GRASSESTAB_H
#include "staticCrop.h"

class staticGrassEstablished: public staticCrop
{
public:
	staticGrassEstablished();
   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList (fieldOrder * PP,int JB_NO,double reduction);
	
	
	

};

#endif

