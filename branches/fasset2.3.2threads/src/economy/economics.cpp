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


   result=theOutput[id]->GetAmount(510,"05.10 Plant products sold");


   result+=theOutput[id]->GetAmount(510,"05.10 Plant products sold");
   result+=theOutput[id]->GetAmount(530,"05.30 Sows sold");
   result+=theOutput[id]->GetAmount(531,"05.31 Baconers sold");
   result+=theOutput[id]->GetAmount(532,"05.32 Piglets sold");
   result+=theOutput[id]->GetAmount(534,"05.34 Weaners sold");

   result-=theOutput[id]->GetAmount(628,"06.28 Weaners bought");
   result-=theOutput[id]->GetAmount(629,"06.29 Piglets bought");
   result-=theOutput[id]->GetAmount(630,"06.30 Baconers bought");
   result-=theOutput[id]->GetAmount(631,"06.31 Sows bought");
   // rs now contains the gross income
   result+=theOutput[id]->GetAmount(610,"06.10 Plant production misc. costs");
   result+=theOutput[id]->GetAmount(611,"06.11 Costs seed");
   result+=theOutput[id]->GetAmount(612,"06.12 Costs mineral fertilizer");
   result+=theOutput[id]->GetAmount(613,"06.13 Costs chemicals");
   result+=theOutput[id]->GetAmount(614,"06.14 Costs fuel");
   result+=theOutput[id]->GetAmount(615,"06.15 Costs electricity");
   result+=theOutput[id]->GetAmount(632,"06.32 Costs feed and bedding");
   result+=theOutput[id]->GetAmount(633,"06.33 Cost of vet and production control");
	theOutput[id]->AddIndicator(501,"05.01 Gross margin","Dkr",result);

   result+=theOutput[id]->GetAmount(702,"07.02 Depreciation buildings & inventory");
   result+=theOutput[id]->GetAmount(703,"07.03 Depreciation machinery");
   result+=theOutput[id]->GetAmount(704,"07.04 Insurance buildings & inventory");
   result+=theOutput[id]->GetAmount(705,"07.05 Insurance machinery");
   result+=theOutput[id]->GetAmount(706,"07.06 Maintenance buildings & inventory");
   result+=theOutput[id]->GetAmount(707,"07.07 Maintenance machinery");
	result+=theOutput[id]->GetAmount(708,"07.08 Cost of machine contractors");
   theOutput[id]->AddIndicator(701,"07.01 Net profit","Dkr",result);


   double capital=0.0;

   capital+=theOutput[id]->GetAmount(301,"03.01 Value of arable land");
   capital+=theOutput[id]->GetAmount(302,"03.02 Value of buildings & inventory");
   capital+=theOutput[id]->GetAmount(303,"03.03 Value of machinery");
   capital+=theOutput[id]->GetAmount(305,"03.05 Value of stored products");
   capital+=theOutput[id]->GetAmount(306,"03.06 Value of livestock");
   theOutput[id]->AddIndicator(307,"03.07 Assets","Dkr",capital);

   // farm savings
   double totalInterest = capital*0.04;
   double workingHours = theOutput[id]->GetAmount(1301,"13.01 Total labour");
   double wagePrHour = theProducts[id]->GetExpectedBuyPriceYear1("hiredHours");
   theOutput[id]->AddIndicator(901,"09.01 Total interest","Dkr",totalInterest);
   theOutput[id]->AddIndicator(902,"09.02 Total salery","Dkr",workingHours*wagePrHour);
   theOutput[id]->AddIndicator(910,"09.10 Total savings","Dkr",result-totalInterest-workingHours*wagePrHour);
   if (workingHours!=0)
   	theOutput[id]->AddIndicator(911,"09.11 Earnings per hour","Dkr",(result-totalInterest)/workingHours);
   if (capital!=0)
   	theOutput[id]->AddIndicator(912,"09.12 Return on capital","\%",(result-totalInterest)/capital*100);
}

