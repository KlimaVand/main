/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"

#include "../base/settings.h"
#include "field.h"
#include "../base/unix_util.h"
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

#include "../base/controlParameters.h"

/****************************************************************************\
Constructor with arguments
\****************************************************************************/
field::field(const char * aName, const int aIndex, const base * aOwner)
   :base(aName,aIndex,aOwner)
{

	Lfutr                      =new cloneList<plan>;
	crrPlan                    =NULL;
   thePatches                 =new cloneList<patch>;
   Minimum                    =0.00001;

   NoFixCrops                 =0;

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


   for (int i=0;i<BackScope;i++)
   {
      History[i]="w1";
   }

   for (int i=0;i<BackScope;i++)
   {
      FixedCrop[i]=NULL;
      FixedSize[i]=0.0;
   }

   if (monitorPatches)
   {
      char filename[40];
      char leader[200];
      sprintf(leader,globalSettings[id]->getOutputDirectory().c_str());

      sprintf(filename,"patches%d1.xls",Index);
      strcat(leader,filename);
      fPatch1.open(leader,ios::out);
      fPatch1.precision(8);
      fPatch1 << "Date\t";
      for (int i=0;i<MaxWritePatches;i++)
         fPatch1 << "Area" << i << "\t";


      sprintf(leader,globalSettings[id]->getOutputDirectory().c_str());
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

   if (globalSettings[id]->DetailsData.getPlantDetails()==true)
   {
      char filename[40];
      char leader[200];

      sprintf(leader,globalSettings[id]->getOutputDirectory().c_str());
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

   distance=fl.distance;

   JBNO=fl.JBNO;
   daysSinceIrrigation=fl.daysSinceIrrigation;
   daysSinceIrrigationRequest=fl.daysSinceIrrigationRequest;
   daysBetweenIrrigation=fl.daysBetweenIrrigation;
   fingerFlow=fl.fingerFlow;
 	irrigationDelay=fl.irrigationDelay;
  	NoFixCrops=fl.NoFixCrops;
   active=fl.active;

   grazingPossible=fl.grazingPossible;
   if (globalSettings[id]->DetailsData.getPlantDetails()==true)
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
   if (globalSettings[id]->DetailsData.getPlantDetails()==true)
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

}

/****************************************************************************\
\****************************************************************************/
void field::InitVariables()
{
   area					= 0.0;
   grazingPossible   = false;

   monitorPatches=false;
   grazingShutDown   = true;
/*
   expectedCumulativeGrowth[0]=0.0; //@@@!!!???
   expectedCumulativeGrowth[1]=0.0; //@@@!!!???
*/
}

#ifdef NITROSCAPE
void field::UpdatePatch(int farmNo)
{
//	pixiData thePixiData;

//	int NumberOfPatches= thePatches->NumOfNodes();
   patch * patchVariable;
	int NumberOfPatches=thePixiData->getNumberOfEco(farmNo,Index);
   double areaPerPatch = area/NumberOfPatches;
   string soilFileName;
	if (NumberOfPatches>0)
	{
		patchVariable=thePatches->ElementAtNumber(0);
        soilFileName += patchVariable->getSoilFileName();
        patchVariable->SetArea(areaPerPatch);
	}
	for(int i=0;i<NumberOfPatches;i++)
	{
		cout<< "Farm No " << farmNo<<" Field no "<<Index<<" patch no " << i<<endl;
		if(thePixiData->getNumberOfFarms()<farmNo)
		{
			cout<<"field::UpdatePatch: farm does not exist in fasset.dist"<<endl;
			cout << "Farm number = " << farmNo << " number in fasset.dist " << thePixiData->getNumberOfFarms() << endl;
         cin.get();
			exit(99);
		}
		if(thePixiData->getNumberOfFields(farmNo)<=Index)
		{
			cout<<"field::UpdatePatch: fields does not exist in fasset.dist"<<endl;
         cin.get();
			exit(99);
		}
		if(thePixiData->getNumberOfEco(farmNo,Index)<=i)
		{
					cout<<"field::UpdatePatch: patch does not exist in fasset.dist"<<endl;
         		cin.get();
					exit(99);
		}
		int EcosystemNumber=thePixiData->getEco(farmNo,Index, i);
		cout<< "Farm No " << farmNo<<" Field no "<<Index<<" patch no " << i << " ecosys no " << EcosystemNumber <<endl;
      if (i>0)
      {
         patchVariable =new patch("patch",Index,this);
         patchVariable->Initialise(soilFileName,i,areaPerPatch);
         thePatches->InsertLast(patchVariable);
      }
//		patch * patchVariable=thePatches->ElementAtNumber(i);
		patchVariable->setEcoModel(EcosystemNumber);
	}
}
#endif
/****************************************************************************\
\****************************************************************************/
void field::ReadPatchParameters(patch *p, commonData * &data)
{
	string soilFileName;

   double patchArea;
   data->setCritical(true);
   data->FindItem("JBNO",&JBNO);
   data->setCritical(false);
   if (!data->FindItem("soilFile",&soilFileName))
   {
	   convert conv;
	   soilFileName="JB"+conv.IntToString(JBNO)+"-M.dat";

   }
   data->FindItem("area",&patchArea);
   area+=patchArea;
   if (area<=0.0)
         theMessage[id]->FatalError("field::ReadParameters - field area cannot be zero");
   p->Initialise(soilFileName,0,patchArea);
}

#ifdef NITROSCAPE
void field::zeroOutputVariables()
{

	for(int i=0;i<<thePatches->NumOfNodes();i++)
	{
		thePatches->ElementAtNumber(i)->zeroOutputVariables();
	}
}
#endif
/****************************************************************************\
\****************************************************************************/
void field::ReadParameters(commonData * &file)
{
   if (thePatches->NumOfNodes()>1)
      theMessage[id]->FatalError("field::ReadParameters - only one patch allowed here");

   file->setCritical(true);
   if (file->FindSection(Name,Index))
   {
	   file->setCritical(false);
      int first,num;
      string namestr;
      char Indexstr[10];
      sprintf(Indexstr,"(%d)",Index);
      namestr=(string) "field" +Indexstr+"."+"patch";
      file->getSectionsNumbers(namestr,&first,&num);  //count the number of patches
#ifndef NITROSCAPE
      if (num>0) //may be more than one patch per field
      {
	      Setcur_pos(0);
         for(int inx=first;inx<=(first+num);inx++)
         {
          file->FindSection(namestr,inx);  // find the correct place in the input file
          patch * p =new patch((char *)namestr.c_str(),inx,this);
          ReadPatchParameters(p,file);
          thePatches->InsertLast(p);
         }
		}
      else   //one patch per field
      {
	      patch * p =new patch("patch",Index,this);
         ReadPatchParameters(p,file);
   	   thePatches->InsertLast(p);
		}

#else

/*      if (num>0) //may be more than one patch per field
      {
		  for(int inx=first;inx<(first+num);inx++)
		  { Setcur_pos(0);
			  FindSection((char*) namestr.c_str(),inx);  // find the correct place in the input file
			  ReadPatchParameters(thePatches->ElementAtNumber(inx));
		  }
      }
      else   //one patch per field
	 {*/
		patch * p =new patch("patch",Index,this);
		ReadPatchParameters(p);
		thePatches->InsertLast(p);
	//}

#endif

   	char number[3];
      char s[21];
      if (theControlParameters[id]->GetUniSizeFields())
         area=1.0; // For testing purposes!
      file->FindItem("distance",&distance);


      int k;
		for (k=0;k<BackScope;k++)
		{
			itoa(k,number,10);
			strcpy(s,"CROP");
			strcat(s,number);
			 file->FindItem(s,&History[k]);
		}

   }
   else
      theMessage[id]->FatalError("field::ReadParameters - section not found");

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
	fP->History[3]=*crop_id;

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
      theMessage[id]->FatalError("field::AjustPatchAreas - call parameter must be above zero");
   if (adjust>1.0 && !balance)
      theMessage[id]->FatalError("field::AjustPatchAreas - call parameter must be not exceed one in this mode");

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
      theMessage[id]->FatalError("field::AjustPatchAreas - attempt to divide uninitialised field");

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
      theOutput[id]->AddIndicator(3700,"37.00 N difference from area adjustment","kg N",Nbefore-Nafter);
      theOutput[id]->AddIndicator(3701,"37.01 N crop difference from area adjustment","kg N",NcropBefore-NcropAfter);
      theOutput[id]->AddIndicator(3702,"37.02 N soil difference from area adjustment","kg N",NsoilBefore-NsoilAfter);
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

   fP->daysSinceIrrigation=daysSinceIrrigation;
	fP->daysSinceIrrigationRequest=daysSinceIrrigationRequest;
   fP->daysBetweenIrrigation=daysBetweenIrrigation;
   fP->irrigationDelay=irrigationDelay;
   fP->fingerFlow=fingerFlow;

	fP->JBNO=JBNO;
	ofstream * fstr=theMessage[id]->GiveHandle();
	*fstr << "Divided. Area 'mother': " << area << "ha. Area 'child': " << Pl->GiveArea() << " ha" << "\n";

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
		theMessage[id]->WarningWithDisplay("field::MakeInstance - attempt to divide a field with an area of zero");
	if (Ar==0.0)
		theMessage[id]->WarningWithDisplay("field::MakeInstance - attempt to divide a field with request of an area of zero");

	if (Ar>area)
   {
		if (Ar>(area+Minimum)) // Handles rounding-erors
		{
         if (theControlParameters[id]->GetFlexSizeFields())
            AdjustFieldArea(Ar);
         else
   			theMessage[id]->FatalError("field::MakeInstance - attempt to divide a field with request of an area bigger than that of the field");
		}
		Ar=area;
	}
	if (area>0.0)
   {
		if (area > Ar && theControlParameters[id]->GetFlexSizeFields()) // Test if field size should be adjusted
         AdjustFieldArea(Ar);
		if (area > (Ar+Minimum)) // Handles rounding-erors
       	DivideThisInstance(fP,Pl);
		else
      { // The field is not to be divided, but renewed
			if ((Lfutr->NumOfNodes())>1)
				theMessage[id]->WarningWithDisplay("field::MakeInstance - error in list of future plans");
			if (Pl->GetFutureNumber()!=Index)
         {
            cout << Pl->GetFutureNumber() << endl;
				theMessage[id]->FatalError("field::MakeInstance - inconsistence in indices");
         }
			if (crrPlan)
				delete crrPlan;
			crrPlan=Pl;
         if (crrPlan->GetAdjustArea()>0)
            AdjustFieldArea(crrPlan->GetAdjustArea());

			PushHistory(this,&cid);
			fstr=theMessage[id]->GiveHandle();
			*fstr << "Renewed. Size: " << area << " ha" << endl;

		}
	}
#ifdef TUPLE
   writeTuples=theControlParameters[id]->GetWriteTuples();
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
            int year = theTime[id].GetYear();
            int month = theTime[id].GetMonth();
            int day = theTime[id].GetDay();
#ifdef MEASCOPE
            if ((P->element->FirstOperationToday(year,month,day) && (OperationCanBePerformed(P->element,thePatches->ElementAtNumber(0))))
            		|| (P->element->NoOpsThisYear(year,month,day)))
#else
            if (P->element->FirstOperationToday(year,month,day) && (OperationCanBePerformed(P->element,thePatches->ElementAtNumber(0))))
#endif
            {

               fstr=theMessage[id]->GiveHandle();
               *fstr << theTime << " >>> Field no " << setw(2) << Index << "  ";

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
      	theMessage[id]->WarningWithDisplay("field::ReadParameters - to many patches on field");
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
	   ofstream * fstr=theMessage[id]->GiveHandle();
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
   return (standingWater<1.0 || theTime[id].GetMonth()==5); // To ensure that fertilizing is possible later
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
			theProducts[id]->GiveProductInformation(aWater);
			aWater->Setamount(30*10); // 30mm, 1ha
#ifdef ARLAS
		   FieldSuitcase->AddIndicator("irrigation",30.0);
#endif
			irrigateFields * FOP=new irrigateFields;
         FOP->DefineIrrigateFieldsOp(Irrigating,crop_id,crop_id,year,month,day,FieldArea_(),0,DistanceToField_(),aWater,false);
		   HandleOperation(FOP);
			theTechnics[id]->ExecuteFields(FOP);
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
      if (theControlParameters[id]->GetForcedHarvest())
      	ReadyForHarvest = aPlan->NextIsHarvestOp() && (aPatch->ReadyForHarvestOrLater() || aPlan->NextIsForcedHarvest());
      else
      	ReadyForHarvest = aPlan->NextIsForageHarvestOp() || (aPlan->NextIsHarvestOp() && aPatch->ReadyForHarvestOrLater());
      bool ReadyForFertilizing = aPlan->NextIsFertilizing() && (SoilIsSuitableForFertilizing() || theControlParameters[id]->GetForcedOperations());
      bool ReadyForOtherOp = theControlParameters[id]->GetForcedOperations() || (!aPlan->NextIsHarvestOp() && !aPlan->NextIsFertilizing());

/* from MEL_FASSET
      bool ReadyForOtherOp = false;
      if (!aPlan->NextIsHarvestOp())
         ReadyForOtherOp = theControlParameters[id]->GetForcedOperations(); // Everything but harvest, incl. fertilization !!!
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
   int year = theTime[id].GetYear();
   int month = theTime[id].GetMonth();
   int day = theTime[id].GetDay();
 //  cout << " " << GetIndex();
   if (active)
   {


      daysSinceIrrigation++;
      cloneList<patch>::PS P;
      thePatches->PeekHead(P);
      if (P==NULL)
         theMessage[id]->FatalError("field::UpdateField - no patches in field");
      if (P->element==NULL)
         theMessage[id]->FatalError("field::UpdateField - pointer error");
      fieldOperationFields * FOP=NULL;
      if (crrPlan!=NULL && !crrPlan->Finished())
         FOP=crrPlan->ReturnOperation(year,month,day,false);
      long int delay=0;
      if (FOP!=NULL)
      {
#ifndef MEASCOPE
         delay=theTime[id].GetDateValue() - FOP->GetOpTime()->GetDateValue();
         if (delay>65)
         {
            cout << endl << GetLongName();
            theMessage[id]->FatalError("field::UpdateField - field operation not performed after more than 65 days delay");
         }
#else
         delay=theTime[id].GetDateValue() - FOP->GetopStart()->GetDateValue();
         if (delay>120)
         {
            cout << endl << GetLongName();
            theMessage[id]->FatalError("field::UpdateField - field operation not performed after more than 120 days delay");
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
                        theMessage[id]->Warning("Operation forced at end date: field no and op name = ",
                           Index,operationNameList[FOP->GetOperationId()].c_str());
                  }
                  crrPlan->CheckIfFinished();
#else
                  if (delay>20)
                  {
                     cout << endl << delay << " days delay of operation type " << FOP->GetOperationId() << " due for " << *FOP->GetOpTime() << " on field no " << Index << endl;
                     if (delay>60)
                        theMessage[id]->WarningWithDisplay("field::UpdateField - field operation performed after more than 60 days delay");
                     else
                        theMessage[id]->Warning("field::UpdateField - field operation performed after more than 20 days delay");
                  }
#endif
                  theTechnics[id]->ExecuteFields(FOP);
                  ofstream * file=theMessage[id]->GiveHandle();  // write end of line marker
                  *file << endl;

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
            AddTuple(theTime[id].GetString2(),deposition_sum*area,0,"DEPOSITION","DEPOSITION",0);
            AddTuple(theTime[id].GetString2(),fixation_sum*area,0,"FIXATION","FIXATION",0);
            deposition_sum=0.0;
            fixation_sum=0.0;
         }
      }
#endif

      UpdatePatches();

		GiveIndicators();

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
   if (globalSettings[id]->DetailsData.getPlantDetails()==true)
      OutputPlantDetails(false);

}

/****************************************************************************\
Returns the crop for a previous season
Parameters
   nr        season
   crop_id   reference for the crop id
\****************************************************************************/
void field::CropHistory(int nr,char * &crop_id)
{
   crop_id =new char[3];
   strcpy(crop_id,History[nr].c_str());
   if (!active)
      theMessage[id]->FatalError("field::CropHistory called for inactive field");
}

/****************************************************************************\
Returns the present crop
\****************************************************************************/
void field::PresentCrop_(char * &crop_id)
{
	crop_id =new char[5];
	strcpy(crop_id,History[3].c_str());
}

/****************************************************************************\
Returns the distance from farm to field (meters)
\****************************************************************************/
double field::DistanceToField_()
{
	return distance;
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
   if (theControlParameters[id]->GetSimTupleHasBeenOpened())
      tuple.open(s1.c_str(),ios::app);
   else
   {
      tuple.open(s1.c_str(),ios::out);
      tuple << "Key\tFarmId\tFieldno\tDate\tIndex\tMainCrop\tArea\tOperation\tProduct\tN\tAmount\tSFU" << endl;
      theControlParameters[id]->SetSimTupleHasBeenOpened(true);
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
      string farmId=theControlParameters[id]->GetFarmID();
      if (farmId=="")
         theMessage[id]->FatalError("fieldOrder::AddTuple - attempt to add tuples but no farm ID specified");
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

    		f << "\tCropName\tphase\tTempSum\tCO2conc\tGLAI\tYLAI\tDMRoot\tDMRootPL\tDMRootTubers\tDMtop\tDMStorage\tDMTotalTop";
    	     	   f << "\tTranspirationRatio\tIrrigationDemand\tHeight\tRootDepth\tTotalRootLength\tInterceptedRadiation";
     	   f << "\tNitrogen\tN15\tNmin()\tNmax()\tNStorage\tNTop\tNVegTop\tNRoot\tNRootPL\tNRootTubers\tfNitrogen\tfNitrogenCurve";
     	   f << "\tRootDeposit\tRootTrans\tNRootDeposit\tTopDeposit\tNTopDeposit";
         f << "\tTopProd\tNUptake\tNfix\tNfixAcc\tdeltaDM\tN15Root\tAccRootResp";
         for(unsigned int i=0;i<globalSettings[id]->RootData.size();i++)
         {
            f << "\tRootLength-"<< globalSettings[id]->RootData[i].getStartDepth() << "-"<< globalSettings[id]->RootData[i].getEndDepth();
         	f << "\tRootMass-"<< globalSettings[id]->RootData[i].getStartDepth() << "-"<< globalSettings[id]->RootData[i].getEndDepth();
         	f << "\tRootN-"<< globalSettings[id]->RootData[i].getStartDepth() << "-"<< globalSettings[id]->RootData[i].getEndDepth();
      	}
      }


	}
   else
      f << theTime[id].GetYear() << "-" << theTime[id].GetMonth() << "-" << theTime[id].GetDay();

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
      theMessage[id]->WarningWithDisplay("Error in budget of field number ", GetIndex());
   }
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
Get herbage mass in excess of residual (kgDM/ha),  in kg
\****************************************************************************/
double field::GetAvailableHerbageMasskg(double residualDM)
{
	double mass= 0.0;
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      mass += p->GetAvailablePatchStandingDMkg(residualDM);
   }
   return mass;
}

/****************************************************************************\
Get herbage mass above cutting height in kg/ha
\****************************************************************************/
double field::GetAvailableHerbageMasskgPerHa(double residualDM)
{
	double mass= 0.0;
   int n=thePatches->NumOfNodes();
   for (int i=0;i<n;i++)
   {
      patch * p = thePatches->ElementAtNumber(i);
      mass += p->GetAvailablePatchStandingDMkg(residualDM)*p->GetArea();
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

   theOutput[id]->AddIndicator(3108,"31.08 N in faeces from grazing","kg N",animal_dung->GetAmount()*animal_dung->GetAllN().n*1000.0);
   theOutput[id]->AddIndicator(4109,"41.09 C in faeces from grazing","kg C",animal_dung->GetAmount()*animal_dung->GetC_content()*1000.0);

// new_area_dung+=dung->GetAmount()*110.0;  //OLD METHOD - area covered per tonne fresh weight from During and Weeda 73, NZJAS

   theOutput[id]->AddIndicator(3107,"31.07 N in urine from grazing","kg N",animal_urine->GetAmount()*animal_urine->GetAllN().n *1000.0);
   theOutput[id]->AddIndicator(4108,"41.08 C in urine from grazing","kg C",animal_urine->GetAmount()*animal_urine->GetC_content() *1000.0);
      // Calculate ammonia volatilisation
   theOutput[id]->AddIndicator(3146,"31.46 NH3-N loss during grazing","kg N",prop_lost_volat * animal_urine->GetAmount()
   						 * animal_urine->GetNH4_content().n * 1000.0);

   double size=0;
   for(int i=0;i<thePatches->NumOfNodes();i++)
   {
	   size+=thePatches->ElementAtNumber(i)->GetArea();
   }
   double totalAmountUrine=animal_urine->GetAmount();
   double totalAmountDung=animal_dung->GetAmount();
   for(int i=0;i<thePatches->NumOfNodes();i++)
   {
   	   double fraction=thePatches->ElementAtNumber(i)->GetArea()/size;
   	   animal_urine->Setamount(totalAmountUrine*fraction);
   	   animal_dung->Setamount(totalAmountDung*fraction);
   	   thePatches->ElementAtNumber(i)->DepositManure(animal_urine, animal_dung);
   }
   animal_urine->Setamount(totalAmountUrine);
   animal_dung->Setamount(totalAmountDung);


}


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
   int year = theTime[id].GetYear();
   int month = theTime[id].GetMonth();
   int day = theTime[id].GetDay();
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
Set grazable to true if crop is of type that is grazable
\****************************************************************************/
void field::AutoGrazingPossible()
{
   char *cropId;
//   bool ret_val=false;

   PresentCrop_(cropId);
   if ((strcmp("C1",cropId)==0) || (strcmp("C2",cropId)==0) || (strcmp("G1",cropId)==0) ||
      (strcmp("G2",cropId)==0) || (strcmp("K1",cropId)==0) || (strcmp("G4",cropId)==0))
      {
	   for(int i=0;i<thePatches->NumOfNodes();i++)
		   thePatches->ElementAtNumber(i)->Setgrazable(true);
         grazingPossible=true;
      }
   else
   {
	   for(int i=0;i<thePatches->NumOfNodes();i++)
	   	thePatches->ElementAtNumber(i)->Setgrazable(false);

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

