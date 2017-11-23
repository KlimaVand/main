/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropHairyvetch.h"
#include "../../products/products.h"

/****************************************************************************\
\****************************************************************************/
cropHairyVetch::cropHairyVetch(const char * aName, const int aIndex, const base * aOwner)
              :cropPea(aName,aIndex,aOwner)
{
   // For complete nomenclature see base class 'crop'
   CropName       = "HairyVetch";
   PlantItemName  = "HAIRYVETCH";                 // no primarly harvest item
   StrawItemName  = "HAIRYVETCH";           // These names maps to products.dat !
	WinterSeed     = true;                     // Is the present crop a wintercrop.
   GrowthContinue = false;

	//Parameters
   SpecificRootLength	 = 20.0;          // Specific root length, m pr. kg.  (corrected for change in rootradius)
	MaxRootDepthCrop 		 = 1.5;            // Maximal root depth for this crop (estimate)
	RootPentrRate    		 = 0.001;       // New estimate from KTK (18_3_3)
   MinAllocToRoot        = 0.5;         // Minimal fraction of dry matter production that is allocated to the root
	MaxAllocToRoot        = 0.8;         // Maximal fraction of dry matter production that is allocated to the root
	MaxFixPerDM 			= 0.02;			// Cannot exceed 1/DMCostPerN. Maximal nitrogen fixation per produced dry matter thius day (g/g).
	CoeffGreenIdx 			= 0.01;           // Coefficient for leaf growth (Optimized)
   MaxRadUseEff 			= 4.0;            //
	MinDMtemp				 = 0.0;                    // Minimum temperature for dry matter production
	MaxDMtemp				 = 10.0;                   // Maximum temperature for dry matter production
   DecayRoot            = 0.007;          // Root decay given as a daily fraction of DM at 10 deg. Celsius
   Phenology->TS1       = 650;            // Emergence to anthesis (estimated from irrigated and non-irrigated peas at Foulum (MEL))

}
/****************************************************************************\
\****************************************************************************/
double cropHairyVetch::Update(double ActivePar)
{
  	double DailyDMProd = cropPea::Update(ActivePar);
   return DailyDMProd;
}


