
#ifndef cropRyegrass_H
#define cropRyegrass_H

#include "crop.h"
#include "../../products/plantItem.h"
#include "../../products/feedItem.h"

class cropRyegrass:
    public crop
{
    protected:
        double MinDayLength, BulkDensity, RespirationTop, RespirationRoot, TransferRate, RecycleFraction,
               TemperatureSumAfterCut, CutDelay;

        // Added by MEL 2009
        bool HasBeenCut;

        // Added by NJH 17.08.00
        double liveOMD, deadOMD;
        double grazableDM, grazableN, grazedDM[4], uptakeWeight[4];
        double AgeClassTop[4];
        double RateOfRipeReSeed;
        double InitialLAIIncrease;
        double ReSeedDM;

        virtual double UpdateAgeClasses(double deltaDryMatt);

        virtual double TopRespiration();

        virtual double DeadDMTop();

        virtual void CalcLeafAreaIndices();

        virtual void TransferDryMatterToStorage(double * deltaDryMatt);

        virtual void RemoveDMFromOldestAgeClass(double transferFromAgeClass);

        virtual void RipeSenescence();

    public:
        cropRyegrass(string       aName,
                     const int    aIndex,
                     const base * aOwner,
                     string       cropName);

        cropRyegrass(const cropRyegrass & acrop);

        virtual ~cropRyegrass() {}

        virtual cropRyegrass * clone();

        virtual double Update(double ActivePar);

        virtual void UpdateHeight();

        virtual bool ReadyForHarvest() {
            return Phenology -> Emerged();
        }

        virtual void RootDecay(double Translocation);

        virtual double GiveEvapFactor();

        virtual double GiveDryMatterVegTop();

        virtual double GetBulkDensity() {
            return BulkDensity;
        }

        virtual double GiveCropHeight();

        virtual feedItem * GetAvailability(double cutHeight,
                                           int    animalType);

        virtual double GetAvailableStandingDM(double residualMass);

        virtual void SetGrazed(double cutHeight,
                               double DMGrazed);

        virtual double GetgrazableDM() {
            return grazableDM;
        }

        virtual double GetgrazableN() {
            return grazableN;
        }

        virtual void Cut(plantItem * cutPlantMaterial,
                         double      cut_height);

        // !Used when the amount grazed has been determined externally and the grazed material needs to be removed

        /*
         * !
         * \param grazedDM the amount of grazed DM in g/sq metre
         */
        virtual void Grazing(double grazedDM);

        virtual void Terminate(decomposable * Straw,
                               decomposable * DeadRoot,
                               double *&      RootLengthList);

        // virtual void GrazeOrCut(decomposable* Hay, double fractionLeft, bool graze);
        virtual void Harvest(decomposable * Storage,
                             decomposable * Straw);

        virtual double GetGrazedDM();

        virtual void ClearTemporaryVariables();

        virtual double Nmax();

        virtual bool EndBudget(double * NRemain,
                               double * DMRemain);

        virtual double GetAvailableDM(double residualDM);

        virtual double GetAvailableN(double residualDM);
};
#endif

