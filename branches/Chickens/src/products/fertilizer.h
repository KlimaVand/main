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

#ifndef __PFERTIL_H
  #define __PFERTIL_H
#include "product.h"
#include "nitrogen.h"
/**
 * Different types of fertilizer
 */
class fertilizer: public product
{
   protected:
   	nitrogen N_content;  // Unit: fraction
      double P_content;  // Unit: fraction
      double K_content;  // Unit: fraction
      double Ca_content; // Unit: fraction
      double Cl_content; // Unit: fraction

      string MineralForm;     // String describing the form of the mineral fertilizer (Solid og Solution)
      double NH4_fraction;    // fraction of N fertilizer that is NH4 (the rest is assumed to be NO3)
      void InitVariables();

   public:
      // Default Constructor
      fertilizer();
      // Constructor with arguments
      fertilizer(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      fertilizer(const fertilizer& afertilizer);
      // Destructor
      virtual ~fertilizer();
      // Operator for output
      friend ostream& operator << (ostream& os, const fertilizer& afertilizer);

      // Set functions
      void SetN_content(nitrogen aN_content) {N_content = aN_content;}
      void SetP_content(double aP_content) {P_content = aP_content;}
      void SetK_content(double aK_content) {K_content = aK_content;}
      void SetCa_content(double aCa_content) {Ca_content = aCa_content;}
      void SetCl_content(double aCl_content) {Cl_content = aCl_content;}
		void SetMineralForm(string aMineralForm) {MineralForm = aMineralForm;}


      // Get functions
      nitrogen GetN_content(void) const {return N_content;}
      double GetP_content(void) const {return P_content;}
      double GetK_content(void) const {return K_content;}
      double GetCa_content(void) const {return Ca_content;}
      double GetCl_content(void) const {return Cl_content;}
		string GetMineralForm() const {return MineralForm;}
      double GetNH4_fraction() {return NH4_fraction;}

      // Other functions
      virtual product& operator=(const product& someFertilizer);
      virtual product& operator+(const product& someFertilizer);
      virtual product& operator-(const product& someFertilizer);

      virtual product*  clone()  const;
      virtual void ReadParameters(fstream * file, bool criticals,const char *  sectionName);

};

#endif

