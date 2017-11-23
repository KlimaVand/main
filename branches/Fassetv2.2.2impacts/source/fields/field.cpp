/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "../data/outputData.h"
#include "../data/systemData.h"
#include "field.h"

#include "fieldOperation/fieldOperationFields.h"
#include "fieldOperation/animalFertFields.h"
#include "fieldOperation/harvestFields.h"
#include "fieldOperation/irrigateFields.h"
#include "fieldOperation/mineralFertFields.h"
#include "fieldOperation/manualLabourFields.h"
#include "fieldOperation/sowFields.h"
#include "fieldOperation/soilTreatFields.h"
#include "fieldOperation/plantCareFields.h"
#include "fieldOrder.h"
#include "../base/message.h"
#include "../technics/technics.h"
#include "../base/IndicatorOutput.h"
#include "../building/thebuild.h"
#include "../base/timeUse.h"
#include "../base/controlParameters.h"

/****************************************************************************\
Constructor with arguments
\****************************************************************************/
field::field(const char * aName, const int aIndex, const base * aOwner)
   :base(aName,aIndex,aOwner)
{
   testover                   =false;
	Lfutr                      =new cloneList<plan>;
	crrPlan                    =NULL;
   thePatches                 =new cloneList<patch>;
   Minimum                    =0.00001;
   MinArea                    =0.0;
   NoFixCrops                 =0;
   fenced                     =0;
   JBNO                       =0;
   daysSinceIrrigation        =0;
	daysSinceIrrigationRequest =0;
   fingerFlow				      =0;
   daysBetweenIrrigation      =0;
   irrigationDelay            =0;
	distance                   =0.0;
   area                       =0.0;
   active                     =true;
   monitorPatches             =false; // !!! Read from some file later!!!
#ifdef TUPLE
   writeTuples                =false;
   tuple_file_open            =false;
#endif
   last_date                  ="";
   deposition_sum             =0.0;
   fixation_sum               =0.0;

   for (int i=0;i<BackScope;i++)
   {
      History[i].HUG_N=0.0;
      strcpy(History[i].crop,"");
   }

   for (int i=0;i<BackScope;i++)
   {
      FixedCrop[i]=NULL;
      FixedSize[i]=0.0;
   }

   if (monitorPatches)
   {
      char filename[40];
      char leader[100];
      sprintf(leader,theSystemData->getOutputDirectory().c_str());

      sprintf(filename,"patches%d1.xls",Index);
      strcat(leader,filename);
      fPatch1.open(leader,ios::out);
      fPatch1.precision(8);
      fPatch1 << "Date\t";
      for (int i=0;i<MaxWritePatches;i++)
         fPatch1 << "Area" << i << "\t";


      sprintf(leader,theSystemData->getOutputDirectory().c_str());
	   sprintf(filename,"patches%d2.xls",Index);
      strcat(leader,filename);
      fPatch2.open(leader,ios::out);
      fPatch2.precision(8);
      fPatch2 << "Date\t";
      for (int i=0;i<MaxWritePatches;i++)
         fPatch2 << "Area" << i << "\t";

      fPatch1 << "Date\t";
      for (int i=0;i<MaxWritePatches;i++)
         fPatch1 << "N-leach" << i << "\t";
      fPatch1 << "Date\t";
      for (int i=0;i<MaxWritePatches;i++)
         fPatch1 << "Denit-" << i << "\t";
      fPatch1 << "Date\t";
      for (int i=0;i<MaxWritePatches;i++)
         fPatch1 << "N2O-" << i << "\t";
      fPatch1 << endl;


      fPatch2 << "Date\t";
      for (int i=0;i<MaxWritePatches;i++)
         fPatch2 << "DM" << i << "\t";
      fPatch2 << "Date\t";
      for (int i=0;i<MaxWritePatches;i++)
         fPatch2 << "LAI" << i << "\t";
      fPatch2 << "Date\t";
      for (int i=0;i<MaxWritePatches;i++)
         fPatch2 << "OFFT" << i << "\t";
      fPatch2 << endl;
   }

   if (theOutputData->DetailsData.getPlantDetails()==true)
   {
      char filename[40];
      char leader[100];

      sprintf(leader,theSystemData->getOutputDirectory().c_str());
      sprintf(filename,"field%d.xls",Index);

      strcat(leader,filename);

      f.open(leader,ios::out);
      f.precision(4);
		OutputPlantDetails(true);
   }
   InitVariables();
}

/****************************************************************************\
Copy constructor
Dung, urine and their areas presently ignored !!!
\****************************************************************************/
field::field(const field& fl) : base(fl)
{
   for (int i=0;i<MaxFixedCrops;i++)
   {
      FixedCrop[i]=fl.FixedCrop[i]; // OK because reference does not change
      FixedSize[i]=fl.FixedSize[i];
   }
   for (int i=0;i<BackScope;i++)
	   History[i]=fl.History[i];
   thePatches=fl.thePatches->clone();
   if (fl.crrPlan)
  	   crrPlan=fl.crrPlan->clone();
   else
      crrPlan=NULL;
   if (fl.Lfutr)
      Lfutr=fl.Lfutr->clone();
   else
      Lfutr=NULL;
   Minimum=fl.Minimum;
   area=fl.area;
   MinArea=fl.MinArea;
   distance=fl.distance;
   fenced=fl.fenced;
   JBNO=fl.JBNO;
   daysSinceIrrigation=fl.daysSinceIrrigation;
   daysSinceIrrigationRequest=fl.daysSinceIrrigationRequest;
   daysBetweenIrrigation=fl.daysBetweenIrrigation;
   fingerFlow=fl.fingerFlow;
 	irrigationDelay=fl.irrigationDelay;
  	NoFixCrops=fl.NoFixCrops;
   active=fl.active;
   pile_days=fl.pile_days;
   pile_counter = 0;
   grazingPossible=fl.grazingPossible;
   if (theOutputData->DetailsData.getPlantDetails()==true)
   {
      char filename[40];
      sprintf(filename,"\\fasset\\field%d.xls",Index);

      f.open(filename,ios::app);
      f.precision(4);
   }
#ifdef TUPLE
   writeTuples=fl.writeTuples;
#endif
#ifdef ARLAS
   FieldID=fl.FieldID;
#endif
}

/****************************************************************************\
Destructor
\****************************************************************************/
field::~field()
{
   if (theOutputData->DetailsData.getPlantDetails()==true)
      f.close();
   if (monitorPatches)
   {
      fPatch1.close();
      fPatch2.close();
   }
   if (Lfutr)
   	delete Lfutr;
	if (crrPlan)
      delete crrPlan;
   if (thePatches)
      delete thePatches;
   if (urine)
   	delete urine;
   if (dung)
      delete dung;
}

/****************************************************************************\
\****************************************************************************/
void field::InitVariables()
{
   area					= 0.0;
   grazingPossible   = false;
   area_urine        = 0.0;
   area_overlap      = 0.0;
   area_dung         = 0.0;
   new_area_urine    = 0.0;
   new_area_dung     = 0.0;
   urine             = NULL;
   dung              = NULL;
   pile_counter = 0;
   pile_days         = 8; // (15) Days between each possible, new patch formation. Should be read from file - but from which???!!!;
   maxPatches			= 1;//200; // 200 !!!!!!!!!!!!!!!!!!!
   if (maxPatches>1)
	   monitorPatches             =true; // Read from some file later!!!
   else
   	monitorPatches					=false;
   grazingShutDown   = true;
/*
   expectedCumulativeGrowth[0]=0.0; //@@@!!!???
   expectedCumulativeGrowth[1]=0.0; //@@@!!!???
*/
}

