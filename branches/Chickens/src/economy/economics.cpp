/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "economics.h"
#include "../products/products.h"
#include "../legislation/legislation.h"

/****************************************************************************\
  Default Constructor
\****************************************************************************/
economics::economics()
	: base()
{

}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
economics::economics(const char * aName, const int aIndex, const base * aOwner)
	: base(aName, aIndex, aOwner)
{
}

/****************************************************************************\
  Destructor
\****************************************************************************/
economics::~economics()
{
}



/****************************************************************************\
\****************************************************************************/
void economics::GiveIndicator()
{
   double result = 0.0;

   double Nlevy1, Nlevy2, Nlevy3, Nlevy4;

   Nlevy1  = theOutput->GetAmount(environmentalIndicator,"31.01 N from mineral fertilizer");
   Nlevy1 += theOutput->GetAmount(environmentalIndicator,"35.02 N from imported manure");
   Nlevy2  = Nlevy1;
   Nlevy2 += theOutput->GetAmount(environmentalIndicator,"35.01 N from imported feed");
   Nlevy3  = Nlevy2;
   Nlevy3 += theOutput->GetAmount(environmentalIndicator,"31.04 N from fixation");
   Nlevy3 -= theOutput->GetAmount(environmentalIndicator,"33.21 N in sold cattle")
   			- theOutput->GetAmount(environmentalIndicator,"34.20 N in sold pigs");
   Nlevy3 -= theOutput->GetAmount(environmentalIndicator,"35.20 N in sold plant products");
	Nlevy4  = theLegislation->GetCurrentTax(3)*(Nlevy3-theLegislation->GetCurrentLevel(3));
   Nlevy1 *= theLegislation->GetCurrentTax(0);
	Nlevy2 *= theLegislation->GetCurrentTax(1);
	Nlevy3 *= theLegislation->GetCurrentTax(2);

   theOutput->AddIndicator(economicIndicator,"06.80 Nlevy1 (mineral fertilizer)","Dkr",Nlevy1);
	theOutput->AddIndicator(economicIndicator,"06.81 Nlevy2 (mineral fertilizer + imported feed)","Dkr",Nlevy2);
	theOutput->AddIndicator(economicIndicator,"06.82 Nlevy3 (import-export)","Dkr",Nlevy3);
	theOutput->AddIndicator(economicIndicator,"06.83 Nlevy4 (import-export) above level","Dkr",Nlevy4);
   result=theOutput->GetAmount(economicIndicator,"05.10 Plant products sold");

   result = Nlevy1+Nlevy2+Nlevy3+Nlevy4;
   result+=theOutput->GetAmount(economicIndicator,"05.10 Plant products sold");
   result+=theOutput->GetAmount(economicIndicator,"05.30 Sows sold");
   result+=theOutput->GetAmount(economicIndicator,"05.31 Baconers sold");
   result+=theOutput->GetAmount(economicIndicator,"05.32 Piglets sold");
   result+=theOutput->GetAmount(economicIndicator,"05.34 Weaners sold");
   result+=theOutput->GetAmount(economicIndicator,"05.70 Ha premium");
   result-=theOutput->GetAmount(economicIndicator,"06.28 Weaners bought");
   result-=theOutput->GetAmount(economicIndicator,"06.29 Piglets bought");
   result-=theOutput->GetAmount(economicIndicator,"06.30 Baconers bought");
   result-=theOutput->GetAmount(economicIndicator,"06.31 Sows bought");
   // rs now contains the gross income
   result+=theOutput->GetAmount(economicIndicator,"06.10 Plant production misc. costs");
   result+=theOutput->GetAmount(economicIndicator,"06.11 Costs seed");
   result+=theOutput->GetAmount(economicIndicator,"06.12 Costs mineral fertilizer");
   result+=theOutput->GetAmount(economicIndicator,"06.13 Costs chemicals");
   result+=theOutput->GetAmount(economicIndicator,"06.14 Costs fuel");
   result+=theOutput->GetAmount(economicIndicator,"06.15 Costs electricity");
   result+=theOutput->GetAmount(economicIndicator,"06.32 Costs feed and bedding");
   result+=theOutput->GetAmount(economicIndicator,"06.33 Cost of vet and production control");
	theOutput->AddIndicator(economicIndicator,"05.01 Gross margin","Dkr",result);

   result+=theOutput->GetAmount(economicIndicator,"07.02 Depreciation buildings & inventory");
   result+=theOutput->GetAmount(economicIndicator,"07.03 Depreciation machinery");
   result+=theOutput->GetAmount(economicIndicator,"07.04 Insurance buildings & inventory");
   result+=theOutput->GetAmount(economicIndicator,"07.05 Insurance machinery");
   result+=theOutput->GetAmount(economicIndicator,"07.06 Maintenance buildings & inventory");
   result+=theOutput->GetAmount(economicIndicator,"07.07 Maintenance machinery");
	result+=theOutput->GetAmount(economicIndicator,"07.08 Cost of machine contractors");
   theOutput->AddIndicator(economicIndicator,"07.01 Net profit","Dkr",result);


   double capital=0.0;

   capital+=theOutput->GetAmount(economicIndicator,"03.01 Value of arable land");
   capital+=theOutput->GetAmount(economicIndicator,"03.02 Value of buildings & inventory");
   capital+=theOutput->GetAmount(economicIndicator,"03.03 Value of machinery");
   capital+=theOutput->GetAmount(economicIndicator,"03.05 Value of stored products");
   capital+=theOutput->GetAmount(economicIndicator,"03.06 Value of livestock");
   theOutput->AddIndicator(economicIndicator,"03.07 Assets","Dkr",capital);

   // farm savings
   double totalInterest = capital*0.04;
   double workingHours = theOutput->GetAmount(economicIndicator,"13.01 Total labour");
   double wagePrHour = theProducts->GetExpectedBuyPriceYear1("hiredHours");
   theOutput->AddIndicator(economicIndicator,"09.01 Total interest","Dkr",totalInterest);
   theOutput->AddIndicator(economicIndicator,"09.02 Total salery","Dkr",workingHours*wagePrHour);
   theOutput->AddIndicator(economicIndicator,"09.10 Total savings","Dkr",result-totalInterest-workingHours*wagePrHour);
   if (workingHours!=0)
   	theOutput->AddIndicator(economicIndicator,"09.11 Earnings per hour","Dkr",(result-totalInterest)/workingHours);
   if (capital!=0)
   	theOutput->AddIndicator(economicIndicator,"09.12 Return on capital","\%",(result-totalInterest)/capital*100);
}

