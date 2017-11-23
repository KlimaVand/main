/*
 * environData.h
 *
 *  Created on: Dec 10, 2009
 *      Author: beer
 */

#ifndef ENVIRONDATA_H_
#define ENVIRONDATA_H_

#include "commonData.h"
class environData: commonData{
public:
	double getCO2Concentration(){return CO2Concentration;};
	double getLatitude(){return latitude;};
	int getPrecipitationCorrection(){return PrecipitationCorrection;};
	double getNitrogenInRain(){return NitrogenInRain;};
	bool getCO2ConcentrationFile(){return CO2ConcentrationFile;};
	bool getLatitudeFile(){return latitudeFile;};
	bool getPrecipitationCorrectionFile(){return PrecipitationCorrectionFile;};
	bool getNitrogenInRainFile(){return NitrogenInRainFile;};
	environData(){
#ifndef NITROSCAPE
	    	cout<<"Loading environ data"<<endl;
	    	loaddata();
	    	checkData();
	      cout<<"Done with loading and checking environ data"<<endl;
#else
	      loaddataNITROSCAPE();
#endif
	    }


protected :
	void add();
	void loaddataNITROSCAPE();
	double CO2Concentration;
	double latitude;
	int PrecipitationCorrection;
	double NitrogenInRain;
	bool CO2ConcentrationFile;
	bool latitudeFile;
	bool PrecipitationCorrectionFile;
	bool NitrogenInRainFile;
    void loaddata();
    void checkData();


};
extern environData * theEnvironData;
#endif /* ENVIRONDATA_H_ */
