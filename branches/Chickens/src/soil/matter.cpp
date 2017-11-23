/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

#include "matter.h"

/****************************************************************************\
Constructor with arguments
Pname - Name of instance
Index - Instance number
owner - Pointer to owner object
\****************************************************************************/
matter::matter(const char * Pname,const int Index,const base * owner)
   : base(Pname,Index,owner)
{
   carbon.Clear();
   influx.Clear();
   intermed.Clear();
   added.Clear();
   Isotope=0;
   cnRatio=-1.0;
   stepFactor=1.0;
   poolName="";
   decompositionRate=0.0;
   clayEffect1=1.0;
   clayEffect2=1.0;
   useClayEffect=false;
   expDecay=false;
   connections=0;
   clayResponseType=0;
   TAG=-1;
   CNC=-1.0;
}

/****************************************************************************\
Copy constructor
\****************************************************************************/
matter::matter(const matter &source)
   : base(source)
{
   cnRatio=source.cnRatio;
   if (!source.IsAOM())
      if (cnRatio<=0)
      {
    	  cout <<"CNratio is "<< cnRatio<<endl;
         Terminate("matter::matter - illegal state of cnRatio variable");
      }
   stepFactor=source.stepFactor;
   poolName=source.poolName;
   decompositionRate=source.decompositionRate;
   clayEffect1=source.clayEffect1;
   clayEffect2=source.clayEffect2;
   useClayEffect=source.useClayEffect;
   expDecay=source.expDecay;
   connections=source.connections;
   clayResponseType=source.clayResponseType;
   TAG=source.TAG;
   CNC=source.CNC;
   carbon=source.carbon;
   influx=source.influx;
   intermed=source.intermed;
   added=source.added;
   Isotope=source.Isotope;
   for (int i=0;i<source.connections;i++)
   {
      connectionList[i].connectPt=NULL;
      connectionList[i].Cfraction=source.connectionList[i].Cfraction;
      connectionList[i].Nfraction=source.connectionList[i].Nfraction;
      connectionList[i].connectname=source.connectionList[i].connectname;
   }
}

/****************************************************************************\
Returns a copy of this instance at the same inheritance level.
\****************************************************************************/
matter* matter::clone() const
{
	matter* m = new matter(*this);
	return m;
}

/****************************************************************************\
\****************************************************************************/
void matter::Scale(double fraction)
{
   carbon=carbon*fraction;
   intermed=intermed*fraction;
   influx=influx*fraction;
   added=added*fraction;
}

/****************************************************************************\
Several conditions assumed equal, eg. for connections, but this is not tested.
\****************************************************************************/
void matter::Add(matter * source,double fraction)
{
   if (poolName!=source->poolName)
      Terminate("matter::Add - pool names must be identical");
   cnRatio=(1.0-fraction)*cnRatio+fraction*source->cnRatio;
   decompositionRate=(1.0-fraction)*decompositionRate+fraction*source->decompositionRate;
   CNC=(1.0-fraction)*CNC+fraction*source->CNC;
   carbon=carbon*(1.0-fraction)+source->carbon*fraction;
   influx=influx*(1.0-fraction)+source->influx*fraction;
   intermed=intermed*(1.0-fraction)+source->intermed*fraction;
   added=added*(1.0-fraction)+source->added*fraction;
   //These are assumed equal, anbd thus unaffected: stepFactor,clayEffect1,clayEffect2,useClayEffect,expDecay,connections,clayResponseType
}

