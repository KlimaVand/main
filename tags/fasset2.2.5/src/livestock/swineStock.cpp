/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "swineStock.h"
#include "../base/IndicatorOutput.h"
#include "../products/products.h"
#include "../building/thebuild.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
swineStock::swineStock(const char * aName, const int aIndex, const base * aOwner)
			:animalStock(aName,aIndex,aOwner)
{
	matingSwines = new swine("SOWS",aIndex,this);
   matingSwines->SetthisProduct((animalProduct*) theProducts->GetProductElement("SOWS"));
	lactatingSwines = new swine("SOWS",aIndex,this);
   lactatingSwines->SetthisProduct((animalProduct*) theProducts->GetProductElement("SOWS"));
	pregnantSwines = new swine("SOWS",aIndex,this);
   pregnantSwines->SetthisProduct((animalProduct*) theProducts->GetProductElement("SOWS"));
   piglets= new slaughterSwine("PIGLETS",aIndex,this);
   piglets->SetthisProduct((animalProduct*) theProducts->GetProductElement("PIGLETS"));
   baconers = new slaughterSwine("BACONERS",aIndex,this);
   baconers->SetthisProduct((animalProduct*) theProducts->GetProductElement("BACONERS"));
   newSows = new slaughterSwine("NEWSOWS",aIndex,this);
   newSows->SetthisProduct((animalProduct*) theProducts->GetProductElement("NEWSOWS"));
   animalProduct *temp = new animalProduct(*(animalProduct*) theProducts->GetProductElement("SUCKLINGPIG"));
   if (temp->GetEndWeight()!=piglets->GetthisProduct()->GetStartWeight())
		theMessage->FatalError("swineStock::piglet start weight differs from weaner end weight");
   delete temp;
   if (piglets->GetthisProduct()->GetEndWeight()!=baconers->GetthisProduct()->GetStartWeight())
		theMessage->FatalError("swineStock::baconer start weight differs from piglet end weight");
   if (baconers->GetthisProduct()->GetEndWeight()!=newSows->GetthisProduct()->GetStartWeight())
		theMessage->FatalError("swineStock::new sow start weight differs from baconer end weight");
   pointyPigs = new linkList<pig>;
   pointyPigs->InsertLast(matingSwines);
	pointyPigs->InsertLast(lactatingSwines);
	pointyPigs->InsertLast(pregnantSwines);
	pointyPigs->InsertLast(piglets);
	pointyPigs->InsertLast(baconers);
	pointyPigs->InsertLast(newSows);

   weanersInOutPrDay = new animalProduct();
   pigletsInOutPrDay = new animalProduct();
   pigletsPrDay = new animalProduct();
   baconersPrDay  = new animalProduct();
   baconersInOutPrDay = new animalProduct();
   newsowsPrDay = new animalProduct();
   newsowsInOutPrDay = new animalProduct();
   castsowsPrDay = new animalProduct();
}

/****************************************************************************\
  Destructor
\****************************************************************************/
swineStock::~swineStock()
{
   delete matingSwines;
   delete lactatingSwines;
   delete pregnantSwines;
   delete piglets;
   delete baconers;
   delete newSows;
   delete weanersInOutPrDay;
   delete pigletsPrDay;
   delete baconersPrDay;
   delete newsowsPrDay;
   delete castsowsPrDay;
	delete pigletsInOutPrDay;
	delete baconersInOutPrDay;
	delete newsowsInOutPrDay;
   pointyPigs->ForgetAll();
   delete pointyPigs;
}

