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
#include "installation.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
installation::installation(const char * aName, const int aIndex, const base * aOwner)
   : techEquip(aName, aIndex, aOwner)
{
   capacity = 0.0;
   electricityConsumption = 0.0;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
installation::installation(const installation& ainstallation)
   : techEquip(ainstallation)
{
   capacity = ainstallation.capacity;
   electricityConsumption = ainstallation.electricityConsumption;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
installation::~installation()
{
}

/****************************************************************************\
\****************************************************************************/
void installation::ReadParameters(fstream * file,char * sectionName)
{
   techEquip::ReadParameters(file,sectionName);
   Setfile(file);
   GetParameter("electricityConsumption",&electricityConsumption);
   GetParameter("capacity",&capacity);
   Setfile(NULL);
}


