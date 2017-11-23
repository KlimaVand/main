// Soil added organic matter pool
// (c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning 1998



#ifndef ORGPROD_H
#define ORGPROD_H

#include "../base/base.h"
#include "../base/message.h"
#include "../base/commonData.h"
#include "../products/cnmatter.h"
#include "../products/nitrogen.h"

class organicProduct:
    public base
{
    private:
        organicProduct & operator = (const organicProduct & o);

        // Dissable the compilers automatic generation of default assignment operator

    protected:
        enum { maxAllocations = 12 };

        // for possible changer in data structure. Only the first 3 is used and is Aom1, Aom2 and nom
        struct allocation {
            int    poolName;
            double cFraction, nFraction, decayRate;
        };


        // ! organic C and Organic N
        cnMatter         amount;
        nitrogen         NO3, NH4;
        double           ReduceTurnover, carbonFraction;
        int              numOfAllocations;
        string           productName;
        allocation       allocationList[maxAllocations];
        organicProduct * next;    // Stuff from old organicProduct

        void InitVariables();

    public:
        organicProduct();

        organicProduct(string       Pname,
                       const int    Index,
                       const base * owner);

        organicProduct(const organicProduct & product);

        void Add(organicProduct * product,
                 double           fraction);

        void ReadParameters(commonData *& file,
                            bool          ncfrac);

        void SetAmount(cnMatter a) {
            amount = a;
        }

        cnMatter GetAmount() {
            return amount;
        }

        string GetName() {
            return productName;
        }

        void SetName(string s) {
            productName = s;
        }

        double GetDecompositionRate(int num) {
            return allocationList[num].decayRate;
        }

        double GetCFraction(int num) {
            return allocationList[num].cFraction;
        }

        double GetNFraction(int num) {
            return allocationList[num].nFraction;
        }

        void SetCFraction(int    num,
                          double f) {
            allocationList[num].cFraction = f;
        }

        void SetNFraction(int    num,
                          double f) {
            allocationList[num].nFraction = f;
        }

        int GetDestPool(int num) {
            return allocationList[num].poolName;
        }

        int GetNumOfFractions() {
            return numOfAllocations;
        }

        double GetCarbonFraction() {
            return carbonFraction;
        }

        void SetCarbon(double c) {
            amount.c = c;
        }

        void SetNitrogen(nitrogen n) {
            amount.n   = n.n;
            amount.n15 = n.n15;
        }

        nitrogen GetNitrate() {
            return NO3;
        }

        nitrogen GetAmmonium() {
            return NH4;
        }

        void SetNO3_content(nitrogen content) {
            NO3 = content;
        }

        void SetNH4_content(nitrogen content) {
            NH4 = content;
        }

        void ClearNO3_content() {
            NO3.Clear();
        }

        void ClearNH4_content() {
            NH4.Clear();
        }

        nitrogen GetOrganicNitrogen();

        double GetOrganicNitrogenN();

        double GetOrganicNitrogenN15();

        nitrogen GetTotalNitrogen();

        double GetCarbon() {
            return amount.c;
        }

        void Scale(double f);

        void SetkAOM1(double k) {
            allocationList[0].decayRate = k;
        }

        void SetkAOM2(double k) {
            allocationList[1].decayRate = k;
        }

        void SetAOM1_fraction(double f);

        void SetAOM2_fraction(double f);

        organicProduct * GetFraction(double frac);

        organicProduct * Next() {
            return next;
        }

        bool AddNext(organicProduct * product);

        bool ProductSimilarity(const organicProduct * product);

        double GetCNratio();

        void SetReduceTurnover(double Reduce) {
            ReduceTurnover = Reduce;
        }

        double GetReduceTurnover() {
            return ReduceTurnover;
        }
};
#endif

