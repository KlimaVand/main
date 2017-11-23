
// Class: uk_cattle

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include <common.h>
#include <base.h>
#include "uk_cattle.h"
#include <feed_resource.h>
#include <NixCbits.h>
#include <output.h>  // only needed for logging to debug file

// Functions for class uk_cattle

/****************************************************************************\
Constructor with arguments
\****************************************************************************/
uk_cattle::uk_cattle(const char *aName,const int aIndex,const base * aOwner):
     cattle(aName,aIndex,aOwner)
{
   breed = 0;
   birthWeightFactor = 0.07; // SCA p45
   partitionSystem=1;
   bodyEnergy = 0.0;
   uterusProtein=0.0;
   uterusEnergy=0.0;
}

/****************************************************************************\
  Copy Constructor - is shallow
\****************************************************************************/
uk_cattle::uk_cattle(const uk_cattle& anAnimal)
   : cattle(anAnimal)
{
   breed = anAnimal.breed;
   bodyEnergy = anAnimal.bodyEnergy;
   weightChange = anAnimal.weightChange;
   birthWeightFactor = anAnimal.birthWeightFactor; // SCA p45
   partitionSystem=anAnimal.partitionSystem;
   uterusWeight		=anAnimal.uterusWeight;
   uterusEnergy		= anAnimal.uterusEnergy; 	//track uterus weight (has to be lost at birth) (kg)
   uterusProtein		=anAnimal.uterusProtein; 	//track uterus weight (has to be lost at birth) (kg)
   a						= anAnimal.a;
   b						= anAnimal.b;
   c						= anAnimal.c;
   Gzero					= anAnimal.Gzero;
   lactating 			= anAnimal.lactating;
   pregnant 			= anAnimal.pregnant;
   calf 					= anAnimal.calf;
   potPeakMilkYield		= anAnimal.potPeakMilkYield;
   lactose           = anAnimal.lactose;
   milkProteinConc       = anAnimal.milkProteinConc;
   milkfat           = anAnimal.milkfat;
   milk		= NULL;
   foetus= NULL;
}

/****************************************************************************\
Destructor
\****************************************************************************/
uk_cattle::~uk_cattle()
{
};

//  Function: 	ManureProduction
void uk_cattle::ManureProduction( fstream* fileStream)
{
	theMessage->FatalError("Function in uk_cattle not implemented");
  *fileStream << ".LIQM    0.0\n"; //dummy line which is never reached - stops a warning
}

/****************************************************************************\
Reads parameters when initialising an animal at start of simulation
\****************************************************************************/
void uk_cattle::ReadParameters(fstream * afile)
{
   Setfile(afile);
	cattle::ReadParameters(afile);
   SetCritical();
	Setcur_pos(0);
   string namestr;
	namestr=(string) "Animal";
   FindSection(namestr.c_str(),code);  // find the correct place in the input file
   GetParameter("amountFluid",&amountFluid );
   GetParameter("amountSolid",&amountSolid );
   GetParameter("heatProduction",&heatProduction );
   GetParameter("Alias",&Alias);
   GetParameter("potPeakMilkYield",&potPeakMilkYield);
   GetParameter("lactose",&lactose);
   GetParameter("milkprotein",&milkProteinConc);
   GetParameter("milkfat",&milkfat);
   GetParameter("a",&a);
   GetParameter("b",&b);
   GetParameter("c",&c);
   GetParameter("Gzero",&Gzero);
   UnsetCritical();
    Setfile(NULL);
}


void uk_cattle::EndBudget(bool show)
{
	if (show)
   {
    cout << "Balance for " << GetName() << endl;
    cout << Nbudget;
   }
	double NRemaining=0;
   NRemaining=bodyProtein/N_to_protein();
   if (foetus){ NRemaining+=((uk_cattle *)foetus)->GetbodyProtein()/N_to_protein();}
   double balance =Nbudget.GetInput()-(Nbudget.GetOutput()+ NRemaining);
	if (abs(balance)>0.01) theMessage->FatalError("uk_cattle: N budget error");
   Nbudget.Balance(NRemaining);
   if (calf)
      calf->checkBalances(show);
}