/****************************************************************************\
\****************************************************************************/
void field::ReadPatchParameters(patch *p)
{
char soilFileName[50];
   char number[3];
   double patchArea;
   SetCritical();
   GetParameter("JBNO",&JBNO);
   UnsetCritical();
   if (!GetParameter("soilFile",soilFileName))
   {
      itoa(JBNO,number,10);
      strcpy(soilFileName,"JB");
      strcat(soilFileName,number);
      strcat(soilFileName,"-M.dat");
   }
   GetParameter("area",&patchArea);
   area+=patchArea;
   if (area<=0.0)
         theMessage->FatalError("field::ReadParameters - field area cannot be zero");
   p->Initialise(soilFileName,0,patchArea);
}
/****************************************************************************\
\****************************************************************************/
void field::ReadParameters(fstream * file)
{
   if (thePatches->NumOfNodes()>1)
      theMessage->FatalError("field::ReadParameters - only one patch allowed here");
   Setfile(file);
   SetCritical();
   if (FindSection(Name,Index))
   {
      UnsetCritical();
      int first,num;
      string namestr;
      char Indexstr[10];
      sprintf(Indexstr,"(%d)",Index);
      namestr=(string) "field" +Indexstr+"."+"patch";
      GetSectionNumbers((char*) namestr.c_str(),&first,&num);  //count the number of patches
      if (num>0) //may be more than one patch per field
      {
	      Setcur_pos(0);
         for(int inx=first;inx<(first+num);inx++)
         {
          FindSection((char*) namestr.c_str(),inx);  // find the correct place in the input file
          patch * p =new patch((char *)namestr.c_str(),inx,this);
          ReadPatchParameters(p);
          thePatches->InsertLast(p);
         }
		}
      else   //one patch per field
      {
	      patch * p =new patch("patch",Index,this);
         ReadPatchParameters(p);
   	   thePatches->InsertLast(p);
		}


   	char number[3];
      char s[21];
      if (theControlParameters->GetUniSizeFields())
         area=1.0; // For testing purposes!
		GetParameter("distance",&distance);
		GetParameter("fenced",&fenced);

      int k;
		for (k=0;k<BackScope;k++)
		{
			itoa(k,number,10);
			strcpy(s,"CROP");
			strcat(s,number);
			GetParameter(s,History[k].crop);
		}
		for (k=0;k<BackScope;k++)
		{
			itoa(k,number,10);
			strcpy(s,"HUG-N");
			strcat(s,number);
			GetParameter(s,&History[k].HUG_N);
      }
   }
   else
      theMessage->FatalError("field::ReadParameters - section not found");
   Setfile(NULL);
   //if we wish to simulate patches with different soil types, this will have to be modified
//   cloneList<patch>::PS P;
//   thePatches->PeekHead(P);
//   P->element->Initialise(soilFileName,Index,area);
}

/****************************************************************************\
Updates cropping history when a new crop is initiated
Parameters:
   fP        pointer to new field
   crop_id   id of new crop
\****************************************************************************/
void field::PushHistory(field * fP,str * crop_id)
{
	fP->History[0]=History[1];
	fP->History[1]=History[2];
	fP->History[2]=History[3];
	strcpy(fP->History[3].crop,*crop_id);
	fP->History[3].HUG_N=0.0;
}

/****************************************************************************\
\****************************************************************************/
field* field::MakeNewField()
{
	return new field(Name,Index,Owner);
}

/****************************************************************************\
\****************************************************************************/
void field::AdjustPatchAreas(double adjust,bool balance)
{
   if (adjust <0.0)
      theMessage->FatalError("field::AjustPatchAreas - call parameter must be above zero");
   if (adjust>1.0 && !balance)
      theMessage->FatalError("field::AjustPatchAreas - call parameter must be not exceed one in this mode");

   double Nbefore=0.0;
   double NcropBefore=0.0;
   double NsoilBefore=0.0;
	if (thePatches)
   {
   	for (int i=0;i<thePatches->NumOfNodes();i++)
      {
         NcropBefore+=thePatches->ElementAtNumber(i)->NitrogenInCrops();
         NsoilBefore+=thePatches->ElementAtNumber(i)->NitrogenInSoil();
         Nbefore+=thePatches->ElementAtNumber(i)->NitrogenInSystem();
   		thePatches->ElementAtNumber(i)->AdjustArea(adjust);
      }
   }
   else
      theMessage->FatalError("field::AjustPatchAreas - attempt to divide uninitialised field");

   double Nafter=0.0;
   double NcropAfter=0.0;
   double NsoilAfter=0.0;
   for (int i=0;i<thePatches->NumOfNodes();i++)
   {
      NcropAfter+=thePatches->ElementAtNumber(i)->NitrogenInCrops();
      NsoilAfter+=thePatches->ElementAtNumber(i)->NitrogenInSoil();
      Nafter+=thePatches->ElementAtNumber(i)->NitrogenInSystem();
   }

   if (balance)
   {
      // Adjust C and water in indicators later
      theOutput->AddIndicator(environmentalIndicator,"37.00 N difference from area adjustment","kg N",Nbefore-Nafter);
      theOutput->AddIndicator(environmentalIndicator,"37.01 N crop difference from area adjustment","kg N",NcropBefore-NcropAfter);
      theOutput->AddIndicator(environmentalIndicator,"37.02 N soil difference from area adjustment","kg N",NsoilBefore-NsoilAfter);
   }
}

/****************************************************************************\
\****************************************************************************/
void field::AdjustFieldArea(double newArea)
{
   // cout << "Adjusting area on field no " << Index << " from " << area << " ha to " << newArea << " ha " << theTime << endl;
   AdjustPatchAreas(newArea/area,true);
   area=newArea;
}

/****************************************************************************\
Divides the present instance of the field.
Copy constuctor can not be made, as "linkList" can not have a copy
constructor.
Parameters:
   fP   pointer to new field
   Pl   new plan of field operations
\****************************************************************************/
void field::DivideThisInstance(field * &fP,plan * Pl)
{
   fP=MakeNewField();                             // to cope with derived fields
   if (fP->thePatches)
      delete fP->thePatches;
	fP->thePatches=thePatches->clone();
	fP->crrPlan = Pl;
   fP->Index=Pl->GetFutureNumber();
	for(int i=0;i<BackScope;i++)
		fP->History[i] = History[i];
	str cid;
	Pl->GiveCrop(&cid);
	PushHistory(fP,&cid);
	fP->area=Pl->GiveArea();
	fP->distance=distance;
	fP->MinArea=MinArea;
   fP->daysSinceIrrigation=daysSinceIrrigation;
	fP->daysSinceIrrigationRequest=daysSinceIrrigationRequest;
   fP->daysBetweenIrrigation=daysBetweenIrrigation;
   fP->irrigationDelay=irrigationDelay;
   fP->fingerFlow=fingerFlow;
   fP->fenced=fenced;
	fP->JBNO=JBNO;
	ofstream * fstr=theMessage->GiveHandle();
	*fstr << "Divided. Area 'mother': " << area << "ha. Area 'child': " << Pl->GiveArea() << " ha" << "\n";
	theMessage->CloseHandle();
   //Adjust areas
   fP->AdjustPatchAreas(fP->area/area,false);
   AdjustPatchAreas((area-Pl->GiveArea())/area,false);
	area=area-Pl->GiveArea();
   for (int i=0; i<NoFixCrops;i++)
   {
   	fP->FixedCrop[i] = FixedCrop[i];
      fP->FixedSize[i] = FixedSize[i];
   }
}

/****************************************************************************\
Renew or divide field
Parameters:
   fP   pointer to new instance of field
   Pl   new plan of field operations
\****************************************************************************/
void field::MakeInstance(field * &fP,plan * Pl)
{
	ofstream * fstr;
	double Ar=Pl->GiveArea();
	str cid;
	Pl->GiveCrop(&cid);
   if (area==0.0)
		theMessage->WarningWithDisplay("field::MakeInstance - attempt to divide a field with an area of zero");
	if (Ar==0.0)
		theMessage->WarningWithDisplay("field::MakeInstance - attempt to divide a field with request of an area of zero");
	if (Ar<MinArea)
		theMessage->Warning("field::MakeInstance - attempt to make field with area less than minimum area");
	if (Ar>area)
   {
		if (Ar>(area+Minimum)) // Handles rounding-erors
		{
         if (theControlParameters->GetFlexSizeFields())
            AdjustFieldArea(Ar);
         else
   			theMessage->FatalError("field::MakeInstance - attempt to divide a field with request of an area bigger than that of the field");
		}
		Ar=area;
	}
	if (area>0.0)
   {
		if (area > Ar && theControlParameters->GetFlexSizeFields()) // Test if field size should be adjusted
         AdjustFieldArea(Ar);
		if (area > (Ar+Minimum)) // Handles rounding-erors
       	DivideThisInstance(fP,Pl);
		else
      { // The field is not to be divided, but renewed
			if ((Lfutr->NumOfNodes())>1)
				theMessage->WarningWithDisplay("field::MakeInstance - error in list of future plans");
			if (Pl->GetFutureNumber()!=Index)
         {
            cout << Pl->GetFutureNumber() << endl;
				theMessage->FatalError("field::MakeInstance - inconsistence in indices");
         }
			if (crrPlan)
				delete crrPlan;
			crrPlan=Pl;
         if (crrPlan->GetAdjustArea()>0)
            AdjustFieldArea(crrPlan->GetAdjustArea());

			PushHistory(this,&cid);
			fstr=theMessage->GiveHandle();
			*fstr << "Renewed. Size: " << area << " ha" << endl;
			theMessage->CloseHandle();
		}
	}
#ifdef TUPLE
   writeTuples=theControlParameters->GetWriteTuples();
#endif
}

