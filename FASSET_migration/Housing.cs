using System;

    public class AnimalHousingClass
    {
        double meanWallHeight = 0;  //mean height of walls in metres
        double meanWallLength = 0;  //mean length of walls in metres
        double wallArea = 0.0;    // area of one wall, assume housing is cubic
    double thermalTransWall = 0; //thermal transmissivity of the wall material, W/(square metre K)
    double thermalTransRoof = 0; //thermal transmissivity of the roof material, W/(square metre K)
    double emissivity = 0;     //emissivity coefficient for thermal radiation, dimensionless
    double externSurfResis = 0; //external surface resistence of the roof, square metres K/W
    double absorbCoeff = 0; //proportion of the incident solar energy that is absorbed by the roof
    //Controlled ventilation only
    double planArea = 0.0;    // plan area of house in square metres
    double minVentilation = 0.0;    // min ventilation rate in cubic metres per sec - controlled ventilation only
    double maxVentilation = 0.0;    // max ventilation rate in cubic metres per sec - controlled ventilation only
    double targetTemperature = 0;   //this is the temperature that we would like to maintain - Celsius
    double maxSupplementaryHeat = 0; //maximum heating capacity in house in kW
    //Freely ventilated only
    double minTemperature = 0.0;    //the temperature below which we would not like the temperature to fall - Celsius
    double maxTemperature = 0.0;    //the temperature above which we would not like the temperature to rise - Celsius
    double meanApertureWidth = 0.0;     // the average total width of the ventilation apertures in one wall of the housing, m
    double maxapertureHeight = 0.0; // the average height of the ventilation apertures when fully open, measured from the bottom of the aperture, m
    double minPropApertureHeight = 0.0; //minimum opening of the aperture, as proportion of the maximum opening, dimensionless
    double optimumAirVelocity=0;
    //outputs
    double insideTemperature = 0.0; //inside temperature in Celsius
    double ventilationRate = 0.0;    // ventilation rate (cubic metres per second)

//get access to utility functions
    NixCSharpBits utilities = new NixCSharpBits();
//create instance of a dummy animal, to provide input
    DummyAnimal theCows = new DummyAnimal(1, 100, 650.0, 25.0, 0, 35.0);

//constructor for controlled ventilated housing
    public AnimalHousingClass(double AmeanWallHeight, double AmeanWallLength, double AthermalTransRoof, double AthermalTransWall, double Aemissivity,
        double AexternSurfResis, double AabsorbCoeff, double AtargetTemperature,double AminVentilation, double AmaxVentilation, double AmaxSupplementaryHeat)
    {
        meanWallHeight = AmeanWallHeight;
        meanWallLength = AmeanWallLength;
        thermalTransRoof = AthermalTransRoof;
        thermalTransWall = AthermalTransWall;
        emissivity=Aemissivity;
        externSurfResis = AexternSurfResis;
        absorbCoeff = AabsorbCoeff;
        minVentilation = AminVentilation;
        maxVentilation = AmaxVentilation;
        targetTemperature = AtargetTemperature;
        planArea = Math.Pow(meanWallLength, 2);
        wallArea = 4 * meanWallLength * meanWallHeight;
        maxSupplementaryHeat = AmaxSupplementaryHeat;
    }
    //constructor for freely-ventilated housing
    public AnimalHousingClass(int dumInt, double AmeanWallHeight, double AmeanWallLength, double AthermalTransRoof, double AthermalTransWall, double Aemissivity,
            double AexternSurfResis, double AabsorbCoeff, double AoptimumAirVelocity, double AapertureWidth, double AmaxapertureHeight, double AminPropApertureHeight)
    {
        meanWallHeight = AmeanWallHeight;
        meanWallLength = AmeanWallLength;
        thermalTransRoof = AthermalTransRoof;
        thermalTransWall = AthermalTransWall;
        emissivity = Aemissivity;
        externSurfResis = AexternSurfResis;
        absorbCoeff = AabsorbCoeff;
        optimumAirVelocity = AoptimumAirVelocity;
        meanApertureWidth = AapertureWidth;
        maxapertureHeight = AmaxapertureHeight;
        minPropApertureHeight = AminPropApertureHeight;
        wallArea = 4 * meanWallLength * meanWallHeight;//equation 1.2
        planArea = Math.Pow(meanWallLength, 2);//equation 1.3
    }

    public void ErrorHandling(int errorNo)
    {
        string ErrorString = "";
        string ErrorString1 = "Error - ";
        string ErrorString2 = " Press any key to exit";
        switch (errorNo)
        {
            case 1:
                ErrorString += ErrorString1 + "Aperture width exceeds mean wall length " + ErrorString2;
                Console.WriteLine(ErrorString);
                Console.ReadKey();
                break;
            case 2:
                ErrorString += ErrorString1 + "Minimum temperature exceeds maximum temperature" + ErrorString2;
                Console.WriteLine(ErrorString);
                Console.ReadKey();
                break;
            default: Console.Write("Unknown error");
                Console.ReadKey();
                break;
        }
        System.Environment.Exit(1);
    }
    //From 4th Report of Working Group on Climatization of Animal Houses Heat and moisture production at animal and house levels Editors: Pedersen, S. & Sällvik, K. 2002
    //equation 29
    //returns proportion of livestock heat production that is as sensible heat
/*!
 \param temperature double Temperature inside animal house, either Celsius or Kelvin
*/

    double CalcPropSensible(double temperature)
    {
        if (temperature > 173)
            temperature -= 273.15;
//equation 1.1
        double ret_val = 0.8 - 0.38 * Math.Pow(temperature, 2) / 1000.0; //divide 0.38 * Math.Pow(temperature, 2) by 1000, as this is scaled to 1000W animal
        ret_val *= 0.85;    // assumes wet surfaces, so 0.85
        return ret_val;
    }

    public void Ventilation(int controlledVent)
    {
        //these values should be read as parameters

        //these values should be read as daily inputs
        double Ameantemp = 30.0;
        double Aradiation = 600.0; //W per square metre
        double ArelativeHumidity = 1.0;
        double Awindspeed = 3.0;

        double heatOp = theCows.GetHeatProduction();
        double airVelocity = 0.0;
        double waterVapourPressure = ArelativeHumidity * utilities.GetsaturatedWaterVapourPressure(Ameantemp);

        // !calculate the air velocity, using the appropriate functions for controlled or freely ventilated systems
        if (controlledVent > 0)
        {
            double supplementaryHeat = 0.0;
            controlled(heatOp, Ameantemp, Aradiation, waterVapourPressure, ref supplementaryHeat);
            airVelocity = ventilationRate / planArea;
        }
        else
        {
            uncontrolled(heatOp, Ameantemp, Awindspeed, Aradiation, ArelativeHumidity);
        }
    }

    public void controlled(double heatOp, double outsideAirTemp, double solarRad, double watervapourPressure, ref double supplementaryHeat)
    {
        // !calculate the amount of sensible heat produced in the animal housing
        // assume that inside temperature is the target temperature
        double propSensibleHeat = CalcPropSensible(targetTemperature);
        double sensibleHeatOp = propSensibleHeat * heatOp;
        double surfaceArea = planArea + wallArea;    // sq meters - surface area of house
        double tmeanThermalTrans = (planArea * thermalTransRoof + wallArea * thermalTransWall)/ surfaceArea;          // in W per metre square per K

        outsideAirTemp += 273.13;  //convert from Celsius to Kelvin
        supplementaryHeat = 0.0;
        ventilationRate = 0.0;

        double airDensity = utilities.GetdensityAir(utilities.GetStandardAirPressure(), outsideAirTemp, utilities.GetsaturatedWaterVapourPressure(outsideAirTemp));
        double longWave = 5.67E-8 * Math.Pow(outsideAirTemp, 4) * (utilities.GetSkyEmissivity(outsideAirTemp, watervapourPressure) - emissivity); //equation 1.4
        // !tempSol = outside air temperature which, in the absence of solar radiation, would give the same temperature distribution and rate of energy transfer
        // !through a wall or roof as that which exists with the actual air temperature and incident radiation
        double tempSol = outsideAirTemp + externSurfResis * (absorbCoeff * solarRad - emissivity * longWave);    // equation 1.5
        double specificHeatCapAir = utilities.GetspecificHeatCapAir(watervapourPressure,outsideAirTemp); // used in equation 1.8 ("c")
        // Note - it is possible that the following can return a negative ventilation rate, if the heat produced can all be disappated through the walls and roof
        if (targetTemperature > outsideAirTemp)
        {    // it will be possible to maintain target temp
            //equation 1.8
            ventilationRate = (sensibleHeatOp - thermalTransRoof * planArea * (outsideAirTemp-tempSol) - wallArea * thermalTransWall *(targetTemperature-outsideAirTemp))/ 
                               (specificHeatCapAir * airDensity * (targetTemperature - outsideAirTemp));
            insideTemperature = targetTemperature - 273.13;
        }

        if ((ventilationRate > maxVentilation) || (targetTemperature <= outsideAirTemp))
        {    // it will not be possible to maintain target temp, so set ventilation to maximum value
            //equation 1.10
            ventilationRate = maxVentilation;
            insideTemperature= (sensibleHeatOp + planArea*thermalTransRoof*tempSol + outsideAirTemp*(specificHeatCapAir*airDensity*maxVentilation + wallArea*thermalTransWall))/
                    (specificHeatCapAir*airDensity*maxVentilation + planArea*thermalTransRoof + wallArea*thermalTransWall);
        }

        if (ventilationRate < minVentilation)
        {    // to keep the animals healthy, there needs to be some ventilation
            ventilationRate = minVentilation;
            // calculate the supplementary heat input necessary to maintain the inside temperature at the target temperature (joules)
            //equation 1.11
           supplementaryHeat = (planArea*thermalTransRoof*(targetTemperature-tempSol) + (wallArea*thermalTransWall
                + specificHeatCapAir*airDensity*minVentilation)*(targetTemperature-outsideAirTemp)) - sensibleHeatOp;
            if (supplementaryHeat > maxSupplementaryHeat)
            {
                supplementaryHeat = maxSupplementaryHeat;
                insideTemperature = (sensibleHeatOp + supplementaryHeat + planArea * thermalTransRoof * tempSol + outsideAirTemp * (specificHeatCapAir * airDensity * minVentilation + wallArea * thermalTransWall)) /
                        (specificHeatCapAir * airDensity * minVentilation + planArea * thermalTransRoof + wallArea * thermalTransWall);
            }
        }
    }

    public void uncontrolled(double heatOp, double outsideAirTemp, double windspeed, double solarRad, double watervapourPressure)
    {
        if (meanApertureWidth > meanWallLength)
            ErrorHandling(1);
        if (minTemperature > maxTemperature)
            ErrorHandling(2);
        // !calculate the amount of sensible heat produced in the animal housing
        // assume that inside temperature is close to outside temperature
        double propSensibleHeat = CalcPropSensible(outsideAirTemp);
        double sensibleHeatOp = propSensibleHeat * heatOp;

        maxTemperature = theCows.GetmaximumTemperature();
        maxTemperature += 273.15;
        minTemperature += 273.15;
        outsideAirTemp += 273.15;
        double longWave = 5.67E-8 * Math.Pow(outsideAirTemp, 4) * (utilities.GetSkyEmissivity(outsideAirTemp, watervapourPressure) - emissivity); //equation 1.4
        double airDensity = utilities.GetdensityAir(utilities.GetStandardAirPressure(), outsideAirTemp, utilities.GetsaturatedWaterVapourPressure(outsideAirTemp));
        double specificHeatCapAir = utilities.GetspecificHeatCapAir(watervapourPressure, outsideAirTemp);

        // !tempSol = outside air temperature which, in the absence of solar radiation, would give the same temperature distribution and rate of energy transfer
        // !through a wall or roof as that which exists with the actual air temperature and incident radiation
        double tempSol = outsideAirTemp + externSurfResis * (absorbCoeff * solarRad - emissivity * longWave);    // equation 1.5

        // !Temperature difference between outside surfaces of roof and air temperature
        //used in equation 10 of Cooper et al
        double deltaSol = tempSol - outsideAirTemp;

        // !Heat input or output to housing through the roof material
        double q = thermalTransRoof * planArea * deltaSol;    // Watts

        // !Set target ventilation rate to that which is necessary to have the optimum air velocity
        double targetventilationRate = optimumAirVelocity * meanWallLength * meanWallHeight;
        double apertureHeight = 0;
        double surfaceAreaWall = 0;
        double deltaTemp = 0;
        if (minPropApertureHeight == 1.0)  //then there is no ability to control temperature
        {
            surfaceAreaWall = 4*((meanWallHeight - maxapertureHeight) * meanWallLength);
            CalcFreeVentilation(airDensity, thermalTransRoof, planArea, thermalTransWall, surfaceAreaWall, apertureHeight, windspeed, outsideAirTemp, watervapourPressure, heatOp,
                                q, ref ventilationRate, ref deltaTemp);
            insideTemperature = outsideAirTemp + deltaTemp;
        }
        else
        {
            // !calculate ventilation with the minimum ventilation
            apertureHeight = minPropApertureHeight * maxapertureHeight;
            double minVentilation = 0;
            double deltaTemperatureMinVent = 0;
            CalcFreeVentilation(airDensity, thermalTransRoof, planArea, thermalTransWall, surfaceAreaWall, apertureHeight, windspeed, outsideAirTemp, watervapourPressure, heatOp,
                                q, ref minVentilation, ref deltaTemperatureMinVent);
            double forcedVentilation = windspeed * meanApertureWidth * apertureHeight;
            double maxVentilation = 0;
            double deltaTemperatureMaxVent = 0;

            apertureHeight = maxapertureHeight;
            surfaceAreaWall = 4 * ((meanWallHeight - apertureHeight) * meanWallLength);
            CalcFreeVentilation(airDensity, thermalTransRoof, planArea, thermalTransWall, surfaceAreaWall, apertureHeight, windspeed, outsideAirTemp, watervapourPressure, heatOp,
                                q, ref maxVentilation, ref deltaTemperatureMaxVent);

            forcedVentilation = windspeed * meanApertureWidth * apertureHeight;
            if (outsideAirTemp + deltaTemperatureMaxVent >= maxTemperature)  //the inside temperature exceeds the maximum, so set ventilation to maximum to minimize the excess as much as possible
            {
                ventilationRate = maxVentilation;
                insideTemperature = outsideAirTemp + deltaTemperatureMaxVent;
            }
            else
            {
                if (outsideAirTemp + deltaTemperatureMinVent <= minTemperature) //the inside temperature is below the minimum, so set ventilation to minimum to minimize the deficiency as much as possible
                {
                    ventilationRate = minVentilation;
                    insideTemperature = outsideAirTemp + deltaTemperatureMinVent;
                }
                else  //the temperature can be kept above the minimum and below the maximum
                {
                    double optApertureProportion = 0;
                    optApertureProportion = minPropApertureHeight + (1 - minPropApertureHeight) * (targetventilationRate - minVentilation) / (maxVentilation - minVentilation);
                    apertureHeight = optApertureProportion * maxapertureHeight;
                    forcedVentilation = windspeed * meanApertureWidth * apertureHeight;
                    double bouyancyVentilation = Math.Pow(Math.Pow(targetventilationRate, 2) - Math.Pow(forcedVentilation, 2), 0.5);
                    deltaTemp = CalcB(outsideAirTemp, 0.6, 4.0 * meanApertureWidth, apertureHeight) * Math.Pow(bouyancyVentilation, 2);
                    if (optApertureProportion <= minPropApertureHeight)  //check to see if the target ventilation rate can be achieved
                    {
                        ventilationRate = minVentilation;
                        insideTemperature = outsideAirTemp + deltaTemperatureMinVent;
                    }
                    else
                    {
                        if (optApertureProportion > 1.0)
                        {
                            ventilationRate = maxVentilation;
                            insideTemperature = outsideAirTemp + deltaTemperatureMaxVent;
                        }
                        else
                        {
                            ventilationRate = targetventilationRate;
                            insideTemperature = outsideAirTemp + deltaTemp;
                        }
                    }
                    if (insideTemperature > maxTemperature)
                    {
                        optApertureProportion = minPropApertureHeight + (1 - minPropApertureHeight) * (maxTemperature - (outsideAirTemp + deltaTemperatureMaxVent)) /
                                        (outsideAirTemp + deltaTemperatureMinVent - deltaTemperatureMaxVent);
                        apertureHeight = optApertureProportion * maxapertureHeight;
                        surfaceAreaWall = 4 * ((meanWallHeight - apertureHeight) * meanWallLength);
                        CalcFreeVentilation(airDensity, thermalTransRoof, planArea, thermalTransWall, surfaceAreaWall, apertureHeight, windspeed, outsideAirTemp, watervapourPressure, heatOp,
                                            q, ref ventilationRate, ref deltaTemp);
                        insideTemperature = outsideAirTemp + deltaTemp;
                    }
                    if (insideTemperature < minTemperature)
                    {
                        optApertureProportion = minPropApertureHeight + (1 - minPropApertureHeight) * (1 - (minTemperature - (outsideAirTemp + deltaTemperatureMaxVent) /
                                        (outsideAirTemp + deltaTemperatureMaxVent - deltaTemperatureMinVent)));
                        apertureHeight = optApertureProportion * maxapertureHeight;
                        CalcFreeVentilation(airDensity, thermalTransRoof, planArea, thermalTransWall, surfaceAreaWall, apertureHeight, windspeed, outsideAirTemp, watervapourPressure, heatOp,
                                            q, ref ventilationRate, ref deltaTemp);
                        insideTemperature = outsideAirTemp + deltaTemp;
                    }

                }
            }
        }
    }

    public void CalcFreeVentilation(double airDensity,double thermalTransRoof,double surfaceAreaRoof, double thermalTransWall, double surfaceAreaWall,double apertureHeight,
        double windspeed,double outsideAirTemp, double waterVapourPressure, double heatOP,double q,ref double ventilationRate,ref double deltaTemper)
    {

        // ! Model from A THERMAL BALANCE MODEL FOR LIVESTOCK BUILDINGS, Cooper et al, 1998
        ventilationRate = 0.0;

        double Cd = 0.6;    // !discharge coefficient for sharp-edged openings

        // !B = factor to simplify subsequent equations
        double B = CalcB(outsideAirTemp,Cd,4 * meanApertureWidth, apertureHeight);
        double[] x= new double[] { 0, 0, 0 };
        int solutions=0;

        // ! a1, b1, c1, d1 are coefficients of the cubic equation number 21 in Cooper et al
        double spheatCap = utilities.GetspecificHeatCapAir(waterVapourPressure,outsideAirTemp);
        double a1 = spheatCap * airDensity;
        double b1 = thermalTransRoof * surfaceAreaRoof + thermalTransWall * surfaceAreaWall;

        // !windforcedVent is the ventilation that occurs due to the effect of the wind blowing air through the apertures in the building
        double windforcedVent = apertureHeight * meanApertureWidth * windspeed;    // we assume a square building, with only one wall facing the wind, cubic m per sec
        double c1 = -Math.Pow(windforcedVent, 2) * spheatCap * airDensity;
        double d1 = -b1 * Math.Pow(windforcedVent, 2) - (heatOP + q) / B;

        // !solve for ventilation rate
        utilities.SolveCubic(a1, b1, c1, d1, ref solutions, ref x);

        int index = 0;
        double test = 0.0;

        // solution is the highest value in the matrix
        for (int j = 0; j < solutions; j++)
        {
            if (x[j] > test)
            {
                test = x[j];
                index = j;
            }
        }

        double forcedVentilation = windspeed * meanApertureWidth * apertureHeight;
        double bouyancyVentilation = Math.Pow(Math.Pow(x[index], 2) - Math.Pow(forcedVentilation, 2), 0.5);
        deltaTemper = CalcB(outsideAirTemp, 0.6, 4.0 * meanApertureWidth, apertureHeight) * Math.Pow(bouyancyVentilation, 2);
        ventilationRate = x[index];
    }

    public double CalcB(double outsideAirTemp, double Cd, double totalApertureWidth, double apertureHeight)
    {
        double B = 9.0 * outsideAirTemp / (Math.Pow(Cd, 2) * Math.Pow(totalApertureWidth, 2) * Math.Pow(apertureHeight, 3) * 9.81);
        return B;
    }
    /*public void CheckTemperatureBounds()
    {
        if ((outsideAirTemp + deltaTemp) > maxTemperature)
        {
            optApertureProportion = minPropApertureHeight + (1 - minPropApertureHeight) * (maxTemperature - (outsideAirTemp + deltaTemperatureMinVent)) /
                            (outsideAirTemp + deltaTemperatureMaxVent - deltaTemperatureMinVent);
            optApertureHeight = optApertureProportion * maxapertureHeight;
            CalcFreeVentilation(airDensity, meanThermalTrans, surfaceArea, optApertureHeight, windspeed, outsideAirTemp, watervapourPressure, heatOp,
                                q, ref ventilationRate, ref deltaTemp);
        }
        if ((outsideAirTemp + deltaTemp) < minTemperature)
        {
            optApertureProportion = minPropApertureHeight + (1 - minPropApertureHeight) * (1 - (minTemperature - (outsideAirTemp + deltaTemperatureMaxVent) /
                            (outsideAirTemp + deltaTemperatureMaxVent - deltaTemperatureMinVent)));
            optApertureHeight = optApertureProportion * maxapertureHeight;
            CalcFreeVentilation(airDensity, meanThermalTrans, surfaceArea, optApertureHeight, windspeed, outsideAirTemp, watervapourPressure, heatOp,
                                q, ref ventilationRate, ref deltaTemp);
        }

    }*/
}

   // ! Model from A THERMAL BALANCE MODEL FOR LIVESTOCK BUILDINGS, Cooper et al, 1998
