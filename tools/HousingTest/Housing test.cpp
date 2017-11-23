// Housing test.cpp : Defines the entry point for the console application.
//

#include <math.h>
#include "testVent.h"
#include <sstream>
#include <iostream>
using namespace std;
int main(int argc)
{
	double controlledVent        = true;
	double minTemperature        = 273.0 + 5.0;
	double maxTemperature = 0;
	double planArea              = pow((21 * 24.5),0.5);
	double meanHeight = (7.5+4.45)/2;
	double thermalTransWall = 3;
	double thermalTransRoof      = 1.5;
	double apertureWidth         = 24.5;
	double maxapertureHeight = 61/apertureWidth;
    double minPropApertureHeight = 0.05;
	double absorbCoeff = 0.2;
	double emissivity = 0.8;
    double externSurfResis       = 0.001;
	double ventilationm3LUh=0;
    double heatOP      =  60 * 65 *pow((140 + 150),0.5) - 800;//102 * 544.0;
	heatOP += 42 * 65 * pow((270 + 150),0.5)-800;
	double LU=(60 * 140 + 42 * 270)/500;
	//used only in controlled ventilation
	double targetTemperature = 20.0 + 273.15;
	double minVentilation = 150 * LU/3600; //m**3/sec
	double maxVentilation = 10 * minVentilation;
	double maxSupplHeat = 1 * 1000;  //W
	//Used only in freely ventilated housing
	double optimumAirVelocity=2.0;

	double maxTHI = 72.0;
	testVent* atestVent = new testVent(controlledVent, 
			meanHeight, 
			planArea,
			thermalTransWall, 
			minTemperature, 
			maxTemperature,
			thermalTransRoof,
			apertureWidth,
			maxapertureHeight,
			minPropApertureHeight,
			absorbCoeff,
			emissivity,
			externSurfResis,
			targetTemperature,
			minVentilation,
			maxVentilation,
			maxSupplHeat,
			optimumAirVelocity) ;
	


//read from data file Cooper.txt
	int hours[30]; //Hour
	int maxTime =20; //set to max number of data rows in input file
	double Ameantemp	[30]; //OutCelsius
  	double Awindspeed[30]; //Windspeed
	double Aradiation[30]; //Rad
	double measuredIntCelsius[30]; //IntCelsius
	double originalModel[30]; //	oModel				
	ifstream inputFile("C:\\workspace\\tools\\HousingTest\\Cooper data.txt");
    string line;
	getline(inputFile, line);
	int iterator=0;
    while (getline(inputFile, line))
    {
        istringstream ss(line);
		//	Hour				oModel						IntCelsius						OutCelsius			Rad						Windspeed
		ss >> hours[iterator] >> originalModel[iterator] >> measuredIntCelsius[iterator] >> Ameantemp[iterator]>>Aradiation[iterator]>>Awindspeed[iterator];
		iterator++;
    }
	ofstream myfile;
	myfile.open ("C:\\workspace\\tools\\HousingTest\\example.txt");
	if (myfile.is_open())
		myfile << "N" << "\t" << " Hr " << "\t" << " out " << "\t" << " meastemp " << "\t" << " simtemp " << "\t" << " orig " << "\t" 
		<< " simvent1 " << "\t" << " simvent1 "<< "\t" << "supplHeat"<< "\t" << "propvent" << endl;

	double Ahumidity=0.80;
	double thisModelIntCelsius=0;
	double thisModelVentilationRate=0;
	double thisPropVent=0;
	double thisEvap=0;
	double thisSupplHeat=0;
	for (int i=0; i<maxTime; i++)
	{
		atestVent->calculate(Ameantemp[i],0,Aradiation[i], Ahumidity, heatOP, maxTHI,
			&thisModelIntCelsius, &thisModelVentilationRate, &thisSupplHeat, &thisPropVent, &thisEvap);
		ventilationm3LUh=thisModelVentilationRate * 3600/LU;

		std::cout << i << " hr " << hours[i] << " meastemp " << measuredIntCelsius[i] << " simtemp " << thisModelIntCelsius 
			<< " orig " << originalModel[i] << " simvent1 " << thisModelVentilationRate << " simvent1 " << ventilationm3LUh 
			<< endl;
		if (myfile.is_open())
		{
			myfile << i << "\t" << hours[i] << "\t" << Ameantemp[i] << "\t" << measuredIntCelsius[i] << "\t" << thisModelIntCelsius 
				<< "\t" << originalModel[i] << "\t" << thisModelVentilationRate << "\t" << ventilationm3LUh << "\t" << thisSupplHeat
				<< "\t" << thisPropVent<< endl;
		}
	}
	myfile.close();
	delete atestVent;

	return 0;
}

