/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CROPPOTATO_H
   #define CROPPOTATO_H

#include "../../base/message.h"
#include "../../soil/soil.h"
#include "crop.h"

class cropPotato : public crop
{
	protected:
		double
	   	TempSumStorage1,               			// Temperature sum for min allocation to storage
	      TempSumStorage2,             				// Temperature sum for max allocation to storage
	      MinAllocToStorage,          				// Min allocation to storage organs
	      MaxAllocToStorage;        					// Max allocation to storage organs



   public:
		cropPotato(const char * aName, const int aIndex, const base * aOwner, string cropName);
      cropPotato(const cropPotato& acrop);
      virtual void Add(crop* aCrop, double fraction);
   	virtual void TransferDryMatterToStorage(double * dDryMatt);

   	virtual bool ReadyForHarvest() {return Phenology->GrainFillStart();}; // BMP corrected Feb. 2007
};

#endif

