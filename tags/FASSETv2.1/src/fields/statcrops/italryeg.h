/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef ITALRYEG_H
#define ITALRYEG_H

#include <statcrop.h>
#include <message.h>
#include <typer.h>

class static_italryeg: public staticCrop
{
protected:

public:
	static_italryeg();
   virtual int SpeciesCanGiveUsableStraw(){return 0;};
   cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
};

#endif

