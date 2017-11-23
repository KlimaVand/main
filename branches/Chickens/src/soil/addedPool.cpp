/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "addedPool.h"

/****************************************************************************\
\****************************************************************************/
addedPool::addedPool(const char * Pname,const int Index,const base * owner)
   : matter(Pname,Index,owner)
{
   poolName="AOM-subpool";
   ReduceTurnover = 0.0;
Derived=false;
}

/****************************************************************************\
Copy constructor
\****************************************************************************/
addedPool::addedPool(const addedPool &source)
   : matter(source)
{
   poolName=source.poolName;
   ReduceTurnover = source.ReduceTurnover;
}

/****************************************************************************\
Returns a copy of this instance at the same inheritance level.
\****************************************************************************/
addedPool* addedPool::clone() const
{
	addedPool* a = new addedPool(*this);
	return a;
}

/****************************************************************************\
Should be extended !!!
\****************************************************************************/
bool addedPool::Alike(addedPool * aP)
{
   return(aP->GetDecompositionRate()==decompositionRate);
}

/****************************************************************************\
\****************************************************************************/
void addedPool::SoakDry(addedPool * aP)
{
   carbon=carbon+aP->carbon;
   intermed=intermed+aP->intermed;
   influx=influx+aP->influx;
   added=added+aP->added;
   aP->carbon.SetAll(0,0,0,0);
   aP->intermed.SetAll(0,0,0,0);
   aP->influx.SetAll(0,0,0,0);
   aP->added.SetAll(0,0,0,0);
}

/****************************************************************************\
\****************************************************************************/
void addedPool::Decay(double environmentEffect,cnMatter &out,int mode,double cnc)
{
   TestState();
   if (!IsAOM())
      if (cnRatio<=0)
         Terminate("matter::TestState - illegal state of cnRatio variable");
   CNC=cnc;
   intermed.SetAll(0.0,0.0,0.0,0.0);
   switch (mode)
   {
      case 1: carbonDerivative[0]=carbon; // Store original value
              carbonDerivative[5]=carbon;
              break;
      case 2: carbon=carbonDerivative[0]+carbonDerivative[1]*0.5;
              if (carbon.c<0.0)
                 carbon.SetAll(0.0,0.0,0.0,0.0);
              carbonDerivative[5]=carbon;
              break;
      case 3: carbon=carbonDerivative[0]+carbonDerivative[2]*0.5;
              if (carbon.c<0.0)
                 carbon.SetAll(0.0,0.0,0.0,0.0);
              carbonDerivative[5]=carbon;
              break;
      case 4: carbon=carbonDerivative[0]+carbonDerivative[3];
              if (carbon.c<0.0)
                 carbon.SetAll(0.0,0.0,0.0,0.0);
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
      if (clayResponseType==1 && useClayEffect) // Like DAISY, only one that affects turnover in the present code
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
   if (intermed.c>0.0)
   {
      double q=1E30;
      if (intermed.n>1E-30)
         q=intermed.c/intermed.n;
      if (q<=cnc)
      {
         out=intermed;
         intermed.SetAll(0.0,0.0,0.0,0.0);
      }
   }
   TestState();
}

/****************************************************************************\
Adds all import to the pool, and returns the current carbon content
content - C content in pool
\****************************************************************************/
void addedPool::CatchInflux(cnMatter &content,int mode)
{
   if (influx.c<0.0)
      Terminate("addedPool::CatchInflux - C influx to pool negative");
   if (influx.n<0.0)
      Terminate("addedPool::CatchInflux - N influx to pool negative");
   if (influx.c>0.0 && influx.n<=0)
      Terminate("addedPool::CatchInflux - Influx to pool has C but no N");
   if (mode!=5)
   {
      carbon=carbon+influx;
      carbon=carbon+added;
   }
   influx.SetAll(0.0,0.0,0.0,0.0);
   if ((mode==0 || mode==5)) // Keep if not update-mode
      added.SetAll(0.0,0.0,0.0,0.0);
   if (mode>0 && mode<5)
      carbonDerivative[mode]=carbon-carbonDerivative[5];
   content=carbon+intermed;
}



/****************************************************************************\
\****************************************************************************/
bool addedPool::Hungry()
{
   double c=carbon.c+intermed.c;
   double n=carbon.n+intermed.n;
   bool retVal=false;
   if (c>0.0)
   {
	   if (n>0.0)

         retVal=(c/n>CNC);
      else
         retVal=true;
   }
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
void addedPool::FinalExportAOM(double ratio,cnMatter &out)
{
   if (ratio>1.0 || ratio<0.0)
       Terminate("addedPool::FinalExportAOM - 'ratio' has illegal value");
   if(intermed.c>0.0)
   {
      carbon=carbon+intermed*(1.0-ratio);
      out=out+intermed*ratio;
      intermed.SetAll(0.0,0.0,0.0,0.0);
   }
}


/****************************************************************************\
\****************************************************************************/
void addedPool::SetReduceTurnover(double Reduce)
{
	ReduceTurnover = Reduce;
   if (Reduce>0.0)
	   decompositionRate = decompositionRate*Reduce;
}

/****************************************************************************\
\****************************************************************************/
void addedPool::ReleaseReduceTurnover()
{
	if (ReduceTurnover>0)
   	decompositionRate = decompositionRate/ReduceTurnover;
   ReduceTurnover = 0;
}
