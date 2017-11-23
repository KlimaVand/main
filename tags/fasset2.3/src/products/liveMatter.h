/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class				New class added by JB
Revision			july major revision of all product's.         (JB)
               	All operators calls the superclass
                  operator check's
                  	if similar types, storage etc  added/subtracted
                     if amount=0 the object gets the same unit, storage etc
                     	as the object that is added to it
JB Added RemoveWater function that converts a "wet" product to a "dry"
\****************************************************************************/

#ifndef __PLIVEMAT_H
  #define __PLIVEMAT_H

#include "../products/product.h"
#include "../products/nitrogen.h"

/**
 * things that is alive
 */
class liveMatter: public product
{
   protected:
	//! fraction N in orgN of total amount
      nitrogen orgN_content;
      //! fraction N in NH4 of total amount
      nitrogen NH4_content;
      //! fraction N in NO3 of total amount
      nitrogen NO3_content;
      //! fraction P of total amount
   	  double P_content;
   	//! fraction K of total amount
      double K_content;
      // a fraction of dryMatter
      double dryMatter;
      //! fraction C of total amount
      double C_content;
      //! fraction S of total amount
      double S_content;
      double organic;
      //!code used to link plantItem and feedItem
      int code;
      void InitVariables();

   public:
      // Default Constructor
      liveMatter();
      // Constructor with arguments
      liveMatter(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
      // Copy Constructor
      liveMatter(const liveMatter& aliveMatter);
      // Destructor
      virtual ~liveMatter();


      // Set functions
      virtual void SetorgN_content(const nitrogen aorgN_content) {orgN_content = aorgN_content;}
      virtual void SetNH4_content(const nitrogen aNH4_content) {NH4_content = aNH4_content;}
      virtual void SetNO3_content(const nitrogen aNO3_content) {NO3_content = aNO3_content;}
      virtual void SetP_content(const double aP_content) {P_content = aP_content;}
      virtual void SetK_content(const double aK_content) {K_content = aK_content;}
      virtual void SetDryMatter(const double DM) {dryMatter = DM;}
      virtual void Setorganic(double aorganic) {organic = aorganic;}
      virtual void SetC_content(const double CC) {C_content = CC;}
      virtual void SetS_content(const double SC) {S_content = SC;}
      virtual void ClearOrgN_content() {orgN_content.Clear();}
      virtual void SetCode(int aCode) {code=aCode;};

      // Get functions
      virtual nitrogen GetorgN_content() const {return orgN_content;}
      virtual nitrogen GetNH4_content() const {return NH4_content;}
      virtual nitrogen GetNO3_content() const {return NO3_content;}
      virtual nitrogen GetTotalN() {return GetAllN();};
      virtual nitrogen GetAllN();
      virtual double GetTotalC() {return amount * C_content;};
      virtual double GetP_content() const {return P_content;}
      virtual double GetK_content() const {return K_content;}
      virtual double GetdryMatter() const {return dryMatter;}
      virtual double Getorganic(void) const {return organic;}
      virtual double GetC_content() const {return C_content;}
      virtual double GetS_content() const {return S_content;}
      virtual int GetCode() const {return code;};
      virtual double Getwater() const {return amount * (1-dryMatter);}

      // Other functions
      virtual void RemoveWater();
      virtual product& operator=(const product& someliveMatter);
      product& operator+(const product& someliveMatter);
      virtual product& operator-(const product& someliveMatter);
      product*  clone() const;
      virtual void ReadParameters(fstream * file, bool criticals,const char *  sectionName);
		virtual double SubtractWater(double waterToSubtract);
      virtual void AddWater(double waterToAdd) {SubtractWater(-waterToAdd);};
};

#endif


