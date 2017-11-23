/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Revision			july major revision of all product's.         (JB)
               	All operators calls the superclass
                  operator check's
                  	if similar types, storage etc  added/subtracted
                     if amount=0 the object gets the same unit, storage etc
                     	as the object that is added to it
\****************************************************************************/

#ifndef __PREMIUM_H
  #define __PREMIUM_H
#include "product.h"
#include "../base/commonData.h"
class premium: public product
{
   protected:
		string plantName; // Unit: fraction
      bool green;

      void InitVariables();

   public:
      // Default constructor
      premium();
      // Constructor with arguments
      premium(const char * aName, const int aIndex = -1, const base * aOwner = nullptr);
      // Copy Constructor
      premium(const premium& aPremium);
      // Destructor
      virtual ~premium();
      // Operator for output
      friend ostream& operator << (ostream& os, const premium& apremium);
      // Get functions
      string GetPlantName(void) const {return plantName;}
      bool GetGreen() const {return green;};
      // Other functions
      virtual product& operator=(const product& apremium);
      virtual product& operator+(const product& apremium);
      virtual product& operator-(const product& apremium);
      void showstats(ostream *os);
      virtual product* clone() const;
      virtual void ReadParameters(commonData  * file,const char * sectionName);
};

#endif


