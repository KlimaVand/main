/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: dk_cattle

//////////////////////////.cpp file/////////////////////////////////////////////////////
#include "../base/common.h"
#include "../base/base.h"
#include "dkCattle.h"
#include "../products/feedResource.h"
#include "../base/nixcbits.h"

#include "../data/outputData.h"
#ifdef _STANDALONE
	#include <message.h>
   #include "../products/products.h"
#endif


#ifdef _STANDALONE
/****************************************************************************\
Constructor with no arguments
\****************************************************************************/
dk_cattle::dk_cattle():
     cattle()
{
	cattle::	Initialise();
   herdMilkYieldConstant			= 0.0;  //bes�tningsydelsen kg EKM
   q0 = q1 								= 0.0;
   lactationEnergyFudgeFactor		= 1.0;
}
#endif


/*
 * Constructor with arguments
 */
dk_cattle::dk_cattle(const char *aName,const int aIndex,const base * aOwner):
     cattle(aName,aIndex,aOwner)
{
   herdMilkYieldConstant			= 0.0;  //bes�tningsydelsen kg EKM
   q0 = q1 								= 0.0;
   lactationEnergyFudgeFactor		= 1.0;
}

/**
 *   Copy Constructor - is shallow
 */
dk_cattle::dk_cattle(const dk_cattle& anAnimal)
   : cattle(anAnimal)
{
   herdMilkYieldConstant=anAnimal.herdMilkYieldConstant;  //bes�tningsydelsen kg EKM
   q0 						= anAnimal.q0;
   q1 						= anAnimal.q1;
   lactationEnergyFudgeFactor		= anAnimal.lactationEnergyFudgeFactor;
}

/**
 * Destructor
 */
dk_cattle::~dk_cattle()
{
};

/**
 * Called daily
*/
void dk_cattle::DailyUpdate()
 {
/*   double milkYieldCapacity = GetMilkYieldCapacity();
   double potentialMilkYield=GetPotentialMilkYield(daysFromParturition);
   double potentialGrowth=GetPotentialGrowth();*/
	cattle::DailyUpdate();
}

/**
 * Reads parameters when initialising an animal at start of simulation
 */
void dk_cattle::ReadParameters(fstream * afile)
{
	cattle::ReadParameters(afile);
   Setfile(afile);
   SetCritical();
	Setcur_pos(0);
   string namestr;
	namestr=(string) "Animal";
   FindSection(namestr.c_str(),code);  // find the correct place in the input file
   GetParameter("herdMilkYieldConstant",&herdMilkYieldConstant );
   GetParameter("lactationEnergyFudgeFactor",&lactationEnergyFudgeFactor);
   GetParameter("q0",&q0 );
   GetParameter("q1",&q1);
   UnsetCritical();
   Setfile(NULL);
}

/**
 * Reads parameters when initialising an animal at start of simulation - used when in STANDALONE mode
 */
void dk_cattle::ReadParameters(char *filename)
{
   SetCritical();
   OpenInputFile(filename);
	ReadParameters(file);
};



/**
 * Partitions energy and protein
 */
void dk_cattle::Partition()
{
   limitation = 0;
//note that daily amounts of N are in g but body protein is in kg
   double  availableN //CK,  heat
   ;
   double avail_FE = GetavailableFodderUnits();
   urineN=0.0;
   //CK  heat=0.0;
   growth->Setamount(0.0);
	double totalNinput= currentDiet->GetAmount() * 1000000.0 * currentDiet->GetorgN_content().n; //in g
   if (isSuckling)
   {
		totalNinput+=milk->GetAmount() * 1000000.0 * milk->GetorgN_content().n; //in g
   }
	ProteinDigestion(&availableN);
   //sort out protein and energy maintenance
   double maintenanceN = CalcMaintenanceN();
   if (availableN<maintenanceN)  //protein remobilisation
   {
     double deficit = availableN-maintenanceN;
     double weightChange = deficit/NinEBWchange();
     ChangeBodyComposition(weightChange, &avail_FE, &availableN);
   }
   else availableN-=maintenanceN;

   double FEmaintenance = calc_energy_maint();
   FEDemand = FEmaintenance;
   if (avail_FE<FEmaintenance)  //no protein remobilisation allowed
   {
     double deficit = avail_FE-FEmaintenance;
     double weightChange = deficit/GetSFUPerKgGain();
     ChangeBodyComposition(weightChange, &avail_FE, &availableN);
   }
   else avail_FE-=FEmaintenance;


   double pregnancyN=DoPregnancy(&avail_FE, &availableN);
   aminoAcidDemand = (maintenanceN + pregnancyN)* N_to_protein();

   double milkN=0.0;
	double growthN=0.0;
	if (lactating)
   {
      DoLactation(&avail_FE, &availableN,&milkN);
   }
   else
   {
   	milk=NULL;
   	lastMilkYield=0.0;
   }
   if (growth->GetAmount()==0.0)   //do not calculate growth if there has been growth or remobilisation in connection with lactation
   {
      double weightChange = GetEnergyLimitedGrowth(&avail_FE);
	   FEDemand += GetSFUPerKgGain() * GetPotentialGrowth();
      limitation = 1;
	   growthN=weightChange * NinEBWchange();
      double growthNDemand = growthN/0.8;
      aminoAcidDemand+=growthNDemand * N_to_protein();
      if (availableN<growthNDemand)   //then growth is restricted by protein
      {
         growthNDemand = availableN;
         growthN = availableN * 0.8;
         weightChange=growthN/NinEBWchange();
         availableN=0.0;
         limitation = 2;
      }
/*      if (weightChange!=0.0)
      {
         growth->SetorgN_content(growthN/(1000.0 * weightChange));
         double growthProtein = growthN * N_to_protein();
         double fatChange = CalcFatInEBWchange(avail_FE,growthProtein);
         bodyProtein+=growthProtein/1000.0;
         bodyFat+=fatChange/1000.0;
         double growthCarbon = GetCarbon(fatChange,growthProtein)/1000.0;
         growth->Setfat(fatChange/(1000.0 * weightChange));
         growth->SetC_content(growthCarbon/weightChange);
         weight+=weightChange;
			growth->Setamount(weightChange);
      }*/
     ChangeBodyComposition(weightChange, &avail_FE, &availableN);
   }
   else
      growthN = growth->GetAmount() * growth->GetorgN_content().n * 1000.0;

   //if rumenNsurplus is negative then it has been used for growth, milk etc
   // NB pregnancyN is incorporated in growthN
   urineN = totalNinput - (dungN + pregnancyN + growthN + milkN);        //NB if growth is negative, urine N increases to reflect loss of remobilised N
   //check here to see if enough N was actually available to be recycled
   if (urineN<0.0)
   {
      cout << theTime << endl;
   	theMessage->FatalError("dk_cattle: urine N has gone negative!");
   }
		if (weight<0.0)
      theMessage->FatalError(GetAlias().c_str()," dk_cattle: weight has fallen below zero");
}



