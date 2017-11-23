/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Noboe Andersen & Poul Lassen
     Date   1996
 Revision   Bj�rn Molt Petersen, May-June 1997
\****************************************************************************/

#include "../base/common.h"
#include "techContract.h"
#include "../base/base.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
techContract::techContract(const char * aName, const int aIndex, const base * aOwner)
   : techEquip(aName, aIndex, aOwner)
{
   startRentingPrice = 0.0;
   rentingPricePerUnit = 0.0;
   unitsHa = 0.0;
   unitsTon = 0.0;
   unitsHours = 0.0;
   numberOfTimes = 0;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
techContract::techContract(const techContract& atechContract)
   : techEquip(atechContract)
{
   startRentingPrice = atechContract.startRentingPrice;
   rentingPricePerUnit = atechContract.rentingPricePerUnit;
   unitsHa = atechContract.unitsHa;
   unitsTon = atechContract.unitsTon;
   unitsHours = atechContract.unitsHours;
   numberOfTimes = atechContract.numberOfTimes;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
techContract::~techContract()
{
}

/****************************************************************************\
 Purpose:    Controls reading of parameters from file
 Parameters:
\****************************************************************************/
void techContract::ReadParameters(commonData * &file)
{
   techEquip::ReadParameters(file);

   file->FindItem("size",&size);
   file->FindItem("carryCapacity",&carryCapacity);
   file->FindItem("startRentingPrice",&startRentingPrice);
   file->FindItem("rentingPricePerUnit",&rentingPricePerUnit);
 
}


