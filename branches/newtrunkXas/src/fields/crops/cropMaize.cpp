/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropMaize.h"
#include "../../products/products.h"

cropMaize::cropMaize(const char * aName, const int aIndex, const base * aOwner, string cropName)
          :crop(aName,aIndex,aOwner, cropName)
{
	// Parameters

   PlantItemName = "MAIZE";           // These names map to products.dat !
   StrawItemName = "MAIZESTRAW";      // These names map to products.dat !
	WinterSeed = false;
	C4Photosynthesis = true;

}
void cropMaize::Cut(plantItem * cutPlantMaterial, double cut_height)
{
	plantItem * Storage=new plantItem();
	plantItem * Straw=new plantItem();
	Harvest(Storage,Straw);
	*cutPlantMaterial=*Storage+*Straw;
}
/****************************************************************************\
\****************************************************************************/
/*double cropMaize::fTW()
{
   double res = 1.0-0.0025*(temp-26.0)*(temp-26.0);    // Response to DM production from CERES.
   res = max(0.0,min(1.0,res));
   return res;
} */

