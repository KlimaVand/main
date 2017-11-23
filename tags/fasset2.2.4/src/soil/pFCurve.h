/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// pfcurve.hpp
// Soil water retension curve
// (c) J�rgen E. Olesen, Statens Planteavlsfors�g
// E-mail: jeo@pvf.sp.dk
// =============================================================================

#ifndef PFCURVE
#define PFCURVE

#include "../base/base.h"
#include "pFCurveImplementation.h"

class pF_Curve : public base
{
private:
   pF_Curve_Implementation* aPF_Curve;
   pF_Curve(){}; // Automatically generated constructor disabled

public:
   pF_Curve(int Intpol = 0);

   pF_Curve(const pF_Curve& pFSource);
   ~pF_Curve();

   void SetIntpol(int Intpol) {aPF_Curve->SetIntpol(Intpol);};
   void AddWaterItem(pF_WaterData data) {aPF_Curve->AddWaterItem(data);};
   void AddWaterData(double RelativeWater,double pFValue) {aPF_Curve->AddWaterData(RelativeWater,pFValue);};
   void AddConductionItem(pF_ConductionData data){aPF_Curve->AddConductionItem(data);};
   void AddConductionData(double pFValue,double logCond,double fluxPoten){aPF_Curve->AddConductionData(pFValue,logCond,fluxPoten);};
   void AddExternalConductionData(double pFValue,double conduc){aPF_Curve->AddExternalConductionData(pFValue,conduc);};
   void MakeHydraulicConductivity(double FieldConductivity){aPF_Curve->MakeHydraulicConductivity(FieldConductivity);};

   double GetpF(double RelativeWater) {return aPF_Curve->GetpF(RelativeWater);};                // unit: log10(-100 m H2O)
   double GetPressurePotential(double RelativeWater) {return aPF_Curve->GetPressurePotential(RelativeWater);}; // unit: - m H2O
   double CalcRelativeWater(double pFValue) {return aPF_Curve->CalcRelativeWater(pFValue);};           // unit: mm/mm
   double GetRelativeWater(double pFValue) {return aPF_Curve->GetRelativeWater(pFValue);};           // unit: mm/mm
   double GetConductivity(double pFValue) {return aPF_Curve->GetConductivity(pFValue);};            // unit: m/s
   double GetFluxPotential(double pFValue) {return aPF_Curve->GetFluxPotential(pFValue);};
   double GetPorosity() {return aPF_Curve->GetPorosity();};
   void ReadParameters(fstream * file,int index,double dryBulkDensity,double clayContent,double siltContent,double carbon, bool TopSoil)
   	{aPF_Curve->ReadParameters(file,index,dryBulkDensity,clayContent,siltContent,carbon,TopSoil);};
};

#endif
