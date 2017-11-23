/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "cattle.h"
#include "../base/base.h"

#include "../products/products.h"
#include "../base/settings.h"


#ifdef _STANDALONE
   #include "../products/message.h"
#endif

#include "../products/feedResource.h"
#include "../base/nixcbits.h"

#ifdef _STANDALONE
cattle::cattle()
	:animal()
{
	Initialise();
}
#endif

/**
 * Constructor with arguments
 * Also calles: Initialise() and
 * base(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
 */
cattle::cattle(const char *aName,const int aIndex,const base * aOwner):
     animal(aName,aIndex,aOwner)
{
	Initialise();
}

/**
 * Copy Constructor - is shallow.
 * Should it ever be caled then ???!!! BMP
 */
cattle::cattle(const cattle& anAnimal)
   : animal(anAnimal)
{
   bodyProtein 				= anAnimal.bodyProtein;
   bodyFat 						= anAnimal.bodyFat;
   age 							= anAnimal.age;
   MBW 							= anAnimal.MBW;
   weight						= anAnimal.weight;
//   uterusProtein				= anAnimal.uterusProtein;
   code 							= anAnimal.code;
   isFemale 					= anAnimal.isFemale;
   isFoetus 					= anAnimal.isFoetus;
   isSuckling					= anAnimal.isSuckling;
   isYoung						= anAnimal.isYoung;
	feedPlanList 				= NULL;//must be set explicitly later
   feedUseList					= NULL;
   daysFromParturition 		= anAnimal.daysFromParturition;
   daysFromConception 		= anAnimal.daysFromConception;
   lactationNumber			= anAnimal.lactationNumber;
   feedingGroup				= 0;  //set explicitly
   lactating 					= anAnimal.lactating;
   pregnant 					= anAnimal.pregnant;
   milk							= NULL;
   variableCosts 				= anAnimal.variableCosts;
   mother						= NULL;
   calf 							= NULL;
   foetus						= NULL;


   DM_intake					= 0.0;
   currentDiet					= NULL;
   milk							= NULL;
   hoursHoused					= anAnimal.hoursHoused;
   gestationPeriod			= anAnimal.gestationPeriod;
   milkProteinConc         = anAnimal.milkProteinConc;
   milkFatConc					= anAnimal.milkFatConc;
   lactoseConc					= anAnimal.lactoseConc;
   ME_intake					= 0.0;
   FE_intake					= 0.0;
   FEDemand						= 0.0;
   lastMilkYield				= anAnimal.lastMilkYield;
   growth 						= new feedItem();
   detailsFile					= NULL;
}

/**
* Destructor
*/
cattle::~cattle()
{
   delete calf;
  	delete foetus;
   delete growth;
   delete feedUseList;
   delete currentDiet;
   if (detailsFile)
   {
/*if(Index==36)
cout << "";*/
//      detailsFile->close();
//      cout << "closing " << Index << endl;
   	delete detailsFile;
      detailsFile = NULL;
   }
}

/**
 * Initializing of variable
 */
void cattle::Initialise()
{
   bodyProtein 						= 0.0;
   bodyFat								= 0.0;
   age 									= 0;
   MBW 									= 0.0;
   weight								= 0.0;
   code									= 0;
   isFemale 							= false;
   isFoetus 							= false;
   isSuckling 							= false;
   isYoung 								= false;
	feedPlanList 						= NULL;//must be set explicitly through
   feedUseList 						= new cloneList <product>;
   daysFromParturition 				= 0;
   daysFromConception 				= 0;
   lactationNumber					= 0;
   feedingGroup						= 0;
   lactating							= false;
   pregnant								= false;
   mother								= NULL;
   calf 									= NULL;
   foetus								= NULL;


   DM_intake							= 0.0;
   currentDiet							= NULL;
   milk									= NULL;
   lastMilkYield						= 0.0;
   hoursHoused							= 0.0;
   gestationPeriod					= 281;
   milkProteinConc					= 0.0;
   milkFatConc							= 0.0;
   lactoseConc							= 0.0;
   ME_intake 							= 0.0;
   FE_intake 							= 0.0;
   FEDemand								= 0.0;
   aminoAcidSupply					= 0.0;
   aminoAcidDemand					= 0.0;
	dungN									= 0.0;
   urineN								= 0.0;
   rumenNsurplus						= 0.0;
   Grazed_DM							= 0.0;
   Grazed_FE							= 0.0;
   growth 								= new feedItem();
#ifndef _STANDALONE
   if (globalSettings->DetailsData.getCowDetails()==true)
		CreateDetailFile(Index);
   else
   	detailsFile = NULL;
#endif
   Nbudget.SetNames("cattle","N");
   Cbudget.SetNames("cattle","C");
}

/**
* Initialize a cattle from a file
* @param afile must be initialize with correct file
*/
void cattle::ReadParameters(commonData * afile)
{

	afile->setCritical(true);

   string namestr;
	namestr=(string) "Animal";
	afile->FindSection(namestr,code);  // find the correct place in the input file
	afile->FindItem("MBW",&MBW);
	afile->FindItem("amountFluid",&amountFluid );
    afile->FindItem("amountSolid",&amountSolid );
    afile->FindItem("Alias",&Alias);
    afile->FindItem("milkprotein",&milkProteinConc);
    afile->FindItem("milkfat",&milkFatConc);
    afile->FindItem("lactose",&lactoseConc);
    afile->FindItem("acceptanceCoefficient",&acceptanceCoefficient);

}

