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
#include <string.h>
#include "../base/commonData.h"
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

	string80 Name;
	int fm,fd,lm,ld;
	commonData  data;
	data.setCritical(true);
	data.readFile(filename);
	int first,num;
	data.getSectionsNumbers("period",&first,&num);
  	for(int index=first;index<=(num);index++)
  	{
  		data.FindSection("period",index);
  		string name;
  		data.FindItem("Name",&name);
  		strcpy(Name,name.c_str());
  		data.FindItem("firstmonth",&fm);
  		data.FindItem("firstday",&fd);
  		data.FindItem("lastmonth",&lm);
  		data.FindItem("lastday",&ld);
		period * per=new period(Name,fm,fd,lm,ld);
		PeriodList->InsertLast(per);
      NumOfPer++;
  	}



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


