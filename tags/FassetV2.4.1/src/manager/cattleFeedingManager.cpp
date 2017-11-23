/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// Class: cattlefeeding_manager_class
#include "../base/common.h"
#include "cattleFeedingManager.h"


#include "../base/settings.h"
#include "../livestock/dkCattle.h"
#include "../tools/fileAccess.h"

#ifndef _STANDALONE
	#include "../building/stable.h"
	#include "../fields/patch.h"
#endif
// Functions for class cattlefeeding_manager_class

 cattleFeedingManager::cattleFeedingManager ():base()

{
   feedingGroups= new linkList<cattleFeedingGroup>;
   dairyPresent=false;
   beefPresent=false;


   startLateLactation=0;
   autoFeed = false;
   turnoutDate = nullptr;
   housingDate =  nullptr;
   fHerdPerformance=nullptr;
   fCattleFeeding = nullptr;
	theLivestock=nullptr;
   dairyLo=0;
   dairyHi=0;				//boundaries of dairy animals in feeding group list

}


cattleFeedingManager::cattleFeedingManager (const char *aName,const int aIndex,const base * aOwner)
											:base(aName,aIndex,aOwner)
{
   feedingGroups= new linkList<cattleFeedingGroup>;
   dairyPresent=false;
   beefPresent=false;


   startLateLactation=0;
   autoFeed = false;
   turnoutDate = nullptr;
   housingDate =  nullptr;
   fHerdPerformance=nullptr;
   fCattleFeeding=nullptr;
	theLivestock=nullptr;
   dairyLo=0;
   dairyHi=0;				//boundaries of dairy animals in feeding group list

}
 cattleFeedingManager::~cattleFeedingManager()
{
	delete feedingGroups;
   if (fHerdPerformance)
   {
	   fHerdPerformance->close();
      fCattleFeeding->close();
      delete fHerdPerformance;
      delete fCattleFeeding;
   }

}


/*******************************************************************
********************************************************************/
void cattleFeedingManager::Initialize(livestock * theLivestockptr)
{
	theLivestock=theLivestockptr;
	dairyPresent = false;
   beefPresent = false;

}

/*******************************************************************
********************************************************************/
void cattleFeedingManager::ReceivePlan()
{
   dairyLo = 3;
	dairyHi=6;

   string logString = "------------ Reading cattle feeding plan ------------";
	theMessage->LogEvent((char*) logString.c_str());
	fileAccess hd;
   char buf[200];
   strcpy(buf,hd.getCurrentPath().c_str());
   cout << buf;
   char fileName[30];
 	strcpy(fileName,"cattlefeedplan.dat");
 	commonData * data=new commonData();
 	data->setCritical(true);
 	data->readFile("cattlefeedplan.dat");

 	data->FindItem("startLateLactation",&startLateLactation);


 	data->FindItem("differentiation",&differentiation);
 	data->FindItem("autoFeed",&autoFeed);

   	int start, stop;
      feedingGroups->ForgetAll();

      string namestr;
		cattleFeedingGroup * aFeedingGroup;
      for(int inx=0;inx<=18;inx++)
      {

         namestr=(string) "Group";
         aFeedingGroup = new cattleFeedingGroup((char *)namestr.c_str(),inx,this); // Possible LEAK, not sure (bmp) !!!
   		aFeedingGroup->Setactive(false);
         feedingGroups->InsertLast(aFeedingGroup);
      }
	   if (dairyPresent && !beefPresent)
      {
         start=0; stop=7;
//         ReadFeedingGroupParameters(start, stop);
         for(int inx=start;inx<=stop;inx++)
         {
         	aFeedingGroup = feedingGroups->ElementAtNumber(inx);
            aFeedingGroup->ReadParameters(data, autoFeed);                           //get the diet info for each period
            aFeedingGroup->Setactive(true);
         }
      }
	   if (beefPresent && !dairyPresent)
      {
         start=0; stop=2;
//         ReadFeedingGroupParameters(start, stop);
         for(int inx=start;inx<=stop;inx++)
         {
         	aFeedingGroup = feedingGroups->ElementAtNumber(inx);
            aFeedingGroup->ReadParameters(data, autoFeed);                           //get the diet info for each period
            aFeedingGroup->Setactive(true);
         }
         start=7; stop=7;
//         ReadFeedingGroupParameters(start, stop);
         for(int inx=start;inx<=stop;inx++)
         {
         	aFeedingGroup = feedingGroups->ElementAtNumber(inx);
            aFeedingGroup->ReadParameters(data, autoFeed);                           //get the diet info for each period
            aFeedingGroup->Setactive(true);
         }
         start=9; stop=18;
//         ReadFeedingGroupParameters(start, stop);
         for(int inx=start;inx<=stop;inx++)
         {
         	aFeedingGroup = feedingGroups->ElementAtNumber(inx);
            aFeedingGroup->ReadParameters(data, autoFeed);                           //get the diet info for each period
            aFeedingGroup->Setactive(true);
         }
         delete data;
/*	   if (beefPresent && dairyPresent)
      {
         start=0; stop=7;
         ReadFeedingGroupParameters(start, stop);
         start=13; stop=18;
         ReadFeedingGroupParameters(start, stop);
      }  */
   }

  //	CalcTurnoutandHousingDates(true);
   if ((globalSettings->DetailsData.getHerdDetails()==true)&& (!fHerdPerformance))
		InitialiseGroupOutput();
}





