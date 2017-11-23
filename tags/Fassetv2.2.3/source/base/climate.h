/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Class "climate"
Handles climate data, provided by an ascii-file.
(c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning
E-mail: bpe@pvf.sp.dk
\****************************************************************************/

#ifndef CLIMCLAS_H
   #define CLIMCLAS_H

#include "typer.h"
#include "base.h"
#include "bstime.h"
#include <string.h>
class climate: public base
{
	enum {strlength=255};
	char buffer[strlength];
	char filenam[strlength];
   double CorrectionFactor[13];

   bsTime FirstDate;            // First date in climate database
	bsTime LastDate;             // Last date in climate database

	int
      firstYear,                         // first year in database
      lastYear,                          // last  year in database
      yr,
      mnth,
      d,
      filetyp,
      firstob,
      EpotCalculationMethod;             // Epot calculation method if not given
   double normRad[13];    //normal radiation MJ/d
   double normTemp[13];   //normal daily mean temperature,C

   bool Circular;                      // If using circular dates!
  	bool DailyCO2Values;
	bool found;
	char climateDirectory[255];

	fstream /*f1,*/ TemperatureFile;

	climate& operator=(const climate& c); // Dissable the compilers generation of default assignment operator.
	climate(const climate& c);            // Dissable the compilers generation of default copy constructor.

	void ReadLine();
   void UpdateMeasuredSoilTemperature();
   double VaporPress(double temp);
   double DeltaVaporPress(double temp);
   double GiveAngle(int month,int day);
	double PenmanEvaporation(double globalRadiation,double temp,double windSpeed,double humidity);
   double MakkinkEvaporation(double globalRadiation,double temp);
   double AdjustPrecip(int month);


public:
	double
      tmin,                            // Celsius
      tmax,                            // Celsius
      tmean,                           // Celsius
      humidity,                        // relative humidity, as a decimal fraction
      rad,                             // MJ per square metre
      precip,                          // mm = kg per square metre
      windVelocity,                    // metres per s
      epot,                            // mm = kg per square metre
      YearlyAverage,                   // average yearly temperature calculated in initclimate
      co2,                             // ppm
      CO2Concentration,                // ppm
      NitrogenInRain,                  // g N mm-1
      degreeDays,								//above zero Celsius, counted from 01.01.
      latitude;

   climate(){filetyp = 0;EpotCalculationMethod = 0; found = false;};
   climate(int aType) {filetyp = aType; EpotCalculationMethod = aType;};
   climate(int aType,int aMethod) {filetyp = aType; EpotCalculationMethod = aMethod;};
   ~climate();

   void SetEpotCalculationMethod(int aMethod) {EpotCalculationMethod = aMethod;}
   double GetCO2Concentration();
	double GetNitrogenInRain() {return NitrogenInRain;}
   double GetnormRad(int monthNo){return normRad[monthNo];};
   double GetnormTemp(int monthNo){return normTemp[monthNo];};
   void SetClimateDirectory(char * aDir){strcpy(climateDirectory,aDir);};

   double GetLattitude() {return latitude;}
   void InitClimate(char *filename, int filetype, int firstobs);      // filetype 0: given Epot, 1: penman , 2: makkink, 3: DAISY
   void InitClimate(char *aDirectory,char *filename);                                      // automaticly recognise the type
	void Update();
	void GetClimate(int year, int month, int day, double &temp, double &min_temp,
						 double &max_temp, double &precip, double &radiation,
						 double &EPot, double &windspeed, double &relHumidity,int &found);
	double Evaporation(double radiation,double temp,double humidity, double transferCoefficient);
	double Evaporation(double temp,double humidity, double transferCoefficient);
	bool FindClimate(int year, int month, int day);
	void GetClimate(double &temp, double &min_temp,
						 double &max_temp, double &prec, double &radiation,
						 double &EPot, double &windspeed, double &relHumidity);
   double GetMeanTemperature(){return tmean;}
   // Sun functions
   double DayAngle();
   double SunDeclination();
	double PhotoPeriod();

   // Soil temperature
   double GetNextSoilTemperature();

   double GetdegreeDays(){return degreeDays;};
   void ResetdegreeDays(){degreeDays=0.0;};
   void SetdegreeDays(double aValue){degreeDays=aValue;};
	void Update(bsTime *aTime);
   void CalcAverageClimate();
	void CalcAverageClimateForPeriod(bsTime *startDate, int numOfDays, double *aveTemperature, double *aveWindspeed);
};

extern climate * theClimate;

#endif

