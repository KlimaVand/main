/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropRyegrass.h"
#include "../../products/products.h"
#include "../../base/IndicatorOutput.h"
#include "../../base/climate.h"
#include "../../base/nixcbits.h"
#include "../../products/feedItem.h"
#include "../../data/commonData.h"

/****************************************************************************\
\****************************************************************************/
cropRyegrass::cropRyegrass(const char * aName, const int aIndex, const base * aOwner)
             :crop(aName,aIndex,aOwner)
{
	// For complete nomenclature see base class 'crop'
   CropName       = "Ryegrass";
   PlantItemName  = "RYEGRASS";         // No primarly harvest item
   StrawItemName  = "RYEGRASS";         // These names maps to products.dat !
	WinterSeed     = true;               // Is the present crop a wintercrop.
   GrowthContinue = true;
	HasBeenCut = false;
	grazableDM=0;

   //---- Parameters -----------------------------------------------------------
   WaterDeficitVegGrowth 				= 0.65;

   // Phenology
   Phenology->TS0          			= 125;              // Sowing to emergence, estimated from irrigated and non-irrigated grass on Foulum
   Phenology->TS1          			= 500;           // Emergence to anthesis, estimated from irrigated and non-irrigated grass on Foulum
   Phenology->TS2             		= 420;                 // Temperature sum for end grain fill
   Phenology->TS3             		= 155;                 // Temperature sum for ripeness
   Phenology->TB0             		= 0;                   // Base temperature until emergence
   Phenology->TB1             		= 4.0;					   // Base temperature until flowering
   Phenology->TB2             		= 6.0;						// Base temperature after flowering
   Phenology->TB3             		= 4.0;                 // Base temperature for ripening
   Phenology->DB              		= 7.0;						// Daylength response   (Weir et al.)
   Phenology->DMAX            		= 20.0;		            // Daylength response
   Phenology->DS_StartFill 			= 1.1;

 	//Dry matter accumulation
   MaxRadUseEff     						= 3.7;
   k          								= 0.44;
   MinDayLength 							= 10;
	MaxDMtemp       						= 2.0;        // Maximum temperature for dry matter production (Celcius) - guesstimate
	MinDMtemp       						= 0.0;        // Minimum temperature for dry matter production.

   // Root growth
	MaxRootDepthCrop 						= 1.5;      			// new estimate from KTK (18_3_3)
	RootPentrRate    						= 0.0004;           // new estimate from KTK (18_3_3)
	RootDistrParm    						= 5.0;             // Root density distribution parameter
	SpecificRootLength   				= 150;         // Specific root length, m pr. kg. (from Evans: Root Morpho.. NZ J. Arg.Res 1977, 20, 331-335)
   DecayRoot								= 0.002;      //estimated from Burrehoejvej data
   RhizoDepositFraction					= 0.001;      //estimated from Burrehoejvej data


	// DM allocation
   // Parameters below adjusted so that the total carbon flow matches data from Paustian et al. 1990. J. Appl. Ecol., 27, 60-84
	RespirationRoot 						= 0.0001;         // Root respiration given as a daily fraction of DM at 10 deg. Celsius, Calibrated to Burreh?jvej data
	RespirationTop  						= 0.0001;          // Top respiration given as a daily fraction of DM at 10 deg. Celsius
   MinAllocToRoot  						= 0.3;
	MaxAllocToRoot  						= 0.6;
	FillFactor								= 0.2;
   StoreForFilling		 				= 0.1;        // Fraction of DM present at initiation of grain filling that is translocated to grain
   TransferRate    						= 0.04; 		  // Fraction (temperature-dependend) of the four age pools that is transferred to subsequent fraction
	BulkDensity     						= 1000;       // g DM/m3 standing crop - guesstimated
   RecycleFraction    					= 0.15;        // Fraction of abcised DM and N that is recycled in plant (g/g).
                                    					// Estimate based on Thornley et al. 1995. Annals of Botany: 79-94
                                     					// but a simpler approach here, so that values not are completly comparable
	TransferRate     						= 0.04;      	// Fraction (temperature-dependend) of the four age pools that is transferred to subsequent fraction

	//Nitrogen
   PowDM   									= -0.38;     	//Marino et al. 2004
   NPctMax 									= 6.2;       	//Marino et al. 2004
	NPctMin 									= 1.5;       	//Marino et al. 2004
   MinN_Store    							= 0.016;      // Minimum content of nitrogen i storage organs
   MaxN_Store    							= 0.026;      // Maximum content of nitrogen i storage organs
   MinN_Root  								= 0.01;       // Whitehead: Grassland Nitrogen
   MaxN_Root  								= 0.030; 	  // Guesstimate

   // LAI and canopy structure
   LAIDistributionPar 					= 0.4;         // Estimated from Lantinga et al
	CoeffGreenIdx      					= 0.015;       // Coefficient for leaf growth  (from Nassiri & Elgersma, 1998)
	LAIDMRatio 								= 0.01;        //specific leaf area index m2/g, Calibrated to Burreh�jvej data
   InitialCAI         					= 0.1;         // Initial green leaf area index after 200 degree days (guestimate)
   MaxPlantHeight     					= 1.0;         // Max plant height
	InitialLAIIncrease 					= 2.0;

	NitrateUptakeRate						= 0.000036;  	// Uptake rate for nitrate-N g/m/d (m root length)

	liveOMD 									= 0.8;       	// Digestibility of first 3 age classes
	deadOMD 									= 0.5;       	// Digestibility of last age class

	RateOfRipeReSeed 						= 0.2;     		//Rate of storage reseeding
   ReducedNInYellow  					= 0.5;
   Conversfactor     					= 0.5;             // Part of green leaf that is converted to yellow leaf by wilting

   // Relative availability of ages classes, according to the values for
   // Thornleys age classes of leaves.
   // Note that Thornleys age class avalabilities for sheth + stem all are unity!!!
   uptakeWeight[0] = 1.00;
   uptakeWeight[1] = 0.75;
   uptakeWeight[2] = 0.50;
   uptakeWeight[3] = 0.25;
   Nbudget.SetNames(CropName,"N");
   DMbudget.SetNames(CropName,"DM");

   // State variables ----------------------------------------------------------
   TemperatureSumAfterCut = 0.0;                                             // temperature sum after last cut
   CutDelay = 0.0;                                                           // The temperature sum nessecary for obtaining 0.5 LAI after a cut
   for(int i=0;i<4;i++)
   {
   	grazedDM[i]=0.0;
      AgeClassTop[i]=0.0;
   }
   ReSeedDM = 0.0;
}

