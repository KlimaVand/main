/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "taxLegislation.h"

/****************************************************************************\
  Default Constructor
\****************************************************************************/
taxLegislation::taxLegislation()
   : base()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments (1)
\****************************************************************************/
taxLegislation::taxLegislation(const char * aName, const int aIndex, const base * aOwner)
   : base(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments (2)
\****************************************************************************/
taxLegislation::taxLegislation(int aType, double aTax, int aStartYear, int aStopYear,
                               const char * aName, const int aIndex, const base * aOwner)
	                            : base(aName, aIndex, aOwner)
{
   InitVariables();
	type      = aType;
   tax       = aTax;
   startYear = aStartYear;
   stopYear  = aStopYear;
}

/****************************************************************************\
 Destructor
\****************************************************************************/
taxLegislation::~taxLegislation()
{
}

/****************************************************************************\
\****************************************************************************/
void taxLegislation::InitVariables()
{
	type      = 0;
   tax       = 0;
   startYear = 0;
   stopYear  = 0;
   level     = 0;
}

/****************************************************************************\
\****************************************************************************/
void taxLegislation::ReadParameters(fstream * file)
{
   SetCritical();
   Setfile(file);
   FindSection(Name,Index);
   GetParameter("type",&type);
	GetParameter("tax",&tax);
	GetParameter("startYear",&startYear);
   GetParameter("stopYear",&stopYear);
   GetParameter("level",&level);
   Setfile(NULL);
}