/**
*	Update what happens with a cow on a daly basis
*/
void cattle::DailyUpdate()
{
	if ((feedPlanList->NumOfNodes()==0))
   	theMessage->FatalError("cattle: DailyUpdate - no feed plan or fields");

	checkBalances(false);
  animal::DailyUpdate();
  if (currentDiet)
	delete currentDiet;
  currentDiet = new feed_resource();

  if (growth)          //should really only be used for standalone (but is used generally until we can manage milking parlours)
  {
   	delete growth;
      growth=NULL;
  }
   growth = new feedItem();
#ifdef _STANDALONE
   CheckBirth();
#endif
   intake();
   age++;
   if (lactating)
   	daysFromParturition++;
	Production();

   double manureN = fluidManurePrDay->GetAmount() * fluidManurePrDay->GetAllN().n + solidManurePrDay->GetAmount() * solidManurePrDay->GetAllN().n;
   double manureC = fluidManurePrDay->GetAmount() * fluidManurePrDay->GetC_content() + solidManurePrDay->GetAmount() * solidManurePrDay->GetC_content();
   Nbudget.AddOutput(manureN *1000.0);
// from here on until the end of the function, manure is scaled for animal number
   manureN*=Number;
   manureC*=Number;
   *fluidManurePrDay * Number;
   *solidManurePrDay * Number;
   double amountManure = fluidManurePrDay->GetAmount() + solidManurePrDay->GetAmount();
#ifndef _STANDALONE
   if (isYoung)
   {
	   theOutput->AddIndicator(environmentalIndicator,"33.41 N in young cattle manure","kg N",manureN*1000.0);
	   theOutput->AddIndicator(environmentalIndicator,"43.41 4 in young cattle manure","kg C",manureC*1000.0);
	   theOutput->AddIndicator(economicIndicator,"19.63 Manure produced by young cattle","t",amountManure);
   }
   else
   {
   	theOutput->AddIndicator(environmentalIndicator,"33.42 N in dairy cattle manure","kg N",manureN*1000.0);
   	theOutput->AddIndicator(environmentalIndicator,"43.42 C in dairy cattle manure","kg C",manureC*1000.0);
	   theOutput->AddIndicator(economicIndicator,"19.64 Manure produced by dairy cattle","t",amountManure);
   }
   // excrete manure to the right location
   if (hoursHoused>0.0)
   {
   	animalSection * theSection = GetStableSection();
      if (theSection)
      {
			manure *faecesToHouse = new manure(*solidManurePrDay);
			manure *urineToHouse = new manure(*fluidManurePrDay);
         //weight amount excreted in animal housing according to the proportion of the day they are housed
         faecesToHouse->Setamount(solidManurePrDay->GetAmount()*(hoursHoused/24.0));
         urineToHouse->Setamount(fluidManurePrDay->GetAmount()*(hoursHoused/24.0));
         double manureNToHousing = (urineToHouse->GetAmount()*urineToHouse->GetAllN().n +
                  faecesToHouse->GetAmount()*faecesToHouse->GetAllN().n);
         double manureCToHousing = (urineToHouse->GetAmount()*urineToHouse->GetC_content() +
                  faecesToHouse->GetAmount()*faecesToHouse->GetC_content());
         theOutput->AddIndicator(environmentalIndicator,"33.81 Cattle manure N to animal housing","kg N",manureNToHousing*1000.0);
         theOutput->AddIndicator(environmentalIndicator,"43.81 Cattle manure C to animal housing","kg C",manureCToHousing*1000.0);

         theSection->RcvManure(urineToHouse,faecesToHouse);          // manure to stable
         // fudge - should calculate heat from diet + latent heat loss
//         GetStableSection()->AddanimalHeatOp(Number*weight*heatProduction/SRW);
         delete faecesToHouse;
         delete urineToHouse;
			double heatOp=Number*GetSensibleHeatProduction();
         theSection->AddanimalHeatOp(heatOp);

      }
      else theMessage->FatalError("cattle: no housing designated for receiving manure from ",GetAlias().c_str());
   }
   if (hoursHoused<24.0)
   {

   }
#endif

  if (globalSettings->DetailsData.getCowDetails()==true)
		WriteCowOutput(false);

//   cout << GetName() << &Nbudget << endl;
  checkBalances(false);
  if ((!calf)&&(milk))          //should really only be used for standalone (but is used generally until we can manage milking parlours)
  {
   	delete milk;
      milk=NULL;
  }
}
/**
 * makes afeedPlanList the new feedPlanList. Is no longer used in Fasset
 */
void cattle::ReceivePlan(cloneList <product>* afeedPlanList)  //redundant
{
  if (feedPlanList) delete feedPlanList;
  feedPlanList = afeedPlanList->clone();
}

