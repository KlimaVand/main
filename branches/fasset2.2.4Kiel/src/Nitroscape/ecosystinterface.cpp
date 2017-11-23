
#include "../Nitroscape/ecosystinterface.h"
//#include "output.h"
#include "../products/plantItem.h"

ecosystinterfaceClass::ecosystinterfaceClass(int ecoNum, char * soilFileName, int aIndex, double anArea)
{
	theEcosystem=new ecosystem("ecosytstem", ecoNum,this);
   theEcosystem->Initialise(soilFileName, aIndex, anArea);
	harvestStuff = new linkList <harvInfoClass>;
    totalLAI=0.0;
    ReadyForHarvestOrLater=true;
    IrrigationDemand=0.0;
    AddedChloride=0.0;
    irrigationWater=0.0;
    irrigationN=0.0;
   killAllCrops=false;  //needs to be reset daily
    AbovegroundCropN=0.0;
    MaxCropHeight=0.0;
    StandingDMdensity=0.0;
    AvailableStandingDM=0.0;
    AvailableStandingDMHeight=0.0;
    dryMatterGrazed=0.0;
    grazedHt=0.0;

    thesurfNitrate=0.0;
    thesurfAmmonium=0.0;
    thesoilNitrate=0.0;
    thesoilAmmonium=0.0;

    theDepth=0.0;                                         // Tillage Depth
    theFractionIncorporated=0.0;                          // Fraction of surface organic matter incorporated


    harvestCode=0;
    propGrainHarvested=0.0;
    propStrawHarvested=0.0;
}


void ecosystinterfaceClass::AddDecomposable(char *decompName, double decompAmount, double decompNH4_N, double decompNO3_N, double decompOrg_N, double decompC)
{
	decomposable * adecomp = new decomposable(decompName,0,NULL);
   adecomp->Setamount(decompAmount);
   adecomp->SetNO3_content(decompNO3_N/decompAmount);
   adecomp->SetNH4_content(decompNH4_N/decompAmount);
   adecomp->SetorgN_content(decompOrg_N/decompAmount);
   adecomp->SetDryMatter(1.0);
   theEcosystem->AddDecomposable(adecomp);
};


void ecosystinterfaceClass::Sow(int seedIndex, double amountSeedDM, double NitrogenInSeed)
{
//	seed * aproduct = (seed *)theProducts->GetProductPtr(seedObj,seedIndex);
   char achar1[20];
//   strcpy(achar1,aproduct->GetName());
   strcpy(achar1,"SEEDWNWHT");
	char achar2[20];         
   AllocateCrop(achar1, achar2);
   theEcosystem->Sow(achar2, amountSeedDM, 1.0, NitrogenInSeed);
};

void ecosystinterfaceClass::Tillage(double theDepth, double theFractionIncorporated)
{
	soilTreatFields * TillageOp = new soilTreatFields();

   TillageOp->SetDepth(theDepth);
   TillageOp->SetFractionIncorporated(theFractionIncorporated);
   theEcosystem->Tillage(TillageOp);
}

void ecosystinterfaceClass::SetirrigationWater(double amountWater, double amountN)
{
	water *aWater = new water();
   aWater->Setamount(amountWater);
   aWater->SetN_content(amountN/amountWater);
	theEcosystem->SetirrigationWater(aWater);
}

void ecosystinterfaceClass::HarvestOperations(int harvestCode, double propGrainHarvested, double propStrawHarvested)
{
	harvestFields *hrv = new harvestFields();
   hrv->SetOperationId(harvestCode);
   hrv->SetFractionGrainHarvested(propGrainHarvested);
	hrv->SetFractionStrawHarvested(propStrawHarvested);
	theEcosystem->HarvestOperations(hrv);
   double totalN = 0.0;
   plantItem *anitem;
   for (int i=0; i<4; i++)
   {
   	if (hrv->GetMainProduct(i))
      {
      	anitem = hrv->GetMainProduct(i);
      	totalN+=anitem->GetAmount() * anitem->GetAllN().n;
      }
   	if (hrv->GetStrawProduct(i))
      {
      	anitem = hrv->GetStrawProduct(i);
      	totalN+=anitem->GetAmount() * anitem->GetAllN().n;
      }
   }

   theOutput->AddIndicator(environmentalIndicator,"35.20 N in sold plant products","kg N",totalN *1000.0);
}

void ecosystinterfaceClass::UpdateStatus()
{
   //	double totalLAI;
      ReadyForHarvestOrLater = theEcosystem->GetReadyForHarvestOrLater();
      IrrigationDemand = theEcosystem->GetIrrigationDemand();
      if (killAllCrops)
      	theEcosystem->KillAllCrops();
      MaxCropHeight= theEcosystem->GetMaxCropHeight();
      StandingDMdensity= theEcosystem->GetStandingDMdensity();
		AvailableStandingDM= theEcosystem->GetAvailableStandingDM(AvailableStandingDMHeight);
//      double dryMatterGrazed;
//      double grazedHt;
      crop * aCrop;
      int numOfCrops = theEcosystem->GetCrops()->NumOfNodes();
      harvestStuff->Reset();
      for (int i=0; i<numOfCrops; i++)
      {
      	aCrop=theEcosystem->GetCrops()->ElementAtNumber(i);
         harvInfoClass *aharvInfoClass = new harvInfoClass();
         aharvInfoClass->SetproductCode(0);
         char achar[20];
         strcpy(achar,aCrop->GetName());
         if (strlen(achar)==0)
         	strcpy(achar,"No crop");
			aharvInfoClass->SetproductName(achar);
         aharvInfoClass->SetamountDM(aCrop->GiveDryMatterStorage());
         aharvInfoClass->SetamountN(aCrop->GetTotStorageN());
         aharvInfoClass->SetamountC(aharvInfoClass->GetamountDM()*0.46);
         harvestStuff->InsertLast(aharvInfoClass);

         aharvInfoClass = new harvInfoClass();
         aharvInfoClass->SetproductCode(1);
         char achar2[20];
         strcpy(achar2,aCrop->GetStrawItemName().c_str());
			aharvInfoClass->SetproductName(achar2);
         aharvInfoClass->SetamountDM(aCrop->GiveDryMatterVegTop());
         aharvInfoClass->SetamountN(aCrop->GetTotalStrawN());
         aharvInfoClass->SetamountC(aharvInfoClass->GetamountDM()*0.46);
         harvestStuff->InsertLast(aharvInfoClass);
      }
}

