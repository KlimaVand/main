/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "organicMatter.h"
#include "addedMatter.h"
#include "bioMatter.h"
#include "../base/message.h"
#include "../base/bstime.h"

/****************************************************************************\
Constructor
\****************************************************************************/
organicMatter::organicMatter(const char * Pname,const int Index,const base * owner)
   : base(Pname,Index,owner)
{
   // Parameters
   useSpringob=false; // Don't set this true - unfinished code !!!
   nCycling=0.0;
   ncSlope=0.0;
   useNCfractionation=false;
   ncIntercept=0.0;
   maxAOM2=1.0;
   exchangeRate=1E+36; // Default full exchange, regardless of time step "scaling"
   exclusivePreferenceNH4=false;
   NH4allwaysLeft=2E-9;
   beta=0.05; // Parameter for Langmuir equation (value taken from Mary et al. 1998)
   nitrificationRate=0.0; // Must be set from model file
   stepFactor=1.0; // Scales the decay rates, used to be able to "mix" different timesteps
   TAGpercent=false;
   ReadTAG=false; // Indicates whether TAG values should be read from file
   C14FromFile=false;
   useDelta14Cvalues=true;
   temperatureAdjust=-1.0;   // Adjustment parameter for temperature response
   Kirschbaum1=-3.432;
   Kirschbaum2=0.186;		//  value corrected from 0.168 to 0.186 by ALJ on 04-08-2010
   KirschbaumTopt=36.9;
   Arrhenius=6352.0;         // deg. Kelvin
   VantHoffQ10=2.0;
   refT=30.0;
   MaxDeltaCarbon=1E-9;
   MaxDeltaIsotope=1E-9;
   HalfTime=5568.0;          // Halftime for 14C (years)
   Rzs=0.0112372;            // Default ratio of 13C to 12C (PDB)
   clayContent=0.0;
   clayContentLimit=0.25;    // Upper limit for effect of clay content on decay
   clayRateLimit=0.5;        // Relative decay at the upper clay limit
   clayParameter1=3.0895;
   clayParameter2=2.672;
   clayParameter3=-7.86;
   clayParameter4=1.67;
   clayParameter5=1.0;
   waterResponseType=1;      // Use SOILN water response by default
   tResponseType=1;          // Use Kirschbaum temperature response by default
   ClayResponseType=1;       // 0 : None
                             // 1: Effect on turnover rates (only specified pools)
                             // 2: Effect on biomass maintenance
   clayEffect1=1.0;          // Get from conditionfile
   clayEffect2=1.0;
   temperatureEffect=-1.0;
   waterEffect=-1.0;
   Isotope=0;                // 0 : None; 1: 13C; 2: 14C; 3: TAG; Set from "main", read from "setup.dat"

   // SOILN water parameters below
   // W1-W4 are be soil dependent, and these initialisations can hence be
   // overwritten by the conditionfile
   W1=5.0;                   // PWP
   W2Offset=10.0;            // PWP + 10%
   W3Offset=-8.0;            // Saturation - 8%
   W4=50.0;                  // Saturation
   Y1=0.0;                   // Parameter for water effect
   Y2=0.6;                   // --------""--------
   m=1.0;                    // --------""--------

   // State variables
   carbon_avalibility=0.0;
   deltaCarbon=1.0;
   deltaIsotope=1.0;


   numberOfPools=0;
   numberOfProducts=0;
   Nresidual=0.0;
	for(int i=0;i<max14CYears;i++)
      radioCarbon[i]=100.0;
	for(int i=0;i<maxOrgProds;i++)
      prodList[i]=NULL;
	for(int i=0;i<maxPools;i++)
      poolList[i]=NULL;


   todayCO2.Clear();

   NO3.Clear();
   NH4.Clear();
   NH4near.Clear();
}

/****************************************************************************\
Copy constructor
\****************************************************************************/
organicMatter::organicMatter(const organicMatter& source)
   : base (source)
{
   if (&source)
   {
         useSpringob=source.useSpringob;
         nCycling=source.nCycling;
         ncSlope=source.ncSlope;
         useNCfractionation=source.useNCfractionation;
         ncIntercept=source.ncIntercept;
         maxAOM2=source.maxAOM2;
         exchangeRate=source.exchangeRate;
         exclusivePreferenceNH4=source.exclusivePreferenceNH4;
         NH4allwaysLeft=source.NH4allwaysLeft;
         beta=source.beta;
         nitrificationRate=source.nitrificationRate;
         stepFactor=source.stepFactor;
         TAGpercent=source.TAGpercent;
         ReadTAG=source.ReadTAG;
         C14FromFile=source.C14FromFile;
         useDelta14Cvalues=source.useDelta14Cvalues;
         temperatureAdjust=source.temperatureAdjust;
         Kirschbaum1=source.Kirschbaum1;
         Kirschbaum2=source.Kirschbaum2;
         KirschbaumTopt=source.KirschbaumTopt;
         Arrhenius=source.Arrhenius;
         VantHoffQ10=source.VantHoffQ10;
         refT=source.refT;
         MaxDeltaCarbon=source.MaxDeltaCarbon;
         MaxDeltaIsotope=source.MaxDeltaIsotope;
         HalfTime=source.HalfTime;
         Rzs=source.Rzs;
         clayContent=source.clayContent;
         clayContentLimit=source.clayContentLimit;
         clayRateLimit=source.clayRateLimit;
         clayParameter1=source.clayParameter1;
         clayParameter2=source.clayParameter2;
         clayParameter3=source.clayParameter3;
         clayParameter4=source.clayParameter4;
         clayParameter5=source.clayParameter5;
         clayEffect1=source.clayEffect1;
         clayEffect2=source.clayEffect2;
         waterResponseType=source.waterResponseType;
         tResponseType=source.tResponseType;
         ClayResponseType=source.ClayResponseType;
         temperatureEffect=source.temperatureEffect;
         waterEffect=source.waterEffect;
         Isotope=source.Isotope;

         W1=source.W1;
         W2Offset=source.W2Offset;
         W3Offset=source.W3Offset;
         W4=source.W4;
         Y1=source.Y1;
         Y2=source.Y2;
         m=source.m;

         // State variables
         carbon_avalibility=source.carbon_avalibility;
         deltaCarbon=source.deltaCarbon;
         deltaIsotope=source.deltaIsotope;


         numberOfPools=source.numberOfPools;
         numberOfProducts=source.numberOfProducts;
         Nresidual=source.Nresidual;
         for(int i=0;i<max14CYears;i++)
            radioCarbon[i]=source.radioCarbon[i];
      	for(int i=0;i<maxOrgProds;i++)
            prodList[i]=source.prodList[i];
         for(int i=0;i<numberOfPools;i++)
            poolList[i] = source.poolList[i]->clone();
         for(int i=0;i<numberOfPools;i++)  // To interconnect pools in the new structure
         {
            poolList[i]->SetOwner(this);
            poolList[i]->Initialize(clayEffect1,clayEffect2,ClayResponseType);
         }


         todayCO2 = source.todayCO2;

         NO3 = source.NO3;
         NH4 = source.NH4;
         NH4near = source.NH4near;
   }
   else
      Terminate("Attempt to copy 'organicMatter' with NULL pointer");
}

