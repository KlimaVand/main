//!Describes the flooring in an animal section
/*!Describes the flooring in an animal section. Mainly used to manage the flow of manure and water.
 * */
#ifndef FLOOR_STORAGE_H
#define FLOOR_STORAGE_H

#include "../products/manure.h"
#include "instore.h"
#include "../base/budget.h"
#include "../base/linklist.h"
class floorstore : public base
{

private:
	//!Contained two stores, one for liquid, one for solids
	linkList<instore> * theOnFloorStores;
	//!Contained zero to two pointers to manure stores that will receive manure when the floor is cleaned
	linkList<manurestore> * theNextStores;
	//!proportion of excreta that is deposited on this floor
	double excretaPartition;
	//!Name of the manure store
	string manuretype;
	//!Alias of the manure store
	string Alias;
	//! budgets
	budget Nbudget, Cbudget;
	//! area of flooring
	double area;
	//!Interval between cleaning in days
	double cleaningInterval;
	//! index identifier for manure storage that will receive manure when floor is cleaned
	int ManureToID;
	//! Name of the manure store that will receive manure when floor is cleaned  
	string manureToName;
	//!Number of days since the floor was last cleaned
	double timeUncleaned;
	//!Washing water input per animal per day (kg)
	double WashingWater;
	//!amount of straw added per animal per day
	double StrawAdded;
	//!area of urine in sq metres per tonne urine
	double volumeToArea;
	//!True if the flooring is ventilated
	bool ventilated;
	//!Ammonia emission factor used in static ammonia emissions modelling
	double NH3Emissionfactor;
	   double N2Emissionfactor;
	   double N2OEmissionfactor;
	   double CO2Emissionfactor;
	   double CH4Emissionfactor;
	//!proportion of floor area fouled by manure
	double proportionFouled;
	//! Proportion of the area of an animal section that is occupied by this floor
	double AreaPartition;
	//! default constructor
	floorstore();

protected:

public:

public:
	//! Constructor with arguments
	floorstore(char *aName,const int aIndex,const base* aOwner,commonData * &runData,commonData * &common,int numberOfFloors);
//	linkList<instore> *GetStoreClone();
	//! Copy constructor
	floorstore(floorstore& s);
	//! Destructor
	~floorstore();
	//!Set the area of the floor (square meters)
	void SetArea(double aVal){area=aVal;};
	//!Returns the proportion of the excreta deposited in the animal section that is partitioned to this floor
	virtual double GetexcretaPartition() const {return excretaPartition;};
	//!Returns the number of stores on the floor
	virtual int GetOnfloorStoreNumber() const {return theOnFloorStores->NumOfNodes();};
		//!Returns the number of nextstores on the floor
	virtual int GetNextStoreNumber() const {return theNextStores->NumOfNodes();};
	//!Returns a pointer to given the logical storage number
	/*!
	 * \param aStoreNumber the logical number of the storage that is of interest
	 * */
	virtual manurestore * GetaStore(int aStoreNumber) const {return theOnFloorStores->ElementAtNumber(aStoreNumber);};
	virtual manurestore * GetaNextStore(int aStoreNumber) const {return theNextStores->ElementAtNumber(aStoreNumber);};
	
	//!Returned the cleaning interval in days
	virtual double GetcleaningInterval() {return cleaningInterval;};
	//!Return time since last cleaned in days
	virtual double GettimeUncleaned() {return timeUncleaned;};
	//!Returned the amount of Straw added  in kilograms per day
	virtual double GetStrawAdded() {return StrawAdded;};
	//!Returned the area of the floor
	virtual double GetArea() {return area;};
	//!Returned the amount of washing water in litres per day
	virtual double GetWashingWater() {return WashingWater;};
	//!Return the proportion of the section that is covered by this floor
	virtual double GetAreaPartition(){return AreaPartition;}
	//!Set the the time since cleaned in days
	virtual void SettimeUncleaned(double aTime) {timeUncleaned=aTime;};
	//! Return index identifier for manure storage that will receive manure when floor is cleaned
	virtual int GetManureToID(){return ManureToID;}
	//! Return name of the manure store that will receive manure when floor is cleaned  
	virtual string GetmanureToName(){return manureToName;}