/*
 * Used to calculate bulk capacity (measured in fill units) - currently only valid for dairy cows?
 */
double dk_cattle::GetbulkCapacity()
{
  // double potentialIntake; //K
   double bulkCapacity;  //K0
   if (lactationNumber==0)
   {
		bulkCapacity = (4.74 * log(weight) - 18.75) * (3.125 - 0.6056*log(weight) + 0.042*log(weight)*log(weight));
      bulkCapacity*=GetCalfSolidIntakeFraction();
   }
   else
   {
      double z=(herdMilkYieldConstant - 6500)*0.0003; //??
      double weeksFromParturition = ((double)daysFromParturition)/7;  //T
   	if (lactationNumber==1)
	   	bulkCapacity = 0.784*(7.08+z) - (0.314*(7.08+z))*exp(-0.04*7*weeksFromParturition);
      else
   		bulkCapacity = (7.08+z) - (0.4*(7.08+z))*exp(-0.047*7*weeksFromParturition) - 0.0033*7*weeksFromParturition;
   }
   if (bulkCapacity<0.0)
   	bulkCapacity=0.0;   ////FUDGE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   return bulkCapacity;

};


/**
 * Used to calculate potential intake (in fill units) - currently only valid for dairy cows?
 */
double dk_cattle::GetPotentialIntake(double fillPerSFU)
{
   double potentialIntake = GetbulkCapacity(); //K0
   double a = 0.44;
   double b=1.6;
   if (fillPerSFU<0.35)
	   potentialIntake*=(a+b*fillPerSFU);
   return potentialIntake;
};
/**
 * Calculate the potential milk yield in one day. Nice to know: Where does a, b and c values come from
 * \param daysIntoLactation is how long the cow has been Lactation
 */
double dk_cattle::GetPotentialMilkYield(int daysIntoLactation) // Ykpot
{
   double a,b,c;
   double weeksFromParturition = ((double) daysIntoLactation)/7;
   if (lactationNumber==1)
   {
   	a=0.9356;      b=0.0611;      c=0.0122;
   }
   else
   {
   	a=1.311;       b=0.0995;      c=0.0256;
   }
	double potentialMilkYield= a*GetMilkYieldCapacity()* pow(weeksFromParturition,b)* exp(-c*weeksFromParturition);
	return potentialMilkYield;
}

/**
 * Checks to see if foetus is at term and if so, produces a calf
 */
dk_cattle* dk_cattle::CheckBirth()
{
   if ((pregnant)&&(daysFromConception>=GetgestationPeriod()))
   {
      double afterBirthProtein;
      double afterBirthFat;
      ((dk_cattle*) foetus)->DoBirth(&afterBirthProtein, &afterBirthFat);
      calf = foetus;
      foetus = NULL;
      weight-=calf->Getweight();     //remove calf  weight
// ignore loss of afterbirth - absorb N and C into mother
      bodyProtein+=afterBirthProtein;
      bodyFat+=afterBirthFat;
      lactating = true;
	   lactationNumber++;
      pregnant = false;
      daysFromConception=0;
      daysFromParturition=0;
      calf->SetfeedPlanList(GetfeedPlanList());	// calf gets same diet choice as mother
#ifndef _STANDALONE
      calf->SetfieldList(fieldList);	//calf has access to same fields as mother
      calf->SetStableSection(GetStableSection());
      cloneList <product> * afeedUseList = new cloneList <product>;
      calf->SetfeedUseList(afeedUseList);
      calf->Setmother(this);
#endif
      calf->SetAlias("CALF");
      calf->SetNumber(Number);
      Nbudget.AddOutput(calf->GetCowN());
      Cbudget.AddOutput(calf->GetCowC());
      return (dk_cattle *) calf;
   }
   else return NULL;
}

/**
 * Calculate demand for energy and protein for pregnancy and update pregnancy component variables
 */
double dk_cattle::DoPregnancy(double *avail_FE, double *availableN)
{
  	double pregnancyNDemand = 0.0;
   if (pregnant)
   {
   	daysFromConception++;
      double foetusFEdemand=GetFoetusEnergyRequirement();
      FEDemand += foetusFEdemand;
      *avail_FE-=foetusFEdemand;
      // scale for assumed 40kg calf, as suggested in SCA p 39

      //now protein
      double newGravidUterusProtein= 0.0;
      double newGravidUterusFat = 0.0;
      if (daysFromConception==1)   //just got pregnant
      {
         newGravidUterusProtein= CalcGravidUterusProtein(0.0);
         newGravidUterusFat = CalcGravidUterusFat(0.0);
      }
      else
      {
         newGravidUterusProtein= CalcGravidUterusProtein(0.0)
                                           -CalcGravidUterusProtein(-1.0);
         newGravidUterusFat = CalcGravidUterusFat(0.0)
                                           -CalcGravidUterusFat(-1.0);
      }
      GrowGravidUterus(newGravidUterusProtein,newGravidUterusFat);

      pregnancyNDemand=newGravidUterusProtein/(N_to_protein()* 0.85);

	   if (*availableN<pregnancyNDemand)  //no protein remobilisation allowed for pregnancy
      {
        double deficit = *availableN - pregnancyNDemand;
        double weightChange = deficit/NinEBWchange();  //deficit is negative here, so weight is lost
        ChangeBodyComposition(weightChange, avail_FE, availableN);
	   }
//	      WriteFeedingError();
  // 	   theMessage->FatalError("dk_cattle: insufficient protein available for pregnancy");
		*availableN-=pregnancyNDemand;
	}
   return  pregnancyNDemand * 0.85;
}

