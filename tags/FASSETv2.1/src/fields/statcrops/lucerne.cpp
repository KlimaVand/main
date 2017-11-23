#include "common.h"
#include "lucerne.h"

/****************************************************************************\
\****************************************************************************/
static_lucerne_estab::static_lucerne_estab()
{
	strcpy(cropName,"LUCERNE");
}

/****************************************************************************\
\****************************************************************************/
void static_lucerne_estab::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * static_lucerne_estab::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	aPlantItem1=new plantItem;
  	aPlantItem1->Setname(cropName);
	aPlantItem2=new plantItem;
  	aPlantItem2->Setname("LUCERNESTRAW");
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"L2");
   ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan - three cuts
      	theMessage->FatalError("static_lucerne_estab: automatic plan for lucerne not implemented");

      {
         plantItem * aPlantItem=new plantItem;
         aPlantItem->Setname("LUCERNE");
         theProducts->GiveProductInformation(aPlantItem);
         aPlantItem->Setamount(ExpectedYield_(JB_NO,PP->GetNLevel(),PP->GetPVStrat()));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(CutSetAside,crop_id,cropName,PP->GetStartYear()+1,6,20,PP->GetArea(),0,PP->GetDistance(),reduction,true);
         FO->SetMainProduct(aPlantItem);
         AddOpList(FO);
         delete aPlantItem;
      }
      {
         plantItem * aPlantItem=new plantItem;
         aPlantItem->Setname("LUCERNE");
         theProducts->GiveProductInformation(aPlantItem);
         aPlantItem->Setamount(ExpectedYield_(JB_NO,PP->GetNLevel(),PP->GetPVStrat()));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(CutSetAside,crop_id,cropName,PP->GetStartYear()+1,7,26,PP->GetArea(),0,PP->GetDistance(),reduction,true);
         FO->SetMainProduct(aPlantItem);
         AddOpList(FO);
         delete aPlantItem;
      }
      {
         plantItem * aPlantItem=new plantItem;
         aPlantItem->Setname("LUCERNE");
         theProducts->GiveProductInformation(aPlantItem);
         aPlantItem->Setamount(ExpectedYield_(JB_NO,PP->GetNLevel(),PP->GetPVStrat()));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(CutSetAside,crop_id,cropName,PP->GetStartYear()+1,9,12,PP->GetArea(),0,PP->GetDistance(),reduction,true);
         FO->SetMainProduct(aPlantItem);
         AddOpList(FO);
         delete aPlantItem;
      }
   }
   delete aPlantItem1;
   delete aPlantItem2;
 	return LP;
}

/****************************************************************************\
\****************************************************************************/
int static_lucerne_estab::NumOfProtectStrat_()
{
	return 1;
}

/****************************************************************************\
\****************************************************************************/
int static_lucerne_estab::NitrogenLevels_()
{
	return 1;
}

/****************************************************************************\
\****************************************************************************/
int static_lucerne_estab::FallowCrop()
{
	return 1;
}

/****************************************************************************\
\****************************************************************************/
double static_lucerne_estab::ExtraNRelease_()
{
	return 0.0;
}

