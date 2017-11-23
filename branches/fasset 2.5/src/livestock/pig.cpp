#include "../base/common.h"
#include "../products/feedItem.h"
#include "../products/products.h"
#include "pig.h"
#include "../base/base.h"
#include "../base/nixcbits.h"
#include "../base/message.h"
#include "../base/IndicatorOutput.h"

/****************************************************************************\
Constructor with arguments
\****************************************************************************/
pig::pig(const char *aName,const int aIndex,const base * aOwner):
     animal(aName,aIndex,aOwner)
{
   NumberPrDay   = 0.0;
   NumberPrYear  = 0.0;
   variableCosts = 0.0;


   SetAlias(aName);
   feedItemList  = new linkList <product>;
}

/****************************************************************************\
Destructor
\****************************************************************************/
pig::~pig()
{
   delete feedItemList;
}

/****************************************************************************\
Called daily
Will need updating if pigs are outside animal housing (as for cattle)
CHECK TO SEE IF FEED REMOVED TWICE !!!
\****************************************************************************/
void pig::DailyUpdate()
{
   animal::DailyUpdate();
   feedItem *afeedItem;
   if (currentfeed) delete currentfeed;
   currentfeed = new feedItem("feedmix",0,this);
   for (int i=0; i<feedItemList->NumOfNodes(); i++)
   {
      afeedItem = (feedItem*) feedItemList->ElementAtNumber(i);
      feedItem * storedfeedItem =(feedItem *) theProducts->GetCopyStoredFeedProduct(afeedItem);   //returns a copy of the product if it is found in storage
      if ((storedfeedItem)&&(storedfeedItem->GetAmount()>0.0))
      {
         storedfeedItem->Setamount(afeedItem->GetAmount());
         theProducts->SubtractProduct(storedfeedItem);                     // subtract feed from storage
         *currentfeed = *currentfeed + *storedfeedItem;
         delete storedfeedItem;
      }
      else
      {
			if (afeedItem->GetproteinN_digestibility()<0.0)
			{
			   cout << "attempt to access feed with protein digestibility less than zero, code = " << afeedItem->GetCode() << endl;
            theMessage->FatalError("Pig:: ");
			}
         theProducts->SubtractProduct(afeedItem);                     // subtract feed from storage
         *currentfeed = *currentfeed + *afeedItem;
      }
   }
   if (currentfeed->GetAmount()==0)
         theMessage->FatalError("Pig:: No feed provided!");

   double surplusN = ProduceManure(fluidManurePrDay,solidManurePrDay);                                // calculates the amount of manure pr day;
//   while (surplusN<0) //iteration necessary as feed quality could vary with amount requested from store, if several stores contain the desired feed
   if (surplusN<0)
   {
      theMessage->FatalError("Pig:: Insufficient protein in feed for ", GetName());
      //note - this can be caused if the feed characteristics are not defined in feedtable.
/*    surplusN=fabs(surplusN)*1.01;  //add 1% to avoid problems with rounding errors
      GetExtraFeed(surplusN);   //then get more feed
      surplusN = ProduceManure(fluidManurePrDay,solidManurePrDay);                                // calculates the amount of manure pr day;*/
   }
   *manurePrDay = *fluidManurePrDay;
   *manurePrDay = *manurePrDay + *solidManurePrDay;

//	Nbudget.AddInput(manurePrDay->GetAmount()*manurePrDay->GetAllN().n);

   GetanimalSection()->RcvManure(fluidManurePrDay,solidManurePrDay);          // manure to stable
         // fudge - should calculate heat from diet + latent heat loss
//   GetStableSection()->AddanimalHeatOp(NumberPrDay*heatProduction);

   double CinFeed = currentfeed->GetAmount() * currentfeed->GetC_content();
   double CinSoldPig = NumberPrDay*C_growth;
   double CinManure =  manurePrDay->GetAmount()*manurePrDay->GetC_content();
   double CinCH4 = 0.01*GetGrossEnergyinDryMatter() *currentfeed->GetAmount() *currentfeed->GetdryMatter()/55.55;
   theOutput->AddIndicator(4464,"44.64 C in pig CH4 production","kg C",CinCH4*1000.0);
   double CinCO2 = CinFeed - (CinSoldPig + CinManure + CinCH4);
   if (CinCO2<0.0)
         {theMessage->FatalError("pig::DailyUpdate - CO2 production cannot be negative");}
   theOutput->AddIndicator(4463,"44.63 C in pig CO2 production","kg C",CinCO2*1000.0);

/*   Nbudget.AddOutput(manurePrDay->GetAmount()*manurePrDay->GetAllN().n);
   Nbudget.AddOutput(NumberPrDay * N_growth);
   Cbudget.AddOutput(CinManure+CinSoldPig+CinCH4+CinCO2);
   Nbudget.Balance(0.0);
   Cbudget.Balance(0.0);  */

//   cout << GetName() << &Nbudget << endl;
}

