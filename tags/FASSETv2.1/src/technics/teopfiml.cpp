/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "teopfiml.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
manualLabourTech::manualLabourTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   hoursPerUnit = 0.0;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
manualLabourTech::~manualLabourTech()
{
}

/****************************************************************************\
\****************************************************************************/
void manualLabourTech::ReadParameters(fstream * file)
{
   fieldOperationTech::ReadParameters(file);
   Setfile(file);
   Setfile(NULL);
}

/****************************************************************************\
\****************************************************************************/
void manualLabourTech::CalcCost(calcLPType mode)
{
   CalcGangHours();
   cost=aGang->GetgangHours();
   #ifdef DEVELOPING
      mode=mode;
   #endif
}

/****************************************************************************\
\****************************************************************************/
void manualLabourTech::CalcGangHours()
{
   aGang->SetgangHours(hoursPerUnit*area);
}


