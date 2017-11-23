
#include "../base/common.h"
#include "patch.h"

#ifndef NITROSCAPE

#include "crops/cropWwheat.h"
#include "crops/cropSpringWheat.h"
#include "crops/cropBare.h"
#include "crops/cropWrape.h"
#include "crops/cropSrape.h"
#include "crops/cropSbarley.h"
#include "crops/cropWbarley.h"
#include "crops/cropRyegrass.h"
#include "crops/cropPea.h"
#include "crops/cropBeet.h"
#include "crops/cropMaize.h"
#include "crops/cropClover.h"
#include "crops/cropSeedgrass.h"
#include "crops/cropRye.h"
#include "crops/cropOat.h"
#include "crops/cropOilRadish.h"
#include "crops/cropDyerswoad.h"
#include "crops/cropChickory.h"
#include "crops/cropPotato.h"

#endif

#include "../products/organicProduct.h"
#include "../products/products.h"
#include "../base/climate.h"
#include "fieldOperation/animalFertFields.h"
#include "fieldOperation/AnimalOnField.h"
#include "fieldOperation/harvestFields.h"
#include "fieldOperation/irrigateFields.h"
#include "fieldOperation/mineralFertFields.h"
#include "fieldOperation/manualLabourFields.h"
#include "fieldOperation/sowFields.h"
#include "fieldOperation/soilTreatFields.h"
#include "fieldOperation/plantCareFields.h"
#include "fieldOperation/makeGrazable.h"
#include "fieldOperation/removeAnimalOnField.h"
#include "../base/controlParameters.h"
#include "../products/nitrogen.h"
#include "../technics/technics.h"
#include "../products/feedResource.h"

#include "ALFAM.h"
#include "../manager/manureManager.h"
#include "../livestock/livestock.h"
#include "../livestock/dkCattle.h"
#include "../livestock/cattleGroup.h"
#ifdef NITROSCAPE

#include "../Nitroscape/patchinterface.h"

#else

#include "ecosystem.h"

#endif

patch::patch(string       aName,
             const int    aIndex,
             const base * aOwner):
    base(aName,
         aIndex,
         aOwner)
{
    soilFileName      = "";
    anALFAM           = nullptr;

#ifdef NITROSCAPE
    thePatchInterface = new patchInterfaceClass("patch", aIndex, this);
#else
    theEcosystem.init("ecosystem", aIndex, this);
#endif

    area              = 0.0;
    grazable          = false;
    soilManure        = nullptr;
    cropManure        = nullptr;
    birthday          = new bsTime(theTime);
    dung              = false;
    overlap           = false;
    my_resource       = nullptr;
    type              = 0;
    MinDMtemp         = 2;       // Minimum temperature for dry matter production (4 in Hansen et al)
    MaxDMtemp         = 10;      // Maximum temperature for dry matter production
    PhotoSyntActPart  = 0.48;    // Photosynthetic active part of total global radiation
    MaxRadUseEff      = 3.33;    // Maximum radiation use efficiency (g/MJ)
}

patch::patch(const patch & p) {
    soilFileName = "";
    MinDMtemp    = MaxRadUseEff = MaxDMtemp = area = PhotoSyntActPart = 0.0;
    anALFAM      = new ALFAM();

#ifndef NITROSCAPE
    theEcosystem =p.theEcosystem;
#endif

    grazable     = p.grazable;
    dung         = p.dung;
    overlap      = p.overlap;
    type         = p.type;

    if (p.soilManure) {
        soilManure = new manure(*p.soilManure);
    } else {
        soilManure = nullptr;
    }

    if (p.cropManure) {
        cropManure = new manure(*p.cropManure);
    } else {
        cropManure = nullptr;
    }

    if (p.my_resource) {
        my_resource = new feedItem(*p.my_resource);
    } else {
        my_resource = nullptr;
    }

    if (p.birthday) {
        birthday = new bsTime(*p.birthday);
    } else {
        birthday = nullptr;
    }
}

patch::~patch() {

#ifdef NITROSCAPE
    delete thePatchInterface;
#else
    
#endif

    if (soilManure) {
        delete soilManure;
    }

    if (cropManure) {
        delete cropManure;
    }

    if (my_resource) {
        delete my_resource;
    }

    if (birthday) {
        delete birthday;
    }
}

/*
 * Returns a copy of this instance at the same inheritance level.
 * Used in connection with 'cloneList'
 */
patch * patch::clone() const {
    patch * p = new patch(*this);

    return p;
}

