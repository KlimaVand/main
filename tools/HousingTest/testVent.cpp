
#include "testVent.h"
#include "nixcbits.h"
#include "constants.h"

/*testVent::testVent()
{
    wallArea              = 0.0;    // area of one wall, assume housing is cubic
    minVentilation        = 0.0;    // min ventilation rate in cubic metres per sec
    maxVentilation        = 0.0;    // max ventilation rate in cubic metres per sec
    planArea              = 0.0;    // plan area of house
    insideTemperature     = 0.0;
    ventilationRate       = 0.0;    // ventilation rate (cubic metres per second)
    apertureWidth         = 0.0;
    maxapertureHeight     = 0.0;
    minPropApertureHeight = 0.0;
    minTemperature        = 0.0;
    maxTemperature        = 0.0;
    planAreaRatio         = 0.0;    // area of house relative to the total area of sections
}
*/
testVent::~testVent() {}

        testVent::testVent(bool acontrolledVent, 
			double ameanHeight, 
			double aplanArea,
			double athermalTransWall, 
			double aminTemperature, 
			double amaxTemperature,
			double	athermalTransRoof,
			double aapertureWidth,
			double amaxapertureHeight,
			double aminPropApertureHeight,
			double aabsorbCoeff,
			double aemissivity,
			double aexternSurfResis,
			double atargetTemperature,
			double aminVentilation,
			double amaxVentilation,
			double amaxSupplHeat,
			double aoptimumAirVelocity) 
		{
            controlledVent        = acontrolledVent;
            minTemperature        = aminTemperature;
			maxTemperature		= amaxTemperature;
            planArea              = aplanArea;
			meanHeight			= ameanHeight;
			thermalTransWall	 = athermalTransWall;
            thermalTransRoof      = athermalTransRoof;
			apertureWidth         = aapertureWidth;
			maxapertureHeight = amaxapertureHeight;
            minPropApertureHeight = aminPropApertureHeight;
			absorbCoeff			= aabsorbCoeff;
			emissivity			 = aemissivity;
            externSurfResis       = aexternSurfResis;
			targetTemperature = atargetTemperature;
			minVentilation = aminVentilation;
            maxVentilation        = amaxVentilation;
			maxSupplHeat		=amaxSupplHeat;
			optimumAirVelocity = aoptimumAirVelocity;
			planAreaRatio=0;
			ventilationRate=0;
			insideTemperature=0;
			waterEvap=0;
			wallArea			= meanHeight * pow(planArea,0.5);
			surfaceArea      = planArea + wallArea;    // sq meters - surface area of house
        }



void testVent::calculate(double Atemp, double Awindspeed, double Aradiation, double Ahumidity, 
						 double heatOP, double thresholdTermalHeatIndex, double *thisModelIntCelsius, 
						 double *thisModelVentilation, double *supplHeat, double *thisPropVent, double *evaporation)
{
            //assume that inside temperature is close to outside temperature
        double propSensibleHeat = CalcPropSensible(Atemp);
        double sensibleHeatOp   = propSensibleHeat * heatOP;
		*supplHeat=0;
		//heatOP is in Watts but time step is seconds, so Watts is equivalent to Joules here
        *evaporation = (1 - propSensibleHeat) * heatOP / GetLatentHeatVaporisationWater(Atemp);

        //!calculate the air velocity, using the appropriate functions for controlled or freely ventilated systems
        if (controlledVent > 0)
		{
            double supplementaryHeat = 0.0;

            controlled(sensibleHeatOp, Atemp, Aradiation, supplHeat);
        } 
		else 
            uncontrolled(sensibleHeatOp, Atemp, Awindspeed, Aradiation, Ahumidity, thresholdTermalHeatIndex);
		*thisModelIntCelsius = insideTemperature;
		*thisModelVentilation =	ventilationRate;
		*thisPropVent = propVent;
}


/*
 * Calculate the area of the wall, Assuming that the testVent is cubic
 */
void testVent::calcWallArea() {
    wallArea = 4 * meanHeight * pow(planArea, 0.5);    // sq m - area of walls

    // pow(planArea,0.5) is the length of a wall.
}

// !Calculates the ventilation and inside temperature of animal housing with forced ventilation systems

/*
 * !
 * param heatOP output of sensible heat by the animals in the housing (in Watts)
 * param outsideAirTemp outside air temperature in Celsius
 * param supplementaryHeat supplementary heating required to maintain the inside temperature at the target level (in Watts)
 */