/****************************************************************************\
Calculate intake
\****************************************************************************/
void uk_cattle::intake()
{
   DM_intake=0.0;
   ME_intake = 0.0;
   double DMFromGrazing = 0.0;
   double rel_capacity=1;
	if (isSuckling)  //only if is a calf and is with mother
   {
      rel_capacity-=SucklingCalfMilkIntake();
      feedPlanList=Getmother()->GetfeedPlanList();  //suckling calves have access to same diet as mother
   }

	int counter=0,Hi=0;
	linkList <feed_resource> * ptrlist = new linkList <feed_resource>;
   DoSelection(ptrlist,&Hi);
   if (fieldList==NULL)
   	{theMessage->FatalError("uk_cattle: no fieldList initialised");}
   for (int i=0;i<fieldList->NumOfNodes(); i++) {fieldOfftakeList[i]= 0.0;};

   //  {work out potential intake}
   double pot_intake=GetPotentialIntake();
   double tonnes_per_ha=0;
 	double tonnes_per_ha_this_resource,rel_avail,rel_intake,DM_intake_this_resource;
   double deficit=0.0; //used to record difference between feed intake capacity and feed availability
//   fieldOfftakeList->ForgetAll();
   feed_resource *aFeed_resource;
   for (counter=Hi;counter>=0;counter--)
   {
//      aFeed_resource=(feed_resource*) ptrlist[counter];
      aFeed_resource= ptrlist->ElementAtNumber(counter);
      if (rel_capacity>0.001)  //avoids rounding errors
      {
         //handle a supplement
         if (aFeed_resource->GetIsSupplement())
         {
            // calc relative intake from this resource
            rel_intake=rel_capacity *  Get_quality_factor(aFeed_resource->GetOMD());
            // calc potential intake from this resource
            DM_intake_this_resource=rel_intake * pot_intake;  //max possible intake of this supplement
            if ((aFeed_resource->GetAmount() * aFeed_resource->GetdryMatter() * 1000.0) < DM_intake_this_resource) //if ration < potential intake
                  DM_intake_this_resource = aFeed_resource->GetAmount() * aFeed_resource->GetdryMatter() * 1000.0;
            rel_capacity -= DM_intake_this_resource/(Get_quality_factor(aFeed_resource->GetOMD())*pot_intake);
            aFeed_resource->Setamount(DM_intake_this_resource/(aFeed_resource->GetdryMatter() * 1000.0)); //convert from DM to FW basis
            *aFeed_resource * GetNumber();     //adjust for animal number prior to output of demand
            theProducts->SubtractProduct(aFeed_resource);                     // subtract feed from storage
            *aFeed_resource * (1/GetNumber());     //adjust back
         }
         else  //handle a grazed feed resource
         {
            if (hoursHoused==24.0)
               {theMessage->WarningWithDisplay("uk_cattle: intake - grazed feed in diet but animal housed all day");}
            tonnes_per_ha_this_resource=aFeed_resource->GetAmount() * aFeed_resource->GetdryMatter()/
                                                aFeed_resource->Getarea_occupied();
            if (tonnes_per_ha_this_resource>0.0)
            {
            // calc relative availability of this resource
               rel_avail=(1-hoursHoused/24.0) * Get_availability_factor(tonnes_per_ha_this_resource);
            // calc relative intake from this resource
               rel_intake=rel_capacity *  Get_quality_factor(aFeed_resource->GetOMD()) * rel_avail;
            // calc intake from this resource
               DM_intake_this_resource=rel_intake * pot_intake;
               DMFromGrazing+=DM_intake_this_resource;
               aFeed_resource->Setamount(DM_intake_this_resource/(aFeed_resource->GetdryMatter() * 1000.0));  //convert from kg DM to tonnes FW
               if (aFeed_resource->GetAmount()>1000.0)
                  theMessage->FatalError("uk_cattle: error in feed resource diet");
	            *aFeed_resource * GetNumber();     //adjust for animal number prior to output of demand
               double NGrazed = aFeed_resource->GetAmount() * aFeed_resource->GetAllN().n * 1000.0;
               ((patch *)aFeed_resource ->GetOwner())->SetGrazed(0.5,DM_intake_this_resource);
               theOutput->AddIndicator(environmentalIndicator,"33.04 N grazed by cattle","kg N",NGrazed);
	            *aFeed_resource * (1/GetNumber());     //adjust back
               // go through field list and find to which field this patch belongs then add the offtake
               for (int i=0;i<fieldList->NumOfNodes(); i++)
               {
                field* aField = (field*) fieldList->ElementAtNumber(i);
                patch* aPatch = (patch *)aFeed_resource ->GetOwner();
                if (aPatch->GetOwner() == aField)
                {
                    {fieldOfftakeList[i]+= DM_intake_this_resource * GetNumber();}
                }
               }
               rel_capacity-=rel_intake;
            }
            else
            {
               DM_intake_this_resource=0.0;
               aFeed_resource->Setamount(0.0);
            }
         }
         DM_intake+=DM_intake_this_resource;
         // add this resource to the current diet
         if (aFeed_resource->GetAmount()>0.0)
                  *currentDiet = *currentDiet + *aFeed_resource;
         ME_intake+= DM_intake_this_resource * (aFeed_resource->GetME()/aFeed_resource->GetdryMatter());
	   }
      if ((counter==0)&&(rel_capacity>0.001))   // is the last feed available and still have capacity for more
      {
      	deficit = rel_capacity *  pot_intake;       //check!
      }
      delete aFeed_resource;
      if (rel_capacity<0.0) rel_capacity=0.0;
   }// end count through grazing resources

   if ((ME_intake<0.00001)||(ME_intake>1000.0))
   	theMessage->FatalError("uk_cattle: error in current diet");

	double totalNinput= currentDiet->GetAmount() * 1000.0 * currentDiet->GetorgN_content().n;

   if (isYoung)    //output BEFORE adding milk (is internal to livestock)
   {theOutput->AddIndicator(environmentalIndicator,"33.01 N in feed for young cattle","kg N",Number * totalNinput);}
   else
   {theOutput->AddIndicator(environmentalIndicator,"33.02 N in feed for adult cattle","kg N",Number * totalNinput);}
   Nbudget.AddInput(currentDiet->GetAmount() * 1000.0 * currentDiet->GetAllN().n);

   if (isSuckling)
   {
   	totalNinput+=milk->GetAmount() * 1000.0 * milk->GetorgN_content().n;
	   Nbudget.AddInput(milk->GetAmount() * 1000.0 * milk->GetorgN_content().n);
      feedPlanList=NULL;  //otherwise, if calf is culled, feedPlanList will be deleted and mother will have no diet
   }
	double feedNPerc= 100.0 *totalNinput/DM_intake;
   if (feedNPerc<1.0)
   	theMessage->WarningWithDisplay("uk_cattle: N in diet <1.0%");
   // if no grazing then house animals (otherwise will try an partition manure to fields)
   if (DMFromGrazing == 0)
   	hoursHoused = 24.0;

   					// used for debugging tempfeedList->ForgetAll();  delete tempfeedList;
}

