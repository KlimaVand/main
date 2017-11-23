/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "plivemat.h"
#ifdef _STANDALONE
	#include <message.h>
#endif

/****************************************************************************\
  Default Constructor
\****************************************************************************/
liveMatter::liveMatter()
   : product()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
liveMatter::liveMatter(const char * aName, const int aIndex, const base * aOwner)
   : product(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
liveMatter::liveMatter(const liveMatter& d)
   : product(d)
{
   orgN_content 	 = d.orgN_content;
   NH4_content 	 = d.NH4_content;
   NO3_content 	 = d.NO3_content;
   P_content 		 = d.P_content;
   K_content 		 = d.K_content;
   dryMatter 		 = d.dryMatter;
   organic 			 = d.organic;
   ObjType 			 = d.ObjType;
   C_content 		 = d.C_content;
   code				 = d.code;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
liveMatter::~liveMatter()
{
}

/****************************************************************************\
\****************************************************************************/
void liveMatter::InitVariables()
{
   orgN_content.Clear();
   NH4_content.Clear();
   NO3_content.Clear();
   P_content 		  = 0.0;
   K_content 		  = 0.0;
   dryMatter 		  = 0.0;
   organic 			  = 0.0;
   C_content 		  = 0.0;
   code				  = 0;
   ObjType		     = -1; // This inheritance level should never be instantiated!
}

/****************************************************************************\
\****************************************************************************/
void liveMatter::RemoveWater()
{
   if (dryMatter>0)
   {
      orgN_content = orgN_content/dryMatter;
      NH4_content  = NH4_content/dryMatter;
      NO3_content  = NO3_content/dryMatter;
      P_content 	 = P_content/dryMatter;
      K_content 	 = K_content/dryMatter;
      C_content 	 = C_content/dryMatter;
      amount       = amount*dryMatter;
      dryMatter 	  = 1.0;
   }
}

/****************************************************************************\
  GetAllN returns the total content of N
\****************************************************************************/
nitrogen liveMatter::GetAllN()
{
 	return orgN_content + NH4_content + NO3_content;
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const liveMatter& d)
{
	os << (product &) d;
   os << "orgN_content " << d.orgN_content.n << endl;
	os << "NH4_content " << d.NH4_content.n << endl;
	os << "NO3_content " << d.NO3_content.n << endl;
	os << "P_content " << d.P_content << endl;
	os << "K_content " << d.K_content << endl;
	os << "dryMatter " << d.dryMatter << endl;
   os << setiosflags(ios::left) << setw(40) << "organic " << d.organic << endl;
   os << "C_content " << d.C_content << endl;
   os << "code " << d.code << endl;
   return os;
}

/****************************************************************************\
   Assignment operator
\****************************************************************************/
product& liveMatter::operator=(const product& someliveMatter)
{
   product::operator=(someliveMatter);
   liveMatter* aliveMatter = (liveMatter*)(&someliveMatter);
	NH4_content      = aliveMatter->NH4_content;
	orgN_content 	  = aliveMatter->orgN_content;
	NO3_content  	  = aliveMatter->NO3_content;
   P_content    	  = aliveMatter->P_content;
	K_content    	  = aliveMatter->K_content;
	dryMatter    	  = aliveMatter->dryMatter;
	organic          = aliveMatter->Getorganic();
   C_content 	 	  = aliveMatter->C_content;
   code				  = aliveMatter->code;
   return *this;
}

/****************************************************************************\
\****************************************************************************/
product& liveMatter::operator+(const product& someliveMatter)
{
   liveMatter* aliveMatter;
	aliveMatter=(liveMatter*)(&someliveMatter);
   double aAmount           = aliveMatter->GetAmount();
	nitrogen aNH4_content    = aliveMatter->GetNH4_content();
	nitrogen aorgN_content   = aliveMatter->GetorgN_content();
	nitrogen aNO3_content    = aliveMatter->GetNO3_content();
	double aP_content        = aliveMatter->GetP_content();
	double aK_content        = aliveMatter->GetK_content();
	double adryMatter        = aliveMatter->GetdryMatter();
	double aorganic          = aliveMatter->Getorganic();
	double aC_content        = aliveMatter->GetC_content();
	if(aAmount!=0)
  	{
   	orgN_content     = (orgN_content*amount + aorgN_content*aAmount)/(amount+aAmount);
		NH4_content   	  = (NH4_content*amount + aNH4_content*aAmount)/(amount+aAmount);
      NO3_content   	  = (NO3_content*amount + aNO3_content*aAmount)/(amount+aAmount);
		P_content   	  = (P_content*amount + aP_content*aAmount)/(amount+aAmount);
		K_content   	  = (K_content*amount + aK_content*aAmount)/(amount+aAmount);
		dryMatter   	  = (dryMatter*amount + adryMatter*aAmount)/(amount+aAmount);
      organic   		  = (organic*amount    + aorganic*aAmount)/(amount+aAmount);
		C_content   	  = (C_content*amount + aC_content*aAmount)/(amount+aAmount);
	}
   product::operator+(someliveMatter);
	return *this;
   //note - 'code' is not copied
}

/****************************************************************************\
\****************************************************************************/
product& liveMatter::operator-(const product& someliveMatter)
{
	liveMatter* aliveMatter;
	aliveMatter=(liveMatter*)(&someliveMatter);
   if(NH4_content  	!= aliveMatter->NH4_content||
		orgN_content 	!= aliveMatter->orgN_content||
		NO3_content  	!= aliveMatter->NO3_content||
		P_content    	!= aliveMatter->P_content||
		K_content    	!= aliveMatter->K_content||
		dryMatter    	!= aliveMatter->dryMatter||
		organic        != aliveMatter->Getorganic()||
	   C_content 	 	!= aliveMatter->C_content||
	   code				!= aliveMatter->code)
         theMessage->Warning("liveMatter::operator- - liveMatter's with different compositions subtracted");
   product::operator-(someliveMatter);
	return *this;
}


/****************************************************************************\
\****************************************************************************/
product* liveMatter::clone() const
{
	liveMatter* aProduct= new liveMatter(*this);
	return (product*) aProduct;
}

/****************************************************************************\
\****************************************************************************/
void liveMatter::ReadParameters(fstream * file)
{
	product::ReadParameters(file);
   SetCritical();
   Setfile(file);
   GetParameter("orgN_content" ,&orgN_content.n);
	GetParameter("NH4_content"  ,&NH4_content.n);
	GetParameter("NO3_content"  ,&NO3_content.n);
	GetParameter("P_content"    ,&P_content);
	GetParameter("K_content"    ,&K_content);
	GetParameter("dryMatter"    ,&dryMatter);

   UnsetCritical();
   if (!GetParameter("C_content"    ,&C_content))
      C_content = 0.46 * dryMatter;
	if (!GetParameter("organic"    ,&organic))
   	organic = 0.06 * dryMatter;
	GetParameter("code"    ,&code);
   SetCritical();
   Setfile(NULL);
}

double liveMatter::SubtractWater(double waterToSubtract)
{
 double water = GetAmount()*(1-GetdryMatter());
 double DM = GetAmount()*GetdryMatter();
 if ((water-waterToSubtract)<0)
     waterToSubtract=water;
 water-=waterToSubtract;
   orgN_content = GetorgN_content() * GetAmount()/(water+DM);
   NH4_content 	= GetNH4_content() * GetAmount()/(water+DM);
   NO3_content 	= GetNO3_content() * GetAmount()/(water+DM);
   P_content 		= GetP_content() * GetAmount()/(water+DM);
   K_content 	= GetK_content() * GetAmount()/(water+DM);
   dryMatter 	= GetdryMatter() * GetAmount()/(water+DM);
   C_content 	= GetC_content() * GetAmount()/(water+DM);
	Setamount(water+DM);
   return waterToSubtract;
}


