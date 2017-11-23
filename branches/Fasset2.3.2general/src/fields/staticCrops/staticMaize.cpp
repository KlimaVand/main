/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticMaize.h"

// THIS FILE IS NOT WORKING!!!!!!!!!!!!!!!!!

/****************************************************************************\
\****************************************************************************/
staticMaize::staticMaize()
{
	strcpy(cropName,"MAIZE");
   underSown = false;         // Default, can be changed
}

/****************************************************************************\
\****************************************************************************/
double staticMaize::PVstratFactor(int PV_strat)
{
	double f=1.00;
	switch (PV_strat)
   {
		case 0: f=0.95;break;
		case 1: f=1.00;break;
		case 2: f=1.00;break;
	}
	return f;
}

/****************************************************************************\
\****************************************************************************/
void staticMaize::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
   if (PP->GetNumOfHarvestDates()>1)
      theMessage->Warning("static_maize::SpecifiedPlan - more than one harvest date specified");       // Might not be a problem is barley with undersown clover/grass
}


/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticMaize::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
	aPlantItem1=new plantItem;
   aPlantItem1->Setname(cropName);
	aPlantItem2=new plantItem;
   aPlantItem2->Setname("MAIZESTRAW");
   char crop_id[3];      // TEMPORARY
   strcpy(crop_id,"MA");
	ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
      theMessage->FatalError("static_maize::MakeOpList - Automatic plan is not implemented");
		if (PP->GetSolidManure()>0) { // Spread solid manure
         animalFertFields * FO=new animalFertFields;
         manure * aManure=new manure;
         aManure->Setname("SOLID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(PP->GetSolidManure());
         FO->DefineAnimalFertFieldsOp(SpreadingSolidManure,crop_id,cropName,PP->GetStartYear()+1,3,27,PP->GetArea(),0,PP->GetDistance(),aManure);
         AddOpList(FO);
         delete aManure;
      }
      if (PP->GetFluidManureSpring()>0) { // Spread liquid manure
         animalFertFields * FO=new animalFertFields;
         manure * aManure=new manure;
         if (PP->GetFluidHugIsSlurry())
            aManure->Setname("PIG-SLURRY-STORED");
         else
            aManure->Setname("FLUID-MANURE");
         theProducts->GiveProductInformation(aManure);
         aManure->Setamount(PP->GetFluidManureSpring());
         FO->DefineAnimalFertFieldsOp(SpreadingLiquidManure,crop_id,cropName,PP->GetStartYear()+1,3,27,PP->GetArea(),0,PP->GetDistance(),aManure);
         AddOpList(FO);
         delete aManure;
      }
      { // Ploughing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(Ploughing,crop_id,cropName,PP->GetStartYear()+1,3,28,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if ((PP->GetN()>0) || (PP->GetP()>0) || (PP->GetK())) { // Spread mineral fertilizer
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
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,4,3,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      { // 1st seed bed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear()+1,4,4,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (JB_NO>4) { // 2nd seed bed harrowing on clayish soil
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear()+1,4,5,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // Sowing
         sowFields * FO=new sowFields;
         seed * aSeed=new seed;
         aSeed->Setname("SEEDMAIZE");
         theProducts->GiveProductInformation(aSeed);
         aSeed->Setamount(aSeed->GetStandardDensity());     // 170
         seed * aSeed1 = NULL;
         if (underSown)
         {
            aSeed->Setamount(125);
            aSeed1 = new seed;
            aSeed1->Setname("SEEDRYEGRASS");
            theProducts->GiveProductInformation(aSeed1);
            aSeed1->Setamount(15);
            char * tmp=new char[10];
            strcpy(tmp, "B2");
            FO->DefineSowFieldsOp(Sowing,tmp,cropName,PP->GetStartYear()+1,5,6,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed);
         }
         else
         	FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear()+1,5,6,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed);
         AddOpList(FO);
         delete aSeed;
         if (aSeed1)
         {
            FO->AddSeed(aSeed1,"G1");
            delete aSeed1;
         }
      }

      { // Combine harvesting
         theProducts->GiveProductInformation(aPlantItem1);
         aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
         theProducts->GiveProductInformation(aPlantItem2);
         aPlantItem2->Setamount(ExpectedStrawYield_(JB_NO,0,2));
         harvestFields * FO=new harvestFields;
         if (gatheringStraw)
            FO->DefineHarvestFieldsOper(CombineHarvesting,crop_id,cropName,PP->GetStartYear()+1,9,20,PP->GetArea(),0,PP->GetDistance(),reduction,false,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         else
            FO->DefineHarvestFieldsOper(CombineHarvestingShred,crop_id,cropName,PP->GetStartYear()+1,9,20,PP->GetArea(),0,PP->GetDistance(),reduction,false,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         FO->SetMainProduct(aPlantItem1);
         FO->SetStrawProduct(aPlantItem2);

         AddOpList(FO);
         if (gatheringStraw)
         { // Gathering straw
            harvestFields * FO=new harvestFields;
            FO->DefineHarvestFieldsOper(GatheringBales,crop_id,cropName,PP->GetStartYear()+1,9,20,PP->GetArea(),0,PP->GetDistance(),reduction,false,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
            FO->SetStrawProduct(aPlantItem2);
         	AddOpList(FO);
         }
      }
      if (!underSown)
      { // Stub harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(StubHarrowing,crop_id,cropName,PP->GetStartYear()+1,9,21,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
   }
   delete aPlantItem1;
   delete aPlantItem2;
	return LP;
}


/****************************************************************************\
\****************************************************************************/
double staticMaize::NitrogenAmount(int JB_NO,double free_N,int N_level)
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
double staticMaize::Nopt(int JB_NO)
{
	if (JB_NO>4) // Clayish soil
		return 130.0;
	else {       // Sandy soil
		if (irrigation)
			return 120.0;
		else
			return 110.0;
	}
}

/****************************************************************************\
\****************************************************************************/
double staticMaize::Ymin(int JB_NO)
{
	if (JB_NO>4) // Clayish soil
		return 2.5;
	else {       // Sandy soil
		if (irrigation)
			return 1.5;
		else
			return 1.0;
	}
}

/****************************************************************************\
\****************************************************************************/
double staticMaize::Ymax(int JB_NO)
{
	if (JB_NO>4) // Clayish soil
		return 5.5;
	else {       // Sandy soil
		if (irrigation)
			return 4.2;
		else
			return 3.3;
	}
}

/****************************************************************************\
\****************************************************************************/
double staticMaize::ExpectedYield_(int JB_NO,int N_level,int PV_strat)
{
	double factor=PVstratFactor(PV_strat);
	double N_act=NitrogenAmount(JB_NO,0,N_level);
   if (N_act>Nopt(JB_NO))
      return factor*Ymax(JB_NO);
   else
      return factor*(Ymin(JB_NO)+(Ymax(JB_NO)-Ymin(JB_NO))*(2*N_act/Nopt(JB_NO)-(N_act/Nopt(JB_NO))*(N_act/Nopt(JB_NO))));
}

/****************************************************************************\
A stubble height of 12 cm is anticipated.
\****************************************************************************/
double staticMaize::ExpectedStrawYield_(int JB_NO,int N_level,int PV_strat)
{
   if (gatheringStraw)
		return 0.48*ExpectedYield_(JB_NO,N_level,PV_strat);
   else
   	return 0.0;
}




