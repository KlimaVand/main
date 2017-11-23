/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "plan.h"
#include "fieldOperation/animalFertFields.h"


/****************************************************************************\
Constructor
\****************************************************************************/
plan::plan()
{
	area=0;
   newArea=0;
	OpListP=nullptr;
	NextOperation=nullptr;
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
   if (NextOperation==nullptr)
      theMessage->Warning("plan::HandPlanOver - no operations specified for this field");  // JB changed this 7/11-02 as might not be wrong (i.e. permanent grass)
   else
   {
      cloneList<fieldOperationFields>::PS P;
      OpListP->PeekHead(P);
      while (P!=nullptr)
      {
         if (strcmp(P->element->GetCropId(),"ADJUSTAREA")==0)
         {
            newArea=P->element->GetArea();

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
 * corresponding object is returned, otherwise nullptr is returned.
 */
fieldOperationFields * plan::ReturnOperation(int year,int month,int day,bool update)
{
	if (NextOperation==nullptr)
   {
		finished=true;
		return nullptr;
	}
	cloneList<fieldOperationFields>::PS NxtOp=NextOperation;
  	long int idx=year*512+month*32+day;
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
		return nullptr;

}

/**
 * see if a given fieldOperationFields has the same day as current fieldOperationFields
 */
bool plan::IsThisTheFirst(fieldOperationFields * aOperation)
{
	cloneList<fieldOperationFields>::PS FirstOperation;
	OpListP->PeekHead(FirstOperation);
   bool retVal=((aOperation->GetYear()==FirstOperation->element->GetYear())
			  && (aOperation->GetMonth()==FirstOperation->element->GetMonth())
			  && (aOperation->GetDay()==FirstOperation->element->GetDay()));

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
	if (OpListP!=nullptr && OpListP->NumOfNodes()>0)
   {
		OpListP->PeekHead(P);
	  	long int idx=year*512+month*32+day;

		long int operatindex;

      operatindex=P->element->GetYear()*512
      	         +P->element->GetMonth()*32
         	      +P->element->GetDay();
		if (P!=nullptr)
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
	if (NextOperation!=nullptr)
		return NextOperation->element->IsHarvestOp();
	else
		return false;
}

/****************************************************************************\
\****************************************************************************/
bool plan::NextIsForageHarvestOp()
{
	if (NextOperation!=nullptr)
		return NextOperation->element->IsForageHarvestOp();
	else
		return false;
}

/****************************************************************************\
\****************************************************************************/
bool plan::NextIsForcedHarvest()
{
	if (NextOperation!=nullptr)
      return NextOperation->element->HarvestAndForced();
   else
      return false;
}

/****************************************************************************\
\****************************************************************************/
bool plan::NextIsFertilizing()
{
	if (NextOperation!=nullptr)
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
   cloneList<fieldOperationFields>::PS Pnullptr=nullptr;
   if (NextOperation)
   {
	   OpListP->InsertBefore(NextOperation,Pnullptr,anOperation);
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
	while (P!=nullptr)
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
                  NextOperation=nullptr;
            }
            OpListP->Remove(P);
            theMessage->Warning("Herbage harvest operation removed");
         }
      }
		OpListP->OneStep(P);
	}
}
