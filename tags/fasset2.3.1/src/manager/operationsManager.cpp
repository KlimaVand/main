/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/


// Class: Operations_manager_class
#include "../base/common.h"

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "operationsManager.h"
#include "../building/stable.h"

#include "../base/settings.h"
#include "../tools/fileAccess.h"

// Functions for class Operations_manager_class

 operationsManager::operationsManager ():base()

{
	grazing_manager=NULL;
//   manure_manager=NULL;
   cattleFeedingManagerInstance = NULL;

}

 operationsManager::~operationsManager()
{
 if (grazing_manager) delete grazing_manager;
// if (manure_manager) delete manure_manager;
 delete cattleFeedingManagerInstance;

}

/*******************************************************************
created 1.5.00 by NJH
call after all livestock, fields etc are created
********************************************************************/
void operationsManager::Initialize(livestock * atheLivestock,
									   cropRotation * atheCropRotation,
                              buildings * atheBuildings)
{
  theLivestock = atheLivestock;
  theCropRotation = atheCropRotation;
  theBuildings = atheBuildings;
  if (theLivestock)
  {
   //check to see if grazing manager is required
     for (int i=0;i<theLivestock->getanimalStock()->NumOfNodes();i++)
     {
      if ((strcmp(theLivestock->getanimalStock()->ElementAtNumber(i)->GetName(),"DairyStock")==0)||
         (strcmp(theLivestock->getanimalStock()->ElementAtNumber(i)->GetName(),"BeefStock")==0))
        {
         cattleFeedingManagerInstance = new cattleFeedingManager("FeedManager",0,this);
         cattleFeedingManagerInstance->Initialize(atheLivestock);
         grazing_manager = new grazingManager ();
         grazing_manager->Initialize(atheLivestock,atheCropRotation,atheBuildings,cattleFeedingManagerInstance);
         if (strcmp(theLivestock->getanimalStock()->ElementAtNumber(i)->GetName(),"DairyStock")==0)
            {cattleFeedingManagerInstance->SetdairyPresent(true);}
         if (strcmp(theLivestock->getanimalStock()->ElementAtNumber(i)->GetName(),"BeefStock")==0)
            {cattleFeedingManagerInstance->SetbeefPresent(true);}
//         cattleFeedingManager->ReceivePlan();
        }
     }
//   manure_manager    = new Manure_manager_class(); // added by NJH Nov 2002
//	manure_manager->Initialize(theCropRotation,theBuildings);
	theManure_manager->Initialize(theCropRotation,theBuildings);
   }


}


void operationsManager::daily_update()
{
	double herbageAvailable=0.0;
   linkList <field> *fields_to_graze = NULL;  //fields that will be grazed
   if (cattleFeedingManagerInstance)  //check to see if cattleFeedingManager is present
   {
      if (cattleFeedingManagerInstance->GetdairyPresent())
      {

         cattleFeedingManagerInstance->GetGroupMember(3)->makePregnant(conceptionTime);  //dairy 1st lact early
         cattleFeedingManagerInstance->GetGroupMember(4)->makePregnant(conceptionTime);  //dairy 1st lact late
         cattleFeedingManagerInstance->GetGroupMember(5)->makePregnant(conceptionTime);  //dairy mature lact early
         cattleFeedingManagerInstance->GetGroupMember(6)->makePregnant(conceptionTime);  //dairy mature lact late
         cattleFeedingManagerInstance->GetGroupMember(3)->DryOff(lengthOfLactation);  //dairy 1st early lact
         cattleFeedingManagerInstance->GetGroupMember(4)->DryOff(lengthOfLactation);  //dairy 1st lact late
         cattleFeedingManagerInstance->GetGroupMember(5)->DryOff(lengthOfLactation);  //dairy  mature early lact
         cattleFeedingManagerInstance->GetGroupMember(6)->DryOff(lengthOfLactation);  //dairy mature late lact
         cattleFeedingManagerInstance->weanCalves(1, 0.0);
      }
      if (cattleFeedingManagerInstance->GetbeefPresent())
      {
         cattleFeedingManagerInstance->GetGroupMember(9)->makePregnant(conceptionTime);  //suckler 1st lact early
         cattleFeedingManagerInstance->GetGroupMember(10)->makePregnant(conceptionTime);  //suckler 1st lact late
         cattleFeedingManagerInstance->GetGroupMember(11)->makePregnant(conceptionTime);  //suckler mature lact early
         cattleFeedingManagerInstance->GetGroupMember(12)->makePregnant(conceptionTime);  //suckler mature lact late
         cattleFeedingManagerInstance->GetGroupMember(9)->DryOff(lengthOfLactation);  //Suckler 1st early lact
         cattleFeedingManagerInstance->GetGroupMember(10)->DryOff(lengthOfLactation);  //suckler 1st lact late
         cattleFeedingManagerInstance->GetGroupMember(11)->DryOff(lengthOfLactation);  //suckler  mature early lact
         cattleFeedingManagerInstance->GetGroupMember(12)->DryOff(lengthOfLactation);  //suckler mature late lact
         cattleFeedingManagerInstance->weanCalves(2, ageAtWeaning);
      }
      cattleFeedingManagerInstance->SortAnimals(true);  //sort animals into their appropriate feeding group
      cattleFeedingManagerInstance->CheckDiets(false);   //get current diet for all the animals
//      cattleFeedingManager->checkPasture();
      double grazedHerbageRequired = cattleFeedingManagerInstance->GetHerbageRequired(false);  //herbage required per day
      double zerograzedHerbageRequired = 0.0;// cattleFeedingManager->GetHerbageRequired(true);  //herbage required per day
      int status = 1;
      int numOthers=0;
      if (grazing_manager)  //check to see if grazing manager is present
      {
      	if (grazing_manager->GetenableGrazing())
         {
            grazing_manager->SetGrazableFields();
            fields_to_graze = grazing_manager->daily_update(&numOthers);  //get fields that will be grazed
            if ((grazedHerbageRequired>0.0)||(zerograzedHerbageRequired>0.0))
            {
   //				herbageAvailable=grazing_manager->GetHerbageAvailability(false);  //
               herbageAvailable+=grazing_manager->GetHerbageAvailability(true);  //
               if (herbageAvailable==0.0)  //simulation is starting, so no grazable feed available. Get a winter diet
               {
                     cattleFeedingManagerInstance->CheckDiets(true);
                     status = 1;
               }
               else  //Goldilocks test: find whether the amount of grass available is too little, too much or just right
               {
                  if (zerograzedHerbageRequired>0.0)
   //               	grazing_manager->AutoGrazingPossible();
   {}
                  else
                  {
                     double numberOfDaysSupply = herbageAvailable/grazedHerbageRequired;
                     if (numberOfDaysSupply<grazing_manager->GetturnoutThreshold())  //get a winter diet
                     {
                        cattleFeedingManagerInstance->CheckDiets(true);
                        grazedHerbageRequired=0.0;
                     }
                     else
                     {
                        status = grazing_manager->GetGrazingSupplyStatus(numberOfDaysSupply);
                     }
                  }
               }
		         cattleFeedingManagerInstance->AllocateFieldsForGrazing(fields_to_graze,numOthers);  //tell the cattle which fields they will graze
            }
         }
      }
      cattleFeedingManagerInstance->daily_update(status);
   }

   if ((theLivestock)&&(theTime.GetDay()==1)&&( theTime.GetMonth()==3))
      theManure_manager->DoAdjustment();
   if ((theLivestock)&&(theTime.GetDay()==1)&&( theTime.GetMonth()==6))
      theManure_manager->ExportStoredManure();
}

