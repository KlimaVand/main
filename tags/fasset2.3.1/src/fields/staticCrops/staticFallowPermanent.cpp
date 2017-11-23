/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticFallowPermanent.h"

/****************************************************************************\
\****************************************************************************/
staticFallowPermanent::staticFallowPermanent()
{
	strcpy(cropName,"FLLW");
}

/****************************************************************************\
\****************************************************************************/
void staticFallowPermanent::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticFallowPermanent::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
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
         aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(CutSetAside,crop_id,cropName,PP->GetStartYear()+1,7,14,PP->GetArea(),0,PP->GetDistance(),reduction,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         FO->SetForageProduct(aPlantItem1);
         AddOpList(FO);
      }
   }
   delete aPlantItem1;
	return LP;
}


