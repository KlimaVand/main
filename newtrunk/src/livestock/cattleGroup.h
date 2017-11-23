/*
 *
 * NJH
 *
 */

#ifndef CATTLEGRP_H
#define CATTLEGRP_H

#include "dkCattle.h"

class cattleGroup:
    public base
{

    /* Attributes */
    protected:
        typedef cattle *   cattleptr;
        linkList<cattle> * group;
        string             Alias;
        double             LivestockUnits;
        int                numberOfCohorts;     // number of instances of cattle per group
        double             numberPerCohort;     // number of animals each cohort simulates (1 = individual modelling)
        double             maxAge;              // age at which animals move to the next group (in years)
        double             minAge;              // age at which animals move into the group (in years)
        int                code;
        bool               areFemale;
        bool               areYoung;            // true if animals are young (directs info flow in to indicators)
        int                animalModel;
        cloneList<diet> *  feedPlanList;        // contains list of the feed items available to the animals in the group
        diet *             theCurrentDiet;      // holds current menu - needed when buying cattle
        animalSection *    theanimalSection;    // holds pointer to relevant animal section in housing

        /* Actions */

    private:
        cattleGroup & operator = (
                const cattleGroup & a);    // Dissable the compilers generation of default assignment operator.

    public:
        linkList<cattle> * GetGroupClone();

        cloneList<diet> * GetFeedPlanListClone();

        cattleGroup(const cattleGroup & a);    // Dissable the compilers generation of default copy constructor.

        // Constructor

    private:
        commonData * data;

        cattleGroup() {
            minAge           = maxAge = 0;
            areFemale        = areYoung = false;
            numberOfCohorts  = numberPerCohort = 0;
            animalModel      = code = 0;
            LivestockUnits   = 0;
            group            = nullptr;
            feedPlanList     = nullptr;
            theanimalSection = nullptr;
            theCurrentDiet   = nullptr;
            data             = nullptr;
        }

    public:
        cattleGroup(string       aName,
                    const int    aIndex,
                    const base * aOwner);

        virtual ~cattleGroup();

        void SetnumberOfCohorts(int aNumber) {
            numberOfCohorts = aNumber;
        }
		int getMinAge()
		{
			return minAge;
		}
        void SetnumberPerCohort(double aNumber) {
            numberPerCohort = aNumber;
        }

        void SetanimalSection(animalSection * aanimalSection) {
            theanimalSection = aanimalSection;
        }

        void SetanimalModel(int aModel) {
            animalModel = aModel;
        }

        int GetnumberOfCohorts() {
            return numberOfCohorts;
        }

        int GetnumberInstances() {
            return group -> NumOfNodes();
        }

        double GetnumberOfAnimals();

        cattleptr GetGroupMember(int elementNum) {
            return group -> ElementAtNumber(elementNum);
        }

        virtual string GetAlias() {
            return Alias;
        }

        double GetmaxAge() const {
            return maxAge;
        }

        double GetminAge() const {
            return minAge;
        }

        bool GetareFemale() const {
            return areFemale;
        }

        animalSection * GetanimalSection() {
            return theanimalSection;
        }

        virtual void ReadParameters(commonData *& file);

        void AddAnimal(cattle * anAnimal);

        void checkBalances(bool show = false);

        void DailyUpdate(void);

        void CheckBirth();

        void CheckAge(linkList<cattle> * oldies);

        void SeedGroup(double startAge,
                       double endAge,
                       int *  IDnumber);

        void CullAnimals(int numberToCull);

        bool CullAnimalFromGroup(cattle * theVictim);

        void CullAnimal(cattle * theVictim,
                        bool     deleteAnimal);

        void AutoRecruit(linkList<cattle> * recruitmentList,
                         int *              IDnumber);

        double GetCattleGroupN();

        double GetCattleGroupC();

        void CheckWean(linkList<cattle> * calves,
                       bool               stopLactation);

        double GetDE();

        void GiveIndicator(int indicatorType);

        // void FlushIndicator(int indicatorType);
        cattle * buyNewAnimal(double currentAge,
                              int *  IDnumber,
                              bool   logit);
};
#endif
