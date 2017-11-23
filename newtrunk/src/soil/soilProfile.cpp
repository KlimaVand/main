/*
 * Column of soil layers stored as a list
 * (c) Joergen E. Olesen, Statens Planteavlsfors�g
 *
 * Changes:
 * JEO 23-06-1997: Support for nullptr-pointer in rootLengthList added
 * JEO 23-06-1997: GetPoolCarbon and GetPoolNitrogen added
 * JEO 08-07-1997: GetAmmoniumLeaching and GetNitrateLeaching added
 * JEO 27-10-1997: New soil evaporation formula
 * BMP 15-09-1999: Corrected errors in function "GetTemperature"
 * BMP 04-07-2000: Corrected error in destructor
 * BMP 02-08-2000: Switch between tipping bucket and darcy flow implemented
 * MEL 01-03-2007: New temperature model (finite difference solution) implemented
 * MEL 15-05-2007: New Water model from the SWAT model implemented
 */



#include "../base/common.h"
#include "../base/settings.h"
#include "soilProfile.h"
#include "../base/cloneList.h"
#include "../base/bstime.h"
#include "../base/message.h"
#include "soil.h"
#include "../base/controlParameters.h"
#include "../base/climate.h"

const double LatentHeatMelting    = 334400.0;                            // J/kg
const double HeatCapacityWater    = 4192.0;                              // J/(kg dgC)
const double HeatCapacitySolid    = 750.0;                               // J/(kg dgC)
const double HeatCapacityIce      = 2050;                                // J/(kg dgC)
const double WaterDensity         = 1000;                                // kg/m3
const double IceDensity           = 920;                                 // kg/m3
const double VolHeatCapacityIce   = HeatCapacityIce * IceDensity;        // J/(m3 dgC)
const double VolHeatCapacityWater = HeatCapacityWater * WaterDensity;    // J/(m3 dgC)
const double snowConductivity     = 2.86e-6 * 300 * 300;                 // se DAISY
const double specCapacity         = LatentHeatMelting * WaterDensity;    // J/m3

soilProfile::soilProfile(string       Pname,
                         const int    Index,
                         const base * owner):
    base(Pname,
         Index,
         owner)
{
    dayZero            = 0;
    first              = nullptr;
    last               = nullptr;
    cum_infiltration   = averageAirTemp = amplitudeAirTemp = 0.0;
    meanAirTemperature = 0;

    for (int i = 0; i < MaxPlants; i++) {
        nitrogenUptake[i].Clear();
    }

    for (int i = 0; i < MaxSoilLayers; i++) {
        soilLayerArray[i]          = nullptr;
        distanceToPreviousLayer[i] = 0;
        distanceToNextLayer[i]     = 0;

        // NJH did this, feb 2009
        temperature[i]           = 0;
        heatConductivity[i]      = 0.0;
        heatConductivitySlope[i] = 0.0;
        heatCapacity[i]          = 0.0;
        SpecWaterCapacity[i]     = 0.0;
    }

    // NJH did this, feb 2009
    infiltration        = 0.0;
    depthDampning       = 0.0;
    maxRootDepth        = 0.0;
    NumOfSoilLayers     = 0;
    EvapExtCoef         = 0.01;
    EvaporationCapacity = 10.0;
    EvaporationContent  = 0.0;
    minTemp[0]          = -10;
    minTemp[1]          = -10;
    minTemp[2]          = -10;
    minTemp[3]          = -3;
    minTemp[4]          = 0;
    minTemp[5]          = 5;
    minTemp[6]          = 7;
    minTemp[7]          = 8;    // Because of start-up problem
    minTemp[8]          = 8;
    minTemp[9]          = 3;
    minTemp[10]         = -5;
    minTemp[11]         = -10;
    lastErrorYear       = -1;
    calculateAverage    = true;
}

/*
 * FARM-N pig farm 96074: calls a nullptr here at some point ???!!!
 */
double soilProfile::GetMaximumDepth() {
    if (!soilLayerArray[NumOfSoilLayers - 1]) {
        theMessage -> FatalError("soilProfile::GetMaximumDepth - array points at nullptr");

        return 0;
    } else {
        return soilLayerArray[NumOfSoilLayers - 1] -> GetEndDepth();
    }
}

void soilProfile::Initialize(string soilFileName) {
    averageAirTemp     = 7.7;
    amplitudeAirTemp   = 8.0;
    dayZero            = 212.0;    // august 1th the hottest day january 30th the coldest day
    meanAirTemperature = 8;

    fileAccess hd;

    hd.changeDir(globalSettings -> getInputDirectory());

    commonData * data = new commonData();

    data -> setCritical(true);
    data -> readFile(soilFileName);

    double startDepth = 0.0;

    data -> FindSection("SoilParameters");

    double defaultTemperature = 8;

    data -> setCritical(false);
    data -> FindItem("defaultTemperature", &defaultTemperature);
	
	data->FindItem("EvaporationCapacity", &EvaporationCapacity);
    data -> setCritical(true);
    data -> FindItem("Nodes", &NumOfSoilLayers);

    double nodeThickNess = 100;

    data -> setCritical(false);
    data -> FindItem("NodeThickness", &nodeThickNess);
    data -> FindItem("averageAirTemp", &averageAirTemp);
    data -> FindItem("amplitudeAirTemp", &amplitudeAirTemp);
    data -> FindItem("dayZero", &dayZero);
    data -> FindItem("meanAirTemperature", &meanAirTemperature);
	double holdbackconstantTopLater = 0.9;
	double holdbackconstantLowerLayer = 0.7;
	data->FindItem("holdbackconstantTopLater", &holdbackconstantTopLater);
	data->FindItem("holdbackconstantLowerLayer", &holdbackconstantLowerLayer);
    double JBC[] = {
        0.030, 0.03, 0.025, 0.02, 0.02, 0.015, 0.015, 0.01, 0.01, 0.01
    };
    int    JB    = 5;

    EvapExtCoef = JBC[JB - 1];

    soilLayer *  aTempSoilLayer;
    commonData * soilData        = new commonData();
    string *     prevoisFileName = new string("nothing");

    for (int i = 0; i < NumOfSoilLayers; i++) {
        aTempSoilLayer = new soilLayer("soilLayer", i, this);
		if (startDepth>199)
			aTempSoilLayer->Initialize(startDepth, nodeThickNess, data, soilData, prevoisFileName, holdbackconstantLowerLayer);
		else
			aTempSoilLayer->Initialize(startDepth, nodeThickNess, data, soilData, prevoisFileName, holdbackconstantTopLater);
        AddItem(aTempSoilLayer);

        soilLayerArray[i] = aTempSoilLayer;
        startDepth        += nodeThickNess;
    }

    delete prevoisFileName;
    delete soilData;

    const double c1 = 0.06;
    const double c2 = 0.15;
    const double d1 = 600.0;
    const double d2 = 2000.0;

    data -> FindSection("SoilParameters");
    data -> setCritical(false);

    if (!data -> FindItem("MaxRootDepth", &maxRootDepth)) {
        double Clay = GetClayContent(500.0, 200.0);

        if (Clay < c1) {
            maxRootDepth = d1;
        } else {
            maxRootDepth = min(d2, d1 + (d2 - d1) * (Clay - c1) / (c2 - c1));
        }
    }

    if (startDepth < maxRootDepth) {
        theMessage -> FatalError(
            "soilProfile::Initialize - The depth of defined layers is smaller than the root depth");
    }

    for (int i = 0; i < NumOfSoilLayers; i++) {    // Calculates the distance between layers and converts to meter
        temperature[i] = defaultTemperature;                                         // default temperature
        aTempSoilLayer = soilLayerArray[i];

        if (i == 0) {
            distanceToPreviousLayer[i] = aTempSoilLayer -> GetThickness() / 2000;    // 1000 ???!!!
        } else {
            distanceToPreviousLayer[i] = (aTempSoilLayer -> GetThickness() + soilLayerArray[i - 1] -> GetThickness())
                                         / 2000.0;
        }

        if (i == (NumOfSoilLayers - 1)) {
            distanceToNextLayer[i] = aTempSoilLayer -> GetThickness() / 1000;
        } else {
            distanceToNextLayer[i] = (aTempSoilLayer -> GetThickness() + soilLayerArray[i + 1] -> GetThickness())
                                     / 2000.0;
        }
    }

    for (int i = 0; i < ExtraLayers; i++) {    // extra layers to ensure no heat flow at bottom (boundary condition)
        soilLayerArray[NumOfSoilLayers + i]      = new soilLayer(*soilLayerArray[NumOfSoilLayers - 1]);    // extraLayer
        temperature[i + NumOfSoilLayers]         = defaultTemperature;
        distanceToNextLayer[i + NumOfSoilLayers] = distanceToNextLayer[NumOfSoilLayers - 1] * (i + 1) * 10.0;    // 50;

        soilLayerArray[NumOfSoilLayers + i] -> SetStartDepth(soilLayerArray[NumOfSoilLayers + i - 1] -> GetStartDepth()
                + 1000.0 * distanceToNextLayer[i + NumOfSoilLayers - 1]);

        distanceToPreviousLayer[i + NumOfSoilLayers] = distanceToNextLayer[i + NumOfSoilLayers - 1];

        soilLayerArray[NumOfSoilLayers + i] -> SetThickness(distanceToNextLayer[i + NumOfSoilLayers] * 1000.0);
        soilLayerArray[NumOfSoilLayers + i] -> SetFieldCapacity(
            soilLayerArray[NumOfSoilLayers - 1] -> GetFieldCapacity()
            / soilLayerArray[NumOfSoilLayers - 1] -> GetThickness()
            * soilLayerArray[NumOfSoilLayers + i] -> GetThickness());
        soilLayerArray[NumOfSoilLayers + i] -> SetWater(soilLayerArray[NumOfSoilLayers + i] -> GetFieldCapacity());
    }

    for (int i = 0; i < (NumOfSoilLayers + ExtraLayers); i++) {
        DistPreviousLayerInv[i] = 1.0 / distanceToPreviousLayer[i];
        DistNextLayerInv[i]     = 1.0 / distanceToNextLayer[i];
    }

    if (theControlParameters -> GetWriteSoilTemperature()) {
        SoilTempFile.open("soiltemp.txt", ios::out);
    }

    delete data;
}

/*
 * Adds nitrogen to the first layer in the profile
 * addNitrate    - Nitrate to be added [g N/m�]
 * addAmmonium   - Ammonium to be added [g N/m�]
 */
