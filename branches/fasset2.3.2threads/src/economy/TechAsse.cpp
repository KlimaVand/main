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
void TechAsset::initEconomy(commonData * &file)
{
	GenAsset::ReadDefaultParameters(file);
	file->FindItem("lifetimeHours",&LifeTimeHours);
}

/**
 *  Give Economic indicators
 *    This should be called by the actual object
 */
void TechAsset::GiveEconomicIndicators(void)
{
	int id =threadID->getID(std::this_thread::get_id());
   theOutput[id]->AddIndicator(703,"07.03 Depreciation machinery","Dkr",-getDepreciationValue());
   theOutput[id]->AddIndicator(705,"07.05 Insurance machinery","Dkr",-getInsuranceValue());
   theOutput[id]->AddIndicator(707,"07.07 Maintenance machinery","Dkr",-getMaintenanceValue());
   theOutput[id]->AddIndicator(303,"03.03 Value of machinery","Dkr",NewValue*AverageValuePercent);
}

