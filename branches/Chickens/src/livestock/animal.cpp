/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// Class: animal

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "../base/common.h"

#include "../products/products.h"
#include "animal.h"
#ifdef _STANDALONE
	#include <products.h>
	#include <message.h>
#endif

// Functions for class animal
/**
 * Initialize following variable: currentfeed, manurePrDay, fluidManurePrDay, solidManurePrDay, vetExpensesPrDay, Nbudget, Cbudget
 * Also calles:
 * @see Initialise()
 * @see base(const char * aName, const int aIndex = -1, const base * aOwner = NULL);
 */
animal::animal(const char * aName, const int aIndex, const base * aOwner)
	: base(aName, aIndex, aOwner)
{
	Initialise();
   currentfeed = new feedItem("feedmix",0,this);
   manurePrDay = new manure("manurePrDay",0,this);
   fluidManurePrDay = new manure("FLUID-MANURE",0,this);
   solidManurePrDay = new manure("SOLID-MANURE",0,this);
	theProducts->GiveProductInformation(fluidManurePrDay);
	theProducts->GiveProductInformation(solidManurePrDay);
   vetExpensesPrDay = new product("VET",0,this);
   Nbudget.SetNames("animal","N");
   Cbudget.SetNames("animal","C");
}


/**
 * Make a copy of the animal instance.
 */
animal::animal(const animal& anAnimal)
   : base(anAnimal)
{
   currentfeed = new feedItem("feedmix",0,this);
   manurePrDay = new manure("manurePrDay",0,this);
   fluidManurePrDay = new manure("FLUID-MANURE",0,this);
   solidManurePrDay = new manure("SOLID-MANURE",0,this);
/*   if (anAnimal.thePartitioning)
	   thePartitioning = new s_part(*anAnimal.thePartitioning);
   else
      thePartitioning=NULL;*/
	theProducts->GiveProductInformation(fluidManurePrDay);
	theProducts->GiveProductInformation(solidManurePrDay);
   vetExpensesPrDay = new product("VET",0,this);
	theProducts->GiveProductInformation(vetExpensesPrDay);

   amountFluid = anAnimal.amountFluid;
   amountSolid  = anAnimal.amountSolid;
   N_growth      = anAnimal.N_growth;
   C_growth      = anAnimal.C_growth;
   P_growth      = anAnimal.P_growth;
   K_growth      = anAnimal.K_growth;
   FE_need       = anAnimal.FE_need;


	LivestockUnits			= anAnimal.LivestockUnits;
   variableCosts = anAnimal.variableCosts;
   thisProduct		=NULL;
   Nbudget.SetNames("animal","N");
   Cbudget.SetNames("animal","C");
   Number			= anAnimal.Number;
   heatProduction = anAnimal.heatProduction;

#ifndef _STANDALONE
   StableSection	=NULL;
#endif
}

#ifdef _STANDALONE
/**
 * A constructor. Only used if _STANDALONE is defined
 */
animal::animal ()
{
	Initialise();
   currentfeed = new feedItem("feedmix",0,this);
   manurePrDay = new manure("manurePrDay",0,this);
   fluidManurePrDay = new manure("FLUID-MANURE",0,this);
   solidManurePrDay = new manure("SOLID-MANURE",0,this);
	theProducts->GiveProductInformation(fluidManurePrDay);
	theProducts->GiveProductInformation(solidManurePrDay);
   vetExpensesPrDay = new product("VET",0,this);
	theProducts->GiveProductInformation(vetExpensesPrDay);
   Nbudget.SetNames("animal","N");
   Cbudget.SetNames("animal","C");
};
#endif
/**
 * make a clone of current instance of Animal
 */
animal* animal::clone() const
{
	animal* p = new animal(*this);
	return p;
}

/**
 * Deleting of pointers
 */
animal::~animal()
{
	delete currentfeed;
   delete manurePrDay;
   delete fluidManurePrDay;
   delete solidManurePrDay;
#ifndef _STANDALONE
   delete vetExpensesPrDay;
#endif
//   delete thePartitioning;
}
/**
 * Initialize of variables
 */
void animal::Initialise()
{
   amountSolid  				= 0.0;
   amountFluid 				= 0.0;
   heatProduction 			= 0.0;

   N_growth      				= 0.0;
   C_growth      				= 0.0;
   P_growth      				= 0.0;
   K_growth      				= 0.0;
   FE_need       				= 0.0;


   Number						=0.0;
   LivestockUnits 			= 0.0;
   variableCosts 				= 0.0;
#ifndef _STANDALONE
   StableSection				= NULL;
#endif
   currentfeed 				= NULL;
   manurePrDay					= NULL;
   fluidManurePrDay 			= NULL;
   solidManurePrDay			= NULL;
   vetExpensesPrDay			= NULL;
   thisProduct					= NULL;
}

void animal::DailyUpdate()
{
 fluidManurePrDay->Setamount(amountFluid/1000.0);      //amount per day in t
 solidManurePrDay->Setamount(amountSolid/1000.0);      //amount per day in t
 *vetExpensesPrDay * Number;
 theProducts->SubtractProduct(vetExpensesPrDay);                 // cost of vetenarian
}

/**
 * Initialize of variables from files
 * @param file need to point a the file
 */
void animal::ReadParameters (fstream * file)
{
 Setfile(file);
 Setcur_pos(0);
 SetCritical();
 FindSection(GetName(),Index);  // find the correct place in the input file
 GetParameter("amountFluid",&amountFluid);
 GetParameter("amountSolid",&amountSolid);
 GetParameter("variableCosts",&variableCosts);
 GetParameter("heatProduction",&heatProduction);
 UnsetCritical();
 GetParameter("Alias",&Alias);

 Setfile(NULL);
}
/**
 * Is giving a Fatal Error:"name"  animal: checkBalances - base class function should not be called"
 * Should not be used Fasset at this point
 */
void animal::checkBalances(bool show)
{
	show=show;
	theMessage->FatalError(GetName()," animal: checkBalances - base class function should not be called");
}


/**
 * Calculate Carbon. Using this formular fat *0.7581 + protein * 0.52;
 * Nice to know: why those numbers?
 * @param fat amount of fat
 * @param protein amount of protein
 */
double animal::GetCarbon(double fat, double protein)
{
 double carbon = fat *0.7581 + protein * 0.52;
 return carbon;
}