void patch::Initialise(string localSoilFileName,
                       int    aIndex,
                       double anArea) {
    soilFileName = localSoilFileName;

    commonData data;

    data.setCritical(false);
    data.readFile(soilFileName);
    data.FindSection("patchParameters");
    data.FindItem("MinDMtemp", &MinDMtemp);
    data.FindItem("MaxDMtemp", &MaxDMtemp);
    data.FindItem("PhotoSyntActPart", &PhotoSyntActPart);
    data.FindItem("MaxRadUseEff", &MaxRadUseEff);

    area = anArea;

#ifndef NITROSCAPE
    theEcosystem.Initialise(soilFileName, aIndex, anArea);
#else
    thePatchInterface -> SetArea(area);
#endif

}

#ifdef NITROSCAPE
void patch::zeroOutputVariables() {
    thePatchInterface -> zeroOutputVariables();
}
#endif

/*
 * Handles a patch operation (e.g. sowing)
 * Parameters:
 * year, month, day
 * FOP                object with operation characteristics
 */
void patch::HandleOp(fieldOperationFields * FOP) {

    /*
     *    crop* aCrop=nullptr;
     * if (theCrops->NumOfNodes()>0)
     * aCrop=theCrops->ElementAtNumber(0); // By default look at first crop;
     */
    if ((FOP -> GetOperationId() == CombineHarvesting) || (FOP -> GetOperationId() == CombineHarvestingShred)
            || (FOP -> GetOperationId() == BeetHarvesting)) {    // "Mowing" moved down to forage section NJH mar 07 !!!
        harvestFields * hrv = (harvestFields *) FOP;

        HarvestOperations(hrv);
    }

    if ((FOP -> GetOperationId() == SpreadingLiquidManure) || (FOP -> GetOperationId() == SpreadingSolidManure)) {
        animalFertFields * aFF       = (animalFertFields *) FOP;
        manure *           theManure = (manure *) aFF -> GetManure() -> clone();    // create copy of manure
        double             factor    = area * theManure_manager -> GetadjustmentFactor();

        // calculate absolute amount of manure  and adjust manure application (set by manure manager). Amount in tonnes or kg N, depending on mainUnit
        *theManure * factor;
        theProducts -> SubtractProduct(
            theManure);    // note that manure is removed from store and quality parameters are updated here

        if (area < 1E-36) 
		{
            theMessage -> FatalError("patch::HandleOp - patch has extremely low or zero area");
        } else {
            *theManure * (1.0 / area);                                                    // reconvert to tonnes per ha
        }

        double N_added = theManure -> GetAmount() * theManure -> GetAllN().n * 1000.0;    // kg per ha

        if (N_added < 1) {
            theMessage -> WarningWithDisplay("patch::HandleOp - very little or no N in manure");
        }

        theOutput -> AddIndicator(3106, "31.06 N from spread manure", "kg N", N_added * area);
        theOutput -> AddIndicator(1965, "19.65 Manure spread on field", "t", theManure -> GetAmount() * area);
        theOutput -> AddIndicator(4002, "40.02 C from field-applied manure", "kg C",
                                  theManure -> GetC_content() * theManure -> GetAmount() * area * 1000.0);

#ifdef TUPLE
        if (((field *) Owner) -> GetWriteTuples())
            ;

        ((field *) Owner) -> AddTuple(theTime.GetString2(), N_added * area, theManure -> GetAmount() * area,
                                      "FERTILISATION", "MANURE", 0);
#endif

        animalFertTech * aTechnique = ((animalFertTech *) theTechnics -> GetFieldTechnique(aFF));
        manureApplic *   applicTech = ((manureApplic *) aTechnique -> GetApplicationTech());

        /*
         *  --------- Should calculate canopy contamination here - stil missing !!!
         * if ((aCrop) && (aTechnique->GetcropContamination()))   //crop present and method will contaminate canopy
         * {
         * }
         * ------------------------------------------------------------------------
         */
        double   NH3volat = 0.0;    // volatilisation as N not NH3
        nitrogen TAN_conc = theManure -> GetNH4_content();

        // double TAN_amount = theManure->GetAmount()*TAN_conc.n;
        switch (theControlParameters -> GetvolatModel()) {
            case 0 :
                NH3volat = theManure -> VolatNitrogen(applicTech -> GetNH3emissionFactor());

                if (applicTech -> GetpropBuried() > 0.0) {
                    decomposable d = (decomposable) *theManure;

                    d * (applicTech -> GetpropBuried());
                    AddDecomposable(&d);
                    *theManure * (1.0 - applicTech -> GetpropBuried());
                }

                AddDecomposable(theManure);
                theOutput -> AddIndicator(3145, "31.45 NH3-N loss by spreading", "kg N", NH3volat * area * 1000.0);

                break;

            case 1 : {

                /*
                 * This code does not work properly. It should be upgraded and used to replace/modify the ALFAM approach
                 * if(applicTech->GetpropBuried()>0.0) {
                 * decomposable d=(decomposable) *theManure;
                 * d*(applicTech->GetpropBuried());
                 * aSoil->AddDecomposable(&d);
                 * aSoil->Incorporate(250.0,1.0);
                 * theManure * (1.0-applicTech->GetpropBuried());
                 * }
                 *
                 * if(applicTech->GetpropBuried()<1.0) {
                 * if (!soilManure)
                 * soilManure = (manure *) theManure->clone();
                 * else // If there is still manure left on the soil from a previous application
                 * {
                 * manure * aManure = (manure *) theManure->clone();
                 * soilManure + * aManure;
                 * delete aManure;
                 * }
                 * double exposureTime = aFF->GetExposeTime();
                 *
                 * if (exposureTime<24.0)  // have to handle volatilisation now, otherwise manure will be buried before volatilisation function is called
                 * Volatilise(exposureTime, true);
                 * }
                 */
                if (!soilManure) {
                    soilManure = (manure *) theManure -> clone();
                } else {

                    // If there is still manure left on the soil from a previous application
                    theMessage -> FatalError(
                        "patch::HandleOp - attempt to apply more manure, before volatilisation has ceased from the previous manure");
                }

                double aveTemperature, aveWindspeed;
                double initDM  = soilManure -> GetdryMatter() * 100.0;
                double initTAN = soilManure -> GetNH4_content().n * 1000.0;
                double appRate = soilManure -> GetAmount();

                if (appRate > 0.0) {
                    int typeOfManure = 0;

                    if (soilManure -> GetfromAnimal() == "pig") {
                        typeOfManure = 1;
                    }

                    theClimate -> CalcAverageClimateForPeriod(&theTime, 7, &aveTemperature,
                            &aveWindspeed);    // get climate average for next 7 days

                    anALFAM = new ALFAM();

                    int applicCode = anALFAM -> GetALFAMApplicCode(FOP -> GetOperationId());

                    anALFAM -> initialise(0, aveTemperature, aveWindspeed, typeOfManure, initDM, initTAN, appRate,
                                          applicCode, aFF -> GetExposeTime());
                }

                break;
            }

            default : {}
        }

        delete theManure;
    }

    if (FOP -> GetOperationId() == MineralFertilizing) {
        mineralFertFields * aMF = (mineralFertFields *) FOP;
        nitrogen            N1, NoFert, NH4_N, NH3Volat, NO3_N;    // NoFert is only used as a "zero" add

        N1 = (aMF -> GetFertilizer1() -> GetN_content()) * (aMF -> GetFertilizer1() -> GetAmount())
             / 10.0;                                               // Convert to g/m. Change to more general algorithm.

        // theOutput->AddIndicator(environmentalIndicator,"31.01 N from mineral fertilizer","kg N",N1.n*10.0*area);
        double NH4_fraction = aMF -> GetFertilizer1() -> GetNH4_fraction();
        double NH3VolatFrac = 0.03;    // /should be a parameter NJH Dec 2007

        NH4_N    = N1 * NH4_fraction;
        NH3Volat = NH4_N * NH3VolatFrac;

        theOutput -> AddIndicator(3148, "31.48 NH3-N loss from mineral fertiliser", "kg N", NH3Volat.n * area * 10.0);

#ifdef NITROSCAPE
        AddNH3Emission(NH3Volat.n * area * 10.0);
#endif

        NH4_N = NH4_N - NH3Volat;
        NO3_N = N1 * (1.0 - NH4_fraction);

        if (aMF -> GetFertilizer1() -> GetMineralForm() == "SOLID") {
            AddNutrient(NO3_N, NH4_N, NoFert, NoFert);
        } else {
            AddNutrient(NoFert, NoFert, NO3_N, NH4_N);
        }

#ifdef TUPLE
        if (((field *) Owner) -> GetWriteTuples())
            ;

        ((field *) Owner) -> AddTuple(theTime.GetString2(), N1.n * area * 10.0, 0, "FERTILISATION", "MINERAL_N", 0);
#endif

        if (aMF -> GetFertilizer1() -> GetCl_content() > 0) {
            AddChloride(aMF -> GetFertilizer1() -> GetCl_content() * aMF -> GetFertilizer1() -> GetAmount()
                        / 10.0);       // convert to g/m2
        }
    }

    if (FOP -> GetOperationId() == Irrigating) {
        irrigateFields * irr             = (irrigateFields *) FOP;
        water *          irrigationWater = irr -> GetWater();

        SetirrigationWater(irrigationWater);
    }

    if ((FOP -> GetOperationId() == Sowing) || (FOP -> GetOperationId() == SowingBeets)) {
        sowFields * S = (sowFields *) FOP;

        for (int i = 0; i < S -> GetSeedNum(); i++) {
            seed * aSeed = S -> GetSeed(i);

            if (aSeed -> GetAmount() < 1E-20) {
                cout << endl;

                // cout << "Field " << Owner->GetIndex() << " crop " << (char*)S->GetCropId(i) << endl;
                theMessage -> FatalError("patch::HandleOp - no seed amount in sowing");
            }

            double RelativeDensity = aSeed -> GetAmount() / aSeed -> GetStandardDensity();

            if ((RelativeDensity > 3.0) && (aSeed -> Getname() != "SEEDRYEGRASS")
                    && (aSeed -> Getname() != "SEEDCLOVER")) {
                cout << aSeed -> Getname() << " " << aSeed -> GetAmount() << " " << aSeed -> GetStandardDensity()
                     << endl;

                theMessage -> WarningWithDisplay(
                    "patch::HandleOp - seed amount more than three times higher than standard");
            }

            if ((RelativeDensity < 0.3333) && (aSeed -> Getname() != "SEEDRYEGRASS")
                    && (aSeed -> Getname() != "SEEDCLOVER") && (aSeed -> Getname() != "SEEDBEET")) {
                cout << aSeed -> Getname() << " " << aSeed -> GetAmount() << " " << aSeed -> GetStandardDensity()
                     << endl;

                theMessage -> WarningWithDisplay(
                    "patch::HandleOp - seed amount more than three times lower than standard");
            }

            // NJH May 2009, cropName allows variety to be passed via seed name for specific operations
            // commented out until we can get back to this system later         Sow((char*)S->GetSpecificCropId(i),(char*)S->GetCropName(),aSeed->GetdryMatter()*aSeed->GetAmount()/10.0,RelativeDensity,aSeed->GetAmount()*aSeed->GetN_content()/10.0); // Convert to g/m�

#ifndef NITROSCAPE
            Sow(S -> GetSpecificCropId(i), aSeed -> GetdryMatter() * aSeed -> GetAmount() / 10.0,
                RelativeDensity, aSeed -> GetAmount() * aSeed -> GetN_content() / 10.0,
                aSeed -> GetAmount() * aSeed -> GetC_content() / 10.0);    // Convert to g/m�
#else
            Sow(S -> GetSpecificCropId(i), S -> GetCropName(),
                aSeed -> GetdryMatter() * aSeed -> GetAmount() / 10.0, RelativeDensity,
                aSeed -> GetAmount() * aSeed -> GetN_content() / 10.0,
                aSeed -> GetAmount() * aSeed -> GetC_content() / 10.0);    // Convert to g/m�
#endif

            double N = aSeed -> GetAmount() * aSeed -> GetN_content() * area;

            theOutput -> AddIndicator(3103, "31.03 N from seed", "kg N", N);
            theOutput -> AddIndicator(4003, "40.03 C from seed", "kg C",
                                      aSeed -> GetAmount() * aSeed -> GetC_content() * area);

#ifdef TUPLE
            if (((field *) Owner) -> GetWriteTuples())
                ;

            ((field *) Owner) -> AddTuple(theTime.GetString2(), N, aSeed -> GetAmount() * area, "SOWING", "SEED", 0);
#endif

        }

        if (theControlParameters -> GetUseReductionParameters())

#ifndef NITROSCAPE
            theEcosystem.SetRUEfactor(
                S -> GetReduction());    // Use reductions depending on previous crop and level of pesticides.
#else
            theMessage -> WarningWithDisplay("patch::HandleOp - GetUseReductionParameters has been called!!");
#endif

    }

    if ((FOP -> GetOperationId() == CutSetAside) || (FOP -> GetOperationId() == ForageHarvesting)
            || (FOP -> GetOperationId() == MakeSilage) || (FOP -> GetOperationId() == Mowing)) {

        // BMP removed the following, but check what fate the beet top has now !!! || FOP->GetOperationId()==BeetHarvesting)
        harvestFields * hrv = (harvestFields *) FOP;

        HarvestOperations(hrv);

        // AddYieldToIndicators(presCrop,area*forage->GetAmount(),forageN.n*area*1000.0);
        // AddHarvestedAreaToIndicators(presCrop,area);
    }

    /*
     *  Code not made @@@ !!!
     * if (FOP->GetOperationId()==StartGrazing) // Simple grazing scheme BMP + JBE
     * {
     * autoGraze       = true;
     * nLoad           = ((grazingFields*)FOP)->GetNitrogenLoad();
     * grazingEffiency = ((grazingFields*)FOP)->GetGrazingEffiency();
     * }
     *
     * if (FOP->GetOperationId()==EndGrazing) // Simple grazing scheme BMP + JBE
     * {
     * autoGraze = false;
     * nLoad     = 0.0;
     * }
     */
    if (FOP -> GetOperationId() == Spraying) {
        plantCareFields * plantCare = (plantCareFields *) FOP;

        if (plantCare -> Get1stChemical() -> GetName()
                == "RoundUp") {    // Currently only chemical that affects plant growth
            KillAllCrops();
        }
    }

    if ((FOP -> GetOperationId() == GatheringBales) || (FOP -> GetOperationId() == GatheringCereal)
		|| (FOP->GetOperationId() == GatheringSilage) || (FOP->GetOperationId() == GatheringHay) || (FOP->GetOperationId() == StrawToField)) {
        harvestFields * hrv = (harvestFields *) FOP;

        HarvestOperations(hrv);
    }

    if ((FOP -> GetOperationId() == Ploughing) || (FOP -> GetOperationId() == StubHarrowing)) {
        soilTreatFields * TillageOp = (soilTreatFields *) FOP;

        Tillage(TillageOp);
    }
	if (FOP->GetOperationId() == PutAnimalonField)
	{
		AnimalonField * animalOp = (AnimalonField *)FOP;
		
		commonData * cattleGroupData = new commonData();
		cattleGroupData->readFile("livestock.dat");
		int anAnimalModel;
		cattleGroupData->setCritical(true);
		cattleGroupData->FindItem("AnimalModel", &anAnimalModel);
		string namestr = "DairyStock.Group";

		int first, num;

		cattleGroupData->getSectionsNumbers(namestr, &first, &num);    // count the number of sections associated with this instance
		cattleGroupData->setCritical(true);
		cattleGroup * acattlegroup=nullptr;
		bool foundIt=false;
		for (int inx = first; inx <= (num) && foundIt==false; inx++) {
			if (acattlegroup != nullptr)
				delete acattlegroup;
			acattlegroup = new cattleGroup(namestr, inx, this);

			acattlegroup->SetanimalModel(anAnimalModel);
			acattlegroup->ReadParameters(cattleGroupData);

			string aCattlerGroupName = acattlegroup->GetAlias();
			string animalType = animalOp->getTypeOfAnimal();

			if (aCattlerGroupName.compare(animalType)==0)
			{
				foundIt = true;

			}
		}
		delete cattleGroupData;
		if (foundIt == false)
			theMessage->FatalError("Could not find cattleGroup");

		commonData * data = new commonData();
		data->readFile("animals.dat");
		
			
				int * cattleId = new int(theLivestock->theCattleonField.size());
				const cattle * tmp = acattlegroup->buyNewAnimal(acattlegroup->getMinAge() * 365 + 1, cattleId, this);
				dk_cattle * tmpCattle = new dk_cattle(*(dk_cattle*)tmp);

				string temp = tmpCattle->GetAlias();
				tmpCattle->addfield(animalOp->fieldForAnimals);
				commonData * infile = new commonData();
				infile->readFile("cattlefeedplan.dat");
				first = -2, num=0;
				infile->getSectionsNumbers("Group", &first, &num); 
				linkList<diet> * theGroupDiets = new linkList<diet>();
				for (int i = first; i < num; i++)
				{
					infile->FindSection("Group", i);    // find the correct place in the input file
					string Alias;
					infile->FindItem("Alias", &Alias);
					if (Alias.compare(animalOp->getTypeOfAnimal())==0)
					{
						string  namestr = "Group(" + conv.IntToString(i) + ")." + "Period";
						int min = 0;
						int max = -1;
						infile->getSectionsNumbers(namestr, &min, &max);
						for (int k = min; k <= max; k++)
						{
							diet * aDiet = new diet(namestr, k, this);    // LEAK (BMP) !!!

							aDiet->ReadParameters(infile, false);
							theGroupDiets->InsertLast(aDiet);
						}
					}
				}
				diet *             theCurrentDiet=nullptr;
				diet * aDiet = nullptr;
				for (int j = 0; j < theGroupDiets->NumOfNodes(); j++) 
				{
					aDiet = theGroupDiets->ElementAtNumber(j);
					if (aDiet->GetstartDate()->GetJulianDay(aDiet->GetstartDate()->GetDay(),
							aDiet->GetstartDate()->GetMonth(), theTime.GetYear()) <= theTime.GetJulianDay())
					{
							theCurrentDiet = aDiet;
					}
				}
				if (theCurrentDiet == nullptr)
					theMessage->FatalError("no diet found");
				tmpCattle->SetfeedPlanList(theCurrentDiet->GetnormalfeedItemList());
			//	cout << theCurrentDiet->GetnormalfeedItemList()->ElementAtNumber(0)->GetAmount() << endl;;
				//if ((feedPlanList->NumOfNodes() == 0) && (fieldList->NumOfNodes() == 0))
				tmpCattle->SetNumber(animalOp->getNumberOfAnimals());
				theLivestock->theCattleonField.push_back(tmpCattle);
			
		
		


	}
	if (FOP->GetOperationId() == removeAnimalFromField)
	{
		removeAnimalOnField * animalOp = (removeAnimalOnField *)FOP;
		
		int amount = animalOp->getNumberOfAnimalsRemoved();
		string type = animalOp->getTypeOfAnimal();

		for (int i = 0; i < theLivestock->theCattleonField.size(); i++)
		{
			if (theLivestock->theCattleonField[i]->GetName().compare(type) == 0 && amount != 0)
			{
				int numberOfAnimalsonField = theLivestock->theCattleonField[i]->GetNumber();
				if (numberOfAnimalsonField>=amount)
				{
					int newNumberOfAnimal = numberOfAnimalsonField - amount;
					theLivestock->theCattleonField[i]->SetNumber(newNumberOfAnimal);
					amount = 0;
				}
				else
				{
					amount = amount - numberOfAnimalsonField;
					theLivestock->theCattleonField[i]->SetNumber(0);
				}
			//	theLivestock->theCattleonField[i]->SetNumber();
			}
			

		}
		
		if (amount != 0)
			theMessage->FatalError("to many animals to remove");
		vector<dk_cattle*>  keep;// = new vector<dk_cattle*>();
		for (int i = 0; i < theLivestock->theCattleonField.size(); i++)
		{
			int number = theLivestock->theCattleonField[i]->GetNumber();
			if (number > 0)
				keep.push_back(theLivestock->theCattleonField[i]);
		}
		theLivestock->theCattleonField = keep;
		
	}
	
}

