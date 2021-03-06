/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "patch.h"
#include <message.h>
#include <cropwwht.h>
#include <cropswht.h>
#include <cropbare.h>
#include <cropwr.h>
#include <cropsr.h>
#include <cropsbrl.h>
#include <cropwbrl.h>
#include <cropgrss.h>
#include <croppea.h>
#include <cropbeet.h>
#include <cropmaize.h>
#include <cropclover.h>
#include <cropseedgrass.h>
#include <cropgrss.h>
#include <croprye.h>
#include <cropoat.h>
#include <cropoilrad.h>
#include <cropdyerswoad.h>
#include <cropchickory.h>
#include <croppotato.h>
#include <orgprod.h>
#include <products.h>
#include <climclas.h>
#include <fiopan.h>
#include <fiopha.h>
#include <fiopir.h>
#include <fiopmi.h>
#include <fiopml.h>
#include <fiopso.h>
#include <fiopst.h>
#include <fioppc.h>
#include <fiopgraz.h>
#include <timeUse.h>
#include <contrparm.h>
#include <output.h>
#include <nitrogen.h>
#include <technics.h>
#include <feed_resource.h>
#include <field.h>
#include <ALFAM.h>
#include <manure_manager.h>
#include <ecosyst.h>

/****************************************************************************\
Constructor with arguments
\****************************************************************************/
patch::patch(const char * aName, const int aIndex, const base * aOwner)
   :base(aName,aIndex,aOwner)
{
#ifndef NITROSCAPE
	theEcosystem					= new ecosystem("ecosystem",0,this);
#else
   theEcosystemInterface		= new ecosystinterfaceClass();
#endif
   area                    = 0.0;
   grazable						= false;
   soilManure					= NULL;
   cropManure					= NULL;
   birthday 					= new bsTime(theTime);
   dung							= false;
   overlap						= false;
   my_resource					= NULL;
   type                    = 0;
}

/****************************************************************************\
Copy constructor.
\****************************************************************************/
patch::patch(const patch& p)
{
	theEcosystem = new ecosystem(*p.theEcosystem);
   grazable				  = p.grazable;
   dung          		  = p.dung;
   overlap             = p.overlap;
   type                = p.type;

   if (p.soilManure)
   	soilManure=new manure(*p.soilManure);
   else
      soilManure=NULL;
   if (p.cropManure)
   	cropManure = new manure(*p.cropManure);
   else
      cropManure = NULL;
   if (p.my_resource)
	   my_resource   = new feedItem(*p.my_resource);
   else
   	my_resource = NULL;
   if (p.birthday)
      birthday		  = new bsTime(*p.birthday);
   else
   	birthday = NULL;
}

/****************************************************************************\
Destructor.
\****************************************************************************/
patch::~patch()
{
	delete theEcosystem;
   if (soilManure)
   	delete soilManure;
   if (cropManure)
   	delete cropManure;
   if (my_resource)
      delete my_resource;
   if (birthday)
      delete birthday;
}

/****************************************************************************\
Returns a copy of this instance at the same inheritance level.
Used in connection with 'cloneList'
\****************************************************************************/
patch* patch::clone() const
{
	patch* p = new patch(*this);
	return p;
}

/****************************************************************************\
\****************************************************************************/
void patch::Initialise(char * soilFileName,int aIndex, double anArea)
{
	area = anArea;
#ifndef NITROSCAPE
  	theEcosystem->Initialise(soilFileName, aIndex, anArea);
#endif
}


/****************************************************************************\
\****************************************************************************/

/****************************************************************************\
Demands that the first crop is the main crop
\****************************************************************************/
bool patch::ReadyForHarvestOrLater()
{
   return theEcosystem->ReadyForHarvestOrLater();
}

/****************************************************************************\
Demands that the first crop is the main crop
\****************************************************************************/
bool patch::IrrigationDemand()
{
	return theEcosystem->IrrigationDemand();
}

