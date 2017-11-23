/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/


// Class: Operations_manager_class
#include <common.h>
#include <output.h>
//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "operations_manager_class.h"
#include "stable.h"
#include <direct.h>
// Functions for class Operations_manager_class

 Operations_manager_class::Operations_manager_class ()

{
	grazing_manager=NULL;
//   manure_manager=NULL;
   cattleFeedingManager = NULL;
   opsfile=NULL;
}

 Operations_manager_class::~Operations_manager_class()
{
 if (grazing_manager) delete grazing_manager;
// if (manure_manager) delete manure_manager;
 delete cattleFeedingManager;
 if (opsfile)
 	opsfile->close();
 delete opsfile;
}

/*******************************************************************
created 1.5.00 by NJH
call after all livestock, fields etc are created
********************************************************************/
void Operations_manager_class::Initialize(livestock * atheLivestock,
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
         cattleFeedingManager = new cattlefeeding_manager_class("FeedManager",0,this);
         cattleFeedingManager->Initialize(atheLivestock);
         grazing_manager = new grazing_manager_class ();
         grazing_manager->Initialize(atheLivestock,atheCropRotation,atheBuildings,cattleFeedingManager);
         if (strcmp(theLivestock->getanimalStock()->ElementAtNumber(i)->GetName(),"DairyStock")==0)
            {cattleFeedingManager->SetdairyPresent(true);}
         if (strcmp(theLivestock->getanimalStock()->ElementAtNumber(i)->GetName(),"BeefStock")==0)
            {cattleFeedingManager->SetbeefPresent(true);}
//         cattleFeedingManager->ReceivePlan();
        }
     }
//   manure_manager    = new Manure_manager_class(); // added by NJH Nov 2002
//	manure_manager->Initialize(theCropRotation,theBuildings);
	theManure_manager->Initialize(theCropRotation,theBuildings);
   if ((theOutputControl->GetOperationalDetails())&& (!opsfile))
   	InitialiseFileOutput();
  }
}


void Operations_manager_class::daily_update()
{
	double herbageAvailable=0.0;
   linkList <field> *fields_to_graze;  //fields that will be grazed
   if (cattleFeedingManager)  //check to see if cattleFeedingManager is present
   {
      if (cattleFeedingManager->GetdairyPresent())
      {

         cattleFeedingManager->GetGroupMember(3)->makePregnant(conceptionTime);  //dairy 1st lact early
         cattleFeedingManager->GetGroupMember(4)->makePregnant(conceptionTime);  //dairy 1st lact late
         cattleFeedingManager->GetGroupMember(5)->makePregnant(conceptionTime);  //dairy mature lact early
         cattleFeedingManager->GetGroupMember(6)->makePregnant(conceptionTime);  //dairy mature lact late
         cattleFeedingManager->GetGroupMember(3)->DryOff(lengthOfLactation);  //dairy 1st early lact
         cattleFeedingManager->GetGroupMember(4)->DryOff(lengthOfLactation);  //dairy 1st lact late
         cattleFeedingManager->GetGroupMember(5)->DryOff(lengthOfLactation);  //dairy  mature early lact
         cattleFeedingManager->GetGroupMember(6)->DryOff(lengthOfLactation);  //dairy mature late lact
         cattleFeedingManager->weanCalves(1, 0.0);
      }
      if (cattleFeedingManager->GetbeefPresent())
      {
         cattleFeedingManager->GetGroupMember(9)->makePregnant(conceptionTime);  //suckler 1st lact early
         cattleFeedingManager->GetGroupMember(10)->makePregnant(conceptionTime);  //suckler 1st lact late
         cattleFeedingManager->GetGroupMember(11)->makePregnant(conceptionTime);  //suckler mature lact early
         cattleFeedingManager->GetGroupMember(12)->makePregnant(conceptionTime);  //suckler mature lact late
         cattleFeedingManager->GetGroupMember(9)->DryOff(lengthOfLactation);  //Suckler 1st early lact
         cattleFeedingManager->GetGroupMember(10)->DryOff(lengthOfLactation);  //suckler 1st lact late
         cattleFeedingManager->GetGroupMember(11)->DryOff(lengthOfLactation);  //suckler  mature early lact
         cattleFeedingManager->GetGroupMember(12)->DryOff(lengthOfLactation);  //suckler mature late lact
         cattleFeedingManager->weanCalves(2, ageAtWeaning);
      }
      cattleFeedingManager->SortAnimals(true);  //sort animals into their appropriate feeding group
      cattleFeedingManager->CheckDiets(false);   //get current diet for all the animals
//      cattleFeedingManager->checkPasture();
      double grazedHerbageRequired = cattleFeedingManager->GetHerbageRequired(false);  //herbage required per day
      double zerograzedHerbageRequired = 0.0;// cattleFeedingManager->GetHerbageRequired(true);  //herbage required per day
      int status = 1;
      int numOthers=0;
      if (grazing_manager)  //check to see if grazing manager is present
      {
         grazing_manager->SetGrazableFields();
         fields_to_graze = grazing_manager->daily_update(&numOthers);  //get fields that will be grazed
         if ((grazedHerbageRequired>0.0)||(zerograzedHerbageRequired>0.0))
         {
//				herbageAvailable=grazing_manager->GetHerbageAvailability(false);  //
				herbageAvailable+=grazing_manager->GetHerbageAvailability(true);  //
            if (herbageAvailable==0.0)  //simulation is starting, so no grazable feed available. Get a winter diet
            {
                  cattleFeedingManager->CheckDiets(true);
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
                     cattleFeedingManager->CheckDiets(true);
                     grazedHerbageRequired=0.0;
                  }
                  else
                  {
                     status = grazing_manager->GetGrazingSupplyStatus(numberOfDaysSupply);
                  }
               }
            }
         }
      }
      cattleFeedingManager->AllocateFieldsForGrazing(fields_to_graze,numOthers);  //tell the cattle which fields they will graze
      cattleFeedingManager->daily_update(status);
   }

   if ((theLivestock)&&(theTime.GetDay()==1)&&( theTime.GetMonth()==3))
      theManure_manager->DoAdjustment();
   if ((theLivestock)&&(theTime.GetDay()==1)&&( theTime.GetMonth()==6))
      theManure_manager->ExportStoredManure();
}

