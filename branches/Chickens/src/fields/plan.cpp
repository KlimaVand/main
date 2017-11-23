/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "plan.h"
#include "fieldOperation/animalFertFields.h"

#ifdef MEASCOPE
   #include "climate.h"
#endif

/****************************************************************************\
Constructor
\****************************************************************************/
plan::plan()
{
	area=0;
   newArea=0;
	OpListP=NULL;
	NextOperation=NULL;
	finished=false;
   irrigationIncluded=false;
   automaticIrrigation=false;
   futureNum=-1;
}

/****************************************************************************\
Copy constructor
\****************************************************************************/
plan::plan(const plan& p)
{
	strcpy(CropId,p.CropId);
	area=p.area;
	newArea=p.newArea;
	finished=p.finished;
   irrigationIncluded=p.irrigationIncluded;
   automaticIrrigation=p.automaticIrrigation;
   OpListP=p.OpListP->clone();
	OpListP->PeekHead(NextOperation);  // This means that "clone" is restricted in use
   futureNum=p.futureNum;
}

/****************************************************************************\
Destructor
\****************************************************************************/
plan::~plan()
{
 	delete OpListP;
}

/****************************************************************************\
clone - used in connection with 'cloneList'
\****************************************************************************/
plan* plan::clone() const
{
	plan* p = new plan(*this);
	return p;
}

/**
 * Make object ready for use.
 */
void plan::HandPlanOver(cloneList<fieldOperationFields> * OP,string crop_id,int futrNum)
{
   futureNum=futrNum;
	OpListP=OP;
	OpListP->PeekHead(NextOperation);
   if (NextOperation==NULL)
      theMessage->Warning("plan::HandPlanOver - no operations specified for this field");  // JB changed this 7/11-02 as might not be wrong (i.e. permanent grass)
   else
   {
      cloneList<fieldOperationFields>::PS P,P1;
      OpListP->PeekHead(P);
      while (P!=NULL)
      {
         if (strcmp(P->element->GetCropId(),"ADJUSTAREA")==0)
         {
            newArea=P->element->GetArea();
            P1=P;
         }
         else if (area<1E-20)
            {
               area=P->element->GetArea();
               strcpy(CropId,crop_id);
            }
         if (P->element->IsIrrigationOp())
         {
            irrigationIncluded=true;
            automaticIrrigation=P->element->AutomaticIrrigation();
         }
         OpListP->OneStep(P);
      }
   }
}

/**
 * If a operation for the date in question exists, a pointer to the
 * corresponding object is returned, otherwise NULL is returned.
 */
fieldOperationFields * plan::ReturnOperation(int year,int month,int day,bool update)
{
	if (NextOperation==NULL)
   {
		finished=true;
		return NULL;
	}
	cloneList<fieldOperationFields>::PS NxtOp=NextOperation;
  	long int idx=year*512+month*32+day;
#ifdef MEASCOPE
	long int operatindex;
   bool doOp = false;
   operatindex=NxtOp->element->GetStartYear()*512
                 +NxtOp->element->GetStartMonth()*32
                 +NxtOp->element->GetStartDay();
	if (operatindex<=idx)
   {
   	if (NxtOp->element->GetTSum()>=theClimate->GetdegreeDays())
      	doOp=true;
	   operatindex=NxtOp->element->GetEndYear()*512
                 +NxtOp->element->GetEndMonth()*32
                 +NxtOp->element->GetEndDay();
		if (operatindex<=idx)
      	doOp=true;
	}
   if (doOp)
   {
   	if (update)
			OpListP->OneStep(NextOperation);
		return NxtOp->element;
   }
	else
		return NULL;
#else
	long int operatindex=NxtOp->element->GetYear()*512
                    +NxtOp->element->GetMonth()*32
                    +NxtOp->element->GetDay();
	if (operatindex<=idx)
   {
   	if (update)
			OpListP->OneStep(NextOperation);
		return NxtOp->element;
	}
	else
		return NULL;
#endif
}

/**
 * see if a given fieldOperationFields has the same day as current fieldOperationFields
 */
