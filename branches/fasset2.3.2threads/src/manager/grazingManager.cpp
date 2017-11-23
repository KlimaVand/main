/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
//Warning! This class currently contains various nasty hardcodings
//Should be restructured now that all cattle are instanced as dairy

// Class: grazing_manager_class
#include "../base/common.h"
#include "grazingManager.h"
#include "../building/stable.h"
#include "../fields/field.h"
#include "../fields/patch.h"
#include "../base/climate.h"

#include "../base/nixcbits.h"

#include "../base/settings.h"

/*******************************************************************
********************************************************************/
grazingManager::grazingManager ():base()
{
  fields_to_graze= new linkList <field>;
  fields_avail_to_graze= new linkList <field>;
  theCattle = new linkList <dairyStock>;


  ExpectedSilageHolding = 0.0;

  initialised = false;
  growthStart = NULL;
  growthEnd = NULL;
  targetCutMass[0]=2.5;
  targetCutMass[1]=2.0;
  periodEnd=NULL;
  maxPeriods = 10;
  SilageMade =0.0;
  grazmanfile=NULL;
  residualDM=0.0;
  minHarvestable=0.0;


  lowerGrazingLimit=0.0;
  upperGrazingLimit=0.0;
}

/*******************************************************************
********************************************************************/
grazingManager::~grazingManager()
{
 fields_to_graze->ForgetAll();
 fields_avail_to_graze->ForgetAll();
 theCattle->ForgetAll();
 delete fields_to_graze;
 delete fields_avail_to_graze;

 delete theCattle;
 delete growthStart;
 delete growthEnd;
 if (grazmanfile)
 	grazmanfile->close();
 delete grazmanfile;
}

/*******************************************************************
created 1.5.00 by NJH
call after all livestock, fields and animal housing  etc are created
********************************************************************/
void grazingManager::Initialize(livestock * theLivestockptr,
									   cropRotation * theCropRotationptr,
                              buildings * theBuildingsptr, cattleFeedingManager * theCattleFeedingManagerptr)
{
	theLivestock = theLivestockptr;
	theCropRotation = theCropRotationptr;
	theBuildings = theBuildingsptr;
   theCattleFeedingManager = theCattleFeedingManagerptr;

   for (int i=0;i<theLivestock ->getanimalStock()->NumOfNodes();i++)
   {
	   if (strcmp(theLivestock ->getanimalStock()->ElementAtNumber(i)->GetName(),"DairyStock")==0)
      //only if dairy animals are present
   	{
         //store address of the DairyStock instance in theCattle
	      theCattle->InsertLast((dairyStock *) theLivestock ->getanimalStock()->ElementAtNumber(i));
		}

      if (strcmp(theLivestock ->getanimalStock()->ElementAtNumber(i)->GetName(),"BeefStock")==0)
         //only if beef animals are present
         {
            //store address of the DairyStock instance in theCattle
            theCattle->InsertLast((dairyStock *) theLivestock ->getanimalStock()->ElementAtNumber(i));
         }
	}

//   InitialiseGrowthEstimates();
   currentPeriod=0;
   if ((globalSettings[id]->DetailsData.getPastureDetails())&& (!grazmanfile))
   	InitialiseFileOutput();
}

/*******************************************************************
********************************************************************/
void grazingManager::HouseAnimals()
{
// Associates animals with animal housing
   for (int i=0;i<theCattle->NumOfNodes();i++)
   {
      for (int j=0;j<theCattle->ElementAtNumber(i)->GetCattleGroup()->NumOfNodes();j++)
      {
         cattleGroup * aGroup = theCattle->ElementAtNumber(i)->GetCattleGroup()->ElementAtNumber(j);
         //note that number of animals is currently an integer here
         double aNumberOfAnimals = (double) aGroup->GetnumberOfAnimals();
         if (aNumberOfAnimals>0)
         {
            double urinePerYear,faecesPerYear;

            aGroup->SetanimalSection(theBuildings->PlaceAnimal(aGroup->GetAlias().c_str()));
            cattle * anAnimal = aGroup->GetGroupMember(0);
            faecesPerYear = aNumberOfAnimals *anAnimal->GetamountSolid()*365/1000.0;
            urinePerYear = aNumberOfAnimals *anAnimal->GetamountFluid()*365/1000.0;
            aGroup->GetanimalSection()->SetAnimalParameters(aNumberOfAnimals,faecesPerYear,urinePerYear);
            for (int k=0;k<aGroup->GetnumberOfCohorts();k++)
            {
               anAnimal = aGroup->GetGroupMember(k);
               anAnimal->SetStableSection(aGroup->GetanimalSection());
            }
         }
      }
   }
}

