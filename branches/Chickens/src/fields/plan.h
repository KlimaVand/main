/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef PLAN_H
   #define PLAN_H

#include "fieldOperation/fieldOperationFields.h"
#include "../base/cloneList.h"

class plan
{
	double
      area,
      newArea;
	//! relate this plan to a field with the same index as futureNum
   int futureNum;
	bool
      finished,
      irrigationIncluded,
      automaticIrrigation;

	typedef char string[id_len];
	typedef cloneList<fieldOperationFields> * OpListPType;
	OpListPType OpListP;
	cloneList<fieldOperationFields>::PS NextOperation;

	string CropId;

	plan& operator=(const plan& m); // Dissable the compilers generation of default assignment operator.

public:
	plan();
   plan(const plan& p);
	virtual ~plan();
   plan* clone() const;
	void HandPlanOver(cloneList<fieldOperationFields> * OP,string crop_id,int futrNum);
	fieldOperationFields * ReturnOperation(int year,int month,int day,bool upddate);

	bool AutomaticIrrigation();
	bool FirstOperationToday(int year,int month,int day);
	bool IsThisTheFirst(fieldOperationFields * aOperation);


	bool Finished();
	double GiveArea();
	double GetAdjustArea();
	void GiveCrop(string * crop_id);
   bool NextIsHarvestOp();
	bool NextIsForageHarvestOp();
   bool NextIsFertilizing();
   bool NextIsForcedHarvest();
   int GetFutureNumber(){return futureNum;};
   //inserted by NJH 


   void InsertAsNextOperation(fieldOperationFields * anOperation);
   int GiveNumOps() {return OpListP->NumOfNodes();};
	void RemoveHerbageHarvests();

#ifdef MEASCOPE
   bool CheckTSum();
	bool CheckCriticalSoilMoisture(double aSoilMoisture);
	bool CheckTSumMoist(double moisture);
	bool CheckIfToDo(double moisture);
	bool CheckIfMustDo();
   void CheckIfFinished();
   bool NoOpsThisYear(int year,int month,int day);
   void SetFinished(bool aVal) {finished = aVal;};
   void CheckOps();
   string * GetCropId() {return &CropId;};
#endif
};

#endif

