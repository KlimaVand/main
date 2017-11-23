/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "manure.h"
#include "../base/climate.h"
#ifdef _STANDALONE
	#include "../base/message.h"
#endif

//double degrate = 0.00001;
/****************************************************************************\
  Constructor
\****************************************************************************/
manure::manure()
   : decomposable()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
manure::manure(const char * aName, const int aIndex, const base * aOwner)
   : decomposable(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
manure::manure(const manure& amanure)
   : decomposable(amanure)
{
   Mg_content   = amanure.Mg_content;
   Cu_content   = amanure.Cu_content;
   Rt_content   = amanure.Rt_content;
   dryMatter    = amanure.dryMatter;
   sand 		    = amanure.sand;

   pH 		    = amanure.pH;
   infiltFactor = amanure.infiltFactor;
   canopyStorageParameter = amanure.canopyStorageParameter;
   maxDeg = amanure.maxDeg;
   degRateConst  = amanure.degRateConst;
   degradableOM  = amanure.degradableOM;
}

/****************************************************************************\
  Copy Constructor - with decomposable (used when adding bedding)
\****************************************************************************/
manure::manure(const decomposable& aDecomp)
   : decomposable(aDecomp)
{
   SetObjType(manureObj); //make sure right object type (could have been changed if there is more than one floor
   Mg_content   = 0.0;
   Cu_content   = 0.0;
   Rt_content   = 0.0;
   sand 		    = 0.0;
   pH 		    = 7.0;

   infiltFactor = 0.0;
   canopyStorageParameter=0.0;
   maxDeg = 0.20;
   degRateConst = 0.0;      //njh - may 2009 - was commented out but lack of initialiation caused memory error
   degradableOM = maxDeg * GetOrgMatterConc();
}
/****************************************************************************\
 Destructor
\****************************************************************************/
manure::~manure()
{
}

/****************************************************************************\
\****************************************************************************/
void manure::InitVariables()
{
	decomposable::InitVariables();
   Mg_content   = 0.0;
   Cu_content   = 0.0;
   Rt_content   = 0.0;
   sand 		    = 0;

   pH 		    = 7.0;
   infiltFactor = 0.0;
   canopyStorageParameter=0.0;
   ObjType	  = manureObj;
   maxDeg = 0.2;
   degRateConst = 0.0;      //njh - may 2009 - was commented out but lack of initialiation caused memory error
   degradableOM = maxDeg * GetOrgMatterConc();
}

/****************************************************************************\
  GetQ(temp) returns the equilibrium constant Q described in
  [Hutchings et. al 95] pp590. given the temperature of the manure
\****************************************************************************/
double manure::GetQ(double temp) const
{
 	double Khx,Knh4x,PHx,Q;
   PHx=GetpH();
 	Khx=pow(10,(-1.69+(1477.7/(temp+273))));
   Knh4x=1+pow(10,(0.09018+(2729.92/(temp+273))-PHx));
   Q=Khx*Knh4x;
   return Q;
}

/*
 * Remove water from Manure. Does also update, Mg_content, Cu_content, Rt_content, sand and degradableOM
 */
double manure::SubtractWater(double waterToSubtract)
{
 double water = GetAmount()*(1-GetdryMatter());
 if (waterToSubtract!=0.0)
 {
    double DM = GetAmount()*GetdryMatter();
    if (fabs(water-waterToSubtract)<0.00000001)
    	waterToSubtract=water;   //to avoid false error caused by precision in double
    if ((water-waterToSubtract)<0)
      waterToSubtract=water;
   water-=waterToSubtract;
   if (DM!=0.0)   //is not clean water
   {
      Mg_content   = GetMg_content() * GetAmount()/(water+DM);
      Cu_content   = GetCu_content() * GetAmount()/(water+DM);
      Rt_content   = GetRt_content() * GetAmount()/(water+DM);
      sand 	     	 = Getsand() * GetAmount()/(water+DM);
      degradableOM  = GetdegradableOM() * GetAmount()/(water+DM);
      liveMatter::SubtractWater(waterToSubtract);
   }
   else
      Setamount(water);
 }
 return waterToSubtract;
}



/****************************************************************************\
\****************************************************************************/
double manure::VolatAmmonia(double temperature, double evap, double infiltration, double precipitation,
		double area, double resistance, double duration, double *volEvap, manure *toSoil)
{  //returns volatilisation in tonnes
	//temperature in K, infilt, precip and evap in (kg per sq metre per hr (mm/hr), area in sq metres and resistance in s per metre
   double volat=0;
/*
  Instantaneous flux (g/sq m/s) = (1/resistance) * Kh * 14 * [TAN]/(R*(temperature+273) * (1+pow(10,-pH)/KN))
  [TAN] =  NH4_content *amount * 1000/(14* amount *(1-dryMatter))   kg mol per litre
  assumes 1 litre=1 kg, molecular wt of N (14) cancels out
  Note - operates on hourly not per second basis; resistance input still in per second basis
*/
   if (!(toSoil)&& (infiltration >0.0))
         theMessage->FatalError("manure:: Infiltration of manure >0 without receiptor object defined");
   if (toSoil)
   {
 		toSoil->Setamount(0.0);
      toSoil->SetDryMatter(0.0);
      toSoil->SetorgN_content(0.0);   //DM and organic N is retained on surface
   }

	if (amount>0.0)
   {
   	if ((pH<1.0)||(pH>12.0))
      theMessage->FatalError("pmanure: volatilisation error in pH values");
	   double loss=0.0;
      double TANtoSoil = 0.0;
      double totalNInput = GetAllN().n * amount;
      double volInfiltration = infiltration * duration/1000.0;  //convert from mm to tonnes per sq metre
      *volEvap= evap * duration/1000.0;  //convert from mm to tonnes per sq metre
      double startTAN = NH4_content.n*amount * 1000000.0/area;   //in grams per sq metre
   	double TANPresent = startTAN;

      double volume = amount * (1-dryMatter) * 1000.0/area;  // litres/m**2  == mm
      double KN=exp(-177.95292 - (1843.22/(temperature+273.15)) + 31.4335*log(temperature+273.15)
      				- 0.0544943*(temperature+273.15));  //no units
      double Kh=exp(160.559-(8621.06/(temperature+273.15))
      				- 25.6767*log(temperature+273.15)+0.035388*(temperature+273.15));  // atm litre /mol
      double R=0.0000820575;  //universal gas constant in m**3 atm/(mol K)
      double KH=14.0 * Kh/(R*(temperature+273.15));   //g litre/(m**3 mol)
      if (resistance==0)
         theMessage->FatalError("manure:: The transfer resistance for ammonia loss cannot be zero");
      double q = 3600.0 * KH/((1+pow(10,-pH)/KN)*resistance * volume * 14.0); //rate constant, per hour
      if (duration * (infiltration + evap - precipitation)>=volume) //dries completely
      {
	         //theMessage->FatalError("manure:: volatilisation - do not use this without checking if it works");
            volInfiltration = volume * (infiltration/(evap + infiltration))/1000.0;  //convert from mm to tonnes per sq metre;
            *volEvap = volume * (evap/(evap + infiltration))/1000.0;  //convert from mm to tonnes per sq metre;
            volat=TANPresent * (evap/(evap + infiltration));
            TANtoSoil =TANPresent * (infiltration/(evap + infiltration));
            loss = volat + TANtoSoil;
      }
      else if ((infiltration + evap - precipitation)==0) //do not use function - get divide by zero error
         {
            volat=TANPresent* (1-exp(-q*duration));           // volume is constant - use exponential
            TANtoSoil=TANPresent * (1-exp(-infiltration*duration));           // volume is constant - use exponential
            loss = volat + TANtoSoil;
         }
         else
         {
//	         theMessage->FatalError("manure:: volatilisation - do not use this without checking if it works");
            volat=TANPresent* (1-exp(-q*duration));           // volume is constant - use exponential
            TANtoSoil=TANPresent * (1-exp(-infiltration*duration));           // volume is constant - use exponential
            loss = volat + TANtoSoil;
/*            double fact = evap - precipitation;
            double f1= 1-(infiltration+fact)*duration/volume;
            double f2= q/(infiltration+fact);
            double f3= infiltration/(infiltration+fact);
            double volatFunction = pow(f1,f2);
            double toSoilFunction = pow(f1,f3);
            loss = TANPresent * (1- volatFunction * toSoilFunction);
            TANtoSoil = loss * (1-toSoilFunction)/((1-toSoilFunction)+(1-volatFunction));
            volat = loss * (1-volatFunction)/((1-toSoilFunction)+(1-volatFunction));*/
         }
    if ((volat<0)||(TANtoSoil<0))
         theMessage->FatalError("manure:: Volatilisation less than zero");
    double newNH4_content;
    volat *= area/1000000.0;       //convert from grams per sq metre to tonnes
    loss *= area/1000000.0;
    TANtoSoil *= area/1000000.0;
    startTAN *= area/1000000.0;
    volume *= area/1000000.0;
    volInfiltration*=area;
    *volEvap*=area;
    if (loss>startTAN)
    {
		volat = startTAN * volat/(volat + TANtoSoil);
		TANtoSoil = startTAN * TANtoSoil/(volat + TANtoSoil);
    	loss = volat +TANtoSoil;
      newNH4_content = 0.0;
    }
    else
    {
    	SubtractWater(volInfiltration + (*volEvap));
      newNH4_content = (startTAN-loss)/amount;
    }
    double balance =  startTAN -(newNH4_content*amount +loss);
    if (fabs(balance)>0.01)
         theMessage->FatalError("manure:: Balance error in volatilisation");

    NH4_content = newNH4_content;   //sets new ammonium conc in pool

	 double totalNToSoil = 0.0;
    if ((toSoil) && (infiltration >0.0))
    {
    	toSoil->Setamount(volInfiltration);
    	toSoil->SetNH4_content(TANtoSoil/toSoil->GetAmount());     //water budgeting may not be correct!
      totalNToSoil = toSoil->GetAllN().n * toSoil->GetAmount();
    }
    double Nremaining = GetAllN().n * amount;
    balance =  totalNInput - (Nremaining + volat  + totalNToSoil);
    if (fabs(balance)>0.0001)
         theMessage->FatalError("manure:: Balance error in volatilisation");
  }
 return volat;
 //volatilisation in tonnes.
}

/**
 * Calculating water Evapulation . Currently returning 0 because it dos not work
 */
double manure::GetWaterEvap(double temperature, double relHumidity, double area, double resistance, double duration)
{    //returns evaporation in tonnes
	//temperature in K, area in sq metres and resistance in s per metre
   if (resistance > 0.0)
   {
      climate * aClimate = new climate();   //to get access to some useful functions
      double transferCoefficient = 1/resistance;
      // divide evaporation by 1000.0 to convert to tonnes
      double result = (aClimate->Evaporation(temperature,relHumidity, transferCoefficient)/1000.0) *
   						         area * duration;
      double volume = GetAmount()*(1-GetdryMatter());
      if (result>volume)  //all water evaporates
      {
       duration =volume/result;
       result=volume;
      }
      delete aClimate;
   }
	else
   	theMessage->FatalError("manure::GetWaterEvap - resistance to water vapour transport must be greater than zero");

//currently disabled
   double result=0.0;
   return result;
}


/****************************************************************************\
\****************************************************************************/
product& manure::operator=(const product& someManure)
{
	manure* aManure;
	aManure      = (manure*)(&someManure);
   decomposable::operator=(someManure);
	Mg_content   = aManure->GetMg_content();
	Cu_content   = aManure->GetCu_content();
	Rt_content   = aManure->GetRt_content();
	sand 	     	 = aManure->Getsand();

	pH           = aManure->GetpH();
   infiltFactor = aManure->GetinfiltFactor();
   canopyStorageParameter = aManure->canopyStorageParameter;
   maxDeg		 = aManure->GetmaxDeg();
   degRateConst = aManure->GetdegRateConst();
   degradableOM = aManure->GetdegradableOM();;
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& manure::operator+(const product& someManure)
{
	manure* aManure;
	aManure = (manure*)(&someManure);
	double aAmount      	= aManure->GetAmount();
	double aMg_content	= aManure->GetMg_content();
	double aCu_content   = aManure->GetCu_content();
	double aRt_content   = aManure->GetRt_content();
	double asand 	     	= aManure->Getsand();

   double apH           = aManure->GetpH();
   double ainfiltFactor	= aManure->GetinfiltFactor();
   double acanopyStorageParameter = aManure->GetcanopyStorageParameter();
   double amaxDeg       = aManure->GetmaxDeg();
   double adegRateConst 		 	= aManure->GetdegRateConst();
   double adegradableOM = aManure->GetdegradableOM();
 	if ((apH<1.0)||(apH>12.0) || (pH<1.0)||(pH>12.0))
   		theMessage->FatalError("pmanure: error in pH values in + routine");
	if((amount>1E-6) && (aAmount>1E-6))
  	{
	   Mg_content     = (Mg_content*amount   + aMg_content*aAmount)/(amount+aAmount);
	   Cu_content     = (Cu_content*amount   + aCu_content*aAmount)/(amount+aAmount);
	   Rt_content     = (Rt_content*amount   + aRt_content*aAmount)/(amount+aAmount);
      sand           = (sand*amount         + asand*aAmount)/(amount+aAmount);

      infiltFactor	= (infiltFactor*amount         + ainfiltFactor*aAmount)/(amount+aAmount);
      canopyStorageParameter	= (canopyStorageParameter*amount  + acanopyStorageParameter*aAmount)/(amount+aAmount);
      maxDeg         = (maxDeg*amount         + amaxDeg*aAmount)/(amount+aAmount);
      degRateConst         = (degRateConst*amount         + adegRateConst*aAmount)/(amount+aAmount);
//	   double tmp     = ( pow(10.0,(-1.0)*pH)*amount + pow(10.0,(-1.0)*apH)*aAmount)/ (amount+aAmount);
//If used alone to calculate mean pH, the above line sometimes generates incorrect and very low numbers, which throw a maths exception
//Done as follows, it is ok - NJH
      double tmp1 = pow(10.0,(-1.0)*pH)*amount;
      double tmp2 = pow(10.0,(-1.0)*apH)*aAmount;
      double tmp =(tmp1 +tmp2)/(amount+aAmount);
	   pH             = (-1.0)*(log10(tmp));
	}
   else if (aAmount>1E-6)
   {
   	Mg_content     = aMg_content;
	   Cu_content     = aCu_content;
	   Rt_content     = aRt_content;
      sand           = asand;
	   pH             = apH;
      infiltFactor	= ainfiltFactor;
      canopyStorageParameter = acanopyStorageParameter;
      maxDeg         = amaxDeg;
      degRateConst				= adegRateConst;
      degradableOM   = adegradableOM;
   }
	decomposable::operator+(someManure);
 	if ((NH4_content.n<0.0)||(orgN_content.n<0.0))
   		theMessage->FatalError("pmanure: error in N values in + routine");
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& manure::operator-(const product& someManure)
{
	manure* aManure;
	aManure      = (manure*)(&someManure);
	if(Mg_content   != aManure->GetMg_content() ||
		Cu_content   != aManure->GetCu_content() ||
		Rt_content   != aManure->GetRt_content() ||
		sand 	     	 != aManure->Getsand() ||

		pH           != aManure->GetpH() ||
      infiltFactor != aManure->GetinfiltFactor() ||
      degRateConst != aManure->GetdegRateConst() ||
      maxDeg       != aManure->GetmaxDeg() ||
      degradableOM != aManure->GetdegradableOM())
         theMessage->Warning("manure::operator- - manure with different composition subtracted");
	decomposable::operator-(someManure);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product*  manure::clone() const
{
	manure* aProduct= new manure(*this);
	return aProduct;
}

/****************************************************************************\
\****************************************************************************/
void manure::ReadParameters(fstream * file, bool criticals,const char *  sectionName)
{
	decomposable::ReadParameters(file,criticals, sectionName);
   Setfile(file);
   critical=criticals;
   if(FindSection(sectionName,Index)==true)
   	{
   GetParameter("Mg_content"  ,&Mg_content);
	GetParameter("Cu_content"  ,&Cu_content);
	GetParameter("Rt_content"  ,&Rt_content);
	GetParameter("sand"        ,&sand);
	GetParameter("pH"          ,&pH);
   	}
   Setfile(NULL);
}

/****************************************************************************\
Simple emission model - emission factor is proportion of TAN
Returns volatilised N in tonnes
\****************************************************************************/
double manure::VolatNitrogen(double emissionFactor)
{
  double totalVolat = emissionFactor * amount * NH4_content.n;
  NH4_content.n*=(1-emissionFactor);
  return totalVolat;
}
/**
 * calculate how much NH3, N2,N2O and N there is evaporate. Those value is return in NH3vol, N2vol, N2Ovol, MinN
 * NH3EF, N2EF, N2OEF, MinF is the fraction of NH4 that is transformed in the process.
 * It is then removed from the manure
 */
void manure::VolatAndMinNitrogen(double NH3EF, double N2EF, double N2OEF, double MinF,
			double *NH3vol, double *N2vol, double *N2Ovol, double *MinN)
{
//	double scalingFactor=1.0;
  *NH3vol = NH3EF * amount * NH4_content.n;
  *N2vol = N2EF * amount * NH4_content.n;
  *N2Ovol = N2OEF * amount * NH4_content.n;
  *MinN= MinF * amount * orgN_content.n;
  NH4_content.n= NH4_content.n*(1.0-(NH3EF+N2EF+N2OEF))+orgN_content.n*MinF;
  orgN_content.n*=(1.0-MinF);
}


/**
 * Nic can you update this
 */
double manure::InstantaneousVolatAmmonia(double temperature, double area, double resistance, double duration)
{  //returns instantaneous volatilisation in tonnes of N
	//temperature in K, infilt, precip and evap in (kg per sq metre per hr (mm/hr), area in sq metres and resistance in s per metre
   double volat=0;
/*
  Instantaneous flux (g/sq m/s) = (1/resistance) * Kh * 14 * [TAN]/(R*(temperature+273.15) * (1+pow(10,-pH)/KN))
  [TAN] =  NH4_content *amount * 1000/(14* amount *(1-dryMatter))   kg mol per litre
  assumes 1 litre=1 kg, molecular wt of N (14) cancels out
  Note - operates on hourly not per second basis; resistance input still in per second basis
*/
	if (amount>0.0)
   {
   	if ((pH<1.0)||(pH>12.0))
      theMessage->FatalError("pmanure: volatilisation error in pH values");
      double TANPresent= NH4_content.n*amount * 1000000.0/area;   //in grams per sq metre
      double volume = amount * (1-dryMatter) * 1000.0/area;  // litres/m**2
      double KN=exp(-177.95292 - (1843.22/(temperature+273.15)) + 31.4335*log(temperature+273.15)
      				- 0.0544943*(temperature+273.15));  //no units
      double Kh=exp(160.559-(8621.06/(temperature+273.15))
      				- 25.6767*log(temperature+273.15)+0.035388*(temperature+273.15));  // atm litre /mol
      double R=0.0000820575;  //universal gas constant in m**3 atm/(mol K)
      double KH=14.0 * Kh/(R*(temperature+273.15));   //g litre/(m**3 mol)
      if (resistance==0)
         theMessage->FatalError("manure:: The transfer resistance for ammonia loss cannot be zero");
      double q = 3600.0 * KH/((1+pow(10,-pH)/KN)*resistance * volume * 14.0); //rate constant, per hour
      volat = q * duration * TANPresent;
      if (volat > TANPresent)
      {
	      SetNH4_content(0.0);
      	volat = TANPresent * area/1000000.0;
      }
      else
      {
         volat *= area/1000000.0;       //convert from grams per sq metre to tonnes
         double newNH4_content = (NH4_content.n*amount-volat)/amount;
         TANPresent *= area/1000000.0;       //convert from grams per sq metre to tonnes
         double balance =  TANPresent -(newNH4_content*amount + volat);
         if (fabs(balance)>0.0001)
            theMessage->FatalError("manure:: Balance error in volatilisation");
         SetNH4_content(newNH4_content);
      }
    }
 return volat;
 //volatilisation in tonnes.
}
/**
 * calculate N2 and N2O. It does also update NH4
 */
double manure::doDenitrification(double temperature, double duration, double *N2, double *N2O)
{  //N2 and N2O emission in kg, placeholder for better funtion
	if (temperature<=0.0)
   	*N2=0.0;
   else
	   *N2 = 0.00001 * duration * mesoTemperatureFunc(temperature) * MoistureFunc() * GetNH4_content().n * amount;
   *N2O = *N2/9.0;
    double N = NH4_content.n * amount;
    N-= (*N2 + *N2O);
    SetNH4_content(N/GetAmount());
   return (*N2 + *N2O);
}
/**
 * return the amount Methane that evaporates, temperature is Kelvin
 */
double manure::GetMethaneEmission(double temperature, double duration)
{  //returns CH4-C emission in kg, placeholder for better funtion
	double ret_val;
	if ((temperature-273.15)<=0.0)
   	ret_val=0.0;
   else
	   ret_val= 0.00001 * duration * temperature * GetOrgMatterConc() * amount;
   return ret_val;
}
/**
 * return the amount Carbon Dioxide that evaporates, temperature is C
 */
double manure::GetCarbonDioxideEmission(double temperature, double duration)
{  //returns CO2-C  emission in kg, placeholder for better funtion
	double ret_val;
	if (temperature<=0.0)
   	ret_val=0.0;
   else
	   ret_val=0.0001 * duration * temperature * GetOrgMatterConc() * amount;
   return ret_val;
}

/**
 * Someone need to documented here (nic)
 */
void manure::CalcOMdegradation(double temperature, double duration, double degRateConst, double *CO2, double *CH4, double *NH4)
{
	double currentOM = GetOrgMatterConc()* amount;
   double OMdeg=degRateConst * duration * mesoTemperatureFunc(temperature) * MoistureFunc() * currentOM ;//* maxDeg;
   double C = C_content * amount;
   double Cdeg = (OMdeg/currentOM) * C;
   *CH4 = 0.75 * Cdeg;
   *CO2 = Cdeg - *CH4;
   C-= (*CO2 + *CH4);
   SetC_content(C/GetAmount());
   double OrgN = orgN_content.n * amount;
   double OrgNdeg =(OMdeg/currentOM) * OrgN;
   *NH4= OrgNdeg;
   NH4_content=(amount * NH4_content.n + *NH4)/amount;
   OrgN-=OrgNdeg;
   SetorgN_content(OrgN/GetAmount());
   CalcDryMatterConc();
}


/**
 * Update the amount of Dry matter in manure
 */
double manure::CalcDryMatterConc()
{
	dryMatter = 2.5 * C_content+sand;
	return dryMatter;
};
/**
 * Calculated how much carbon is evaporated. emissionFactor is the factor of carbon. The missing carbon is removed from the manure
 */
double manure::VolatCarbon(double emissionFactor)
{
  double totalVolat = emissionFactor * amount * C_content;
  C_content*=(1-emissionFactor);
  return totalVolat;
}

/**
 * function to adjust mesophilic biological activity according to temperature
 * temperature in Celsius
 * Looks very simelar to manure::MoistureFunc(). Need to talk to nic
 */
double manure::mesoTemperatureFunc(double temperature)
{
	double ret_val=0.0;
   //all these limits are guesses....
   double t1 = 0.0;
   double t2 = 20.0;
   double t3 = 30.0;
   double t4 = 40.0;
   if ((temperature<t1) || (temperature>t4))
   	ret_val= 0.0;
   else
	   if ((temperature>t2) && (temperature<t3))
	   	ret_val= 1.0;
      else
		   if ((temperature>t1) && (temperature<t2))
		   	ret_val= (temperature-t1)/(t2-t1);
		   else
		   	ret_val= (t4-temperature)/(t4-t3);
  	return ret_val;
}
/**
 * Looks funny. Need to talk with .. Nic?
 */
double manure::MoistureFunc()
{
	double ret_val=0.0;
   //all these limits are guesses....
   double w1 = 0.0;  //water content as %
   double w2 = 20.0;
   double w3 = 30.0;
   double w4 = 100.0;
   double waterContent = 100.0 * (1-dryMatter);
   if ((waterContent<w1) || (waterContent>w4))
	   ret_val= 0.0;
   else if ((waterContent>w2) && (waterContent<w3))
	   	ret_val= 1.0;
   else if ((waterContent>w1) && (waterContent<w2))
	   ret_val= (waterContent-w1)/(w2-w1);
	else
		   	ret_val= (w4 - waterContent)/(w4-w3);
  	return ret_val;
}

