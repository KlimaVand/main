
#ifndef MANURE_STORAGE_H
#define MANURE_STORAGE_H

#include "../building/storage.h"
#include "../products/manure.h"
#include "../products/plantItem.h"
#include "../base/budget.h"

/*
 *
 * Class: manurestore
 * instanciable class used to simulate a slurry-storage.
 *
 *
 */
class manurestore {
    private:
	    convert conv;
    public:
        manurestore();

        string Alias;
        base * aBase;
        double NH3Emissionfactor;
        double N2Emissionfactor;
        double N2OEmissionfactor;
        double CO2Emissionfactor;
        double CH4Emissionfactor;
        double NMinfactor;
        double OMdegRateConst;    // maximum degradation rate of organic matter

        // ! Depth of manure remaining after cleaning (m)
        double postCleanDepth;

        // ! pointer to a store of whatever is being stored
        storage * theStore;

        // !surface area of the storage in square metres
        double surfaceArea;

        // ! depth of store in metres
        double depth;

        // !pointer to the manure storage (if any) to which manure is sent when cleaning occurs
        manurestore * nextStore;

        // ! name of the manure storage (if any) to which manure is sent when cleaning occurs e.g. slurrystore
        string manureToName;

        // ! number of the manure storage (if any) to which manure is sent when cleaning occurs e.g. 0, if slurry should be sent to slurrystore(0)
        int manureToIndex;

        // ! N budget - used mainly for debugging
        budget Nbudget;

        // ! C budget - used mainly for debugging
        budget Cbudget;

        // ! cumulative ammonia volatilisation
        double cumvolat;

        // ! set to true if the storage is not contained in another building
        bool external;

        // ! manure type to use when using the ALFAM model to model ammonia emission ( must be pig or cattle)
        string ALFAMtype;

        // !true if manure cannot be removed other than by a request for cleaning to occur
        bool locked;

        // ! file for detailed data output (if requested)
        fstream * detailsFile;

        // temporary variables
        // ! emission of NH3 (kg per day)
        double NH3Emission;

        // ! emission of nitrous oxide (kg per day)
        double N2OEmission;

        // ! emission of nitric oxide (kg per day)
        double NOEmission;

        // ! emission of dinitrogen (kg per day)
        double N2Emission;

        // ! emission of CO2 (kg per day)
        double CO2Emission;

        // ! emission of CH4 (kg per day)
        double CH4Emission;

        // ! unique identifier number
        int manureStoreID;

        // !ID number for manurestore receiving manure from this floor (optional)
        int ManureToID;

        // ! true if this is the final destination of the manure before field application
        // bool final;

    public:
        manurestore(string        aName,
                    const int     aIndex,
                    const base *  aOwner,
                    commonData *& runData,
                    commonData *& common);

        manurestore(string       aName,
                    const int    aIndex,
                    const base * aOwner,
                    string       manureType);

        manurestore(const manurestore & s);

        virtual ~manurestore();

        // initialize from file
        virtual manurestore * GetnextStore() {
            return nextStore;
        }

        virtual void SetnextStore(manurestore * aManureStore) {
            nextStore = aManureStore;
        }

        virtual void Setexternal(bool isItExternal) {
            external = isItExternal;
        }

        virtual void SetpostCleanDepth(double apostCleanDepth) {
            postCleanDepth = apostCleanDepth;
        }

        virtual void SetsurfaceArea(double area) {
            surfaceArea = area;
        }

        virtual void Setdepth(double aDepth) {
            depth = aDepth;
        }

        void SetOMdegRateConst(double aValue) {
            OMdegRateConst = aValue;
        }

        virtual string GetName() {
            return aBase -> GetName();
        }

        virtual const int GetIndex() {
            return aBase -> GetIndex();
        }

        virtual string GetAlias() {
            return Alias;
        }

        virtual int GetManureToIndex() {
            return manureToIndex;
        }

        virtual string GetManureToName() {
            return manureToName;
        }

