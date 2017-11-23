/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

/**
 *  Describes a separate free-standing store)
 */
#include "../base/common.h"
#include "digestateStore.h"
#include "../base/IndicatorOutput.h"
#include "../base/climate.h"
#include "../base/controlParameters.h"
#include "../base/nixcbits.h"

#include "../data/outputData.h"
/****************************************************************************\
  Destructor
\****************************************************************************/
digestateStore::~digestateStore()
{
   external = true;
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
digestateStore::digestateStore(char *aName,const int aIndex,const base* aOwner)
 : slurrystore(aName,aIndex,aOwner)
{
}


/**
 * Init reads the relevant values from its owners file
 */
void digestateStore::ReadParameters(fstream * file)
{
	manurestore::ReadParameters(file);
	building::ReadParameters(file);
   Setfile(file);
   FindSection(aBase->GetName(),aBase->GetIndex()); // perform this in the top of hierarchy - and only there
//   GetParameter("AreaPrAnimal",&AreaPrAnimal);
   SetCritical();
   Setfile(NULL);
   if (theOutputData->DetailsData.getManureManageDetails()==true)
		OpenDetailsFile();
   else
   	detailsFile=NULL;
}

/**
 * 	Dailyactions
 */
void digestateStore::DailyUpdate()
{
	building::DailyUpdate();
/*	manurestore::DailyUpdate(volatAmmonia, N2Emission, N2OEmission, CO2Emission, CH4Emission);
   theOutput->AddIndicator(environmentalIndicator,"35.43 NH3-N volatilisation from manure storage","kg N",volatAmmonia*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"35.44 N2-N volatilisation from manure storage","kg N",N2Emission*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"35.45 N2O-N volatilisation from manure storage","kg N",N2OEmission*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"45.43 CO2-C volatilisation from manure storage","kg C",CO2Emission*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"45.44 CH4-C volatilisation from manure storage","kg C",CH4Emission*1000.0);
  */
   GiveIndicator(environmentalIndicator);

}


/****************************************************************************\
\****************************************************************************/
void digestateStore::GiveIndicator(int indicatorType)
{
	if (indicatorType==economicIndicator)
   			building::GiveIndicator(indicatorType);
	if (indicatorType==environmentalIndicator)
   {

/*      theOutput->AddStateIndicator(environmentalIndicator,"35.60 N bound in manure storage","kg N",N*1000.0);
      theOutput->AddStateIndicator(environmentalIndicator,"45.60 C bound in manure storage","kg C",C*1000.0);
      theOutput->AddStateIndicator(economicIndicator,"11.70 Slurry in digestateStore","t",Vt/1000);
  */ }
}

/**
 * Calling manurestore::RcvManure(const manure* someManure)
 */
void digestateStore::RcvManure(const manure* someManure)
{
	manurestore::RcvManure(someManure);
//	theOutput->AddIndicator(economicIndicator,"19.72 Biogas slurry produced","t",someManure->GetAmount());
}



