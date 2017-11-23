/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "bioMatter.h"
#include <string.h>
/****************************************************************************\
Constructor with arguments
Pname - Name of instance
Index - Instance number
owner - Pointer to owner object
\****************************************************************************/
bioMatter::bioMatter(const char * Pname,const int Index,const base * owner)
   : matter(Pname,Index,owner)
{
   maintenance=0.0;
   utilizationEfficiency=0.0;
   deathRate=0.0;
   minCNratio=-1.0;
   cnRatioDefault=-1.0;
   lackOfN=0.0;
   if (stepFactor<1E-60)
	   theMessage->FatalError("bioMatter::bioMatter - 'stepFactor' unrealisticly small");
}

/****************************************************************************\
Copy constructor
\****************************************************************************/
bioMatter::bioMatter(const bioMatter &source)
   : matter(source)
{
	   if (!source.IsBiomass())
	          theMessage->FatalError("bioMatter::bioMatter - source is not biomatter");
   maintenance=source.maintenance;
   utilizationEfficiency=source.utilizationEfficiency;
   deathRate=source.deathRate;
   minCNratio=source.minCNratio;
   cnRatioDefault=source.cnRatioDefault;
   lackOfN=source.lackOfN;
   intermedCO2=source.intermedCO2;
}

/****************************************************************************\
Returns a copy of this instance at the same inheritance level.
\****************************************************************************/
bioMatter* bioMatter::clone() const
{
	bioMatter* b = new bioMatter(*this);
	return b;
}

/****************************************************************************\
\****************************************************************************/
void bioMatter::Add(matter * source,double fraction)
{
   matter::Add(source,fraction);
   bioMatter * source1=(bioMatter*)source;
   maintenance=(1.0-fraction)*maintenance+fraction*source1->maintenance;
   utilizationEfficiency=(1.0-fraction)*utilizationEfficiency+fraction*source1->utilizationEfficiency;
   deathRate=(1.0-fraction)*deathRate+fraction*source1->deathRate;
   minCNratio=(1.0-fraction)*minCNratio+fraction*source1->minCNratio;
   cnRatioDefault=(1.0-fraction)*cnRatioDefault+fraction*source1->cnRatioDefault;
   lackOfN=(1.0-fraction)*lackOfN+fraction*source1->lackOfN;
   intermedCO2=intermedCO2*(1.0-fraction)+source1->intermedCO2*fraction;
}

/****************************************************************************\
Initializes the organic pool object
Must be called AFTER all parameters are read by all organic matter objects
\****************************************************************************/
void bioMatter::Initialize(double clayEff1,double clayEff2,int clayResp)
{
   matter::Initialize(clayEff1,clayEff2,clayResp);
   if (useClayEffect && clayResp!=0)
   {
      clayEffect1=clayEff1;
      clayEffect2=clayEff2;
   }
   if (clayResp==3) // New principle (added august 2001)
   {
      maintenance=utilizationEfficiency*deathRate+clayEff1*utilizationEfficiency*deathRate-deathRate;
      if (maintenance<=0)
          theMessage->FatalError("bioMatter::Initialize - maintenance can not be zero or below with present clayresponse");
   }
   if (clayResp==5 && poolName==2) // Principle added march 2002
      SetParameterByName("TurnoverRatio",((organicMatter*)Owner)->GetTurnoverRatioSMB1(0.1));
}

/****************************************************************************\
\****************************************************************************/
void bioMatter::SetCarbon(cnMatter c)
{
   if (c.n>0.0)
   {
      if (c.c/c.n>(cnRatio+1E-6) || c.c/c.n<(minCNratio-1E-6))
          theMessage->FatalError("bioMatter::SetCarbon - C/N ratio beyond possible borders");
   }
   else
      if (c.c>0.0)
          theMessage->FatalError("bioMatter::SetCarbon - C present in pool but no N");
   carbon=c;
}