/****************************************************************************\
	ReceivePlan read the plan for all animals
\****************************************************************************/
void swineStock::ReceivePlan(char* fileExtension)
//Calculates number of animals to be imported + herd structure
{
   char fileName[13];
   //animalProduct * anAnimalProduct;
   double urinePerYear,faecesPerYear;

   strcpy(fileName,"Pigs.");
   strcat(fileName,fileExtension);
   UnsetCritical();
//   if(!OpenInputFile(fileName))
   if (FileExists(fileName))
	   OpenInputFile(fileName);
   else
   {
   	strcpy(fileName,"pigs.fnn");
   	if (FileExists(fileName))
      OpenInputFile(fileName);
   	else
		theMessage->FatalError("swineStock::ReceivePlan - error in opening pigs.f??");
   }

	//double aNumber;
// Get number of animals produced per year
   double numSows=0, numBaconers=0, numPiglets=0, numNewSows=0;
   SetCritical();
   Setcur_pos(0);
/*   string inputString;
   getline(*file,inputString,'\n');
   cout << inputString << endl;*/
   FindSection(GetName(),GetIndex());

   GetParameter("SOWS",&numSows);						// number of sows present per day
   GetParameter("NEWSOWS",&numNewSows);           // production of new sows for breeding per year
   GetParameter("BACONERS",&numBaconers);          //production of baconers per year
   GetParameter("PIGLETS",&numPiglets);            //production of piglets per year
   UnsetCritical();
   CloseInputFile();

 	double daysInYear = (double)theTime.daysInYear(1);

// Get production cycle details from building
  	double daysPregnant = ((pigstysection*) pregnantSwines->GetStableSection())->GetDaysInSection();
   double daysMating = ((pigstysection*) matingSwines->GetStableSection())->GetDaysInSection();
   double dayslactating =((pigstysection*) lactatingSwines->GetStableSection())->GetDaysInSection();
  	SowProductionCycle =  daysPregnant + daysMating + dayslactating;
   double pigletProductionCycle = ((pigstysection*) piglets->GetStableSection())->GetDaysInSection();
   double baconersProductionCycle = ((pigstysection*) baconers->GetStableSection())->GetDaysInSection();
   double newsowsProductionCycle = ((pigstysection*) newSows->GetStableSection())->GetDaysInSection();

   Nbudget.Reset();   //reset budget or it will accumulate N between years and CheckBalance will throw an error

// Set up objects to store information on purchase and sale of animals
   *weanersInOutPrDay = *theProducts->GetProductElement("SUCKLINGPIG");
   weanersInOutPrDay->Setamount(0.0);
   *pigletsInOutPrDay = *piglets->GetthisProduct(); //*theProducts->GetProductElement("PIGLETS");
   pigletsInOutPrDay->Setamount(0.0);
   *baconersInOutPrDay = *baconers->GetthisProduct();
   baconersInOutPrDay->Setamount(0.0);
   *newsowsInOutPrDay = *newSows->GetthisProduct();;
   newsowsInOutPrDay->Setamount(0.0);

   piglets->SetaveWeight((pigletsInOutPrDay->GetEndWeight()+weanersInOutPrDay->GetEndWeight())/2);
   baconers->SetaveWeight((weanersInOutPrDay->GetEndWeight()+baconersInOutPrDay->GetEndWeight())/2);
   newSows->SetaveWeight((baconersInOutPrDay->GetEndWeight()+newsowsInOutPrDay->GetEndWeight())/2);
   pregnantSwines->SetaveWeight(newsowsInOutPrDay->GetEndWeight());
   lactatingSwines->SetaveWeight(newsowsInOutPrDay->GetEndWeight());
   matingSwines->SetaveWeight(newsowsInOutPrDay->GetEndWeight());

   double weaners= numSows * pregnantSwines->GetpigletsPerLitter() * daysInYear/SowProductionCycle;
   double animalsChange;
   //see if sows produce too many piglets for production and buy deficit/sell surplus
  	animalsChange=numPiglets-weaners;	//positive = import animals, negative = export animals
   if(animalsChange !=0.0)  //import or export of weaners
      weanersInOutPrDay->Setamount(animalsChange * weanersInOutPrDay->GetEndWeight()/(daysInYear * 1000.0));
  	animalsChange=numBaconers-numPiglets;     //note - calculate number of PIGLETS that must be bought or sold to get correct number of BACONERS
   if(animalsChange !=0.0)  //import or export of piglets
      pigletsInOutPrDay->Setamount(animalsChange * pigletsInOutPrDay->GetEndWeight()/(daysInYear * 1000.0));
  	animalsChange=numNewSows-numBaconers;
   if(animalsChange !=0.0) //import or export of baconers
      baconersInOutPrDay->Setamount(animalsChange* baconersInOutPrDay->GetEndWeight()/(daysInYear * 1000.0));
   newsowsInOutPrDay->Setamount(-numNewSows * newsowsInOutPrDay->GetEndWeight()/(daysInYear * 1000.0));  //only export newsows

	// divide sows up between stages of reproduction
	pregnantSwines->SetNumberPrDay(numSows * daysPregnant/SowProductionCycle);
   lactatingSwines->SetNumberPrDay(numSows * dayslactating/SowProductionCycle);
	matingSwines->SetNumberPrDay(numSows * daysMating/SowProductionCycle);

   // number present per year is same as number present per day
   pregnantSwines->SetNumberPrYear(pregnantSwines->GetNumberPrDay());
   lactatingSwines->SetNumberPrYear(lactatingSwines->GetNumberPrDay());
   matingSwines->SetNumberPrYear(matingSwines->GetNumberPrDay());

// determine how many of each animal type is present on a daily basis
   piglets->SetNumberPrYear(numPiglets);
   piglets->SetNumberPrDay(numPiglets* pigletProductionCycle/daysInYear);
   baconers->SetNumberPrYear(numBaconers);
   baconers->SetNumberPrDay(numBaconers* baconersProductionCycle/daysInYear);
   newSows->SetNumberPrYear(numNewSows);
   newSows->SetNumberPrDay(numNewSows * newsowsProductionCycle/daysInYear);


   *pigletsPrDay = *piglets->GetthisProduct(); //*theProducts->GetProductElement("PIGLETS");
   pigletsPrDay->Setamount(numPiglets * pigletsPrDay->GetEndWeight()/daysInYear);
   *baconersPrDay = *baconers->GetthisProduct();//*theProducts->GetProductElement("BACONERS");
   baconersPrDay->Setamount(numBaconers * baconersPrDay->GetEndWeight()/daysInYear);
   *newsowsPrDay = *newSows->GetthisProduct();//*theProducts->GetProductElement("NEWSOWS");
   newsowsPrDay->Setamount(numNewSows * newsowsPrDay->GetEndWeight()/daysInYear);
   *castsowsPrDay = *matingSwines->GetthisProduct();//*theProducts->GetProductElement("SOWS");
   castsowsPrDay->Setamount(numSows * castsowsPrDay->GetEndWeight()/daysInYear);

   ReceiveFeed(fileExtension);

   // This next loop sets up the manure handling in animal housing. Gives an error in housing if it is removed!
	double heatOp=0.0;
   double energyIn=0.0;
   double maintEnergy=0.0;
   for(int i=0;i<pointyPigs->NumOfNodes();i++)
	{
      pig *aPig=pointyPigs->ElementAtNumber(i);
      if (aPig->GetNumberPrDay()>0.0)
      {
         faecesPerYear = aPig->GetNumberPrDay()*aPig->GetamountSolid()*365/1000.0;
         urinePerYear = aPig->GetNumberPrDay()*aPig->GetamountFluid()*365/1000.0;
         energyIn=aPig->Getcurrentfeed()->GetAmount()*aPig->Getcurrentfeed()->GetME()*1000000.0/(aPig->GetNumberPrDay()*24.0 * 3600.0);
         maintEnergy=aPig->CalcMaintenanceEnergy(aPig->GetaveWeight());
         double n =  energyIn/maintEnergy;
         heatOp=aPig->GetNumberPrDay()* aPig->GetSensibleHeatProduction(aPig->GetaveWeight(),n);
         aPig->GetStableSection()->SetAnimalParameters(aPig->GetNumberPrDay(),faecesPerYear,urinePerYear,heatOp);
      }
   }
}

