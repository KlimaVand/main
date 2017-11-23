/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "technics.h"
#include "fieldOperationTech.h"
#include "soilTreatTech.h"
#include "sowTech.h"
#include "harvestTech.h"
#include "irrigateTech.h"
#include "animalFertTech.h"
#include "mineralFertTech.h"
#include "plantCareTech.h"

#include "techEquip.h"
#include "../fields/fieldOperation/manualLabourFields.h"
#include "../products/product.h"
#include "../products/products.h"
#include "../base/unix_util.h"
/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
technics::technics(const char * aName, const int aIndex, const base * aOwner)
   : base(aName, aIndex, aOwner)
{
   operationList = new linkList <operation>;
   techFarmList = new linkList <techEquip>;
   thePeriods = new periods;
   for (int i=0; i<maxPeriods;i++)
   	maxOwnLabour[i] = maxHiredLabour[i] =0.0;

   labourSum=0.0;
   currentPeriod = 0;


}

/****************************************************************************\
  Destructor
\****************************************************************************/
technics::~technics()
{
  	delete operationList;
 	delete techFarmList;
   delete thePeriods;
}

/****************************************************************************\
Initializes the operation list and two technical equipment lists
\****************************************************************************/
void technics::Initialize(string opFileName, string techFileNameFarm)
{
   CreateOperationList(opFileName);
   CreateFarmList(techFileNameFarm);
   thePeriods->Initialize("per_tech.dat");

   currentPeriod = 0;
   commonData data;
   data.setCritical(true);
   data.readFile("labperid.dat");
	int first,num;
	data.getSectionsNumbers("maxLabour",&first,&num);
	for(int index=first;index<=(first+num);index++)
	{
		data.FindSection("maxLabour",index);
		data.FindItem("maxOwnLabour",&maxOwnLabour[index]);
		data.FindItem("maxHiredLabour",&maxHiredLabour[index]);
   }

}

/****************************************************************************\
Creates operation list from file input
\****************************************************************************/
void technics::CreateOperationList(string aFileName)
{

   int  index,first,num;
   char operationSection[81];
   commonData * data =new commonData();
   data->readFile(aFileName);
   data->setCritical(true);
   data->getSectionsNumbers("soilTreatTech",&first,&num);

   strcpy(operationSection,"soilTreatTech");
   for(index=first;index<(first+num);index++)
   {
      soilTreatTech * soilTreatTechInstance = new soilTreatTech(operationSection,index,this);
      soilTreatTechInstance->ReadParameters(data);
      operationList->InsertLast(soilTreatTechInstance);
   }
   strcpy(operationSection,"harvestTech");
   data->getSectionsNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      harvestTech * harvestTechInstance = new harvestTech(operationSection,index,this);
      harvestTechInstance->ReadParameters(data);
      operationList->InsertLast(harvestTechInstance);
   }

   strcpy(operationSection,"irrigateTech");
   data->getSectionsNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      irrigateTech * irrigateTechInstance = new irrigateTech(operationSection,index,this);
      irrigateTechInstance->ReadParameters(data);
      operationList->InsertLast(irrigateTechInstance);
   }

   strcpy(operationSection,"plantCareTech");
   data->getSectionsNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      plantCareTech * plantCareTechInstance = new plantCareTech(operationSection,index,this);
      plantCareTechInstance->ReadParameters(data);
      operationList->InsertLast(plantCareTechInstance);
   }

   strcpy(operationSection,"sowTech");
   data->getSectionsNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      sowTech * sowTechInstance = new sowTech(operationSection,index,this);
      sowTechInstance->ReadParameters(data);
      operationList->InsertLast(sowTechInstance);
   }



   data->setCritical(true);
   strcpy(operationSection,"animalFertTech");
   data->getSectionsNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      animalFertTech * animalFertTechInstance = new animalFertTech(operationSection,index,this);
      animalFertTechInstance->ReadParameters(data);
      operationList->InsertLast(animalFertTechInstance);
   }

   strcpy(operationSection,"mineralFertTech");
   data->getSectionsNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      mineralFertTech * mineralFertTechInstance = new mineralFertTech(operationSection,index,this);
      mineralFertTechInstance->ReadParameters(data);
      operationList->InsertLast(mineralFertTechInstance);
   }
   strcpy(operationSection,"mineralFertTech");
   data->getSectionsNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      mineralFertTech * mineralFertTechInstance = new mineralFertTech(operationSection,index,this);
      mineralFertTechInstance->ReadParameters(data);
      operationList->InsertLast(mineralFertTechInstance);
   }
   delete data;
}

/****************************************************************************\
\****************************************************************************/
void technics::CreateFarmList(string fileName)
{

   int index,first,num;
   char techEquipSection[81];
   commonData * data=new commonData();
   data->readFile(fileName);
   data->setCritical(false); 							// Irrigation not necessarily present
   strcpy(techEquipSection,"installation");
   data->getSectionsNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      installation * installationInstance = new installation(techEquipSection,index,this);
      installationInstance->ReadParameters(data);
      techFarmList->InsertLast(installationInstance);
   }

   data->setCritical(true);
   strcpy(techEquipSection,"implement");
   data->getSectionsNumbers(techEquipSection,&first,&num);
 for(index=first;index<(first+num);index++)
   {
      implement * implementInstance = new implement(techEquipSection,index,this);
      implementInstance->ReadParameters(data);
      techFarmList->InsertLast(implementInstance);
   }
