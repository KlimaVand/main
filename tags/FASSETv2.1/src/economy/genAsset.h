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

#ifndef GenAsset_H
	#define GenAsset_H

#include <base.h>

class GenAsset
{
protected:

	   bool scalable;
	   double RefCapacity;     //max capacity (for whatever is in the building)
      double ActualCapacity;  // (for whatever is in the building)

	//Attributes
	double NewValue;            // DKK
	double AverageValuePercent; // A fraction of NewValue
	double MaintenancePercent;  // A fraction of NewValue per year or hour
	double InsurancePercent;    // A fraction of NewValue
	double DepreciationPercent; // A fraction of NewValue

	// Copy constructor disabled
	GenAsset(GenAsset& aGenAsset);

public:

	GenAsset();
	virtual ~GenAsset() {};
	void setNewValue(double aNewValue) {NewValue=aNewValue;};
	void setAverageValuePercent(double aAverageValuePercent)
	 	{AverageValuePercent=aAverageValuePercent;};
	virtual void setMaintenancePercent(double aMaintenancePercent)
	  	{MaintenancePercent=aMaintenancePercent;};
	void setInsurancePercent(double aInsurancePercent)
	  	{InsurancePercent=aInsurancePercent;};
	void setDepreciationPercent(double aDepreciationPercent)
	  	{DepreciationPercent=aDepreciationPercent;};

	virtual double getMaintenanceValue() ;
	virtual double getInsuranceValue() ;
	virtual double getDepreciationValue() ;
   virtual double getTotalValue();
   virtual void SetScalable(bool aVal) {scalable = aVal;}
   virtual double GetRefCapacity() {return RefCapacity;}
   virtual void SetActualCapacity(double aVal) {ActualCapacity = aVal;}

	virtual void initEconomy(base* aBase); // Initialization by the actual object
	virtual void GiveEconomicIndicators(void); // Gives indicators to theOutput
};

#endif
