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

		void InitializeTax();

		double GetContribution(char* aFieldName);
      double GetCurrentTax(int type);
      double GetCurrentLevel(int type);
      void Update(void);

};

   extern legislate * theLegislation;

#endif


