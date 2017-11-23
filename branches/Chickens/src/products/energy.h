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

#ifndef __PENERGY_H
  #define __PENERGY_H
#include "product.h"

class energy: public product
{
   protected:
      double kWhPerUnit; // Unit: kWh

      void InitVariables();

   public:
      // Default Constructor
      energy();
      // Constructor with arguments
      energy(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      energy(const energy& aenergy);
      // Destructor
      virtual ~energy();


      double GetkWhPerUnit(void) const {return kWhPerUnit;}
      // Other functions
      virtual product& operator=(const product& someEnergy);
      virtual product& operator+(const product& someEnergy);
      virtual product& operator-(const product& someEnergy);

      virtual product* clone() const;
      virtual void ReadParameters(fstream * file,bool criticals,const char *  sectionName);
};

#endif

