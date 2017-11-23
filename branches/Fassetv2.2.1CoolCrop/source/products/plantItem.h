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

#ifndef __PPLANTIT_H
  #define __PPLANTIT_H
#include "decomposable.h"
#include "product.h"
#include "../base/base.h"
/****************************************************************************\
 Class: plantItem
\****************************************************************************/

class plantItem: public decomposable
{
   protected:
		string tradeName;
		int tradeable;    // can the plantitem be traded
		int primaryProduct; // is the plantitem a primary product (is used for output for gams)

		void InitVariables();

   public:
      // Default Constructor
      plantItem();
      // Constructor with arguments
      plantItem(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      plantItem(const plantItem& aplantItem);
      // Destructor
      virtual ~plantItem();
      // Operator for output
      friend ostream& operator << (ostream& os, const plantItem& aplantItem);
      // Set functions

      string GetTradeName(void) const {return tradeName;}
		int GetTradeable(void) const {return tradeable;}
      int GetPrimaryProduct(void) const {return primaryProduct;}
      // Other functions
      virtual product& operator=(const product& somePlantItem);
      virtual product& operator+(const product& somePlantItem);
      virtual product& operator-(const product& somePlantItem);
      virtual void showstats(ostream *os);
      virtual product* clone() const;
      virtual void ReadParameters(fstream * file);
};

#endif

