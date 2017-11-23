/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author: Lars Kroll Kristensen
     Date: 1997
 Revision: Bj�rn Molt Petersen 1998
\****************************************************************************/

#include "../base/common.h"
#include "TechAsse.h"
#include "../base/IndicatorOutput.h"

/****************************************************************************\
  Constructor
\****************************************************************************/

TechAsset::TechAsset()
	:GenAsset()
{
	LifeTimeHours=0;
}

/****************************************************************************\
	Compute Depreciation
\****************************************************************************/
double TechAsset::getDepreciationValue()
{
	return NewValue*getHoursUsed()/LifeTimeHours;
}


/**
 * Loads the user defines values into fasset
 */
void TechAsset::initEconomy(base* aBase)
{
	GenAsset::ReadDefaultParameters(aBase);
	aBase->GetParameter("lifetimeHours",&LifeTimeHours);
}

/**
 *  Give Economic indicators
 *    This should be called by the actual object
 */
void TechAsset::GiveEconomicIndicators(void)
{
   theOutput->AddIndicator(economicIndicator,"07.03 Depreciation machinery","Dkr",-getDepreciationValue());
   theOutput->AddIndicator(economicIndicator,"07.05 Insurance machinery","Dkr",-getInsuranceValue());
   theOutput->AddIndicator(economicIndicator,"07.07 Maintenance machinery","Dkr",-getMaintenanceValue());
   theOutput->AddIndicator(economicIndicator,"03.03 Value of machinery","Dkr",NewValue*AverageValuePercent);
}

