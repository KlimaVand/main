/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CROPBEET_H
   #define CROPBEET_H

#include "../../base/message.h"
#include "../../soil/soil.h"
#include "crop.h"

class cropBeet : public crop
{
	protected:
		double
	   	TempSumStorage1,               			// Temperature sum for min allocation to storage
	      TempSumStorage2,             				// Temperature sum for max allocation to storage
	      MinAllocToStorage,          				// Min allocation to storage organs
	      MaxAllocToStorage;        					// Max allocation to storage organs

      virtual void CalcSenescenseSpeed(){};     // Leaves value unchanged

   public:
		cropBeet(const char * aName, const int aIndex, const base * aOwner);
      cropBeet(const cropBeet& acrop);
      virtual bool ReadyForHarvest() {return Phenology->DS>1.5;}; // May get stuck here otherwise !!!
      virtual void Add(crop* aCrop, double fraction);
   	virtual void TransferDryMatterToStorage(double * dDryMatt);
      virtual void ReadParameters(char* fileName);
};

#endif