/****************************************************************************\
Calculate quality parameter for SCA 1990 intake equation
\****************************************************************************/
double uk_cattle::Get_quality_factor( double digestibility)
{
// double factor=1 - 1.7*(0.8 - digestibility) ;
	double coeff = 1.7;
 	double factor=1 - coeff * (0.8 - digestibility);
 	return factor;
}


/****************************************************************************\
Calculate Normal weight (ie LW adjusted to standard condition)
\****************************************************************************/
double  uk_cattle::Get_norm_weight()
{
	 double ret_val;
	 double k, months, birth_w;

	 birth_w = MBW * birthWeightFactor;
	 k = 0.47 / pow(MBW, 0.27);
	 ret_val =  MBW - (MBW - birth_w) * exp(-k *  age * 12/theTime.daysInYear(0));
	 return ret_val;
} /* Get_norm_weight */


/****************************************************************************\
Calculate current size of animal (adjusted for CS), relative to MBW
\****************************************************************************/
double  uk_cattle::relsiz()
{
  double ret_val;
  ret_val = Get_norm_weight() / MBW;
  if (ret_val > 1.0) ret_val = 1.0;

  return ret_val;
} /* relsiz */

/****************************************************************************\
Calculate potential intake from SCA 1990 intake equation
\****************************************************************************/
double uk_cattle::GetPotentialIntake()
{
// double ret_val = 0.024 * MBW * relsiz() * (1.7-relsiz());
 double ret_val;
 double s = 1.0;
 if (age < 60)
 	s = 1.0/(1.0 + exp(0.2*(60-age)));
 ret_val = s * (0.020 * MBW * relsiz() * (1.7-relsiz()));
 return ret_val;
};


