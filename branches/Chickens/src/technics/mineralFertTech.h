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

#ifndef __TEOPFIMI_H
   #define __TEOPFIMI_H
#include "../products/fertilizer.h"
#include "../products/products.h"
#include "fieldOperationTech.h"

/****************************************************************************\
 Class: mineralFertTech
\****************************************************************************/

class mineralFertTech: public fieldOperationTech
{
   /* Attributes */
   protected:
      double prepLoading;
      double roadSpeed;
      double vol;
      double fillingTimeSpreader;
      double prepFilling;
      double changingTrailer;
      double loadingTime;
      fertilizer * theFertilizer1;
      fertilizer * theFertilizer2;
      fertilizer * theFertilizer3;

   /* Actions */
   private:
      mineralFertTech();                                    // Dissable the use of default constructor
      mineralFertTech(const mineralFertTech& m);            // Dissable the compilers generation of default copy constructor.
   	mineralFertTech& operator=(const mineralFertTech& m); // Dissable the compilers generation of default assignment operator.

   protected:
      virtual void CalcGangHours();

   public:
      // Constructor with arguments
      mineralFertTech(const char * aName, const int aIndex, const base * aOwner = NULL);
      // Destructor
      virtual ~mineralFertTech();
      virtual void ReadParameters(fstream * file);
      void SetTheFertilizers(fertilizer * aFertilizer1, fertilizer * aFertilizer2, fertilizer * aFertilizer3);
      void ClearTheFertilizers();
      void UpdateProducts(bool contractor=false);
      void UpdateFertilizer(fertilizer* aFertilizer);
};

#endif


