/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "bstime.h"
#include "message.h"

#include "../base/unix_util.h"
/****************************************************************************\
\****************************************************************************/
void bsTime::InitDaysPerMonth(void)
{
   tabDaysPerMonth[0][0]  = 31;
   tabDaysPerMonth[0][1]  = 28;
   tabDaysPerMonth[0][2]  = 31;
   tabDaysPerMonth[0][3]  = 30;
   tabDaysPerMonth[0][4]  = 31;
   tabDaysPerMonth[0][5]  = 30;
   tabDaysPerMonth[0][6]  = 31;
   tabDaysPerMonth[0][7]  = 31;
   tabDaysPerMonth[0][8]  = 30;
   tabDaysPerMonth[0][9]  = 31;
   tabDaysPerMonth[0][10] = 30;
   tabDaysPerMonth[0][11] = 31;

   tabDaysPerMonth[1][0]  = 31;
   tabDaysPerMonth[1][1]  = 29;
   tabDaysPerMonth[1][2]  = 31;
   tabDaysPerMonth[1][3]  = 30;
   tabDaysPerMonth[1][4]  = 31;
   tabDaysPerMonth[1][5]  = 30;
   tabDaysPerMonth[1][6]  = 31;
   tabDaysPerMonth[1][7]  = 31;
   tabDaysPerMonth[1][8]  = 30;
   tabDaysPerMonth[1][9]  = 31;
   tabDaysPerMonth[1][10] = 30;
   tabDaysPerMonth[1][11] = 31;
   int cumul = 0;
   for (int i = 1900; i <= 2300; i++)
   {
   	cumul += 365 + (((i%4==0) && (i%100!=0)) || (i%400==0));
      AccumulatedDaysSince1900[i - 1900] = cumul;
   }
}

/****************************************************************************\
\****************************************************************************/
bsTime::bsTime()
{
   InitDaysPerMonth();

   day       = 1;
   month     = 1;
   year      = 1995;
   dayInYear = 1;
   dayInWeek = 7;
   CalculateDateValue();
}

/****************************************************************************\
\****************************************************************************/
bsTime::bsTime(int d, int m, int y)
{
   InitDaysPerMonth();
   SetTime(d,m,y);
   CalculateDateValue();
}

/****************************************************************************\
\****************************************************************************/
bsTime::bsTime(const bsTime& f)
{
 InitDaysPerMonth();
 day=f.day;
 month= f.month;
 year = f.year;
 dayInYear = f.dayInYear;
 dayInWeek = f.dayInWeek;
 longTime = f.longTime;
 CalculateDateValue();
}

/****************************************************************************\
\****************************************************************************/
void bsTime::SetTime(int d, int m, int y)
{
   day       = d;
   month     = m;
   if ((y<0) || (y>2300))
#ifdef __BCplusplus__
	{
      theMessage->FatalError("Time::Illegal year");
      char ch=getch();
      if (ch=='y' || ch=='Y' )
         exit(99);
   }
#else
	exit(99);
#endif

   if (y<50) y = 2000+y;
   if (y<100) y = 1900+y;
   year      = y;
   dayInYear = 0;
   dayInWeek = 0;
   for(int i=0;i<m-1;i++)
      dayInYear += (tabDaysPerMonth[IsLeapYear(year)][i]);
   dayInYear += d;
   dayInWeek  = (dayInYear+6)%7;
   CalculateDateValue();
}

/****************************************************************************\
\****************************************************************************/
void bsTime::SetTime(string day,string month, string year)
{
   if (day=="")
      day="1";
   if (month=="")
      month="8";
   if (year=="R1")                                                                             // facilitates relative years (usefull in field plans)
   	SetTime(atoi((char*) day.c_str()), atoi((char*) month.c_str()), theTime.GetYear()+1);
   else
   	if (year=="" || year=="R0")
      	SetTime(atoi((char*) day.c_str()), atoi((char*) month.c_str()), theTime.GetYear());
  		else
         SetTime(atoi((char*) day.c_str()), atoi((char*) month.c_str()), atoi((char*) year.c_str()));
}

/****************************************************************************\
\****************************************************************************/
void bsTime::SetTime(string aTimeString)
{
   string newtime = aTimeString + " ";
   int index = min((int) aTimeString.find('-'),(int) aTimeString.find('/'));
   index = min(index,(int) aTimeString.find('.'));
   {
      index = max((int) aTimeString.find('-'),(int) aTimeString.find('/'));
      index = max(index,(int) aTimeString.find('.'));
   }
   int index1 = max((int) aTimeString.find('-',index+1),(int) aTimeString.find('/',index+1));
   index1 = max(index1,(int) aTimeString.find('.',index+1));
	if (index>0)
   {
   	string SubString[3];
      SubString[0] = aTimeString.substr(0,index);
      SubString[1] = aTimeString.substr(index+1,index1-index-1);

      if (index1>0)
      	SubString[2] = aTimeString.substr(index1+1);
      else
      	SubString[2] = "";
      SetTime(SubString[0], SubString[1], SubString[2]);
   }
   else
   	SetTime(1, 8, theTime.GetYear()); // Set to default date
}