void soilProfile::AddNutrient(nitrogen soilNitrate,
                              nitrogen soilAmmonium) {
    if (first) {
        first -> AddNutrient(soilNitrate, soilAmmonium);
    }
}

/*
 * Adds a soil layer to the profile
 * data          - Soil layer object
 */
void soilProfile::AddItem(soilLayer * data) {
    if (!first) {
        first = data;
    } else {
        last -> AddNext(data);
    }

    last = data;
}

void soilProfile::CheckCoherence(int checkpoint) {
    int TotalNumberOfLayers = NumOfSoilLayers + ExtraLayers;

    for (int i = 0; i < TotalNumberOfLayers; i++) {
        if (soilLayerArray[i] -> GetIce() > soilLayerArray[i] -> GetTotalWater() + 0.0000001) {
            cout << "Checkpoint " << checkpoint << ", layer " << i << " is not coherent";
            cout << endl;
        }
    }
}

void soilProfile::checkWaterCont(double waterContent, double maxContent) {
if (waterContent > maxContent + 0.0001) {
                theMessage -> WarningWithDisplay(
                    "soilProfile::UpdateInfiltration - top layer contains more water than physically possible");
            }
}

/*
 * Performs infiltration of water, nitrate and ammonium in profile using the SWAT
 * water model. Simulates the transport of water and solutes with timestep one hour.
 * Solutes are equilibrated between mobile and immobile domain only once a day.
 * Optional use of convection-dispersion equation for transport in mobile domain.
 * (MEL 2007)
 * surplus         - Water to be added to/returned from profile [mm]
 * nitrateLeached  - Nitrate to the added to/returned from profile [g N/mï¿½]
 * ammoniumLeached - Ammonium to the added to/returned from profile [g N/mï¿½]
 */
void soilProfile::UpdateInfiltrationSWAT(double * surplus,
        nitrogen *                                surfaceNitrate,
        nitrogen *                                surfaceAmmonium,
        nitrogen *                                NitrateLeached,
        nitrogen *                                AmmoniumLeached,
        double                                    Chloride) {

    int    TotalNumberOfLayers = NumOfSoilLayers + ExtraLayers;
    double totalFlux[MaxSoilLayers], maxContent[MaxSoilLayers], waterContent[MaxSoilLayers],
           FC[MaxSoilLayers], satHydraulicConductivity[MaxSoilLayers];
    double     standingWater;
    nitrogen   surplusNH4[MaxSoilLayers];
    nitrogen   surplusNO3[MaxSoilLayers];
    double     simulationTime = 0.0;
    double     deltaT         = 3600;
    pF_Curve * pF[MaxSoilLayers];


    for (int i = 0; i < TotalNumberOfLayers; i++) {
        soilLayerArray[i] -> ClearLeaching();    // set leaching to zero

        waterContent[i] = soilLayerArray[i] -> GetTotalWater();
        pF[i]           = soilLayerArray[i] -> GetpF_Curve();
        maxContent[i] = (soilLayerArray[i] -> GetPorosity() * soilLayerArray[i] -> GetThickness());
        totalFlux[i]  = 0.0;
        FC[i]         = soilLayerArray[i] -> GetFieldCapacity();

        surplusNH4[i].Clear();
        surplusNO3[i].Clear();
    }

    standingWater = *surplus;

    for (int i = 0; i < TotalNumberOfLayers; i++) {
        satHydraulicConductivity[i] = pF[i] -> GetConductivity(pF[i] -> GetpF(maxContent[i]
                / soilLayerArray[i] -> GetThickness()));    // in m/s
    }

    cum_infiltration = 0;

    while (simulationTime < 86400) {                         // simulates 24 hours
        double fluxInTimeStep[MaxSoilLayers];

        //fill_n(fluxInTimeStep, TotalNumberOfLayers, 0.0);    // init all fluxInTimeStep values to 0
        for (int i = 0; i < TotalNumberOfLayers; i++) {
        	fluxInTimeStep[i]  = 0;

        }

        // Perform infiltration of top layer.
        infiltration     = max(0.0, min(standingWater, maxContent[0] - waterContent[0]));    // in mm
        cum_infiltration += infiltration;

        if (infiltration > 0.0) {
            standingWater   -= infiltration;
            waterContent[0] += infiltration;

            checkWaterCont(waterContent[0], maxContent[0]);
        } else if (infiltration < (-1E-10)) {
            theMessage -> WarningWithDisplay(
                "soilProfile::UpdateInfiltrationSWAT - top layer contains more water than physically possible");
        }

        for (int i = 0; i < TotalNumberOfLayers; i++) {
            double NotFrozenWater  = waterContent[i] - soilLayerArray[i] -> GetIce();  // in mm
            double travelTime      = (maxContent[i] - FC[i]) / (satHydraulicConductivity[i] / 1000); // in s
            double soilWaterExcess = max(0.0, min(NotFrozenWater, waterContent[i] - FC[i])); // in mm

            if (soilWaterExcess > 0.0) { // drainage
                fluxInTimeStep[i] = soilWaterExcess * (1.0 - exp(-deltaT / travelTime));    // in mm per timestep

                if (i < (TotalNumberOfLayers - 1)) {
                	// reduce flux according to capacity in next layer
                    fluxInTimeStep[i] = max(
                        0.0, min(fluxInTimeStep[i], maxContent[i + 1] - waterContent[i + 1]));
                    waterContent[i + 1] += fluxInTimeStep[i]; // add flux to next layer
                }

                waterContent[i] -= fluxInTimeStep[i];  // remove flux from present layer
                totalFlux[i]    += fluxInTimeStep[i];
            } else {
                fluxInTimeStep[i] = 0.0;
            }

            checkWaterCont(waterContent[i], maxContent[i]);
        }

        // Transport of water and solutes, is this really SLIM??
        double f = 1.0;  // f is fraction of surface water that enter the soil

        if (*surplus > 0) {
            f = (*surplus - standingWater) / (*surplus);
        }

        nitrogen Nadd = *surfaceNitrate * f + (*surfaceAmmonium * f);

        soilLayerArray[0] -> AddWater(*surplus * f, *surfaceNitrate * f, *surfaceAmmonium * f, Chloride);

        for (int i = 0; i < TotalNumberOfLayers; i++) {
        	checkWaterCont(waterContent[i], maxContent[i]);

            soilLayerArray[i] -> SetWaterFlux(fluxInTimeStep[i]);
            soilLayerArray[i] -> SetEquilibrated(false);

            nitrogen NO3;
            nitrogen NH4;

            if (fluxInTimeStep[i] > 0.0) {    // Downward movement
                soilLayerArray[i] -> RemoveWaterHourly(fluxInTimeStep[i], &NO3, &NH4, &Chloride);

                if (i == TotalNumberOfLayers - 1) {
                    *NitrateLeached  = NO3;
                    *AmmoniumLeached = NH4;
                } else { // i < (TotalNumberOfLayers - 1)
                    soilLayerArray[i + 1] -> AddWater(fluxInTimeStep[i], NO3, NH4, Chloride);
                }
            }
        }

        *surplus         = *surplus * (1.0 - f);
        *surfaceNitrate  = *surfaceNitrate * (1.0 - f);
        *surfaceAmmonium = *surfaceAmmonium * (1.0 - f);

        // hvad med chloride!
        simulationTime += deltaT;
    }    // End transport of water and solute

    EvaporationContent = min(EvaporationCapacity, infiltration + EvaporationContent);

    for (int i = 0; i < TotalNumberOfLayers; i++) {

        // Equilibrate between mobile and immobile water (once a day)
        if (waterContent[i] < FC[i]) {
            soilLayerArray[i] -> EquilibrateNitrogen(0.0);
        } else {
            soilLayerArray[i] -> EquilibrateNitrogen(waterContent[i] - FC[i]);
        }

        soilLayerArray[i] -> SetWaterFlux(totalFlux[i]);
    }

    // End transport of water and solute
}

/*
 * Calculates soil water content for the profile.
 * returns       - Water content [mm]
 */
double soilProfile::GetTotalWater() {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content += current -> GetTotalWater();
        current = current -> Next();
    }

    return Content;
}

/*
 * Calculates soil water content for a portion of the profile.
 * startDep      - Start depth of layer to be calculated for [mm]
 * thick         - Thickness of layer to be calculated for [mm]
 * returns       - Plant available water [mm]
 */
double soilProfile::GetTotalWater(double startDepth,
                                  double thickness) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content += current -> GetTotalWater(startDepth, thickness);
        current = current -> Next();
    }

    return Content;
}

/*
 * Calculates plant available soil water content for a portion of the profile.
 * startDep      - Start depth of layer to be calculated for [mm]
 * thick         - Thickness of layer to be calculated for [mm]
 * returns       - Plant available water [mm]
 */
double soilProfile::GetAvailWater(double startDepth,
                                  double thickness) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content += current -> GetAvailWater(startDepth, thickness);
        current = current -> Next();
    }

    return Content;
}

/*
 * Calculates capacity for plant available water for a portion of the profile.
 * startDep      - Start depth of layer to be calculated for [mm]
 * thick         - Thickness of layer to be calculated for [mm]
 * returns       - Capacity for plant available water [mm]
 */
double soilProfile::GetAvailCapacity(double startDepth,
        double                              thickness) {
    double      Capacity = 0.0;
    soilLayer * current  = first;

    while (current) {
        Capacity += current -> GetAvailCapacity(startDepth, thickness);
        current  = current -> Next();
    }

    return Capacity;
}

/*
 * Calculates field capacity for water for a portion of the profile.
 * startDep      - Start depth of layer to be calculated for [mm]
 * thick         - Thickness of layer to be calculated for [mm]
 * returns       - Capacity for plant available water [mm]
 */
double soilProfile::GetFieldCapacity(double startDepth,
        double                              thickness) {
    double      Capacity = 0.0;
    soilLayer * current  = first;

    while (current) {
        Capacity += current -> GetFieldCapacity(startDepth, thickness);
        current  = current -> Next();
    }

    return Capacity;
}

/*
 * Calculates wilting capacity for for a portion of the profile.
 * startDep      - Start depth of layer to be calculated for [mm]
 * thick         - Thickness of layer to be calculated for [mm]
 * returns       - Capacity for plant available water [mm]
 */