bool plan::IsThisTheFirst(fieldOperationFields * aOperation)
{
	cloneList<fieldOperationFields>::PS FirstOperation;
	OpListP->PeekHead(FirstOperation);
#ifdef MEASCOPE
   bool retVal;
   if (!aOperation->GetopStart())
   {
	   if (!FirstOperation->element->GetopStart())
	   	retVal=((aOperation->GetYear()==FirstOperation->element->GetYear())
			  && (aOperation->GetMonth()==FirstOperation->element->GetMonth())
			  && (aOperation->GetDay()==FirstOperation->element->GetDay()));
      else
	   	retVal=((aOperation->GetYear()==FirstOperation->element->GetStartYear())
			  && (aOperation->GetMonth()==FirstOperation->element->GetStartMonth())
			  && (aOperation->GetDay()==FirstOperation->element->GetStartDay()));
   }
   else
   {
	   if (!FirstOperation->element->GetopStart())
	   	retVal=((aOperation->GetStartYear()==FirstOperation->element->GetYear())
			  && (aOperation->GetStartMonth()==FirstOperation->element->GetMonth())
			  && (aOperation->GetStartDay()==FirstOperation->element->GetDay()));
      else
	   	retVal=((aOperation->GetStartYear()==FirstOperation->element->GetStartYear())
			  && (aOperation->GetStartMonth()==FirstOperation->element->GetStartMonth())
			  && (aOperation->GetStartDay()==FirstOperation->element->GetStartDay()));
   }
#else
   bool retVal=((aOperation->GetYear()==FirstOperation->element->GetYear())
			  && (aOperation->GetMonth()==FirstOperation->element->GetMonth())
			  && (aOperation->GetDay()==FirstOperation->element->GetDay()));
#endif
   return retVal;
}



/****************************************************************************\
\****************************************************************************/
bool plan::AutomaticIrrigation()
{
   return automaticIrrigation;
}

/****************************************************************************\
States whether the first operation is today. Note that the function also
will return 1 if the date for the first operation is passed.
\****************************************************************************/
bool plan::FirstOperationToday(int year,int month,int day)
{
	cloneList<fieldOperationFields>::PS P;
	if (OpListP!=NULL && OpListP->NumOfNodes()>0)
   {
		OpListP->PeekHead(P);
	  	long int idx=year*512+month*32+day;

		long int operatindex;
   #ifdef MEASCOPE
      if (P->element->GetopStart())
      operatindex=P->element->GetopStart()->GetYear()*512
      	              +P->element->GetopStart()->GetMonth()*32
         	           +P->element->GetopStart()->GetDay();

		else
   #endif
      operatindex=P->element->GetYear()*512
      	         +P->element->GetMonth()*32
         	      +P->element->GetDay();
		if (P!=NULL)
			return (operatindex<=idx);
	}
	return 0;
}

/****************************************************************************\
Returns true if there are no further operations concerning the present crop.
\****************************************************************************/
bool plan::Finished()
{
	return finished;
}



/****************************************************************************\
\****************************************************************************/
double plan::GiveArea()
{
	return area;
}

/****************************************************************************\
\****************************************************************************/
void plan::GiveCrop(string * crop_id)
{
	strcpy(*crop_id,CropId);
}

/****************************************************************************\
\****************************************************************************/
bool plan::NextIsHarvestOp()
{
	if (NextOperation!=NULL)
		return NextOperation->element->IsHarvestOp();
	else
		return false;
}

/****************************************************************************\
\****************************************************************************/
bool plan::NextIsForageHarvestOp()
{
	if (NextOperation!=NULL)
		return NextOperation->element->IsForageHarvestOp();
	else
		return false;
}

/****************************************************************************\
\****************************************************************************/
bool plan::NextIsForcedHarvest()
{
	if (NextOperation!=NULL)
      return NextOperation->element->HarvestAndForced();
   else
      return false;
}

/****************************************************************************\
\****************************************************************************/
bool plan::NextIsFertilizing()
{
	if (NextOperation!=NULL)
      return NextOperation->element->IsFertilizingOp();
   else
      return false;
}



/****************************************************************************\
NJH Aug 2002
Used by grazing manager to insert a pasture cut
\****************************************************************************/
void plan::InsertAsNextOperation(fieldOperationFields * anOperation)
{
   cloneList<fieldOperationFields>::PS PNULL=NULL;
   if (NextOperation)
   {
	   OpListP->InsertBefore(NextOperation,PNULL,anOperation);
      fieldOperationFields * aTestOp;
      for(int i=0;i<OpListP->NumOfNodes();i++)      //find where InsertBefore put the operation and make it NextOperation
      {
         aTestOp=OpListP->ElementAtNumber(i);
         if (aTestOp == anOperation)
            OpListP->SearchByNumber(NextOperation,i);
      }
   }
   else
   {
	   OpListP->InsertLast(anOperation);
      OpListP->PeekTail(NextOperation);
   }
}

/****************************************************************************\
\****************************************************************************/
double plan::GetAdjustArea()
{
	return newArea;
}

/****************************************************************************\
\****************************************************************************/
void plan::RemoveHerbageHarvests()
{
	cloneList<fieldOperationFields>::PS P;
	OpListP->PeekHead(P);
	while (P!=NULL)
   {
      if (P->element->GetOperationId()==CutSetAside || P->element->GetOperationId()==ForageHarvesting ||
  			 P->element->GetOperationId()==Mowing)
      {
      	if (NextOperation)
         {
            if (NextOperation->element==P->element)
            {
               if (!P->next==0)
                  NextOperation=P->next;
               else
                  NextOperation=NULL;
            }
            OpListP->Remove(P);
            theMessage->Warning("Herbage harvest operation removed");
         }
      }
		OpListP->OneStep(P);
	}
}

