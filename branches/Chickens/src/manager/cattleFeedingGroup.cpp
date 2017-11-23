/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "cattleFeedingGroup.h"

#ifndef _STANDALONE
	#include "../building/stable.h"
	#include "../fields/patch.h"
#else
	#include "../base/message.h"
#endif

#include "../livestock/dkCattle.h"

/****************************************************************************\
\****************************************************************************/
cattleFeedingGroup::cattleFeedingGroup ()
{
   theGroup = new linkList <cattle>;
   theGroupDiets = new linkList <diet>;
   theCurrentDiet=NULL;
   currentFeedingGroup=0;

}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
cattleFeedingGroup::cattleFeedingGroup(const char * aName, const int aIndex, const base * aOwner)
			:base(aName,aIndex,aOwner)
{
   theGroup = new linkList <cattle>;
   theGroupDiets = new linkList <diet>;
   theCurrentDiet=NULL;
   currentFeedingGroup=0;

}

/****************************************************************************\
\****************************************************************************/
cattleFeedingGroup::~cattleFeedingGroup()
{
	theGroup->ForgetAll();
   delete theGroup;
   delete theGroupDiets;
}

/*******************************************************************
********************************************************************/
void cattleFeedingGroup::ReadParameters(fstream *infile, bool autoFeed)
{
	Setfile(infile);
   SetCritical();
   FindSection(GetName(),GetIndex());  // find the correct place in the input file
 	GetParameter("Alias",&Alias); // Causes a LEAK, probably in base.cpp (hard to understand - BMP) !!!
//   cout << Alias << endl;
   string namestr;
   char Indexstr[10];
   sprintf(Indexstr,"(%d)",GetIndex());
   namestr=(string) "Group"+Indexstr+"."+"Period";
   int first,num;
   FindSection(GetName(),GetIndex());  // find the correct place in the input file
   GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of periods for which a diet is defined
   UnsetCritical();
   for(int index=first;index<(first+num);index++)
   {
       diet *aDiet=new diet((char*) namestr.c_str(),index,this); // LEAK (BMP) !!!
       aDiet->ReadParameters(infile,autoFeed);                           //get the diet info for each period
       theGroupDiets->InsertLast(aDiet);
   }
	Setfile(NULL);
}



/*******************************************************************
Make the animals pregnant
********************************************************************/
void cattleFeedingGroup::makePregnant(int daySinceParturition)
{
   for(int i=0;i<theGroup->NumOfNodes();i++)
   {
	   cattle * tempcattle = theGroup->ElementAtNumber(i);
      if (tempcattle->GetdaysFromParturition()==daySinceParturition)
      if (!(tempcattle->isPregnant()))
      {
         //flip flop between male and female calf
         if (wasFemale)
         {
            tempcattle->MakePregnant(false);
            wasFemale = false;
         }
         else
         {
            tempcattle->MakePregnant(true);
            wasFemale = true;
         }
      }
   }
}

/*******************************************************************
Stop lactation
********************************************************************/
void cattleFeedingGroup::DryOff(int daySinceParturition)
{
   for(int i=0;i<theGroup->NumOfNodes();i++)
   {
	   cattle * tempcattle = theGroup->ElementAtNumber(i);
      if (tempcattle->GetdaysFromParturition()==daySinceParturition)
      if ((tempcattle->isLactating()))
	      tempcattle->DryOff();
   }
}


void cattleFeedingGroup::ShowGroupDiets(fstream *afile, bool header)
 {
	if (theGroupDiets)
   {
   	if (!header)
		   *afile << GetAlias() << "\t";
      for(int j=0;j<theGroupDiets->NumOfNodes();j++)
      {
         diet * aDiet= theGroupDiets->ElementAtNumber(j);
         aDiet->ShowDiet(afile, header);
      }
   }
 };



/*******************************************************************
created Feb 2003 by NJH
finds the current diet for an animal group
********************************************************************/
void cattleFeedingGroup::CheckDiets(bool startup)
{
   if (startup)
   	theCurrentDiet=NULL;
//   if (theGroup->NumOfNodes()>0)
   {
      diet *aDiet=NULL;
      for(int j=0;j<theGroupDiets->NumOfNodes();j++)
      {
         aDiet= theGroupDiets->ElementAtNumber(j);
         if (!startup)   //get the current diet
         {
//           if ((aDiet->GetstartDate()->GetDay()<=theTime.GetDay())&&
           //    (aDiet->GetstartDate()->GetMonth()<=theTime.GetMonth()))

            if (aDiet->GetstartDate()->GetJulianDay(aDiet->GetstartDate()->GetDay(),aDiet->GetstartDate()->GetMonth(), theTime.GetYear())
            								<=theTime.GetJulianDay())
                   theCurrentDiet=aDiet;
         }
         else  //simulation is just starting - no roughage to graze yet, so get a diet without grazing
            if(!aDiet->IsGrazing())
                   theCurrentDiet=aDiet;

      }
      if (!theCurrentDiet)
            {theMessage->FatalError(GetAlias(),": No cattle diet found for this date");}
   }
}