/****************************************************************************\
	Get feed input and calculate the N needed each day for growth
   Only for animal model 1
\****************************************************************************/
void swineStock::ReceiveFeed(char* fileExtension)
//Load feed and calculate growth N requirements
{
   char fileName[13];
   double numPiglets =piglets->GetNumberPrYear();
   double numBaconers = baconers->GetNumberPrYear();
   double numNewSows = newSows->GetNumberPrYear();
   double numSows = pregnantSwines->GetNumberPrYear() + matingSwines->GetNumberPrYear() +
      		lactatingSwines->GetNumberPrYear();

   strcpy(fileName,"PIGSFEED.");
   if (fileExtension[0]=='F')                        // modification due to bad GAMS planning
   {
      int yearNumber = 0;
      sscanf(fileExtension,"F%02d",&yearNumber);
      yearNumber = max(0,yearNumber);
      fileExtension[0] = '\0';
      sprintf(fileExtension,"F%02d",yearNumber);
   }
   strcat(fileName,fileExtension);

   double startNContent, startWeight, endNContent, endWeight, NGrowth, startCContent, endCContent, CGrowth;
   if (numSows>0)
   {
      pregnantSwines->ReceivePlan(fileName);
      lactatingSwines->ReceivePlan(fileName);
      matingSwines->ReceivePlan(fileName);

// Calculate N required for growth - note amount is in tonnes
// Calculate the N partitioned to growth based on the start weight and N content and the end weight and N content
      startNContent = ((animalProduct*) theProducts->GetProductElement("NEWSOWS"))->GetN_content();
      startWeight =   ((animalProduct*) theProducts->GetProductElement("NEWSOWS"))->GetEndWeight();
      endNContent = ((animalProduct*) theProducts->GetProductElement("SOWS"))->GetN_content();
      endWeight =   ((animalProduct*) theProducts->GetProductElement("SOWS"))->GetEndWeight();
      //Note that sow endweight is average start weight + average weight gain per parity
      NGrowth = (endWeight* endNContent - startWeight * startNContent)/(SowProductionCycle * 1000.0);
      matingSwines->SetN_growth(NGrowth);
      startCContent = ((animalProduct*) theProducts->GetProductElement("NEWSOWS"))->GetC_content();
      endCContent = ((animalProduct*) theProducts->GetProductElement("SOWS"))->GetC_content();
      CGrowth = (endWeight * endCContent - startWeight * startCContent)/(SowProductionCycle * 1000.0);
      matingSwines->SetC_growth(CGrowth);

// allow for growth of foetus and suckling piglets. The growth is here averaged over pregnancy and lactation
      double NContent = ((animalProduct*) theProducts->GetProductElement("SUCKLINGPIG"))->GetN_content();   //suckler pig final N content
      double Weight =   ((animalProduct*) theProducts->GetProductElement("SUCKLINGPIG"))->GetEndWeight();   //suckler pig final weight
      double totalSucklerPigletN = NContent * Weight * pregnantSwines->GetpigletsPerLitter();  //total litter N per sow per pregnancy
      double sucklerDays = (((pigstysection*) lactatingSwines->GetStableSection())->GetDaysInSection() +    //days taken for suckler to reach end of suckling
                   ((pigstysection*) pregnantSwines->GetStableSection())->GetDaysInSection());
      NGrowth += totalSucklerPigletN/(sucklerDays * 1000);    // add foetus and suckler growth demand to sow growth demand
      endCContent = ((animalProduct*) theProducts->GetProductElement("SUCKLINGPIG"))->GetC_content();
      double totalSucklerPigletC = endCContent * Weight * pregnantSwines->GetpigletsPerLitter();  //total litter N per sow per pregnancy
      CGrowth += totalSucklerPigletC/(sucklerDays * 1000);
      lactatingSwines->SetN_growth(NGrowth);
      pregnantSwines->SetN_growth(NGrowth);
      lactatingSwines->SetC_growth(CGrowth);
      pregnantSwines->SetC_growth(CGrowth);
   }
   else
   {
      pregnantSwines->Getcurrentfeed()->Setamount(0.0);
      lactatingSwines->Getcurrentfeed()->Setamount(0.0);
      matingSwines->Getcurrentfeed()->Setamount(0.0);
   }
// new sows
   if (numNewSows>0)
   {
      newSows->ReceivePlan(fileName);
      //gets starting values from previous (younger) animal category
	   double newsowsProductionCycle = ((pigstysection*) newSows->GetStableSection())->GetDaysInSection();
      startNContent = baconers->GetthisProduct()->GetN_content();
      startWeight =   baconers->GetthisProduct()->GetEndWeight();
      endNContent = newSows->GetthisProduct()->GetN_content();
      endWeight =   newSows->GetthisProduct()->GetEndWeight();
      NGrowth = (endWeight* endNContent - startWeight * startNContent)/(newsowsProductionCycle * 1000);
      newSows->SetN_growth(NGrowth);
      startCContent = baconers->GetthisProduct()->GetC_content();
      endCContent = newSows->GetthisProduct()->GetC_content();
      CGrowth = (endWeight * endCContent - startWeight * startCContent)/(newsowsProductionCycle * 1000);
      newSows->SetC_growth(CGrowth);
   }
   else
      newSows->Getcurrentfeed()->Setamount(0.0);
// piglets
   if (numPiglets>0)
   {
      piglets->ReceivePlan(fileName);
      //gets starting values from previous (younger) animal category - here no instance exists so get from Products.dat
	   double pigletProductionCycle = ((pigstysection*) piglets->GetStableSection())->GetDaysInSection();
      startNContent = ((animalProduct*) theProducts->GetProductElement("SUCKLINGPIG"))->GetN_content();
      startWeight =   piglets->GetthisProduct()->GetStartWeight();
      endNContent = piglets->GetthisProduct()->GetN_content();
      endWeight =   piglets->GetthisProduct()->GetEndWeight();
      NGrowth = (endWeight* endNContent - startWeight * startNContent)/(pigletProductionCycle * 1000);
      piglets->SetN_growth(NGrowth);
      startCContent = ((animalProduct*) theProducts->GetProductElement("SUCKLINGPIG"))->GetC_content();
      endCContent = piglets->GetthisProduct()->GetC_content();
      CGrowth = (endWeight * endCContent - startWeight * startCContent)/(pigletProductionCycle * 1000);
      piglets->SetC_growth(CGrowth);
   }
   else
      piglets->Getcurrentfeed()->Setamount(0.0);
// baconers
   if (numBaconers>0)
   {
      baconers->ReceivePlan(fileName);
	   double baconersProductionCycle = ((pigstysection*) baconers->GetStableSection())->GetDaysInSection();
      startNContent = piglets->GetthisProduct()->GetN_content();
      startWeight =   piglets->GetthisProduct()->GetEndWeight();
      endNContent = baconers->GetthisProduct()->GetN_content();
      endWeight =   baconers->GetthisProduct()->GetEndWeight();
      NGrowth = (endWeight* endNContent - startWeight * startNContent)/(baconersProductionCycle * 1000);
      baconers->SetN_growth(NGrowth);
      startCContent = piglets->GetthisProduct()->GetC_content();
      endCContent = baconers->GetthisProduct()->GetC_content();
      CGrowth = (endWeight * endCContent - startWeight * startCContent)/(baconersProductionCycle * 1000);
      baconers->SetC_growth(CGrowth);
   }
   else
      baconers->Getcurrentfeed()->Setamount(0.0);
// make sure that all the growth is exported
   CheckNbalance();

// load the product info
//THIS WILL CURRENTY RETURN INCORRECT VALUES BECAUSE AMOUNTS ARE IN TONNES NOT NUMBER
   if (strcmp(fileExtension,"lp3")!=0)
   {
      double livestockPrice = 0.0;
      double sell = theProducts->GetActualSellPrice("SUCKLINGPIG");
      double buy  = theProducts->GetActualBuyPrice("SUCKLINGPIG");
      livestockPrice += ((sell+buy)/2)*weanersInOutPrDay->GetAmount();
      sell = theProducts->GetActualSellPrice("PIGLETS");
      buy  = theProducts->GetActualBuyPrice("PIGLETS");
      livestockPrice += ((sell+buy)/2)*pigletsInOutPrDay->GetAmount();
      sell = theProducts->GetActualSellPrice("BACONERS");
      buy  = theProducts->GetActualBuyPrice("BACONERS");
      livestockPrice += ((sell+buy)/2)*baconersInOutPrDay->GetAmount();
      sell = theProducts->GetActualSellPrice("NEWSOWS");
      buy  = theProducts->GetActualBuyPrice("NEWSOWS");
      livestockPrice += ((sell+buy)/2)*newsowsInOutPrDay->GetAmount();
      sell = theProducts->GetActualSellPrice("SOWS");
      buy  = theProducts->GetActualBuyPrice("SOWS");
      livestockPrice += (sell+buy)/2*(numNewSows/theTime.daysInYear(1));
      theOutput->AddIndicator(economicIndicator,"03.06 Value of livestock","Dkr",livestockPrice);
   }
}

