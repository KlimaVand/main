/*
 * Revision               july major revision of all product's.         (JB)
 * All operators calls the superclass
 * operator check's
 * if similar types, storage etc  added/subtracted
 * if amount=0 the object gets the same unit, storage etc
 * as the object that is added to it
 *
 * new function AddAmount
 */

#ifndef __PRODUCT_H
#define __PRODUCT_H

#include "../base/base.h"
#include "../base/commonData.h"
#include "price.h"

#ifdef _STANDALONE

#include "../base/common.h"

#else

#include "../base/message.h"

#endif

#include <string.h>

class product:
    public base
{
    protected:
        double  amount;
        int     mainUnitName;
        price * aPrice;
        int     storable;
        int     ObjType;

        int GetUnitType(string aMainUnitType);

        void InitVariables();

       

    public:
        product();
		product(const product & aproduct);
        product(string       aName,
                const int    aIndex = -1,
                const base * aOwner = nullptr);

        virtual ~product();    // virtual to ensure correct deletion of derivered objects

        friend ostream & operator <<(ostream &       os,
                                     const product & aproduct);

        void Setname(string aname) {
            Name = aname;
        }    //

        void Setamount(double aamount) {
            amount = aamount;
        }

        string Getname(void) const {
            return Name;
        }

        void SetmainUnitName(int amainUnitName) {
            mainUnitName = amainUnitName;
        }

        void SetaPrice(price * aaPrice) {
            aPrice = aaPrice;
        }
		void  UpdateProductPtr(void);
        void SetObjType(int aObjType) {
            ObjType = aObjType;
        }

        double GetAmount(void) const {
            return amount;
        }

        int GetmainUnitName(void) const {
            return mainUnitName;
        }

        string GetUnitString(int amainUnitName);

        price * GetaPrice(void) const {
            return aPrice;
        }

        int GetObjType(void) const {
            return ObjType;
        }

        bool IsStorable(void) {
            return (storable != 0);
        }

        virtual string DerivedFrom() const;    // Returns name of product that can be transformed to this class

        virtual product & operator = (const product & aProduct);

        virtual product & operator +(const product & aProduct);

        virtual product & operator -(const product & aProduct);

        virtual product & operator *(const double & aFactor);

        virtual void showstats(ostream * os);

        virtual void ReadParameters(commonData *& data,
                                    string        sectionName);

        string GetnameByChar();

        virtual product * clone() const;
};
#endif

