/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// ============================================================================
// Soil layer
// (c) J�rgen E. Olesen, Statens Planteavlsfors�g
// Changes:
// JEO 24-06-1997: Support for initialization of added organic carbon and nitrogen
// JEO 01-07-1997: Nutrient contents in AOM changed from percent to relative
// JEO 18-07-1997: Error in MaxNitrogenUptake corrected.
// JEO 27-10-1997: New soil evaporation formula
// CHD 12-06-2004: Added denitrification and N2O emission model
// =============================================================================

#include "common.h"
#include "soillayr.h"
#include <timeuse.h>
#include <bstime.h>
#include <message.h>

const double HeatCapacityFreezingWater = 334400.0;  // J/kg
const double HeatCapacityWater         = 4192.0;    // J/kgC
const double HeatCapacityIce           = 2000.0;    // J/kgC
const double HeatCapacitySolid         = 750.0;     // J/kgC
const double WaterDensity              = 1000.0;    // kg/m3

/****************************************************************************\
\****************************************************************************/
soilLayer::soilLayer(const char * aName, const int aIndex, const base * aOwner)
 : base(aName,aIndex,aOwner)
{
   next                     = NULL;
   pF                       = NULL;
   OrganicMatter            = new organicMatter("OrganicMatter",0,this);
   ammoniumLeaching.Clear();
   nitrateLeaching.Clear();
   nitrate_mob.Clear();
   nitrate_imob.Clear();
   ammonium_mob.Clear();
   ammonium_imob.Clear();
   N2OFromNitrification.Clear();
   N2OFromDenitrification.Clear();
   Denitrification.Clear();
   waterContent             = 0;
   temperature              = 8;
   waterFlux                = 0.0;                     // mm
   iceContent               = 0.0;
   TillageEffectTemperature = 0.0;
   equilibrated             = false;
   clayContent              = 0;
   Chloride_mob             = 0;
   Chloride_imob            = 0;
   drainageConstant         = 0;
   holdbackConstant         = 0;
   dryBulkDensity           = 0;
   porosity                 = 0;
   fieldCapacity            = 0;
   startDepth               = 0;
   thickness                = 0;
   wiltCapacity             = 0;
   nitrificationRate        = 0;
   tortuosityLimit          = 0;
   tortuosityFactor         = 0;
   waterUptake              = 0;
   CO2Evolution             = 0;
   mobility                 = 0;
   mobility1                = 0;
   Nbudget.SetNames("soillayer","N");
   N15budget.SetNames("soillayer","N15");
   WaterBudget.SetNames("soillayer","water");
}

/****************************************************************************\
\****************************************************************************/
soilLayer::soilLayer(const soilLayer& Layer)
: base(Layer)
{
 next = NULL;
 if (&Layer)
 {
  TillageEffectTemperature = Layer.TillageEffectTemperature;
  waterContent             = Layer.waterContent;
  nitrate_mob              = Layer.nitrate_mob;
  nitrate_imob             = Layer.nitrate_imob;
  ammonium_mob             = Layer.ammonium_mob;
  ammonium_imob            = Layer.ammonium_imob;
  temperature              = Layer.temperature;
  drainageConstant         = Layer.drainageConstant;
  holdbackConstant         = Layer.holdbackConstant;
  dryBulkDensity           = Layer.dryBulkDensity;
  porosity                 = Layer.porosity;
  fieldCapacity            = Layer.fieldCapacity;
  startDepth               = Layer.startDepth;
  thickness                = Layer.thickness;
  wiltCapacity             = Layer.wiltCapacity;
  clayContent              = Layer.clayContent;
  nitrificationRate        = Layer.nitrificationRate;
  tortuosityLimit          = Layer.tortuosityLimit;
  tortuosityFactor         = Layer.tortuosityFactor;
  waterUptake              = Layer.waterUptake;
  waterFlux                = Layer.waterFlux;
  CO2Evolution             = Layer.CO2Evolution;
  ammoniumLeaching         = Layer.ammoniumLeaching;
  nitrateLeaching          = Layer.nitrateLeaching;
  iceContent               = Layer.iceContent;
  equilibrated             = Layer.equilibrated;
  Chloride_mob             = Layer.Chloride_mob;
  Chloride_imob            = Layer.Chloride_imob;
  mobility                 = Layer.mobility;
  mobility1                = Layer.mobility1;
  N2OFromNitrification     = Layer.N2OFromNitrification;
  N2OFromDenitrification   = Layer.N2OFromDenitrification;
  Denitrification          = Layer.Denitrification;
  OrganicMatter            = new organicMatter(*Layer.OrganicMatter);
  OrganicMatter->SetOwner(this);
  pF                       = new pF_Curve(*Layer.pF);
  Nbudget			         = budget(Layer.Nbudget);
  Nbudget.Reset();         // History ignored!
  N15budget			         = budget(Layer.N15budget);
  N15budget.Reset();       // History ignored!
  WaterBudget 	            = budget(Layer.WaterBudget);
  WaterBudget.Reset();     // History ignored!
 }
}

/****************************************************************************\
pF curves are assumed to be equal. This assumption is not tested though.
\****************************************************************************/
void soilLayer::Add(soilLayer* Layer, double fraction)
{
 if (Layer)
 {
  TillageEffectTemperature= TillageEffectTemperature*(1.0-fraction)+Layer->TillageEffectTemperature*fraction;
  nitrate_mob 				= nitrate_mob*(1.0-fraction)+Layer->nitrate_mob*fraction;
  nitrate_imob 			= nitrate_imob*(1.0-fraction)+Layer->nitrate_imob*fraction;
  ammonium_mob 			= ammonium_mob*(1.0-fraction)+Layer->ammonium_mob*fraction;
  ammonium_imob 			= ammonium_imob*(1.0-fraction)+Layer->ammonium_imob*fraction;
  ammoniumLeaching 		= ammoniumLeaching*(1.0-fraction)+Layer->ammoniumLeaching*fraction;
  nitrateLeaching 		= nitrateLeaching*(1.0-fraction)+Layer->nitrateLeaching*fraction;
  N2OFromNitrification  = N2OFromNitrification*(1.0-fraction)+Layer->N2OFromNitrification*fraction;
  N2OFromDenitrification= N2OFromDenitrification*(1.0-fraction)+Layer->N2OFromDenitrification*fraction;
  Denitrification       = Denitrification*(1.0-fraction)+Layer->Denitrification*fraction;

  waterContent 			= (1.0-fraction)*waterContent+fraction*Layer->waterContent;
  temperature 				= (1.0-fraction)*temperature+fraction*Layer->temperature;
  drainageConstant 		= (1.0-fraction)*drainageConstant+fraction*Layer->drainageConstant;
  holdbackConstant 		= (1.0-fraction)*holdbackConstant+fraction*Layer->holdbackConstant;
  dryBulkDensity 			= (1.0-fraction)*dryBulkDensity+fraction*Layer->dryBulkDensity;
  porosity 					= (1.0-fraction)*porosity+fraction*Layer->porosity;
  fieldCapacity 			= (1.0-fraction)*fieldCapacity+fraction*Layer->fieldCapacity;
  startDepth 				= (1.0-fraction)*startDepth+fraction*Layer->startDepth;
  thickness 				= (1.0-fraction)*thickness+fraction*Layer->thickness;
  wiltCapacity         	= (1.0-fraction)*wiltCapacity+fraction*Layer->wiltCapacity;
  clayContent          	= (1.0-fraction)*clayContent+fraction*Layer->clayContent;
  nitrificationRate    	= (1.0-fraction)*nitrificationRate+fraction*Layer->nitrificationRate;
  tortuosityLimit 		= (1.0-fraction)*tortuosityLimit+fraction*Layer->tortuosityLimit;
  tortuosityFactor 		= (1.0-fraction)*tortuosityFactor+fraction*Layer->tortuosityFactor;
  waterUptake 				= (1.0-fraction)*waterUptake+fraction*Layer->waterUptake;
  waterFlux 				= (1.0-fraction)*waterFlux+fraction*Layer->waterFlux;
  CO2Evolution 			= (1.0-fraction)*CO2Evolution+fraction*Layer->CO2Evolution;
  iceContent 				= (1.0-fraction)*iceContent+fraction*Layer->iceContent;
  Chloride_mob          = (1.0-fraction)*Chloride_mob+fraction*Layer->Chloride_mob;
  Chloride_imob         = (1.0-fraction)*Chloride_imob+fraction*Layer->Chloride_imob;
  mobility              = (1.0-fraction)*mobility+fraction*Layer->mobility;
  mobility1             = (1.0-fraction)*mobility1+fraction*Layer->mobility1;

  Nbudget.Add(Layer->Nbudget,fraction);
  N15budget.Add(Layer->N15budget,fraction);
  WaterBudget.Add(Layer->WaterBudget,fraction);
  OrganicMatter->Add(Layer->OrganicMatter,fraction);
 }
}

/****************************************************************************\
\****************************************************************************/
soilLayer::~soilLayer()
{
   delete OrganicMatter;
   if (pF)
      delete pF;
   pF = NULL;
}