/*******************************************************************
********************************************************************/
void cattleFeedingManager::SortAnimals(bool Cull)
{
	ClearGroups();
   dairyStock* theCattle = GetHerd(1);
   if (theCattle)
   //if dairy animals are present
   {
	   theCattle->theSorting(Cull);
      for (int j=0;j<theCattle->GetCattleGroup()->NumOfNodes();j++)
      {
         cattleGroup * anAgeGroup = theCattle->GetCattleGroup()->ElementAtNumber(j);  //get each age group in turn
         for (int k=0;k<anAgeGroup->GetnumberInstances();k++)
         {
            cattle * anAnimal = anAgeGroup->GetGroupMember(k);
   //divide the cattle up between the feeding groups
            if (anAnimal->Getage()<=183)
            {
               feedingGroups->ElementAtNumber(0)->AddToGroup(anAnimal); //Heifers1 - 0.0 to 0.5 year old heifers
               anAnimal->SetfeedingGroup(0);
            }

            if ((anAnimal->Getage()>183)&&(anAnimal->Getage()<=365))
            {
               feedingGroups->ElementAtNumber(1)->AddToGroup(anAnimal); //Heifers2 - 0.5 to 1 year old heifers
               anAnimal->SetfeedingGroup(1);
            }

            if ((anAnimal->Getage()>365)&& (anAnimal->GetlactationNumber()==0))
            {
               feedingGroups->ElementAtNumber(2)->AddToGroup(anAnimal);   //Heifers3 - 1 to 2 year old heifers
               anAnimal->SetfeedingGroup(2);
            }

            if ((anAnimal->isLactating())&& (anAnimal->GetlactationNumber()==1))   //if 1st lactation and lactating
            {
               if (anAnimal->GetdaysFromParturition()<=startLateLactation)
               {
                  feedingGroups->ElementAtNumber(3)->AddToGroup(anAnimal);  //1st lactation, early lactation
	               anAnimal->SetfeedingGroup(3);
               }
               else  //1st lactation, late lactation
               {
                  feedingGroups->ElementAtNumber(4)->AddToGroup(anAnimal); //1st lactation, late lactation
                  anAnimal->SetfeedingGroup(4);
               }
            }

            if ((anAnimal->isLactating())&& (anAnimal->GetlactationNumber()>1))   //if mature and lactating
            {
               if (anAnimal->GetdaysFromParturition()<=startLateLactation)
               {
                  feedingGroups->ElementAtNumber(5)->AddToGroup(anAnimal);  //mature, early lactation
	               anAnimal->SetfeedingGroup(5);
               }
               else  //mature, late lactation
               {
                  feedingGroups->ElementAtNumber(6)->AddToGroup(anAnimal); //mature, late lactation
                  anAnimal->SetfeedingGroup(6);
               }
            }

            if (!(anAnimal->isLactating())&& (anAnimal->GetlactationNumber()>0))
            {
                feedingGroups->ElementAtNumber(7)->AddToGroup(anAnimal); //dairy dry animals
                anAnimal->SetfeedingGroup(7);
            }

//cast animals not implemented
         }
      }  //end for each cattle group
   }
      //end of dairy cattle

   theCattle = GetHerd(2);
   if (theCattle)
   //only if beef animals are present
   {
	   theCattle->theSorting(Cull);
      for (int j=0;j<theCattle->GetCattleGroup()->NumOfNodes();j++)
      {
         cattleGroup * anAgeGroup = theCattle->GetCattleGroup()->ElementAtNumber(j);  //get each age group in turn
         for (int k=0;k<anAgeGroup->GetnumberInstances();k++)
         {
            cattle * anAnimal = anAgeGroup->GetGroupMember(k);
            if (anAnimal->Getage()<183)   // animals 0 to 6 months
            {
               if (anAnimal->GetAlias()=="HEIFERS1")
               {
                  feedingGroups->ElementAtNumber(0)->AddToGroup(anAnimal);
                  anAnimal->SetfeedingGroup(0);
               }
               if (anAnimal->GetAlias()=="FBEEF1")
               {
                  feedingGroups->ElementAtNumber(13)->AddToGroup(anAnimal);
                  anAnimal->SetfeedingGroup(13);
               }
               if (anAnimal->GetAlias()=="MBEEF1")
               {
                  feedingGroups->ElementAtNumber(16)->AddToGroup(anAnimal);
                  anAnimal->SetfeedingGroup(16);
               }
				}
            if ((anAnimal->Getage()>=183)&&(anAnimal->Getage()<=365))   //young animals 6 months to 1 year
            {
               if (anAnimal->GetAlias()=="HEIFERS2")
               {
                  feedingGroups->ElementAtNumber(1)->AddToGroup(anAnimal); //
                  anAnimal->SetfeedingGroup(1);
               }
               if (anAnimal->GetAlias()=="FBEEF2")
               {
                  feedingGroups->ElementAtNumber(14)->AddToGroup(anAnimal); //
                  anAnimal->SetfeedingGroup(14);
               }
               if (anAnimal->GetAlias()=="MBEEF2")
               {
                  feedingGroups->ElementAtNumber(17)->AddToGroup(anAnimal);
                  anAnimal->SetfeedingGroup(17);
               }
				}
            if ((anAnimal->Getage()>365)&& (anAnimal->GetlactationNumber()==0))
            {
               if (anAnimal->GetAlias()=="HEIFERS3")
               {
                  feedingGroups->ElementAtNumber(2)->AddToGroup(anAnimal); //
                  anAnimal->SetfeedingGroup(2);
               }
               if (anAnimal->GetAlias()=="FBEEF3")
               {
                  feedingGroups->ElementAtNumber(15)->AddToGroup(anAnimal); //
                  anAnimal->SetfeedingGroup(15);
               }
               if (anAnimal->GetAlias()=="MBEEF3")
               {
                  feedingGroups->ElementAtNumber(18)->AddToGroup(anAnimal);
                  anAnimal->SetfeedingGroup(18);
               }
				}
            if ((anAnimal->isLactating())&& (anAnimal->GetlactationNumber()==1))   //if 1st lactation and lactating
            {
               if (anAnimal->GetdaysFromParturition()<=startLateLactation)
               {
                  feedingGroups->ElementAtNumber(9)->AddToGroup(anAnimal);  //1st lactation, early lactation
	               anAnimal->SetfeedingGroup(9);
               }
               else  //1st lactation, late lactation
               {
                     feedingGroups->ElementAtNumber(10)->AddToGroup(anAnimal); //1st lactation, late lactation
		               anAnimal->SetfeedingGroup(10);
               }
            }

            if ((anAnimal->isLactating())&& (anAnimal->GetlactationNumber()>1))   //if mature and lactating
            {
               if (anAnimal->GetdaysFromParturition()<=startLateLactation)
               {
                  feedingGroups->ElementAtNumber(11)->AddToGroup(anAnimal);  //mature, early lactation
	               anAnimal->SetfeedingGroup(11);
               }
               else  //mature, late lactation
               {
                  feedingGroups->ElementAtNumber(12)->AddToGroup(anAnimal); //mature, late lactation
                  anAnimal->SetfeedingGroup(12);
               }
            }

            if (!(anAnimal->isLactating())&& (anAnimal->GetlactationNumber()>0))
            {
                feedingGroups->ElementAtNumber(7)->AddToGroup(anAnimal); //suckler dry animals
                anAnimal->SetfeedingGroup(7);
            }
         }
      }   //end of age group
   } // end of beef

}

