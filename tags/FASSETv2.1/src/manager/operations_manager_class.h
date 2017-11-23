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

#include <livestoc.h> //Required for include files
#include <thebuild.h>
#include <croprot.h> //Required for include files
#include <grazing_manager_class.h>
#include <manure_manager.h>
#include <cattlefeeding_manager_class.h>
#include <bstime.h>

class Operations_manager_class: public base
{
  livestock * theLivestock;
  cropRotation * theCropRotation;
  buildings * theBuildings;
  grazing_manager_class * grazing_manager;
//  Manure_manager_class *manure_manager;
  cattlefeeding_manager_class * cattleFeedingManager;

  int lengthOfLactation; // in days from parturition
  int conceptionTime; //days after parturition
  double milkQuota;					//litres per year
  double meatExport;
  double ageAtWeaning;		//age at which beef animals are weaned
  fstream *opsfile;			//file for output of operational information

public:

//user created functions

 Operations_manager_class ();

 ~Operations_manager_class ()  ;


  void SettheBuildings (buildings * atheBuildings) {theBuildings = atheBuildings;};
  livestock * gettheLivestock();
  cropRotation * gettheCropRotation();

  void daily_update ()  ;
  void Initialize(livestock * atheLivestock,
									   cropRotation * atheCropRotation,
                              buildings * atheBuildings);
  void ReceivePlan(char *fileExtension, char *inputDir);
  void autoCropPilot();
  void GetStatus();
  double GetmeatExport();
  double GetmilkQuota() {return milkQuota;};
  int GetlengthOfLactation() {return lengthOfLactation;}
  int GetconceptionTime() {return  conceptionTime;};
  void SetroughageEnergyAllowance();
  void InitialiseFileOutput();
  void GetNumber();
};
#endif
