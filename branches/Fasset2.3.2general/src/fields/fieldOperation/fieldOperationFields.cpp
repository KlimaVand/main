/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "fieldOperationFields.h"

/****************************************************************************\
\****************************************************************************/
fieldOperationFields::fieldOperationFields()
{
	fieldno  = 0;
	area     = 0.0;
	distance = 0.0;
   op_id    = operationNames(0); //Merged March 2009 was -1
   strcpy(crop_id,"XX");
	strcpy(cropName,"XX");
	opTime = new bsTime();
#ifdef MEASCOPE
	opStart = NULL;
	opEnd = NULL;
#endif
}

/****************************************************************************\
\****************************************************************************/
fieldOperationFields::fieldOperationFields(const fieldOperationFields& f)
{
	fieldno=f.fieldno;
	area=f.area;
	distance=f.distance;
  	op_id=f.op_id;
	strcpy(crop_id,f.crop_id);
	strcpy(cropName,f.cropName);
	opTime = new bsTime(*f.opTime);
#ifdef MEASCOPE
	opStart = new bsTime(*f.opStart);
	opEnd = new bsTime(*f.opEnd);
#endif
}

/****************************************************************************\
\****************************************************************************/
fieldOperationFields::~fieldOperationFields()
{
	delete opTime;
#ifdef MEASCOPE
	delete opStart;
	delete opEnd;
#endif
}

/**
 * Define the operation. Should this be a constructor?
 */
void fieldOperationFields::DefineOp(operationNames op,
												string      crop,
												string      cropNm,
												int            yr,
												int            mon,
												int            d,
												double         ar,
												int            fieldn,
												double         dist)
{
	op_id=op;
	strcpy(crop_id,crop.c_str());
	strcpy(cropName,cropNm.c_str());
   opTime->SetTime(d,mon,yr);
	area=ar;
	fieldno=fieldn;
	distance=dist;
}

/****************************************************************************\
\****************************************************************************/
ostream &operator<<(ostream &os,const fieldOperationFields &aFieldop)
{
	os  << operationNameList[aFieldop.op_id];
	os << " Crop: " << aFieldop.crop_id << "  Size: " << aFieldop.area << " ha ";
   if (strcmp(aFieldop.crop_id,"XX")==0)
      cout << "Unspecified crop in operation - serious fault!!!" << endl;

#ifdef MEASCOPE
	if (!aFieldop.opStart)
#endif
      os << aFieldop.opTime->GetDay() << "-" << aFieldop.opTime->GetMonth() << "-" << aFieldop.opTime->GetYear() << endl;

	return os;
}





#ifdef MEASCOPE
/****************************************************************************\
\****************************************************************************/
void fieldOperationFields::DefineOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
      					int Endyr,int Endmon,int Endday,double aTSum, double aSoilMoisture, bool anupper, double ar,int fieldn,double dist)
{
	if ((opEnd)||(opStart))
   	theMessage->FatalError("fieldOperationFields::DefineOp - date for this operation has already been defined");
   else
   {
		opStart = new bsTime();
		opEnd = new bsTime();
   }
	op_id=op;
	strcpy(crop_id,crop);
	strcpy(cropName,cropNm);
   opStart->SetTime(Startday,Startmon,Startyr);
   opEnd->SetTime(Endday,Endmon,Endyr);
   if (*opStart > *opEnd)
   {
   	theMessage->WarningWithDisplay("Field no: ", fieldno);
   	theMessage->WarningWithDisplay("Crop: ", crop);
      theMessage->WarningWithDisplay("fieldOperationFields::DefineOp - field no and op name ",
							GetFieldNumber(),operationNameList[GetOperationId()].c_str());
   	theMessage->FatalError("fieldOperationFields::DefineOp - start date for operation is after end date");
   }
   TSum=aTSum;
   soilMoisture=aSoilMoisture;
   upper=anupper;
	area=ar;
	fieldno=fieldn;
	distance=dist;
}
#endif

