/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include <fiopan.h>

/****************************************************************************\
\****************************************************************************/
animalFertFields::animalFertFields()
{
	aManure=NULL;
   exposeTime=168;
}

/****************************************************************************\
\****************************************************************************/
animalFertFields::animalFertFields(const animalFertFields& a)
   : fieldOperationFields(a)
{
   if (a.aManure)
	   aManure=new manure(*a.aManure);
   else
      aManure=NULL;
   exposeTime=a.exposeTime;
}

/****************************************************************************\
\****************************************************************************/
animalFertFields::~animalFertFields()
{
	if (aManure)
		delete aManure;
}

/****************************************************************************\
\****************************************************************************/
animalFertFields* animalFertFields::clone() const
{
	animalFertFields* a=new animalFertFields(*this);
	return a;
}

/****************************************************************************\
\****************************************************************************/
void animalFertFields::DefineAnimalFertFieldsOp(operationNames op,id_string crop,id_string cropNm,
		int yr,int mon,int d,double ar,int fieldn,double dist,manure * theManure, double anExposeTime)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,yr, mon, d, ar,fieldn,dist);
	aManure=new manure(*theManure);
   exposeTime=anExposeTime;
}

/****************************************************************************\
\****************************************************************************/
bool animalFertFields::CalcCost(linkList<operation>    * aOperationList,
                                   linkList<techEquip>     * aTechFarmList,
                                   calcLPType               mode,
                                   double                 * cost,
                                   bool                  update)
{
   if (!update && ((mode==hoursCombine) || (mode==hoursMineralFert)
      || (mode==hoursPlantCare) || (mode==varCostTotalNotFert)
      || (mode==varCostMineralFert)))
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
      theMessage->WarningWithDisplay("animalFertFields::CalcCost - Operation ",operationNameList[op_id].c_str()," not found in list");
      return false;
   }
   else
      if(area<=0.0)
      {
         theMessage->WarningWithDisplay("animalFertFields::CalcCost - area is less than zero");
         return false;
      }
      else
      {
         animalFertTech * animalFertTechInstance;
         animalFertTechInstance= (animalFertTech*)aOperationNode->element;
         animalFertTechInstance->Setarea(area);
         animalFertTechInstance->Setdistance(distance);
         animalFertTechInstance->SetoperationName(operationNameList[op_id]);
         animalFertTechInstance->SetcropName(cropName);
         animalFertTechInstance->SetTheManure(aManure);
         if (animalFertTechInstance->CreateGang(aTechFarmList))
         {
            // All neccesary machines present at farm
	         if(update)
	            animalFertTechInstance->Update(aTechFarmList);
	         else
	         {
	            animalFertTechInstance->CalcCost(mode);
	            *cost=animalFertTechInstance->Getcost();
	         }
         }
         else {  // Buy service
	         if(update)
            {
               product * hiredService = new product;
					hiredService->Setname("MachinePoolCost");
					theProducts->GiveProductInformation(hiredService);
					hiredService->Setamount(animalFertTechInstance->GetMachinePoolPrice()*aManure->GetAmount());
               // theProducts->SubtractProduct(hiredService); Removed BMP 12.10.2000
		         animalFertTechInstance->UpdateProducts(true);
               theOutput->AddIndicator(economicIndicator,"07.08 Cost of machine contractors","Dkr",-animalFertTechInstance->GetMachinePoolPrice()*aManure->GetAmount());
				   delete hiredService;
            }
	         else
               if (mode==varCostAnimalFert)
	               *cost=-animalFertTechInstance->GetMachinePoolPrice()*aManure->GetAmount();
               else
                  *cost=0.0;
         }

         animalFertTechInstance->DeleteGang();
         //NJH, feb07
			fstream * file=theMessage->GiveHandle();
		   Output(file);
         theMessage->CloseHandle();
         //end
         animalFertTechInstance->ClearTheManure();
         return true;
   }
}

/****************************************************************************\
\****************************************************************************/
fieldOperationTech* animalFertFields::GetfieldOperationTech(linkList<operation>    * aOperationList)
{
   animalFertTech * animalFertTechInstance = NULL;
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
      theMessage->FatalError("animalFertFields::GetfieldOperationTech - Operation ",operationNameList[op_id].c_str()," not found in list");
      return NULL;
   }
   else
   {
      animalFertTechInstance= (animalFertTech*)aOperationNode->element;
   }
   return animalFertTechInstance;
}

/****************************************************************************\
\****************************************************************************/
void animalFertFields::AdjustManure(double adjustmentFactor)
{
 aManure->Setamount(aManure->GetAmount() * adjustmentFactor);
}

/****************************************************************************\
NJH feb07, output to logfile
\****************************************************************************/
void animalFertFields::Output(fstream * fs)
{
	*fs  << " Manure applied " << aManure->GetName() << " " << aManure->GetAmount()
         			<< " t/ha " << (aManure->GetAmount() * aManure->GetAllN().n*1000.0) << " kgN/ha ";
}

#ifdef MEASCOPE
/****************************************************************************\
\****************************************************************************/
void animalFertFields::DefineAnimalFertFieldsOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
      					int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper,double ar,int fieldn,double dist,manure * theManure, double anExposeTime)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,Startyr,Startmon,Startday,Endyr,Endmon,Endday,aTSum,asoilMoisture,anupper,ar,fieldn,dist);
	aManure=new manure(*theManure);
   exposeTime=anExposeTime;
}
#endif

