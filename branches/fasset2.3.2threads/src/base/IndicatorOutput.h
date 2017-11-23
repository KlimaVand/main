/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __INDICAT_H
   #define __INDICAT_H
#include "item.h"
#include "message.h"
#include "bstime.h"
#include "linklist.h"
#include "../products/nitrogen.h"
#include "id.h"

class IndicatorOutput
{
	int id;
   private:
   	enum {NumberOfIndicatorTypes=5,economicIndicatorStart=0,economicIndicatorStop=2100,environmentalIndicatorStart=3000,SubIndex=10000};

      double
         oldstoreCropN,

         lastErrorCropN,

         lastErrorN,
         lastErrorSoilN,
         lastErrorLivestockN,
         lastErrorHouseEtcN,
         lastErrorStoredN,
         oldstoreW,
         storeSoilN,
         oldstoreSoilN,
         storeLivestockN,
         oldstoreLivestockN,
         storeHouseEtcN,
         oldstoreHouseEtcN,
			oldstoredProductN,
         storeW,
         oldstoreN,
         storeN;
      double
         oldstoreCropC,
         storeCropC,
         lastErrorCropC,
         lastErrorC,
         lastErrorSoilC,
         lastErrorLivestockC,
         lastErrorHouseEtcC,
         storeSoilC,
         oldstoreSoilC,
         storeLivestockC,
         oldstoreLivestockC,
         storeHouseEtcC,
         oldstoreHouseEtcC,
			oldstoredProductC,
         oldstoreC,
         storeC;

		double maxErrorN,maxErrorC;
      bool makingList;

      static fstream* fileStreamCSV[100];

      static int yearNumber[100];

      item * indicatorList[SubIndex];

   	IndicatorOutput& operator=(const IndicatorOutput& o); // Dissable the compilers generation of default assignment operator.

      static int FindString(item * aItem);
      static int BiggerThan(item * aItem);
      int FindSubIndex(const char * aName);

      void InitializeIndicators();

   public:
      IndicatorOutput();
      ~IndicatorOutput();
      IndicatorOutput(const IndicatorOutput& o);
      void Initialize(char * filenameCSV);

      static  void OutCSV(item * aItem);
      void AddIndicator(int index, const char * aName, const char * aUnit, double aAmount);
      void AddStateIndicator(int index, const char * aName, const char * aUnit, double aAmount);
      void FlushIndicator(int indicatorType, int aYearNumber);
      double GetAmount(int index, string aName);
      void DailyUpdate();
      void CheckBalance();
      void SetMaxErrorN(double maxE){maxErrorN=maxE;};
      void SetMaxErrorC(double maxE){maxErrorC=maxE;};
};

extern IndicatorOutput * theOutput[100];

#endif


