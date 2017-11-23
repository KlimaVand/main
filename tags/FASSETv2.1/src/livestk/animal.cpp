/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// Class: animal

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include <common.h>
#include <output.h>
#include "animal.h"
#ifdef _STANDALONE
	#include <products.h>
	#include <message.h>
#endif

// Functions for class animal

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


/****************************************************************************\
  Copy Constructor
\****************************************************************************/
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
   P_growth      = anAnimal.P_growth;
   K_growth      = anAnimal.K_growth;
   FE_need       = anAnimal.FE_need;
	P_need			= anAnimal.P_need;
	K_need			= anAnimal.K_need;
	LivestockUnits			= anAnimal.LivestockUnits;
   variableCosts = anAnimal.variableCosts;
   thisProduct		=NULL;
   Nbudget.SetNames("animal","N");
   Cbudget.SetNames("animal","C");
   Number			= anAnimal.Number;
   heatProduction = anAnimal.heatProduction;
   normNExcretion = anAnimal.normNExcretion;
#ifndef _STANDALONE
   StableSection	=NULL;
#endif
}

#ifdef _STANDALONE
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

animal* animal::clone() const
{
	animal* p = new animal(*this);
	return p;
}

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

void animal::Initialise()
{
   amountSolid  				= 0.0;
   amountFluid 				= 0.0;
   heatProduction 			= 0.0;
   normNExcretion 			= 0.0;
   N_growth      				= 0.0;
   P_growth      				= 0.0;
   K_growth      				= 0.0;
   FE_need       				= 0.0;
	P_need						= 0.0;
	K_need						= 0.0;
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

//get animal info
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
 GetParameter("normNExcretion",&normNExcretion);
 Setfile(NULL);
}

void animal::checkBalances(bool show)
{
	show=show;
	theMessage->FatalError(GetName()," animal: checkBalances - base class function should not be called");
}

/**********************************************************************
Calculate C in animal (and no, don't ask me where is comes from)
**********************************************************************/
double animal::GetCarbon(double fat, double protein)
{
 double carbon = fat *0.7581 + protein * 0.52;
 return carbon;
}


