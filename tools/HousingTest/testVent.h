
#ifndef STABLE_H
#define STABLE_H

#include <fstream>

class testVent
{

    public:
        testVent() {
            //detailsFile           = nullptr;
            controlledVent        = false;
            minTemperature        = maxTemperature = wallArea = 0.0;
            planArea              = planAreaRatio = meanHeight = thermalTransWall = 0.0;
            thermalTransRoof      = ventilationRate = insideTemperature = 0.0;
            apertureWidth         = maxapertureHeight = 0.0;
            minPropApertureHeight = absorbCoeff = emissivity = 0.0;
            externSurfResis       = targetTemperature = minVentilation = 0.0;
            maxVentilation        = optimumAirVelocity = 0.0;
			waterEvap=0;
        }
        testVent(bool acontrolledVent, 
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
			double aoptimumAirVelocity) ;
private:
		//!minimum desirable temperature (Kelvin)
        double minTemperature;
		//!maximum desirable temperature (Kelvin)
        double maxTemperature;
		//! Water evaporation (kg/s)
		double waterEvap;

        // !area of all walls, assume housing in cubic
        double wallArea;

        // ! plan area of house (sq metres)
        double planArea;

        // ! area of house relative to the total area of sections
        double planAreaRatio;

        // !mean height of house (metres)
        double meanHeight;

        // !thermal transmissivity of wall (W per sq metre per Kelvin)
        double thermalTransWall;

        // !thermal transmissivity of roof (W per sq metre per Kelvin)
        double thermalTransRoof;

        // !true if the housing has controlled ventilation
        int controlledVent;

        // ! ventilation rate (cubic metres per second)
        double ventilationRate;

        // !Inside temperature in degrees Celsius
        double insideTemperature;

        // ! following params are only for wind-forced ventilation
        // !max average width of aperture to outside (m) - uncontrolled ventilation
        double apertureWidth;

        // !max average height of aperture to outside (m) - uncontrolled ventilation
        double maxapertureHeight;

        // ! assume that the ventilation cannot be choked off completely. This is the minimum closure, as a proportion of the maximum height
        double minPropApertureHeight;

        // !absorption coefficient for solar radiation (roof only)
        double absorbCoeff;

        // ! emissivity of roof for long wave radiation
        double emissivity;

        // ! surface resistence of roof
        double externSurfResis;

        // !Target inside temperature
        double targetTemperature;

        // !min ventilation rate in cubic metres per sec
        double minVentilation;

        // !max ventilation rate in cubic metres per sec
        double maxVentilation;

        // !optimum air velocity in freely-ventilated housing, in metres per sec
        double optimumAirVelocity;

		//!Porportion of max ventilation capacity
		double propVent;
        // !File for detailed output (if activated)
        //fstream * detailsFile;

		//! Mean thermal transmissivity (W per metre square per K)
		double meanThermalTrans;

		//! Total surface area of roof and walls together (sq metres)
		double surfaceArea;

		//!Maximum heating capacity, controlled ventilated housing, Watts
		double maxSupplHeat;

    public:

        virtual ~testVent();

		// !Return the ventilation rate of the animal housing (in cubic metres per second)
        virtual double GetventilationRate() {
            return ventilationRate;
        }

        // ! Handles all daily actions of the stable; should be called by the manager
        /*
         * !
		 * param Atemp outside air temperature (Celsius)
		 * param Awindspeed wind speed (m/s)
		 * param Aradiation solar radation (W)
		 * param Ahumidity relative air humidity (%)
         * param heatOP output of sensible heat by the animals in the housing (in Watts)
         * param thresholdTermalHeatIndex maximum desirable thermal humidity index
         * param thisModelIntCelsius modelled inside temperature (Celsius)
		 * param thisModelVentilation modelled ventilation rate (cubic metres/s)
		 * param thisPropVent modelled ventilation rate (controlled) or aperture opening (freely ventilated) as proportion of maximum possible
		 * param evaporation modelled evaporation of water (kg/s)
         */
        void testVent::calculate(double Atemp, double Awindspeed, double Aradiation, double Ahumidity, 
			double heatOP, double thresholdTermalHeatIndex, double *thisModelIntCelsius, 
			double *thisModelVentilation, double *supplHeat, double *thisPropVent, double *evaporation);