/****************************************************************************\
Investigates whether the field should be divided or renewed.
Parameters
   year, month, day
   fP                 Pointer to new instance. NULL if field is undivided.
\****************************************************************************/
void field::SplitMe(field * &fP)
{
   if (active)
   {
	   ofstream * fstr;
      fP=NULL;
      if ((crrPlan==NULL) || (crrPlan->Finished()))
      {
         cloneList<plan>::PS P;
         Lfutr->PeekHead(P);
         if (P!=NULL)
         {
            int year = theTime.GetYear();
            int month = theTime.GetMonth();
            int day = theTime.GetDay();
#ifdef MEASCOPE
            if ((P->element->FirstOperationToday(year,month,day) && (OperationCanBePerformed(P->element,thePatches->ElementAtNumber(0))))
            		|| (P->element->NoOpsThisYear(year,month,day)))
#else
            if (P->element->FirstOperationToday(year,month,day) && (OperationCanBePerformed(P->element,thePatches->ElementAtNumber(0))))
#endif
            {

               fstr=theMessage->GiveHandle();
               *fstr << theTime << " >>> Field no " << setw(2) << Index << "  ";
               theMessage->CloseHandle();
               MakeInstance(fP,P->element);
               Lfutr->Release(P);

            }
         }
      }
   }
}

/****************************************************************************\
\****************************************************************************/
void field::ImplementFingerFlow()
{
	if ((JBNO==1) && fingerFlow)                                     // irrigation on JB1 implies finger flow on 2/3 of the area and 1/3 do not receive any water!
   {
      if (thePatches->NumOfNodes()>2)
      	theMessage->WarningWithDisplay("field::ReadParameters - to many patches on field");
//      MeltAllPatches();

   	cloneList<patch>::PS P;
   	thePatches->PeekHead(P);
      patch* P2= P->element->clone();
      P2->SetArea(area*1.0/3.0);
#ifndef NITROSCAPE
      P2->SetFingerFlow(2);
      P->element->SetFingerFlow(1);
#endif
      P->element->SetArea(area*2.0/3.0);
      thePatches->InsertLast(P2);
   }
   daysSinceIrrigationRequest = 0;
}

/****************************************************************************\
Handles a field operation (e.g. sowing)
Parameters:
   FOP                object with operation characteristics
\****************************************************************************/
bool field::HandleOperation(fieldOperationFields * FOP)
{

	if(FOP->GetCropName()[1]==1)
		cout<<FOP->GetCropName()<<"test 4 "<<FOP->GetCropName()[1]<<endl;
	bool delayOp = false;
   //NJH commented out next section because it should be transferred to management class 
/*  	if ((FOP->GetOperationId()==ForageHarvesting)||(FOP->GetOperationId()==Mowing))
   {  //delay cutting if no material would be harvested
   	if (GetMaxCropHeight()<=0.03)  //cutting height should come from technics not hardcoded  NJH June 2002
      {
      	FOP->DelayOneDay();
         delayOp=true;
      }
   } */
   if (!delayOp)
   {
	   ofstream * fstr=theMessage->GiveHandle();
      *fstr
         << theTime
         << " Field no "
         << setw(2)
         << Index
         << " "
         << setfill(' ')
         << setiosflags(ios::left)
         << setw(20);
	   fstr->flush();
  	*fstr
         << *FOP;
      theMessage->CloseHandle();

      cloneList<patch>::PS P;
      thePatches->PeekHead(P);
      while (P!=NULL)
      {
         P->element->HandleOp(FOP);
         thePatches->OneStep(P);
      }
	}
   return !delayOp;
}

/****************************************************************************\
Returns whether the soil is trafficable. Presently very simplistic, if
standing water is below a threshold, it returns true.
\****************************************************************************/
bool field::SoilIsSuitableForFertilizing()
{
   double standingWater=0.0;
   double areaSum=0.0;
   cloneList<patch>::PS P;
 	thePatches->PeekHead(P);
   while (P!=NULL)
   {
      double area=P->element->GetArea();
      areaSum+=area;
      standingWater+=P->element->StandingWater()*area;
      thePatches->OneStep(P);
   }
   standingWater=standingWater/areaSum;
   return (standingWater<1.0 || theTime.GetMonth()==5); // To ensure that fertilizing is possible later
}

/****************************************************************************\
Perform irrigation of field
Parameters
   year, month, day
\****************************************************************************/
void field::Irrigate(int year,int month,int day)
{
   if (daysSinceIrrigation>=daysBetweenIrrigation)                        // Must be redefined to capacity of technics!
   {
      if (daysSinceIrrigationRequest>=irrigationDelay)
      {
	   	char * crop_id;
			PresentCrop_(crop_id);
			water * aWater=new water;
			aWater->Setname("WATER");
			theProducts->GiveProductInformation(aWater);
			aWater->Setamount(30*10); // 30mm, 1ha
#ifdef ARLAS
		   FieldSuitcase->AddIndicator(environmentalIndicator,"irrigation",30.0);
#endif
			irrigateFields * FOP=new irrigateFields;
         FOP->DefineIrrigateFieldsOp(Irrigating,crop_id,History[3].crop,year,month,day,FieldArea_(),0,DistanceToField_(),aWater,false);
		   HandleOperation(FOP);
			theTechnics->ExecuteFields(FOP);
         delete FOP;
			delete aWater;
	      daysSinceIrrigation=0;
         daysSinceIrrigationRequest=0;
      }
      else
      	daysSinceIrrigationRequest++;
	}
   else
   	daysSinceIrrigationRequest=0;
}

/****************************************************************************\
Determines if the operation can be performed
Note addition of forage harvest (NJH May 2004)
\****************************************************************************/
bool field::OperationCanBePerformed(plan* aPlan,patch* aPatch)
{
   if (aPlan!=NULL && !aPlan->Finished()) // Last term added 28.02.2007 by BMP for speed and transparency
	{
#ifndef MEASCOPE
    	bool ReadyForHarvest;
      if (theControlParameters->GetForcedHarvest())
      	ReadyForHarvest = aPlan->NextIsHarvestOp() && (aPatch->ReadyForHarvestOrLater() || aPlan->NextIsForcedHarvest());
      else
      	ReadyForHarvest = aPlan->NextIsForageHarvestOp() || (aPlan->NextIsHarvestOp() && aPatch->ReadyForHarvestOrLater());
      bool ReadyForFertilizing = aPlan->NextIsFertilizing() && (SoilIsSuitableForFertilizing() || theControlParameters->GetForcedOperations());
      bool ReadyForOtherOp = theControlParameters->GetForcedOperations() || (!aPlan->NextIsHarvestOp() && !aPlan->NextIsFertilizing());

/* from MEL_FASSET
      bool ReadyForOtherOp = false;
      if (!aPlan->NextIsHarvestOp())
         ReadyForOtherOp = theControlParameters->GetForcedOperations(); // Everything but harvest, incl. fertilization !!!
*/
   	return (ReadyForHarvest || ReadyForFertilizing || ReadyForOtherOp);
#else
      bool OpTrigger = false;
      OpTrigger = aPlan->CheckIfToDo(0.0);   //should have real moisture from patch!!!
      if ((!OpTrigger)&& aPlan->CheckIfMustDo())  //if have reached latest date at which an operation may be performed
         	OpTrigger = true;
      return OpTrigger;
#endif
	}
   else
   	return false;
}