/****************************************************************************\
Reads parameters and initial state from file
\****************************************************************************/
void bioMatter::ReadParameters(commonData * &file)
{
   matter::ReadParameters(file);
   if (stepFactor<1E-60)
       theMessage->FatalError("bioMatter::ReadParameters - 'stepFactor' unrealisticly small");
   file->setCritical(true);
   file->FindItem("UtilizationEfficiency",&utilizationEfficiency);
   file->FindItem("CNratioMin",&minCNratio);
   file->FindItem("CNratioMax",&cnRatio);
   file->setCritical(false);
   file->FindItem("CNratioDefault",&cnRatioDefault);
   if (cnRatioDefault<0.0)
      cnRatioDefault=minCNratio;
   if (minCNratio>cnRatio)
       theMessage->FatalError("bioMatter::ReadParameters - max cn ratio must be higher than min cn ratio");
   file->FindItem("DeathRate",&deathRate);
   deathRate=deathRate*stepFactor;
   file->FindItem("Maintenance",&maintenance);
   maintenance=maintenance*stepFactor;
   double tt=0.0;
   double tr=0.0;
   file->FindItem("TotalTurnover",&tt);
   tt=tt*stepFactor;
   if (file->FindItem("TurnoverRatio",&tr))
   {
      if (tt<=0.0 && tr<=0.0)
          theMessage->FatalError("bioMatter::ReadParameters - both 'TurnoverRatio' and 'TotalTurnover' must be specified and above zero");
      if (maintenance>0.0 || deathRate>0.0)
          theMessage->FatalError("bioMatter::ReadParameters - inconsistent reading of parameters");
      deathRate=tt*tr;
      maintenance=tt-deathRate;
   }
   if (deathRate<=0.0)
       theMessage->FatalError("bioMatter::ReadParameters - 'deathRate' must be above zero");

   decompositionRate=deathRate+maintenance;
}

/****************************************************************************\
\****************************************************************************/
bool bioMatter::Hungry()
{
   double m=intermed.n-intermed.c/CNcrit()
            +influx.n-(influx.c*utilizationEfficiency)/cnRatio;
   return (m<0.0);
}

