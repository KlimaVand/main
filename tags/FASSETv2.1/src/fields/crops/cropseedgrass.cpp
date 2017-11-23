/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropseedgrass.h"

/****************************************************************************\
\****************************************************************************/
cropSeedGrass::cropSeedGrass(const char * aName, const int aIndex, const base * aOwner)
              :cropRyegrass(aName,aIndex,aOwner)
{
   // For complete nomenclature see base class 'crop'
   CropName       = "SeedGrass";
   PlantItemName  = "GRASSSEED";
}

/****************************************************************************\
\****************************************************************************/
void cropSeedGrass::Harvest(decomposable* Storage, decomposable* Straw)
{
//   GrazeOrCut(Straw,0,false); @@@

   topMatter          = NULL;
   rootMatter         = NULL;
   double fraction = 0.3;                               // a harvest index of 30% REFINE!??????????!
   *Storage = *Straw;
   Storage->Setamount(Straw->GetAmount()*fraction);
   Straw->Setamount(Straw->GetAmount()*(1.0-fraction));

   double NRemain, DMRemain;
   EndBudget(&NRemain,&DMRemain);
}