/****************************************************************************\
\****************************************************************************/
void pig::ReceivePlan(commonData * data, char * fileName)
{
   currentfeed->Setamount(0.0);

   data->setCritical(false);

   string logString = ("------------ Reading "+(string) this->GetName()+" feeding plan from file "+fileName) ;
   theMessage->LogEvent((char*) logString.c_str());
   cout << "------------ Reading " << (string) this->GetName() << " feeding plan ------------" << " from " << fileName << endl;


   string animalName;
   string namestr;
   namestr=(string) "Group";
   int first,num;
   data->getSectionsNumbers(namestr,&first,&num);  //count the number of periods for which a diet is defined
   int index=first;
   bool gotit=false;
   while ((index<=(first+num))&&(!gotit))
   {
	   data->FindSection((char*) namestr.c_str(),index);  // find the correct place in the input file
      string animalName   = GetAlias().c_str();
      string thisAlias;
      data->FindItem("Alias",&thisAlias);
      if (thisAlias==animalName)      //then this is the animal group we want here
      {
         gotit=true;
         char Indexstr[10];
		   sprintf(Indexstr,"(%d)",index);
   		namestr=(string) "Group"+Indexstr+"."+"Feed";
      }
      else
      {

         index++;
      }
   }

   if (!gotit)
   {
	   cout<<"the alias "<<GetAlias()<<" is not precent in the file"<<endl;
	   theMessage->FatalError("pig::ReceivePlan - cannot find pig group in pigfeed.f??");
   }
   else
   {
   	feedItemList->Reset();
   	data->getSectionsNumbers(namestr,&first,&num);  //count the number of feeds
      for(int index=first;index<=(first+num);index++)
      {
    	  data->FindSection(namestr,index);  // find feed
         int feedCode;
         product* afeedItem;
         product* feedItemClone;
         data->FindItem("FeedCode",&feedCode);
         afeedItem=((product *) theProducts->GetProductElement(feedCode));
         if(afeedItem==nullptr)
         {
        	 convert conv;
        	 conv.IntToString(feedCode);

        	 theMessage->FatalError("pig::ReceivePlan: cannot find feedCode "+ conv.IntToString(feedCode));
         }
         double feedAmount;
         data->FindItem("Amount",&feedAmount);
         if (feedAmount>0.0)
         {
            feedAmount/=1000.0;  // convert from kg per animal per day to tonnes per animal per day
            feedAmount*=NumberPrDay; // convert from tonnes per animal per day to tonnes per day
            feedItemClone = afeedItem->clone();
            feedItemClone->Setamount(feedAmount); //set amount to total daily requirement

//          feedItemClone->Setamount(feedAmount); //set amount to daily requirement per animal
//          feedItemClone->Setamount(feedAmount/(theTime.daysInYear()*NumberPrDay)); //set amount to daily requirement per animal
            feedItemList->InsertLast(feedItemClone);       //load feed items into the feed item list
            *currentfeed  =  (*currentfeed) + (*feedItemClone);
         }
      }
   }

}

