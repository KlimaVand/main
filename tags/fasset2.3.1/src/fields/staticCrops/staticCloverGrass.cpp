/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "../fieldOperation/silageFields.h"
#include "staticCloverGrass.h"

/****************************************************************************\
\****************************************************************************/
staticCloverGrass::staticCloverGrass(bool permanentGrass)
{
   permanent = permanentGrass;
	strcpy(cropName,"CLOVERGRASS");
}

/****************************************************************************\
\****************************************************************************/
double staticCloverGrass::PVstratFactor(int PV_strat)
{
	double f=1.00;
	switch (PV_strat) {
		case 0: f=0.80;break;
		case 1: f=0.93;break;
		case 2: f=1.00;break;
	}
	return f;
}

/****************************************************************************\
\****************************************************************************/
void staticCloverGrass::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticCloverGrass::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	aPlantItem1=new plantItem;
   aPlantItem1->Setname("RYEGRASS");
  	char crop_id[3];      // TEMPORARY
   if (!permanent)
   	strcpy(crop_id,"C1");
   else
   	strcpy(crop_id,"C2");
	ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
      if (!permanent)
      {
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
            sowFields * FO = new sowFields;
            seed * aSeed1  = new seed;
            seed * aSeed2  = new seed;
            aSeed1->Setname("SEEDRYEGRASS");
            theProducts->GiveProductInformation(aSeed1);
            aSeed1->Setamount(12.5);
            aSeed2->Setname("SEEDCLOVER");
            theProducts->GiveProductInformation(aSeed2);
            aSeed2->Setamount(12.5);
            FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear(),9,3,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed1);
				FO->AddSeed(aSeed2,"K1");
            AddOpList(FO);
            delete aSeed1;
            delete aSeed2;
         }
      }
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
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear()+1,3,16,PP->GetArea(),0,PP->GetDistance(),aManure);
         AddOpList(FO);
         delete aManure;
      }
      //ensure no grazing at end of year and over winter
		//first harvest
      {
         plantItem * aPlantItem=new plantItem;
         aPlantItem->Setname("RYEGRASS");
         theProducts->GiveProductInformation(aPlantItem);
         aPlantItem->Setamount(ExpectedYield_(JB_NO,0,2));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(ForageHarvesting,crop_id,cropName,PP->GetStartYear()+1,6,1,PP->GetArea(),0,PP->GetDistance(),1.0,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
			FO->SetForageProduct(aPlantItem);
         AddOpList(FO);
         delete aPlantItem;
      }
      //second harvest
      {
         plantItem * aPlantItem=new plantItem;
         aPlantItem->Setname("RYEGRASS");
         theProducts->GiveProductInformation(aPlantItem);
         aPlantItem->Setamount(ExpectedYield_(JB_NO,0,2));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(ForageHarvesting,crop_id,cropName,PP->GetStartYear()+1,7,15,PP->GetArea(),0,PP->GetDistance(),1.0,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         FO->SetForageProduct(aPlantItem);
         AddOpList(FO);
         delete aPlantItem;
      }
      //third harvest
      {
         plantItem * aPlantItem=new plantItem;
         aPlantItem->Setname("RYEGRASS");
         theProducts->GiveProductInformation(aPlantItem);
         aPlantItem->Setamount(ExpectedYield_(JB_NO,0,2));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(ForageHarvesting,crop_id,cropName,PP->GetStartYear()+1,9,5,PP->GetArea(),0,PP->GetDistance(),1.0,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         FO->SetForageProduct(aPlantItem);
         AddOpList(FO);
         delete aPlantItem;
      }
	}
   delete aPlantItem1;
	return LP;
}


/****************************************************************************\
\****************************************************************************/
double staticCloverGrass::NitrogenAmount(int JB_NO,double free_N,int N_level)
{
	return 0;
}

/****************************************************************************\
\****************************************************************************/
double staticCloverGrass::ExpectedYield_(int JB_NO,int N_level,int PV_strat)
{
	double factor=PVstratFactor(PV_strat);
	if (JB_NO>4) // Clayish soil
		return factor*4.5;                                  // FORKERTE V�RDIER RET!!!
	else {            // Sandy soil
		if (irrigation)
         return factor*3.5;
		else
         return factor*2.0;
	}
}


