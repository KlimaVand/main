/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "premium.h"
#ifdef _STANDALONE
	#include "../base/message.h"
#endif

/****************************************************************************\
  Constructor
\****************************************************************************/
premium::premium()
   : product()
{
   InitVariables();
}
/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
premium::premium(const char * aName, const int aIndex, const base * aOwner)
   : product(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Copy Constructor
\****************************************************************************/
premium::premium(const premium& apremium)
   : product(apremium)
{
   InitVariables();
}

/****************************************************************************\
  Destructor
\****************************************************************************/
premium::~premium()
{
}

/****************************************************************************\
\****************************************************************************/
void premium::InitVariables()
{
   plantName = "";
   green     = false;
   ObjType	 = premiumObj;
}

/****************************************************************************\
  Operator << for output
\****************************************************************************/
ostream& operator<< (ostream& os, const premium& apremium)
{
  	os << (product &) apremium;
  	os << setiosflags(ios::left) << setw(40) << "plantName " << apremium.plantName << endl;
   return os;
}

/****************************************************************************\
\****************************************************************************/
product& premium::operator=(const product& somepremium)
{
	premium* apremium;
	apremium=(premium*)(&somepremium);
	product::operator=(somepremium);
	plantName = apremium->GetPlantName();
   green     = apremium->GetGreen();
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& premium::operator+(const product& somepremium)
{
//	premium* apremium=(premium*)(&somepremium);
	product::operator+(somepremium);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
product& premium::operator-(const product& somepremium)
{
	premium* apremium;
	apremium=(premium*)(&somepremium);
   string aplantName	= apremium->GetPlantName();
   if (plantName != aplantName)
   	theMessage->Warning("premium::operator- - premiums with different compositions subtracted");
	product::operator-(somepremium);
	return *this;
}

/****************************************************************************\
\****************************************************************************/
void premium::showstats(ostream *os)
{
	product::showstats(os);
	*os << "plantName         " << plantName << endl;
}

/****************************************************************************\
\****************************************************************************/
product* premium::clone() const
{
	premium* aProduct= new premium(*this);
	return (product *) aProduct;
}

/****************************************************************************\
\****************************************************************************/
void premium::ReadParameters(commonData * file,const char * sectionName)
{
	product::ReadParameters(file,sectionName);

   string isGreenString;
	file->FindItem("plantName",&plantName);
   file->FindItem("isgreen",&isGreenString);
   green = (isGreenString=="yes");

}

