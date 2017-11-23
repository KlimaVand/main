/*
 *  Author   Lars Kroll Kristensen
 *
 */

#ifndef __ECONOMIC_H
#define __ECONOMIC_H

#include "../base/IndicatorOutput.h"
#include "../base/base.h"

/*
 *
 * Class: economics
 *
 */
class economics:
    public base
{
    protected:
        double CapitalInterest;

    public:

        // Constructor
        economics();

        economics(string       aName,
                  const int    aIndex,
                  const base * aOwner);

        economics(const economics & aeconomics);

        ~economics();

        friend ostream & operator <<(ostream &         os,
                                     const economics & aeconomics);

        void SetInterest(double aInterest) {
            CapitalInterest = aInterest;
        }

        void run();

        void Initialize();

        void ReceivePlan();

        void CalcLP();

        void GiveIndicator();

        void DailyUpdate();
};
#endif

