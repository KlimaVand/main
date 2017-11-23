/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "manualLabourFields.h"
#include "../../technics/technics.h"

/****************************************************************************\
\****************************************************************************/
manualLabourFields::manualLabourFields()
{

	hours=0.0;
}

/****************************************************************************\
\****************************************************************************/
manualLabourFields::manualLabourFields(const manualLabourFields& m)
   : fieldOperationFields(m)
{

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

	hours=HoursPerUnit;
}

/**
 * Calculate the cost of this operation.
 * If update is true it will write the cost to the indicate file.
 * If update is false it does not do anything)
 */
bool manualLabourFields::CalcCost(linkList<operation>    * aOperationList,
                                  linkList<techEquip>     * aTechFarmList,
                                  double                 * cost,
                                  bool                  update)
{
   if (!update)
      return false;
	// Calculate cost of operation
   *cost=area*hours;
   if(update)
   {
	   theTechnics->UpdateFieldLabour(*cost);
	   ofstream* fs = theMessage->GiveHandle();
      *fs << theTime << " " << "manual labour " << *cost << endl;
   	theMessage->CloseHandle();
   }
   return true;
}


