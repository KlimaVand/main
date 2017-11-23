/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
//Warning! This class currently contains various nasty hardcodings
//Should be restructured now that all cattle are instanced as dairy

// Class: grazing_manager_class
#include <common.h>
#include <grazing_manager_class.h>
#include <stable.h>
#include <field.h>
#include <patch.h>
#include <climclas.h>
#include <output.h>
#include <nixcbits.h>

/*******************************************************************
********************************************************************/
grazing_manager_class::grazing_manager_class ()
{
  fields_to_graze= new linkList <field>;
  fields_avail_to_graze= new linkList <field>;
  theCattle = new linkList <dairyStock>;
  dairyHouse = NULL;
  followerHouse = NULL;
  ExpectedSilageHolding = 0.0;
  periodStart = new linkList <bsTime>;
  initialised = false;
  growthStart = NULL;
  growthEnd = NULL;
  targetCutMass[0]=2.5;
  targetCutMass[1]=2.0;
  periodEnd=NULL;
  maxPeriods = 10;
  SilageMade =0.0;
  grazmanfile=NULL;
  residualHeight=0.0;
  minHarvestable=0.0;
  minProteinConc=0.0;
  minEnergyConc=0.0;
  lowerGrazingLimit=0.0;
  upperGrazingLimit=0.0;
}

/*******************************************************************
********************************************************************/
grazing_manager_class::~grazing_manager_class()
{
 fields_to_graze->ForgetAll();
 fields_avail_to_graze->ForgetAll();
 theCattle->ForgetAll();
 delete fields_to_graze;
 delete fields_avail_to_graze;
 delete periodStart;
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
void grazing_manager_class::Initialize(livestock * theLivestockptr,
									   cropRotation * theCropRotationptr,
                              buildings * theBuildingsptr, cattlefeeding_manager_class * theCattleFeedingManagerptr)
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
   if ((theOutputControl->GetPastureDetails())&& (!grazmanfile))
   	InitialiseFileOutput();
}

