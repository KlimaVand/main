#include "common.h"
#include "cropoilrad.h"
#include "products.h"

/****************************************************************************\
\****************************************************************************/
cropOilRadish::cropOilRadish(const char * aName, const int aIndex, const base * aOwner)
              :cropBeet(aName,aIndex,aOwner)
{
   // For complete nomenclature see base class 'crop'
   CropName       = "OilRadish";
   PlantItemName  = "OILRADISH";                 // no primarly harvest item
   StrawItemName  = "OILRADISH";           // These names maps to products.dat !
	WinterSeed     = true;                     // Is the present crop a wintercrop.
   GrowthContinue = false;

   // Root growth
   Phenology->TS0          = 20;              // Sowing to emergence
   //Phenology->LinearLeafPhase = 250;
	//InitialCAI = 2;

	MaxRadUseEff 		 = 3.3;        // guestimate
	MaxRootDepthCrop   = 3.0;        // new estimate from KTK (18_3_3)
	RootPentrRate      = 5.0*0.001;  // guestimate
	RootDistrParm      = 1.0;        // Root density distribution parameter

   MinN_Root     = 0.01;           // Guesstimate Minimum content of nitrogen i root organs
	MaxN_Root     = 0.01;           // Guestimate Maximum content of nitrogen i root organs

#ifdef MELCAL
//For IMPACTS
	MinAllocToRoot=0.2;
   MaxAllocToRoot=0.3;
	MaxRadUseEff 		 = 5.0;        // guestimate
   MinN_Root     = 0.005;           // Guesstimate Minimum content of nitrogen i root organs
	MaxN_Root     = 0.03;           // Guestimate Maximum content of nitrogen i root organs
// TEST - a lot more needs to be done !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   Phenology->TS1          = 235;              // Emergence to anthesis     Taken from beet!!!!!!!!!!
   Phenology->TS2          = 3650;             // Anthesis to end of grainfilling
   Phenology->TS3          = 0;                // End of grainfilling to ripeness
#endif
}
/****************************************************************************\
\****************************************************************************/
double cropOilRadish::Update(double ActivePar)
{
/*   if (topMatter!=NULL)
   {
   	delete topMatter;
      topMatter = NULL;
   }   */
  	double DailyDMProd = cropBeet::Update(ActivePar);

/*   if (temp<-5.0 && !terminated)
   {
		topMatter = new decomposable;

   	topMatter->Setname(StrawItemName);
	   theProducts->GiveProductInformation(topMatter);
	   topMatter->Setamount(0.01*DryMatterVegTop/topMatter->GetdryMatter());

	   nitrogen StrawN = Nitrogen - NitrogenInRoot(); //- NitrogenInHarvest....;   To be fixed later

	   if (topMatter->GetAmount()>0)
			topMatter->SetorgN_content(StrawN/(DryMatterVegTop/topMatter->GetdryMatter()));

	   Nitrogen = Nitrogen-StrawN;
      DryMatterVegTop = 0.0;
   	terminated     = true;

      Nbudget.AddOutput(StrawN.n); //Old, probably wrong   // Budgets
*/
/*      Nbudget.AddOutput(Nitrogen.n);                    // Budgets           To be fixed later
      N15budget.AddOutput(Nitrogen.n15);
      DMbudget.AddOutput(DryMatterRoot+DryMatterVegTop+DryMatterStorage);
*/
//   }
   return DailyDMProd;
}


