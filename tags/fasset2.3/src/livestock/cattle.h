/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#ifndef __cattle_H  //Required for current class
  #define __cattle_H

#include "../base/cloneList.h"
#include "../base/common.h"
#include "animal.h" //Required for base class
 #include "../products/feedResource.h"
 #include "../products/manure.h"
 #include "diet.h"
 #include "../base/IndicatorOutput.h"
#ifndef _STANDALONE
	#include "../fields/field.h"
	#include "../building/stable.h"
#endif

class cattle: public animal
{
 protected:
// 	IndicatorOutput *outdata;
   double bodyProtein;              //in kg
   double bodyFat;                  //in kg
   int age;								//age in days
   double MBW;								//Mature Body Weight in kg
   double weight;							//Liveweight in kg
   int code;								//contains animal's code from animals.dat
   bool isFemale;							//true if female
   bool isFoetus;							//true if it is a foetus
   bool isSuckling;						// true if animal is suckling from its mother
   bool isYoung;							//true if animal is classed as young
	cloneList <product> *feedPlanList;   //holds a list of the feed items available to the animal
	cloneList <product> *feedUseList;   //holds a list of the feed items eatten by the animal
   double daysFromParturition;      //days since calf born
   double daysFromConception;       //days since animal conceived
   int lactationNumber;   				//number of times animal has lactated (incl any current lactation)
   int feedingGroup;                //records the feeding group to which the animal belongs (set in cattlefeeding_manager_class.cpp)
   bool lactating;                  //true if animal is lactating
   bool pregnant;                   // true if animal is pregnant
   cattle * mother;                 // for calf, a pointer to mother
   cattle * calf;                   //used in mother to point to calf
   cattle * foetus;                 //used in mother to point to its foetus
#ifndef _STANDALONE
	linkList <field> *fieldList;     //list of the fields to which the animal has access for grazing
	double fieldOfftakeList[500];    //records the offtake of DM from different fields. Used to partition excretal returns
#endif
   double DM_intake;						//intake in kg DM per day
   feed_resource * currentDiet; 		//holds details of current diet of animal
   feedItem * milk;                 //holds details of milk produced (mother) or milk consumed (calf). If suckling, object is deleted by calf, otherwise by mother
   double lastMilkYield;		      //milk yield on previous day - used to prevent milk yield increasing after time of peak production potential
   double hoursHoused;              //hours per day spent in animal housing
   double gestationPeriod;				//gestation period in days
   double milkProteinConc; 			//g/kg in milk
   double milkFatConc; 					//g/kg in milk
   double lactoseConc;  				//g/kg in milk
   double acceptanceCoefficient;     // rate at which rejection due to presence of dung wears off (per d)

	fstream *detailsFile;    			// here for use when testing cow model
   char detailsFileName[100];
   int limitation;

   //variables made members because they are used for recording status of cattle
   double ME_intake;						//ME intake in MJ per day
   double FE_intake;						//intake in Scandinavian feed unit per day
   double FEDemand;
   feedItem *growth;						//used for recording only
   double aminoAcidSupply;
   double aminoAcidDemand;
	double dungN, urineN;    //dung and urine N in grams
   double rumenNsurplus;
   double Grazed_DM;
   double Grazed_FE;

   private:
#ifndef _STANDALONE
	cattle (){};  //block use of default constructor from outside animal
#endif

protected:
#ifdef _STANDALONE
	cattle ();
#endif
	virtual void intake(){};
	virtual void ProteinDigestion(double *availableN);
	virtual void Partition(){};
   virtual double CalcMaintenanceN();
	virtual double NinEBWchange(){return 0.0;};
	virtual feed_resource * GetFeedItem(product * menuPtr);
	virtual double Get_availability_factor( double herbage_mass);
	virtual double Get_quality_factor(double digestibility){return 0.0;};
	virtual double SucklingCalfMilkIntake(){return 0.0;};
	virtual void DoSelection(linkList <feed_resource> * , int * count);
   virtual void Createfoetus(){};

public:
			//Constructor setting attributes to default values
	cattle (const char *aName,const int aIndex,const base * aOwner);
	cattle (const cattle& anAnimal);
	virtual ~cattle(); //Destructor

// Set functions for attributes
   virtual void Setmother(cattle *aMother) {mother = aMother;}
   virtual void Setage(int anAge) {age = anAge;};
   virtual void Setcode(int aCode) {code = aCode;}
   virtual void Setweight(double aWeight) {weight = aWeight;};
   virtual void SetisFemale(bool isaFemale) {isFemale = isaFemale;};
	virtual void SetfeedPlanList(cloneList <product> * aFeedPlan) {feedPlanList = aFeedPlan;};
	virtual void SetfeedUseList(cloneList <product> * aFeedUseList) {feedUseList = aFeedUseList;};
   virtual void SetisYoung(bool isItYoung) {isYoung = isItYoung;}
   virtual void SetisSuckling(bool isItSuckling) {isSuckling = isItSuckling;}
   virtual void SetisFoetus(bool itIsAFoetus) {isFoetus=itIsAFoetus;};
   virtual void SetbodyProtein(double aProtein) {bodyProtein=aProtein;};
   virtual void SetbodyFat(double abodyFat) {bodyFat=abodyFat;};
   void SetFoetus(cattle * aFoetus) {foetus = aFoetus;};
   virtual void SethoursHoused(double aValue){hoursHoused=aValue;};
   virtual void SetfeedingGroup(int afeedingGroup) {feedingGroup=afeedingGroup;};
#ifndef _STANDALONE
	virtual void SetfieldList(linkList <field> * afieldList) {fieldList = afieldList;};
#endif
//   void SetisGrazing(bool itIsGrazing) {isGrazing = itIsGrazing;}


