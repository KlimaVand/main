/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "sowFields.h"
#include "../../products/products.h"

/****************************************************************************\
\****************************************************************************/
sowFields::sowFields()
{
   numOfPlants=0;
}

/****************************************************************************\
\****************************************************************************/
sowFields::sowFields(const sowFields& s)
   : fieldOperationFields(s)
{
   PVStrategy=s.PVStrategy;
   reduction=s.reduction;
   numOfPlants=s.numOfPlants;
	for (int i=0;i<numOfPlants;i++)
   {
   	plants[i].seeds=new seed(*s.plants[i].seeds);
   	strcpy(plants[i].crop_id,s.plants[i].crop_id);
   }
}

/****************************************************************************\
\****************************************************************************/
sowFields::~sowFields()
{
	for (int i=0;i<numOfPlants; i++)
  		if (plants[i].seeds)
         delete plants[i].seeds;
}

/****************************************************************************\
\****************************************************************************/
sowFields* sowFields::clone() const
{
	sowFields* s=new sowFields(*this);
	return s;
}

/****************************************************************************\
\****************************************************************************/
void sowFields::AddSeed(seed * s,string crop)
{
   if (numOfPlants==0)
      theMessage->FatalError("sowFields::AddSeed - function can not be called yet");
   if(numOfPlants>MaxPlants)
	   theMessage->FatalError("sowFields::AddSeed - To many plants");
	strcpy(plants[numOfPlants].crop_id,crop.c_str());
	plants[numOfPlants].seeds=new seed(*s);
   numOfPlants++;
}

/****************************************************************************\
\****************************************************************************/
void sowFields::DefineSowFieldsOp(operationNames op,id_string crop,id_string cropNm,
	int yr,int mon,int d,double ar,int fieldn,double dist,int aPVStrategy,double reduc,seed * theSeed)
{
   if (numOfPlants>0)
      theMessage->FatalError("sowFields::DefineSowFieldsOp - no plants should be added at this stage");
   if(numOfPlants>MaxPlants)
  	   theMessage->FatalError("sowFields::AddSeed - To many plants");
	fieldOperationFields::DefineOp(op,crop,cropNm,yr, mon, d, ar,fieldn,dist);
	strcpy(plants[0].crop_id,crop);
   PVStrategy = aPVStrategy;
   reduction=reduc;
	plants[0].seeds=new seed(*theSeed);
   numOfPlants++;
}

/**
 * Calculate the cost of this operation.
 * If update is true it will write the cost to the indicate file.
 * If update is false it will return the cost in the cost variable
 */
bool sowFields::CalcCost(linkList<operation>    * aOperationList,
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
      theMessage->WarningWithDisplay("sowFields::CalcCost - Operation ",operationNameList[op_id].c_str()," not found in list");
      return false;
   }
   else
      if(area<=0.0)
      {
         theMessage->WarningWithDisplay("sowFields::CalcCost - area is less than zero");
         return false;
      }
      else
      {
         sowTech * sowTechInstance;
         sowTechInstance = (sowTech*)aOperationNode->element;
         sowTechInstance->Setarea(area);
         sowTechInstance->Setdistance(distance);
         sowTechInstance->SetoperationName(operationNameList[op_id]);

         sowTechInstance->ResetSeeds();
         for (int i=0;i<numOfPlants;i++)
            sowTechInstance->AddSeed(plants[i].seeds);

         if (sowTechInstance->CreateGang(aTechFarmList))
         {
            // All neccesary machines present at farm
	         if(update)
	            sowTechInstance->Update(aTechFarmList);

         }
         else { // Buy service
	         if(update) {
               product * hiredService = new product;
					hiredService->Setname("MachinePoolCost");
					theProducts->GiveProductInformation(hiredService);
					hiredService->Setamount(sowTechInstance->GetMachinePoolPrice()*area);
               //theProducts->AddProduct(hiredService); Removed BMP 12.10.2000
               sowTechInstance->UpdateProducts(true);
               theOutput->AddIndicator(economicIndicator,"07.08 Cost of machine contractors","Dkr",-sowTechInstance->GetMachinePoolPrice()*area); 
				   delete hiredService;
            }

         }

         sowTechInstance->DeleteGang();
         //NJH, feb07
         ofstream * file=theMessage->GiveHandle();
		   Output(file);
         theMessage->CloseHandle();

         sowTechInstance->ClearTheSeeds();
         return true;
      }
}

/****************************************************************************\
\****************************************************************************/
void sowFields::Output(ofstream * fs)
{
	for (int i=0;i<numOfPlants;i++)
   {
   	*fs << plants[i].crop_id << " ";
   	*fs << *plants[i].seeds;
   }
}

#ifdef MEASCOPE
/****************************************************************************\
\****************************************************************************/
void sowFields::DefineSowFieldsOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
      					int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper, double ar,int fieldn,double dist,int aPVStrategy,double reduc,seed * theSeed)
{
   if (numOfPlants>0)
      theMessage->FatalError("sowFields::DefineSowFieldsOp - no plants should be added at this stage");
	fieldOperationFields::DefineOp(op,crop,cropNm,Startyr,Startmon,Startday,Endyr,Endmon,Endday,aTSum,asoilMoisture,anupper, ar,fieldn,dist);
	strcpy(plants[0].crop_id,crop);
   PVStrategy = aPVStrategy;
   reduction=reduc;
	plants[0].seeds=new seed(*theSeed);
   numOfPlants++;
}
#endif