/****************************************************************************\
Handles a patch operation (e.g. sowing)
Parameters:
   year, month, day
   FOP                object with operation characteristics
\****************************************************************************/
void patch::HandleOp(fieldOperationFields * FOP)
{
/*   crop* aCrop=NULL;
   if (theCrops->NumOfNodes()>0)
      aCrop=theCrops->ElementAtNumber(0); // By default look at first crop;
  */
   if ((FOP->GetOperationId()==CombineHarvesting) || (FOP->GetOperationId()==CombineHarvestingShred)  || FOP->GetOperationId()==BeetHarvesting)   // "Mowing" moved down to forage section NJH mar 07 !!!
   {
      harvestFields * hrv=(harvestFields *)FOP;
      theEcosystem->HarvestOperations(hrv);
		plantItem * Storage=hrv->GetMainProduct();
   }

	if ((FOP->GetOperationId()==SpreadingLiquidManure)||(FOP->GetOperationId()==SpreadingSolidManure))
   {
      animalFertFields * aFF=(animalFertFields *)FOP;
		manure * theManure = (manure *) aFF->GetManure()->clone();   //create copy of manure
      double factor = area * theManure_manager->GetadjustmentFactor();
      // calculate absolute amount of manure  and adjust manure application (set by manure manager). Amount in tonnes or kg N, depending on mainUnit
		*theManure*factor;
		theProducts->SubtractProduct(theManure);  //note that manure is removed from store and quality parameters are updated here
      if (area<1E-36)
         theMessage->FatalError("patch::HandleOp - patch has extremely low or zero area");
      else
         *theManure * (1.0/area);     //reconvert to tonnes per ha
      double N_added = theManure->GetAmount()*theManure->GetAllN().n*1000.0;  //kg per ha

      if (N_added<1)
         theMessage->WarningWithDisplay("patch::HandleOp - very little or no N in manure");
	   theOutput->AddIndicator(environmentalIndicator,"31.06 N from spread manure","kg N",N_added*area);
	   theOutput->AddIndicator(economicIndicator,"19.65 Manure spread on field","t",theManure->GetAmount()*area);
      theOutput->AddIndicator(environmentalIndicator,"40.02 C from field-applied manure","kg C",theManure->GetC_content()*theManure->GetAmount()*area*1000.0);
#ifdef TUPLE
      if (((field*)Owner)->GetWriteTuples());
         ((field*)Owner)->AddTuple(theTime.GetString2(),N_added*area,theManure->GetAmount()*area,"FERTILISATION","MANURE",0);
#endif
      animalFertTech * aTechnique = ((animalFertTech *) theTechnics->GetFieldTechnique(aFF));
      manureApplic *applicTech = ((manureApplic *) aTechnique->GetApplicationTech());

      /* --------- Should calculate canopy contamination here - stil missing !!!
      if ((aCrop) && (aTechnique->GetcropContamination()))   //crop present and method will contaminate canopy
      {
      }
      ------------------------------------------------------------------------*/

      double NH3volat=0.0;   //volatilisation as N not NH3
      nitrogen TAN_conc=theManure->GetNH4_content();
//      double TAN_amount = theManure->GetAmount()*TAN_conc.n;
		switch (theControlParameters->GetvolatModel())
      {
      	case 0:NH3volat= theManure->VolatNitrogen(applicTech->GetNH3emissionFactor());
               if(applicTech->GetpropBuried()>0.0)
               {
                  decomposable d=(decomposable) *theManure;
                  d*(applicTech->GetpropBuried());
                  AddDecomposable(&d);
                  Incorporate(250.0,1.0);
                  *theManure * (1.0-applicTech->GetpropBuried());
               }
               AddDecomposable(theManure);
			      theOutput->AddIndicator(environmentalIndicator,"31.45 NH3-N loss by spreading","kg N",NH3volat*area*1000.0);
         		break;
        case 1:
		        {
/*This code does not work properly. It should be upgraded and used to replace/modify the ALFAM approach
               if(applicTech->GetpropBuried()>0.0)
               {
                  decomposable d=(decomposable) *theManure;
                  d*(applicTech->GetpropBuried());
                  aSoil->AddDecomposable(&d);
                  aSoil->Incorporate(250.0,1.0);
                  *theManure * (1.0-applicTech->GetpropBuried());
               }

               if(applicTech->GetpropBuried()<1.0)
               {
                  if (!soilManure)
                     soilManure = (manure *) theManure->clone();
                  else // If there is still manure left on the soil from a previous application
                  {
                     manure * aManure = (manure *) theManure->clone();
                     *soilManure + * aManure;
                     delete aManure;
                  }
                  double exposureTime = aFF->GetExposeTime();

                  if (exposureTime<24.0)  // have to handle volatilisation now, otherwise manure will be buried before volatilisation function is called
                     Volatilise(exposureTime, true);
               }
*/
               if (!soilManure)
                  soilManure = (manure *) theManure->clone();
               else // If there is still manure left on the soil from a previous application
                     theMessage->FatalError("patch::HandleOp - attempt to apply more manure, before volatilisation has ceased from the previous manure");

					double aveTemperature, aveWindspeed;
               double initDM=soilManure->GetdryMatter()*100.0;
               double initTAN=soilManure->GetNH4_content().n*1000.0;
               double appRate=soilManure->GetAmount();
               if (appRate>0.0)
               {
                  int typeOfManure = 0;
                  if (soilManure->GetfromAnimal()=="pig")
                     typeOfManure=1;
                  theClimate->CalcAverageClimateForPeriod(&theTime, 7, &aveTemperature, &aveWindspeed);         //get climate average for next 7 days
                  anALFAM = new ALFAM();
                  int applicCode=anALFAM->GetALFAMApplicCode(FOP->GetOperationId());
                  anALFAM->initialise(0, aveTemperature, aveWindspeed, typeOfManure, initDM, initTAN, appRate, applicCode,aFF->GetExposeTime());
               }
					break;
      }

         default:{};
      };
      delete theManure;
	}

	if (FOP->GetOperationId()==MineralFertilizing)
   {
		mineralFertFields * aMF=(mineralFertFields *)FOP;
      nitrogen N1,NoFert;                                              // NoFert is only used as a "zero" add
		N1 = (aMF->GetFertilizer1()->GetN_content())*(aMF->GetFertilizer1()->GetAmount())/10.0;    // Convert to g/m�. Change to more general algorithm.
//     	theOutput->AddIndicator(environmentalIndicator,"31.01 N from mineral fertilizer","kg N",N1.n*10.0*area);
      double NH4_fraction = aMF->GetFertilizer1()->GetNH4_fraction();
      double NH3VolatFrac = 0.03;  ///should be a parameter NJH Dec 2007
      double NH4_N = N1.n * NH4_fraction;
      double NH3Volat = NH4_N * NH3VolatFrac;
      theOutput->AddIndicator(environmentalIndicator,"31.48 NH3-N loss from mineral fertiliser","kg N",NH3Volat*area*10.0);
      NH4_N-=NH3Volat;
      double NO3_N = N1.n * (1.0 - NH4_fraction);
      if (aMF->GetFertilizer1()->GetMineralForm()=="SOLID")
			theEcosystem->AddNutrient(NO3_N,NH4_N,NoFert,NoFert);
      else
      	theEcosystem->AddNutrient(NoFert,NoFert,NO3_N,NH4_N);

#ifdef TUPLE
      if (((field*)Owner)->GetWriteTuples());
         ((field*)Owner)->AddTuple(theTime.GetString2(),N1.n*area*10.0,0,"FERTILISATION","MINERAL_N",0);
#endif
      if (aMF->GetFertilizer1()->GetCl_content()>0)
      	theEcosystem->AddChloride(aMF->GetFertilizer1()->GetCl_content()*aMF->GetFertilizer1()->GetAmount()/10.0);    // convert to g/m2
   }

	if (FOP->GetOperationId()==Irrigating)
   {
		irrigateFields * irr=(irrigateFields *)FOP;
      water *irrigationWater=irr->GetWater();
      theEcosystem->SetirrigationWater(irrigationWater);
	}

   if (FOP->GetOperationId()==Sowing || FOP->GetOperationId()==SowingBeets)
   {
      sowFields * S=(sowFields *)FOP;
      for (int i=0;i<S->GetSeedNum();i++)
      {
         seed * aSeed=S->GetSeed(i);
         if (aSeed->GetAmount() < 1E-20)
         {
         	cout << endl;
 //        	cout << "Field " << Owner->GetIndex() << " crop " << (char*)S->GetCropId(i) << endl;
            theMessage->FatalError("patch::HandleOp - no seed amount in sowing");
         }
         double RelativeDensity = aSeed->GetAmount()/aSeed->GetStandardDensity();
         if (RelativeDensity > 3.0 && aSeed->Getname()!="SEEDRYEGRASS" && aSeed->Getname()!="SEEDCLOVER")
         {
            cout << aSeed->Getname() << " " << aSeed->GetAmount() << " " << aSeed->GetStandardDensity() << endl;
            theMessage->WarningWithDisplay("patch::HandleOp - seed amount more than three times higher than standard");
         }
         if (RelativeDensity < 0.3333 && aSeed->Getname()!="SEEDRYEGRASS" && aSeed->Getname()!="SEEDCLOVER" && aSeed->Getname()!="SEEDBEET")
         {
            cout << aSeed->Getname() << " " << aSeed->GetAmount() << " " << aSeed->GetStandardDensity() << endl;
            theMessage->WarningWithDisplay("patch::HandleOp - seed amount more than three times lower than standard");
         }
//NJH May 2009, cropName allows variety to be passed via seed name for specific operations
         theEcosystem->Sow((char*)S->GetSpecificCropId(i),(char*)S->GetCropName(),aSeed->GetdryMatter()*aSeed->GetAmount()/10.0,RelativeDensity,aSeed->GetAmount()*aSeed->GetN_content()/10.0); // Convert to g/m�
         double N=aSeed->GetAmount()*aSeed->GetN_content()*area;
         theOutput->AddIndicator(environmentalIndicator,"31.03 N from seed","kg N",N);
         theOutput->AddIndicator(environmentalIndicator,"40.03 C from seed","kg C",aSeed->GetAmount()*aSeed->GetC_content()*area);

#ifdef TUPLE
         if (((field*)Owner)->GetWriteTuples());
            ((field*)Owner)->AddTuple(theTime.GetString2(),N,aSeed->GetAmount()*area,"SOWING","SEED",0);
#endif
      }
      if (theControlParameters->GetUseReductionParameters())
#ifndef NITROSCAPE
         theEcosystem->SetRUEfactor(S->GetReduction()); // Use reductions depending on previous crop and level of pesticides.
#else
            theMessage->WarningWithDisplay("patch::HandleOp - GetUseReductionParameters has been called!!");
#endif
   }


   	if (FOP->GetOperationId()==CutSetAside || FOP->GetOperationId()==ForageHarvesting ||
      						FOP->GetOperationId()==MakeSilage || FOP->GetOperationId()==Mowing) // BMP removed the following, but check what fate the beet top has now !!! || FOP->GetOperationId()==BeetHarvesting)
      {
         harvestFields * hrv=(harvestFields *)FOP;
         theEcosystem->HarvestOperations(hrv);
//               AddYieldToIndicators(presCrop,area*forage->GetAmount(),forageN.n*area*1000.0);
//               AddHarvestedAreaToIndicators(presCrop,area);
      }

      /* Code not made @@@ !!!
      if (FOP->GetOperationId()==StartGrazing) // Simple grazing scheme BMP + JBE
      {
         autoGraze       = true;
         nLoad           = ((grazingFields*)FOP)->GetNitrogenLoad();
         grazingEffiency = ((grazingFields*)FOP)->GetGrazingEffiency();
      }

   	if (FOP->GetOperationId()==EndGrazing) // Simple grazing scheme BMP + JBE
      {
         autoGraze = false;
         nLoad     = 0.0;
      }
      */

	if (FOP->GetOperationId()==Spraying)
   {
      plantCareFields * plantCare=(plantCareFields *)FOP;
      if (strcmp(plantCare->Get1stChemical()->GetName(),"RoundUp")==0)         // Currently only chemical that affects plant growth
         theEcosystem->KillAllCrops();
   }

   if (FOP->GetOperationId()==GatheringBales||FOP->GetOperationId()==GatheringCereal||FOP->GetOperationId()==GatheringSilage||FOP->GetOperationId()==GatheringHay)
   {
         harvestFields * hrv=(harvestFields *)FOP;
         theEcosystem->HarvestOperations(hrv);
	}

	if ((FOP->GetOperationId()==Ploughing)||(FOP->GetOperationId()==StubHarrowing))
   {
   	soilTreatFields * TillageOp=(soilTreatFields *)FOP;
   	theEcosystem->Tillage(TillageOp);
   }
}

