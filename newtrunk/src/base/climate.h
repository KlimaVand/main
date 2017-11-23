/*
 * * * Class "climate"
 * Handles climate data, provided by an ascii-file.
 * (c) Bj�rn Molt Petersen, Danmarks JordbrugsForskning
 * E-mail: bpe@pvf.sp.dk
 * \
 */



#ifndef CLIMCLAS_H
#define CLIMCLAS_H

#include "typer.h"
#include "base.h"
#include "bstime.h"
#include "../tools/fileAccess.h"

class climate:
    public base
{
    enum { strlength = 255 };

    char              buffer[strlength];
string filename;
    double            CorrectionFactor[13];
    fstream *         file;
    fileAccess        hd;
    bsTime            FirstDate;                                      // First date in climate database
    bsTime            LastDate;                                       // Last date in climate database
    int               firstYear,                                      // first year in database
                      lastYear,                                       // last  year in database
                      yr, mnth, d, filetyp, EpotCalculationMethod;    // Epot calculation method if not given
    double            normRad[13];                                    // normal radiation MJ/d
    double            normTemp[13];                                   // normal daily mean temperature,C
    bool              Circular;                                       // If using circular dates!
    bool              DailyCO2Values;
    bool              found;
    string              climateDirectory;


    fstream /* f1, */ TemperatureFile;

    climate & operator = (const climate & c);    // Dissable the compilers generation of default assignment operator.

    climate(const climate & c);    // Dissable the compilers generation of default copy constructor.

    void ReadLine();

    void UpdateMeasuredSoilTemperature();

    double VaporPress(double temp);

    double DeltaVaporPress(double temp);

    double GiveAngle(int month,
                     int day);
	double e(double temprature);
public:
	double Rnl();
	double Rnet();

	double PenmanEvaporation();

    double MakkinkEvaporation(double globalRadiation,
                              double temp);

    double AdjustPrecip(int month);

    int GetnumPrecipDays() {
        return numPrecipDays;
    }

    public:
		double tmin,                // Celsius
			tmax,                // Celsius
			tmean,               // Celsius
			tmeanMinus1,
			humidity,            // relative humidity, as a decimal fraction
			rad,                 // MJ per square metre
			precip,              // mm = kg per square metre
			windVelocity,        // metres per s
			epot,                // mm = kg per square metre
			YearlyAverage,       // average yearly temperature calculated in initclimate
			co2,                 // ppm
			CO2Concentration,    // ppm
			NitrogenInRain,      // g N mm-1
			degreeDays,          // above zero Celsius, counted from 01.01.
			latitude,
			Tdew;
		double ChangeMinPlantTemp;


        int    numPrecipDays;

        climate() {
			tmin = tmax = tmean =tmeanMinus1 = humidity = rad = precip = windVelocity = epot = 0.0;
            YearlyAverage         = co2 = CO2Concentration = NitrogenInRain = 0.0;
            degreeDays            = latitude = 0.0;
            firstYear             = lastYear = yr = mnth = d = filetyp = EpotCalculationMethod = numPrecipDays = 0;
            Circular              = false;
            DailyCO2Values        = false;
            found                 = false;
            file                  = nullptr;
            filetyp               = 0;
            EpotCalculationMethod = 0;
            found                 = false;
        }

        climate(int aType,
                int aMethod) {
            tmin                  = tmax = tmean = humidity = rad = precip = windVelocity = epot = 0.0;
            YearlyAverage         = co2 = CO2Concentration = NitrogenInRain = 0.0;
            degreeDays            = latitude = 0.0;
            firstYear             = lastYear = yr = mnth = d = filetyp = EpotCalculationMethod = numPrecipDays = 0;
            Circular              = false;
            DailyCO2Values        = false;
            found                 = false;
            file                  = nullptr;
            filetyp               = aType;
            EpotCalculationMethod = aMethod;
        }

        ~climate();

        double GetCO2Concentration();

        double GetNitrogenInRain() {
            return NitrogenInRain;
        }

        double GetnormRad(int monthNo) {
            return normRad[monthNo];
        }

        double GetnormTemp(int monthNo) {
            return normTemp[monthNo];
        }

        void InitClimate(string filename);

        void InitClimate(string aDirectory,
                         string filename);    // automaticly recognise the type

        void Update();

        double Evaporation(double radiation,
                           double temp,
                           double humidity,
                           double transferCoefficient);

        double Evaporation(double temp,
                           double humidity,
                           double transferCoefficient);

        void GetClimate(double & temp,
                        double & min_temp,
                        double & max_temp,
                        double & prec,
                        double & radiation,
                        double & EPot,
                        double & windspeed,
                        double & relHumidity);

        double GetMeanTemperature() {
            return tmean;
        }

        // Sun functions
        double DayAngle();

        double SunDeclination();

        double PhotoPeriod();

        // Soil temperature
        double GetNextSoilTemperature();

        void ResetdegreeDays() {
            degreeDays = 0.0;
        }

        void Update(bsTime * aTime);

        void CalcAverageClimate();

        void CalcAverageClimateForPeriod(bsTime * startDate,
                                         int      numOfDays,
                                         double * aveTemperature,
                                         double * aveWindspeed);
};


extern climate * theClimate;
#endif

