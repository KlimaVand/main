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

#ifndef __TEOPFIML_H
   #define __TEOPFIML_H

#include "fieldOperationTech.h"

/****************************************************************************\
 Class: manualLabourTech
\****************************************************************************/

class manualLabourTech: public fieldOperationTech
{
   /* Attributes */
   protected:
      double hoursPerUnit;

   /* Actions */
   private:
      manualLabourTech();                                     // Dissable the use of default constructor
      manualLabourTech(const manualLabourTech& m);            // Dissable the compilers generation of default copy constructor.
   	manualLabourTech& operator=(const manualLabourTech& m); // Dissable the compilers generation of default assignment operator.

   protected:
      virtual void CalcGangHours();

   public:
      // Constructor with arguments
      manualLabourTech(const char * aName, const int aIndex, const base * aOwner = NULL);
      // Destructor
      virtual ~manualLabourTech();
      virtual void ReadParameters(fstream * file);

};

#endif


