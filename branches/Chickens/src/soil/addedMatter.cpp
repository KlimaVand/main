/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "addedMatter.h"

/****************************************************************************\
Constructor with arguments
Pname - Name of instance
Index - Instance number
owner - Pointer to owner object
\****************************************************************************/
addedMatter::addedMatter(const char * Pname,const int Index,const base * owner)
   : matter(Pname,Index,owner)
{
   numOfSubPools=0;
}

/****************************************************************************\
Copy Constructor
\****************************************************************************/
addedMatter::addedMatter(const addedMatter& source)
   : matter(source)
{
	   if (!source.IsAOM())
	      if (cnRatio<=0)
	         Terminate("addedMatter::addedMatter - illegal state of cnRatio variable");
   numOfSubPools=source.numOfSubPools;
	for(int i=0;i<numOfSubPools;i++)
      addPoolList[i]=new addedPool(*source.addPoolList[i]);
}

/****************************************************************************\
Returns a copy of this instance at the same inheritance level.
\****************************************************************************/
addedMatter* addedMatter::clone() const
{
	addedMatter* a = new addedMatter(*this);
	return a;
}

/****************************************************************************\
\****************************************************************************/
void addedMatter::Add(matter * source,double fraction)
{
   addedMatter * source1=(addedMatter*)source;
	for(int i=0;i<numOfSubPools;i++)
      addPoolList[i]->Scale(1.0-fraction);
	for(int i=numOfSubPools;i<(numOfSubPools+source1->numOfSubPools);i++)
   {
      source1->addPoolList[i-numOfSubPools]->Scale(fraction);
      addPoolList[i]=(addedPool*)source1->addPoolList[i-numOfSubPools]->clone();
   }
   numOfSubPools+=source1->numOfSubPools;
   if (numOfSubPools>maxSubPools)
      Terminate("addedMatter::Add - to many pools");
	for(int i=0;i<(numOfSubPools-1);i++)
      for (int k=i+1;k<numOfSubPools;k++)
      {
         if (addPoolList[i]->Alike(addPoolList[k]))
         {
            cnMatter cn = addPoolList[k]->GetCarbon();
            if (cn.c>0)
               addPoolList[i]->SoakDry(addPoolList[k]);
         }
      }
   // Remove empty pools
   int lo=0;
   int hi=numOfSubPools-1;
   while (hi>lo)
   {
      // Find lowest empty pool
      if (lo<numOfSubPools)
         while (lo<numOfSubPools && addPoolList[lo]->GetCarbon().c>0)
         {
            lo++;
            if (lo>=numOfSubPools)
               break;
         }
      // Find highest non-empty pool
      if (hi>=0)
         while (hi>=0 && addPoolList[hi]->GetCarbon().c<1E-36)
         {
            hi--;
            if (hi<0)
               break;
         }
      if (hi>lo)
      {  // Swap pools
         addedPool * p=addPoolList[hi];
         addPoolList[hi]=addPoolList[lo];
         addPoolList[lo]=p;
      }
   }
   // Delete empty pools if present
   if (lo<numOfSubPools)
      for (int i=lo;i<numOfSubPools;i++)
         delete addPoolList[i];
   numOfSubPools=lo;
}

/****************************************************************************\
Destructor
\****************************************************************************/
addedMatter::~addedMatter()
{
	for(int i=0;i<numOfSubPools;i++)
      delete addPoolList[i];
}

