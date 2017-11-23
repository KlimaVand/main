/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef ITALRYEG_H
#define ITALRYEG_H

#include "staticCrop.h"
#include "../../base/message.h"
#include "../../base/typer.h"

class staticItalianRyeGrass: public staticCrop
{
protected:

public:
	staticItalianRyeGrass();

   cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
};

#endif

