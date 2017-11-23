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
#include <plivemat.h>

class animalProduct: public liveMatter
{
   private:
		double startWeight;          // t
		double endWeight;            // t
//      double weight; 					//kg
      double animalPrHa;           // pcs pr Ha

      void InitVariables();

   public:
      // Default Constructor
      animalProduct();
      // Constructor with arguments
      animalProduct(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      animalProduct(const animalProduct& aanimalProduct);
      // Destructor
      virtual ~animalProduct();
      // Operator for output
      friend ostream& operator << (ostream& os, const animalProduct& aanimalProduct);

      // Get functions
      double GetN_content() {return orgN_content.n;};
      double GetStartWeight() {return startWeight;};
      double GetEndWeight() {return endWeight;};
//      double GetWeight() {return weight;};
      double GetAnimalPrHa() {return animalPrHa;};

      // Set functions
      void SetN_content(double aContent) {orgN_content = aContent;};
      void SetEndWeight(double anendWeight) {endWeight = anendWeight;};

      // Other functions
      virtual product& operator=(const product& someanimalProduct);
      virtual product& operator+(const product& someanimalProduct);
      virtual product& operator-(const product& someanimalProduct);
      void showstats(ostream *os);
      virtual product* clone() const;
      virtual void ReadParameters(fstream * file);
      virtual double GetNumber() {return amount/endWeight;}
};

#endif