/*
 *
 * Initialise budget variables
 *
 */
void patch::StartBudget() {

#ifndef NITROSCAPE
    theEcosystem.StartBudget();
#endif

}

/*
 * Check to see if budget is still valid
 */
bool patch::EndBudget() {

#ifdef NITROSCAPE
    return true;
#else
    return theEcosystem.EndBudget();
#endif

}

/*
 * Returns the nitrogen amount in the soil and the crop (kg/ha)
 */
double patch::NitrogenInSystem() {
    return NitrogenInSoil() + NitrogenInCrops();
}

/*
 * Returns the nitrogen amount in the soil (kg/ha)
 */
double patch::NitrogenInSoil() {

#ifdef NITROSCAPE
    theMessage -> FatalError("function not operative within NITROSCAPE");

    return 0.0;
#else
    double ret = theEcosystem.NitrogenInSoil();

    if (soilManure) {
        ret += soilManure -> GetAllN().n * 10.0;
    }

    ret = ret * area;

    return ret;
#endif

}

/*
 * Returns the nitrogen amount in the crops (kg/ha)
 */
double patch::NitrogenInCrops() {

#ifdef NITROSCAPE
    theMessage -> FatalError("function not operative within NITROSCAPE");

    return 0.0;
#else
    double ret = theEcosystem.NitrogenInCrops();

    if (cropManure) {
        ret += cropManure -> GetAllN().n * 10.0;
    }

    ret = ret * area;    // kg/ha

    return ret;
#endif

}

