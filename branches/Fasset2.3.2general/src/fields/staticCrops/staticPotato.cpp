/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticPotato.h"

/****************************************************************************\
\****************************************************************************/
staticPotato::staticPotato()
{
	strcpy(cropName,"POTATO");
}

/****************************************************************************\
\****************************************************************************/
double staticPotato::PVstratFactor(int PV_strat)
{
	double f=1.00;
	switch (PV_strat) {
		case 0: f=0.80;break;
		case 1: f=0.95;break;
		case 2: f=1.00;break;
	}
	return f;
}

/****************************************************************************\
\****************************************************************************/
void staticPotato::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
   if (PP->GetNumOfHarvestDates()>1)
      theMessage->FatalError("static_potato::SpecifiedPlan - more than one harvest date specified");
   int year=PP->GetStartYear()+1;
   int month=11;
   int day=2;
   if (PP->GetNumOfHarvestDates()>0)
   {
      string * date=PP->GetHarvestDate(0);
      year=PP->GetYear(date);
      month=PP->GetMonth(date);
      day=PP->GetDay(date);
   }
	theProducts->GiveProductInformation(aPlantItem1);
	theProducts->GiveProductInformation(aPlantItem2);
	aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
   aPlantItem2->Setamount(ExpectedStrawYield_(JB_NO,0,2));
	harvestFields * FO=new harvestFields;
	FO->DefineHarvestFieldsOper(BeetHarvesting,crop_id,cropName,year,month,day,PP->GetArea(),0,PP->GetDistance(),1.0,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
	FO->SetMainProduct(aPlantItem1);
   FO->SetStrawProduct(aPlantItem2);
   AddOpList(FO);
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticPotato::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
  	aPlantItem1=new plantItem;
  	aPlantItem1->Setname(cropName);
  	aPlantItem2=new plantItem;
  	aPlantItem2->Setname("POTATOTOP");
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"PO"); // ---"----
	const double MaxGylle = 30.0;
	ResetOpList();
   double fluid_HUG_fall=PP->GetFluidManureFall();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
      if (fluid_HUG_fall>0)
         theMessage->Warning("static_potato::MakeOpList - Fluid manure in fall not allowed for potatos");
      if (PP->GetSolidManure()>0)
      { // Spread solid manure
         animalFertFields * FO=new animalFertFields;
         manure * aManure=new manure;
         aManure->Setname("SOLID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(PP->GetSolidManure());
         FO->DefineAnimalFertFieldsOp(SpreadingSolidManure,crop_id,cropName,PP->GetStartYear()+1,3,27,PP->GetArea(),0,PP->GetDistance(),aManure);
         AddOpList(FO);
         delete aManure;
      }
      if (PP->GetFluidManureSpring()>0.0) // Apply fluid manure
      {
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("SLURRY");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         if (PP->GetFluidManureSpring()<=MaxGylle)
            aManure->Setamount(PP->GetFluidManureSpring());
         else
            aManure->Setamount(PP->GetFluidManureSpring()/2.0);
         animalFertFields * FO=new animalFertFields;
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear()+1,4,4,PP->GetArea(),0,PP->GetDistance(),aManure);
         AddOpList(FO);
         delete aManure;
      }
      { // Ploughing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(Ploughing,crop_id,cropName,PP->GetStartYear()+1,4,5,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // 1st seed bed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear()+1,4,6,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (JB_NO<=4) // 2nd seed bed harrowing (only clayish soils)
      {
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear()+1,4,7,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (JB_NO<=4) // 3. seed bed harrowing (only clayish soils)
      {
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear()+1,4,8,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // Sowing. Trivially wrong, as potatoes are planted, but works technically.
        // Correct if some day the field machinery comes into focus again !!!!!!
         sowFields * FO=new sowFields;
         seed * aSeed=new seed;
         aSeed->Setname("SEEDPOTATO");
         theProducts->GiveProductInformation(aSeed);
         aSeed->Setamount(aSeed->GetStandardDensity());
         FO->DefineSowFieldsOp(SowingBeets,crop_id,cropName,PP->GetStartYear()+1,4,20,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed);
         AddOpList(FO);
         delete aSeed;
      }
      if ((PP->GetN()>0) || (PP->GetP()>0) || (PP->GetK())) // Spread mineral fertilizer
      {
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
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,4,11,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      {  // "Row cultivating" 1. time
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(RowCultivating,crop_id,cropName,PP->GetStartYear()+1,5,15,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (2<=1) // Row cultivating 2. time
      {  // "Row cultivating" 2. time
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(RowCultivating,crop_id,cropName,PP->GetStartYear()+1,6,2,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (PP->GetFluidManureSpring()>MaxGylle) // Apply fluid manure 2. time
      {
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("SLURRY");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(PP->GetFluidManureSpring()/2.0);
         animalFertFields * FO=new animalFertFields;
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear()+1,5,31,PP->GetArea(),0,PP->GetDistance(),aManure);
         AddOpList(FO);
         delete aManure;
      }
      if (JB_NO<=4 && irrigation) { // Irrigate 30 mm on sandy soils
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(30*10); // 30mm, 1ha
         irrigateFields * FO=new irrigateFields;
         FO->DefineIrrigateFieldsOp(Irrigating,crop_id,cropName,PP->GetStartYear()+1,7,15,PP->GetArea(),0,PP->GetDistance(),aWater,true);
         AddOpList(FO);
         delete aWater;
      }
      { // Plantprotection registrations - hours for the whole season
         manualLabourFields * FO=new manualLabourFields;
         FO->DefineManualLabourFieldsOp(MonitoringCrop,crop_id,cropName,PP->GetStartYear()+1,10,1,PP->GetArea(),0,PP->GetDistance(),0,1.5);
         AddOpList(FO);
      }
      { // Harvesting potatos.
        // The wrong machinery is used - correct if some day the field machinery comes into focus again !!!
         plantItem * aPlantItem1=new plantItem;
         aPlantItem1->Setname("POTATO");
         theProducts->GiveProductInformation(aPlantItem1);
         aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
         plantItem * aPlantItem2=new plantItem;
         aPlantItem2->Setname("POTATOTOP");
         theProducts->GiveProductInformation(aPlantItem2);
         aPlantItem2->Setamount(ExpectedStrawYield_(JB_NO,0,2));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(CombineHarvestingShred,crop_id,cropName,PP->GetStartYear()+1,9,15,PP->GetArea(),0,PP->GetDistance(),reduction,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
			FO->SetMainProduct(aPlantItem1);
         FO->SetStrawProduct(aPlantItem2);
         AddOpList(FO);
      }
   }
   delete aPlantItem1;
   delete aPlantItem2;
	return LP;
}

/****************************************************************************\
\****************************************************************************/
double staticPotato::NitrogenAmount(int JB_NO,double free_N,int N_level)
{
	double N=Nopt(JB_NO);
	switch (N_level) {
		case 0: N=0;break;
		case 1: N=0.50*N;break;
		case 2: N=0.55*N;break;
		case 3: N=0.60*N;break;
		case 4: N=0.65*N;break;
		case 5: N=0.70*N;break;
		case 6: N=0.75*N;break;
		case 7: N=0.80*N;break;
		case 8: N=0.85*N;break;
		case 9: N=0.90*N;break;
		case 10:N=0.95*N;break;
		case 11:;         // Here the value of N is unchanged
	}
	N = N-free_N;
	if (N<0)
		N=0;
	return N;
}

/****************************************************************************\
Have to look into this later !!!!!!!!!!!!!
\****************************************************************************/
double staticPotato::Nopt(int JB_NO)
{
	if (JB_NO>4) // Clayish soil
		return 200.0;
	else {       // Sandy soil
		if (irrigation)
			return 180.0;
		else
			return 170.0;
	}
}

/****************************************************************************\
Yields in fresh weight. Based on 18% dry matter.
Have to look into this later !!!!!!!!!!!!!
\****************************************************************************/
double staticPotato::Ymin(int JB_NO)
{
	if (JB_NO>4) // Clayish soil
		return 28.0; // 5 t DM
	else {       // Sandy soil
		if (irrigation)
			return 22.0; // 4 t DM
		else
			return 17.0; // 3 t DM
	}
}

/****************************************************************************\
Yields in fresh weight. Based on 24% dry matter.
Have to look into this later !!!!!!!!!!!!!
\****************************************************************************/
double staticPotato::Ymax(int JB_NO)
{
	if (JB_NO>4) // Clayish soil
		return 61.0; // 11 t DM
	else {       // Sandy soil
		if (irrigation)
			return 50.0; // 9 t DM
		else
			return 39; // 7 t DM
	}
}

/****************************************************************************\
\****************************************************************************/
double staticPotato::ExpectedYield_(int JB_NO,int N_level,int PV_strat)
{
	double N_act=NitrogenAmount(JB_NO,0,N_level);
	double factor=PVstratFactor(PV_strat);
   if (N_act>Nopt(JB_NO))
      return factor*Ymax(JB_NO);
   else
		return factor*(Ymin(JB_NO)+(Ymax(JB_NO)-Ymin(JB_NO))*(2*N_act/Nopt(JB_NO)-(N_act/Nopt(JB_NO))*(N_act/Nopt(JB_NO))));
}