/*******************************************************************
Find the first diet available for housed animals
********************************************************************/
diet * cattleFeedingGroup::GetHousedDiet()
{
	diet *aDiet=NULL;
   for(int j=0;j<theGroupDiets->NumOfNodes();j++)
   {
      aDiet= theGroupDiets->ElementAtNumber(j);
     	if(!aDiet->IsGrazing())
      		    return aDiet;

   }
	theMessage->FatalError(GetName(),": No housing diet found for this animal");
   return aDiet;
}

/*******************************************************************
Selects the diet for each animal, depending on the current status for grazed herbage availability
********************************************************************/
void cattleFeedingGroup::FeedCattle(int status)
{
  for(int i=0;i<theGroup->NumOfNodes();i++)
   {
	   cattle * tempcattle = theGroup->ElementAtNumber(i);
      if (tempcattle->GetlactationNumber()==0)
      	status = 1;   // no feed regulation for young stock??
      // grazing - 0 = deficiency, 1 = normal, 2 = surplus. If no grazing, status is 1 on input to this function
      	switch (status){
         case 0:tempcattle->SetfeedPlanList(theCurrentDiet->GetdeficiencyfeedItemList());
                currentFeedingGroup = 100*tempcattle->GetfeedingGroup()+ 10 + theCurrentDiet->GetIndex();
	         break;
         case 1:tempcattle->SetfeedPlanList(theCurrentDiet->GetnormalfeedItemList());
                currentFeedingGroup = 100*tempcattle->GetfeedingGroup()+ 20 + theCurrentDiet->GetIndex();
//         	cout <<
	         break;
         case 2:tempcattle->SetfeedPlanList(theCurrentDiet->GetsurplusfeedItemList());
                currentFeedingGroup = 100*tempcattle->GetfeedingGroup()+ 30 + theCurrentDiet->GetIndex();
	         break;
            }
      tempcattle->SetfeedingGroup(currentFeedingGroup);  // 3 digit output format: 1st digit = group number,
      																	// 2nd digit - 1=deficiency diet, 2=normal diet, 3=surplus diet
                                                         // 3rd digit - number of feed plan period
      if (tempcattle->GetCalf())
      	tempcattle->GetCalf()->SetfeedPlanList(tempcattle->GetfeedPlanList());
      if ((tempcattle->GetfeedPlanList()->NumOfNodes()==0) && (theCurrentDiet->GetHoused()==24.0))
      		theMessage->FatalError(GetAlias(),": animals in housing but there are no feed items");
   }
}

/*******************************************************************
Gets the current expected intake of grazed herbage, DM in tonnes
********************************************************************/
double cattleFeedingGroup::GetHerbageRequired(bool zeroGrazed)
{
	double herbageRequired = 0.0;
   for(int i=0;i<theGroup->NumOfNodes();i++)
   {
	   cattle * tempcattle = theGroup->ElementAtNumber(i);
		herbageRequired += theCurrentDiet->GetGrazingFeedRequirement(zeroGrazed) * tempcattle->GetNumber();
   }
   return herbageRequired;
}

/*******************************************************************
Clears the group prior to resorting
********************************************************************/
void cattleFeedingGroup::ClearGroup()
{
	theGroup->ForgetAll();
}

#ifndef _STANDALONE

/*******************************************************************
********************************************************************/
void cattleFeedingGroup::AllocateFieldsForGrazing(linkList <field> *fields_avail_for_grazing,
						 int start, int stop)
{

	if (theCurrentDiet)	//could be NULL if there are beef cattle but not dairy cattle present on farm
   {


      bool isGrazing = theCurrentDiet->IsGrazing();
      double hoursHoused=theCurrentDiet->GetHoused();

      for(int i=0;i<theGroup->NumOfNodes();i++)
      {
         cattle * tempcattle = theGroup->ElementAtNumber(i);
         if (isGrazing)
            tempcattle->SetfieldList(fields_avail_for_grazing);
         tempcattle->SethoursHoused(hoursHoused);
      }
   }
}
#endif

