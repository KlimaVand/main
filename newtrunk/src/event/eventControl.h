
#ifndef __EVENT_H
#define __EVENT_H

#include "../base/bstime.h"
#include "../technics/technics.h"
#include "../manager/operationsManager.h"
#include "../manager/manureManager.h"
#include "../fields/cropRotation.h"
#include "../livestock/livestock.h"
#include "../products/products.h"
#include "../base/settings.h"
#include "../building/buildings.h"
#include "../base/budget.h"

class eventControl:
    public base
{

    /* Attributes */
    private:
        
        bool           WithoutFields;
        cropRotation * theCropRotation;

        // manager * theManager;
        operationsManager * theOperational_manager;
        budget              Nbudget;

        /* Actions */
        void InitVariables();

    public:
        eventControl();

        ~eventControl();

        void Initialize(bsTime * stopTime,
                        string   inputDir,
                        string   climateDir,
                        string   climateFilename,
                        string   outputPath);

        void ReceivePlan(string fileExtension,
                         string inputDir,int yearNumber);

        void DailyUpdate();

        void Simulation(int    runNumber,
                        bsTime stopTime,
                        string inputDir,
                        string outputDir,
                        string climateDir,
                        string climateFilename,
                        string economicIndicatorDate,
                        string environmentalIndicatorDate);

        void EndSimulation(bool show);

        void GiveIndicator(indicatorTypes indicatorType,
                           int            yearNumber);

        cropRotation * getCropRotation() {
            return theCropRotation;
        }
};
#endif

