/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropdyerswoad.h"
#include "products.h"

/****************************************************************************\
\****************************************************************************/
cropDyersWoad::cropDyersWoad(const char * aName, const int aIndex, const base * aOwner)
              :cropSbarley(aName,aIndex,aOwner)
{
   // For complete nomenclature see base class 'crop'
   CropName       = "DyersWoad";
   PlantItemName  = "DYERSWOAD";                 // no primarly harvest item
   StrawItemName  = "DYERSWOAD";           // These names maps to products.dat !
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

}
/****************************************************************************\
\****************************************************************************/
double cropDyersWoad::Update(double ActivePar)
{
   if (topMatter!=NULL)
   {
   	delete topMatter;
      topMatter = NULL;
   }
  	double DailyDMProd = cropSbarley::Update(ActivePar);

   if (temp<-5.0 && !terminated)
   {
		topMatter = new decomposable;

   	topMatter->Setname(StrawItemName);
	   theProducts->GiveProductInformation(topMatter);
	   topMatter->Setamount(0.01*DryMatterVegTop/topMatter->GetdryMatter());

	   nitrogen StrawN = Nitrogen - NitrogenInRoot();

	   if (topMatter->GetAmount()>0)
			topMatter->SetorgN_content(StrawN/(DryMatterVegTop/topMatter->GetdryMatter()));

	   Nitrogen = Nitrogen-StrawN;
      DryMatterVegTop = 0.0;
   	terminated     = true;

      Nbudget.AddOutput(StrawN.n);                    // Budgets

   }
   return DailyDMProd;
}


