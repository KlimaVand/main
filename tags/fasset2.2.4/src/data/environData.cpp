/*
 * environData.cpp
 *
 *  Created on: Dec 10, 2009
 *      Author: beer
 */
#include  "environData.h"
#include "systemData.h"
#include "../tools/convert.h"
#include "../base/common.h"

#include "../products/products.h"
#include "../livestock/animal.h"
void environData::loaddataNITROSCAPE()
{
	add();
}
void environData::checkData()
{

}
void environData::add()
{
	CO2Concentration=377.0;
	latitude=56.5;
	PrecipitationCorrection=0;
	NitrogenInRain=0.002;
	CO2ConcentrationFile=false;
	latitudeFile=false;
	PrecipitationCorrectionFile=false;
	NitrogenInRainFile=false;

}
void environData::loaddata()
{

	fileAccess hd;
	hd.changeDir(theSystemData->getInputDirectory());
	add();
	convert conv;
	if(readFileWithoutExit("environ.dat"));
	{
		for(unsigned int i=0;i<items.size();i++)
		{
			string line=items.at(i);
			string nameOfItem=conv.partOfAString(line,1);
			string value=conv.partOfAString(line,2);
			if(nameOfItem.compare("CO2Concentration")==0)
			{
				CO2Concentration=conv.StringToDouble(value);
				CO2ConcentrationFile=true;
			}
			else if(nameOfItem.compare("latitude")==0)
			{
				latitude=conv.StringToDouble(value);
				latitudeFile=true;
			}
			else if(nameOfItem.compare("PrecipitationCorrection")==0)
			{
				PrecipitationCorrection=conv.StringToInt(value);
				PrecipitationCorrectionFile=true;
			}
			else if(nameOfItem.compare("NitrogenInRain")==0)
			{
				NitrogenInRain=conv.StringToDouble(value);
				NitrogenInRainFile=true;
			}
			else
			{
				string output="for environ.dat is there a keyword: "+conv.partOfAString(line,1)+"that is not used ";
				//theMessage->Warning(output);
			}
		}

	}

}


