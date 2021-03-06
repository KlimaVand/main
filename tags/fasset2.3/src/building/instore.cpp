/****************************************************************************\
 Created by NJH - 25.02.00
  ----------------------------------------------------------------------------
      Log
 Describes a inhouse manure store
\****************************************************************************/
#include "../base/common.h"
#include "instore.h"
#include "../base/controlParameters.h"
#ifndef _STANDALONE
#include "../base/IndicatorOutput.h"
#include "../data/commonData.h"
#include "../tools/convert.h"
#endif

/****************************************************************************\
  Destructor
\****************************************************************************/
instore::~instore()
{
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
instore::instore(const char *aName,const int aIndex,const base* aOwner,commonData * runData,commonData * orignal)
: base(aName,aIndex,aOwner), manurestore(aName,aIndex,aOwner, runData, orignal),GenAsset()

{
	runData->setCritical(true);
	runData->FindSection(Name,Index);
	runData->FindItem("Alias",&Alias);
	runData->setCritical(false);
	orignal->setCritical(true);
	string sectionOldBase(Name);

	int indexInOld;
		string place;
		orignal->findSection(aName,0,"Alias",Alias,&place,&indexInOld);
		orignal->FindSection(place,indexInOld);


	orignal->FindItem("ManureToID",&ManureToID);

	runData->FindItem("ManureToID",&ManureToID);

		switch (theControlParameters->GetvolatModel())
		{
		case 0:
		{

			break;
		}
		case 1:
		{

			orignal->FindItem("AreaPrAnimal",&AreaPrAnimal);
			runData->FindItem("AreaPrAnimal",&AreaPrAnimal);
			orignal->FindItem("VelocityConstant",&velocityConstant);
			runData->FindItem("VelocityConstant",&velocityConstant);

			break;
		}
		}

}
instore::instore(const instore& aInstore):manurestore(aInstore)
{
	velocityConstant=aInstore.velocityConstant;
}


/**
 * Control that Fasset does not exceed the Capacity of the instore
 */

void instore::DailyUpdate()
{
	if (theStore->Getamountstored()>theStore->GetCapacity())
		theMessage->FatalError(manurestore::GetName()," inhouse manure storage capacity exceeded");
}

/**
 * Volatize gasses from the inhouse manure store - used for static (emission factor) modelling
 */


void instore::Volatize(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission,
		double *CO2Emission)
{
	double Nlost, Clost;
	manurestore::Volatize(&Nlost, &Clost,volatAmmonia, N2Emission, N2OEmission, CH4Emission, CO2Emission);
	Nbudget.AddOutput(Nlost);
	Cbudget.AddOutput(Clost);
}

/**
 * Volatize nitrogen gasses from the floor - used for dynamic modelling
 */
void instore::floorVolatize(double airVelocity, double humidity, double temperature, double areaSoiled, double duration,
		double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission)
{
	//   EndBudget(false);
	*N2Emission=0.0;
	*N2OEmission=0.0;
	*volatAmmonia=0.0;
	duration*=24.0; //convert from days to hours
	manure * aManure=(manure *) theStore->GetProductPointer();
	double resistance = 1/(velocityConstant * pow(airVelocity+0.02,0.8) * pow(temperature+273,-1.4)); //Aarninck model from 1997
	//   double resistance = 1/(50.1 * pow(airVelocity,0.8) * pow(temperature+273,-1.4)); //Aarninck model from 1997
	if (areaSoiled>surfaceArea)
		areaSoiled=surfaceArea;
	if (areaSoiled>0.0)
	{
		double evaporation = aManure->GetWaterEvap(temperature, humidity, areaSoiled, resistance, duration); //total evap in tonnes during 'duration'
		evaporation*= 1000.0/(areaSoiled * duration);  //convert to kg per sq metre per sec
		*volatAmmonia = aManure->InstantaneousVolatAmmonia(temperature, areaSoiled, resistance, duration);
		aManure->doDenitrification(temperature, duration/24.0, N2Emission,N2OEmission);
		// denitrification losses are here removed after ammonia volatilisation - this needs to be integrated later??
		//	 aManure->GetCgasEmissions(temperature, duration, CO2Emission, CH4Emission);
		aManure->SubtractWater(evaporation * areaSoiled * duration/1000.0);

	}
	Nbudget.AddOutput(*volatAmmonia + *N2Emission + *N2OEmission);
	Cbudget.AddOutput(*CO2Emission + *CH4Emission);
	//   EndBudget(false);
}

/**
 * Volatize nitrogen gasses from the inhouse manure store - used for dynamic modelling
 */

void instore::inhouseVolatize(double airVelocity, double humidity, double temperature, double duration,
		double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission)
{
	if (airVelocity>0.0)
	{
		duration*=24.0; //convert from days to hour
		manure * aManure=(manure *) theStore->GetProductPointer();
		if (aManure->GetAmount()>0)
		{
			double resistance = 1/(velocityConstant * pow(airVelocity,0.8) * pow(temperature+273,-1.4)); //Aarninck model from 1997
			double evaporation = aManure->GetWaterEvap(temperature, humidity, surfaceArea, resistance, duration);
			double infiltration = 0;
			double precipitation = 0;
			*volatAmmonia=aManure->VolatAmmonia(temperature, evaporation, infiltration, precipitation, surfaceArea, resistance, duration,
					&evaporation, NULL);
			//         aManure->GetCgasEmissions(temperature,  duration, CH4Emission, CO2Emission);
			Nbudget.AddOutput(*volatAmmonia + *N2Emission + * N2OEmission);
			Cbudget.AddOutput(*CH4Emission + *CO2Emission);
		}
	}
}


/**
 * Scale instore to number of animals present, pass info to next store. Note cumulative effect on capacity
 */

void instore::ScaleInstore(double annualAmount)
{
	scalable=true;
	double newCapacity = annualAmount + GetCapacity();
	Scale(newCapacity);  //scales capcity and area via manurestore
	//   ActualCapacity = annualAmount;
	nextStore->Scale(annualAmount);
}

/****************************************************************************\
	Operator << for output
\****************************************************************************/

ostream& operator<< (ostream& os,const instore& s)
{
	//os <<	"air velocity const " << s.velocityConstant << endl;
	return os;
}