/****************************************************************************\
\****************************************************************************/
void soilLayer::Initialize(double startDep,
									double thick,
									fstream * f)
{
   Setfile(f);
   startDepth          = startDep;
   thickness           = thick;
   double center       = startDep+0.5*thick;
   bool foundLayer=false;
   int n,first;
   SetCritical();
   GetSectionNumbers("SoilLayer",&first,&n);
   int i=first;
   double topOfLayer=0.0;
   while (!foundLayer && (i<(first+n)))
   {
      FindSection("SoilLayer",i);
      double totalThickness;
      GetParameter("Thickness",&totalThickness);
      if (totalThickness<=0.0)
      	Terminate("soilLayer::Initialize - Thickness must be above zero");
      foundLayer=(center>=topOfLayer) && (center<=(topOfLayer+totalThickness));
      if (foundLayer)
      {
         UnsetCritical();
         int soilType=5;
         GetParameter("SoilType",&soilType);
         GetParameter("Ammonium",&ammonium_imob.n);
         ammonium_imob=ammonium_imob*thickness/totalThickness;
         ammonium_mob.n        = 0.0;
         GetParameter("Nitrate",&nitrate_imob.n);
         nitrate_imob=nitrate_imob*thickness/totalThickness;
         nitrate_mob.Clear();
         double siltContent;
         GetParameter("SiltContent",&siltContent);
         if (!GetParameter("ClayContent",&clayContent))
         {
            double JBC[] = {0.025,0.025,0.075,0.075,0.125,0.125,0.20,0.35,0.50,0.10};
            int JB = 5;
            if (soilType>=1 && soilType<=10)
            	JB = soilType;
            clayContent= JBC[JB-1];
         }
         if (!GetParameter("DrainageConstant",&drainageConstant))
         {
            drainageConstant=1.0271-3.02*clayContent*clayContent;   // Function from Addiscot & Whitmore 1991, Soil Use. Man., 94-102
            drainageConstant=min(1.0,max(0.1,drainageConstant));    // Cut at [0.1;1.0]
            drainageConstant=min(1.0,drainageConstant*15.462*pow(thickness,-0.7)); // Fitted to Fig. 2c, Addiscot & Whitmore 1991
         }
         if (!GetParameter("HoldbackConstant",&holdbackConstant))
         {
            holdbackConstant=0.1;
            if (startDep>199)
             	holdbackConstant=0.3;
         }

         if (!GetParameter("DryBulkDensity",&dryBulkDensity))
          	dryBulkDensity=1.5;
         porosity = 1.0-dryBulkDensity/2.65;
         dryBulkDensity=dryBulkDensity*1E6;
         double carbon=0.0;
         GetParameter("CarbonPct",&carbon);

			bool TopSoil = (startDep<250);
         pF = new pF_Curve(0); // Interpolation method set to zero. Correct ????!!!!!!!!!!!!!!!
         pF->ReadParameters(f,i,dryBulkDensity,clayContent,siltContent,carbon,TopSoil);          // Extra parameters added for support of Mualem van Genucten model
         double pF_FC = 2.0;
         /* Uverificeret ???!!!
         if (soilType==1)
         	pF_FC = 1.8;
         */
         fieldCapacity = pF->GetRelativeWater(pF_FC)*thickness;
         waterContent = fieldCapacity;
         double MaxContent = porosity*thickness;
         if (fieldCapacity>MaxContent)
         	porosity = pF->GetRelativeWater(0.0);        // HACK !!!!!???
         if (porosity<=.9*pF->GetRelativeWater(0.0))
         	porosity = pF->GetRelativeWater(0.0);        // HACK !!!!!???

         wiltCapacity = pF->GetRelativeWater(4.2)*thickness;
         nitrificationRate    = 0.1;
         tortuosityLimit      = 0.1;
         tortuosityFactor     = 2.0;
         carbon=carbon*dryBulkDensity*thickness/10E4;
         char sfn[80]="cnmodel.dat";
         GetParameter("SomFileName",sfn);
         OrganicMatter->Initialize(clayContent,carbon,f,i,sfn);
      }
      topOfLayer+=totalThickness;
      i++;
   }
   if (!foundLayer)
    Terminate("soilLayer::Initialize - no appropriate layer found");
   Setfile(NULL);

   mobility = 0.4;
   mobility1 = 0.25;
   if (OpenInputFile("\\FASSET\\CropParameters.dat"))
   {
      FindSection("Soil");
      GetParameter("Mobility",&mobility);
      GetParameter("Mobility1",&mobility1);
	   CloseInputFile();
   }
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::GetFrozenPart()
{
   double retVal=iceContent/waterContent;
   if (iceContent>waterContent+0.000001)
      theMessage->WarningWithDisplay("soilLayer::GetFrozenPart - to much ice relative to water");
   if (iceContent<-0.000001)
      theMessage->WarningWithDisplay("soilLayer::GetFrozenPart - ice content below zero");
   if (waterContent<-0.000001)
      theMessage->WarningWithDisplay("soilLayer::GetFrozenPart - water content below zero");
   return max(0.0,min(1.0,retVal));
}

/****************************************************************************\
\****************************************************************************/
void soilLayer::SetIceContent(double aIceContent)
{
   if (aIceContent>waterContent)
      theMessage->WarningWithDisplay("soilLayer::SetIceContent - requsted ice content exceeds total amount of water");
/*   if (aIceContent<0)
      theMessage->WarningWithDisplay("soilLayer::SetIceContent - requsted ice content is negative");
!!!!!!!!!!!!!!!!!!!!!!!!!! PUT BACK !!!!!!!!!!!!!! */
   iceContent = max(0.0,min(waterContent,aIceContent));
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::GetCenterDepth()
{
   double retVal=startDepth+thickness/2.0;
   if (retVal<0)
      theMessage->FatalError("soilLayer::GetCenterDepth() - center depth of a soil layer can not be negative");
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilLayer::GetTotalNitrogen()
{
   nitrogen TotalNitrogen = ammonium_mob+ammonium_imob+nitrate_mob+nitrate_imob;
   TotalNitrogen = TotalNitrogen+OrganicMatter->GetNitrogen();
   return TotalNitrogen;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilLayer::GetPoolNitrogen(const char * Name,double startDepth,double thickness)
{
	return OrganicMatter->GetPoolNitrogen(Name)*IntervalFraction(startDepth,thickness);
}

/****************************************************************************\
\****************************************************************************/
void soilLayer::AddNext(soilLayer * layer)
{
 next = layer;
}

/****************************************************************************\
Adds nitrogen to layer
addNitrate    - Nitrate to be added [g N/m�]
addAmmonium   - Ammonium to be added [g N/m�]
\****************************************************************************/
void soilLayer::AddNutrient(nitrogen addNitrate, nitrogen addAmmonium)
{
	if (waterContent > fieldCapacity)
   {
   	nitrate_mob = nitrate_mob + addNitrate;
   	ammonium_mob = ammonium_mob + addAmmonium;
   }
   else
   {
   	nitrate_imob = nitrate_imob + addNitrate;
      ammonium_imob = ammonium_imob + addAmmonium;
   }
   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::AddNutrient - negative n concentrations");

   Nbudget.AddInput(addNitrate.n+addAmmonium.n);
   N15budget.AddInput(addNitrate.n15+addAmmonium.n15);
}
/****************************************************************************\
Adds chloride to layer (g-CL m-2)
\****************************************************************************/
void soilLayer::AddChloride(double Amount)
{
 	if (waterContent > fieldCapacity)
      Chloride_mob += Amount;
   else
     	Chloride_imob += Amount;
}

/****************************************************************************\
Calculates the volumetric heatcapacity
\****************************************************************************/
double soilLayer::HeatCapacity()    //kg/m3/dgC
{
	return (HeatCapacitySolid*dryBulkDensity/1000
			  +HeatCapacityWater*WaterDensity*(waterContent-iceContent)/thickness
			  +HeatCapacityIce*WaterDensity*iceContent/thickness);
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::SpecWaterCapacity()
{
	double relativeWater = waterContent/thickness;
   double specWater;
   double pF1;
   pF1 = pF->GetPressurePotential(relativeWater);
   if (waterContent<porosity*thickness)
      if (pF1==pF->GetPressurePotential(relativeWater-0.01))
   		specWater = 1e10;
      else
      	specWater = 0.01/(pF1-pF->GetPressurePotential(relativeWater-0.01));
   else
   	if (pF->GetPressurePotential(relativeWater+0.01)==pF1)
         specWater = 1e10;                     // er ss forkert
      else
      	specWater = -0.01/(pF1-pF->GetPressurePotential(relativeWater+0.01));
   return 1.1019e-5*specWater;                // converts m to J m-3
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::GetPressurePotential()
{
	double relativeWater = waterContent/thickness;
   return pF->GetPressurePotential(relativeWater);
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilLayer::GetAmmonium(double startDepth,double thickness)
{
	return (ammonium_mob+ammonium_imob)*IntervalFraction(startDepth,thickness);
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilLayer::GetNitrate(double startDepth,double thickness)
{
	return (nitrate_mob+nitrate_imob)*IntervalFraction(startDepth,thickness);
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::GetChloride(double startDepth,double thickness)
{
	return (Chloride_mob+Chloride_imob)*IntervalFraction(startDepth,thickness);
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilLayer::GetOrganicNitrogen(double startDepth, double thickness)
{
	return GetOrganicNitrogen()*IntervalFraction(startDepth,thickness);
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::HeatCapacityFreezing()
{
	return HeatCapacityFreezingWater*waterContent*WaterDensity*0.001;
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::HeatConductivity()
{
   // Taken from SOIL-N (1996 p18 & p24)

   const double a1=0.70000;
   const double a2=0.40000;
   const double a3=0.62450;
	double b1=0.00158;
	double b2=1.336;
	double b3=0.00375;
	double b4=0.9118;
	if (clayContent>0.1)
   {
   	b1=0.00144;
		b2=1.32;
		b3=0.0036;
		b4=0.8743;
	}
   double frozenPart=iceContent/waterContent;
	double waterConcentration = max(1.0,waterContent)*100/thickness;   // converted mm to vol %. Minimum of 1% according to COUP figure p 31
   double BulkDensity = dryBulkDensity/1e6;
   if (waterConcentration<=0)
      theMessage->FatalError("soilLayer::HeatConductivity - water content zero or below");
   double k_hm = 0.1434*(a1*log10(waterConcentration/BulkDensity)+a2)*pow(10.0,a3*BulkDensity);
   double k_hmi = b1*pow(10.0,b2*BulkDensity)+b3*waterConcentration/BulkDensity*pow(10.0,b4*BulkDensity);
   double result = (1-frozenPart)*k_hm+frozenPart*k_hmi;
   result=max(0.05,result);                                          // According to COUP figure p 31
   if (result>10)
   	theMessage->WarningWithDisplay("soilLayer::HeatConductivity - conductivity to high");
	return result;
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::AvailableWater(rootStructure* root)
{
   return (waterContent-thickness*pF->GetRelativeWater(root->rootpF));
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::WaterApparancy(rootStructure* root)
{
   double maxUptake = min(AvailableWater(root),MaxTranspiration(root));
   return min(1.0,maxUptake/waterContent);
}

/****************************************************************************\
Calculates the maximum transpiration from the soil layer
returns       - Maximum transpiration [mm]
\****************************************************************************/
double soilLayer::MaxTranspiration(rootStructure* root)
{
   double radius = root->rootRadius;
   double pF     = root->rootpF;
   double rootLength = root->rootLengthList[Index];
   double maxTranspiration = MaxTranspiration(radius,pF,rootLength);
   return maxTranspiration;
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::MaxTranspiration(double rootRadius,
											  double rootpF,
											  double rootLength)
{
	if (rootLength <= 0)
  		return 0.;
 	else
 	{
  		double rootDens = rootLength/thickness;                            //m m-1
  		double relWater = waterContent/thickness;
   	double UptakeRate = -4.0*M_PI*(pF->GetFluxPotential(pF->GetpF(relWater))-pF->GetFluxPotential(rootpF))/
							 (-log(rootRadius*rootRadius*M_PI*rootDens));
  		if (UptakeRate<=0)
   		return 0.0;
  		double availableWater = waterContent-thickness*pF->GetRelativeWater(rootpF);

      return min(max(0.0,availableWater),1000.*86400.*UptakeRate*rootLength*relWater/porosity);
 	}
}

/****************************************************************************\
Subtracts estimated transpiration from the water in the layer.
aWaterUptake         - water uptake [mm]
\****************************************************************************/
void soilLayer::SubtractTranspiration(double &aWaterUptake)
{
   if (waterContent<aWaterUptake)
   	aWaterUptake=waterContent;
   waterUptake = aWaterUptake;
	waterContent -= waterUptake;
   WaterBudget.AddOutput(aWaterUptake);
}

/****************************************************************************\
Estimate the fraction of mobile water (SLIM concept)
\****************************************************************************/
double soilLayer::FractionMobileWater()
{
	return max(waterContent-fieldCapacity,0.0)/waterContent;
}

/****************************************************************************\
   Perform percolation of water and nitrogen in layer by use of the TETRANS concept
   surplus         - Water to be added to next layer [mm]
   nitrogenLeached - Nitrogen to be added to next layer [g N/m�]
\****************************************************************************/
void soilLayer::ExchangeSubstance(double * waterLeached,nitrogen * nitrateLeached, nitrogen * ammoniumLeached, double* chlorideLeached)
{
   WaterBudget.AddInput(*waterLeached);
   Nbudget.AddInput(nitrateLeached->n+ammoniumLeached->n);
   N15budget.AddInput(nitrateLeached->n15+ammoniumLeached->n15);

   nitrogen leachNitrate;
   nitrogen leachAmmonium;
   double leachChloride = 0.0;

   mobility           = 0.05; //max(0.1,0.5-2.0*clayContent); // Default value !!!
   double leachW      = max(*waterLeached+waterContent-fieldCapacity,0.0)*drainageConstant;
   double retainW     = *waterLeached+waterContent-leachW;

	double waterBypass = *waterLeached-(retainW-(1-mobility)*waterContent);
   if (leachW>0.0)
   {
      if (waterBypass>1E-9)
      {

         double frac = min(1.0,max(0.0,(leachW-waterBypass)/waterContent));
         //double frac = min(1.0,max(0.0,(leachW)/waterContent));
         double fAm    = FracAmmoniumInWater();
         leachNitrate  = (nitrate_mob+nitrate_imob)*frac + (*nitrateLeached)*waterBypass/(*waterLeached);
         leachAmmonium = (ammonium_mob+ammonium_imob)*fAm*frac+(*ammoniumLeached)*waterBypass/(*waterLeached);
         leachChloride = (Chloride_mob+Chloride_imob)*frac + (*chlorideLeached)*waterBypass/(*waterLeached);
         /*leachNitrate  = (nitrate_mob+nitrate_imob)*mobility + (*nitrateLeached)*waterBypass/(*waterLeached);
         leachAmmonium = (ammonium_mob+ammonium_imob)*mobility + (*ammoniumLeached)*waterBypass/(*waterLeached);
         leachChloride = (Chloride_mob+Chloride_imob)*mobility + (*chlorideLeached)*waterBypass/(*waterLeached);
         leachNitrate  = nitrate_imob*mobility + nitrate_mob*waterBypass/(*waterLeached);
         leachAmmonium = ammonium_imob*mobility + ammonium_mob*waterBypass/(*waterLeached);
         leachChloride = Chloride_imob*mobility + Chloride_mob*waterBypass/(*waterLeached);

         Chloride_mob = Chloride_mob*(1-waterBypass/(*waterLeached));
         Chloride_imob = Chloride_imob*(1-mobility);
         nitrate_mob = nitrate_mob*(1-waterBypass/(*waterLeached));
         nitrate_imob = nitrate_imob*(1-mobility);
         ammonium_mob = ammonium_mob*(1-waterBypass/(*waterLeached));
         ammonium_imob = ammonium_imob*(1-mobility);  */
      }
      else
      {
         double frac = min(1.0,max(0.0,leachW/waterContent));
         leachNitrate  = (nitrate_mob+nitrate_imob)*frac;
         leachAmmonium = AccesibleAmmonium()*frac;
         leachChloride = (Chloride_mob+Chloride_imob)*frac;
/*         double ClRemain = Chloride_mob+Chloride_imob-leachChloride;
         Chloride_mob = ClRemain*(1-mobility);
         Chloride_imob = ClRemain*mobility;

         nitrogen NO3Remain = nitrate_mob+nitrate_imob-leachNitrate;
         nitrate_mob = NO3Remain*(1-mobility);
         nitrate_imob = NO3Remain*mobility;

         nitrogen NH4Remain = ammonium_mob+ammonium_imob-leachAmmonium;
         ammonium_mob = NH4Remain*(1-mobility);
         ammonium_imob = NH4Remain*mobility;
*/
      }
   }
   waterContent += *waterLeached-leachW;

   double frac = 0.0;
   if (nitrate_mob.n>0) frac = nitrate_mob/(nitrate_mob+nitrate_imob);
   nitrate_mob = nitrate_mob + (*nitrateLeached - leachNitrate)*frac;
   nitrate_imob = nitrate_imob + (*nitrateLeached - leachNitrate)*(1.0-frac);

   frac = 0.0;
   if (ammonium_mob.n>0) frac = ammonium_mob/(ammonium_mob+ammonium_imob);
   ammonium_mob = ammonium_mob + (*ammoniumLeached - leachAmmonium)*frac;
   ammonium_imob = ammonium_imob + (*ammoniumLeached - leachAmmonium)*(1.0-frac);

   *waterLeached = leachW;
   *nitrateLeached = leachNitrate;
   *ammoniumLeached = leachAmmonium;
   *chlorideLeached = leachChloride;

   ammoniumLeaching = ammoniumLeaching + *ammoniumLeached;
   nitrateLeaching  = nitrateLeaching + *nitrateLeached;
   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::ExchangeSubstance - negative n concentrations");

   WaterBudget.AddOutput(*waterLeached);
   SetWaterFlux(*waterLeached);
   Nbudget.AddOutput(nitrateLeached->n+ammoniumLeached->n);
   N15budget.AddOutput(nitrateLeached->n15+ammoniumLeached->n15);
}

/****************************************************************************\
Perform infiltration of water, nitrate and ammonium in layer.
surplus         - Water to be added to current (next) layer [mm]
nitrateLeached  - Nitrate to the added to current (next) layer [g N/m�]
ammoniumLeached - Ammonium to the added to current (next) layer [g N/m�]
\****************************************************************************/
void soilLayer::AddWater(double surplus,
                         nitrogen nitrateLeached,
								 nitrogen ammoniumLeached,
                         double ChlorideLeached)
{
   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::AddWater - negative n concentrations");
	if (surplus<0)
   	theMessage->WarningWithDisplay("soilLayer::AddWater - amount of water to add can not be negative");
   if (waterContent<fieldCapacity)
   {
      double f = 0.0;
      // Nitrate
      if ((nitrate_mob+nitrate_imob).n>0.0)
      	f = max(0.0,min(1.0,nitrate_mob/(nitrate_mob+nitrate_imob)));
#ifdef NEWSLIM
      f=1.0; //FractionMobileWater();
#endif
      nitrate_mob  = nitrate_mob+nitrateLeached*f;
      nitrate_imob = nitrate_imob+nitrateLeached*(1.0-f);

      // Ammonium
      f = 0.0;
      if ((ammonium_mob.n+ammonium_imob.n)>0.0)
      	f = max(0.0,min(1.0,ammonium_mob/(ammonium_mob+ammonium_imob)));
#ifdef NEWSLIM
      f=1.0; //FractionMobileWater();
#endif
      ammonium_mob  = ammonium_mob+ammoniumLeached*f;
      ammonium_imob = ammonium_imob+ammoniumLeached*(1.0-f);

      // Chloride
      Chloride_mob  += ChlorideLeached;
   }
   else
   {
      nitrate_mob  = nitrate_mob+nitrateLeached;
      ammonium_mob = ammonium_mob+ammoniumLeached;
      Chloride_mob += ChlorideLeached;
   }
   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::AddWater - negative n concentrations");

   waterContent  += surplus;
   WaterBudget.AddInput(surplus);
   Nbudget.AddInput(nitrateLeached.n+ammoniumLeached.n);
   N15budget.AddInput(nitrateLeached.n15+ammoniumLeached.n15);
   if((waterContent+0.001)<iceContent)
   	theMessage->WarningWithDisplay("soilLayer::AddWater - ice exceeds total water at the end of the function");
}

/****************************************************************************\
Adds water, nitrate, ammonium and chloride to layer.
surplus         - Water to be added to current (next) layer [mm]
nitrateLeached  - Nitrate to the added to current (next) layer [g N/m�]
ammoniumLeached - Ammonium to the added to current (next) layer [g N/m�]
\****************************************************************************/
void soilLayer::AddWaterAndSolutes(double waterInFlow,
                         nitrogen nitrateAdd,
								 nitrogen ammoniumAdd,
                         double chlorideAdd)
{
	double f=0.0;  //fraction of waterInFlow that is directed to mobile domain

/*	if ((waterContent<fieldCapacity) && (waterInFlow>0.0))
   	f=min(1.0,max(0.0,(waterInFlow-(waterContent-fieldCapacity))/waterInFlow));
   else */   //!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   	f=1.0;

   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::AddWaterAndSolutes - negative n concentrations");
	if (waterInFlow<0)
   	theMessage->WarningWithDisplay("soilLayer::AddWaterAndSolutes - amount of water to add can not be negative");

		//Nitrate
		nitrate_mob = nitrate_mob+nitrateAdd*f;
      nitrate_imob = nitrate_imob+nitrateAdd*(1.0-f);
      // Ammonium
     	ammonium_mob  = ammonium_mob+ammoniumAdd*f;
     	ammonium_imob  = ammonium_imob+ammoniumAdd*(1.0-f);
      // Chloride
      Chloride_mob  += chlorideAdd*f;
      Chloride_imob  += chlorideAdd*(1.0-f);


   waterContent += waterInFlow;
   WaterBudget.AddInput(waterInFlow);
   Nbudget.AddInput(nitrateAdd.n+ammoniumAdd.n);
   N15budget.AddInput(nitrateAdd.n15+ammoniumAdd.n15);
   if((waterContent+0.001)<iceContent)
   	theMessage->WarningWithDisplay("soilLayer::AddWaterAndSolutes - ice exceeds total water at the end of the function");
}

/****************************************************************************\
Perform infiltration of water, nitrate and ammonium in layer.
Used with the convection dispersion equation.
The fluxes of nitrate ammonium and chloride can be negative.
waterOutFlow   - Water to be revoved from current layer [mm]
nitrateRemove  - Nitrate to be revomed from current layer [g N/m�]
ammoniumRemove - Ammonium to be revomed from current layer [g N/m�]
\****************************************************************************/
void soilLayer::RemoveWaterAndSolutes(double waterOutflow,
                            		 nitrogen nitrateRemove,
								    		 nitrogen ammoniumRemove,
                            		 double ChlorideRemove)
{

   waterContent-= waterOutflow;
   WaterBudget.AddOutput(waterOutflow);

   nitrate_mob = nitrate_mob-nitrateRemove;
   ammonium_mob  = ammonium_mob-ammoniumRemove;
   Chloride_mob  -= ChlorideRemove;

   Nbudget.AddOutput(ammoniumRemove.n+nitrateRemove.n);
   N15budget.AddOutput(ammoniumRemove.n15+nitrateRemove.n15);

   if (waterContent<0)
   	theMessage->FatalError("soilLayer::RemoveWaterAndSolutes - water content below zero");
   if((nitrate_mob.n+nitrate_imob.n+ammonium_mob.n+ammonium_imob.n)<-1E-6)
   	theMessage->FatalError("soilLayer::RemoveWaterAndSolutes - mineral nitrogen content below zero");
   if((waterContent+0.001)<iceContent)
   	theMessage->WarningWithDisplay("soilLayer::RemoveWaterAndSolutes - ice exceeds total water");
   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::RemoveWaterAndSolutes - negative n concentrations");
}

/****************************************************************************\
Intra-layer movement of water and solute
\****************************************************************************/
void soilLayer::EquilibrateNitrogen(double mobileWater)
{
   double f=0.0;
   double immobileWater=waterContent-mobileWater;
   if (waterContent<0.0)
   	theMessage->FatalError("soilLayer::EquilibrateNitrogen - water content below zero");
   if (waterContent>0.0)
   	f = max(0.0,min(1.0,mobileWater/waterContent));

   // Nitrate
#ifdef NEWSLIM
	double rateCoefficient=0.9; //0.9;  holdbackconstant =0.1
   if (startDepth>199)     // 199
   	rateCoefficient=0.7; //0.7;      holdbackconstant =0.3
#endif

/*   nitrogen Add  = (nitrate_mob+nitrate_imob)*(1.0-holdbackConstant);       // Holdback constant burde afh�nge af antallet af lag!
   nitrate_mob   = nitrate_mob*holdbackConstant + Add*f;
   nitrate_imob  = nitrate_imob*holdbackConstant + Add*(1.0-f);

   // Ammonium
   double fAm    = FracAmmoniumInWater();
   Add           = (ammonium_mob+ammonium_imob)*(1.0-holdbackConstant);
   ammonium_mob  = ammonium_mob*holdbackConstant + Add*f*fAm;
   ammonium_imob = ammonium_imob*holdbackConstant + Add*(1-f*fAm);   */

   if (mobileWater > 0.0)
   {
       // Chloride
   	double concDiffChloride  = Chloride_mob/mobileWater-(Chloride_imob+Chloride_mob)/waterContent;
      double moveChlorideFromMobile=concDiffChloride*mobileWater*(rateCoefficient);
      Chloride_mob   = Chloride_mob - moveChlorideFromMobile;
      Chloride_imob  = Chloride_imob + moveChlorideFromMobile;
      //Nitrate
      nitrogen concDiffNitrate  = nitrate_mob/mobileWater-(nitrate_imob+nitrate_mob)/waterContent;
      nitrogen moveNitrateFromMobile=concDiffNitrate*mobileWater*(rateCoefficient);
      nitrate_mob   = nitrate_mob - moveNitrateFromMobile;
      nitrate_imob  = nitrate_imob + moveNitrateFromMobile;
      //Ammonium
   	double fAm    = FracAmmoniumInWater();
      nitrogen concDiffAmmonium  = ammonium_mob/mobileWater-(ammonium_imob+ammonium_mob)/waterContent;
      nitrogen moveAmmoniumFromMobile=concDiffAmmonium*mobileWater*(rateCoefficient);
      ammonium_mob   = ammonium_mob - moveAmmoniumFromMobile*fAm;
      ammonium_imob  = ammonium_imob + moveAmmoniumFromMobile*fAm;
   }
   else
   {
   Chloride_imob=Chloride_mob+Chloride_imob;       //if no mobile phase is present the mobile solutes are moved to the immobile phase
   Chloride_mob=0.0;
   nitrate_imob=nitrate_mob+nitrate_imob;
   nitrate_mob=0.0;
   ammonium_imob=ammonium_mob+ammonium_imob;
   ammonium_mob=0.0;
   }
/*   // Chloride
   double AddChloride  = (Chloride_mob+Chloride_imob)*(1.-holdbackConstant);       // Holdback constant burde afh�nge af antallet af lag!
   Chloride_mob   = Chloride_mob*holdbackConstant + AddChloride*f;
   Chloride_imob  = Chloride_imob*holdbackConstant + AddChloride*(1.-f);*/

   if(Chloride_mob<-1E-6 || Chloride_imob<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::Equilibrate - negative cloride concentrations");
   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::Equilibrate - negative n concentrations");

   equilibrated  = true;
}

/****************************************************************************\
Perform infiltration of water, nitrate and ammonium in layer.
surplus         - Water to be added to current (next) layer [mm]
nitrateLeached  - Nitrate to the added to current (next) layer [g N/m�]
ammoniumLeached - Ammonium to the added to current (next) layer [g N/m�]
\****************************************************************************/
void soilLayer::RemoveWater(double moveWater,
                            nitrogen * nitrateLeached,
								    nitrogen * ammoniumLeached,
                            double * ChlorideLeached,
                            bool downwardMovement)
{
   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::RemoveWater - negative n concentrations");
   nitrateLeached->Clear();
   ammoniumLeached->Clear();
   if (moveWater>0)
   {
      double FirstMoveFraction = 0.0;                        // Default value 0.5
      double mobileWater = max(waterContent-fieldCapacity,0.0);
      double alpha = 0.0;
      if (mobileWater>0)
         alpha = min(1.0,moveWater/mobileWater);       // As defined in original SLIM

      // Leach first proportion of the water using current mobile solute concentrations
      *nitrateLeached  = nitrate_mob*alpha*FirstMoveFraction;
      nitrate_mob      = nitrate_mob - *nitrateLeached;

      *ammoniumLeached = ammonium_mob*alpha*FirstMoveFraction;
      ammonium_mob     = ammonium_mob - *ammoniumLeached;

      *ChlorideLeached = Chloride_mob*alpha*FirstMoveFraction;
      Chloride_mob -= *ChlorideLeached;

      EquilibrateNitrogen(mobileWater);
      waterContent -= FirstMoveFraction*moveWater;
      mobileWater  -= FirstMoveFraction*moveWater;

      // Leach second proportion of the water using new mobile solute concentrations

      nitrogen N       = nitrate_mob*alpha*(1.0-FirstMoveFraction);
      *nitrateLeached  = *nitrateLeached + N;
      nitrate_mob      = nitrate_mob - N;

      N                = ammonium_mob*alpha*(1.0-FirstMoveFraction);
      *ammoniumLeached = *ammoniumLeached + N;
      ammonium_mob     = ammonium_mob - N;

      *ChlorideLeached += Chloride_mob*alpha*(1.0-FirstMoveFraction);
      Chloride_mob     -= Chloride_mob*alpha*(1.0-FirstMoveFraction);


      waterContent -= (1.0-FirstMoveFraction)*moveWater;
      WaterBudget.AddOutput(moveWater);
   }
   else
   	if (moveWater<0)
   		theMessage->WarningWithDisplay("soilLayer::RemoveWater - amount of water to remove can not be negative");
   if (downwardMovement)
   {
      ammoniumLeaching = ammoniumLeaching + *ammoniumLeached;
      nitrateLeaching  = nitrateLeaching + *nitrateLeached;
   }

   Nbudget.AddOutput(ammoniumLeached->n+nitrateLeached->n);
   N15budget.AddOutput(ammoniumLeached->n15+nitrateLeached->n15);

   if(ammoniumLeaching.n<-1E-6 || nitrateLeaching.n<-1E-6 )
   	theMessage->WarningWithDisplay("soilLayer::RemoveWater - negative leaching");
   if (waterContent<0)
   	theMessage->FatalError("soilLayer::RemoveWater - water content below zero");
   if((nitrate_mob.n+nitrate_imob.n+ammonium_mob.n+ammonium_imob.n)<-1E-6)
   	theMessage->FatalError("soilLayer::RemoveWater - mineral nitrogen content below zero");
   if((waterContent+0.001)<iceContent)
   	theMessage->WarningWithDisplay("soilLayer::RemoveWater - ice exceeds total water");
}

/****************************************************************************\
Perform infiltration of water, nitrate and ammonium in layer. Used for SWAT
water model with hourly timestep
surplus         - Water to be added to current (next) layer [mm]
nitrateLeached  - Nitrate to the added to current (next) layer [g N/m�]
ammoniumLeached - Ammonium to the added to current (next) layer [g N/m�]
\****************************************************************************/
void soilLayer::RemoveWaterHourly(double moveWater,
                            		 nitrogen * nitrateLeached,
								    		 nitrogen * ammoniumLeached,
                            		 double * ChlorideLeached,
                            		 bool downwardMovement)
{
   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::RemoveWater - negative n concentrations");
   nitrateLeached->Clear();
   ammoniumLeached->Clear();
   if (moveWater>0)
   {
      double FirstMoveFraction = 0.0;                        // Default value 0.5. BMP and JBE changed this - very important difference!!!
      double mobileWater = max(waterContent-fieldCapacity,0.0);
      double alpha = 0.0;
      if (mobileWater>0)
         alpha = min(1.0,moveWater/mobileWater);       // As defined in original SLIM

      // Leach first proportion of the water using current mobile solute concentrations
      *nitrateLeached  = nitrate_mob*alpha*FirstMoveFraction;
      nitrate_mob      = nitrate_mob - *nitrateLeached;

      *ammoniumLeached = ammonium_mob*alpha*FirstMoveFraction;
      ammonium_mob     = ammonium_mob - *ammoniumLeached;

      *ChlorideLeached = Chloride_mob*alpha*FirstMoveFraction;
      Chloride_mob -= *ChlorideLeached;

      EquilibrateNitrogen(mobileWater);
      waterContent -= FirstMoveFraction*moveWater;
      mobileWater  -= FirstMoveFraction*moveWater;

      // Leach second proportion of the water using new mobile solute concentrations

      nitrogen N       = nitrate_mob*alpha*(1.0-FirstMoveFraction);
      *nitrateLeached  = *nitrateLeached + N;
      nitrate_mob      = nitrate_mob - N;

      N                = ammonium_mob*alpha*(1.0-FirstMoveFraction);
      *ammoniumLeached = *ammoniumLeached + N;
      ammonium_mob     = ammonium_mob - N;

      *ChlorideLeached += Chloride_mob*alpha*(1.0-FirstMoveFraction);
      Chloride_mob     -= Chloride_mob*alpha*(1.0-FirstMoveFraction);


      waterContent -= (1.0-FirstMoveFraction)*moveWater;
      WaterBudget.AddOutput(moveWater);
   }
   else
   	if (moveWater<0)
   		theMessage->WarningWithDisplay("soilLayer::RemoveWater - amount of water to remove can not be negative");
   if (downwardMovement)
   {
      ammoniumLeaching = ammoniumLeaching + *ammoniumLeached;
      nitrateLeaching  = nitrateLeaching + *nitrateLeached;
   }

   Nbudget.AddOutput(ammoniumLeached->n+nitrateLeached->n);
   N15budget.AddOutput(ammoniumLeached->n15+nitrateLeached->n15);

   if(ammoniumLeaching.n<-1E-6 || nitrateLeaching.n<-1E-6 )
   	theMessage->WarningWithDisplay("soilLayer::RemoveWater - negative leaching");
   if (waterContent<0)
   	theMessage->FatalError("soilLayer::RemoveWater - water content below zero");
   if((nitrate_mob.n+nitrate_imob.n+ammonium_mob.n+ammonium_imob.n)<-1E-6)
   	theMessage->FatalError("soilLayer::RemoveWater - mineral nitrogen content below zero");
   if((waterContent+0.001)<iceContent)
   	theMessage->WarningWithDisplay("soilLayer::RemoveWater - ice exceeds total water");
}

/****************************************************************************\
Perform infiltration of water, nitrate and ammonium in layer.
surplus         - Water to be added to current (next) layer [mm]
nitrateLeached  - Nitrate to the added to current (next) layer [g N/m�]
ammoniumLeached - Ammonium to the added to current (next) layer [g N/m�]
\****************************************************************************/
void soilLayer::RemoveWaterSimple(double moveWater,
                                  nitrogen * nitrateLeached,
                                  nitrogen * ammoniumLeached,
                                  double * ChlorideLeached,
                                  bool downwardMovement)
{
   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6)
   	theMessage->WarningWithDisplay("soilLayer::RemoveWater - negative n concentrations");
   nitrateLeached->Clear();
   ammoniumLeached->Clear();
   if (moveWater>0)
   {
      double mobileWater = max(waterContent-fieldCapacity,0.0);
      if (moveWater>mobileWater)
      	theMessage->FatalError("soilLayer::RemoveWaterSimple - more water than possible requested");
      double alpha = 0.0;
      if (mobileWater>0)
         alpha = min(1.0,moveWater/mobileWater);
      nitrogen N       = nitrate_mob*alpha;
      *nitrateLeached  = *nitrateLeached + N;
      nitrate_mob      = nitrate_mob - N;

      N                = ammonium_mob*alpha;
      *ammoniumLeached = *ammoniumLeached + N;
      ammonium_mob     = ammonium_mob - N;

      *ChlorideLeached += Chloride_mob*alpha;
      Chloride_mob     -= Chloride_mob*alpha;

      waterContent -= moveWater;
      WaterBudget.AddOutput(moveWater);
   }
   else
   	if (moveWater<0)
   		theMessage->WarningWithDisplay("soilLayer::RemoveWater - amount of water to remove can not be negative");
   if (downwardMovement)
   {
      ammoniumLeaching = ammoniumLeaching + *ammoniumLeached;
      nitrateLeaching  = nitrateLeaching + *nitrateLeached;
   }

   Nbudget.AddOutput(ammoniumLeached->n+nitrateLeached->n);
   N15budget.AddOutput(ammoniumLeached->n15+nitrateLeached->n15);

   if(ammoniumLeaching.n<-1E-6 || nitrateLeaching.n<-1E-6 )
   	theMessage->WarningWithDisplay("soilLayer::RemoveWater - negative leaching");
   if (waterContent<0)
   	theMessage->FatalError("soilLayer::RemoveWater - water content below zero");
   if((nitrate_mob.n+nitrate_imob.n+ammonium_mob.n+ammonium_imob.n)<-1E-6)
   	theMessage->FatalError("soilLayer::RemoveWater - mineral nitrogen content below zero");
}

/****************************************************************************\
Subtracts evaporation from layer
evaporation   - Evaporation to be subtracted [mm].
\****************************************************************************/
void soilLayer::SubtractEvaporation(double evaporation)
{
 double draw = max(0.0,min(waterContent-iceContent-1E-10,evaporation));
 waterContent -= draw;
 WaterBudget.AddOutput(draw);
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::NitrogenApparancy(bool nitrate, rootStructure* root)
{
   nitrogen N_amount;
   if (nitrate)
   	N_amount      = nitrate_mob+nitrate_imob;
   else                               // ammonium
   	N_amount      = (ammonium_mob+ammonium_imob)*FracAmmoniumInWater();

   double maxFlux = MaxNitrogenFlux(nitrate,root);
   if (N_amount.n>0)
   	return min(1.0,maxFlux/N_amount.n);
   else
   	return 0.0;
}

/****************************************************************************\
Calculates the maximum flux of nitrate or ammonium into roots in the layer.
returns       - Maximum nitrogen uptake in layer [g N/m/d]
\****************************************************************************/
double soilLayer::MaxNitrogenFlux(bool nitrate, rootStructure* root)
{
	double concentration, bufferingCoef, N_amount, uptake;
   double StdDiff = 0.0;
   if (nitrate)
   {
      StdDiff       = 3600 * 2.0e-5 * 24 * 1e-4;            // convert to m2 d-1
      uptake        = root->NitrateUptakeRate;
   	N_amount      = nitrate_mob.n+nitrate_imob.n;
      concentration = 0;
      if (waterContent>0)
         concentration=max(0.0,1000.*N_amount/waterContent-root->MinimumSoilNitrate);
 		bufferingCoef = 1.0; //waterContent/thickness;   NOT USED IN CURRENT DAISY CONCEPT ????????
   }
   else                               // ammonium
   {
      StdDiff       = 3600 * 1.8e-5 * 24 * 1e-4;            // convert to m2 d-1
   	uptake        = root->AmmoniumUptakeRate;
      N_amount      = ammonium_mob.n+ammonium_imob.n;
      concentration = 0;
      if (waterContent>0)
         concentration = max(0.0,1000.*N_amount/waterContent*FracAmmoniumInWater()-root->MinimumSoilAmmonium);
  		bufferingCoef = 1.0; //AmmoniumBuffering();   NOT USED IN CURRENT DAISY CONCEPT ????????
   }
   double diffusionCoef = StdDiff*pow(1.03,temperature)*Tortuosity()*waterContent/thickness;   // m2 day-1
   double rootLength = root->rootLengthList[Index];
   double maxFlux = MaxNitrogenFlux(root->rootRadius,uptake,rootLength,concentration,diffusionCoef,bufferingCoef);
   double Nuptake = rootLength*maxFlux;
   return min(N_amount,Nuptake);
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::MaxNitrogenFlux(double rootRadius,
											 double uptakeRate,
											 double rootLength,
											 double conc,
											 double diffusionCoef,
											 double bufferingCoef)
{
   if (rootLength<1e-15)
   	return 0.0;
   double q = waterUptake/(1000.*rootLength);
   double rootDens = 1000.0*rootLength/thickness;
   double alpha = 0;
   if (q>0 && diffusionCoef>0)
   	alpha = q/(2.*M_PI*diffusionCoef*bufferingCoef);
   double beta = 1./(rootRadius*sqrt(M_PI*rootDens));
   double beta_2 = beta*beta;
   double beta_1 = beta_2-1.;
   double beta_3 = beta_1*(1-0.5*alpha);
   double I = 0;
   if (alpha>1E-13)
   {
      if (alpha==2.)
      	I = q*beta_1*conc/(beta_1-log(beta_2));
      else
      	I = q*beta_3*conc/(beta_3-pow(beta,2.-alpha)+1.);
   }
   else
   	I = 4.*M_PI*diffusionCoef*conc/(beta_2*log(beta_2)/beta_1-1.0);
      // old: 2.*M_PI*diffusionCoef*bufferingCoef*conc/(beta_2*log(beta)/beta_1-0.5);

   return min(I,uptakeRate);
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilLayer::SubtractNitrateUptake(double &nitrateUptake)
{
	nitrogen nitrate = nitrate_mob+nitrate_imob;
   double N15RatioNitrate = nitrate.Get15NRatio();

   nitrogen NO3_Uptake;
   if (nitrateUptake>0 && nitrate.n>0)
 	{
      NO3_Uptake.SetBoth(nitrateUptake,N15RatioNitrate*nitrateUptake);

 		// N
  		double frac = max(0.0,min(1.0,nitrate_mob.n/nitrate.n));
 		if (frac<1e-10) frac = 0;
 		nitrate_mob.n -= nitrateUptake*frac;
  		nitrate_imob.n -= nitrateUptake*(1.0-frac);

      // 15N
  		frac = 1.0;
      if (nitrate.n15>0)
      	frac = max(0.0,min(1.0,nitrate_mob.n15/nitrate.n15));
      if (frac<1e-10) frac = 0;
 		nitrate_mob.n15 -= N15RatioNitrate*nitrateUptake*frac;
  		nitrate_imob.n15 -= N15RatioNitrate*nitrateUptake*(1.0-frac);

      if (nitrate_mob.n<-1e-6 || nitrate_mob.n15<-1e-6 || nitrate_imob.n<-1e-6 || nitrate_imob.n15<-1e-6)
      	theMessage->WarningWithDisplay("SoilLayer:: SubtractNitrate nitrate less than zero");
      Nbudget.AddOutput(NO3_Uptake.n);
 		N15budget.AddOutput(NO3_Uptake.n15);
 	}
   return NO3_Uptake;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilLayer::SubtractAmmoniumUptake(double &ammoniumUptake)
{
	nitrogen ammonium = ammonium_mob+ammonium_imob;
   double N15RatioAmmonium = ammonium.Get15NRatio();
   if (ammonium_mob.n15<-1e-10 || ammonium_imob.n15<-1e-10)
   	theMessage->WarningWithDisplay("SoilLayer:: SubtractAmmonium ammonium-N15 less than zero");

 	nitrogen NH4_Uptake;
   if (ammoniumUptake>0 && ammonium.n>0)
 	{
  		NH4_Uptake.SetBoth(ammoniumUptake,N15RatioAmmonium*ammoniumUptake);

 		// N
  		double frac = max(0.0,min(1.0,ammonium_mob.n/ammonium.n));

 		ammonium_mob.n  -= ammoniumUptake*frac;
  		ammonium_imob.n -= ammoniumUptake*(1-frac);
      if (ammonium_mob.n<0 ||  ammonium_imob.n<0)
      	theMessage->WarningWithDisplay("SoilLayer:: SubtractAmmonium ammonium less than zero");

      // 15N
      frac = 1.0;
      if (ammonium_mob.n15<-1e-10 || ammonium_imob.n15<-1e-10)
      	theMessage->WarningWithDisplay("SoilLayer:: SubtractAmmonium ammonium-N15 less than zero");
		if (ammonium.n15>0)
	  		frac = max(0.0,min(1.0,ammonium_mob.n15/ammonium.n15));
 		ammonium_mob.n15 -= N15RatioAmmonium*ammoniumUptake*frac;
  		ammonium_imob.n15 -= N15RatioAmmonium*ammoniumUptake*(1-frac);
      if (ammonium_mob.n15<-1e-10 || ammonium_imob.n15<-1e-10)
      	theMessage->WarningWithDisplay("SoilLayer:: SubtractAmmonium ammonium-N15 less than zero");

      Nbudget.AddOutput(NH4_Uptake.n);
		N15budget.AddOutput(NH4_Uptake.n15);

 	}
   return NH4_Uptake;
}

/****************************************************************************\
Different ways to set effects of tillage
\****************************************************************************/
void soilLayer::SetTillageEffect(int type,double effect)
{
	if (type==1)                                      // Increase turnover by artifically increasing temperature
   	TillageEffectTemperature = effect;
   if (type==2)												  //
   	OrganicMatter->ReleaseReduceTurnover();
}

/****************************************************************************\
Update organic matter pools in layer.
\****************************************************************************/
double soilLayer::UpdateOrganicMatter()
{
   // TEST !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   TillageEffectTemperature = max(0.0,TillageEffectTemperature-0.05);
   double NewTemperature = min(37.0,TillageEffectTemperature+temperature);

   double pFValue = pF->GetpF(waterContent/thickness);

   nitrogen nitrate  = nitrate_mob + nitrate_imob;
   nitrogen ammonium = ammonium_mob + ammonium_imob;
   double nitfrac = 0.0;
   if (nitrate.n>0.0)
      nitfrac = max(0.0,min(1.0,nitrate_mob/nitrate));

   double ammfrac = 0.0;
   if ((ammonium_mob.n>0.0) && (ammonium.n >0.0))
      ammfrac = max(0.0,min(1.0,ammonium_mob/ammonium));

   // Deals with protected NH4
   double unprotecFrac = 0.0;
   if (ammonium.n>0.0)
   	unprotecFrac = AccesibleAmmonium()/ammonium.n;
   nitrogen ProtectedNH4_mob  = ammonium_mob*(1.0-unprotecFrac);
   nitrogen ProtectedNH4_imob = ammonium_imob*(1.0-unprotecFrac);
   ammonium = ammonium*unprotecFrac;

   CO2Evolution = OrganicMatter->Update(NewTemperature,pFValue,&nitrate,&ammonium);
#ifdef NEWSLIM
nitfrac=FractionMobileWater();
ammfrac = nitfrac;
#endif
	nitrate_mob  = nitrate*nitfrac;
  	nitrate_imob = nitrate*(1.0-nitfrac);
  	ammonium_mob = ammonium*ammfrac+ProtectedNH4_mob;
  	ammonium_imob= ammonium*(1.0-ammfrac)+ProtectedNH4_imob;

   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6 || OrganicMatter->GetNitrogen().n15<-1e-10)
   	theMessage->WarningWithDisplay("soilLayer::UpdateOrganicMatter - negative n concentrations");
   return CO2Evolution;
}

/****************************************************************************\
Potential carbon turnover used in denitrification model
\****************************************************************************/
double soilLayer::PotentialCarbonTurnover()
{
 	double PotTurnover = OrganicMatter->GetCarbonAvailability();
   return PotTurnover;
}

/****************************************************************************\
\****************************************************************************/
double soilLayer::GetWaterFilledPorosity()
{
 	double PorosityFilled = waterContent/(porosity*thickness);
   return min(1.0,max(0.0,PorosityFilled));
}

/****************************************************************************\
Calculates the ammonium (g/m2) which is accesible for nitrification and mineralisation
The rest is assumed to be physically-chemically protected from nitrifying
bacteria by clay minerals allthough it is still chemically exchangeable.
This protected part is only assumed to constitute part of the total
adsorbed but exchangeable ammonium.
\****************************************************************************/
double soilLayer::AccesibleAmmonium()
{

	double AmmoniumSum          = ammonium_imob.n+ammonium_mob.n;
	double adsorbedAmmonium     = AmmoniumSum*(1-FracAmmoniumInWater());

	const double protectionCoeff = 1E-9;                     // optimised on organic crop rotation experiment by JB (24_3_3)
	double maxProtectedAmmonium = thickness*clayContent*dryBulkDensity*protectionCoeff;
	double protectedAmmonium    = min(maxProtectedAmmonium,adsorbedAmmonium);
	return AmmoniumSum-protectedAmmonium;

}

/****************************************************************************\
Perform nitrification and update contents of ammonia and nitrate.
returns       - Nitrification [g N/m�/d]
\****************************************************************************/
nitrogen soilLayer::UpdateNitrification()
{
   if (ammonium_imob.n<-1e-10 || ammonium_mob.n<-1e-10 || ammonium_imob.n15<-1e-10 || ammonium_mob.n15<-1e-10)
   	theMessage->WarningWithDisplay("SoilLayer::UpdateNitrification NH4 less than zero");
	nitrogen AddN;
   double ratio = 1.;
   if ((nitrate_imob+nitrate_mob).n>0)
   	ratio = nitrate_mob/(nitrate_imob+nitrate_mob);
   double N15RatioAmmonium = 1.0;
   if (ammonium_imob.n+ammonium_mob.n>0.0)
   	N15RatioAmmonium = max(0.0,min(1.0,(ammonium_imob.n15+ammonium_mob.n15)/(ammonium_imob.n+ammonium_mob.n)));

   //M-N kinetics DAISY parameters
   /*double NH4Conc = (ammonium_imob+ammonium_mob).n/(thickness/10)/10000;            // unit g-N/cm-3
   double MNRate = 5e-5*NH4Conc/(NH4Conc+5e-5)*thickness/50*10000;
   double Add = MNRate*
                TemperatureNitrificationEffect()*
                WaterNitrificationEffect();*/
	//Old DAISY nitrification model
  	double FirstOrderRate = nitrificationRate*AccesibleAmmonium();

   double Add = FirstOrderRate*
                TemperatureNitrificationEffect()*
                WaterNitrificationEffect();


   AddN.SetBoth(Add,Add*N15RatioAmmonium);

   double N2Oratio = RatioN2OEmissionNitri();
#ifdef NEWSLIM
ratio=FractionMobileWater();
#endif
   nitrate_mob  = nitrate_mob + AddN*ratio*(1.0-N2Oratio);
   nitrate_imob = nitrate_imob + AddN*(1.0-ratio)*(1.0-N2Oratio);

   N2OFromNitrification = AddN*N2Oratio;

   // Remove ammonium
   ratio = 1.;
   if (ammonium_imob.n+ammonium_mob.n>0)
   	ratio = max(0.0,min(1.0,ammonium_mob.n/(ammonium_imob.n+ammonium_mob.n)));

   ammonium_mob.n  = ammonium_mob.n - AddN.n*ratio;
   ammonium_imob.n = ammonium_imob.n - AddN.n*(1-ratio);

   // 15N
   ratio = 1.;
   if (ammonium_imob.n15+ammonium_mob.n15>0)
   	ratio = max(0.0,min(1.0,ammonium_mob.n15/(ammonium_imob.n15+ammonium_mob.n15)));

   ammonium_mob.n15  = ammonium_mob.n15 - AddN.n15*ratio;
   ammonium_imob.n15 = ammonium_imob.n15 - AddN.n15*(1-ratio);

   if (ammonium_imob.n<-1e-10 || ammonium_mob.n<-1e-10 || ammonium_imob.n15<-1e-10 || ammonium_mob.n15<-1e-10)
   	theMessage->WarningWithDisplay("SoilLayer::UpdateNitrification NH4 less than zero");

   Nbudget.AddOutput(N2OFromNitrification.n);
   N15budget.AddOutput(N2OFromNitrification.n15);

   return AddN*(1.0-N2Oratio);
}

/****************************************************************************\
Perform denitrification and update contents of nitrate.
returns       - Denitrification [g N/m�/d]
\****************************************************************************/
void soilLayer::UpdateN2OFromDenitrification(double TotalCO2Emission)
{
   nitrogen NRemove;

	// Water effect on denitrificaiton, data presented in Parton et al. (2000)
   double WaterEffect = max(0.0,min(1.0,0.0116+1.36/(1+exp(-(GetWaterFilledPorosity()-0.815)/0.0896))));

   // N effect on denitrificaiton, based on loam soil
   double NitrateConc = (nitrate_mob.n+nitrate_imob.n)/thickness*1000/dryBulkDensity*1e6; //unit mg kg-1
   double NitrateEffect = max(0.0,min(1.0,1.17*NitrateConc/(32.7+NitrateConc)));

   // Temperature effect on denitrificaiton
   double tempAdjust = 1.0/exp(-3.432+0.168*10*(1.0-0.5*10/36.9));
   double TemperatureEffect = tempAdjust*exp(-3.432+0.168*temperature*(1.0-0.5*temperature/36.9));

   // kd estimation
	double DenPotClay = 0.286+0.0028*clayContent*100; // clay effect is based on Drury et al., 1991  (Based on linear regression of data - JBE)

   // Actual denitrification
	double Denitrification = DenPotClay * PotentialCarbonTurnover()*WaterEffect*NitrateEffect*TemperatureEffect;
   Denitrification = min(Denitrification,0.9999*(nitrate_mob.n+nitrate_imob.n));

   if (Denitrification>0)
   {
      double N15RatioNitrate= 1.0;
   	if (nitrate_imob.n+nitrate_mob.n>0.0)
   		N15RatioNitrate = max(0.0,min(1.0,(nitrate_imob.n15+nitrate_mob.n15)/(nitrate_imob.n+nitrate_mob.n)));

      NRemove.SetBoth(Denitrification,N15RatioNitrate*Denitrification);
      // N from nitrate pools
	   double ratio = 1.;
     	if (nitrate_imob.n+nitrate_mob.n>0)
      	ratio = max(0.0,min(1.0,nitrate_mob.n/(nitrate_imob.n+nitrate_mob.n)));

     	nitrate_mob.n -= ratio*Denitrification;
     	nitrate_imob.n -= (1-ratio)*Denitrification;

      // 15N from nitrate pools
      ratio = 1.;
     	if (nitrate_imob.n15+nitrate_mob.n15>0)
      	ratio = max(0.0,min(1.0,nitrate_imob.n15/(nitrate_imob.n15+nitrate_mob.n15)));
     	nitrate_imob.n15 -= N15RatioNitrate*ratio*Denitrification;
     	nitrate_mob.n15 -= N15RatioNitrate*(1-ratio)*Denitrification;
      if(nitrate_mob.n<-0.0 || nitrate_imob.n<-0.0)
   		theMessage->WarningWithDisplay("soilLayer::Denitrification - negative nitrate concentrations");

	}
   Nbudget.AddOutput(NRemove.n);
   N15budget.AddOutput(NRemove.n15);

   double N2Oratio = RatioN2OEmissionDenitri(TotalCO2Emission);

   N2OFromDenitrification = NRemove;
}

/****************************************************************************\
Perform estimation of N2 production from N2O pool
returns       - emission
\****************************************************************************/
nitrogen soilLayer::UpdateN2Production()
{
	nitrogen TotalN2OProduction = N2OFromDenitrification + N2OFromNitrification;

   // W-factor (deficit of oxygen), based on Nommik (1956) and Wijler & Delwiche (1954)
   double fWater = 1.0-(0.0116+1.36/(1.0+exp(-(GetWaterFilledPorosity()-0.815)/0.0896))); // inversed water factor
   fWater = max(0.0,min(1.0,fWater));

   // Diffusion model substitution; the profile N2O also will be included!!!
   double fTemperature = 1.0/(1+exp(-0.64+0.08*temperature)); // Finn's temperature factor

   // Updated for loam soil to the clayContent range, corr. with data from Letey et al. (1980)
   double fClay = 1.26*exp(-1.16*clayContent)-0.249;
   // Soil N20 emission profile for loam soil, based on Yoh et al. (1999)
   double depth = GetCenterDepth()/1000.0;
   double fProfile = max(0.0,min(1.0,1.0008-0.0343*depth-3.1816*depth*depth));

   // N2 evolution and distribution of emitted N2O between the sources
   double N2ORatio = fClay*fWater*fTemperature*fProfile; // N2O reduction corrected!

   nitrogen N2OEmission = TotalN2OProduction*N2ORatio;
   N2OFromDenitrification = N2OFromDenitrification*N2ORatio;
   N2OFromNitrification = N2OFromNitrification*N2ORatio;

   Denitrification = TotalN2OProduction*(1-N2ORatio);

   return TotalN2OProduction*(1-N2ORatio); // notice this sum only includes N2 from denitrification
}
/****************************************************************************\
Perform N2O emission from nitrification
returns       - emission
\****************************************************************************/
double soilLayer::RatioN2OEmissionNitri()
{
	double N2Oratio;

	// Approach by Ingwersen et al, 1999
   double tFactor = max(0.0,min(1.0,exp(-0.5*pow(temperature/(2*3.14*2.72)-2.0,2.0))));
   double WaterFactor = max(0.0,min(1.0,GetWaterFilledPorosity()));

   // Combined emission factor from er Ambus (2001), anual effect as from N2Oratio = 0.28-0.48% (Maag and Vinther, 1996)
   N2Oratio = 0.047*tFactor*WaterFactor; //// Combined emission factor from er Ambus (2001), anual effect as from N2Oratio = 0.006;
   return N2Oratio;
}

/****************************************************************************\
Perform N2O emission from denitrification
TotalCO2Emission is used for scaling to layer emission
returns       - emission
Meaningless now @@@!!!???????????????????????????????????????
\****************************************************************************/
double soilLayer::RatioN2OEmissionDenitri(double TotalCO2Emission)
{
	double N2Oratio = 1.0;

   return N2Oratio;
}

/****************************************************************************\
Calculate the fraction of ammonium that is in the water phase.
returns       - Fraction of ammonium in soluble phase.
\****************************************************************************/
double soilLayer::FracAmmoniumInWater()
{
 const double Vp=5.96E-3;
 const double Ve=0.308E-3;
 const double Kp=630;
 const double Ke=13.7;
 double f;

 /*double a1 = -waterContent/thickness;
 double Kp1 = Kp*1e-3;
 double Ke1 = Ke*1e-3;
 double N = (ammonium_mob.GetTotalN()+ammonium_imob.GetTotalN())/thickness;
 double b1 = N+a1*(Ke1+Kp1)-clayContent*dryBulkDensity/1000*(Vp+Ve);
 double c1 = N*(Ke1+Kp1)+a1*Ke1*Kp1-clayContent*dryBulkDensity/1000*(Ke1*Vp+Kp1*Ve);
 double d1 = N*Ke1*Kp1;
 double a=b1/a1;
 double b=c1/a1;
 double c=d1/a1;
 double q = (a*a-3.0*b)/9;
 double r = (2*a*a*a-9*a*b+27*c)/54;
 if (r*r<q*q*q)
 {
   double fi = acos(r/sqrt(q*q*q));
 	double x1 = -2.0*sqrt(q)*cos(fi/3)-a/3;
 	double x2 = -2.0*sqrt(q)*cos((fi+2*3.1415926)/3)-a/3;
 	double x3 = -2.0*sqrt(q)*cos((fi-2*3.1415926)/3)-a/3;
   if (N>1e-5)
   {
     //	if (Index==2)

		   //f1 << f << "\t" << (-x2*a1/N) << endl; // << " " << q << " " << fi << " " << a1 << " " << N << " " << r << endl;
   f = -x2*a1/N;
   }
 }
 else
 	cout << "problem";*/

 f = (waterContent/thickness)/(clayContent*dryBulkDensity*(Vp/Kp+Ve/Ke)+(waterContent/thickness));
 return min(1.,f);
}

/****************************************************************************\
Calculate the ammonium buffering capacity
returns       - Ammonium buffering coeffient
\****************************************************************************/
double soilLayer::AmmoniumBuffering()
{
 const double Vp=5.96E-3;
 const double Ve=0.308E-3;
 const double Kp=630;
 const double Ke=13.7;
 double c = clayContent*dryBulkDensity;
 double C = 0.0;
 if (waterContent>0.0)
    C = 1000.0*FracAmmoniumInWater()*(ammonium_mob.n+ammonium_imob.n)/waterContent;
 double f = (c*Vp*Kp)/((Kp+C)*(Kp+C))+
				(c*Ve*Ke)/((Ke+C)*(Ke+C))+(waterContent/thickness);
 return f;
}

/****************************************************************************\
Calculate the tortuosity factor
returns       - Tortuosity factor
\****************************************************************************/
double soilLayer::Tortuosity()
{
   double relWat = waterContent/thickness;
   double val = pow(relWat,7.0/3.0)/pow(pF->GetRelativeWater(0.0),2.0);        // Millington-Quirk
   return val;
   //double f = 1.0E-6;
   //if (relWat>tortuosityLimit)
   //f = 1.0E-6+tortuosityFactor*(relWat-tortuosityLimit);
}

/****************************************************************************\
Calculates the effect of temperature on nitrification
returns       - Relative effect
\****************************************************************************/
double soilLayer::TemperatureNitrificationEffect()
{
   double F = 0;
   if (temperature<=1.) F = 0.0;
   else if (temperature<=5) F = 0.125*(temperature-1.);
   else if (temperature<=20) F = 0.1*temperature;
   else F = exp(0.4657-0.027259*temperature+0.0019315*temperature*temperature);
   return F;
}

/****************************************************************************\
Calculates the effect of soil water content on nitrification
returns       - Relative effect
\****************************************************************************/
double soilLayer::WaterNitrificationEffect()
{
   double F = 0.0;
   double pf = pF->GetpF(waterContent/thickness);
   if (pf<=0 || pf>=5)
      F = 0.0;
   else
      if (pf<1.5)
         F = pf/1.5;
      else
         if (pf<2.5)
            F = 1.0;
         else
            F = (5.0-pf)/2.5;
   return F;
}

/****************************************************************************\
Calculates the effect of temperature on denitrification
returns       - Relative effect
\****************************************************************************/
double soilLayer::TemperatureDenitrificationEffect()
{
   double f = 0.0;
   // Old DAISY version of temperature response
   if (temperature>0 && temperature<=20)
      f = 0.1*temperature;
   if (temperature>20)
   	f = exp(0.47-0.027*temperature+0.00193*temperature*temperature);

	// New temperature response proposed by Finn Vinther
   //f = 1.2*exp(min(temperature,20.0)*0.17)/(1.2*exp(10*0.17));                    // Cut of at 20 (as no measurements there)
   return f;
}

/****************************************************************************\
Calculates the effect of soil water content on denitrification
returns       - Relative effect
\****************************************************************************/
double soilLayer::WaterDenitrificationEffect()
{
   // Old DAISY version of water response
   double f = 0.0;
   double WaterLimit = fieldCapacity+0.05*(porosity*thickness-fieldCapacity);
   if (waterContent>=porosity*thickness)
   	f = 1.0;
   else if (waterContent>WaterLimit)
   	f = (waterContent-WaterLimit)/(porosity*thickness-WaterLimit);

   // New water function proposed by Finn Vinther
   //double pf = pF->GetpF(waterContent/thickness);
   //double f = 0.002;
   //if (pf<=2.0) f = (1.15-0.574*pf);
   return f;
}

/****************************************************************************\
\****************************************************************************/
void soilLayer::AddProduct(organicProduct& product)
{
   organicProduct * p = new organicProduct(product);
   nitrate_mob  = nitrate_mob+p->GetNitrate();
   ammonium_mob = ammonium_mob+p->GetAmmonium();

   p->ClearNO3_content();
   p->ClearNH4_content();

   nitrogen nitrate  = nitrate_mob + nitrate_imob;
   nitrogen ammonium = ammonium_mob + ammonium_imob;
   double nitfrac = 0.0;
   if (nitrate.n>0.0)
      nitfrac = max(0.0,min(1.0,nitrate_mob/nitrate));

   double ammfrac = 0.0;
   if ((ammonium_mob.n>0.0) && (ammonium.n >0.0))
      ammfrac = max(0.0,min(1.0,ammonium_mob/ammonium));

   // Deals with protected NH4
   double unprotecFrac = 0.0;
   if (ammonium.n>0.0)
   	unprotecFrac = AccesibleAmmonium()/ammonium.n;
   nitrogen ProtectedNH4_mob  = ammonium_mob*(1.0-unprotecFrac);
   nitrogen ProtectedNH4_imob = ammonium_imob*(1.0-unprotecFrac);
   ammonium = ammonium*unprotecFrac;

   OrganicMatter->SetMineralNitrogen(nitrate,ammonium);
   OrganicMatter->AddProduct(p);
   OrganicMatter->GetMineralNitrogen(nitrate,ammonium);

#ifdef NEWSLIM
   nitfrac=FractionMobileWater();
   ammfrac = nitfrac;
#endif

	nitrate_mob  = nitrate*nitfrac;
  	nitrate_imob = nitrate*(1.0-nitfrac);
  	ammonium_mob = ammonium*ammfrac+ProtectedNH4_mob;
  	ammonium_imob= ammonium*(1.0-ammfrac)+ProtectedNH4_imob;

   if(ammonium_mob.n<-1E-6 || ammonium_imob.n<-1E-6 || nitrate_mob.n<-1E-6 || nitrate_imob.n<-1E-6 || OrganicMatter->GetNitrogen().n15<-1e-10)
   	theMessage->WarningWithDisplay("soilLayer::UpdateOrganicMatter - negative n concentrations");

   Nbudget.AddInput(product.GetTotalNitrogen().n);
   N15budget.AddInput(product.GetTotalNitrogen().n15);

   delete p;
}

/****************************************************************************\
\****************************************************************************/
void soilLayer::StartBudget()
{
   nitrogen NO=OrganicMatter->GetNitrogen();
   nitrogen N = nitrate_mob+nitrate_imob+ammonium_mob+ammonium_imob+NO;
   Nbudget.SetInput(N.n);
   N15budget.SetInput(N.n15);
   WaterBudget.SetInput(waterContent);
}

/****************************************************************************\
\****************************************************************************/
void soilLayer::EndBudget()
{
   nitrogen NO=OrganicMatter->GetNitrogen();
   nitrogen NRemain = nitrate_mob+nitrate_imob+ammonium_mob+ammonium_imob+NO;
   if (!Nbudget.Balance(NRemain.n))
      cout << "soilLayer::EndBudget() - Name path " << GetLongName() << endl; // !!!
   N15budget.Balance(NRemain.n15);
   double WaterRemain = waterContent;
   WaterBudget.Balance(WaterRemain);
}

