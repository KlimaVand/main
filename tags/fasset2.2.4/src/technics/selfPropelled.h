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

#ifndef __TETESELF_H
   #define __TETESELF_H
#include "techEquip.h"

/****************************************************************************\
 Class: selfPropelled
\****************************************************************************/

class selfPropelled: public techEquip
{
   /* Attributes */
   protected:
      double fuelConsumption;
      double fuelConsumptionTransport;
      double netCapacityWheat;
      double hp;

   /* Actions */
   private:
      selfPropelled(); // Dissable use of default constructor

   public:
      // Constructor with arguments
      selfPropelled(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      selfPropelled(const selfPropelled& aselfPropelled);
      // Destructor
      virtual ~selfPropelled();
      virtual void ReadParameters(fstream * file,char * sectionName=NULL);
      double GetfuelConsumption(){return fuelConsumption;}
      double GetFuelConsumptionTransport() {return fuelConsumptionTransport;};
      double GetNetCapacityWheat() {return netCapacityWheat;};
};

#endif