void testVent::controlled(double   heatOP,
                        double   outsideAirTemp,
                        double   solarRad,
                        double * supplementaryHeat) {
    outsideAirTemp += 273.13;

    double deltaTemper = 0.0;

    *supplementaryHeat = 0.0;
    ventilationRate    = 0.0;
	meanThermalTrans = (planArea * thermalTransRoof + wallArea * thermalTransWall)
                              / surfaceArea;          // in W per metre square per K

    double skyTemp = outsideAirTemp;    // apparent temperature of sky, K
	
    // !Temperature of outside surface of roof
    double surfaceTemp = outsideAirTemp;    // rough estimate of temperature of roof surface, K
    double longWave    = GetNetLongWave(skyTemp, surfaceTemp, emissivity);
    double airDensity  = GetdensityAir(StandardAirPressure, targetTemperature,
                                       GetsaturatedWaterVapourPressure(targetTemperature));

    // !tempSol = outside air temperature which, in the absence of solar radiation, would give the same temperature distribution and rate of energy transfer
    // !through a wall or roof as that which exists with the actual air temperature and incident radiation
    double tempSol = outsideAirTemp
                     + externSurfResis * (absorbCoeff * solarRad - emissivity * longWave);    // eqn 9 in Cooper et al

    // !Temperature difference between outside surfaces of roof and air temperature
    double deltaSol = tempSol - outsideAirTemp;

    // !Heat input or output to housing through the roof material
    double q = thermalTransRoof * planArea * deltaSol;    // Watts

    // should include a water balance!
    if (targetTemperature > outsideAirTemp) {    // it will be possible to maintain target temp
        ventilationRate = (((heatOP - q) / (targetTemperature - outsideAirTemp)) - meanThermalTrans * surfaceArea)
                          / (GetspecificHeatCapAir() * airDensity);
        insideTemperature = targetTemperature - 273.13;
    }

    if ((ventilationRate > maxVentilation)
            || (targetTemperature
                <= outsideAirTemp)) {    // it will not be possible to maintain target temp, so set ventilation to max
        ventilationRate = maxVentilation;
        deltaTemper     = (heatOP - q)
                          / (GetspecificHeatCapAir() * airDensity * ventilationRate + meanThermalTrans * surfaceArea);

        // calculate the inside temperature
        insideTemperature = outsideAirTemp + deltaTemper - 273.13;
    }

    if (ventilationRate < minVentilation)     // to keep the animals healthy, there needs to be some ventilation
        {
			ventilationRate = minVentilation;

        // calculate the supplementary heat input necessary to maintain the inside temperature at the target temperature (joules)
        *supplementaryHeat = (targetTemperature - outsideAirTemp)
                             * (GetspecificHeatCapAir() * airDensity * ventilationRate
                                + meanThermalTrans * surfaceArea) - heatOP;
        insideTemperature = targetTemperature - 273.13;
		if (*supplementaryHeat>maxSupplHeat)//cannot maintain target temperature
		{
			*supplementaryHeat=maxSupplHeat;
	        deltaTemper     = (heatOP + maxSupplHeat - q)
                          / (GetspecificHeatCapAir() * airDensity * ventilationRate + meanThermalTrans * surfaceArea);
        // calculate the inside temperature
	        insideTemperature = outsideAirTemp + deltaTemper - 273.13;
		}
    }
	propVent=ventilationRate/maxVentilation;
}

/*
 * Nic need to document this one
 */