/****************************************************************************\
	DailyUpdate is the daily execution of swines
\****************************************************************************/
void swineStock::DailyUpdate(void)
{
   double NumberPrDay = matingSwines->GetNumberPrDay() + lactatingSwines->GetNumberPrDay() +
      pregnantSwines->GetNumberPrDay();
   theOutput->AddStateIndicator(economicIndicator,"11.30 Sows present","no",NumberPrDay);
   NumberPrDay = baconers->GetNumberPrDay();
   theOutput->AddStateIndicator(economicIndicator,"11.31 Baconers present","no",NumberPrDay);
   NumberPrDay = piglets->GetNumberPrDay();
   theOutput->AddStateIndicator(economicIndicator,"11.32 Piglets present","no",NumberPrDay);
   NumberPrDay = newSows->GetNumberPrDay();
   theOutput->AddStateIndicator(economicIndicator,"11.33 New sows present","no",NumberPrDay);

	for(int i=0;i<pointyPigs->NumOfNodes();i++)
	{
    if ((pointyPigs->ElementAtNumber(i))->GetNumberPrDay()>0)
    {
       pig * tempPig = pointyPigs->ElementAtNumber(i);
       tempPig->DailyUpdate();
    }
   }
   ExportStock();
	theOutput->AddStateIndicator(environmentalIndicator,"34.60 N bound in pigs","kg N",0.0);
}

