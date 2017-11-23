/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __TAXLEGIS_H
  #define __TAXLEGIS_H

#include "../base/base.h"

class taxLegislation: public base
{
	protected:
   	int type;          // 0: 1: 2: 3:
      double tax;        // the tax
      double level;
      int startYear;     // the first simulation year the tax should be applied
      int stopYear;      // the last simulation year the tax should be applied

      void InitVariables();

	public:
		// Default Constructor
		taxLegislation();
		// Constructor with arguments (1)
      taxLegislation(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
		// Constructor with arguments (2)
      taxLegislation(int aType, double tax, int startYear, int stopYear,
                     const char * aName, const int aIndex = -1, const base * aOwner = NULL);
		// Destructor
		~taxLegislation();
		// Set and Get functions
		bool taxThisYear(int simulationYear) {return ((simulationYear>=startYear) && (simulationYear<=stopYear));}
      double getTax() {return tax;};
      double getLevel() {return level;};
      int getType() {return type;};
      void ReadParameters(fstream * file);
};

#endif