/****************************************************************************\
Implicit assumptions, eg. equality of number of pools, are not tested
\****************************************************************************/
void organicMatter::Add(organicMatter * source, double fraction)
{
   if (&source)
   {
      nCycling=(1.0-fraction)*nCycling+fraction*source->nCycling;
      carbon_avalibility=(1.0-fraction)*carbon_avalibility+fraction*source->carbon_avalibility;
      ncSlope=(1.0-fraction)*ncSlope+fraction*source->ncSlope;
      ncIntercept=(1.0-fraction)*ncIntercept+fraction*source->ncIntercept;
      maxAOM2=(1.0-fraction)*maxAOM2+fraction*source->maxAOM2;
      exchangeRate=(1.0-fraction)*exchangeRate+fraction*source->exchangeRate;
      NH4allwaysLeft=(1.0-fraction)*NH4allwaysLeft+fraction*source->NH4allwaysLeft;
      beta=(1.0-fraction)*beta+fraction*source->beta;
      nitrificationRate=(1.0-fraction)*nitrificationRate+fraction*source->nitrificationRate;
      stepFactor=(1.0-fraction)*stepFactor+fraction*source->stepFactor;
      temperatureEffect=(1.0-fraction)*temperatureEffect+fraction*source->temperatureEffect;
      waterEffect=(1.0-fraction)*waterEffect+fraction*source->waterEffect;
      clayContent=(1.0-fraction)*clayContent+fraction*source->clayContent;
      clayParameter1=(1.0-fraction)*clayParameter1+fraction*source->clayParameter1;
      clayParameter2=(1.0-fraction)*clayParameter2+fraction*source->clayParameter2;
      clayParameter3=(1.0-fraction)*clayParameter3+fraction*source->clayParameter3;
      clayParameter4=(1.0-fraction)*clayParameter4+fraction*source->clayParameter4;
      clayParameter5=(1.0-fraction)*clayParameter5+fraction*source->clayParameter5;
      deltaCarbon=(1.0-fraction)*deltaCarbon+fraction*source->deltaCarbon;
      deltaIsotope=(1.0-fraction)*deltaIsotope+fraction*source->deltaIsotope;
      Rzs=(1.0-fraction)*Rzs+fraction*source->Rzs;
      Kirschbaum1=(1.0-fraction)*Kirschbaum1+fraction*source->Kirschbaum1;
      Kirschbaum2=(1.0-fraction)*Kirschbaum2+fraction*source->Kirschbaum2;
      KirschbaumTopt=(1.0-fraction)*KirschbaumTopt+fraction*source->KirschbaumTopt;
      Arrhenius=(1.0-fraction)*Arrhenius+fraction*source->Arrhenius;
      VantHoffQ10=(1.0-fraction)*VantHoffQ10+fraction*source->VantHoffQ10;
      refT=(1.0-fraction)*refT+fraction*source->refT;
      temperatureAdjust=(1.0-fraction)*temperatureAdjust+fraction*source->temperatureAdjust;
      clayContentLimit=(1.0-fraction)*clayContentLimit+fraction*source->clayContentLimit;
      clayRateLimit=(1.0-fraction)*clayRateLimit+fraction*source->clayRateLimit;
      MaxDeltaCarbon=(1.0-fraction)*MaxDeltaCarbon+fraction*source->MaxDeltaCarbon;
      MaxDeltaIsotope=(1.0-fraction)*MaxDeltaIsotope+fraction*source->MaxDeltaIsotope;
      HalfTime=(1.0-fraction)*HalfTime+fraction*source->HalfTime;
      W1=(1.0-fraction)*W1+fraction*source->W1;
      W2Offset=(1.0-fraction)*W2Offset+fraction*source->W2Offset;
      W3Offset=(1.0-fraction)*W3Offset+fraction*source->W3Offset;
      W4=(1.0-fraction)*W4+fraction*source->W4;
      Y1=(1.0-fraction)*Y1+fraction*source->Y1;
      Y2=(1.0-fraction)*Y2+fraction*source->Y2;
      m=(1.0-fraction)*m+fraction*source->m;
      clayEffect1=(1.0-fraction)*clayEffect1+fraction*source->clayEffect1;
      clayEffect2=(1.0-fraction)*clayEffect2+fraction*source->clayEffect2;
      Nresidual=(1.0-fraction)*Nresidual+fraction*source->Nresidual;


      todayCO2=todayCO2*(1.0-fraction)+source->todayCO2*fraction;

      NO3=NO3*(1.0-fraction)+source->NO3*fraction;
      NH4=NH4*(1.0-fraction)+source->NH4*fraction;
      NH4near=NH4near*(1.0-fraction)+source->NH4near*fraction;
      for(int i=0;i<numberOfPools;i++)
         poolList[i]->Add(source->poolList[i],fraction);
   }
   else
      Terminate("organicMatter::Add - function called with NULL pointer");
}

/****************************************************************************\
Destructor
\****************************************************************************/
organicMatter::~organicMatter()
{
   if (numberOfPools>=maxPools) // Previous problems, caused by incomplete copy constructors - should be solved now
      Terminate("organicMatter::~organicMatter - 'numberOfPools' to high!");
   if (numberOfProducts>=maxOrgProds) // Previous problems, caused by incomplete copy constructors - should be solved now
      Terminate("organicMatter::~organicMatter - 'numberOfProducts' to high!");
   for(int i=0;i<numberOfPools;i++)
      if (poolList[i])
         delete poolList[i];
   for(int i=0;i<numberOfProducts;i++)
      if (prodList[i])
         delete prodList[i];
}

