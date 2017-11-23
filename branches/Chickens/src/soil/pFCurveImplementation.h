/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// pfcurve.hpp
// Soil water retension curve
// (c) Jørgen E. Olesen, Statens Planteavlsforsøg
// E-mail: jeo@pvf.sp.dk
// =============================================================================

#ifndef PFCURVE_IMPL
#define PFCURVE_IMPL

#include "../base/base.h"

struct pF_WaterData
{
   double relativeWaterContent;   // unit: mm/mm
   double pF;                     // unit: log10(-100 m H2O)
   pF_WaterData * next;
};

struct ExternalConductionData
{
   double pF;                     // unit: log10(-100 m H2O)
   double logConduc;              // unit: log10(m/s)
};

struct pF_ConductionData
{
   double pF;                     // unit: log10(-100 m H2O)
   double logConduc;              // unit: log10(m/s)
   double fluxPotential;          // unit: m²/s
   pF_ConductionData * next;
};

class pF_Curve_Implementation : public base
{
private:
   enum {maxTablePoints = 150, maxSplinePoints=100};
   int NumberOfPFCurves;
   double minpF;
   double maxpF;
   bool tabulated;
   int numExternalConduc;

   ExternalConductionData ExternalConduc[maxSplinePoints];

   double pFTable[maxTablePoints+1];
   double relativeWaterTable[maxTablePoints+1];
   double conductivityTable[maxTablePoints+1];
   double fluxPotentialTable[maxTablePoints+1];

   pF_WaterData * firstW;      // Pointer to first water content data point
   pF_ConductionData * firstC; // Pointer to first hydraulic conductivity data point
   int IntpolMethod;           // Interpolation method (0: spline, 1: linear)

   pF_Curve_Implementation& operator=(const pF_Curve_Implementation& p); // Dissable the compilers generation of default assignment operator.

   void InitVars(int Intpol);
   double GetExternalConductivity(double pF);
   double CalcpF(double RelativeWater);                // unit: log10(-100 m H2O)
   double CalcPressurePotential(double RelativeWater); // unit: - m H2O
   double CalcConductivity(double pFValue);            // unit: m/s
   double CalcFluxPotential(double pFValue);

public:
   pF_Curve_Implementation(int Intpol = 0);

   ~pF_Curve_Implementation();

   void MakeTables();

   void AddWaterItem(pF_WaterData data);
   void AddWaterData(double RelativeWater,double pFValue);
   void AddConductionItem(pF_ConductionData data);
   void AddConductionData(double pFValue,double logCond,double fluxPoten);
   void MakeHydraulicConductivity(double FieldConductivity);
   double GetpF(double RelativeWater)      // unit: log10(-100 m H2O)
   {
      if (!tabulated)
      	return CalcpF(RelativeWater);         // no table made yet
      if (RelativeWater>0.5)
      	return -1.0;                           // outside upper boundary
      double index = 2.0*RelativeWater*maxTablePoints;
      int index1   = (int) floor(index);
      double low   = pFTable[index1];
      double high  = pFTable[index1+1];
      return low + (high-low)*(index-index1);
   };
   double GetPressurePotential(double RelativeWater)     // unit: - m H2O
        {return -pow(10.,GetpF(RelativeWater))/100.;};
   double CalcRelativeWater(double pFValue);             // unit: mm/mm
   double GetRelativeWater(double pFValue);              // unit: mm/mm
   double GetConductivity(double pFValue)                // unit: m/s
   {
      pFValue=max(0.0,pFValue);
      if (!tabulated)
      	return CalcConductivity(pFValue);                   // no table made yet
      double index = (pFValue-minpF)/(maxpF-minpF)*maxTablePoints;
      int index1 = (int) floor(index);
      double low = conductivityTable[index1];
      double high = conductivityTable[index1+1];
      return low + (high-low)*(index-index1);
   };
   double GetFluxPotential(double pFValue);

   double GetNumberOfPFCurves() {return NumberOfPFCurves;};    // NumberOfPFCurves indicates how many pF_curves that points on this implementation
   void AddOnePFCurve() {NumberOfPFCurves++;};
   void RemoveOnePFCurve() {NumberOfPFCurves--;};

   void ReadParameters(fstream * file,int index,double dryBulkDensity,double clayContent,double siltContent,double carbon, bool TopSoil);
};

#endif

