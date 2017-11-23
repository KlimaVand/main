/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "cropswht.h"
#include <products.h>
/****************************************************************************\
\****************************************************************************/

cropSpringWheat::cropSpringWheat(const char * aName, const int aIndex, const base * aOwner)
                :cropSbarley(aName,aIndex,aOwner)
{
   CropName = "SpringWheat";
	PlantItemName = "SPWHT";                   // These names maps to products.dat !
   StrawItemName = "WHEATSTRAW";              // These names maps to products.dat !

	WinterSeed=0;                     // Is the present crop a wintercrop.
   MaxRootDepthCrop = 1.0;           // new estimate from KTK (18_3_3)

   // Working on these - but in principle fixed
	MaxRadUseEff=3.2; //0.8*3.33;            // 0.8 to simulate Maricopa remove !!!!!! Maximum radiation use efficiency (g/MJ)

   // Phenology
   // spring wheat estimed 2002 by combining spring barley and winter wheat
   // According to Olesen et al. 2002
   Phenology->TS0          = 125;              // Sowing to emergence (from winter wheat)
   Phenology->TS1          = 445;          	  // Emergence to anthesis (from spring barley)
   Phenology->TS2          = 310;              // Anthesis to end of grainfilling (from spring barley)
   Phenology->TS3          = 218;              // End of grainfilling to ripeness (from MARKVAND)

   //Phenology->DS_Flagligule= 0.6;              // (from spring barley)
   //Phenology->DS_StartFill = 1.2;              // (from spring barley)
   MinimumSenescense       = 0.9*0.35;            // Used to calculate minimum senescense - value as calibrated for winter wheat
   
#ifdef MELCAL
	MinN_Store = 0.016;                 // Minimum content of nitrogen i storage organs - estimated
	MaxN_Store = 0.026;                 // Maximum content of nitrogen i storage organs - estimated
#endif
}


