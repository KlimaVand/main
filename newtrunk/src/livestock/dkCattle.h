
#ifndef __dk_cattle_H  //Required for current class
#define __dk_cattle_H

#include "../base/cloneList.h"
#include "../base/common.h"
#include "cattle.h"
#include "../products/feedResource.h"
#include "../products/manure.h"
#include "diet.h"

#ifndef _STANDALONE


#include "../building/stable.h"

#endif

using namespace std;

class dk_cattle:
    public cattle
{
    protected:

        // ***********************
        double herdMilkYieldConstant;    // bes�tningsydelsen kg EKM  (Ykap)
        double q0, q1;    // coefficients that determine the remobilisation of body tissue to support milk production
        double lactationEnergyFudgeFactor;

        // ***********************
        virtual void intake();

        virtual double GetbulkCapacity();

        virtual double calc_energy_maint();

        virtual void Partition();

        double GetPotentialMilkYield(int daysIntoLactation);

        virtual double DoPregnancy(double * avail_ME,
                                   double * availableN);

        virtual void DoLactation(double * avail_FE,
                                 double * availableN,
                                 double * milkN);

        double SucklingCalfMilkIntake() {
            return 0.0;
        }

        double GetavailableFodderUnits();

        double CalcFoetusWeight(double timeFromConception);

        void Createfoetus();

        double GetSFUPerKgGain();

        double GetEnergyLimitedGrowth(double * energyForGrowth);

        dk_cattle * CheckBirth();

        void DoBirth(double * afterBirthProtein,
                     double * afterBirthFat);

    public:

        // Constructor setting attributes to default values
        dk_cattle(string       aName,
                  const int    aIndex,
                  const base * aOwner);

        dk_cattle(const dk_cattle & anAnimal);
		dk_cattle(const cattle & anAnimal);
#ifdef _STANDALONE
        dk_cattle();
#endif

        virtual ~dk_cattle();    // Destructor

        virtual void ReadParameters(commonData *& data);

        virtual void DailyUpdate();

        void MakePregnant(bool isAFemale);

        // ***********************
        double GetSize();

        double GetPotentialGrowth();

        double GetPotentialIntake(double fillPerSFU);

        double GetMilkYieldCapacity();

        double Get_norm_weight();

        double Getfill(feed_resource * aFeed_resource);

        double GetfillperSFU(feed_resource * aFeed_resource);

        double Getweight() const {
            return weight;
        }

        double NinEBWchange();

        void calculateFill(feedItem * aFeedItem);

        double CalcFoetusProtein(double offset);

        double CalcGravidUterusProtein(double offset);

        double GetFoetusWeight(double offset);

        double GetGravidUterusWeight(double offset);

#ifdef _STANDALONE
        virtual void Initialise(double anAge,
                                int    thedaysFromConception,
                                int    thelactationNumber);
#endif

        bool GetAmountNextFeed(feed_resource * aFeed_resource);

        double CalcFatInEBWchange(double energy,
                                  double protein);

        void GrowGravidUterus(double newProtein,
                              double newFat);

        void InitialiseFoetus();

        void CalcHeatProduction(manure * urine);

        double GetPalatability(feed_resource * aFeed_resource);

        void ChangeBodyComposition(double   weightChange,
                                   double * avail_FE,
                                   double * availableN);

        double GetPotentialMilkSolidsIntake(int days);

        double GetPotentialAnnualMilkProduction(int durationOfLactation);

        double GetEnergyRequirement();

        double GetSFUsPerKgEKM() {
            return 0.4;
        }

        // cy
        double GetFoetusEnergyRequirement();

        double GetFoetusPorteinRequirement();

        double GetProteinRequirement();

        double GetPotentialMilkIntake();

        double GetCalfSolidIntakeFraction();
};
#endif