double soilProfile::GetWiltCapacity(double startDepth,
        double                             thickness) {
    double      Capacity = 0.0;
    soilLayer * current  = first;

    while (current) {
        Capacity += current -> GetWiltCapacity(startDepth, thickness);
        current  = current -> Next();
    }

    return Capacity;
}

double soilProfile::GetDiscreteDepth(double depth) {
    int i = 0;

    while ((soilLayerArray[i] -> GetEndDepth() < depth) && (i < NumOfSoilLayers)) {
        i++;
    }

    return soilLayerArray[i] -> GetEndDepth();
}

/*
 * The temperature of a given soil layer is defined at the top of the layer
 */
double soilProfile::GetTemperature(double depth) {
    int i = 0;

    while ((soilLayerArray[i] -> GetStartDepth() <= depth) && (i < NumOfSoilLayers)) {
        i++;
    }

    if (i == (NumOfSoilLayers - 1)) {
        return soilLayerArray[i] -> GetTemperature();
    }

    double a = depth - soilLayerArray[i - 1] -> GetStartDepth();
    double b = soilLayerArray[i] -> GetStartDepth() - depth;

    return soilLayerArray[i - 1] -> GetTemperature() * b / (a + b)
           + soilLayerArray[i] -> GetTemperature() * a / (a + b);
}

double soilProfile::GetDrainage(double depth) {
    if (depth <= (soilLayerArray[0] -> GetThickness() / 2.0)) {
        return soilLayerArray[0] -> GetWaterFlux();
    } else if (depth
               >= (soilLayerArray[NumOfSoilLayers - 1] -> GetStartDepth()
                   + soilLayerArray[NumOfSoilLayers - 1] -> GetThickness() / 2.0)) {
        return soilLayerArray[NumOfSoilLayers - 1] -> GetWaterFlux();
    }

    int i = 0;

    while ((soilLayerArray[i] -> GetStartDepth() + soilLayerArray[i] -> GetThickness() / 2.0) < depth) {
        i++;
    }

    if (i == (NumOfSoilLayers - 1)) {
        return soilLayerArray[i] -> GetWaterFlux();
    }

    return soilLayerArray[i - 1] -> GetWaterFlux();
}

/*
 * Estimated maximum evaporation from whole profile
 */
double soilProfile::GetMaximumEvaporation() {
    soilLayer * current = first;
    double      sum     = 0.0;

    while (current) {
        sum += max(0., current -> GetAvailWater())
               * exp(-EvapExtCoef * current -> GetCenterDepth());    // current->GetThickness()*
        current = current -> Next();
    }

    return sum;
}

/*
 * Subtracts evaporation from profile
 * evaporation   - Evaporation to be subtracted [mm].
 */
void soilProfile::SubtractEvaporation(double soilEvaporation) {

    /*
     * // Newest, but presently discarded method
     * soilLayer * current = first;
     * while (soilEvaporation>0) {
     * double subtractAmount = min(soilEvaporation,current->GetAvailWater());
     * soilEvaporation -= subtractAmount;
     * current->SubtractEvaporation(subtractAmount);
     * current = current->Next();
     * }
     */

    // Original method - extended with the EvaporationContent part
    soilLayer * current = first;
    double      a;
    double      directEvaporation = min(min(soilEvaporation, EvaporationContent), soilLayerArray[0] -> GetAvailWater());

    soilEvaporation    -= directEvaporation;
    EvaporationContent -= directEvaporation;

    soilLayerArray[0] -> SubtractEvaporation(directEvaporation);

    if (soilEvaporation > 0) {
        double sum = GetMaximumEvaporation();

        while ((current) && (sum > 0)) {
            a =    // current->GetThickness()*
                max(0., current -> GetAvailWater()) * exp(-EvapExtCoef * current -> GetCenterDepth());

            current -> SubtractEvaporation(soilEvaporation * a / sum);

            current = current -> Next();
        }
    }
}

/*
 * Returns the water pressure potential in a given depth
 */
double soilProfile::GetPressurePotential(double depth) {
    if (depth <= (soilLayerArray[0] -> GetThickness() / 2.0)) {
        return soilLayerArray[0] -> GetPressurePotential();
    } else if (depth
               >= (soilLayerArray[NumOfSoilLayers - 1] -> GetStartDepth()
                   + soilLayerArray[NumOfSoilLayers - 1] -> GetThickness() / 2.0)) {
        return soilLayerArray[NumOfSoilLayers - 1] -> GetPressurePotential();
    }

    int i = 0;

    while ((soilLayerArray[i] -> GetStartDepth() + soilLayerArray[i] -> GetThickness() / 2.0) < depth) {
        i++;
    }

    if (i == (NumOfSoilLayers - 1)) {
        return soilLayerArray[i] -> GetPressurePotential();
    } else {
        return soilLayerArray[i - 1] -> GetPressurePotential();
    }
}

/*
 * Calculates the maximum transpiration from the soil profile
 * rootRadius     - Root radius [m]
 * rootpF         - Water potential at root surface [pF]
 * rootLengthList - List of root lengths for each layer [m/m�]
 * returns        - Maximum transpiration [mm]
 */
double soilProfile::MaxTranspiration(linkList<rootStructure> * rootList) {
    if (!rootList) {
        return 0.0;    // No transpiration without roots
    }

    double   maxTranspiration = 0.0;
    double   rootRadius, rootpF;
    double * rootLengthList;

    linkList<rootStructure>::PS currentRoot;
    rootList -> PeekHead(currentRoot);

    while (currentRoot) {
        rootRadius     = currentRoot -> element -> rootRadius;
        rootpF         = currentRoot -> element -> rootpF;
        rootLengthList = currentRoot -> element -> rootLengthList;

        soilLayer * current = first;

        while (current) {
            maxTranspiration += current -> MaxTranspiration(rootRadius, rootpF, *rootLengthList);

            rootLengthList++;

            current = current -> Next();
        }

        rootList -> OneStep(currentRoot);
    }

    return maxTranspiration;
}

/*
 * Generates average total root
 */
void soilProfile::EstimateTotalRoot(rootStructure * TotalRoot,
        linkList<rootStructure> *                   rootList) {
    TotalRoot -> rootRadius          = 0.0;
    TotalRoot -> rootpF              = 0.0;
    TotalRoot -> NitrateUptakeRate   = 0.0;
    TotalRoot -> AmmoniumUptakeRate  = 0.0;
    TotalRoot -> MinimumSoilNitrate  = 0.0;
    TotalRoot -> MinimumSoilAmmonium = 0.0;
    TotalRoot -> rootLengthList      = new double[MaxSoilLayers];    // LEAK !!!

    for (int j = 0; j < MaxSoilLayers; j++) {
        TotalRoot -> rootLengthList[j] = 0.0;
    }

    double TotalPlantRootLength[MaxPlants];
    double TotalRootLength = 0.0;
    int    AttackerNumber  = 0;

    if (rootList) {
        AttackerNumber = rootList -> NumOfNodes();
    }

    for (int i = 0; i < AttackerNumber; i++) {
        rootStructure * currentRoot = rootList -> ElementAtNumber(i);

        TotalPlantRootLength[i] = 0.0;

        for (int j = 0; j < MaxSoilLayers; j++) {
            TotalPlantRootLength[i] += currentRoot -> rootLengthList[j];
        }

        TotalRootLength += TotalPlantRootLength[i];
    }

    if (TotalRootLength > 0) {
        for (int i = 0; i < AttackerNumber; i++) {
            rootStructure * currentRoot = rootList -> ElementAtNumber(i);

            TotalRoot -> rootRadius        += TotalPlantRootLength[i] / TotalRootLength * currentRoot -> rootRadius;
            TotalRoot -> rootpF            += TotalPlantRootLength[i] / TotalRootLength * currentRoot -> rootpF;
            TotalRoot -> NitrateUptakeRate += TotalPlantRootLength[i] / TotalRootLength
                                              * currentRoot -> NitrateUptakeRate;
            TotalRoot -> AmmoniumUptakeRate += TotalPlantRootLength[i] / TotalRootLength
                                               * currentRoot -> AmmoniumUptakeRate;
            TotalRoot -> MinimumSoilNitrate += TotalPlantRootLength[i] / TotalRootLength
                                               * currentRoot -> MinimumSoilNitrate;
            TotalRoot -> MinimumSoilAmmonium += TotalPlantRootLength[i] / TotalRootLength
                    * currentRoot -> MinimumSoilAmmonium;

            for (int j = 0; j < MaxSoilLayers; j++) {
                TotalRoot -> rootLengthList[j] += currentRoot -> rootLengthList[j];
            }
        }
    }
}

/*
 * Subtracts estimated transpiration from the water in the layer.
 */