/**
 * Calls functions to calc growth + production of dung and urine, returns N surplus in diet
 * For dairy cattle, functions also calc foetus growth and milk production
*/
double cattle::Production()
{
	string name = this->GetName();
   double deficit=0.0;

   Partition();

//   Nbudget.AddOutput((urineN + dungN)/1000.0);

   if (urineN>=0)
   {
      double amountfeed			= currentDiet->GetAmount() * 1000.0;   //in kg
      double amountFluidFW = GetUrineVolume();    //new urine volume model
      double totalAmountPrDay= amountFluidFW/1000.0; //convert to tonnes

	// sort out urine first
      double urineNConc = urineN/(totalAmountPrDay * 1000000.0);  //concentration, t per t
      double urineCconc = urineNConc*60.0/28.0;   //assumes all N in urine is urea (ratio of C to N in urea (2*(NH2) + CO))

      //WHAT HAPPENS TO THE C WHEN IT IS DEPOSITED ???
      double urineCarbon = urineCconc * amountFluidFW;
      double urineDMconc = urineCconc/0.4;
      double urineDM = 0.0; //1000.0 * totalAmountPrDay * urineDMconc;
      fluidManurePrDay->Setname("CATTLE-URINE");
      theProducts->GiveProductInformation(fluidManurePrDay);
      fluidManurePrDay->Setamount(totalAmountPrDay);
      fluidManurePrDay->SetorgN_content(0.0);
      fluidManurePrDay->SetNH4_content(urineNConc);
      fluidManurePrDay->SetDryMatter(urineDMconc);
      fluidManurePrDay->SetC_content(urineCconc);
      fluidManurePrDay->SetNO3_content(0.0);
      fluidManurePrDay->SetP_content((amountfeed *(currentfeed->GetP_content())*(currentfeed->GetP_digest())-P_growth)/totalAmountPrDay);
      fluidManurePrDay->SetK_content(0.0);                      // rettes senere
      fluidManurePrDay->SetpH(8.0);


//solid manure
      double dungNConc;
      double ash = amountfeed * (currentDiet->GetdryMatter() - currentDiet->Getorganic());
      double indigestibleOM = amountfeed * (1-currentDiet->GetOMD()) * currentDiet->Getorganic() - urineDM;
		double faecalCarbon = indigestibleOM * 0.4;
      double solidDM = indigestibleOM + ash;
      double amountSolidFW = solidDM/0.15;   //assumes 15% DM content of faeces
      double faecalCconc = faecalCarbon/amountSolidFW;
      totalAmountPrDay = amountSolidFW/1000.0; //convert to tonnes
      dungNConc = dungN/(totalAmountPrDay * 1000000.0) ;
      if (totalAmountPrDay<0.0)
      	theMessage->FatalError(" ");
      solidManurePrDay->Setname("CATTLE-FAECES");
      theProducts->GiveProductInformation(solidManurePrDay);          // sets the default values for solid-manure
      solidManurePrDay->Setamount(totalAmountPrDay);
      solidManurePrDay->SetorgN_content(dungNConc);
      solidManurePrDay->SetNH4_content(0.0); solidManurePrDay->SetNO3_content(0.0);
      solidManurePrDay->SetDryMatter(0.15);
      solidManurePrDay->SetC_content(faecalCconc);
      solidManurePrDay->SetP_content(amountfeed*(currentfeed->GetP_content())*(1.0-currentfeed->GetP_digest())/totalAmountPrDay);
      solidManurePrDay->SetK_content((amountfeed*(currentfeed->GetK_content())-K_growth)/totalAmountPrDay);
      solidManurePrDay->SetpH(7.6);

//  C and heat dynamics
      double methane = GetMethaneProduction();  //g per day
      double methaneCarbon = methane * 12/(16 * 1000.0); //kg
      double milkCarbon = 0.0;
      if (milk)
         milkCarbon = milk->GetAmount() * 1000.0 * milk->GetC_content();
		CalcHeatProduction(fluidManurePrDay);
      double carbonIntake = currentDiet->GetAmount() * 1000.0 * currentDiet->GetC();
      double CRetained = growth->GetAmount() * growth->GetC_content();
      if (foetus)
				 CRetained+=foetus->Getgrowth()->GetAmount() * foetus->Getgrowth()->GetC_content();
		double CO2_C = carbonIntake - (CRetained + methaneCarbon + urineCarbon + faecalCarbon + milkCarbon);
#ifndef _STANDALONE
      theOutput->AddIndicator(environmentalIndicator,"41.01 C input into cattle","kg C",Number * carbonIntake);
      theOutput->AddIndicator(environmentalIndicator,"41.04 C in cattle faeces","kg C",Number * faecalCarbon);
      theOutput->AddIndicator(environmentalIndicator,"41.03 C in cattle urine","kg C",Number * urineCarbon);
      theOutput->AddIndicator(environmentalIndicator,"41.07 C retained in cattle body","kg C",Number * CRetained);
      theOutput->AddIndicator(environmentalIndicator,"41.05 C in cattle CO2 production","kg C",Number * CO2_C);
      theOutput->AddIndicator(environmentalIndicator,"41.02 C in cattle CH4 production","kg C",Number * methaneCarbon);
      theOutput->AddIndicator(environmentalIndicator,"41.06 C in milk production","kg C",Number * milkCarbon);
#endif
      double carbonOutput = CO2_C + methaneCarbon + urineCarbon + faecalCarbon + milkCarbon;
      Cbudget.AddOutput(carbonOutput);
      }
    else {
    		string animalName =this->GetName();
//         theMessage->Warning(animalName+" required additional feed");
         theMessage->FatalError("cattle::Production - urine gone negative");
         deficit=urineN;
         }

 return deficit;
}

/**
 * Makes a Fatal Error and stops Fasset. Should not be called
 */
void cattle::ManureProduction( fstream* fileStream)
{
	theMessage->FatalError("Function in cattle not implemented");
  *fileStream << ".LIQM    0.0\n"; //dummy line which is never reached - stops a warning
}