/**
 * Make this cow pregnant
 */
void dk_cattle::MakePregnant(bool isAFemale)
{
	if (pregnant)
   	   theMessage->Warning("dk_cattle: animal is already pregnant");
   else
   {
//assumes calf is same genetic makeup as mother
      foetus = new dk_cattle(*this);
      foetus->Setage(int(GetgestationPeriod()));
      foetus->SetisFoetus(true);     //prob should collect these calls together in one routine
      foetus->Setage(0);
      foetus->Initialise(Getcode(), 0, LivestockUnits, Number, true);
      foetus->Setmother(this);
      foetus->SetFoetus(NULL);
      foetus->SetCalf(NULL);
      foetus->SetlactationNumber(0);
      foetus->SetisFemale(isAFemale);
      foetus->Setweight(0.0);
      bodyProtein-=foetus->GetbodyProtein();  //steal foetus protein from mother
      bodyFat-=foetus->GetbodyFat();  //steal foetus fat from mother
      daysFromConception=0;
      pregnant = true;
   }
};

/**
 * Calculate lactation
 */
void dk_cattle::DoLactation(double *avail_FE, double *availableN, double *milkN)
{
   *milkN = 0.0;
   limitation = 0;
   double actMilkYield=0.0;
   double weeksFromParturition = ((double)daysFromParturition)/7;
   double energyForMilk,energyForMaxMilk,energyForGrowth,energyForMaxGrowth;
   energyForMaxMilk =    GetSFUsPerKgEKM() * GetPotentialMilkYield(int(daysFromParturition));
   energyForMaxGrowth = GetSFUPerKgGain() * GetPotentialGrowth();
   FEDemand+=energyForMaxMilk + energyForMaxGrowth;
	energyForMilk = ((*avail_FE - energyForMaxGrowth) + energyForMaxMilk/(q0+q1*weeksFromParturition))
                     /(1.0 + 1.0/(q0+q1*weeksFromParturition));
   energyForGrowth =  *avail_FE - energyForMilk;

   if (energyForGrowth<0.0)
   {
      double weightChange = GetEnergyLimitedGrowth(&energyForGrowth);
      ChangeBodyComposition(weightChange, avail_FE, availableN);
      limitation = 1;
   }
   milk = new feedItem(*(feedItem*)theProducts->GetProductElement(308));   //deleted later in this routine (if no calf), otherwise deleted by calf

   //CK bool proteinLimited = false;
   //CK  bool energyLimited = true;
   double lactationN = 0.0;
   double energyLimitedMilk = energyForMilk/GetSFUsPerKgEKM();
   if (calf) // see if has a calf (only has calf if calf is suckling)
   {
      // see if calf intake limits milk production
       double calfPotentialIntake = calf->GetPotentialMilkIntake();
       if (calfPotentialIntake<energyLimitedMilk) energyLimitedMilk=calfPotentialIntake;
   }
   *milkN = energyLimitedMilk * milkProteinConc/N_to_protein();
   actMilkYield=energyLimitedMilk;
   lactationN = energyLimitedMilk * milkProteinConc/N_to_protein();   //DK recommendations for protein for milk production
   aminoAcidDemand += energyLimitedMilk * milkProteinConc;
   if (*availableN < lactationN)  // see if protein is limiting
   {
	   double proteinLimitedMilk = *availableN * N_to_protein()/milkProteinConc;
	   //CK	proteinLimited = true;
   	//CK	energyLimited = false;
      actMilkYield=proteinLimitedMilk;
      *milkN = actMilkYield * milkProteinConc/N_to_protein();
      lactationN=*availableN;
      limitation = 2;
   }

   if (((lactationNumber==1)&&(daysFromParturition>60))||((lactationNumber>1)&&(daysFromParturition>50)))  //stops milk production increasing after peak potential production, if feed supply increases
	   if (actMilkYield>lastMilkYield)
      {
	      actMilkYield=lastMilkYield;
         *milkN = actMilkYield * milkProteinConc/N_to_protein();
         lactationN = actMilkYield * milkProteinConc/N_to_protein();
         limitation = 3;
      }
   lastMilkYield = actMilkYield;
   energyForMilk = actMilkYield * GetSFUsPerKgEKM();
   *avail_FE-= energyForMilk;
   *availableN-=lactationN;

   //these following values need to be checked!!!!
   milk->SetorgN_content(milkProteinConc/(1000.0 * N_to_protein()));
   milk->Setamount(actMilkYield/1000.0);
   milk->SetC_content(GetMilkCconc());
   if (!calf)
   {
//         theProducts->AddProduct(milk);
#ifndef _STANDALONE
      milk->SetaPrice(theProducts->GetPricePtr("MILK"));
      double value = milk->GetaPrice()->GetactualSellPrice();
      theOutput->AddIndicator(environmentalIndicator,"33.20 N in milk","kg N",Number * *milkN/1000.0);
      theOutput->AddIndicator(economicIndicator,"05.55 Milk sold","Dkr",Number * milk->GetAmount() * 1000.0 * value);    //proce is in litrs
      theOutput->AddIndicator(economicIndicator,"19.40 Milk sold","litres",Number * milk->GetAmount() * 1000.0);
#endif
   }
   Nbudget.AddOutput(*milkN/1000.0);   //milk C is handled later
}


/**
 * Returns the relative size of the animal
 */
double dk_cattle::GetSize()
{
// double size= MBW * pow((V728/MBW),((728-age)/569));
//return size;
 	 double k, birth_w,ret_val;

	 birth_w = MBW * 0.07;
	 k = 0.47 / pow(MBW, 0.27);
	 ret_val =  MBW - (MBW - birth_w) * exp(-k *  age * 12/theTime.daysInYear(0));
    return ret_val;
};

/**
 * Returns the potential (nutrient unlimited) growth
 */