void soilProfile::SubtractTranspiration(linkList<rootStructure> * rootList) {
    double waterBefore = soilLayerArray[0] -> GetAvailWater();
    double PlantDemand[MaxPlants];
    double WaterUptakeAbility[MaxSoilLayers * MaxPlants];
    double TotalWaterUptakeAbility[MaxSoilLayers * MaxPlants];
    double WaterResource[MaxSoilLayers];
    double PlantUptake[MaxPlants * MaxSoilLayers];

    for (int i = 0; i < MaxPlants * MaxSoilLayers; i++) {
        PlantUptake[i] = 0;
    }

    int AttackerNumber = rootList -> NumOfNodes();

    if (AttackerNumber > MaxPlants) {
        cout << GetLongName() << endl;

        theMessage -> FatalError("soilProfile::SubtractTranspiration maximum number of crops exceeded.");
    }

    for (int i = 0; i < AttackerNumber; i++) {
        PlantDemand[i] = max(0.0, rootList -> ElementAtNumber(i) -> transpirationDemand);
    }

    rootStructure * TotalRoot = new rootStructure;

    EstimateTotalRoot(TotalRoot, rootList);

    int         ResourceNumber   = 0;
    soilLayer * currentSoilLayer = first;

    while (currentSoilLayer) {
        currentSoilLayer = currentSoilLayer -> Next();

        ResourceNumber++;
    }

    double TotalPlantUptake[MaxPlants];

    for (int j = 0; j < AttackerNumber; j++) {
        TotalPlantUptake[j] = 0.0;
    }

    linkList<rootStructure>::PS currentRoot;
    rootList -> PeekHead(currentRoot);

    int PlantIndex = 0;

    while (currentRoot) {
        int         SoilLayerIndex   = 0;
        soilLayer * currentSoilLayer = first;

        while (currentSoilLayer) {
            WaterUptakeAbility[SoilLayerIndex + PlantIndex * ResourceNumber] = currentSoilLayer -> MaxTranspiration(
                currentRoot -> element);
            currentSoilLayer = currentSoilLayer -> Next();

            SoilLayerIndex++;
        }

        rootList -> OneStep(currentRoot);
        PlantIndex++;
    }

    currentSoilLayer = first;

    int SoilLayerIndex = 0;

    while (currentSoilLayer) {
        TotalWaterUptakeAbility[SoilLayerIndex] = currentSoilLayer -> MaxTranspiration(TotalRoot);
        WaterResource[SoilLayerIndex]           = min(currentSoilLayer -> GetTotalWater()
                - currentSoilLayer -> GetIce(), TotalWaterUptakeAbility[SoilLayerIndex]);
        currentSoilLayer = currentSoilLayer -> Next();

        SoilLayerIndex++;
    }

    ResourceCompetition(ResourceNumber, AttackerNumber, PlantDemand, &WaterUptakeAbility[0], &WaterResource[0],
                        &PlantUptake[0]);

#ifndef __BCplusplus__
    delete[] TotalRoot -> rootLengthList;    // BMP added June 2002
#else

    delete [sizeof(double)*MaxSoilLayers] TotalRoot->rootLengthList;
    // BMP added June 2002
#endif

    delete TotalRoot;

    if (rootList) {    // Perform only if plants has roots
        soilLayer * currentSoilLayer = first;
        int         SoilLayerIndex   = 0;

        while (currentSoilLayer) {
            double subtractTransp = 0.0;

            for (int j = 0; j < AttackerNumber; j++) {
                double transpirationRate = PlantUptake[SoilLayerIndex + j * ResourceNumber];

                transpirationRate = min(transpirationRate,
                                        currentSoilLayer -> MaxTranspiration(rootList -> ElementAtNumber(j)));
                subtractTransp      += transpirationRate;
                TotalPlantUptake[j] += transpirationRate;
            }

            currentSoilLayer -> SubtractTranspiration(subtractTransp);

            currentSoilLayer = currentSoilLayer -> Next();

            SoilLayerIndex++;
        }
    }

    for (int i = 0; i < AttackerNumber; i++) {
        rootList -> ElementAtNumber(i) -> actualTranspiration = TotalPlantUptake[i];

        if (TotalPlantUptake[i] < 0) {
            theMessage -> WarningWithDisplay("soilProfile::SubtractTranspiration negative ", TotalPlantUptake[i]);
        }

        if (TotalPlantUptake[i] - rootList -> ElementAtNumber(i) -> transpirationDemand > 1e-5) {
            cout << theTime << endl;
            cout << "Layer: " << i << "  TotalPlantUptake: " << TotalPlantUptake[i]
                 << "  RootList_transpirationDemand: " << rootList -> ElementAtNumber(i) -> transpirationDemand;

            theMessage -> WarningWithDisplay("soilProfile::SubtractTranspiration exceeds demand by ",
                                             TotalPlantUptake[i]
                                             - rootList -> ElementAtNumber(i) -> transpirationDemand);
        }
    }

    if (waterBefore > 1E-5) {
        EvaporationContent *= soilLayerArray[0] -> GetAvailWater() / waterBefore;
    }
}

/*
 * Subtracts estimated nitrogen uptake by the plants from the mineral
 * nitrogen pools in the layer.
 */
nitrogen * soilProfile::SubtractNitrogenUptake(linkList<rootStructure> * rootList) {
    double PlantDemand[MaxPlants];
    double NitrogenUptakeAbility[2 * MaxSoilLayers * MaxPlants];
    double NitrogenResource[2 * MaxSoilLayers];
    double PlantUptake[MaxPlants * 2 * MaxSoilLayers];
    int    Iteration = MaxPlants * 2 * MaxSoilLayers;

    for (int i = 0; i < Iteration; i++) {
        PlantUptake[i] = 0;
    }

    nitrogen TotalPlantUptake[MaxPlants];
    int      AttackerNumber = rootList -> NumOfNodes();

    for (int i = 0; i < AttackerNumber; i++) {
        PlantDemand[i] = rootList -> ElementAtNumber(i) -> NitrogenDemand;

        TotalPlantUptake[i].Clear();
    }

    rootStructure * TotalRoot = new rootStructure;

    EstimateTotalRoot(TotalRoot, rootList);

    int         ResourceNumber   = 0;
    soilLayer * currentSoilLayer = first;

    while (currentSoilLayer) {
        currentSoilLayer = currentSoilLayer -> Next();

        ResourceNumber++;
    }

    ResourceNumber *= 2;    // 2 as both nitrate and ammonium are substrates

    linkList<rootStructure>::PS currentRoot;
    rootList -> PeekHead(currentRoot);

    int PlantIndex = 0;

    while (currentRoot) {
        int         SoilLayerIndex   = 0;
        soilLayer * currentSoilLayer = first;

        while (currentSoilLayer) {
            NitrogenUptakeAbility[2 * SoilLayerIndex + PlantIndex * ResourceNumber] = currentSoilLayer ->
                 MaxNitrogenFlux(false, currentRoot -> element);
            NitrogenUptakeAbility[2 * SoilLayerIndex + PlantIndex * ResourceNumber + 1] = currentSoilLayer ->
                 MaxNitrogenFlux(true, currentRoot -> element);
            currentSoilLayer = currentSoilLayer -> Next();

            SoilLayerIndex++;
        }

        rootList -> OneStep(currentRoot);
        PlantIndex++;
    }

    int SoilLayerIndex = 0;

    currentSoilLayer = first;

    while (currentSoilLayer) {
        double TotalAmmoniumUptake = currentSoilLayer -> MaxNitrogenFlux(false, TotalRoot);
        double LayerAmmonium       = currentSoilLayer -> GetAmmonium().n * currentSoilLayer -> FracAmmoniumInWater();

        NitrogenResource[2 * SoilLayerIndex] = min(LayerAmmonium, TotalAmmoniumUptake);

        double TotalNitrateUptake = currentSoilLayer -> MaxNitrogenFlux(true, TotalRoot);
        double LayerNitrate       = currentSoilLayer -> GetNitrate().n;

        NitrogenResource[2 * SoilLayerIndex + 1] = min(LayerNitrate, TotalNitrateUptake);
        currentSoilLayer                         = currentSoilLayer -> Next();

        SoilLayerIndex++;
    }

    ResourceCompetition(ResourceNumber, AttackerNumber, PlantDemand, &NitrogenUptakeAbility[0], &NitrogenResource[0],
                        &PlantUptake[0]);

    // delete [sizeof(double)*MaxSoilLayers] TotalRoot->rootLengthList; // BMP added June 2002
    delete[] TotalRoot -> rootLengthList;    // BMP added June 2002
    delete TotalRoot;

    if (rootList) {    // Perform only if plants has roots
        soilLayer * currentSoilLayer = first;
        int         SoilLayerIndex   = 0;

        while (currentSoilLayer) {
            for (int j = 0; j < AttackerNumber; j++) {
                nitrogen NH4LayerUptake =
                    currentSoilLayer -> SubtractAmmoniumUptake(PlantUptake[2 * SoilLayerIndex + j * ResourceNumber]);
                nitrogen NO3LayerUptake =
                    currentSoilLayer -> SubtractNitrateUptake(PlantUptake[2 * SoilLayerIndex + j * ResourceNumber + 1]);

                TotalPlantUptake[j] = TotalPlantUptake[j] + NH4LayerUptake + NO3LayerUptake;
            }

            currentSoilLayer = currentSoilLayer -> Next();

            SoilLayerIndex++;
        }
    }

    for (int i = 0; i < AttackerNumber; i++) {
        nitrogenUptake[i] = TotalPlantUptake[i];
    }

    return nitrogenUptake;
}

void soilProfile::ResourceCompetition(int ResourceNumber,
        int                               AttackerNumber,
        double *                          Demand,
        double *                          UptakeAbility,
        double *                          Resource,
        double *                          ResourceRemoved) {
    double PossibleUptake[MaxPlants];
    double MaxResourceUptake[2 * MaxSoilLayers];

    for (int i = 0; i < ResourceNumber; i++) {
        MaxResourceUptake[i] = 0.0;
    }

    for (int j = 0; j < AttackerNumber; j++) {
        PossibleUptake[j] = 0.0;

        for (int i = 0; i < ResourceNumber; i++) {
            ResourceRemoved[i + j * ResourceNumber] = 0.0;
        }
    }

    bool DemandFullfilled = false;

    while (!DemandFullfilled) {
        DemandFullfilled = true;

        for (int j = 0; j < AttackerNumber; j++) {
            for (int i = 0; i < ResourceNumber; i++) {
                MaxResourceUptake[i] += UptakeAbility[i + j * ResourceNumber];
            }
        }

        for (int j = 0; j < AttackerNumber; j++) {
            for (int i = 0; i < ResourceNumber; i++) {
                if (MaxResourceUptake[i] > 0) {
                    PossibleUptake[j] += UptakeAbility[i + j * ResourceNumber]
                                         * min(1.0, Resource[i] / MaxResourceUptake[i]);
                }
            }

            DemandFullfilled = DemandFullfilled & (PossibleUptake[j] > Demand[j]);
        }

        if (DemandFullfilled) {
            for (int j = 0; j < AttackerNumber; j++) {
                for (int i = 0; i < ResourceNumber; i++) {
                    if ((PossibleUptake[j] > 0) && (MaxResourceUptake[i] > 0) && (Demand[j] > 0)) {
                        ResourceRemoved[i + j * ResourceNumber] = Demand[j] / PossibleUptake[j]
                                * UptakeAbility[i + j * ResourceNumber] * min(1.0, Resource[i] / MaxResourceUptake[i]);
                    }
                }
            }
        } else {
            bool singleDemand = false;

            for (int j = 0; j < AttackerNumber; j++) {
                if ((PossibleUptake[j] > Demand[j]) && (Demand[j] > 0)) {    // single plant demand can be fullfilled !!
                    for (int i = 0; i < ResourceNumber; i++) {
                        if ((PossibleUptake[j] > 0) && (MaxResourceUptake[i] > 0)) {
                            ResourceRemoved[i + j * ResourceNumber] = Demand[j] / PossibleUptake[j]
                                    * UptakeAbility[i + j * ResourceNumber]
                                    * min(1.0, Resource[i] / MaxResourceUptake[i]);
                        }

                        UptakeAbility[i + j * ResourceNumber] = 0.0;
                    }

                    singleDemand = true;
                    Demand[j]    = 0.0;
                }
            }

            if (!singleDemand) {    // all plants uptake are below demand !!
                for (int j = 0; j < AttackerNumber; j++) {
                    for (int i = 0; i < ResourceNumber; i++) {
                        if ((PossibleUptake[j] > 0) && (MaxResourceUptake[i] > 0) && (Demand[j] > 0)) {
                            ResourceRemoved[i + j * ResourceNumber] = UptakeAbility[i + j * ResourceNumber]
                                    * min(1.0, Resource[i] / MaxResourceUptake[i]);
                        }
                    }
                }

                DemandFullfilled = true;
            }
        }
    }
}