	//!daily update routine
	virtual void DailyUpdate();

	//!Dynamic modelling of the gaseous losses from the floor storage
	/*!
	 * \param airVelocity air velocity across the floor surface in metres per second
	 * \param waterEvapThisFloor evaporation of water in kilograms
	 * \param temperature temperature of the air passing of the floor surface in degrees Celsius
	 * \param volatAmmonia ammonia volatilised in kilograms
	 * \param N2Emission emission of nitrogen gas in kilograms
	 * \param N2OEmission emission of nitrous oxide in kilograms
	 * \param CH4Emission emission of methane in kilograms
	 * \param CO2Emission emission of carbon dioxide in kilograms
	 * */

	virtual void Volatize(double airVelocity, double waterEvapThisFloor, double temperature,
			double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission);
	//!Static modelling of gaseous losses from floor storage
	/*!
	 * \param volatAmmonia ammonia volatilised in kilograms
	 * \param N2Emission emission of nitrogen gas in kilograms
	 * \param N2OEmission emission of nitrous oxide in kilograms
	 * \param CH4Emission emission of methane in kilograms
	 * \param CO2Emission emission of carbon dioxide in kilograms
	 * */
	virtual void Volatize(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission,
			double *CO2Emission);
	// 	virtual double Volatize(manure *aManure);
	//! Receives manure
	/*!
	 * \param aUrine the urine to be added to the floor storage
	 * \param aFaeces the faeces to be added to the floor storage
	 * */
	virtual void RcvManure(manure* aUrine,manure* aFaeces);
	/*!
	 * \param aBeddingMaterial the bedding to be added to the floor storage
	 * */
	virtual void RcvBedding(plantItem *aBeddingMaterial);
	//!Clean the floor, sending manure to storage
	/*!
	 * Cleans the floor, with the amount of washing water used depending on the number of animals present
	 * \param numberOfAnimals number of animals present
	 * */
	virtual manure * CleanFloor(double numberOfAnimals);
	//!Return the amount of manure in the floor store in tonnes
	virtual double GetManureAmount();
	virtual manure * GetManure();
	//!Return the amount of nitrogen stored in kilograms
	virtual double GetTotalNStored();
	//!Return the amount of carbon stored in kilograms
	virtual double GetTotalCStored();
	//!Returned the amount of dry matter stored in kilograms
	virtual double GetTotalDMStored();

	//!Reads initialisation data, text file

	//!Return an amount of manure from the floor storage
	/*!
	 * Returns the amount that is requested or the amount present, whichever is the lesser
	 * \param RequestedAmount the amount requested
	 * \param p the manure type requested
	 * \param checkAccess set true if wishing to check the renewal is accessible
	 * */
	virtual product * GetFromManureStore(double RequestedAmount, product * p, bool checkAccess = false);

	//!Empties all manure from the floor storage
	/*!
	 * \param checkAccess set true if wishing to check the renewal is accessible
	 * */
	product * TakeAllManureFromFloor(bool checkAccess);
	//!budget function
	virtual void EndBudget(bool show);
	//!Scales the floor storage to a number of animals present
	/*!
	 * \param numberOfAnimals number of animals present
	 * \param annualManure total annual deposition of animal manure to this floor
	 * */
	virtual void ScaleSize(double annualManure);

	friend ostream& operator <<(ostream& os,const floorstore& s);

	virtual string getmanureName();
	/*!
	 * \param aManureStore points to a manurestore, that, if have the right name and id, will be set in theNextStore
	 */
	void LinkManureStores(manurestore * aManureStore);
};

/****************************************************************************\
 End of Class floorstore
\****************************************************************************/



#endif
