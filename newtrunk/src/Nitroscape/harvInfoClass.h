
#ifdef NITROSCAPE

/*
 *
 * $URL$
 * $LastChangedDate$
 * $LastChangedRevision$
 * $LastChangedBy$
 *
 */

#include <string>

#ifndef __HARVINFO_H
#define __HARVINFO_H
using namespace std;
// ! Holds information on the amount, type and characteristics of an individual harvest product
class harvInfoClass {
    protected:

        // !Amount of dry matter in a harvest product (kilograms per ha)
        double amountDM;

        // !Amount of nitrogen in a harvest product (kilograms per ha)
        double amountN;

        // !Amount of carbon in a harvest product (kilograms per ha)
        double amountC;
		// !The name of the harvest product
        string productName;
        // !Harvest product code (unique identifier, equates to the plantitem index in products.dat
        int productCode;

        

    public:

        // ! Default Constructor
        harvInfoClass();

        harvInfoClass(const harvInfoClass & aharvInfoClass) {}

        virtual ~harvInfoClass() {}

        // ! Set function

        /*
         * !
         * param aamountDM Amount of harvest product dry matter (tonnes)
         */
        void SetamountDM(double aamountDM) {
            amountDM = aamountDM;
        }

        // ! Set function

        /*
         * !
         * param aamountN Amount of nitrogen in harvest product (kilograms)
         */
        void SetamountN(double aamountN) {
            amountN = aamountN;
        }

        // ! Set function

        /*
         * !
         * /pram aamountC Amount of carbon in harvest product (kilograms)
         */
        void SetamountC(double aamountC) {
            amountC = aamountC;
        }

        // ! Set function

        /*
         * !
         * param aVal Unique product identifier code
         */
        void SetproductCode(int aVal) {
            productCode = aVal;
        }

        // Get functions
        // !Returns amount of dry matter in a harvest product (tonnes)
        double GetamountDM(void) const {
            return amountDM;
        }

        // !Returns amount of nitrogen in a harvest product (kilograms)
        double GetamountN(void) const {
            return amountN;
        }

        // !Returns amount of carbon in a harvest product (kilograms)
        double GetamountC(void) const {
            return amountC;
        }

        // !Returns product code
        int GetproductCode() {
            return productCode;
        }

        void SetproductName(string aVal) {
            productName= aVal;
        }

        string GetproductName() {
            return productName;
        }
};
#endif

#endif

