/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticWbarley.h"

/****************************************************************************\
\****************************************************************************/
staticWbarley::staticWbarley()
{
	strcpy(cropName,"WNBRL");
}

/****************************************************************************\
\****************************************************************************/
double staticWbarley::PVstratFactor(int PV_strat)
{
	double f=1.00;
	switch (PV_strat) {
		case 0: f=0.93;break;
		case 1: f=0.95;break;
		case 2: f=1.00;break;
	}
	return f;
}

/****************************************************************************\
\****************************************************************************/
void staticWbarley::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
   if (PP->GetNumOfHarvestDates()>1)
      theMessage->FatalError("static_wbarl::SpecifiedPlan - more than one harvest date specified");
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticWbarley::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
   aPlantItem1=new plantItem;
   aPlantItem1->Setname("BARLEY");
   aPlantItem2=new plantItem;
   aPlantItem2->Setname("BARLEYSTRAW");
	char crop_id[3];      // TEMPORARY
 	if (!gatheringStraw)
      strcpy(crop_id,"B5");
   else
      strcpy(crop_id,"B6");
	ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   {   // Automatic plan
   	const double MaxGylle = 30;
      const double Max_N_1_sand = 50;
      const double vsm=-0.0000001;
      const double vs = 0.0000001;
      double N=PP->GetN();
      double P=PP->GetP();
      double K=PP->GetK();
      double fHUG=PP->GetFluidManureSpring();
      if (PP->GetFluidManureFall()>0)
         theMessage->Warning("static_wnbrl::BasicOpList - Fluid manure not allowed in fall.");
      if (PP->GetSolidManure()>0) { // Apply solid manure
         manure * aManure=new manure;
         aManure->Setname("SOLID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(PP->GetSolidManure());
         animalFertFields * FO=new animalFertFields;
         FO->DefineAnimalFertFieldsOp(SpreadingSolidManure,crop_id,cropName,PP->GetStartYear(),9,11,PP->GetArea(),0,PP->GetDistance(),aManure);
         AddOpList(FO);
         delete aManure;
      }
      { // Plough
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(Ploughing,crop_id,cropName,PP->GetStartYear(),9,12,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // 1st seed bed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear(),9,13,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (JB_NO>4) { // 2nd seed bed harrowing (only clayish soils)
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear(),9,14,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // Sowing
         seed * aSeed=new seed;
         aSeed->Setname("SEEDWNBRL");
         theProducts->GiveProductInformation(aSeed);
         aSeed->Setamount(aSeed->GetStandardDensity());  // 160.0
         sowFields * FO=new sowFields;
         FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear(),9,15,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed);
         AddOpList(FO);
         delete aSeed;
      }
      if (2==0)
      { // Plantprotection registrations - hour for the fall
         manualLabourFields * FO=new manualLabourFields;
         FO->DefineManualLabourFieldsOp(MonitoringCrop,crop_id,cropName,PP->GetStartYear(),10,1,PP->GetArea(),0,PP->GetDistance(),0,0.3);
         AddOpList(FO);
      }
      if (2>0) { // Spraying herbicide
         chemical * aChemical1=new chemical;
         aChemical1->Setname("Stomp-SC");
         theProducts->GiveProductInformation(aChemical1);
         if (2==2)
            aChemical1->SetByTFI(0.75);
         else
            aChemical1->SetByTFI(0.5);
         chemical * aChemical2=new chemical;
         aChemical2->Setname("Arelon");
         theProducts->GiveProductInformation(aChemical2);
         aChemical2->SetByTFI(1);
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(500);
         plantCareFields * FO=new plantCareFields;
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear(),10,15,PP->GetArea(),0,PP->GetDistance(),aChemical1,aChemical2,NULL,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aChemical2;
         delete aWater;
      }
      if (2==0) { // Weedharrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(WeedHarrowing,crop_id,cropName,PP->GetStartYear(),10,15,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (PP->GetFluidManureSpring()>MaxGylle) { // Apply FLUID manure
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("SLURRY");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(PP->GetFluidManureSpring()/2);
         animalFertFields * FO=new animalFertFields;
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear()+1,3,20,PP->GetArea(),0,PP->GetDistance(),aManure);
         fHUG-=PP->GetFluidManureSpring()/2;
         AddOpList(FO);
         delete aManure;
      }
      if ((PP->GetFluidManureSpring()<=MaxGylle) && (PP->GetSolidManure()<=0)
         && ((PP->GetN()>0) || (PP->GetP()>0) || (PP->GetK()>0))) { // Apply mineral fertilizer
         double Napply=PP->GetN();
         if ((PP->GetFluidManureSpring()<=0) && (PP->GetSolidManure()<=0) && (JB_NO<=4))
            Napply=min(PP->GetN(),Max_N_1_sand);
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
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,3,20,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         N-=Napply;
         P-=PP->GetP();
         K-=PP->GetK();
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      if (((PP->GetFluidManureSpring()<=0) && (PP->GetSolidManure()>0))
         && ((PP->GetN()>0) || (PP->GetP()>0) || (PP->GetK()>0))){ // Apply mineral fertilizer
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
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,4,10,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         N-=PP->GetN();
         P-=PP->GetP();
         K-=PP->GetK();
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      if ((fHUG>0) && (PP->GetSolidManure()>0)) { // Apply FLUID manure
         animalFertFields * FO=new animalFertFields;
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("SLURRY");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(PP->GetFluidManureSpring());
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear()+1,4,10,PP->GetArea(),0,PP->GetDistance(),aManure);
         fHUG-=PP->GetFluidManureSpring();
         AddOpList(FO);
         delete aManure;
      }
      if ((fHUG>0) && (PP->GetSolidManure()==0)) { // Apply FLUID manure
         animalFertFields * FO=new animalFertFields;
         double HUGapply=PP->GetFluidManureSpring();
         if (PP->GetFluidManureSpring()>MaxGylle)
            HUGapply=PP->GetFluidManureSpring()/2;
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("SLURRY");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(HUGapply);
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear()+1,4,20,PP->GetArea(),0,PP->GetDistance(),aManure);
         fHUG-=HUGapply;
         AddOpList(FO);
         delete aManure;
      }
      if ((N>0) || (P>0) || (K>0)) { // Apply mineral fertilizer for the last time
         fertilizer * aFertilizer1=new fertilizer;
         aFertilizer1->Setname("MINERAL-N");
         theProducts->GiveProductInformation(aFertilizer1);
         aFertilizer1->Setamount(N/aFertilizer1->GetN_content().n);
         fertilizer * aFertilizer2=new fertilizer;
         aFertilizer2->Setname("MINERAL-P");
         theProducts->GiveProductInformation(aFertilizer2);
         aFertilizer2->Setamount(PP->GetP()/aFertilizer2->GetP_content());
         fertilizer * aFertilizer3=new fertilizer;
         aFertilizer3->Setname("MINERAL-K");
         theProducts->GiveProductInformation(aFertilizer3);
         aFertilizer3->Setamount(K/aFertilizer3->GetK_content());
         mineralFertFields * FO=new mineralFertFields;
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,4,21,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         N=0;
         P=0;
         K=0;
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }

      if ((N<vsm) || (N>vs) || (P<vsm) || (P>vs) || (K<vsm) || (K>vs) || (fHUG<vsm) || (fHUG>vs))
         theMessage->WarningWithDisplay("static_wnbrl::BasicOpList - Error in fertilizer handling.");

      if (2==2) { // Spray herbicide
         chemical * aChemical1=new chemical;
         aChemical1->Setname("Starane-180");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(1);
         plantCareFields * FO=new plantCareFields;
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(500); 
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear()+1,4,11,PP->GetArea(),0,PP->GetDistance(),aChemical1,NULL,NULL,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aWater;
      }
      if (2==0) { // Weedharrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(WeedHarrowing,crop_id,cropName,PP->GetStartYear()+1,4,11,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (2==0) { // Weedharrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(WeedHarrowing,crop_id,cropName,PP->GetStartYear()+1,4,26,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (2==2) { // Spray fungicide
         chemical * aChemical1=new chemical;
         aChemical1->Setname("TiltTop");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(0.5);
         plantCareFields * FO=new plantCareFields;
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(500);
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear()+1,5,1,PP->GetArea(),0,PP->GetDistance(),aChemical1,NULL,NULL,aWater);
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
         FO->DefineIrrigateFieldsOp(Irrigating,crop_id,cropName,PP->GetStartYear()+1,5,20,PP->GetArea(),0,PP->GetDistance(),aWater,true);
         AddOpList(FO);
         delete aWater;
      }
      if (2>0) { // Spray fungicides - 2nd time
         chemical * aChemical1=new chemical;
         aChemical1->Setname("TiltTop");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(0.5);
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(500); 
         plantCareFields * FO=new plantCareFields;
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear()+1,5,20,PP->GetArea(),0,PP->GetDistance(),aChemical1,NULL,NULL,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aWater;
      }
      { // Plantprotection registrations - hours for the spring and summer
         manualLabourFields * FO=new manualLabourFields;
         double HoursPerUnit=0.7;
         if (2==0)
            HoursPerUnit=1.0;
         FO->DefineManualLabourFieldsOp(MonitoringCrop,crop_id,cropName,PP->GetStartYear()+1,8,1,PP->GetArea(),0,PP->GetDistance(),0,HoursPerUnit);
         AddOpList(FO);
      }

      { // Use harvester
			theProducts->GiveProductInformation(aPlantItem1);
			aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
			theProducts->GiveProductInformation(aPlantItem2);
			aPlantItem2->Setamount(ExpectedStrawYield_(JB_NO,0,2));
			harvestFields * FO=new harvestFields;
			if (gatheringStraw)
	   		FO->DefineHarvestFieldsOper(CombineHarvesting,crop_id,cropName,PP->GetStartYear()+1,7,20,PP->GetArea(),0,PP->GetDistance(),reduction,false,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         else
				FO->DefineHarvestFieldsOper(CombineHarvestingShred,crop_id,cropName,PP->GetStartYear()+1,7,20,PP->GetArea(),0,PP->GetDistance(),reduction,false,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
			FO->SetMainProduct(aPlantItem1);
         FO->SetStrawProduct(aPlantItem2);
         AddOpList(FO);
		}
      if (gatheringStraw)
	   { // Gather straw
	   	harvestFields * FO=new harvestFields;
	   	theProducts->GiveProductInformation(aPlantItem2);
	   	aPlantItem2->Setamount(ExpectedStrawYield_(JB_NO,0,2));
	   	FO->DefineHarvestFieldsOper(GatheringBales,crop_id,cropName,PP->GetStartYear()+1,7,21,PP->GetArea(),0,PP->GetDistance(),reduction,false,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
	   	FO->SetStrawProduct(aPlantItem1);
         AddOpList(FO);
	   }

      { // Stubharrow
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(StubHarrowing,crop_id,cropName,PP->GetStartYear()+1,7,22,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
   }
   delete aPlantItem1;
   delete aPlantItem2;
	return LP;
}



/****************************************************************************\
\****************************************************************************/
double staticWbarley::NitrogenAmount(int JB_NO,double free_N,int N_level)
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
double staticWbarley::Nopt(int JB_NO)
{
	if (JB_NO>4) // Clayish soils
		return 180;
	else {
      if (irrigation) // Sandy soils
			return 160;
		else
         return 110;
	}
}

/****************************************************************************\
\****************************************************************************/
double staticWbarley::Ymin(int JB_NO)
{
	if (JB_NO>4) // Clayish soils
		return 3.5;
	else {
      if (irrigation) // Sandy soils
			return 2.3;
		else
         return 1.5;
	}
}

/****************************************************************************\
\****************************************************************************/
double staticWbarley::Ymax(int JB_NO)
{
	if (JB_NO>4) // Clayish soils
		return 7.0;
	else {
      if (irrigation) // Sandy soils
			return 5.3;
		else
         return 3.1;
	}
}

/****************************************************************************\
\****************************************************************************/
double staticWbarley::ExpectedYield_(int JB_NO,int N_level,int PV_strat)
{
	double factor=PVstratFactor(PV_strat);
	double N_act=NitrogenAmount(JB_NO,0,N_level);
   if (N_act>Nopt(JB_NO))
      return factor*Ymax(JB_NO);
   else
		return factor*(Ymin(JB_NO)+(Ymax(JB_NO)-Ymin(JB_NO))*(2*N_act/Nopt(JB_NO)-(N_act/Nopt(JB_NO))*(N_act/Nopt(JB_NO))));
}





/****************************************************************************\
\****************************************************************************/
double staticWbarley::ExpectedStrawYield_(int JB_NO,int N_level,int PV_strat)
{
   if (gatheringStraw)
		return 0.48*ExpectedYield_(JB_NO,N_level,PV_strat); 				// OBS. Assuming a stub heigth of 12 cm.
   else
   	return 0.0;
}
