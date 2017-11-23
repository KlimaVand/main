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

#ifndef __Panimal_H
  #define __Panimal_H
#include "liveMatter.h"

class animalProduct: public liveMatter
{
   private:
		//! The minimum weight that this type of animal can have in tons
		double startWeight;
		//! The maximum weight that this type of animal can have in tons
		double endWeight;

      void InitVariables();

   public:
      //! Default Constructor
      animalProduct();
      //! Constructor with arguments
      animalProduct(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      //! Copy Constructor
      animalProduct(const animalProduct& aanimalProduct);
      //! Destructor
      virtual ~animalProduct();


      // Get functions
      //! Get the amount of organic N
      double GetN_content() {return orgN_content.n;};
      //! the the start Weight
      double GetStartWeight() {return startWeight;};
      //! the the end Weight
      double GetEndWeight() {return endWeight;};
      //! Get a fraction of weight
      virtual double GetNumber() {return amount/endWeight;}


      // Set functions
      void SetN_content(double aContent) {orgN_content = aContent;};
      void SetEndWeight(double anendWeight) {endWeight = anendWeight;};

      // Other functions
      virtual product& operator=(const product& someanimalProduct);
      virtual product& operator+(const product& someanimalProduct);
      virtual product& operator-(const product& someanimalProduct);

      virtual product* clone() const;
      virtual void ReadParameters(fstream * file,bool criticals,const char *  sectionName);

};

#endif

