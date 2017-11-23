// Class: pig                  //Borland C++ 5.02 - FASSET style
#define NT     //stops FASSET files from redefining Boolean

#ifndef __pig_H  //Required for current class
#define __pig_H

#include "../base/linklist.h"
#include "../base/common.h"
#include "animal.h" //Required for base class
#include "../building/stable.h" //Required for include files
#include "../products/feedItem.h" //Required for include files
#include "../products/manure.h" //Required for include files
#include "../building/pigstysection.h"
#include "../base/commonData.h"

class pig:
    public animal
{
    protected:
        double              NumberPrDay;
        double              NumberPrYear;
        double              aveWeight;
        linkList<product> * feedItemList;

        pig() {
            NumberPrDay  = NumberPrYear = aveWeight = 0.0;
            feedItemList = nullptr;
        }

        // :animal();

    public:

        // Constructor setting attributes to default values
        pig(string       aName,
            const int    aIndex,
            const base * aOwner);

        virtual ~pig();    // Destructor

        // Get and set functions for attributes
        double GetNumberPrDay() const {
            return NumberPrDay;
        }

        double GetNumberPrYear() const {
            return NumberPrYear;
        }

        void SetNumberPrDay(double aNumberPrDay) {
            NumberPrDay = aNumberPrDay;
        }

        void SetNumberPrYear(double aNumberPrYear) {
            NumberPrYear = aNumberPrYear;
        }

        void SetaveWeight(double aVal) {
            aveWeight = aVal;
        }

        double GetaveWeight() {
            return aveWeight;
        }

        virtual double GetDE() {
            return NumberPrYear * LivestockUnits;
        }

        // overload animal class functions
        // virtual void SetanimalSection (pigstysection* aanimalSection){ animalSection = aanimalSection;}
        virtual pigstysection * GetanimalSection() {
            return (pigstysection *) theanimalSection;
        }

        // void SetdailyWeightGain(double adailyWeightGain) {dailyWeightGain = adailyWeightGain;}
        // user created functions
        virtual void ReadParameters(commonData *& file);

        virtual void DailyUpdate();

        void ReceivePlan(commonData * data,
                         string       fileName);

        double ProduceManure(manure * fluidManure,
                             manure * solidManure);

        // GAMs functions
        virtual void EndBudget(bool show = false);

        virtual double GetSensibleHeatProduction(double weight) {
            theMessage -> FatalError("pig::GetSensibleHeatProduction should always be overridden");

            return 0.0;
        }

        virtual double CalcMaintenanceEnergy(double weight) {
            theMessage -> FatalError("pig::CalcMaintenanceEnergy should always be overridden");

            return 0.0;
        }

        // !return pig heat production in Watts (not currently used)
        double CalcHeatProduction(double weight);

        // !return average pig heat production in Watts (not currently used)
        double CalcAverageHeatProduction(double startWeight,
                                         double endWeight);

        virtual double GetTotalHeatProduction(double weight) {
            theMessage -> FatalError("pig::GetTotalHeatProduction should always be overridden");

            return 0.0;
        }
};
#endif

