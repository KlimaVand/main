/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
   Author Lars Kroll lokal 1875
     Date 21/5 97
 Revision NJH Jan 2002
 ----------------------------------------------------------------------------
      Log
\****************************************************************************/
#include "../base/common.h"
#include "poultrysection.h"

poultrysection::poultrysection(char* aName,const int aIndex,const base* aOwner)
   	:animalSection(aName,aIndex,aOwner)
{
   Capacity     		= 0.0;
   numberOfAnimalsPrBatch= 0.0;
   batchInterval 		= 0.0;
   DaysInSection     = 0;
}

poultrysection::~poultrysection()
{
}

double poultrysection::GetCapacity(double ProductionCyclus)
{
	return Places*ProductionCyclus/DaysInSection;
}
/**
 * Calculate how many hours that is used to feed and clean a poultrysection:
 */
/**
 * read user input file
 */
void poultrysection::ReadParameters(fstream *file)
{
	animalSection::ReadParameters(file);
	Setfile(file);
   string namestr;
   char Indexstr[10];
   sprintf(Indexstr,"(%d)",Index);
	namestr=(string) GetName()+Indexstr;
   SetCritical();
   FindSection(namestr);
   GetParameter("DaysInSection",&DaysInSection);
   UnsetCritical();
   Setfile(NULL);
}