/**
 * Simulates digestion of protein in gut - returns the amount of N in dung, surplus rumen N and N available for metabolism
*/
void cattle::ProteinDigestion( double *availableN)
{

//   double AAfromMilk = 0.0;
   *availableN = 0.0;

   rumenNsurplus = 1000.0 * currentDiet->Getpbv() * currentDiet->GetAmount()/N_to_protein();
/*
   This code calculates protein digestibility using the DK system. It has been replaced by the a system that assumes that all N
   not ending in dung is available to support protein demand (which is not true, since the balance of amino acids is also a factor).
   However, the new system, based on an empirical equation to calculate faecal N, works better.
	*availableN =1000.0 * currentDiet->Getaat() * currentDiet->GetAmount()/N_to_protein();
   double trueProteinDigestibility = currentDiet->GetPropRDP() + (1 -currentDiet->GetPropRDP()) * currentDiet->GetpropUDPdigestible();
   double undigestedFeedProtein =  (1-trueProteinDigestibility) * currentDiet->GetAmount() * 1E6 * currentDiet->GetProteinContent();
   double undigestedMicrobialProtein =   currentDiet->GetAmount() * 1000.0 * currentDiet->GetCHOContent()
   															* currentDiet->GetpropDigestCHO() * 179.0 * (1.0 - 0.78);
   double endogenousFaecalProtein = 12.5 * DM_intake;
   dungN = (undigestedFeedProtein + undigestedMicrobialProtein + endogenousFaecalProtein)/N_to_protein(); */

   if (isSuckling)    //note that milk bypasses the rumen
   {
/*      double milkProtein = milk->GetAmount() * 1000000.0 * milk->GetProteinContent();
      AAfromMilk = milk->GetAmount()* 1000000.0 * milk->GetProteinContent() *
                       milk->GetpropAAinUDP() * milk->GetpropUDPdigestible();
	   *availableN = AAfromMilk/N_to_protein();*/
		*currentDiet = *currentDiet + *milk;
      Nbudget.AddInput(milk->GetAmount() * 1000.0 * milk->GetAllN().n);
      Cbudget.AddInput(milk->GetAmount() * 1000.0 * milk->GetC());
      delete milk;
      milk = NULL;
   }

   double N_intake = 1E6 * currentDiet->GetorgN_content().n * currentDiet->GetAmount();
   dungN = 0.04 * N_intake + (DM_intake * DM_intake * 1.8/6.25) + DM_intake * 20.0/6.25;     //note that DM_intake is updated to take account of milk intake in the intake function
	*availableN = N_intake - dungN;
   aminoAcidSupply = *availableN * N_to_protein();   //register amino acid supply
   if (dungN<0.0)
   	theMessage->FatalError("cattle::ProteinDigestion - dung N gone negative");
}

/**
 * Returns the maintenance N requirement in g.
 * Calculated by 3.02 * weight^0.7)/6.25. Nice to know: why this fomular and those numbers
 *
 */
double cattle::CalcMaintenanceN()
{
 double ret_val = 3.02 * pow(weight,0.75)/N_to_protein();

 return ret_val;
}

/**
 * Makes a feed_resource with default values if aStoredProduct is true or with the same values as menuPtr
 */
feed_resource * cattle::GetFeedItem(product * menuPtr)
{
   product * aStoredProduct =theProducts->GetCopyStoredFeedProduct(menuPtr);   //returns a copy of the product if it is found in storage
   //next line will get a feed of a particular type, rather than a particular feed. Obsolete?
   //product * aStoredProduct = ((feed_resource *) theProducts->GetProductType(menuPtr));

   feed_resource * aFeed_resource = new feed_resource(*(feedItem*) menuPtr);
   if (aStoredProduct)
   {
      if (aStoredProduct)
      {
         //this section updates the default variables with the actual characteristics of the feed stored
         //if there is not enough in store, the remainder will later be imported (with the same quality as that in storage!)
         aFeed_resource->Setamount(0.0);
         *aFeed_resource+ *aStoredProduct;
         aFeed_resource->Setamount(menuPtr->GetAmount());
      }
      delete aStoredProduct;
   }
   aFeed_resource->SetIsSupplement(true);
   return aFeed_resource;
}

//NJH March 2003
/**
* Initialise cattle
*/
void cattle::Initialise(int aCode, int anAge, double LU, double numPrCohort, bool isAYoung)
{
   SetisYoung(isAYoung);
   SetNumber(numPrCohort);
   SetDE(LU);
   code = aCode;
   age = anAge;
   bodyProtein = 0.0;
   bodyFat = 0.0;
   if (!isFoetus)
   {
	   //!Create a standard animal
      weight = Get_norm_weight();
      GetStandardComposition(weight,&bodyFat,&bodyProtein);
      Nbudget.AddInput(bodyProtein/N_to_protein());
      Cbudget.AddInput(GetCowC());
   }
   else   //is a foetus
   {
       SetdaysFromConception((int) age);
       Setage(0);
       InitialiseFoetus();
       SetisSuckling(false);
       SetisYoung(true);
       SetdaysFromParturition(0);
       SetlactationNumber(0);
       Setlactating(false);
       Setpregnant(false);
       Nbudget.AddInput(bodyProtein/N_to_protein());
       Cbudget.AddInput(GetCowC());
       currentDiet=NULL;
   }

   if ((daysFromConception>0) && (!GetisFoetus()))
   {
      Createfoetus();
      foetus->SetisFoetus(true);
      foetus->Setmother((cattle*) this);
      foetus->Initialise(code, int(daysFromConception), LivestockUnits, Number, true);
      Nbudget.AddInput((/*uterusProtein + */foetus->GetbodyProtein())/N_to_protein());
      Cbudget.AddInput(GetCarbon(foetus->GetbodyFat(),(/*uterusProtein+ */foetus->GetbodyProtein())));
      weight+=foetus->Getweight();
      pregnant = true;
   }

//		bodyProtein = 12.0 * log(1.0 + exp(-6.0*(weight/MBW)+2.4)) + 72*(weight/MBW) - 29.84;
//	should be set to a real value - if I could work out what it should be! Does not matter unless animals need to remobilise energy
   variableCosts = 0.0;
   mother		= NULL;

//      Nbudget.AddInput(bodyProtein/N_to_protein());
   ME_intake	=0.0;
   FE_intake	=0.0;
   DM_intake	=0.0;
   hoursHoused	=0.0;
//   EndBudget(false);
}
void cattle::InitialiseFoetus()
{
 SetbodyProtein(CalcFoetusProtein(0.0)/1000.0);
 SetbodyFat(CalcFoetusFat(0.0)/1000.0);
 Setweight(GetFoetusWeight(0.0));
}

