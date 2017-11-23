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
#include "../data/outputData.h"
#include "../data/systemData.h"
#include "../tools/fileAccess.h"

// Functions for class Operations_manager_class

 operationsManager::operationsManager ():base()

{
	grazing_manager=NULL;
//   manure_manager=NULL;
   cattleFeedingManagerInstance = NULL;
   opsfile=NULL;
}

 operationsManager::~operationsManager()
{
 if (grazing_manager) delete grazing_manager;
// if (manure_manager) delete manure_manager;
 delete cattleFeedingManagerInstance;
 if (opsfile)
 	opsfile->close();
 delete opsfile;
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
   if ((theOutputData->DetailsData.getOperationalDetails())&& (!opsfile))
   	InitialiseFileOutput();

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
   strcpy(fileName,"opsman.dat");
   if(!OpenInputFile(fileName))
		theMessage->FatalError("Operations_manager::ReceivePlan - error in opening ",fileName);
   else
   {
      Setfile(file);
      FindSection("Crops",0);
      SetCritical();
      bool SellAllPlantProducts=true;
      bool SelltheStrawProducts=true;
      bool SelltheNonRoughageProducts=true;
      GetParameter("SellAllPlantProducts",&SellAllPlantProducts);
      if (!SellAllPlantProducts)
      {
         GetParameter("SelltheStrawProducts",&SelltheStrawProducts);
         GetParameter("SelltheNonRoughageProducts",&SelltheNonRoughageProducts);
      }
      theProducts->SetSellthePlantProducts(SellAllPlantProducts);
      theProducts->SetSelltheStrawProducts(SelltheStrawProducts);
      theProducts->SetSelltheNonRoughageProducts(SelltheNonRoughageProducts);
      SetCritical();
	   if (cattleFeedingManagerInstance)  //check to see if cattleFeedingManager is present
	   {
         FindSection("Cattle",0);
         GetParameter("conceptionTime",&conceptionTime);
         GetParameter("lengthOfLactation",&lengthOfLactation);
         UnsetCritical();
         GetParameter("milkQuota",&milkQuota);
         GetParameter("meatExport",&meatExport);
         GetParameter("ageAtWeaning",&ageAtWeaning);
		}
      SetCritical();
   }
   CloseInputFile();
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
         else
	         cattleFeedingManagerInstance->SettargetMilkYieldFactor(factor);
      }
//      cattleFeedingManager->AutoFeed(conceptionTime,lengthOfLactation);
   }

	theManure_manager->ReceivePlan(fileExtension);
}

void operationsManager::autoCropPilot()
{
  for (int i=0;i<theCropRotation->GetFields()->NumOfNodes();i++)
  {
//   theCropRotation->GetnormN()
	}
};


void operationsManager::GetStatus()
{
   if (cattleFeedingManagerInstance && theOutputData->DetailsData.getHerdDetails()==true)  //check to see if cattleFeedingManager is present
   {
   	cattleFeedingManagerInstance->WriteGroupOutput();
	}
   if (grazing_manager)
		grazing_manager->GetStatus();
   if (theOutputData->DetailsData.getOperationalDetails()==true)
   {
      *opsfile << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
      feedItem * aFeedItem = new feedItem();
      for (int i=1;i<3;i++)
      {
         aFeedItem->SetfeedCategory(i);
         theBuildings->GetAvailableStoredFeed(aFeedItem);
         *opsfile << aFeedItem->GetAmount()*aFeedItem->GetdryMatter() << "\t";
         *opsfile << aFeedItem->GetAmount()*aFeedItem->GetfeedUnitsPerItemUnit() << "\t";
      }
      delete aFeedItem;
      *opsfile <<  theBuildings->GetTotalSlurryStored()<< "\t";
      *opsfile <<  1000.0 * theBuildings->GetTotalManureNStored()<< "\t";
      *opsfile << endl;
   }
}

//void Operations_manager_class::Set(char *fileExtension, char *inputDir)
void operationsManager::InitialiseFileOutput()
{
   char filename[20];
   char leader[100];

   opsfile = new fstream();
   sprintf(leader,theSystemData->getOutputDirectory().c_str());
#ifndef __BCplusplus__
   sprintf(filename,"opman.xls");
#else
   sprintf(filename,"opman.xls",Index);
#endif
   strcat(leader,filename);
   opsfile->open(leader,ios::out);
   opsfile->precision(8);

   *opsfile << "Date" << "\t";
   *opsfile << "ConcDM" << "\t" << "ConcSFU" << "\t"<< "RoughDM" << "\t" << "RoughSFU" << "\t"
   					<< "ManureFW" << "\t"<< "ManureN" << "\t";
   *opsfile << endl;
   *opsfile << " " << "\t" << "t" << "\t" << " " << "\t" << "t" << "\t" << " " << "\t"
   					<< "t" << "\t"<< "kg" << "\t";
   *opsfile << endl;
}

void operationsManager::GetNumber()
{
 cattleFeedingManagerInstance->GetNumber();
}
