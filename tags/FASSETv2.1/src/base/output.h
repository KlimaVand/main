/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class: output
(c) Jørgen Berntsen, Danmarks JordbrugsForskning
E-mail: bpe@pvf.sp.dk

Controls the detailed output from different objects
\****************************************************************************/

#ifndef __OUTPUT_H
   #define __OUTPUT_H

#include <base.h>

class OutputControl : public base
{
   private:
      fstream f;
   	enum {NumberWaterProfiles=20};
      double WaterStart[NumberWaterProfiles];
      double WaterEnd[NumberWaterProfiles];
      double NminStart[NumberWaterProfiles];
      double NminEnd[NumberWaterProfiles];
      double SoilCStart[NumberWaterProfiles];
      double SoilCEnd[NumberWaterProfiles];
      double CupDepth[NumberWaterProfiles];
      double TemperatureDepth[NumberWaterProfiles];
      bool PlantDetails;
      bool SoilDetails;
      bool WaterDetails;
      bool CowDetails;
      bool HerdDetails;
      bool PastureDetails;
      bool OperationalDetails;
      bool ManureManageDetails;

   typedef char string80[80];
      string80 outputDirectory;

   public:
      OutputControl();
      ~OutputControl();
      void Initialize(char* filename);
      void ScenarieLogNewLine();
      void ScenarieLog(string aLogString);
      void ScenarieLog(double aNumber);
 		double GetWaterStartDepth(int i) {return WaterStart[i];};
 		double GetWaterEndDepth(int i) {return WaterEnd[i];};
 		double GetNminStart(int i) {return NminStart[i];};
 		double GetNminEnd(int i) {return NminEnd[i];};
 		double GetSoilCStart(int i) {return SoilCStart[i];};
 		double GetSoilCEnd(int i) {return SoilCEnd[i];};
		double PoreWaterDepth(int i) {return CupDepth[i];};
		double GetTemperatureDepth(int i) {return TemperatureDepth[i];};

      bool GetPlantDetails() {return PlantDetails;};
      bool GetSoilDetails() {return SoilDetails;};
      bool GetWaterDetails() {return WaterDetails;};
      bool GetCowDetails() {return CowDetails;};
      bool GetHerdDetails() {return HerdDetails;};
      bool GetPastureDetails() {return PastureDetails;};
      bool GetOperationalDetails() {return OperationalDetails;};
      bool GetManureManageDetails() {return ManureManageDetails;};

      void SetPlantDetails(bool pd) {PlantDetails=pd;};
      void SetSoilDetails(bool sd) {SoilDetails=sd;};

      void SetoutputDirectory(char *aCharStr) {strcpy(outputDirectory,aCharStr);}
      char *GetoutputDirectory() {return outputDirectory;}
};

extern OutputControl * theOutputControl;

#endif


