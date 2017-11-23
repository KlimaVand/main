/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author Mikkel Fischer
 Revision 4/6 lkk added getQ function
 			 20/6 parameter not by reference in set... functions (JB)
          july major revision of all product's.         (JB)
          All operators calls the superclass
          operator check's
          if similar types, storage etc  added/subtracted
          if amount=0 the object gets the same unit, storage etc
          as the object that is added to it
\****************************************************************************/

#ifndef __PMANURE_H
  #define __PMANURE_H
#include <product.h>
#include <pdecomp.h>


/****************************************************************************\
 Class: manure
\****************************************************************************/

class manure: public decomposable
{
   private:
   	string fromAnimal;
      double Mg_content;
      double Rt_content;
      double pH;
      double Cu_content;
      double sand;
      double volatisation;
      double infiltFactor;
      double canopyStorageParameter; //determines storage capacity per %DM per unit LAI
      double maxDeg; //maximum proportion of manure OM that can be degraded during storage
      double degRateConst;
      double degradableOM;

   public:
      // Default Constructor
      manure();
      // Constructor with arguments
      manure(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      manure(const manure& amanure);
      manure(const decomposable& aDecomp);
      // Destructor
      virtual ~manure();
      // Operator for output
      friend ostream& operator << (ostream& os, const manure& amanure);

      // Set functions
      void SetfromAnimal(string afromAnimal) {fromAnimal = afromAnimal;}
      void SetMg_content(double aMg_content) {Mg_content = aMg_content;}
      void SetCu_content(double aCu_content) {Cu_content = aCu_content;}
      void SetRt_content(double aRt_content) {Rt_content = aRt_content;}
      void Setsand(double asand) {sand = asand;}
      void Setvolatisation(double aVol) {volatisation = aVol;}
      void SetpH(double apH) {pH = apH;}
      void SetinfiltFactor(double factor) {infiltFactor=factor;}

      // Get functions
      string GetfromAnimal(void) const {return fromAnimal;}
      double GetMg_content(void) const {return Mg_content;}
      double GetCu_content(void) const {return Cu_content;}
      double GetRt_content(void) const {return Rt_content;}
      double Getsand(void) const {return sand;}
      double Getvolatisation(void) const {return volatisation;}
      double GetpH(void) const {return pH;}
      double GetQ(double temp) const;
      double GetinfiltFactor() const {return infiltFactor;}
      double GetmaxDeg() {return maxDeg;};
      double GetdegRateConst() {return degRateConst;};
      double GetdegradableOM() {return degradableOM;};
      // Other functions
      void InitVariables();
		virtual double VolatAmmonia(double temperature, double evap, double infiltration, double precipitation,
					double area, double resistance, double duration, double *volEvap, manure *toSoil);
		virtual double VolatNitrogen(double emissionFactor);
		virtual void VolatAndMinNitrogen(double NH3EF, double N2EF, double N2OEF, double MinF,
			double *NH3vol, double *N2vol, double *N2Ovol, double *MinN);
		virtual void VolatNitrogen(double temperature, double area, double resistance, double duration,
				double *volatAmmonia, double *N2Emission, double *N2OEmission);
		virtual double GetWaterEvap(double temperature, double relHumidity, double area, double resistance, double duration);
      virtual double SubtractWater(double waterToSubtract);
      virtual void AddWater(double waterToAdd);
      virtual product& operator=(const product& someManure);
      virtual product& operator+(const product& someManure);
      virtual product& operator-(const product& someManure);
      virtual product*  clone() const;
      virtual void ReadParameters(fstream * file);
      virtual double GiveInfiltrationReduction();
      virtual double GetWater_content() {return (1.0 - GetdryMatter());}
      virtual double GiveInfiltrationResistance();
      virtual double GetcanopyStorageParameter() {return canopyStorageParameter;}
      virtual void SetcanopyStorageParameter(double aValue) {canopyStorageParameter=aValue;};
		virtual double InstantaneousVolatAmmonia(double temperature, double area, double resistance, double duration);
		virtual double GetMethaneEmission(double temperature, double duration);
		virtual double GetCarbonDioxideEmission(double temperature, double duration);
      virtual double doDenitrification(double temperature, double duration, double *N2, double *N2O);
		virtual void CalcOMdegradation(double temperature, double duration, double degRateConst, double *CO2, double *CH4, double *NH4);
      virtual double GetOrgMatterConc() {return dryMatter-sand;};
      virtual double CalcDryMatterConc();
      virtual double VolatCarbon(double emissionFactor);
		virtual double mesoTemperatureFunc(double temperature);
      virtual double MoistureFunc();
};
#endif

