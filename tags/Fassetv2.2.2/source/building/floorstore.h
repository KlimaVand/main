/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
//!Describes the flooring in an animal section
/*!Describes the flooring in an animal section. Mainly used to manage the flow of manure and water.
 * */
#ifndef FLOOR_STORAGE_H
  #define FLOOR_STORAGE_H

#include "../products/manure.h"
#include "instore.h"
#include "../base/budget.h"

class floorstore : public base
{

private:
	//!Contained two stores, one for liquid, one for solids
   linkList<instore> * theStores;
   //!proportion of excreta that is deposited on this floor
   double excretaPartition;
   //!Name of the manure store
   string manuretype;
   //! budgets
	budget Nbudget, Cbudget;
	//! area of flooring
   double area;
   //!Interval between cleaning in days
   double cleaningInterval;
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
   //!surface area per animal - used if manure store is to be scaled to animal numbers present
   double AreaPrAnimal;
   //!proportion of floor area fouled by manure
   double proportionFouled;
	//! default constructor
   floorstore();

protected:

public:

public:
   //! Constructor with arguments
   floorstore(char *aName,const int aIndex,const base* aOwner);
   //! Copy constructor
   floorstore(floorstore& s);
   //! Destructor
   ~floorstore();
   //!Returns the proportion of the excreta deposited in the animal section that is partitioned to this floor
    virtual double GetexcretaPartition() const {return excretaPartition;};
    //!Returns the number of stores on the floor
   virtual int GetStoreNumber() const {return theStores->NumOfNodes();};
   //!Returns a pointer to given the logical storage number
   /*!
    * \param aStoreNumber the logical number of the storage that is of interest
    * */
   virtual manurestore * GetaStore(int aStoreNumber) const {return theStores->ElementAtNumber(aStoreNumber);};
   //!Returned the cleaning interval in days
   virtual double GetcleaningInterval() {return cleaningInterval;};
   //!Return time since last cleaned in days
   virtual double GettimeUncleaned() {return timeUncleaned;};
   //!Returned the surface area per animal - used if manure store is to be scaled to animal numbers present
   virtual double GetAreaPrAnimal() {return AreaPrAnimal;};
   //!Returned the amount of Straw added  in kilograms per day
   virtual double GetStrawAdded() {return StrawAdded;};
   //!Returned the area of the floor
   virtual double GetArea() {return area;};
   //!Returned the amount of washing water in litres per day
	virtual double GetWashingWater() {return WashingWater;};
	//!Set the the time since cleaned in days
   virtual void SettimeUncleaned(double aTime) {timeUncleaned=aTime;};
   //!daily update routine
   virtual void DailyUpdate();
   //!Calculate indicators for floor storage
   virtual void GiveIndicator();
   //!Dynamic modelling of the gaseous losses from the floor storage
   /*!
    * \param airVelocity air velocity across the floor surface in metres per second
    * \param humidity relative humidity of the air passing over the floor surface
    * \param temperature temperature of the air passing of the floor surface in degrees Celsius
    * \param volatAmmonia ammonia volatilised in kilograms
    * \param waterEvap evaporation of water in kilograms
    * \param N2Emission emission of nitrogen gas in kilograms
    * \param N2OEmission emission of nitrous oxide in kilograms
    * \param CH4Emission emission of methane in kilograms
    * \param CO2Emission emission of carbon dioxide in kilograms
    * */

	virtual void Volatize(double airVelocity, double humidity, double temperature,
			double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission);
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
	 * \param aManure the manure to be added to the floor storage
	 * */
	virtual void RcvManure(manure *aManure);
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
   //!Return the amount of nitrogen stored in kilograms
   virtual double GetTotalNStored();
   //!Return the amount of carbon stored in kilograms
   virtual double GetTotalCStored();
   //!Returned the amount of dry matter stored in kilograms
   virtual double GetTotalDMStored();
   //!probably obsolete
	virtual void CalcLP();
	//!Reads initialisation data, text file
   virtual void ReadParameters(fstream * file);
   //!Return an amount of manure from the floor storage
   /*!
    * Returns the amount that is requested or the amount present, whichever is the lesser
    * \param RequestedAmount the amount requested
    * \param p the manure type requested
    * \param checkAccess set true if wishing to check the renewal is accessible
    * */
   virtual product * GetFromManureStore(double RequestedAmount, product * p, bool checkAccess = false);
   //!budget function
	virtual void EndBudget(bool show);
	//!Scales the floor storage to a number of animals present
	/*!
	 * \param numberOfAnimals number of animals present
	 * \param annualManure total annual deposition of animal manure to this floor
	 * */
	virtual void ScaleSize(double numberOfAnimals, double annualManure);
	//!Outputs streaming
   friend ostream& operator <<(ostream& os,const floorstore& s);
};

/****************************************************************************\
 End of Class slurrystore
\****************************************************************************/



#endif
