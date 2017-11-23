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

#include "genAsset.h"

#ifndef TechAsset_H
	#define TechAsset_H

class TechAsset: public GenAsset
{
private:

	double LifeTimeHours;

public:

	TechAsset();
	~TechAsset(){};

	// getHoursUsed MUST be implemented by the inheriting class.
	// Should return the number of hours the techasset
	// has been used this year. Used to compute depreciation.
	virtual double getHoursUsed()=0;

	virtual double getMaintenanceValue() {return NewValue*MaintenancePercent*getHoursUsed();};
	virtual double getDepreciationValue();
   // initialization by the actual object
	virtual void initEconomy(base* aBase);
   virtual void GiveEconomicIndicators(void);
};

#endif

