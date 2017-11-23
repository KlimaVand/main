/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "fallow.h"

static_fallow::static_fallow()
{
	strcpy(cropName,"FLLW");
}

/****************************************************************************\
\****************************************************************************/
void static_fallow::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * static_fallow::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	aPlantItem1=new plantItem;
   aPlantItem1->Setname("RYEGRASS");
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"F1");
	ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
      { // Ploughing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(Ploughing,crop_id,cropName,PP->GetStartYear(),9,1,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // Seedbed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear(),9,2,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // Sowing
         sowFields * FO=new sowFields;
         seed * aSeed=new seed;
         aSeed->Setname("SEEDRYEGRASS");
         theProducts->GiveProductInformation(aSeed);
         aSeed->Setamount(aSeed->GetStandardDensity());     // 	25;
         FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear(),9,3,PP->GetArea(),0,PP->GetDistance(),PP->GetPVStrat(),reduction,aSeed);
         AddOpList(FO);
         delete aSeed;
      }
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

int static_fallow::NumOfProtectStrat_()
{
	return 1;
}

int static_fallow::NitrogenLevels_()
{
	return 1;
}

int static_fallow::FallowCrop()
{
	return 1;
}

double static_fallow::ExtraNRelease_()
{
	return 0.0;
}

