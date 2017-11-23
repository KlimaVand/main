/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
 \****************************************************************************/
/****************************************************************************\
 Created by NJH - 2.6.99
 ----------------------------------------------------------------------------
 Log
 Describes a separate free-standing store)
 \****************************************************************************/
#include "../base/common.h"
#include "slurrystore.h"
#include "../base/IndicatorOutput.h"
#include "../base/climate.h"
#include "../base/controlParameters.h"
#include "../base/nixcbits.h"
#include "../base/settings.h"


/****************************************************************************\
  Destructor
 \****************************************************************************/
slurrystore::~slurrystore()
{
	external = true;
}

/****************************************************************************\
  Constructor with arguments
 \****************************************************************************/
slurrystore::slurrystore(char *aName, const int aIndex, const base* aOwner,commonData *  &runData,commonData * &orignal) :
	manurestore(aName, aIndex, aOwner, runData, orignal), building(aName, aIndex, aOwner, runData, orignal)
{


	kOMi = 0;
	kOMs = 0;
	kOMf = 0;
	kOMvfa = 0;
	OMi.Clear();
	OMs.Clear();
	OMf.Clear();
	OMvfa.Clear();


	runData->setCritical(true);
	runData->FindSection(Name,Index);
	runData->FindItem("Alias",&Alias);
	runData->setCritical(false);
	orignal->setCritical(true);
	string sectionNameOrg(aName);
	int indexInOld;
		string place;
		orignal->findSection(aName,0,"Alias",Alias,&place,&indexInOld);
		orignal->FindSection(place,indexInOld);

	orignal->FindItem("AreaPrAnimal",&AreaPrAnimal);
	runData->FindItem("AreaPrAnimal",&AreaPrAnimal);

	orignal->FindItem("CrustResistance",&crustResistance);
		runData->FindItem("CrustResistance",&crustResistance);

	orignal->FindItem("SpecificResistance",&specificResistance);
	runData->FindItem("SpecificResistance",&specificResistance);

	orignal->FindItem("CoverResistance",&coverResistance);
	runData->FindItem("CoverResistance",&coverResistance);

	if (globalSettings[id]->DetailsData.getManureManageDetails() == true)
		OpenDetailsFile();
	else
		detailsFile = NULL;
}

/****************************************************************************\
  Copy Constructor
 \****************************************************************************/
slurrystore::slurrystore(slurrystore& s) :
	manurestore(s), building(s)
{
	crustResistance = s.crustResistance; //crust resistance to volatization
	specificResistance = s.specificResistance; //building spec. resistance to volatization
	coverResistance = s.coverResistance; //Covers on slurrystore resistance --||--
	ALFAMtype = s.ALFAMtype;
	kOMi = 0;
	kOMs = 0;
	kOMf = 0;
	kOMvfa = 0;
	OMi.Clear();
	OMs.Clear();
	OMf.Clear();
	OMvfa.Clear();
}



/**
 * Dailyactions that updates volatAmmonia, N2Emission, N2OEmission, CO2Emission and CH4Emission. Does update Indicat-file with that information
 */
void slurrystore::DailyUpdate()
{
	/*     fstream * filehandle = theMessage[id]->GiveDebugHandle();
	 *filehandle << " storeV " << GetAmountManure() << " storeN " << GetTotalNStored() << " ";
	 theMessage[id]->CloseDebugHandle();*/
	building::DailyUpdate();
	double volatAmmonia = 0.0, N2Emission = 0.0, N2OEmission = 0.0,
			CH4Emission = 0.0, CO2Emission = 0.0;

	switch (theControlParameters[id]->GetvolatModel())
	{
	case 0:
	{
		double Nlost, Clost;
		Volatize(&Nlost, &Clost, &volatAmmonia, &N2Emission, &N2OEmission,
				&CH4Emission, &CO2Emission);
		Nbudget.AddOutput(Nlost);
		Cbudget.AddOutput(Clost);
	}
		break;
	case 1:
		VolatizeSlurry(&volatAmmonia, &N2Emission, &N2OEmission, &CO2Emission,
				&CH4Emission);
		break;
	}
	manurestore::DailyUpdate(volatAmmonia, N2Emission, N2OEmission,
			CO2Emission, CH4Emission);
	theOutput[id]->AddIndicator(
			3543,"35.43 NH3-N volatilisation from manure storage", "kg N",
			volatAmmonia * 1000.0);
	theOutput[id]->AddIndicator(
			3544,"35.44 N2-N volatilisation from manure storage", "kg N", N2Emission
					* 1000.0);
	theOutput[id]->AddIndicator(
			3545,"35.45 N2O-N volatilisation from manure storage", "kg N",
			N2OEmission * 1000.0);
	theOutput[id]->AddIndicator(
			3543,"45.43 CO2-C volatilisation from manure storage", "kg C",
			CO2Emission * 1000.0);
	theOutput[id]->AddIndicator(
			3544,"45.44 CH4-C volatilisation from manure storage", "kg C",
			CH4Emission * 1000.0);
	GiveIndicator(environmentalIndicator);

}