/****************************************************************************\
Calculate production of dung and urine, returns N surplus in diet
\****************************************************************************/
double pig::ProduceManure(manure* fluidManure, manure* solidManure)
{
	string animalName =this->GetName();
   double surplus_digestibleN=0.0,Nconc=0.0;
   double totalAmountPrDay;
   if (NumberPrDay>1e-10)
   {
// Fluid manure
   	double amountfeed			= currentfeed->GetAmount();
   	double energyInput		= currentfeed->GetAmount() * currentfeed->GetpigFeedUnitsPerItemUnit();

      if (energyInput<0.75*(NumberPrDay*FE_need)) // BMP changed this january 2007
      {
         if (energyInput<0.50*(NumberPrDay*FE_need))
         {
         	cout << endl;
         	cout << "Energy as percentage of requirement = " << (100.0*energyInput/(NumberPrDay*FE_need)) << endl;
#ifdef NITROSCAPE
            theMessage->Warning("pig::ProduceManure - ",Alias.c_str()," Very low amount of energy in feed");
#else
            theMessage->FatalError("pig::ProduceManure - ",Alias.c_str()," Very low amount of energy in feed");
#endif
         }
         else
            theMessage->Warning("pig::ProduceManure - ",Alias.c_str()," Insufficient energy in feed");
      }

      if (energyInput>1.6*(NumberPrDay*FE_need))
      {
      	double energyPercent = 100.0*energyInput/(NumberPrDay*FE_need);
         theMessage->WarningWithDisplay(Alias.c_str(),energyPercent,"% excess energy in feed ");
      }

      double totalNInput		= amountfeed * currentfeed->GetAllN().n;
	   double protein_N_conc	= currentfeed->Getprotein_N();
	   double proteinNDigested = amountfeed*protein_N_conc*(currentfeed->GetproteinN_digestibility());
	   double ammoniumRatio; 					// CHECK!!!!  value taken from "Kv�lstof i husdyrg�dning", 1990, J.F. Hansen et al.
      double other_N          = amountfeed*(currentfeed->GetAllN().n-protein_N_conc);
	   Nbudget.AddInput(totalNInput);
      double totalCInput		= amountfeed * currentfeed->GetC_content();
	   Cbudget.AddInput(totalCInput);
	   //urine N is the difference between the digested N (protein + other) and the N used for growth
	   surplus_digestibleN     = other_N + proteinNDigested - NumberPrDay * N_growth;
      /*
      cout << Alias << " N in feed per day per individual (kg N) " << (1000*totalNInput/NumberPrDay) << endl;  // Test - remove !!!
      cout << "N growth per day per individual (kg N) " << N_growth*1000.0 << endl;
      cout << "Protein digestibility " << currentfeed->GetproteinN_digestibility() << endl;
      cout << endl;         // Test - remove !!!
      */
      if (surplus_digestibleN>=0)
      {
         //solid manure
         totalAmountPrDay = amountSolid*NumberPrDay/1000.0; //convert to tonnes
         ammoniumRatio=0.0;     //CHECK
         //faecal N is the undigested protein N
         Nconc 	= (amountfeed*protein_N_conc-proteinNDigested)/totalAmountPrDay;
         if (Nconc<0.0)
            theMessage->FatalError("pig::protein N digested greater than protein N present");
         solidManure->Setname("PIG-FAECES");
         theProducts->GiveProductInformation(solidManure);          // sets the default values for solid-manure
//         solidManure->SetfromAnimal(animalName);
         solidManure->Setamount(totalAmountPrDay);
         solidManure->SetorgN_content((1.0-ammoniumRatio)*Nconc);
         solidManure->SetNH4_content(ammoniumRatio*Nconc);
         solidManure->SetNO3_content(0.0);
			double faecalCarbon = amountfeed*currentfeed->GetC_content()* (1-currentfeed->GetOMD());
         solidManure->SetC_content(faecalCarbon/totalAmountPrDay);
         solidManure->SetP_content(amountfeed*(currentfeed->GetP_content())*(1.0-currentfeed->GetP_digest())/totalAmountPrDay);
         solidManure->SetK_content((amountfeed*(currentfeed->GetK_content())-NumberPrYear*K_growth)/totalAmountPrDay);
         solidManure->SetpH(7.6);

      	ammoniumRatio=1.0;     //CHECK !!!
         fluidManure->Setname("PIG-URINE");
         theProducts->GiveProductInformation(fluidManure);
			totalAmountPrDay = amountFluid*NumberPrDay/1000.0; //convert to tonnes
         fluidManure->Setamount(totalAmountPrDay);
 //        fluidManure->SetfromAnimal(animalName);
         Nconc=surplus_digestibleN/totalAmountPrDay;
			double urineCconc = Nconc*60.0/28.0;   //assumes all N in urine is urea (ratio of C to N in urea (2*(NH2) + CO))
         fluidManure->SetorgN_content((1.0-ammoniumRatio)*Nconc);
         fluidManure->SetNH4_content(ammoniumRatio*Nconc);
         fluidManure->SetNO3_content(0.0);
         fluidManure->SetC_content(urineCconc);
         fluidManure->SetP_content((amountfeed*(currentfeed->GetP_content())*(currentfeed->GetP_digest())-NumberPrDay*P_growth)/totalAmountPrDay);
         fluidManure->SetK_content(0.0);                      // rettes senere
         fluidManure->SetpH(8.0);

         double balance = totalNInput - (fluidManure->GetAmount() * fluidManure->GetAllN().n +
                          solidManure->GetAmount() * solidManure->GetAllN().n + NumberPrDay * N_growth);
         if(fabs(balance)>0.0001)
            theMessage->FatalError("pig::ProduceManure - error in N balance in pig manure partitioning");
      }
      else
         theMessage->FatalError("pig::ProduceManure - ",animalName.c_str()," required additional protein in feed"); // BMP modified this !!!
      double Nefficiency =  NumberPrDay * N_growth/totalNInput;
      if (Nefficiency>0.7)
         theMessage->FatalError("pig::ProduceManure - N efficiency over 70%");
   }
   else
   {                                                             // sets the default values for manure
   	fluidManure->Setname("FLUID-MANURE");
     	theProducts->GiveProductInformation(fluidManure);
	   fluidManure->Setamount(0.0);

   	solidManure->Setname("SOLID-MANURE");
      theProducts->GiveProductInformation(solidManure);
	   solidManure->Setamount(0.0);
   }
   return surplus_digestibleN;
}




