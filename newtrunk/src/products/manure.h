
#ifndef __PMANURE_H
#define __PMANURE_H

#include "product.h"
#include "decomposable.h"

class manure:
    public decomposable
{
    private:
        string Alias;

        // ! Mg fraction of amount
        double Mg_content;

        // ! Rt Fraction of amount
        double Rt_content;

        // ! pH in the Manure
        double pH;

        // ! Cu Fraction of amount
        double Cu_content;

        // ! sand Fraction of amount
        double sand;

        // ! downward movement of water
        double infiltFactor;

        // ! determines storage capacity per %DM per unit LAI
        double canopyStorageParameter;

        // ! maximum proportion of manure OM that can be degraded during storage
        double maxDeg;
        double degRateConst;
        double degradableOM;

    public:
        double NH3emissionFactor;

        manure();

        manure(string       aName,
               const int    aIndex = -1,
               const base * aOwner = nullptr);

        manure(const manure & amanure);

        manure(const decomposable & aDecomp);

        virtual ~manure();

        void SetMg_content(double aMg_content) {
            Mg_content = aMg_content;
        }

        void SetCu_content(double aCu_content) {
            Cu_content = aCu_content;
        }

        void SetRt_content(double aRt_content) {
            Rt_content = aRt_content;
        }

        void Setsand(double asand) {
            sand = asand;
        }

        void SetpH(double apH) {
            pH = apH;
        }

        void SetinfiltFactor(double factor) {
            infiltFactor = factor;
        }

        void SetNH3emissionFactor(double input) {
            NH3emissionFactor = input;
        }

        // Get functions
        string GetfromAnimal(void) const {
            return Alias;
        }

        double GetMg_content(void) const {
            return Mg_content;
        }

        double GetCu_content(void) const {
            return Cu_content;
        }

        double GetRt_content(void) const {
            return Rt_content;
        }

        double Getsand(void) const {
            return sand;
        }

        double GetNH3emissionFactor() {
            return NH3emissionFactor;
        }

        double GetpH(void) const {
            return pH;
        }

        double GetQ(double temp) const;

        double GetinfiltFactor() const {
            return infiltFactor;
        }

        double GetmaxDeg() {
            return maxDeg;
        }

        double GetdegRateConst() {
            return degRateConst;
        }

        double GetdegradableOM() {
            return degradableOM;
        }

        void InitVariables();

        // !Calculate the ammonia volatilisation from manure

        /*
         * !
         * param area in square metres
         * param resistance resistance to ammonia transport
         * param duration time period over which ammonia volatilisation should be calculated (hours)
         */
        virtual double VolatAmmonia(double   temperature,
                                    double   evap,
                                    double   infiltration,
                                    double   precipitation,
                                    double   area,
                                    double   resistance,
                                    double   duration,
                                    double * volEvap,
                                    manure * toSoil);

        // !General function to calculate a gaseous emission of N

        /*
         * !
         * param emissionFactor proportion of the ammoniacal N to be emitted
         */
        virtual double VolatNitrogen(double emissionFactor);

        // !function to calculate gaseous emissions of N and mineralisation of organic N, using a factor approach

        /*
         * !
         * param NH3EF emission factor for ammonia
         * param N2EF emission factor for dinitrogen
         * param N2OEF emission factor for nitrous oxide
         * param MinF proportion of organic N that is mineralised
         * param *NH3vol pointer used to return the amount of ammonia volatilised
         * param *N2vol pointer used to return the amount of dinitrogen volatilised
         * param *N2Ovol pointer used to return the amount of nitrous oxide volatilised
         * param *MinN pointer used to return the amount of organic nitrogen mineralised
         */
        virtual void VolatAndMinNitrogen(double   NH3EF,
                                         double   N2EF,
                                         double   N2OEF,
                                         double   MinF,
                                         double * NH3vol,
                                         double * N2vol,
                                         double * N2Ovol,
                                         double * MinN);

        virtual double GetWaterEvap(double temperature,
                                    double relHumidity,
                                    double area,
                                    double resistance,
                                    double duration);

        virtual double SubtractWater(double waterToSubtract);

        virtual product & operator = (const product & someManure);

        virtual product & operator +(const product & someManure);

        virtual product & operator -(const product & someManure);

        virtual product * clone() const;

        virtual void ReadParameters(commonData * data,
                                    string       sectionName);

        virtual double GetcanopyStorageParameter() {
            return canopyStorageParameter;
        }

        // !Returns the instantaneous ammonia emission (grams N/ sq metre / sec)

        /*
         * !
         * param temperature temperature in Celsius
         * param area area of the emitting surface in square metres
         * param resistance atmospheric resistance to ammonia transport in seconds per metre
         */
        virtual double InstantaneousVolatAmmonia(double temperature,
                double                                  area,
                double                                  resistance);

        // !Adjusts the concentration of ammonium nitrogen to reflect the addition or subtraction of the amount specified. Returns the amount adjusted (which for subtraction may be lower than the amount requested, if the concentration would fall below zero)

        /*
         * !
         * param amount the amount of nitrogen to add or subtract from the manure (tonnes)
         */
        double AdjustNH4N(double amountToAdjust);

        // virtual double GetMethaneEmission(double temperature, double duration);
        // virtual double GetCarbonDioxideEmission(double temperature,
        // double duration);
        virtual double doDenitrification(double   temperature,
                                         double   duration,
                                         double * N2,
                                         double * N2O);

        virtual void CalcOMdegradation(double   temperature,
                                       double   duration,
                                       double   degRateConst,
                                       double * CO2,
                                       double * CH4,
                                       double * NH4);

        virtual double GetOrgMatterConc() {
            return dryMatter - sand;
        }

        virtual double CalcDryMatterConc();

        virtual double VolatCarbon(double emissionFactor);

        virtual double mesoTemperatureFunc(double temperature);

        virtual double MoistureFunc();

        virtual void setAlias(string InAlias) {
            Alias = InAlias;
        }

        virtual string getAlias() {
            return Alias;
        }
};
#endif

