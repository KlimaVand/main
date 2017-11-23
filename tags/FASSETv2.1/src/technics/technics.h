/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Noboe Andersen & Poul Lassen
     Date
 Revision   Bj�rn Molt Petersen, May-June 1997
\****************************************************************************/

#ifndef __TECHNICS_H
   #define __TECHNICS_H


#include "teoperat.h"
#include <fiop.h>
#include <periods.h>
#include <clonlist.h>

/****************************************************************************\
 Class: technics
\****************************************************************************/

class technics: public base
{
   /* Attributes */
   private:
   	enum {maxPeriods = 12};
      linkList<operation>* operationList;
      linkList<techEquip>* techFarmList;
      periods * thePeriods;
      int currentPeriod;
      double maxOwnLabour[maxPeriods];
      double maxHiredLabour[maxPeriods];
      double labourSum;
      char * labourFileName;

   /* Actions */
   private:
      technics();                             // Dissable use of default constructor
   	technics& operator=(const technics& t); // Dissable the compilers generation of default assignment operator.
	   technics(const technics& t);            // Dissable the compilers generation of default copy constructor.

      void CreateOperationList(char * aFileName);
      void CreateFarmList(char * aFileName);
      void WritePeriodsToGams(char * fileName);
      void WriteDaysToGams(char * fileName);
      void WriteTechnicsToGams(char * fileName1,char * fileName2,char * fileName3);
      void WriteMaxPeriodWorkToGams(char * fileName);
      void WritePeriodWorkToGams(char * fileName);
      void AddLabourSumThisPeriod(double hours);

   public:
      // Constructor with arguments
      technics(const char * aName, const int aIndex, const base * aOwner = NULL);
      // Destructor
      ~technics();
      periods * GetTechnicsPeriods(){return thePeriods;};
      linkList <techEquip> * GetFarmList(){return techFarmList;};
      void Initialize(char * opFileName, char * techFileNameFarm);
      void CalcLP();
      double CalcLPFields(cloneList <fieldOperationFields> * aFieldOpList, calcLPType mode, period * per);
      void ExecuteFields(fieldOperationFields * aFieldOp);
      void ExecuteAnimals();
      void Reset();
      void YearlyCalc();
      void UpdateFieldLabour(double hours);
      operation * GetFieldTechnique(fieldOperationFields * aFieldOp);

};

extern technics * theTechnics;

#endif