/****************************************************************************\
	Initialize's a new swinestock
   BMP changed the name (hide of virtual function) - are there better options ???!!!
\****************************************************************************/
void swineStock::InitSwines(fstream * file, buildings* theBuildings)
{
	for(int i=0;i<pointyPigs->NumOfNodes();i++)
    pointyPigs->ElementAtNumber(i)->ReadParameters(file);
	matingSwines->SetAlias("SOWS_MATING");
	pregnantSwines->SetAlias("SOWS_PREGNANT");
	lactatingSwines->SetAlias("SOWS_LACTATING");

   //char buffer [3];
   // Warning - itoa is not ANSI standard
//   itoa(GetIndex(), buffer, 10);
	char Indexstr[10];
   sprintf(Indexstr,"(%d)",Index);

   string outstring;
//Find animal housing for pigs
   outstring = (string) "Mating" + Indexstr;
	matingSwines->SetStableSection(theBuildings->PlaceAnimal(outstring.c_str()));
   outstring = (string) "Pregnant" + Indexstr;
	pregnantSwines->SetStableSection(theBuildings->PlaceAnimal(outstring.c_str()));
   outstring = (string) "Lactating" + Indexstr;
	lactatingSwines->SetStableSection(theBuildings->PlaceAnimal(outstring.c_str()));
   outstring = (string) "Weaner" + Indexstr;
	piglets->SetStableSection(theBuildings->PlaceAnimal(outstring.c_str()));
   outstring = (string) "Finisher" + Indexstr;
	baconers->SetStableSection(theBuildings->PlaceAnimal(outstring.c_str()));
   outstring = (string) "NewSow" + Indexstr;
   newSows->SetStableSection(theBuildings->PlaceAnimal(outstring.c_str()));
}

