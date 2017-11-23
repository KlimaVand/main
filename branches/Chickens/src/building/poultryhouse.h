#ifndef POULTRYHOUSE_H
#define POULTRYHOUSE_H

#include "../base/common.h"
#include "stable.h"
#include "poultrysection.h"
//class swineStock;

class poultryhouse :public stable
{
	//	swineStock* Livestock;
	double batchInterval;

public:
	poultryhouse(char* aname,const int aIndex,const base* aOwner,
			animalStock* alivestock=NULL);
	virtual ~poultryhouse();

	//   virtual double GetAreaUsed();                     //overrides stable::GetAreaUsed()
	virtual string GetAnimalType();
	double GetLabour();
	double GetProductionCycle();

	virtual void DailyUpdate();
	virtual void ReadParameters(fstream* file);

	virtual void GiveIndicator(int indicatorType);
	virtual bool CapacityOverflow(){return false;}
	void PutStockInSection();
	virtual double CalcMinimumVentilationRate();
	virtual double CalcMaximumVentilationRate();
};

#endif