/*******************************************************************
********************************************************************/
void cattleFeedingManager::daily_update(int status)
{
   for(int j=0;j<feedingGroups->NumOfNodes();j++)
   {
      cattleFeedingGroup * aFeedingGroup = feedingGroups->ElementAtNumber(j);
   	aFeedingGroup->FeedCattle(status);        //set diet for all groups
   }
//   weanCalves();    //temporary - go wean calves
   if (housingDate)
      if (theTime==*housingDate)
         housingDate->AddOneYear();
}


/*******************************************************************
Calculate herbage required by herd, DM in tonnes
********************************************************************/
double cattleFeedingManager::GetHerbageRequired(bool zeroGrazed =false)
{
	double herbageRequired=0.0;
   herbageRequired = GetHerbageRequired(0,feedingGroups->NumOfNodes()-1,zeroGrazed);
	return herbageRequired;
}

/*******************************************************************
Calculate herbage required by part of the herd
********************************************************************/
double cattleFeedingManager::GetHerbageRequired(int start, int end, bool zeroGrazed)
{
	double herbageRequired=0.0;
   for(int j=start;j<(end+1);j++)
   {
      cattleFeedingGroup * aFeedingGroup = feedingGroups->ElementAtNumber(j);
   	herbageRequired+=aFeedingGroup->GetHerbageRequired(zeroGrazed);
   }
	return herbageRequired;
}

