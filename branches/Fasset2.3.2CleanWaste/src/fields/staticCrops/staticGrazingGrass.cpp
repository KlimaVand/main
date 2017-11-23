/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticGrazingGrass.h"

/****************************************************************************\
\****************************************************************************/
staticGrazingGrass::staticGrazingGrass()
{
	strcpy(cropName,"GRASS");
}

/****************************************************************************\
\****************************************************************************/
void staticGrazingGrass::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticGrazingGrass::MakeOpList (fieldOrder * PP,int JB_NO,double reduction)
{
   #ifdef DEVELOPING
   reduction=reduction;
   #endif
	aPlantItem1=new plantItem;
   aPlantItem1->Setname("RYEGRASS");
	aPlantItem2=new plantItem;
   aPlantItem2->Setname("GRASSIL1");
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"G4");
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
      { // Apply mineral fertilizer
         fertilizer * aFertilizer1=new fertilizer;
         aFertilizer1->Setname("MINERAL-N");
         theProducts->GiveProductInformation(aFertilizer1);
         aFertilizer1->Setamount(PP->GetN());
         mineralFertFields * FO=new mineralFertFields;
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,4,3,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,NULL,NULL);
         AddOpList(FO);
         delete aFertilizer1;
      }
      { // Sowing
         sowFields * FO=new sowFields;
         seed * aSeed=new seed;
         aSeed->Setname("SEEDRYEGRASS");
         theProducts->GiveProductInformation(aSeed);
         aSeed->Setamount(aSeed->GetStandardDensity());
         FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear(),9,3,PP->GetArea(),0,PP->GetDistance(),2,1.0,aSeed);
         AddOpList(FO);
         delete aSeed;
      }
   }
   delete aPlantItem1;
   delete aPlantItem2;
   return LP;
}