void patch::WriteNitrogen(double f) {

#ifdef NITROSCAPE
    theMessage -> FatalError("function not operative within NITROSCAPE");
#else
    if (soilManure) {
        cout << "N in soil manure " << soilManure -> GetAllN().n * f << endl;
    }

    if (cropManure) {
        cout << "N in crop manure " << cropManure -> GetAllN().n * f << endl;
    }

    theEcosystem.WriteNitrogen(f);
#endif

}

/*
 * Volatilise ammonia (only called if manure is present)
 */
void patch::Volatilise() {
    switch (theControlParameters -> GetvolatModel()) {
        case 1 :
            double volat = 0.0;    // volat in kg/ha

            if (anALFAM) {
                volat = anALFAM -> ALFAM_volatilisation(24.0);

                // cout << volat << endl;
                if (volat == 0.0) {
                    if (anALFAM -> GetIsfinished()) {
                        decomposable d = (decomposable) *soilManure;

                        AddDecomposable(&d);

                        delete soilManure;

                        soilManure = nullptr;

                        delete anALFAM;

                        anALFAM = nullptr;
                    }

                    // else   //user input says manure is incorporated before max time period is reached but there has been no incorporation
                    // theMessage->FatalError("patch:Volatilise - premature termination of volatilisation (missing incorporation)");
                } else {
                    soilManure -> SetNH4_content((soilManure -> GetNH4_content() * soilManure -> GetAmount()
                                                  - volat / 1000.0) / soilManure -> GetAmount());
                    theOutput -> AddIndicator(3145, "31.45 NH3-N loss by spreading", "kg N", volat * area);

#ifdef NITROSCAPE
                    AddNH3Emission(volat * area);
#endif

                }
            }

            break;
    }
}

