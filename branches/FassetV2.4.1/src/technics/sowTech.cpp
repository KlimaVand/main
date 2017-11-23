/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "sowTech.h"
#include "../products/energy.h"
#include "../products/products.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
sowTech::sowTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   prepFillSowing = 0.0;
   fillingTime1Sowing = 0.0;
   fillingTime2Sowing = 0.0;
   fillingAmount1 = 0.0;
   fillingAmount2 = 0.0;
   roadSpeed = 0.0;
   changingTrailer = 0.0;
   loadingTime = 0.0;
   prepLoading = 0.0;
   numOfSeeds=0;
   for (int i=0;i<MaxPlants; i++)
   	theSeeds[i] = nullptr;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
sowTech::~sowTech()
{
    for (int i=0;i<MaxPlants; i++)
   	if (theSeeds[i])
         delete theSeeds[i];
}

/****************************************************************************\
\****************************************************************************/
void sowTech::ReadParameters(commonData * &file)
{
   fieldOperationTech::ReadParameters(file);

   file->FindItem("prepFillSowing",&prepFillSowing);
   file->FindItem("fillingTime1Sowing",&fillingTime1Sowing);
   file->FindItem("fillingTime2Sowing",&fillingTime2Sowing);
   file->FindItem("fillingAmount1",&fillingAmount1);
   file->FindItem("fillingAmount2",&fillingAmount2);
   file->FindItem("roadSpeed",&roadSpeed);
   file->FindItem("changingTrailer",&changingTrailer);
   file->FindItem("prepLoading",&prepLoading);
   file->FindItem("loadingTime",&loadingTime);

}

/****************************************************************************\
\****************************************************************************/
void sowTech::AddSeed(seed * s)
{
   theSeeds[numOfSeeds] = new seed(*s);
   numOfSeeds++;
}

/****************************************************************************\
\****************************************************************************/
void sowTech::ClearTheSeeds()
{
   for (int i=0;i<MaxPlants;i++)
   {
   	if (theSeeds[i])
         delete theSeeds[i];
      theSeeds[i] = nullptr;
   }
}



/****************************************************************************\
\****************************************************************************/
void sowTech::UpdateProducts(bool contractor)
{
   fieldOperationTech::UpdateProducts(contractor);

   for (int i=0;i<MaxPlants;i++)
   	if (theSeeds[i])
		{
      	theSeeds[i]->Setamount(theSeeds[i]->GetAmount()*area);
   		theProducts->SubtractProduct(theSeeds[i]);
      }
}


