/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef cropClover_h
  #define cropClover_h

#include <clonlist.h>
#include <cropgrss.h>

class cropClover: public cropRyegrass
{
protected:
   double
      MaxFixPerDM,
      DMCostPerN,
      NFixationThisDay,
      PotNFix;

   virtual double CalcPotentialNFixation(double dDryMatt);

public:
   cropClover(const char * aName, const int aIndex, const base * aOwner);
   cropClover(const cropClover& acrop); 
	virtual cropClover* clone(){return new cropClover(*this);};
   void Add(crop* aCrop, double fraction);
   virtual void NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio);
   virtual void ReadParameters(char* filename);
   virtual void TransferDryMatterToRoot(double *dDryMatt);
   virtual double GetNFixationThisDay(){return NFixationThisDay;};
};

#endif

