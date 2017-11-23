/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "soilTreatTech.h"
#include "../products/energy.h"
#include "../products/products.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
soilTreatTech::soilTreatTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   k = 0;
   firstFurrow = 0.0;
   markHeadland = 0.0;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
soilTreatTech::~soilTreatTech()
{
}

/****************************************************************************\
\****************************************************************************/
void soilTreatTech::ReadParameters(commonData * &file)
{
   fieldOperationTech::ReadParameters(file);

   file->FindItem("firstFurrow",&firstFurrow);
   file->FindItem("markHeadland",&markHeadland);
   file->FindItem("k",&k);

}



/****************************************************************************\
\****************************************************************************/
void soilTreatTech::UpdateProducts(bool contractor)
{
   fieldOperationTech::UpdateProducts(contractor);
}