/****************************************************************************\
Daily update of field state
\****************************************************************************/
void field::UpdateField()
{
   int year = theTime.GetYear();
   int month = theTime.GetMonth();
   int day = theTime.GetDay();
 //  cout << " " << GetIndex();
   if (active)
   {
      timeConsumption->startProfiling(UpdateFieldSimulation);
      timeConsumption->startProfiling(HandleOperationsSimulation);

      daysSinceIrrigation++;
      cloneList<patch>::PS P;
      thePatches->PeekHead(P);
      if (P==NULL)
         theMessage->FatalError("field::UpdateField - no patches in field");
      if (P->element==NULL)
         theMessage->FatalError("field::UpdateField - pointer error");
      fieldOperationFields * FOP=NULL;
      if (crrPlan!=NULL && !crrPlan->Finished())
         FOP=crrPlan->ReturnOperation(year,month,day,false);
      long int delay=0;
      if (FOP!=NULL)
      {
#ifndef MEASCOPE
         delay=theTime.GetDateValue() - FOP->GetOpTime()->GetDateValue();
         if (delay>65)
         {
            cout << endl << GetLongName();
            theMessage->FatalError("field::UpdateField - field operation not performed after more than 65 days delay");
         }
#else
         delay=theTime.GetDateValue() - FOP->GetopStart()->GetDateValue();
         if (delay>120)
         {
            cout << endl << GetLongName();
            theMessage->FatalError("field::UpdateField - field operation not performed after more than 120 days delay");
         }
#endif
      }
      if (OperationCanBePerformed(crrPlan,P->element))
      {
         FOP=crrPlan->ReturnOperation(year,month,day,true);
         if (FOP!=NULL)
         {
            if (crrPlan->IsThisTheFirst(FOP) && fingerFlow)
               ImplementFingerFlow();
            if (!(FOP->IsIrrigationOp() && crrPlan->AutomaticIrrigation()))
            {
               if (HandleOperation(FOP))
               {
#ifdef MEASCOPE
  						if (FOP->GetopEnd())
                  {
                     if (theTime>=*FOP->GetopEnd())
                        theMessage->Warning("Operation forced at end date: field no and op name = ",
                           Index,operationNameList[FOP->GetOperationId()].c_str());
                  }
                  crrPlan->CheckIfFinished();
#else
                  if (delay>20)
                  {
                     cout << endl << delay << " days delay of operation type " << FOP->GetOperationId() << " due for " << *FOP->GetOpTime() << " on field no " << Index << endl;
                     if (delay>60)
                        theMessage->WarningWithDisplay("field::UpdateField - field operation performed after more than 60 days delay");
                     else
                        theMessage->Warning("field::UpdateField - field operation performed after more than 20 days delay");
                  }
#endif
                  theTechnics->ExecuteFields(FOP);
                  ofstream * file=theMessage->GiveHandle();  // write end of line marker
                  *file << endl;
		            theMessage->CloseHandle();
               }
            }
         }
      }
      while (P!=NULL)
      {
	      if (P->element->IrrigationDemand() && crrPlan->AutomaticIrrigation())
   	      Irrigate(year,month,day);
      	thePatches->OneStep(P);
      }

#ifdef TUPLE
      if (writeTuples)
      {
         cloneList<patch>::PS P;
         thePatches->PeekHead(P);
         if (P!=NULL)
         {
            fixation_sum+=P->element->GetNFixationThisDay();
            deposition_sum+=0.0411; // HACK, UPDATE !!!!!!!!!!!!!!!!!!!!!!!!!!!!
         }
         if (day==31 && month==12)
         {
            AddTuple(theTime.GetString2(),deposition_sum*area,0,"DEPOSITION","DEPOSITION",0);
            AddTuple(theTime.GetString2(),fixation_sum*area,0,"FIXATION","FIXATION",0);
            deposition_sum=0.0;
            fixation_sum=0.0;
         }
      }
#endif
      timeConsumption->endProfiling(HandleOperationsSimulation);
      UpdatePatches();
      timeConsumption->endProfiling(PatchOutput);

      timeConsumption->startProfiling(UpdateFieldIndicators);
		GiveIndicators();
      timeConsumption->endProfiling(UpdateFieldIndicators);
      timeConsumption->endProfiling(UpdateFieldSimulation);
   }
}

/****************************************************************************\
\****************************************************************************/
void field::UpdatePatches()
{
   cloneList<patch>::PS P;
  	thePatches->PeekHead(P);
   int k=0;
   while (P!=NULL)
   {
      P->element->UpdatePatches();
      thePatches->OneStep(P);
      k++;
   }
   if (theOutputData->DetailsData.getPlantDetails()==true)
      OutputPlantDetails(false);
#ifndef NITROSCAPE
   if (dung || urine)
   {
      if (pile_counter>0)
//   	   ExcretionDynamics();
      theMessage->FatalError("field::UpdatePatches function not defined here");
      delete dung;
      delete urine;
      dung = NULL;                                          // added by JB to ensure urine and dung are removed every day
      urine = NULL;
   }
//   WritePatches();  //not defined!!
#endif
}

/****************************************************************************\
Returns the crop for a previous season
Parameters
   nr        season
   crop_id   reference for the crop id
\****************************************************************************/
void field::CropHistory(int nr,char * &crop_id)
{
   crop_id=History[nr].crop;
   if (!active)
      theMessage->FatalError("field::CropHistory called for inactive field");
}

/****************************************************************************\
Returns the present crop
\****************************************************************************/
void field::PresentCrop_(char * &crop_id)
{
	crop_id=History[3].crop;
}

/****************************************************************************\
Returns the distance from farm to field (meters)
\****************************************************************************/
double field::DistanceToField_()
{
	return distance;
}

/****************************************************************************\
Returns 1 if the field is fenced, otherwise 0
\****************************************************************************/
int field::IsFieldFenced_()
{
	return fenced;
}

/****************************************************************************\
Returns the area of the field (ha)
\****************************************************************************/
double field::FieldArea_()
{
	return area;
}

/****************************************************************************\
Returns the JB classification number of the soil
\****************************************************************************/
int field::JBno()
{
	return JBNO;
}

/****************************************************************************\
Returns the expected extra release of nitrogen during the comming growth
season (g/m2)
\****************************************************************************/
double field::ExtraN()
{
	return History[0].HUG_N*0.01+History[1].HUG_N*0.02
			 +History[2].HUG_N*0.03+History[3].HUG_N*0.04;
}

/****************************************************************************\
Inserts the plan in the list of plans for the following growth season
Parmeters:
   OP            plan (list of field operations)
   str crop_id   crop identification
\****************************************************************************/
void field::PlanField_(cloneList<fieldOperationFields> * OP,str crop_id,int futureNum)
{
	plan * pl=new plan;
	cloneList<fieldOperationFields>::PS P;
	OP->PeekHead(P);
	pl->HandPlanOver(OP,crop_id,futureNum);
   if (pl->GiveNumOps()==0)
   	delete pl;
   else
   {
#ifdef MEASCOPE
   	pl->CheckOps();
#endif
		Lfutr->InsertFirst(pl);
   }
}

/****************************************************************************\
\****************************************************************************/
int field::GetOperationIndex(string date)
{
   if (last_date==date)
      last_idx++;
   else
      last_idx=1;
   last_date=date;
   return last_idx;
}

#ifdef TUPLE
/****************************************************************************\
\****************************************************************************/
void field::CloseTupleFile()
{
   if (tuple_file_open)
   {
      tuple.close();
      tuple_file_open=false;
   }
}

/****************************************************************************\
\****************************************************************************/
void field::OpenTupleFile(string s)
{
   string s1="c:\\fasset_v2\\F_"+s+"_sim.txt";
   if (theControlParameters->GetSimTupleHasBeenOpened())
      tuple.open(s1.c_str(),ios::app);
   else
   {
      tuple.open(s1.c_str(),ios::out);
      tuple << "Key\tFarmId\tFieldno\tDate\tIndex\tMainCrop\tArea\tOperation\tProduct\tN\tAmount\tSFU" << endl;
      theControlParameters->SetSimTupleHasBeenOpened(true);
   }
   tuple_file_open=true;
}

/****************************************************************************\
Parametre: dato,N,m�ngde,operation,type
N�gle (unik)	Bedrifts-id	Marknummer	Dato	Index (ved flere op. samme dato)	Hovedafgr�de	Areal (ha)	Operation	Produktnavn	H�stet N (kg)	M�ngde (kg - ikke sl�t)	H�stet FE
Eksempel: 95023_8_15-08-2001_1_RY	95023	8	15-08-2001	1	RY	6.4	Combineharvesting	RYEGRAIN	478.9	29440	-
\****************************************************************************/
void field::AddTuple(string date,double n,double amount,string op,string type,double SFU)
{
   if (area>=0.001 && writeTuples)
   {
      string farmId=theControlParameters->GetFarmID();
      if (farmId=="")
         theMessage->FatalError("fieldOrder::AddTuple - attempt to add tuples but no farm ID specified");
      if (!tuple_file_open)
         OpenTupleFile(farmId);
      int idx = GetOperationIndex(date);
      tuple << farmId << "_" << Index << "_" <<  date << "_" << idx << "_" << History[3].crop << "\t";
      tuple << farmId << "\t" << Index << "\t" << date << "\t" << idx << "\t";
      tuple << History[3].crop << "\t" << area << "\t" << op << "\t" << type << "\t" << n;
      tuple << "\t" << amount << "\t" << SFU << endl;
      CloseTupleFile(); // Only in field objects
   }
}
#endif
/****************************************************************************\
Adds indicator values to indicator object
\****************************************************************************/
void field::GiveIndicator()
{
}

/****************************************************************************\
Resets the legal crop list
\****************************************************************************/
void field::ResetFixedCrops()
{
   NoFixCrops=0;
}

