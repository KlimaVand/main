/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticSbarley.h"

/****************************************************************************\
\****************************************************************************/
staticSbarley::staticSbarley()
{
	strcpy(cropName,"SPBARL");
   underSown = false;         // Default, can be changed
}

/****************************************************************************\
\****************************************************************************/
double staticSbarley::PVstratFactor(int PV_strat)
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
void staticSbarley::HarvestPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   /* Could be undersown !!!
   if (PP->GetNumOfHarvestDates()>1)
      theMessage->Warning("static_spbarl::SpecifiedPlan - more than one harvest date specified");  */
   int year=PP->GetStartYear()+1;
   int month=8;
   int day=16;
   if (PP->GetNumOfHarvestDates()>0)
   {
      string * date=PP->GetHarvestDate(0);
      year=PP->GetYear(date);
      month=PP->GetMonth(date);
      day=PP->GetDay(date);
   }
   { // Combine harvesting
      theProducts->GiveProductInformation(aPlantItem1);
      aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
      theProducts->GiveProductInformation(aPlantItem2);
      aPlantItem2->Setamount(ExpectedStrawYield_(JB_NO,0,2));
      harvestFields * FO=new harvestFields;
      if (gatheringStraw)
      	FO->DefineHarvestFieldsOper(CombineHarvesting,crop_id,cropName,year,month,day,PP->GetArea(),0,PP->GetDistance(),1.0,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
      else
      	FO->DefineHarvestFieldsOper(CombineHarvestingShred,crop_id,cropName,year,month,day,PP->GetArea(),0,PP->GetDistance(),1.0,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
	   FO->SetMainProduct(aPlantItem1);
      // FO->SetStrawProduct(0,aPlantItem2); !!!
      AddOpList(FO);
      if (gatheringStraw)
      { // Gathering straw
         bsTime bs;
         bs.SetTime(day,month,year);
         bs.AddOneDay();
       	harvestFields * FO1=new harvestFields;
       	FO1->DefineHarvestFieldsOper(GatheringBales,crop_id,cropName,bs.GetYear(),bs.GetMonth(),bs.GetDay(),PP->GetArea(),0,PP->GetDistance(),1.0,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
         FO1->SetStrawProduct(aPlantItem2);
         AddOpList(FO1);
      }

      for (int i=1;i<PP->GetNumOfHarvestDates();i++)             // cooping with several harvesting dates (i.e. clover/grass)
		{
      	string * date=PP->GetHarvestDate(i);
      	plantItem * aPlantItem=new plantItem;
      	aPlantItem->Setname("CLOVERGRASS");
      	theProducts->GiveProductInformation(aPlantItem);
      	aPlantItem->Setamount(ExpectedYield_(JB_NO,0,2));
      	harvestFields * FO=new harvestFields;
      	FO->DefineHarvestFieldsOper(CutSetAside,crop_id,cropName,PP->GetYear(date),PP->GetMonth(date),PP->GetDay(date),PP->GetArea(),0,PP->GetDistance(),1.0,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
      	FO->SetMainProduct(aPlantItem);
         AddOpList(FO);
      	delete aPlantItem;
   	}
   }
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticSbarley::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
   aPlantItem1 = new plantItem;
   aPlantItem1->Setname("BARLEY");
   aPlantItem2 = new plantItem;
   aPlantItem2->Setname("BARLEYSTRAW"); 
	char crop_id[3];      // TEMPORARY
   if (underSown)
   	strcpy(crop_id,"B8");
   else
	   if (gatheringStraw)
			strcpy(crop_id,"B2");
	   else
	   	strcpy(crop_id,"B1");
	ResetOpList();
   if (!PP->GetAutomatic())
   {
      SpecifiedPlan(PP,JB_NO,crop_id);
//      HarvestPlan(PP,JB_NO,crop_id); Probably superfluous
   }
   else
   { // Automatic plan
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
            aManure->Setname("SLURRY");
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
         aSeed->Setname("SEEDSPBARL");
         theProducts->GiveProductInformation(aSeed);
         aSeed->Setamount(aSeed->GetStandardDensity());     // 170
         seed * aSeed1 = NULL;
         seed* aSeed2 = NULL;
         if (underSown)
         {
            aSeed->Setamount(125);
            aSeed1 = new seed;
            aSeed1->Setname("SEEDRYEGRASS");
            theProducts->GiveProductInformation(aSeed1);
            aSeed1->Setamount(15);
        /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            aSeed2 = new seed;
            aSeed2->Setname("SEEDCLOVER");
            theProducts->GiveProductInformation(aSeed2);
            aSeed2->Setamount(5);
         */
            char * input=new char[20];
            strcpy(input,"B2");
            FO->DefineSowFieldsOp(Sowing,input,cropName,PP->GetStartYear()+1,4,6,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed);
         }
         else
         	FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear()+1,4,6,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed);
         AddOpList(FO);
         delete aSeed;
         if (aSeed1)
         {
            FO->AddSeed(aSeed1,"G1");
            delete aSeed1;
         }
         if (aSeed2)
         {
            FO->AddSeed(aSeed2,"K1");
            delete aSeed2;
         }
      }
      if (2==0) { // Weed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(WeedHarrowing,crop_id,cropName,PP->GetStartYear()+1,5,4,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (2>0) { // Spraying
         chemical * aChemical1=new chemical;
         aChemical1->Setname("Express");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(0.5);
         chemical * aChemical2=new chemical;
         aChemical2->Setname("Oxitril");
         theProducts->GiveProductInformation(aChemical2);
         aChemical2->SetByTFI(0.3);
         chemical * aChemical3=new chemical;
         aChemical3->Setname("Manganchelate");
         theProducts->GiveProductInformation(aChemical3);
         aChemical3->SetByTFI(1);
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(300); // ???
         plantCareFields * FO=new plantCareFields;
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear()+1,5,6,PP->GetArea(),0,PP->GetDistance(),aChemical1,aChemical2,aChemical3,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aChemical2;
         delete aChemical3;
         delete aWater;
      }
      if (2==0) { // Weed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(WeedHarrowing,crop_id,cropName,PP->GetStartYear()+1,5,21,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (JB_NO<=4 && irrigation) { // Irrigating 30 mm on sandy soil
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(30*10); // 30mm, 1ha
         irrigateFields * FO=new irrigateFields;
         FO->DefineIrrigateFieldsOp(Irrigating,crop_id,cropName,PP->GetStartYear()+1,5,20,PP->GetArea(),0,PP->GetDistance(),aWater,true);
         AddOpList(FO);
         delete aWater;
      }
      { // Crop monitoring - hours for the spring & summer
         manualLabourFields * FO=new manualLabourFields;
         double HoursPerUnit=0.5;
         if (2==0)
            HoursPerUnit=0.8;
         FO->DefineManualLabourFieldsOp(MonitoringCrop,crop_id,cropName,PP->GetStartYear()+1,7,30,PP->GetArea(),0,PP->GetDistance(),0,HoursPerUnit);
         AddOpList(FO);
      }

      HarvestPlan(PP,JB_NO,crop_id);
   /*{ // Combine harvesting
         plantItem * aPlantItem1=new plantItem;
         aPlantItem1->Setname("BARLEY");
         theProducts->GiveProductInformation(aPlantItem1);
         aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
         plantItem * aPlantItem2=new plantItem;
         aPlantItem2->Setname("BARLEYSTRAW");
         theProducts->GiveProductInformation(aPlantItem2);
         aPlantItem2->Setamount(ExpectedStrawYield_(JB_NO,0,2));
         harvestFields * FO=new harvestFields;
         if (gatheringStraw)
            FO->DefineHarvestFieldsOper(CombineHarvesting,crop_id,cropName,PP->GetStartYear()+1,8,16,PP->GetArea(),0,PP->GetDistance(),reduction,false);
         else
            FO->DefineHarvestFieldsOper(CombineHarvestingShred,crop_id,cropName,PP->GetStartYear()+1,8,16,PP->GetArea(),0,PP->GetDistance(),reduction,false);
         FO->SetMainProduct(0,aPlantItem1);
         FO->SetStrawProduct(0,aPlantItem2);

         AddOpList(FO);
         if (gatheringStraw)
         { // Gathering straw
            harvestFields * FO=new harvestFields;
            FO->DefineHarvestFieldsOper(GatheringBales,crop_id,cropName,PP->GetStartYear()+1,8,16,PP->GetArea(),0,PP->GetDistance(),reduction,false);
            FO->SetStrawProduct(0,aPlantItem2);
         	AddOpList(FO);
         }
         delete aPlantItem1;
         delete aPlantItem2;
      } */
      if (!underSown)
      { // Stub harrowing
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
double staticSbarley::NitrogenAmount(int JB_NO,double free_N,int N_level)
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
double staticSbarley::Nopt(int JB_NO)
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
double staticSbarley::Ymin(int JB_NO)
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
double staticSbarley::Ymax(int JB_NO)
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
double staticSbarley::ExpectedYield_(int JB_NO,int N_level,int PV_strat)
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
double staticSbarley::ExpectedStrawYield_(int JB_NO,int N_level,int PV_strat)
{
   if (gatheringStraw)
		return 0.48*ExpectedYield_(JB_NO,N_level,PV_strat);
   else
   	return 0.0;
}