double dk_cattle::GetPotentialGrowth()
{
 double n=pow(MBW,-0.27)/36;
 double potGrowth,EBW;
 EBW=weight;
 if (foetus)
    EBW-=GetFoetusWeight(0.0)/1000.0;
 potGrowth=-n*EBW*log(EBW/MBW);
 return potGrowth;
}


/**
 * Max milk yield
 */
double dk_cattle::GetMilkYieldCapacity()  //Ykap
{
  double milkYieldCapacity = (herdMilkYieldConstant/244.0) - 0.8;
  return milkYieldCapacity;
}


double dk_cattle::Get_norm_weight()
{
	return GetSize();
};


/**
 * Calculate intake
 */
void dk_cattle::intake()
{
   DM_intake=0.0;
   ME_intake = 0.0;
   FE_intake = 0.0;
   Grazed_DM = 0.0;
   feedUseList->Reset();

	if (isSuckling)  //only if is a calf and is with mother
   {
      milk = Getmother()->Getmilk();
      DM_intake=milk->GetAmount() * milk->GetdryMatter();
      ME_intake= milk->GetAmount()*milk->GetME();
      FE_intake= milk->GetAmount()*milk->GetfeedUnitsPerItemUnit();
   }


	int Hi=0;
	linkList <feed_resource> * ptrlist = new linkList <feed_resource>;
   DoSelection(ptrlist,&Hi);
#ifndef _STANDALONE
   if (fieldList)
	   for (int i=0;i<fieldList->NumOfNodes(); i++) {fieldOfftakeList[i]= 0.0;};
#endif

   double //CK tonnes_per_ha_this_resource,
   DM_intake_this_resource;
   bool animalIsFull = false;
   feed_resource *aFeed_resource;
//   for (int counter=0;counter<=Hi;counter++)           taken out until BPE sorts out Comsort
	int counter=Hi;
   while ((counter>=0) && (!animalIsFull))
   {
      aFeed_resource= (feed_resource *) ptrlist->ElementAtNumber(counter);
      if (!animalIsFull)
      {
         //handle a supplement
         if (aFeed_resource->GetIsSupplement())
         {
				animalIsFull = GetAmountNextFeed(aFeed_resource);
            DM_intake_this_resource= aFeed_resource->GetAmount()* aFeed_resource->GetdryMatter() * 1000.0;
            if (DM_intake_this_resource>0.0)
            {
               *aFeed_resource * GetNumber();     //adjust for animal number prior to output of demand
               theProducts->SubtractProduct(aFeed_resource);                     // subtract feed from storage
               *aFeed_resource * (1.0/GetNumber());     //adjust back
         	}
         }
#ifndef _STANDALONE

         else  //handle a grazed feed resource
         {
            if (hoursHoused==24.0)
               {theMessage->WarningWithDisplay("dk_cattle: intake - grazed feed in diet but animal housed all day");}

            //CK            tonnes_per_ha_this_resource=aFeed_resource->GetAmount() * aFeed_resource->GetdryMatter();
//                                                aFeed_resource->Getarea_occupied();
            double pot_availability = aFeed_resource->GetAmount() * aFeed_resource->GetdryMatter();
            if (pot_availability>0.0)
            {
            // calc possible intake from this resource
					animalIsFull = GetAmountNextFeed(aFeed_resource);  //on return, the amount is the intake of this feed
	            DM_intake_this_resource= aFeed_resource->GetAmount()* aFeed_resource->GetdryMatter() * 1000.0;
               Grazed_DM+=DM_intake_this_resource;
               aFeed_resource->Setamount(DM_intake_this_resource/(aFeed_resource->GetdryMatter()* 1000.0));  //convert from kg DM to tonnes FW
               if (aFeed_resource->GetAmount()>1000.0)
                  theMessage->FatalError("dk_cattle: error in feed resource diet");
	            *aFeed_resource * GetNumber();     //adjust for animal number prior to output of demand
               double NGrazed = aFeed_resource->GetAmount() * aFeed_resource->GetAllN().n* 1000.0;
               double CGrazed = aFeed_resource->GetAmount() * aFeed_resource->GetC_content()* 1000.0;
               ((patch *) aFeed_resource ->GetOwner())->SetGrazed(0.5,DM_intake_this_resource);
               theOutput->AddIndicator(environmentalIndicator,"33.04 N grazed by cattle","kg N",Number * NGrazed);
               theOutput->AddIndicator(environmentalIndicator,"43.04 C grazed by cattle","kg C",Number * CGrazed);
	            *aFeed_resource * (1.0/GetNumber());     //adjust back
               // go through field list and find to which field this patch belongs then add the offtake
               for (int i=0;i<fieldList->NumOfNodes(); i++)
               {
                field *aField = fieldList->ElementAtNumber(i);
                patch* aPatch = (patch *) aFeed_resource ->GetOwner();
                if (aPatch->GetOwner() == aField)
                {
                    {fieldOfftakeList[i]+= DM_intake_this_resource * GetNumber();}
                }
               }

            }
            else
            {
               DM_intake_this_resource=0.0;
               aFeed_resource->Setamount(0.0);
            }
         } //if a grazed resource
#endif
         DM_intake+=DM_intake_this_resource;
         // add this resource to the current diet
         if (aFeed_resource->GetAmount()>0.0)
         {
            *currentDiet = *currentDiet + *aFeed_resource;
            ME_intake+= DM_intake_this_resource * aFeed_resource->GetME()/(aFeed_resource->GetdryMatter()*1000.0);
            FE_intake+= DM_intake_this_resource * aFeed_resource->GetfeedUnitsPerItemUnit()/(aFeed_resource->GetdryMatter()*1000.0);
         if (currentDiet->GetdryMatter() - currentDiet->Getorganic()<0.0)
   	theMessage->FatalError("dk_cattle: om");
#ifndef _STANDALONE
            if (theOutputData->DetailsData.getCowDetails()==true)
            {
               feed_resource * anItem = new feed_resource(*aFeed_resource);
               feedUseList->InsertLast(anItem);
            }
#else
            feed_resource * anItem = new feed_resource(*aFeed_resource);
            feedUseList->InsertLast(anItem);
#endif
			}
		} //if animal is full
      delete aFeed_resource;
      counter--;
   }// end count through grazing resources

/*if (age>1063)
            cout << endl;*/
   if (FE_intake>1000.0)
   	theMessage->FatalError("dk_cattle: error in current diet - too much energy");
   if (FE_intake<0.00001)
   {
   	cout << endl << theTime << endl;
   	theMessage->FatalError("dk_cattle: error in current diet - too little energy for ",GetAlias().c_str());
   }

	double totalNinput= Number * currentDiet->GetAmount() * currentDiet->GetorgN_content().n* 1000.0;
	double totalCinput= Number * currentDiet->GetAmount() * currentDiet->GetC_content()* 1000.0;

#ifndef _STANDALONE
   if (isYoung)    //output BEFORE adding milk (is internal to livestock)
   {
      theOutput->AddIndicator(environmentalIndicator,"33.01 N in feed for young cattle","kg N",totalNinput);
      theOutput->AddIndicator(environmentalIndicator,"43.01 C in feed for young cattle","kg C",totalCinput);
   }
   else
   {
   	theOutput->AddIndicator(environmentalIndicator,"33.02 N in feed for adult cattle","kg N",totalNinput);
   	theOutput->AddIndicator(environmentalIndicator,"43.02 C in feed for adult cattle","kg C",totalCinput);
   }
#endif
   Nbudget.AddInput(currentDiet->GetAmount() * 1000.0 * currentDiet->GetAllN().n);
   Cbudget.AddInput(currentDiet->GetAmount() * 1000.0 * currentDiet->GetC());

   if (isSuckling)
   {
//   	totalNinput+=milk->GetAmount()* 1000.0 * milk->GetorgN_content().n;
  //	   Nbudget.AddInput(milk->GetAmount() * 1000.0 * milk->GetorgN_content().n);
//      feedPlanList=NULL;  //otherwise, if calf is culled, feedPlanList will be deleted and mother will have no diet
      //this needs to be updated - what happens to suckled milk?
   }
	double feedNPerc= 100.0 *totalNinput/(DM_intake * Number);   //totalNinput is total consumed by animals, DM_intake is per animal
   if ((feedNPerc<1.0)&&(!isSuckling))
   	theMessage->WarningWithDisplay("dk_cattle: N in diet <1.0%");
   // if no grazing then house animals (otherwise will try an partition manure to fields)
   if ((Grazed_DM == 0)&&(hoursHoused<24.0))
   	hoursHoused = 24.0;
   					// used for debugging tempfeedList->ForgetAll();  delete tempfeedList;
  ptrlist->ForgetAll();
  delete ptrlist;
}


