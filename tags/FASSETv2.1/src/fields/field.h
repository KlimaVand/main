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
(c) 1996-2007 Bj�rn Molt Petersen, J�rgen Berntsen, Nicholas John Hutchings
    Danmarks JordbrugsForskning
\****************************************************************************/

#ifndef FIELD_H
   #define FIELD_H

#include <plan.h>
#include <pplantit.h>
#include <patch.h>
#include <clonlist.h>
#include <pmanure.h>

#ifdef ARLAS
   #include <suitcase.h>
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
	struct historyStruct
   {                                 // Previous crop rotation
		str crop;                      // crop id
		double HUG_N;                  // Nitrogen from applied organic fertilizer, kg/ha
	};

   char * FixedCrop[MaxFixedCrops];  // Legal crops, initialized by cropRotation
   double FixedSize[MaxFixedCrops];  // Area of future crops, initialized by cropRotation
	historyStruct History[BackScope]; // History of crops and manure application
   cloneList<patch> * thePatches;
  	plan * crrPlan;                   // Current plan of field operations
   cloneList<plan> * Lfutr;          // Linked list of future plans
   string last_date;

   double
      Minimum,                       // A very small number used to ignore rounding errors
      area,                          // Present area of field, ha
      MinArea,                       // Minimal legal area of field
      distance,                      // Distance to farm, meters
      deposition_sum,
      fixation_sum;            

	int
      last_idx,
      fenced,                        // Indicates (0,1) whether field is fenced
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
      testover,                      // Remove later !!!
      active,                        // If false, field is 'dormant' with no area assigned
      monitorPatches,
      grazingShutDown,
      grazingPossible;

#ifdef ARLAS
   int FieldID;
#endif
   double
      area_urine,
      area_overlap,
      area_dung,
      new_area_urine,
      new_area_dung,
      DMOfftake;

   //NJH Dec 2005 (and again Oct 2006)
   double expectedCumulativeGrowth[2];
   double expectedGrowth[366];

   manure * urine;
   manure * dung;

   int maxPatches;
   int pile_days;
   int pile_counter;
   // Suitcase *FieldSuitcase; ARLAS???!!!
   double NitrogenInSystem();
   void UpdatePatches();
	void DivideThisInstance(field * &fP,plan * Pl);
	void MakeInstance(field * &fP,plan * Pl);
   field* MakeNewField();
	void PushHistory(field * fP,str * crop_id);
	void AssignNewCrop();
	bool HandleOperation(fieldOperationFields * FOP);
   void Irrigate(int year,int month,int day);
   bool SoilIsSuitableForFertilizing();
   bool OperationCanBePerformed(plan* aPlan,patch* aPatch);
	void InitVariables();
   void AdjustFieldArea(double newArea);
   int GetOperationIndex(string date);
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
	void ShowHistory();
	void AssignNumber(int num){Index=num;};
	int ReturnNumber(){return Index;};
	void CropHistory(int nr,char * &crop_id);
	void PresentCrop_(char * &crop_id);
	double DistanceToField_();
	int IsFieldFenced_();
	double FieldArea_();
	int JBno();
   void SetMinimumArea(double min_area) {MinArea=min_area;}
   void SetDaysBetweenIrrigation(int interval) {daysBetweenIrrigation=interval;};
	void SetIrrigationDelay(int delay) {irrigationDelay=delay;};
	void SetFingerFlow(int aFingerFlow) {fingerFlow = aFingerFlow;};
	void PlanField_(cloneList<fieldOperationFields> * OP,str crop_id,int futureNum);
	void ForceCurrentPlant(linkList<fieldOperationFields> * OP,str crop_id);
	void ShowCurrentPlan(ostream * fs);
	void ShowFuturePlans(ostream * fs);
   void DisplayCurrentPlan();
   void OutputPlantDetails(bool header);
	double ExtraN();
   void GiveIndicator();
   void ResetFixedCrops();
   void AddFixedCrop(const char * crop_id,double size);
   int NumOfFixedCrops();
   const char * GiveFixedCrop(int No);
   double GiveFixedSize(int No);
   double GetArea() {return area;}
   void SetActive(bool a){active=a;};
   bool GetActive(){return active;};
   bool FuturePlanExists(){return Lfutr->NumOfNodes()>0;};
   bool GetGrazingPossible();
   void SetGrazingPossible(bool trueOrfalse) {grazingPossible = trueOrfalse;};
#ifdef TUPLE
   void AddTuple(string date,double n,double amount,string op,string type,double SFU);
   bool GetWriteTuples(){return writeTuples;};
#endif

   // PATCH functions
   patch * GetPatch(int aNumber) {return thePatches->ElementAtNumber(aNumber);};
   void AdjustPatchAreas(double adjust,bool balance);
   const cloneList<patch>* GetThePatches() {return thePatches;};
   void SlicePatch(double sliceArea);
   const int GetNumOfPatches() {return thePatches->NumOfNodes();};
   void MeltAllPatches();
   void GiveIndicators();
   void StartBudget();
   void EndBudget();
   bool GetGrazingShutDown() {return grazingShutDown;};
   void SetGrazingShutDown(bool sd) {grazingShutDown=sd;};
   // ------ Grazing functions - BMP version
   double GetDMavailableForGrazing(){return 0.0;}; // Get initial info
   void ConsumeGrass(double * DM, double * N, double * digestibility){};
   void DepositManure(manure * urineToField, manure * dungToField, double area_ur, double area_du);
	void OutputYearlyData(char * inputDir);
	void OutputDailyData();
	double GetMaxCropHeight();   // NJH June 2002
   plan * GivecrrPlan() {return crrPlan;}
   plan * GiveNextPlan() {return Lfutr->ElementAtNumber(0);}
	double GetHerbageMasskg();
	double GetHerbageMasskgPerHa();

   patch* GetMeanPatch(bool incl_basal, bool incl_urine, bool incl_overlap, bool incl_dung);
   void calc_new_spots(double new_area_urine, double new_area_dung,
      double* new_area_urine_once, double * new_area_dung_once, double* new_area_urine_overlap,
      double* new_area_dung_overlap, double* new_area_basal);
   double AreaBudget();
#ifndef NITROSCAPE
//   void ExcretionDynamics();
	patch* getCurrentPatch(bool dung, bool overlap);
   void TestSplit();
   void WriteTestState();
   void AddPatch(decomposable* matter,patch *new_patch,int type,char * name,double area);
#endif
   double GetAvailableHerbageMasskgPerHa(double cuttingHeight);
   double GetAvailableHerbageMasskg(double cuttingHeight);
   void GetRoughageProducts(linkList <product> *aList);
   int GetForageItemCode();
   void AutoGrazingPossible();
   double GetDMGrazed();
   double GetAbovegroundCropN();
   double GetDailyDMProduction();
	double InitExpectedGrowthRate(double radiation, double temperature);
   void AddExpectedGrowth(int period, double value) {	expectedCumulativeGrowth[period]+=value;};
   double GetExpectedGrowth(bsTime *aDate); // Old one!!!
   void SetexpectedGrowth(int day, double value) {expectedGrowth[day]= value;};
   double GetexpectedGrowth(int day) {return expectedGrowth[day];};
	void ClearTemporaryVariables();
};

#endif
