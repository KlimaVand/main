/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../../base/common.h"
#include "cropBare.h"
#include "../../base/unix_util.h"
cropbare::cropbare(const char * aName, const int aIndex, const base * aOwner, string cropName)
         :crop(aName,aIndex,aOwner, cropName)
{
	//CropName = "NoCrop";
}

void cropbare::GetStatus  (double * aRootRadius,
									double * aRootpF,
									double * aNitrogenDemand,
									double * aNitrateUptakeRate,
									double * aAmmoniumUptakeRate,
                           double * aMinimumSoilNitrate,
                           double * aMinimumSoilAmmonium)
{
	*aRootRadius=0.05;
	*aRootpF = 4.2;
	*aNitrogenDemand=0.0;
	*aNitrateUptakeRate=0.0;
	*aAmmoniumUptakeRate=0.0;
   *aMinimumSoilNitrate=0.0;
   *aMinimumSoilAmmonium=0.0;
}

double cropbare::Update(double ActivePar)
{
   return 0.0;
}

void cropbare::Harvest(decomposable* Storage,decomposable *Straw)
{
	char buffer[3];
   itoa(Index,buffer,10);
   theMessage->FatalError("Field: ", buffer," cropbare::Harvest - this virtual crop can not be harvested");
}

