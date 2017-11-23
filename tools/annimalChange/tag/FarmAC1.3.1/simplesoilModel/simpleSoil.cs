using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace simplesoilModel
{
    class simpleSoil
    {

        //! Cumulative amount of water input into the system		
        double waterIn; //Attribute data member
        //! Cumulative amount of water leaving the system		
        double waterOut; //Attribute data member
        //! Amount of water in soil (mm)
        double waterInSystem;
        //! Plant-available water of whole soil at field capacity (millimetres)		
        double soilFieldCapacity; //Attribute data member
        //! Is set to true if the soil should be irrigated		
        bool irrigate; //Attribute data member        

        //! Pool containing snow						
        pool theSnow;
        //! Rooting pool
        pool theRooting;
        //! Pool of water beneath the rooting pool
        pool theSubZone;
        //! Plant canopy interception pool
        pool canopyInterception;
        //!Pool that describes the water that can be lost in the soil by evaporation
        pool theEvaporation;
        /*!Pool that describes the water available in the upper part of the rooting zone. Only used when precipitation
        or irrigation water falls onto the soil at a time when the soil moisture in the rooting zone is below the breakpoint
        for reduced transpiration.
        */
        pool upperRooting;

        //! Pointer to a list of instances containing the physical characteristics of each soil layer
        List<layerClass> theLayers; //1:M aggregation part data member
        double maxRootingDepth;
        double drainageConst;
        double breakpoint = 1.0;
        public double getwaterIn() { return waterIn; }
        public double getwaterOut() { return waterOut; }
        public double getwaterInSystem() { return waterInSystem; }
        double getsoilFieldCapacity() { return soilFieldCapacity; }
        bool getirrigate() { return irrigate; }
        void setwaterIn(double awaterIn) { waterIn = awaterIn; }
        void setwaterOut(double awaterOut) { waterOut = awaterOut; }
        void setsoilFieldCapacity(double asoilFieldCapacity) { soilFieldCapacity = asoilFieldCapacity; }
        void setirrigate(bool airrigate) { irrigate = airrigate; }
        pool gettheSnow() { return theSnow; }
        pool gettheRooting() { return theRooting; }
        pool gettheSubZone() { return theSubZone; }
        pool getcanopyInterception() { return canopyInterception; }
        pool gettheEvaporation() { return theEvaporation; }
        pool getupperRooting() { return upperRooting; }
        void settheSnow(pool atheSnow) { theSnow = atheSnow; }
        void settheRooting(pool atheRooting) { theRooting = atheRooting; }
        void settheSubZone(pool atheSubZone) { theSubZone = atheSubZone; }
        void setcanopyInterception(pool acanopyInterception) { canopyInterception = acanopyInterception; }
        void settheEvaporation(pool atheEvaporation) { theEvaporation = atheEvaporation; }
        void setupperRooting(pool aupperRooting) { upperRooting = aupperRooting; }
        public List<layerClass> gettheLayers() { return theLayers; }
        //!Initialise an instance of this class
        /*! 
        \param thesoilData pointer to the instance of a class containing the soil parameters
        */
        //  void Initialise(soilDataClass thesoilData, double rootingDepth, double totalLAI)
        public void Initialise(double rootingDepth, double totalLAI)
        {
            theLayers = new List<layerClass>();
            double z_upper = 0.0;
            soilFieldCapacity = 0.0;
            FileInformation fileInfo = new FileInformation(GlobalVars.Instance.getSimplesoilModelFileName());
            fileInfo.setPath("maxRootingDepth(-1)");
            maxRootingDepth = fileInfo.getItemDouble("Value");
            fileInfo.setPath("drainageConst(-1)");
            drainageConst = fileInfo.getItemDouble("Value");

            fileInfo.setPath("layerClass");
            int min = 99; int max = 0;
            fileInfo.getSectionNumber(ref min, ref max);
            for (int index = min; index <=max; index++)
            {
                layerClass alayerClass = new layerClass();
                if (index == 1)
                    z_upper = 0.0;
                else
                    z_upper = theLayers[index - 2].getz_lower();  //lower boundary of one layer is the upper boundary of the next
                alayerClass.Initialise(index,z_upper);
                soilFieldCapacity += alayerClass.getfieldCapacity() * alayerClass.getthickness();
                theLayers.Add(alayerClass);
            }

            waterIn = 0.0;
            waterOut = 0.0;
            irrigate = false;
            layerClass anewlayerClass;
            theSnow = new pool();
            theSnow.Initialise(0.0, 100000.0, 1.0);
            theSnow.setname("snow");
            anewlayerClass = theLayers[0];
            theEvaporation = new pool();
            theEvaporation.Initialise(10.0, 10.0, 1.0);
            theEvaporation.setname("evaporation");
            //Load the lowest soil layer
            anewlayerClass = theLayers[theLayers.Count - 1];
            double soilDepth = anewlayerClass.getz_lower();
            double poolDepth;
            anewlayerClass = theLayers[0];
            double evapDepth = 10.0 / (1000.0 * anewlayerClass.getfieldCapacity());   //evaporation pool has always a capacity of10 millimetres
            //depth of rooting poolis always greater or equal to the depth of evaporation pool
            poolDepth = rootingDepth;
            if (poolDepth < evapDepth)
                poolDepth = evapDepth;
            else
                poolDepth = evapDepth;
            //calculate the drainage constant for the rooting pool
            double pooldrainageConst = drainageConst + (1 - drainageConst) * (soilDepth - poolDepth) / soilDepth;
            //initialise the rooting pool
            theRooting = new pool();
            theRooting.Initialise(10.0, 10.0, pooldrainageConst);
            theRooting.setname("rooting");
            double availWater = 0.0;
            //calculate the thickness of the different layers
            double thickness;
            for (int index = 0; index < theLayers.Count; index++)
            {
                anewlayerClass = gettheLayers()[index];
                thickness = anewlayerClass.getthickness();
                availWater += thickness * anewlayerClass.getfieldCapacity();
            }
            waterIn += availWater;

            //note initial value of poolDepth is depth of rooting pool
            poolDepth = soilDepth - poolDepth;
            //calculate the drainage constant for the subroot zone
            pooldrainageConst = drainageConst + (1 - drainageConst) * poolDepth / soilDepth;
            theSubZone = new pool();
            theSubZone.Initialise(availWater - 10.0, availWater - 10.0, pooldrainageConst);
            theSubZone.setname("subzone");
            //set the upper rooting to zero
            upperRooting = new pool();
            upperRooting.Initialise(0.0, 0.0, 0.0);
            upperRooting.setname("upperRooting");
            //initialise the canopy interception pool.
            canopyInterception = new pool();
            canopyInterception.Initialise(0.0, 0.5 * totalLAI, 1.0);
            canopyInterception.setname("interception");
        }
        //! Execute the daily functions
        /*!
        \param potEvap potential evapotranspiration (millimetres per day)
        \param precipIn precipitation (millimetres per day)
        \param Tmean mean air temperature (Celsius)
        */
        public void dailyRoutine(double potEvap, double precipIn, double Tmean, double totalLAI, double rootingDepth)
        {
            //	linkList<double>

            waterIn += precipIn;
            //update pool status, since rooting depth may have changed
            updatePools(rootingDepth, totalLAI);
            double drainage = 0.0;
            //handle snow
            double snowEvap = potEvap;
            drainage = theSnow.dailySnow(ref potEvap, precipIn, Tmean);
            snowEvap -= potEvap;   //subtract evaporation from snow from the potential evapotranspiration
            waterOut += snowEvap;
            ///irrigation
            double irrigationWater = 0.0;
            if (irrigate)
            {
                irrigationWater = getSMD(maxRootingDepth, maxRootingDepth);
                waterIn += irrigationWater;
                drainage += irrigationWater;
            }
            double croppotEvap;
            double croppotEvapGreen;
            double soilpotEvap;
            //set interception pool to zeroif no plants present


            //partition potevap between crop and soil surface
            double attenuationCoeff = 0.45; //read from Globvars
            double propToSoil = Math.Exp(-attenuationCoeff * totalLAI);
            croppotEvap = potEvap * (1 - propToSoil);  //potential evapotranspiration from crop
            soilpotEvap = potEvap * propToSoil;      //potential evaporation from soil
            //water available for evaporation from canopy includes any drainage from snow melt or irrigation
            double availWater = canopyInterception.getvolume() + drainage;
            if (availWater < croppotEvap)
            {
                canopyInterception.update(drainage, 0.0, availWater, 0.0);
                croppotEvap -= availWater;
                waterOut += availWater;
                drainage = 0.0;
            }
            else
            {
                drainage = canopyInterception.update(drainage, 0.0, croppotEvap, 0.0);
                waterOut += croppotEvap;
                croppotEvap = 0.0;
            }

            if (totalLAI > 0.0)   //calculate potential transpiration from green leaf
                croppotEvapGreen = croppotEvap;
            else
                croppotEvapGreen = 0.0;
            double canopyDrainage = drainage;
            //deal with soil surface layer
            double surfaceAvailWater = theEvaporation.getvolume() + drainage;   //water available for evaporation
            double rootAvailWater = theRooting.getvolume() + drainage;          //water available for evaporation or transpiration
            double evap = 0.0;
            double capillary = 0.0;
            if (surfaceAvailWater > soilpotEvap)  //if there is enough water in the surface to enable evaporation at the potential rate
                evap = soilpotEvap;
            else  //insufficient water available in surface to enable evaporation at the potential rate
            {
                if (rootAvailWater > 0.0)  //there is water in the rooting pool
                {
                    capillary = 0.15 * (soilpotEvap - surfaceAvailWater);   //calculate the evaporation supported by capillary flow
                    evap = capillary + surfaceAvailWater;
                }
                else
                    evap = surfaceAvailWater;    //no capillary flow
            }
            waterOut += evap;

            if (capillary > 0.0)  //if there has been capillary flow
            {
                theEvaporation.update(drainage, capillary, evap, 0.0);    //update the evaporation pool
                if (evap <= (theRooting.getvolume() + drainage))
                {
                    drainage = theRooting.update(drainage, 0.0, evap, 0.0);
                    drainage = theSubZone.update(drainage, 0.0, 0.0, 0.0);
                }
                else
                {
                    double fromRooting = theRooting.getvolume();    //capillary flow provided from the rooting zone
                    double fromSubZone = evap - (fromRooting + drainage); //capillary flow provided from below the rooting zone
                    drainage = theRooting.update(drainage, fromSubZone, evap, 0.0);  //update rooting zone
                    drainage = theSubZone.update(drainage, fromSubZone, fromSubZone, 0.0);  //update sub rooting zone
                    waterIn += fromSubZone;
                }
            }
            else   //update all the soil pools
            {
                theEvaporation.update(drainage, 0.0, evap, 0.0);
                drainage = theRooting.update(drainage, 0.0, evap, 0.0);
                drainage = theSubZone.update(drainage, 0.0, 0.0, 0.0);
            }
            waterOut += drainage;

            //now calculate transpiration
            double transpire = 0.0;
            if (theRooting.getvolume() >= breakpoint * theRooting.getmaxVolume())    //if the water available in the rooting zone is sufficient to allow transpiration to proceed at the maximum rate
            {
                //zero the upper rooting pool
                upperRooting.setvolume(0.0);
                upperRooting.setmaxVolume(0.0);
            }
            else  //calculate contents of the upper rooting pool
            {
                upperRooting.setmaxVolume(Math.Min(theRooting.getmaxVolume(), (upperRooting.getmaxVolume() + canopyDrainage - evap)));
                if (upperRooting.getmaxVolume() < 0.0)
                    upperRooting.setmaxVolume(0.0);
                upperRooting.setvolume(upperRooting.getvolume() + canopyDrainage - evap);
                if (upperRooting.getvolume() < 0.0)
                    upperRooting.setvolume(0.0);
            }
            if ((upperRooting.getvolume() >= breakpoint * upperRooting.getmaxVolume()) &&
                  (upperRooting.getvolume() >= croppotEvapGreen))     //if there is sufficient water in the upper rooting pool to enable transpiration to proceed at the central rate
            {     //take no action
            }
            else  //set up a rooting pool to 0
            {
                upperRooting.setvolume(0.0);
                upperRooting.setmaxVolume(0.0);
            }
            if ((upperRooting.getvolume() > 0.0) || (theRooting.getvolume() > breakpoint * theRooting.getmaxVolume()))
                //then transpiration can precede at the potential rate
                transpire = croppotEvapGreen;
            else    //transpiration will be at a rate lower than the potential
                if ((upperRooting.getvolume() == 0.0) && (theRooting.getvolume() > 0.0)
                        && (theRooting.getvolume() < (breakpoint * theRooting.getmaxVolume())))
                    //transpiration rate is reduced to a fraction of the potential
                    transpire = (theRooting.getvolume() / (breakpoint * theRooting.getmaxVolume())) * croppotEvapGreen;
                else  //there will be no transpiration
                    transpire = 0.0;

            if (upperRooting.getvolume() > transpire)   //update up a rooting pool
                upperRooting.setvolume(upperRooting.getvolume() - transpire);
            else
                upperRooting.setvolume(0.0);

            double waterFromEvaporationPool = 0.0;
            if (theRooting.getvolume() > 0.0)
                //calculate the contribution to transpiration from the evaporation will
                waterFromEvaporationPool = transpire * theEvaporation.getvolume() / theRooting.getvolume();
            theEvaporation.setvolume(theEvaporation.getvolume() - waterFromEvaporationPool);     //update evaporation pool
            theRooting.setvolume(theRooting.getvolume() - transpire);     //update rooting pool
            theRooting.settranspiration(transpire);
            waterOut += transpire;
            if (theEvaporation.getvolume() < 0.000000001)  //set evaporation pool to 0 (limited precision means that calculations can lead to very small negative values)
                theEvaporation.setvolume(0.0);


            checkBudget();    //check for water budget closure
        }
        //!Calculate the capacity of a soil water pool
        /*! 
        \param startDepth depth below the soil surface of the upper boundary of the pool (millimetres)
        \param endDepth depth below the soil surface of the lower boundary of the pool (millimetres)
        */
        double calcPoolVolume(double startDepth, double endDepth)
        {
            double ret_val = 0.0;
            int num = gettheLayers().Count;
            double z_upper = 0.0;
            for (int index = 0; index < num; index++)
            {
                layerClass alayerClass = gettheLayers()[index];
                double z_lower = alayerClass.getz_lower();
                if ((startDepth <= z_upper) && (endDepth >= z_lower))  //whole layer is included
                    ret_val += (z_lower - z_upper) * alayerClass.getfieldCapacity();
                if ((endDepth > z_upper) && (endDepth < z_lower))  //upper part of layer included only
                    ret_val += (endDepth - z_upper) * alayerClass.getfieldCapacity();
                if ((startDepth > z_upper) && (startDepth < z_lower))  //lower part of layer included only
                    ret_val += (z_lower - startDepth) * alayerClass.getfieldCapacity();
                z_upper = alayerClass.getz_lower();
            }
            return ret_val;
        }
        //!Adjust the variables and parameters of the soil pool in response to a tillage operation
        /*! 
        \param depth depth to which the soil is tilled (millimetres)
        */
        void tillage(double depth)
        {

            //checkBudget();
            layerClass alayerClass = gettheLayers()[0];
            double z_lower = alayerClass.getz_lower();
            double depthRootingPool = theRooting.getmaxVolume() / alayerClass.getfieldCapacity();  //depth of rooting pool may be > rooting depth (if == to evaporation pool)
            double newMoistVal;
            if (depth > z_lower)
                GlobalVars.Instance.Error("tillage depth is greater than the depth of the uppermost soil layer");
            if (depth > depthRootingPool)  //then tillage will affect all of the rooting pool and some of the sub zone pool
            {
                z_lower = gettheLayers()[gettheLayers().Count].getz_lower();
                double depthSubZonePool = z_lower - depthRootingPool;
                double oldRootZoneVol = theRooting.getvolume();
                double oldRootZoneMaxVol = theRooting.getmaxVolume();
                double oldSubZoneVol = theSubZone.getvolume();
                double oldSubZoneMaxVol = theSubZone.getmaxVolume();
                //new moisture value for the rooting pool is calculated as the weighted average of the rooting and sub rooting pools
                double tempVol = oldRootZoneVol + (depth - depthRootingPool) * oldSubZoneVol / depthSubZonePool;
                double tempMaxVol = oldRootZoneMaxVol + (depth - depthRootingPool) * oldSubZoneMaxVol / depthSubZonePool;
                newMoistVal = tempVol / tempMaxVol;
                theRooting.setmaxVolume(10.0);
                double newRootingVol = newMoistVal * theRooting.getmaxVolume();
                theRooting.setvolume(newRootingVol);
                double newEvapVol = newMoistVal * theEvaporation.getmaxVolume();
                theEvaporation.setvolume(newEvapVol);
                //now calculate new moisture value for the sub zone
                theSubZone.setmaxVolume(oldRootZoneMaxVol + oldSubZoneVol - theRooting.getmaxVolume());
                theSubZone.setvolume(oldSubZoneVol + oldRootZoneVol - theRooting.getvolume());
            }
            else  //there will be no change in the rooting pool but a change in the evaporation pool only
            {
                newMoistVal = theRooting.getvolume() / theRooting.getmaxVolume();
                double newEvapVol = newMoistVal * theEvaporation.getmaxVolume();
                theEvaporation.setvolume(newEvapVol);
            }
            //   checkBudget();
        }
        //! Update the soil pools to account for changes in the crop rooting depth
        void updatePools(double rootingDepth, double totalLAI)
        {
            // update pools
            double newrootCapacity;

            if (rootingDepth > maxRootingDepth) //rooting depth cannot exceed the maximum rooting depth of the soil
                rootingDepth = maxRootingDepth;
            newrootCapacity = calcPoolVolume(0, rootingDepth); //recalculate the capacity of the rooting pool
            if (newrootCapacity < 10.0)
                newrootCapacity = 10.0;
            canopyInterception.setmaxVolume(0.5 * totalLAI); //update the capacity of the canopy interception pool

            double oldrootCapacity = theRooting.getmaxVolume();
            theRooting.setmaxVolume(newrootCapacity);
            double oldrootVolume = theRooting.getvolume();
            double oldSubZonecapacity = theSubZone.getmaxVolume();
            double oldSubZonevolume = theSubZone.getvolume();
            if (newrootCapacity <= oldrootCapacity)   //if the rooting depth is decreasing
            {
                theRooting.setvolume(oldrootVolume + (newrootCapacity - oldrootCapacity) * oldrootVolume / oldrootCapacity);
                if (theRooting.getvolume() < 10.0)
                    theRooting.setvolume(10.0);
            }
            else  //the rooting pool will inherit water from the sub zone pool
                theRooting.setvolume(oldrootVolume + (newrootCapacity - oldrootCapacity) * oldSubZonevolume / oldSubZonecapacity);

            layerClass alayerClass;
            double poolDepth;
            alayerClass = gettheLayers()[0];
            double evapDepth = 10.0 / (1000.0 * alayerClass.getfieldCapacity());
            alayerClass = gettheLayers()[gettheLayers().Count-1];

            poolDepth = rootingDepth;
            if (poolDepth < evapDepth)
                poolDepth = evapDepth;

            else
                poolDepth = evapDepth;
            double soilDepth = alayerClass.getz_lower();
            //recalculate the rooting pool drainage constant
            double pooldrainageConst = drainageConst + (1 - drainageConst)
                                                 * (soilDepth - poolDepth) / soilDepth;
            theRooting.setdrainageConst(pooldrainageConst);
            //note initial value of poolDepth is depth of rooting pool
            poolDepth = soilDepth - poolDepth;
            //recalculate the sub zone drainage constant
            pooldrainageConst = drainageConst + (1 - drainageConst)
                                                 * poolDepth / soilDepth;
            theSubZone.setdrainageConst(pooldrainageConst);

            //recalculate volume and capacity of the sub zone
            theSubZone.setvolume(oldSubZonevolume - (theRooting.getvolume() - oldrootVolume));
            double fieldCap = 0.0;
            double thickness;
            for (int index = 0; index < gettheLayers().Count; index++)
            {
                layerClass anewlayerClass = gettheLayers()[index];
                thickness = anewlayerClass.getthickness();
                fieldCap += thickness * anewlayerClass.getfieldCapacity();
            }

            theSubZone.setmaxVolume(fieldCap - theRooting.getmaxVolume());
            double newupperRootingCapacity = Math.Min(theRooting.getmaxVolume(), upperRooting.getvolume());
            if (newupperRootingCapacity > 0.0)
                upperRooting.setvolume(upperRooting.getvolume() - (upperRooting.getmaxVolume() - newupperRootingCapacity)
                 * upperRooting.getvolume() / upperRooting.getmaxVolume());
            else
                upperRooting.setvolume(0.0);
            upperRooting.setmaxVolume(newupperRootingCapacity);
        }

        //! Reports an error if the soil water budget cannot be closed
        public void checkBudget()
        {
            waterInSystem = 0.0;
            waterInSystem += canopyInterception.getvolume();
            waterInSystem += theRooting.getvolume();
            waterInSystem += theSubZone.getvolume();
            double error = waterInSystem + waterOut - waterIn;
            if (error > 0.0000001)
            {
                GlobalVars.Instance.Error(error.ToString());
            }
        }

        //!Outputs the soil variables to a file
        /*! 
        \param afile pointer to the output file
        \param header set to true if only the variable names should be printed
        */
        public void Write()
        {
            GlobalVars.Instance.writeStartTab("simpleSoil");
            theSnow.outputDetails();
            canopyInterception.outputDetails();
            theEvaporation.outputDetails();
            theRooting.outputDetails();
            theSubZone.outputDetails();
            upperRooting.outputDetails();
            GlobalVars.Instance.writeEndTab();
        }

        public void WriteDebugHeader()
        {
            GlobalVars.Instance.theZoneData.WriteToDebug("day ");
            theSnow.OutputDebugHeader(1);
            canopyInterception.OutputDebugHeader(1);
            theEvaporation.OutputDebugHeader(1);
            theRooting.OutputDebugHeader(1);
            theSubZone.OutputDebugHeader(1);
            upperRooting.OutputDebugHeader(1);
            GlobalVars.Instance.theZoneData.WriteLineToDebug("");
            GlobalVars.Instance.theZoneData.WriteToDebug(" ");
            theSnow.OutputDebugHeader(2);
            canopyInterception.OutputDebugHeader(2);
            theEvaporation.OutputDebugHeader(2);
            theRooting.OutputDebugHeader(2);
            theSubZone.OutputDebugHeader(2);
            upperRooting.OutputDebugHeader(2);
            GlobalVars.Instance.theZoneData.WriteLineToDebug("");
        }
        public void WriteDebug(int day)
        {
            GlobalVars.Instance.theZoneData.WriteToDebug(day + " ");
            theSnow.OutputDebug();
            canopyInterception.OutputDebug();
            theEvaporation.OutputDebug();
            theRooting.OutputDebug();
            theSubZone.OutputDebug();
            upperRooting.OutputDebug();
            GlobalVars.Instance.theZoneData.WriteLineToDebug("");
        }
        //!Returns the soil moisture deficit to a given depth
        /*! 
        \param depth depth below the soil surface for which the soil moisture deficit should be calculated
        */
        double getSMD(double depth, double rootingDepth)
        {
            double ret_val = 0.0;
            layerClass alayerClass = gettheLayers()[0];
            double evapDepth = 10.0 / (1000.0 * alayerClass.getfieldCapacity());   //evaporation pool has always a capacity of10 millimetres
            if (depth <= evapDepth)   //retrieve the deficit in the evaporation pool
                ret_val = theEvaporation.getDeficit();
            else
            {
                if (rootingDepth > depth)   //only a rooting pool is involved
                    ret_val = theRooting.getDeficit();
                else      //both the rooting and the sub zone pools are involved
                    ret_val = (rootingDepth * theRooting.getDeficit() + (depth - rootingDepth) * theSubZone.getDeficit()) / depth;
            }
            return ret_val;
        }

        //! Return the depth of the soil
        double getSoilDepth()
        {
            layerClass alayerClass = gettheLayers()[(gettheLayers().Count)];
            return alayerClass.getz_lower();
        }
    }
}