void Operations_manager_class::ReceivePlan(char *fileExtension, char *inputDir)
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
	   if (cattleFeedingManager)  //check to see if cattleFeedingManager is present
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
   _chdir(inputDir);
   if (cattleFeedingManager)  //check to see if cattleFeedingManager is present
   {
      cattleFeedingManager->ReceivePlan();
      if (cattleFeedingManager->GetautoFeed())
	      cattleFeedingManager->AutoFeed(conceptionTime,lengthOfLactation);
      cattleFeedingManager->CalcTurnoutandHousingDates(false);   //is only true for zero grazing - which does not work yet
      cattleFeedingManager->SortAnimals(false);  //sort animals into their appropriate feeding group
      grazing_manager->ReceivePlan(fileName);
      if (milkQuota>0.0)  //need to regulate feeding to achieve a target annual yield
      {
         double potentialMilkYield = cattleFeedingManager->GetPotentialMilkYield(lengthOfLactation);
         if (potentialMilkYield==0.0)
         	theMessage->FatalError("OperationsManager: milk quota set but no milk will be produced!");
         double factor = milkQuota/potentialMilkYield;
         if (factor>1.0)
         	theMessage->Warning("OperationsManager: milk quota higher than potential milk yield!");
         else
	         cattleFeedingManager->SettargetMilkYieldFactor(factor);
      }
//      cattleFeedingManager->AutoFeed(conceptionTime,lengthOfLactation);
   }

	theManure_manager->ReceivePlan(fileExtension);
}

void Operations_manager_class::autoCropPilot()
{
  for (int i=0;i<theCropRotation->GetFields()->NumOfNodes();i++)
  {
//   theCropRotation->GetnormN()
	}
};


void Operations_manager_class::GetStatus()
{
   if (cattleFeedingManager && theOutputControl->GetHerdDetails())  //check to see if cattleFeedingManager is present
   {
   	cattleFeedingManager->WriteGroupOutput();
	}
   if (grazing_manager)
		grazing_manager->GetStatus();
   if (theOutputControl->GetOperationalDetails())
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
void Operations_manager_class::InitialiseFileOutput()
{
   char filename[20];
   char leader[100];

   opsfile = new fstream();
   sprintf(leader,theOutputControl->GetoutputDirectory());
#ifdef __ANSICPP__
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

void Operations_manager_class::GetNumber()
{
 cattleFeedingManager->GetNumber();
}
