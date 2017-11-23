/*
 *
 *  Author   Lars Noboe Andersen & Poul Lassen
 * Date
 * Revision   Bjørn Molt Petersen, May-June 1997
 *
 */

#ifndef __TECHNICS_H
#define __TECHNICS_H

#include "operation.h"
#include "../fields/fieldOperation/fieldOperationFields.h"
#include "../base/periods.h"
#include "../base/cloneList.h"

/*
 *
 * Class: technics
 *
 */
class technics:
    public base
{

    /* Attributes */
    private:
        enum { maxPeriods = 12 };

        linkList<operation> * operationList;
        linkList<techEquip> * techFarmList;
        periods *             thePeriods;
        int                   currentPeriod;
        double                maxOwnLabour[maxPeriods];
        double                maxHiredLabour[maxPeriods];
        double                labourSum;
        string                labourFileName;

        /* Actions */

    private:
        technics();                      // Dissable use of default constructor

        technics & operator = (const technics & t);    // Dissable the compilers generation of default assignment operator.

        technics(const technics & t);    // Dissable the compilers generation of default copy constructor.

        void CreateOperationList(string aFileName);

        void CreateFarmList(string aFileName);

        void WritePeriodsToGams(string fileName);

        void WriteDaysToGams(string fileName);

        void WriteTechnicsToGams(string fileName1,
                                 string fileName2,
                                 string fileName3);

        void WriteMaxPeriodWorkToGams(string fileName);

        void WritePeriodWorkToGams(string fileName);

        void AddLabourSumThisPeriod(double hours);

    public:
        technics(string       aName,
                 const int    aIndex,
                 const base * aOwner = nullptr);

        ~technics();

        periods * GetTechnicsPeriods() {
            return thePeriods;
        }

        linkList<techEquip> * GetFarmList() {
            return techFarmList;
        }

        void Initialize(string opFileName,
                        string techFileNameFarm);

        void CalcLP();

        double CalcLPFields(cloneList<fieldOperationFields> * aFieldOpList,
                            calcLPType                        mode,
                            period *                          per);

        void ExecuteFields(fieldOperationFields * aFieldOp);

        void ExecuteAnimals();

        void Reset();

        void YearlyCalc();

        void UpdateFieldLabour(double hours);

        operation * GetFieldTechnique(fieldOperationFields * aFieldOp);
};


extern technics * theTechnics;
#endif