/*******************************************************************
finds the turnout date of an animal group (if any)
********************************************************************/
bsTime * cattleFeedingGroup::GetTurnoutDate(bool zeroGrazed)
{
	diet *aDiet=NULL;
   bsTime * turnoutDate = NULL;
   for(int j=0;j<theGroupDiets->NumOfNodes();j++)
   {
      aDiet= theGroupDiets->ElementAtNumber(j);
      if (aDiet->GetGrazingFeedRequirement(zeroGrazed)>0.0)
      {
      	if (!turnoutDate)
         	turnoutDate =aDiet->GetstartDate();
         else if (aDiet->GetstartDate()<turnoutDate)
         	turnoutDate =aDiet->GetstartDate();
      }
   }
   return turnoutDate;
}

/*******************************************************************
finds the housing date of an animal group
********************************************************************/
bsTime * cattleFeedingGroup::GetHousingDate(bool zeroGrazed)
{
	diet *aDiet=NULL;
   bsTime * housingDate = NULL;
   for(int j=0;j<theGroupDiets->NumOfNodes();j++)
   {
      aDiet= theGroupDiets->ElementAtNumber(j);
      if (aDiet->GetGrazingFeedRequirement(zeroGrazed)==0.0)
      {
      	if (!housingDate)
         	housingDate =aDiet->GetstartDate();
         else if (aDiet->GetstartDate()>housingDate)
         	housingDate =aDiet->GetstartDate();
      }
   }
   return housingDate;
}



/*******************************************************************
********************************************************************/
void cattleFeedingGroup::GetStatus(fstream * aFile)
{
//	bool isGrazing=false;
//   double aminoAcidSupply = 0.0;
//   double aminoAcidDemand = 0.0;
   double LW = 0.0;
   double growth= 0.0;
   double milk_prod = 0.0;
   double N_urine= 0.0;
   double N_faeces= 0.0;
//   double FEDemand= 0.0;
   double DM_intake= 0.0;
   double ME_intake= 0.0;
   double Grazed_DM= 0.0;
   double FE_intake= 0.0;
//   double Grazed_FE= 0.0;
   double numInGroup = theGroup->NumOfNodes();
   for(int i=0;i<numInGroup;i++)
   {
	   cattle * tempcattle = theGroup->ElementAtNumber(i);
     // double numInEach=tempcattle->GetNumber();
      LW+=tempcattle->Getweight();
      growth+=tempcattle->Getgrowth()->GetAmount();
      milk_prod+=tempcattle->GetlastMilkYield();
      FE_intake+=tempcattle->GetFE_intake();
      ME_intake+=tempcattle->GetME_intake();
//      FEDemand+=tempcattle->GetFEDemand();
//      aminoAcidSupply += tempcattle->GetaminoAcidSupply();
//      aminoAcidDemand +=tempcattle->GetaminoAcidDemand();
      N_urine+=tempcattle->GeturineN();
      N_faeces+=tempcattle->GetdungN();
      DM_intake+=tempcattle->GetDM_intake();
      Grazed_DM+=tempcattle->GetGrazed_DM();
//      Grazed_FE+=tempcattle->GetGrazed_FE();


   }
	if (numInGroup>0)
   {
	  LW/= numInGroup;
	  growth/= numInGroup;
	  milk_prod/= numInGroup;
	  FE_intake/= numInGroup;
	  ME_intake/= numInGroup;
	  N_urine/= numInGroup;
	  N_faeces/= numInGroup;
	  DM_intake/= numInGroup;
	  Grazed_DM/= numInGroup;
   }
  *aFile <<
		   currentFeedingGroup << "\t" <<
  			GetNumber()<< "\t" <<
			LW << "\t" <<
         growth << "\t" <<
         milk_prod << "\t" <<
         ME_intake << "\t" <<
         FE_intake << "\t" <<
/*         FEDemand << "\t" <<
         aminoAcidSupply << "\t" <<
         aminoAcidDemand << "\t" <<*/
         N_urine << "\t" <<
         N_faeces<< "\t" <<
         DM_intake << "\t" <<
         Grazed_DM<< "\t";
//         Grazed_FE << "\t";

}


/*******************************************************************
********************************************************************/
double cattleFeedingGroup::GetNumber()
{
   double ret_val=0.0;
   for(int i=0;i<theGroup->NumOfNodes();i++)
   {
	   dk_cattle * tempcattle = (dk_cattle*)theGroup->ElementAtNumber(i);
      ret_val+=tempcattle->GetNumber();
   }
   return ret_val;
}


