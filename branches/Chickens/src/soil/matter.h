/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// Soil organic matter pool
// (c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning 1998
// =============================================================================

#ifndef MATTER_H
   #define MATTER_H

#include "../base/base.h"
#include "../products/organicProduct.h"
#include "organicMatter.h"
#include "../base/bstime.h"

class matter : public base
{
   protected:
      enum {maxConnections=20};
   	struct connection
   	{
   		matter * connectPt;
   		double Cfraction,Nfraction;
         string connectname;
   	};

      // Parameters
      bool
         useClayEffect,                 // if true use clay effect
         expDecay;                      // if true use exponential decay

      double
         cnRatio,
         stepFactor,
         decompositionRate,             // Decomposition rate coefficient [/d]
         clayEffect1,
         clayEffect2;
      int
         TAG,                           // -1: no tagging; 0: untag all; 1: tag all
         Isotope,
         clayResponseType;              // See "container.cpp". 0 if unused.

      // State variables
      cnMatter
         carbon,                        // Carbon content
         influx,                        // Influx from other pools
         intermed,                      // Internal storage, used in connection with N-limitation
         added;                         // Added carbon

      double CNC;
      cnMatter carbonDerivative[6];
      cnMatter outfluxDerivative[5];

      int connections;
      string poolName;
      connection connectionList[maxConnections];

      matter();                   // Dissable the compilers automatic generation of default constructor
   	matter& operator=(const matter& m); // Dissable the compilers automatic generation of default assignment operator

      void DirectOutFlux(double flux);
      double CNcrit();
      void TestState();

    public:
       matter(const char * Pname,const int Index,const base * owner);
       matter(const matter &source);
       virtual ~matter(){};
       virtual matter* clone() const;
       virtual void Add(matter * source,double fraction);
       virtual void Initialize(double clayEffect1,double clayEffect2,int clayResp);
       virtual void ReadParameters(fstream * file);
       virtual void Import(cnMatter c);
       virtual void Export(cnMatter c);
       virtual void Update(double environmentEffect,int mode);
       virtual void CatchInflux(cnMatter &content,int mode);
       virtual void TestAddProduct(organicProduct * prod,double * check,double * CN,int * num,bool forAOM);
       virtual void AddOrganicProduct(organicProduct * prod,int fractionNumber,double * check);

       virtual void SetConnectionFraction(int nr,double frC,double frN=-1.0);
       virtual double GetConnectionFraction(string poolName);

       virtual void SetDecompositionRate(double d){decompositionRate=d;};
       virtual void SetDeathRate(double d) {Terminate("Can not set death-rate for this type of pool.");};
       virtual void SetMaintenanceRate(double m) {Terminate("Can not set mainatance-rate for this type of pool.");};
       virtual double GetDeathRate() {Terminate("Can not get death-rate for this type of pool.");return 0.0;};
       virtual double GetMaintenanceRate() {Terminate("Can not get mainatance-rate for this type of pool.");return 0.0;};
       virtual void SetCarbon(cnMatter cn){carbon=cn;};
       virtual void AddCarbon(cnMatter cn){carbon=cn+carbon;};
       virtual cnMatter GetCarbon();
       virtual double GetDecompositionRate() {return decompositionRate;};
       virtual string GetPoolName(){return poolName;};
       virtual void SetIsotope(int i){Isotope=i;};
       virtual bool GetExponentialDecay(){return expDecay;};
       virtual void SetExponentialDecay(bool ed){expDecay=ed;};
       virtual bool SetParameterByName(string parmName,double val);
       virtual void SetStepFactor(double sf){stepFactor=sf;};
       virtual double GetCNratio(){return cnRatio;};
       virtual double GetDefaultCNratio(){return cnRatio;};
       virtual double GetCNcrit(){return cnRatio;};
       virtual void Scale(double fraction);
       virtual double ResidualCarbon();
       virtual void FinalExport(double ratio);



       virtual double PotentialNitrogenMineralisation();
       virtual double PotentialNitrogenImmobilisation();
       virtual bool Hungry();
       virtual double GetMicrobialPoolNdesire(){return 0.0;};
       virtual void GiveMicrobialPoolExtraN(double r){};
       virtual bool IsBiomass(){return false;};
       virtual bool IsAOM() const {return false;};
       virtual double PotentialCarbonTurnover(){return carbon.c*decompositionRate;};
       virtual void CycleNitrogen(double nCycling,double environmentEffect){}; // Only affects biomass pools
       virtual void ReleaseReduceTurnover(){};
       virtual bool ExtraTurnover(double Fraction);
       virtual bool CloneAndModify(double Fraction,double TurnoverFactor);
};

#endif

