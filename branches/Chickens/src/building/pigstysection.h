/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
   Author Lars Kroll lokal 1875
     Date 21/5 97
 Revision
 ----------------------------------------------------------------------------
      Log
\****************************************************************************/

#ifndef PIGSTYSECTION_H
	#define PIGSTYSECTION_H

#include "../economy/genAsset.h"
#include "animalSection.h"

class pigstysection: public animalSection
{
	int DaysInSection;

public:
	pigstysection(char* aName,const int aIndex,const base* aOwner);
	virtual ~pigstysection();

public:

	double GetDaysInSection() {return DaysInSection;};
	double GetCapacity(double ProductionCyclus);
	void ReadParameters(fstream* file);

};

#endif
