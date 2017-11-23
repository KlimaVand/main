/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: cattlefeeding_manager_class                  //Borland C++ 5.02 - FASSET style
#ifndef __cattlefeeding_manager_class_H  //Required for current class
  #define __cattlefeeding_manager_class_H

#include <cattle.h>
#ifndef _STANDALONE
   #include <livestoc.h>
   #include <dairystk.h>
   #include <cattlegrp.h>
#endif
#include <cattlefeeding_group_class.h>

class cattlefeeding_manager_class: public base
{
   linkList <cattlefeeding_group_class> * feedingGroups;
	bool dairyPresent;
   bool beefPresent;
	livestock * theLivestock;
   double firstLateLimit,matureLateLimit;
   int startLateLactation;        //defines start of late lactation in days from parturition
   int dairyLo,dairyHi;				//boundaries of dairy animals in feeding group list
   double dairyFeedAdjustmentFactor;       //used to adjust concentrate feeding in dairy cattle
   double targetMilkYieldFactor;      //target as proportion of potential yield
   double differentiation;			//proportional change in amount of concentrate feed in surplus diet (negative change) and deficiency diet (positive change)
   fstream *fHerdPerformance;
   fstream *fCattleFeeding;
   bsTime * housingDate;
   bsTime * turnoutDate;
   bool zeroGrazing;
   bool autoFeed;				//true if concentrate feeding should be calculated

public:

//user created functions

 	cattlefeeding_manager_class ();
	cattlefeeding_manager_class (const char *aName,const int aIndex,const base * aOwner);


 	~cattlefeeding_manager_class ()  ;

	void SetdairyPresent(bool isPresent){dairyPresent=isPresent;};
	bool GetdairyPresent(){return dairyPresent;};
	void SetbeefPresent(bool isPresent){beefPresent=isPresent;};
	bool GetbeefPresent(){return beefPresent;};
   bool GetautoFeed() {return autoFeed;};
   void SettargetMilkYieldFactor(double aValue) {targetMilkYieldFactor=aValue;};
   cattlefeeding_group_class * GetGroupMember(int elementNum) {return feedingGroups->ElementAtNumber(elementNum);}

	void daily_update (int status);
	void Initialize(livestock * theLivestockptr);
   void ReceivePlan();
	void ReadFeedingGroupParameters(int start, int stop);
 //	void checkPasture();
	void SortAnimals(bool noCull);
   void CheckDiets(bool startup);
	double GetHerbageRequired(bool zeroGrazed);
	double GetHerbageRequired(int start, int end,bool zeroGrazed);
   void weanCalves(int herdNo, double daysSinceBirth);
	dairyStock*  GetHerd(int whichHerd);
   void ClearGroups();
	void AllocateFieldsForGrazing(linkList <field> *fields_avail_for_grazing, int numOthers);
   bsTime * GetTurnoutDate(){return turnoutDate;};
   bsTime * GetHousingDate(){return housingDate;};
   void CalcTurnoutandHousingDates(bool zeroGrazed);
   double GetCattleIntake();
	void WriteGroupOutput();
	void InitialiseGroupOutput();
	void GetRoughageRequired(linkList <product> *aList);
   double GetDairyHerbageRequired(bool zeroGrazed) {return GetHerbageRequired(dairyLo,dairyHi, zeroGrazed);};
   double GetFollowerHerbageRequired(bool zeroGrazed) {return GetHerbageRequired(0,dairyLo-1,zeroGrazed);};
   double GetBeefHerbageRequired(bool zeroGrazed) {return GetHerbageRequired(dairyHi,GetNumFeedingGroups()-1,zeroGrazed);};
   int GetNumFeedingGroups() {return feedingGroups->NumOfNodes();};
	double GetGrassSilageRequired();
	double GetSilageRequired();
   double GetPotentialMilkYield(int durationOfLactation);
   double GetEnergyRequirement();
	void AutoFeed(int conceptionTime, int lengthOfLactation);
   void GetNumber();
};
#endif