/****************************************************************************\
Initialise budget variables
\****************************************************************************/
void patch::StartBudget()
{
#ifndef NITROSCAPE
	theEcosystem->StartBudget();
#endif
}

/****************************************************************************\
Check to see if budget is still valid
\****************************************************************************/
bool patch::EndBudget()
{
#ifdef NITROSCAPE
   return true;
#else
  return theEcosystem->EndBudget();
#endif
}

/****************************************************************************\
Returns the nitrogen amount in the soil and the crop (kg/ha)
\****************************************************************************/
double patch::NitrogenInSystem()
{
   return NitrogenInSoil()+NitrogenInCrops();
}

/****************************************************************************\
Returns the nitrogen amount in the soil (kg/ha)
\****************************************************************************/
double patch::NitrogenInSoil()
{
#ifdef NITROSCAPE
	theMessage->FatalError("function not operative within NITROSCAPE");
   return 0.0;
#else
   double ret = theEcosystem->NitrogenInSoil();
   if (soilManure)
   	ret+=soilManure->GetAllN().n * 10.0;
   ret=ret*area;
   return ret;
#endif
}

/****************************************************************************\
Returns the nitrogen amount in the crops (kg/ha)
\****************************************************************************/
double patch::NitrogenInCrops()
{
#ifdef NITROSCAPE
	theMessage->FatalError("function not operative within NITROSCAPE");
   return 0.0;
#else
   double ret=theEcosystem->NitrogenInCrops();
   if (cropManure)
   	ret+=cropManure->GetAllN().n * 10.0;
   ret=ret*area; // kg/ha
   return ret;
#endif
}

