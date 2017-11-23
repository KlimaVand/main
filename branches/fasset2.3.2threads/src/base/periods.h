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
#include "linklist.h"

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
		void Initialize(string filename);

      int GetActualPeriodNumber(const int month,const int day);

};

#endif