/*
 * Called by the animal object to find out what is available
 * Returns quality and tonnes fresh weight per ha
 */
feedItem * patch::GetFeedResource(double bite_depth_factor,
                                  double min_stubble_ht,
                                  int    animalType) {
    if (my_resource) {
        delete my_resource;
    }

    switch (animalType) {
        case 1 : {
            my_resource = new feed_resource();    // the use if 'this' allows the animals to know which resource has provided the material

            ((feed_resource *) my_resource) -> SetIsSupplement(false);

            break;
        }

        default : {
            theMessage -> FatalError("ecosystem:GetFeedResource - not an allowed animal type");

            break;
        }
    }

    my_resource -> SetOwner(this);
    my_resource -> Setamount(0.0);
    my_resource -> SetCode(1000 + GetIndex());

    double grazing_height = bite_depth_factor * GetMaxPatchHeight();

    GetFeedResource(my_resource, grazing_height, min_stubble_ht, animalType);

    if (type > 1) {
        ((feed_resource *) my_resource) -> SetTime_since_dunged(GetPatchAge());
    } else {
        ((feed_resource *) my_resource) -> SetTime_since_dunged(0);
    }

    ((feed_resource *) my_resource) -> Setarea_occupied(GetArea());

    return my_resource;
}

/*
 * Get age of patch
 */
