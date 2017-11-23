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
#include "tractor.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/

tractor::tractor(const char * aName, const int aIndex, const base * aOwner)
   : techEquip(aName, aIndex, aOwner)
{
   hp=0.0;
   fuelConsumptionTransport = 0.0;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/

tractor::tractor(const tractor& atractor)
   : techEquip(atractor)
{
   hp = atractor.hp;
   fuelConsumptionTransport = atractor.fuelConsumptionTransport;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
tractor::~tractor()
{
}

/****************************************************************************\
\****************************************************************************/
void tractor::ReadParameters(fstream * file,char * sectionName)
{
   techEquip::ReadParameters(file,sectionName);
   Setfile(file);
   GetParameter("hp",&hp);
   GetParameter("fuelConsumptionTransport",&fuelConsumptionTransport);
   Setfile(NULL);
}


