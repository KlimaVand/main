/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// Soil biomatter pool
// (c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning 1998-2000
// =============================================================================

#ifndef BIOMATT_H
   #define BIOMATT_H

#include "matter.h"

class bioMatter : public matter
{
   protected:
      double
         lackOfN,
         cnRatioDefault,
         minCNratio,
         maintenance,
         deathRate,
         utilizationEfficiency;

      cnMatter maintenanceDerivative[5];
      cnMatter intermedCO2;

    public:
       bioMatter(const char * Pname,const int Index,const base * owner);
       bioMatter(const bioMatter &source);
       virtual bioMatter* clone() const;
       virtual void Add(matter * source,double fraction);
       virtual void Initialize(double clayEff1,double clayEff2,int clayResp);
       virtual void ReadParameters(fstream * file);
       virtual void Update(double environmentEffect,int mode);
       virtual void CatchInflux(cnMatter &content,int mode);

       virtual void SetDeathRate(double d) {deathRate=d;};
       virtual void SetMaintenanceRate(double m) {maintenance=m;};
       virtual double GetDeathRate() {return deathRate;};
       virtual double GetMaintenanceRate() {return maintenance;};
       virtual double GetUtilizationEfficiency() {return utilizationEfficiency;};
       virtual bool SetParameterByName(string parmName,double val);
       virtual double GetCNcrit();
       virtual double GetDefaultCNratio(){return cnRatioDefault;};
       virtual double PotentialNitrogenMineralisation();
       virtual double PotentialNitrogenImmobilisation();
       virtual double ResidualCarbon();

       virtual void FinalExport(double ratio);
       virtual bool Hungry();
       virtual double GetMicrobialPoolNdesire(){return lackOfN;};
       virtual void GiveMicrobialPoolExtraN(double r);
       virtual cnMatter GetCarbon();
       virtual bool IsBiomass()const {return true;};
       virtual void SetCarbon(cnMatter c);
       virtual double GetTurnoverRatio(){return deathRate/(maintenance+deathRate);};
       virtual double PotentialCarbonTurnover(){return carbon.c*(maintenance+deathRate);};
       virtual void CycleNitrogen(double nCycling,double environmentEffect);

       virtual bool ExtraTurnover(double Fraction);
};

#endif