void ecosystinterfaceClass::DoDailyEco()
{
	theEcosystem->CropSoilExchange();
   theEcosystem->GiveIndicators();
	UpdateStatus();
};


void ecosystinterfaceClass::AllocateCrop(char * seedName, char *cropCode)
{
   if (strcmp(seedName,"SEEDWNWHT")==0)
   strcpy(cropCode,"W1");
   if (strcmp(seedName,"SEEDWNRPE")==0)
   strcpy(cropCode,"RW");
   if (strcmp(seedName,"SEEDSPRPE")==0)
   strcpy(cropCode,"RS");
   if (strcmp(seedName,"SEEDSPBARL")==0 )
   strcpy(cropCode,"B1");
   if (strcmp(seedName,"SEEDRYEGRASS")==0 )
   strcpy(cropCode,"G1");
   if (strcmp(seedName,"SEEDWNBRL")==0)
   strcpy(cropCode,"B5");
   if (strcmp(seedName,"PSEEDPEA")==0)
   strcpy(cropCode,"PE");
   if (strcmp(seedName,"SEEDSPWHT")==0)
   strcpy(cropCode,"W5");
   if (strcmp(seedName,"SEEDBEET")==0)
   strcpy(cropCode,"BE");
   if (strcmp(seedName,"SEEDPOTATO")==0)
   strcpy(cropCode,"PO");
   if (strcmp(seedName,"SEEDMAIZE")==0)
   strcpy(cropCode,"MA");
   if (strcmp(seedName,"SEEDCLOVER")==0)
   strcpy(cropCode,"C1");
   if (strcmp(seedName,"SEEDRYE")==0)
   strcpy(cropCode,"RY");
   if (strcmp(seedName,"SEEDOAT")==0 )
   strcpy(cropCode,"O1");
   if (strcmp(seedName,"SEEDOILRADISH")==0)
   strcpy(cropCode,"OR");
   if (strcmp(seedName,"SEEDRYEGRASS")==0)
   strcpy(cropCode,"S1");
   if (strcmp(seedName,"SEEDITALRYEGRASS")==0)
   strcpy(cropCode,"IR");
   if (strcmp(seedName,"SEEDSOYBEAN")==0)
   strcpy(cropCode,"SB");
   if (strcmp(seedName,"SEEDSUNFLOWER")==0)
   strcpy(cropCode,"SF");
   if (strcmp(seedName,"SEEDLUCERNE")==0)
   strcpy(cropCode,"L1");
}

void ecosystinterfaceClass::AddOrganic(int theType, double theamount, double theDepth, double theTotalN, double theNH4N,
		double theNO3N, double thepercentDM, double theC)
{
	//convert from g/m**2 to tonnes/ha (ecosystem will convert back to g/m**2)
   double NH4N=  theNH4N/100.0;
   double NO3N =  theNO3N/100.0;
   double totalN= theTotalN/100.0;
   double orgN = totalN -(NH4N + NO3N);
   double decompC = theC/100.0;
   theamount/=100.0;
   //CK int type = int(theType);
   decomposable* aproduct =new decomposable();
   aproduct->Setamount(theamount);
   aproduct->SetDryMatter(thepercentDM/100.0);
   aproduct->SetorgN_content(orgN/theamount);
   aproduct->SetNH4_content(NH4N/theamount);
   aproduct->SetNO3_content(NO3N/theamount);
   aproduct->SetC_content(decompC/theamount);
	theEcosystem->AddDecomposable(aproduct);
   theEcosystem->Incorporate(theDepth,1.0, false, "adding organic matter");
   theOutput->AddIndicator(environmentalIndicator,"31.06 N from spread manure","kg N",totalN *1000.0);
   theOutput->AddIndicator(environmentalIndicator,"35.02 N from imported manure","kg N",totalN *1000.0);
   theOutput->AddIndicator(environmentalIndicator,"40.02 C from field-applied manure","kg C",decompC *1000.0);
}

void ecosystinterfaceClass::SetFertilisation(double theNH4N, double theNO3N)
{
	nitrogen surfNitrate = theNO3N;
   nitrogen surfAmmonium = theNH4N;
   nitrogen soilNitrate = 0.0;
   nitrogen soilAmmonium = 0.0;
	theEcosystem->AddNutrient(surfNitrate,surfAmmonium, soilNitrate, soilAmmonium);
   nitrogen Nfert = surfNitrate + surfAmmonium + soilNitrate + soilAmmonium;
   theOutput->AddIndicator(environmentalIndicator,"31.01 N from mineral fertilizer","kg N",Nfert.n *10.0);
}