/**
 * Return the fill for a feed; varies depending on animal age
 */
double dk_cattle::Getfill(feed_resource *aFeed_resource)
{
   if (lactationNumber>0)
   	return aFeed_resource->Getfill();
   else
   	return aFeed_resource->GetfillU();
};

/**
 * Return the fill per SFU for a feed - used in intake calculation
 */
double dk_cattle::GetfillperSFU(feed_resource *aFeed_resource)
{
   double fillPerSFU = Getfill(aFeed_resource)/aFeed_resource->GetfeedUnitsPerItemUnit();
   return fillPerSFU;
};


/**
 * Maintenance energy requirement
 */
double dk_cattle::calc_energy_maint()
{
 return weight/200.0 + 1.5;
}
/**
 * Nice to know: where does the numher in the fomular comes from
 */
double dk_cattle::GetavailableFodderUnits()
{
	double ret_val=0.0;
   if (FE_intake<12.0)
   	ret_val=FE_intake;
   else
   {
      double k0 = GetbulkCapacity();
      ret_val = 0.24 * FE_intake + 2.6 * k0 - ((2.21 * k0*k0)/FE_intake);
   }
   return ret_val;
}

/**
 *  Calculate the weight of the foetus in g
 */
double dk_cattle::GetFoetusWeight(double offset)
{
	double foetusWeight = CalcFoetusProtein(offset)/0.18467;
   return foetusWeight;
}

/**
 * Calculate the weight of the gravid uterus in g
 */
double dk_cattle::GetGravidUterusWeight(double offset)
{
	double gravidWeight = CalcGravidUterusProtein(offset)/0.18467;
   return gravidWeight;
}

void dk_cattle::Createfoetus()
{
 foetus = new dk_cattle(*this);
}


/**
 * SFUs required per kg weight gain
 */
double dk_cattle::GetSFUPerKgGain()
{
  double cg = 2.65; //SFUs per kg weight gain
  return cg;
}

/**
 * Calculate the animal growth or mobilisation in kg
 */
double dk_cattle::GetEnergyLimitedGrowth(double *energyForGrowth)
{
   double cg1 = 5.00; //SFUs per kg weight gain when putting on fat
   double cg = GetSFUPerKgGain();
	double growth;
   double potGrowth = GetPotentialGrowth();
   if (*energyForGrowth<0.0)
   	growth=	*energyForGrowth/cg;  //mobilisation
   else if (potGrowth<0.0)
      growth= *energyForGrowth/cg1;                      //depositing fat
   else if (*energyForGrowth<=potGrowth*cg)
   	growth = *energyForGrowth/cg;
   else
   	growth =potGrowth+(*energyForGrowth-potGrowth*cg)/cg1;
   return growth;
}

/**
 * Concentration of N in empty body weight change
 */
double  dk_cattle::NinEBWchange()
{
 double ret_val;
 double proteinReq = 112.0;  //g protein per kg EBG
 ret_val = proteinReq/N_to_protein();
 return ret_val;
}

//calculate the fill factors for a feedItem
//THIS is TEMPORARY - need real relationship
/**
 * Fudged calculation of fill for situations where fill details are not available from tables
 */
void dk_cattle::calculateFill(feedItem *aFeedItem)
{
	if ((aFeedItem->Getfill()==0.0)||(aFeedItem->GetfillU()==0.0))
   {
    aFeedItem->Setfill(100.0 * 0.5/aFeedItem->GetfeedUnitsPerItemUnit());
    aFeedItem->SetfillU(100.0/aFeedItem->GetfeedUnitsPerItemUnit());
 	}
};