/****************************************************************************\
Adds a legal crop to the list
Parameters:
   crop_id   crop identificator
   size      area (ha)
\****************************************************************************/
void field::AddFixedCrop(const char * crop_id,double size)
{
   if (NoFixCrops>=(MaxFixedCrops-1))
      theMessage->WarningWithDisplay("field::AddFixedCrop - to many fixed crops");
   else
   {
       FixedCrop[NoFixCrops]=(char*)crop_id;
       FixedSize[NoFixCrops]=size;
       NoFixCrops++;
   }
}

/****************************************************************************\
Returns the number of legal crops
\****************************************************************************/
int  field::NumOfFixedCrops()
{
   return NoFixCrops;
}

/****************************************************************************\
Returns crop identificator
Parameters:
   No   number of crop in list
\****************************************************************************/
const char * field::GiveFixedCrop(int No)
{
   return FixedCrop[No];
}

/****************************************************************************\
Returns area for a given, legal crop
Parameters:
   No   number of crop in list
\****************************************************************************/
double field::GiveFixedSize(int No)
{
   return FixedSize[No];
}

/****************************************************************************\
Test-function
\****************************************************************************/
void field::ShowCurrentPlan(ostream * fs)
{
	str s;
	crrPlan->GiveCrop(&s);
	*fs << "Crop: " << s << "\n";
	*fs << "Area: " << crrPlan->GiveArea() << "\n";
	crrPlan->ShowContent(fs);
}

/****************************************************************************\
Test-function
\****************************************************************************/
void field::DisplayCurrentPlan()
{
	str s;
	crrPlan->GiveCrop(&s);
	cout << "Crop: " << s << "\n";
	cout << "Area: " << crrPlan->GiveArea() << "\n";
	crrPlan->DisplayContent();
}

/****************************************************************************\
Test-function
\****************************************************************************/
void field::ShowFuturePlans(ostream * fs)
{
	str s;
	cloneList<plan>::PS P;
	Lfutr->PeekHead(P);
	int i=0;
	while (P!=NULL)
   {
		*fs << "Future plan no " << i << "\n";
		P->element->GiveCrop(&s);
		*fs << "Crop: " << s << "\n";
		*fs << "Area: " << P->element->GiveArea() << "\n";
		P->element->ShowContent(fs);
		Lfutr->OneStep(P);
		i++;
	}
}

/****************************************************************************\
Test-function
\****************************************************************************/
void field::ShowHistory()
{
	int i;
	for (i=0;i<BackScope;i++)
		cout << History[i].crop << "\n";
}

/****************************************************************************\
*
\****************************************************************************/
void field::OutputPlantDetails(bool header)
{
	if (header)
	{
      f << "date";
      f << "\tcrop number\tpClover";
      for (int i=0;i<MaxPlants;i++)
      {
      	f << "\tCropName\tphase\tTempSum\tCO2conc\tGLAI\tYLAI\tDMRoot\tDMRootPL\tDMRootTubers\tDMVegTop\tDMStorage\tDMTop";
     	   f << "\tTranspirationRatio\tIrrigationDemand\tHeight\tRootDepth\tTotalRootLength\tInterceptedRadiation";
     	   f << "\tNitrogen\tN15\tNmin()\tNmax()\tNStorage\tNTop\tNVegTop\tNRoot\tNRootPL\tNRootTubers\tfNitrogen\tfNitrogenCurve";
     	   f << "\tRootDeposit\tRootTrans\tNRootDeposit\tTopDeposit\tNTopDeposit";
         f << "\tTopProd\tNUptake\tNfix\tNfixAcc\tdeltaDM\tN15Root\tAccRootResp";
         for(unsigned int i=0;i<theOutputData->RootData.size();i++)
         {
            f << "\tRootLength-"<< theOutputData->RootData[i].getStartDepth() << "-"<< theOutputData->RootData[i].getEndDepth();
         	f << "\tRootMass-"<< theOutputData->RootData[i].getStartDepth() << "-"<< theOutputData->RootData[i].getEndDepth();
         	f << "\tRootN-"<< theOutputData->RootData[i].getStartDepth() << "-"<< theOutputData->RootData[i].getEndDepth();
      	}
      }


	}
   else
      f << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay();

   cloneList<patch>::PS P;
   thePatches->PeekHead(P);
   while (P!=NULL)
   {
      P->element->outputCropDetails(&f);
      thePatches->OneStep(P);
   }
   f << endl;
}

/****************************************************************************\
Initialise budget variables
\****************************************************************************/
void field::StartBudget()
{
	if (thePatches)
   	for (int i=0;i<thePatches->NumOfNodes();i++)
   		thePatches->ElementAtNumber(i)->StartBudget();
}

/****************************************************************************\
Test if budget is valid
\****************************************************************************/
void field::EndBudget()
{
	bool ret_val=true;
//   double N=0;,DM=0;
   if (thePatches)
   	for (int i=0;i<thePatches->NumOfNodes();i++)
	   	if (!thePatches->ElementAtNumber(i)->EndBudget()) ret_val=false;
   if (!ret_val)
   {
      theMessage->WarningWithDisplay("Error in budget of field number ", GetIndex());
   }
}

/****************************************************************************\
Returns total N in kg
\****************************************************************************/
double field::NitrogenInSystem()
{
	double totalN=0.0;
	if (thePatches)
   	for (int i=0;i<thePatches->NumOfNodes();i++)
   		totalN+=thePatches->ElementAtNumber(i)->GetArea() * thePatches->ElementAtNumber(i)->NitrogenInSystem();
   return totalN*10.0;
}

/****************************************************************************\
\****************************************************************************/
void field::GiveIndicators()
{
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      p->GiveIndicators();
   }
}

/****************************************************************************\
Get max height of crop in field (m)
\****************************************************************************/
double field::GetMaxCropHeight()
{
	double maxHeight = 0.0;
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      double ht = p->GetMaxPatchHeight();
      if (ht>maxHeight)
         maxHeight=ht;
   }
   return maxHeight;
}

/****************************************************************************\
Assigns new crop according to "crrPlan"
\****************************************************************************/
void field::AssignNewCrop()
{
	str cropId;
	ImplementFingerFlow();
   crrPlan->GiveCrop(&cropId);
}

/****************************************************************************\
Get total herbage mass in kg
\****************************************************************************/
double field::GetHerbageMasskg()
{
	double mass= 0.0;
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      mass += p->GetPatchStandingDMkg()*p->GetArea();
   }
   return mass;
}

/****************************************************************************\
Get herbage mass in kg
\****************************************************************************/
double field::GetHerbageMasskgPerHa()
{
	double mass= 0.0;
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      mass += p->GetPatchStandingDMkg()*p->GetArea();
   }
   mass/=area;
   return mass;
}

/****************************************************************************\
Get herbage mass above cutting height in kg
\****************************************************************************/
double field::GetAvailableHerbageMasskg(double cuttingHeight)
{
	double mass= 0.0;
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      mass += p->GetAvailablePatchStandingDMkg(cuttingHeight);
   }
   return mass;
}

/****************************************************************************\
Get herbage mass above cutting height in kg/ha
\****************************************************************************/
double field::GetAvailableHerbageMasskgPerHa(double cuttingHeight)
{
	double mass= 0.0;
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      mass += p->GetAvailablePatchStandingDMkg(cuttingHeight)*p->GetArea();
   }
   mass/=area;
   return mass;
}

/****************************************************************************\
\****************************************************************************/
void field::DepositManure(manure * animal_urine, manure * animal_dung, double area_ur, double area_du)
{
//needs rewriting for multiple patches
	double prop_lost_volat=0.07;
   patch* base_patch= thePatches->ElementAtNumber(0);
   theOutput->AddIndicator(environmentalIndicator,"31.08 N in faeces from grazing","kg N",animal_dung->GetAmount()*animal_dung->GetAllN().n*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"41.09 C in faeces from grazing","kg C",animal_dung->GetAmount()*animal_dung->GetC_content()*1000.0);
   animal_dung->Setamount(animal_dung->GetAmount()/GetArea());  // in tonnes per ha
// new_area_dung+=dung->GetAmount()*110.0;  //OLD METHOD - area covered per tonne fresh weight from During and Weeda 73, NZJAS

   theOutput->AddIndicator(environmentalIndicator,"31.07 N in urine from grazing","kg N",animal_urine->GetAmount()*animal_urine->GetAllN().n *1000.0);
   theOutput->AddIndicator(environmentalIndicator,"41.08 C in urine from grazing","kg C",animal_urine->GetAmount()*animal_urine->GetC_content() *1000.0);
      // Calculate ammonia volatilisation
   theOutput->AddIndicator(environmentalIndicator,"31.46 NH3-N loss during grazing","kg N",prop_lost_volat * animal_urine->GetAmount()
   						 * animal_urine->GetNH4_content().n * 1000.0);
   animal_urine->SetNH4_content(animal_urine->GetNH4_content()*(1.0-prop_lost_volat));
   animal_urine->Setamount(animal_urine->GetAmount()/GetArea());  // in tonnes per ha
   base_patch->DepositManure(animal_urine, animal_dung);

/*   fstream * filehandle = theMessage->GiveDebugHandle();
   *filehandle << " graz " << (prop_lost_volat * animal_urine->GetAmount() * animal_urine->GetNH4_content().n * 1000.0) << " " ;
   theMessage->CloseDebugHandle();
  */


/*
   double Nsum=(urine->GetAmount() * urine->GetAllN().n + dung->GetAmount() * dung->GetAllN().n)*1000.0;
   cout << endl << "Urine/dung added, cumulated amount: " << Nsum << " total kg N";
   cout << endl;
*/
// new_area_urine+=urine->GetAmount() * 100.0;    //OLD METHOD - area covered per tonne urine (guess)
}

