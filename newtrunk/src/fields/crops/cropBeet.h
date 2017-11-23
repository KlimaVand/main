
#ifndef CROPBEET_H
#define CROPBEET_H

#include "../../base/message.h"
#include "../../soil/soil.h"
#include "crop.h"

class cropBeet:
    public crop
{
    protected:
        double TempSumStorage1,      // Temperature sum for min allocation to storage
               TempSumStorage2,      // Temperature sum for max allocation to storage
               MinAllocToStorage,    // Min allocation to storage organs
               MaxAllocToStorage;    // Max allocation to storage organs

    public:
        cropBeet(string       aName,
                 const int    aIndex,
                 const base * aOwner,
                 string       cropName);

        cropBeet(const cropBeet & acrop);

        virtual bool ReadyForHarvest() {
            return Phenology -> DS > 1.5;
        }

        virtual void TransferDryMatterToStorage(double * dDryMatt);
};
#endif

