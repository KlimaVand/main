/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "manualLabourTech.h"

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
void manualLabourTech::CalcGangHours()
{
   aGang->SetgangHours(hoursPerUnit*area);
}


