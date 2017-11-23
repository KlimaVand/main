/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Container class for the crop rotation model in FASSET
\****************************************************************************/

#ifndef CROPROT_H
   #define CROPROT_H
class manureManager;  //forward reference NJH Aug 2002

#include "../base/periods.h"
#include "fieldOrder.h"
#include "staticCrops/staticCrop.h"
#include "field.h"

class cropRotation: public base
{
private:
	enum
      {max_pv_strat=3,
      Maxperiods=3,
      max_n_lev=12,
      max_n_lev_multper=3,
      nocattle_cropnum=13,
      cattle_cropnum=38,
      gams_str_len=80};
	typedef linkList<fieldOrder> FieldOrderList;
	typedef char string9[9];
	struct Cropstruct          // "CrpNme" will become an attribute in 'staticCrop' and
	{                          // derivatives when all the classes are programmed.
		char * CrpNme;
		staticCrop * CP;
	};
	static linkList<field>::PS GlobalP;
	static int
      field_no_,
      soil_type_;
	static cropRotation * ths;                    // Points at the object (type "cropRotation"), and is used in static functions.
   static double manureAdjustment;      //used to hold adjustment factor for manure applications - is set by manure manager NJH Aug 2002
	int conventional,irrigate,cattle;
	int Legal[cattle_cropnum][cattle_cropnum];    // Defines legal crop rotations.
	float Reduc[cattle_cropnum][cattle_cropnum];  // Crop rotation related change in yield.
	Cropstruct CropArray[cattle_cropnum];
	staticCrop staticBase;
	staticCrop * CropPointer;
	const char * PvName;
	const char * FertLevName;
	const char * FieldName;
	linkList <field> fieldList;
   periods thePeriods;
   fstream f;

	cropRotation& operator=(const cropRotation& cR); // Dissable the compilers generation of default assignment operator.
	cropRotation(const cropRotation& cR);            // Dissable the compilers generation of default copy constructor.

