/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
NJH Jan 2001 - enables or disables fields for grazing 
\****************************************************************************/

#include <common.h>
#include "fiopgraz.h"

/****************************************************************************\
\****************************************************************************/
makeGrazable::makeGrazable()
{
}

/****************************************************************************\
\****************************************************************************/
makeGrazable::makeGrazable(const makeGrazable& h)
   : harvestFields(h)
{
 makeItPossible = h.makeItPossible;
}

/****************************************************************************\
\****************************************************************************/
makeGrazable::~makeGrazable()
{
}

/****************************************************************************\
\****************************************************************************/
makeGrazable* makeGrazable::clone() const
{
	makeGrazable* h=new makeGrazable(*this);
	return h;
}

/****************************************************************************\
\****************************************************************************/
void makeGrazable::DefinemakeGrazableOper(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist, bool makePossible)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,yr, mon, d, ar,fieldn,dist);
   makeItPossible = makePossible;
}

#ifdef MEASCOPE
/****************************************************************************\
\****************************************************************************/
void makeGrazable::DefinemakeGrazableOper(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
						int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper, double ar,int fieldn,double dist, bool makePossible)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,Startyr,Startmon,Startday,Endyr,Endmon,Endday,aTSum,asoilMoisture,anupper, ar,fieldn,dist);
   makeItPossible = makePossible;
}
#endif
