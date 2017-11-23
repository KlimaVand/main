/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  Archive
	Author
	  Date
 Revision
 ----------------------------------------------------------------------------
		Log
\****************************************************************************/
#include "../base/common.h"
#include "fieldAttribute.h"

/****************************************************************************\
  Default Constructor
\****************************************************************************/

fieldAttrib::fieldAttrib()
	: base()
{
	isgreen=false;
	contributionThisYear = 0.0;
	contributionIndex = 0;
	contributionList = '\0';
	boundary = 0;
	fieldName[0] = '\0';


}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/

fieldAttrib::fieldAttrib(char * aName, int aIndex, base * aOwner)
	: base(aName, aIndex, aOwner)
{
	isgreen=false;
	contributionThisYear = 0.0;
	contributionIndex = 0;
	contributionList = '\0';
	boundary = 0;
	fieldName[0]= '\0';


}

/****************************************************************************\
  Destructor
\****************************************************************************/

fieldAttrib::~fieldAttrib()
{
delete [] contributionList;
}


/****************************************************************************\
 Function:   Update
 Purpose:
 Parameters: void
 Comments:
\****************************************************************************/

bool fieldAttrib::Update()
{
	if ( (contributionIndex+1)<boundary)
   {
		contributionIndex++;
		contributionThisYear=contributionList[contributionIndex];
		return true;
   }
	else
   	return false;
}



