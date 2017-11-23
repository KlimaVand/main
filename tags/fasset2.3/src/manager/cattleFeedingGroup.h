/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: cattlefeeding_group_class                  //Borland C++ 5.02 - FASSET style
#ifndef __cattlefeeding_group_class_H  //Required for current class
  #define __cattlefeeding_group_class_H

#include "../livestock/cattle.h"
#ifndef _STANDALONE
	#include "../livestock/livestock.h"
#endif
class cattleFeedingGroup: public base
{
   linkList <cattle> * theGroup;
   linkList <diet> * theGroupDiets;
 	diet *theCurrentDiet;
   string Alias;
   bool wasFemale;
	int currentFeedingGroup;

   bool active;

public:

//user created functions

 cattleFeedingGroup ();

 cattleFeedingGroup(const char * aName, const int aIndex, const base * aOwner);

 ~cattleFeedingGroup ()  ;

 bool Getactive(){return active;};
 void Setactive(bool aVal){active=aVal;};
 void ReadParameters(fstream *infile, bool autoFeed);

 void AddToGroup(cattle *anAnimal){theGroup->InsertLast(anAnimal);}; // LEAK - probably stems from here (bmp) !!!
 void makePregnant(int daySinceParturition);
 void DryOff(int daySinceParturition);
 void CheckDiets(bool startup);
 diet * GetHousedDiet();
 void ShowGroupDiets(fstream *afile, bool header);
 void FeedCattle(int status);
 double GetHerbageRequired(bool zeroGrazed);
 void ClearGroup();
 const char * GetAlias() {return Alias.c_str();}

#ifndef _STANDALONE
 void AllocateFieldsForGrazing(linkList <field> *fields_avail_for_grazing, int start, int stop);
#endif
 bsTime * GetTurnoutDate(bool zeroGrazed);
 bsTime * GetHousingDate(bool zeroGrazed);

 void GetStatus(fstream * aFile);
 double GetNumber();


 double GetSilageRequired(int startCode, int endCode);
 double GetPotentialMilkYield(int durationOfLactation);

// void SetroughageEnergyAllowance();
 void CalcDefaultDiets(double totalEnergyRequired, double proteinRequired, double differentiation, bool defSurp,fstream *aFile);
};
#endif

