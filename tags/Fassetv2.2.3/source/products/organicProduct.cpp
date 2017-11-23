
#include "../base/common.h"
#include "organicProduct.h"
#include "../soil/organicMatter.h"

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
         allocationList[i].fraction=product.allocationList[i].fraction;
         allocationList[i].nfraction=product.allocationList[i].nfraction;
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
"allocationList" and "productName" assumed equal, but this is not tested
\****************************************************************************/
void organicProduct::Add(organicProduct * product, double fraction)
{
   stepFactor=(1.0-fraction)*stepFactor+fraction*product->stepFactor;
   carbonFraction=(1.0-fraction)*carbonFraction+fraction*product->carbonFraction;
   NO3 = NO3*(1.0-fraction)+product->NO3*fraction;
   NH4 = NH4*(1.0-fraction)+product->NH4*fraction;
   amount = amount*(1.0-fraction)+product->amount*fraction;
   ReduceTurnover = ReduceTurnover*(1.0-fraction)+product->ReduceTurnover*fraction;
   next=NULL; // ???!!!
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
      allocationList[i].fraction=0.0;
      allocationList[i].nfraction=0.0;
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

/****************************************************************************\
\****************************************************************************/
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

/****************************************************************************\
\****************************************************************************/
bool organicProduct::ProductSimilarity(const organicProduct * product)
{
   double deviation=0.0;
   if (amount.c>1E-8 && product->amount.c>1E-8 && product->amount.n>1E-8)
      deviation=1.0-(amount.n*product->amount.c)/(amount.c*product->amount.n);
   bool resemblance = (allocationList[0].decayRate==product->allocationList[0].decayRate
                       && allocationList[1].decayRate==product->allocationList[1].decayRate
                       && allocationList[2].fraction==product->allocationList[2].fraction
                       && ReduceTurnover==product->ReduceTurnover
                       && fabs(deviation)<0.05);
   return resemblance;
}

/****************************************************************************\
\****************************************************************************/
void organicProduct::SetAOM1_fraction(double f)
{
   allocationList[0].fraction=f;
   allocationList[2].fraction=max(0.0,1.0-f-allocationList[1].fraction);
   allocationList[0].nfraction=allocationList[0].fraction;
   allocationList[2].nfraction=allocationList[2].fraction;
   double check=0.0;
   for (int i=0;i<3;i++)
      check+=allocationList[i].fraction;
   if (fabs(check-1.0)>1E-10)
      Terminate("organicProduct::SetAOM1_fraction - fractions do not add to one");
}

/****************************************************************************\
\****************************************************************************/
void organicProduct::SetAOM2_fraction(double f)
{
   allocationList[1].fraction=f;
   allocationList[2].fraction=max(0.0,1.0-f-allocationList[0].fraction);
   allocationList[1].nfraction=allocationList[1].fraction;
   allocationList[2].nfraction=allocationList[2].fraction;
   double check=0.0;
   for (int i=0;i<3;i++)
      check+=allocationList[i].fraction;
   if (fabs(check-1.0)>1E-10)
      Terminate("organicProduct::SetAOM2_fraction - fractions do not add to one");
}

/****************************************************************************\
\****************************************************************************/
organicProduct * organicProduct::GetFraction(double frac)
{
   organicProduct * p = new organicProduct(*this);
   p->Scale(frac);
   return p;
}

/****************************************************************************\
Reads parameters and initial state from file
Presently not used in FASSET
\****************************************************************************/
void organicProduct::ReadParameters(fstream * file, bool ncfrac)
{
   Terminate("organicProduct::ReadParameters - should not be called from FASSET");
   SetCritical();
   Setfile(file);
   if (FindSection(Name,Index))
   {
      GetParameter("Name",&productName);
      UnsetCritical();
      GetParameter("CarbonFraction",&carbonFraction); // If incomming matter is measured in e.g. DM
      SetCritical();
      // Read allocations
      int first,n;
      GetVectorInfo("Pool",&first,&n);
      if (first>0)
         Terminate("Vector parameter numbering for addded types must start with zero");
      double check=0.0;
      double ncheck=0.0;
      for(int i=first;i<(first+n);i++)
      {
         allocationList[i].fraction=0.0;
         allocationList[i].decayRate=-1.0;
         GetParameter("Pool",&allocationList[i].poolName,i);
         if (ncfrac)
            UnsetCritical();
         if (!GetParameter("Fraction",&allocationList[i].fraction,i))
            allocationList[i].fraction=0.0;
         allocationList[i].nfraction=allocationList[i].fraction;
         UnsetCritical();
         GetParameter("NFraction",&allocationList[i].nfraction,i);
         if (GetParameter("DecayRate",&allocationList[i].decayRate,i)) // Rate optional. If not set, takes rate from model definition file.
            allocationList[i].decayRate=allocationList[i].decayRate*stepFactor;
         check+=allocationList[i].fraction;
         ncheck+=allocationList[i].nfraction;
         numOfAllocations++;
      }
      if (fabs(check-1.0)>1E-5 && !ncfrac)
      {
         cout << "Incorrect sum of C fractions in " << Name << "(" << Index << ")" << endl;
         Terminate();
      }
      if (fabs(ncheck-1.0)>1E-5 && !ncfrac)
      {
         cout << "Incorrect sum of N fractions in " << Name << "(" << Index << ")" << endl;
         Terminate();
      }
   }
   else
   {
      cout << "Section " << Name << "(" << Index << ") not found" << endl;
      Terminate();
   }
   Setfile(NULL);
}
/****************************************************************************\
\****************************************************************************/
nitrogen organicProduct::GetOrganicNitrogen()
{
   nitrogen OrganicN;
   OrganicN.SetBoth(amount.n,amount.n15);
	return OrganicN;
}
/****************************************************************************\
\****************************************************************************/
nitrogen organicProduct::GetTotalNitrogen()
{
	return GetOrganicNitrogen()+NO3+NH4;
}
/****************************************************************************\
\****************************************************************************/
void organicProduct::Scale(double f)
{
   NO3=NO3*f;
   NH4=NH4*f;
   amount=amount*f;
}

/****************************************************************************\
\****************************************************************************/
void organicProduct::SetParameterByName(string parmName,double val)
{
   if (val<0.0)
      val=0.0;
   bool found=false;
   if (parmName=="CarbonFraction")
   {
      if (val>1.0)
         val=1.0;
      carbonFraction=val;
      found=true;
   }
   {
   if (parmName=="Fraction(0)") // Hack, only valid for 2 connections !!!
      {
         if (val>1.0)
            val=1.0;
         if (numOfAllocations!=2)
            Terminate("organicProduct::SetParameter - can only set fraction when there are two allocations");
         double f1=min(1.0,max(val,0.0));
         double f2=1.0-f1;
         allocationList[0].fraction=f1;
         allocationList[1].fraction=f2;
         found=true;
      }
   if (parmName=="Fraction(1)") // Hack, only valid for 2 connections !!!
      {
         if (val>1.0)
            val=1.0;
         if (numOfAllocations!=2)
            Terminate("organicProduct::SetParameter - can only set fraction when there are two allocations");
         double f2=min(1.0,max(val,0.0));
         double f1=1.0-f2;
         allocationList[0].fraction=f1;
         allocationList[1].fraction=f2;
         found=true;
      }
   }
   if (parmName=="DecayRate(0)")
   {
      allocationList[0].decayRate=val*stepFactor;
      found=true;
   }
   if (parmName=="DecayRate(1)")
   {
      allocationList[1].decayRate=val*stepFactor;
      found=true;
   }
   if (parmName=="DecayRate(2)")
   {
      allocationList[2].decayRate=val*stepFactor;
      found=true;
   }
   if (parmName=="DecayRate(3)")
   {
      allocationList[3].decayRate=val*stepFactor;
      found=true;
   }
   if (!found)
      Terminate("organicProduct::SetParameter - no match found for ",(char*)parmName.c_str());
}

