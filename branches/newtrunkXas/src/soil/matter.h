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
#include "../base/commonData.h"
class matter : public base
{
   protected:
      enum {maxConnections=20};
   	struct connection
   	{
   		matter * connectPt;
   		double Cfraction,Nfraction;
   		/**
   		       at this time of writeing connectname can have one of the fellowing numbers
   		      AOM1 0
   		      AOM2 1
   		      SMB1 2
   		      SMB2 3
   		      SMR  4
   		      NOM  5
   		      IOM  6
   		      AOM-subpool 7
   		      CO2  8
   		      SMB  9
   		      ROM  10
   		      HUM  11*/
         int connectname;
         char connectnameChar[250];
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
      /**
       at this time of writeing poolName can have one of the fellowing numbers
      AOM1 0
      AOM2 1
      SMB1 2
      SMB2 3
      SMR  4
      NOM  5
      IOM  6
      AOM-subpool 7
      CO2  8
      SMB  9
      ROM  10
      HUM  11*/
      int poolName;
      char poolNameChar[250];
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
       virtual void ReadParameters(commonData * &file);

       virtual void Import(cnMatter c);
       virtual void Export(cnMatter c);
       virtual void Update(double environmentEffect,int mode);
       virtual void CatchInflux(cnMatter &content,int mode);
       virtual void TestAddProduct(organicProduct * prod,double * check,double * CN,int * num,bool forAOM);
       virtual void AddOrganicProduct(organicProduct * prod,int fractionNumber,double * check);
       virtual void Decay14C(double HalfTime);
       virtual void SetConnectionFraction(int nr,double frC,double frN=-1.0);
       virtual double GetConnectionFraction(int poolName);
       virtual void WriteSelf(iostream * s);
       virtual void SetDecompositionRate(double d){decompositionRate=d;};
       virtual void SetDeathRate(double d) { theMessage->FatalError("Can not set death-rate for this type of pool.");};
       virtual void SetMaintenanceRate(double m) { theMessage->FatalError("Can not set mainatance-rate for this type of pool.");};
       virtual double GetDeathRate() { theMessage->FatalError("Can not get death-rate for this type of pool.");return 0.0;};
       virtual double GetMaintenanceRate() { theMessage->FatalError("Can not get mainatance-rate for this type of pool.");return 0.0;};
       virtual void SetCarbon(cnMatter cn){carbon=cn;};
       virtual void AddCarbon(cnMatter cn){carbon=cn+carbon;};
       virtual cnMatter GetCarbon();
       virtual double GetCarbonC();
       virtual nitrogen GetNitrogen();
       virtual double GetNitrogenN();
       virtual double GetNitrogenN15();
       virtual double GetDecompositionRate() {return decompositionRate;};
       virtual int GetPoolName(){
    	   if(poolName>=7)
    	      		  theMessage->FatalError("fdfd");
    	   return poolName;};
       virtual char * GetPoolNameChar(){return poolNameChar;};
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
   virtual void CatchInfluxIfNotHungry(cnMatter &content,int mode){if(!Hungry()) CatchInflux(content,mode);};
       virtual void CatchInfluxIfHungry(cnMatter &content,int mode){if(Hungry()) CatchInflux(content,mode);};
       virtual void FinalDecayIfHungry(double ratio);
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
	   virtual void print(int current,int Last);
};

#endif

