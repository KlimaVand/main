/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "seedgrass.h"
/****************************************************************************\
Grass for seed. This is currently only partly implemented
\****************************************************************************/

/****************************************************************************\
\****************************************************************************/
void static_seedgrass::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
   if (PP->GetNumOfHarvestDates()>1)
      theMessage->FatalError("static_seedgrass::SpecifiedPlan - more than one harvest date specified");
   delete aPlantItem1;
   delete aPlantItem2;
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * static_seedgrass::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	aPlantItem1 = new plantItem;
   aPlantItem1->Setname("GRASSSEED");                   // grains
	aPlantItem2 = new plantItem;
   aPlantItem2->Setname("GRASSSEEDSTRAW");
	ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,PP->GetCropId());
   else
   	theMessage->FatalError("static_seedgrass::MakeOpList - automatic plan not implemented");
   return LP;
}

/****************************************************************************\
\****************************************************************************/
double static_seedgrass::ExpectedYield_(int JB_NO,int N_level,int PV_strat)
{
   return 1.0;
}

