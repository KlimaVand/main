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

#ifndef __TETETRAC_H
   #define __TETETRAC_H
#include "techEquip.h"

/****************************************************************************\
 Class: tractor
\****************************************************************************/

class tractor: public techEquip
{
   /* Attributes */
   protected:
      double hp;
      double fuelConsumptionTransport;

   /* Actions */
   private:
      tractor(); // Dissable use of default constructor

   public:
      // Constructor with arguments
      tractor(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      tractor(const tractor& atractor);
      // Destructor
      virtual ~tractor();
      virtual void ReadParameters(fstream * file,char * sectionName=NULL);
      double GetFuelConsumptionTransport() {return fuelConsumptionTransport;};
};

#endif