/****************************************************************************\
\****************************************************************************/
void patch::WriteNitrogen(double f)
{
#ifdef NITROSCAPE
	theMessage->FatalError("function not operative within NITROSCAPE");
#else
   if (soilManure)
      cout << "N in soil manure " << soilManure->GetAllN().n*f << endl;
   if (cropManure)
      cout << "N in crop manure " << cropManure->GetAllN().n*f << endl;

	theEcosystem->WriteNitrogen(f);
#endif
}

/****************************************************************************\
Volatilise ammonia (only called if manure is present)
\****************************************************************************/
void patch::Volatilise()
{
	switch (theControlParameters->GetvolatModel())
   {
   	case 1:
             double volat=0.0;  //volat in kg/ha
             if (anALFAM)
             {
             	volat= anALFAM->ALFAM_volatilisation(24.0);
//             cout << volat << endl;
                if (volat==0.0)
                {
                     if (anALFAM->GetIsfinished())
                     {
                        decomposable d=(decomposable) *soilManure;
                        AddDecomposable(&d);
                        delete soilManure;
                        soilManure=NULL;
                        delete anALFAM;
                        anALFAM=NULL;
                     }
//                     else   //user input says manure is incorporated before max time period is reached but there has been no incorporation
  //                     theMessage->FatalError("patch:Volatilise - premature termination of volatilisation (missing incorporation)");
                }
                else
                {
                  soilManure->SetNH4_content((soilManure->GetNH4_content()*soilManure->GetAmount()-volat/1000.0)/soilManure->GetAmount());
                  theOutput->AddIndicator(environmentalIndicator,"31.45 NH3-N loss by spreading","kg N",volat*area);
                }
				}
      break;
   }
}