/****************************************************************************\
Reads parameters and initial state from file
\****************************************************************************/
void matter::ReadParameters(fstream * file)
{
   char charvar[40];
   Setfile(file);
   SetCritical();
   if (FindSection(Name,Index)) // Top of inheritance hierarcy
   {
      GetParameter("Name",charvar);
      poolName=charvar;
      UnsetCritical();
      GetParameter("CNratio",&cnRatio);
      GetParameter("UseClayEffect",&useClayEffect);
      GetParameter("DecompositionRate",&decompositionRate);
      decompositionRate=decompositionRate*stepFactor;
      GetParameter("ExponentialDecay",&expDecay);
      GetParameter("TAG",&TAG);
      if (!(TAG==-1 || TAG==0 || TAG==1))
         Terminate("'TAG' must be either 0 or 1 in structurefile");
      SetCritical();
      // Read connections
      double checkN=0.0;
      double checkC=0.0;
      int first,n;
      GetVectorInfo("Direction",&first,&n);
      if (first>0)
         Terminate("Vector parameter numbering must start with zero in structurefile"); // Make text more specific
      for(int i=first;i<(first+n);i++)
      {
         GetParameter("Direction",&connectionList[i].connectname,i);
         if (GetParameter("Fraction",&connectionList[i].Cfraction,i))
            connectionList[i].Nfraction=connectionList[i].Cfraction;
         UnsetCritical();
         GetParameter("FractionN",&connectionList[i].Nfraction,i);
         checkC+=connectionList[i].Cfraction;
         checkN+=connectionList[i].Nfraction;
         connections++;
      }
      if (fabs(checkC-1.0)>1E-6 || fabs(checkN-1.0)>1E-6)
      {
         cout << "Incorrect sum of fractions in " << Name << "." << Index << endl;
         Terminate();
      }
   }
   Setfile(NULL);
}

/****************************************************************************\
\****************************************************************************/
cnMatter matter::GetCarbon()
{
   return carbon+intermed+influx+added;
}

/****************************************************************************\
\****************************************************************************/
double matter::ResidualCarbon()
{
   return intermed.c;
}

