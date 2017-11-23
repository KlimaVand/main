/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "../../products/manure.h"
#include "staticBarleyPea.h"

staticBarleyPea::staticBarleyPea()
{
	strcpy(cropName,"BARLEYPEA");
}

double staticBarleyPea::PVstratFactor(int PV_strat)
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
void staticBarleyPea::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
}

/****************************************************************************\
\****************************************************************************/

cloneList<fieldOperationFields> * staticBarleyPea::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	aPlantItem1=new plantItem;
   aPlantItem1->Setname("BARLEYPEA");
	aPlantItem2=new plantItem;
   aPlantItem2->Setname("BARLEYPEASTRAW");
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"BP");
   ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
   	{ // Ploughing
			soilTreatFields * FO=new soilTreatFields;
			FO->DefineOp(Ploughing,cropName,cropName,PP->GetStartYear()+1,3,28,PP->GetArea(),0,PP->GetDistance());
			AddOpList(FO);
		}
		if ((PP->GetK()>0) || (PP->GetP()>0)) { // Spread mineral fertilizer
			fertilizer * aFertilizer1=new fertilizer;
			aFertilizer1->Setname("MINERAL-P");
			theProducts->GiveProductInformation(aFertilizer1);
			aFertilizer1->Setamount(PP->GetP()/aFertilizer1->GetP_content());
				fertilizer * aFertilizer2=new fertilizer;
			aFertilizer2->Setname("MINERAL-K");
			theProducts->GiveProductInformation(aFertilizer2);
			aFertilizer2->Setamount(PP->GetK()/aFertilizer2->GetK_content());
			mineralFertFields * FO=new mineralFertFields;
			FO->DefineMineralFertFieldsOp(MineralFertilizing,cropName,cropName,PP->GetStartYear()+1,3,29,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,NULL);
			AddOpList(FO);
			delete aFertilizer1;
			delete aFertilizer2;
		}
		{ // 1st seed bed harrowing
			soilTreatFields * FO=new soilTreatFields;
			FO->DefineOp(SeedbedHarrowing,cropName,cropName,PP->GetStartYear()+1,3,30,PP->GetArea(),0,PP->GetDistance());
			AddOpList(FO);
		}
		if (JB_NO>4) { // 2nd seed bed harrowing on clayish soil
			soilTreatFields * FO=new soilTreatFields;
			FO->DefineOp(SeedbedHarrowing,cropName,cropName,PP->GetStartYear()+1,3,31,PP->GetArea(),0,PP->GetDistance());
			AddOpList(FO);
		}
	   if (PP->GetFluidManureSpring()>0) { // Apply FLUID manure
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

		{ // Sowing
			sowFields * FO = new sowFields;
			seed * aSeed1  = new seed;
         seed * aSeed2  = new seed;
			aSeed1->Setname("SEEDPEA");
			theProducts->GiveProductInformation(aSeed1);
			aSeed1->Setamount(155);
			aSeed2->Setname("SEEDSPBARL");
			theProducts->GiveProductInformation(aSeed2);
			aSeed2->Setamount(83);
	      FO->DefineSowFieldsOp(Sowing,cropName,cropName,PP->GetStartYear()+1,4,1,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed1);
         FO->AddSeed(aSeed2,"B1");
			AddOpList(FO);
			delete aSeed1;
	      delete aSeed2;
		}

		if (JB_NO<=4 && irrigation)
      { // Irrigating 30 mm on sandy soil
			water * aWater=new water;
			aWater->Setname("WATER");
			theProducts->GiveProductInformation(aWater);
			aWater->Setamount(30*10); // 30mm, 1ha
			irrigateFields * FO=new irrigateFields;
			FO->DefineIrrigateFieldsOp(Irrigating,cropName,cropName,PP->GetStartYear()+1,6,20,PP->GetArea(),0,PP->GetDistance(),aWater,true);
			AddOpList(FO);
			delete aWater;
		}
		{ // Crop monitoring - hours for the spring & summer
			manualLabourFields * FO=new manualLabourFields;
			double HoursPerUnit=0.7;

			FO->DefineManualLabourFieldsOp(MonitoringCrop,cropName,cropName,PP->GetStartYear()+1,7,30,PP->GetArea(),0,PP->GetDistance(),0,HoursPerUnit);
			AddOpList(FO);
		}
		{ // Combine harvesting
			theProducts->GiveProductInformation(aPlantItem1);
			aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
			theProducts->GiveProductInformation(aPlantItem2);
			aPlantItem2->Setamount(ExpectedStrawYield_(JB_NO,0,2));
			harvestFields * FO=new harvestFields;
			FO->DefineHarvestFieldsOper(CombineHarvestingShred,cropName,cropName,PP->GetStartYear()+1,8,15,PP->GetArea(),0,PP->GetDistance(),reduction,false,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         FO->SetMainProduct(aPlantItem1);
         FO->SetStrawProduct(aPlantItem2);
         AddOpList(FO);
		}
		{ // Stub harrowing
			soilTreatFields * FO=new soilTreatFields;
			FO->DefineOp(StubHarrowing,cropName,cropName,PP->GetStartYear()+1,8,16,PP->GetArea(),0,PP->GetDistance());
			AddOpList(FO);
		}
   }
   delete aPlantItem1;
   delete aPlantItem2;
   return LP;
}


double staticBarleyPea::NitrogenAmount(int JB_NO,double free_N,int N_level)
{
   #ifdef DEVELOPING
   JB_NO=JB_NO;
   free_N=free_N;
   N_level=N_level;
   #endif
	return 0;
}

double staticBarleyPea::ExpectedYield_(int JB_NO,int N_level,int PV_strat)
{
   #ifdef DEVELOPING
   N_level=N_level;
   #endif
	double factor=PVstratFactor(PV_strat);
	if (JB_NO>4) // Clayish soil
		return factor*4.5;
	else {            // Sandy soil
		if (irrigation)
         return factor*3.5;
		else
         return factor*2.0;
	}
}

