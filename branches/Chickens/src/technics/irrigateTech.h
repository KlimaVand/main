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

#ifndef __TEOPFIIR_H
   #define __TEOPFIIR_H
#include "../products/water.h"
#include "../products/products.h"
#include "fieldOperationTech.h"

/****************************************************************************\
 Class: irrigateTech
\****************************************************************************/

class irrigateTech: public fieldOperationTech
{
   /* Attributes */
   protected:
      double workingLength;
      double constantPerShift;
      double variablePerLength;
      double hoursPerDay;
      water * theWater;

   /* Actions */
   private:
      irrigateTech();                                 // Dissable the use of default constructor
      irrigateTech(const irrigateTech& i);            // Dissable the compilers generation of default copy constructor.
   	irrigateTech& operator=(const irrigateTech& i); // Dissable the compilers generation of default assignment operator.

   protected:
      virtual void CalcGangHours();
      virtual void CalcGangElectricity();

   public:
      // Constructor with arguments
      irrigateTech(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Destructor
      virtual ~irrigateTech();
      virtual void ReadParameters(fstream * file);
      void SetTheWater(water * aWater);
      void ClearTheWater();

      virtual void UpdateProducts(bool contractor=false);
};

#endif