int patch::GetPatchAge() {
    int age = theTime.GetDateValue() - birthday -> GetDateValue();

    return age;
}

/*
 * Get mass of dry matter of patch (kg)
 */
double patch::GetPatchStandingDMkg() {
    return GetPatchStandingDMkgPerHa() * area;
}

/*
 * Get mass of dry matter of patch (kg/ha)
 */
double patch::GetPatchStandingDMkgPerHa() {
    return GetStandingDMdensity() * 10.0;
}

/*
 * Sets the crop variables that record the material grazed
 * Called by each animal that grazes this patch
 */
void patch::SetGrazed(double bite_depth_factor,
                      double DMGrazed,
                      double NGrazed) {
    DMGrazed /= GetArea() * 10.0;    // Convert from kg DM grazed to gDM per sq metre

    double grazing_height = bite_depth_factor * GetMaxPatchHeight();

    SendGrazed(grazing_height, DMGrazed, NGrazed);
}

/*
 * Add's two patches together
 */
void patch::Add(patch * addPatch,
                double  fraction) {
    if (soilManure) {
        *soilManure = *soilManure * (1.0 - fraction);
    }

    if (addPatch -> soilManure) {
        *(addPatch -> soilManure) = *(addPatch -> soilManure) * fraction;

        if (soilManure) {
            *soilManure = *soilManure + *(addPatch -> soilManure);
        } else {
        	//ck if null????   
            *soilManure = *(addPatch -> soilManure);
        }
    }

    if (cropManure) {
        *cropManure = *cropManure * (1.0 - fraction);
    }

    if (addPatch -> cropManure) {
        *(addPatch -> cropManure) = *(addPatch -> cropManure) * (1.0 - fraction);

        if (cropManure) {
            *cropManure = *cropManure + *(addPatch -> cropManure);
        } else {
           //ck if null????  
           *cropManure = *(addPatch -> cropManure);
        }
    }

    if (!overlap == addPatch -> overlap) {
        overlap = false;
    }

    if (!dung == addPatch -> dung) {
        dung = false;
    }

    if (!grazable == addPatch -> grazable) {
        grazable = false;
    }

    theMessage -> FatalError("patch::Add ecosystem not implemented!!");

    area += addPatch -> GetArea();

    birthday -> SetTime(theTime.GetDay(), theTime.GetMonth(), theTime.GetYear());
}