/*******************************************************************
Weans all calves - need something better some time.
********************************************************************/
void cattleFeedingManager::weanCalves(int herdNo, double daysSinceBirth)
{
	dairyStock *theCattle = GetHerd(herdNo);   //wean all dairy cows immediately
   for (int j=0;j<theCattle->GetCattleGroup()->NumOfNodes();j++)
   {
      cattleGroup * anAgeGroup = theCattle->GetCattleGroup()->ElementAtNumber(j);  //get each age group in turn
      for (int k=0;k<anAgeGroup->GetnumberInstances();k++)
      {
         cattle * anAnimal = anAgeGroup->GetGroupMember(k);
         if (anAnimal->GetCalf())
         {
         	if (anAnimal->GetCalf()->Getage()>=daysSinceBirth)
	         	anAnimal->GetCalf()->SetisSuckling(false);
         }
      }
   }
   if (herdNo==2)
	   theCattle->CheckWean(true);
   else
	   theCattle->CheckWean(false);
}

/*******************************************************************
Gets the relevant herd (1 = dairy, 2 = beef)
********************************************************************/
dairyStock*  cattleFeedingManager::GetHerd(int whichHerd)
{
   dairyStock* theCattle = nullptr;
   char herdName[20];
   if (whichHerd==1)
   	strcpy(herdName,"DairyStock");
   else
   	strcpy(herdName,"BeefStock");

   for (int i=0;i<theLivestock ->getanimalStock()->NumOfNodes();i++)
   {
	   if (strcmp(theLivestock ->getanimalStock()->ElementAtNumber(i)->GetName(),herdName)==0)
   //if dairy animals are present
   	{
      //store address of the dairyStock instance containing dairy cattle in theCattle
	      theCattle = (dairyStock *) theLivestock ->getanimalStock()->ElementAtNumber(i);
      }
   }
	return theCattle;
}