	void Dot();
	static void ShowH(field * PP);
	static void MakeSequenceInstance(fieldOrder * PP);
	void UpdateFld(field * PP);
	void InitReducTable();
	void InitSingleFields(string filename,double min_area,int daysBetweenIrrigation, int irrigationDelay,int FingerFlow);
	void InitStaticCrops();
	void InitLegalTable();
	double ExtraNRelease(char * crop_id);
	void SetCropPointer(char * crop_id);
	double NitrogenAmount(int field_no,char * crop_id,int N_level,int yearOffset);
	double PhosphorAmount(char * crop_id);
	double PotassiumAmount(char * crop_id);
	int GiveCropNo(char * crop_id);
	double ExpectedYield(int field_no,char * crop_id,int N_level,int PV_strat);
	double ExpectedStrawYield(int field_no,char * crop_id,int N_level,int PV_strat);
	int NumOfProtectStrat(char * crop_id);
	int NumOfCrops();
	int NitrogenLevels(char * crop_id);
	void CropId(int crop_no,char * &crop_id);
	double YieldReduction(int field_no,char * crop_id);
	double DistanceToField(int field_no);
	int IsFieldFenced(int field_no);
	int IsCropFallow(char * crop_id);
   int IsCropGrass(int crop_num);
	int IsCropLegal(int field_no,char * crop_id);
	const char * ProductName(int crop_num);
	void PlanField(int field_no,FieldOrderList &FOL);
   void CorrectCropId(fieldOrder * PP);
 	//----------LP model communication-------------------------------------------
   int TransformLevel(int i);
	void RecurRotation(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteRotationToGams(string filename,int firstSeason);
	void RecurPCT1(int a,int b,string9 & s,fstream * f,int fieldno);
	void WritePCT1NGams(string  filename,int firstSeason,int lastSeason);
   void RecurPCT1NPK(int a,int b,string9 & s,fstream * f,int fieldno);
	void WritePCT1NPKGams(string  filename,int firstSeason,int lastSeason);
	void RecurNRHS2(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteNRHS2Gams(string  filename,int firstSeason,int lastSeason);
	void RecurPCTNPK(int a,int b,string9 & s,fstream * f,int fieldno);
	void WritePCTNPKGams(string  filename,int firstSeason,int lastSeason);
	void RecurCCB(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteCCB_ToGams(string  filename,int firstSeason,int lastSeason);
	void RecurCCT(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteCCT_ToGams(string  filename,int firstSeason,int lastSeason);
	void RecurPremium(int a,int b,string9 & s,fstream * f,int fieldno);
	void WritePremiumsToGams(string  filename,int firstSeason,int lastSeason);
	void RecurReform(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteReformToGams(string  filename,int firstSeason,int lastSeason);
	void RecurLabCapUse(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteLabCapUseToGams(string  filename,int firstSeason,int lastSeason);
	void RecurCombCapUse(int a,int b,string9 & s,fstream * f,int fieldno);
	void RecurCombCapUseWithPV(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteCombCapUseGams(string  filename,int firstSeason,int lastSeason);
	void RecurSprayCapUse(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteSprayCapUseGams(string  filename,int firstSeason,int lastSeason);
	void RecurAllowed(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteAllowedToGams(string  filename,int firstSeason,int lastSeason);
	void RecurManureIntervals(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteManureIntervalsToGams(string  filename,int firstSeason,int lastSeason);
	void RecurFertilizer(int a,int b,string9 & s,fstream * f,int fieldno,int mode,int N_levels);
	void WriteFertilizerGams(string  filename,int firstSeason,int lastSeason,int mode);
   void RecurNitrFix(int a,int b,string9 & s,fstream * f,int fieldno,int N_levels);
   void WriteNitrFixGams(string  filename,int firstSeason,int lastSeason);
	void RecurYield(int a,int b,string9 & s,fstream * f,int fieldno,int mode);
   void RecurYieldWithPV(int a,int b,string9 & s,fstream * f,int fieldno,int mode);
	void WriteYieldCropsGams(string  filename,int firstSeason,int lastSeason,int mode);
	void RecurCostParameters(int a,int b,string9 & s,fstream * f,int fieldno);
	void RecurCostParametersWithPV(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteCostParametersGams(string  filename,int firstSeason,int lastSeason);
	void RecurCropSet(int a,int b,string9 & s,fstream * f,int fieldno);
   void WriteCropSetToGams(string  filename,int firstSeason,int lastSeason);
	void RecurStrawCropSet(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteStrawCropSetToGams(string filename,int firstSeason,int lastSeason);
	void RecurFallowCropSet(int a,int b,string9 & s,fstream * f,int fieldno);
   void WriteFallowCropSet(fstream * f,int fieldno);
	void WriteFallowCropSetToGams(string filename,int firstSeason,int lastSeason);
	void RecurGreenCropSet(int a,int b,string9 & s,fstream * f,int fieldno);
	void WriteGreenCropSetToGams(string filename,int firstSeason,int lastSeason);
	void WriteCCT_ToGams(string filename); // For one period LP-model.
   void WriteCCB_ToGams(string filename); // For one period LP-model.
	void WriteFieldSetToGams(string  filename);
	void WriteAreasToGams(string filename);
	void WritePVSetToGams(string filename);
	void WriteNlevSetToGams(string filename,int seasons);
   void WritePXPToGams(string filename);
   void WriteFieldPeriodsToGams(string filename);
   void writeMacpphaToGams(string filename);
   void SetFixedCrops(string filename);
   void SetFixedCropsAll();
   void AddAreaToIndicators(char * cid, double a, int JBNO);
   void WriteFieldList();

public:
	cropRotation();
	~cropRotation();
	void Initialize(int convent,int irrigat,int daysBetweenIrrigation,int irrigationDelay,
		int FingerFlow,int cattl,double MinArea);
	void DailyUpdate();
	void CalcLP(int seasons);
	void ReceivePlan(char * filename);
	int NumOfFields();
	double FieldArea(int field_no);
   int FieldJBNo(int field_no);
   double GetTotalPremium();
   double GetDiversePlant();
   double GetTotalArea();
   double GetValueofLand();
   linkList <field>* GetFields() {return &fieldList;}
   void InsertGhostField();
   void ReplaceField(field * fP);
	//------ Test functions -----------------------------------------------------
	void ShowAllCurrentPlans(char * filename);
	void ShowAllFuturePlans(char * filename);
	void ShowHist();
   void GiveIndicators();
   void EndBudget();
//   void SetmanureAdjustment(double aValue) {manureAdjustment = aValue;}
};

#endif