/****************************************************************************\
\****************************************************************************/
cropRyegrass::cropRyegrass(const cropRyegrass& acrop)
   : crop(acrop)
{
   BulkDensity            = acrop.BulkDensity;
   RespirationTop         = acrop.RespirationTop;
   RespirationRoot        = acrop.RespirationRoot;
   TransferRate           = acrop.TransferRate;
   RecycleFraction        = acrop.RecycleFraction;
   TemperatureSumAfterCut = acrop.TemperatureSumAfterCut;
   CutDelay               = acrop.CutDelay;
   liveOMD                = acrop.liveOMD;
   deadOMD                = acrop.deadOMD;
   grazableDM             = acrop.grazableDM;
   grazableN              = acrop.grazableN;
   for (int i=0;i<4;i++)
   {
      AgeClassTop[i]      = acrop.AgeClassTop[i];
	   grazedDM[i]         = acrop.grazedDM[i];
      uptakeWeight[i]     = acrop.uptakeWeight[i];
   }
 }


/****************************************************************************\
\****************************************************************************/
void cropRyegrass::Add(crop* aCrop, double fraction)
{
   crop::Add(aCrop,fraction);
   cropRyegrass * c = (cropRyegrass*)aCrop;

   BulkDensity            =(1-fraction)*BulkDensity + fraction*c->BulkDensity;
   RespirationTop         =(1-fraction)*RespirationTop + fraction*c->RespirationTop;
   RespirationRoot        =(1-fraction)*RespirationRoot + fraction*c->RespirationRoot;
   TransferRate           =(1-fraction)*TransferRate + fraction*c->TransferRate;
   RecycleFraction        =(1-fraction)*RecycleFraction + fraction*c->RecycleFraction;
   TemperatureSumAfterCut =(1-fraction)*TemperatureSumAfterCut + fraction*c->TemperatureSumAfterCut;
   CutDelay               =(1-fraction)*CutDelay + fraction*c->CutDelay;
   for (int i=0;i<4;i++)
   {
      AgeClassTop[i]=(1-fraction)*AgeClassTop[i]+fraction*c->AgeClassTop[i];
	   grazedDM[i]=(1-fraction)*grazedDM[i]+fraction*c->grazedDM[i];
      uptakeWeight[i]     = (1-fraction)*uptakeWeight[i]+fraction*c->uptakeWeight[i];
   }
}

/****************************************************************************\
\****************************************************************************/
cropRyegrass * cropRyegrass::clone()
{
   cropRyegrass* aClone = new cropRyegrass(*this);
   return aClone;
}

/****************************************************************************\
\****************************************************************************/
void cropRyegrass::ReadParameters(char* filename)
{
   commonData data;
   if (data.readFileWithoutExit("CropParameters.dat"))
   {
	   int sectionName=data.FindSection(CropName);
      int sectionCrop=data.FindSection("Crop");
      int maxes=max(sectionName,sectionCrop);
		if (maxes!=-1)
      {
         data.FindItem("BulkDensity",maxes,BulkDensity);
         data.FindItem("RespirationTop",maxes,RespirationTop);
         data.FindItem("RespirationRoot",maxes,RespirationRoot);
         data.FindItem("TransferRate",maxes,TransferRate);
         data.FindItem("RecycleFraction",maxes,RecycleFraction);

		}
	}
}

/****************************************************************************\
An externally given cutting height should replace fractionLeft !!!
JBE implemented this @@@
\****************************************************************************/
void cropRyegrass::GrazeOrCut(decomposable* Hay,double fractionLeft, bool graze)
{
   theMessage->WarningWithDisplay("cropRyegrass::GrazeOrCut should never be called - use cropRyegrass::Cut instead. Bj�rn, things to do !!!");
   CutOrHarvested = true;
   PlantHeight = PlantHeight*fractionLeft;
   if (GiveDryMatterVegTop()<=0.0)
      theMessage->WarningWithDisplay("cropRyegrass::Cut/Graze - vegetative top must be bigger than zero here");

   Hay->Setname(StrawItemName);
   theProducts->GiveProductInformation(Hay);
   Hay->RemoveWater();

   double aDryMatterInVegTop = GiveDryMatterVegTop();
   nitrogen aNitrogenInVegTop = NitrogenInVegTop();
   if (aDryMatterInVegTop > 0)
   {
		Hay->Setamount(aDryMatterInVegTop*(1-fractionLeft));
      Hay->SetorgN_content(aNitrogenInVegTop/(aDryMatterInVegTop*(1-fractionLeft)));    //MEL 2009
   }
   else
   	theMessage->Warning("RyeGrass:: Nothing to cut/graze");


   double WeightedAverage = 0.0;                         // JB implemented the selectively grazing 6/9/2002
   double IntakeSelectivity [] = {0.4,0.3,0.2,0.1};
#ifdef BIOMODTEST
	if (!graze)
   {
      TemperatureSumAfterCut = 0.0;                      // Reset temperature sum
      CutDelay = 300.0*DryMatterTotalStandVegTop*(1.0-fractionLeft)/350.0;       // from K. S�egaard p 21.
      for (int i=0;i<4;i++) IntakeSelectivity[i] = 0.25;
   }
#endif
   nitrogen HayN =  Hay->GetorgN_content()*Hay->GetAmount();
   Nitrogen = Nitrogen - HayN;

   double RemoveDM = (1.0-fractionLeft)*GiveDryMatterVegTop();       // DM to remove
	double Removed = RemoveDM;
	while (Removed>1e-10)
   {
      WeightedAverage = 0.0;
   	for (int i=0;i<4;i++)
   		WeightedAverage += IntakeSelectivity[i]*AgeClassTop[i];

   	for (int i=0;i<4;i++)
   	{
         double DMRemove = min(AgeClassTop[i],RemoveDM*IntakeSelectivity[i]*AgeClassTop[i]/WeightedAverage);
   		AgeClassTop[i] -= DMRemove;
         Removed -= DMRemove;
   	}
      RemoveDM = Removed;
   }

   DryMatterVegTop=GiveDryMatterVegTop();       // updates drymattervegtop
   CalcLeafAreaIndices();

   Nbudget.AddOutput(HayN.n);                    // Budgets
   N15budget.AddOutput(HayN.n15);
   DMbudget.AddOutput(Hay->GetAmount());

   double NRemain, DMRemain;
   EndBudget(&NRemain,&DMRemain);
   HasBeenCut=true;
   Phenology->SetDSAfterCut();
}

