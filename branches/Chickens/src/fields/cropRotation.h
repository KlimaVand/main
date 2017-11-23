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
      cattle_cropnum=37,
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

	int conventional;
	bool irrigate;
	int Legal[cattle_cropnum][cattle_cropnum];    // Defines legal crop rotations.
	float Reduc[cattle_cropnum][cattle_cropnum];  // Crop rotation related change in yield.
	Cropstruct CropArray[cattle_cropnum];
	staticCrop staticBase;
	staticCrop * CropPointer;



	linkList <field> fieldList;

   fstream f;

	cropRotation& operator=(const cropRotation& cR); // Dissable the compilers generation of default assignment operator.
	cropRotation(const cropRotation& cR);            // Dissable the compilers generation of default copy constructor.


	static void MakeSequenceInstance(fieldOrder * PP);
	void UpdateFld(field * PP);
	void InitReducTable();
	void InitSingleFields(string filename,int daysBetweenIrrigation, int irrigationDelay,int FingerFlow);
	void InitStaticCrops();
	void InitLegalTable();

	void SetCropPointer(char * crop_id);


	int GiveCropNo(char * crop_id);



	int NumOfCrops();

	void CropId(int crop_no,char * &crop_id);
	double YieldReduction(int field_no,char * crop_id);
	double DistanceToField(int field_no);

   int IsCropGrass(int crop_num);
	int IsCropLegal(int field_no,char * crop_id);

	void PlanField(int field_no,vector<fieldOrder*> &FOL);
   void CorrectCropId(fieldOrder * PP);

   void AddAreaToIndicators(char * cid, double a, int JBNO);
   void WriteFieldList();

public:
	cropRotation();
	~cropRotation();
	void Initialize(int convent,bool irrigat,int daysBetweenIrrigation,int irrigationDelay,
		int FingerFlow);
	void DailyUpdate();

	void ReceivePlan(char * filename);
	int NumOfFields();


   double GetTotalPremium();
   double GetDiversePlant();
   double GetTotalArea();
   double GetValueofLand();
   linkList <field>* GetFields() {return &fieldList;}
   void InsertGhostField();
   void ReplaceField(field * fP);



   void EndBudget();
//   void SetmanureAdjustment(double aValue) {manureAdjustment = aValue;}
};

#endif