/*******************************************************************
Created by NJH 2001
Logs grass or grass/clover fields as grazable by animals (fields_avail_to_graze used by cattle)
********************************************************************/
void grazingManager::SetGrazableFields()
{
   //patch *aPatch;
   field *aField;
   fields_avail_to_graze->ForgetAll();
   bsTime *housingDate = theCattleFeedingManager->GetHousingDate();
   //bsTime *turnoutDate = theCattleFeedingManager->GetTurnoutDate();
/*   if (turnoutDate)
   {
      if ((theTime>=*turnoutDate)&&(theTime<=*housingDate))*/
   if (housingDate)
   {
      if ((initialised)&&(theTime[id]<=*housingDate))
      {
         for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
         {
            aField = theCropRotation ->GetFields()->ElementAtNumber(i);
            char *cropId;
            aField->PresentCrop_(cropId);
            if (aField->GetActive() && aField->GetGrazingPossible())
            {
               if ((strcmp("C1",cropId)==0) || (strcmp("C2",cropId)==0) || (strcmp("G1",cropId)==0) ||
                  (strcmp("G2",cropId)==0) || (strcmp("K1",cropId)==0) || (strcmp("G4",cropId)==0) || (strcmp("G5",cropId)==0))
               {
//               	if (!aField->GivecrrPlan()->FieldOPStopsGrazing())
	                  fields_avail_to_graze->InsertLast(aField);
               }
            }
         }
      }
   }
}

/*******************************************************************
********************************************************************/
void grazingManager::ReceivePlan(char *fileName)
{
	commonData data;
	data.setCritical(true);
	data.readFile(fileName);

	data.FindSection("Grazing",0);
	data.FindItem("enableGrazing",&enableGrazing);
      if (enableGrazing)
      {
    	  data.FindItem("residualDM",&residualDM);
    	  data.FindItem("minHarvestable",&minHarvestable);

    	  data.FindItem("turnoutThreshold",&turnoutThreshold);
    	  data.FindItem("lowerGrazingLimit",&lowerGrazingLimit);
    	  data.FindItem("upperGrazingLimit",&upperGrazingLimit);
    	  data.FindItem("springCutoffDay",&springCutoffDay);
    	  data.FindItem("potentialGrowthFactor",&potentialGrowthFactor);
    	  data.FindItem("maxPeriods",&maxPeriods);
      }


   if (enableGrazing)
	   InitialiseGrowthEstimates();

//Put animals into animal housing - all animals housed at start of year
	HouseAnimals();
   ExpectedSilageHolding = calcSilageRequirement();
//   AutoGrazingPossible();
   if ((enableGrazing)&& (theCattleFeedingManager->GetTurnoutDate())&&(!initialised))
	   periodEnd = theCattleFeedingManager->GetTurnoutDate()->clone();
}

