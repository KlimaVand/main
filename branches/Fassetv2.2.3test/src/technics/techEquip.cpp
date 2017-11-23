/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Noboe Andersen & Poul Lassen
     Date
 Revision   Bj�rn Molt Petersen, May-June 1997
\****************************************************************************/

#include "../base/common.h"
#include "techEquip.h"
#include "../base/IndicatorOutput.h"
#include <string.h>
/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
techEquip::techEquip(const char * aName, const int aIndex, const base * aOwner)
   : base(aName, aIndex, aOwner)
{
   techEquipName = "";
   size = 0.0;
   carryCapacity = 0.0;
   age = 0;
   hoursPrimo = 0;
   hoursUltimo = 0;
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
techEquip::techEquip(const techEquip& atechEquip)
   : base(atechEquip)
{
   techEquipName   = atechEquip.techEquipName;
   size            = atechEquip.size;
   carryCapacity   = atechEquip.carryCapacity;
   age             = atechEquip.age;
   hoursPrimo      = atechEquip.hoursPrimo;
   hoursUltimo     = atechEquip.hoursUltimo;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
techEquip::~techEquip()
{
}

/****************************************************************************\
\****************************************************************************/
void techEquip::ReadParameters(fstream * file,char * sectionName)
{
   char charVar[80]="";
   char section[80]="";
   Setfile(file);
   if (sectionName)
      strcpy(section,sectionName);
   else
      strcpy(section,Name);
   if (FindSection(section,Index))
   {
      GetParameter("techEquipName",charVar);
      techEquipName=charVar;
      GetParameter("size",&size);
      GetParameter("carryCapacity",&carryCapacity);
      initEconomy(this); // economy class
   }
   else
      theMessage->FatalError("techEquip::ReadParameters - ",section," not found");
   Setfile(NULL);
}

/****************************************************************************\
\****************************************************************************/
double techEquip::getHoursUsed()
{
	return GethoursUltimo()-GethoursPrimo();
}

/****************************************************************************\
\****************************************************************************/
void techEquip::UpdateHours()
{
   hoursPrimo = 0.0;
   hoursUltimo = 0.0;
}

/****************************************************************************\
	Should be called once a year
\****************************************************************************/
void techEquip::GiveIndicators()
{
   GiveEconomicIndicators();
   hoursPrimo=hoursUltimo;
}


