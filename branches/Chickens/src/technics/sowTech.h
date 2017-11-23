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

#ifndef __TEOPFISO_H
   #define __TEOPFISO_H
#include "../products/seed.h"
#include "fieldOperationTech.h"

/****************************************************************************\
 Class: sowTech
\****************************************************************************/

class sowTech: public fieldOperationTech
{
   /* Attributes */
   protected:
      double prepFillSowing;
      double fillingTime1Sowing;
      double fillingTime2Sowing;
      double fillingAmount1;
      double fillingAmount2;
      double roadSpeed;
      double changingTrailer;
      double loadingTime;
      double prepLoading;
      int    numOfSeeds;
      seed * theSeeds[MaxPlants];

   /* Actions */
   private:
      sowTech();                            // Dissable the use of default constructor
      sowTech(const sowTech& s);            // Dissable the compilers generation of default copy constructor.
   	sowTech& operator=(const sowTech& s); // Dissable the compilers generation of default assignment operator.

   protected:
      virtual void CalcGangHours();

   public:
      // Constructor with arguments
      sowTech(const char * aName, const int aIndex, const base * aOwner = NULL);
      // Destructor
      virtual ~sowTech();
      virtual void ReadParameters(fstream * f);
      void AddSeed(seed * s);
      void ClearTheSeeds();

      virtual void UpdateProducts(bool contractor=false);
      virtual void ResetSeeds(){numOfSeeds=0;};
};

#endif