/**
 * Returns g of protein in foetus
 */
double dk_cattle::CalcFoetusProtein(double offset)  //weight in grams
{
      double foetusProtein = cattle::CalcFoetusProtein(offset);
      return foetusProtein;
}

/**
 * Returns g protein in gravid uterus
 */
double dk_cattle::CalcGravidUterusProtein(double offset) //weight in grams
{
      double gravidUterusProtein= cattle::CalcGravidUterusProtein(offset);
      return gravidUterusProtein;
}

/**
 *  Initialise a cow - used in standalone version
 */
#ifdef _STANDALONE

void dk_cattle::Initialise(double anAge, int thedaysFromConception, int thelactationNumber)
{
	SetdaysFromConception(thedaysFromConception);
   if (thedaysFromConception>0)
   	pregnant = true;
   lactationNumber = thelactationNumber;
   lactating=true;
   cattle::Initialise(2, 0, anAge, 0, 1, false);
}
#endif
/**
 * Detailed data output to file - used in standalone version
 */
void dk_cattle::WriteCowOutput(fstream * afile, bool header)
{
//	cattle::WriteCowOutput(afile, header);
            theMessage->FatalError("dk_cattle::WriteCowOutput - function not implemented");

};


/**
 * Calculates the amount (in tonnes) of the feed 'aFeed_resource' that is consumed, given an existing diet
 */
bool dk_cattle::GetAmountNextFeed(feed_resource * aFeed_resource)
{
	bool ret_val = false;
   double maxAmountNewFeed = aFeed_resource->GetAmount()*1000.0; //is the amount of this feed available
   double amountNewFeed; //is the limit applied by the animal to the intake of this feed
   if ((aFeed_resource->GetCode()>=307)&&(aFeed_resource->GetCode()<=311))   //dealing with a milk substitute here
   {
	   amountNewFeed=GetPotentialMilkSolidsIntake(age)/aFeed_resource->GetdryMatter();
   }
   else
   {
      double k0 = GetbulkCapacity();    //capacity returned is appropriate to animal age
      double currentAmount = currentDiet->GetAmount()* 1000.0;
      double currentfillPerKg=0.0;      //NB - all content variables (x per kg) are on a fresh weight basis
      double currentfillPerSFU=0.0;
      double currentSFUperKg=0.0;
      amountNewFeed = 0.0;
      calculateFill(aFeed_resource);  //need to calculate fill values if not already read from feed table
      double newFeedfillPerKg = Getfill(aFeed_resource)/1000.0;  //fill value returned is appropriate to animal age (ie young or old)
      double newFeedfillPerSFU = GetfillperSFU(aFeed_resource);
      double newFeedSFUperKg = aFeed_resource->GetfeedUnitsPerItemUnit()/1000.0;
      if (currentAmount == 0)          //aFeed_resource is the first feed item
      {
         if (newFeedfillPerSFU >=0.35)
            amountNewFeed = k0/newFeedfillPerKg;
         else
            amountNewFeed = GetPotentialIntake(newFeedfillPerSFU)/newFeedfillPerKg;
      }
      else
      {
         currentfillPerKg = Getfill(currentDiet)/1000.0;
         currentfillPerSFU = GetfillperSFU(currentDiet);
         currentSFUperKg = currentDiet->GetfeedUnitsPerItemUnit()/1000.0;
         if (currentfillPerSFU>=0.35)
            amountNewFeed = (k0 - currentAmount*currentfillPerKg)/newFeedfillPerKg;
         else
         {
          double a = 0.44; double b=1.6;
          double constantTerm =  currentAmount*(k0*(a*currentSFUperKg+b*currentfillPerKg)-currentAmount* currentfillPerKg *currentSFUperKg);
          double linearTerm = k0*(a*newFeedSFUperKg+b*newFeedfillPerKg)
                       -currentAmount*(currentfillPerKg * newFeedSFUperKg + newFeedfillPerKg * currentSFUperKg);
          double quadraticTerm = -newFeedfillPerKg * newFeedSFUperKg;
          if (!SolveQuadratic(false, quadraticTerm,linearTerm,constantTerm,&amountNewFeed))
            theMessage->FatalError("NixCbits - SolveQuadratic: solution cannot be found");
          double newDietfillPerSFU =  (currentfillPerKg*currentAmount +amountNewFeed*newFeedfillPerKg)
               /(currentAmount*currentSFUperKg+amountNewFeed*newFeedSFUperKg);
          double newDietFill = currentAmount * currentfillPerKg + amountNewFeed * newFeedfillPerKg;
          double maxFill = GetPotentialIntake(newDietfillPerSFU);
          if (newDietFill>maxFill)    //new diet would have fill per SFU > 0.35
          {
            amountNewFeed = (maxFill - currentAmount*currentfillPerKg)/newFeedfillPerKg;
          }
         }
      }
      if (amountNewFeed<0.0)
      	amountNewFeed=0.0;   //can happen when milk substitute is fed to calves in combination with a feed with fill
   }
   if (amountNewFeed > maxAmountNewFeed)
   {
   	amountNewFeed=maxAmountNewFeed;
      ret_val = false;
   }
   else
   	ret_val = true;
   aFeed_resource->Setamount(amountNewFeed/1000.0);
   return ret_val;
}



/**
 * Fat (g) per kg weight change
 */
double dk_cattle::CalcFatInEBWchange(double energy, double protein)
{
	double fat = 1000.0 * (energy * 7.86 - protein * 23.6/1000.0)/39.3;
   return fat;
}



/**
 *  Add weight and protein to foetus
 */
void dk_cattle::GrowGravidUterus(double newProtein, double newFat)
{
	if (isFoetus)
   	theMessage->FatalError("cattle:GrowFoetus - this animal is a foetus!");
   else
   {
   	double newGravidUterus = (GetGravidUterusWeight(0.0)/1000.0)-foetus->Getweight();     //is actually weight of gravid uterus
      foetus->GrowFoetus(newProtein/1000.0,newFat/1000.0,newGravidUterus);
      weight+=newGravidUterus;
   }
};
/**
 * Updates protein, fat and wieght
 */
