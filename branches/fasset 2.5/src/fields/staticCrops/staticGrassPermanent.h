/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#include "staticCrop.h"

class staticGrassPermanent: public staticCrop
{
public:
	staticGrassPermanent();
	virtual cloneList<fieldOperationFields> * MakeOpList(fieldOrder * PP,int JB_NO);
	virtual int NumOfProtectStrat_();
	virtual int NitrogenLevels_();
	virtual int FallowCrop();
	virtual double ExtraNRelease_();
};

