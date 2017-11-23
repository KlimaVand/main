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
product* premium::clone() const
{
	premium* aProduct= new premium(*this);
	return (product *) aProduct;
}

/****************************************************************************\
\****************************************************************************/
void premium::ReadParameters(fstream * file, bool criticals,const char *  sectionName)
{
	product::ReadParameters(file,criticals,sectionName);
   critical=criticals;
   Setfile(file);
   string isGreenString;
   if(FindSection(sectionName,Index)==true)
   	{
	GetParameter("plantName",&plantName);
   GetParameter("isgreen",&isGreenString);
   green = (isGreenString=="yes");
   	}
   Setfile(NULL);
}