/****************************************************************************\
\****************************************************************************/
patch* field::GetMeanPatch(bool incl_basal, bool incl_urine, bool incl_overlap, bool incl_dung)
{
	patch* mean_patch=NULL;
   int i=thePatches->NumOfNodes();
   for (int count=0;count<i;count++)
   {
      bool addThisPatch = false;
      patch * temp_ptr = thePatches->ElementAtNumber(count);
      if ((count==0) && (incl_basal)) // If this is a basal patch and incl_basal is true
         addThisPatch = true;
      else
         if ((incl_overlap) && (temp_ptr->GetPatchType()==3)) // If this is an overlap patch and incl_overlap is true
            addThisPatch = true;
         else
            if ((incl_dung) && (temp_ptr->GetPatchType()==2)) // If this is a dung patch and incl_dung is true
               addThisPatch = true;
            else
               if ((incl_urine) && (temp_ptr->GetPatchType()==1))
                  addThisPatch = true;
      if (addThisPatch)
      {
      	if (mean_patch==NULL)
	         mean_patch = temp_ptr->clone();
         else
         {
            double f=temp_ptr->GetArea()/(temp_ptr->GetArea()+mean_patch->GetArea());
            double meanArea=mean_patch->GetArea()+temp_ptr->GetArea();
            mean_patch->Add(temp_ptr,f);
            mean_patch->SetArea(meanArea);
         }
      }
   }
   return mean_patch;
}

/****************************************************************************\
@@@ Obsolete ???
\****************************************************************************/
void field::calc_new_spots(double new_area_urine, double new_area_dung,
   double* new_area_urine_once, double * new_area_dung_once, double* new_area_urine_overlap,
   double* new_area_dung_overlap, double* new_area_basal)
{
#ifdef DEVELOPING
   new_area_urine=new_area_urine;
   new_area_dung=new_area_dung;
   new_area_urine_once=new_area_urine_once;
   new_area_dung_once=new_area_dung_once;
   new_area_urine_overlap=new_area_urine_overlap;
   new_area_dung_overlap=new_area_dung_overlap;
   new_area_basal=new_area_basal;
#endif
}

/****************************************************************************\
\****************************************************************************/
double field::AreaBudget()
{
   double cum_area=0.0;
   area_urine=0.0;
   area_dung=0.0;
   area_overlap=0.0;
   patch* base_patch= thePatches->ElementAtNumber(0);
   patch* temp_patch;
   int i=thePatches->NumOfNodes();
   int count=1;
   while (count<i)
   {
      temp_patch=thePatches->ElementAtNumber(count);
/* Presently unused @@@ !!! ----------------------------------------
      if (temp_patch->GetPatchAge()>365)
      {
         double f=temp_patch->GetArea()/(temp_patch->GetArea()+base_patch->GetArea());
         double new_area=base_patch->GetArea()+temp_patch->GetArea();
         base_patch->Add(temp_patch,f);
         base_patch->SetArea(new_area);
         cloneList<patch>::PS P;
         thePatches->SearchByNumber(P,count);
         thePatches->Remove(P);
         i--;
         // cout << "Removed old patch at " << theTime << endl;
      }
      else
------------------------------------------------------------------*/
      {
         if (temp_patch->GetPatchType()==1)
            area_urine+=temp_patch->GetArea();
         if (temp_patch->GetPatchType()==2)
            area_dung+=temp_patch->GetArea();
         if (temp_patch->GetPatchType()==3)
            area_overlap+=temp_patch->GetArea();
         count++;
      }
   }
   cum_area=(base_patch->GetArea() + area_urine + area_overlap + area_dung);
   return cum_area;
}

