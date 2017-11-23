/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "mineralFertTech.h"
#include "../products/energy.h"
#include "../products/nitrogen.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
mineralFertTech::mineralFertTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   prepLoading = 0.0;
   roadSpeed = 0.0;
   vol = 0.0;
   fillingTimeSpreader = 0.0;
   prepFilling = 0.0;
   changingTrailer = 0.0;
   loadingTime = 0.0;
   theFertilizer1 = nullptr;
   theFertilizer2 = nullptr;
   theFertilizer3 = nullptr;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
mineralFertTech::~mineralFertTech()
{
   if(theFertilizer1)
      delete theFertilizer1;
   if(theFertilizer2)
      delete theFertilizer2;
   if(theFertilizer3)
      delete theFertilizer3;
}

/****************************************************************************\
\****************************************************************************/
void mineralFertTech::ReadParameters(commonData * &file)
{
   fieldOperationTech::ReadParameters(file);

   file->FindItem("prepLoading",&prepLoading);
   file->FindItem("roadSpeed",&roadSpeed);
   file->FindItem("vol",&vol);
   file->FindItem("fillingTimeSpreader",&fillingTimeSpreader);
   file->FindItem("prepFilling",&prepFilling);
   file->FindItem("changingTrailer",&changingTrailer);
   file->FindItem("loadingTime",&loadingTime);

}

/****************************************************************************\
\****************************************************************************/
void mineralFertTech::SetTheFertilizers(fertilizer * aFertilizer1, fertilizer * aFertilizer2, fertilizer * aFertilizer3)
{
   if(aFertilizer1)
   {
      theFertilizer1 = new fertilizer;
      *theFertilizer1 = *aFertilizer1;
   }

   if(aFertilizer2)
   {
      theFertilizer2 = new fertilizer;
      *theFertilizer2 = *aFertilizer2;
   }

   if(aFertilizer3)
   {
      theFertilizer3 = new fertilizer;
      *theFertilizer3 = *aFertilizer3;
   }
}

/****************************************************************************\
\****************************************************************************/
void mineralFertTech::ClearTheFertilizers()
{
   if(theFertilizer1) {
      delete theFertilizer1;
      theFertilizer1=nullptr;
   }
   if(theFertilizer2) {
      delete theFertilizer2;
      theFertilizer2=nullptr;
   }
   if(theFertilizer3) {
      delete theFertilizer3;
      theFertilizer3=nullptr;
   }
}



/****************************************************************************\
\****************************************************************************/
void mineralFertTech::UpdateFertilizer(fertilizer* aFertilizer)
{
   (*aFertilizer)*area;
   theProducts->SubtractProduct(aFertilizer);

   nitrogen N_content = aFertilizer->GetN_content()*aFertilizer->GetAmount();
	double P_content = (aFertilizer->GetAmount())*(aFertilizer->GetP_content());
	double K_content = (aFertilizer->GetAmount())*(aFertilizer->GetK_content());
	theOutput->AddIndicator(3101,"31.01 N from mineral fertilizer","kg N",N_content.n);
	theOutput->AddIndicator(5001,"50.01 Phosphorus from mineral fertilizer","kg P",P_content);
	theOutput->AddIndicator(6001,"60.01 Potassium from mineral fertilizer","kg K",K_content);
}

void mineralFertTech::UpdateProducts(bool contractor)
{
   fieldOperationTech::UpdateProducts(contractor);

   if (theFertilizer1)
      UpdateFertilizer(theFertilizer1);
   if (theFertilizer2)
      UpdateFertilizer(theFertilizer2);
   if (theFertilizer3)
      UpdateFertilizer(theFertilizer3);
}