/****************************************************************************\
Calculate maintenance energy (MJ ME per day)
\****************************************************************************/
double uk_cattle::calc_energy_maint()
{
  double dum, ret_val, age_in_yrs, maturity_factor;

	age_in_yrs = age / theTime.daysInYear(0);
	// following line ensures age factor has maximum value of 0.84
	if (age_in_yrs > 6.0) age_in_yrs = 6.0;
	dum = eff_energy_main();
   if (isFemale)
   {
//for B.taurus, females
	ret_val = (1.4 * 1.0 * 0.26 * pow(weight, 0.75) *
					exp(age_in_yrs * -0.03)/ dum)
					+  ME_intake * 0.09;
//					+ (Egraze(All_r, Sel_r, num_r) / dum);
	}
   else
   {
	ret_val = (1.4 * 1.15 * 0.26 * pow(weight, 0.75) *
					exp(age_in_yrs * -0.03)/ dum)
					+  ME_intake * 0.09;
   }

	return ret_val;
}


/*
Calculate EFFICIENCY OF USE OF ENERGY FOR MAINTENANCE
NJH 20.06.00
*/

double  uk_cattle::eff_energy_main()
{
  double ret_val;
  double prop_solid=0;
  double conc_ME_solid_diet = 0.0;
  double ME_milk = 0.0;
  double ME_solid = 0.0;
  double DM_solid = 0.0;
  if (isSuckling)
  {
   ME_milk = milk->GetAmount() * milk->GetME();
   ME_solid = ME_intake - ME_milk;
   DM_solid = DM_intake - milk->GetAmount() * milk->GetdryMatter();
  }
  else
  {
  	ME_solid = ME_intake;
   DM_solid = DM_intake;
  }

  if (DM_solid>0.0)
  		conc_ME_solid_diet = ME_solid/DM_solid;
  ret_val = (ME_milk/ME_intake) * 0.85 +  (ME_solid/ME_intake) * (conc_ME_solid_diet * 0.02 + 0.5);
  return ret_val;
}


/****************************************************************************\
Calculate efficiency of use of ME for growth
\****************************************************************************/
double uk_cattle::eff_energy_growth()
{
 double ret_val;
  double prop_solid=0;
  double conc_ME_solid_diet = 0.0;
  double ME_milk = 0.0;
  double ME_solid = 0.0;
  double DM_solid = 0.0;
  if (isSuckling)
  {
   ME_milk = milk->GetAmount() * milk->GetME();
   ME_solid = ME_intake - ME_milk;
   DM_solid = DM_intake - milk->GetAmount() * milk->GetdryMatter();
  }
  else
  {
  	ME_solid = ME_intake;
   DM_solid = DM_intake;
  }

  if (DM_solid>0.0)
  		conc_ME_solid_diet = ME_solid/DM_solid;
  ret_val = (ME_milk/ME_intake) * 0.70 +  (ME_solid/ME_intake) * (conc_ME_solid_diet * 0.043);
 return ret_val;
}


/*
Return energy in EBW change (in MJ of ME)
*/
double  uk_cattle::MEinEBWchange()
{   //from SCA p 43, assuming zero rate of change of body weight
 double ret_val;
 double P = weight/MBW;
 ret_val = 4.7 + 22.3/(1+exp(-6.0*(P-0.4)));
 return ret_val;
}


/****************************************************************************\
Calculate change in weight for a given amount of ME available for growth
\****************************************************************************/
double uk_cattle::GetWeightChange(double avail_ME)
{
	double ret_val;
   if (avail_ME<0)
   {
    double effFactor = 0.8;  // from SCA p26
    ret_val =avail_ME/(effFactor * MEinEBWchange());
   }
   else	ret_val=avail_ME * eff_energy_growth()/MEinEBWchange();
	return ret_val;
}


/****************************************************************************\
Returns the amount of N required/released (in g) by a 1 kg change in empty body weight
\****************************************************************************/
double  uk_cattle::NinEBWchange()
{   //from SCA p 103, assuming zero rate of change of body weight
 double ret_val;
 double P = weight/MBW;
 double proteinReq = 220 - 148/(1+exp(-6.0*(P-0.4)));
 ret_val = proteinReq/N_to_protein();
 return ret_val;
}