/****************************************************************************\
\****************************************************************************/
void cropRyegrass::Harvest(decomposable* Storage, decomposable* Straw)
{
   CutOrHarvested = true;
   double CuttingHeight = 0.1;
   double RatioStrawCut = (PlantHeight-CuttingHeight)/PlantHeight;
//   Storage->Setname(PlantItemName);
//   if (PlantItemName!="")
   Storage->Setname("GRASSSEED");
//   if (PlantItemName!="")
   {
	   theProducts->GiveProductInformation(Storage);
	   Storage->Setamount(DryMatterStorage/Storage->GetdryMatter());
   }

//   Straw->Setname(StrawItemName);
   Straw->Setname("GRASSSEEDSTRAW");
   theProducts->GiveProductInformation(Straw);
   Straw->Setamount(GiveDryMatterVegTop()*RatioStrawCut/Straw->GetdryMatter());

   // Nitrogen
	nitrogen RootN, StorageN, StrawN;

   //check amount of nitrogen at harvest
   double minimumN = Nmin();
   double maximumN = Nmax();

   if (minimumN > Nitrogen.n * 1.25)  //Corrected MEL 2009
   {
      cout << GetLongName() << endl;
   	theMessage->WarningWithDisplay("crop::Harvest - not enough nitrogen in plant at harvest");
   }
   if (maximumN < Nitrogen.n * 0.75)  //Corrected MEL 2009
   {
      cout << GetLongName() << endl;
	   theMessage->WarningWithDisplay("crop::Harvest - too much nitrogen in plant at harvest");
   }
   //partition between compartments
   double fN = fNitrogen();
  	double TotalRootN  = (MinN_Root+fN*(MaxN_Root-MinN_Root))*DryMatterRoot;
   double TotStorageN = (MinN_Store+fN*(MaxN_Store-MinN_Store))*DryMatterStorage;
   double N15Ratio    = Nitrogen.Get15NRatio();
   double TotalStrawN =  (NpctMinVegTop()/100+fN*(NpctMaxVegTop()-NpctMinVegTop())/100)*GiveDryMatterVegTop();
   double Nrest = Nitrogen.n - TotalRootN - TotStorageN - TotalStrawN;

   if ((Nrest<-0.01)||(Nrest>0.01))
   {
      theMessage->WarningWithDisplay("crop::Harvest - Nitrogen content adjusted");
   	double RedFac = (Nitrogen.n+Nrest)/Nitrogen.n;
  		TotalRootN = RedFac*TotalRootN;
   	TotStorageN = RedFac*TotStorageN;
   }


   RootN.SetBoth(TotalRootN,N15Ratio*TotalRootN);
   StorageN.SetBoth(TotStorageN,N15Ratio*TotStorageN);
   StrawN = Nitrogen - RootN - StorageN;
   if (Storage->GetAmount()>0)
   	Storage->SetorgN_content(StorageN/Storage->GetAmount());
   if (Straw->GetAmount()>0)
		Straw->SetorgN_content(StrawN/Straw->GetAmount());

	Nitrogen = Nitrogen-StorageN -StrawN;
	if (!Phenology->Ripe())
      theMessage->Warning("crop::Harvest - crop not ready for harvest");
   if (GiveDryMatterVegTop()<0 || DryMatterStorage<0 || StorageN.n<0 || StrawN.n<0 || Nitrogen.n<0)
   {
	   theMessage->WarningWithDisplay("crop:: Harvest values should not be negative");
      StorageN.n = max(0.0,StorageN.n);
      StrawN.n = max(0.0,StrawN.n);
	}

   Nbudget.AddOutput(StorageN.n+StrawN.n);                    // Budgets
   N15budget.AddOutput(StorageN.n15+StrawN.n15);                    // Budgets
   DMbudget.AddOutput(GiveDryMatterVegTop()*RatioStrawCut+DryMatterStorage);

   for (int i=0;i<4;i++)
   	AgeClassTop[i] -= RatioStrawCut*AgeClassTop[i];
	DryMatterStorage   = 0.0;
	GreenCropAreaIndex = 0.1;         //TEST
	YellowCropAreaIndex= 0.2;
   PlantHeight        = 0.1;                      // Modify to real cutting height!!!!!
	topMatter          = NULL;

   double NRemain, DMRemain;
   EndBudget(&NRemain,&DMRemain);
   Phenology->SetDSAfterCut();
   HasBeenCut = true;
}
/****************************************************************************\
Ripe senescence including DS setback and loss of grains to topmatter
\****************************************************************************/
void cropRyegrass::RipeSenescence()
{
   Phenology->SetDSAfterCut();
   if (DryMatterStorage> 0.0)
   {
   	nitrogen aNitrogenInStorage = NitrogenInStorage();
      double aDryMatterStorage = GiveDryMatterStorage();
		double RSeneStorage=min(RateOfRipeReSeed*TemperatureEffect(temp)*aDryMatterStorage,aDryMatterStorage);

   	if (RSeneStorage>0.0)
   	{

      	DryMatterStorage -= RSeneStorage;

      	// Transfer DM to ReSeedDM
         ReSeedDM += RSeneStorage;
       }
   }

      //Hvad mere??
}
/****************************************************************************\
\****************************************************************************/
void cropRyegrass::RootDecay(double RootTranslocation)
{
   crop::RootDecay(RootTranslocation);
   RootRespiration = 0;

   // Root respiration
   if (DryMatterRoot>0 && temp>0)
   {
      double SoilTemp=aSoil->GetTemperature(200);
    	RootRespiration  = DryMatterRoot*RespirationRoot*TemperatureEffect(SoilTemp);
    	DryMatterRoot -= RootRespiration;
      DMbudget.AddOutput(RootRespiration);
      AccumulatedRootRespiration += RootRespiration;
   	AccumulatedDMProduction -= RootRespiration;         // Respiration are not included in this budget!
      RootTranslocation -=  RootRespiration;
   }
   //MEL 2009
   double minimumN = Nmin();
   double maximumN = Nmax();
   if ((DryMatterStorage + DryMatterRoot + DryMatterVegTop > 0.0) && (Nitrogen.n > 0.5))
   {
   	if (minimumN > Nitrogen.n * 1.25)
      {
      	cout << GetLongName() << endl;
   		theMessage->WarningWithDisplay("crop::RootDecay - not enough nitrogen in plant");
   	}
   	if (maximumN < Nitrogen.n * 0.75)
   	{
      	cout << GetLongName() << endl;
	   	theMessage->WarningWithDisplay("crop::RootDecay - too much nitrogen in plant");
      }
   }

}


