/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "staticPea.h"

/****************************************************************************\
\****************************************************************************/
staticPea::staticPea()
{
	strcpy(cropName,"PEA");
}

/****************************************************************************\
\****************************************************************************/
double staticPea::PVstratFactor(int PV_strat)
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
void staticPea::SpecifiedPlan(fieldOrder * PP,int JB_NO,char * crop_id)
{
   staticCrop::SpecifiedPlan(PP,JB_NO,crop_id);
   if (PP->GetNumOfHarvestDates()>1)
      theMessage->FatalError("static_pea::SpecifiedPlan - more than one harvest date specified");
}

/****************************************************************************\
\****************************************************************************/
cloneList<fieldOperationFields> * staticPea::MakeOpList(fieldOrder * PP,int JB_NO,double reduction)
{
  	aPlantItem1=new plantItem;
  	aPlantItem1->Setname(cropName);
  	aPlantItem2=new plantItem;
  	aPlantItem2->Setname("PEASTRAW");
	char crop_id[3];      // TEMPORARY
	strcpy(crop_id,"PE"); // ---"----
   ResetOpList();
   if (!PP->GetAutomatic())
      SpecifiedPlan(PP,JB_NO,crop_id);
   else
   { // Automatic plan
      { // Ploughing
         soilTreatFields * FO=new soilTreatFields;
//         FO->DefineOp(Ploughing,crop_id,cropName,PP->GetStartYear(),3,28,PP->GetArea(),0,PP->GetDistance());    Corrected MEL 2009
         FO->DefineOp(Ploughing,crop_id,cropName,PP->GetStartYear()+1,3,28,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if ((PP->GetN()>0) || (PP->GetK()>0) || (PP->GetP()>0))
      { // Spread mineral fertilizer
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
         FO->DefineMineralFertFieldsOp(MineralFertilizing,crop_id,cropName,PP->GetStartYear()+1,3,29,PP->GetArea(),0,PP->GetDistance(),aFertilizer1,aFertilizer2,aFertilizer3);
         AddOpList(FO);
         delete aFertilizer1;
         delete aFertilizer2;
         delete aFertilizer3;
      }
      { // 1st seed bed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear()+1,3,30,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (JB_NO>4)
      { // 2nd seed bed harrowing on clayish soil
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(SeedbedHarrowing,crop_id,cropName,PP->GetStartYear()+1,3,31,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      { // Sowing
         sowFields * FO=new sowFields;
         seed * aSeed=new seed;
         aSeed->Setname("SEEDPEA");
         theProducts->GiveProductInformation(aSeed);
         aSeed->Setamount(aSeed->GetStandardDensity());     // 	200
         FO->DefineSowFieldsOp(Sowing,crop_id,cropName,PP->GetStartYear()+1,4,1,PP->GetArea(),0,PP->GetDistance(),2,reduction,aSeed);
         AddOpList(FO);
         delete aSeed;
      }
      if (2==0) { // Weed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(WeedHarrowing,crop_id,cropName,PP->GetStartYear()+1,4,15,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (2>0) { // Spraying
         chemical * aChemical1=new chemical;
         aChemical1->Setname("Stomp-SC");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(0.38);
         chemical * aChemical2=new chemical;
         aChemical2->Setname("Basagran-480");
         theProducts->GiveProductInformation(aChemical2);
         aChemical2->SetByTFI(0.5);
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(200);
         plantCareFields * FO=new plantCareFields;
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear()+1,4,15,PP->GetArea(),0,PP->GetDistance(),aChemical1,aChemical2,NULL,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aChemical2;
         delete aWater;
      }
      if (2==0) { // Weed harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(WeedHarrowing,crop_id,cropName,PP->GetStartYear()+1,4,29,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
      if (2>0)
      { // Spraying
         chemical * aChemical1=new chemical;
         aChemical1->Setname("Stomp-SC");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(0.38);
         chemical * aChemical2=new chemical;
         aChemical2->Setname("Basagran-480");
         theProducts->GiveProductInformation(aChemical2);
         aChemical2->SetByTFI(0.5);
         chemical * aChemical3=new chemical;
         aChemical3->Setname("Fastac");
         theProducts->GiveProductInformation(aChemical3);
         aChemical3->SetByTFI(1.0);
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(200);
         plantCareFields * FO=new plantCareFields;
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear()+1,4,28,PP->GetArea(),0,PP->GetDistance(),aChemical1,aChemical2,aChemical3,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aChemical2;
         delete aChemical3;
         delete aWater;
      }
      if (2>0)
      { // Spraying
         chemical * aChemical1=new chemical;
         aChemical1->Setname("Manganchelate");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(2.0); // 3kg/ha
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(200);
         plantCareFields * FO=new plantCareFields;
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear()+1,5,1,PP->GetArea(),0,PP->GetDistance(),aChemical1,NULL,NULL,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aWater;
      }
      if (2>1)
      { // Spraying against pests
         chemical * aChemical1=new chemical;
         aChemical1->Setname("Pirimor-G");
         theProducts->GiveProductInformation(aChemical1);
         aChemical1->SetByTFI(1.0);
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(200);
         plantCareFields * FO=new plantCareFields;
         FO->DefinePlantCareFieldsOp(Spraying,crop_id,cropName,PP->GetStartYear()+1,6,1,PP->GetArea(),0,PP->GetDistance(),aChemical1,NULL,NULL,aWater);
         AddOpList(FO);
         delete aChemical1;
         delete aWater;
      }
      if (JB_NO<=4 && irrigation)
      { // Irrigating 30 mm on sandy soil
         water * aWater=new water;
         aWater->Setname("WATER");
         theProducts->GiveProductInformation(aWater);
         aWater->Setamount(30*10); // 30mm, 1ha
         irrigateFields * FO=new irrigateFields;
         FO->DefineIrrigateFieldsOp(Irrigating,crop_id,cropName,PP->GetStartYear()+1,6,20,PP->GetArea(),0,PP->GetDistance(),aWater,true);
         AddOpList(FO);
         delete aWater;
      }
      { // Crop monitoring - hours for the spring & summer
         manualLabourFields * FO=new manualLabourFields;
         double HoursPerUnit=0.7;
         if (2==0)
            HoursPerUnit=1.0;
         FO->DefineManualLabourFieldsOp(MonitoringCrop,crop_id,cropName,PP->GetStartYear()+1,7,30,PP->GetArea(),0,PP->GetDistance(),0,HoursPerUnit);
         AddOpList(FO);
      }
      { // Combine harvesting
      theProducts->GiveProductInformation(aPlantItem1);
      aPlantItem1->Setamount(ExpectedYield_(JB_NO,0,2));
      theProducts->GiveProductInformation(aPlantItem2);
      aPlantItem2->Setamount(ExpectedStrawYield_(JB_NO,0,2));
      harvestFields * FO=new harvestFields;
      FO->DefineHarvestFieldsOper(CombineHarvestingShred,crop_id,cropName,PP->GetStartYear()+1,8,15,PP->GetArea(),0,PP->GetDistance(),reduction,false,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
      FO->SetMainProduct(aPlantItem1);
      FO->SetStrawProduct(aPlantItem2);
      AddOpList(FO);
      }
      { // Stub harrowing
         soilTreatFields * FO=new soilTreatFields;
         FO->DefineOp(StubHarrowing,crop_id,cropName,PP->GetStartYear()+1,8,16,PP->GetArea(),0,PP->GetDistance());
         AddOpList(FO);
      }
   }
   delete aPlantItem1;
  	delete aPlantItem2;
	return LP;
}

/****************************************************************************\
\****************************************************************************/
double staticPea::NitrogenAmount(int JB_NO,double free_N,int N_level)
{
   return 0;
}

/****************************************************************************\
\****************************************************************************/
double staticPea::ExpectedYield_(int JB_NO,int N_level,int PV_strat)
{
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