void testVent::uncontrolled(double heatOP,
                          double outsideAirTemp,
                          double windspeed,
                          double solarRad,
                          double relativeHumidity,
						  double thresholdTermalHeatIndex) 
{
    maxTemperature = GetMaxPermissableTemperature(thresholdTermalHeatIndex, relativeHumidity);
    maxTemperature += 273.15;
    outsideAirTemp += 273.15;

    double skyTemp = outsideAirTemp;    // apparent temperature of sky, K

    // !Temperature of outside surface of roof
    double surfaceTemp = outsideAirTemp;    // rough estimate of temperature of roof surface, K
    double longWave    = GetNetLongWave(skyTemp, surfaceTemp, emissivity);
    double airDensity  =
        GetdensityAir(StandardAirPressure, outsideAirTemp,
                      GetsaturatedWaterVapourPressure(outsideAirTemp));    // density of the air in kg per cubic metre

    // should include a water balance!
    ventilationRate = 0;    // cubic meters per sec

    double deltaTemper, tempDeltaTemp;

    // !tempSol = outside air temperature which, in the absence of solar radiation, would give the same temperature distribution and rate of energy transfer
    // !through a wall or roof as that which exists with the actual air temperature and incident radiation
    double tempSol = outsideAirTemp
                     + externSurfResis * (absorbCoeff * solarRad - emissivity * longWave);    // eqn 9 in Cooper et al

    // !Temperature difference between outside surfaces of roof and air temperature
    double deltaSol = tempSol - outsideAirTemp;

    // !Heat input or output to housing through the roof material
    double q = thermalTransRoof * planArea * deltaSol;    // Watts

    // !First set ventilation rate to that which is necessary to have the optimum air velocity
    double targetventilationRate = optimumAirVelocity * wallArea/4;

	double ThermalTrans=0;

	if (outsideAirTemp>maxTemperature) //open aperature to maximum and calculate the ventilation rate and temperature difference
	{
		//! Calculate the ventilation rate and temperature difference when aperture is at maximum opening
		ThermalTrans = (planArea * thermalTransRoof + (wallArea - maxapertureHeight * apertureWidth) * thermalTransWall)
                              / (planArea + wallArea - maxapertureHeight * apertureWidth);          // in W per metre square per K

	    CalcFreeVentilation(airDensity, ThermalTrans, surfaceArea, maxapertureHeight, windspeed, outsideAirTemp, heatOP,
                        q, &ventilationRate, &deltaTemper);
		propVent=1;
	}
	else
	{
		ThermalTrans = (planArea * thermalTransRoof + (wallArea - 0.5 * maxapertureHeight * apertureWidth) * thermalTransWall)
                              / surfaceArea;          // in W per metre square per K
		//!Calculate the temperature difference that will be achieved by the target ventilation with outside air
	    deltaTemper = (heatOP + q)
                  / (GetspecificHeatCapAir() * airDensity * targetventilationRate + ThermalTrans * surfaceArea);

		// !if the inside temperature achieved obtaining the optimal ventilation rate exceeds the maximum permitted, recalculate the ventilation rate to prevent this
		if (outsideAirTemp + deltaTemper > maxTemperature) 
		{
			double maxDeltaT                 = maxTemperature - outsideAirTemp;
			ThermalTrans = (planArea * thermalTransRoof + (wallArea - maxapertureHeight * apertureWidth) * thermalTransWall)
                              / surfaceArea;          // in W per metre square per K

			double heatTransportThroughWalls = maxDeltaT * ThermalTrans * surfaceArea;

			targetventilationRate = (((heatOP + q)/maxDeltaT) - heatTransportThroughWalls)
									/ (GetspecificHeatCapAir() * airDensity);
			deltaTemper = maxDeltaT;
		}

	    // !if the temperature achieved obtaining the optimal ventilation rate is below the minimum permitted, recalculate the ventilation rate to prevent this
		if (outsideAirTemp + deltaTemper < minTemperature) 
		{
			double minDeltaT                = minTemperature - outsideAirTemp;
			ThermalTrans = (planArea * thermalTransRoof + (wallArea - minPropApertureHeight * maxapertureHeight * apertureWidth) * thermalTransWall)
                              / surfaceArea;          // in W per metre square per K
			double heatTransportThroughWalls = minDeltaT * ThermalTrans * surfaceArea;

			targetventilationRate = (((heatOP + q)/minDeltaT) - heatTransportThroughWalls)
									/ (GetspecificHeatCapAir() * airDensity);
			deltaTemper = minDeltaT;
		}

		// !now we know the target ventilation rate, we need to see if this is achievable by adjusting the aperture size
		// !assume initially that this is possible
		ventilationRate = targetventilationRate;

		double minVent;
		// !calculate ventilation with the minimum ventilation
		double apertureHeight = minPropApertureHeight * maxapertureHeight;

		ThermalTrans = (planArea * thermalTransRoof + (wallArea - minPropApertureHeight * maxapertureHeight * apertureWidth) * thermalTransWall)
							/ surfaceArea;          // in W per metre square per K
		CalcFreeVentilation(airDensity, ThermalTrans, surfaceArea, apertureHeight, windspeed, outsideAirTemp, heatOP,
							q, &minVent, &tempDeltaTemp);

		double maxVent;
		// !calculate ventilation with the maximum ventilation
		apertureHeight = maxapertureHeight;

		ThermalTrans = (planArea * thermalTransRoof + (wallArea - maxapertureHeight * apertureWidth) * thermalTransWall)
								/ surfaceArea;          // in W per metre square per K
		CalcFreeVentilation(airDensity, ThermalTrans, surfaceArea, apertureHeight, windspeed, outsideAirTemp, heatOP,
							q, &maxVent, &tempDeltaTemp);

		// !if the target ventilation is less than the minimum, use the minimum
		if (targetventilationRate < minVent) 
		{
			ventilationRate = minVent;
			deltaTemper     = tempDeltaTemp;
		}

		// !If ventilation exceeds the maximum, use the maximum
		if (targetventilationRate > maxVent) 
		{
			ventilationRate = maxVent;
			deltaTemper     = tempDeltaTemp;
		}
		propVent=(ventilationRate-minVent)/(maxVent-minVent);
	}


    insideTemperature = outsideAirTemp + deltaTemper - 273.0;
}