/*******************************************************************
Clear groups prior to resorting
********************************************************************/
void cattleFeedingManager::ClearGroups()
{
   for(int j=0;j<feedingGroups->NumOfNodes();j++)
   {
      cattleFeedingGroup * aFeedingGroup = feedingGroups->ElementAtNumber(j);
   	aFeedingGroup->ClearGroup();
   }
}


/*******************************************************************
Tells cattle which fields to graze
********************************************************************/
void cattleFeedingManager::AllocateFieldsForGrazing(linkList <field> *fields_to_graze,
										int numOthers)
{
	int maxFields=fields_to_graze->NumOfNodes();
   for(int j=0;j<feedingGroups->NumOfNodes();j++)
   {
      cattleFeedingGroup * aFeedingGroup = feedingGroups->ElementAtNumber(j);
      if (numOthers==-9999)  //open all fields for grazing
   		aFeedingGroup->AllocateFieldsForGrazing(fields_to_graze,0,maxFields);
      else
      {
         if ((j>=dairyLo)&&(j<=dairyHi))
            aFeedingGroup->AllocateFieldsForGrazing(fields_to_graze,numOthers,maxFields);
         else
            aFeedingGroup->AllocateFieldsForGrazing(fields_to_graze,0,numOthers);
      }
   }
}


/*******************************************************************
Finds the turnout date and latest housing date
********************************************************************/
void cattleFeedingManager::CalcTurnoutandHousingDates(bool zeroGrazed)

{
	delete turnoutDate;
   turnoutDate=nullptr;
   delete housingDate;
   housingDate=nullptr;
   for(int j=0;j<feedingGroups->NumOfNodes();j++)
   {
   	bsTime * groupTurnoutDate;
      cattleFeedingGroup * aFeedingGroup = feedingGroups->ElementAtNumber(j);
      groupTurnoutDate = aFeedingGroup->GetTurnoutDate(zeroGrazed);
     	if ((!turnoutDate)&&(groupTurnoutDate))
         	turnoutDate = groupTurnoutDate;
      else if (groupTurnoutDate)
      		if (*groupTurnoutDate<*turnoutDate)
         	turnoutDate =groupTurnoutDate;

   	bsTime * groupHousingDate;
      groupHousingDate = aFeedingGroup->GetHousingDate(zeroGrazed);
     	if (!housingDate)
         	housingDate = groupHousingDate;
      else if (groupHousingDate)
      		if (*groupHousingDate>*housingDate)
         	housingDate =groupHousingDate;
   }
   if (turnoutDate)
	   if (*turnoutDate<theTime)
	  	   turnoutDate->AddOneYear();
};




void cattleFeedingManager::WriteGroupOutput()
{
   *fHerdPerformance << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
   for(int j=0;j<feedingGroups->NumOfNodes();j++)
   {
      cattleFeedingGroup * aFeedingGroup = feedingGroups->ElementAtNumber(j);
      aFeedingGroup->GetStatus(fHerdPerformance);
   }
   *fHerdPerformance << endl;
}

