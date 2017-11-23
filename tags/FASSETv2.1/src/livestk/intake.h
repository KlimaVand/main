/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// Class: intake                  //Borland C++ 5.02 - FASSET style with linkList
#define NT     //stops FASSET files from redefining Boolean

#ifndef __intake_H  //Required for current class
  #define __intake_H

#include <base.h>
class intake: public base
{

public:
  intake ();// :();
  intake (const char *aName, const int aIndex, const base * aOwner);

 ~ intake ( ) //Destructor
{}

// Get and set functions for attributes

//user created functions

};
#endif
