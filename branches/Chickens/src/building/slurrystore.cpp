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

#include "../data/outputData.h"

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
slurrystore::slurrystore(char *aName, const int aIndex, const base* aOwner) :
	manurestore(aName, aIndex, aOwner), building(aName, aIndex, aOwner)
{
	crustResistance = 0; //crust resistance to volatization
	specificResistance = 0; //building spec. resistance to volatization
	coverResistance = 0; //Covers on slurrystore resistance --||--
	kOMi = 0;
	kOMs = 0;
	kOMf = 0;
	kOMvfa = 0;
	OMi.Clear();
	OMs.Clear();
	OMf.Clear();
	OMvfa.Clear();
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
 * Init reads the relevant values from its owners file
 */
void slurrystore::ReadParameters(fstream * file)
{
	manurestore::ReadParameters(file);
	building::ReadParameters(file);
	Setfile(file);
	FindSection(aBase->GetName(), aBase->GetIndex()); // perform this in the top of hierarchy - and only there
	GetParameter("AreaPrAnimal", &AreaPrAnimal);
	GetParameter("CrustResistance", &crustResistance);
	GetParameter("SpecificResistance", &specificResistance);
	GetParameter("CoverResistance", &coverResistance);
//	GetParameter("OrganicMatterInert", &OMi);
	/* etc */
	Setfile(NULL);
	if (theOutputData->DetailsData.getManureManageDetails() == true)
		OpenDetailsFile();
	else
		detailsFile = NULL;
}

/**
 * Dailyactions that updates volatAmmonia, N2Emission, N2OEmission, CO2Emission and CH4Emission. Does update Indicat-file with that information
 */
void slurrystore::DailyUpdate()
{
	/*     fstream * filehandle = theMessage->GiveDebugHandle();
	 *filehandle << " storeV " << GetAmountManure() << " storeN " << GetTotalNStored() << " ";
	 theMessage->CloseDebugHandle();*/
	building::DailyUpdate();
	double volatAmmonia = 0.0, N2Emission = 0.0, N2OEmission = 0.0,
			CH4Emission = 0.0, CO2Emission = 0.0;

	switch (theControlParameters->GetvolatModel())
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
	theOutput->AddIndicator(environmentalIndicator,
			"35.43 NH3-N volatilisation from manure storage", "kg N",
			volatAmmonia * 1000.0);
	theOutput->AddIndicator(environmentalIndicator,
			"35.44 N2-N volatilisation from manure storage", "kg N", N2Emission
					* 1000.0);
	theOutput->AddIndicator(environmentalIndicator,
			"35.45 N2O-N volatilisation from manure storage", "kg N",
			N2OEmission * 1000.0);
	theOutput->AddIndicator(environmentalIndicator,
			"45.43 CO2-C volatilisation from manure storage", "kg C",
			CO2Emission * 1000.0);
	theOutput->AddIndicator(environmentalIndicator,
			"45.44 CH4-C volatilisation from manure storage", "kg C",
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

		theClimate->GetClimate(Ameantemp, Amin_temp, Amax_temp, Aprecip,
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

		/*   *theMessage->GiveDebugHandle() << theTime.GetDay() << "," << theTime.GetMonth() << "," << theTime.GetYear() << ","
		 << Ameantemp << "," << volat*1000.0 << endl;
		 theMessage->CloseDebugHandle();*/
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

		theOutput->AddStateIndicator(environmentalIndicator,
				"35.60 N bound in manure storage", "kg N", N * 1000.0);
		theOutput->AddStateIndicator(environmentalIndicator,
				"45.60 C bound in manure storage", "kg C", C * 1000.0);
		theOutput->AddStateIndicator(economicIndicator,
				"11.70 Slurry in slurrystore", "t", Vt / 1000);
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
	theOutput->AddIndicator(economicIndicator, "19.72 Slurry produced", "t",
			someManure->GetAmount());
	//   if (theStore->Getamountstored()>theStore->GetCapacity())
	// 	theMessage->FatalError(manurestore::GetName()," slurry storage capacity exceeded");
}

/**
 * 	Daily turnover of organic matter in the pools
 */
void slurrystore::organicMatterDynamics()
{
	double decay_C, tempDecay_C = 0.0;
	double const gOMi = 0.2;					// fraction of decayed C returned to OMi
	double const gOMvfa = 0.4;					// fraction of decayed C returned to OMvfa
	double const kOMs_std = 0.003;				// Decay rate of OMs at standard conditions

	double Ameantemp, Amin_temp, Amax_temp, Aprecip, Aradiation, AEPot,
			Awindspeed, Ahumidity;

	theClimate->GetClimate(Ameantemp, Amin_temp, Amax_temp, Aprecip,
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

	double hCH4 = methaneEmissionFraction(slurryTemperature());
	double gEmm = 1.0 - gOMi - gOMvfa;	// fraction of decayed C emitted as CO2 or CH4
	double EmmCH4 = decay_C*gEmm*(hCH4) + kOMvfa*OMvfa_C/2;
	double EmmCO2 = decay_C*gEmm*(1.0-hCH4) + kOMvfa*OMvfa_C/2;

	/* Emission: Budget for CH4 and CO2?
	 */

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

void slurrystore::ReadDefaultParameters()
{
	if (OpenInputFile("StandardBuildings.dat"))
	{
//   verbosity =true;
	   SetCritical();
	   building::ReadDefaultParameters();
	   FindSection("Slurrystore");
		GetParameter("Alias",Alias);
		GetParameter("CrustResistance",&crustResistance);
		GetParameter("SpecificResistance",&specificResistance);
		GetParameter("CoverResistance",&coverResistance);
		GetParameter("NH3Emissionfactor",&NH3Emissionfactor);
		GetParameter("N2OEmissionfactor",&N2OEmissionfactor);
		GetParameter("N2Emissionfactor",&N2Emissionfactor);
		GetParameter("CO2Emissionfactor",&CO2Emissionfactor);
		GetParameter("CH4Emissionfactor",&CH4Emissionfactor);
		//GetParameter("CMinfactor",&CMinfactor);
		GetParameter("NMinfactor",&NMinfactor);
		theStore->ReadDefaultParameters(file);
		UnsetCritical();
		CloseInputFile();
//   verbosity =false;
	}
}