/**
 * Generate a fatal Error. Should not be called
 */
double cattle::Get_norm_weight()
{
   theMessage->FatalError("cattle: Get_norm_weight() - this should be called from an overloaded function");
   return 0.0;
}

/**
 * Get all N in cattle - takes account of whether the individual cow instances represent more than one animal
*/
double cattle::GetCattleN()
{
	double NRemaining=GetCowN();
   if (calf)
   	NRemaining+=(calf->GetbodyProtein())/N_to_protein();
	return Number * NRemaining;
}

/**
 * Get all N in a cow - takes no account of whether the cow represents one or several animals
*/
double cattle::GetCowN()
{
	double NRemaining=0;
   NRemaining=bodyProtein/N_to_protein();
   if (foetus)
   	NRemaining+=(foetus->GetbodyProtein())/N_to_protein();
	return NRemaining;
}

/**
 * return a factor based on herbage_mass. The factor represent how much grass on a field  a cow can eat.
 *  Wrong calculation is used!!
 * <param name="herbage_mass"> is a plant</param>
 */
double cattle::Get_availability_factor( double herbage_mass)
{
 double factor;
 if (herbage_mass>3.0) factor = 1.0;   //to avoid exp function going bonkers for large herbage masses
 else
 {
   factor = 1.0; // !!!!!!!!!!!!!!!!!!!!!!!!!!
   if (herbage_mass<0.25)
      factor=0.0;
//   factor = 1.0-exp(-5.0*herbage_mass); // earlier value 5 !!!
 }
 if ((factor<0.0)||(factor>1.0))
 {
	 cout << herbage_mass << " " << factor << endl;
 	theMessage->FatalError("uk_cattle: Availability factor less than zero og bigger than one!");
  }
 return factor;
}

/**
 * Makes a list of feed_resource based upon nr of notes in feedPlanList combined with number if patches.
 * Sorted in the end using quicksort2
 * <param name="alist"> is the sorted list</param>
 * <param name="count"> is the size of the list</param>
 */
void cattle::DoSelection(linkList <feed_resource> * alist, int * count)
{
	int counter;
	temp_double_array palatabilities;  									// stores palatabilities
	temp_ptr_array ptrlist;

   if(!feedPlanList)
		theMessage->FatalError("cattle::", GetName()," feedPlanList not present");

   feed_resource *aFeed_resource;
	int Hi=0;
	// intlize alist, palatabilities and Hi;
   for (int i=0; i<feedPlanList->NumOfNodes(); i++)
   {
		product * menuPtr = feedPlanList->ElementAtNumber(i);
      aFeed_resource = GetFeedItem(menuPtr);      //returns copy of the feed resource. Gets stored feed if available, otherwise table values used
     										// for debugging   tempfeedList->InsertLast(aFeed_resource);
   	if ((aFeed_resource->GetdryMatter() - aFeed_resource->Getorganic())<0.0)
							   theMessage->FatalError("org");
		alist->InsertLast(aFeed_resource);
      //get DM palatabilities
		palatabilities[Hi]=GetPalatability(aFeed_resource);
		Hi++;
   }

	if (Hi==0) theMessage->FatalError("cattle: no feed available!!");
// Sort feed resources according to OMD
   Hi--;
   for (counter=Hi;counter>=0;counter--)
   	ptrlist[counter]=(void*) alist->ElementAtNumber(counter);
   // Below is the "Combsort" sorting algorithm, which is nearly as fast as
   // Quicksort, and does not require extra space on heap and stack, i.e. more
   // suited for this purpose than the implemented Quicksort (bmp).
//	Comsort(palatabilities, ptrlist, Hi);
   quicksort2(palatabilities,ptrlist,0,Hi);  //rank patches in order of digestibility}
   alist->ForgetAll();
   for (counter=0;counter<=Hi;counter++)
   {
		alist->InsertLast((feed_resource*) ptrlist[counter]);
   }
   *count=Hi;
}



/**
 * Setting to lactating false
 */
void cattle::DryOff()
{
	lactating = false;
}
/**
 * printing all items of feedPlanList out to fs if it is available or else to cout
 * <param name="fs"> a filestream witch feedPlanList can be printet to if it is available</param>
 */
