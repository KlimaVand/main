/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#include <statcrop.h>

class static_fallow: public staticCrop
{
public:
	static_fallow();
   virtual void SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id);
	virtual cloneList<fieldOperationFields> * MakeOpList (fieldOrder * PP,int JB_NO,double reduction);
	virtual int NumOfProtectStrat_();
	virtual int NitrogenLevels_();
	virtual int FallowCrop();
	virtual double ExtraNRelease_();
};