/****************************************************************************\
Partitions energy and protein
\****************************************************************************/
void uk_cattle::Partition(double *dungN, double *urineN)
{
//note that daily amounts of N are in g but body protein is in kg
	double rumenNsurplus, availableN, heat;
   *urineN=0.0;
   heat=0.0;
	double totalNinput= currentDiet->GetAmount() * 1000000.0 * currentDiet->GetorgN_content().n; //in g
   if (isSuckling)
   {
		totalNinput+=milk->GetAmount() * 1000000.0 * milk->GetorgN_content().n; //in g
   }
	ProteinDigestion(&availableN);
   //sort out protein and energy maintenance
   double maintenanceN = CalcMaintenanceN();
   if (availableN<maintenanceN)  //no protein remobilisation allowed
      theMessage->FatalError(GetAlias().c_str()," uk_cattle: insufficient protein available for maintenance");
   else availableN-=maintenanceN;
   double emain = calc_energy_maint();
   double avail_ME = ME_intake - emain;

   double pregnancyN=DoPregnancy(&avail_ME, &availableN);

   if (avail_ME<0.0)  //remobilise body reserves
   {
		weightChange=GetWeightChange(avail_ME);
      weight+=weightChange;  //weight change is negative here
      if (weight<0.0)
       theMessage->FatalError("uk_cattle: lost too much weight - is now <zero!");

      bodyEnergy+=weightChange * MEinEBWchange();    //weight change is negative
      double remobilisedN= -weightChange * NinEBWchange();
      bodyProtein-=remobilisedN/1000.0;
	   heat+=ME_intake - weightChange * MEinEBWchange();  //weight change is negative
      *urineN=availableN + remobilisedN;  //assumes that no protein is made available - all is used for energy
   }

   double milkN = DoLactation(&avail_ME, &availableN);     //milkN is in grams

   double growthN=0.0;
   if (avail_ME>0.0)
   {
		weightChange=GetWeightChange(avail_ME); //start by assuming growth limited by energy
	   growthN = weightChange * NinEBWchange();
	   double growthNDemand = growthN/0.59;  //0.59 comes from TCOORN report p 799
		if (availableN<growthNDemand)   //then growth is restricted by protein
      {
         growthNDemand = availableN;
      	growthN = availableN * 0.59;
         weightChange=growthN/NinEBWchange();
      }
   // this is a fudge to stop the animal growing enormous
      double maxWeight = 1.0 * Get_norm_weight();
      if ((weight + weightChange) > maxWeight)
      {
         weightChange = maxWeight - weight;
         growthN = weightChange * NinEBWchange();
      }
      bodyEnergy+=weightChange*MEinEBWchange();
      bodyProtein+=growthN * N_to_protein()/1000.0;
      weight+= weightChange;
      heat+=ME_intake - weightChange*MEinEBWchange();
      *urineN = totalNinput - (*dungN + growthN + pregnancyN + milkN);
	}

   if (isSuckling)     // finished with milk so delete it
      delete milk;

   //if rumenNsurplus is negative then it has been used for growth, milk etc
   //check here to see if enough N was actually available to be recycled
   if (*urineN<0.0)
   {
   	theMessage->FatalError("uk_cattle: urine N has gone negative!");
   }
/*   if (rumenNsurplus<0.0) //if rumenNsurplus is negative, check to see if enough N is available to be recycled
//this currently creates an error in the farm N balance
   {
    	if (abs(rumenNsurplus)>*urineN)    //not enough N for recycling - force feed urea but issue warning
      {
      	double deficit= abs(rumenNsurplus)-*urineN;
        	feed_resource * aFeed_resource = new feed_resource();
	      aFeed_resource->Setname("UREA");
   	   theProducts->GiveProductInformation(aFeed_resource);
         double amountRequired = deficit/(aFeed_resource->GetAllN().n*1000000.0);
         aFeed_resource->Setamount(amountRequired);
         theProducts->SubtractProduct(aFeed_resource); // subtract feed from storage
			*currentDiet = *currentDiet + *aFeed_resource;
         delete aFeed_resource;
         if (isYoung)
         {theOutput->AddIndicator(environmentalIndicator,"21.30 N in feed for young cattle","kg N",Number * deficit/1000.0);}
         else
         {theOutput->AddIndicator(environmentalIndicator,"21.31 N in feed for adult cattle","kg N",Number * deficit/1000.0);}
//         theMessage->WarningWithDisplay("cattle: insufficient N available for recycling - urea force-fed!");
         theMessage->Warning("cattle: insufficient N available for recycling - urea force-fed!");
//			    theMessage->FatalError(GetAlias().c_str()," cattle: insufficient N available for recycling");
      }
   } */
		if (weight<0.0)
      theMessage->FatalError(GetAlias().c_str()," uk_cattle: weight has fallen below zero");
}