/*
 * Get amount grazed from this patch during the current period (kg)
 */
double patch::GetGrazedDM() {

#ifndef NITROSCAPE
    return theEcosystem.GetGrazedDM();
#else
    return 0.0;
#endif

}

/*
 * Returns growth in kg DM
 */
double patch::GetDailyDMProduction() {

#ifndef NITROSCAPE
    return theEcosystem.GetDailyDMProduction();
#else
    return 0.0;
#endif

}

/*
 * Developed by JBE + BMP
 */
void patch::SimpleGraze() {

#ifndef NITROSCAPE
    theEcosystem.SimpleGraze();
#endif

}

/*
 * Returns mean potential growth in kg DM/ha/day
 * Is fairly crude - assumes total interception + even distribution between crops
 */
double patch::GetPotentialGrowthRate(double radiation,
        double                              temperature) {

#ifndef NITROSCAPE
    return theEcosystem.GetPotentialGrowthRate(radiation, temperature);
#else
    double retVal = 0.0;
    double fTW;

    if (temperature <= MinDMtemp) {
        fTW = 0.0;
    } else if (temperature >= MaxDMtemp) {
        fTW = 1.0;
    } else {
        fTW = (temperature - MinDMtemp) / (MaxDMtemp - MinDMtemp);
    }

    double CO2conc = theClimate -> GetCO2Concentration();

    retVal = radiation * PhotoSyntActPart * MaxRadUseEff * exp(0.4537 - 170.97 / CO2conc)    // CO2 effect
             * fTW;

    return retVal * 10.0;    // converts from g/sq metre to kg/ha
#endif

}

