
#include "../base/common.h"
#include "organicProduct.h"
#include "../soil/organicMatter.h"
#include "../base/message.h"
/****************************************************************************\
Constructor without arguments
\****************************************************************************/
organicProduct::organicProduct()
   : base("",0,NULL)
{
   InitVariables();
}

/****************************************************************************\
Constructor with arguments
Pname - Name of instance
Index - Instance number
owner - Pointer to owner object
\****************************************************************************/
organicProduct::organicProduct(const char * Pname,const int Index,const base * owner)
   : base(Pname,Index,owner)
{
   InitVariables();
}


/****************************************************************************\
Copy constructor
\****************************************************************************/
organicProduct::organicProduct(const organicProduct& product)
 : base(product)
{
   if (&product)
   {
      InitVariables();
      stepFactor=product.stepFactor;
      carbonFraction=product.carbonFraction;
      numOfAllocations=product.numOfAllocations;
      productName=product.productName;
      for (int i=0;i<product.numOfAllocations;i++)
      {
         allocationList[i].poolName=product.allocationList[i].poolName;
         allocationList[i].cFraction=product.allocationList[i].cFraction;
         allocationList[i].nFraction=product.allocationList[i].nFraction;
         allocationList[i].decayRate=product.allocationList[i].decayRate;
      }
      NO3    = product.NO3;
      NH4    = product.NH4;
      amount = product.amount;
      ReduceTurnover = product.ReduceTurnover;
      next=NULL; // ???!!!
   }
}



/****************************************************************************\
\****************************************************************************/
void organicProduct::InitVariables()
{
   amount.Clear();
   carbonFraction=1.0; // If incomming matter is measured in e.g. DM
   numOfAllocations=0;
   productName="";
   stepFactor=1.0;
   ReduceTurnover=0.0;
   NO3.Clear();
   NH4.Clear();
   for (int i=0;i<maxAllocations;i++)
   {
      allocationList[i].poolName="";
      allocationList[i].cFraction=0.0;
      allocationList[i].nFraction=0.0;
      allocationList[i].decayRate=-1.0;
   }
   allocationList[0].poolName="AOM1";
   allocationList[1].poolName="AOM2";
   allocationList[2].poolName="NOM";
   numOfAllocations=3;
   next=NULL;
}

/****************************************************************************\
\****************************************************************************/
double organicProduct::GetCNratio()
{
   double retVal=0.0;
   if (amount.n>0.0)
      retVal=amount.c/amount.n;
   return retVal;
}

/**
 * Adding carbonFraction, N03, NH4, amount from input input product
 */
bool organicProduct::AddNext(organicProduct * product)
{
   if (ProductSimilarity(product))
   {
      double totalAmount = amount.c + product->amount.c;
      double amount1 = product->amount.c;
      if (totalAmount>1e-10)
      {
         carbonFraction = (amount.c*carbonFraction+amount1*product->carbonFraction)/totalAmount;
         NO3  = (NO3*amount.c+product->NO3*amount1)/totalAmount;
         NH4 = (NH4*amount.c+product->NH4*amount1)/totalAmount;
         amount=amount+product->amount;
 	   }
      delete product;
      return false;
   }
   else
   {
	   next = product;
      return true;
   }
}

/**
 * See if 2 products have the same amount if C and N, ReduceTurnover and if AOM1 and AOM2 has the same decay and if Nom has the same cFraction
 */
bool organicProduct::ProductSimilarity(const organicProduct * product)
{
   double deviation=0.0;
   if (amount.c>1E-8 && product->amount.c>1E-8 && product->amount.n>1E-8)
      deviation=1.0-(amount.n*product->amount.c)/(amount.c*product->amount.n);
   bool resemblance = (allocationList[0].decayRate==product->allocationList[0].decayRate
                       && allocationList[1].decayRate==product->allocationList[1].decayRate
                       && allocationList[2].cFraction==product->allocationList[2].cFraction
                       && ReduceTurnover==product->ReduceTurnover
                       && fabs(deviation)<0.05);
   return resemblance;
}

/**
 * Update C and N fraction for AOM1 and C and N fraction for NOM
 */
void organicProduct::SetAOM1_fraction(double f)
{
   allocationList[0].cFraction=f;
   allocationList[2].cFraction=max(0.0,1.0-f-allocationList[1].cFraction);
   allocationList[0].nFraction=allocationList[0].cFraction;
   allocationList[2].nFraction=allocationList[2].cFraction;
   double check=0.0;
   for (int i=0;i<3;i++)
      check+=allocationList[i].cFraction;
   if (fabs(check-1.0)>1E-10)
      Terminate("organicProduct::SetAOM1_fraction - fractions do not add to one");
}

/**
 * Update C and N fraction for AOM1 and C and N fraction for NOM
 */
void organicProduct::SetAOM2_fraction(double f)
{
   allocationList[1].cFraction=f;
   allocationList[2].cFraction=max(0.0,1.0-f-allocationList[0].cFraction);
   allocationList[1].nFraction=allocationList[1].cFraction;
   allocationList[2].nFraction=allocationList[2].cFraction;
   double check=0.0;
   for (int i=0;i<3;i++)
      check+=allocationList[i].cFraction;
   if (fabs(check-1.0)>1E-10)
      Terminate("organicProduct::SetAOM2_fraction - fractions do not add to one");
}

/**
 * Get a copy of this product that is scaled with frac
 */
organicProduct * organicProduct::GetFraction(double frac)
{
   organicProduct * p = new organicProduct(*this);
   p->Scale(frac);
   return p;
}


/**
 * Getting organic N and N15
 */
nitrogen organicProduct::GetOrganicNitrogen()
{
   nitrogen OrganicN;
   OrganicN.SetBoth(amount.n,amount.n15);

   if(amount.n<0.0||amount.n15<0.0)
	   theMessage->FatalError("organicProduct::GetOrganicNitrogen - negative amount of organic N");
	return OrganicN;
}
/**
 * Getting total amount of Nitrogen (organic, NO3 and NH4)
 */
nitrogen organicProduct::GetTotalNitrogen()
{
	return GetOrganicNitrogen()+NO3+NH4;
}
/**
 * Scales N03, NH4,C and N
 */
void organicProduct::Scale(double f)
{
   NO3=NO3*f;
   NH4=NH4*f;
   amount=amount*f;
}

