/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive    price object
   Author    Mikkel
 Revision    rewritten by JB okt-97: sell and buy prices added,
 							reads its own parameters, + lot of other changes
\****************************************************************************/
#include "price.h"
#include "../base/common.h"

/****************************************************************************\
  Default Constructor
\****************************************************************************/
price::price()
: base()
{
	InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
price::price(const char * aName, const int aIndex, const base * aOwner)
: base(aName, aIndex, aOwner)
{
	InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
price::price(const price& aprice)
: base(aprice)
{
	sellPriceSet = aprice.sellPriceSet;
	buyPriceSet = aprice.buyPriceSet;
	baseSellPrice = aprice.baseSellPrice;
	actualSellPrice = aprice.actualSellPrice;
	expectedSellPrice = aprice.expectedSellPrice;
	baseBuyPrice = aprice.baseBuyPrice;
	actualBuyPrice = aprice.actualBuyPrice;
	expectedBuyPrice = aprice.expectedBuyPrice;
	lambda = aprice.lambda;
	alpha = aprice.alpha;
	useNormalization = aprice.useNormalization;
	rightRange = aprice.rightRange;
	leftRange = aprice.leftRange;
	//name = aprice.name;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
price::~price()
{
	if (sellPriceSet)
		delete sellPriceSet;
	if (buyPriceSet)
		delete buyPriceSet;
}

/****************************************************************************\
\****************************************************************************/
void price::InitVariables()
{
	sellPriceSet = NULL;
	buyPriceSet = NULL;
	baseSellPrice = 0.0;
	actualSellPrice = 0.0;
	expectedSellPrice = 0.0;
	baseBuyPrice = 0.0;
	actualBuyPrice = 0.0;
	expectedBuyPrice = 0.0;
	lambda = 0.0;
	alpha = 1.0;               // 1.0 => no inflation
	rightRange = 0.0;
	leftRange = 0.0;
}

/**
 * updates baseprices according to whether it's fixed or inflation.
 * ActualPrice baseprice with noise,
 * expectedPrice price next year where you expectation is corrected according to the
 * actual price this year
 */
void price::UpdatePrices(int simulationYear)
{
	if (sellPriceSet)
		baseSellPrice = sellPriceSet[simulationYear];          // fixed prices
	else
		baseSellPrice  = alpha*baseSellPrice;
	actualSellPrice   = GetPseudo(leftRange,rightRange,baseSellPrice);
	expectedSellPrice = alpha*(expectedSellPrice + lambda*(actualSellPrice - expectedSellPrice));
	if (buyPriceSet)
		baseBuyPrice = buyPriceSet[simulationYear];          // fixed prices
	else
		baseBuyPrice   = alpha*baseBuyPrice;
	actualBuyPrice    = GetPseudo(leftRange,rightRange,baseBuyPrice);
	expectedBuyPrice  = alpha*(expectedBuyPrice + lambda*(actualBuyPrice - expectedBuyPrice));
}

/****************************************************************************\
\****************************************************************************/
double price::GetExpectedSellPriceYear(int year)
{
	if (year==0)
		return expectedSellPrice;
	else
		return pow(alpha,year)*(expectedSellPrice + lambda*(actualSellPrice - expectedSellPrice));
}

/****************************************************************************\
\****************************************************************************/
double price::GetExpectedBuyPriceYear(int year)
{
	if (year==0)
		return expectedBuyPrice;
	else
		return pow(alpha,year)*(expectedBuyPrice + lambda*(actualBuyPrice - expectedBuyPrice));
}

/****************************************************************************\
 Function:   GetPseudo
 Parameters:  double lrange,  double rrange,  double price
 returns a price with noise (according to triangle noise)
\****************************************************************************/
double price::GetPseudo( double lrange,  double rrange,  double price )
{
	double result, min, max, nr;
	min=price-(lrange/100.0)*price;
	max=price+(rrange/100.0)*price;
	nr=( (rand() % 1001)/1000.0);
	if (nr>0.5)
		result=min + sqrt(nr*(max-min)*(price-min));
	else
		result=max - sqrt( (1-nr)*(max-min)*(max-price));
	return result;
}

/****************************************************************************\
 Function:   ReadParameters
 Reads the price objects parameters
\****************************************************************************/
void price::ReadParameters(fstream * file,int simulationLength)
{
	Setfile(file);
	SetCritical();
	int ayear=0;
	double someyearprice	=0.0;
	FindSection("price",Index);
	//char instring;
	GetParameter("pricename" ,Name);
	UnsetCritical();
	GetParameter("rightrange",&rightRange);
	GetParameter("leftrange",&leftRange);
	if ((GetParameter("sellYear0",&someyearprice)) || (GetParameter("buyYear0",&someyearprice)))     // fixed prices
	{
		sellPriceSet = new double[simulationLength];
		buyPriceSet  = new double[simulationLength];
		while(ayear<simulationLength)
		{
			if (GetParameter("sellYear"+ayear,&someyearprice))
				sellPriceSet[ayear] = someyearprice;
			else
				sellPriceSet[ayear] = (ayear==0) ? 0 : sellPriceSet[ayear-1];     // no price given

			if (GetParameter("buyYear"+ayear,&someyearprice))
				buyPriceSet[ayear] = someyearprice;
			else
				buyPriceSet[ayear] = (ayear==0) ? 0 : buyPriceSet[ayear-1];       // no price given
			ayear++;
		}
		baseSellPrice  = sellPriceSet[0];
		baseBuyPrice = buyPriceSet[0];
	}
	else                                                                       // no fixed prices
	{
		GetParameter("basesellprice"    ,&baseSellPrice);
		GetParameter("basebuyprice"    ,&baseBuyPrice);
	}
	expectedSellPrice = baseSellPrice;
	actualSellPrice = GetPseudo(leftRange,rightRange,baseSellPrice);
	expectedBuyPrice = baseBuyPrice;
	actualBuyPrice   = GetPseudo(leftRange,rightRange,baseBuyPrice);
	GetParameter("alpha"    ,&alpha);
	GetParameter("lambda"    ,&lambda);
	Setfile(NULL);
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const price& aprice)
{
	os << setiosflags(ios::left) << setw(40) << "baseSellPrice " << aprice.baseSellPrice << endl;
	os << setiosflags(ios::left) << setw(40) << "actualSellPrice " << aprice.actualSellPrice << endl;
	os << setiosflags(ios::left) << setw(40) << "expectedSellPrice " << aprice.expectedSellPrice << endl;
	os << setiosflags(ios::left) << setw(40) << "baseBuyPrice " << aprice.baseBuyPrice << endl;
	os << setiosflags(ios::left) << setw(40) << "actualBuyPrice " << aprice.actualBuyPrice << endl;
	os << setiosflags(ios::left) << setw(40) << "expectedBuyPrice " << aprice.expectedBuyPrice << endl;
	os << setiosflags(ios::left) << setw(40) << "lambda " << aprice.lambda << endl;
	os << setiosflags(ios::left) << setw(40) << "alpha " << aprice.alpha << endl;
	os << setiosflags(ios::left) << setw(40) << "useNormalization " << aprice.useNormalization << endl;
	os << setiosflags(ios::left) << setw(40) << "rightRange " << aprice.rightRange << endl;
	os << setiosflags(ios::left) << setw(40) << "leftRange " << aprice.leftRange << endl;
	os << setiosflags(ios::left) << setw(40) << "name " << aprice.Name << endl;
	return os;
}