/****************************************************************************\
	GetNumberPrDay return the total number of swines and slaughter swines
\****************************************************************************/
double swineStock::GetNumber()
{
 double ret_val=0;
 for(int i=0;i<pointyPigs->NumOfNodes();i++)
	{
	 ret_val+=pointyPigs->ElementAtNumber(i)->GetNumberPrDay();
   }
	return ret_val;
}


void swineStock::CheckNbalance()
{
//Checks to make sure that all N accumulated is also sold
 double totalGrowthN=0;
 double totalExportN=0.0;//Nbudget.GetOutput()*1000.0;;
 animalProduct * aProduct = GetStockBought();
 double totalImportN=aProduct->GetAmount() * aProduct->GetN_content() * theTime.daysInYear(1) * 1000.0; //Nbudget.GetInput()*1000.0;
 delete aProduct;
//growth first
 for(int i=0;i<pointyPigs->NumOfNodes();i++)
	{
    pig * tempPig = pointyPigs->ElementAtNumber(i);
    if (tempPig->GetNumberPrDay()>0.0)
		 totalGrowthN+= tempPig->GetNumberPrDay() * tempPig->GetN_growth() * theTime.daysInYear(1);
   }
// convert from tonnes to kg
 totalGrowthN*=1000.0;
//export
 aProduct = GetStockSold();
 totalExportN=aProduct->GetAmount() * aProduct->GetN_content() * theTime.daysInYear(1) * 1000.0;    //in kg
 delete aProduct;
 double balance=totalExportN-(totalGrowthN+totalImportN);
 if (fabs(balance)>0.001)
 	theMessage->FatalError("Swinestock: N budget error - exported N and growth N are unequal");
 manure* fluidManure = new manure();
 manure* solidManure = new manure();
 for(int i=0;i<pointyPigs->NumOfNodes();i++)
 {
    double manureN=0;
    double feedN=0;
    double growthN=0;
    pig * tempPig = pointyPigs->ElementAtNumber(i);
    if (tempPig->GetNumberPrDay()>0.0)
    {
       feedItem *currentfeed;
       currentfeed=tempPig->Getcurrentfeed();
       feedN = currentfeed->GetAmount() * currentfeed->GetAllN().n * theTime.daysInYear(1) * 1000.0;
       growthN = tempPig->GetNumberPrDay() * tempPig-> GetN_growth() * theTime.daysInYear(1) * 1000.0;
       tempPig->ProduceManure( fluidManure, solidManure);
       manureN = theTime.daysInYear(1) * (fluidManure->GetAmount() * fluidManure->GetAllN().n +
                           solidManure->GetAmount() * solidManure->GetAllN().n) * 1000.0;
       balance = feedN-(growthN + manureN);
       if (fabs(balance)>1.0)
          theMessage->FatalError("Swinestock: ", tempPig->GetName()," error - N in feed > (growth N + manure N)");
    }
 }
 delete fluidManure; delete solidManure;
}