/****************************************************************************\
Calculate potential intake from SCA 1990 intake equation
\****************************************************************************/
double uk_cattle::GetPotentialMilkIntake()
{
 double ret_val;
 ret_val = pow((MBW * relsiz()),0.75) * (0.42 + 0.58*exp(-0.036 * age));
 return ret_val;
};


/****************************************************************************\
Remobilise body protein (only in an emergency!) NJH March 2001
\****************************************************************************/
/*double uk_cattle::GetCattleN()
{
 return bodyProtein/N_to_protein();
};
  */


/*feed_resource * cattle::GetOutputResource(feed_resource * aFeedResource)
{
	feed_resource * anOutputResource = new feed_resource(*aFeedResource);
   return anOutputResource;
} */



/****************************************************************************\
Used to calculate potential intake of a given feedstuff (e.g. used to determine silage requirement over winter
\****************************************************************************/
double uk_cattle::GetPotentialIntake(double digestibility)
{
 double potentialIntake;
 potentialIntake = GetPotentialIntake() * Get_quality_factor(digestibility);
 // equation from SCA p 213
 if (lactating)
 {
  double lactationFactor=0.0;
  lactationFactor = 1.0 + 0.0024 * pow((daysFromParturition+15),1.7)
 													*exp(-0.021*(daysFromParturition+15));
                                       //note hack on dayfrompart NJH ??
  potentialIntake *= lactationFactor;
 }
 return potentialIntake;
};

/*
efficiency of use of ME for milk production
*/
double uk_cattle::eff_energy_lactation()
{
 double effFactor = 0.02 * ME_intake/DM_intake + 0.4;
 return effFactor;
}

double uk_cattle::GetPotentialMilkYield()
{
   a = potPeakMilkYield * 1.156;
	double potentialMilkYield= a*(exp(-exp(Gzero-b*daysFromParturition)))*exp(-c*daysFromParturition);
	return potentialMilkYield;
}

/****************************************************************************\
Checks to see if foetus is at term and if so, produces a calf
\****************************************************************************/
//cattle* cattle::CheckBirth()
uk_cattle* uk_cattle::CheckBirth()
{
   if ((pregnant)&&(daysFromConception>=GetgestationPeriod()))
   {
      weight-=(uterusWeight + foetus->Getweight());
      foetus->Setage(0);
      foetus->SetisFoetus(false);
      calf = foetus;
      foetus = NULL;
      calf->SetisYoung(true);
      calf->SetisSuckling(true);
      bodyEnergy-=uterusEnergy;
      uterusEnergy=0.0;
      bodyProtein-=uterusProtein;
		theOutput->AddIndicator(environmentalIndicator,"33.40 N in dairy cattle waste","kg N",Number * uterusProtein/N_to_protein());
      Nbudget.AddOutput(uterusProtein/N_to_protein());
      uterusProtein=0.0;
      Nbudget.AddOutput(((uk_cattle *)calf)->GetbodyProtein()/N_to_protein());
      lactating = true;
	   lactationNumber++;
      pregnant = false;
//      calf->SetfeedPlanList(NULL);	// calf gets same diet choice as mother
      calf->SetfieldList(fieldList);	//calf has access to same fields as mother
      calf->SetAlias("CALF");
      calf->SetStableSection(GetStableSection());
      ((uk_cattle *)calf)->SetFoetus(NULL);
      calf->SetNumber(Number);
      return (uk_cattle *) calf;
   }
   else return NULL;

}