void patch::UpdatePatches() {

#ifndef NITROSCAPE
    theEcosystem.WriteOutput();
#endif

    CropSoilExchange();

    if ((cropManure) || (soilManure)) {
        Volatilise();
    }
}

void patch::DepositManure(manure * animal_urine,
                          manure * animal_dung) {
    AddDecomposable(animal_dung);
    AddDecomposable(animal_urine);
}

void patch::GiveIndicators() {
    if (soilManure) {
        double NInsoilManure = soilManure -> GetAllN().n * soilManure -> GetAmount() * area * 1000.0;       // kg N/ha

        theOutput -> AddStateIndicator(3161, "31.61 Total soil/soilsurface N", "kg N", NInsoilManure);

        double CInsoilManure = soilManure -> GetC_content() * soilManure -> GetAmount() * area * 1000.0;    // kg C/ha

        theOutput -> AddStateIndicator(4060, "40.60 Total C in soil", "kg C", CInsoilManure);
    }

    if (cropManure) {
        double NIncropManure = cropManure -> GetAllN().n * cropManure -> GetAmount() * 1000.0;              // kg N/ha

        theOutput -> AddStateIndicator(3161, "31.61 Total soil/soilsurface N", "kg N", NIncropManure);

        double CIncropManure = cropManure -> GetC_content() * cropManure -> GetAmount() * area * 1000.0;    // kg C/ha

        theOutput -> AddStateIndicator(4060, "40.60 Total C in soil", "kg C", CIncropManure);
    }

    GiveEcoIndicators();
}

#ifdef NITROSCAPE
double patch::GetAvailablePatchStandingDMkg(double residualDM) {
    double retVal    = 0.0;
    double DMdensity = GetStandingDMdensity() * 10.0;    // kg DM/ha

    if (DMdensity > residualDM) {
        retVal = DMdensity - residualDM;
    }

    return retVal;
}
#endif

