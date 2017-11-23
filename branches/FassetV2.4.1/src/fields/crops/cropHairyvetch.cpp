/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropHairyvetch.h"
#include "../../products/products.h"

/****************************************************************************\
\****************************************************************************/
cropHairyVetch::cropHairyVetch(const char * aName, const int aIndex, const base * aOwner, string cropName)
              :cropPea(aName,aIndex,aOwner, cropName)
{
   // For complete nomenclature see base class 'crop'

   PlantItemName  = "HAIRYVETCH";                 // no primarly harvest item
   StrawItemName  = "HAIRYVETCH";           // These names maps to products.dat !
	WinterSeed     = true;                     // Is the present crop a wintercrop.
   GrowthContinue = false;



}
/****************************************************************************\
\****************************************************************************/
double cropHairyVetch::Update(double ActivePar)
{
  	double DailyDMProd = cropPea::Update(ActivePar);
   return DailyDMProd;
}