/****************************************************************************\
\****************************************************************************/
void bsTime::AddOneDay(void)
{
   if(day < 28)
      day += 1;
   else
   {
      if(day < (tabDaysPerMonth[IsLeapYear(year)][month-1]))
         day += 1;
      else
      {
         day = 1;

         if(month < 12)
            month += 1;
         else
         {
            dayInYear = 0;
            month = 1;
            year += 1;
         }
      }
   }

   dayInYear++;
   dayInWeek++;
   if(dayInWeek > 7)
      dayInWeek -= 7;

   longTime++;
}

/****************************************************************************\
\****************************************************************************/
void bsTime::AddOneYear(void)
{
   year += 1;
   CalculateDateValue();
}




/****************************************************************************\
\****************************************************************************/
void bsTime::CalculateDateValue()
{
	if(year==1900)
		longTime=0;
	else if(year<1900)
	{
		cout<<"bsTime::CalculateDateValue: Year "<<year<<" is not supported"<<endl;
	}
	else
		longTime = AccumulatedDaysSince1900[year - 1900 - 1];
   for (int i = 1; i < month; i++)
   	longTime += tabDaysPerMonth[IsLeapYear(year)][i-1];
   longTime += day;
}

/****************************************************************************\
\****************************************************************************/
bool bsTime::operator == (bsTime time2)
{
   return (year==time2.year && month==time2.month && day==time2.day);
}

/****************************************************************************\
\****************************************************************************/
bool bsTime::operator < (bsTime time2)
{
   return (longTime<time2.longTime);
}

/****************************************************************************\
\****************************************************************************/
bool bsTime::operator <= (bsTime time2)
{
	return (longTime <= time2.longTime);
}

/****************************************************************************\
\****************************************************************************/
bool bsTime::operator > (bsTime time2)
{
   return (longTime > time2.longTime);
}

/****************************************************************************\
\****************************************************************************/
bool bsTime::operator >= (bsTime time2)
{
   return (longTime >= time2.longTime);
}

/****************************************************************************\
\****************************************************************************/
int bsTime::operator - (bsTime time2)
{
   return (longTime - time2.longTime);
}

/****************************************************************************\
\****************************************************************************/
ostream& operator<< (ostream& os, const bsTime& aTime)
{
	os << setiosflags(ios::right)
      << setw(2) << aTime.day << "/"
      << setw(2) << aTime.month << "/"
      << setw(2) << aTime.year<<setiosflags(ios::left);
   return os;
}



/****************************************************************************\
\****************************************************************************/
string bsTime::GetString2()
{
	string returnString = "";
   char string1[10];
   itoa(day,string1,10);
   if (day<10)
      returnString +="0";
   returnString += (string) string1 + "-";
   itoa(month,string1,10);
   if (month<10)
      returnString +="0";
   returnString += (string) string1 + "-";
   itoa(year,string1,10);
   returnString += string1;
   return returnString;
}

/****************************************************************************\
\****************************************************************************/
bool	bsTime::IsLeapYear(int year)
{
	if (((year%4==0) && (year%100!=0)) || (year%400==0))
   	return true;
 	else
   	return false;
}

/****************************************************************************\
Returns the number of days in a year. offset = 0 for this year, = 1 for next year
Use offset = 1 if using this function to plan ahead
\****************************************************************************/
int bsTime::daysInYear(int offset)
{
	if (IsLeapYear(year+offset))
   	return 366;
 	else
		return 365;
}

/****************************************************************************\
\****************************************************************************/
bsTime* bsTime::clone() const
{
	bsTime * someTime = new bsTime(*this);
   return someTime;
}


/****************************************************************************\
\****************************************************************************/
void bsTime::AddDays(int nrOfDays)
{
	for (int i=0;i<nrOfDays;i++)
      AddOneDay();
}

/****************************************************************************\
\****************************************************************************/
int bsTime::GetJulianDay(int aday, int amonth, int ayear)
{
	int ret_val=0;
	for (int i=0;i<amonth-1;i++)
   {
   	if (!IsLeapYear(ayear))
         ret_val +=tabDaysPerMonth[0][i];
		else
         ret_val +=tabDaysPerMonth[1][i];
   }
   ret_val+=aday;
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
int bsTime::GetJulianDay()
{
   return GetJulianDay(day,month,year);
}