void cattle::ShowCurrentDiet(fstream * fs)
{
   for (int i=0; i<feedPlanList->NumOfNodes(); i++)
   {
		feed_resource * menuPtr = (feed_resource*) feedPlanList->ElementAtNumber(i);
      if (fs)
 		*fs << menuPtr;
      else
      cout << menuPtr;
   }
}

/**
 * Detailed data output to file - used in standalone version
 */
void cattle::WriteCowOutput(bool header)
{
	if (header)
   {
      *detailsFile << "date" << "\t" << "age" << "\t" << "emptyBodyWeight" << "\t" << "DM_intake" << "\t" << "ME_intake" << "\t" << "FE_intake" << "\t"
         << "feedN" << "\t" << "AAT" << "\t" << "limit" << "\t" << "foetusWt" << "\t"  << "milkYield" << "\t" << "dungN" << "\t"
         << "urineN" << "\t" << "milkN" << "\t" << "growthN" << "\t" << "feedGrp" << "\t" << "housed" << "\t" <<
          "code1" << "\t" << "DM1" << "\t" << "FE1" << "\t" <<
          "code2" << "\t" << "DM2" << "\t" << "FE2" <<"\t" <<
          "code3" << "\t" << "DM3" << "\t" << "FE3" <<"\t" <<
          "code4" << "\t" << "DM4" << "\t" << "FE4" <<"\t" <<
          "code5" << "\t" << "DM5" << "\t" << "FE5" << "\t" <<
          "code6" << "\t" << "DM6" << "\t" << "FE6" <<"\t" <<
          "code7" << "\t" << "DM7" << "\t" << "FE7" <<"\t" <<
          "code8" << "\t" << "DM8" << "\t" << "FE8" <<
          endl;
   }
   else
   {
      *detailsFile << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay() << "\t";
      double foetusWeight = 0.0;
      double growthN =0.0;  //does not include growth in uterus protein
      if (foetus)
      {
         foetusWeight=foetus->Getweight();
         growthN+=foetus->Getgrowth()->GetAmount() * foetus->Getgrowth()->GetTotalN().n * 1000.0;
      }
      growthN+=growth->GetAmount() * growth->GetTotalN().n * 1000.0;
      double milkYield=0.0;
      double milkN =0.0;
      if (milk)
      {
         milkYield=milk->GetAmount() * 1000.0;
         milkN=milkYield * 1000.0 * milk->GetorgN_content().n;
      }
      double feedN=currentDiet->GetAmount()*currentDiet->GetTotalN().n*1000000.0;
      double dungN = solidManurePrDay->GetTotalN().n * solidManurePrDay->GetAmount() * 1000000/GetNumber();
      double urineN = fluidManurePrDay->GetTotalN().n * fluidManurePrDay->GetAmount() * 1000000/GetNumber();
      *detailsFile << age << "\t" << weight << "\t" << DM_intake << "\t" << ME_intake << "\t" << FE_intake << "\t"
            << feedN << "\t"  << 1000.0 * currentDiet->GetAmount()*currentDiet->Getaat() << "\t"  << limitation << "\t" << foetusWeight << "\t"
            << milkYield << "\t" << dungN << "\t" << urineN << "\t" << milkN << "\t" << growthN << "\t" << feedingGroup << "\t" << hoursHoused << "\t" ;
      double grazedHerbageDM=0.0;
      double grazedHerbageFE=0.0;
      for (int counter=0;counter<feedUseList->NumOfNodes();counter++)
      {
      	feed_resource * aFeedResource = (feed_resource *) feedUseList->ElementAtNumber(counter);
//	      *afile << aFeedResource->GetCode() << "\t" << aFeedResource->GetAmount() * aFeedResource->GetdryMatter() << "\t";
			if (aFeedResource->GetCode()<1000)
		      *detailsFile << aFeedResource->GetCode() << "\t" << aFeedResource->GetAmount() * aFeedResource->GetdryMatter() * 1000.0
            << "\t" << aFeedResource->GetAmount() * aFeedResource->GetfeedUnitsPerItemUnit() << "\t";
         else
         {
	      	grazedHerbageDM+=aFeedResource->GetAmount() * aFeedResource->GetdryMatter() * 1000.0;
	      	grazedHerbageFE+=aFeedResource->GetAmount() * aFeedResource->GetfeedUnitsPerItemUnit();
         }
      }
      if (grazedHerbageDM>0.0)
      {
      	*detailsFile << "1000+" << "\t" << grazedHerbageDM << "\t";
      	*detailsFile << "1000+" << "\t" << grazedHerbageFE << "\t";
      }
      *detailsFile << endl;
   }
//   detailsFile->close();
}

/**
 * Create and open a file for details of cow production (only called if cowDetails set in output)
 */
void cattle::CreateDetailFile(int Num)
{
   detailsFile = new fstream();
   char filename[20];
   char stub[20];
   const char * name=GetName();
   sprintf(stub,name);

   sprintf(detailsFileName,globalSettings->getOutputDirectory().c_str());
   strcat(detailsFileName,stub);
   sprintf(filename,"%d.xls",Num);
   strcat(detailsFileName,filename);
   detailsFile->open(detailsFileName, ios::out);
/*   cout << "creating " << leader << endl;
   if (Num==36)
   cout << "";                           */
   WriteCowOutput(true);
}



/**
 *Add weight and protein to foetus
 */
