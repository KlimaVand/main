/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef PHEN_H
   #define PHEN_H

#include <base.h>

// 0: emerged, 1: anthesis, 2: end grain fill, 3: ripe

class phenology : public base
{
protected:

public:
// Definition variables
	double
   	DS_Flagligule,                // DS scale for start of flag ligule
      DS_StartFill,                 // DS scale for start of grainfill
      TS0,                         	// Emergence temperature sum
      TS1,                         	// Temperature sum for Anthesis
      TS2,                         	// Temperature sum for end grain fill
      TS3,                         	// Temperature sum for ripeness
   	TB0,                          // Base temperature until emergence
      TB1,									// Base temperature until flowering
      TB2,									// Base temperature after flowering
   	TB3,                          // Base temperature for ripening
      DB,  									// Daylength response
      DMAX,                         // Daylength response
      LinearLeafPhase;              // Growth phase for linear leaf growth (related to TempSumForLeaf!)
// State variables
	double
   	DS,									// DS development stage
   	DSIncrease,                   // Daily increase
      TempSumAfterFirstMarch,       // Temperature sum after 1. march (used in some old crop models)
      TempSumForLeaf;               // Temperature som for leaf expansion (used in linear growth)

   phenology();
   virtual ~phenology();

   phenology(const phenology& aphenology);
   void Add(phenology* aPhenology, double fraction);

   bool Sown() {return DS>=-1.0;};
   bool Emerged() {return DS>=0.0;};
   bool Anthesis() {return DS>=1.0;};
   bool GrainFillStart() {return DS>=DS_StartFill;};
   bool GrainFillEnd() {return DS>=2.0;};
   bool Ripe() {return DS>=3.0;};

   double GetFractionToAnthesis();
   double GetFractionOfGrainFill();
   double GetFractionToFlagLigule();
   double LeafSenesence();

   void Sow();
   void SetDSAfterCut();
   void Update(double AirTemp,double soilTemp,double DayLength);
   void ReadParameters(char* filename,string CropName);
	void Terminate();
};

#endif
