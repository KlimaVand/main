/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include <fiopml.h>
#include <technics.h>

/****************************************************************************\
\****************************************************************************/
manualLabourFields::manualLabourFields()
{
	length=0.0;
	hours=0.0;
}

/****************************************************************************\
\****************************************************************************/
manualLabourFields::manualLabourFields(const manualLabourFields& m)
   : fieldOperationFields(m)
{
	length=m.length;
	hours=m.hours;
}

/****************************************************************************\
\****************************************************************************/
manualLabourFields::~manualLabourFields()
{
}

/****************************************************************************\
\****************************************************************************/
manualLabourFields* manualLabourFields::clone() const
{
	manualLabourFields* m=new manualLabourFields(*this);
	return m;
}

/****************************************************************************\
\****************************************************************************/
void manualLabourFields::DefineManualLabourFieldsOp(operationNames op,
																	 id_string      crop,
																	 id_string      cropNm,
																	 int            yr,
																	 int            mon,
																	 int            d,
																	 double         ar,
																	 int            fieldn,
																	 double         dist,
																	 double         len,
																	 double         HoursPerUnit)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,yr, mon, d, ar,fieldn,dist);
	length=len;
	hours=HoursPerUnit;
}

/****************************************************************************\
\****************************************************************************/
bool manualLabourFields::CalcCost(linkList<operation>    * aOperationList,
                                  linkList<techEquip>     * aTechFarmList,
                                  calcLPType               mode,
                                  double                 * cost,
                                  bool                  update)
{
   if (!update && ((mode==hoursCombine) || (mode==hoursAnimalFert)
      || (mode==hoursMineralFert) || (mode==hoursPlantCare)
      || (mode==varCostAnimalFert) || (mode==varCostMineralFert)))
      return false;
	// Calculate cost of operation
   *cost=area*hours;
   if(update)
   {
	   theTechnics->UpdateFieldLabour(*cost);
      fstream* fs = theMessage->GiveHandle();
      *fs << theTime << " " << "manual labour " << *cost << endl;
   	theMessage->CloseHandle();
   }
   return true;
}