/**
 * Does callculate volatAmmonia, N2Emission ,N2OEmission, CO2Emission and CH4Emission
 */
void slurrystore::VolatizeSlurry(double *volatAmmonia, double *N2Emission,
		double *N2OEmission, double *CO2Emission, double *CH4Emission)
{
	if (GetAmountInManureStore() > 0.0)
	{
		double waterEvap = 0.0;
		double rb, ra, resistance;

		double Ameantemp, Amin_temp, Amax_temp, Aprecip, Aradiation, AEPot,
				Awindspeed, Ahumidity;

		theClimate[id]->GetClimate(Ameantemp, Amin_temp, Amax_temp, Aprecip,
				Aradiation, AEPot, Awindspeed, Ahumidity);

		if (Awindspeed < 0.0001)
			Awindspeed = 0.0001;

		double height = 2.0;
		double displacementHt = 0.6 * height;
		double zeroPlane = 0.001;
		double ustar = GetFrictionVelocity(height, displacementHt, zeroPlane,
				Awindspeed);
		rb = 6.2 * pow(ustar, -0.67); //Olesen & Summer 94  boundary layer res.   86400 (s->day) inserted by JB

		ra = (log(height / zeroPlane)) / (GetvonKarman() * ustar); //------||--------    Aerodynamic res.
		resistance = rb + ra + coverResistance + crustResistance
				+ specificResistance; //rC is the crusts resistance to volatization

		manurestore::Volatize(Ameantemp, Ahumidity, Aprecip, surfaceArea,
				resistance, volatAmmonia, &waterEvap, N2Emission, N2OEmission,
				CO2Emission, CH4Emission);

		/*   *theMessage[id]->GiveDebugHandle() << theTime[id].GetDay() << "," << theTime[id].GetMonth() << "," << theTime[id].GetYear() << ","
		 << Ameantemp << "," << volat*1000.0 << endl;
		 theMessage[id]->CloseDebugHandle();*/
	}
}

/****************************************************************************\
\****************************************************************************/
void slurrystore::GiveIndicator(int indicatorType)
{
	if (indicatorType == economicIndicator)
		building::GiveIndicator(indicatorType);
	if (indicatorType == environmentalIndicator)
	{
		double Vt = 0, N = 0, C = 0;
		manure * aManure = (manure *) theStore->GetProductPointer();

		if (aManure)
		{
			Vt = aManure->GetAmount(); //The total amount of slurry in kg
			N = Vt * aManure->GetTotalN().n; //The total N-fraction
			C = Vt * aManure->GetC_content();
		}

		theOutput[id]->AddStateIndicator(
				3560,"35.60 N bound in manure storage", "kg N", N * 1000.0);
		theOutput[id]->AddStateIndicator(
				4560,"45.60 C bound in manure storage", "kg C", C * 1000.0);
		theOutput[id]->AddStateIndicator(
				1170,"11.70 Slurry in slurrystore", "t", Vt / 1000);
	}
}

/**
 * Calls manurestore's routine - is defined in slurrystore to help in debugging
 */
