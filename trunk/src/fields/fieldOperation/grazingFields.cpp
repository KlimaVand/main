/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "grazingFields.h"

grazingFields::grazingFields()
{
   nL=0;
   grazingEffiency = 0.84;
}

grazingFields::~grazingFields()
{
}

grazingFields::grazingFields(const grazingFields& g)
   : fieldOperationFields(g)
{
   nL=g.nL;
   grazingEffiency=g.grazingEffiency;
}

grazingFields* grazingFields::clone() const
{
	grazingFields* s=new grazingFields(*this);
	return s;
}

void grazingFields::DefineGrazingOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double nLoad,double eff)
{
	op_id=op;
	strcpy(crop_id,crop);
	strcpy(cropName,cropNm);
   opTime->SetTime(d,mon,yr);
	area=ar;
	fieldno=fieldn;
   nL=nLoad;
   grazingEffiency = eff;
}