/****************************************************************************\
Remove when the test is finished  !!!
\****************************************************************************/
/*void field::TestSplit()
{
   NitrogenInSystem();
   patch* base_patch=thePatches->ElementAtNumber(0);
   patch* new_patch=base_patch->clone();
   new_patch->SetOwner(this);
   new_patch->SetPatchBirthday(theTime);
   new_patch->SetName("Test");
   new_patch->GetSoil()->SetMasterSoil(base_patch->GetSoil());
   new_patch->SetArea(0.5);
   new_patch->SetIndex(1);
   new_patch->StartBudget();
   base_patch->SetArea(base_patch->GetArea()-0.5);
   if (base_patch->GetArea()<0.0)
      theMessage->FatalError("field::TestSplit: Basal patch has gone negative");
   thePatches->InsertLast(new_patch);
   NitrogenInSystem();
}
*/
/****************************************************************************\
\****************************************************************************/
#ifndef NITROSCAPE
/*void field::AddPatch(decomposable* matter,patch *new_patch,int type,char * name,double area)
{
   patch* base_patch=thePatches->ElementAtNumber(0);
   new_patch= base_patch->clone();  // Copy contents of base patch
   new_patch->SetOwner(this);
   new_patch->SetPatchBirthday(theTime);
   new_patch->SetName(name);
   new_patch->GetSoil()->SetMasterSoil(base_patch->GetSoil());
   if (area<1E-10)
      theMessage->FatalError("field::AddPatch - attempting to assign extremely low area to patch");
   new_patch->SetArea(area);
   new_patch->SetIndex(thePatches->NumOfNodes());
   new_patch->SetPatchType(type);
   new_patch->StartBudget();
   matter->Setamount(matter->GetAmount()/area); // To adjust for the patch area in question
   new_patch->GetSoil()->AddDecomposable(matter);

   if (((type<=2)&&(pile_counter==1))||((type==3)&&(pile_counter==2))) // Add this patch to list
      thePatches->InsertLast(new_patch);
   else
   { // Not the time to keep the new patch, so melt with the relevant patch
      patch * meltPatch=NULL;
      int i=thePatches->NumOfNodes()-1;
      while ((i>0)&&(meltPatch==NULL))
      {
         patch * p = thePatches->ElementAtNumber(i);
         if (p->GetPatchType()==type)
            meltPatch=p;
         i--;
      }
      if (meltPatch==NULL)
         theMessage->FatalError("field::AddPatch - there should be a relevant patch in the list at this point");
      double f=new_patch->GetArea()/(new_patch->GetArea()+meltPatch->GetArea());
      double meltArea=meltPatch->GetArea()+new_patch->GetArea();
      meltPatch->Add(new_patch,f);
      meltPatch->SetArea(meltArea);
      delete new_patch;
   }
   if (type<=2)
	   base_patch->SetArea(base_patch->GetArea()-new_patch->GetArea());
}
*/
/****************************************************************************\
\****************************************************************************/
/*void field::ExcretionDynamics()
{
   const double hectare = 10000.0; // Constant expressing m2 per ha
   const double prop_lost_volat = 0.07; //should be handled better!!! NJH  does not allow for water lost by evap

   if (thePatches->NumOfNodes()<maxPatches)
   {
      pile_counter++;
      if ((pile_counter>pile_days)&&((new_area_urine + new_area_dung)>0.0))
         pile_counter=1;
   }

   if ((new_area_urine + new_area_dung)>0.0)  //animals have been grazing
   {
      patch* base_patch=thePatches->ElementAtNumber(0);
      manure * overlapManure;

      if (urine)
      {
         theOutput->AddIndicator(environmentalIndicator,"31.07 N in urine from grazing","kg N",urine->GetAmount()*urine->GetAllN().n *1000.0);
         theOutput->AddIndicator(environmentalIndicator,"41.08 C in urine from grazing","kg C",urine->GetAmount()*urine->GetC_content() *1000.0);

         // Calculate ammonia volatilisation
         theOutput->AddIndicator(environmentalIndicator,"31.46 NH3-N loss during grazing","kg N",prop_lost_volat * urine->GetAmount() * urine->GetNH4_content().n * 1000.0);
         urine->SetNH4_content(urine->GetNH4_content()*(1.0-prop_lost_volat));
      }

      if (dung)
      {
         theOutput->AddIndicator(environmentalIndicator,"31.08 N in faeces from grazing","kg N",dung->GetAmount()*dung->GetAllN().n*1000.0);
         theOutput->AddIndicator(environmentalIndicator,"41.09 C in faeces from grazing","kg C",dung->GetAmount()*dung->GetC_content()*1000.0);
      }

      // Calculate area of potential new patches
      double new_area_overlap = 0.0;
      double new_area_urine_on_basal = base_patch->GetArea() * new_area_urine/(hectare * area);
      double new_area_dung_on_basal  = base_patch->GetArea() * new_area_dung/(hectare * area);

      bool use_overlap = true;
      patch *newOverlapPatch=NULL;
      if (use_overlap)
      {  //Create manure for overlap patch and reduce amount going to urine and dung patches
         int n=thePatches->NumOfNodes();
         for (int i=1;i<n;i++)
         {
            patch * p = thePatches->ElementAtNumber(i);
            double tempArea =	p->GetArea() * (new_area_urine + new_area_dung)/(hectare * area);
            if (newOverlapPatch==NULL)
            {
               newOverlapPatch=p->clone();
	            newOverlapPatch->SetArea(tempArea);
            }
            else
            {
	            patch * temp_patch=p->clone();
	            temp_patch->SetArea(tempArea);
               double f=tempArea/(tempArea+newOverlapPatch->GetArea());
               double overlapArea=newOverlapPatch->GetArea()+tempArea;
               newOverlapPatch->Add(temp_patch,f);
               newOverlapPatch->SetArea(overlapArea);
               delete temp_patch;
            }
            p->SetArea(p->GetArea()-tempArea);
            new_area_overlap+=tempArea;
         }
         overlapManure = new manure(*urine);
         overlapManure->Setamount(overlapManure->GetAmount() *
               new_area_overlap/(hectare * area));
         *urine-*overlapManure;
         manure *tempManure = new manure(*dung);
         tempManure->Setamount(tempManure->GetAmount() *
               new_area_overlap/(hectare * area));
         *dung-*tempManure;
         *overlapManure + *tempManure;
         delete tempManure;
      }

      if (new_area_urine_on_basal>0.0)
      {
         if (thePatches->NumOfNodes()>=maxPatches) // No more available patches, add to base patch
         {
            urine->Setamount(urine->GetAmount()/GetArea());  // in tonnes per ha
            base_patch->GetSoil()->AddDecomposable(urine);
         }
         else
         // Make new patch (which may be added to another patch)
            AddPatch((decomposable*) urine,NULL,1,"Single urination",new_area_urine_on_basal);
      }
      if (new_area_dung_on_basal>0.0)
      {
         if (thePatches->NumOfNodes()>=maxPatches) // No more available patches, add to base patch
         {
 		       dung->Setamount(dung->GetAmount()/GetArea());  // in tonnes per ha
             base_patch->GetSoil()->AddDecomposable(dung);
         }
         else
             AddPatch((decomposable*) dung,NULL,2,"Dung",new_area_dung_on_basal);
      }

      if (new_area_overlap>0.0)
      {  // Now deal with multiple dunging and urinations
         if (thePatches->NumOfNodes()>=maxPatches) // No more available patches, add to base patch
         {
            overlapManure->Setamount(urine->GetAmount()/GetArea());  // in tonnes per ha
            base_patch->GetSoil()->AddDecomposable(overlapManure);
         }
         else
            // Make new patch (which may be added to another patch)
            AddPatch((decomposable*) overlapManure,newOverlapPatch,3,"Overlap",new_area_overlap);
      }
      if (base_patch->GetArea()<0.0)
         theMessage->FatalError("Grazed_field: Basal patch has gone negative - stocking rate may be too high");
      new_area_dung=0.0;  //Clear, ready for next day
      new_area_urine=0.0; //Clear, ready for next day
      new_area_overlap=0.0;
      AreaBudget();
	}
}
*/
/****************************************************************************\
\****************************************************************************/
patch* field::getCurrentPatch(bool dung, bool overlap)
{
   patch* temp_patch;
   int i=thePatches->NumOfNodes();
   int count=i-1;   //start at latest and work back

   while (count>0)
   {
      temp_patch=thePatches->ElementAtNumber(count);
      if ((temp_patch->getoverlap()==overlap) && (temp_patch->getdung()==dung)
              && theTime.GetMonth()==temp_patch->Getbirthday()->GetMonth())
      	return temp_patch;
      count--;
   }
   return NULL;
}

/****************************************************************************\
Write to two files because Excel cannot cope with more than 256 columns of data
\****************************************************************************/
/*void field::WritePatches()
{
   if (monitorPatches)
   {
      theOutput->AddStateIndicator(environmentalIndicator,"98.95 Number of patches"," ",thePatches->NumOfNodes());
      int i;
      fPatch1 << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
      fPatch2 << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
      cloneList<patch>::PS P,PH;
      thePatches->PeekHead(P);
      PH=P; // Set head pointer
      i=0;
      while (P!=NULL && i<MaxWritePatches)
      {  // Write area
         fPatch1 << P->element->GetArea() << "\t";
         fPatch2 << P->element->GetArea() << "\t";
         thePatches->OneStep(P);
         i++;
      }
      if (i<MaxWritePatches)
         for (int j=i;j<MaxWritePatches;j++)
         {
            fPatch1 << "0\t";
            fPatch2 << "0\t";
         }

   // File 1
      i=0;
      P=PH;
      fPatch1 << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
      soil * aSoil;
      nitrogen leach;
      while (P!=NULL && i<MaxWritePatches)
      {  // Write leaching
         aSoil=P->element->GetSoil();
         leach =aSoil->GetNitrateLeaching(aSoil->GetMaximumDepth())+aSoil->GetAmmoniumLeaching(aSoil->GetMaximumDepth());
         fPatch1 << leach.n << "\t";
         thePatches->OneStep(P);
         i++;
      }
      aSoil=PH->element->GetSoil();
      leach =aSoil->GetNitrateLeaching(aSoil->GetMaximumDepth())+aSoil->GetAmmoniumLeaching(aSoil->GetMaximumDepth());
      if (i<MaxWritePatches)
         for (int j=i;j<MaxWritePatches;j++)
            fPatch1 << leach.n << "\t";

      i=0;
      P=PH;
      fPatch1 << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
      nitrogen denit;
      while (P!=NULL && i<MaxWritePatches)
      {  // Write N2 loss
         aSoil=P->element->GetSoil();
         denit=aSoil->GetDenitrification();
         fPatch1 << denit.n << "\t";
         thePatches->OneStep(P);
         i++;
      }
      aSoil=PH->element->GetSoil();
      denit=aSoil->GetDenitrification();
      if (i<MaxWritePatches)
         for (int j=i;j<MaxWritePatches;j++)
            fPatch1 << denit.n << "\t";

      i=0;
      P=PH;
      fPatch1 << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
      nitrogen n2o;
      while (P!=NULL && i<MaxWritePatches)
      {  // Write N2O loss
         aSoil=P->element->GetSoil();
         n2o=aSoil->GetN2OFromNitrification()+aSoil->GetN2OFromDenitrification();
         fPatch1 << n2o.n << "\t";
         thePatches->OneStep(P);
         i++;
      }
      aSoil=PH->element->GetSoil();
      n2o=aSoil->GetN2OFromNitrification()+aSoil->GetN2OFromDenitrification();
      if (i<MaxWritePatches)
         for (int j=i;j<MaxWritePatches;j++)
            fPatch1 << n2o.n << "\t";

   // File 2
      i=0;
      P=PH;
      fPatch2 << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
      while (P!=NULL && i<MaxWritePatches)
      {  // Write DM
         fPatch2 << P->element->GetPatchStandingDMkg() << "\t";
         thePatches->OneStep(P);
         i++;
      }
      if (i<MaxWritePatches)
      {
         double DM = PH->element->GetPatchStandingDMkg();
         for (int j=i;j<MaxWritePatches;j++)
            fPatch2 << DM << "\t";
      }

      i=0;
      P=PH;
      fPatch2 << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
      while (P!=NULL && i<MaxWritePatches)
      {  // Write LAIg
         fPatch2 << P->element->GetTotalLAI() << "\t";
         thePatches->OneStep(P);
         i++;
      }
      if (i<MaxWritePatches)
      {
         double LAI = PH->element->GetTotalLAI();
         for (int j=i;j<MaxWritePatches;j++)
            fPatch2 << LAI << "\t";
      }

      i=0;
      P=PH;
      fPatch2 << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
      while (P!=NULL && i<MaxWritePatches)
      {  // Write DM grazed
         fPatch2 << P->element->GetGrazedDM() << "\t";
         thePatches->OneStep(P);
         i++;
      }
      if (i<MaxWritePatches)
      {
         double grazedDM = PH->element->GetGrazedDM();
         for (int j=i;j<MaxWritePatches;j++)
            fPatch2 << grazedDM << "\t";
      }

      i=0;
      P=PH;
      fPatch1 << endl;
      fPatch2 << endl;
   }
}
*/
/****************************************************************************\
\****************************************************************************/
void field::SlicePatch(double sliceArea)
{
   cloneList<patch>::PS P;
 	thePatches->PeekHead(P);
   if (P->element->GetArea() < (sliceArea-0.001))
      theMessage->FatalError("field::SlicePatch - requested area not accesible");
   patch * newPatch=P->element->clone();
   newPatch->SetArea(sliceArea);
   thePatches->InsertLast(newPatch);
   P->element->SetArea(P->element->GetArea()-sliceArea);
}

