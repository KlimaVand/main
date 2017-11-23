#ifndef cropLucerne_h
  #define cropLucerne_h

#include "../../base/cloneList.h"
#include "cropRyegrass.h"

class cropLucerne: public cropRyegrass
{
protected:
   double
      MaxFixPerDM,
      DMCostPerN,
      NFixationThisDay,
      PotNFix;

   virtual double CalcPotentialNFixation(double dDryMatt);

public:
   cropLucerne(const char * aName, const int aIndex, const base * aOwner, string CropName);
   cropLucerne(const cropLucerne& acrop);
	virtual cropLucerne* clone(){return new cropLucerne(*this);};

   virtual void NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio);

   virtual void TransferDryMatterToRoot(double *dDryMatt);
   virtual double GetNFixationThisDay(){return NFixationThisDay;};
};

#endif