/****************************************************************************\
\****************************************************************************/
double cropRyegrass::TopRespiration()
{
   double topRespiration=0.0;
   double RespirationClass[] = {1.0,1.0,0.75,0.5};      // respiration dependent on class number according to thornley p. 38
	for (int i=0;i<4;i++)
   {

      double RespCoeff = RespirationClass[i]*RespirationTop*TemperatureEffect(temp);
      double r=AgeClassTop[i]*RespCoeff;

      AgeClassTop[i] -= r;
      topRespiration += r;
   }
   DMbudget.AddOutput(topRespiration);
   AccumulatedDMProduction -= topRespiration;

   //Test nitrogen MEL 2009
   double minimumN = Nmin();
   double maximumN = Nmax();
   if ((DryMatterStorage + DryMatterRoot + DryMatterVegTop > 0.0) && (Nitrogen.n > 0.5))
   {
   	if (minimumN > Nitrogen.n * 1.25)
   	{
   		cout << GetLongName() << endl;
      	theMessage->WarningWithDisplay("crop::TopRespiration - not enough nitrogen in plant");
   	}
   	if (maximumN < Nitrogen.n * 0.75)
   	{
   		cout << GetLongName() << endl;
	   	theMessage->WarningWithDisplay("crop::TopRespiration - too much nitrogen in plant");
   	}
   }

   return topRespiration;
}