/*
 * Calculates soil temperature using a simple algorithm.
 * Returned values for periods without frost are good estimates, returned
 * values for periods with frost are fair estimates as frost and snow influence
 * are not accounted for in a fully satisfactory fashion using this simple method.
 * Konstants are presently hand-fitted (8.3.1999).
 * soilTemperature    -
 * airTemperature     - mean air temperature of the current day (Celsius)
 * depth              - soil depth in mm
 * meanAirTemperature - yearly mean air temperature for the site (Celsius)
 */
void soilProfile::UpdateT(double * soilTemperature,
                          double   airTemperature,
                          double   depth,
                          double   meanAirTemperature) {
    const double k1 = 30000.0;
    const double k2 = -0.00033;
    const double k3 = 0.006;
    const double k4 = 0.12;

    if (depth > 0) {
        double a1 = k1 / (k1 + (depth * depth));

        if (*soilTemperature < k3 * depth) {
            a1 = a1 * k4;
        }

        double dampenFactor = exp(k2 * depth);
        double Tdampened    = ((airTemperature - meanAirTemperature) * dampenFactor)
                              + meanAirTemperature;    // Dampen amplitude. Tdampened is target value for this depth

        *soilTemperature += a1 * (Tdampened - *soilTemperature);    // Pursue target value
    } else {
        theMessage -> FatalError("soil::UpdateT - depth must be > 0.0");
    }
}

/*
 * Updates temperature from "master" soil
 */
void soilProfile::UpdateTemperature(const base * aOwner) {
    soilLayer * currentLayer = first;
    soil *      aSoil        = (soil *) aOwner;
    double      depth;

    while (currentLayer) {
        depth = currentLayer -> GetStartDepth() + 0.5 * currentLayer -> GetThickness();

        currentLayer -> PutTemperature(aSoil -> GetTemperature(depth));

        currentLayer = currentLayer -> Next();
    }
}

/*
 * Default temperature model (MEL 2007): UpdateFinDiffTemperature calculates
 * the soil temperature according the finite difference sceme
 * The upper boundary condition consist terms with
 * both airtemperature, radiation and snow cover.
 * fractionEvaporation is used to correct radiation for
 * reflection by leaves.
 */