   virtual double Getage() {return age;};
   virtual int Getcode() const {return code;};
   virtual double Getweight() const {return weight;};
   virtual cattle * Getmother() {return mother;}
   virtual bool GetisFemale() const {return isFemale;};
	virtual cloneList <product> * GetfeedPlanList() {return feedPlanList;};
   virtual feedItem * Getmilk() {return milk;}
   virtual bool GetisSuckling() {return isSuckling;}
   virtual bool GetisFoetus() {return isFoetus;};
   virtual double GetgestationPeriod() {return gestationPeriod;};
   virtual double GetCattleN();
   virtual double GetCowN();
   virtual double GetbodyProtein() const {return bodyProtein;};
   virtual double GetbodyFat() const {return bodyFat;};
	virtual double GetPotentialMilkIntake(){return 0.0;};
   virtual int GetfeedingGroup() {return feedingGroup;};

   double GetlastMilkYield() {return lastMilkYield;}
   void Setlactating(bool aLactation) {lactating = aLactation;};
   void Setpregnant(bool aPregnant) {pregnant = aPregnant;};
   void SetlactationNumber(int aNumber) {lactationNumber=aNumber;};
   void SetCalf(cattle * aCalf) {calf = aCalf;};
   void SetdaysFromParturition(double days) {daysFromParturition=days;}
   void SetdaysFromConception(double days) {daysFromConception=days;};
   bool isLactating() const {return lactating;};
   bool isPregnant() const {return pregnant;};
   virtual bool GetisYoung() {return isYoung;};
   int GetlactationNumber() {return lactationNumber;};
   cattle * GetFoetus() {return foetus;};
   cattle * GetCalf() {return calf;};
   double GetdaysFromParturition() const {return daysFromParturition;}
   double GetdaysFromConception() const {return daysFromConception;};
	virtual double GetPotentialMilkYield(int daysFromParturition){return 0.0;};
	virtual double GetPotentialMilkYield(){return 0.0;};
   double GetaminoAcidSupply(){return aminoAcidSupply;}
   double GetaminoAcidDemand(){return aminoAcidDemand;}
	double GetdungN() {return dungN;}
	double GeturineN() {return urineN;}
   double GetrumenNsurplus() {return rumenNsurplus;}
   double GetGrazed_DM() {return Grazed_DM;}
   double GetGrazed_FE() {return Grazed_FE;}
   double GetDM_intake() {return DM_intake;}
   double GetFE_intake() {return FE_intake;}
   double GetFEDemand() {return FEDemand;};
   double GetME_intake() {return ME_intake;}

	virtual void ReadParameters(fstream * file);
	virtual void DailyUpdate ()  ;
   virtual void ReceivePlan (cloneList <product>* afeedPlanList)  ;
	virtual double Production();
  //GAMs functions - not checked!!!!
	void ManureProduction ( fstream* fileStream)  ;
	void FeedMax ( fstream* fileStream)  ;
	void FeedMin ( fstream* fileStream)  ;

//   void Manure(manure* fluidManure, manure* solidManure);
   void ChangeName(char * newname){strcpy(Name,newname);}
   void IncrementIndex() {Index++;}
//	virtual bool TruncateName(string * ret_string);
   double GetPotentialIntake(){return 0.0;};
   virtual cattle* CheckBirth(){return NULL;};
   virtual void DryOff();
   virtual void MakePregnant(bool isAFemale){};
//   virtual void Initialise(int anAnimalModel, int aCode, double anAge, double LU, double numPrCohort, bool isAYoung, fstream * CattlefileStream);
   virtual void Initialise(int aCode, int anAge, double LU, double numPrCohort, bool isAYoung);
   virtual void Initialise();

   virtual double Get_norm_weight();
  	virtual double CalcFoetusProtein(double offset);
	virtual double GetFoetusWeight(double offset){return 0.0;};

   virtual void ShowCurrentDiet(fstream * fs);
	virtual void checkBalances(bool show);
	virtual double CalcGravidUterusProtein(double offset);
	virtual void WriteCowOutput(bool header);
	virtual void CreateDetailFile(int Num);

   virtual void GrowFoetus(double newFoetusProtein, double newFoetusFat, double newFoetus);
   virtual feedItem * Getgrowth(){return growth;};
   virtual double GetMethaneProduction();
	virtual double CalcFoetusEnergy(double offset);
	virtual double CalcFoetusFat(double offset);
	virtual double CalcGravidUterusEnergy(double offset);
	virtual double CalcGravidUterusFat(double offset);
	virtual void GrowGravidUterus(double newProtein, double newFat){};
   virtual double GetCattleC();
   virtual double GetCowC();
//   virtual void EndBudget(bool show);
   virtual void InitialiseFoetus();
	virtual double GetMilkCconc();
   virtual void CalcHeatProduction(manure *urine){};
	virtual double GetUrineVolume();

	virtual void WeanCalf();
   virtual double GetPalatability(feed_resource *aFeed_resource) {return 0.0;};
	double GetAcceptanceFactor(feed_resource *aFeed_resource);
   void loadAnimalProduct(animalProduct *anAnimalProduct);
   virtual double GetPotentialAnnualMilkProduction(int durationOfLactation){return 0.0;};
   virtual double GetEnergyRequirement(){return 0.0;};
   virtual double GetSensibleHeatProduction();
   //!calculate the amount of fat and protein in our an animal of a particular empty body weight
   /*!
    *\param EBW empty body weight of the animal (kilograms)
    *\param fat amount of fat in the animal (kilograms)
    *\param protein amount of protein in the animal (kilograms)
    */
   virtual void GetStandardComposition(double EBW, double *fat, double *protein);
};
#endif