double uk_cattle::DoPregnancy(double *avail_ME, double *availableN)
{
  	double pregnancyNDemand = 0.0;
   if (pregnant)
   {
	   double pregnancyEnergyDemand=0.0;
      double newFoetusEnergy = exp(11.946 - 16.595 * exp(-0.00334*(daysFromConception+1)))-exp(11.946 - 16.595 * exp(-0.00334*daysFromConception));
      double newGravidUterusEnergy= exp(349.222 -  349.164 * exp(-0.0000576*(daysFromConception+1)))-exp(349.222 -  349.164 * exp(-0.0000576*daysFromConception));
      double newUterusEnergy = newGravidUterusEnergy - newFoetusEnergy;
      // scale for assumed 40kg calf, as suggested in SCA p 39
      double adjustment = MBW*birthWeightFactor/40.0;
      newFoetusEnergy *= adjustment;
      ((uk_cattle *)foetus)->SetbodyEnergy(((uk_cattle *)foetus)->GetbodyEnergy() + newFoetusEnergy);
      newUterusEnergy *=adjustment;
      bodyEnergy+=newUterusEnergy;
      uterusEnergy+=newUterusEnergy;
      double newUterus = GetWeightChange(newUterusEnergy);  //note that GetWeightChange will update weight
      uterusWeight+=newUterus;
      double newFoetus = newFoetusEnergy/5.8;  //estimate of MJ/kg EBW of foetus from energy at term/weight at term
      foetus->Setweight(foetus->Getweight()+newFoetus);
      pregnancyEnergyDemand = (newFoetusEnergy + newUterusEnergy)/eff_energy_growth();
      weightChange=newUterus + newFoetus;

      //now protein
      double newFoetusProtein = CalcFoetusProtein(1)-CalcFoetusProtein(0);

      double newGravidUterusProtein= CalcGravidUterusProtein(1)
      											 - CalcGravidUterusProtein(0);
      double newUterusProtein = newGravidUterusProtein - newFoetusProtein;
      newFoetusProtein *=adjustment;  //scale for assumed calf size of 40 kg
      newUterusProtein *=adjustment;
      ((uk_cattle *)foetus)->AddBodyProtein(newFoetusProtein/1000.0);
      bodyProtein+= newUterusProtein/1000.0;
      uterusProtein+=newUterusProtein/1000.0;

      pregnancyNDemand=(newFoetusProtein + newUterusProtein)/(N_to_protein()* 0.85);

      *avail_ME-=pregnancyEnergyDemand;
	   if (*availableN<pregnancyNDemand)  //no protein remobilisation allowed for pregnancy
   	   theMessage->FatalError("uk_cattle: insufficient protein available for pregnancy");
		else *availableN-=pregnancyNDemand;
	}
   return  pregnancyNDemand * 0.85;
}

void uk_cattle::MakePregnant(bool isAFemale)
{
//assumes calf is same genetic makeup as mother
	foetus = new uk_cattle(*this);
	foetus->Setage(GetgestationPeriod());
	foetus->SetisFoetus(true);     //prob should collect these calls together in one routine
   foetus->Setweight(0.0);
//   foetus->Initialise(animalModel, Getcode(), 0.0, LivestockUnits, GetNumber(), true, (fstream *) CattleSuitcase->GetOutputFile());
   foetus->Initialise(Getcode(), 0.0, LivestockUnits, GetNumber(), true);
   foetus->Setmother(this);
   ((uk_cattle *)foetus)->SetFoetus(NULL);
   foetus->SetCalf(NULL);
   foetus->SetlactationNumber(0);
   foetus->SetisFemale(isAFemale);

   daysFromConception=0;
   pregnant = true;
//   lactationNumber++;
};