/*******************************************************************
********************************************************************/
linkList <field> *  grazingManager::daily_update(int *numOthers)
{
   if (enableGrazing)
   {
      int growthPeriod=0;
      static bool setNewPeriod;
      if (periodEnd)
      {
         if ((setNewPeriod)&&(initialised))
         {
            if ((currentPeriod<=maxPeriods)&&(SilageMade<ExpectedSilageHolding))
            {
               if (currentPeriod==0)
                  growthPeriod=0;
               else
                  growthPeriod=1;
               int period = CalcGrazingPeriod(periodEnd, growthPeriod);
               SetGrazing(periodEnd,period,numOthers);
               currentPeriod++;
               setNewPeriod=false;
            }
            else
               SetGrazingAll();
         }
         bool forceCut=CheckConservationFields();
         CheckOpStopsGrazing();		//see if any operations (eg ploughing) means that one or more field cannot be grazed
         if ((theTime[id]==*periodEnd)||(forceCut))
         {
            setNewPeriod=true;
            if (currentPeriod==0)
            {
               if (!initialised)
                  StartGrazingManagement();
               if (!CheckGrazingTheshold())
               {
                  setNewPeriod=false;
                  periodEnd->AddOneDay();
               }
               else
                  SilageMade=0.0;
            }
            else
               setNewPeriod=CutConservationFields(forceCut);
         }
         if ((theTime[id]==(*theCattleFeedingManager->GetHousingDate())))
         {
            setNewPeriod=false;
            EndOfSeason(true);
         }
      }
   }
   return fields_to_graze;
}


/*******************************************************************
********************************************************************/
double grazingManager::calcSilageRequirement()
{
	double silageDemand=theCattleFeedingManager->GetSilageRequired();
	return silageDemand;
}



/*******************************************************************
********************************************************************/
double grazingManager::GetHerbageAvailability(bool restricted)
{

   double herbageAvailable=0.0;
   int max;
	//If restricted is true, only return herbage that is on fields that are to be grazed
   if (restricted)
   	max = fields_to_graze->NumOfNodes();
   else
   	max = fields_avail_to_graze->NumOfNodes();

   for (int i=0;i<max;i++)
   {

      double fieldMassAvail = GetHerbageAvailability(restricted,i);
		herbageAvailable +=  fieldMassAvail;
   }
   return herbageAvailable;
};
/*******************************************************************
Returns herbage availability in kg DM for a field
********************************************************************/
double grazingManager::GetHerbageAvailability(bool restricted, int fieldNo)
{
   field *aField;
   if (restricted)
      aField = (field*) fields_to_graze->ElementAtNumber(fieldNo);
   else
      aField = (field*) fields_avail_to_graze->ElementAtNumber(fieldNo);
   double herbageAvailable = aField->GetAvailableHerbageMasskg(residualDM);
   if (herbageAvailable<0.0)
      	herbageAvailable=0.0;
   return herbageAvailable;
};



/*******************************************************************
Calculates the length of a grazing period
********************************************************************/
int grazingManager::CalcGrazingPeriod(bsTime *aDate, int growthPeriod)
{
	int period = 0;
   bsTime *tempTime = aDate->clone();
   double cummass=0.0;
   double cumarea=0.0;
 //  double currentAveMass=0.0;
   for (int i=0;i<fields_avail_to_graze->NumOfNodes();i++)
   {
      field *aField = fields_avail_to_graze->ElementAtNumber(i);
      cumarea+=aField->GetArea();
   }

   double aveMass=0.0;
//   if (cummass>0.0)
  // 	aveMass=cummass/cumarea;
   while ((aveMass< targetCutMass[growthPeriod])&&(*tempTime<=*theCattleFeedingManager->GetHousingDate()))
   {
      for (int i=0;i<fields_avail_to_graze->NumOfNodes();i++)
      {
	      field *aField = fields_avail_to_graze->ElementAtNumber(i);
         cummass+= aField->GetArea() * GetExpectedGrowthRate(tempTime, i)/1000.0;
      }
	   if (cummass>0.0)
   	   aveMass=cummass/cumarea;
      tempTime->AddOneDay();
   	period++;
   }
   delete tempTime;
   return period;
}



