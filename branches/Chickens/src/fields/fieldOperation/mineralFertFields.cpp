/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "mineralFertFields.h"

/****************************************************************************\
\****************************************************************************/
mineralFertFields::mineralFertFields()
{
	aFertilizer1=NULL;
	aFertilizer2=NULL;
	aFertilizer3=NULL;
}

/****************************************************************************\
\****************************************************************************/
mineralFertFields::mineralFertFields(const mineralFertFields& m)
   : fieldOperationFields(m)
{
   if (m.aFertilizer1)
   	aFertilizer1=new fertilizer(*m.aFertilizer1);
   else
      aFertilizer1=NULL;
   if (m.aFertilizer2)
	   aFertilizer2=new fertilizer(*m.aFertilizer2);
   else
      aFertilizer2=NULL;
   if (m.aFertilizer3)
	   aFertilizer3=new fertilizer(*m.aFertilizer3);
   else
      aFertilizer3=NULL;
}

/****************************************************************************\
\****************************************************************************/
mineralFertFields::~mineralFertFields()
{
	if (aFertilizer1)
		delete aFertilizer1;
	if (aFertilizer2)
		delete aFertilizer2;
	if (aFertilizer3)
		delete aFertilizer3;
}

/****************************************************************************\
\****************************************************************************/
 mineralFertFields*  mineralFertFields::clone() const
{
	 mineralFertFields* m=new  mineralFertFields(*this);
	return m;
}

/****************************************************************************\
\****************************************************************************/
void mineralFertFields::DefineMineralFertFieldsOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,fertilizer * theFertilizer1,fertilizer * theFertilizer2,fertilizer * theFertilizer3)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,yr, mon, d, ar,fieldn,dist);
	if (theFertilizer1)
		aFertilizer1=new fertilizer(*theFertilizer1);
	if (theFertilizer2)
		aFertilizer2=new fertilizer(*theFertilizer2);
	if (theFertilizer3)
		aFertilizer3=new fertilizer(*theFertilizer3);
}

/**
 * Calculate the cost of this operation.
 * If update is true it will write the cost to the indicate file.
 * If update is false it will return the cost in the cost variable
 */
bool mineralFertFields::CalcCost(linkList<operation>    * aOperationList,
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
      theMessage->WarningWithDisplay("mineralFertFields::CalcCost - Operation ",operationNameList[op_id].c_str()," not found in list");
      return false;
   }
   else
      if(area<=0.0)
      {
         theMessage->WarningWithDisplay("mineralFertFields::CalcCost - area is less than zero");
         return false;
      }
      else
      {
         mineralFertTech * mineralFertTechInstance;
         mineralFertTechInstance= (mineralFertTech*)aOperationNode->element;
         mineralFertTechInstance->Setarea(area);
         mineralFertTechInstance->Setdistance(distance);
         mineralFertTechInstance->SetoperationName(operationNameList[op_id]);

         mineralFertTechInstance->SetTheFertilizers(aFertilizer1,aFertilizer2,aFertilizer3);

         if (mineralFertTechInstance->CreateGang(aTechFarmList))
         {
            // All neccesary machines present at farm
	         if(update)
	            mineralFertTechInstance->Update(aTechFarmList);
	                 }
         else { // Buy service
	         if(update) {
               product * hiredService = new product;
					hiredService->Setname("MachinePoolCost");
					theProducts->GiveProductInformation(hiredService);
					hiredService->Setamount(mineralFertTechInstance->GetMachinePoolPrice()*area);
               // theProducts->AddProduct(hiredService); Removed BMP 12.10.2000
		         mineralFertTechInstance->UpdateProducts(true);
               theOutput->AddIndicator(economicIndicator,"07.08 Cost of machine contractors","Dkr",-mineralFertTechInstance->GetMachinePoolPrice()*area);
				   delete hiredService;
            }
         }

         mineralFertTechInstance->DeleteGang();
         //NJH, feb07
         ofstream * file=theMessage->GiveHandle();
		   Output(file);
         theMessage->CloseHandle();

         mineralFertTechInstance->ClearTheFertilizers();
         return true;
      }
}

/****************************************************************************\
\****************************************************************************/
void mineralFertFields::Output(ofstream * fs)
{
	*fs  << operationNameList[op_id] << " ";
   *fs  << opTime->GetDay() << "." << opTime->GetMonth() << "." << opTime->GetYear()<< " ";
   if (aFertilizer1)
		*fs << *aFertilizer1;
   if (aFertilizer2)
		*fs << *aFertilizer2;
   if (aFertilizer3)
		*fs << *aFertilizer3;
   *fs << endl;
}

#ifdef MEASCOPE
/****************************************************************************\
\****************************************************************************/
void mineralFertFields::DefineMineralFertFieldsOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
						int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper, double ar,int fieldn,double dist,fertilizer * theFertilizer1,fertilizer * theFertilizer2,fertilizer * theFertilizer3)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,Startyr,Startmon,Startday,Endyr,Endmon,Endday,aTSum,asoilMoisture,anupper, ar,fieldn,dist);
	if (theFertilizer1)
		aFertilizer1=new fertilizer(*theFertilizer1);
	if (theFertilizer2)
		aFertilizer2=new fertilizer(*theFertilizer2);
	if (theFertilizer3)
		aFertilizer3=new fertilizer(*theFertilizer3);
}
#endif

