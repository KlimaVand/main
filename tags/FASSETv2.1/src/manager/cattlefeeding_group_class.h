/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: cattlefeeding_group_class                  //Borland C++ 5.02 - FASSET style
#ifndef __cattlefeeding_group_class_H  //Required for current class
  #define __cattlefeeding_group_class_H

#include <cattle.h>
#ifndef _STANDALONE
	#include <livestoc.h>
#endif
class cattlefeeding_group_class: public base
{
   linkList <cattle> * theGroup;
   linkList <diet> * theGroupDiets;
 	diet *theCurrentDiet;
   string Alias;
   bool wasFemale;
	int currentFeedingGroup;
	linkList <field> *fields_to_graze;
   bool active;

public:

//user created functions

 cattlefeeding_group_class ();

 cattlefeeding_group_class(const char * aName, const int aIndex, const base * aOwner);

 ~cattlefeeding_group_class ()  ;

 linkList <cattle> * GettheGroup() {return theGroup;}
 bool Getactive(){return active;};
 void Setactive(bool aVal){active=aVal;};
 void ReadParameters(fstream *infile, bool autoFeed);
 void ReadParameters(char *feedFileName, bool autoFeed);
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
 diet * GettheCurrentDiet() {return theCurrentDiet;}
#ifndef _STANDALONE
 void AllocateFieldsForGrazing(linkList <field> *fields_avail_for_grazing, int start, int stop);
#endif
 bsTime * GetTurnoutDate(bool zeroGrazed);
 bsTime * GetHousingDate(bool zeroGrazed);
 double GetCattleIntake();
 void GetStatus(fstream * aFile);
 double GetNumber();
 void GetRoughageRequired(linkList <product> *aList);
 double GetGrassSilageRequired();
 double GetSilageRequired(int startCode, int endCode);
 double GetPotentialMilkYield(int durationOfLactation);
 double GetEnergyRequirement();
// void SetroughageEnergyAllowance();
 void CalcDefaultDiets(double totalEnergyRequired, double proteinRequired, double differentiation, bool defSurp,fstream *aFile);
};
#endif

