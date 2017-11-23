/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticSrape.h"


/****************************************************************************\
\****************************************************************************/
staticSrape::staticSrape()
{
	strcpy(cropName,"SPRAPE");
}

/****************************************************************************\
\****************************************************************************/
double staticSrape::PVstratFactor(int PV_strat)
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
void staticSrape::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
   if (PP->GetNumOfHarvestDates()>1)
      theMessage->FatalError("static_sprape::SpecifiedPlan - more than one harvest date specified");
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticSrape::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	aPlantItem1=new plantItem;
	aPlantItem1->Setname(cropName);
	aPlantItem2=new plantItem;
 	aPlantItem2->Setname("RAPESTRAW");
	const double vsm=-0.0000001;
	const double vs = 0.0000001;
	double N=PP->GetN();
	double P=PP->GetP();
	double K=PP->GetK();
	double fHUG=PP->GetFluidManureSpring();
   double fluid_HUG_spring=PP->GetFluidManureSpring();
   double fluid_HUG_fall=PP->GetFluidManureFall();
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"RS"); // ---"----
	const double MaxGylle = 30.0;
	const double Max_N_1_sand = 50.0;
	const double Nfall=30.0;
	ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
   	theMessage->FatalError("static_sprape::automatic plan for spring rape not implemented yet!");
     	if (PP->GetSolidManure()>0)
      { // Spread solid manure
         animalFertFields * FO=new animalFertFields;
         manure * aManure=new manure;
         aManure->Setname("SOLID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(PP->GetSolidManure());
         FO->DefineAnimalFertFieldsOp(SpreadingSolidManure,crop_id,cropName,PP->GetStartYear(),8,10,PP->GetArea(),0,PP->GetDistance(),aManure);
         AddOpList(FO);
         delete aManure;
      }
      if (fluid_HUG_fall>0) { // Spread fluid manure
         animalFertFields * FO=new animalFertFields;
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("SLURRY");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         double Neff = 0.5*aManure->GetTotalN().n;
         if (Neff>75) {  // Neccasary because LP-model is incomplete
            fluid_HUG_spring += (Neff-Nfall)/(0.5*aManure->GetTotalN().n);
            fluid_HUG_fall   -= (Neff-Nfall)/(0.5*aManure->GetTotalN().n);
         }
         aManure->Setamount(fluid_HUG_fall);
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear(),8,10,PP->GetArea(),0,PP->GetDistance(),aManure);
         AddOpList(FO);
         delete aManure;
      }
      if ((fluid_HUG_fall<=0) && (PP->GetN()>0)) { // Apply mineral fertilizer
         mineralFertFields * FO=new mineralFertFields;
         double Napply=Nfall;
         if (Napply>PP->GetN())
            Napply=Nfall;
         fertilizer * aFertilizer=new fertilizer;
         aFertilizer->Setname("MINERAL-N");
         theProducts->GiveProductInformation(aFertilizer);
         aFertilizer->Setamount(Napply/aFertilizer->GetN_content().n);
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear(),9,1,PP->GetArea(),0,PP->GetDistance(),aFertilizer,NULL,NULL);
         N-=Napply;
         AddOpList(FO);
         delete aFertilizer;
      }
      { // Ploughing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(Ploughing,crop_id,cropName,PP->GetStartYear(),8,11,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // 1st seed bed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear(),8,12,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (JB_NO<=4) { // 2nd seed bed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear(),8,13,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (JB_NO<=4) { // 3. seed bed harrowing (only clayish soils)
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear(),8,14,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // Sowing
         sowFields * FO=new sowFields;
         seed * aSeed=new seed;
         aSeed->Setname("SEEDWNRPE");
         theProducts->GiveProductInformation(aSeed);
         aSeed->Setamount(aSeed->GetStandardDensity());     // 5
         FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear(),8,15,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed);
         AddOpList(FO);
         delete aSeed;
      }
      if (2==2) { // Spraying herbicide
         chemical * aChemical1=new chemical;
         aChemical1->Setname("Treflan");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(1);
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(500);
         plantCareFields * FO=new plantCareFields;
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear(),9,15,PP->GetArea(),0,PP->GetDistance(),aChemical1,NULL,NULL,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aWater;
      }
      if (2<2) { // Weedharrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(WeedHarrowing,crop_id,cropName,PP->GetStartYear(),9,15,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // Plantprotection registrations - hours for the entire fall
         manualLabourFields * FO=new manualLabourFields;
         FO->DefineManualLabourFieldsOp(MonitoringCrop,crop_id,cropName,PP->GetStartYear(),10,1,PP->GetArea(),0,PP->GetDistance(),0,3);
         AddOpList(FO);
      }
      if ((fluid_HUG_spring<=MaxGylle) && (PP->GetSolidManure()<=0)
         && ((PP->GetN()>0) || (PP->GetK()>0) || (PP->GetP()>0))) { // Apply mineral fertilizer
         double Napply=PP->GetN();
         if ((fluid_HUG_spring<=0) && (PP->GetSolidManure()<=0) && (JB_NO<=4))
            Napply=min(PP->GetN(),Max_N_1_sand);
         if (Napply>N)
            Napply=N;
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
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,3,10,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         N-=Napply;
         P-=PP->GetP();
         K-=PP->GetK();
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      if (fluid_HUG_spring>MaxGylle) { // Apply fluid manure
         animalFertFields * FO=new animalFertFields;
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("SLURRY");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(fluid_HUG_spring/2);
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear()+1,3,20,PP->GetArea(),0,PP->GetDistance(),aManure);
         fHUG-=fluid_HUG_spring/2;
         AddOpList(FO);
         delete aManure;
      }
    if (((fluid_HUG_spring<=0) && (PP->GetSolidManure()>0)) && ((PP->GetN()>0) || (PP->GetK()>0) || (PP->GetP()>0))) { // Apply mineral fertilizer
         double Napply=PP->GetN();
         if ((fluid_HUG_fall<=0) && (PP->GetSolidManure()<=0))
            Napply-=Nfall;
         if (Napply<0)
            Napply=0;
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
         N-=Napply;
         P-=PP->GetP();
         K-=PP->GetK();
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      if ((fHUG>0) && (PP->GetSolidManure()>0)) { // Apply fluid manure
         animalFertFields * FO=new animalFertFields;
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("SLURRY");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(fHUG);
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear()+1,4,1,PP->GetArea(),0,PP->GetDistance(),aManure);
         fHUG=0;
         AddOpList(FO);
         delete aManure;
      }
      if ((fHUG>0) && (PP->GetSolidManure()<=0)) { // Apply fluid manure
         animalFertFields * FO=new animalFertFields;
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("SLURRY");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(fHUG);
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear()+1,4,10,PP->GetArea(),0,PP->GetDistance(),aManure);
         fHUG=0;
         AddOpList(FO);
         delete aManure;
      }
      if ((N>0) || (P>0) || (K>0)) { // Apply mineral fertilizer - last time
         fertilizer * aFertilizer1=new fertilizer;
         aFertilizer1->Setname("MINERAL-N");
         theProducts->GiveProductInformation(aFertilizer1);
         aFertilizer1->Setamount(N/aFertilizer1->GetN_content().n);
         fertilizer * aFertilizer2=new fertilizer;
         aFertilizer2->Setname("MINERAL-P");
         theProducts->GiveProductInformation(aFertilizer2);
         aFertilizer2->Setamount(P/aFertilizer2->GetP_content());
         fertilizer * aFertilizer3=new fertilizer;
         aFertilizer3->Setname("MINERAL-K");
         theProducts->GiveProductInformation(aFertilizer3);
         aFertilizer3->Setamount(K/aFertilizer3->GetK_content());
         mineralFertFields * FO=new mineralFertFields;
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,4,11,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         N=0;
         P=0;
         K=0;
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      if ((N<vsm) || (N>vs) || (P<vsm) || (P>vs) || (K<vsm) || (K>vs) || (fHUG<vsm) || (fHUG>vs))
         theMessage->WarningWithDisplay("static_sprape::BasicOpList - Error in fertilizer handling.");
      if (2==0) { // Weedharrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(WeedHarrowing,crop_id,cropName,PP->GetStartYear()+1,4,12,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (2>0) { // Spraying insecticide
         chemical * aChemical1=new chemical;
         aChemical1->Setname("Fastac");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(1);
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(500); 
         plantCareFields * FO=new plantCareFields;
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear()+1,6,1,PP->GetArea(),0,PP->GetDistance(),aChemical1,NULL,NULL,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aWater;
      }
      if (JB_NO<=4 && irrigation) { // Irrigate 30 mm on sandy soils
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(30*10); // 30mm, 1ha
         irrigateFields * FO=new irrigateFields;
         FO->DefineIrrigateFieldsOp(Irrigating,crop_id,cropName,PP->GetStartYear()+1,6,10,PP->GetArea(),0,PP->GetDistance(),aWater,true);
         AddOpList(FO);
         delete aWater;
      }
      if (2>1) { // Spraying insecticide
         chemical * aChemical1=new chemical;
         aChemical1->Setname("Fastac");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(1);
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(500); 
         plantCareFields * FO=new plantCareFields;
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear()+1,6,15,PP->GetArea(),0,PP->GetDistance(),aChemical1,NULL,NULL,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aWater;
      }
      { // Skårlægning
         plantItem * aPlantItem=new plantItem;
         aPlantItem->Setname("RAPEPLANT");
         theProducts->GiveProductInformation(aPlantItem);
         aPlantItem->Setamount(ExpectedYield_(JB_NO,0,2)+ExpectedStrawYield_(JB_NO,0,2));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(Mowing,crop_id,cropName,PP->GetStartYear()+1,7,20,PP->GetArea(),0,PP->GetDistance(),reduction,false,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         FO->SetStrawProduct(aPlantItem);
         AddOpList(FO);
         delete aPlantItem;
      }
      { // Plantprotection registrations - hours for spring and summer
         manualLabourFields * FO=new manualLabourFields;
         double HoursPerUnit=1.0;
         if (2==0)
            HoursPerUnit=1.3; // ¤
         FO->DefineManualLabourFieldsOp(MonitoringCrop,crop_id,cropName,PP->GetStartYear()+1,8,1,PP->GetArea(),0,PP->GetDistance(),0,HoursPerUnit);
         AddOpList(FO);
      }
      { // Using harvester
         theProducts->GiveProductInformation(aPlantItem1);
         aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
         theProducts->GiveProductInformation(aPlantItem2);
         aPlantItem2->Setamount(ExpectedStrawYield_(JB_NO,0,2));
         harvestFields * FO=new harvestFields;
         FO->DefineHarvestFieldsOper(CombineHarvestingShred,crop_id,cropName,PP->GetStartYear()+1,8,2,PP->GetArea(),0,PP->GetDistance(),reduction,false,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         FO->SetMainProduct(aPlantItem1);
         FO->SetStrawProduct(aPlantItem2);
         AddOpList(FO);
         delete aPlantItem1;
         delete aPlantItem2;
      }
      { // Stubharrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(StubHarrowing,crop_id,cropName,PP->GetStartYear()+1,8,17,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
   }
  	delete aPlantItem1;
  	delete aPlantItem2;
	return LP;
}

/****************************************************************************\
\****************************************************************************/
double staticSrape::NitrogenAmount(int JB_NO,double free_N,int N_level)
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
\****************************************************************************/
double staticSrape::Nopt(int JB_NO)
{
	if (JB_NO>4) // Clayish soil
		return 220.0;
	else {       // Sandy soil
		if (irrigation)
			return 190.0;
		else
			return 130.0;
	}
}

/****************************************************************************\
\****************************************************************************/
double staticSrape::Ymin(int JB_NO)
{
	if (JB_NO>4) // Clayish soil
		return 2.0;
	else {       // Sandy soil
		if (irrigation)
			return 1.5;
		else
			return 1.0;
	}
}

/****************************************************************************\
\****************************************************************************/
double staticSrape::Ymax(int JB_NO)
{
	if (JB_NO>4) // Clayish soil
		return 4.0;
	else {       // Sandy soil
		if (irrigation)
			return 3.0;
		else
			return 1.8;
	}
}

/****************************************************************************\
\****************************************************************************/
double staticSrape::ExpectedYield_(int JB_NO,int N_level,int PV_strat)
{
	double N_act=NitrogenAmount(JB_NO,0,N_level);
	double factor=PVstratFactor(PV_strat);
   if (N_act>Nopt(JB_NO))
      return factor*Ymax(JB_NO);
   else
		return factor*(Ymin(JB_NO)+(Ymax(JB_NO)-Ymin(JB_NO))*(2*N_act/Nopt(JB_NO)-(N_act/Nopt(JB_NO))*(N_act/Nopt(JB_NO))));
}