/****************************************************************************\
Get all above ground N, including N in manure, crops etc in kg
\****************************************************************************/
double patch::GetAbovegroundCropN()
{
   return theEcosystem->GetAbovegroundCropN();
}

/****************************************************************************\
Get all above ground N, including N in manure, crops etc in kg
\****************************************************************************/
/*double patch::GetTotalAbovegroundN()
{
	double amountN = theEcosystem->GetTotalAbovegroundN();
   if (soilManure)
		 amountN += soilManure->GetAmount()*soilManure->GetAllN().n*1000;
   if (cropManure)
		 amountN += cropManure->GetAmount()*cropManure->GetAllN().n*1000;
   amountN*=area;
   return amountN;
}
*/
/****************************************************************************\
Get max crop height (m)
\****************************************************************************/
double patch::GetMaxPatchHeight()
{
	return theEcosystem->GetMaxCropHeight();
}

/****************************************************************************\
Called by the animal object to find out what is available
Returns quality and tonnes fresh weight per ha
\****************************************************************************/
feedItem * patch::GetFeedResource(double bite_depth_factor, double min_stubble_ht, int animalType)
{
   if (my_resource)
   	delete my_resource;
   switch (animalType)
   {
   	case 1:
      {
			my_resource = new feed_resource();  //the use if 'this' allows the animals to know which resource has provided the material
		   ((feed_resource*) my_resource)->SetIsSupplement(false);
         break;
      }
   	default:
      {
         theMessage->FatalError("ecosystem:GetFeedResource - not an allowed animal type");
         break;
      }
   }
	my_resource->SetOwner(this);
   my_resource->Setamount(0.0);
   my_resource->SetCode(1000+GetIndex());
   double grazing_height = bite_depth_factor * GetMaxPatchHeight();
	theEcosystem->GetFeedResource(my_resource, grazing_height, min_stubble_ht, animalType);
	if (type > 1)
      ((feed_resource*) my_resource)->SetTime_since_dunged(GetPatchAge());
   else
      ((feed_resource*) my_resource)->SetTime_since_dunged(0);
   ((feed_resource*) my_resource)->Setarea_occupied(GetArea());

	return my_resource;
}