/****************************************************************************\
\****************************************************************************/
void pig::ReadParameters(commonData * &file)
{
	animal::ReadParameters(file);
	file->setCritical(true);

	 file->FindSection(GetName(),GetIndex());  // find the correct place in the input file
   file->FindItem("LivestockUnits",&LivestockUnits);
   file->FindItem("FE_need",&FE_need);


   file->setCritical(false);

}

/****************************************************************************\
\****************************************************************************/
void pig::EndBudget(bool show)
{
	if (show)
   {
      cout << "Balance for " << GetName() << endl;
      cout << Nbudget;
   }
	double NRemaining=0;
	NRemaining=Nbudget.GetInput()-Nbudget.GetOutput();
	if (NRemaining!=0)
      theMessage->FatalError(GetName(),"pig::EndBudget - N budget error");
}


double pig::CalcAverageHeatProduction(double startWeight, double endWeight)
{
	double interval = (endWeight-startWeight)/10;
	double weight=startWeight;
	double heatProduction=0.0;
	for(int i=0;i<10;i++)
	{
		heatProduction+=CalcHeatProduction(weight);
		weight+=interval;
	}
	return heatProduction;
}
double pig::CalcHeatProduction(double weight)
{
	double heatProduction=14.11*pow(weight,-0.38)*weight;
	return heatProduction;
}
