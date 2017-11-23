/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __PRICE_H
  #define __PRICE_H

	#include "../base/base.h"


class price: public base
{
protected:
	double* sellPriceSet;
	double* buyPriceSet;
	double baseSellPrice; // Unit: kr
	double actualSellPrice; // Unit: kr
	double expectedSellPrice; // Unit: kr
	double baseBuyPrice; // Unit: kr
	double actualBuyPrice; // Unit: kr
	double expectedBuyPrice; // Unit: kr
	double lambda;
	double alpha;
	int useNormalization;
	double rightRange;
	double leftRange;

	void InitVariables();
	double GetPseudo(double leftRange,double rightRange,double basePrice);

public:
	// Default Constructor
	price();
	// Constructor with arguments
	price(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
	// Copy Constructor
	price(const price& aprice);
	// Destructor
	virtual ~price();
	// Operator for output
	friend ostream& operator << (ostream& os, const price& aprice);

	// Get functions
	double GetbaseSellPrice(void) {return baseSellPrice;}
	double GetactualSellPrice(void) {return actualSellPrice;}
	double GetbaseBuyPrice(void) {return baseBuyPrice;}
	double GetactualBuyPrice(void) {return actualBuyPrice;}
	double GetExpectedSellPriceYear(int year);
	double GetExpectedBuyPriceYear(int year);
	//string Getname(void) {return name;}

	// Other functions
	void ReadParameters(fstream * file, int duration)  ;
	virtual void UpdatePrices(int simulationYear);
};

#endif