        // !Return a pointer to the named animal section

        // !Returns the temperature inside the building in Celsius
        double GetinsideTemperature() {
            return insideTemperature;
        }


        // !Calculates the total wall area of the animal housing (in square metres)
        virtual void calcWallArea();

        // !Calculates the ventilation and inside temperature of animal housing with forced ventilation systems

        /*
         * !
         * param heatOP output of sensible heat by the animals in the housing (in Watts)
         * param outsideAirTemp outside air temperature in Celsius
         * param supplementaryHeat supplementary heating required to maintain the inside temperature at the target level (in Watts)
         */
        virtual void controlled(double   heatOP,
                                double   outsideAirTemp,
                                double   solarRad,
                                double * supplementaryHeat);

        // !Calculates the ventilation and inside temperature of freely-ventilated housing

        /*
         * !
         * param heatOP output of sensible heat by the animals in the housing (in Watts)
         * param outsideAirTemp outside air temperature in Celsius
         * param windspeed outside windspeed (in meters per second)
         * param solarRad solar radiation in Watts per meter squared
         * param supplementaryHeat supplementary heat required to maintain adequate temperature
		 * param thresholdTermalHeatIndex maximum desirable thermal heat index 
         */
        virtual void uncontrolled(double heatOP,
                                  double outsideAirTemp,
                                  double windspeed,
                                  double solarRad,
                                  double relativeHumidity,
								  double thresholdTermalHeatIndex);

        // !Calculates the ventilation rate of freely ventilated housing

        /*
         * !
         * param airDensity density of the air in kilograms per metre cubed
         * param thermalTrans mean thermal transmissivity of the wall and roof material (in Watts per metre Kelvin)
         * param surfaceArea total surface area (wall and roof combined) in square metres
         * param apertureArea total area of apertures in the wall or roof (in square metres)
         * param windspeed outside windspeed (in meters per second)
         * param outsideAirTemp outside air temperature in Celsius
         * param heatOP output of sensible heat by the animals in the housing (in Watts)
         * param q input of heat via solar radiation of the roof (Watts)
         * param ventilationRate ventilation rate in cubic metres per second
         * param deltaTemper temperature differential between inside and outside
         */
        void CalcFreeVentilation(double   airDensity,
                                 double   thermalTrans,
                                 double   surfaceArea,
                                 double   apertureHeight,
                                 double   windspeed,
                                 double   outsideAirTemp,
                                 double   heatOP,
                                 double   q,
                                 double * ventilationRate,
                                 double * deltaTemper);

        // !
        // !Return net exchange of long wave radiation with a surface (Watts per square metre)

        /*
         * !
         * param skyTemp sky temperature (Kelvin)
         * param surfaceTemp surface temperature (Kelvin)
         * param emissivity emissivity of the surface
         */
        double GetNetLongWave(double skyTemp,
                              double surfaceTemp,
                              double emissivity);

        // !Calculates the minimum ventilation rate (will be overloaded)
        virtual double CalcMinimumVentilationRate() {
            return 0.0;
        }

        // !Calculates the maximum ventilation rate (will be overloaded)
        virtual double CalcMaximumVentilationRate() {
            return 0.0;
        }

        // !Calculate the proportion of the heat energy that is in the sensible form

        /*
         * !
         * param temperature temperature in housing (Celsius)
         */
        virtual double CalcPropSensible(double temperature);


        // !Open the file that will store the details of the animal house variables
        void OpenDetailsFile();

        // !Close the file that will store the details of the animal house state variables
        void CloseDetailsFile();

		//! 
        double GetMaxPermissableTemperature(double thermalHeatIndex,
                double                             relativeHumidity);

		//!return number of international livestock units present in housing
		double GetIntLivestockUnits();

		//!Volume of air in house (cubic metres)
		double GetTotalAirVolume();
		void ZeroIntLivestockUnits();

};
#endif