void slurrystore::RcvManure(const manure* someManure)
{
	double aAmount;
	double inputC_OMi, inputC_OMs, inputC_OMf, inputC_OMvfa;
	double inputN_OMi, inputN_OMs, inputN_OMf;
	double const CNratio_OMi = 10.0;

	aAmount = someManure->GetAmount();
	inputC_OMi = aAmount * (1 - someManure->GetAOM1_fraction() - someManure->GetAOM2_fraction());
	inputC_OMs = aAmount * someManure->GetAOM1_fraction();
	inputC_OMf = aAmount * someManure->GetAOM2_fraction() * (1 - someManure->GetVFA_fraction());
    inputC_OMvfa = aAmount * someManure->GetAOM2_fraction() * someManure->GetVFA_fraction();

    inputN_OMi = inputC_OMi/CNratio_OMi;
    /* necessary to update TAN - either some N is removed or added to TAN to ensure CNratio = 10 in OMi */
    inputN_OMs = aAmount * someManure->GetAOM1_Nfraction();
    inputN_OMf = aAmount * someManure->GetAOM2_Nfraction() * (1 - someManure->GetVFA_fraction());


	cnMatter *inputcnmatter;
	inputcnmatter = new cnMatter(inputC_OMi, 0.0, inputN_OMi, 0.0);
	OMi + *inputcnmatter;

	inputcnmatter = new cnMatter(inputC_OMs, 0.0, inputN_OMs, 0.0);
	OMs + *inputcnmatter;

	inputcnmatter = new cnMatter(inputC_OMf, 0.0, inputN_OMf, 0.0);
	OMf + *inputcnmatter;

	inputcnmatter = new cnMatter(inputC_OMvfa, 0.0, 0.0, 0.0);
	OMvfa + *inputcnmatter;

	delete inputcnmatter;



	manurestore::RcvManure(someManure);
	theOutput[id]->AddIndicator(1972, "19.72 Slurry produced", "t",
			someManure->GetAmount());
	//   if (theStore->Getamountstored()>theStore->GetCapacity())
	// 	theMessage[id]->FatalError(manurestore::GetName()," slurry storage capacity exceeded");
}

/**
 * 	Daily turnover of organic matter in the pools
 */
void slurrystore::organicMatterDynamics()
{
	double decay_C=0.0, tempDecay_C = 0.0;
	double const gOMi = 0.2;					// fraction of decayed C returned to OMi
	double const gOMvfa = 0.4;					// fraction of decayed C returned to OMvfa
	double const kOMs_std = 0.003;				// Decay rate of OMs at standard conditions

	double Ameantemp, Amin_temp, Amax_temp, Aprecip, Aradiation, AEPot,
			Awindspeed, Ahumidity;

	theClimate[id]->GetClimate(Ameantemp, Amin_temp, Amax_temp, Aprecip,
			Aradiation, AEPot, Awindspeed, Ahumidity);

	kOMs = kOMs_std*Kirschbaum(slurryTemperature());
	kOMf = 10.0 * kOMs;
	kOMvfa = 10.0 * kOMs;


	double OMs_C = OMs.GetTotalC();
	tempDecay_C = OMs_C * kOMs;			// Decay of C from OMs
	OMs_C -= tempDecay_C;
	decay_C += tempDecay_C;

	double OMf_C = OMf.GetTotalC();
	tempDecay_C = OMf_C * kOMf;			// Decay of C from OMf
	OMf_C -= tempDecay_C;
	decay_C += tempDecay_C;

	double OMi_C = OMf.GetTotalC();		// The fraction gOMi of the decayed C is returned to OMi
	OMi_C += decay_C*gOMi;

	double OMvfa_C = OMvfa.GetTotalC();		// The fraction gOMvfa of the decayed C is returned to OMvfa
	OMvfa_C += decay_C*gOMvfa;



}

double slurrystore::slurryTemperature()
{
	// need to implement the slurry temperature model. Meanwhile:
	return 10.0;

}

double slurrystore::methaneEmissionFraction(double temperature)
{
	double const a = -0.0522;
	double const b = 1.04;
	double const c = 17.8;
	double const d = 3.09;
	double const PI = 3.1415926;

	double fCH4 = a + b*(atan((temperature)-c)/d + PI/2)/PI;
	return fCH4;
}

double slurrystore::Kirschbaum(double temperature)
{
	double const Kirschbaum1 = -3.432;
	double const Kirschbaum2 = 0.186;
	double const KirschbaumOpt = 36.9;
	double const temperatureAdjust = 0.1614; // adjustment so the function returns 1.0 for temperature = 10.0 C

    return temperatureAdjust*exp(Kirschbaum1+Kirschbaum2*temperature*(1.0-0.5*temperature/KirschbaumOpt));
}

/**
 * 	Scale the manure storage that receives material from this instore  Note the cumulative effect on capacity
 */
void slurrystore::Scale(double annualAmount)
{
	scalable = true;
	double newCapacity = annualAmount + GetCapacity();
	manurestore::Scale(newCapacity); //scales capcity and area via manurestore
}

/****************************************************************************\
	Operator << for output
 \****************************************************************************/
ostream& operator<<(ostream& os, slurrystore s)
{
	/* os << setiosflags(ios::left) << setw(40) << s.rT << endl;
	 os << setiosflags(ios::left) << setw(40) << "Cover resistance " << s.rCover << endl;*/
	return os;
}

