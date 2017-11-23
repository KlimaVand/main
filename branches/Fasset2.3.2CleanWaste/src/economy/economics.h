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


      void GiveIndicator();

};

#endif