void soilProfile::UpdateFinDiffTemperature(double AirTemperature,
        double                                    snow) {
    if (theControlParameters -> GetSimpleTemperatureModel() || theControlParameters -> GetMeasuredSoilTemperature()
            || theControlParameters -> GetAirToSoilTemperature()) {
        if (theControlParameters -> GetSimpleTemperatureModel()) {
            double soilTemperature;
            double depth;

            for (int i = 1; i < NumOfSoilLayers; i++) {
                soilTemperature = soilLayerArray[i] -> GetTemperature();

                if ((soilTemperature < -50) || (soilTemperature > 50)) {
                    theMessage -> FatalError(
                        "soilProfile::UpdateFinDiffTemperature - soil temperature not within realistic range");
                }

                depth = soilLayerArray[i] -> GetStartDepth() + soilLayerArray[i] -> GetThickness() * 0.5;

                UpdateT(&soilTemperature, AirTemperature, depth, 8.0);
            }

            soilLayerArray[0] -> PutTemperature(AirTemperature);
        } else if (theControlParameters -> GetAirToSoilTemperature()) {

            // set soil temperature to the same as air temperature - special for laboratory conditions
            for (int i = 0; i < NumOfSoilLayers; i++) {
                soilLayerArray[i] -> PutTemperature(AirTemperature);
            }
        } else {
            for (int i = 0; i < NumOfSoilLayers; i++) {
                soilLayerArray[i] -> PutTemperature(theClimate -> GetNextSoilTemperature());
            }
        }
    } else {
        int         i;
        soilLayer * currentLayer;
        double      deltaTime   = 0.0;
        double      currentTime = 0.0;
        double      moreFrozen[MaxSoilLayers];
        double      waterFlux[MaxSoilLayers], waterFluxMean[MaxSoilLayers], waterFluxSlope[MaxSoilLayers];
        double      heatConductivity[MaxSoilLayers], heatConductivityMean[MaxSoilLayers],
                    heatConductivitySlope[MaxSoilLayers], SpecWaterCapacity[MaxSoilLayers];
        double UpperVector[MaxSoilLayers];
        double MidVector[MaxSoilLayers];
        double LowerVector[MaxSoilLayers];
        double TOldArray[MaxSoilLayers];
        double TNew[MaxSoilLayers];
        double TOld[MaxSoilLayers];
        double A[MaxSoilLayers];
        double B[MaxSoilLayers];
        double Ka[MaxSoilLayers];
        double Kb[MaxSoilLayers];
        double Kc[MaxSoilLayers];
        bool   frostChange[MaxSoilLayers];
        double TLB, TUB;
        bool   reduceTimestep = false;
        double averageThermalCond;
        double averageHeatCapacity;
        double WaterInFlux         = WaterDensity * infiltration / 86400 / 1000;                  // unit kg m-2 s-1
        int    numberOfTotalLayers = NumOfSoilLayers + ExtraLayers;

        for (i = 0; i < numberOfTotalLayers; i++) {
            moreFrozen[i] = 0.0;
            waterFlux[i]  = WaterDensity * soilLayerArray[i] -> GetWaterFlux() / 86400 / 1000;    // unit kg m-2 s-1

            if (fabs(waterFlux[i]) > 0.01) {
                theMessage -> WarningWithDisplay(
                    "soilProfile::UpdateFinDiffTemperature waterflux is larger than 0.01 kg m-2 s-1");
            }

            if (waterFlux[i] < 0.0) {
                theMessage -> WarningWithDisplay("soilProfile::UpdateFinDiffTemperature waterflux is negative");
            } else if (waterFlux[i] > 10) {
                theMessage -> WarningWithDisplay("soilProfile::UpdateFinDiffTemperature - Waterflux unrealistic high");
            }
        }

        waterFluxMean[0] = (WaterInFlux + waterFlux[0]) * 0.5;

        for (i = 1; i < numberOfTotalLayers - 1; i++) {
            waterFluxMean[i] = (waterFlux[i - 1] + waterFlux[i]) * 0.5;
        }

        waterFluxMean[numberOfTotalLayers - 1] = (waterFlux[numberOfTotalLayers - 1]
                + waterFlux[numberOfTotalLayers - 2]) * 0.5;

        // calculate the slope of the waterfluxes
        waterFluxSlope[0] = (waterFlux[0] - WaterInFlux) * DistPreviousLayerInv[0];

        for (i = 1; i < numberOfTotalLayers - 1; i++) {
            waterFluxSlope[i] = (waterFlux[i] - waterFlux[i - 1]) * DistPreviousLayerInv[i];
        }

        waterFluxSlope[numberOfTotalLayers - 1] = (waterFlux[numberOfTotalLayers - 1]
                - waterFlux[numberOfTotalLayers - 2]) * DistPreviousLayerInv[numberOfTotalLayers - 1];

        // get new LB and UB conditions
        // LB from analytical solution (as in DAISY)
        double maxDepth = (GetMaximumDepth() + soilLayerArray[numberOfTotalLayers - 2] -> GetThickness()
                           + soilLayerArray[numberOfTotalLayers - 1] ->
                                GetThickness()) / 1000;    // change to actual depth!!!!!!!!!!!!!!!!!!!!!!!

        if (calculateAverage) {
            averageThermalCond  = 0.0;
            averageHeatCapacity = 0.0;

            for (i = 0; i < numberOfTotalLayers; i++) {
                averageThermalCond  += soilLayerArray[i] -> HeatConductivity() * GetLayerThickness(i) / 1000 / maxDepth;
                averageHeatCapacity += soilLayerArray[i] -> HeatCapacity() * GetLayerThickness(i) / 1000 / maxDepth;
            }

            calculateAverage = false;
            depthDampning    = pow(2 * averageThermalCond / (averageHeatCapacity * 0.0172142 / (3600 * 24)),
                                   0.5);    // approximately 2.4
        }

        double dayInYear    = theTime.GetDayInYear();
        double daysFromZero = dayInYear - dayZero;

        TLB = averageAirTemp
              + amplitudeAirTemp * exp(-maxDepth / depthDampning)
                * cos(0.0172142 * (daysFromZero) -maxDepth / depthDampning);

        if (snow > 0.0) {
            if (AirTemperature > 0.0) {    // snow contains liquid water
                TUB = 0.0;
            } else {                       // no water in snowpack
                TUB = (AirTemperature * snowConductivity / (snow / 1000.0)
                       + temperature[0] * soilLayerArray[0] -> HeatConductivity()
                         * DistPreviousLayerInv[0]) / (snowConductivity / (snow / 1000.0)
                             + soilLayerArray[0] -> HeatConductivity() * DistPreviousLayerInv[0]);
            }
        } else {
            TUB = AirTemperature;
        }

        // Initialise starting condition
        for (i = 0; i < numberOfTotalLayers; ++i) {
            TNew[i] = temperature[i];

            if (temperature[i] < 0.0) {
                reduceTimestep = true;
            }

            if (soilLayerArray[i] -> GetIce() > 0.0) {
                reduceTimestep = true;
            }
        }

        if (AirTemperature < 0.0) {
            reduceTimestep = true;
        }

        deltaTime = 3600;       // timestep  1 hour

        if (reduceTimestep) {
            deltaTime = 100;    // timestep 5 min
        }

        bool firstTime = true;

        // simulate 24 hours
        while (currentTime < 86400) {
            for (i = 0; i < numberOfTotalLayers; i++) {

                // transfer of new temperature to old temperature
                TOld[i]        = TNew[i];
                frostChange[i] = fabs(moreFrozen[i]) > 1E-30;                          // True if change in icecontent

                // calculate heatconductivity etc.
                if (frostChange[i] || firstTime) {
                    heatConductivity[i]  = soilLayerArray[i] -> HeatConductivity();
                    heatCapacity[i]      = soilLayerArray[i] -> HeatCapacity();
                    SpecWaterCapacity[i] = soilLayerArray[i] -> SpecWaterCapacity();
                }
            }

            for (i = 0; i < numberOfTotalLayers; i++) {    // calculates the slope of the heatconductivity
                if (frostChange[i] || firstTime) {
                    if (i == 0) {
                        heatConductivitySlope[i] = (0.025
                                                    + (heatConductivity[i + 1] - heatConductivity[i])
                                                      * DistNextLayerInv[i]) * 0.5;    // mean between soil and air
                        heatConductivityMean[i] = (heatConductivity[i + 1] + heatConductivity[i]) * 0.5;
                    } else if (i == (numberOfTotalLayers - 1)) {
                        heatConductivitySlope[i] = (heatConductivity[i] - heatConductivity[i - 1])
                                                   * DistPreviousLayerInv[i];
                        heatConductivityMean[i] = heatConductivity[i];
                    } else {
                        heatConductivitySlope[i] = ((heatConductivity[i + 1] - heatConductivity[i])
                                                    * DistNextLayerInv[i] + (heatConductivity[i]
                                                        - heatConductivity[i - 1]) * DistPreviousLayerInv[i]) * 0.5;
                        heatConductivityMean[i] = (heatConductivity[i + 1] + heatConductivity[i]) * 0.5;
                    }
                }
            }

            for (i = 0; i < numberOfTotalLayers; i++) {
                if (frostChange[i] || firstTime) {
                    currentLayer = soilLayerArray[i];

                    if (((TNew[i] < 0.0) && (currentLayer -> GetIce() < currentLayer -> GetTotalWater())) ||    // freezing
                            ((TNew[i] > 0.0) && (currentLayer -> GetIce() > 0.0)))                              // thawing
                            {
                        A[i] = specCapacity * specCapacity / 273.0 * SpecWaterCapacity[i]
                               + currentLayer -> HeatCapacity();
                        B[i] = specCapacity * waterFluxSlope[i];
                    } else {
                        A[i] = currentLayer -> HeatCapacity();
                        B[i] = 0;
                    }

                    Ka[i] = A[i] / deltaTime;
                    Kb[i] = heatConductivityMean[i] / (distanceToPreviousLayer[i] + distanceToNextLayer[i]);
                    Kc[i] = (heatConductivitySlope[i] - HeatCapacityWater * waterFluxMean[i])
                            / (2 * (distanceToPreviousLayer[i] + distanceToNextLayer[i]));
                }
            }

            // Initialization of diagonal vectors
            for (i = 0; i < numberOfTotalLayers; ++i) {
                if (frostChange[i] || firstTime) {
                    UpperVector[i]                       = -Kb[i] * DistNextLayerInv[i] - Kc[i];        // c
                    MidVector[i]                         = Ka[i]
                            + Kb[i] * (DistNextLayerInv[i] + DistPreviousLayerInv[i]);                  // b
                    LowerVector[i]                       = -Kb[i] * DistPreviousLayerInv[i] + Kc[i];    // a
                    LowerVector[0]                       = 0.0;
                    UpperVector[numberOfTotalLayers - 1] = 0.0;

                    if (MidVector[0] == 0.0) {
                        theMessage -> WarningWithDisplay(
                            "soilProfile::UpdateFinDiffTemperature - Error 1 in tridag vectors");
                    }
                }
            }

            // Initialization of vector with old values
            i            = 0;
            TOldArray[i] = Ka[i] * TOld[i]
                           + Kb[i]
                             * ((TOld[i + 1] - TOld[i]) * DistNextLayerInv[i]
                                - (TOld[i] - TUB) * DistPreviousLayerInv[i]) + Kc[i] * (TOld[i + 1] - TUB) - B[i]
                                    - (-Kb[i] * DistPreviousLayerInv[i] + Kc[i]) * TUB;

            for (i = 1; i < numberOfTotalLayers - 1; ++i) {
                TOldArray[i] = Ka[i] * TOld[i]
                        + Kb[i]
                            * ((TOld[i + 1] - TOld[i]) * DistNextLayerInv[i]
                                - (TOld[i] - TOld[i - 1]) * DistPreviousLayerInv[i]) + Kc[i] * (TOld[i + 1] - TOld[i - 1]) - B[i];
            }

            TOldArray[numberOfTotalLayers - 1] = Ka[numberOfTotalLayers - 1] * TOld[numberOfTotalLayers - 1]
                    + Kb[numberOfTotalLayers - 1]
                      * ((TLB - TOld[numberOfTotalLayers - 1]) * DistNextLayerInv[numberOfTotalLayers - 1]
                         - (TOld[numberOfTotalLayers - 1] - TOld[numberOfTotalLayers - 2])
                           * DistPreviousLayerInv[numberOfTotalLayers - 1]) + Kc[numberOfTotalLayers - 1] * (TLB - TOld[numberOfTotalLayers - 2]) - B[numberOfTotalLayers - 1] + (Kb[numberOfTotalLayers - 1] * DistNextLayerInv[numberOfTotalLayers - 1] + Kc[numberOfTotalLayers - 1]) * TLB;

            // Solve with the double sweep method (function tridag)
            Tridag(LowerVector, MidVector, UpperVector, TOldArray, TNew, numberOfTotalLayers);

            // update IceContent
            for (int i = 0; i < numberOfTotalLayers; i++) {
                currentLayer = soilLayerArray[i];

                // is there freezing or thawing
                if (((TNew[i] < 0.0) && (currentLayer -> GetIce() < currentLayer -> GetTotalWater())) ||    // freezing
                        ((TNew[i] > 0.0) && (currentLayer -> GetIce() > 0.0))) {                            // thawing
                    double TimeDerivTemp = (TNew[i] - TOld[i]) / deltaTime;
                    double TimeDerivIce  = WaterDensity / IceDensity
                                           * (-1.0 / WaterDensity * waterFluxSlope[i]
                                              - (specCapacity * SpecWaterCapacity[i] / 273.0 * TimeDerivTemp));

                    moreFrozen[i] = deltaTime * TimeDerivIce * currentLayer -> GetThickness();    // in mm

                    if (moreFrozen[i] + currentLayer -> GetIce()
                            > currentLayer -> GetTotalWater()) {    // frozen water exceeds water
                        TNew[i]       = -0.001;
                        moreFrozen[i] = currentLayer -> GetTotalWater() - currentLayer -> GetIce();
                    } else if (moreFrozen[i] + currentLayer -> GetIce() < 0.0) {    // frozen water negative
                        TNew[i]       = 0.001;
                        moreFrozen[i] = -currentLayer -> GetIce();
                    }

                    double ice = currentLayer -> GetIce();

                    currentLayer -> SetIceContent(max(0.0, min(currentLayer -> GetTotalWater(), ice + moreFrozen[i])));
                    CheckCoherence(-90);
                } else {
                    moreFrozen[i] = 0.0;
                }

                if (TNew[i] > 50) {
                    theMessage -> WarningWithDisplay(
                        "soilProfile::UpdateFinDiffTemperature - soil layer temperature above realistic temperature");
                } else if (TNew[i] < -50) {
                    theMessage -> WarningWithDisplay(
                        "soilProfile::UpdateFinDiffTemperature - soil layer temperature below realistic value");
                }
            }

            currentTime += deltaTime;
            firstTime   = false;
        }

        for (int i = 0; i < numberOfTotalLayers; i++) {
            temperature[i] = TNew[i];
        }

        // check if the temperature is witin realistic boundaries
        int month = theTime.GetMonth();
        int year  = theTime.GetYear();

        for (int i = 0; i < numberOfTotalLayers; i++) {
            if ((i > 9) && (i < 19) && (temperature[i] < minTemp[month - 1]) && (year != lastErrorYear)) {
                lastErrorYear = year;

                cout << "Layer no " << i << " temperature is " << temperature[i];

                theMessage -> WarningWithDisplay(
                    "soilProfile::UpdateFinDiffTemperature - soil layer temperature below expert opinion monthly threshold");
            }

            if (temperature[i] > 100) {
                theMessage -> FatalError(
                    "soilProfile::UpdateFinDiffTemperature - soil layer temperature above boiling point");
            } else if (temperature[i] > 50) {
                theMessage -> WarningWithDisplay(
                    "soilProfile::UpdateFinDiffTemperature - soil layer temperature above realistic temperature");
            } else if (temperature[i] < -50) {
                theMessage -> FatalError(
                    "soilProfile::UpdateFinDiffTemperature - soil layer temperature below possible value");
            }

            soilLayerArray[i] -> PutTemperature(temperature[i]);
        }

        if (theControlParameters -> GetWriteSoilTemperature()) {
            SoilTempFile << theTime;

            for (int i = 0; i < numberOfTotalLayers; i++) {
                SoilTempFile << "\t" << temperature[i];
            }

            SoilTempFile << endl;
        }
    }
}

/*
 *
 * Solves for a vector u[0..n-1] the tridiagonal linear set. The input vectors
 * a,b,c and r are unmodified, and u contains the result
 * Slightly modified after Numerical Recipies in C (Press, 1992), main modification
 * is shift from [1..n] to [0..n-1], in concordance with mainstream C++ conventions.
 * Used to calculate the Finite difference solution for temperature.
 * \
 */
void soilProfile::Tridag(double        a[],
                         double        b[],
                         double        c[],
                         double        r[],
                         double        u[],
                         unsigned long n) {
    unsigned long j;
    double        bet, *gam;

    gam = new double[n];    //

    if (0 == b[0]) {
        theMessage -> FatalError("Error 1 in function Tridag");
    }

    bet  = b[0];
    u[0] = r[0] / bet;

    for (j = 1; j < n; j++) {             // Decomposition and forward substitution
        gam[j] = c[j - 1] / bet;          // e[j]
        bet    = b[j] - a[j] * gam[j];    // g[j]

        if (0 == bet) {
            theMessage -> FatalError("Error 2 in function Tridag");
        }

        u[j] = (r[j] - a[j] * u[j - 1]) / bet;
    }

    for (j = n - 2; j + 1 > 0; j--) {
        u[j] -= gam[j + 1] * u[j + 1];    // Backsubstitution
    }

    delete[] gam;
}

