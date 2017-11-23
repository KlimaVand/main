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

#ifndef __PWATER_H
  #define __PWATER_H
#include "product.h"
//! water class in fasset
class water: public product
{
   protected:
	//! fraction of N in water
		double N_content; // Unit: fraction

      void InitVariables();

   public:
      // Default Constructor
      water();
      // Constructor with arguments
      water(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      water(const water& awater);
      // Destructor
      virtual ~water();

      // Set functions
      void SetN_content(double aN_content) {N_content = aN_content;}
      // Get functions
      double GetN_content(void) const {return N_content;}
      // Other functions
      virtual product& operator=(const product& someWater);
      virtual product& operator+(const product& someWater);
      virtual product& operator-(const product& someWater);

      virtual product* clone() const;
      virtual void ReadParameters(fstream * file,bool criticals, const char *  sectionName);
};

#endif
