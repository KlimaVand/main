/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Created by NJH - 25.02.00
  ----------------------------------------------------------------------------
      Log
 Describes a inhouse manure store
\****************************************************************************/
#include <common.h>
#include <instore.h>
#include <contrparm.h>
#ifndef _STANDALONE
	#include <indicat.h>
#endif

/****************************************************************************\
  Destructor
\****************************************************************************/
instore::~instore()
{
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
instore::instore(const char *aName,const int aIndex,const base* aOwner)
	: manurestore(aName,aIndex,aOwner), GenAsset()

{
	velocityConstant=0;
//   independent = false;
}

/****************************************************************************\
	Init reads the relevant values from its owners file
\****************************************************************************/

void instore::ReadParameters(fstream * file)
{
	manurestore::ReadParameters(file);
   Setfile(file);
   UnsetCritical();
   Setcur_pos(0);
   switch (theControlParameters->GetvolatModel())
   {
    case 0:
    {

     break;
    }
    case 1:
    {
      FindSection(aBase->GetName(),aBase->GetIndex()); // perform this in the top of hierarchy - and only there
      GetParameter("AreaPrAnimal",&AreaPrAnimal);   //this is declared in manurestore
      SetCritical();
      GetParameter("VelocityConstant",&velocityConstant);
      UnsetCritical();
      break;
    }
   }
  Setfile(NULL);
}


/****************************************************************************\
	Dailyactions
\****************************************************************************/

void instore::DailyUpdate()
{
   if (theStore->Getamountstored()>theStore->GetCapacity())
   	theMessage->FatalError(manurestore::GetName()," inhouse manure storage capacity exceeded");
}

/****************************************************************************\
Volatize gasses from the inhouse manure store - used for static (emission factor) modelling
\****************************************************************************/

void instore::Volatize(double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission,
		double *CO2Emission)
{
  double Nlost, Clost;
  manurestore::Volatize(&Nlost, &Clost,volatAmmonia, N2Emission, N2OEmission, CH4Emission, CO2Emission);
  Nbudget.AddOutput(Nlost);
  Cbudget.AddOutput(Clost);
}

/****************************************************************************\
Volatize nitrogen gasses from the floor - used for dynamic modelling
\****************************************************************************/

void instore::floorVolatize(double airVelocity, double humidity, double temperature, double areaSoiled, double duration,
		double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission)
{
//   EndBudget(false);
	*N2Emission=0.0;
   *N2OEmission=0.0;
   *volatAmmonia=0.0;
   duration*=24.0; //convert from days to hours
   manure * aManure=(manure *) theStore->GetProductPointer();
   double resistance = 1/(velocityConstant * pow(airVelocity+0.02,0.8) * pow(temperature+273,-1.4)); //Aarninck model from 1997
//   double resistance = 1/(50.1 * pow(airVelocity,0.8) * pow(temperature+273,-1.4)); //Aarninck model from 1997
   if (areaSoiled>surfaceArea)
   	areaSoiled=surfaceArea;
   if (areaSoiled>0.0)
   {
    double evaporation = aManure->GetWaterEvap(temperature, humidity, areaSoiled, resistance, duration); //total evap in tonnes during 'duration'
    evaporation*= 1000.0/(areaSoiled * duration);  //convert to kg per sq metre per sec
 	 *volatAmmonia = aManure->InstantaneousVolatAmmonia(temperature, areaSoiled, resistance, duration);
    aManure->doDenitrification(temperature, duration/24.0, N2Emission,N2OEmission);
// denitrification losses are here removed after ammonia volatilisation - this needs to be integrated later??
//	 aManure->GetCgasEmissions(temperature, duration, CO2Emission, CH4Emission);
    aManure->SubtractWater(evaporation * areaSoiled * duration/1000.0);

   }
   Nbudget.AddOutput(*volatAmmonia + *N2Emission + *N2OEmission);
   Cbudget.AddOutput(*CO2Emission + *CH4Emission);
//   EndBudget(false);
}

/****************************************************************************\
Volatize nitrogen gasses from the inhouse manure store - used for dynamic modelling
\****************************************************************************/

void instore::inhouseVolatize(double airVelocity, double humidity, double temperature, double duration,
		double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission)
{
   if (airVelocity>0.0)
   {
      duration*=24.0; //convert from days to hour
      manure * aManure=(manure *) theStore->GetProductPointer();
      if (aManure->GetAmount()>0)
      {
         double resistance = 1/(velocityConstant * pow(airVelocity,0.8) * pow(temperature+273,-1.4)); //Aarninck model from 1997
         double evaporation = aManure->GetWaterEvap(temperature, humidity, surfaceArea, resistance, duration);
         double infiltration = 0;
         double precipitation = 0;
         *volatAmmonia=aManure->VolatAmmonia(temperature, evaporation, infiltration, precipitation, surfaceArea, resistance, duration,
       				 &evaporation, NULL);
//         aManure->GetCgasEmissions(temperature,  duration, CH4Emission, CO2Emission);
         Nbudget.AddOutput(*volatAmmonia + *N2Emission + * N2OEmission);
         Cbudget.AddOutput(*CH4Emission + *CO2Emission);
   	}
   }
}

double instore::VolatizeDung(bool ventilated)
{
	double volat;
	if (ventilated)
   {
      manure * aManure=(manure *) theStore->GetProductPointer();
      volat = aManure->VolatNitrogen(NH3Emissionfactor);
//      double totalN = aManure->GetNH4_content().n*aManure->GetAmount();
//      aManure->SetNH4_content(aManure->GetNH4_content().n*(1-Emissionfactor));
//		volat=Emissionfactor * totalN/100.0;
   }
   else volat = 0.0;
   Nbudget.AddOutput(volat);
   cumvolat+=volat;
   return volat;
}


/****************************************************************************\
	GiveIndicator.
\****************************************************************************/

void instore::GiveIndicator(int indicatorType)
{
	if (indicatorType == economicIndicator)
			GenAsset::GiveEconomicIndicators();
}

/****************************************************************************\
	Scale instore to number of animals present, pass info to next store. Note cumulative effect on capacity
\****************************************************************************/
void instore::ScaleInstore(double annualAmount)
{
	scalable=true;
   double newCapacity = annualAmount + GetCapacity();
	Scale(newCapacity);  //scales capcity and area via manurestore
//   ActualCapacity = annualAmount;
   nextStore->Scale(annualAmount);
}

/****************************************************************************\
	Operator << for output
\****************************************************************************/

ostream& operator<< (ostream& os,const instore& s)
{
  os <<	"air velocity const " << s.velocityConstant << endl;
  return os;
}
