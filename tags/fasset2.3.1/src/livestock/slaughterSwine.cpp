/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
\****************************************************************************/
#include "../base/common.h"
#include "slaughterSwine.h"

#include "../base/IndicatorOutput.h"
#include "../products/products.h"
/****************************************************************************\
  Default Constructor
\****************************************************************************/
slaughterSwine::slaughterSwine()
   : pig()
{
   slaughterSwinePrDay = new animalProduct;

}

slaughterSwine::slaughterSwine(const char * aName, const int aIndex, const base * aOwner)
	: pig(aName, aIndex, aOwner)
{
   slaughterSwinePrDay = new animalProduct;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
slaughterSwine::~slaughterSwine()
{
	delete slaughterSwinePrDay;
}

/****************************************************************************\
DailyUpdate
\****************************************************************************/
void slaughterSwine::DailyUpdate()
{
   pig::DailyUpdate();

   double manureAmount = 1000*manurePrDay->GetAmount();              // unit kg;																		// the daily indicators
	double feedAmount =   currentfeed->GetAmount() * 1000.0;   //kg
   int animalType = 0;
   if (!strcmp(GetName(),"PIGLETS"))
   		animalType=1;
   if (!strcmp(GetName(),"BACONERS"))
   		animalType=2;
   if (!strcmp(GetName(),"NEWSOWS"))
   		animalType=3;
   switch (animalType)
   {
    case 0:
     {
   	theMessage->FatalError("slghtswn::Unknown animal category.");
      break;
     }
    case 1:
     {
      theOutput->AddIndicator(economicIndicator,"19.12 Feed for piglets","t",feedAmount/1000);
      theOutput->AddIndicator(environmentalIndicator,"34.01 N in feed for piglets","kg N",feedAmount*currentfeed->GetAllN().n);
      theOutput->AddIndicator(environmentalIndicator,"44.01 C in feed for piglets","kg C",feedAmount*currentfeed->GetC_content());
      theOutput->AddIndicator(environmentalIndicator,"50.04 feed-P for piglets","kg P",feedAmount*currentfeed->GetP_content());
      theOutput->AddIndicator(environmentalIndicator,"60.04 feed-K for piglets","kg K",feedAmount*currentfeed->GetK_content());
      theOutput->AddIndicator(economicIndicator,"19.62 Manure from piglets","t",manureAmount/1000);
      theOutput->AddIndicator(environmentalIndicator,"34.42 N in piglets manure","kg N",manureAmount*manurePrDay->GetAllN().n);
      theOutput->AddIndicator(environmentalIndicator,"44.42 C in piglets manure","kg C",manureAmount*manurePrDay->GetC_content());
      theOutput->AddIndicator(environmentalIndicator,"50.12 manure-P from piglets","kg P",manureAmount*manurePrDay->GetP_content());
      theOutput->AddIndicator(environmentalIndicator,"60.12 manure-K from piglets","kg K",manureAmount*manurePrDay->GetK_content());
      break;
     }

    case 2:
     {
      theOutput->AddIndicator(economicIndicator,"19.11 Feed for baconers","t",feedAmount/1000);
      theOutput->AddIndicator(environmentalIndicator,"34.03 N in feed for baconers","kg N",feedAmount*currentfeed->GetAllN().n);
      theOutput->AddIndicator(environmentalIndicator,"44.03 C in feed for baconers","kg C",feedAmount*currentfeed->GetC_content());
      theOutput->AddIndicator(environmentalIndicator,"50.03 feed-P for baconers","kg P",feedAmount*currentfeed->GetP_content());
      theOutput->AddIndicator(environmentalIndicator,"60.03 feed-K for baconers","kg K",feedAmount*currentfeed->GetK_content());
      theOutput->AddIndicator(economicIndicator,"19.61 Manure from baconers","t",manureAmount/1000);
      theOutput->AddIndicator(environmentalIndicator,"34.41 N in baconers manure","kg N",manureAmount*manurePrDay->GetAllN().n);
      theOutput->AddIndicator(environmentalIndicator,"44.41 C in baconers manure","kg C",manureAmount*manurePrDay->GetC_content());
      theOutput->AddIndicator(environmentalIndicator,"50.11 manure-P from baconers","kg P",manureAmount*manurePrDay->GetP_content());
      theOutput->AddIndicator(environmentalIndicator,"60.11 manure-K from baconers","kg K",manureAmount*manurePrDay->GetK_content());
      break;
     }
    case 3:
     {
      theOutput->AddIndicator(economicIndicator,"19.10 Feed for sows","t",feedAmount/1000);
      theOutput->AddIndicator(environmentalIndicator,"34.02 N in feed for sows","kg N",feedAmount*currentfeed->GetAllN().n);
	   theOutput->AddIndicator(environmentalIndicator,"44.02 C in feed for sows","kg N",feedAmount*currentfeed->GetC_content());
      theOutput->AddIndicator(environmentalIndicator,"50.02 feed-P for sows","kg P",feedAmount*currentfeed->GetP_content());
      theOutput->AddIndicator(environmentalIndicator,"60.02 feed-K for sows","kg K",feedAmount*currentfeed->GetK_content());
      theOutput->AddIndicator(economicIndicator,"19.60 Manure from sows","t",manureAmount/1000);
      theOutput->AddIndicator(environmentalIndicator,"34.40 N in sows manure","kg N",manureAmount*manurePrDay->GetAllN().n);
	   theOutput->AddIndicator(environmentalIndicator,"44.40 C in sows manure","kg C",manureAmount*manurePrDay->GetC_content());
      theOutput->AddIndicator(environmentalIndicator,"50.10 manure-P from sows","kg P",manureAmount*manurePrDay->GetP_content());
      theOutput->AddIndicator(environmentalIndicator,"60.10 manure-K from sows","kg K",manureAmount*manurePrDay->GetK_content());
      break;
     }
   }

//	theOutput->AddIndicator(economicIndicator,"03.04 Polts bought","Dkr",-828/theTime.daysInYear()*NumberPrYear);
}


/****************************************************************************\
ReceivePlan.
Reads the feedplan and store's the total feed mix in totalfeedMix
\****************************************************************************/

//void slaughterSwine::ReceivePlan(char * FileName)
void slaughterSwine::ReceivePlan(char * FileName)
{
  	pig::ReceivePlan(FileName);
   vetExpensesPrDay->Setamount(NumberPrYear*variableCosts/theTime.daysInYear(1));
}


/****************************************************************************\
return sensible heat production in W
\****************************************************************************/
double slaughterSwine::GetSensibleHeatProduction(double weight, double n)
{
	double ret_val=0.0;
   int animalType = 0;
   if (!strcmp(GetName(),"PIGLETS"))
   		animalType=1;
   if (!strcmp(GetName(),"BACONERS"))
   		animalType=2;
   if (!strcmp(GetName(),"NEWSOWS"))
   		animalType=3;
   switch (animalType)
   {
    case 0:
     {
   	theMessage->FatalError("slghtswn::Unknown animal category.");
      break;
     }
    case 1:
     {
     		ret_val = 7.4*pow(weight,0.66)+(1 -(0.47 + 0.003*weight))*(n* 7.4* pow(weight,0.66) - 7.4 * pow(weight,0.66));
			 break;
     }
     case 2:
	     	ret_val= 5.09*pow(weight,0.75) + (1 - (0.47 + 0.003*weight))*(n * 5.09 *pow(weight,0.75) - 5.09* pow(weight,0.75));
			break;
     case 3:

     		ret_val = 4.85* pow(weight,0.75)+ 8 * pow(10.0,-5)* pow(70.0,3.0) + 76 * 0.62;
     	break;
   }
   return ret_val;
}