/****************************************************************************\
\****************************************************************************/
double cropRyegrass::UpdateAgeClasses(double deltaDryMatt)
{
   double N,DM;
   double ReducFac = 1.0;
   if (YellowCropAreaIndex+GreenCropAreaIndex > 0.0)
   	ReducFac = (ReducedNInYellow*YellowCropAreaIndex+GreenCropAreaIndex)/
                        (YellowCropAreaIndex+GreenCropAreaIndex);
   nitrogen aNitrogenInVegTop = NitrogenInVegTop();
   double aDryMatterVegTop = GiveDryMatterVegTop();
	if (topMatter!=NULL)
   {
      delete topMatter;
      topMatter=NULL;
   }
   double transfer[4];
   double T=TransferRate*TemperatureEffect(temp);
   transfer[0]=deltaDryMatt;
   transfer[1]=2.0*T*AgeClassTop[0];
   transfer[2]=T*AgeClassTop[1];
   transfer[3]=T*AgeClassTop[2];

   double RF = RecycleFraction;
   if(Phenology->Anthesis())
      RF = 0.5*RecycleFraction;

   double senescenceDM;
   senescenceDM=(1.0-RF)*T*AgeClassTop[3];
   if (Phenology->DS > 1.5)
   	senescenceDM = T*GiveDryMatterVegTop();
   double recycledDM=RF*T*AgeClassTop[3];

   	transfer[0]+=recycledDM;

   nitrogen senescenceN;
   nitrogen grazedN;
   if (senescenceDM>0.0)
   {
     	double NConcentrationInYellow = ReducedNInYellow*aNitrogenInVegTop.n/aDryMatterVegTop/ReducFac;   //MEL 2009
//		double TotalLostN = NConcentrationInYellow*(1.0-RF)*senescenceDM;
		double TotalLostN = NConcentrationInYellow*senescenceDM;
       senescenceN.SetBoth(TotalLostN*(1.0-Nitrogen.Get15NRatio()),TotalLostN*Nitrogen.Get15NRatio());
      if (senescenceN.n<0.0)
   	{
      	theMessage->Warning("cropRyeGrass:: negative N loss");
         senescenceN.Clear();
      }

      // Transfer DM and N senesced to "topMatter"
      topMatter=new decomposable;
      topMatter->Setname("RYEGRASS");
      theProducts->GiveProductInformation(topMatter);
    	topMatter->Setamount(0.01*senescenceDM/topMatter->GetdryMatter()); // Conversion from g/m2 to t/ha
      topMatter->SetorgN_content(senescenceN/(senescenceDM/topMatter->GetdryMatter()));

      AccumulatedTopDeposit += senescenceDM;     // Accumulated top deposition (state variable)
   	AccumulatedTopNDeposit = AccumulatedTopNDeposit + senescenceN;
   }

   AgeClassTop[0]+=transfer[0]-transfer[1] - grazedDM[0];
   if (AgeClassTop[0]<0.0)
   {
      theMessage->Warning("cropRyeGrass::UpdateAgeClasses - grazing (or transfer) has resultet in negative DM in age class 0");
      AgeClassTop[0]=1.0; // Hack to get movin' !!!
   }
   AgeClassTop[1]+=transfer[1]-transfer[2] - grazedDM[1];
   if (AgeClassTop[1]<0.0)
   {
      theMessage->Warning("cropRyeGrass::UpdateAgeClasses - grazing (or transfer) has resultet in negative DM in age class 1");
      AgeClassTop[1]=1.0; // Hack to get movin' !!!
   }
   AgeClassTop[2]+=transfer[2]-transfer[3] - grazedDM[2];
   if (AgeClassTop[2]<0.0)
   {
      theMessage->Warning("cropRyeGrass::UpdateAgeClasses - grazing (or transfer) has resultet in negative DM in age class 2");
      AgeClassTop[2]=1.0; // Hack to get movin' !!!
   }
   AgeClassTop[3]+=transfer[3];
   double transferFromAgeClasses = recycledDM + senescenceDM + grazedDM[3];
   if (transferFromAgeClasses>0.0)
   	RemoveDMFromOldestAgeClass(transferFromAgeClasses);

   if (AgeClassTop[3]<0.0)
   {
      theMessage->WarningWithDisplay("cropRyeGrass::UpdateAgeClasses - grazing (or transfer) has resultet in negative DM in age class 3");
      AgeClassTop[3]=1.0; // Hack to get movin' !!!
   }
   //Budget for grazing losses - NJH 13.12.2000
	double totDMGrazed = grazedDM[0] + grazedDM[1] + grazedDM[2] + grazedDM[3];
   double NC = 0.0;
   if (grazableDM>1E-3) // Changed BMP!!!
   {	
      NC = grazableN/grazableDM;
   }
   double TotalgrazedN = NC*totDMGrazed; // To correct for changed conditions relative to corresponding prior step
   grazedN.SetBoth(TotalgrazedN*(1.0-Nitrogen.Get15NRatio()),TotalgrazedN*Nitrogen.Get15NRatio());
   nitrogen lostN=senescenceN + grazedN;
   double lostDM=senescenceDM + totDMGrazed;
   Nitrogen = Nitrogen - lostN;
   Nbudget.AddOutput(lostN.n);										// budgets
	N15budget.AddOutput(lostN.n15);										// budgets
   DMbudget.AddOutput(lostDM);

   if (Nitrogen.n<0.0)
   {
      theMessage->Warning("cropRyeGrass::UpdateAgeClasses - grazing has resultet in negative N");
      Nitrogen.n=0.1; // Hack to get movin' !!!
   }
   /*zero grazedDM and grazedN @@@
   Now reset in ClearTemporaryVariables, called from grazing_manager_class
   for(int i=0;i<4;i++)
      grazedDM[i]=0.0;
   */
   EndBudget(&N,&DM);
   return deltaDryMatt-lostDM;
}

