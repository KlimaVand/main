/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
\****************************************************************************/
#include "../base/common.h"
#include "broilers.h"

#include "../base/IndicatorOutput.h"
#include "../products/products.h"
/****************************************************************************\
  Default Constructor
\****************************************************************************/
broilers::broilers()
   : poultry()
{
   broilersPrDay = new animalProduct;

}

broilers::broilers(const char * aName, const int aIndex, const base * aOwner)
	: poultry(aName, aIndex, aOwner)
{
   broilersPrDay = new animalProduct;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
broilers::~broilers()
{
	delete broilersPrDay;
}

/****************************************************************************\
DailyUpdate
\****************************************************************************/
void broilers::DailyUpdate()
{
   poultry::DailyUpdate();

   double manureAmount = 1000*manurePrDay->GetAmount();              // unit kg;																		// the daily indicators
	double feedAmount =   currentfeed->GetAmount() * 1000.0;   //kg
      theOutput->AddIndicator(economicIndicator,"19.12 Feed for broilers","t",feedAmount/1000);
      theOutput->AddIndicator(environmentalIndicator,"34.01 N in feed for broilers","kg N",feedAmount*currentfeed->GetAllN().n);
      theOutput->AddIndicator(environmentalIndicator,"44.01 C in feed for broilers","kg C",feedAmount*currentfeed->GetC_content());
      theOutput->AddIndicator(environmentalIndicator,"50.04 feed-P for broilers","kg P",feedAmount*currentfeed->GetP_content());
      theOutput->AddIndicator(environmentalIndicator,"60.04 feed-K for broilers","kg K",feedAmount*currentfeed->GetK_content());
      theOutput->AddIndicator(economicIndicator,"19.62 Manure from broilers","t",manureAmount/1000);
      theOutput->AddIndicator(environmentalIndicator,"34.42 N in broilers manure","kg N",manureAmount*manurePrDay->GetAllN().n);
      theOutput->AddIndicator(environmentalIndicator,"44.42 C in broilers manure","kg C",manureAmount*manurePrDay->GetC_content());
      theOutput->AddIndicator(environmentalIndicator,"50.12 manure-P from broilers","kg P",manureAmount*manurePrDay->GetP_content());
      theOutput->AddIndicator(environmentalIndicator,"60.12 manure-K from broilers","kg K",manureAmount*manurePrDay->GetK_content());

//	theOutput->AddIndicator(economicIndicator,"03.04 Polts bought","Dkr",-828/theTime.daysInYear()*NumberPrYear);
}

/****************************************************************************\
feedingDays calculates the number of feeding days. The quality of
the feed determines the number of feeding days.
\****************************************************************************/
double broilers::feedingDays()
{
   double aNumberOfDay=0.0;//GetStableSection()->GetDaysInSection();
   return aNumberOfDay;
}

/****************************************************************************\
ReceivePlan.
Reads the feedplan and store's the total feed mix in totalfeedMix
\****************************************************************************/

//void broilers::ReceivePlan(char * FileName)
void broilers::ReceivePlan(char * FileName)
{
  	poultry::ReceivePlan(FileName);
   vetExpensesPrDay->Setamount(NumberPrYear*variableCosts/theTime.daysInYear(1));
}


/****************************************************************************\
return sensible heat production in W
\****************************************************************************/
double broilers::GetSensibleHeatProduction(double weight, double n)
{
	double ret_val=0.0;
   return ret_val;
}


