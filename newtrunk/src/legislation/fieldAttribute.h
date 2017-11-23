/*
 *
 * Archive
 * Author
 * Date
 * Revision
 * ----------------------------------------------------------------------------
 * Log
 *
 */

#ifndef __FIELDATT_H
#define __FIELDATT_H

#include "../base/base.h"
#include <string.h>

/*
 *
 * Class: fieldAttrib
 *
 */
class fieldAttrib:
    public base
{

    /* Attributes */
    private:

    protected:
        double   contributionThisYear;    //
        int      contributionIndex;       //
        bool     isgreen;
        double * contributionList;        //
        int      boundary;
        string   fieldName;               //

    public:

        /* Actions */

    private:

    protected:

    public:
        fieldAttrib();

        fieldAttrib(string aName,
                    int    aIndex = -1,
                    base * aOwner = nullptr);

        ~fieldAttrib();

        // Set and Get functions
        void SetcontributionThisYear(double acontributionThisYear) {
            contributionThisYear = acontributionThisYear;
        }

        void SetcontributionIndex(int acontributionIndex) {
            contributionIndex = acontributionIndex;
        }

        void SetcontributionList(double * acontributionList) {
            contributionList = acontributionList;
        }

        void SetfieldName(string afieldName) {
            fieldName = afieldName;
        }

        void Setisgreen(bool aisgreen) {
            isgreen = aisgreen;
        }

        void Setboundary(int aboundary) {
            boundary = aboundary;
        }

        int Getboundary() {
            return boundary;
        }

        bool Getisgreen() {
            return isgreen;
        }

        double GetcontributionThisYear(void) {
            return contributionThisYear;
        }

        int GetcontributionIndex(void) {
            return contributionIndex;
        }

        double * GetcontributionList(void) {
            return contributionList;
        }

        string GetfieldName() {
            return fieldName;
        }

        double GetContributionAmount(void) {
            return contributionThisYear;
        }

        void Initialize(string   afieldName,
                        bool     aisgreen,
                        double * acontributionList,
                        int      aboundary);

        bool Update(void);
};
#endif

