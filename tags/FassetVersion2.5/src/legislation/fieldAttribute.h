/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
   Author
     Date
 Revision
 ----------------------------------------------------------------------------
      Log
\****************************************************************************/

#ifndef __FIELDATT_H
  #define __FIELDATT_H

#include "../base/base.h"
#include <string.h>
/****************************************************************************\
 Class: fieldAttrib
\****************************************************************************/

class fieldAttrib: public base
{
	/* Attributes */
	private:

	protected:

		 double   contributionThisYear; //
		  int     contributionIndex; //
		  bool isgreen;
		  double* contributionList; //
		  int 	 boundary;
		  char    fieldName[41]; //


	public:


	/* Actions */
	private:

	protected:

	public:
		// Default Constructor
		fieldAttrib();
		// Constructor with arguments
		fieldAttrib(char * aName, int aIndex = -1, base * aOwner = nullptr);
		// Destructor
		~fieldAttrib();
		// Set and Get functions
		void SetcontributionThisYear(double acontributionThisYear) {contributionThisYear = acontributionThisYear;}
		void SetcontributionIndex(int acontributionIndex) {contributionIndex = acontributionIndex;}
		void SetcontributionList(double* acontributionList) {contributionList = acontributionList;}
		void SetfieldName(char * afieldName) {strcpy(fieldName,afieldName);}
		void Setisgreen(bool aisgreen) {isgreen=aisgreen;}
		void Setboundary(int aboundary) {boundary=aboundary;}
		int  Getboundary() {return boundary;}
		bool Getisgreen() {return isgreen;}
		double GetcontributionThisYear(void) {return contributionThisYear;}
		int GetcontributionIndex(void) {return contributionIndex;}
		double* GetcontributionList(void) {return contributionList;}
		char * GetfieldName() {return fieldName;}
		double GetContributionAmount(void) {return contributionThisYear;}
		// Other functions
      void Initialize(char* afieldName,bool aisgreen, double* acontributionList,int aboundary)  ;
      bool Update(void)  ;
};

#endif


