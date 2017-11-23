/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "animalFertFields.h"

/****************************************************************************\
\****************************************************************************/
animalFertFields::animalFertFields()
{
	aManure=nullptr;
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
      aManure=nullptr;
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

/**
 * Changing the time, crop information, operation information and expose time. Does also create a new Manure
 */
void animalFertFields::DefineAnimalFertFieldsOp(operationNames op,id_string crop,id_string cropNm,
		int yr,int mon,int d,double ar,int fieldn,double dist,manure * theManure, double anExposeTime)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,yr, mon, d, ar,fieldn,dist);

	aManure=new manure(*theManure);

   exposeTime=anExposeTime;
}

/**
 * Calculate the cost of this operation.
 * If update is true it will write the cost to the indicate file.
 * If update is false it will return the cost in the cost variable (this part is never called)
 */

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
   while(aOperationNode!=nullptr)
   {
      if(aOperationNode->element->GetoperationNumber()==op_id)
         break;
      aOperationNode=aOperationNode->next;
   }
   if(aOperationNode==nullptr)
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
         //see if we have the equipment on the farm
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
		           theOutput->AddIndicator(708,"07.08 Cost of machine contractors","Dkr",-animalFertTechInstance->GetMachinePoolPrice()*aManure->GetAmount());
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
         ofstream * file=theMessage->GiveHandle();
		   Output(file);

         //end
         animalFertTechInstance->ClearTheManure();
         return true;
   }
}


/**
 * Return a fieldOperationTech if it can be found in aOperationList and has the same op_id as this instance of animalFertFields
 */
fieldOperationTech* animalFertFields::GetfieldOperationTech(linkList<operation>    * aOperationList)
{
   animalFertTech * animalFertTechInstance = nullptr;
   linkList<operation>::PS aOperationNode;

   aOperationList->PeekHead(aOperationNode);
   while(aOperationNode!=nullptr)
   {
      if(aOperationNode->element->GetoperationNumber()==op_id)
         break;
      aOperationNode=aOperationNode->next;
   }
   if(aOperationNode==nullptr)
   {
      theMessage->FatalError("animalFertFields::GetfieldOperationTech - Operation ",operationNameList[op_id].c_str()," not found in list");
      return nullptr;
   }
   else
   {
      animalFertTechInstance= (animalFertTech*)aOperationNode->element;
   }
   return animalFertTechInstance;
}

/**
 * multiply the amount of manure with adjustmentFactor
 */
void animalFertFields::AdjustManure(double adjustmentFactor)
{
 aManure->Setamount(aManure->GetAmount() * adjustmentFactor);

}
/**
 * Write how much manure, manure name and how much N used
 */
void animalFertFields::Output(ofstream * fs)
{
	*fs  << " Manure applied " << aManure->GetName() << " " << aManure->GetAmount()
         			<< " t/ha " << (aManure->GetAmount() * aManure->GetAllN().n*1000.0) << " kgN/ha ";
}


