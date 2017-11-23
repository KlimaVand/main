/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "manureManager.h"


manureManager::manureManager ():base()
{
  MaxManureImp = 1E+30; // BMP changed this september 2006 !!!
  MinManureImp = 0.0;

  ExpectedONHolding = 0.0;
  ActualONHolding = 0.0;

  importedManure = 0.0;
  adjustmentFactor = 1.0;
  manureExtracted = 0.0;
  adjustManure = false;
}

manureManager::~manureManager()
{
}

/*******************************************************************
created 14.8.02 by NJH
Only call after all livestock, fields etc are created
********************************************************************/
void manureManager::Initialize(//livestock * atheLivestock,
									   cropRotation * atheCropRotation,
                              buildings * atheBuildings)
{
 theCropRotation=atheCropRotation;
 theBuildings=atheBuildings;
}

/****************************************************************************\
	ReceivePlan read the plan for all animals
\****************************************************************************/
void manureManager::ReceivePlan(char* fileExtension)
//Calculates number of animals to be imported + herd structure
{
   char fileName[15];
   strcpy(fileName,"manure.");
   strcat(fileName,fileExtension);
   commonData data;
   data.setCritical(false);
   if(!data.readFile(fileName))
   {
   	strcpy(fileName,"manure.fnn");
   	data.setCritical(true);
    data.readFile(fileName);

   }

	theMessage->LogEvent("Reading manure import/export details");
	data.setCritical(true);
   data.FindSection("farm");
   data.FindItem("MaxManureImp",&MaxManureImp);
   data.FindItem("MinManureImp",&MinManureImp);

   data.FindItem("adjustManure",&adjustManure);

}

void manureManager::DoAdjustment()
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

double manureManager::GetImportableManure(double requestedManure)
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

void manureManager::ExportStoredManure()
{
	manure *aManure;
   aManure=(manure *) theBuildings->EmptyManureStores();
   theOutput->AddIndicator(3522,"35.22 N in exported manure","kg N",aManure->GetTotalN().n*aManure->GetAmount()*1000.0);
   theOutput->AddIndicator(3522,"45.22 C in exported manure","kg C",aManure->GetC_content()*aManure->GetAmount()*1000.0);
	theOutput->AddIndicator(3521,"35.21 N removed from manure storage","kg N",aManure->GetTotalN().n*aManure->GetAmount()*1000.0);
   theOutput->AddIndicator(4521,"45.21 C removed from manure storage","kg C",aManure->GetC_content()*aManure->GetAmount()*1000.0);
   theMessage->LogEventWithTime("Tonnes manure exported ", aManure->GetAmount());
   delete aManure;
}
