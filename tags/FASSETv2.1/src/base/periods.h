/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class: periods
(c) Bjørn Molt Petersen, Danmarks JordbrugsForskning
E-mail: bpe@pvf.sp.dk
\****************************************************************************/

#ifndef __PERIODS_H
	#define __PERIODS_H
#include "period.h"

class periods
{
	private:
		typedef char string80[80];

		linkList<period> * PeriodList;
		int NumOfPer;

		periods& operator=(const periods& p); // Dissable the compilers generation of default assignment operator.
		periods(const periods& p);            // Dissable the compilers generation of default copy constructor.

	public:
		periods();
		~periods();
		void Initialize(char * filename);
		int NumOfPeriods(){return NumOfPer;}
		void GetPeriodData(int num,const char * &Name,int &firstMonth,int &firstDay,int &lastMonth,int &lastDay);
		period * GetPeriod(int num);
      period * GetActualPeriod(const int month,const int day);
      int GetActualPeriodNumber(const int month,const int day);

};

#endif

