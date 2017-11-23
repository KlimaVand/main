
#ifndef __PRICE_H
#define __PRICE_H

#include "../base/base.h"
#include <math.h>
#include "../base/commonData.h"

class price:
    public base
{
    protected:
        double * sellPriceSet;
        double * buyPriceSet;
        double   baseSellPrice;        // Unit: kr
        double   actualSellPrice;      // Unit: kr
        double   expectedSellPrice;    // Unit: kr
        double   baseBuyPrice;         // Unit: kr
        double   actualBuyPrice;       // Unit: kr
        double   expectedBuyPrice;     // Unit: kr
        double   lambda;
        double   alpha;
        int      useNormalization;
        double   rightRange;
        double   leftRange;
        string   name;

        void InitVariables();

        double GetPseudo(double leftRange,
                         double rightRange,
                         double basePrice);

    public:
        price();

        price(string       aName,
              const int    aIndex = -1,
              const base * aOwner = nullptr);

        price(const price & aprice);

        virtual ~price();

        friend ostream & operator <<(ostream &     os,
                                     const price & aprice);

        // Get functions
        double GetbaseSellPrice(void) {
            return baseSellPrice;
        }

        double GetactualSellPrice(void) {
            return actualSellPrice;
        }

        double GetbaseBuyPrice(void) {
            return baseBuyPrice;
        }

        double GetactualBuyPrice(void) {
            return actualBuyPrice;
        }

        double GetExpectedSellPriceYear(int year);

        double GetExpectedBuyPriceYear(int year);

        string Getname(void) {
            return name;
        }

        void ReadParameters(commonData * file,
                            int          duration);

        // virtual void UpdatePrices(int simulationYear);
};
#endif

