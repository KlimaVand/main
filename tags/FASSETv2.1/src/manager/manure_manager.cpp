/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <common.h>
#include "manure_manager.h"
#include <stable.h>
#include <plan.h>

Manure_manager_class::Manure_manager_class ()
{
  MaxManureImp = 1E+30; // BMP changed this september 2006 !!!
  MinManureImp = 0.0;
  MinManureExp = 0.0;
  ExpectedONHolding = 0.0;
  ActualONHolding = 0.0;
  exportedManure = 0.0;
  importedManure = 0.0;
  adjustmentFactor = 1.0;
  manureExtracted = 0.0;
  adjustManure = false;
}

Manure_manager_class::~Manure_manager_class()
{
}

/*******************************************************************
created 14.8.02 by NJH
Only call after all livestock, fields etc are created
********************************************************************/
void Manure_manager_class::Initialize(//livestock * atheLivestock,
									   cropRotation * atheCropRotation,
                              buildings * atheBuildings)
{
 theCropRotation=atheCropRotation;
 theBuildings=atheBuildings;
}

/****************************************************************************\
	ReceivePlan read the plan for all animals
\****************************************************************************/
void Manure_manager_class::ReceivePlan(char* fileExtension)
//Calculates number of animals to be imported + herd structure
{
   char fileName[15];
   strcpy(fileName,"manure.");
   strcat(fileName,fileExtension);
   UnsetCritical();
   if(!OpenInputFile(fileName))
   {
   	strcpy(fileName,"manure.fnn");
      if (!OpenInputFile(fileName))
		   {theMessage->FatalError("Manure_manager::ReceivePlan - error in opening manure.f??");}
   }

	theMessage->LogEvent("Reading manure import/export details");
   SetCritical();
   FindSection("farm");
   GetParameter("MaxManureImp",&MaxManureImp);
   GetParameter("MinManureImp",&MinManureImp);
   GetParameter("MinManureExp",&MinManureExp);
   GetParameter("adjustManure",&adjustManure);
   UnsetCritical();
   CloseInputFile();
}

void Manure_manager_class::DoAdjustment()
{
 double a = 0.5;
 double b = 0.6;
 ExpectedONHolding = 1.0 * theBuildings->GetTotalSlurryCap();
 ActualONHolding = theBuildings->GetTotalSlurryStored();

 adjustmentFactor=1.0;
 if ((adjustManure)&& ((ExpectedONHolding * a)>0)) // BMP added !!!
    adjustmentFactor=b * ActualONHolding/(a * ExpectedONHolding);
 if (adjustmentFactor>1.5)
 	adjustmentFactor=1.5;
 if (adjustmentFactor<0.5)
 	adjustmentFactor=0.5;
// adjustmentFactor=1.0;
};

double Manure_manager_class::GetImportableManure(double requestedManure)
{
 double permissableManure = MaxManureImp - importedManure;
 if (permissableManure>requestedManure)
 {
   importedManure+=requestedManure;
   return requestedManure;
 }
 else
 {
   importedManure+=permissableManure;
   return permissableManure;
 }

 }

void Manure_manager_class::ExportStoredManure()
{
	manure *aManure;
   aManure=(manure *) theBuildings->EmptyManureStores();
   theOutput->AddIndicator(environmentalIndicator,"35.22 N in exported manure","kg N",aManure->GetTotalN().n*aManure->GetAmount()*1000.0);
   theOutput->AddIndicator(environmentalIndicator,"45.22 C in exported manure","kg C",aManure->GetC_content()*aManure->GetAmount()*1000.0);
   theMessage->LogEventWithTime("Tonnes manure exported ", aManure->GetAmount());
   delete aManure;
}
