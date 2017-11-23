/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 NJH
\****************************************************************************/

#ifndef CATTLEGRP_H
  #define CATTLEGRP_H


#include "dkCattle.h"

/****************************************************************************\
\****************************************************************************/

class cattleGroup  : public base
{
   /* Attributes */
   protected:
//      typedef cattle * cattleptr;
	//	linkList <cattle> * group;
      typedef cattle * cattleptr;
		linkList <cattle> * group;
		string Alias;
      double LivestockUnits;
	   int numberOfCohorts;   //number of instances of cattle per group
      double numberPerCohort; // number of animals each cohort simulates (1 = individual modelling)
      double maxAge;	// age at which animals move to the next group (in years)
      double minAge;	// age at which animals move into the group (in years)
      int code;
      bool areFemale;
      bool areYoung;		//true if animals are young (directs info flow in to indicators)
      int animalModel;
		cloneList <diet> *feedPlanList;   //contains list of the feed items available to the animals in the group
 		diet * theCurrentDiet;  //holds current menu - needed when buying cattle
      animalSection * StableSection;    //holds pointer to relevant animal section in housing

   /* Actions */
   private:
		cattleGroup& operator=(const cattleGroup& a); // Dissable the compilers generation of default assignment operator.
   public:
		linkList<cattle> *GetGroupClone();
		cloneList <diet> *GetFeedPlanListClone();
		cattleGroup(const cattleGroup& a);            // Dissable the compilers generation of default copy constructor.
      // Constructor
   private:
      cattleGroup(){};

   public:
      // Constructor with arguments
      cattleGroup(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
      virtual ~cattleGroup();


   void SetnumberOfCohorts(int aNumber) {numberOfCohorts = aNumber;}
   void SetnumberPerCohort(double aNumber) {numberPerCohort = aNumber;};

   void SetanimalSection(animalSection * aStableSection) {StableSection = aStableSection;};


   void SetanimalModel(int aModel) {animalModel = aModel;}


   int GetnumberOfCohorts() {return numberOfCohorts;};
   int GetnumberInstances(){return group->NumOfNodes();}

   double GetnumberOfAnimals();
   cattleptr GetGroupMember(int elementNum) {return group->ElementAtNumber(elementNum);}
	virtual string GetAlias(){return Alias;};

   double GetmaxAge() const {return maxAge;}
   double GetminAge() const {return minAge;}
   bool GetareFemale() const {return areFemale;}

   animalSection * GetanimalSection() {return StableSection;}


	virtual void ReadParameters(fstream* file);
   void AddAnimal(cattle * anAnimal);

   void checkBalances(bool show = false);

   void DailyUpdate(void);
   void CheckBirth();
   void CheckAge(linkList<cattle> * oldies);
	void SeedGroup(double startAge, double endAge, int *IDnumber);
	void CullAnimals(int numberToCull);
	bool CullAnimalFromGroup(cattle *theVictim);
	void CullAnimal(cattle *theVictim, bool deleteAnimal);
	void AutoRecruit(linkList<cattle> * recruitmentList, int *IDnumber);
	double GetCattleGroupN();
	double GetCattleGroupC();
	void CheckWean(linkList<cattle> * calves, bool stopLactation);
   double GetDE();
	void GiveIndicator(int indicatorType);
//	void FlushIndicator(int indicatorType);
   cattle * buyNewAnimal(double currentAge, int *IDnumber, bool logit);
};
#endif