void cattleFeedingManager::InitialiseGroupOutput()
{
   char filename[20];
   char leader[200];

   fCattleFeeding = new fstream();

   sprintf(leader,globalSettings->getOutputDirectory().c_str());
#ifndef __BCplusplus__
   sprintf(filename,"cattlefeed.xls");
#else
   sprintf(filename,"cattlefeed.xls",Index);
#endif
   strcat(leader,filename);
   fCattleFeeding->open(leader,ios::out);
   fCattleFeeding->precision(8);

   fHerdPerformance = new fstream();

   sprintf(leader,globalSettings->getOutputDirectory().c_str());
#ifndef __BCplusplus__
   sprintf(filename,"herd.xls");
#else
   sprintf(filename,"herd.xls",Index);
#endif
   strcat(leader,filename);
   fHerdPerformance->open(leader,ios::out);
   fHerdPerformance->precision(8);

   *fHerdPerformance << "Date" << "\t";
   for(int i=0;i<=feedingGroups->NumOfNodes();i++)
   {
      *fHerdPerformance << "Group"<< "\t";
      *fHerdPerformance << "Number"<< "\t";
      *fHerdPerformance << "LW" << "\t";
      *fHerdPerformance << "Growth" << "\t";
      *fHerdPerformance << "Milk_prod" << "\t";
      *fHerdPerformance << "ME_Intake" << "\t";
      *fHerdPerformance << "FE_Intake" << "\t";
/*      *fHerdPerformance << "FEDemand" << "\t";
      *fHerdPerformance << "Aat_supp" << "\t";
      *fHerdPerformance << "Aat_dem" << "\t";    */
      *fHerdPerformance << "N_urine" << "\t";
      *fHerdPerformance << "N_faeces" << "\t";
      *fHerdPerformance << "DM_intake" << "\t";
      *fHerdPerformance << "Grazing_DM" << "\t";
//         *fHerdPerformance << "Grazed_FE" << "\t";
   }
   *fHerdPerformance << endl;
   *fHerdPerformance << " "<< "\t";
	*fCattleFeeding << "Group" << "\t";

   for(int i=0;i<=feedingGroups->NumOfNodes();i++)
   {
      *fHerdPerformance << " "<< "\t";
      *fHerdPerformance << " "<< "\t";
      *fHerdPerformance << "kg" << "\t";
      *fHerdPerformance << "kg/d" << "\t";
      *fHerdPerformance << "kg/d" << "\t";
      *fHerdPerformance << "ME/d" << "\t";
      *fHerdPerformance << "FE/d" << "\t";
/*      *fHerdPerformance << "FE/d" << "\t";
      *fHerdPerformance << "g/d" << "\t";
      *fHerdPerformance << "g/d" << "\t";    */
      *fHerdPerformance << "g/d" << "\t";
      *fHerdPerformance << "g/d" << "\t";
      *fHerdPerformance << "kg/d" << "\t";
      *fHerdPerformance << "kg/d" << "\t";
//         *fHerdPerformance << "Grazed_FE" << "\t";
   }
   *fHerdPerformance << endl;

   for(int i=0;i<=feedingGroups->NumOfNodes();i++)
   {
   	cattleFeedingGroup * aFeedingGroup = feedingGroups->ElementAtNumber(i);
      if (aFeedingGroup)
      {
      	if (i==0)
         {
				aFeedingGroup->ShowGroupDiets(fCattleFeeding,true);
            *fCattleFeeding << endl;
			}
			aFeedingGroup->ShowGroupDiets(fCattleFeeding,false);
         *fCattleFeeding << endl;
      }
   }

}

void cattleFeedingManager::CheckDiets(bool startup)
{
   for(int j=0;j<feedingGroups->NumOfNodes();j++)
   {
      cattleFeedingGroup * aFeedingGroup = feedingGroups->ElementAtNumber(j);
		if (aFeedingGroup->Getactive())
	   	aFeedingGroup->CheckDiets(startup);
   }
}




