/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: grazing_manager_class                  //Borland C++ 5.02 - FASSET style
#ifndef __grazing_manager_class_H  //Required for current class
  #define __grazing_manager_class_H

#include "../building/thebuild.h"
#include "../fields/cropRotation.h" //Required for include files
#include "../livestock/livestock.h"
#include "../livestock/dairyStock.h"
#include "../livestock/cattleGroup.h"
#include "cattleFeedingManager.h"

class grazingManager: public base
{
   linkList <field> *fields_to_graze;        //graze these fields
	linkList <field> *fields_avail_to_graze;   //potentially grazable fields (are pasture)


	livestock * theLivestock;
   linkList <dairyStock> * theCattle;
	cropRotation * theCropRotation;
   buildings * theBuildings;
	cattleFeedingManager * theCattleFeedingManager;
   bool enableGrazing;
   double ExpectedSilageHolding;    //expected silage requirement
   double SilageMade;
   double residualDM,			//expected residual mass (kg/ha) after cutting/grazing
   minHarvestable;			//minimum herbage mass allowed (t/ha)before cutting can occur


   double lowerGrazingLimit;	   //number of days grazing available before switch to deficiency feed plan
   double upperGrazingLimit;    //number of days grazing available before switch to surplus feed plan

   double turnoutThreshold;     //number of days grazing available before turnout is permited

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

 grazingManager ();


 ~grazingManager ()  ;



	double GetminHarvestable() {return minHarvestable;};
   bool GetenableGrazing() {return enableGrazing;}
   linkList <field> * daily_update (int *numOthers);

	void Initialize(livestock * theLivestockptr,
									   cropRotation * theCropRotationptr,
                              buildings * theBuildingsptr,
                              cattleFeedingManager * theCattleFeedingManagerptr);
	void HouseAnimals();
	void ReceivePlan(char *fileName);//, bsTime * turnout, bsTime * housing);
	void SetGrazableFields();
	double calcSilageRequirement();


   double GetHerbageAvailability(bool restricted);
	double GetHerbageAvailability(bool restricted, int fieldNo);

   double GetturnoutThreshold() {return turnoutThreshold;};
	int CalcGrazingPeriod(bsTime *aDate, int growthPeriod);


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

	void AutoGrazingPossible();

	void CheckOpStopsGrazing();
};
#endif