void swineStock::ExportStock()
// Sell a few animals per day
// Selling cast sows means that at the end of the year, all sows are sold
{
/*   if (castsowsPrDay->GetAmount()>0.0)  //buy and sell sows
   {
	   theProducts->SubtractProduct(castsowsPrDay);
	   theProducts->AddProduct(castsowsPrDay);
   }
  */
   if (weanersInOutPrDay->GetAmount()>0.0)  //buy animals
	   theProducts->SubtractProduct(weanersInOutPrDay);
   if (weanersInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	weanersInOutPrDay->Setamount(weanersInOutPrDay->GetAmount()*-1.0);
	   theProducts->AddProduct(weanersInOutPrDay);
   	weanersInOutPrDay->Setamount(weanersInOutPrDay->GetAmount()*-1.0);
   }

   if (pigletsInOutPrDay->GetAmount()>0.0)  //buy animals
	   theProducts->SubtractProduct(pigletsInOutPrDay);
   if (pigletsInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	pigletsInOutPrDay->Setamount(pigletsInOutPrDay->GetAmount()*-1.0);
	   theProducts->AddProduct(pigletsInOutPrDay);
   	pigletsInOutPrDay->Setamount(pigletsInOutPrDay->GetAmount()*-1.0);
   }

   if (baconersInOutPrDay->GetAmount()>0.0)  //buy animals
	   theProducts->SubtractProduct(baconersInOutPrDay);
   if (baconersInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	baconersInOutPrDay->Setamount(baconersInOutPrDay->GetAmount()*-1.0);
	   theProducts->AddProduct(baconersInOutPrDay);
   	baconersInOutPrDay->Setamount(baconersInOutPrDay->GetAmount()*-1.0);
   }

   if (newsowsInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	newsowsInOutPrDay->Setamount(newsowsInOutPrDay->GetAmount()*-1.0);
	   theProducts->AddProduct(newsowsInOutPrDay);
   	newsowsInOutPrDay->Setamount(newsowsInOutPrDay->GetAmount()*-1.0);
   }
//   double Nout=0.0;
}

