/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropSeedgrass.h"
#include "../../products/products.h"
/****************************************************************************\
\****************************************************************************/
cropSeedGrass::cropSeedGrass(const char * aName, const int aIndex, const base * aOwner, string cropName)
              :cropRyegrass(aName,aIndex,aOwner, cropName)
{
   // For complete nomenclature see base class 'crop'

   PlantItemName  = "GRASSSEED";
}

/****************************************************************************\
\****************************************************************************/
void cropSeedGrass::Harvest(decomposable* Storage, decomposable* Straw)
{
//   GrazeOrCut(Straw,0,false); @@@
	Storage->Setname(PlantItemName);

	Straw->Setname(StrawItemName);
	theProducts->GiveProductInformation(Storage);
	theProducts->GiveProductInformation(Straw);

   topMatter          = NULL;
//   rootMatter         = NULL;
   double fraction = 0.3;                               // a harvest index of 30% REFINE!??????????!

   Storage->Setamount(Straw->GetAmount()*fraction);
   Straw->Setamount(Straw->GetAmount()*(1.0-fraction));

   double NRemain, DMRemain;
   EndBudget(&NRemain,&DMRemain);
}


