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

#ifndef __TETEINST_H
   #define __TETEINST_H
#include "techEquip.h"

/****************************************************************************\
 Class: installation
\****************************************************************************/

class installation: public techEquip
{
   /* Attributes */
   protected:
      double capacity;
      double electricityConsumption;

   /* Actions */
   private:
      installation(); // Dissable the use of default constructor

   public:
      // Constructor with arguments
      installation(const char * aName, const int aIndex, const base * aOwner = NULL);
      // Copy Constructor
      installation(const installation& ainstallation);
      // Destructor
      virtual ~installation();
      virtual void ReadParameters(fstream * file,char * sectionName=NULL);
      double GetelectricityConsumption(void) {return electricityConsumption;}
      double Getcapacity(void) {return capacity;}
};

#endif

