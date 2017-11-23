// Soil organic matter pool
// (c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning 1998



#ifndef MATTER_H
#define MATTER_H

#include "../base/base.h"
#include "../products/organicProduct.h"
#include "organicMatter.h"
#include "../base/bstime.h"
#include "../base/commonData.h"

class matter:
    public base
{
    protected:
        enum { maxConnections = 20 };

        struct connection {
            matter * connectPt;
            double   Cfraction, Nfraction;

            /*
             * at this time of writeing connectname can have one of the fellowing numbers
             * AOM1 0
             * AOM2 1
             * SMB1 2
             * SMB2 3
             * SMR  4
             * NOM  5
             * IOM  6
             */
            int    connectname;
            string connectnameChar;
        };


        // Parameters
        bool   useClayEffect,                 // if true use clay effect
               expDecay;                      // if true use exponential decay
        double cnRatio, decompositionRate,    // Decomposition rate coefficient [/d]
               clayEffect1, clayEffect2;

        // State variables
        cnMatter carbon,      // Carbon content
                 influx,      // Influx from other pools
                 intermed,    // Internal storage, used in connection with N-limitation
                 added;       // Added carbon
        double   CNC;
        int      connections;

        /*
         * at this time of writeing poolName can have one of the fellowing numbers
         * AOM1 0
         * AOM2 1
         * SMB1 2
         * SMB2 3
         * SMR  4
         * NOM  5
         * IOM  6
         */
        int        poolName;
        string     poolNameChar;
        connection connectionList[maxConnections];

        matter();                     // Dissable the compilers automatic generation of default constructor

        matter & operator = (
                const matter & m);    // Dissable the compilers automatic generation of default assignment operator

        void DirectOutFlux(double flux);

        double CNcrit();

        void TestState();

    public:
        matter(string       Pname,
               const int    Index,
               const base * owner);

        matter(const matter & source);

        virtual ~matter() {}

        virtual matter * clone() const;

        virtual void Initialize(double clayEffect1,
                                double clayEffect2);

        virtual void ReadParameters(commonData *& file);

        virtual void Import(cnMatter c);

        virtual void Export(cnMatter c);

        virtual void Update(double environmentEffect);

        virtual void CatchInflux(cnMatter & content);

        virtual void TestAddProduct(organicProduct * prod,
                                    double *         check,
                                    double *         CN,
                                    int *            num,
                                    bool             forAOM);

        virtual void AddOrganicProduct(organicProduct * prod,
                                       int              fractionNumber,
                                       double *         check);

        virtual double GetConnectionFraction(int poolName);

        virtual void SetDecompositionRate(double d) {
            decompositionRate = d;
        }

        virtual void SetDeathRate(double d) {
            theMessage -> FatalError("Can not set death-rate for this type of pool.");
        }

        virtual void SetMaintenanceRate(double m) {
            theMessage -> FatalError("Can not set mainatance-rate for this type of pool.");
        }

        virtual double GetDeathRate() {
            theMessage -> FatalError("Can not get death-rate for this type of pool.");

            return 0.0;
        }

        virtual double GetMaintenanceRate() {
            theMessage -> FatalError("Can not get mainatance-rate for this type of pool.");

            return 0.0;
        }

        virtual void SetCarbon(cnMatter cn) {
            carbon = cn;
        }

        virtual void AddCarbon(cnMatter cn) {
            carbon = cn + carbon;
        }

        virtual cnMatter GetCarbon();

        virtual double GetCarbonC();

        virtual nitrogen GetNitrogen();

        virtual double GetNitrogenN();

        virtual double GetNitrogenN15();

        virtual double GetDecompositionRate() {
            return decompositionRate;
        }

        virtual int GetPoolName() {
            return poolName;
        }

        virtual string GetPoolNameChar() {
            return poolNameChar;
        }

        virtual bool GetExponentialDecay() {
            return expDecay;
        }

        virtual void SetExponentialDecay(bool ed) {
            expDecay = ed;
        }

        virtual double GetCNratio() {
            return cnRatio;
        }

        virtual double GetDefaultCNratio() {
            return cnRatio;
        }

        virtual double GetCNcrit() {
            return cnRatio;
        }

        virtual double ResidualCarbon();

        virtual void FinalExport(double ratio);

        virtual double PotentialNitrogenMineralisation();

        virtual bool IsBiomass() {
            return false;
        }

        virtual bool IsAOM() const {
            return false;
        }

        virtual double PotentialCarbonTurnover() {
            return carbon.c * decompositionRate;
        }
};
#endif