/****************************************************************************\
Get age of patch
\****************************************************************************/
int patch::GetPatchAge()
{
	int age = theTime.GetDateValue()-birthday->GetDateValue();
   return age;
}

/****************************************************************************\
Get mass of dry matter of patch (kg)
\****************************************************************************/
double patch::GetPatchStandingDMkg()
{
   return GetPatchStandingDMkgPerHa() * area;
}

/****************************************************************************\
Get mass of dry matter of patch (kg/ha)
\****************************************************************************/
double patch::GetPatchStandingDMkgPerHa()
{
   return theEcosystem->GetStandingDMdensity() * 10.0;
}
/****************************************************************************\
Get mass of dry matter (kg DM)of patch that is above a certain cutting
or grazing height (m)
\****************************************************************************/
double patch::GetAvailablePatchStandingDMkg(double height)
{
	return theEcosystem->GetAvailableStandingDM(height) * 10.0;
}

/****************************************************************************\
Sets the crop variables that record the material grazed
Called by each animal that grazes this patch
\****************************************************************************/
void patch::SetGrazed(double bite_depth_factor, double DMGrazed)
{
	DMGrazed /= GetArea() * 10.0; //Convert from kg DM grazed to gDM per sq metre
   double grazing_height= bite_depth_factor * theEcosystem->GetMaxCropHeight();
	theEcosystem->SetGrazed(grazing_height, DMGrazed);
}

