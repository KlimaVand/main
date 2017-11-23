/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticGrassPermanent.h"

staticGrassPermanent::staticGrassPermanent()
{
	strcpy(cropName,"GRASS");
}

cloneList<fieldOperationFields> * staticGrassPermanent::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"G3");
	ResetOpList();
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
		FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear(),9,3,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed);
		AddOpList(FO);
      delete aSeed;
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
	{ // Afhugning
		plantItem * aPlantItem=new plantItem;
		aPlantItem->Setname("RYEGRASS");
		theProducts->GiveProductInformation(aPlantItem);
		aPlantItem->Setamount(ExpectedYield_(JB_NO,0,2));
		harvestFields * FO=new harvestFields;
		FO->DefineHarvestFieldsOper(CutSetAside,crop_id,cropName,PP->GetStartYear()+1,7,14,PP->GetArea(),0,PP->GetDistance(),reduction,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
		FO->SetForageProduct(aPlantItem);
		AddOpList(FO);
		delete aPlantItem;
	}

   // second year

	{ // Apply mineral fertilizer
		fertilizer * aFertilizer1=new fertilizer;
		aFertilizer1->Setname("MINERAL-N");
		theProducts->GiveProductInformation(aFertilizer1);
		aFertilizer1->Setamount(PP->GetN());
		mineralFertFields * FO=new mineralFertFields;
		FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+2,4,3,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,NULL,NULL);
		AddOpList(FO);
		delete aFertilizer1;
	}
	{ // Afhugning
		plantItem * aPlantItem=new plantItem;
		aPlantItem->Setname("RYEGRASS");
		theProducts->GiveProductInformation(aPlantItem);
		aPlantItem->Setamount(ExpectedYield_(JB_NO,0,2));
		harvestFields * FO=new harvestFields;
		FO->DefineHarvestFieldsOper(CutSetAside,crop_id,cropName,PP->GetStartYear()+2,7,14,PP->GetArea(),0,PP->GetDistance(),reduction,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
		FO->SetForageProduct(aPlantItem);
		AddOpList(FO);
		delete aPlantItem;
	}
	return LP;
}

