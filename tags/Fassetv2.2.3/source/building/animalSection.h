/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
//! Section of an animal house that contains livestock
/*! This class described as a part of the animal housing that contains livestock.  Such a section
 * would contain one or two floor types and would normally contain one type of livestock.
 * */

/****************************************************************************\
Created by NJH 3/11/99
\****************************************************************************/

#ifndef __ANIMSEC_H
	#define __ANIMSEC_H

#include "floorstore.h"
#include "../economy/genAsset.h"
#include "../base/bstime.h"
class animalSection:public base, public GenAsset
{
protected:
	//!Number of animals present
   double NrOfAnimals;
   //!Maximum number of places for animals
   double Places;
   //!water input per animal per day (kg)
   double SpiltDrinkingWater;
   //!kW per animal per year
   double ElectricityUse;
   //!total input of faeces to this section in a year (estimated on startup)
   double faecesPerYear;
   double urinePerYear;
   string Alias;
   //!name of animal housed
   string Contains;
   double VariableFeeding;
   double VariableCleaning;
   double ConstantFeeding;
   double ConstantCleaning;
   //!Pointer to a list of objects describing the flooring in the animal section
   linkList<floorstore> *flooring;
   //!  Nitrogen budget class
   budget Nbudget,
   //! carbon budget last
   Cbudget;
   //! Total the production of the livestock present
   double animalHeatOp;

	public:
		//! Constructor with arguments
      animalSection(char* aName,const int aIndex,const base* aOwner);
      //! Destructor
	   virtual ~animalSection();

   //!set number of animals
	void setNrOfAnimals(double aNr) {NrOfAnimals=aNr;};
	//! Set the production by animals
   virtual void SetanimalHeatOp(double aValue){animalHeatOp=aValue;};
   //! Set annual faeces production by animals in kilograms fresh weight
   virtual void SetfaecesPerYear(double someFaeces) {faecesPerYear = someFaeces;}
   //! Said annual murine production by animals in kilograms fresh weight
   virtual void SeturinePerYear(double someUrine) {urinePerYear = someUrine;}
   //! Return number of animals present
	double GetNrOfAnimals() {return NrOfAnimals;};
	//! Return area used by animals
	double GetAreaUsed() {return GetAreaPrAnimal()*GetNrOfAnimals();};
	//! Return number of places available
   bool GetPlaces() {return Places;};
   //! Returned name of animal section
   virtual string GetAlias(){return Alias;};
   //! Return name of animals housed in section
   virtual string GetContains(){return Contains;};
   //! Return amount of manure stored within the section
   double GetManureAmount();
   //!Returned total amount of electricity used
   virtual double GetElectricityUse() {return ElectricityUse;}
   //!Return day the electricity use
   virtual double GetDailyElectricityUse() {return NrOfAnimals * ElectricityUse/theTime.daysInYear(0);}
   //!Return total into production by animals that
   virtual double GetanimalHeatOp(){return animalHeatOp;};
   //!Function for reading input parameters from textile
	virtual void ReadParameters(fstream* file);
	//!Function of dynamically calculating ammonia volatilisation and other gaseous losses
   virtual void Volatize(double airVelocity, double humidity, double temperature,
							double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission);
	//!Function of statically calculating ammonia volatilisation and other gaseous losses
	virtual void Volatize(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission,  double *CO2Emission);
	//!Clean manure from section
	/*!
	 * \param forceIt is set to true to force cleaning
	 * */
   virtual manure * CleanSection(string forceIt);
   //!Received manure from somewhere else
   /*!
    * \param aUrine contains amount of urine
    * \param aFaeces contains an amount of faeces
    * */
	virtual void RcvManure(manure* aUrine,manure* aFaeces);
	//!Links manure storage from section to recipient storage
	virtual void LinkManureStores(manurestore * aManureStore);
	//!Checks to see if or manure stores have a recipient
	virtual void CheckLinks();
	//!Checks if nutrient budgets are closed
   virtual void EndBudget(bool show = false);
   //!Returned the amount of nitrogen stored
   virtual double GetStoredN();
   //!Return amount of carbon stored
   virtual double GetStoredC();
   //!Return amount of dry matter of stored
   virtual double GetStoredDM();
   //!Return amount of nitrogen input to the animal section so far
	virtual double GetNInput() {return Nbudget.GetInput();}
	//!Return the amount of nitrogen output from the animal section so far
	virtual double GetNOutput() {return Nbudget.GetOutput();}
	//!Requests an amount of manure from storage
	/*!
	 * \param RequestedAmount is the wet weight of manure requested
	 * \param p points to the type of manure requested
	 * \param checkAccess is set true if one wants to check to see if the manure is accessible
	 * */
	virtual product * GetFromManureStore(double RequestedAmount, product * p, bool checkAccess = false);
	//!Daily update function
	void DailyUpdate();
	//!Scaled the animal section to the number of animals present
	virtual void ScaleSize();
	//!Return the area in square metres per animal
   virtual double GetAreaPrAnimal();
   //!Returned the total area of the animal section
   virtual double GetTotalArea();
   //!Return Labour used
   virtual double GetLabour(){return 0.0;};
   //!Add amount to animal heat production
   virtual void AddanimalHeatOp(double aValue){animalHeatOp+=aValue;};
   //!Initialise animal parameters
	virtual void SetAnimalParameters(double numberOfAnimals,double faecesPerYear, double urinePerYear, double heatOP=0.0);
	//!Streaming operator
   friend ostream& operator <<(ostream& os,const animalSection& s);

};

#endif
