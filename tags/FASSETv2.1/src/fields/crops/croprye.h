/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef CROPRYE
   #define CROPRYE

#include <crop.h>
#include <phenology.h>

class cropRye:public crop
{
protected:
// State variables

   double
   	HeightA,                // Height constants see Olesen, 2002 for description
      HeightB,
      GrainNumber,            // Number of grains per m2
      LeafNumber,             // Number of appeared leaves
      CritLeafNo,             // Critical number of leaves
      FinalLeafNo,            // Final leaf number
      VernalIndex,            // Vernalization index
      TSum;                   // Current temperature sum [Cd]
   bool
      Sown,                   // Sown
      Germinated,             // Germinated
      Emerged,                // Emerged
      Vernalized,             // Vernalized
      CommitFlag,             // Committed to flag leaf
      Extending,              // Extending
      Anthesis;               // Past anthesis

public:
	cropRye(const char * aName, const int aIndex, const base * aOwner);
   cropRye(const cropRye& acrop);
   virtual void Sow(double SeedDensDryMatt,double RelativeDensity,double NitrogenInSeed);
   virtual double fNitrogen();
   virtual void UpdateHeight();
   virtual void ReadParameters(char* filename);
};

#endif
