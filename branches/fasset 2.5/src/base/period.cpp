/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "period.h"

	const int monthLength[12]={31,28,31,30,31,30,31,31,30,31,30,31};
/****************************************************************************\
Constructor with arguments
The period is inclusive both dates
\****************************************************************************/
period::period(char * aName,int firstMonth,int firstDay,int lastMonth,int lastDay)
{
	name=aName;
	firstmonth=firstMonth;
	firstday=firstDay;
   lastmonth=lastMonth;
   lastday=lastDay;
}
period::period(int firstMonth,int firstDay,int lastMonth,int lastDay)





{
	name="";
	firstmonth=firstMonth;
	firstday=firstDay;
   lastmonth=lastMonth;
   lastday=lastDay;
}

/****************************************************************************\
Copy constructor
\****************************************************************************/
period::period(const period& p)
{
   name       = p.name;
	firstmonth = p.firstmonth;
	firstday   = p.firstday;
   lastmonth  = p.lastmonth;
   lastday    = p.lastday;
}

/****************************************************************************\
Destructor
\****************************************************************************/
period::~period()
{
}

/****************************************************************************\
Assignment operator
\****************************************************************************/
period& period::operator=(const period& p)
{
   if (this==&p)
      return *this;
   name       = p.name;
	firstmonth = p.firstmonth;
	firstday   = p.firstday;
   lastmonth  = p.lastmonth;
   lastday    = p.lastday;
   return *this;
}

/****************************************************************************\
\****************************************************************************/
int period::IsDateInPeriod(int month,int day) const
{
   int a=month*31+day;
   int b1=firstmonth*31+firstday;
   int b2=lastmonth*31+lastday;
   int ret=(a>=b1 && a<=b2);
   return ret;
}

/****************************************************************************\
\****************************************************************************/
int period::GetLength()
{
   int daysum=lastday-firstday+1;
   if (lastmonth>firstmonth)
      for (int i=firstmonth;i<lastmonth;i++)
         daysum+=monthLength[i-1];
   return daysum;
}

/****************************************************************************\
\****************************************************************************/
int period::FullyContained(const period & per)
{
   return (IsDateInPeriod(per.firstmonth,per.firstday)
           && IsDateInPeriod(per.lastmonth,per.lastday));
}

/****************************************************************************\
day overlap between two periods added by JB
\****************************************************************************/
int period::Overlap(const period & per)
{
   int overlapDays = 0;
   int month = firstmonth;
   int day = firstday;
   while ((month<=lastmonth) && (day<=lastday))
   {
   	if (per.IsDateInPeriod(month,day)) overlapDays++;
      day++;
      if (day>monthLength[month-1])
      {
      	day=1;
         month++;
      }
   }

   return overlapDays;
}

