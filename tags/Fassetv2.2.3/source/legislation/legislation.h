/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef _LEGISLATE___H
  #define _LEGISLATE___H

#include "../base/message.h"
#include "taxLegislation.h"
#include "../products/product.h"
#include "../base/linklist.h"

class legislate: public base
{
	protected:
   	linkList <product>* fieldAttribList;
      linkList <taxLegislation>* taxList;
      int simulationYear;
      double GreenPrecent;
      double Fallow;
      double Nprice;
      double Pprice;
      double Kprice;
      double Nmax;

      void InitVariables();

	public:
		// Default Constructor
		legislate();
		// Constructor with arguments
		legislate(char * aName, int aIndex = -1, base * aOwner = NULL);
		// Destructor
		~legislate();
		// Other functions
      void Initialize( string LawApplyFileName)  ;
		void Initializeproductutes();
		void InitializeTax();
		bool IsGreen(char* aFieldName)  ;
		double GetContribution(char* aFieldName);
      double GetCurrentTax(int type);
      double GetCurrentLevel(int type);
      void Update(void);
      void CalcLP();
};

   extern legislate * theLegislation;

#endif


