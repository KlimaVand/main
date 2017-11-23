/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#ifndef __cattlefeeding_manager_class_H  //Required for current class
  #define __cattlefeeding_manager_class_H

#include "../livestock/cattle.h"
#ifndef _STANDALONE
   #include "../livestock/livestock.h"
   #include "../livestock/dairyStock.h"
   #include "../livestock/cattleGroup.h"
#endif
#include "cattleFeedingGroup.h"

class cattleFeedingManager: public base
{
   linkList <cattleFeedingGroup> * feedingGroups;
	bool dairyPresent;
   bool beefPresent;
	livestock * theLivestock;

   int startLateLactation;        //defines start of late lactation in days from parturition
   int dairyLo,dairyHi;				//boundaries of dairy animals in feeding group list


   double differentiation;			//proportional change in amount of concentrate feed in surplus diet (negative change) and deficiency diet (positive change)
   fstream *fHerdPerformance;
   fstream *fCattleFeeding;
   bsTime * housingDate;
   bsTime * turnoutDate;

   bool autoFeed;				//true if concentrate feeding should be calculated

public:

//user created functions

   cattleFeedingManager ();
   cattleFeedingManager (const char *aName,const int aIndex,const base * aOwner);


 	~cattleFeedingManager ()  ;

	void SetdairyPresent(bool isPresent){dairyPresent=isPresent;};
	bool GetdairyPresent(){return dairyPresent;};
	void SetbeefPresent(bool isPresent){beefPresent=isPresent;};
	bool GetbeefPresent(){return beefPresent;};
   bool GetautoFeed() {return autoFeed;};

   cattleFeedingGroup * GetGroupMember(int elementNum) {return feedingGroups->ElementAtNumber(elementNum);}

	void daily_update (int status);
	void Initialize(livestock * theLivestockptr);
   void ReceivePlan();

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

	void WriteGroupOutput();
	void InitialiseGroupOutput();

   double GetDairyHerbageRequired(bool zeroGrazed) {return GetHerbageRequired(dairyLo,dairyHi, zeroGrazed);};
   double GetFollowerHerbageRequired(bool zeroGrazed) {return GetHerbageRequired(0,dairyLo-1,zeroGrazed);};

   int GetNumFeedingGroups() {return feedingGroups->NumOfNodes();};

	double GetSilageRequired();
   double GetPotentialMilkYield(int durationOfLactation);

	void AutoFeed(int conceptionTime, int lengthOfLactation);

};
#endif

