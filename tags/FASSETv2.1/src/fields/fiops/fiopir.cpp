/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "fiopir.h"

/****************************************************************************\
\****************************************************************************/
irrigateFields::irrigateFields()
{
	aWater=NULL;
   automaticIrrigation=false;
}

/****************************************************************************\
\****************************************************************************/
irrigateFields::irrigateFields(const irrigateFields& i)
   : fieldOperationFields(i)
{
   if (i.aWater)
   	aWater=new water(*i.aWater);
   else
      aWater=NULL;
   automaticIrrigation=i.automaticIrrigation;
}

/****************************************************************************\
\****************************************************************************/
irrigateFields::~irrigateFields()
{
	if (aWater)
		delete aWater;
}

/****************************************************************************\
\****************************************************************************/
irrigateFields* irrigateFields::clone() const
{
	irrigateFields* i=new irrigateFields(*this);
	return i;
}

/****************************************************************************\
\****************************************************************************/
void irrigateFields::DefineIrrigateFieldsOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,
                           double ar,int fieldn,double dist,water * theWater,bool automatic)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,yr, mon, d, ar,fieldn,dist);
	aWater=new water(*theWater);
   automaticIrrigation=automatic;
}

/****************************************************************************\
\****************************************************************************/
bool irrigateFields::CalcCost(linkList<operation>    * aOperationList,
                                 linkList<techEquip>     * aTechFarmList,
                                 calcLPType               mode,
                                 double                 * cost,
                                 bool                  update)
{
   if (!update && ((mode==hoursCombine) || (mode==hoursAnimalFert)
      || (mode==hoursMineralFert) || (mode==hoursPlantCare)
      || (mode==varCostAnimalFert) || (mode==varCostMineralFert)))
      return false;
// Calculate cost of operation
   linkList<operation>::PS aOperationNode;

   aOperationList->PeekHead(aOperationNode);
   while(aOperationNode!=NULL)
   {
      if(aOperationNode->element->GetoperationNumber()==op_id)
         break;
      aOperationNode=aOperationNode->next;
   }
   if(aOperationNode==NULL)
   {
      theMessage->WarningWithDisplay("irrigateFields::CalcCost - Operation ",operationNameList[op_id].c_str()," not found in list");
      return false;
   }
   else
      if(area<=0.0)
      {
         theMessage->WarningWithDisplay("irrigateFields::CalcCost - area is less than zero");
         return false;
      }
      else
      {
         irrigateTech * irrigateTechInstance;
         irrigateTechInstance= (irrigateTech*)aOperationNode->element;

         irrigateTechInstance->Setarea(area);
         irrigateTechInstance->Setdistance(distance);
         irrigateTechInstance->SetoperationName(operationNameList[op_id]);
         irrigateTechInstance->SetcropName(cropName);
         irrigateTechInstance->SetTheWater(aWater);

         irrigateTechInstance->CreateGang(aTechFarmList);

         if(update)
            irrigateTechInstance->Update(aTechFarmList);
         else
         {
            irrigateTechInstance->CalcCost(mode);
            *cost=irrigateTechInstance->Getcost();
         }

         irrigateTechInstance->DeleteGang();
         irrigateTechInstance->ClearTheWater();
         return true;
      }
}

#ifdef MEASCOPE
/****************************************************************************\
\****************************************************************************/
void irrigateFields::DefineIrrigateFieldsOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
      					int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper, double ar,int fieldn,double dist, water * theWater,bool automatic)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,Startyr,Startmon,Startday,Endyr,Endmon,Endday,aTSum,asoilMoisture,anupper,ar,fieldn,dist);
	aWater=new water(*theWater);
   automaticIrrigation=automatic;
}
#endif