/*
 *
 * Updates organic matter pools in profile.
 * \
 */
double soilProfile::UpdateOrganicMatter() {
    soilLayer * current      = first;
    double      CO2Evolution = 0.0;

    while (current) {
        CO2Evolution += current -> UpdateOrganicMatter();
        current      = current -> Next();
    }

    return CO2Evolution;
}

/*
 * Performs nitrification and updates contents of ammonia and nitrate.
 * returns       - Nitrification [g N/m�/d]
 */
nitrogen soilProfile::UpdateNitrification() {
    nitrogen    sum;
    soilLayer * current = first;

    while (current) {
        sum     = sum + current -> UpdateNitrification();
        current = current -> Next();
    }

    return sum;
}

/*
 * Performs denitrification and updates contents of nitrate.
 * //  returns       - Nitrification [g N/m�/d]
 */
nitrogen soilProfile::UpdateDenitrification(double TotalCO2Emission) {
    nitrogen    sum;
    soilLayer * current = first;

    while (current) {
        current -> UpdateN2OFromDenitrification(TotalCO2Emission);

        sum     = sum + current -> UpdateN2Production();
        current = current -> Next();
    }

    return sum;
}

nitrogen soilProfile::GetAmmonium(double startDep,
                                  double thick) {
    nitrogen    Content;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetAmmonium(startDep, thick);
        current = current -> Next();
    }

    return Content;
}

double soilProfile::GetAmmoniumN(double startDep,
                                 double thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetAmmoniumN(startDep, thick);
        current = current -> Next();
    }

    return Content;
}

double soilProfile::GetAmmoniumN15(double startDep,
                                   double thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetAmmoniumN15(startDep, thick);
        current = current -> Next();
    }

    return Content;
}

nitrogen soilProfile::GetNitrate(double startDep,
                                 double thick) {
    nitrogen    Content;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetNitrate(startDep, thick);
        current = current -> Next();
    }

    if (Content.n < -1e6) {
        theMessage -> FatalError("soilProfile::GetNitrate - returns negative value");
    }

    return Content;
}

double soilProfile::GetNitrateN(double startDep,
                                double thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetNitrateN(startDep, thick);
        current = current -> Next();
    }

    if (Content < -1e6) {
        theMessage -> FatalError("soilProfile::GetNitrate - returns negative value");
    }

    return Content;
}

double soilProfile::GetNitrateN15(double startDep,
                                  double thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetNitrateN15(startDep, thick);
        current = current -> Next();
    }

    if (Content < -1e6) {
        theMessage -> FatalError("soilProfile::GetNitrate - returns negative value");
    }

    return Content;
}

nitrogen soilProfile::GetN2OFromNitrification() {
    nitrogen    Content;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetN2OFromNitrification();
        current = current -> Next();
    }

    return Content;
}

nitrogen soilProfile::GetN2OFromDenitrification() {
    nitrogen    Content;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetN2OFromDenitrification();
        current = current -> Next();
    }

    return Content;
}

double soilProfile::GetChloride(double startDep,
                                double thick) {
    double      Content = 0.0;
    soilLayer * current = first;

    while (current) {
        Content += current -> GetChloride(startDep, thick);
        current = current -> Next();
    }

    if (Content < -1e6) {
        theMessage -> FatalError("soilProfile::GetChloride - returns negative value");
    }

    return Content;
}

nitrogen soilProfile::GetAmmonium() {
    nitrogen    Content;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetAmmonium();
        current = current -> Next();
    }

    if (Content.n < 0.0) {
        theMessage -> FatalError("soilProfile::GetAmmonium - returns negative value");
    }

    return Content;
}

double soilProfile::GetAmmoniumN() {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetAmmoniumN();
        current = current -> Next();
    }

    if (Content < 0.0) {
        theMessage -> FatalError("soilProfile::GetAmmonium - returns negative value");
    }

    return Content;
}

nitrogen soilProfile::GetNitrate() {
    nitrogen    Content;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetNitrate();
        current = current -> Next();
    }

    return Content;
}

double soilProfile::GetNitrateN() {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetNitrateN();
        current = current -> Next();
    }

    return Content;
}

int soilProfile::GetNumberOfLayers() {
    int         n       = 0;
    soilLayer * current = first;

    while (current) {
        n++;

        current = current -> Next();
    }

    return n;
}

double soilProfile::GetLayerThickness(int No) {
    int         n       = 0;
    double      thick   = 0.;
    soilLayer * current = first;

    while (current) {
        if (n == No) {
            thick = current -> GetThickness();
        }

        n++;

        current = current -> Next();
    }

    return thick;
}

double soilProfile::GetCarbon() {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content += current -> GetCarbon();
        current = current -> Next();
    }

    return Content;
}

nitrogen soilProfile::GetOrganicNitrogen() {
    nitrogen    Content;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetOrganicNitrogen();
        current = current -> Next();
    }

    if (Content.n < 0.0) {
        theMessage -> FatalError("soilProfile::GetOrganicNitrogen - returns negative value");
    }

    return Content;
}

double soilProfile::GetOrganicNitrogenN() {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetOrganicNitrogenN();
        current = current -> Next();
    }

    if (Content < 0.0) {
        theMessage -> FatalError("soilProfile::GetOrganicNitrogen - returns negative value");
    }

    return Content;
}

nitrogen soilProfile::GetOrganicNitrogen(double startDep,
        double                                  thick) {
    nitrogen    Content;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetOrganicNitrogen(startDep, thick);
        current = current -> Next();
    }

    if (Content.n < 0.0) {
        theMessage -> FatalError("soilProfile::GetOrganicNitrogen - returns negative value");
    }

    return Content;
}

double soilProfile::GetOrganicNitrogenN15(double startDep,
        double                                   thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetOrganicNitrogenN15(startDep, thick);
        current = current -> Next();
    }

    if (Content < 0.0) {
        theMessage -> FatalError("soilProfile::GetOrganicNitrogen - returns negative value");
    }

    return Content;
}

double soilProfile::GetOrganicNitrogenN(double startDep,
        double                                 thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetOrganicNitrogenN(startDep, thick);
        current = current -> Next();
    }

    if (Content < 0.0) {
        theMessage -> FatalError("soilProfile::GetOrganicNitrogen - returns negative value");
    }

    return Content;
}

double soilProfile::GetOrganicCarbon(double startDep,
        double                              thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content += current -> GetOrganicCarbon(startDep, thick);
        current = current -> Next();
    }

    return Content;
}

double soilProfile::GetSoilMass(double startDep,
                                double thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content += current -> GetSoilMass(startDep, thick);
        current = current -> Next();
    }

    return Content;
}

double soilProfile::GetClayContent(double startdepth,
                                   double thick) {
    double      Content     = 0.0;
    double      TotalWeight = 0.0;
    soilLayer * current     = first;

    while (current) {
        double top = current -> GetStartDepth();
        double th  = current -> GetThickness();

        if ((top + th) > startdepth && (top < (startdepth + thick))) {    // Test if within range
            double slice = th;

            if (top < startdepth) {
                slice = max(0.0, top + th - startdepth);
            }

            if ((top + th) > (startdepth + thick)) {
                slice = max(0.0, startdepth + thick - top);
            }

            TotalWeight += slice * current -> GetDryBulkDensity();
            Content     += slice * current -> GetDryBulkDensity() * current -> GetClayContent();
        }

        current = current -> Next();
    }

    Content = Content / TotalWeight;

    return Content;
}

double soilProfile::GetPoolCarbon(int    Name,
                                  double startDep,
                                  double thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content += current -> GetPoolCarbon(Name, startDep, thick);
        current = current -> Next();
    }

    return Content;
}

double soilProfile::GetPoolNitrogenN15(int Name,
        double                             startDep,
        double                             thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetPoolNitrogenN15(Name, startDep, thick);
        current = current -> Next();
    }

    return Content;
}

double soilProfile::GetPoolNitrogenN(int Name,
        double                           startDep,
        double                           thick) {
    double      Content = 0;
    soilLayer * current = first;

    while (current) {
        Content = Content + current -> GetPoolNitrogenN(Name, startDep, thick);
        current = current -> Next();
    }

    return Content;
}

nitrogen soilProfile::GetTotalNitrogen() {
    nitrogen TotalN = GetAmmonium() + GetNitrate() + GetOrganicNitrogen();

    return TotalN;
}

double soilProfile::GetTotalNitrogenN() {
    double TotalN = GetAmmoniumN() + GetNitrateN() + GetOrganicNitrogenN();

    return TotalN;
}

nitrogen soilProfile::GetAmmoniumLeaching(double depth) {
    nitrogen    N;
    bool        found   = false;
    soilLayer * current = first;

    while (!found && (current)) {
        if ((depth >= current -> GetStartDepth())
                && (depth <= current -> GetStartDepth() + current -> GetThickness())) {
            N     = current -> GetAmmoniumLeaching();
            found = true;
        }

        current = current -> Next();
    }

    if (N.n < -1e15) {
        theMessage -> WarningWithDisplay("SoilProfile: negative ammonium leaching");
    }

    return N;
}

nitrogen soilProfile::GetNitrateLeaching(double depth) {
    nitrogen    N;
    bool        found   = false;
    soilLayer * current = first;

    while (!found && (current)) {
        if ((depth >= current -> GetStartDepth())
                && (depth <= current -> GetStartDepth() + current -> GetThickness())) {
            N     = current -> GetNitrateLeaching();
            found = true;
        }

        current = current -> Next();
    }

    if (N.n < -1e15) {
        theMessage -> WarningWithDisplay("SoilProfile: negative nitrate leaching");
    }

    return N;
}

void soilProfile::AddProduct(const organicProduct & product,
                             double *               rootLengthList) {
    if (rootLengthList) {                                          // Perform only if a root distribution is defined
        int      n    = GetNumberOfLayers();
        double * L    = rootLengthList;
        double   sumL = 0.;

        for (int i = 0; i < n; i++) {
            sumL += *(L++);
        }

        soilLayer *      current = first;
        organicProduct * prod    = (organicProduct *) &product;    // To avoid warning
        organicProduct * p;

        L = rootLengthList;

        int k = 0;                                                 // Remove later !!!

        while (current) {
            if (*L > 0) {
                p = prod -> GetFraction((*L) / sumL);

                current -> AddProduct(*p);

                delete p;
            }

            current = current -> Next();

            L++;
            k++;
        }
    } else {
        theMessage -> FatalError("SoilProfile::Product not added");
    }
}

