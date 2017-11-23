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
#include "implement.h"

/****************************************************************************\
   Constructor with arguments
\****************************************************************************/
implement::implement(const char * aName, const int aIndex, const base * aOwner)
   : techEquip(aName, aIndex, aOwner)
{
   fuelConsumption = 0.0;
}

/****************************************************************************\
   Copy Constructor
\****************************************************************************/
implement::implement(const implement& aimplement)
   : techEquip(aimplement)
{
   fuelConsumption = aimplement.fuelConsumption;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
implement::~implement()
{
}

/****************************************************************************\
\****************************************************************************/
void implement::ReadParameters(fstream * file,char * sectionName)
{
   techEquip::ReadParameters(file,sectionName);
   Setfile(file);
   GetParameter("fuelConsumption",&fuelConsumption);
   Setfile(NULL);
}

