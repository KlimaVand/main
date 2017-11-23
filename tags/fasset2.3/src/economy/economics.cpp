/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "economics.h"
#include "../products/products.h"


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


   result=theOutput->GetAmount(economicIndicator,"05.10 Plant products sold");


   result+=theOutput->GetAmount(economicIndicator,"05.10 Plant products sold");
   result+=theOutput->GetAmount(economicIndicator,"05.30 Sows sold");
   result+=theOutput->GetAmount(economicIndicator,"05.31 Baconers sold");
   result+=theOutput->GetAmount(economicIndicator,"05.32 Piglets sold");
   result+=theOutput->GetAmount(economicIndicator,"05.34 Weaners sold");

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

