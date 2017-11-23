
#ifdef NITROSCAPE

#ifndef __ECOINTERFACE_H
#define __ECOINTERFACE_H

#include "../fields/ecosystem.h"
#include "../Nitroscape/harvInfoClass.h"

// !Interface class of an individual ecosystem
class ecosystinterfaceClass:
    public base
{
    private:
        double totalLAI;
        bool   ReadyForHarvestOrLater;
        double IrrigationDemand;
        double AddedChloride;
        double irrigationWater;
        double irrigationN;
        bool   killAllCrops;    // needs to be reset daily
        double TotalEcosystemN;
        double TotalEcosystemC;
        double MaxCropHeight;
        double StandingDMdensity;
        double AvailableStandingDM;
        double AvailableStandingDMHeight;
        double dryMatterGrazed;
        double grazedHt;

        // ! from N fertiliser addition
        double thesurfNitrate, thesurfAmmonium, thesoilNitrate, thesoilAmmonium;

        // ! tillage
        string theType;                    // Tillage Type
        double theDepth;                   // Tillage Depth
        double theFractionIncorporated;    // Fraction of surface organic matter incorporated

        // ! harvesting
        linkList<harvInfoClass> * harvestStuff;
        int                       harvestCode;
        double                    propGrainHarvested;
        double                    propStrawHarvested;

        // !output file for ecosystem plant details
        fstream           plantDetailFile;
        IndicatorOutput * ecoOutput;
        ecosystem *       theEcosystem;

        ecosystinterfaceClass(const ecosystinterfaceClass & aecosystinterfaceClass) {}

    public:
        ecosystinterfaceClass(int    aIndex,
                              string soilFileName,
                              double anArea = 0.0);

        ~ecosystinterfaceClass();

        // !Performs the daily update of the ecosystem
        void DoDailyEco();

        // !Add an amount of decomposable matter to the ecosystem

        /*
         * !
         * param decompName name of the material
         * param decompAmount amount of material added (grams per metre squared of dry matter)
         * param decompNH4_N amount of ammonium nitrogen added (grams per metre squared)
         * param decompNO3_N amount of nitrate nitrogen added (grams per metre squared)
         * param decompOrg_N amount of organic nitrogen added (grams per metre squared)
         * param decompC amount of carbon added (grams per metre squared)
         */
        void AddDecomposable(string decompName,
                             double decompAmount,
                             double decompNH4_N,
                             double decompNO3_N,
                             double decompOrg_N,
                             double decompC);

        // !Incorporate surface-applied material into the soil
        void Incorporate();

        // !Returned the total leaf area index of the ecosystem
        double GetTotalLAI() {
            return totalLAI;
        }

        // !Set the total leaf area index of the ecosystem

        /*
         * !
         * param aVal The value to which the leaf area index should be set
         */
        void SetTotalLAI(double aVal) {
            totalLAI = aVal;
        }

        // !Returns true if the crop is ready for harvesting
        bool GetReadyForHarvestOrLater() {
            return ReadyForHarvestOrLater;
        }

        // !Sets the crop to be ready for harvesting
        // void SetReadyForHarvestOrLater(){theEcosystem.GetReadyForHarvestOrLater();};
        // void SetIrrigationDemand(double aVal){IrrigationDemand = aVal;};
        // !Returns the amount of water required to return the soil to field capacity (millimetres)
        double GetIrrigationDemand() {
            return IrrigationDemand;
        }

        // !Set the indicator that will result in the termination of all crops present in the ecosystem
        void KillAllCrops() {
            killAllCrops = true;
        }

        // !Returned the height of the patch (millimetres)
        double GetMaxPatchHeight() {
            return MaxCropHeight;
        }

        // !Return the amount of aboveground dry matter (grams per metre squared)
        double GetStandingDMdensity() {
            return StandingDMdensity;
        }

        // !Return the amount of dry matter above the AvailableStandingDMHeight (grams per metre squared)
        double GetAvailableStandingDM() {
            return AvailableStandingDM;
        }

        // !Set the height above which dry matter should be considered available for harvesting or grazing

        /*
         * !
         * param AvailableStandingDMHeight height above which dry matter should be considered available for harvesting or grazing (millimetres)
         */
        void SetAvailableStandingDMHeight(double aVal) {
            AvailableStandingDMHeight = aVal;
        }

        // !Return the maximum crop height (millimetres)
        double GetMaxCropHeight() {
            return MaxCropHeight;
        }

        // !Return the proportion of grain that is harvested during a harvest operation
        double GetpropGrainHarvested() {
            return propGrainHarvested;
        }

        // !Sets the proportion of grain that is harvested during a harvest operation

        /*
         * !
         * param propGrainHarvested The proportion of grain that is harvested
         */
        void SetpropGrainHarvested(double aVal) {
            propGrainHarvested = aVal;
        }

        // !Returns the proportion of straw harvested during a harvesting operation
        double GetpropStrawHarvested() {
            return propStrawHarvested;
        }

        // !Sets the proportion of straw that is harvested during a harvest operation

        /*
         * !
         * param propGrainHarvested The proportion of straw that is harvested
         */
        void SetpropStrawHarvested(double aVal) {
            propStrawHarvested = aVal;
        }

        // !Returns a list of the harvestable material
        linkList<harvInfoClass> * GetharvestStuff() {
            return harvestStuff;
        }

        // !Instructs the ecosystem to initiate a harvesting operation

        /*
         * !
         * param harvestCode codes to indicate which harvesting method should be used
         * param propGrainHarvested proportion of grain to be harvested
         * param propStrawHarvested proportion of straw to be harvested
         */
        void HarvestOperations(int    harvestCode,
                               double propGrainHarvested,
                               double propStrawHarvested);

        // !Adds nitrogen into the ecosystem

        /*
         * !
         * param surfNitrate nitrate to be added to the surface of the soil (grams per metre squared)
         * param surfAmmonium ammonium to be added to the surface of the soil (grams per metre squared)
         * param soilNitrate nitrate to be added to the soil (grams per metre squared)
         * param soilAmmonium ammonium to be added to the soil (grams per metre squared)
         */
        void AddNutrient(double surfNitrate,
                         double surfAmmonium,
                         double soilNitrate,
                         double soilAmmonium) {
            theEcosystem.AddNutrient(surfNitrate, surfAmmonium, soilNitrate, soilAmmonium);
        }

        // !Incorporates any surface material into the soil

        /*
         * !
         * param incorpDepth depth to which incorporation should occur (millimetres)
         * param tillage set to true if this is a tillage operation
         * param tillagetype if this is a tillage operation, this gives the name of the tillage type
         */
        void Incorporate(double incorpDepth,
                         bool   tillage,
                         string tillagetype) {
            theEcosystem.Incorporate(incorpDepth, 1.0, tillage, tillagetype);
        }

        // !Sets the amount of irrigation water and the amount of nitrogen applied therein

        /*
         * !
         * param amountWater amount of water applied (millimetres)
         * param amountN amount of nitrogen applied (grams per metre squared)
         */
        void SetirrigationWater(double amountWater,
                                double amountN);

        // !Instruct the ecosystem model to perform a tillage operation

        /*
         * !
         * param theType indicates the type of tillage operation to be performed
         * param theDepth the depth to which the tillage should be performed (millimetres)
         * param theFractionIncorporated the fraction of any surface material that should be incorporated during the tillage operation
         */
        void Tillage(int    theType,
                     double theDepth,
                     double theFractionIncorporated);

        // !
        // void GetFeedResource(feedItem * my_resource, double grazing_height, double min_stubble_ht, int animalType);
        // !Remove the amount of dry matter harvested by grazing

        /*
         * !
         * param DMGrazed the amount of dry matter to be grazed  (grams per metre squared)
         * param NGrazed the amount of N to be grazed  (grams per metre squared)
         */
        void Grazing(double DMGrazed);

        // plantItem*  createHarvestProduct(string productName, double amountDM, double amountN, double amountC);
        // void SetHarvestProduct(plantItem* aProduct, int code);
        // !Instructs the ecosystem model to sow a crop

        /*
         * !
         * param seedIndex identifier to indicate the crop is to be sown
         * param amountSeedDM amount of seed dry matter to be sown (grams per metre squared)
         * param NitrogenInSeed amount of nitrogen applied in seed (grams per metre squared)
         */
        void Sow(int    seedIndex,
                 double amountSeedDM,
                 double NitrogenInSeed,
                 double CarbonInSeed);

        // !Update the ecosystem status variables
        void UpdateStatus();

        // !Returned the FASSET crop identifier for a particular crop code

        /*
         * !
         * param cropID the FASSET crop identifier
         * param cropCode the crop code input
         */
        void AllocateCrop(string cropID,
                          int    cropCode);

        // !Add organic material to the ecosystem

        /*
         * !
         * param theType indicates the type of organic material added
         * param theamount the amount of material added (grams fresh matter per metre square)
         * param theDepth the depth into the soil to which the material is deposited (millimetres)
         * param thepercentTotalN the total percentage of nitrogen applied in fresh weight
         * param thepercentNH4N the percentage of ammonium nitrogen in fresh weight
         * param thepercentNO3N the percentage of nitrate nitrogen in fresh weight
         * param thepercentDM the percentage dry matter in fresh weight
         * param thepercentC the percentage carbon in fresh weight
         */
        void AddOrganic(int    theType,
                        double theamount,
                        double theDepth,
                        double thepercentTotalN,
                        double thepercentNH4N,
                        double thepercentNO3N,
                        double thepercentDM,
                        double thepercentC);

        // !Add fertilizer

        /*
         * !
         * param theNH4N the amount of ammonium applied (grams per square metre)
         * param theNO3N the amount of nitrate applied (grams per square metre)
         */
        void SetFertilisation(double theNH4N,
                              double theNO3N);

        // !Return the amount of nitrogen in the ecosystem
        double GetTotalEcosystemN() {
            return TotalEcosystemN;
        }

        // !Return the amount of Carbon in the ecosystem
        double GetTotalEcosystemC() {
            return TotalEcosystemC;
        }

        // !Instruct the ecosystem model to write daily status variables to file

        /*
         * !
         * param initialise set true to print the file header
         */
        void OutputPlantDetails(bool initialise);

        // !Close the file containing the ecosystem model status variables
        void ClosePlantDetails();

        void SetECOGlobals();
};
#endif

#endif