/*******************************************************************
NJH May 2004 Insert a cut into a field operation list
********************************************************************/
void grazingManager::InsertCut(field * aField, bool harvestCut)
{
   char * crop_id =new char[5];
   strcpy(crop_id,"G4");
   harvestFields * FO=new harvestFields;
   aField->PresentCrop_(crop_id);
   plantItem *aPlantItem1=new plantItem;
   aPlantItem1->Setname("RYEGRASS");
   theProducts[id]->GiveProductInformation(aPlantItem1);
   FO->SetMainProduct(aPlantItem1);

#ifndef MEASCOPE

   if (harvestCut)
      FO->DefineHarvestFieldsOper(ForageHarvesting,crop_id,"Pasture",
         theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay(),
         aField->GetArea(),0,aField->DistanceToField_(),1.0,true,residualDM,0); // @@@ Might be problems here regarding parameters !!!
   else
      FO->DefineHarvestFieldsOper(CutSetAside,crop_id,"Pasture",
         theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay(),
         aField->GetArea(),0,aField->DistanceToField_(),1.0,true,residualDM,0); // @@@ Might be problems here regarding parameters !!!
   aField->GivecrrPlan()->InsertAsNextOperation(FO);

#else
//      fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Cutting",0,aField);
      aField->GivecrrPlan()->SetFinished(false);
      double CuttingHeight = 0.07;
      int processProduct;
    	harvestFields * FO1 = NULL;
      if (harvestCut)
      {
      	processProduct = 1;
         FO->DefineHarvestFieldsOper(ForageHarvesting, crop_id,"Pasture",theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay(),
                        theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay()+1,0.0,
                        0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
      }
      else
      {
	      int processProduct = 0;  //set elsewhere  !!!!!!!!!!!!!!!!!!!!!!!!
         switch (processProduct)  // 0=cut set-aside, 1 = silage, 2 = hay
         {
            case 0:FO->DefineHarvestFieldsOper(CutSetAside, crop_id,"Pasture",theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay(),
                     theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay()+1,0.0,
                     0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
                  break;
            case 1:FO->DefineHarvestFieldsOper(Mowing,  crop_id,"Pasture",theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay(),
                     theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay()+1,0.0,
                     0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
                    //collect silage
                    FO1=new harvestFields;
                    FO1->DefineHarvestFieldsOper(GatheringSilage,  crop_id,"Pasture",theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay(),
                     theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay()+1,0.0,
                     0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
                    FO1->SetMainProduct(aPlantItem1);
                    break;
            case 2:FO->DefineHarvestFieldsOper(Mowing,  crop_id,"Pasture",theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay(),
                     theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay()+1,0.0,
                     0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
                    //collect silage
                    FO1=new harvestFields;
                    FO1->DefineHarvestFieldsOper(GatheringHay,  crop_id,"Pasture",theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay(),
                     theTime[id].GetYear(),theTime[id].GetMonth(),theTime[id].GetDay()+1,0.0,
                     0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
                    FO1->SetMainProduct(aPlantItem1);
                  break;
            default:
            theMessage[id]->FatalError("static crop - invalid process product in field no");//, FO->GetFieldNumber());
         }
      }
      FO->SetCuttingHeight(CuttingHeight);
	   aField->GivecrrPlan()->InsertAsNextOperation(FO);
      if (FO1)
            theMessage[id]->FatalError("grazing manager - operation not tested"); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//		   aField->GivecrrPlan()->InsertAsNextOperation(FO1);


#endif
   delete aPlantItem1;



}


/*******************************************************************
NJH Dec 2005 Estimate maximum accumulated growth for each field
Growth is in kg DM/ha
REPLACE WITH SOME STATIC CALCULATION, AS THIS METHOD DEPENDS ON A
PRECISE LOOK INTO THE FUTURE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!@@@
********************************************************************/
void grazingManager::InitialiseGrowthEstimates()
{
//   theMessage[id]->FatalError("grazing_manager_class::InitialiseGrowthEstimates() - this code is not completed");

   bsTime *tempTime = theTime[id].clone();
   double maxGrowthRate=0.0;
   int daysWithGrowth=0;
   int daysWithoutGrowth=0;
   theClimate[id]->CalcAverageClimate();
   int totalDays = 365;
   if (theTime[id].IsLeapYear(theTime[id].GetYear()))
   	totalDays=366;
   for (int j=0;j<totalDays;j++)
   {
	   bool hasGrown = false;
      theClimate[id]->Update(tempTime);
      int day = tempTime->GetDay();
      int month = tempTime->GetMonth();
      int year = tempTime->GetYear();
      for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
      {
         field *aField = theCropRotation ->GetFields()->ElementAtNumber(i);
         maxGrowthRate=potentialGrowthFactor * aField->InitExpectedGrowthRate(theClimate[id]->GetnormTemp(month), theClimate[id]->GetnormRad(month));
         int dayNumber = tempTime->GetJulianDay()-1;
         aField->SetexpectedGrowth(dayNumber,maxGrowthRate);
         if (maxGrowthRate>10.0)
         	hasGrown = true;
         if (tempTime->GetJulianDay(day, month,year)<springCutoffDay)
	         aField->AddExpectedGrowth(0,maxGrowthRate);
			else
  	         aField->AddExpectedGrowth(1,maxGrowthRate);
      }
      //estimate the dates when growth starts and ends
      if ((hasGrown)&&(tempTime->GetJulianDay(day, month,year)<springCutoffDay))
        	daysWithGrowth++;
      if ((!hasGrown)&&(tempTime->GetJulianDay(day, month,year)>=springCutoffDay))
        	daysWithoutGrowth++;
		if ((!growthStart)&&(tempTime->GetJulianDay(day, month,year)<springCutoffDay)&&(daysWithGrowth>10))
      	growthStart = tempTime->clone();
		if ((!growthEnd)&&(tempTime->GetJulianDay(day, month,year)>=springCutoffDay)&&(daysWithoutGrowth>10))
      	growthEnd = tempTime->clone();
      tempTime->AddOneDay();
   }
   delete tempTime;
   theClimate[id]->Update(&theTime[id]);
}



void grazingManager::StartGrazingManagement()
{

   fields_to_graze->ForgetAll();
   AutoGrazingPossible();
   fields_avail_to_graze->ForgetAll();
   for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
   {
      field * aField = theCropRotation ->GetFields()->ElementAtNumber(i);
      if (aField->GetActive() && aField->GetGrazingPossible())
      {
			aField->SetGrazingShutDown(false);
         aField->GivecrrPlan()->RemoveHerbageHarvests();
			fields_avail_to_graze->InsertLast(aField);
      }
   }
   SortFields();
   SilageMade =0.0;
	initialised=true;
   theMessage[id]->LogEventWithTime("Grazing management started");
}

/***************************************
Sort fields into order of distance from steading
***************************************/
void grazingManager::SortFields()
{
   int counter;
	temp_double_array distances;
	temp_ptr_array ptrlist;
   int numOfFields = fields_avail_to_graze->NumOfNodes();
   for (counter=numOfFields-1;counter>=0;counter--)
   {
   	field *aField = fields_avail_to_graze->ElementAtNumber(counter);
      distances[counter]=aField->DistanceToField_();
   	ptrlist[counter]=(void*) aField;
 	}
   Comsort(distances, ptrlist, numOfFields);
   fields_avail_to_graze->ForgetAll();
   for (counter=0;counter<numOfFields;counter++)
   {
		fields_avail_to_graze->InsertLast((field*) ptrlist[counter]);
   }
}

/***************************************
***************************************/
void grazingManager::SetGrazing(bsTime * aDate, int duration, int *numOthers)
{
	double grazedFeedReq;
   fields_to_graze->ForgetAll();
   aDate->AddDays(duration);
   double currentHerbageMass=0.0;
//start with followers
   grazedFeedReq =(double) duration * theCattleFeedingManager->GetFollowerHerbageRequired(false);     //in tonnes DM
   double feedAvailable=0.0; //in tonnes DM
   double newGrowth=0.0;
   int i=fields_avail_to_graze->NumOfNodes()-1;
   //allocate fields, starting with those that are furthest from the steading
   while ((i>=0)&&(feedAvailable < grazedFeedReq))
   {
   	field *aField = fields_avail_to_graze->ElementAtNumber(i);
      aField->SetGrazingShutDown(false);
		fields_to_graze->InsertLast(aField);
      newGrowth+=duration * GetAverageGrowthRate(aDate, i, duration) * aField->GetArea()/1000.0;
      currentHerbageMass+= aField->GetAvailableHerbageMasskg(residualDM)/1000.0;
      feedAvailable = currentHerbageMass + newGrowth;
      i--;
   }
   int minOthers = i+1;   //lowest number of field in fields_avail_to_graze that has been allocated to animals other than dairy cattle
   *numOthers = fields_to_graze->NumOfNodes();  //number of fields grazed by animals other than dairy cattle

//Now allocate fields to dairy cattle, starting with those closest to the steading
   grazedFeedReq =((double) duration) * theCattleFeedingManager->GetDairyHerbageRequired(false);   //tonnes DM
   newGrowth=0.0;
   feedAvailable=0.0;
   currentHerbageMass=0.0;
   i=0;
   while ((i<minOthers)&&(feedAvailable < grazedFeedReq))
   {
   	field *aField = fields_avail_to_graze->ElementAtNumber(i);
      aField->SetGrazingShutDown(false);
		fields_to_graze->InsertLast(aField);
      newGrowth+=duration * GetAverageGrowthRate(aDate, i, duration) * aField->GetArea()/1000.0;
      currentHerbageMass+= aField->GetAvailableHerbageMasskg(residualDM)/1000.0;
      feedAvailable = currentHerbageMass + newGrowth;
      i++;
   }

//shut down grazing on any remaining fields
   for (int counter=i;counter<minOthers;counter++)
   {
   	field *aField = fields_avail_to_graze->ElementAtNumber(counter);
      aField->SetGrazingShutDown(true);
   }
   if (fields_to_graze->NumOfNodes()>0)
	   theMessage[id]->LogEventWithTime("Fields grazed ", fields_to_graze->NumOfNodes());
}

/*****************************
Returns the expected growth rate from a particular field for a date, measured in kgDM/ha/d
REPLACE WITH SOME STATIC CALCULATION, AS THIS METHOD DEPENDS ON A
PRECISE LOOK INTO THE FUTURE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!@@@
*****************************/
double grazingManager::GetExpectedGrowthRate(bsTime *aDate, int fieldNo)
{
 	field *aField = fields_avail_to_graze->ElementAtNumber(fieldNo);
   double ret_val;
//   double expectedGrowth =aField->GetExpectedGrowth(aDate); //cumulative growth in either spring or summer
   int JulianaDate = aDate->GetJulianDay();
   ret_val =aField->GetexpectedGrowth(JulianaDate); //cumulative growth in either spring or summer
/*   int JulianStart = growthStart->GetJulianDay();
   int JulianEnd = growthEnd->GetJulianDay();
   if (JulianaDate<=springCutoffDay)              //calculate the appropriate expected daily growth rate in kg DM/ha
     ret_val = 2 * (JulianaDate - JulianStart) * expectedGrowth/((springCutoffDay - JulianStart)*(springCutoffDay - JulianStart));
   else
     ret_val = 2 *(JulianaDate - springCutoffDay) * expectedGrowth/((JulianEnd-springCutoffDay)*(JulianEnd-springCutoffDay));*/


   return ret_val;
}

/*****************************
*****************************/
bool grazingManager::CutConservationFields(bool forceCut)
{
	bool ret_val=false;
	int growthPeriod=0;
   if (theTime[id].GetJulianDay()>springCutoffDay)
   	growthPeriod=1;
	int numFieldsToCut=0;
   int numOfFields = fields_avail_to_graze->NumOfNodes();
   int numCuttableFields= numOfFields-fields_to_graze->NumOfNodes();
   for (int counter=0;counter<numOfFields;counter++)
   {
   	field *aField = fields_avail_to_graze->ElementAtNumber(counter);
      if (aField->GetGrazingShutDown())
      {
      	double massDensity =aField->GetAvailableHerbageMasskgPerHa(residualDM)/1000.0;
      	if ( (massDensity>=targetCutMass[growthPeriod])||(forceCut))
               numFieldsToCut++;  //Count fields to cut
      }
   }
   if (numCuttableFields>0)
   {
   	if (2*numFieldsToCut>=numCuttableFields) //only cut when half of the cuttable fields are ready
      {
         for (int counter=0;counter<numOfFields;counter++)
         {
            field *aField = fields_avail_to_graze->ElementAtNumber(counter);
            if (aField->GetGrazingShutDown())
            {
            	SilageMade+=aField->GetAvailableHerbageMasskg(residualDM)/1000.0;
               InsertCut(aField,true);  //Cut that grass!!
            }
         }
         ret_val=true;
      }
      else
         periodEnd->AddOneDay();
   }
   else
		ret_val=true;

   return ret_val;
}

/*******************************************************************
NJH May 2004 Estimate average growth rate in next period (kg DM/ha/day)
********************************************************************/
double grazingManager::GetAverageGrowthRate(bsTime * aDate, int fieldNo, int duration)
{
	double growthRate=0.0;
   bsTime *tempTime = aDate->clone();
   int JulianStartPeriod = aDate->GetJulianDay();
   int JulianEndPeriod = JulianStartPeriod + duration;
   for (int i=JulianStartPeriod; i<JulianEndPeriod; i++)
   {
		growthRate+=GetExpectedGrowthRate(tempTime, fieldNo);
      tempTime->AddOneDay();
   }
   growthRate/=duration;
   delete tempTime;
   return growthRate;
}

void grazingManager::EndOfSeason(bool topping)
{
   int numOfFields = fields_avail_to_graze->NumOfNodes();
   for (int counter=0;counter<numOfFields;counter++)
   {
      field *aField = fields_avail_to_graze->ElementAtNumber(counter);
     	double massDensity =aField->GetAvailableHerbageMasskgPerHa(residualDM)/1000.0;
      if ((massDensity>=targetCutMass[1])&& (!topping))
         InsertCut(aField,true);  //Cut grass and harvest
      else
         InsertCut(aField,false);  //Cut grass but leave it lying
   }
	currentPeriod=0;
   delete periodEnd;
   initialised=false;
	periodEnd=theCattleFeedingManager->GetTurnoutDate()->clone();
}

/***************************************
***************************************/
void grazingManager::SetGrazingAll()
{
   fields_to_graze->ForgetAll();
   for (int i=0;i<fields_avail_to_graze->NumOfNodes();i++)
   {
      field *aField = fields_avail_to_graze->ElementAtNumber(i);
      aField->SetGrazingShutDown(false);
		fields_to_graze->InsertLast(aField);
   }

//   theMessage[id]->LogEvent("Fields grazed ", fields_to_graze->NumOfNodes());
}

bool grazingManager::CheckGrazingTheshold()
{
//	double areaAvailable = GetGrazingAreaAvailable();
   double grazedFeedReq =theCattleFeedingManager->GetHerbageRequired(0,theCattleFeedingManager->GetNumFeedingGroups()-1, false);
	grazedFeedReq +=theCattleFeedingManager->GetHerbageRequired(0,theCattleFeedingManager->GetNumFeedingGroups()-1, true);
   if (grazedFeedReq>0.0)
   {
		double herbageAllowance = GetHerbageAvailability(false)/grazedFeedReq;
	   if (herbageAllowance>turnoutThreshold)
      	return true;
      else
      	return false;
   }
	return false;

}
void grazingManager::GetStatus()
{
   //int numGrazableFields = GetNumGrazableFields();
//   int numFieldsGrazed = fields_avail_to_graze->NumOfNodes();
   if (globalSettings[id]->DetailsData.getPastureDetails())
   {
   	*grazmanfile << theTime[id].GetYear() << "-" << theTime[id].GetMonth() << "-" << theTime[id].GetDay() << "\t";
	   for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
   	{
         double proteinConc = 0.0;
         double FEperKgDM = 0.0;
	      field *aField = theCropRotation ->GetFields()->ElementAtNumber(i);
         double mass = aField->GetHerbageMasskg();
         double area = aField->GetArea();
//      	if (aField->GetActive() && aField->GetGrazingPossible())
      	if (aField->GetActive())
      	{
            if (aField->GetHerbageMasskgPerHa()>100.0)    //avoid daft values that occur just after sowing
            {
               proteinConc=aField->GetAbovegroundCropN()*N_to_protein()*1000.0/mass;
               FEperKgDM = N_to_FE(proteinConc/(N_to_protein() * 1000.0));
            }
            mass/=1000.0;
         }
         *grazmanfile << i << "\t" << (mass/area) << "\t" << proteinConc << "\t" << FEperKgDM
         		<< "\t" << aField->GetDailyDMProduction()/area <<"\t" << aField->GetDMGrazed()/area<< "\t";
      }
      *grazmanfile << endl;
	}
	for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
   {
   	field *aField = theCropRotation ->GetFields()->ElementAtNumber(i);
   	aField->ClearTemporaryVariables();
   }
}

void grazingManager::InitialiseFileOutput()
{
   char filename[20];
   char leader[200];

   grazmanfile = new fstream();
   sprintf(leader,globalSettings[id]->getOutputDirectory().c_str());
#ifdef __BCplusplus
   sprintf(filename,"pasture.xls",Index);
#else
   sprintf(filename,"pasture.xls");
#endif
   strcat(leader,filename);
   grazmanfile->open(leader,ios::out);
   grazmanfile->precision(8);

   *grazmanfile << "Date" << "\t";
//   int numGrazableFields = GetNumGrazableFields();
   for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
   {
      *grazmanfile << "FieldNO" << "\t" << "HerbageMass" << "\t" << "ProteinConc" << "\t"<< "FEperKgDM" << "\t"
      		<< "DMProd" << "\t"<< "DMGrazed" << "\t" << "GLAI" << "\t";
   }
   *grazmanfile << endl;
   *grazmanfile << " "<< "\t";
   for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
   {
      *grazmanfile << " " << "\t" << "tDM/ha" << "\t" << "g/kg" << "\t"<< " " << "\t"<< "kgDM/ha/d"<< "\t"<< "kgDM/ha/d" << "\t";
   }
   *grazmanfile << endl;
}


/*****************************
*****************************/
bool grazingManager::CheckConservationFields()
{
	bool ret_val=false;
//	int numFieldsToCut=0;
   int numOfFields = fields_avail_to_graze->NumOfNodes();
//   cout << theTime[id].GetDay() << " " << theTime[id].GetMonth() << " " << numOfFields << endl;
   for (int counter=0;counter<numOfFields;counter++)
   {
   	field *aField = fields_avail_to_graze->ElementAtNumber(counter);
      if (aField->GetGrazingShutDown())
      {
         double mass = aField->GetHerbageMasskg();
         double proteinConc = 0.0;

      	double massDensity =aField->GetAvailableHerbageMasskgPerHa(residualDM)/1000.0;
 //        double area = aField->GetArea();
         if (massDensity>minHarvestable)    //avoid daft values that occur just after sowing
         {
            proteinConc=aField->GetAbovegroundCropN()*N_to_protein()/mass;

            proteinConc*=1000.0;  //convert to g/kg DM
//            if ((FEperKgDM<GetminEnergyConc())||(proteinConc<GetminProteinConc()))
//            if (proteinConc<GetminProteinConc())
//            	ret_val=true;
            	ret_val=false;
         }
      }
   }
   return ret_val;
}

int grazingManager::GetGrazingSupplyStatus(double numberOfDaysSupply)
{
	int status=1;
   {
      if (numberOfDaysSupply<=lowerGrazingLimit)
         status=0;                                 //deficiency of grazed pasture
      if (numberOfDaysSupply>=upperGrazingLimit)
         status=2;                                 //surplus of grazed pasture
   }
   return status;
}



void grazingManager::AutoGrazingPossible()
{
   for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
   {
      field *aField = theCropRotation ->GetFields()->ElementAtNumber(i);
      aField->AutoGrazingPossible();
   }
}


/****************************************************
See if one or more fields will have an operation that will stop grazing (eg ploughed)  NJH Sep 2007
*****************************************************/
void grazingManager::CheckOpStopsGrazing()
{
	linkList<field>::PS P;
   fields_to_graze->PeekHead(P);
   while (P!=NULL)
   {
      if (P->element->GetGrazingShutDown())
         fields_to_graze->Release(P);
      fields_to_graze->OneStep(P);
   }
}
