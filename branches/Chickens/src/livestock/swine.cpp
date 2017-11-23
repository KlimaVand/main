/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
\****************************************************************************/
#include "../base/common.h"
#include "swine.h"
#include "../base/IndicatorOutput.h"
/****************************************************************************\
  Default Constructor
\****************************************************************************/
swine::swine()
   : pig()
{
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
swine::swine(const char * aName, const int aIndex, const base * aOwner)
	: pig(aName, aIndex, aOwner)
{
}

/****************************************************************************\
  Destructor
\****************************************************************************/
swine::~swine()
{
}

/****************************************************************************\
\****************************************************************************/
void swine::DailyUpdate()
{
   pig::DailyUpdate();
   //Note that the logging of sows feed/manure includes both producing sows and new breeding sows
   double amount;																		// the daily indicators
   amount = 1000*manurePrDay->GetAmount();              // unit kg
   theOutput->AddIndicator(economicIndicator,"19.60 Manure from sows","t",amount/1000);
   theOutput->AddIndicator(environmentalIndicator,"34.40 N in sows manure","kg N",amount*manurePrDay->GetAllN().n);
   theOutput->AddIndicator(environmentalIndicator,"44.40 C in sows manure","kg C",amount*manurePrDay->GetC_content());
   theOutput->AddIndicator(environmentalIndicator,"50.10 manure-P from sows","kg P",amount*manurePrDay->GetP_content());
   theOutput->AddIndicator(environmentalIndicator,"60.10 manure-K from sows","kg K",amount*manurePrDay->GetK_content());
   amount =   currentfeed->GetAmount() * 1000.0;
   theOutput->AddIndicator(economicIndicator,"19.10 Feed for sows","t",amount/1000);
   theOutput->AddIndicator(environmentalIndicator,"34.02 N in feed for sows","kg N",amount*currentfeed->GetAllN().n);
   theOutput->AddIndicator(environmentalIndicator,"44.02 C in feed for sows","kg N",amount*currentfeed->GetC_content());
   theOutput->AddIndicator(environmentalIndicator,"50.02 feed-P for sows","kg P",amount*currentfeed->GetP_content());
   theOutput->AddIndicator(environmentalIndicator,"60.02 feed-K for sows","kg K",amount*currentfeed->GetK_content());
}

/****************************************************************************\
Reads the feedplan and store's the total feed mix in totalfeedMix
\****************************************************************************/
void swine::ReceivePlan(char * FileName)
{
	pig::ReceivePlan(FileName);
   vetExpensesPrDay->Setamount(NumberPrYear*variableCosts/theTime.daysInYear(1));
}

/****************************************************************************\
\****************************************************************************/
void swine::ReadParameters(fstream * file)
{
	pig::ReadParameters(file);
   Setfile(file);
   Setcur_pos(0);
   SetCritical();
   FindSection(GetName(),GetIndex());  // find the correct place in the input file
   UnsetCritical();
   Setfile(NULL);
}

/****************************************************************************\
\****************************************************************************/
double swine::GetSensibleHeatProduction(double weight, double n)
{
	double ret_val=0.0;
   int animalType = 0;
   if (GetAlias()=="SOWS_MATING")
   		animalType=1;
   if (GetAlias()=="SOWS_PREGNANT")
   		animalType=2;
   if (GetAlias()=="SOWS_LACTATING")
   		animalType=3;
   switch (animalType)
   {
    case 0:
     {
   	theMessage->FatalError("slghtswn::Unknown animal category.");
      break;
     }
    case 1:
    case 2:
     {
     		ret_val = 4.85* pow(weight,0.75) + 8 * pow(10.0,-5.0)* pow(70.0,3.0) + 76 * 0.18;
			 break;
     }
     case 3:
     		ret_val = 4.85*pow(weight,0.75)+ 28.0 * 6.0;
     	break;
   }
   return ret_val;
}