/****************************************************************************\
Calculate lactation
\****************************************************************************/
double uk_cattle::DoLactation(double *avail_ME, double *availableN)
{
   double milkN = 0.0; //milkN is in grams
	if (lactating)
   {
   	milk = new feedItem();   //deleted later in this routine (if no calf), otherwise deleted by calf
      milk->Setname("MILK");
      theProducts->GiveProductInformation(milk);

      bool proteinLimited = false;
      bool energyLimited = false;
	   double lactationN = 0.0;
      double potentialMilkYield = GetPotentialMilkYield();
      if (calf) // see if has a calf (only has calf if calf is suckling)
      {
         // see if calf intake limits milk production
            double calfPotentialIntake = ((uk_cattle *)calf)->GetPotentialMilkIntake();
            if (calfPotentialIntake<potentialMilkYield) potentialMilkYield=calfPotentialIntake;
      }
      double NEMilk = 0.0381 * milkfat + 0.0245 * milkProteinConc + 0.0165 * lactose;   //SCA p54
      double lactationEnergy = potentialMilkYield * NEMilk/eff_energy_lactation();
      milkN = potentialMilkYield * milkProteinConc/N_to_protein();
      lactationN = milkN/0.68; //from TCOORN p809
      double proteinLimitedMilk = 9999.0, energyLimitedMilk = 9999.0, actMilkYield;
      if (*availableN < lactationN)  // see if protein is limiting
      {
         proteinLimitedMilk = *availableN * 0.68 * N_to_protein()/milkProteinConc;
      }
      if (*avail_ME<lactationEnergy)  // see if energy is limiting
      {
         energyLimitedMilk = *avail_ME * eff_energy_lactation()/NEMilk;
      }
   // if energy or protein limiting, find out which one
      if ((proteinLimitedMilk<potentialMilkYield)||(energyLimitedMilk<potentialMilkYield))
      {
         if (proteinLimitedMilk<energyLimitedMilk) proteinLimited = true;
         else energyLimited=true;
      }
      else
      {
         actMilkYield=potentialMilkYield;
         *avail_ME-=lactationEnergy;
         *availableN-=lactationN;
   	}
// if protein limiting then there is no remobilisation
      if (proteinLimited)
      {
         actMilkYield=proteinLimitedMilk;
         milkN = *availableN * 0.68;
         lactationEnergy =  actMilkYield * NEMilk/eff_energy_lactation();
         *availableN=0.0;
      }
// if energy is limiting, there SHOULD be remobilisatin but this is not implemented
      if (energyLimited)
      {
         actMilkYield=energyLimitedMilk;
         lactationEnergy = *avail_ME;
         milkN = actMilkYield * milkProteinConc/N_to_protein();
         lactationN = milkN/0.68;
         *avail_ME = 0.0;
      }

      //these following values need to be checked!!!!
      milk->SetME(NEMilk*1000.0);  //convert to ME per tonne (standard for products)
      milk->SetorgN_content(milkProteinConc/(1000.0 * N_to_protein()));
      milk->Setamount(actMilkYield/1000.0);
      if (!calf)
      {
//         theProducts->AddProduct(milk);
//		   CattleSuitcase->AddIndicator(environmentalIndicator,"Milk yield",actMilkYield);
			double value = milk->GetaPrice()->GetactualSellPrice();
         theOutput->AddIndicator(environmentalIndicator,"33.20 N in milk","kg N",Number * milkN/1000.0);
         theOutput->AddIndicator(economicIndicator,"05.55 Milk sold","Dkr",Number * milk->GetAmount() * 1000.0 * value);    //proce is in litrs
         theOutput->AddIndicator(economicIndicator,"19.40 Milk sold","litres",Number * milk->GetAmount() * 1000.0);
      	delete milk;   //should really be exported to a tank
      }
		Nbudget.AddOutput(milkN/1000.0);
   }
	return milkN;
}

double uk_cattle::CalcFoetusEnergy(double timeFromConception)
{
	double FoetusEnergy = exp(11.946 - 16.595 * exp(-0.00334*(timeFromConception)));
   return FoetusEnergy;
}

double uk_cattle::CalcFoetusWeight(double timeFromConception)
{
	double foetusWeight = CalcFoetusEnergy(timeFromConception)/5.8;  //estimate of MJ/kg EBW of foetus from energy at term/weight at term
   return foetusWeight;
}


double uk_cattle::SucklingCalfMilkIntake()
{
   	milk = ((uk_cattle*)mother)->Getmilk();
      double potMilkIntake = GetPotentialMilkIntake();
      double intakeReduction = milk->GetAmount()*1000.0/potMilkIntake;
      ME_intake= milk->GetAmount() * milk->GetME();
      DM_intake =milk->GetAmount() * milk->GetdryMatter() * 1000.0;
      return intakeReduction;
}

void uk_cattle::Createfoetus()
{
	foetus = new uk_cattle(*this);
}

double uk_cattle::CalcFoetusProtein(double daysFromConception)
{
      double foetusProtein = cattle::CalcFoetusProtein(daysFromConception);
      return foetusProtein;
}

double uk_cattle::CalcGravidUterusProtein(double daysFromConception)
{
      double gravidUterusProtein= cattle::CalcGravidUterusProtein(daysFromConception);
      return gravidUterusProtein;
}