/****************************************************************************\
\****************************************************************************/
double cropRyegrass::GiveDryMatterVegTop()
{
   double retVal=0.0;
   for (int i=0;i<4;i++)
      retVal+=AgeClassTop[i];
   if (retVal<0.0)
      theMessage->WarningWithDisplay("cropRyegrass::GiveDryMatterVegTop - negative dry matter");
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double cropRyegrass::DeadDMTop()
{
	double retval = 0.5*AgeClassTop[3];
//   if (Phenology->Anthesis())
//   retval = (0.25*(Phenology->DS) )*  GiveDryMatterVegTop();
   return retval;
}

/****************************************************************************\
\****************************************************************************/
void cropRyegrass::CalcLeafAreaIndices()
{
   if (Phenology->Emerged() && ((GiveDryMatterVegTop()-DeadDMTop())>0))
	{
	   if (Phenology->TempSumForLeaf<=Phenology->LinearLeafPhase)
      	GreenCropAreaIndex =  InitialCAI*RelativeDensity*(Phenology->TempSumForLeaf/Phenology->LinearLeafPhase);      // value from Porter ....
      else
      {
      	if ((HasBeenCut) || (UnderSown))  // if the grass has not been cut and is not undersown => more vigorous growth
      		GreenCropAreaIndex  = pow(LAIDMRatio*(GiveDryMatterVegTop()-DeadDMTop()),0.726);     //this power is from Lemaire: Diagnosis of the n status in crops pp65.
         else                              // if the grass has not been cut and is not undersown => more vigorous growth
            GreenCropAreaIndex  = InitialLAIIncrease*pow(LAIDMRatio*(GiveDryMatterVegTop()-DeadDMTop()),0.726);     // estimate
      }
   }

   else
	   GreenCropAreaIndex = 0.0;
   // JB removed the out-commenting of the cut delay
   if (TemperatureSumAfterCut<CutDelay && DryMatterTotalStandVegTop>0)
   	GreenCropAreaIndex = 0.5*TemperatureSumAfterCut/CutDelay*GiveDryMatterVegTop()/DryMatterTotalStandVegTop;
   YellowCropAreaIndex = LAIDMRatio*DeadDMTop();
}

/****************************************************************************\
\****************************************************************************/
double cropRyegrass::Update(double ActivePar)
{
   double DailyDMGrowth = 0.0;
//   rootMatter = NULL;    NJH pinged this out March 2009
	Rg 	= ActivePar;
   temp = theClimate->tmean;

	if (Phenology->Sown())                              // Never stops
   {
      if (aSoil==NULL)
         theMessage->FatalError("cropRyegrass::Update - Invalid soil pointer");
   	double soilTemp=aSoil->GetTemperature(20);
      double DayLength = theClimate->PhotoPeriod();
 //     if ((!Phenology -> Anthesis()) || (!UnderSown))
      	Phenology->Update(temp,soilTemp,DayLength);      // updates phenology


		TempSumRoot += max(0.0,temp);
      TemperatureSumAfterCut += max(0.0,temp);
		UpdateHeight();
      double deltaDryMatt=DeltaDryMatter();
      DailyDMGrowth = deltaDryMatt;

   	DMbudget.AddInput(deltaDryMatt);              // Budgets
      AccumulatedDMProduction += deltaDryMatt;

      if (Phenology->TempSumForLeaf<=200)
      {
         double DMTransfer=2.0*TopFraction*InitialSeedDM*(max(0.0,temp)/Phenology->LinearLeafPhase);
         DMTransfer = min(DMTransfer,SeedDM);
         SeedDM -= DMTransfer;
         if (ReSeedDM >0.0)
         {
         	double ReSeedDMTransfer = 2.0*TopFraction*InitialSeedDM*(max(0.0,temp)/Phenology->LinearLeafPhase);
         	ReSeedDMTransfer = min(ReSeedDMTransfer,ReSeedDM);
            DMTransfer += ReSeedDMTransfer;

         }
         AgeClassTop[0] += 0.5*(DMTransfer);
         DryMatterRoot  += 0.5*(DMTransfer);
      }

      TransferDryMatterToRoot(&deltaDryMatt);
      CalcRootGrowth();
      	TransferDryMatterToStorage(&deltaDryMatt);
      DeltaDMTop = UpdateAgeClasses(deltaDryMatt);

      double Respiration = TopRespiration();
      DeltaDMTop -= Respiration;
      DailyDMGrowth -= Respiration;
      DryMatterVegTop=GiveDryMatterVegTop();
      if (Phenology->Ripe() || DayLength < MinDayLength)
      	RipeSenescence();    // Still some debugging to do!!!!!!!!!!!!!!!!!!!!!!!
      CalcLeafAreaIndices();
	}
   return DailyDMGrowth;
}
/****************************************************************************\
MEL 2009: Removes DM from oldest ageclass
\****************************************************************************/
void cropRyegrass::RemoveDMFromOldestAgeClass(double transferFromAgeClass)
{
	double remove=0.0;
	   for(int i=0;i<4;i++)
   {
   	remove=min(AgeClassTop[3-i],transferFromAgeClass);
      AgeClassTop[3-i] -= remove;
      transferFromAgeClass -= remove;
   }

}
/****************************************************************************\
MEL 2009: Storage added to grass to simulate seedgrass
Limitation to the transfer of DM to storage according
to the concept of Justes et al. 1994. Annals of Botany 74:397-407.
\****************************************************************************/
void cropRyegrass::TransferDryMatterToStorage(double * dDryMatt)
{
	if (Phenology->Anthesis() && FillFlag==0)
   {
   	if (TransferableStorage<1E-8)
      	TransferableStorage = GiveDryMatterVegTop()*StoreForFilling;
      TransferableStorage += *dDryMatt*FillFactor;
      TransferedDMToStorage = TransferableStorage;
   }
   // MEL 2009
   if (Phenology->GrainFillStart())
   {
   	FillFlag = 1;
      if (fNitrogen() > 0.0)
      {
      	double transfer = max(0.0,TransferableStorage*Phenology->GetFractionOfGrainFill());     // linear fill of DM stored during lag phase
         RemoveDMFromOldestAgeClass(transfer);
         DryMatterStorage += transfer+*dDryMatt*FillFactor*ConversionCoefficient;


   		double minimumN = Nmin();
   		double maximumN = Nmax();
      	if (minimumN > Nitrogen.n + 0.01)    //Check nitrogen status and transfer back
         {
            double transferBack = max(0.0,min(transfer,(minimumN - Nitrogen.n)/MinN_Store));
         	RemoveDMFromOldestAgeClass(-transferBack);
         	DryMatterStorage -= transferBack;
         	transfer = transfer - transferBack;
         }
   		if (maximumN < Nitrogen.n - 0.01)     //Check nitrogen status and transfer back
   		{
            double transferBack = max(0.0,min(transfer,(Nitrogen.n - maximumN)/MaxN_Store));
         	RemoveDMFromOldestAgeClass(-transferBack);
         	DryMatterStorage -= transferBack;
         	transfer = transfer - transferBack;
 	     	}

         DMbudget.AddOutput((*dDryMatt*FillFactor)*(1-ConversionCoefficient));
         *dDryMatt = *dDryMatt*(1-FillFactor);
         TransferedDMToStorage -= transfer;
      }
   }
   if (Phenology->GrainFillEnd() && TransferedDMToStorage>0)             // Check to see if all transferable storage is transfered
   {
      double transfer=max(0.0,TransferedDMToStorage);
   	DryMatterStorage += transfer;
      RemoveDMFromOldestAgeClass(transfer);
      double minimumN = Nmin();
   	double maximumN = Nmax();
      if (minimumN > Nitrogen.n + 0.01)       //Check nitrogen status and transfer back
      {
         double transferBack = max(0.0,min(transfer,(minimumN - Nitrogen.n)/MinN_Store));
      	RemoveDMFromOldestAgeClass(-transferBack);
        	DryMatterStorage -= transferBack;
         transfer = transfer - transferBack;
      }
   	if (maximumN < Nitrogen.n - 0.01)    //Check nitrogen status and transfer back
   	{
         double transferBack = max(0.0,min(transfer,(Nitrogen.n - maximumN)/MaxN_Store));
       	RemoveDMFromOldestAgeClass(-transferBack);
        	DryMatterStorage -= transferBack;
         transfer = transfer - transferBack;
      }
      TransferedDMToStorage = 0.0;
   }
}

/****************************************************************************\
\****************************************************************************/
void cropRyegrass::UpdateHeight()
{
	if (Phenology->Emerged())
	   PlantHeight += max(0.0,temp)/1000.0*MaxPlantHeight; // JBE's original formulation, seems to work very well with undersown crops
   else
   	PlantHeight = 0.0;
   PlantHeight = min(min(PlantHeight,MaxPlantHeight),DryMatterTotalStandVegTop/BulkDensity); // But modifiy if DM becomes limiting
   // Transitions between undersown crops and removal of previous main crop might need later attention !!!
}

/****************************************************************************\
\****************************************************************************/
double cropRyegrass::GiveEvapFactor()
{
   return 1.0;
}

/****************************************************************************\
Cut and return a plantItem     NJH March 2001
returns cut material in g/sq metre
\****************************************************************************/
void cropRyegrass::Cut(plantItem * cutPlantMaterial, double cut_height)
{
   if (cut_height<0.0)
      cut_height=0.01;
   if (GiveDryMatterVegTop()<=0.0)
      theMessage->WarningWithDisplay("cropRyegrass::Cut - vegetative top must be bigger than zero here");
/*-------------------------------
   double temp_height, tempDM, proportionCut;
   grazableDM = 0.0;
   for(int i=0;i<4;i++)
   {
      temp_height=PlantHeight;
      double cutH=max(cutHeight,0.01);
      if (temp_height>cutH)
       	proportionCut=(temp_height - cutH)/temp_height;
      else
      	proportionCut = 0.0;
      tempDM=proportionCut * AgeClassTop[i] * uptakeWeight[i];
	   grazableDM +=tempDM;
   }
   return grazableDM;
-------------------------------*/
   if (cutPlantMaterial->Getname()=="") // This might need expansion in the future. For FarmN purpose this will probably do. !!!
   {
      cutPlantMaterial->Setname(PlantItemName);
      theProducts->GiveProductInformation(cutPlantMaterial);
   }
   double temp_height, tempDM, proportionCut;
   double cutDM = 0.0;
   nitrogen cutN = 0.0;
   double initialDM=GiveDryMatterVegTop();
   nitrogen  aNitrogenInVegTop = NitrogenInVegTop();
   for(int i=0;i<4;i++)
   {
      temp_height=PlantHeight;
      if (temp_height>cut_height)
      	proportionCut = (temp_height-cut_height)/temp_height;
      else
      	proportionCut = 0.0;

      tempDM = proportionCut*AgeClassTop[i];
      AgeClassTop[i]-= tempDM;   //cut material
	   cutDM += tempDM;
   }
   if (cutDM>0)
  	{
      cutN = aNitrogenInVegTop*cutDM/initialDM;
      Nitrogen = Nitrogen-cutN;
   }
   if (DryMatterStorage > 0.0)
   {
   	nitrogen aNitrogenInStorage=NitrogenInStorage().n;
      cutDM += DryMatterStorage;
      cutN.n += aNitrogenInStorage.n;
      DryMatterStorage=0.0;
      Nitrogen.n -=  aNitrogenInStorage.n;
   }

   if (cutPlantMaterial->GetdryMatter()<0.01)
   {
      theMessage->FatalError("cropRyegrass::Cut - dry matter content is below 1 %");
      cutPlantMaterial->SetDryMatter(0.10); // In order to be able to debug
   }
   cutPlantMaterial->Setamount(cutDM/cutPlantMaterial->GetdryMatter());     //adjust for DM content
   if (cutPlantMaterial->GetAmount()>0.0)
	   cutPlantMaterial->SetorgN_content(cutN/cutPlantMaterial->GetAmount());
   Nbudget.AddOutput(cutN.n);                    // Budgets
   DMbudget.AddOutput(cutDM);
   if (GiveDryMatterVegTop()<=0.0)
      theMessage->WarningWithDisplay("cropRyegrass::Cut - zero or negative amount of dry matter in veg. top");
   HasBeenCut = true;
   Phenology->SetDSAfterCut();
}

/****************************************************************************\
\****************************************************************************/
double cropRyegrass::Nmax()
{
   double StdNmax = crop::Nmax();
//   if (!CutOrHarvested)
//   	StdNmax = RelativeDensity*StdNmax;  Deleted MEL 2009!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   return StdNmax;
}

/****************************************************************************\
\****************************************************************************/
double cropRyegrass::GiveCropHeight()
{
   return PlantHeight;
}

/****************************************************************************\
Calculates grazable DM, N,C
Loads information on the available material into myProduct
Returns amount in g fresh weight/sq metre
\****************************************************************************/
feedItem *  cropRyegrass::GetAvailability(double cutHeight, int animalType)
{
	feedItem *myProduct = new feedItem(*(feedItem *)theProducts->GetProductElement(465));  //set product to grass, 100kgN, cut after 2 weeks (N content will be modified)
	myProduct->Setamount(0.0);
   double OMD=0;
   double temp_height, tempDM, proportionCut;
   double tempDigest = 0.0;
   grazableDM = 0.0;
   grazableN = 0.0;

   double N_conc = NitrogenInVegTop().n/GiveDryMatterVegTop();
   liveOMD = 0.4 + 15.4 * (N_conc-0.006);
   if (liveOMD>0.8)
   	liveOMD = 0.8;
   if (liveOMD<deadOMD)
   	liveOMD = deadOMD;

   for(int i=0;i<4;i++)
   {
      temp_height=PlantHeight;
      if (temp_height>(cutHeight+0.01)) // Allways leave 1 cm
      	proportionCut=(temp_height - cutHeight)/temp_height;
      else
      	proportionCut = 0.0;

      tempDM=proportionCut * AgeClassTop[i] * uptakeWeight[i];
      if (i<3)
         tempDigest += tempDM * liveOMD * uptakeWeight[i];
      else
         tempDigest += tempDM * deadOMD * uptakeWeight[i];
	   grazableDM +=tempDM;
   }
   if (grazableDM>0)
   	{
         grazableN =grazableDM * NitrogenInVegTop().n/GiveDryMatterVegTop();
         myProduct->Setamount(grazableDM/myProduct->GetdryMatter());
         myProduct->SetorgN_content(grazableN/myProduct->GetAmount());
         switch (animalType)
         {
            case 1:
            {
               OMD=tempDigest/grazableDM;
               // FUDGE!!!------------------------------------------------------
               /*
               OMD=0.73+grazableDM*0.0001;
               if (OMD>0.77)
                  OMD=0.77+grazableDM*0.000001; // Worst hack' ever made (since the last time at least) !!!
               if (OMD>0.78)
                  OMD=0.78;
               */
               // --------------------------------------------------------------
               myProduct->SetOMD(OMD);
               myProduct->CalcME();
               myProduct->CalcFE();
               myProduct->CalcFill(2);
               double MEPerKg = 0.16*myProduct->GetOMD()*100.0 - 1.8;  // from SCA 1990
               myProduct->SetME(MEPerKg * myProduct->GetdryMatter());
               break;
            }
            default:
            {
               theMessage->FatalError("cropRyegrass::GetAvailability - not an allowed animal type");
               break;
            }
         }
      }
	return myProduct;
}

/****************************************************************************\
Returns DM above the 'cut height'
\****************************************************************************/
double cropRyegrass::GetAvailableStandingDM(double cutHeight,bool useUptakeWeight)
{
   double temp_height, tempDM, proportionCut;
   grazableDM = 0.0;
   for(int i=0;i<4;i++)
   {
      temp_height=PlantHeight;
      double cutH=max(cutHeight,0.01);
      if (temp_height>cutH)
       	proportionCut=(temp_height - cutH)/temp_height;
      else
      	proportionCut = 0.0;
      double w=1.0;
      if (useUptakeWeight)
         w=uptakeWeight[i];
      tempDM=proportionCut * AgeClassTop[i] * w;
	   grazableDM +=tempDM;
   }
   return grazableDM;

   /* NJH code @@@
   double cropHeight = GiveCropHeight();
   double availableDM = 0.0;
   if (cropHeight>cutHeight)
   	availableDM=(1.0-(cutHeight/cropHeight))*DryMatterTotalStand;
   return availableDM;
   */

}

/****************************************************************************\
Adds an animal's contribution to the patch's grazed material
Each animal calls this routine if it grazes this patch
\****************************************************************************/
void cropRyegrass::SetGrazed(double grazing_height, double DMGrazed)
{
	double temp_height=0.0;
   double proportionCut=0.0;
   double availDM;
   for(int i=0;i<4;i++)
   {
      temp_height=PlantHeight;
      if (temp_height>(grazing_height+0.01)) // Allways leave 1 cm
      	proportionCut = (temp_height - grazing_height)/temp_height;
      else
      	proportionCut = 0.0;
      availDM = proportionCut*AgeClassTop[i] * uptakeWeight[i];

		// Material is removed by grazing in proportion to its contribution to grazable DM
      if (grazableDM>0.0)
      	grazedDM[i]+=DMGrazed * availDM/grazableDM;
   }
}

/****************************************************************************\
\****************************************************************************/
double cropRyegrass::GiveTotalDryMatter()
{
   double ret_val=0.0;
   ret_val=GiveDryMatterVegTop()+ GiveDryMatterRoot();
   return ret_val;
}

/****************************************************************************\
NJH - Aug 2001 - as for crop.cpp but checks to make sure grazing has not
occurred on the day of ploughing
\****************************************************************************/
void cropRyegrass::Terminate(decomposable* Straw,decomposable* DeadRoot,double *& RootLengthList)
{
   for(int i=0;i<4;i++)
      if (grazedDM[i]>0)
         theMessage->FatalError("cropRyegrass::Terminate - terminating a crop on same day as it was grazed");
   DryMatterRoot += ReSeedDM;
   ReSeedDM = 0.0;
/*   Nitrogen = Nitrogen + ReSeedN;
   ReSeedN.SetBoth(0.0,0.0);     */
   crop::Terminate(Straw,DeadRoot,RootLengthList);
}

/****************************************************************************\
NJH 2005 return the amount of DM grazed during the last period, then zero the values
\****************************************************************************/
double cropRyegrass::GetGrazedDM()
{
	double ret_val=0.0;
   for(int i=0;i<4;i++)
   {
      ret_val+=grazedDM[i];
//      grazedDM[i]=0.0;
   }
   return ret_val;
}

/****************************************************************************\
\****************************************************************************/
void cropRyegrass::ClearTemporaryVariables()
{
   for(int i=0;i<4;i++)
      grazedDM[i]=0.0;
}
/****************************************************************************\
\****************************************************************************/
bool cropRyegrass::EndBudget(double * NRemain, double * DMRemain)
{
	bool retVal=true;
  	*NRemain=Nitrogen.n;
  	*DMRemain=GiveDryMatterVegTop()+DryMatterRoot+DryMatterStorage+SeedDM+ReSeedDM;
   if (!Nbudget.Balance(*NRemain))
   	retVal=false;
   if (!DMbudget.Balance(*DMRemain))
   {
   	retVal=false;
      cout << "Dry matter " << GiveDryMatterVegTop() << " " << DryMatterRoot
           << " " << DryMatterStorage << " " << SeedDM << " "  << ReSeedDM << endl; // !!!!!!
      cout << "";
   }
   double N15Remain = Nitrogen.n15;
   if (!N15budget.Balance(N15Remain))
   	retVal=false;
   return retVal;
}


