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
   Capacity     		= 0.0;
   VariableFeeding	= 0.0;
   VariableCleaning	= 0.0;
   ConstantFeeding	= 0.0;
   ConstantCleaning	= 0.0;
   variablePeriodical= 0.0;
   constantPeriodical= 0.0;
   numberOfAnimalsPrBatch= 0.0;
   batchInterval 		= 0.0;
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
 * Calculate how many hours that is used to feed and clean a pigstysection:
 */
double pigstysection::GetLabour()
{
 	double minutes;

  	minutes = NrOfAnimals *(VariableFeeding + VariableCleaning);
   minutes += ConstantFeeding + ConstantCleaning;
   return minutes/60;
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
   GetParameter("numberPigsPerSty",&numberPigsPerSty);
   GetParameter("VariableFeeding",&VariableFeeding);
   GetParameter("VariableCleaning",&VariableCleaning);
   GetParameter("ConstantFeeding",&ConstantFeeding);
   GetParameter("ConstantCleaning",&ConstantCleaning);
   GetParameter("VariablePeriodical",&variablePeriodical);
   GetParameter("ConstantPeriodical",&constantPeriodical);
   GetParameter("DaysInSection",&DaysInSection);
   UnsetCritical();
   Setfile(NULL);
}