void dk_cattle::InitialiseFoetus()
{
 SetbodyProtein(CalcGravidUterusProtein(0.0)/1000.0);
 SetbodyFat(CalcGravidUterusFat(0.0)/1000.0);
 Setweight(GetGravidUterusWeight(0.0)/1000.0);
}
/**
 * Update heat Production. Nice to know. Why are those number correct
 */
void dk_cattle::CalcHeatProduction(manure *urine)
{
   double digestibleEnergy = currentDiet->GetdigestibleEnergy();
	double urineEnergy = 18.0 * 1000.0 * urine->GetdryMatter() * urine->GetAmount();
   double methaneEnergy = GetMethaneProduction() * 0.0555; //calculated from DJF rapport 54, v2, p86
   double metEnergy = digestibleEnergy - methaneEnergy - urineEnergy;
   double productEnergy = 23.6 * growth->GetAmount() * growth->GetorgN_content().n
   								+  39.3 * growth->GetAmount() * growth->Getfat();
	if (milk)
   {
   	double milkEnergy = milk->GetAmount() * 1000.0 * 0.4 * 7.89;   //0.4 = SFU/kg milk, 7.89 = MJ/SFU
   	productEnergy +=  milkEnergy;
   }
   if (foetus)
		productEnergy += 23.6 * foetus->Getgrowth()->GetAmount() * foetus->Getgrowth()->GetorgN_content().n
   								+  39.3 * foetus->Getgrowth()->GetAmount() * foetus->Getgrowth()->Getfat();

   heatProduction = metEnergy - productEnergy;
};


/**
 * Get animal intake
 */
void dk_cattle::Getintake(double *DM, double *ME, double *FE, double *DM_herb, double *ME_herb, double *FE_herb)
{
   *DM=0.0;
   *ME = 0.0;
   *FE = 0.0;
   *DM_herb=0.0;
   *ME_herb = 0.0;
   *FE_herb = 0.0;

   if (currentDiet)
   	delete currentDiet;
   currentDiet = new feed_resource();

   double DMFromGrazing = 0.0;
	if (isSuckling)  //only if is a calf and is with mother
   {
//      rel_capacity-=SucklingCalfMilkIntake();
      feedPlanList=Getmother()->GetfeedPlanList();  //suckling calves have access to same diet as mother
   }

	int Hi=0;
	linkList <feed_resource> * ptrlist = new linkList <feed_resource>;
   DoSelection(ptrlist,&Hi);
#ifndef _STANDALONE
   if (fieldList)
//   	{theMessage->FatalError("uk_cattle: no fieldList initialised");}
	   for (int i=0;i<fieldList->NumOfNodes(); i++) {fieldOfftakeList[i]= 0.0;};
#endif

   //  {work out potential intake}
/*   double pot_intake=9999.0;
   double tonnes_per_ha=0;
 	double tonnes_per_ha_this_resource,fillThisFeed,fillPerKg,DM_intake_this_resource;
   double fillSoFar = 0.0;*/
	   double tonnes_per_ha_this_resource,DM_intake_this_resource;
	   //CK cout << GetPotentialMilkSolidsIntake(age);
   bool animalIsFull = false;
   feed_resource *aFeed_resource;
   for (int counter=0;counter<=Hi;counter++)
   {
      aFeed_resource= (feed_resource *) ptrlist->ElementAtNumber(counter);
      if (!animalIsFull)
      {
         //handle a supplement
         if (aFeed_resource->GetIsSupplement())
         {
				animalIsFull = GetAmountNextFeed(aFeed_resource);
            DM_intake_this_resource= aFeed_resource->GetAmount()* aFeed_resource->GetdryMatter() * 1000.0;
         }
#ifndef _STANDALONE

         else  //handle a grazed feed resource
         {
            if (hoursHoused==24.0)
               {theMessage->WarningWithDisplay("dk_cattle: intake - grazed feed in diet but animal housed all day");}

            tonnes_per_ha_this_resource=aFeed_resource->GetAmount() * aFeed_resource->GetdryMatter();
//                                                aFeed_resource->Getarea_occupied();
            if (tonnes_per_ha_this_resource>0.0)
            {
            // calc relative availability of this resource
				   double pot_availability = (GetbulkCapacity()/aFeed_resource->Getfill())
  										 * Get_availability_factor(tonnes_per_ha_this_resource);
//				   double pot_availability = aFeed_resource->GetAmount() * aFeed_resource->GetdryMatter();
               if ((24-hoursHoused)<6.0)
               	pot_availability*=(24-hoursHoused)/6.0;
               aFeed_resource->Setamount(pot_availability); //adjust amount to reflect availability
            // calc possible intake from this resource
					animalIsFull = GetAmountNextFeed(aFeed_resource);  //on return, the amount is the intake of this feed
	            DM_intake_this_resource= aFeed_resource->GetAmount()* aFeed_resource->GetdryMatter() * 1000.0;
               DMFromGrazing+=DM_intake_this_resource;
               aFeed_resource->Setamount(DM_intake_this_resource/(aFeed_resource->GetdryMatter()* 1000.0));  //convert from kg DM to tonnes FW
               if (aFeed_resource->GetAmount()>1000.0)
                  theMessage->FatalError("dk_cattle: error in feed resource diet");
            }
            else
            {
               DM_intake_this_resource=0.0;
               aFeed_resource->Setamount(0.0);
            }
            *DM_herb+=DM_intake_this_resource;
            *ME_herb += DM_intake_this_resource * aFeed_resource->GetME()/(aFeed_resource->GetdryMatter()*1000.0);
            *FE_herb +=  DM_intake_this_resource * aFeed_resource->GetfeedUnitsPerItemUnit()/(aFeed_resource->GetdryMatter()*1000.0);
         } //if a grazed resource
#endif
         *DM+=DM_intake_this_resource;
         *ME+= DM_intake_this_resource * aFeed_resource->GetME()/(aFeed_resource->GetdryMatter()*1000.0);
         *FE+= DM_intake_this_resource * aFeed_resource->GetfeedUnitsPerItemUnit()/(aFeed_resource->GetdryMatter()*1000.0);
		} //if animal is full
      delete aFeed_resource;
   }// end count through grazing resources
  delete currentDiet;
  currentDiet = NULL;
  ptrlist->ForgetAll();
  delete ptrlist;
}