/****************************************************************************\
\****************************************************************************/
double matter::PotentialNitrogenImmobilisation()
{
   double retVal=0.0;
   if(Hungry())
   {
      retVal=intermed.n-intermed.c/CNcrit();
      if (cnRatio>0)
         retVal+=influx.n-influx.c/cnRatio;
   }
   if (retVal>1E-12)
      Terminate("matter::PotentialNitrogenImmobilisation - returns positive value");
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double matter::PotentialNitrogenMineralisation()
{
   double retVal=0.0;
   retVal=intermed.n-intermed.c/CNcrit();
   if (cnRatio>0)
      retVal+=influx.n-influx.c/cnRatio;
   if (retVal>1E-10 && intermed.c>1E-10)
   {
      cout << endl << "Warning: matter::PotentialNitrogenMineralisation - possible programming inconsistency" << endl;
      cout << "Press any key to continue" << endl;
#ifndef __BCplusplus__
      char dummy;
      cin >> dummy;
#else
      getch();
#endif
   }
   return retVal;
}

/****************************************************************************\
Initializes the organic pool object
Must be called AFTER all parameters are read by all organic matter objects
\****************************************************************************/
void matter::Initialize(double clayEff1,double clayEff2,int clayResp)
{
   clayResponseType=clayResp;
   if (useClayEffect && clayResp!=0)
   {
      clayEffect1=clayEff1;
      clayEffect2=clayEff2;
   }
   for (int i=0;i<connections;i++)
      connectionList[i].connectPt=((organicMatter*)Owner)->GetPoolPointer(connectionList[i].connectname);
}

/****************************************************************************\
Adds carbon to the current carbon content in the pool, via a "transit deposit"
c - Carbon to add to the current content
\****************************************************************************/
void matter::Import(cnMatter c)
{
   if (c.c<0)
      Terminate("C import to pool negative");
   if (c.n<0)
      Terminate("N import to pool negative");
   influx=influx+c;
}

/****************************************************************************\
Investigates whether one of the fractions of the added matter fits with the
current pool. If this is the case, the matter is added
prod - pointer to organic product
\****************************************************************************/
void matter::TestAddProduct(organicProduct * prod,double * check,double * CN,int * num,bool forAOM)
{
   *num=0;
   if (!forAOM)
   {
      for(int i=0;i<prod->GetNumOfFractions();i++)
         if (poolName==prod->GetDestPool(i))
         {
            if (true /* !!!!!!!!!!!!!!!!! */) // A crude version of indirect MIT !!!
            {
               cnMatter am = prod->GetAmount();
               ((organicMatter*)Owner)->EquilibrateN15(am);
               prod->SetAmount(am);
            }
            AddOrganicProduct(prod,i,check);
            *num=i;
            *CN=cnRatio;
         }
   }
}

/****************************************************************************\
Adds a fraction of an organic product to the present pool
prod           - pointer to organic product
fractionNumber - number of fraction to be added
\****************************************************************************/
void matter::AddOrganicProduct(organicProduct * prod,int fractionNumber,double * check)
{
   cnMatter add;
   cnMatter prd=prod->GetAmount();
   add.c=prd.c*prod->GetCFraction(fractionNumber)*prod->GetCarbonFraction();
   add.c_iso=prd.c_iso*prod->GetCFraction(fractionNumber)*prod->GetCarbonFraction();
 //  double nf=prod->GetNFraction(fractionNumber);
   add.n=prd.n*prod->GetNFraction(fractionNumber);
   add.n15=prd.n15*prod->GetNFraction(fractionNumber);
   if (!IsAOM())
   {  // Adjust to match C/N ratio
      if (cnRatio<=0.0)
         Terminate("matter::AddOrganicProduct - C/N ratio should not be zero or negative here");
   	double addN=add.c/cnRatio;
      double factor=0.0;
      if (add.n>0)
         factor = addN/add.n;
      add.n15=factor*add.n15;
      add.n=addN;
   }
   *check+=add.c;
   added=added+add;
   if (add.n<0.0)
      Terminate("matter::AddOrganicProduct - negative amount of N added");
   if (add.c<0.0)
      Terminate("matter::AddOrganicProduct - negative amount of C added");
}

/****************************************************************************\
For the moment this function simply returns the highest C/N ratio of all
receiver pools.
\****************************************************************************/
double matter::CNcrit()
{
   if (CNC<=0)
   {
      for (int i=0;i<connections;i++)
      {
         if (connectionList[i].connectPt)
         {
            double cn=connectionList[i].connectPt->GetCNcrit();
            if (cn<=0)
               Terminate("addedMatter::CalcCNcrit - a pool has undefined CNcrit");
            if (cn<CNC || CNC<0.0)
               CNC=cn;
         }
         else
         {
            double n=connectionList[i].Nfraction; // Any N to CO2 is mineralised, but should generally not occur
            if (n>0.0)
               cout << "Warning - no N should flow to directly to CO2" << endl;
         }
      }
   }
   return CNC;
}

/****************************************************************************\
\****************************************************************************/
bool matter::Hungry()
{
   double m=intermed.n-intermed.c/CNcrit();
   if (cnRatio>0)
      m+=influx.n-influx.c/cnRatio;
   return (m<0.0);
}

/****************************************************************************\
\****************************************************************************/
void matter::TestState()
{
   if (carbon.c_iso<0.0)
      carbon.c_iso=0.0;
   if (carbon.n15<0.0)
      carbon.n15=0.0;
   if (carbon.c<-1E-10)
      Terminate("matter::TestState - negative amount of C");
   if (carbon.n<-1E-10)
      Terminate("matter::TestState - negative amount of N");
   if (!IsAOM())
      if (cnRatio<=0)
         Terminate("matter::TestState - illegal state of cnRatio variable");
}

/****************************************************************************\
Calculates the decomposition of the organic matter in the pool and transfers
the carbon to the receiver pools.
environmentEffect - covers combined effects of water content and temperature.
\****************************************************************************/
void matter::Update(double environmentEffect,int mode)
{
   TestState();
   if (intermed.c>0.0)
      Terminate("matter::Update - programming inconsistency");
   intermed.SetAll(0.0,0.0,0.0,0.0);
   switch (mode)
   {
      case 1: carbonDerivative[0]=carbon; // Store original value
              carbonDerivative[5]=carbon;
              break;
      case 2: carbon=carbonDerivative[0]+carbonDerivative[1]*0.5;
              if (carbon.c<0.0)
              {
                 carbon.c=0.0;
                 carbon.c_iso=0.0;
              }
              carbonDerivative[5]=carbon;
              break;
      case 3: carbon=carbonDerivative[0]+carbonDerivative[2]*0.5;
              if (carbon.c<0.0)
              {
                 carbon.c=0.0;
                 carbon.c_iso=0.0;
              }
              carbonDerivative[5]=carbon;
              break;
      case 4: carbon=carbonDerivative[0]+carbonDerivative[3];
              if (carbon.c<0.0)
              {
                 carbon.c=0.0;
                 carbon.c_iso=0.0;
              }
              carbonDerivative[5]=carbon;
              break;
      case 5: carbon=carbonDerivative[0]+carbonDerivative[1]/6.0+carbonDerivative[2]/3.0
                    +carbonDerivative[3]/3.0+carbonDerivative[4]/6.0;
              intermed=outfluxDerivative[1]/6.0+outfluxDerivative[2]/3.0
                     +outfluxDerivative[3]/3.0+outfluxDerivative[4]/6.0;
   }
   if (mode!=5 && carbon.c>1E-45)
   {
      double cl=1.0;
      if (clayResponseType==1 && useClayEffect) // Like DAISY, the only one that affects turnover in this code
         cl=clayEffect1;
      if (expDecay) // Exponential decay
         intermed=carbon*(1-exp(-decompositionRate*environmentEffect*cl));
      else // Euler integration
         intermed=carbon*decompositionRate*environmentEffect*cl;
      if (intermed.c>carbon.c)
         intermed=carbon;
      carbon=carbon-intermed;
   }
   if (mode>0 && mode<5)
      outfluxDerivative[mode]=intermed;
   if (intermed.n>0.0)
   {
      if ((intermed.c/intermed.n)<=CNcrit())
      {
         Export(intermed);
         intermed.SetAll(0.0,0.0,0.0,0.0);
      }
   }
   TestState();
}

/****************************************************************************\
Adds all import to the pool, and returns the current carbon content
content - C content in pool
\****************************************************************************/
void matter::CatchInflux(cnMatter &content,int mode)
{
   if (influx.c<0.0)
      Terminate("Influx to pool negative");
   cnMatter in=influx+added;
   if ((in.c)>0.0)
   {
      if (in.n>0)
      {
         if (cnRatio<=0.0)
            Terminate("matter::CatchInflux - 'cnRatio' has illegal value for this instance");
         nitrogen nit;
         nit.n=in.n-in.c/cnRatio;
         nit.n15=in.n15/in.n*nit.n;
         in.n-=nit.n;
         ((organicMatter*)Owner)->ExchangeNitrogen(&nit);
         in.n15-=nit.n15;
      }
      else
         Terminate("matter::CatchInflux - C with no associated N");
   }
   if (mode!=5)
      carbon=carbon+in;
   influx.SetAll(0.0,0.0,0.0,0.0);
   if ((mode==0 || mode==5)) // Keep if not update-mode
      added.SetAll(0.0,0.0,0.0,0.0);

   content=carbon;
   if (mode>0 && mode<5)
      carbonDerivative[mode]=carbon-carbonDerivative[5];
}

/****************************************************************************\
\****************************************************************************/
void matter::FinalExport(double ratio)
{
   if (ratio>1.0 || ratio<0.0)
       Terminate("matter::FinalExport - 'ratio' has illegal value");
   if(intermed.c>0.0)
   {
      carbon=carbon+intermed*(1.0-ratio);
      Export(intermed*ratio);
      intermed.SetAll(0.0,0.0,0.0,0.0);
   }
}

/****************************************************************************\
Exports carbon to other pools
c - Carbon to export
\****************************************************************************/
void matter::Export(cnMatter c)
{
   if (c.c>0.0)
   {
      if (Isotope==3 && TAG>=0) // "Tagging" should be performed
      {
         cnMatter balance=c;
         if (TAG==1)
            c.c_iso=c.c;
         if (TAG==0)
            c.c_iso=0.0;
         balance=balance-c;
      }
      if (clayResponseType==2 && useClayEffect) // Like RothC
      {
         double f=1.0/(1.0+clayEffect1);
         ((organicMatter*)Owner)->CO2emmision(c*(1.0-f)); // Flux directly to CO2
         c=c*f;                                       // Keep the rest for other flux
      }
      if (IsAOM() && false /*!!!!!!!!!!!!!!!!!!!!!!*/) // A crude version of indirect MIT
         ((organicMatter*)Owner)->EquilibrateN15(c);
      for (int i=0;i<connections;i++)
      {
         if (connectionList[i].Cfraction<0.0 || connectionList[i].Cfraction>1.0000001)
            Terminate("matter::Export - C fraction is illegal for ",(char*)poolName.c_str());
         if (connectionList[i].Nfraction<0.0 || connectionList[i].Nfraction>1.0000001)
            Terminate("matter::Export - N fraction is illegal for ",(char*)poolName.c_str());
         cnMatter exportMatter;
         exportMatter.SetAll(c.c*connectionList[i].Cfraction,
                       c.c_iso*connectionList[i].Cfraction,
                       c.n*connectionList[i].Nfraction,
                       c.n15*connectionList[i].Nfraction);

         if (connectionList[i].connectPt)
            connectionList[i].connectPt->Import(exportMatter);
         else
            ((organicMatter*)Owner)->CO2emmision(exportMatter); // Pointer NULL, so flux directly to CO2
      }
   }
}


/****************************************************************************\
\****************************************************************************/
void matter::SetConnectionFraction(int nr,double frC,double frN)
{
   connectionList[nr].Cfraction=frC;
   if (frN<0.0)
      connectionList[nr].Nfraction=frC;
   else
      connectionList[nr].Nfraction=frC;
}

/****************************************************************************\
\****************************************************************************/
double matter::GetConnectionFraction(string poolName)
{
   double retval=0.0;
   for (int i=0;i<connections;i++)
   {
      if (connectionList[i].connectname==poolName)
         retval=connectionList[i].Cfraction;
   }
   return retval;
}

/****************************************************************************\
\****************************************************************************/
bool matter::SetParameterByName(string parmName,double val)
{
   if (val<0.0)
      val=0.0;
   bool found=false;
   if (parmName=="CNratio")
   {
      if (val<1.0)
         val=1.0;
      cnRatio=val;
      found=true;
      if (carbon.c>0.0 && carbon.n>0.0)
      {
         double q=carbon.c/carbon.n;
         q=q/cnRatio;
         carbon.n=carbon.n*q;
         carbon.n15=carbon.n15*q;
      }
   }
   if (parmName=="DecompositionRate")
   {
      decompositionRate=val*stepFactor;
      found=true;
   }
   if (parmName=="ClayEffect1")
   {
      clayEffect1=val;
      found=true;
   }
   if (parmName=="ClayEffect2")
   {
      clayEffect2=val;
      found=true;
   }
   if (parmName=="PM" && Isotope==2)
   {
      double q=100.0*carbon.c_iso/carbon.c;
      q=max(0.0,val/q);
      carbon.c_iso=carbon.c_iso*q;
      found=true;
   }
   if (parmName=="Fraction(0)") // Hack, only valid for 2 connections !!!
   {
      if (connections!=2)
         Terminate("matter::SetParameterByName - can only set fraction by two connections");
      if (val>1.0)
         val=1.0;
      double f1=val;
      double f2=1.0-f1;
      SetConnectionFraction(0,f1);
      SetConnectionFraction(1,f2);
      found=true;
   }
   if (parmName=="Fraction(1)") // Hack, only valid for 2 connections !!!
   {
      if (connections!=2)
         Terminate("matter::SetParameterByName - can only set fraction by two connections");
      if (val>1.0)
         val=1.0;
      double f2=min(1.0,max(val,0.0));
      double f1=1.0-f2;
      SetConnectionFraction(0,f1);
      SetConnectionFraction(1,f2);
      found=true;
   }
   return found;
}

/****************************************************************************\
\****************************************************************************/
bool matter::ExtraTurnover(double Fraction)
{
   return false;
}

/****************************************************************************\
\****************************************************************************/
bool matter::CloneAndModify(double Fraction,double TurnoverFactor)
{
   return false;
}

