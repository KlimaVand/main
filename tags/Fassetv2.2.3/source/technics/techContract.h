/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Noboe Andersen & Poul Lassen
     Date   1996
 Revision   Bjørn Molt Petersen, May-June 1997
\****************************************************************************/

#ifndef __TECHCONT_H
   #define __TECHCONT_H
#include "techEquip.h"

/****************************************************************************\
 Class: techContract
\****************************************************************************/

class techContract: public techEquip
{
   /* Attributes */
   protected:
      double startRentingPrice; // Unit: Dkr.
      double rentingPricePerUnit; // Unit: Dkr./unit (Ha, Ton, Hours)
      double unitsHa; // Number of hectars covered by contract operations. Acuumulated over a year. Must be reset before start of a year.
      double unitsTon; // Number of tons material used by contract operations. Acuumulated over a year. Must be reset before start of a year.
      double unitsHours; // Number of hours used by contract operations. Acuumulated over a year. Must be reset before start of a year.
      int numberOfTimes; // Number of times a contract is used by an operation during a year. Must be reset before start of a year.

   /* Actions */
   private:
      techContract(); // Dissable the use of default constructor

   public:
      // Constructor with arguments
      techContract(const char * aName, const int aIndex, const base * aOwner = NULL);
      // Copy Constructor
      techContract(const techContract& atechContract);
      // Destructor
      virtual ~techContract();
      // Set functions
      void SetstartRentingPrice(double astartRentingPrice) {startRentingPrice = astartRentingPrice;}
      void SetrentingPricePerUnit(double arentingPricePerUnit) {rentingPricePerUnit = arentingPricePerUnit;}
      void SetunitsHa(double aunitsHa) {unitsHa = aunitsHa;}
      void SetunitsTon(double aunitsTon) {unitsTon = aunitsTon;}
      void SetunitsHours(double aunitsHours) {unitsHours = aunitsHours;}
      void SetnumberOfTimes(int anumberOfTimes) {numberOfTimes = anumberOfTimes;}
      // Get functions
      double GetstartRentingPrice(void) {return startRentingPrice;}
      double GetrentingPricePerUnit(void) {return rentingPricePerUnit;}
      double GetunitsHa(void) {return unitsHa;}
      double GetunitsTon(void) {return unitsTon;}
      double GetunitsHours(void) {return unitsHours;}
      int GetnumberOfTimes(void) {return numberOfTimes;}
      // Other functions
      virtual void ReadParameters(fstream * file,char * sectionName=NULL);
};



#endif


