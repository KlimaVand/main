/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Kroll Kristensen
\****************************************************************************/

#ifndef __ECONOMIC_H
  #define __ECONOMIC_H

#include "../base/IndicatorOutput.h"
#include "../base/base.h"

/****************************************************************************\
 Class: economics
\****************************************************************************/
class economics: public base
{
   protected:
   	double CapitalInterest;

	public:
		// Constructor
		economics();
		// Constructor with arguments
		economics(const char * aName, const int aIndex, const base * aOwner);
  		// Copy Constructor
		economics(const economics& aeconomics);
		// Destructor
		~economics();
		// Operator for output
		friend ostream& operator << (ostream& os, const economics& aeconomics);
      void SetInterest(double aInterest) {CapitalInterest = aInterest;};
		void run();
      void Initialize();
      void ReceivePlan();
      void CalcLP();
      void GiveIndicator();
      void DailyUpdate();
};

#endif


