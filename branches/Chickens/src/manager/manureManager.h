/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/

// Class: Manure_manager_class                  //Borland C++ 5.02 - FASSET style
#define NT     //stops FASSET files from redefining Boolean

#ifndef __Manure_manager_class_H  //Required for current class
  #define __Manure_manager_class_H

#include "../building/thebuild.h"
#include "../fields/cropRotation.h"

class manureManager: public base
{
  buildings * theBuildings;
  cropRotation * theCropRotation;
  double MaxManureImp;            //maximum import in tonnes per farm per year
  double MinManureImp;            //minimum import in tonnes per farm per year

  double ExpectedONHolding;    // expected amount of organic manure (currently just slurry) in storage (kg)
  double ActualONHolding;     // actual amount of organic manure (currently just slurry) in storage (kg)
  double adjustmentFactor;

  double importedManure;
  double manureExtracted;
  bool adjustManure; //balances manure application to field with manure production, NJH Oct 2007

public:

 manureManager ();
 ~manureManager ()  ;

//  livestock * gettheLivestock();

  void Initialize(cropRotation * atheCropRotation, buildings * atheBuildings);
  void ReceivePlan(char* fileExtension);
  double GetadjustmentFactor() {return adjustmentFactor;}
  void DoAdjustment();


  double GetImportableManure(double requestedManure);
  void AddManureExtracted(double aVal){manureExtracted+=aVal;};
	void ExportStoredManure();
};

extern manureManager *theManure_manager;

#endif