/****************************************************************************\
Initializes the organic pool object
Must be called AFTER all parameters are read by all organic matter objects
\****************************************************************************/
void addedMatter::Initialize(double clayEff1,double clayEff2,int clayResp)
{
   if (cnRatio>0)
      Terminate("The added matter pool '",(char*)poolName.c_str(),"' can not have a fixed C/N ratio");
   matter::Initialize(clayEff1,clayEff2,clayResp);
   if (carbon.c>0 && 0==numOfSubPools)
   {
      addedPool * aP = new addedPool(poolName.c_str(),numOfSubPools,Owner);
      aP->SetDecompositionRate(decompositionRate);
      aP->SetCarbon(carbon);
      aP->SetClayEffect1(clayEff1);
      aP->SetClayEffect2(clayEff2);
      aP->SetClayResponseType(clayResp);
      aP->SetUseClayEffect(useClayEffect);
      aP->SetExponentialDecay(expDecay);
      addPoolList[0]=aP;
      numOfSubPools=1;
   }
}

/****************************************************************************\
Calculates the decomposition of the organic matter in the pool
Does ONLY transfer the carbon to the receiver pools, if the subpool contributes
to a net mineralisation, NOT if it contributes to a net immobilisation.
environmentEffect - covers combined effects of water content and temperature.
\****************************************************************************/
void addedMatter::Update(double environmentEffect,int mode)
{
   cnMatter outflux;
   cnMatter out;
   if (numOfSubPools>0)
   {
      for(int i=0;i<numOfSubPools;i++)
      {
         out.SetAll(0.0,0.0,0.0,0.0);
         addPoolList[i]->Decay(environmentEffect,out,mode,CNcrit());
         outflux=outflux+out;
      }
      if (outflux.c>0.0)
         Export(outflux);
      carbon=carbon-outflux; // In order to do the double book-keeping
   }
}

/****************************************************************************\
\****************************************************************************/
double addedMatter::PotentialCarbonTurnover()
{
   double retVal=0.0;
   if (numOfSubPools>0)
      for(int i=0;i<numOfSubPools;i++)
         retVal+=addPoolList[i]->PotentialCarbonTurnover();
   return retVal;
}


