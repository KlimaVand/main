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
                                   double                 * cost,
                                   bool                  update)
{
   if (!update)
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

         animalFertTechInstance->SetTheManure(aManure);
         //see if we have the equipment on the farm
         if (animalFertTechInstance->CreateGang(aTechFarmList))
         {
            // All neccesary machines present at farm
	         if(update)
	            animalFertTechInstance->Update(aTechFarmList);

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
	                 }

         animalFertTechInstance->DeleteGang();
         //NJH, feb07
         ofstream * file=theMessage->GiveHandle();
		   Output(file);
         theMessage->CloseHandle();
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


/**
 * Write how much manure, manure name and how much N used
 */
void animalFertFields::Output(ofstream * fs)
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

