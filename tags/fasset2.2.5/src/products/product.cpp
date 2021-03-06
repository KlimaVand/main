/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Revision		10/7 JB added corrected price calculations, check for
 							units, check for storages
\****************************************************************************/
//#include <common.h>
//#include <products.h>
#include "product.h"
#ifdef _STANDALONE
	#include "../base/message.h"
#endif

/****************************************************************************\
  Default Constructor
\****************************************************************************/
product::product() : base()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
product::product(const char * aName, const int aIndex, const base * aOwner)
			:base(aName,aIndex,aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
product::product(const product& aproduct)
			: base(aproduct)
{
   amount = aproduct.amount;
   mainUnitName = aproduct.mainUnitName;
   aPrice = aproduct.aPrice;
	storable=aproduct.storable;
   ObjType = aproduct.ObjType;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
product::~product()
{
}

/****************************************************************************\
\****************************************************************************/
void product::InitVariables()
{
   amount = 0.0;
   mainUnitName = -1;
   aPrice = NULL;
	storable=0;
   ObjType = productObj;
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const product& aproduct)
{
   os << setiosflags(ios::left) << setw(40) << "name " << aproduct.Getname() << endl; //added LKK
	os << setiosflags(ios::left) << setw(40) << "amount " << aproduct.amount << endl;
	os << setiosflags(ios::left) << setw(40) << "mainUnitName " << aproduct.mainUnitName << endl;
	os << setiosflags(ios::left) << setw(40) << "aPrice " << aproduct.aPrice << endl;
  	os << setiosflags(ios::left) << setw(40) << "ObjType " << aproduct.ObjType << endl;
	return os;
}

/****************************************************************************\
 Function purpose: returns name of product that can be transformed to this class
\****************************************************************************/
string product::DerivedFrom() const
{
 	return "NONE";
}

/****************************************************************************\
\****************************************************************************/
product& product::operator=(const product& aProduct)
{
	product * someProduct;
	someProduct=(product*)(&aProduct);
	char tmp[MAX_TXT];
	strcpy(tmp,someProduct->GetName());
   strcpy(Name,tmp);
	amount			= someProduct->GetAmount();
	mainUnitName	= someProduct->GetmainUnitName();
	storable			= someProduct->storable;
	aPrice			= someProduct->GetaPrice();
   ObjType        = someProduct->GetObjType();
   Index				= someProduct->GetIndex();
	return *this;
}

/****************************************************************************\
Needs cleaning up !!!
\****************************************************************************/
product& product::operator+(const product& aProduct)
{
	product* someProduct;
	someProduct = (product*)(&aProduct);
   double aAmount       = someProduct->GetAmount();
   int aMainUnitName	= someProduct->GetmainUnitName();
   if (amount==0)
   {
   	mainUnitName	= aMainUnitName;
      strcpy(Name,someProduct->Name);
		storable			= someProduct->storable;
		aPrice			= someProduct->GetaPrice();
   }
   else
   {
   	if (aAmount!=0)
      {
   		//if (aPrice!=someProduct->GetaPrice())
         //	aPrice = NULL;       //was: =NULL LKK
			// products with different prices should not be added? JB    JB
			// same price for same kinds of products LKK
   		if (mainUnitName!=aMainUnitName)
			{
		   	string WarningString;
		      WarningString = Name;
            WarningString+=" + "+someProduct->Getname()+" : ";
		      WarningString += GetUnitString(mainUnitName) + " + "+GetUnitString(someProduct->GetmainUnitName());
			  	theMessage->WarningWithDisplay("product::operator+ - products with different units added: ",WarningString.c_str());
		   }
      }
   }
	amount		+= aAmount;
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& product::operator-(const product& aProduct)
{
	product* someProduct;
	someProduct	= (product*)(&aProduct);
   double aAmount       = someProduct->GetAmount();
   if (amount==0)
   	aPrice = someProduct->GetaPrice();
   else
   	if ((aAmount!=0) && (aPrice!=someProduct->GetaPrice()))
   		aPrice = NULL;       // a composite item has no price??           JB
   if (mainUnitName!=someProduct->GetmainUnitName())
   	theMessage->WarningWithDisplay("product::operator- - products with different units subtracted");
	amount	   -= aAmount;
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& product::operator*(const double& aFactor)
{
	amount*=aFactor;
	return *this;
}

/****************************************************************************\
\****************************************************************************/
void product::showstats(ostream *os)
{
	*os << "productname  " << Name << endl;
	*os << "amount       " << amount << endl;
   *os << "main unit    " << GetUnitString(mainUnitName) << endl;
}


/****************************************************************************\
\****************************************************************************/
char * product::GetnameByChar()
{
	return Name;
}

/****************************************************************************\
\****************************************************************************/
product* product::clone() const
{
	product* aProduct;
	aProduct= new product;
	(*aProduct)=(*this);
	return aProduct;
}

/****************************************************************************\
\****************************************************************************/
int product::GetUnitType(string aMainUnitType)
{
	bool found = false;
	int typeRes=-1;
	if (aMainUnitType=="kwh")
     	{found=true;typeRes=kwh;}
	if (aMainUnitType=="tfi")
     	{found=true;typeRes=tfi;}
	if (aMainUnitType=="t")
     	{found=true;typeRes=t;}
	if (aMainUnitType=="kg")
    	{found=true;typeRes=kg;}
	if (aMainUnitType=="g")
     	{found=true;typeRes=g;}
	if (aMainUnitType=="m3")
     	{found=true;typeRes=m3;}
	if (aMainUnitType=="l")
     	{found=true;typeRes=l;}
	if (aMainUnitType=="ha")
     	{found=true;typeRes=ha;}
	if (aMainUnitType=="m2")
     	{found=true;typeRes=m2;}
	if (aMainUnitType=="m")
     	{found=true;typeRes=m;}
	if (aMainUnitType=="mm")
     	{found=true;typeRes=mm;}
	if (aMainUnitType=="sfu")
     	{found=true;typeRes=sfu;}
	if (aMainUnitType=="day")
     	{found=true;typeRes=day;}
	if (aMainUnitType=="hour")
    	{found=true;typeRes=hour;}
	if (aMainUnitType=="kr")
     	{found=true;typeRes=kr;}
	if (aMainUnitType=="fraction")
      {found=true;typeRes=fraction;}
	if (aMainUnitType=="pcs")
      {found=true;typeRes=pcs;}
	if(!found)
   {
   	theMessage->WarningWithDisplay("product::GetUnitType - the unit type <", (char*)aMainUnitType.c_str(), "> does not exist");
      return -1;
   }
	else
   	return typeRes;
}

/****************************************************************************\
\****************************************************************************/
string product::GetUnitString(int aMainUnit)
{
	if (aMainUnit==kwh)
      return "kwh";
	if (aMainUnit==tfi)
    	return "tfi";
	if (aMainUnit==t)
    	return "t";
	if (aMainUnit==kg)
    	return "kg";
	if (aMainUnit==g)
    	return "g";
	if (aMainUnit==m3)
    	return "m3";
	if (aMainUnit==l)
    	return "l";
	if (aMainUnit==ha)
    	return "ha";
	if (aMainUnit==m2)
    	return "m2";
	if (aMainUnit==m)
    	return "m";
	if (aMainUnit==mm)
    	return "mm";
	if (aMainUnit==sfu)
    	return "sfu";
	if (aMainUnit==day)
   	return "day";
   if (aMainUnit==hour)
    	return "hour";
	if (aMainUnit==kr)
    	return "kr";
	if (aMainUnit==fraction)
    	return "fraction";
	if (aMainUnit==pcs)
    	return "pcs";
   return "illegal unit";
}

/****************************************************************************\
\****************************************************************************/
void product::ReadParameters(fstream * file)
{
   Setfile(file);
   SetCritical();
   FindSection(Name,Index); // Top of hierarcy
	string aUnit;
   GetParameter("productname",Name);
   GetParameter("mainunitname",&aUnit);
   UnsetCritical();
   if (!GetParameter("amount",&amount))
      amount = 0.0;
   if (!GetParameter("storable",&storable))
      storable = 0;
	mainUnitName = GetUnitType(aUnit);
   Setfile(NULL);
}

