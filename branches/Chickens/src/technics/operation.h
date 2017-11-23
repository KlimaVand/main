/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
   Author   Lars Noboe Andersen & Poul Lassen
     Date
 Revision   Bjørn Molt Petersen, May-June 1997
\****************************************************************************/

#ifndef __TEOPERAT_H
   #define __TEOPERAT_H

#include "../base/IndicatorOutput.h"
#include "../base/operationNames.h"
#include "gang.h"

/****************************************************************************\
 Class: operation
\****************************************************************************/

class operation: public base
{
   /* Attributes */
   protected:
      string operationName;
      int operationNumber;
      double cost,oldCost;
      linkList <string> * techEquipList;
      gang * aGang;

   /* Actions */
   private:
      operation();                              // Dissable use of default constructor
      operation(const operation& o);            // Dissable the compilers generation of default copy constructor.
   	operation& operator=(const operation& o); // Dissable the compilers generation of default assignment operator.

   protected:
      void Setcost(double acost) {cost = acost;}
      void SetaGang(gang * aaGang) {aGang = aaGang;}

      void UpdateFarmList(linkList <techEquip> * aTechFarmList);
      virtual void UpdateProducts(bool contractor=false){};

   public:
      // Constructor with arguments
      operation(const char * aName, const int, const base * aOwner = NULL);
      // Destructor
      virtual ~operation();
      void SetoperationName(const string& aoperationName) {operationName = aoperationName;}

      int GetoperationNumber();

      virtual void ReadParameters(fstream * file);
      int CreateGang(linkList <techEquip> * aTechFarmList);
      void DeleteGang();
      virtual void CalcGangHours(){};
      virtual void CalcGangFuel(){};
      virtual void CalcGangElectricity();
      void Update(linkList <techEquip> * aTechFarmList);
};

#endif


