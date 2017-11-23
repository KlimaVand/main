/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "crop.h"
#ifndef CROPPEA_H
#define CROPPEA_H
class cropPea:public crop
{
protected:
   double

      MaxFixPerDM,
      PotNFix,
      NFixationThisDay,
      DMCostPerN;

   virtual double CalcPotentialNFixation(double dDryMatt);

public:
   cropPea(const char * aName, const int aIndex, const base * aOwner);
   cropPea(const cropPea& acrop);
   virtual ~cropPea(){};
   void Add(crop* aCrop, double fraction);
   virtual void NitrogenAndTranspiration(nitrogen aNitrogenUptake,double aTranspirationRatio);
   virtual void ReadParameters(char* filename);
   virtual void TransferDryMatterToRoot(double *dDryMatt);
   virtual double GetNFixationThisDay(){return NFixationThisDay;};
};

#endif
