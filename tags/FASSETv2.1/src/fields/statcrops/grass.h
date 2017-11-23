/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#include <statcrop.h>

class static_grass: public staticCrop
{
public:
	static_grass();
   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO,double reduction);
	virtual int NumOfProtectStrat_();
	virtual int NitrogenLevels_();
	virtual int FallowCrop();
	virtual double ExtraNRelease_();
};