void swineStock::EndBudget(bool show)
{
 for(int i=0;i<pointyPigs->NumOfNodes();i++)
	{
	  pointyPigs->ElementAtNumber(i)->EndBudget(show);
   }
}

/****************************************************
Returns a product that contains the total N and C purchased in pigs per day
****************************************************/
animalProduct *swineStock::GetStockBought()
{
	animalProduct * aProduct = new animalProduct();
   if (weanersInOutPrDay->GetAmount()>0.0)  //buy animals
	   *aProduct + *weanersInOutPrDay;
   if (pigletsInOutPrDay->GetAmount()>0.0)  //buy animals
	   *aProduct + *pigletsInOutPrDay;
   if (baconersInOutPrDay->GetAmount()>0.0)  //buy animals
	   *aProduct + *baconersInOutPrDay;
	return aProduct;
}

/****************************************************************************\
Returns a product that contains the total N and C sold  in pigs per day
\****************************************************************************/
animalProduct * swineStock::GetStockSold()
{
	animalProduct * aProduct = new animalProduct();
//piglets, baconers and new sows sold

   if (weanersInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	weanersInOutPrDay->Setamount(weanersInOutPrDay->GetAmount()*-1.0);
	   *aProduct + *weanersInOutPrDay;
   	weanersInOutPrDay->Setamount(weanersInOutPrDay->GetAmount()*-1.0);
   }

   if (pigletsInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	pigletsInOutPrDay->Setamount(pigletsInOutPrDay->GetAmount()*-1.0);
	   *aProduct + *pigletsInOutPrDay;
   	pigletsInOutPrDay->Setamount(pigletsInOutPrDay->GetAmount()*-1.0);
   }

   if (baconersInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	baconersInOutPrDay->Setamount(baconersInOutPrDay->GetAmount()*-1.0);
	   *aProduct + *baconersInOutPrDay;
   	baconersInOutPrDay->Setamount(baconersInOutPrDay->GetAmount()*-1.0);
   }

   if (newsowsInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	newsowsInOutPrDay->Setamount(newsowsInOutPrDay->GetAmount()*-1.0);
	   *aProduct + *newsowsInOutPrDay;
   	newsowsInOutPrDay->Setamount(newsowsInOutPrDay->GetAmount()*-1.0);
   }
	return aProduct;
}