double cattleFeedingGroup::GetSilageRequired(int startCode, int endCode)
{
	double ret_val=0.0;
   double number =  GetNumber();
   if (number>0.0)
   {
      int startSeason = 0;
      bsTime *aDate = GetTurnoutDate(false);
      int winter = 0;
      if (!aDate)
      	winter = 365;
      else
      {
      	startSeason = aDate->GetJulianDay();
         int endSeason = 365;
         aDate = GetHousingDate(false);
         if (aDate)
            endSeason = aDate->GetJulianDay();
         if (startSeason<endSeason)    //we are in the Northern hemisphere
            winter = startSeason + 365 - endSeason;
         else
            winter = 365 - startSeason + endSeason;
      }
      diet * aDiet = GetHousedDiet();
      ret_val=aDiet->GetSilageRequired(number,startCode, endCode);
      ret_val*=winter;
   }
   return ret_val;
};

double cattleFeedingGroup::GetPotentialMilkYield(int durationOfLactation)
{
   double ret_val= 0;
   for(int j=0;j<theGroup->NumOfNodes();j++)
   {
		   cattle * tempcattle = theGroup->ElementAtNumber(j);
         ret_val+=tempcattle->GetPotentialAnnualMilkProduction(durationOfLactation);
   }
   return ret_val;
}


void cattleFeedingGroup::CalcDefaultDiets(double totalEnergyRequired, double proteinRequired,double differentiation, bool defSurp, fstream *aFile)
{
   double roughageEnergyAllowance;            //all three values in SFUs
   double conservedRoughageEnergyAllowance;
   double grazedRoughageAllowance;
   if (aFile)
   	*aFile << GetAlias()<< "\t";
   proteinRequired/=1000.0;
   for(int j=0;j<theGroupDiets->NumOfNodes();j++)
   {
      double roughageProtein=0.0;
//      double concentrateProtein=0.0;
      diet *aDiet= theGroupDiets->ElementAtNumber(j);
		roughageEnergyAllowance=aDiet->GetpropRoughage() * totalEnergyRequired;
      if (aDiet->GetpropGrazed()<0.0)
      	aDiet->SetpropGrazed(aDiet->GetpropGrazed()*-1.0);
      grazedRoughageAllowance = aDiet->GetpropGrazed() * roughageEnergyAllowance;
      conservedRoughageEnergyAllowance = roughageEnergyAllowance - grazedRoughageAllowance;
      double concentrateEnergyReq = totalEnergyRequired - roughageEnergyAllowance;
      if (concentrateEnergyReq<0.0)
      	concentrateEnergyReq=0.0;
		feedItem* conservedRoughage =((feedItem*) aDiet->FindConservedRoughageFeed());
      if (conservedRoughageEnergyAllowance > 0.0)
      {
      	conservedRoughage->Setamount(conservedRoughageEnergyAllowance
         	/conservedRoughage->GetfeedUnitsPerItemUnit());
         roughageProtein=conservedRoughage->GetAmount() * conservedRoughage->Getaat() * 1000.0;  //in kg
      }
      else conservedRoughage->Setamount(0.0);

      if (grazedRoughageAllowance > 0.0)
      {
	     	feedItem* grazed = ((feedItem*) aDiet->FindGrazedFeed());
      	grazed->Setamount(grazedRoughageAllowance
         	/grazed->GetfeedUnitsPerItemUnit());
         roughageProtein+=grazed->GetAmount() * grazed->Getaat() * 1000.0;  //in kg
      }
     	feedItem* concentrate =((feedItem*) aDiet->FindConcentrateFeed());
      if (concentrateEnergyReq > 0.0)
      {
      	concentrate->Setamount(concentrateEnergyReq
         	/concentrate->GetfeedUnitsPerItemUnit());
/*         concentrateProtein=concentrate->GetAmount() * concentrate->Getaat()  * 1000.0;  //in kg
         double totalProtein = concentrateProtein + roughageProtein;
         if (totalProtein<proteinRequired)
         {
          	concentrateProtein= proteinRequired - roughageProtein;
	      	concentrate->Setamount(concentrateProtein
   	      	/(concentrate->Getaat()*1000.0));
         }      */
      }
      else concentrate->Setamount(0.0);
      if ((grazedRoughageAllowance > 0.0)&&(defSurp))
         	aDiet->CreateDefaultDiets(differentiation);
      if (aFile)
      	aDiet->ShowDiet(aFile, false);
   }
      if (aFile)
      	*aFile << endl;
}


