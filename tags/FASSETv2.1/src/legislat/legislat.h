/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef _LEGISLATE___H
  #define _LEGISLATE___H

#include <message.h>
#include <taxlegis.h>
#include <product.h>

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
      void Initialize( char* LawApplyFileName)  ;
		void Initializeproductutes();
		void InitializeTax();
		bool IsGreen(char* aFieldName)  ;
		double GetContribution(char* aFieldName);
      taxLegislation* GetCurrentTax(int type);
      void Update(void);
      void CalcLP();
};

   extern legislate * theLegislation;

#endif