void cattle::GrowFoetus(double newFoetusProtein, double newFoetusFat, double newFoetus)   //these should be in kg
{
	if (!isFoetus)
   	theMessage->FatalError("cattle:GrowFoetus - this animal is not a foetus!");
   else
   {
      bodyProtein+=newFoetusProtein;
      Nbudget.AddInput(newFoetusProtein/N_to_protein());
      bodyFat+=newFoetusFat;
      Cbudget.AddInput(GetCarbon(newFoetusFat,newFoetusProtein));
      growth->Setamount(newFoetus);
      growth->SetorgN_content(newFoetusProtein/(N_to_protein() * newFoetus));
      growth->SetC_content((GetCarbon(newFoetusFat, newFoetusProtein))/newFoetus);
      weight+=newFoetus;
      daysFromConception++;
   }
}

/*
 * Calculate methane production in g per day. Nice to know: why those default values and where does the fomula come from?
 */
double cattle::GetMethaneProduction()
{
	double methane = 0.0;
   double amountFeed = 1000.0 * currentDiet->GetAmount();
   double protein = currentDiet->GetorgN_content().n * N_to_protein();
   double a;

	if (lactationNumber == 0)
   	a = 16.0;
   else
   	a = 63.0;

   methane = a + amountFeed * (79 * currentDiet->Getcell() + 10 * currentDiet->CalcNFE() +   //from husdyrbrug rapport 54, vol 2, p336
    					26 * protein - 212 * currentDiet->Getfat());
   if (methane<0.0)
   	methane=0.0; // could be cos animals are young. Equation gives negative values for diets that are based on milk products
   return methane;
}

/**
 * Calculate gravid uterus protein in grams. Nice to know: why those default values and where does the fomula come from?
 */
double cattle::CalcGravidUterusProtein(double offset) //weight in grams
{
  double gravidUterusProtein= 1000.0 * exp(8.536 - 13.12 * exp(-0.00262*(daysFromConception + offset)));
  double adjustment = MBW*0.07/40.0;
  gravidUterusProtein*=adjustment;
  return gravidUterusProtein;
}
/**
 * Calculate gravid uterus energy in MJ. Nice to know: why those default values and where does the fomula come from?
 */
double cattle::CalcGravidUterusEnergy(double offset) //weight in grams
{
  double gravidUterusEnergy= exp(349.222 - 349.164 * exp(-0.0000576*(daysFromConception + offset)));
  double adjustment = MBW*0.07/40.0;
  gravidUterusEnergy*=adjustment;
  return gravidUterusEnergy;
}

/**
 * Calculate Foetus Protein energy in MJ. Nice to know: why those default values and where does the fomula come from?
 */
double cattle::CalcFoetusProtein(double offset)  //weight in grams
{
  double foetusProtein = 1000.0 * exp(5.358 - 15.229 * exp(-0.00538*(daysFromConception + offset)));
  double adjustment = MBW*0.07/40.0;
  foetusProtein*=adjustment;
  return foetusProtein;
}

/**
 * Calculate foetus energy in MJ. Nice to know: why those default values and where does the fomula come from?
 */
double cattle::CalcFoetusEnergy(double offset)
{
	double FoetusEnergy = exp(11.946 - 16.595 * exp(-0.00334*(daysFromConception + offset)));
   double adjustment = MBW*0.07/40.0;
   FoetusEnergy*=adjustment;
   return FoetusEnergy;
}

/**
 * Calculate fat in foetus (g) . Nice to know: why those default values and where does the fomula come from?
 */
double cattle::CalcFoetusFat(double offset)
{
	double energy = CalcFoetusEnergy(offset);
   double fat = 1000.0 * (energy - CalcFoetusProtein(offset) * 23.6/1000.0)/39.3;
   return fat;
}

/**
 * Calculate fat in gravid uterus (g). Nice to know: why those default values and where does the fomula come from?
 */
double cattle::CalcGravidUterusFat(double offset)
{
	double energy = CalcGravidUterusEnergy(offset);
   double fat = 1000.0 * (energy - CalcGravidUterusProtein(offset) * 23.6/1000.0)/39.3;
   return fat;
}

/**
 * Get carbon in cattle. Nice to know: why those default values and where does the fomula come from?
 */
double cattle::GetCattleC()
{
 double carbon = GetCowC();
 if (calf)
 	carbon+=calf->GetCowC();
 return Number * carbon;
}

/**
 * Get carbon in cow. Nice to know: why those default values and where does the fomula come from?
 */
double cattle::GetCowC()
{
 double carbon = bodyFat *0.7581 + bodyProtein * 0.52;
 if (foetus)
 	carbon+=foetus->GetCowC();
 return carbon;
}

/**
 * Check all balances. Note that the balance for a suckling cow will include the calf.
 * The calf balance will be checked separately.
 */
void cattle::checkBalances(bool show)
{
	if (show)
   {
    cout << "N balance for " << GetName() << endl;
    cout << Nbudget;
    cout << "C balance for " << GetName() << endl;
    cout << Cbudget;
   }
	double NRemaining=0;
	double CRemaining=0;
   NRemaining=GetCowN();
   CRemaining=GetCowC();
   Nbudget.Balance(NRemaining);
   Cbudget.Balance(CRemaining);
   if (calf)
   	{ calf->checkBalances(show);}
}

/**
 * Get carbon in milk. Nice to know: why those default values and where does the fomula come from?
 */
double cattle::GetMilkCconc()
{
 double carbon = (milkFatConc *0.7016 + milkProteinConc * 0.533 + lactoseConc * 0.4205)/1000.0;
 return carbon;
}

