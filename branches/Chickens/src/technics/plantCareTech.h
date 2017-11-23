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

#ifndef __TEOPFIPC_H
   #define __TEOPFIPC_H
#include "../products/water.h"
#include "../products/chemical.h"
#include "../products/products.h"
#include "fieldOperationTech.h"

/****************************************************************************\
 Class: plantCareTech
\****************************************************************************/

class plantCareTech: public fieldOperationTech
{
   /* Attributes */
   protected:
      double roadSpeed;
      double specificArea;
      double fillingTime;
      double prepFillingTank;


      water * theWater;
      chemical * theChemical1;
      chemical * theChemical2;
      chemical * theChemical3;

   /* Actions */
   private:
      plantCareTech();                                  // Dissable the use of default constructor
      plantCareTech(const plantCareTech& p);            // Dissable the compilers generation of default copy constructor.
   	plantCareTech& operator=(const plantCareTech& p); // Dissable the compilers generation of default assignment operator.

   protected:
      virtual void CalcGangHours();

   public:
      // Constructor with arguments
      plantCareTech(const char * aName, const int aIndex, const base * aOwner = NULL);
      // Destructor
      virtual ~plantCareTech();
      virtual void ReadParameters(fstream * file);
      void SetTheWaterAndChemicals(water * aWater, chemical * aChemical1, chemical * aChemical2, chemical * aChemical3);
      void ClearTheWaterAndChemicals();

      virtual void UpdateProducts(bool contractor=false);
      virtual void UpdateChemical(chemical* aChemical);
};

#endif