        virtual double GetpostCleanDepth() {
            return postCleanDepth;
        }

        virtual bool Getexternal() {
            return external;
        }

        // virtual void setfinal(bool aVal){final = aVal;}
        virtual double Getdepth() {
            return depth;
        }

        virtual double GetAmountManure();

        virtual double GetTotalNStored();

        virtual double GetTotalCStored();

        virtual double GetTotalDMStored();

        // virtual bool Getfinal(){return final;}
        virtual void DailyUpdate();

        // ! Receive some manure from somewhere

        /*
         * !
         * param someManure pointer to the manure added
         */
        virtual void RcvManure(const manure * someManure);

        // !Sends the amount of manure (or all that is available) to the next storage

        /*
         * !
         * param amount is the amount of manure to send (tonnes fresh weight)
         * param aManure points to the manure to be sent
         */
        virtual void SendManure(const double amount,
                                manure *     aManure);

        // !Cleans the storage (leaves an amount behind, depending on PostCleanDepth). Returns true if successful.

        /*
         * !
         * param aManure pointer to the manure that was removed
         */
        virtual bool CleanStore(manure * aManure);

        // empties the storage

        /*
         * !
         * param aManure pointer to the manure that was removed
         */

        // virtual void FullyCleanStore(manure * aManure);
        // !Returns a pointer to an instance manure, containing the amount requested from storage or all that is available

        /*
         * !
         * param RequestedAmount is the wet weight of manure requested
         * param p points to the type of manure requested
         * param checkAccess is set true if one wants to check to see if the manure is accessible
         */
        virtual product * GetFromManureStore(double RequestedAmount,
                product *                           p,
                bool                                checkAccess = false);

        // Volatize the slurry in the store
        // virtual double Volatize();
        virtual void Volatize(double   Ameantemp,
                              double   relHumidity,
                              double   precipitation,
                              double   surfaceArea,
                              double   resistance,
                              double * volatAmmonia,
                              double * waterEvap,
                              double * N2Emission,
                              double * N2OEmission,
                              double * CH4Emission,
                              double * CO2Emission);

        virtual double GetAvailableAmountOf(const product * p);

        virtual double GetCapacity();

        virtual void SetManure(manure * aManure);

        virtual manure * GetManure() {
            return (manure *) theStore -> GetProductPointer();
        }

        virtual void EndBudget(bool show = false);

        virtual double GetNInput() {
            return Nbudget.GetInput();
        }

        virtual double GetNOutput() {
            return Nbudget.GetOutput();
        }

        virtual void SetNOutput(double aVal) {
            Nbudget.SetOutput(aVal);
        }

        virtual void ReceiveBedding(plantItem * bedding);

        friend ostream & operator <<(ostream &           os,
                                     const manurestore & s);

        virtual void Scale(double anAmount);

        virtual void SetCapacity(double aValue) {
            theStore -> SetCapacity(aValue);
        }

        virtual double GetAmountInManureStore();

        virtual void Volatize(double * Nlost,
                              double * Clost,
                              double * volatAmmonia,
                              double * N2Emission,
                              double * N2OEmission,
                              double * CH4Emission,
                              double * CO2Emission);

        // !Returns a pointer to an instance manure, containing all that is available

        /*
         * !
         * param checkAccess is set true if one wants to check to see if the manure is accessible
         */
        virtual product * EmptyManureStore(bool checkAccess);

        virtual void OpenDetailsFile();

        virtual void CloseDetailsFile();

        virtual void ShowManureStore(fstream * afile,
                                     bool      header);

        virtual string getManureName() {
            return theStore -> GiveNameStoredProduct();
        }

        virtual int getmanureStoreID() {
            return manureStoreID;
        }

        int getManureToID() {
            return ManureToID;
        }

        void setManureToID(int aVal) {
            ManureToID = aVal;
        }

        virtual double isInstore() {
            return false;
        }
};

/*
 *
 * End of Class manurestore
 *
 */
#endif