/*******************************************************************
********************************************************************/
void grazing_manager_class::HouseAnimals()
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
void grazing_manager_class::SetGrazableFields()
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
      if ((initialised)&&(theTime<=*housingDate))
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
void grazing_manager_class::ReceivePlan(char *fileName)
{
   if(!OpenInputFile(fileName))
		theMessage->FatalError("grazing_manager::ReceivePlan - error in opening ",fileName);
   else
   {
	  Setfile(file);
      SetCritical();
      FindSection("Grazing",0);
      GetParameter("enableGrazing",&enableGrazing);
      if (enableGrazing)
      {
         GetParameter("residualHeight",&residualHeight);
         GetParameter("minHarvestable",&minHarvestable);
         GetParameter("minProteinConc",&minProteinConc);
         GetParameter("minEnergyConc",&minEnergyConc);
         GetParameter("turnoutThreshold",&turnoutThreshold);
         GetParameter("lowerGrazingLimit",&lowerGrazingLimit);
         GetParameter("upperGrazingLimit",&upperGrazingLimit);
         GetParameter("springCutoffDay",&springCutoffDay);
         GetParameter("potentialGrowthFactor",&potentialGrowthFactor);
         GetParameter("maxPeriods",&maxPeriods);
      }
   }
   CloseInputFile();
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
linkList <field> *  grazing_manager_class::daily_update(int *numOthers)
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
         if ((theTime==*periodEnd)||(forceCut))
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
         if ((theTime==(*theCattleFeedingManager->GetHousingDate())))
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
double grazing_manager_class::calcSilageRequirement()
{
	double silageDemand=theCattleFeedingManager->GetSilageRequired();
	return silageDemand;
}

/*******************************************************************
NJH Aug 2002
Checks to see if silage is accumulating - sell some if necessary
********************************************************************/
void grazing_manager_class::checkSilageHolding()
{
 double silageHolding =0.0;
 double amountToSell = 0.0;
 feedItem * grassSilage = (feedItem*) theProducts->GetProductElement("FEEDRYEGRASS");
 grassSilage->Setamount(0.0);
 silageHolding = theBuildings->AmountProductStored(grassSilage) * grassSilage->GetdryMatter();
 feedItem * grasscloverSilage = (feedItem*) theProducts->GetProductElement("FEEDGRASSCLOVER");
 grasscloverSilage->Setamount(0.0);
 silageHolding = theBuildings->AmountProductStored(grasscloverSilage)  * grasscloverSilage->GetdryMatter();
 if (silageHolding>ExpectedSilageHolding)
 {
  if (grassSilage->GetAmount()>0)
  {
     feedItem * aplantItem = (feedItem*) grassSilage->clone();
     amountToSell = (silageHolding-ExpectedSilageHolding)/grassSilage->GetdryMatter();
     aplantItem->Setamount(amountToSell);
	  theProducts->SellPlantItem(aplantItem); //try selling grass silage first
     delete aplantItem;
     checkSilageHolding();                    //check to see if need to look to sell grassclover silage
  }
  else
  {  //no pure grass silage, try selling grassclover silage
     feedItem * aplantItem = (feedItem *) grasscloverSilage->clone();
     amountToSell = (silageHolding-ExpectedSilageHolding)/grasscloverSilage->GetdryMatter();
     aplantItem->Setamount(amountToSell);
	  theProducts->SellPlantItem(aplantItem);
     delete aplantItem;
  }
  theOutput->AddIndicator(economicIndicator,"19.70 Exported roughage","t",amountToSell);

 }
}

/*******************************************************************
NJH Aug 2002
Checks to see if DM is accumulating on pasture - cut some if necessary
********************************************************************/
void grazing_manager_class::checkPasture()
{
   field *aField;
   double herbageMass;
   for (int i=0;i<fields_avail_to_graze->NumOfNodes();i++)
   {
		aField = (field*) fields_avail_to_graze->ElementAtNumber(i);
		herbageMass = aField->GetHerbageMasskgPerHa();
      if (herbageMass>2.0)
         InsertCut(aField, true);
	}
}

/*******************************************************************
********************************************************************/
double grazing_manager_class::GetHerbageAvailability(bool restricted)
{
   field *aField;
   double herbageAvailable=0.0;
   int max;
	//If restricted is true, only return herbage that is on fields that are to be grazed
   if (restricted)
   	max = fields_to_graze->NumOfNodes();
   else
   	max = fields_avail_to_graze->NumOfNodes();

   for (int i=0;i<max;i++)
   {
	   if (restricted)
			aField = (field*) fields_to_graze->ElementAtNumber(i);
      else
			aField = (field*) fields_avail_to_graze->ElementAtNumber(i);
      double fieldMassAvail = GetHerbageAvailability(restricted,i);
		herbageAvailable +=  fieldMassAvail;
   }
   return herbageAvailable;
};
/*******************************************************************
Returns herbage availability in kg DM for a field
********************************************************************/
double grazing_manager_class::GetHerbageAvailability(bool restricted, int fieldNo)
{
   field *aField;
   if (restricted)
      aField = (field*) fields_to_graze->ElementAtNumber(fieldNo);
   else
      aField = (field*) fields_avail_to_graze->ElementAtNumber(fieldNo);
   double herbageAvailable = aField->GetAvailableHerbageMasskg(residualHeight);
   if (herbageAvailable<0.0)
      	herbageAvailable=0.0;
   return herbageAvailable;
};

/*******************************************************************
Gets the total area that is potentially available for grazing (ha)
********************************************************************/
double grazing_manager_class::GetGrazingAreaAvailable()
{
   field *aField;
   double area=0.0;
   for (int i=0;i<fields_avail_to_graze->NumOfNodes();i++)
   {
		aField = (field*) fields_avail_to_graze->ElementAtNumber(i);
      area+= aField->GetArea();
   }
   return area;
}

/*******************************************************************
Calculates the length of a grazing period
********************************************************************/
int grazing_manager_class::CalcGrazingPeriod(bsTime *aDate, int growthPeriod)
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
Allocate those fields that will be grazed in the next period
********************************************************************/
void grazing_manager_class::SetFieldsGrazed(bsTime * aDate, int duration, double herbageRequired)
{
}

/*******************************************************************
Allocate those fields that will be grazed tomorrow - use only for single field simulations
********************************************************************/
void grazing_manager_class::SetFieldsGrazed(double herbageRequired)
{
	fields_to_graze->ForgetAll();
   for (int i=0;i<fields_avail_to_graze->NumOfNodes();i++)         //fields should be in order of distance from farm buildings
   {
   	field *aField = (field*) fields_avail_to_graze->ElementAtNumber(i);
      if (herbageRequired > 0.0)
      {
        double herbageMass = aField->GetHerbageMasskgPerHa();
        if (herbageMass > 5.0) // Never!!!!!! (5.5) Value ???!!!???
                  InsertCut(aField, true);
        if ((herbageMass > 1.75)&&(aField->GetGrazingShutDown()==true))
        {
            aField->SetGrazingShutDown(false);
		      theMessage->LogEventWithTime("Field opened for grazing, number: ", aField->GetIndex());
        }
        if ((herbageMass < 1.5) &&(aField->GetGrazingShutDown()==false))
        {
            aField->SetGrazingShutDown(true);
		      theMessage->LogEventWithTime("Field closed for grazing, number: ", aField->GetIndex());
        }
        if (aField->GetGrazingShutDown()==false)
                  fields_to_graze->InsertLast(aField);
      }
   }
}


/*******************************************************************
NJH May 2004 Insert a cut into a field operation list
********************************************************************/
void grazing_manager_class::InsertCut(field * aField, bool harvestCut)
{
   char * crop_id = "G4";
   harvestFields * FO=new harvestFields;
   aField->PresentCrop_(crop_id);
   plantItem *aPlantItem1=new plantItem;
   aPlantItem1->Setname("RYEGRASS");
   theProducts->GiveProductInformation(aPlantItem1);
   FO->SetMainProduct(aPlantItem1);

#ifndef MEASCOPE

   if (harvestCut)
      FO->DefineHarvestFieldsOper(ForageHarvesting,crop_id,"Pasture",
         theTime.GetYear(),theTime.GetMonth(),theTime.GetDay(),
         aField->GetArea(),0,aField->DistanceToField_(),1.0,true,residualHeight,0); // @@@ Might be problems here regarding parameters !!!
   else
      FO->DefineHarvestFieldsOper(CutSetAside,crop_id,"Pasture",
         theTime.GetYear(),theTime.GetMonth(),theTime.GetDay(),
         aField->GetArea(),0,aField->DistanceToField_(),1.0,true,residualHeight,0); // @@@ Might be problems here regarding parameters !!!
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
         FO->DefineHarvestFieldsOper(ForageHarvesting, crop_id,"Pasture",theTime.GetYear(),theTime.GetMonth(),theTime.GetDay(),
                        theTime.GetYear(),theTime.GetMonth(),theTime.GetDay()+1,0.0,
                        0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
      }
      else
      {
	      int processProduct = 0;  //set elsewhere  !!!!!!!!!!!!!!!!!!!!!!!!
         switch (processProduct)  // 0=cut set-aside, 1 = silage, 2 = hay
         {
            case 0:FO->DefineHarvestFieldsOper(CutSetAside, crop_id,"Pasture",theTime.GetYear(),theTime.GetMonth(),theTime.GetDay(),
                     theTime.GetYear(),theTime.GetMonth(),theTime.GetDay()+1,0.0,
                     0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
                  break;
            case 1:FO->DefineHarvestFieldsOper(Mowing,  crop_id,"Pasture",theTime.GetYear(),theTime.GetMonth(),theTime.GetDay(),
                     theTime.GetYear(),theTime.GetMonth(),theTime.GetDay()+1,0.0,
                     0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
                    //collect silage
                    FO1=new harvestFields;
                    FO1->DefineHarvestFieldsOper(GatheringSilage,  crop_id,"Pasture",theTime.GetYear(),theTime.GetMonth(),theTime.GetDay(),
                     theTime.GetYear(),theTime.GetMonth(),theTime.GetDay()+1,0.0,
                     0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
                    FO1->SetMainProduct(aPlantItem1);
                    break;
            case 2:FO->DefineHarvestFieldsOper(Mowing,  crop_id,"Pasture",theTime.GetYear(),theTime.GetMonth(),theTime.GetDay(),
                     theTime.GetYear(),theTime.GetMonth(),theTime.GetDay()+1,0.0,
                     0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
                    //collect silage
                    FO1=new harvestFields;
                    FO1->DefineHarvestFieldsOper(GatheringHay,  crop_id,"Pasture",theTime.GetYear(),theTime.GetMonth(),theTime.GetDay(),
                     theTime.GetYear(),theTime.GetMonth(),theTime.GetDay()+1,0.0,
                     0.0, 0,aField->GetArea(),0,aField->DistanceToField_(),1.0,true,0.95,0.95);
                    FO1->SetMainProduct(aPlantItem1);
                  break;
            default:
            theMessage->FatalError("static crop - invalid process product in field no");//, FO->GetFieldNumber());
         }
      }
      FO->SetCuttingHeight(CuttingHeight);
	   aField->GivecrrPlan()->InsertAsNextOperation(FO);
      if (FO1)
            theMessage->FatalError("grazing manager - operation not tested"); //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
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
void grazing_manager_class::InitialiseGrowthEstimates()
{
//   theMessage->FatalError("grazing_manager_class::InitialiseGrowthEstimates() - this code is not completed");

   bsTime *tempTime = theTime.clone();
   double maxGrowthRate=0.0;
   int daysWithGrowth=0;
   int daysWithoutGrowth=0;
   theClimate->CalcAverageClimate();
   int totalDays = 365;
   if (theTime.IsLeapYear(theTime.GetYear()))
   	totalDays=366;
   for (int j=0;j<totalDays;j++)
   {
	   bool hasGrown = false;
      theClimate->Update(tempTime);
      int day = tempTime->GetDay();
      int month = tempTime->GetMonth();
      int year = tempTime->GetYear();
      for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
      {
         field *aField = theCropRotation ->GetFields()->ElementAtNumber(i);
         maxGrowthRate=potentialGrowthFactor * aField->InitExpectedGrowthRate(theClimate->GetnormTemp(month), theClimate->GetnormRad(month));
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
   theClimate->Update(&theTime);
}



void grazing_manager_class::StartGrazingManagement()
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
   theMessage->LogEventWithTime("Grazing management started");
}

/***************************************
Sort fields into order of distance from steading
***************************************/
void grazing_manager_class::SortFields()
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
void grazing_manager_class::SetGrazing(bsTime * aDate, int duration, int *numOthers)
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
      currentHerbageMass+= aField->GetAvailableHerbageMasskg(residualHeight)/1000.0;
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
      currentHerbageMass+= aField->GetAvailableHerbageMasskg(residualHeight)/1000.0;
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
	   theMessage->LogEvent("Fields grazed ", fields_to_graze->NumOfNodes());
}

/*****************************
Returns the expected growth rate from a particular field for a date, measured in kgDM/ha/d
REPLACE WITH SOME STATIC CALCULATION, AS THIS METHOD DEPENDS ON A
PRECISE LOOK INTO THE FUTURE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!@@@
*****************************/
double grazing_manager_class::GetExpectedGrowthRate(bsTime *aDate, int fieldNo)
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
bool grazing_manager_class::CutConservationFields(bool forceCut)
{
	bool ret_val=false;
	int growthPeriod=0;
   if (theTime.GetJulianDay()>springCutoffDay)
   	growthPeriod=1;
	int numFieldsToCut=0;
   int numOfFields = fields_avail_to_graze->NumOfNodes();
   int numCuttableFields= numOfFields-fields_to_graze->NumOfNodes();
   for (int counter=0;counter<numOfFields;counter++)
   {
   	field *aField = fields_avail_to_graze->ElementAtNumber(counter);
      if (aField->GetGrazingShutDown())
      {
      	double massDensity =aField->GetAvailableHerbageMasskgPerHa(residualHeight)/1000.0;
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
            	SilageMade+=aField->GetAvailableHerbageMasskg(residualHeight)/1000.0;
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
double grazing_manager_class::GetAverageGrowthRate(bsTime * aDate, int fieldNo, int duration)
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

void grazing_manager_class::EndOfSeason(bool topping)
{
   int numOfFields = fields_avail_to_graze->NumOfNodes();
   for (int counter=0;counter<numOfFields;counter++)
   {
      field *aField = fields_avail_to_graze->ElementAtNumber(counter);
     	double massDensity =aField->GetAvailableHerbageMasskgPerHa(residualHeight)/1000.0;
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
void grazing_manager_class::SetGrazingAll()
{
   fields_to_graze->ForgetAll();
   for (int i=0;i<fields_avail_to_graze->NumOfNodes();i++)
   {
      field *aField = fields_avail_to_graze->ElementAtNumber(i);
      aField->SetGrazingShutDown(false);
		fields_to_graze->InsertLast(aField);
   }

//   theMessage->LogEvent("Fields grazed ", fields_to_graze->NumOfNodes());
}

bool grazing_manager_class::CheckGrazingTheshold()
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
void grazing_manager_class::GetStatus()
{
   //int numGrazableFields = GetNumGrazableFields();
//   int numFieldsGrazed = fields_avail_to_graze->NumOfNodes();
   if (theOutputControl->GetPastureDetails())
   {
   	*grazmanfile << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
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
         		<< "\t" << aField->GetDailyDMProduction()/area <<"\t" << aField->GetDMGrazed()/area<< "\t" << aField->GetPatch(0)->GetTotalLAI() << "\t";
      }
      *grazmanfile << endl;
	}
	for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
   {
   	field *aField = theCropRotation ->GetFields()->ElementAtNumber(i);
   	aField->ClearTemporaryVariables();
   }
}

void grazing_manager_class::InitialiseFileOutput()
{
   char filename[20];
   char leader[100];

   grazmanfile = new fstream();
   sprintf(leader,theOutputControl->GetoutputDirectory());
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
bool grazing_manager_class::CheckConservationFields()
{
	bool ret_val=false;
//	int numFieldsToCut=0;
   int numOfFields = fields_avail_to_graze->NumOfNodes();
//   cout << theTime.GetDay() << " " << theTime.GetMonth() << " " << numOfFields << endl;
   for (int counter=0;counter<numOfFields;counter++)
   {
   	field *aField = fields_avail_to_graze->ElementAtNumber(counter);
      if (aField->GetGrazingShutDown())
      {
         double mass = aField->GetHerbageMasskg();
         double proteinConc = 0.0;
         double FEperKgDM = 0.0;
      	double massDensity =aField->GetAvailableHerbageMasskgPerHa(residualHeight)/1000.0;
 //        double area = aField->GetArea();
         if (massDensity>minHarvestable)    //avoid daft values that occur just after sowing
         {
            proteinConc=aField->GetAbovegroundCropN()*N_to_protein()/mass;
            FEperKgDM = N_to_FE(proteinConc/(N_to_protein()));
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

int grazing_manager_class::GetGrazingSupplyStatus(double numberOfDaysSupply)
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


int grazing_manager_class::GetNumGrazableFields()
{
	int numGrazableFields = 0;
   for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
   {
      field *aField = theCropRotation ->GetFields()->ElementAtNumber(i);
      if (aField->GetActive() && aField->GetGrazingPossible())
      	numGrazableFields++;
   }
	return numGrazableFields;
}

void grazing_manager_class::AutoGrazingPossible()
{
   for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
   {
      field *aField = theCropRotation ->GetFields()->ElementAtNumber(i);
      aField->AutoGrazingPossible();
   }
}

/****************************************************
Clears variables that record DM grazed from grazed crops
If need to retain details of grazing (e.g. for debugging, interpretation), remove clearing
code from cropxx.cpp and insert call to this function in daily_update
*****************************************************/
void grazing_manager_class::ClearTemporaryVariables()
{
	for (int i=0;i<theCropRotation ->GetFields()->NumOfNodes();i++)
   {
   	field *aField = theCropRotation ->GetFields()->ElementAtNumber(i);
   	aField->ClearTemporaryVariables();
   }
}

/****************************************************
See if one or more fields will have an operation that will stop grazing (eg ploughed)  NJH Sep 2007
*****************************************************/
void grazing_manager_class::CheckOpStopsGrazing()
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
