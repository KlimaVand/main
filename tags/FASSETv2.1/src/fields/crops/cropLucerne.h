#ifndef cropLucerne_h
  #define cropLucerne_h

#include <clonlist.h>
#include <cropgrss.h>

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
   cropLucerne(const char * aName, const int aIndex, const base * aOwner);
   cropLucerne(const cropLucerne& acrop);
	virtual cropLucerne* clone(){return new cropLucerne(*this);};
   void Add(crop* aCrop, double fraction);
   virtual void NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio);
   virtual void ReadParameters(char* filename);
   virtual void TransferDryMatterToRoot(double *dDryMatt);
   virtual double GetNFixationThisDay(){return NFixationThisDay;};
};

#endif

