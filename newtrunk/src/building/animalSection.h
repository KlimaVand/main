// ! Section of an animal house that contains livestock

/*
 * ! This class described as a part of the animal housing that contains livestock.  Such a section
 * would contain one or two floor types and would normally contain one type of livestock.
 */

/*
 *
 * Created by NJH 3/11/99
 *
 */

#ifndef __ANIMSEC_H
#define __ANIMSEC_H

#include "floorstore.h"
#include "../base/bstime.h"

class animalSection:
    public base
{
    protected:

        // !Number of animals present
        double NrOfAnimals;

        // ! Area of section per animal housed
        double AreaPrAnimal;

        // !Maximum number of places for animals
        double Places;

        // !water input per animal per day (kg)
        double SpiltDrinkingWater;

        // !kW per animal per year
        double ElectricityUse;

        // !total input of faeces to this section in a year (kg)
        double faecesPerYear;

        // !total input of urine to this section in a year (kg)
        double urinePerYear;

        // ! minimum ventilation rate in cubic metres per sec per animal
        double minVentilation;

        // ! maximum vnetilation rate in cubic metres per sec per animal
        double maxVentilation;
        string Alias;

        // !name of animal housed
        string Contains;

        // !international livestock units in section
		double IntLivestockUnits;

    public:
        linkList<floorstore> * flooring;

        // !  Nitrogen budget class
        budget Nbudget,

        // ! carbon budget last
        Cbudget;

        // ! Total the production of the livestock present
        double animalHeatOp;

        // ! true if the section is in use
        bool inUse;

    public:
        animalSection(string        aName,
                      const int     aIndex,
                      const base *  aOwner,
                      commonData *& runData,
                      commonData *& common,
                      bool          controlledVent);

        virtual ~animalSection();

        animalSection(const animalSection & aAnimalSection);

        // !set number of animals
        void setNrOfAnimals(double aNr) {
            NrOfAnimals = aNr;
        }

        // ! Set the production by animals
        virtual void SetanimalHeatOp(double aValue) {
            animalHeatOp = aValue;
        }

        // ! Set annual faeces production by animals in kilograms fresh weight
        virtual void SetfaecesPerYear(double someFaeces) {
            faecesPerYear = someFaeces;
        }

        // ! Set annual murine production by animals in kilograms fresh weight
        virtual void SeturinePerYear(double someUrine) {
            urinePerYear = someUrine;
        }

        // ! Return number of animals present
        double GetNrOfAnimals() {
            return NrOfAnimals;
        }

        // ! Return number of places available
        double GetPlaces() {
            return Places;
        }

        // ! Returned name of animal section
        // ! Return name of animals housed in section
        virtual string GetContains() {
            return Contains;
        }

        // ! return minimum ventilation rate
        double GetminVentilation() {
            return minVentilation;
        }

        // ! return maximum ventilation rate
        double GetmaxVentilation() {
            return maxVentilation;
        }

        // ! Return amount of manure stored within the section
        double GetManureAmount();

        // !Return day the electricity use
        virtual double GetDailyElectricityUse() {
            return NrOfAnimals * ElectricityUse / theTime.daysInYear(0);
        }

        // !Return total into production by animals that
        virtual double GetanimalHeatOp() {
            return animalHeatOp;
        }

        // !Function for dynamically calculating ammonia volatilisation and other gaseous losses

        /*
         * !
         * param airVelocity air velocity in the animal section (metres per second)
         * param waterEvapThisSection evaporation of water (kg/ time period)
         * param temperature temperature of the surface the floor (Celsius)
         * param volatAmmonia points to amount of NH3-N that have emitted (kg/time period)
         * param N2Emission points to amount of N2-N that have emitted (kg/time period)
         * param N2OEmission points to amount of N2O-N that have emitted (kg/time period)
         * param CO2Emission points to amount of CO2 that have emitted (kg/time period)
         * param CH4Emission points to amount of CH4 that have emitted (kg/time period)
         */
        virtual void Volatize(double   airVelocity,
                              double   waterEvapThisSection,
                              double   temperature,
                              double * volatAmmonia,
                              double * N2Emission,
                              double * N2OEmission,
                              double * CH4Emission,
                              double * CO2Emission);

        // !Function of statically calculating ammonia volatilisation and other gaseous losses
        virtual void Volatize(double * volatAmmonia,
                              double * N2Emission,
                              double * N2OEmission,
                              double * CH4Emission,
                              double * CO2Emission);

        // !Clean manure from section. Creates an instance of manure class and returns a pointer

        /*
         * !
         * param forceIt is set to true to force cleaning
         */
        virtual manure * CleanSection(string forceIt);

        // !Received manure from somewhere else

        /*
         * !
         * param aUrine contains amount of urine
         * param aFaeces contains an amount of faeces
         */
        virtual void RcvManure(manure * aUrine,
                               manure * aFaeces);

        // !Links manure storage from section to recipient storage
        virtual void LinkManureStores(manurestore * aManureStore);

        // !Checks to see if or manure stores have a recipient
        virtual void CheckLinks();

        // !Checks if nutrient budgets are closed
        virtual void EndBudget(bool show = false);

        // !Returned the amount of nitrogen stored
        virtual double GetStoredN();

        // !Return amount of carbon stored
        virtual double GetStoredC();

        // !Return amount of dry matter of stored
        virtual double GetStoredDM();

        // !Return amount of nitrogen input to the animal section so far
        virtual double GetNInput() {
            return Nbudget.GetInput();
        }

        // !Return the amount of nitrogen output from the animal section so far
        virtual double GetNOutput() {
            return Nbudget.GetOutput();
        }

        // !Requests an amount of manure from storage, Creates an instance of product class and returns a pointer

        /*
         * !
         * param RequestedAmount is the wet weight of manure requested
         * param p points to the type of manure requested
         * param checkAccess is set true if one wants to check to see if the manure is accessible
         */
        virtual product * GetFromManureStore(double RequestedAmount,
                product *                           p,
                bool                                checkAccess = false);

        // !Takes all the manure from a store. Creates an instance of product class and returns a pointer

        /*
         * !
         * param p points to the type of manure requested
         * param checkAccess is set true if one wants to check to see if the manure is accessible
         */
        virtual product * TakeAllManureFromSection(bool checkAccess);

        // !Daily update function
        void DailyUpdate();

        // !Scaled the animal section to the number of animals present
        virtual void ScaleSize();

        virtual double GetTotalArea();

        // !Add amount to animal heat production
        virtual void AddanimalHeatOp(double aValue) {
            animalHeatOp += aValue;
        }

        // !Initialise animal parameters
        virtual void SetAnimalParameters(double numberOfAnimals,
                                         double faecesPerYear,
                                         double urinePerYear,
                                         double heatOP = 0.0);

        // virtual void CheckFloorIntegrity();
        bool GetinUse() {
            return inUse;
        }

        void setinUse(bool aVal) {
            inUse = aVal;
        }

		virtual void AddIntLivestockUnits(double aVal)
		{
			IntLivestockUnits+=aVal;
		}

		virtual double GetIntLivestockUnits()
		{
			return IntLivestockUnits;
		}

		virtual void ZeroIntLivestockUnits()
		{
			IntLivestockUnits=0.0;
		}

};
#endif

