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

//#include <uk_cattle.h>
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
		cattleGroup(const cattleGroup& a);            // Dissable the compilers generation of default copy constructor.
      // Constructor
      cattleGroup(){};

   public:
      // Constructor with arguments
      cattleGroup(const char *aName,const int aIndex,const base * aOwner);
      // Destructor
      virtual ~cattleGroup();

   linkList <cattle> * GetGroup(){return group;};
   void SetnumberOfCohorts(int aNumber) {numberOfCohorts = aNumber;}
   void SetnumberPerCohort(double aNumber) {numberPerCohort = aNumber;};
   void SetminAge(double anAge) {minAge =anAge;}
   void SetanimalSection(animalSection * aStableSection) {StableSection = aStableSection;};
   void SettheCurrentDiet(diet* aDiet) {theCurrentDiet = aDiet;};  //holds current menu - needed when buying cattle
   void SetareYoung(bool young) {areYoung = young;}
   void SetanimalModel(int aModel) {animalModel = aModel;}
   diet * GettheCurrentDiet() const {return theCurrentDiet;}  //holds current menu - needed when buying cattle

   int GetnumberOfCohorts() {return numberOfCohorts;};
   int GetnumberInstances(){return group->NumOfNodes();}
   double GetnumberPerCohort() {return numberPerCohort;};
   double GetnumberOfAnimals();
   cattleptr GetGroupMember(int elementNum) {return group->ElementAtNumber(elementNum);}
	virtual string GetAlias(){return Alias;};
//   double GetNumber() {return number;}
   int Getcode() const {return code;};
   double GetmaxAge() const {return maxAge;}
   double GetminAge() const {return minAge;}
   bool GetareFemale() const {return areFemale;}
   int GetanimalModel() {return animalModel;}
   animalSection * GetanimalSection() {return StableSection;}
	cloneList <diet> * GetfeedPlanList() const {return feedPlanList;}   //contains list of the feed items available to the animals in the group


	virtual void ReadParameters(fstream* file);
   void AddAnimal(cattle * anAnimal);

   void checkBalances(bool show = false);
   void InitCattleGrp(fstream * file);
   void DailyUpdate(void);
   void CheckBirth();
   void CheckAge(linkList<cattle> * oldies);
	void SeedGroup(double startAge, double endAge, int aggregation, int *IDnumber);
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