/****************************************************************************\
\****************************************************************************/
double bioMatter::GetCNcrit()
{
   double retVal=1E6;
   if (utilizationEfficiency>0.0)
      retVal=cnRatio/utilizationEfficiency;
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double bioMatter::ResidualCarbon()
{
   return intermed.c+intermedCO2.c;
}

/****************************************************************************\
\****************************************************************************/
double bioMatter::PotentialNitrogenMineralisation()
{
   double retVal=0.0;
//  if (!Hungry())
//  {
      if (cnRatio>0)
      {
         retVal=influx.n-(influx.c*utilizationEfficiency)/cnRatio;
         if (intermed.c>0.0)
            retVal+=intermed.n-intermed.c/CNcrit();
      }
      else
          theMessage->FatalError("C/N ratio not set for the '",poolNameChar,"' pool");
//    if (retVal<-1E-12)
//          theMessage->FatalError("bioMatter::PotentialNitrogenMineralisation - returns negative value");
//   }
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double bioMatter::PotentialNitrogenImmobilisation()
{
   double retVal=0.0;
   if(Hungry())
   {
      if (cnRatio>0)
         retVal=influx.n-(influx.c*utilizationEfficiency)/cnRatio;
      else
          theMessage->FatalError("C/N ratio not set for the '",poolNameChar,"' pool");
   }
   if (retVal>1E-12)
       theMessage->FatalError("bioMatter::PotentialNitrogenImmobilisation - returns positive value");
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
void bioMatter::FinalExport(double ratio)
{
   if (ratio>1.0 || ratio<0.0)
        theMessage->FatalError("matter::FinalExport - 'ratio' has illegal value");
   if(intermed.c>0.0)
   {
      carbon=carbon+(intermed+intermedCO2)*(1.0-ratio);
      Export(intermed*ratio);
      ((organicMatter*)Owner)->CO2emmision(intermedCO2*ratio);
      intermed.SetAll(0.0,0.0,0.0,0.0);
      intermedCO2.SetAll(0.0,0.0,0.0,0.0);
   }
}

/****************************************************************************\
\****************************************************************************/
void bioMatter::CycleNitrogen(double nCycling,double environmentEffect)
{
   nitrogen nit0,nit1;
   nit0.n=carbon.n*nCycling;
   nit1.n=-nit0.n;
   nit0.n15=carbon.n15*nCycling;
   carbon.n-=nit0.n;
   carbon.n15-=nit0.n15;
   ((organicMatter*)Owner)->ExchangeNitrogen(&nit0);
   ((organicMatter*)Owner)->ExchangeNitrogen(&nit1);
   carbon.n-=nit1.n;
   carbon.n15-=nit1.n15;

}




/****************************************************************************\
Calculates the death of the biomatter in the pool and transfers the carbon
to the receiver pools.
Calculates respiration and transfers the carbon to the CO2 book-keeping
function in the owner class.
environmentEffect - covers combined effects of water content and temperature.
// OBS maintanancerespiration not accounted correctly when using Runge-Kutta
\****************************************************************************/
void bioMatter::Update(double environmentEffect)
{
   if (carbon.c>0 && carbon.n<=0)
   {
      cout << "Name path: " << GetLongName() << endl;
      cout << theTime << endl;
       theMessage->FatalError("bioMatter::Update - C present but no N (1)");
   }
   else
   {
      double cnrat=carbon.c/carbon.n;
      if (fabs(cnrat-cnRatio)>0.01)
          theMessage->FatalError("bioMatter::Update - illegal C/N ratio");
   }
   if (maintenance<=0.0)
       theMessage->FatalError("bioMatter::Update - 'maintenance' must be above zero");
   TestState();
   if (carbon.n<-1E-10)
       theMessage->FatalError("bioMatter::Update - negative N content");
   if (intermed.c>0.0 || intermedCO2.c>0.0)
       theMessage->FatalError("bioMatter::Update - programming inconsistency");

   if ( carbon.c>1E-45)
   {
      double dr=deathRate;
      double mr=maintenance;
      if (clayResponseType==5)
      {
         double tot=dr+mr;
         dr=dr*clayEffect2;
         mr=tot-dr;
      }
      cnMatter totalLoss;
      double cl=clayEffect1;
      if (clayResponseType==2 || clayResponseType==3 || !useClayEffect)
         cl=1.0; // Like RothC, new or unused
      dr=dr*environmentEffect*cl; // Like DAISY, otherwise "cl" is one.
      mr=mr*environmentEffect*cl; // Both DAISY and "homemade", otherwise "cl" is one
      double tr=dr+mr;
      if (tr>0.0)
      {
         if (expDecay)
            totalLoss=carbon*(1-exp(-tr));
         else // Euler integration
            totalLoss=carbon*tr;
         if (totalLoss.c>carbon.c)
            totalLoss=carbon;
         intermed=totalLoss*dr/tr;
         intermedCO2=totalLoss*mr/tr;
      }
      if (intermed.c>carbon.c)
         intermed=carbon;
      TestState();
      carbon=carbon-intermedCO2-intermed;
      TestState();
      intermed.n+=intermedCO2.n;
      intermed.n15+=intermedCO2.n15;
      intermedCO2.n=0.0;
      intermedCO2.n15=0.0;
   }
   if (intermed.n>0.0)
   {
      if ((intermed.c/intermed.n)<=CNcrit())
      {
         Export(intermed);
         ((organicMatter*)Owner)->CO2emmision(intermedCO2);
         intermed.SetAll(0.0,0.0,0.0,0.0);
         intermedCO2.SetAll(0.0,0.0,0.0,0.0);
      }
   }

   TestState();
   if (carbon.n<-1E-10)
       theMessage->FatalError("bioMatter::Update - negative N content");
   if (carbon.c>0 && carbon.n<=0)
       theMessage->FatalError("bioMatter::Update - C present but no N (2)");
   else
   {
      double cnrat=carbon.c/carbon.n;
      if (fabs(cnrat-cnRatio)>0.01)
          theMessage->FatalError("bioMatter::Update - illegal C/N ratio");
   }
}

/****************************************************************************\
Adds all import to the pool, and returns the current carbon content.
Calculates respiration from nurishment utilisation and transfers the carbon
to the CO2 book-keeping function in the owner class.
content - C content in pool
\****************************************************************************/
void bioMatter::CatchInflux(cnMatter &content,int mode)
{
   cnMatter utilizationRespiration;
   utilizationRespiration.c=influx.c*(1-utilizationEfficiency);
   utilizationRespiration.c_iso=influx.c_iso*(1-utilizationEfficiency);

   if (mode!=5)
   {
      carbon.c=carbon.c+(influx.c+added.c)*utilizationEfficiency;
      carbon.c_iso=carbon.c_iso+(influx.c_iso+added.c_iso)*utilizationEfficiency;
      carbon.n+=influx.n+added.n;
      carbon.n15+=influx.n15+added.n15;
      lackOfN+=(influx.c+added.c)*utilizationEfficiency*(1.0/minCNratio-1.0/cnRatio);
   }
   ((organicMatter*)Owner)->CO2emmision(utilizationRespiration);
   if (carbon.c>0.0)
   {
      if (cnRatio<=0.0)
          theMessage->FatalError("bioMatter::CatchInflux - 'cnRatio' has illegal value for this instance");
      nitrogen nit;
      nit.n=(influx.n+added.n)-utilizationEfficiency*(influx.c+added.c)/cnRatio;
      if (nit.n>0.0)
         nit.n15=carbon.n15/carbon.n*nit.n;
      carbon.n-=nit.n;
      ((organicMatter*)Owner)->ExchangeNitrogen(&nit);
      carbon.n15-=nit.n15;
   }

   influx.SetAll(0.0,0.0,0.0,0.0);
   if ((mode==0 || mode==5)) // Keep if not update-mode
      added.SetAll(0.0,0.0,0.0,0.0);

   content=carbon;
   if (mode>0 && mode<5)
      carbonDerivative[mode]=carbon-carbonDerivative[5];
}

/****************************************************************************\
\****************************************************************************/
void bioMatter::GiveMicrobialPoolExtraN(double r)
{
   if (r<0.0)
       theMessage->FatalError("bioMatter::GiveMicrobialPoolExtraN - ratio can not be below zero");
   if (r>0.0 && lackOfN>0.0)
   {
      nitrogen nit;
      nit.n=-lackOfN*r;
      ((organicMatter*)Owner)->ExchangeNitrogen(&nit);
      carbon.n-=nit.n;
      carbon.n15-=nit.n15;
   }
   lackOfN=0.0;
}

/****************************************************************************\
\****************************************************************************/
bool bioMatter::SetParameterByName(string parmName,double val)
{
   if (val<0.0)
      val=0.0;
   bool found=matter::SetParameterByName(parmName,val);

   if (parmName=="CNratioMin")
   {
      if (val>cnRatio)
         val=cnRatio;
      minCNratio=val;
      found=true;
   }

   if (parmName=="CNratioMax")
   {
      if (val<minCNratio)
         val=minCNratio;
      cnRatio=val;
      found=true;
   }

   if (parmName=="UtilizationEfficiency")
   {
      if (val>1.0)
         val=1.0;
      utilizationEfficiency=max(0.001,val);
      found=true;
   }
   if (parmName=="Maintenance")
   {
      maintenance=val*stepFactor;
      found=true;
   }
   if (parmName=="DeathRate")
   {
      deathRate=val*stepFactor;
      found=true;
   }
   if (parmName=="TotalTurnover")
   {
      if (val<1E-7)
         val=1E-7;
      double tot=deathRate+maintenance;
      if (tot<=0.0)
          theMessage->FatalError("bioMatter::SetParameterByName - sum of rates must be above zero");
      double q=val*stepFactor/tot;
      maintenance=maintenance*q;
      deathRate=deathRate*q;
      found=true;
      if (fabs(val*stepFactor-deathRate-maintenance)>val*1E-5)
      {
         cout << "Warning: bioMatter::SetParameterByName - possible programming error 1";
#ifdef __BCplusplus__
         cout << "Press any key to continue" << endl;
         getch();
#endif
      }
   }
   if (parmName=="TurnoverRatio")
   {
      if (val>0.9999)
         val=0.9999;
      if (val<0.0001)
         val=0.0001;
      double tot=deathRate+maintenance;
      if (tot<=0.0)
          theMessage->FatalError("bioMatter::SetParameterByName - sum of rates must be above zero");
      deathRate=tot*val;
      maintenance=tot-deathRate;
      if (fabs(tot-deathRate-maintenance)>1E-5)
      {
         cout << "Warning: bioMatter::SetParameterByName - possible programming error 2";
#ifdef __BCplusplus__
         cout << "Press any key to continue" << endl;
         getch();
#endif
      }
      if (fabs(val-deathRate/(deathRate+maintenance))>1E-5)
      {
         cout << "Warning: bioMatter::SetParameterByName - possible programming error 3";
#ifdef __BCplusplus__
         cout << "Press any key to continue" << endl;
         getch();
#endif
      }
      found=true;
   }
   return found;
}


/****************************************************************************\
\****************************************************************************/
cnMatter bioMatter:: GetCarbon()
{return carbon+intermed+intermedCO2+influx+added;};
double bioMatter:: GetCarbonC()
{return carbon.c+intermed.c+intermedCO2.c+influx.c+added.c;};
double bioMatter::GetNitrogenN()
{
	return carbon.n+intermed.n+intermedCO2.n+influx.n+added.n;
}
double bioMatter::GetNitrogenN15()
{
	return carbon.n15+intermed.n15+intermedCO2.n15+influx.n15+added.n15;
}
nitrogen bioMatter::GetNitrogen()
{
	nitrogen tmp;
	double n=carbon.n+intermed.n+intermedCO2.n+influx.n+added.n;
	double n15=carbon.n15+intermed.n15+intermedCO2.n15+influx.n15+added.n15;
	tmp.SetBoth(n,n15);
	return tmp;
}
/****************************************************************************\
\****************************************************************************/
bool bioMatter::ExtraTurnover(double Fraction)
{
   double dr=deathRate;
   double mr=maintenance;
   if (clayResponseType==5)
   {
      double tot=dr+mr;
      dr=dr*clayEffect2;
      mr=tot-dr;
   }
   cnMatter totalLoss;
   double factor = Fraction/(dr+mr);
   dr=factor*dr;
   mr=factor*mr;
   double tr=dr+mr;
   if (fabs(tr-Fraction)>1E-10)
       theMessage->FatalError("bioMatter::ExtraTurnover - coding error!");
   tr=Fraction;

   totalLoss=carbon*tr;
   intermed=totalLoss*dr/tr;
   intermedCO2=totalLoss*mr/tr;
   carbon=carbon-intermedCO2-intermed;
   TestState();
   intermed.n+=intermedCO2.n;
   intermed.n15+=intermedCO2.n15;
   intermedCO2.n=0.0;
   intermedCO2.n15=0.0;

   if (intermed.n>0.0)
   {
      if ((intermed.c/intermed.n)<=CNcrit())
      {
         Export(intermed);
         ((organicMatter*)Owner)->CO2emmision(intermedCO2);
         intermed.SetAll(0.0,0.0,0.0,0.0);
         intermedCO2.SetAll(0.0,0.0,0.0,0.0);
      }
   }
   double cnrat=carbon.c/carbon.n;
   if (fabs(cnrat-cnRatio)>0.01)
       theMessage->FatalError("bioMatter::Update - illegal C/N ratio");
   return true;
}