/****************************************************************************\
Initiates all soil matter pools with data which each instance reads
from the file.
   filename   input file name
\****************************************************************************/
void organicMatter::InitStructure(char * filename)
{
   UnsetCritical();
   if (!OpenInputFile(filename))
      Terminate("Structurefile [",filename,"] could not be opened.");
   int n,first;
   GetSectionNumbers("AddedMatter",&first,&n);
	for(int i=first;i<(first+n);i++)
	{
		matter * m=new addedMatter("AddedMatter",i,this);
      m->SetStepFactor(stepFactor);
      m->ReadParameters(file);
      poolList[numberOfPools]=m;
      numberOfPools+=1;

	}
   GetSectionNumbers("BioMatter",&first,&n);
	for(int i=first;i<(first+n);i++)
	{
		matter * m=new bioMatter("BioMatter",i,this);
      m->SetStepFactor(stepFactor);
      m->ReadParameters(file);
      poolList[numberOfPools]=m;
      numberOfPools+=1;

	}
   GetSectionNumbers("Matter",&first,&n);
	for(int i=first;i<(first+n);i++)
	{
		matter * m=new matter("Matter",i,this);
      m->SetStepFactor(stepFactor);
      m->ReadParameters(file);
      poolList[numberOfPools]=m;
      numberOfPools+=1;

	}
   if (numberOfPools<1)
      Terminate("Pools are not properly defined in structurefile.");
   if (FindSection("Parameters"))
   {
      GetParameter("TemperatureResponse",&tResponseType);
      GetParameter("WaterResponse",&waterResponseType);
      GetParameter("W1",&W1);
      GetParameter("W4",&W4);
      GetParameter("W2Offset",&W2Offset);
      GetParameter("W3Offset",&W3Offset);
      GetParameter("Y1",&Y1);
      GetParameter("Y2",&Y2);
      GetParameter("m",&m);
      GetParameter("HalfTime",&HalfTime);
      GetParameter("ClayResponse",&ClayResponseType);
      GetParameter("ClayContentLimit",&clayContentLimit);
      GetParameter("ClayRateLimit",&clayRateLimit);
      GetParameter("TemperatureAdjust",&temperatureAdjust);
      GetParameter("Kirschbaum1",&Kirschbaum1);
      GetParameter("Kirschbaum2",&Kirschbaum2);
      GetParameter("KirschbaumTopt",&KirschbaumTopt);
      GetParameter("Arrhenius",&Arrhenius);
      GetParameter("VantHoffQ10",&VantHoffQ10);
      GetParameter("ReferenceTemperature",&refT);
      GetParameter("ClayParameter1",&clayParameter1);
      GetParameter("ClayParameter2",&clayParameter2);
      GetParameter("ClayParameter3",&clayParameter3);
      GetParameter("ClayParameter4",&clayParameter4);
      GetParameter("ClayParameter5",&clayParameter5);
      GetParameter("NitrificationRate",&nitrificationRate);
      if (GetParameter("NCslope",&ncSlope))
      {
         useNCfractionation=true;
         GetParameter("NCintercept",&ncIntercept);
         GetParameter("MaxAOM2",&maxAOM2);
         if (maxAOM2>1.0 || maxAOM2<0.0)
            Terminate("Model parameter 'MaxAOM2' has illegal value");
      }
      nitrificationRate=nitrificationRate*stepFactor;
      exchangeRate=exchangeRate*stepFactor;
   }
   CloseInputFile();
   DeleteInputFile(); // ??????!!!!! I just don't get why this works without "DeleteInputFile()" most of the time, but not here - BMP
}



/****************************************************************************\
\****************************************************************************/
cnMatter organicMatter::GetCarbonInPool(char * c)
{
   string s=c;
   return GetCarbonInPool(&s);
}

/****************************************************************************\
\****************************************************************************/
bool organicMatter::ExistsPool(string * poolName)
{
   int j=-1;
   if (*poolName=="CO2")
      return false;
   for(int i=0;i<numberOfPools;i++)
      if (*poolName==poolList[i]->GetPoolName())
         j=i;
   if (j==-1)
      return false;
   else
      return true;
}

/****************************************************************************\
\****************************************************************************/
double organicMatter::GetRatio(string poolName)
{
   bioMatter * b=(bioMatter*)GetPoolPointer(poolName);
   double d=b->GetDeathRate();
   double m=b->GetMaintenanceRate();
   return d/(d+m);
}

/****************************************************************************\
\****************************************************************************/
double organicMatter::GetE(string poolName)
{
   bioMatter * b=(bioMatter*)GetPoolPointer(poolName);
   return b->GetUtilizationEfficiency();
}

/****************************************************************************\
\****************************************************************************/
double organicMatter::GetfSMB2()
{
   addedMatter * a=(addedMatter*)GetPoolPointer("AOM2");
   return a->GetConnectionFraction("SMB2");
}

/****************************************************************************\
\****************************************************************************/
double organicMatter::GetTurnoverRatioSMB1(double clayStandard)
{
   double retval;
   double E=GetE("SMB1");
   double fSMB2=GetfSMB2();
   double r2=GetRatio("SMB2");
   double R=clayParameter4*(1.85+1.6*exp(clayParameter3*clayStandard));
   retval=1.0/((1.0+R)*E*(1.0-fSMB2+fSMB2*E*r2));
   return retval;
}