/**
 * Calculate urine volume - from Bannink et al, J of Dairy Sci, 82 p1008
 */
double cattle::GetUrineVolume()
{
 double sodiumPercent = 100.0 * currentDiet->GetNa_content()/currentDiet->GetdryMatter();
 double potassiumPercent = 100.0 * currentDiet->GetK_content()/currentDiet->GetdryMatter();
 double milkVol = 0.0;
 if (milk)
 	milkVol = 1000.0 *milk->GetAmount();
 double nitrogenPercent = 100.0 * currentDiet->GetAllN().n/currentDiet->GetdryMatter();
 double urineVol = 1.3441 * DM_intake * (1.079 * sodiumPercent + 0.5380 * potassiumPercent
 							+ 0.1266 * nitrogenPercent)
                     - milkVol * (0.1216 + 0.0275 * milkProteinConc/10.0);
 return urineVol;
}

/**
 * Wean a calf
 */
void cattle::WeanCalf()
{
//   Nbudget.AddOutput(calf->GetbodyProtein()/N_to_protein());
//   Cbudget.AddOutput(GetCarbon(calf->GetbodyFat(),calf->GetbodyProtein()));
	calf = NULL;
}

/**
 * Calculate the factor used to reduce palatability in the presence of dung
 */
double cattle::GetAcceptanceFactor(feed_resource *aFeed_resource)
{
    double ret_val = 1.0;
    if (aFeed_resource->GetTime_since_dunged() > 0.0)
    {
    	ret_val = acceptanceCoefficient * aFeed_resource->GetTime_since_dunged();
    	if (ret_val>1.0)
       ret_val = 1.0;
    }
    return ret_val;
}



/**
 * Update anAnimalProduct
 */
void cattle::loadAnimalProduct(animalProduct *anAnimalProduct)
{
	double weight = Getweight();
   double N_content = GetCowN()/weight;
   anAnimalProduct->SetN_content(N_content);
   double C_content = GetCowC()/weight;
   anAnimalProduct->SetC_content(C_content);
   anAnimalProduct->SetEndWeight(weight);
   anAnimalProduct->Setamount(weight*Number);
}

/**
 * Calculated Heat Production. There are 4 scenarios: young and non-pregnant female, male, female with lactation, pregnent with no lactation
 */
double cattle::GetSensibleHeatProduction()
{
	double ret_val=0.0;
   double growthRate=growth->GetAmount();
   if ((isYoung) && (isFemale) && !(pregnant)) //female calves
   	ret_val = 6.44*pow(weight,0.7)+(13.3 * growthRate*(6.28 + 0.0188*weight))/(1-0.3*growthRate);
   if (!isFemale) //male animals
   	ret_val = 7.64*pow(weight,0.69)+(13.3 * growthRate*((23.0/(currentDiet->GetME()/1000.0))-1)) *
      							(57.27 + 0.302*weight)/(1-0.171*growthRate);
   if ((pregnant) && (lactationNumber==0))//pregnant heifers
   	ret_val = 7.64*pow(weight,0.69)+(13.3 * growthRate*((23.0/(currentDiet->GetME()/1000.0))-1)) *
      							(57.27 + 0.302*weight)/(1-0.171*growthRate) + 1.6*0.00001 * pow(daysFromConception,3);
   if ((isFemale) && (lactationNumber>0))// mature cows
   	ret_val = 5.6*pow(weight,0.75)+ 22.0 * lastMilkYield + 1.6*0.00001 * pow(daysFromConception,3);
   if (ret_val==0.0)
   	theMessage->FatalError("cattle:GetSensibleHeatProduction - could not find animal category");
   double temperature = theClimate->GetMeanTemperature();         //assumes animals experience outside temperature - could be wrong in extreme climates
   double temperatureAdjustment = 1+(4*(20-temperature))/1000.0;
   ret_val*= temperatureAdjustment;
   return ret_val;

}

//!calculate the amount of fat and protein in our an animal of a particular empty body weight
/*!
 *\param EBW empty body weight of the animal (kilograms)
 *\param fat amount of fat in the animal (kilograms)
 *\param protein amount of protein in the animal (kilograms)
 */
void cattle::GetStandardComposition(double EBW, double *fat, double *protein)
{
   //first calculate the protein at birth, valid for 40kg EBW at birth
   double birthProtein = 1000.0 * exp(5.358 - 15.229 * exp(-0.00538*gestationPeriod));
   //calculate the adjustment required to scale for mature body weight
   double birthWeight = MBW*0.07;
   double adjustment = birthWeight/40.0;
   //recalculate birthProtein for actual birth weight
   birthProtein*=adjustment;

   //next calculate the energy at birth
   double birthEnergy = exp(11.946 - 16.595 * exp(-0.00334*gestationPeriod));
   //recalculate birthEnergy for actual birth weight
   birthEnergy*=adjustment;

   //calculate birth fat
   double birthFat = 1000.0 * (birthEnergy - birthProtein * 23.6/1000.0)/39.3;
   double matureFatContent = 250.0; //fat content of empty body weight at maturity (p42, SCA (1990))
   double matureFat =  matureFatContent * MBW;
   *fat =  (birthFat + ((matureFat-birthFat)/(MBW- birthWeight)) * (EBW - birthWeight))/1000.0;

   double matureProtein = 216 * (MBW - matureFat/1000.0);
   *protein = (birthProtein + ((matureProtein - birthProtein)/(MBW - birthWeight)) * (EBW - birthWeight))/1000.0;
}
