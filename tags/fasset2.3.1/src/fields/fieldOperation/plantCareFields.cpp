/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  $Archive$
   $Author$
     $Date$
 $Revision$
 ****************************************************************************
      $Log$
\****************************************************************************/

#include "../../base/common.h"
#include "plantCareFields.h"

plantCareFields::plantCareFields()
{
	aChemical1=NULL;
	aChemical2=NULL;
	aChemical3=NULL;
	aWater=NULL;
}

plantCareFields::plantCareFields(const plantCareFields& p)
   : fieldOperationFields(p)
{
   if (p.aChemical1)
   	aChemical1=new chemical(*p.aChemical1);
   else
      aChemical1=NULL;
   if (p.aChemical2)
    	aChemical2=new chemical(*p.aChemical2);
   else
      aChemical2=NULL;
   if (p.aChemical3)
  	   aChemical3=new chemical(*p.aChemical3);
   else
      aChemical3=NULL;
   if (p.aWater)
  	   aWater=new water(*p.aWater);
   else
      aWater=NULL;
}

plantCareFields::~plantCareFields()
{
	if (aChemical1)
		delete aChemical1;
	if (aChemical2)
		delete aChemical2;
	if (aChemical3)
		delete aChemical3;
	if (aWater)
		delete aWater;
}

plantCareFields* plantCareFields::clone() const
{
	plantCareFields* p=new plantCareFields(*this);
	return p;
}

void plantCareFields::DefinePlantCareFieldsOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,chemical * chem1,chemical * chem2,chemical * chem3,water * wat)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,yr, mon, d, ar,fieldn,dist);
	aChemical1=new chemical(*chem1);
	if (chem2)
		aChemical2=new chemical(*chem2);
	if (chem3)
		aChemical3=new chemical(*chem3);
	aWater=new water(*wat);
}

/**
 * Calculate the cost of this operation.
 * If update is true it will write the cost to the indicate file.
 * If update is false it will return the cost in the cost variable
 */

bool plantCareFields::CalcCost(linkList<operation>    * aOperationList,
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
      theMessage->WarningWithDisplay("plantCareFields::CalcCost - Operation ",operationNameList[op_id].c_str()," not found in list");
      return false;
   }
   else
      if(area<=0.0)
      {
         theMessage->WarningWithDisplay("plantCareFields::CalcCost - area is less than zero");
         return false;
      }
      else
      {
         plantCareTech * plantCareTechInstance;
         plantCareTechInstance= (plantCareTech*)aOperationNode->element;
         plantCareTechInstance->Setarea(area);
         plantCareTechInstance->Setdistance(distance);
         plantCareTechInstance->SetoperationName(operationNameList[op_id]);

         plantCareTechInstance->SetTheWaterAndChemicals(aWater,aChemical1,aChemical2,aChemical3);

         if (plantCareTechInstance->CreateGang(aTechFarmList))
         {
            // All neccesary machines present at farm
	         if(update)
	            plantCareTechInstance->Update(aTechFarmList);

         }
         else { // Buy service
	         if(update) {
               product * hiredService = new product;
					hiredService->Setname("MachinePoolCost");
					theProducts->GiveProductInformation(hiredService);
					hiredService->Setamount(plantCareTechInstance->GetMachinePoolPrice()*area); 
               // theProducts->AddProduct(hiredService); Removed BMP 12.10.2000
               plantCareTechInstance->UpdateProducts(true);
               theOutput->AddIndicator(economicIndicator,"07.08 Cost of machine contractors","Dkr",-plantCareTechInstance->GetMachinePoolPrice()*area); 
				   delete hiredService;
            }
         }

         plantCareTechInstance->DeleteGang();
         plantCareTechInstance->ClearTheWaterAndChemicals();
         return true;
      }
}



