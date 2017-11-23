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

#ifndef __TETEIMPL_H
   #define __TETEIMPL_H
#include "techEquip.h"

/****************************************************************************\
 Class: implement
\****************************************************************************/

class implement: public techEquip
{
   /* Attributes */
   protected:
      double fuelConsumption;

   /* Actions */
   private:
      implement(); // Dissable the use of default constructor

   public:
      // Constructor with arguments
      implement(const char * aName, const int aIndex, const base * aOwner = NULL);
      // Copy Constructor
      implement(const implement& aimplement);
      // Destructor
      virtual ~implement();
      virtual void ReadParameters(fstream * file,char * sectionName=NULL);
      double GetfuelConsumption(void) {return fuelConsumption;}
};

#endif

