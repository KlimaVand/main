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
#include "manualLabourTech.h"
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
   SetCritical();
	OpenInputFileString("labperid.dat");
	int first,num;
   GetSectionNumbers("maxLabour",&first,&num);
	for(int index=first;index<(first+num);index++)
	{
      FindSection("maxLabour",index);
      GetParameter("maxOwnLabour",&maxOwnLabour[index]);
      GetParameter("maxHiredLabour",&maxHiredLabour[index]);
   }
	CloseInputFile();
}

/****************************************************************************\
Creates operation list from file input
\****************************************************************************/
void technics::CreateOperationList(string aFileName)
{
   SetCritical();
   OpenInputFileString(aFileName);
   int  index,first,num;
   char operationSection[81];

   strcpy(operationSection,"soilTreatTech");
   GetSectionNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      soilTreatTech * soilTreatTechInstance = new soilTreatTech(operationSection,index,this);
      soilTreatTechInstance->ReadParameters(file);
      operationList->InsertLast(soilTreatTechInstance);
   }

   strcpy(operationSection,"harvestTech");
   GetSectionNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      harvestTech * harvestTechInstance = new harvestTech(operationSection,index,this);
      harvestTechInstance->ReadParameters(file);
      operationList->InsertLast(harvestTechInstance);
   }

   strcpy(operationSection,"irrigateTech");
   GetSectionNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      irrigateTech * irrigateTechInstance = new irrigateTech(operationSection,index,this);
      irrigateTechInstance->ReadParameters(file);
      operationList->InsertLast(irrigateTechInstance);
   }

   strcpy(operationSection,"plantCareTech");
   GetSectionNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      plantCareTech * plantCareTechInstance = new plantCareTech(operationSection,index,this);
      plantCareTechInstance->ReadParameters(file);
      operationList->InsertLast(plantCareTechInstance);
   }

   strcpy(operationSection,"sowTech");
   GetSectionNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      sowTech * sowTechInstance = new sowTech(operationSection,index,this);
      sowTechInstance->ReadParameters(file);
      operationList->InsertLast(sowTechInstance);
   }

   UnsetCritical(); // Presently no manual labour operations used
   strcpy(operationSection,"manualLabourTech");
   GetSectionNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      manualLabourTech * manualLabourTechInstance = new manualLabourTech(operationSection,index,this);
      manualLabourTechInstance->ReadParameters(file);
      operationList->InsertLast(manualLabourTechInstance);
   }

   SetCritical();
   strcpy(operationSection,"animalFertTech");
   GetSectionNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      animalFertTech * animalFertTechInstance = new animalFertTech(operationSection,index,this);
      animalFertTechInstance->ReadParameters(file);
      operationList->InsertLast(animalFertTechInstance);
   }

   strcpy(operationSection,"mineralFertTech");
   GetSectionNumbers(operationSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      mineralFertTech * mineralFertTechInstance = new mineralFertTech(operationSection,index,this);
      mineralFertTechInstance->ReadParameters(file);
      operationList->InsertLast(mineralFertTechInstance);
   }
   CloseInputFile();
}

/****************************************************************************\
\****************************************************************************/
void technics::CreateFarmList(string fileName)
{
   OpenInputFileString(fileName);
   int index,first,num;
   char techEquipSection[81];

   UnsetCritical(); 							// Irrigation not necessarily present
   strcpy(techEquipSection,"installation");
   GetSectionNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      installation * installationInstance = new installation(techEquipSection,index,this);
      installationInstance->ReadParameters(file);
      techFarmList->InsertLast(installationInstance);
   }

   SetCritical();
   strcpy(techEquipSection,"implement");
   GetSectionNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      implement * implementInstance = new implement(techEquipSection,index,this);
      implementInstance->ReadParameters(file);
      techFarmList->InsertLast(implementInstance);
   }

   strcpy(techEquipSection,"tractor");
   GetSectionNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      tractor * tractorInstance = new tractor(techEquipSection,index,this);
      tractorInstance->ReadParameters(file);
      techFarmList->InsertLast(tractorInstance);
   }

   strcpy(techEquipSection,"selfPropelled");
   GetSectionNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      selfPropelled * selfPropelledInstance = new selfPropelled(techEquipSection,index,this);
      selfPropelledInstance->ReadParameters(file);
      techFarmList->InsertLast(selfPropelledInstance);
   }

   strcpy(techEquipSection,"manureApplicator");
   GetSectionNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      manureApplic * manureApplicInstance = new manureApplic(techEquipSection,index,this);
      manureApplicInstance->ReadParameters(file);
      techFarmList->InsertLast(manureApplicInstance);
   }
   strcpy(techEquipSection,"forageCutter");
   GetSectionNumbers(techEquipSection,&first,&num);
   for(index=first;index<(first+num);index++)
   {
      forageCutter * forageCutterInstance = new forageCutter(techEquipSection,index,this);
      forageCutterInstance->ReadParameters(file);
      techFarmList->InsertLast(forageCutterInstance);
   }

   CloseInputFile();
}


/****************************************************************************\
Executes a calculation of cost for an operation and updates storages,
technical equipments etc.
\****************************************************************************/
void technics::ExecuteFields(fieldOperationFields * aFieldOp)
{
   const bool DoUpdate=true;
   double        oneCost=0.0;   // dummy, som er noedvendig parameter, men som ikke bruges


   aFieldOp->CalcCost(operationList,
                      techFarmList,
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

   while (aTechEquipInFarmList!=NULL)
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
   while (aTechEquipInFarmList!=NULL)
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
   itoa(actualPeriod,periodName,10);
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
	itoa(IndicatIndex,IndicatIndexString,10);

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

