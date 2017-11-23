/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticGrass.h"
#include "../fieldOperation/makeGrazable.h"
#include "../../base/controlParameters.h"

/****************************************************************************\
\****************************************************************************/
staticGrass::staticGrass()
{
	strcpy(cropName,"GRASS");
}

/****************************************************************************\
\****************************************************************************/
void staticGrass::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);

   for (int i=0;i<PP->GetNumOfGrazingStartDates();i++)
	{
      string * date=PP->GetGrazingStartDate(i);
      makeGrazable * FO=new makeGrazable();
      FO->DefinemakeGrazableOper(MakeGrazable,crop_id,cropName,PP->GetYear(date),PP->GetMonth(date),PP->GetDay(date),0.0,0,0.0,true);
      AddOpList(FO);
   }
   for (int i=0;i<PP->GetNumOfGrazingStopDates();i++)
	{
      string * date=PP->GetGrazingStopDate(i);
      makeGrazable * FO=new makeGrazable();
      FO->DefinemakeGrazableOper(MakeGrazable,crop_id,cropName,PP->GetYear(date),PP->GetMonth(date),PP->GetDay(date),0.0,0,0.0,false);
      AddOpList(FO);
   }
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticGrass::MakeOpList (fieldOrder * PP,int JB_NO,double reduction)
{
   aPlantItem1=new plantItem;
   aPlantItem1->Setname("RYEGRASS");
   aPlantItem2=new plantItem;
	aPlantItem2->Setname("RYEGRASS");   // "SILAGE" previously !!!???
   char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"G1");
	ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
      /*
      This must be an error !!!???
      aPlantItem1=new plantItem;
      aPlantItem1->Setname("RYEGRASS");
      aPlantItem2=new plantItem;
      aPlantItem2->Setname("SILAGE");
      */
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
         aSeed->Setamount(aSeed->GetStandardDensity());
         FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear(),9,3,PP->GetArea(),0,PP->GetDistance(),2,1.0,aSeed);
         AddOpList(FO);
         delete aSeed;
      }
      if (PP->GetFluidManureSpring()>0) { // Apply fluid manure
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("SLURRY");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(PP->GetFluidManureSpring());
         animalFertFields * FO=new animalFertFields;
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,cropName,cropName,PP->GetStartYear()+1,3,16,PP->GetArea(),0,PP->GetDistance(),aManure);
         AddOpList(FO);
         delete aManure;
      }
      if ((PP->GetN()>0) || (PP->GetP()>0) || (PP->GetK()>0))
      { // Apply mineral fertilizer
         fertilizer * aFertilizer1=new fertilizer;
         aFertilizer1->Setname("MINERAL-N");
         theProducts->GiveProductInformation(aFertilizer1);
         aFertilizer1->Setamount(PP->GetN()/aFertilizer1->GetN_content().n);
         fertilizer * aFertilizer2=new fertilizer;
         aFertilizer2->Setname("MINERAL-P");
         theProducts->GiveProductInformation(aFertilizer2);
         aFertilizer2->Setamount(PP->GetP()/aFertilizer2->GetP_content());
         fertilizer * aFertilizer3=new fertilizer;
         aFertilizer3->Setname("MINERAL-K");
         theProducts->GiveProductInformation(aFertilizer3);
         aFertilizer3->Setamount(PP->GetK()/aFertilizer3->GetK_content());
         mineralFertFields * FO=new mineralFertFields;
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,4,15,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      {
         theProducts->GiveProductInformation(aPlantItem1);
         makeGrazable * FO=new makeGrazable();
         FO->DefinemakeGrazableOper(MakeGrazable,crop_id,cropName,PP->GetStartYear(),8,1,PP->GetArea(),0,0.0,true);
			FO->SetForageProduct(aPlantItem1);
         AddOpList(FO);
         FO=new makeGrazable();
         FO->DefinemakeGrazableOper(MakeGrazable,crop_id,cropName,PP->GetStartYear()+1,7,31,PP->GetArea(),0,0.0,false);
         AddOpList(FO);
      }
   }
   delete aPlantItem1;
   delete aPlantItem2;
   return LP;
}