/****************************************************************************\
Returns the effect of clay content on processes
Return value range 0-1
clay - clay fraction (0-1)
responseType
   0 : Clay response not used
   1 : Like DAISY
   2 : Like RothC
   3 : New principle
\****************************************************************************/
double organicMatter::ClayEffect(double clay,int responseType)
{
   double retVal=1.0;
   double b,r,r1,r2,E,fSMB2,s,h,fNOM,rCheck;
   bioMatter * bio;
   switch (responseType)
   {
      case 0: // Clay response not used
         retVal=1.0;
      break;
      case 1:
         // Like DAISY
         retVal = clayRateLimit;    // Default 0.5
         if (clay<clayContentLimit) // Default 0.25
         retVal = 1.0-clay*(1.0-clayRateLimit)/clayContentLimit;
      break;
      case 2: // Like RothC
         retVal=clayParameter1+clayParameter2*exp(clayParameter3*clay);
      break;
      case 3: // New principle. Difference lies in class "bioMatter".
         retVal=clayParameter4*(1.85+1.6*exp(-7.86*clay));
      break;
      case 4:
         // JBE principle
         if (clayParameter5<=0.0)
            Terminate("'ClayParameter5 must be above zero");
         b=(0.1+clayParameter5)/clayParameter5;
         retVal = clayParameter5*b/(clay+clayParameter5);
      break;
      case 5:
      case 8:
         // Double principle March 2002. Model pools AOM1, AOM2, SMB1, SMB2, SMR, NOM, IOM.
         // If responseType is 8, effect on total turnover like DAISY
         if (clayParameter5<=0.0)
            Terminate("'ClayParameter5 must be above zero");
         if (responseType==5)
         {
            b=(0.1+clayParameter5)/clayParameter5;
            retVal=clayParameter5*b/(clay+clayParameter5);
         }
         else
         {
            retVal=1.0-2.0*min(0.25,clay);
            retVal=retVal/0.8; // !!! To ensure unity at 10% clay !!!
         }
         r1=GetTurnoverRatioSMB1(0.1);
         bio=(bioMatter*)GetPoolPointer("SMB1");
         rCheck=bio->GetTurnoverRatio();
         rCheck=fabs(r1/rCheck-1.0);
         if (rCheck>0.02)
            theMessage->WarningWithDisplay("Turnover rate for 'SMB1' is inconsistent with clay function");
         bio->SetParameterByName("TurnoverRatio",r1); // Set exact value every time
         r2=GetRatio("SMB2"); // GetR2
         E=GetE("SMB1");
         fSMB2=GetfSMB2();
         h=clayParameter4*(1.85+1.6*exp(-7.86*clay));
         h=1.0/(h+1.0);
         if (fSMB2>1E-5)
         {
            s=r1*(1.0-fSMB2)*r1*(1.0-fSMB2)+4.0*h*r1*r2*fSMB2;
            if (s<0.0)
               Terminate("organicMatter::ClayEffect - illegal numerical operation attempted");
            s=sqrt(s);
            clayEffect2=(s-r1*(1.0-fSMB2))/(2.0*E*r1*r2*fSMB2);
         }
         else
            clayEffect2=h/(E*r1);
         if (clayEffect2<0.0)
            Terminate("organicMatter::ClayEffect - 'clayEffect2' is negative");
         break;
      case 6:
         // Double principle June 2002. Model pools AOM1, AOM2, SMB, NOM, IOM.
         if (clayParameter5<=0.0)
            Terminate("'ClayParameter5 must be above zero");
         b=(0.1+clayParameter5)/clayParameter5;
         retVal=clayParameter5*b/(clay+clayParameter5);
         r=GetRatio("SMB");
         E=GetE("SMB");
         h=clayParameter4*(1.85+1.6*exp(-7.86*clay));
         h=1.0/(h+1.0);
         clayEffect2=h/(E*r);
         if (clayEffect2<0.0)
            Terminate("container::ClayEffect - 'clayEffect2' is negative");
      break;
      case 7:
         // Double principle June 2002. Model pools AOM1, AOM2, SMB, SMR, NOM, IOM.
         if (clayParameter5<=0.0)
            Terminate("'ClayParameter5 must be above zero");
         b=(0.1+clayParameter5)/clayParameter5;
         retVal=clayParameter5*b/(clay+clayParameter5);
         r=GetRatio("SMB");
         E=GetE("SMB");
         bio=(bioMatter*)GetPoolPointer("SMB");
         fNOM=bio->GetConnectionFraction("NOM");
         h=clayParameter4*(1.85+1.6*exp(-7.86*clay));
         h=1.0/(h+1.0);
         if (fNOM>0.99)
            clayEffect2=h/(E*r);
         else
            if (fNOM>h)
               clayEffect2=(fNOM*r+sqrt(fNOM*r*fNOM*r+4.0*h*fNOM*(1.0-fNOM)))/(2.0*E*(1.0-fNOM)*fNOM);
            else
               clayEffect2=1000.0;
         if (clayEffect2<0.0)
            Terminate("container::ClayEffect - 'clayEffect2' is negative");
      break;
      case 9:
         // Double principle December 2002. Model pools AOM1, AOM2, SMB1, SMB2, SMR, NOM, IOM.
         // Effect on total turnover like DAISY, and unity at 0 % clay.
         if (clayParameter5<=0.0)
            Terminate("'ClayParameter5 must be above zero");
         retVal=1.0-2.0*min(0.25,clay); // Clay function taken from DAISY
         r1=GetTurnoverRatioSMB1(0.0);
         bio=(bioMatter*)GetPoolPointer("SMB1");
         rCheck=bio->GetTurnoverRatio();
         rCheck=fabs(r1/rCheck-1.0);
         if (rCheck>0.02)
         {
            cout << endl << "Turnover rate for 'SMB1' " << bio->GetTurnoverRatio()
                 << " inconsistent with clay function. New value set at " << r1 << endl;
         }
         bio->SetParameterByName("TurnoverRatio",r1);
         r2=GetRatio("SMB2");
         E=GetE("SMB1");
         fSMB2=GetfSMB2();
         h=clayParameter4*(1.85+1.6*exp(clayParameter3*clay));
         h=1.0/(h+1.0);
         if (fSMB2>1E-5)
         {
            s=r1*(1.0-fSMB2)*r1*(1.0-fSMB2)+4.0*h*r1*r2*fSMB2;
            if (s<0.0)
               Terminate("container::ClayEffect - illegal numerical operation attempted");
            s=sqrt(s);
            clayEffect2=(s-r1*(1.0-fSMB2))/(2.0*E*r1*r2*fSMB2);
         }
         else
            clayEffect2=h/(E*r1);
         if (clayEffect2<0.0)
            Terminate("container::ClayEffect - 'clayEffect2' is negative");
      break;
      default:
         Terminate("Illegal state of clay effect type");
   }
   return retVal;
}

/****************************************************************************\
Returns the effect of temperature on processes
temp - Soil temperature [�C]
responseType
   0 : Temperature response not used
   1 : Kirschbaum   - default adjusted to unity at 10 deg.
   2 : ROTHC
   3 : DAISY
   4 : Arrhenius    - default adjusted to unity at 10 deg.
   5 : Van't Hoff   - defualt adjusted to unity at 10 deg.
   6 : Value used directly
\****************************************************************************/
double organicMatter::TemperatureEffect(double temp,int responseType)
{
   if (temperatureEffect>0.0)
      return temperatureEffect; // Return "fixed" value, regardless of settings
   double retVal=1.0;
   switch (responseType)
   {
      case 0: // Temperature response not used
         retVal=1.0;
         break;
      case 1: // Kirschbaum
         if (temperatureAdjust<0)
         { // Adjust to give unity output at 10 deg.
            temperatureAdjust=1.0;
            temperatureAdjust=1/TemperatureEffect(10.0,responseType);
         }
         retVal=temperatureAdjust*exp(Kirschbaum1+Kirschbaum2*temp*(1.0-0.5*temp/KirschbaumTopt));
         break;
      case 2: // ROTHC
         retVal=0.0;
         if (temperatureAdjust<0)
            temperatureAdjust=1.0;
         if (temp>-17) // Theoretically -18.3, but return value is so low for -17 that this is choosen to avoid numerical problems
            retVal=temperatureAdjust*47.9/(1.0+exp(106.0/(temp+18.3)));
         break; // To get almost exact matches with RothC, use parameters 106.058 & 18.2715 instead. Don't know why.
      case 3: // DAISY
         retVal=0.0;
         if (temperatureAdjust<0)
            temperatureAdjust=1.0;
         if (temp>0 && temp<=20)
            retVal = 0.1*temperatureAdjust*temp;
         if (temp>20)
            retVal=temperatureAdjust*exp(0.47-0.027*temp+0.00193*temp*temp);
         break;
      case 4: // Arrhenius
         if (temperatureAdjust<0)
         { // Adjust to give unity output at 10 deg.
            temperatureAdjust=1.0;
            temperatureAdjust=1/TemperatureEffect(10.0,responseType);
         }
         retVal=temperatureAdjust*exp(Arrhenius*(temp-refT)/((refT+273.2)*(temp+273.2)));
         break;
      case 5: // Van't Hoff
         if (temperatureAdjust<0)
         { // Adjust to give unity output at 10 deg.
            temperatureAdjust=1.0;
            temperatureAdjust=1/TemperatureEffect(10.0,responseType);
         }
         retVal=temperatureAdjust*pow(VantHoffQ10,(temp-refT)/10.0);
         break;
      case 6: // Value used directly
         retVal=temp;
         break;
      default:
         Terminate("Illegal state of temperature effect type");
   }
   if (retVal<0.0)
      Terminate("Temperature effect negative");
   return retVal;
}

