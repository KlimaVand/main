/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include <fallow2.h>

/****************************************************************************\
\****************************************************************************/
static_fallow_perm::static_fallow_perm()
{
	strcpy(cropName,"FLLW");
}

/****************************************************************************\
\****************************************************************************/
void static_fallow_perm::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * static_fallow_perm::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	aPlantItem1=new plantItem;
   aPlantItem1->Setname("RYEGRASS");
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"F2");
	ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
      { // Afhugning
         theProducts->GiveProductInformation(aPlantItem1);
         aPlantItem1->Setamount(ExpectedYield_(JB_NO,PP->GetNLevel(),PP->GetPVStrat()));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(CutSetAside,crop_id,cropName,PP->GetStartYear()+1,7,14,PP->GetArea(),0,PP->GetDistance(),reduction,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         FO->SetForageProduct(aPlantItem1);
         AddOpList(FO);
      }
   }
   delete aPlantItem1;
	return LP;
}

/****************************************************************************\
\****************************************************************************/
int static_fallow_perm::NumOfProtectStrat_()
{
	return 1; 
}

/****************************************************************************\
\****************************************************************************/
int static_fallow_perm::NitrogenLevels_()
{
	return 1;
}

/****************************************************************************\
\****************************************************************************/
int static_fallow_perm::FallowCrop()
{
	return 1;
}

/****************************************************************************\
\****************************************************************************/
double static_fallow_perm::EkstraNRelease_()
{
	return 0.0;
}

