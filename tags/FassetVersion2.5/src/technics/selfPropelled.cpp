/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Noboe Andersen & Poul Lassen
     Date
 Revision   Bjørn Molt Petersen, May-June 1997
\****************************************************************************/
#include "../base/common.h"
#include "selfPropelled.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
selfPropelled::selfPropelled(const char * aName, const int aIndex, const base * aOwner)
   : techEquip(aName, aIndex, aOwner)
{
   fuelConsumption = 0.0;
   fuelConsumptionTransport = 0.0;
   hp = 0.0;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
selfPropelled::selfPropelled(const selfPropelled& aselfPropelled)
   : techEquip(aselfPropelled)
{
   fuelConsumption = aselfPropelled.fuelConsumption;
   fuelConsumptionTransport = aselfPropelled.fuelConsumptionTransport;
   hp = aselfPropelled.hp;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
selfPropelled::~selfPropelled()
{
}

/****************************************************************************\
\****************************************************************************/
void selfPropelled::ReadParameters(commonData * &file)
{
   techEquip::ReadParameters(file);

   file->FindItem("hp",&hp);
   file->FindItem("fuelConsumption",&fuelConsumption);
   file->FindItem("netCapacityWheat",&netCapacityWheat);
   file->FindItem("fuelConsumptionTransport",&fuelConsumptionTransport);

}