void soilProfile::StartBudget() {
    soilLayer * current = first;

    while (current) {
        current -> StartBudget();

        current = current -> Next();
    }
}

void soilProfile::EndBudget() {
    soilLayer * current = first;

    while (current) {
        current -> EndBudget();

        current = current -> Next();
    }
}

string soilProfile::IntToName(int name) {
    string names;

    switch (name) {
        case 0 :
            names = "AOM1";

            break;

        case 1 :
            names = "AOM2";

            break;

        case 2 :
            names = "SMB1";

            break;

        case 3 :
            names = "SMB2";

            break;

        case 4 :
            names = "SMR";

            break;

        case 5 :
            names = "NOM";

            break;
    }

    return names;
}

void soilProfile::WriteCarbon(int  Name,
                              bool usedBefores) {
    soilLayer *     current = first;
    static ofstream Cconent;

    if (!usedBefores) {
        Cconent.open((globalSettings -> getOutputDirectory() + IntToName(Name) + "_C.xls").c_str(), ios::out);

        Cconent << theTime << "\t";

        while (current) {
            double Content = 0;

            Content = current -> GetStartDepth();

            Cconent << Content << "\t";

            current = current -> Next();
        }

        current = first;

        Cconent << flush;
        Cconent << endl;
        Cconent << theTime << '\t';

        while (current) {
            current = current -> Next();
        }

        current = first;

        Cconent << flush;
        Cconent << endl;
        Cconent << theTime << '\t';
    } else {
        Cconent.open((globalSettings -> getOutputDirectory() + IntToName(Name) + "_C.xls").c_str(), ios::app);

        Cconent << theTime << "\t";
    }

    int i = 0;

    while (current) {
        i++;

        double Content = 0;

        Content = current -> GetPoolCarbon(Name);

        Cconent << Content << "\t";

        current = current -> Next();
    }

    Cconent << flush;
    Cconent << endl;

    Cconent.close();
}

void soilProfile::WriteN(int  Name,
                         bool usedBefores) {
    soilLayer *     current = first;
    static ofstream Cconent;

    if (!usedBefores) {
        Cconent.open((globalSettings -> getOutputDirectory() + IntToName(Name) + "_N.xls").c_str(), ios::out);

        Cconent << theTime << "\t";

        while (current) {
            double Content = 0;

            Content = current -> GetStartDepth();

            Cconent << Content << "\t";

            current = current -> Next();
        }

        current = first;

        Cconent << flush;
        Cconent << endl;
        Cconent << theTime << '\t';

        while (current) {
            current = current -> Next();
        }

        current = first;

        Cconent << flush;
        Cconent << endl;
        Cconent << theTime << '\t';
    } else {
        Cconent.open((globalSettings -> getOutputDirectory() + IntToName(Name) + "_N.xls").c_str(), ios::app);

        Cconent << theTime << "\t";
    }

    while (current) {
        double Content = 0;

        Content = current -> GetPoolNitrogen(Name).n;

        Cconent << Content << "\t";

        current = current -> Next();
    }

    Cconent << flush;
    Cconent << endl;

    Cconent.close();
}

void soilProfile::WriteDecompositionRate(int Name,
        bool                                 usedBefores) {
    soilLayer *     current = first;
    static ofstream Cconent;

    if (!usedBefores) {
        Cconent.open((globalSettings -> getOutputDirectory() + IntToName(Name) + "_DecompositionRate.xls").c_str(),
                     ios::out);

        Cconent << theTime << "\t";

        while (current) {
            double Content = 0;

            Content = current -> GetStartDepth();

            Cconent << Content << "\t";

            current = current -> Next();
        }

        current = first;

        Cconent << flush;
        Cconent << endl;
        Cconent << theTime << '\t';

        while (current) {
            current = current -> Next();
        }

        current = first;

        Cconent << flush;
        Cconent << endl;
        Cconent << theTime << '\t';
    } else {
        Cconent.open((globalSettings -> getOutputDirectory() + IntToName(Name) + "_DecompositionRate.xls").c_str(),
                     ios::app);

        Cconent << theTime << "\t";
    }

    while (current) {
        double Content = 0;

        Content = current -> GetPoolDecompositionRate(Name);

        Cconent << Content << "\t";

        current = current -> Next();
    }

    Cconent << flush;
    Cconent << endl;

    Cconent.close();
}

void soilProfile::WriteCN(int  Name,
                          bool usedBefores) {
    soilLayer *     current = first;
    static ofstream Cconent;

    if (!usedBefores) {
        Cconent.open((globalSettings -> getOutputDirectory() + IntToName(Name) + "_CN.xls").c_str(), ios::out);

        Cconent << theTime << "\t";

        while (current) {
            double Content = 0;

            Content = current -> GetStartDepth();

            Cconent << Content << "\t";

            current = current -> Next();
        }

        current = first;

        Cconent << flush;
        Cconent << endl;
        Cconent << theTime << '\t';

        while (current) {
            current = current -> Next();
        }

        current = first;

        Cconent << flush;
        Cconent << endl;
        Cconent << theTime << '\t';
    } else {
        Cconent.open((globalSettings -> getOutputDirectory() + IntToName(Name) + "_CN.xls").c_str(), ios::app);

        Cconent << theTime << "\t";
    }

    while (current) {
        double Content = 0;

        Content = current -> GetPoolCarbon(Name) / current -> GetPoolNitrogen(Name).n;

        Cconent << Content << "\t";

        current = current -> Next();
    }

    Cconent << flush;
    Cconent << endl;

    Cconent.close();
}

void soilProfile::WriteC() {
    soilLayer *     current = first;
    static ofstream Cconent;
    static bool     usedBefore = false;

    if (!usedBefore) {
        usedBefore = true;

        Cconent.open((globalSettings -> getOutputDirectory() + "Ccontent.xls").c_str(), ios::out);

        Cconent << theTime << "\t";

        while (current) {
            double Content = 0;

            Content = current -> GetStartDepth();

            Cconent << Content << "\t";

            current = current -> Next();
        }

        current = first;

        Cconent << flush;
        Cconent << endl;
        Cconent << theTime << '\t';
    } else {
        Cconent.open((globalSettings -> getOutputDirectory() + "Ccontent.xls").c_str(), ios::app);

        Cconent << theTime << "\t";
    }

    while (current) {
        double Content = 0;

        Content = current -> GetCarbon();

        Cconent << Content << "\t";

        current = current -> Next();
    }

    Cconent << flush;
    Cconent << endl;

    Cconent.close();
}

void soilProfile::changeLayer(int starLayer,
                              int endLayer) {
    soilLayer *         current    = first;
    soilLayer *         firstLayer = nullptr;
    soilLayer *         LastLayer  = nullptr;
    vector<soilLayer *> layser;
    int                 start = 0;

    while (current) {
        if ((starLayer - 1) == start) {
            firstLayer = current;
        }

        if ((start >= starLayer) && (start <= endLayer)) {
            layser.push_back(current);
        }

        if (start == endLayer + 1) {
            LastLayer = current;
        }

        start++;

        current = current -> Next();
    }

    for (unsigned int i = 1; i <= (layser.size() - 1); i++) {
        layser.at(i) -> AddNext(layser.at(i - 1));
    }

    if (starLayer != 0) {
        firstLayer -> AddNext(layser.back());
    } else {
        first = layser.back();
    }

    layser[0] -> AddNext(LastLayer);

    current = first;

    int    i          = 0;
    double startDepth = 0;

    while (current) {
        current -> SetStartDepth(startDepth);

        startDepth        += current -> GetThickness();
        soilLayerArray[i] = current;
        current           = current -> Next();

        i++;
    }
}

void soilProfile::BeforeAmmonium() {
    soilLayer * current = first;

    while (current) {
        current -> BeforeAmmonium();

        current = current -> Next();
    }
}

void soilProfile::PrintAmmonium() {
    soilLayer * current = first;
    int         i       = 0;

    while (current) {
        current -> PrintAmmonium(i);

        current = current -> Next();

        i++;
    }
}

void soilProfile::PrintNitrate() {
    soilLayer * current = first;
    int         i       = 0;

    while (current) {
        current -> PrintNitrate(i);

        current = current -> Next();

        i++;
    }
}

void soilProfile::BeforeNitrate() {
    soilLayer * current = first;

    while (current) {
        current -> BeforeNitrate();

        current = current -> Next();
    }
}

soilProfile::soilProfile(const soilProfile & Profile):
    base(Profile)
{
    NumOfSoilLayers     = Profile.NumOfSoilLayers;
    EvapExtCoef         = Profile.EvapExtCoef;
    EvaporationCapacity = Profile.EvaporationCapacity;
    EvaporationContent  = Profile.EvaporationContent;
    maxRootDepth        = Profile.maxRootDepth;

    for (int i = 0; i < MaxPlants; i++) {
        nitrogenUptake[i].Clear();
    }

    int i = 0;

    if (&Profile) {
        soilLayer * current;
        soilLayer * tempLayer;

        current = Profile.first;

        while (current) {
            tempLayer = new soilLayer(*current);

            AddItem(tempLayer);

            soilLayerArray[i] = tempLayer;

            soilLayerArray[i] -> SetOwner(this);

            current = current -> Next();

            i++;
        }

        for (int i = 0; i < ExtraLayers; i++) {                   // extra layers
            soilLayerArray[NumOfSoilLayers + i] = new soilLayer(
                *Profile.soilLayerArray[NumOfSoilLayers + i]);    // extraLayer

            soilLayerArray[NumOfSoilLayers + i] -> SetOwner(this);
        }

        for (int i = 0; i < (NumOfSoilLayers + ExtraLayers); i++) {
            distanceToPreviousLayer[i] = Profile.distanceToPreviousLayer[i];
            distanceToNextLayer[i]     = Profile.distanceToNextLayer[i];
            temperature[i]             = Profile.temperature[i];
        }
    }
}

soilProfile::~soilProfile() {
    if (NumOfSoilLayers > 0) {    // Otherwise never initialised
        for (int i = 0; i < (NumOfSoilLayers + ExtraLayers); i++) {
            delete soilLayerArray[i];
        }
    }

    if (SoilTempFile) {
        SoilTempFile.close();
    }
}
