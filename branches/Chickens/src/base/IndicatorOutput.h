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


class IndicatorOutput
{
   private:
   	enum {NumberOfIndicatorTypes=5,SubIndex=20};

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

      static fstream* fileStreamCSV;

      static int yearNumber;

      linkList<item>* indicatorList[NumberOfIndicatorTypes][SubIndex];

   	IndicatorOutput& operator=(const IndicatorOutput& o); // Dissable the compilers generation of default assignment operator.
	   IndicatorOutput(const IndicatorOutput& o);            // Dissable the compilers generation of default copy constructor.
      static int FindString(item * aItem);
      static int BiggerThan(item * aItem);
      int FindSubIndex(const char * aName);
      linkList <item>* FindList(int indicatorType, char* aName);
      void InitializeIndicators();

   public:
      IndicatorOutput();
      ~IndicatorOutput();
      void Initialize(char * filenameCSV);

      static  void OutCSV(item * aItem);
      void AddIndicator(int indicatorType, string aName, string aUnit, double aAmount);
      void AddStateIndicator(int indicatorType, string aName, string aUnit, double aAmount);
      void AddIndicator(int indicatorType, string aName, string aUnit, nitrogen aAmount);
      void AddStateIndicator(int indicatorType, string aName, string aUnit, nitrogen aAmount);
      void FlushIndicator(int indicatorType, int aYearNumber);
      double GetAmount(int indicatorType, string aName);
      void DailyUpdate();
      void CheckBalance();
      void SetMaxErrorN(double maxE){maxErrorN=maxE;};
      void SetMaxErrorC(double maxE){maxErrorC=maxE;};
};

extern IndicatorOutput * theOutput;

#endif