/****************************************************************************\
Add's two patches together
\****************************************************************************/
void patch::Add(patch* addPatch, double fraction)
{
   if (soilManure)
      *soilManure=*soilManure*(1.0-fraction);
   if (addPatch->soilManure)
      *(addPatch->soilManure)=*(addPatch->soilManure)*fraction;
   if (soilManure && addPatch->soilManure)
      *soilManure=*soilManure+*(addPatch->soilManure);
   if (!soilManure && addPatch->soilManure)
      *soilManure=*(addPatch->soilManure);
   if (cropManure)
      *cropManure=*cropManure*(1.0-fraction);
   if (addPatch->cropManure)
      *(addPatch->cropManure)=*(addPatch->cropManure)*(1.0-fraction);
   if (cropManure && addPatch->cropManure)
      *cropManure=*cropManure+*(addPatch->cropManure);
   if (!cropManure && addPatch->cropManure)
      *cropManure=*(addPatch->cropManure);
   if (!overlap==addPatch->overlap)
   	overlap = false;
   if (!dung==addPatch->dung)
   	dung = false;
   if (!grazable==addPatch->grazable)
   	grazable = false;

/*#ifndef NITROSCAPE
   *(addPatch->theEcosystem)=*(addPatch->theEcosystem)*(1.0-fraction);
   *theEcosystem=*theEcosystem+*(addPatch->theEcosystem);
#endif*/
      theMessage->FatalError("patch::Add ecosystem not implemented!!");
   area += addPatch->GetArea();
   birthday->SetTime(theTime.GetDay(),theTime.GetMonth(),theTime.GetYear());
}

/****************************************************************************\
\****************************************************************************/
void patch::SetPatchBirthday(bsTime aTime)
{
 birthday->SetTime(aTime.GetDay(),aTime.GetMonth(),aTime.GetYear());
}

/****************************************************************************\
Get amount grazed from this patch during the current period (kg)
\****************************************************************************/
double patch::GetGrazedDM()
{
#ifndef NITROSCAPE
	return theEcosystem->GetGrazedDM();
#else
	return 0.0;
#endif
}

/****************************************************************************\
Returns growth in kg DM
\****************************************************************************/
double patch::GetDailyDMProduction()
{
#ifndef NITROSCAPE
	return theEcosystem->GetDailyDMProduction();
#else
	return 0.0;
#endif
}

/****************************************************************************\
Developed by JBE + BMP
\****************************************************************************/
void patch::SimpleGraze()
{
#ifndef NITROSCAPE
	theEcosystem->SimpleGraze();
#endif
}

/****************************************************************************\
Returns mean potential growth in kg DM/ha/day
Is fairly crude - assumes total interception + even distribution between crops
\****************************************************************************/
double patch::GetPotentialGrowthRate(double radiation, double temperature)
{
//#ifndef NITROSCAPE
   return theEcosystem->GetPotentialGrowthRate(radiation, temperature);
//#endif
}

/****************************************************************************\
\****************************************************************************/
void patch::clearPatchRecords()
{
	theEcosystem->clearGrazingRecords();
}

void patch::UpdatePatches()
{
	theEcosystem->CropSoilExchange();
   if ((cropManure) || (soilManure))
   {
  		Volatilise();
   }
}

void patch::DepositManure(manure * animal_urine, manure * animal_dung)
{
   AddDecomposable(animal_dung);
	AddDecomposable(animal_urine);
}

void patch::GiveIndicators()
{
   if (soilManure)
   {
   	double NInsoilManure= soilManure->GetAllN().n*soilManure->GetAmount() * area *1000.0; // kg N/ha
	   theOutput->AddStateIndicator(environmentalIndicator,"31.61 Total soil/soilsurface N","kg N",NInsoilManure);
   	double CInsoilManure= soilManure->GetC_content()*soilManure->GetAmount() * area *1000.0; // kg C/ha
	   theOutput->AddStateIndicator(environmentalIndicator,"40.60 Total C in soil","kg C",CInsoilManure);
   }
   if (cropManure)
   {
   	double NIncropManure= cropManure->GetAllN().n*cropManure->GetAmount()*1000.0; // kg N/ha
	   theOutput->AddStateIndicator(environmentalIndicator,"31.61 Total soil/soilsurface N","kg N",NIncropManure);
   	double CIncropManure= cropManure->GetC_content()*cropManure->GetAmount() * area *1000.0; // kg C/ha
	   theOutput->AddStateIndicator(environmentalIndicator,"40.60 Total C in soil","kg C",CIncropManure);
   }
	theEcosystem->GiveIndicators();
}