/****************************************************************************\
Returns the effect of soil water status on soil organic matter turnover
Return value range 0-1
water - Soil water status
responseType
   0 : Water response not used
   1 : Like SOILN
   2 : Value used directly
\****************************************************************************/
double organicMatter::SoilWaterEffect(double water,int responseType)
{
   if (waterEffect>0.0)
      return waterEffect; // Return "fixed" value, regardless of settings
   double f=1.0;
   switch (responseType)
   {
      case 0: // Water response not used
      {
         f=1.0;
         break;
      }
      case 1:
      {
         if (W2Offset<0)
            Terminate("W2Offset can not be negative (soil water function)");
         if (W3Offset>0)
            Terminate("W3Offset can not be positive (soil water function)");
         double W2=W1+W2Offset;
         double W3=W4+W3Offset;
         if (W2>W3)
            Terminate("W2 is larger than W3 (soil water function)");
         f = Y1;
         if (water>W1)
         {
            f = 1.0;
            if (water<=W2)
               f = Y1+(1.0-Y1)*pow((water-W1)/(W2-W1),m);
            if (water>W3)
               f = 1.0-(1.0-Y2)*pow((water-W3)/(W4-W3),m);
            f = max(0.0,f);
         }
         break;
      }
      case 2: // Value used directly
      {
         f=max(0.0,water);
         break;
      }
      default:
         Terminate("Illegal state of water effect type");
   }
   if (f<0.0)
      Terminate("Water effect negative");
   return f;
}

/****************************************************************************\
Returns a pointer to a given pool
poolName - name of pool for which a pointer is requested
\****************************************************************************/
matter * organicMatter::GetPoolPointer(string poolName)
{
   int j=-1;
   if (poolName=="CO2")
      return NULL;
   for(int i=0;i<numberOfPools;i++)
      if (poolName==poolList[i]->GetPoolName())
      {
         j=i;
         i=numberOfPools;            // Improved efficiency - inserted by JBE
      }
   if (j==-1)
      Terminate("Pool [",(char*)poolName.c_str(),"] not found");
   return poolList[j];
}

/****************************************************************************\
\****************************************************************************/
cnMatter organicMatter::GetCarbonInPool(string * s)
{
   return GetPoolPointer(*s)->GetCarbon();
}


/****************************************************************************\
\****************************************************************************/
void organicMatter::SetCarbonInPool(string * s,cnMatter c)
{
   GetPoolPointer(*s)->SetCarbon(c);
}