strcpy(techEquipSection,"tractor");
   data->getSectionsNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      tractor * tractorInstance = new tractor(techEquipSection,index,this);
      tractorInstance->ReadParameters(data);
      techFarmList->InsertLast(tractorInstance);
   }

   strcpy(techEquipSection,"selfPropelled");
   data->getSectionsNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      selfPropelled * selfPropelledInstance = new selfPropelled(techEquipSection,index,this);
      selfPropelledInstance->ReadParameters(data);
      techFarmList->InsertLast(selfPropelledInstance);
   }

   strcpy(techEquipSection,"manureApplicator");
   data->getSectionsNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      manureApplic * manureApplicInstance = new manureApplic(techEquipSection,index,this);
      manureApplicInstance->ReadParameters(data);
      techFarmList->InsertLast(manureApplicInstance);
   }
   strcpy(techEquipSection,"forageCutter");
   data->getSectionsNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      forageCutter * forageCutterInstance = new forageCutter(techEquipSection,index,this);
      forageCutterInstance->ReadParameters(data);
      techFarmList->InsertLast(forageCutterInstance);
   }
   delete data;

}


/****************************************************************************\
Calculates cost for each operation and accumulates to total costs
and returns result
\****************************************************************************/
double technics::CalcLPFields(cloneList <fieldOperationFields> * aFieldOpList, calcLPType mode, period * per)
{
   const bool DoNotUpdate=false;
   double        oneCost=0.0;
   double        allCost=0.0;
   double        area=0.0;
   cloneList<fieldOperationFields>::PS aFieldOp;

   aFieldOpList->PeekHead(aFieldOp);
   area=aFieldOp->element->GetArea();
   while (aFieldOp!=nullptr)
   {
      oneCost=0.0;
#ifndef MEASCOPE
      if(per->IsDateInPeriod(aFieldOp->element->GetMonth(),aFieldOp->element->GetDay()))
#endif
      if(aFieldOp->element->CalcCost(operationList,
                                     techFarmList,
                                     mode,
                                     &oneCost,
                                     DoNotUpdate))
      {
         allCost+=oneCost;
      }
      aFieldOp=aFieldOp->next;
   }
   return allCost/area;
}

/****************************************************************************\
Executes a calculation of cost for an operation and updates storages,
technical equipments etc.
\****************************************************************************/
void technics::ExecuteFields(fieldOperationFields * aFieldOp)
{
   const bool DoUpdate=true;
   double        oneCost=0.0;   // dummy, som er noedvendig parameter, men som ikke bruges
   calcLPType    mode=noCalcLP; // dummy, som er noedvendig parameter, men som ikke bruges

   aFieldOp->CalcCost(operationList,
                      techFarmList,
                      mode,
                      &oneCost,
                      DoUpdate);
}

/****************************************************************************\
\****************************************************************************/
void technics::ExecuteAnimals()
{
}

/****************************************************************************\
\****************************************************************************/
void technics::Reset()
{
   linkList <techEquip>::PS aTechEquipInFarmList;
   techFarmList->PeekHead(aTechEquipInFarmList);

   while (aTechEquipInFarmList!=nullptr)
   {
      aTechEquipInFarmList->element->UpdateHours();
      aTechEquipInFarmList=aTechEquipInFarmList->next;
   }
}

/****************************************************************************\
\****************************************************************************/
void technics::YearlyCalc()
{
	//this loop added by LKK calculates depreciation, maintenance and insurance */
   linkList <techEquip>::PS aTechEquipInFarmList;
	techFarmList->PeekHead(aTechEquipInFarmList);
   while (aTechEquipInFarmList!=nullptr)
   {
      aTechEquipInFarmList->element->GiveIndicators();
      aTechEquipInFarmList=aTechEquipInFarmList->next;
   }
}

/****************************************************************************\
\****************************************************************************/
void technics::UpdateFieldLabour(double hours)
{
   int actualPeriod = thePeriods->GetActualPeriodNumber(theTime.GetMonth(),theTime.GetDay());
   char periodName[40];
   _itoa(actualPeriod, periodName, 10);
    if (currentPeriod != actualPeriod)
   {
      labourSum = 0.0;
      currentPeriod = actualPeriod;
   }

   double ownAvail = maxOwnLabour[currentPeriod]-labourSum;
   double freeHours=max(0.0,min(ownAvail,hours));
   double hiredHours=hours-freeHours;

   if (hiredHours>0.0)
   {
      product * hiredLabour = new product;
	   hiredLabour->Setname("hiredHours");
	   theProducts->GiveProductInformation(hiredLabour);
   	hiredLabour->Setamount(hiredHours);
      theProducts->SubtractProduct(hiredLabour);
      delete hiredLabour;
   }

   labourSum+=hours;
   int IndicatIndex = 11 + atoi(periodName);
   char IndicatIndexString[5]; // Changed from 2 to prevent memory errors BMP
   _itoa(IndicatIndex, IndicatIndexString, 10);

   theOutput->AddIndicator(economicIndicator,"13.10 Labour, field","hours",hours);

   string LabourPeriod = "13."+ (string) IndicatIndexString + " Field-labour " + (string) periodName;
   theOutput->AddIndicator(economicIndicator,(char*) LabourPeriod.c_str(),"hours",hours);
   theOutput->AddIndicator(economicIndicator,"13.50 Labour, maintance and management","hours",0.15*hours);  // 15%  maintance and management
	theOutput->AddIndicator(economicIndicator,"13.01 Total labour","hours",hours*1.15);
}

/****************************************************************************\
\****************************************************************************/
operation * technics::GetFieldTechnique(fieldOperationFields * aFieldOp)
{
   return aFieldOp->GetfieldOperationTech(operationList);
}