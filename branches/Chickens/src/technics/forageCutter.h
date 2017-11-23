/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Created by Nick Hutchings, Sept 2006
\****************************************************************************/

#ifndef __TETECUT_H
   #define __TETECUT_H
#include "techEquip.h"

/****************************************************************************\
 Class: forageCutter
\****************************************************************************/
class forageCutter: public techEquip
{
   /* Attributes */
   protected:
      double residualHeight;
   /* Actions */
   private:
      forageCutter(); // Dissable use of default constructor

   public:
      // Constructor with arguments
      forageCutter(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      forageCutter(const forageCutter& aforageCutter);
      // Destructor
      virtual ~forageCutter();
      virtual void ReadParameters(fstream * file,char * sectionName=NULL);

};

#endif

