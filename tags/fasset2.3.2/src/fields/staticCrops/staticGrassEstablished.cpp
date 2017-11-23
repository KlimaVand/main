/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticGrassEstablished.h"
#include "../fieldOperation/makeGrazable.h"
#include "../../base/controlParameters.h"

/****************************************************************************\
\****************************************************************************/
staticGrassEstablished::staticGrassEstablished()
{
	strcpy(cropName,"GRASS");
}

/****************************************************************************\
\****************************************************************************/
void staticGrassEstablished::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   theProducts->GiveProductInformation(aPlantItem1);
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
   for (int i=0;i<PP->GetNumOfHarvestDates();i++)
	{
      string * date=PP->GetHarvestDate(i);
      aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
      harvestFields * FO=new harvestFields;
      FO->DefineHarvestFieldsOper(ForageHarvesting,crop_id,cropName,PP->GetYear(date),PP->GetMonth(date),PP->GetDay(date),PP->GetArea(),0,PP->GetDistance(),1.0,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
      FO->SetForageProduct(aPlantItem1);
      AddOpList(FO);
   }

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
cloneList<fieldOperationFields> * staticGrassEstablished::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
   aPlantItem1=new plantItem;
   aPlantItem1->Setname("RYEGRASS");
   aPlantItem2=new plantItem;
   aPlantItem2->Setname("RYEGRASS");
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"G2");
	ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   {
      if (PP->GetFluidManureSpring()>0)
      { // Apply fluid manure
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
      { // Apply mineral fertilizer for first time
         double Napply=PP->GetN()/3;
         fertilizer * aFertilizer1=new fertilizer;
         aFertilizer1->Setname("MINERAL-N");
         theProducts->GiveProductInformation(aFertilizer1);
         aFertilizer1->Setamount(Napply/aFertilizer1->GetN_content().n);
         fertilizer * aFertilizer2=new fertilizer;
         aFertilizer2->Setname("MINERAL-P");
         theProducts->GiveProductInformation(aFertilizer2);
         aFertilizer2->Setamount(PP->GetP()/aFertilizer2->GetP_content());
         fertilizer * aFertilizer3=new fertilizer;
         aFertilizer3->Setname("MINERAL-K");
         theProducts->GiveProductInformation(aFertilizer3);
         aFertilizer3->Setamount(PP->GetK()/aFertilizer3->GetK_content());
         mineralFertFields * FO=new mineralFertFields;
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,4,1,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      if ((PP->GetN()>0) || (PP->GetP()>0) || (PP->GetK()>0))
      { // Apply mineral fertilizer for second time
         double Napply=PP->GetN()/3;
         fertilizer * aFertilizer1=new fertilizer;
         aFertilizer1->Setname("MINERAL-N");
         theProducts->GiveProductInformation(aFertilizer1);
         aFertilizer1->Setamount(Napply/aFertilizer1->GetN_content().n);
         fertilizer * aFertilizer2=new fertilizer;
         aFertilizer2->Setname("MINERAL-P");
         theProducts->GiveProductInformation(aFertilizer2);
         aFertilizer2->Setamount(PP->GetP()/aFertilizer2->GetP_content());
         fertilizer * aFertilizer3=new fertilizer;
         aFertilizer3->Setname("MINERAL-K");
         theProducts->GiveProductInformation(aFertilizer3);
         aFertilizer3->Setamount(PP->GetK()/aFertilizer3->GetK_content());
         mineralFertFields * FO=new mineralFertFields;
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,5,15,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      if ((PP->GetN()>0) || (PP->GetP()>0) || (PP->GetK()>0))
      { // Apply mineral fertilizer for third and last time
         double Napply=PP->GetN()/3;
         fertilizer * aFertilizer1=new fertilizer;
         aFertilizer1->Setname("MINERAL-N");
         theProducts->GiveProductInformation(aFertilizer1);
         aFertilizer1->Setamount(Napply/aFertilizer1->GetN_content().n);
         fertilizer * aFertilizer2=new fertilizer;
         aFertilizer2->Setname("MINERAL-P");
         theProducts->GiveProductInformation(aFertilizer2);
         aFertilizer2->Setamount(PP->GetP()/aFertilizer2->GetP_content());
         fertilizer * aFertilizer3=new fertilizer;
         aFertilizer3->Setname("MINERAL-K");
         theProducts->GiveProductInformation(aFertilizer3);
         aFertilizer3->Setamount(PP->GetK()/aFertilizer3->GetK_content());
         mineralFertFields * FO=new mineralFertFields;
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,7,1,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
//		if (PP->Getgrazable())
      {
         makeGrazable * FO=new makeGrazable();
         FO->DefinemakeGrazableOper(MakeGrazable,crop_id,cropName,PP->GetStartYear(),8,1,PP->GetArea(),0,0.0,true);
         AddOpList(FO);
         FO=new makeGrazable();
         FO->DefinemakeGrazableOper(MakeGrazable,crop_id,cropName,PP->GetStartYear()+1,7,31,PP->GetArea(),0,0.0,false);
         AddOpList(FO);
      }
   }
   delete aPlantItem1;
   return LP;
}