#ifdef MEASCOPE //------------------------------------------------------------

/****************************************************************************\
NJH Sep 2005
Used to see if a TSum triggered operation can be performed
\****************************************************************************/
bool plan::CheckTSum()
{
	double TSum = NextOperation->element->GetTSum();
   if (theClimate->GetdegreeDays()>TSum)
   	return true;
   else
		return false;
}

/****************************************************************************\
NJH Sep 2005
Used to see if a soil moisture triggered operation can be performed
\****************************************************************************/
bool plan::CheckCriticalSoilMoisture(double aSoilMoisture)
{
	fieldOperationFields * anOperation = NextOperation->element;
	bool retVal=true;
	bool upper= anOperation->Getupper();
   if (upper)
   {
		if (aSoilMoisture > anOperation->GetsoilMoisture())
      retVal=false;
   }
   else
   {
		if (aSoilMoisture < anOperation->GetsoilMoisture())
      retVal=false;
   }
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
bool plan::CheckTSumMoist(double moisture)
{
	bool retVal=true;
   fieldOperationFields * anOperation = NextOperation->element;
   if (anOperation->GetopStart())
   {
   	retVal=false;
   	if ((CheckTSum())&&(CheckCriticalSoilMoisture(moisture)))
	      retVal=true;
   }
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
bool plan::CheckIfToDo(double moisture)
{
	bool retVal=true;
   if (NextOperation==NULL)
   	return false;
   else
   {
      fieldOperationFields * anOperation = NextOperation->element;
      if (anOperation->GetopStart())
      {
         retVal=false;
         if ((theTime>=*anOperation->GetopStart())&&(CheckTSumMoist(moisture)))
            retVal=true;
      }
   }
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
bool plan::CheckIfMustDo()
{
	bool retVal=false;
   if (NextOperation==NULL)
   	return false;
   else
   {
      fieldOperationFields * anOperation = NextOperation->element;
      if (anOperation->GetopEnd())
			if (theTime>=*anOperation->GetopEnd())
            retVal=true;
   }
   return retVal;
}

void plan::CheckIfFinished()
{
   if (NextOperation==NULL)
   	finished=true;
};

/****************************************************************************\
Returns true if first operation is in more than one year's time
Used to trap situations where there are no operations to trigger adoption of a new plan (mainly fallow)
NJH Aug 2007
\****************************************************************************/
bool plan::NoOpsThisYear(int year,int month,int day)
{
	if (OpListP!=NULL && OpListP->NumOfNodes()>0)
   {
      int daysToOp=0;
//      int currentJulianDay = NextOperation->element->GetopEnd()->GetJulianDay(day, month,year);
      int currentJulianDay = NextOperation->element->GetopEnd()->GetJulianDay(1, 8, year);
      int nextOpJulianDay =  NextOperation->element->GetopEnd()->GetJulianDay();
      if ((NextOperation->element->GetopEnd()->GetYear()) > year)
      	daysToOp = 365 - currentJulianDay +  nextOpJulianDay;
      if (daysToOp > 365)
      	return true;
		else
			return false;
	}
	return 0;
}

/****************************************************************************\
Check to see if the ith operation has an end date that is after the end date of the (i+1)th operation
NJH Oct 2007
\****************************************************************************/
void plan::CheckOps()
{
	cloneList<fieldOperationFields>::PS P;
	OpListP->PeekHead(P);
	long int operatindex=0;
	long int nextoperatindex=100000000;
	while (P!=NULL)
   {
      operatindex=P->element->GetEndYear()*512
                    +P->element->GetEndMonth()*32
                    +P->element->GetEndDay();
      if (!P->next==0)
         nextoperatindex=P->next->element->GetEndYear()*512
                    +P->next->element->GetEndMonth()*32
                    +P->next->element->GetEndDay();
      else
         nextoperatindex=1000000000;
      if (operatindex>nextoperatindex)
      {
      	cout << "Field no " << GetFutureNumber() << endl;   //should be a warning
      	cout << "Crop name " << P->element->GetCropId() << endl;   //should be a warning
//      	cout << "Operation name " << P->element->GetOperationId() << endl;   //should be a warning
         cout << "Operation name " << operationNameList[P->element->GetOperationId()].c_str()<< " with end date " <<
         		P->element->GetEndDay() << "." << P->element->GetEndMonth()<< endl;
			cout << "Overlaps with operation name " << operationNameList[P->next->element->GetOperationId()].c_str()<< " with end date " <<
         		P->next->element->GetEndDay() << "." << P->next->element->GetEndMonth()<< endl;
         theMessage->FatalError("plan: end date overlap");
      }
		OpListP->OneStep(P);
   }
}




#endif //----- MEASCOPE ----------------------------------------------------