/****************************************************************************\
\****************************************************************************/
double addedMatter::PotentialNitrogenMineralisation()
{
   double retVal=0.0;
   if (numOfSubPools>0)
      for(int i=0;i<numOfSubPools;i++)
         retVal+=addPoolList[i]->PotentialNitrogenMineralisation();
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double addedMatter::PotentialNitrogenImmobilisation()
{
   double retVal=0.0;
   if (numOfSubPools>0)
      for(int i=0;i<numOfSubPools;i++)
         retVal+=addPoolList[i]->PotentialNitrogenImmobilisation();
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
double addedMatter::ResidualCarbon()
{
   double retVal=0.0;
   if (numOfSubPools>0)
      for(int i=0;i<numOfSubPools;i++)
         retVal+=addPoolList[i]->ResidualCarbon();
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
void addedMatter::FinalExport(double ratio)
{
   cnMatter outflux;
   if (numOfSubPools>0)
   {
      for(int i=0;i<numOfSubPools;i++)
         addPoolList[i]->FinalExportAOM(ratio,outflux);
      Export(outflux);
   }
}

/****************************************************************************\
Adds all import to the pool, and returns the current carbon content
content - C content in pool
\****************************************************************************/
void addedMatter::CatchInfluxAOM(cnMatter &content,int mode, bool hungry)
{
   if (!hungry)
      carbon.SetAll(0.0,0.0,0.0,0.0); // !!!????????????????
   cnMatter c;
   if (numOfSubPools>0)
   {
      for(int i=0;i<numOfSubPools;i++)
      {
         addPoolList[i]->CatchInflux(c,mode);
         carbon=carbon+c;
      }
      content=carbon;
   }
}

/****************************************************************************\
Investigates whether one of the fractions of the added matter fits with the
current pool. If this is the case, the matter is added to a subpool.
prod - pointer to organic product
\****************************************************************************/
void addedMatter::TestAddProduct(organicProduct * prod,double * check,double * CN,int * num,bool forAOM)
{
   *CN=-1.0;
   *num=0;
   if (forAOM)
   {
      for(int k=0;k<prod->GetNumOfFractions();k++)
         if (poolName==prod->GetDestPool(k))
         {
            *num=k;
            double decompr;
            decompr=prod->GetDecompositionRate(k);
            if (decompr<0) // Decompositionrate not defined in product
               decompr=decompositionRate; // Set to pre-defined rate
            bool found=false;
            for(int i=0;i<numOfSubPools;i++)
            {
               double addDecomp = addPoolList[i]->GetDecompositionRate();
               if ((prod->GetReduceTurnover()==0 && addDecomp==decompr) ||
                   (prod->GetReduceTurnover()>0 && prod->GetReduceTurnover()==addPoolList[i]->GetReduceTurnover() && fabs(addDecomp-decompr*prod->GetReduceTurnover())<1e-15))
               {
                  found=true;
                  addPoolList[i]->AddOrganicProduct(prod,k,check);
                  break;
               }
            }
            if (!found)
            {
               addedPool * aP = new addedPool(poolName.c_str(),numOfSubPools,Owner);
               aP->SetDecompositionRate(decompr);
               aP->AddOrganicProduct(prod,k,check);
               aP->SetClayEffect1(clayEffect1);
               aP->SetClayEffect2(clayEffect2);
               aP->SetClayResponseType(clayResponseType);
               aP->SetUseClayEffect(useClayEffect);
               aP->SetExponentialDecay(expDecay);
               addPoolList[numOfSubPools]=aP;
               aP->SetReduceTurnover(prod->GetReduceTurnover());
               numOfSubPools++;
               if (numOfSubPools>maxSubPools)
                  Terminate("addedMatter::TestAddProduct - to many pools");

            }
         }
   }
}

/****************************************************************************\
\****************************************************************************/
cnMatter addedMatter::GetCarbon()
{
   cnMatter retVal;
   if (numOfSubPools>0)
      for(int i=0;i<numOfSubPools;i++)
         retVal=retVal+addPoolList[i]->GetCarbon();
   return retVal;
}



/****************************************************************************\
Streams detailed information about the current pool and its subpools
\****************************************************************************/
void addedMatter::ReleaseReduceTurnover()
{
	for(int i=0;i<numOfSubPools;i++)
   	addPoolList[i]->ReleaseReduceTurnover();
}

/****************************************************************************\
\****************************************************************************/
void addedMatter::CreateAndTransfer(int PoolNo,double Fraction,double TurnoverFactor)
{
   //cout << "addedMatter::CreateAndTransfer, treating pool no " << PoolNo << endl;
   // WriteSubPoolsToScreen();

   cnMatter start=GetCarbon();

   // i)   Make a clone
   addPoolList[numOfSubPools]=(addedPool*)addPoolList[PoolNo]->clone();

   // ii)  Scale to the right amount of SOM in 'mother' and 'daughter'
   addPoolList[PoolNo]->Scale(1.0-Fraction);
   addPoolList[numOfSubPools]->Scale(Fraction);

   // iii) Multiply turnover of 'daughter'
   double k=addPoolList[PoolNo]->GetDecompositionRate();
   addPoolList[numOfSubPools]->SetDecompositionRate(k*TurnoverFactor);

   // iv)  Mark 'daughter' as 'derived'
      addPoolList[numOfSubPools]->SetDerived(true);

   // v)   Update number of pools
   numOfSubPools++;
   cnMatter end=GetCarbon();

   //WriteSubPoolsToScreen();
   
   if (fabs(start.c - end.c)>0.01)
      cout << "addedMatter::CreateAndTransfer - ERROR" << endl;
}

/****************************************************************************\
\****************************************************************************/
bool addedMatter::CloneAndModify(double Fraction,double TurnoverFactor)
{
   int N = numOfSubPools;
	for(int i=0;i<N;i++)
      if (!addPoolList[i]->IsDerived())
      {
         //cout << "addedMatter::CloneAndModify - subpool no " << i << endl;
   	   CreateAndTransfer(i,Fraction,TurnoverFactor);
      }
   return true;
}

