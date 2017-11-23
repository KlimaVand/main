/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "message.h"
#include "periods.h"
#include "base.h"

/****************************************************************************\
  Constructor
\****************************************************************************/
periods::periods()
{
   NumOfPer=0;
	PeriodList=new linkList<period>;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
periods::~periods()
{
	delete PeriodList;
}

/****************************************************************************\
\****************************************************************************/
void periods::Initialize(string  filename)
{
	base aBase;
	string80 Name;
	int fm,fd,lm,ld;
   aBase.SetCritical();
	aBase.OpenInputFileString(filename);
	int first,num;
   aBase.GetSectionNumbers("period",&first,&num);
  	for(int index=first;index<(first+num);index++)
  	{
 		aBase.FindSection("period",index);
		aBase.GetParameter("Name",Name);
		aBase.GetParameter("firstmonth",&fm);
		aBase.GetParameter("firstday",&fd);
		aBase.GetParameter("lastmonth",&lm);
		aBase.GetParameter("lastday",&ld);
		period * per=new period(Name,fm,fd,lm,ld);
		PeriodList->InsertLast(per);
      NumOfPer++;
  	}


   aBase.CloseInputFile();
}



/****************************************************************************\
\****************************************************************************/
int periods::GetActualPeriodNumber(const int month,const int day)
{
   int index = 0;
	linkList<period>::PS P;
	PeriodList->PeekHead(P);
   while ((P) && !P->element->IsDateInPeriod(month,day))
   {
      PeriodList->OneStep(P);
      index++;
   }

   if (!P)
		theMessage->FatalError("periods::GetActualPeriod - period not found");

  	return index;
}


