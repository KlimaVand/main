using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace simplesoilModel
{
    class simplePlant
    {
        //! Name of the crop		
        string Name; //Attribute data member
        //! Green leaf area index		
        double Lg; //Attribute data member
        //! Yellow leaf area index		
        double Ly; //Attribute data member
        //! Total leaf area index		
        double Ltot; //Attribute data member
        //! Current root depth (millimetres)		
        double rootDepth; //Attribute data member
        //! Breakpoint for soil moisture below which transpiration is reduced below the potential rate (proportion of available water capacity)		
        double breakpoint; //Attribute data member
        //! Maximum leaf area index		
        double Lmax; //Attribute data member
        //! Leaf area index during the winter		
        double Lwinter; //Attribute data member
        //! Maximum dead leaf area index		
        double Lymax; //Attribute data member
        //! Leaf area index of grass in undersown crops, after the cereal crop has been harvested		
        double Lmin; //Attribute data member
        //! Attenuation coefficient of Leaf area index		
        double attenuationCoeff; //Attribute data member
        //! Temperature sum for crop emergence in the spring (degree days)		
        double So; //Attribute data member
        //! Temperature sum for emergence of crops sown in the autumn (degree days)		
        double Soe; //Attribute data member
        //! Temperature sum to achieve maximum leaf area index (degree days)		
        double Sf; //Attribute data member
        //! Temperature sum to achieve maximum leaf area index in the autumn (degree days)		
        double Sfe; //Attribute data member
        //! Temperature sum for the start of maturation (degree days)		
        double Sr; //Attribute data member
        //! Temperature sum to achieve full maturity (degree days)		
        double Sm; //Attribute data member
        //! Root depth of crops during the winter (millimetres)		
        double rootDepthWinter; //Attribute data member
        //! Rate of increase of rooting depth (millimetres per day)		
        double croot; //Attribute data member
        //! Current temperature sum		
        double plantTsum; //Attribute data member
        //! Set to false if no crop is present		
        bool active; //Attribute data member
        //! Set to true if the transformation is specific to the autumn should be used		
        bool useAutumn; //Attribute data member
        //! Set to true if the transformation is specific to the winter should be used		
        bool useWinter; //Attribute data member   
        public string getName() { return Name; }

        //!Get accessor function for non-static attribute data member
        public double getLg() { return Lg; }

        //!Get accessor function for non-static attribute data member
        public double getLy() { return Ly; }

        //!Get accessor function for non-static attribute data member
        public double getLtot() { return Ltot; }

        //!Get accessor function for non-static attribute data member
        public double getrootDepth() { return rootDepth; }

        //!Get accessor function for non-static attribute data member
        public double getbreakpoint() { return breakpoint; }

        //!Get accessor function for non-static attribute data member
        public double getLmax() { return Lmax; }

        //!Get accessor function for non-static attribute data member
        public double getLwinter() { return Lwinter; }

        //!Get accessor function for non-static attribute data member
        public double getLymax() { return Lymax; }

        //!Get accessor function for non-static attribute data member
        public double getLmin() { return Lmin; }

        //!Get accessor function for non-static attribute data member
        public double getattenuationCoeff() { return attenuationCoeff; }

        //!Get accessor function for non-static attribute data member
        public double getSo() { return So; }

        //!Get accessor function for non-static attribute data member
        public double getSoe() { return Soe; }

        //!Get accessor function for non-static attribute data member
        public double getSf() { return Sf; }

        //!Get accessor function for non-static attribute data member
        public double getSfe() { return Sfe; }

        //!Get accessor function for non-static attribute data member
        public double getSr() { return Sr; }

        //!Get accessor function for non-static attribute data member
        public double getSm() { return Sm; }

        //!Get accessor function for non-static attribute data member
        public double getrootDepthWinter() { return rootDepthWinter; }

        //!Get accessor function for non-static attribute data member
        public double getcroot() { return croot; }

        //!Get accessor function for non-static attribute data member
        public double getplantTsum() { return plantTsum; }

        //!Get accessor function for non-static attribute data member
        public bool getactive() { return active; }

        //!Get accessor function for non-static attribute data member
        public bool getuseAutumn() { return useAutumn; }

        //!Get accessor function for non-static attribute data member
        public bool getuseWinter() { return useWinter; }

        //!Set accessor function for non-static attribute data member
        public void setName(string aName) { Name = aName; }

        //!Set accessor function for non-static attribute data member
        public void setLg(double aLg) { Lg = aLg; }

        //!Set accessor function for non-static attribute data member
        public void setLy(double aLy) { Ly = aLy; }

        //!Set accessor function for non-static attribute data member
        public void setLtot(double aLtot) { Ltot = aLtot; }

        //!Set accessor function for non-static attribute data member
        public void setrootDepth(double arootDepth) { rootDepth = arootDepth; }

        //!Set accessor function for non-static attribute data member
        public void setbreakpoint(double abreakpoint) { breakpoint = abreakpoint; }

        //!Set accessor function for non-static attribute data member
        public void setLmax(double aLmax) { Lmax = aLmax; }

        //!Set accessor function for non-static attribute data member
        public void setLwinter(double aLwinter) { Lwinter = aLwinter; }

        //!Set accessor function for non-static attribute data member
        public void setLymax(double aLymax) { Lymax = aLymax; }

        //!Set accessor function for non-static attribute data member
        public void setLmin(double aLmin) { Lmin = aLmin; }

        //!Set accessor function for non-static attribute data member
        public void setattenuationCoeff(double aattenuationCoeff) { attenuationCoeff = aattenuationCoeff; }

        //!Set accessor function for non-static attribute data member
        public void setSo(double aSo) { So = aSo; }

        //!Set accessor function for non-static attribute data member
        public void setSoe(double aSoe) { Soe = aSoe; }

        //!Set accessor function for non-static attribute data member
        public void setSf(double aSf) { Sf = aSf; }

        //!Set accessor function for non-static attribute data member
        public void setSfe(double aSfe) { Sfe = aSfe; }

        //!Set accessor function for non-static attribute data member
        void setSr(double aSr) { Sr = aSr; }

        //!Set accessor function for non-static attribute data member
        void setSm(double aSm) { Sm = aSm; }

        //!Set accessor function for non-static attribute data member
        public void setrootDepthWinter(double arootDepthWinter) { rootDepthWinter = arootDepthWinter; }

        //!Set accessor function for non-static attribute data member
        public void setcroot(double acroot) { croot = acroot; }

        //!Set accessor function for non-static attribute data member
        public void setplantTsum(double aplantTsum) { plantTsum = aplantTsum; }

        //!Set accessor function for non-static attribute data member
        public void setactive(bool aactive) { active = aactive; }

        //!Set accessor function for non-static attribute data member
        public void setuseAutumn(bool auseAutumn) { useAutumn = auseAutumn; }

        //!Set accessor function for non-static attribute data member
        public void setuseWinter(bool auseWinter) { useWinter = auseWinter; }



        /*! Performs the daily functions
        \param Tmean current air temperature (Celsius)
        \param winter set to true if the winter functions should be used
        */
        void dailyRoutine(double Tmean, bool winter)
        {
            useWinter = winter;
            if (active)  //this is set to false if there is no crop present
            {
                if (!useWinter)  //this is set to true if the winter crop  routines should be used
                {
                    plantTsum += Tmean;
                    if (!useAutumn)    //this is set to true if the autumn crop routines to be used
                    {
                        if (plantTsum > So)    //crop has emerged
                        {
                            rootDepth += croot;
                            if (Name == "Grass")
                            {
                                Lg = Lmax * (Math.Exp(2.4 * (plantTsum - So) / (Sf - So)) - 1) / 10.0;
                                if (Lg > Lmax)
                                    Lg = Lmax;
                            }
                            else
                            {
                                if (plantTsum < Sr)  //crop has not begun to ripen
                                {
                                    Lg = Lmax * (Math.Exp(2.4 * (plantTsum - So) / (Sf - So)) - 1) / 10.0;
                                    if (Lg > Lmax) //green leaf area is limited to Lmax
                                        Lg = Lmax;
                                }
                                else
                                    if ((plantTsum >= Sr) && (plantTsum < Sm))  //ripening stage
                                    {
                                        Ltot = Lmax - (Lmax - Lymax) * (plantTsum - Sr) / (Sm - Sr);
                                        Lg = Lmax * (1 - (plantTsum - Sr) / (Sm - Sr));
                                        if (Lg < Lmin)
                                            Lg = Lmin;
                                        if (Lg > Lmax)
                                            Lg = Lmax;
                                        Ly = Ltot - Lg;
                                    }
                                    else
                                    {
                                        if (Lmin == 0) //crop is ripe and there is no undersown grass
                                        {
                                            Ltot = Lymax;
                                            Lg = 0.0;
                                        }
                                        else //crop is right and undersown grasses is present
                                        {
                                            Lg = Lmin;
                                            Ltot = Lg + Ly;
                                        }
                                    }

                            }
                        }
                    }
                    else  //this is the autumn
                    {
                        if (plantTsum > Soe)    //autumn sown crop has emerged
                        {
                            rootDepth += croot;
                            Lg = Lmax * (Math.Exp(2.4 * (plantTsum - Soe) / (Sfe - Soe)) - 1) / 10.0;
                            if (Lg > Lmax)
                                Lg = Lmax;
                        }
                    }
                }
                else  //this is the winter
                {
                    useAutumn = false;
                    rootDepth += croot;
                    if (rootDepth > rootDepthWinter)
                        rootDepth = rootDepthWinter;
                    if (Lg > 0.0)  //if Lg > 0.0, crop already established
                    {
                        Lg = Lwinter;
                        plantTsum = So + Math.Log((10.0 * Lg / Lmax) + 1) * (Sf - So) / 2.4;
                    }
                }
                breakpoint = 0.5;
                Ltot = Lg + Ly;
            }
        }
        /*! Initialises the parameters of the crop model
        \param thecropChars pointer to an instance containing the crop parameters
        */
        void loadCropChars(cropCharClass thecropChars)  
{

	Name=thecropChars.getName();
	Lg=0.0;
   Ltot=Lg;
   Ly=0.0;
   plantTsum = 0.0;
   rootDepth = 0.0;
   attenuationCoeff = thecropChars.getattenuationCoeff();
   Lmax = thecropChars.getLmax();
   Lwinter = thecropChars.getLwinter();
   Lymax = thecropChars.getLymax();
   Lmin = thecropChars.getLmin();
   So = thecropChars.getSo();
   Soe = thecropChars.getSoe();
   Sf = thecropChars.getSf();
   Sfe = thecropChars.getSfe();
   Sr = thecropChars.getSr();
   Sm = thecropChars.getSm();
	rootDepthWinter = thecropChars.getrootDepthWinter();
	croot= thecropChars.getcroot();
   breakpoint = 0.5;
   active = true;
}
        /*! Outputs the crop variables to file
        \param afile pointer to the output file
        \param header set to true if only the variable names should be printed
        */
        void outputDetails()
        {
            GlobalVars.Instance.writeStartTab("simplePlant");
            GlobalVars.Instance.writeInformationToFiles("Lg", " Lg", "-", Lg);
            GlobalVars.Instance.writeInformationToFiles("Ly", " Ly", "-", Ly);
            GlobalVars.Instance.writeInformationToFiles("rootDepth", "rootDepth", "-", rootDepth);
            GlobalVars.Instance.writeInformationToFiles("plantTsum", " plantTsum", "-", plantTsum);

            GlobalVars.Instance.writeEndTab();

        }
        /*! Deletes all existing crop parameters
        \param autumn set to true if the autumn functions should be implemented
        */
        void Initialise(bool autumn)
        {
            Name ="No Crop";
            kill();
            useAutumn = autumn;
        }
        //! Sets all plant parameters to 0
        void kill()  
{

	Name = "No Crop";
	Lg=0.0;
   Ltot=0.0;
	Ly=0.0;
   rootDepth = 0.0;
   Lmax = 0.0;
   Lwinter =0.0;
   Lymax = 0.0;
   So = 0.0;
   Soe = 0.0;
   Sf = 0.0;
   Sfe = 0.0;
   Sr = 0.0;
   Sm = 0.0;
   active = false;
}
        /*! Harvests the crop
        \param residualLAI leaf area index of the crop post harvesting
        */
        void harvest(double residualLAI)
        {
            if (Lmin == 0.0) //there is no undersown grass
                kill();
            else
            {
               
                Name = "Grass";
                Lg = Lmin;
                if (Lg > residualLAI)  //some of the undersown grass may be harvested along with the cereal crop
                {
                    Lg = residualLAI;
                    Lmin = Lg;
                    Ly = 0.0;
                }
                else
                    Ly = residualLAI - Lg;
                Ltot = Lg + Ly;
                plantTsum = So + Math.Log((10.0 * Lg / Lmax) + 1) * (Sf - So) / 2.4;
                rootDepth = rootDepthWinter;
            }
        }
        /*! Cuts a grass crop
        \param residualLAI leaf area index of the crop post cutting
        */
        void cut(double residualLAI)
        {
            if (Lg > residualLAI)
            {
                Lg = residualLAI;
                Ltot = Lg;
                if (Lg > 0.0)
                    plantTsum = So + Math.Log((10.0 * Lg / Lmax) + 1) * (Sf - So) / 2.4;
                else
                    plantTsum = 0.0;
            }
        }




    }
}