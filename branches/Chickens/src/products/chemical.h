/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Revision       	july major revision of all product's.         (JB)
               	All operators calls the superclass
                  operator check's
                  	if similar types, storage etc  added/subtracted
                     if amount=0 the object gets the same unit, storage etc
                     	as the object that is added to it
\****************************************************************************/

#ifndef __PCHEMIC_H
  #define __PCHEMIC_H
#include "product.h"

class chemical: public product
{
   protected:
	//! Enum for types of chemicals: herbicide, fungicide, pesticicde
      int type;
      //! how much of the chemical that is reacting
      double activeSubstancePerUnit;
      double amountPerTFI;

      void InitVariables();
   	int GetChemType(string aChemType);

   public:
      // Default Constructor
      chemical();
      // Constructor with arguments
      chemical(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      chemical(const chemical& achemical);
      // Destructor
      virtual ~chemical();

      // Set functions
   	void Settype(const int atype) {type = atype;}

      // Get functions
      int Gettype(void) const {return type;}
      double GetactiveSubstancePerUnit(void) const {return activeSubstancePerUnit;}
      double GetAmountPerTFI(void) const {return amountPerTFI;}
      // Other functions
      virtual product& operator=(const product& someChemical);
      virtual product& operator+(const product& someChemical);
      virtual product& operator-(const product& someChemical);

		virtual product*  clone() const;

      void SetByTFI(double aTFI);
      virtual void ReadParameters(fstream * file,bool criticals,const char *  sectionName );
};

#endif


