/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: Operations_manager_class                  //Borland C++ 5.02 - FASSET style
#define NT     //stops FASSET files from redefining Boolean

#ifndef __Operations_manager_class_H  //Required for current class
  #define __Operations_manager_class_H

#include "../livestock/livestock.h" //Required for include files
#include "../building/thebuild.h"
#include "../fields/cropRotation.h" //Required for include files
#include "grazingManager.h"
#include "manureManager.h"
#include "cattleFeedingManager.h"
#include "../base/bstime.h"

class operationsManager: public base
{
  livestock * theLivestock;
  cropRotation * theCropRotation;
  buildings * theBuildings;
  grazingManager * grazing_manager;
//  Manure_manager_class *manure_manager;
  cattleFeedingManager * cattleFeedingManagerInstance;

  int lengthOfLactation; // in days from parturition
  int conceptionTime; //days after parturition
  double milkQuota;					//litres per year
  double meatExport;
  double ageAtWeaning;		//age at which beef animals are weaned
  fstream *opsfile;			//file for output of operational information

public:

//user created functions

 operationsManager ();

 ~operationsManager ()  ;





  void daily_update ()  ;
  void Initialize(livestock * atheLivestock,
									   cropRotation * atheCropRotation,
                              buildings * atheBuildings);
  void ReceivePlan(char *fileExtension, char *inputDir);
  void autoCropPilot();
  void GetStatus();
  void InitialiseFileOutput();

};
#endif
