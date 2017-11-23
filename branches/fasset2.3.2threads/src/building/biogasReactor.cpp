/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "biogasReactor.h"
#include "../base/IndicatorOutput.h"
#include "../base/climate.h"
#include "../base/controlParameters.h"
#include "../base/nixcbits.h"
#include "../base/settings.h"

/****************************************************************************\
  Destructor
\****************************************************************************/
biogasReactor::~biogasReactor()
{
   external = true;
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
biogasReactor::biogasReactor(char *aName,const int aIndex,const base* aOwner,commonData * runData,commonData * common)
 : manurestore(aName,aIndex,aOwner, runData, common), building(aName,aIndex,aOwner,runData, common)
{
	 if (globalSettings[id]->DetailsData.getManureManageDetails()==true)
			OpenDetailsFile();
	   else
	   	detailsFile=NULL;
}



/**
 * Dailyactions
 */
void biogasReactor::DailyUpdate()
{
	building::DailyUpdate();
/*   theOutput[id]->AddIndicator("35.43 NH3-N volatilisation from manure storage","kg N",volatAmmonia*1000.0);
   theOutput[id]->AddIndicator("35.44 N2-N volatilisation from manure storage","kg N",N2Emission*1000.0);
   theOutput[id]->AddIndicator("35.45 N2O-N volatilisation from manure storage","kg N",N2OEmission*1000.0);
   theOutput[id]->AddIndicator("45.43 CO2-C volatilisation from manure storage","kg C",CO2Emission*1000.0);
   theOutput[id]->AddIndicator("45.44 CH4-C volatilisation from manure storage","kg C",CH4Emission*1000.0);
   GiveIndicator(environmentalIndicator);
  */

}

/****************************************************************************\
\****************************************************************************/
void biogasReactor::GiveIndicator(int indicatorType)
{
	if (indicatorType==economicIndicator)
   			building::GiveIndicator(indicatorType);
	if (indicatorType==environmentalIndicator)
   {
/*
      theOutput[id]->AddStateIndicator("35.60 N bound in manure storage","kg N",N*1000.0);
      theOutput[id]->AddStateIndicator("45.60 C bound in manure storage","kg C",C*1000.0);
      theOutput[id]->AddStateIndicator("11.70 Slurry in biogasReactor","t",Vt/1000);
  */ }
}

/****************************************************************************\
	Scale the manure storage that receives material from this instore  Note the cumulative effect on capacity
\****************************************************************************/
void biogasReactor::Scale(double annualAmount)
{
   scalable = true;
   double newCapacity = annualAmount + GetCapacity();
   manurestore::Scale(newCapacity);  //scales capcity and area via manurestore
}

/****************************************************************************\
	Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os,biogasReactor s)
{
/* os << setiosflags(ios::left) << setw(40) << s.rT << endl;
   os << setiosflags(ios::left) << setw(40) << "Cover resistance " << s.rCover << endl;*/
   return os;
}



