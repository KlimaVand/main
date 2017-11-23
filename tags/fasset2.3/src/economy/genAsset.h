#ifndef GenAsset_H
	#define GenAsset_H

#include "../base/base.h"

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

	void setAverageValuePercent(double aAverageValuePercent)
	 	{AverageValuePercent=aAverageValuePercent;};


	virtual double getMaintenanceValue() ;
	virtual double getInsuranceValue() ;
	virtual double getDepreciationValue() ;
   virtual double getTotalValue();
   virtual void SetScalable(bool aVal) {scalable = aVal;}

   virtual void SetActualCapacity(double aVal) {ActualCapacity = aVal;}

	virtual void ReadDefaultParameters(base* aBase); // Initialization by the actual object
	virtual void ReadParameters(base* aBase); // Initialization by the actual object
	virtual void GiveEconomicIndicators(void); // Gives indicators to theOutput
};

#endif
