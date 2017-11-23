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

#ifndef __PMILK_H
  #define __PMILK_H
#include "product.h"

class milk: public product
{
   private:
   	double fat;
      double dryMatter;
      double protein;
      double urea;

      void InitVariables();

   public:
      // Default Constructor
      milk();
      // Constructor with arguments
      milk(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      milk(const milk& amilk);
      // Destructor
      virtual ~milk();
      // Operator for output

      // Get functions
      double Getfat(void) const {return fat;}
      double GetdryMatter(void) const {return dryMatter;}



      // Other functions
      virtual product& operator=(const product& someMilk);
      virtual product& operator+(const product& someMilk);
      virtual product& operator-(const product& someMilk);
      product*  clone() const;
      virtual void ReadParameters(fstream * file, bool criticals,const char *  sectionName);
};

#endif