/****************************************************************************\
\****************************************************************************/
double organicMatter::GetFractionFromdelta(double delta)
{
   double retVal;
   if (Rzs>1E-6)
   {
      double Rzi=Rzs+(Rzs*delta)/1000.0;
      retVal=(Rzi+Rzi*Rzs)/(Rzs+Rzs*Rzi);
   }
   else
      retVal=(delta+1000.0)/1000.0;
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double organicMatter::FromAtomExecessToInternal(double excess)
{
   return (excess+0.3663033)/100.0;
}

/****************************************************************************\
\****************************************************************************/
void organicMatter::SetFractionation(organicProduct * prod)
{
   double pc,pn,nc,fAOM2;
   int n=prod->GetNumOfFractions();
   double fsum=0.0;
   for (int i=0;i<n;i++)
      fsum+=prod->GetCFraction(i);
   if (fsum<0.9999 && prod->GetAmount().c>0.0)
   {  // Fractions have no default value, set according to N/C ratio
      // Assumed to have AOM1 (0) and AOM2 (1), AOM2 beeing the easily decomposable
      pc=prod->GetAmount().c;
      pn=prod->GetAmount().n;
      nc=pn/pc;
      fAOM2=ncIntercept+ncSlope*nc;
      fAOM2=max(0.0,min(maxAOM2,fAOM2));
      prod->SetCFraction(0,1.0-fAOM2);
      prod->SetCFraction(1,fAOM2);
      prod->SetNFraction(0,1.0-fAOM2); // No method for N fractionation decided yet !!!
      prod->SetNFraction(1,fAOM2);
   }
}


/****************************************************************************\
Updates CO2 emmision variables
co2 - CO2-C emmision
\****************************************************************************/
void organicMatter::CO2emmision(cnMatter co2)
{

   todayCO2=todayCO2+co2;
}

/****************************************************************************\
\****************************************************************************/
void organicMatter::SetMineralNitrogen(nitrogen nitrate,nitrogen ammonium)
{
   if (NH4.n<-1E-12 || NO3.n<-1E-12)
      Terminate("organicMatter::SetMineralNitrogen - negative value for mineral nitrogen");
   NH4=ammonium;
   NO3=nitrate;
}

/****************************************************************************\
\****************************************************************************/
void organicMatter::GetMineralNitrogen(nitrogen &nitrate,nitrogen &ammonium)
{
   ammonium=NH4;
   nitrate=NO3;
}

/*********************************************h*******************************\
A comparatively crude mimic of the indirect-MIT theory
\****************************************************************************/
void organicMatter::EquilibrateN15(cnMatter & c)
{
   if (c.n>1E-36 && NH4.n>1E-36)
   {
      double q=(c.n15+NH4.n15)/(c.n+NH4.n);
      NH4.n15=NH4.n*q;
      c.n15=c.n*q;
   }
}

/****************************************************************************\
\****************************************************************************/
void organicMatter::ExchangeNitrogen(nitrogen * nit)
{
	if ((NO3.n+NH4.n)<-nit->n)
      Terminate("organicMatter::ExchangeNitrogen - total mineral N content below zero");
   if (NH4.n<-1E-9)
      Terminate("organicMatter::ExchangeNitrogen - NH4 content below zero");
   if (NO3.n<-1E-9)
      Terminate("organicMatter::ExchangeNitrogen - NO3 content below zero");
   if (nit->n<0)
   {  // Imobilisation
      double f=1.0;
      double ntot=max(0.0,NH4.n-NH4allwaysLeft)+NO3.n;
      if (NH4.n<=NH4allwaysLeft)
         f=0.0;
      else
      {
         if (NO3.n>0.0 && !exclusivePreferenceNH4)
            f=(NH4.n/ntot)/(beta+(1.0-beta)*NH4.n/ntot); // Langmuir equation
         f=min(f,max(0.0,NH4.n-NH4allwaysLeft)/(-nit->n));
      }
      if (f<0.0)
         f=0.0;
      if (f>1.0)
         f=1.0;
      if (f<0.0 || f >1.0)
      {
         cout << f << " " << NH4.n << " " << nit->n << endl;
         Terminate("organicMatter::ExchangeNitrogen - f must be in the range [0;1]");
      }
      nitrogen fromNH4=*nit*f;
      nitrogen fromNO3=*nit*(1.0-f);

      double f1=0.0; // From "near" NH4 pool
      if (NH4near.n>1E-9)
      {
         if (NH4near.n>=-fromNH4.n)
            f1=1.0;
         else
            f1=NH4near.n/(-fromNH4.n);
      }
      fromNH4.n15=0.0;
      if (NH4.n>1E-15)
      {   if (NH4near.n>1E-9 && (NH4.n-NH4near.n)>1E-9)
            fromNH4.n15=fromNH4.n*((1.0-f1)*(NH4.n15-NH4near.n15)/(NH4.n-NH4near.n)+f1*NH4near.n15/NH4near.n);
         else
            fromNH4.n15=fromNH4.n*NH4.n15/NH4.n;
      }
      fromNO3.n15=0.0;
      if (NO3.n>1E-15)
         fromNO3.n15=fromNO3.n*NO3.n15/NO3.n;
      NH4=NH4+fromNH4;
      NH4near.n=max(0.0,NH4near.n+fromNH4.n);
      NH4near.n15=max(0.0,NH4near.n15+fromNH4.n15);
      NO3=NO3+fromNO3;
      if (NO3.n<-1E-9)
      	Terminate("organicMatter::ExchangeNitrogen - NO3 content below zero");
      nit->n15=fromNH4.n15+fromNO3.n15;
   }
   else // Mineralisation
   {
      NH4=NH4+*nit;
      NH4near=NH4near+*nit;
   }

   if (NH4.n<-1E-9)
      Terminate("organicMatter::ExchangeNitrogen - NH4 content below zero");
   if (NO3.n<-1E-9)
      Terminate("organicMatter::ExchangeNitrogen - NO3 content below zero");
}

/****************************************************************************\
Returns the total amount of carbon and nitrogen in the whole system
\****************************************************************************/
cnMatter organicMatter::GetTotalSystem()
{
   cnMatter retval;
   for(int i=0;i<numberOfPools;i++)
      retval=retval+poolList[i]->GetCarbon();
   retval.n+=NH4.n+NO3.n;
   retval.n15+=NH4.n15+NO3.n15;
   return retval;
}

/****************************************************************************\
Returns the total amount of carbon and nitrogen in all the soil pools
\****************************************************************************/
cnMatter organicMatter::GetTotalCarbon()
{
   cnMatter retval;
   for(int i=0;i<numberOfPools;i++)
      retval=retval+poolList[i]->GetCarbon();
   return retval;
}

/****************************************************************************\
Initializes the organic pool object
 clay       - Clay content of soil layer [fraction]
 carbon     - C content of layer [g/m2]
 f          - Parameter file
\****************************************************************************/
void organicMatter::Initialize(double clay,
                               double carbon,
										 fstream * f,
                               int index,
                               char * modelFileName)
{
   clayContent=clay;
   double TotalC=carbon;
   InitStructure(modelFileName);
   Setfile(f);
   if (FindSection("SoilLayer",index)) // Top of inheritance hierarcy
   {
      if (Isotope==2) // 14C
         Rzs=0.0;
      double checkSum=0.0;
      double cnRatio=11;
      GetParameter("CNRatio",&cnRatio);
      GetParameter("MinContent",&W1);
      GetParameter("MaxContent",&W4);
      double d=0.0;
      if (GetParameter("NH4",&d))
         NH4.n=d;
      if (GetParameter("NH4.N15",&d))
         NH4.n15=NH4.n*FromAtomExecessToInternal(d);
      if (GetParameter("NO3",&d))
         NO3.n=d;
      if (GetParameter("NO3.N15",&d))
         NO3.n15=NO3.n*FromAtomExecessToInternal(d);
      d=0.0;
      for(int i=0;i<numberOfPools;i++)
      {
         poolList[i]->SetIsotope(Isotope);
         if (GetParameter(poolList[i]->GetPoolName().c_str(),&d))
         {
            cnMatter c;
            c.c=d*TotalC;
            checkSum+=d;
            if (Isotope==1) // 13 C
            {
               string s=poolList[i]->GetPoolName()+".d13C";
               if (GetParameter(s.c_str(),&d))
               {
                  if (d>=-1000.0)
                     c.c_iso=GetFractionFromdelta(d)*c.c;
                  else
                     Terminate("delta value for carbon-13 in file is below -1000");
               }
               else
                  c.c_iso=c.c;
            }
            if (Isotope==2) // 14 C
            {
               string s=poolList[i]->GetPoolName()+".PM";
               if (GetParameter(s.c_str(),&d))
               {
                  if (d>=0.0)
                     c.c_iso=c.c*d/100.0;
                  else
                     Terminate("Percent modern for carbon-14 in file is below zero");
               }
               else
               {
                  s=poolList[i]->GetPoolName()+".d14C";
                  if (GetParameter(s.c_str(),&d))
                  {
                     if (d>=-1000.0)
                        c.c_iso=GetFractionFromdelta(d)*c.c;
                     else
                        Terminate("delta value for carbon-14 in file is below -1000");
                  }
                  else
                     c.c_iso=c.c;
               }
            }
            if (Isotope==3) // Tagging
            {
               string s=poolList[i]->GetPoolName()+".TAG";
               if (GetParameter(s.c_str(),&d))
               {
                  if (d>=0.0 && d<=1.0)
                     c.c_iso=d*c.c;
                  else
                     Terminate("TAG value in file must be between zero and one");
               }
               else
                  c.c_iso=0.0;
            }
            string s=poolList[i]->GetPoolName()+".N";
            if (GetParameter(s.c_str(),&d))
            {
//             if (poolList[i]->GetCNratio()>0 && !poolList[i]->IsBiomass())
//                Terminate("Can not set N content in '",(char*)poolList[i]->GetPoolName().c_str(),"' as this pool has a fixed C/N ratio");
               c.n=d*TotalC;
            }
            else
            {
               double r=poolList[i]->GetDefaultCNratio();
               if (r<=0 && c.c>1E-9)
                  Terminate("C/N ratio must be set for pool '",(char*)poolList[i]->GetPoolName().c_str());
               if (r>0)
                  c.n=c.c/r;
            }
            s=poolList[i]->GetPoolName()+".N15";
            if (GetParameter(s.c_str(),&d))
               c.n15=c.n*FromAtomExecessToInternal(d);
            poolList[i]->SetCarbon(c);
         }
      }
      if (fabs(1.0-checkSum)>1E-3)
         Terminate("organicMatter::Initialise - Pool carbon fractions do not add to one.");
      if (GetParameter("ClayContent",&clayContent))
      {
         if (clayContent>1.0)
            Terminate("Clay fraction can not be bigger than one.");
         clayEffect1=ClayEffect(clayContent,ClayResponseType);
      }
      if (GetParameter("ClayEffect",&clayEffect1)) // Overwrites clay content
         if (clayEffect1<0.0)
            Terminate("The parameter ClayEffect can not be below zero");
      if (GetParameter("TemperatureEffect",&temperatureEffect)) // "Fixed" temperature effect
         if (temperatureEffect<0.0)
            Terminate("The parameter TemperatureEffect can not be below zero");
      if (GetParameter("WaterEffect",&waterEffect))             // "Fixed" water effect
         if (waterEffect<0.0)
            Terminate("The parameter WaterEffect can not be below zero");
      for(int i=0;i<numberOfPools;i++)
         poolList[i]->Initialize(clayEffect1,clayEffect2,ClayResponseType);
      if (useSpringob) // Adjust size of "IOM" according to C/N ratio - not working properly!!!
      {
         cnRatio=16;
         double iomf=61.651*pow(cnRatio,-1.7314);
         iomf=min(1.0,iomf);
         string IOMname="";
         string name="IOM";
         if (ExistsPool(&name))
            IOMname=name;
         cnMatter iom=GetCarbonInPool((char*)IOMname.c_str());
         cnMatter before=GetTotalCarbon();
         double activeNow=0.0; // Is set below to 1 - fIOM
         if (before.c>0)
         {
            activeNow = (before.c-iom.c)/before.c;
            double activeAfter=activeNow*iomf;
            iomf=(1.0-activeAfter)/(1.0-activeNow);
            name="ROM";
            if (ExistsPool(&name))
               IOMname=name;
            if (IOMname=="")
               Terminate("container::SetLocalParameterByName - could not find relevant inert pool for setting 'IOMFactor'");
            string humName="";
            name="HUM";
            if (ExistsPool(&name))
               humName=name;
            name="NOM";
            if (ExistsPool(&name))
               humName=name;
            if (humName=="")
               Terminate("Could not find relevant 'humus' pool for setting 'IOMFactor'");
            cnMatter nom=GetCarbonInPool((char*)humName.c_str());
            double move_carbon=(1.0-iomf)*iom.c;
            double move_14c=0.0;
            if (nom.c>0)
               move_14c=move_carbon*nom.c_iso/nom.c;
            iom.c-=move_carbon;
            iom.c_iso-=move_14c;
            if (iom.c_iso<0.0)
			   iom.c_iso=0.0;
            nom.c+=move_carbon;
            nom.c_iso+=move_14c;
            SetCarbonInPool((char*)IOMname.c_str(),iom);
            SetCarbonInPool((char*)humName.c_str(),nom);
            cnMatter after=GetTotalCarbon();
            if (fabs(before.c_iso-after.c_iso)>0.01)
            {
               cout << "Balancing error carbon isotope." << endl;
               cout << "Before scaling: " << before.c_iso
                    << "  After scaling: " << after.c_iso << endl;
            }
            if (fabs(before.c-after.c)>0.01)
            {
               cout << "Balancing error C-12." << endl;
               cout << "Before scaling: " << before.c
                    << "  After scaling: " << after.c << endl;
               cout << "Press a key." << endl;
#ifndef __BCplusplus__
               char dum;
               cin >> dum;
#else
               getch();
#endif
            }
         }
      }
      // BALANCE C/N RATIO USING POOL WITH ZERO TURNOVER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      // IF NO SUCH POOL, WRITE A WARNING
   }
   file=NULL;
}

/****************************************************************************\
Performs mineralisation of the organic matter pools in the soil layer.
 temp          - Soil temperature [�C]
 pFValue       - Soil water status as a pF-value
 nitrate_mob   - Nitrate in the mobile water phase [g N/m�/d]
 nitrate_imob  - Nitrate in the immobile water phase [g N/m�/d]
 ammonium_mob  - Ammonium in the mobile water phase [g N/m�/d]
 ammonium_imob - Ammonium in the immobile water phase [g N/m�/d]
 Returns CO2 evolution
\****************************************************************************/
double organicMatter::Update(double temp,
									  double pFValue,
									  nitrogen * nitrate,
									  nitrogen * ammonium)
{

   if (nitrate->n<-1E-9)
      Terminate("organicMatter::Update - function called with nitrate below zero");
   if (ammonium->n<-1E-9)
      Terminate("organicMatter::Update - function called with anmmnium below zero");
   carbon_avalibility=0.0;

   // Calculate frations of nitrate and ammonium in mobile water phases
   NO3 = *nitrate;
   NH4 = *ammonium;
   cnMatter tot=GetTotalSystem();

// Decompose ----------------------------------------------------------------
   cnMatter c;

   // 1. Update all pools, outflux that would cause immobilisation in
   // a receiver pool is stored in a "transit" pool.
   double fT=TemperatureEffect(temp,tResponseType);
   double environmentEffect=fT*SoilWaterEffect(pFValue,waterResponseType);

   for(int j=0;j<numberOfPools;j++)
   {
      if (!poolList[j]->IsBiomass())
         carbon_avalibility+=poolList[j]->PotentialCarbonTurnover();
      c=poolList[j]->GetCarbon();
      poolList[j]->Update(environmentEffect,0);
   }

   // 2. Catch all influx, none off which would cause immobilisation
   for(int j=0;j<numberOfPools;j++)
      poolList[j]->CatchInflux(c,0);

   // 3. Calculate the total potential immobilisation and the available N
   double Nimob=0.0;
   for(int j=0;j<numberOfPools;j++)
   {
      double Ni=poolList[j]->PotentialNitrogenMineralisation();
      if (Ni<=1E-20)
         Nimob+=Ni;
      else
         Terminate("organicMatter::UpdatePools - programming inconsistency");
   }

   double availN=max(0.0,NO3.n+max(0.0,NH4.n-NH4allwaysLeft)); // max takes care of minute rounding errors
                                                               // All N considered avaliable !!!

   // 4. Calculate the remaining total C turnover
   double Cpot=0.0;
   for(int j=0;j<numberOfPools;j++)
      Cpot+=poolList[j]->ResidualCarbon();

   // 5. For each pool determine the individual ratio, export this ratio
   // and put the rest back
   for(int j=0;j<numberOfPools;j++)
   {
      double Nwanted=-poolList[j]->PotentialNitrogenMineralisation();
      if (Nwanted>0.0)
      {
         double Cf=poolList[j]->ResidualCarbon()/Cpot;
         double rt=min(1.0,availN*Cf/Nwanted);
         poolList[j]->FinalExport(rt);
      }
      else
         poolList[j]->FinalExport(1.0); // Probably unnescescary
   }
   NH4near=NH4near*max(0.0,min(1.0,(1.0-exchangeRate)));

   // 6. Transfer the last influxes to the pool. All theses influxes will
   // cause immobilisation.
   for(int j=0;j<numberOfPools;j++)
      poolList[j]->CatchInflux(c,0);

   // 7. Put possible N-surplus into biomatter pools - variant of "luxury
   //    uptake.

   availN=max(0.0,NO3.n+max(0.0,NH4.n-NH4allwaysLeft));
   double Ndesire=0.0;
   for(int j=0;j<numberOfPools;j++)
      Ndesire+=poolList[j]->GetMicrobialPoolNdesire();

   double ratio=0.0;
   if (Ndesire>1E-12 && availN>1E-12)
      ratio=min(1.0,availN/Ndesire);
   for(int j=0;j<numberOfPools;j++)
      poolList[j]->GiveMicrobialPoolExtraN(ratio);

   // 8. Optional "cycling" of biomass N
   if (nCycling>1E-10)
   {
      for(int j=0;j<numberOfPools;j++)
         poolList[j]->CycleNitrogen(nCycling,environmentEffect);
   }

// Decomposition finished-------------------------------------------------------

  if (NO3.n<0.0)
  {
     if (NO3.n>1E-15)
        theMessage->Warning("organicMatter::Update - NO3 below zero");
     NO3.Clear();
  }
  if (NH4.n<0.0)
  {
     if (NH4.n>1E-15)
        theMessage->Warning("organicMatter::Update - NH4 below zero");
     NH4.Clear();
  }

  *nitrate  = NO3;
  *ammonium = NH4;

   cnMatter tot1=GetTotalSystem();
   if (fabs(tot.n-tot1.n)>1E-5)
      Terminate("organicMatter::Update - imbalance in N");

   cnMatter RetVal=todayCO2;
   todayCO2.Clear();
   return RetVal.c;
}

/****************************************************************************\
Return carbon in organic matter pools [g C/m�]
\****************************************************************************/
double organicMatter::GetCarbon()
{
   cnMatter cn=GetTotalCarbon();
   if (cn.c<0.0)
      Terminate("organicMatter::GetCarbon - returns negative value");
   return cn.c;
}

/****************************************************************************\
Return nitrogen in organic matter pools [g N/m�]
\****************************************************************************/
nitrogen organicMatter::GetNitrogen()
{
   cnMatter cn=GetTotalCarbon();
   nitrogen TotalNitrogen;
   TotalNitrogen.SetBoth(cn.n,cn.n15);
   if (cn.n<0.0)
      Terminate("organicMatter::GetNitrogen - returns negative value");
   return TotalNitrogen;
}

/****************************************************************************\
Return carbon in organic matter pools with a specific name [g C/m�]
   name - Pool name
\****************************************************************************/
double organicMatter::GetPoolCarbon(const char * name)
{
   cnMatter cn=GetCarbonInPool((char*)name);
   return cn.c;
}

/****************************************************************************\
Return nitrogen in organic matter pools with a specific name [g N/m�]
   name - Pool name
\****************************************************************************/
nitrogen organicMatter::GetPoolNitrogen(const char * name)
{
   cnMatter cn=GetCarbonInPool((char*)name);
   nitrogen PoolNitrogen;
   PoolNitrogen.SetBoth(cn.n,cn.n15);
   return PoolNitrogen;
}

/****************************************************************************\
Add an organic product to the organic matter
   product - Organic product object
\****************************************************************************/
void organicMatter::AddProduct(organicProduct * product)
{
   /*
   if (product->GetAmount().c>0 && product->GetAmount().n<=0)
      Terminate("organicMatter::AddProduct - added matter contains C but no N");
   */
   double tot=GetTotalSystem().n+product->GetTotalNitrogen().n;
   if (product->GetOrganicNitrogen().n<-1E-36)
      Terminate("organicMatter::AddProduct - The added product has negative N content");
   if (useNCfractionation)
      SetFractionation(product);
   double f=product->GetCarbonFraction();

   if ((product->GetAmount()*product->GetCarbonFraction()).n<0.0)
      Terminate("organicMatter::AddProduct - negative amount of N added");
   if ((product->GetAmount()*product->GetCarbonFraction()).c<0.0)
      Terminate("organicMatter::AddProduct - negative amount of C added");
   double check=0.0;
   for(int i=0;i<product->GetNumOfFractions();i++)
      if (product->GetDestPool(i)=="CO2")
      {
         cnMatter CO2directly=product->GetAmount()
                             *product->GetCFraction(i)*f;
         check+=CO2directly.c;
         CO2emmision(CO2directly); // Direct release of CO2
      }

   // Test if animal manure needs readjustment of C fractionation
   // Coding specific for the CN-SIM model, january 2003
   double f0=product->GetCFraction(0);
   double f1=product->GetCFraction(1);
   double f2=product->GetCFraction(2);
   matter * nom=(matter*)GetPoolPointer("NOM");
   if (nom==NULL)
      Terminate("The pool 'NOM' was not found");
   double maxCToNOM=0.0;
   if (product->GetCNratio()>0.0)
      maxCToNOM = nom->GetCNratio()/product->GetCNratio();
   if (maxCToNOM<f2) // Adjust to avoid program termination
   {
      double diff=f2-maxCToNOM;
      double factor=(diff+f0+f1)/(f0+f1);
      product->SetCFraction(0,f0*factor);
      product->SetCFraction(1,f1*factor);
      product->SetCFraction(2,maxCToNOM);
   }

   double fsum=0.0;
   double used=0.0;
   double CN;
   int num;
   for(int i=0;i<numberOfPools;i++) // For pools with fixed C/N ratio
   {
      num=0;
      poolList[i]->TestAddProduct(product,&check,&CN,&num,false);
      if (num>0) // Match found
      {
         fsum+=product->GetNFraction(num);
         used+=product->GetCFraction(num)*product->GetCNratio()/CN;
      }
   }
   if (used>1.0)
      Terminate("organicMatter::AddProduct - The added product did not contain enough N to match consistently with pools");
   double factor=(1.0-used)/(1.0-fsum);
   cnMatter amount=product->GetAmount();
   amount.n=amount.n*factor;
   amount.n15=amount.n15*factor;
   product->SetAmount(amount);
   for(int i=0;i<numberOfPools;i++) // For pools without fixed C/N ratio
      poolList[i]->TestAddProduct(product,&check,&CN,&num,true);
   if (fabs(check-amount.c*f)>1E-6)
      Terminate("organicMatter::AddProduct - The added product was not consistently matched with pools for C");

   double tot1=GetTotalSystem().n;
   if (fabs(tot-tot1)>1E-5)
      Terminate("organicMatter::Update - imbalance in N");
}


/****************************************************************************\
Fraction (0-1) : fraction of SMB killed
\****************************************************************************/
void organicMatter::KillSMB(double Fraction)
{
   int count=0;
	for(int i=0;i<numberOfPools;i++)
      if ("SMB1"==poolList[i]->GetPoolName() || "SMB2"==poolList[i]->GetPoolName())
      {
         count++;
         if (!poolList[i]->ExtraTurnover(Fraction))
            Terminate("organicMatter::KillSMB - illegal call for this type of pool");
      }
   if (count!=2)
      Terminate("organicMatter::KillSMB - SMB pool(s) not found");
}

/****************************************************************************\
Fraction (0-1) : fraction of AOM transferred to pools with a modified
                 turnover rate
TurnoverFactor : Factor for modification of turnover rate
\****************************************************************************/
void organicMatter::ModifyAOM(double Fraction,double TurnoverFactor)
{
   int count=0;
	for(int i=0;i<numberOfPools;i++)
      if ("AOM1"==poolList[i]->GetPoolName() || "AOM2"==poolList[i]->GetPoolName())
      {
         count++;
         if (!poolList[i]->CloneAndModify(Fraction,TurnoverFactor))
            Terminate("organicMatter::ModifyAOM - illegal call for this type of pool");
      }
   if (count!=2)
      Terminate("organicMatter::ModifyAOM - AOM pool(s) not found");
}