/****************************************************************************\
Melt All Patches
\****************************************************************************/
void field::MeltAllPatches()
{
   NitrogenInSystem();
   cloneList<patch>::PS T;
	patch* mainPatch;
   mainPatch = thePatches->ElementAtNumber(0);
   while (thePatches->NumOfNodes()>1)
   {
      thePatches->PeekTail(T);
      double fraction = T->element->GetArea()/(mainPatch->GetArea()+T->element->GetArea());
   	mainPatch->Add(T->element,fraction);
      thePatches->Remove(T);
   }
}

/****************************************************************************\
Test function - remove later !!!
\****************************************************************************/
/*void field::WriteTestState()
{
   cloneList<patch>::PS P;
  	thePatches->PeekHead(P);
   int k=0;
   while (P!=NULL)
   {
      nitrogen minN=P->element->GetSoil()->GetMinNitrogen();
      nitrogen totN=P->element->GetSoil()->GetTotalNitrogen();
      nitrogen AOM1=P->element->GetSoil()->GetPoolNitrogen("AOM1");
      nitrogen AOM2=P->element->GetSoil()->GetPoolNitrogen("AOM2");
      nitrogen SMB1=P->element->GetSoil()->GetPoolNitrogen("SMB1");
      nitrogen SMB2=P->element->GetSoil()->GetPoolNitrogen("SMB2");
      nitrogen SMR=P->element->GetSoil()->GetPoolNitrogen("SMR");
      nitrogen NOM=P->element->GetSoil()->GetPoolNitrogen("NOM");
      nitrogen IOM=P->element->GetSoil()->GetPoolNitrogen("IOM");
//      double C=P->element->GetSoil()->GetTotalCarbon();
//      double area=P->element->GetArea();
      thePatches->OneStep(P);
      k++;
   }
}
  */
#endif
/****************************************************************************\
\****************************************************************************/
bool field::GetGrazingPossible()
{
//   char *cropId;
   bool ret_val=true;
/*   patch* mainPatch;
   mainPatch = thePatches->ElementAtNumber(0);
   if (!mainPatch->Getgrazable())
     return false;
*/
	if (!grazingPossible)
   	return false;
   // Check to make sure there is no other operation that would stop grazing
   int year = theTime.GetYear();
   int month = theTime.GetMonth();
   int day = theTime.GetDay();
   fieldOperationFields * FOP=NULL;
   if ((crrPlan==NULL) || (crrPlan->Finished()))
   {
      cloneList<plan>::PS P;
      Lfutr->PeekHead(P);
      if (P)
         FOP=(fieldOperationFields *) P->element->ReturnOperation(year,month,day,false);
   }
   else
      FOP=crrPlan->ReturnOperation(year,month,day,false);

   if (FOP)
   {
      int opID=FOP->GetOperationId();
      switch (opID)
      {
         case 0:
         case 1:
         case 2:
         case 3:
         case 4:
         case 5:
         case 6:
         case 12:
         case 13:
         case 15:
         case 16:
         case 17:
         case 19:
         {
            ret_val=false;
            grazingShutDown=true;
            break;
         }
         default:
         {
            ret_val=true;
            break;
         }
      }
   }
	return ret_val;
}

/****************************************************************************\
\****************************************************************************/
void field::GetRoughageProducts(linkList <product> *aList)
{
	bool found=false;
   int code = GetForageItemCode();
   for(int j=0;j<aList->NumOfNodes();j++)
   {
	   product * aProduct = aList->ElementAtNumber(j);
      if (!((feedItem*) aProduct)->GetCode() == code)
      {
            theMessage->FatalError("field - check this function");

      	aProduct->Setamount(aProduct->GetAmount()*area);
         found=true;
      }
   }
   if (!found)
   {
      switch (code)
      {
         case 565:
         case 593:
         case 596:
         case 522:
         {
         	feedItem * afeedItem= new feedItem();
            afeedItem->SetCode(code);
         }
         break;
         default:{};
      }
   }
}

/****************************************************************************\
\****************************************************************************/
int  field::GetForageItemCode()
{
	int theCode=0;
   theMessage->FatalError("field::GetForageItemCode - code not syncronized and hence invalid");

/* @@@ This is not brought up to date - do later. BMP MAY 2006 !!!!!!!!!!!!!!!!!
   for(int j=0;j<crrPlan->GiveOpListP()->NumOfNodes();j++)
   {
	   fieldOperationFields * anOP = crrPlan->GiveOpListP()->ElementAtNumber(j);
	   if ((anOP->GetOperationId()==CombineHarvesting) || (anOP->GetOperationId()==CombineHarvestingShred)
      	|| anOP->GetOperationId()==BeetHarvesting || anOP->GetOperationId()==ForageHarvesting || anOP->GetOperationId()==MakeGrazable)
      {
			linkList <plantItem> *aList= ((harvestFields* )anOP)->GetForageProducts();
         theCode = theProducts->GetHarvestProductCode(aList);
      }
   }
*/
  	return theCode;
}

/****************************************************************************\
Set grazable to true if crop is of type that is grazable
\****************************************************************************/
void field::AutoGrazingPossible()
{
   char *cropId;
//   bool ret_val=false;
   patch* mainPatch;
   mainPatch = thePatches->ElementAtNumber(0);
   PresentCrop_(cropId);
   if ((strcmp("C1",cropId)==0) || (strcmp("C2",cropId)==0) || (strcmp("G1",cropId)==0) ||
      (strcmp("G2",cropId)==0) || (strcmp("K1",cropId)==0) || (strcmp("G4",cropId)==0))
      {
	      mainPatch->Setgrazable(true);
         grazingPossible=true;
      }
   else
   {
      mainPatch->Setgrazable(false);
      grazingPossible=false;
   }
}

/****************************************************************************\
\****************************************************************************/
double field::GetDMGrazed()
{
   double ret_val = 0.0;
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      ret_val+=p->GetGrazedDM();
   }
   return ret_val;
}

/****************************************************************************\
Returns above ground crop N in kg/ha
\****************************************************************************/
double field::GetAbovegroundCropN()
{
   double ret_val = 0.0;
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      ret_val+=p->GetAbovegroundCropN();
   }
   return ret_val;
}

/****************************************************************************\
Returns growth in kg DM
\****************************************************************************/
double field::GetDailyDMProduction()
{
	double ret_val=0.0;
   for (int i=0;i<thePatches->NumOfNodes();i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      ret_val+=p->GetDailyDMProduction();
   }
   return ret_val;
}

/****************************************************************************\
Estimate potential daily growth, given radiation and temperature
\****************************************************************************/
double   field::InitExpectedGrowthRate(double radiation, double temperature)
{
   double ret_val = 0.0;
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      ret_val+=p->GetPotentialGrowthRate(radiation, temperature);
   }
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
double field::GetExpectedGrowth(bsTime *aDate)
{
	if (aDate->GetJulianDay()<=171)
   	return expectedCumulativeGrowth[0];
   else
   	return expectedCumulativeGrowth[1];
}

/**************************************************************
This resets the variables that track grazing
Done here cos we might be interested in the variables for debugging
**************************************************************/
void field::ClearTemporaryVariables()
{
   for (int i=0;i<thePatches->NumOfNodes();i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      p->clearPatchRecords();
   }
}