double cattleFeedingManager::GetSilageRequired()
{
	double ret_val=0.0;
   for(int j=0;j<feedingGroups->NumOfNodes();j++)
   {
      cattleFeedingGroup * aFeedingGroup = feedingGroups->ElementAtNumber(j);
   	ret_val+=aFeedingGroup->GetSilageRequired(522,600);
   }
   return ret_val;
}

double cattleFeedingManager::GetPotentialMilkYield(int durationOfLactation)
{
	double ret_val=0.0;
   for(int j=dairyLo;j<dairyHi;j++)
   {
      cattleFeedingGroup * aFeedingGroup = feedingGroups->ElementAtNumber(j);
   	ret_val+=aFeedingGroup->GetPotentialMilkYield(durationOfLactation);
   }
   return ret_val;
}



/************************************************************************************
Calculates the feed ration necessary to satisfy animal needs
************************************************************************************/
void cattleFeedingManager::AutoFeed(int conceptionTime, int lengthOfLactation)
{


      for (int i=1; i<=2; i++)
      {
	      dairyStock* theCattle = GetHerd(i);
         if (theCattle)
         {
            dk_cattle *tempcow= new dk_cattle("",0,this);
            commonData * data=new commonData();
               data->setCritical(true);
               data->readFile("animals.dat");
            tempcow->ReadParameters(data);
            delete data;
            cattleFeedingGroup * aFeedingGroup;
            if (conceptionTime > startLateLactation)
               theMessage->FatalError("conceptionTime > startLateLactation");
            // get animal type groups (dairy has 3, beef 10)
            for (int j=0;j<theCattle->GetCattleGroup()->NumOfNodes();j++)
//            for (int j=0;j<numOfFeedingGroups;j++)
            {
               cattleGroup * anAgeGroup = theCattle->GetCattleGroup()->ElementAtNumber(j);  //get each age group in turn
               double minAge = anAgeGroup->GetminAge();
               double maxAge = anAgeGroup->GetmaxAge();
               double energyRequired, proteinRequired;
               switch (j)         //Note that numbers relate to groups in Livestock, not feeding groups
               {                  //BEEF animals are only present on beef farms, HEIFERS can be present on both dairy and beef
                  case 0:   //HEIFERS1
                  case 1:   //HEIFERS2
                  case 2:   //HEIFERS3
                  case 4:   //FBEEF1
                  case 5:   //FBEEF2
                  case 6:   //FBEEF3
                  case 7:   //MBEEF1
                  case 8:   //MBEEF2
                  case 9:   //MBEEF3
                     tempcow->Setpregnant(false);
                     tempcow->SetdaysFromConception(0);
                     tempcow->SetdaysFromParturition(0.0);
                     tempcow->SetlactationNumber(0);
                     tempcow->SetisYoung(true);
                     tempcow->Setage(int(maxAge*365.0));
                     tempcow->Setweight(tempcow->Get_norm_weight());
                     tempcow->Setlactating(false);
                     if ((j==7)||(j==8)||(j==9))
                       	tempcow->SetisFemale(false);
                     else
                       	tempcow->SetisFemale(true);
                     if (j==2)
                     {
	                     tempcow->Setpregnant(true);
	                     tempcow->SetdaysFromConception(tempcow->GetgestationPeriod());
                     }
                     energyRequired= tempcow->GetEnergyRequirement();
                     proteinRequired= tempcow->GetProteinRequirement();
                     if (j<3)  //treat beef heifers and dairy heifers the same
   	                  aFeedingGroup= feedingGroups->ElementAtNumber(j);
							else
	                     aFeedingGroup= feedingGroups->ElementAtNumber(j+9);
                     aFeedingGroup->CalcDefaultDiets(energyRequired,proteinRequired, differentiation, false,fCattleFeeding);
					      cout << "Initialised " << aFeedingGroup->GetAlias() << endl;
                     break;
                  case 3:  //DAIRY and SUCKLER
                  {

                     int limitLo, limitHi;
                     if (i==1)
                     {
                     	limitLo=3;
                        limitHi=6;
                     }
                     if (i==2 && !dairyPresent)
                     {
                     	limitLo=9;
                        limitHi=12;
                     }
                     if (i==2 && dairyPresent)
                     {
                     	limitLo=13;
                        limitHi=12;
                     }
                     //deal with these damn dry animals first. Use same feed for dry dairy and suckler animals
                     //could handled in a more elegant way
                     //Note - if dairy and sucklers are present, dry cows will be initialised twice. This should not be a problem :-)
                     aFeedingGroup= feedingGroups->ElementAtNumber(7);
                     tempcow->Setpregnant(true);
                     tempcow->Setlactating(false);
                     tempcow->SetdaysFromParturition(0.0);
                     tempcow->SetdaysFromConception(tempcow->GetgestationPeriod()-1);
                     tempcow->SetlactationNumber(2);
                     tempcow->SetisYoung(false);
                     tempcow->Setage(int((minAge+(maxAge-minAge)/2)*365.0));
                     tempcow->Setweight(tempcow->Get_norm_weight());
                     proteinRequired= tempcow->GetProteinRequirement();
                     energyRequired= tempcow->GetEnergyRequirement();
                     aFeedingGroup->CalcDefaultDiets(energyRequired,proteinRequired,differentiation,true,fCattleFeeding);
      cout << "Initialised " << aFeedingGroup->GetAlias() << endl;
                     //end of dry animals
                     //now do lactating animals
                     for (int k=limitLo;k<limitHi+1; k++)
                     {
                        aFeedingGroup= feedingGroups->ElementAtNumber(k);
                        tempcow->Setpregnant(true);
                        tempcow->Setlactating(true);
                        if (k==limitLo)  //1st lactation early
                        {
                           tempcow->SetdaysFromParturition(startLateLactation/2);
                           tempcow->SetdaysFromConception((startLateLactation/2)-conceptionTime);
                           tempcow->SetlactationNumber(1);
                        }
                        if (k==limitLo+1) //1st lactation late
                        {
                           double num = startLateLactation + lengthOfLactation/2;
                           tempcow->SetdaysFromParturition(num);
                           tempcow->SetdaysFromConception(num - conceptionTime);
                           tempcow->SetlactationNumber(1);
                        }
                        if (k==limitLo+2) // mature early lactation
                        {
                           tempcow->SetdaysFromParturition(startLateLactation/2);
                           tempcow->SetdaysFromConception((startLateLactation/2)-conceptionTime);
                           tempcow->SetlactationNumber(1);
                        }
                        if (k==limitLo+3)  // mature late lactation
                        {
                           double num = startLateLactation + lengthOfLactation/2;
                           tempcow->SetdaysFromParturition(num);
                           tempcow->SetdaysFromConception(num - conceptionTime);
                           tempcow->SetlactationNumber(2);
                        }
/*                        if (k==limitLo+5)  //cast animals
                        {
                              tempcow->SetdaysFromParturition(0.0);
                              tempcow->SetdaysFromConception(0.0);
                              tempcow->SetlactationNumber(2);
                              tempcow->Setlactating(false);
                              tempcow->Setpregnant(false);
                        }*/
                        tempcow->SetisYoung(false);
                        tempcow->Setage(int((minAge+(maxAge-minAge)/2)*365.0));    //should really
                        tempcow->Setweight(tempcow->Get_norm_weight());
	                     proteinRequired= tempcow->GetProteinRequirement();
                        energyRequired= tempcow->GetEnergyRequirement();
                        aFeedingGroup->CalcDefaultDiets(energyRequired,proteinRequired,differentiation,true,fCattleFeeding);
						      cout << "Initialised " << aFeedingGroup->GetAlias() << endl;
                     }
                  break;
                  }  //end case 3
               } //end switch
            } // end else cattle groups
         	delete tempcow;
         } // if herd present


   } // end else found file
}