void testVent::CalcFreeVentilation(double   airDensity,
                                 double   thermalTrans,
                                 double   surfaceArea,
                                 double   apertureHeight,
                                 double   windspeed,
                                 double   outsideAirTemp,
                                 double   heatOP,
                                 double   q,
                                 double * ventilationRate,
                                 double * deltaTemper) {

    // ! Model from A THERMAL BALANCE MODEL FOR LIVESTOCK BUILDINGS, Cooper et al, 1998
    *ventilationRate = 0.0;

    double Cd = 0.6;    // !discharge coefficient for sharp-edged openings

    // !B = factor to simplify subsequent equations
    double B = 9.0 * outsideAirTemp / (pow(Cd, 2) * pow(apertureWidth, 2) * pow(apertureHeight, 3) * 9.81);
    double x[3];
    int    solutions;

    // ! a1, b1, c1, d1 are coefficients of the cubic equation number 21 in Cooper et al
    double spheatCap = GetspecificHeatCapAir();
    double a1        = spheatCap * airDensity;
    double b1        = thermalTrans * surfaceArea;

    // !windforcedVent is the ventilation that occurs due to the effect of the wind blowing air through the apertures in the building
    double windforcedVent =
        0.25 * apertureHeight * apertureWidth
        * windspeed;    // we assume a square building, with only one wall facing the wind, cubic m per sec
    double c1 = -pow(windforcedVent, 2) * spheatCap * airDensity;
    double d1 = -thermalTrans * surfaceArea * pow(windforcedVent, 2) - (heatOP + q) / B;

    // !solve for ventilation rate
    SolveCubic(a1, b1, c1, d1, &solutions, x);

    int    index = 0;
    double test  = 0.0;

    // solution is the highest value in the matrix
    for (int j = 0; j < solutions; j++) {
        if (x[j] > test) {
            test  = x[j];
            index = j;
        }

        *deltaTemper     = (heatOP + q) / (spheatCap * airDensity * x[index] + thermalTrans * surfaceArea);
        *ventilationRate = x[index];
    }
}

double testVent::GetNetLongWave(double skyTemp,
                              double surfaceTemp,
                              double emissivity) {
    double longWaveDown = GetlongWaveDown(skyTemp);    // longwave radiation input to surface, Watts per square metre
    double longWaveUp   =
        GetlongWaveUp(
            emissivity,
            surfaceTemp);    // longwave radiation output of surface, Watts per square metre; note small error here, if surface temperature not equal to air temperature
    double longWave = longWaveDown - longWaveUp;    // net longwave energy exchange, Watts per square metre

    return longWave;
}


/*
 * return proportion of total heat production that is sensible heat
 * Equation 29 4th Report of Working Group on Climatization of Animal Houses
 * Heat and moisture production at animal and house levels
 * Editors: Pedersen, S. & S�llvik, K.
 */
double testVent::CalcPropSensible(double temperature) {
	double ret_val = (0.8*(1000 + 12 * (20-temperature)) - 0.38 * pow(temperature, 2))/1000;

    ret_val *= 0.85;    // assumes wet surfaces so 0.85

    return ret_val;
}


double testVent::GetMaxPermissableTemperature(double thermalHeatIndex, double relativeHumidity) 
{
	relativeHumidity/=100;
    // source = National Research Council. 1971. A guide to environmental research on animals. Natl. Acad. Sci., Washington, DC.
    // only valid for dairy cattle
    return (thermalHeatIndex - (46.3 - 0.143 * relativeHumidity)) / (0.81 + 0.0099 * relativeHumidity);
}


double testVent::GetTotalAirVolume()
{
	return meanHeight * planArea;
}