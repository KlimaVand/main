/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class "field"
Simulates a single field, and acts as container class for patch and plan
objects.
(c) 1996-2007 Bjørn Molt Petersen, Jørgen Berntsen, Nicholas John Hutchings
    Danmarks JordbrugsForskning
\****************************************************************************/

#ifndef FIELD_H
   #define FIELD_H

#include "plan.h"
#include "../products/plantItem.h"
#include "patch.h"
#include "../base/cloneList.h"
#include "../products/manure.h"
#ifdef NITROSCAPE
   #include "../data/pixiData.h"
   #include "../Nitroscape/patchinterface.h"
#endif

class field: public base
{
private:
	field();                          // Dissable use of default constructor
	field& operator=(const field& f); // Dissable the compilers generation of default assignment operator.

protected:
   fstream f,fPatch1,fPatch2;
#ifdef TUPLE
   fstream tuple;
#endif
   enum {MaxFixedCrops=25};          // Upper limit of legal crops
   enum {MaxWritePatches=25};        // Upper limit of patches written to patch inspection file
	typedef char str[id_len];

   char * FixedCrop[MaxFixedCrops];  // Legal crops, initialized by cropRotation
   double FixedSize[MaxFixedCrops];  // Area of future crops, initialized by cropRotation
   string History[BackScope]; // History of crops and manure application
   cloneList<patch> * thePatches;
  	plan * crrPlan;                   // Current plan of field operations
   cloneList<plan> * Lfutr;          // Linked list of future plans


   double
      Minimum,                       // A very small number used to ignore rounding errors
      area,                          // Present area of field, ha
      distance;                      // Distance to farm, meters

	int

      JBNO,                          // JB number of the soil (Danish soil classification system)
      daysSinceIrrigation,           // days since the last irrigation
      daysSinceIrrigationRequest,    // days since water deficiet
      daysBetweenIrrigation,
      fingerFlow,                    // Indicates (0,1) whether fingerflow should be used on sandy soil
   	irrigationDelay,
   	NoFixCrops;                    // Number of legal crops
#ifdef TUPLE
   bool
      tuple_file_open,
      writeTuples;
#endif
	bool

      active,                        // If false, field is 'dormant' with no area assigned
      monitorPatches,
      grazingShutDown,
      grazingPossible;

#ifdef ARLAS
   int FieldID;
#endif




   //NJH Dec 2005 (and again Oct 2006)
   double expectedCumulativeGrowth[2];
   double expectedGrowth[366];

   void UpdatePatches();
	void DivideThisInstance(field * &fP,plan * Pl);
	void MakeInstance(field * &fP,plan * Pl);
   field* MakeNewField();
	void PushHistory(field * fP,str * crop_id);

	bool HandleOperation(fieldOperationFields * FOP);
   void Irrigate(int year,int month,int day);
   bool SoilIsSuitableForFertilizing();
   bool OperationCanBePerformed(plan* aPlan,patch* aPatch);
	void InitVariables();
   void AdjustFieldArea(double newArea);

#ifdef TUPLE
   void CloseTupleFile();
   void OpenTupleFile(string s);
#endif

public:
   field(const char * aName, const int aIndex, const base * aOwner = NULL);
	field(const field& f);
	~field();


	void ReadPatchParameters(patch *p);
   void ReadParameters(fstream * file);
	void SplitMe(field * &fP);
	void ImplementFingerFlow();
//   void WritePatches();
   void UpdateField();

	int ReturnNumber(){return Index;};
	void CropHistory(int nr,char * &crop_id);
	void PresentCrop_(char * &crop_id);
	double DistanceToField_();

	double FieldArea_();
	int JBno();

   void SetDaysBetweenIrrigation(int interval) {daysBetweenIrrigation=interval;};
	void SetIrrigationDelay(int delay) {irrigationDelay=delay;};
	void SetFingerFlow(int aFingerFlow) {fingerFlow = aFingerFlow;};
	void PlanField_(cloneList<fieldOperationFields> * OP,str crop_id,int futureNum);
   void OutputPlantDetails(bool header);


   double GetArea() {return area;}
   void SetActive(bool a){active=a;};
   bool GetActive(){return active;};
   bool FuturePlanExists(){return Lfutr->NumOfNodes()>0;};
   bool GetGrazingPossible();

#ifdef TUPLE
   void AddTuple(string date,double n,double amount,string op,string type,double SFU);
   bool GetWriteTuples(){return writeTuples;};
#endif

   // PATCH functions
   patch * GetPatch(int aNumber) {return thePatches->ElementAtNumber(aNumber);};
   void AdjustPatchAreas(double adjust,bool balance);


   const int GetNumOfPatches() {return thePatches->NumOfNodes();};

   void GiveIndicators();
   void StartBudget();
   void EndBudget();
   bool GetGrazingShutDown() {return grazingShutDown;};
   void SetGrazingShutDown(bool sd) {grazingShutDown=sd;};
   // ------ Grazing functions - BMP version
   void DepositManure(manure * urineToField, manure * dungToField, double area_ur, double area_du);

   plan * GivecrrPlan() {return crrPlan;}

	double GetHerbageMasskg();
	double GetHerbageMasskgPerHa();

   double GetAvailableHerbageMasskgPerHa(double residualDM);
   double GetAvailableHerbageMasskg(double residualDM);


   void AutoGrazingPossible();
   double GetDMGrazed();
   double GetAbovegroundCropN();
   double GetDailyDMProduction();
	double InitExpectedGrowthRate(double radiation, double temperature);
   void AddExpectedGrowth(int period, double value) {	expectedCumulativeGrowth[period]+=value;};

   void SetexpectedGrowth(int day, double value) {expectedGrowth[day]= value;};
   double GetexpectedGrowth(int day) {return expectedGrowth[day];};
	void ClearTemporaryVariables();
};

#endif
