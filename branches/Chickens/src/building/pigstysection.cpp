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
#include "pigstysection.h"

pigstysection::pigstysection(char* aName,const int aIndex,const base* aOwner)
   	:animalSection(aName,aIndex,aOwner)
{

   DaysInSection     = 0;
}

pigstysection::~pigstysection()
{
}

double pigstysection::GetCapacity(double ProductionCyclus)
{
	return Places*ProductionCyclus/DaysInSection;
}

/**
 * read user input file
 */
void pigstysection::ReadParameters(fstream *file)
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