/**
 * Convert a foetus into a calf
 */
void dk_cattle::DoBirth(double *afterBirthProtein, double *afterBirthFat)
{
  double gravidUterusProtein = GetbodyProtein();     //foetus and uterus protein
  double foetusProtein = CalcFoetusProtein(0.0)/1000.0;   //days since conception has been updated - use yesterday's to avoid false growth
  double gravidUterusFat = GetbodyFat();
  double foetusFat = CalcFoetusFat(0.0)/1000.0;
  *afterBirthFat = gravidUterusFat - foetusFat;
  *afterBirthProtein = gravidUterusProtein - foetusProtein;
  SetbodyProtein(foetusProtein);
  Nbudget.AddOutput(*afterBirthProtein/N_to_protein());
  Cbudget.AddOutput(GetCarbon(*afterBirthFat,*afterBirthProtein));
  SetbodyFat(foetusFat);
  Setweight(GetFoetusWeight(0.0)/1000.0);
  Setage(0);
  SetisFoetus(false);
  SetisYoung(true);
  SetisSuckling(true);
  SetFoetus(NULL);
}

/**
 * Calculate palatability (here equated with concentration of rumen fill)
 */
double dk_cattle::GetPalatability(feed_resource *aFeed_resource)
{
	double ret_val = GetfillperSFU(aFeed_resource);
   if (ret_val>0.0)
	   return 1/ret_val;
   else
   	return 9999.0;
};


/**
 * Update amino Acid, body Protein,weight, availableN, bodyFat and growth
 */
void dk_cattle::ChangeBodyComposition(double weightChange, double *avail_FE, double *availableN)
{
	if (weightChange!=0.0)
   {
      feedItem * anItem = new feedItem();
      double growthN = weightChange * NinEBWchange();
      double proteinChange =growthN * N_to_protein();
      aminoAcidSupply+=proteinChange;
      bodyProtein+=proteinChange/1000.0;
      *availableN=*availableN - growthN;  //growthN is negative here
      anItem->SetorgN_content(growthN/(1000.0 * weightChange));
      anItem->Setamount(weightChange);
      weight+=weightChange;
      double energyChange = GetSFUPerKgGain()*weightChange;
      *avail_FE-=energyChange;
      double fatChange = CalcFatInEBWchange(energyChange,growthN*N_to_protein());
      anItem->Setfat(fatChange/(1000.0 * weightChange));
      bodyFat+=fatChange/1000.0;
      double growthCarbon = GetCarbon(fatChange,proteinChange)/1000.0;
      anItem->SetC_content(growthCarbon/weightChange);
      *growth + *anItem;
      delete anItem;
   }
}

/**
 * Calculate potential intake from SCA 1990 intake equation
 */
double dk_cattle::GetPotentialMilkSolidsIntake(int days)
{
 double ret_val;
 ret_val = GetPotentialMilkYield(days+1)*0.125;  // estimates milk intake as maximum milk yield * assumed solids content of 12.5%
 return ret_val;
};

/**
 * Calculate the potential milk production over more days
 */
double dk_cattle::GetPotentialAnnualMilkProduction(int durationOfLactation)
{
 double ret_val=0.0;
 for (int days=0;days<durationOfLactation;days++)
	 ret_val += GetPotentialMilkYield(days);
 return ret_val;
};

/**
 * calculate the amount of milk the cow is producing in one day since it have given bird
 */
double dk_cattle::GetPotentialMilkYield()
{
	return GetPotentialMilkYield(int(daysFromConception));
};

/**
 * Calculate the amount of Energy needs. Does only support lactating or pregnant cow
 */
double dk_cattle::GetEnergyRequirement()
{
	double ret_val=calc_energy_maint() + GetSFUPerKgGain() * GetPotentialGrowth();
   if (lactating)
 	  ret_val+= GetSFUsPerKgEKM() * GetPotentialMilkYield(int(daysFromParturition)) * lactationEnergyFudgeFactor;
   if (pregnant)
   	ret_val+=GetFoetusEnergyRequirement();
   return ret_val;
}

/**
 * Returns energy requirement in SFU
 */
double dk_cattle::GetFoetusEnergyRequirement()
{
   //new foetus here includes associated pregnancy tissue such as additional uterus
   double weeksBeforeCalving = (gestationPeriod - daysFromConception)/7;
   if (weeksBeforeCalving<0)
      weeksBeforeCalving=0.0;
   double foetusFEdemand = (MBW/242)* exp(-0.14*weeksBeforeCalving);
   return foetusFEdemand;
}


/**
 * Subroutine for GetProteinRequirement
 */
double dk_cattle::GetFoetusPorteinRequirement()
{
   //new foetus here includes associated pregnancy tissue such as additional uterus
	double ret_val = CalcGravidUterusProtein(1)
                                           -CalcGravidUterusProtein(0);
   return ret_val;
}
/**
 * Calculate the amount of Protein needs. Does only support lactating or pregnant cow
 */
double dk_cattle::GetProteinRequirement()
{
	double ret_val=(CalcMaintenanceN() + NinEBWchange() * GetPotentialGrowth())* N_to_protein();
   if (lactating)
 	  ret_val+= milkProteinConc * GetPotentialMilkYield(int(daysFromParturition));
   if (pregnant)
   	ret_val+=GetFoetusPorteinRequirement()/0.85;
   ret_val/=0.75;		//estimate protein digestibility to 75%
   return ret_val;
}

/**
 * Calculate potential intake from SCA 1990 intake equation
 */
double dk_cattle::GetPotentialMilkIntake()
{
 double ret_val;
 ret_val = pow((GetSize()),0.75) * (0.42 + 0.58*exp(-0.036 * age));
 return ret_val;
};

/**
 * Nice to know what are we calculating here?
 */
double dk_cattle::GetCalfSolidIntakeFraction()
{
	double ret_val=1.0;
	if (age < 60)
 		ret_val = 1.0/(1.0 + exp(0.2*(60-age)));
   return ret_val;
}

