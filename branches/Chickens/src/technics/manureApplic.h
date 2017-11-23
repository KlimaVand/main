/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Created by Nick Hutchings, Sept 2006
\****************************************************************************/

#ifndef __TETEMAN_H
   #define __TETEMAN_H
#include "techEquip.h"

/****************************************************************************\
 Class: manureApplic
\****************************************************************************/

class manureApplic: public techEquip
{
   /* Attributes */
   protected:
      double propBuried;
      double propAreaWetted;
      double volume;
      bool cropContamination;
      double NH3emissionFactor;
   /* Actions */
   private:
      manureApplic(); // Dissable use of default constructor

   public:
      // Constructor with arguments
      manureApplic(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      manureApplic(const manureApplic& amanureApplic);
      // Destructor
      virtual ~manureApplic();
      virtual void ReadParameters(fstream * file,char * sectionName=NULL);
      double GetpropBuried() {return propBuried;};

      double GetNH3emissionFactor() {return NH3emissionFactor;};

};

#endif

