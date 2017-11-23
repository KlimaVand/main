/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "poultryStock.h"
#include "../base/IndicatorOutput.h"
#include "../products/products.h"
#include "../building/thebuild.h"
#include "hens.h"
#include "broilers.h"
#include "pullets.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
poultryStock::poultryStock(const char * aName, const int aIndex, const base * aOwner)
			:animalStock(aName,aIndex,aOwner)
{
	theHens = new hens("HENS",aIndex,this);
	theHens->SetthisProduct((animalProduct*) theProducts->GetProductElement("HENS"));
	thePullets= new pullets("PULLETS",aIndex,this);
	thePullets->SetthisProduct((animalProduct*) theProducts->GetProductElement("PULLETS"));
	theBroilers= new broilers("BROILERS",aIndex,this);
	theBroilers->SetthisProduct((animalProduct*) theProducts->GetProductElement("BROILERS"));
	/*   animalProduct *temp = new animalProduct(*(animalProduct*) theProducts->GetProductElement("PULLETS"));
	if (temp->GetEndWeight()!=piglets->GetthisProduct()->GetStartWeight())
		theMessage->FatalError("poultryStock::piglet start weight differs from weaner end weight");
	delete temp; */
	pointyPoultry = new linkList<poultry>;
	pointyPoultry->InsertLast(theHens);
	pointyPoultry->InsertLast(thePullets);
	pointyPoultry->InsertLast(theBroilers);

	pulletsInOutPrDay = new animalProduct();
	hensInOutPrDay = new animalProduct();
	broilersInOutPrDay = new animalProduct();
	broilersPrDay = new animalProduct();
	pulletsPrDay  = new animalProduct();
	hensPrDay = new animalProduct();
	PoultryProductionCycle=0.0;    //days between successive births
	henProductionCycle=0.0;
   pulletProductionCycle=0.0;
	broilersProductionCycle=0.0;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
poultryStock::~poultryStock()
{
   delete theHens;
   delete thePullets;
   delete theBroilers;
   delete pulletsInOutPrDay;
   delete hensInOutPrDay;
   delete broilersInOutPrDay;
   delete broilersPrDay;
   delete pulletsPrDay;
   delete hensPrDay;
   pointyPoultry->ForgetAll();
   delete pointyPoultry;
}

/****************************************************************************\
	ReceivePlan read the plan for all animals
\****************************************************************************/
void poultryStock::ReceivePlan(char* fileExtension)
//Calculates number of animals to be imported + herd structure
{
   char fileName[13];
   //animalProduct * anAnimalProduct;
   double uricAcidPerYear;

   strcpy(fileName,"poultry.");
   strcat(fileName,fileExtension);
   UnsetCritical();
//   if(!OpenInputFile(fileName))
   if (FileExists(fileName))
	   OpenInputFile(fileName);
   else
   {
   	strcpy(fileName,"poultry.fnn");
   	if (FileExists(fileName))
      OpenInputFile(fileName);
   	else
		theMessage->FatalError("poultryStock::ReceivePlan - error in opening poultry.f??");
   }

	//double aNumber;
// Get number of animals produced per year
   double numHens=0, numBroilers=0, numPullets=0;
   SetCritical();
   Setcur_pos(0);
/*   string inputString;
   getline(*file,inputString,'\n');
   cout << inputString << endl;*/
   FindSection(GetName(),GetIndex());

   GetParameter("HENS",&numHens);
   GetParameter("BROILERS",&numBroilers);
   GetParameter("PULLETS",&numPullets);
   UnsetCritical();
   CloseInputFile();

 	double daysInYear = (double)theTime.daysInYear(1);

// Get production cycle details from building
  	henProductionCycle = ((poultrysection*) theHens->GetStableSection())->GetDaysInSection();
   pulletProductionCycle = ((poultrysection*) thePullets->GetStableSection())->GetDaysInSection();
   broilersProductionCycle = ((poultrysection*) theBroilers->GetStableSection())->GetDaysInSection();

   Nbudget.Reset();   //reset budget or it will accumulate N between years and CheckBalance will throw an error

// Set up objects to store information on purchase and sale of animals
   *pulletsInOutPrDay = *thePullets->GetthisProduct(); //*theProducts->GetProductElement("pullets");
   pulletsInOutPrDay->Setamount(0.0);
   *broilersInOutPrDay = *theBroilers->GetthisProduct();
   broilersInOutPrDay->Setamount(0.0);
   *hensInOutPrDay = *theHens->GetthisProduct();;
   hensInOutPrDay->Setamount(0.0);

   thePullets->SetaveWeight(pulletsInOutPrDay->GetEndWeight()/2);
   theBroilers->SetaveWeight(broilersInOutPrDay->GetEndWeight()/2);
   theHens->SetaveWeight(hensInOutPrDay->GetEndWeight()/2);

// determine how many of each animal type is present on a daily basis
   thePullets->SetNumberPrYear(numPullets);
   thePullets->SetNumberPrDay(numPullets* pulletProductionCycle/daysInYear);
   theBroilers->SetNumberPrYear(numBroilers);
   theBroilers->SetNumberPrDay(numBroilers* broilersProductionCycle/daysInYear);
   theHens->SetNumberPrYear(numHens);
   theHens->SetNumberPrDay(numHens * henProductionCycle/daysInYear);


   *pulletsPrDay = *thePullets->GetthisProduct(); //*theProducts->GetProductElement("pullets");
   pulletsPrDay->Setamount(numPullets * pulletsPrDay->GetEndWeight()/daysInYear);
   *broilersPrDay = *theBroilers->GetthisProduct();//*theProducts->GetProductElement("broilers");
   broilersPrDay->Setamount(numBroilers * broilersPrDay->GetEndWeight()/daysInYear);
   *hensPrDay = *theHens->GetthisProduct();//*theProducts->GetProductElement("hens");
   hensPrDay->Setamount(numHens * hensPrDay->GetEndWeight()/daysInYear);

   ReceiveFeed(fileExtension);

   // This next loop sets up the manure handling in animal housing. Gives an error in housing if it is removed!
	double heatOp=0.0;
   double energyIn=0.0;
   double maintEnergy=0.0;
   for(int i=0;i<pointyPoultry->NumOfNodes();i++)
	{
      poultry *aPoultry=pointyPoultry->ElementAtNumber(i);
      if (aPoultry->GetNumberPrDay()>0.0)
      {
         uricAcidPerYear= aPoultry->GetNumberPrDay()*aPoultry->GetamountSolid()*365/1000.0;
         energyIn=aPoultry->Getcurrentfeed()->GetAmount()*aPoultry->Getcurrentfeed()->GetME()*1000000.0/(aPoultry->GetNumberPrDay()*24.0 * 3600.0);
         maintEnergy=aPoultry->CalcMaintenanceEnergy(aPoultry->GetaveWeight());
         double n =  energyIn/maintEnergy;
         heatOp=aPoultry->GetNumberPrDay()* aPoultry->GetSensibleHeatProduction(aPoultry->GetaveWeight(),n);
         theMessage->FatalError("check");
         aPoultry->GetStableSection()->SetAnimalParameters(aPoultry->GetNumberPrDay(),uricAcidPerYear,0.0,heatOp);
      }
   }
}

/****************************************************************************\
	Get feed input and calculate the N needed each day for growth
   Only for animal model 1
\****************************************************************************/
void poultryStock::ReceiveFeed(char* fileExtension)
//Load feed and calculate growth N requirements
{
   char fileName[13];
   double numPullets =thePullets->GetNumberPrYear();
   double numBroilers = theBroilers->GetNumberPrYear();
   double numHens = theHens->GetNumberPrYear();

   strcpy(fileName,"POULTRYFEED.");
   if (fileExtension[0]=='F')
   {
      int yearNumber = 0;
      sscanf(fileExtension,"F%02d",&yearNumber);
      yearNumber = max(0,yearNumber);
      fileExtension[0] = '\0';
      sprintf(fileExtension,"F%02d",yearNumber);
   }
   strcat(fileName,fileExtension);

   double startNContent, startWeight, endNContent, endWeight, NGrowth, startCContent, endCContent, CGrowth;
   if (numHens>0)
   {
      theHens->ReceivePlan(fileName);

// Calculate N required for growth - note amount is in tonnes
// Calculate the N partitioned to growth based on the start weight and N content and the end weight and N content
      startNContent = ((animalProduct*) theProducts->GetProductElement("PULLETS"))->GetN_content();
      startWeight =   ((animalProduct*) theProducts->GetProductElement("PULLETS"))->GetEndWeight();
      endNContent = ((animalProduct*) theProducts->GetProductElement("HENS"))->GetN_content();
      endWeight =   ((animalProduct*) theProducts->GetProductElement("HENS"))->GetEndWeight();
      //Note that sow endweight is average start weight + average weight gain per parity
      NGrowth = (endWeight* endNContent - startWeight * startNContent)/(henProductionCycle * 1000.0);
      theHens->SetN_growth(NGrowth);
      startCContent = ((animalProduct*) theProducts->GetProductElement("PULLETS"))->GetC_content();
      endCContent = ((animalProduct*) theProducts->GetProductElement("HENS"))->GetC_content();
      CGrowth = (endWeight * endCContent - startWeight * startCContent)/(henProductionCycle * 1000.0);
      theHens->SetC_growth(CGrowth);
      theHens->SetN_growth(NGrowth);

// allow for growth of foetus and suckling pullets. The growth is here averaged over pregnancy and lactation
      double NContent = ((animalProduct*) theProducts->GetProductElement("EGGS"))->GetN_content();   //suckler pig final N content
      double Weight =   ((animalProduct*) theProducts->GetProductElement("EGGS"))->GetEndWeight();   //suckler pig final weight
      double totalEggN = NContent * Weight * theHens->GetEggsPerBird();  //total egg N per hen per lifetime
      double henLifetime = ((poultrysection*) theHens->GetStableSection())->GetDaysInSection();
      double eggN= totalEggN/(henLifetime * 1000);
      endCContent = ((animalProduct*) theProducts->GetProductElement("EGGS"))->GetC_content();
      double totalEggC = endCContent * Weight * theHens->GetEggsPerBird();
      double eggC = totalEggC/(henLifetime * 1000);
      theHens->SeteggN(eggN);
      theHens->SeteggC(eggC);
   }
   else
      theHens->Getcurrentfeed()->Setamount(0.0);
// pullets
   if (numPullets>0)
   {
      thePullets->ReceivePlan(fileName);
      //gets starting values from previous (younger) animal category - here no instance exists so get from Products.dat
	   double pulletProductionCycle = ((poultrysection*) thePullets->GetStableSection())->GetDaysInSection();
      startNContent = ((animalProduct*) theProducts->GetProductElement("PULLET"))->GetN_content();
      startWeight =   thePullets->GetthisProduct()->GetStartWeight();
      endNContent = thePullets->GetthisProduct()->GetN_content();
      endWeight =   thePullets->GetthisProduct()->GetEndWeight();
      NGrowth = (endWeight* endNContent - startWeight * startNContent)/(pulletProductionCycle * 1000);
      thePullets->SetN_growth(NGrowth);
      startCContent = ((animalProduct*) theProducts->GetProductElement("PULLET"))->GetC_content();
      endCContent = thePullets->GetthisProduct()->GetC_content();
      CGrowth = (endWeight * endCContent - startWeight * startCContent)/(pulletProductionCycle * 1000);
      thePullets->SetC_growth(CGrowth);
   }
   else
      thePullets->Getcurrentfeed()->Setamount(0.0);
// broilers
   if (numBroilers>0)
   {
      theBroilers->ReceivePlan(fileName);
	   double broilersProductionCycle = ((poultrysection*) theBroilers->GetStableSection())->GetDaysInSection();
      startNContent = thePullets->GetthisProduct()->GetN_content();
      startWeight =   thePullets->GetthisProduct()->GetEndWeight();
      endNContent = theBroilers->GetthisProduct()->GetN_content();
      endWeight =   theBroilers->GetthisProduct()->GetEndWeight();
      NGrowth = (endWeight* endNContent - startWeight * startNContent)/(broilersProductionCycle * 1000);
      theBroilers->SetN_growth(NGrowth);
      startCContent = thePullets->GetthisProduct()->GetC_content();
      endCContent = theBroilers->GetthisProduct()->GetC_content();
      CGrowth = (endWeight * endCContent - startWeight * startCContent)/(broilersProductionCycle * 1000);
      theBroilers->SetC_growth(CGrowth);
   }
   else
      theBroilers->Getcurrentfeed()->Setamount(0.0);
// make sure that all the growth is exported
   CheckNbalance();

// load the product info
//THIS WILL CURRENTY RETURN INCORRECT VALUES BECAUSE the code has not been fully adapted from pigs
   if (strcmp(fileExtension,"lp3")!=0)
   {
      double livestockPrice = 0.0;
      double sell = theProducts->GetActualSellPrice("PULLETS");
      double buy  = theProducts->GetActualBuyPrice("PULLETS");
      livestockPrice += ((sell+buy)/2)*pulletsInOutPrDay->GetAmount();
      sell = theProducts->GetActualSellPrice("broilers");
      buy  = theProducts->GetActualBuyPrice("broilers");
      livestockPrice += ((sell+buy)/2)*broilersInOutPrDay->GetAmount();
      sell = theProducts->GetActualSellPrice("hens");
      buy  = theProducts->GetActualBuyPrice("hens");
      livestockPrice += ((sell+buy)/2)*hensInOutPrDay->GetAmount();
      livestockPrice += (sell+buy)/2*(numHens/theTime.daysInYear(1));
      theOutput->AddIndicator(economicIndicator,"03.06 Value of livestock","Dkr",livestockPrice);
   }
}

/****************************************************************************\
	DailyUpdate is the daily execution of swines
\****************************************************************************/
void poultryStock::DailyUpdate(void)
{
   double NumberPrDay = theBroilers->GetNumberPrDay();
   theOutput->AddStateIndicator(economicIndicator,"11.31 broilers present","no",NumberPrDay);
   NumberPrDay = thePullets->GetNumberPrDay();
   theOutput->AddStateIndicator(economicIndicator,"11.32 pullets present","no",NumberPrDay);
   NumberPrDay = theHens->GetNumberPrDay();
   theOutput->AddStateIndicator(economicIndicator,"11.33 hens present","no",NumberPrDay);

	for(int i=0;i<pointyPoultry->NumOfNodes();i++)
	{
    if ((pointyPoultry->ElementAtNumber(i))->GetNumberPrDay()>0)
    {
       poultry * tempChuckies = pointyPoultry->ElementAtNumber(i);
       tempChuckies->DailyUpdate();
    }
   }
   ExportStock();
	theOutput->AddStateIndicator(environmentalIndicator,"34.60 N bound in poultry","kg N",0.0);
}

/****************************************************************************\
	Initialize's a new poultryStock
   BMP changed the name (hide of virtual function) - are there better options ???!!!
\****************************************************************************/
void poultryStock::InitPoultry(fstream * file, buildings* theBuildings)
{
	for(int i=0;i<pointyPoultry->NumOfNodes();i++)
    pointyPoultry->ElementAtNumber(i)->ReadParameters(file);
	char Indexstr[10];
   sprintf(Indexstr,"(%d)",Index);

   string outstring;
//Find animal housing for pigs
   outstring = (string) "Pullets" + Indexstr;
	thePullets->SetStableSection(theBuildings->PlaceAnimal(outstring.c_str()));
   outstring = (string) "Broilers" + Indexstr;
	theBroilers->SetStableSection(theBuildings->PlaceAnimal(outstring.c_str()));
   outstring = (string) "Hens" + Indexstr;
   theHens->SetStableSection(theBuildings->PlaceAnimal(outstring.c_str()));
}

/****************************************************************************\
	GetNumberPrDay return the total number of swines and slaughter swines
\****************************************************************************/
double poultryStock::GetNumber()
{
 double ret_val=0;
 for(int i=0;i<pointyPoultry->NumOfNodes();i++)
	{
	 ret_val+=pointyPoultry->ElementAtNumber(i)->GetNumberPrDay();
   }
	return ret_val;
}


void poultryStock::CheckNbalance()
{
//Checks to make sure that all N accumulated is also sold
 double totalGrowthN=0;
 double totalExportN=0.0;//Nbudget.GetOutput()*1000.0;;
 animalProduct * aProduct = GetStockBought();
 double totalImportN=aProduct->GetAmount() * aProduct->GetN_content() * theTime.daysInYear(1) * 1000.0; //Nbudget.GetInput()*1000.0;
 delete aProduct;
//growth first
 for(int i=0;i<pointyPoultry->NumOfNodes();i++)
	{
    poultry * tempPoultry = pointyPoultry->ElementAtNumber(i);
    if (tempPoultry->GetNumberPrDay()>0.0)
		 totalGrowthN+= tempPoultry->GetNumberPrDay() * tempPoultry->GetN_growth() * theTime.daysInYear(1);
   }
// convert from tonnes to kg
 totalGrowthN*=1000.0;
//export
 aProduct = GetStockSold();
 totalExportN=aProduct->GetAmount() * aProduct->GetN_content() * theTime.daysInYear(1) * 1000.0;    //in kg
 delete aProduct;
 double balance=totalExportN-(totalGrowthN+totalImportN);
 if (fabs(balance)>0.001)
 	theMessage->FatalError("poultryStock: N budget error - exported N and growth N are unequal");
 manure* fluidManure = new manure();
 manure* solidManure = new manure();
 for(int i=0;i<pointyPoultry->NumOfNodes();i++)
 {
    double manureN=0;
    double feedN=0;
    double growthN=0;
    poultry * tempPoultry = pointyPoultry->ElementAtNumber(i);
    if (tempPoultry->GetNumberPrDay()>0.0)
    {
       feedItem *currentfeed;
       currentfeed=tempPoultry->Getcurrentfeed();
       feedN = currentfeed->GetAmount() * currentfeed->GetAllN().n * theTime.daysInYear(1) * 1000.0;
       growthN = tempPoultry->GetNumberPrDay() * tempPoultry-> GetN_growth() * theTime.daysInYear(1) * 1000.0;
       tempPoultry->ProduceManure( fluidManure, solidManure);
       manureN = theTime.daysInYear(1) * (fluidManure->GetAmount() * fluidManure->GetAllN().n +
                           solidManure->GetAmount() * solidManure->GetAllN().n) * 1000.0;
       balance = feedN-(growthN + manureN);
       if (fabs(balance)>1.0)
          theMessage->FatalError("poultryStock: ", tempPoultry->GetName()," error - N in feed > (growth N + manure N)");
    }
 }
 delete fluidManure; delete solidManure;
}

void poultryStock::ExportStock()
{
// Sell a few animals per day
   if (pulletsInOutPrDay->GetAmount()>0.0)  //buy animals
	   theProducts->SubtractProduct(pulletsInOutPrDay);
   if (pulletsInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	pulletsInOutPrDay->Setamount(pulletsInOutPrDay->GetAmount()*-1.0);
	   theProducts->AddProduct(pulletsInOutPrDay);
   	pulletsInOutPrDay->Setamount(pulletsInOutPrDay->GetAmount()*-1.0);
   }

   if (broilersInOutPrDay->GetAmount()>0.0)  //buy animals
	   theProducts->SubtractProduct(broilersInOutPrDay);
   if (broilersInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	broilersInOutPrDay->Setamount(broilersInOutPrDay->GetAmount()*-1.0);
	   theProducts->AddProduct(broilersInOutPrDay);
   	broilersInOutPrDay->Setamount(broilersInOutPrDay->GetAmount()*-1.0);
   }

   if (hensInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	hensInOutPrDay->Setamount(hensInOutPrDay->GetAmount()*-1.0);
	   theProducts->AddProduct(hensInOutPrDay);
   	hensInOutPrDay->Setamount(hensInOutPrDay->GetAmount()*-1.0);
   }
//   double Nout=0.0;
}

void poultryStock::EndBudget(bool show)
{
 for(int i=0;i<pointyPoultry->NumOfNodes();i++)
	{
	  pointyPoultry->ElementAtNumber(i)->EndBudget(show);
   }
}

/****************************************************
Returns a product that contains the total N and C purchased in pigs per day
****************************************************/
animalProduct *poultryStock::GetStockBought()
{
	animalProduct * aProduct = new animalProduct();
   if (hensInOutPrDay->GetAmount()>0.0)  //buy animals
	   *aProduct + *hensInOutPrDay;
   if (pulletsInOutPrDay->GetAmount()>0.0)  //buy animals
	   *aProduct + *pulletsInOutPrDay;
   if (broilersInOutPrDay->GetAmount()>0.0)  //buy animals
	   *aProduct + *broilersInOutPrDay;
	return aProduct;
}

/****************************************************************************\
Returns a product that contains the total N and C sold  in pigs per day
\****************************************************************************/
animalProduct * poultryStock::GetStockSold()
{
	animalProduct * aProduct = new animalProduct();
//pullets, broilers and hens sold

   if (pulletsInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	pulletsInOutPrDay->Setamount(pulletsInOutPrDay->GetAmount()*-1.0);
	   *aProduct + *pulletsInOutPrDay;
   	pulletsInOutPrDay->Setamount(pulletsInOutPrDay->GetAmount()*-1.0);
   }

   if (broilersInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	broilersInOutPrDay->Setamount(broilersInOutPrDay->GetAmount()*-1.0);
	   *aProduct + *broilersInOutPrDay;
   	broilersInOutPrDay->Setamount(broilersInOutPrDay->GetAmount()*-1.0);
   }

   if (hensInOutPrDay->GetAmount()<0.0)  //sell animals
   {
   	hensInOutPrDay->Setamount(hensInOutPrDay->GetAmount()*-1.0);
	   *aProduct + *hensInOutPrDay;
   	hensInOutPrDay->Setamount(hensInOutPrDay->GetAmount()*-1.0);
   }
	return aProduct;
}

