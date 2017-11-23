/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: grazing_manager_class                  //Borland C++ 5.02 - FASSET style
#ifndef __grazing_manager_class_H  //Required for current class
  #define __grazing_manager_class_H

#include <thebuild.h>
#include <croprot.h> //Required for include files
#include <livestoc.h>
#include <dairystk.h>
#include <cattlegrp.h>
#include <cattlefeeding_manager_class.h>

class grazing_manager_class: public base
{
   linkList <field> *fields_to_graze;        //graze these fields
	linkList <field> *fields_avail_to_graze;   //potentially grazable fields (are pasture)
   catstable * dairyHouse;
   catstable * followerHouse;
	livestock * theLivestock;
   linkList <dairyStock> * theCattle;
	cropRotation * theCropRotation;
   buildings * theBuildings;
	cattlefeeding_manager_class * theCattleFeedingManager;
   bool enableGrazing;
   double ExpectedSilageHolding;    //expected silage requirement
   double SilageMade;
   double residualHeight,			//expected residual height (metres) after cutting/grazing
   minHarvestable,			//minimum herbage mass allowed (t/ha)before cutting can occur
   minProteinConc,		//min concentration of protein in dry matter for cut grass
   minEnergyConc;			//min concentration of energy in dry matter for cut grass
   double lowerGrazingLimit;	   //number of days grazing available before switch to deficiency feed plan
   double upperGrazingLimit;    //number of days grazing available before switch to surplus feed plan
   linkList <bsTime> * periodStart;  // date of start of each grazing period
   double turnoutThreshold;     //number of days grazing available before turnout is permited
   int duration [4];
   bool initialised;				//false until expected growth has been calculated
   bsTime * growthStart;    //approximate date for start and end of grass growth
   bsTime * growthEnd;
   double targetCutMass[2];   // target mass in first and subsequent growth periods
   bsTime *periodEnd;			//date of end of next period
   int currentPeriod;			//number of the current period
   int maxPeriods;				//maximum number of grazing periods allowed
   fstream *grazmanfile;			//file for output of pasture management information
   int springCutoffDay;			//day number when spring growth changes to summer growth
   double potentialGrowthFactor;	//expected growth as a fraction of potential growth

public:

//user created functions

 grazing_manager_class ();


 ~grazing_manager_class ()  ;

	double GetminEnergyConc() {return minEnergyConc;};
	double GetminProteinConc() {return minProteinConc;};
	double GetminHarvestable() {return minHarvestable;};
   linkList <field> * daily_update (int *numOthers);

	void Initialize(livestock * theLivestockptr,
									   cropRotation * theCropRotationptr,
                              buildings * theBuildingsptr,
                              cattlefeeding_manager_class * theCattleFeedingManagerptr);
	void HouseAnimals();
	void ReceivePlan(char *fileName);//, bsTime * turnout, bsTime * housing);
	void SetGrazableFields();
	double calcSilageRequirement();
   void checkSilageHolding();
	void checkPasture();
   double GetHerbageAvailability(bool restricted);
	double GetHerbageAvailability(bool restricted, int fieldNo);
   double GetGrazingAreaAvailable();
   double GetturnoutThreshold() {return turnoutThreshold;};
	int CalcGrazingPeriod(bsTime *aDate, int growthPeriod);
   void SetFieldsGrazed(bsTime * aDate, int duration, double herbageRequired);
	void SetFieldsGrazed(double herbageRequired);
	void InsertCut(field * aField, bool harvestCut);
	double GetAverageGrowthRate(bsTime * aDate, int fieldNo, int duration);
	void InitialiseGrowthEstimates();
	void StartGrazingManagement();
	void SortFields();
	void SetGrazing(bsTime * aDate, int duration,int *numOthers);
	double GetExpectedGrowthRate(bsTime *aDate, int fieldNo);
	bool CutConservationFields(bool forceCut);
	void EndOfSeason(bool topping);
	void SetGrazingAll();
	bool CheckGrazingTheshold();
   void InitialiseFileOutput();
	void GetStatus();
	bool CheckConservationFields();
   int GetGrazingSupplyStatus(double numberOfDaysSupply);
	int GetNumGrazableFields();
	void AutoGrazingPossible();
	void ClearTemporaryVariables();
	void CheckOpStopsGrazing();
};
#endif