void patch::outputCropDetails(fstream *afile, bool header)
{
	theEcosystem->outputCropDetails(afile, header);
}

/****************************************************************************
Volatilise ammonia (only called if manure is present)
NJH Aug 2007 - Relic code but contains some features that we might wish to use later
void patch::Volatilise(double duration, bool kill)
{
   return; // Code presently not used in order to get movin' !!!!!!!!!!!!!!!!!!!!!!!!!

	double actualNH4_loss=0.0;
	double temperature, min_temp, max_temp, evap, infiltration, ppt, radiation,
                     windspeed, relHumidity, resistance, ra, rb, rCrop;
   if (soilManure)
   {
   	double Nin = soilManure->GetAllN().n*soilManure->GetAmount()*1000.0; // kg N/ha as soilManure is in t/ha
      manure *infiltManure = (manure *) soilManure->clone();
      // Below climate module is called, but not "ready" !!!
      theClimate->GetClimate(temperature, min_temp, max_temp, ppt, radiation, evap, windspeed, relHumidity);
      infiltration = 0.0;
      ppt /=24.0;  //convert to hourly rate
      evap/=24.0;
      // calculate infitration rate - this is a fudge!!!
      double currentWater = aSoil->GetAvailCapacity(aSoil->GetLayerStartDepth(0),aSoil->GetLayerStartDepth(1));
      double maxWater = aSoil->GetFieldCapacity(aSoil->GetLayerStartDepth(0),aSoil->GetLayerStartDepth(1));
      double diff = maxWater-currentWater;
      infiltration = diff * soilManure->GiveInfiltrationReduction()/24.0;

      double resistance = 0.0;
      if(windspeed<0.0001) windspeed=0.0001;
      rb=6.2*pow(windspeed,-0.67);				//Olesen & Summer 94  boundary layer res.
      double l=0.071;
      ra=(log(l/0.01))/(0.4*windspeed);		//------||--------    Aerodynamic res.
      rCrop=0.0;
      resistance=rb+ra+rCrop;                 		//rC is the crusts resistance to volatization

      // actualNH4loss is in tonnes per sq metre, as soilManure is in tonnes per ha
      double waterEvap = 0.0;
      actualNH4_loss=soilManure->VolatAmmonia(temperature, evap, infiltration, ppt, 10000.0, resistance, duration, &waterEvap, infiltManure);

      double remainingTAN = soilManure->GetAmount()*soilManure->GetNH4_content().n*1000.0; //amount remaining in kg per ha
      double remainTotN=(soilManure->GetAmount()*soilManure->GetAllN().n + infiltManure->GetAmount()*infiltManure->GetAllN().n)*1000.0;  //in kg per ha
      double test = Nin-(remainTotN+actualNH4_loss*1000.0);
      if (fabs(test)>0.1)
            theMessage->FatalError("patch::Volatilise - balance error");

      // If manure contains little TAN then absorb remainder of soilManure. Also absorb if soilManure killed (eg by ploughing)
      if ((remainingTAN<1.0) || (kill))
      {
      	*infiltManure + *soilManure;
         delete soilManure;
         soilManure = NULL;
      }

      if (infiltManure->GetAmount()>0.0)
      {
         decomposable d=(decomposable) *infiltManure;
         aSoil->AddDecomposable(&d);
         aSoil->Incorporate(0.01,0.02); // From NJH @@@ Seems a very small depth !!!???
         delete infiltManure;
      }
   }
   theOutput->AddIndicator(environmentalIndicator,"31.45 Ammonia-N loss by spreading","kg N",actualNH4_loss * area *1000.0);
}

****************************************************************************/