void operationsManager::ReceivePlan(char *fileExtension, char *inputDir)
{
   char fileName[15];
   commonData data;
   data.setCritical(true);
   data.readFile("opsman.dat");
   strcpy(fileName,"opsman.dat");


   data.FindSection("Crops",0);

      bool SellAllPlantProducts=true;
      bool SelltheStrawProducts=true;
      bool SelltheNonRoughageProducts=true;
      data.FindItem("SellAllPlantProducts",&SellAllPlantProducts);
      if (!SellAllPlantProducts)
      {
    	  data.FindItem("SelltheStrawProducts",&SelltheStrawProducts);
    	  data.FindItem("SelltheNonRoughageProducts",&SelltheNonRoughageProducts);
      }
      theProducts->SetSellthePlantProducts(SellAllPlantProducts);
      theProducts->SetSelltheStrawProducts(SelltheStrawProducts);
      theProducts->SetSelltheNonRoughageProducts(SelltheNonRoughageProducts);

	   if (cattleFeedingManagerInstance)  //check to see if cattleFeedingManager is present
	   {
		   data.FindSection("Cattle",0);
         data.FindItem("conceptionTime",&conceptionTime);
         data.FindItem("lengthOfLactation",&lengthOfLactation);
         data.setCritical(false);
         data.FindItem("milkQuota",&milkQuota);
         data.FindItem("meatExport",&meatExport);
         data.FindItem("ageAtWeaning",&ageAtWeaning);
		}


fileAccess hd;
   hd.changeDir(inputDir);
   if (cattleFeedingManagerInstance)  //check to see if cattleFeedingManager is present
   {
      cattleFeedingManagerInstance->ReceivePlan();
      if (cattleFeedingManagerInstance->GetautoFeed())
	      cattleFeedingManagerInstance->AutoFeed(conceptionTime,lengthOfLactation);
      cattleFeedingManagerInstance->CalcTurnoutandHousingDates(false);   //is only true for zero grazing - which does not work yet
      cattleFeedingManagerInstance->SortAnimals(false);  //sort animals into their appropriate feeding group
      grazing_manager->ReceivePlan(fileName);
      if (milkQuota>0.0)  //need to regulate feeding to achieve a target annual yield
      {
         double potentialMilkYield = cattleFeedingManagerInstance->GetPotentialMilkYield(lengthOfLactation);
         if (potentialMilkYield==0.0)
         	theMessage->FatalError("OperationsManager: milk quota set but no milk will be produced!");
         double factor = milkQuota/potentialMilkYield;
         if (factor>1.0)
         	theMessage->Warning("OperationsManager: milk quota higher than potential milk yield!");

      }
//      cattleFeedingManager->AutoFeed(conceptionTime,lengthOfLactation);
   }

	theManure_manager->ReceivePlan(fileExtension);
}




void operationsManager::GetStatus()
{
   if (cattleFeedingManagerInstance && globalSettings->DetailsData.getHerdDetails()==true)  //check to see if cattleFeedingManager is present
   {
   	cattleFeedingManagerInstance->WriteGroupOutput();
	}
   if (grazing_manager)
		grazing_manager->GetStatus();

}


