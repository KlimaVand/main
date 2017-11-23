/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Column of soil layers stored as a list
(c) J�rgen E. Olesen, Statens Planteavlsfors�g

Changes:
   JEO 23-06-1997: Support for NULL-pointer in rootLengthList added
   JEO 23-06-1997: GetPoolCarbon and GetPoolNitrogen added
   JEO 08-07-1997: GetAmmoniumLeaching and GetNitrateLeaching added
   JEO 27-10-1997: New soil evaporation formula
   BMP 15-09-1999: Corrected errors in function "GetTemperature"
   BMP 04-07-2000: Corrected error in destructor
   BMP 02-08-2000: Switch between tipping bucket and darcy flow implemented
   MEL 01-03-2007: New temperature model (finite difference solution) implemented
   MEL 15-05-2007: New Water model from the SWAT model implemented
\****************************************************************************/

#include "common.h"
#include "soilprof.h"
#include <clonlist.h>
#include <bstime.h>
#include <message.h>
#include <soil.h>
#include <timeuse.h>
#include <contrparm.h>
#include <climclas.h>

const double LatentHeatMelting = 334400.0;          					//J/kg
const double HeatCapacityWater = 4192.0;            					// J/(kg dgC)
const double HeatCapacitySolid = 750.0;           						// J/(kg dgC)
const double HeatCapacityIce = 2050;                              // J/(kg dgC)
const double WaterDensity=1000;                   						// kg/m3
const double IceDensity=920;                      						// kg/m3
const double VolHeatCapacityIce=HeatCapacityIce*IceDensity; 		//J/(m3 dgC)
const double VolHeatCapacityWater=HeatCapacityWater*WaterDensity; //J/(m3 dgC)
const double snowConductivity = 2.86e-6*300*300;  // se DAISY
const double specCapacity = LatentHeatMelting*WaterDensity; //J/m3

/****************************************************************************\
\****************************************************************************/
soilProfile::soilProfile(const char * Pname,const int Index,const base * owner)
   : base(Pname,Index,owner)
{
 	last=first = NULL;
 	for (int i=0;i<MaxPlants;i++)
 		nitrogenUptake[i].Clear();
 	for (int i=0;i<MaxSoilLayers;i++)
   {
      soilLayerArray[i]=NULL;
      distanceToPreviousLayer[i]=0;
      distanceToNextLayer[i]=0;
      //NJH did this, feb 2009
      temperature[i]=0;
		heatConductivity[i]=0.0;
		heatConductivitySlope[i]=0.0;
		heatCapacity[i]=0.0;
		SpecWaterCapacity[i]=0.0;
//
   }
      //NJH did this, feb 2009
   infiltration = 0.0;
   depthDampning = 0.0;
 	maxRootDepth        = 0.0;
 	NumOfSoilLayers     = 0;
 	EvapExtCoef         = 0.01;
 	EvaporationCapacity = 10.0;
 	EvaporationContent  = 0.0;
   minTemp[0]=-10;
   minTemp[1]=-10;
   minTemp[2]=-10;
   minTemp[3]=-3;
   minTemp[4]=0;
   minTemp[5]=5;
   minTemp[6]=7;
   minTemp[7]=8; // Because of start-up problem
   minTemp[8]=8;
   minTemp[9]=3;
   minTemp[10]=-5;
   minTemp[11]=-10;
   lastErrorYear=-1;
   calculateAverage=true;
}

/****************************************************************************\
\****************************************************************************/
soilProfile::soilProfile(const soilProfile& Profile)
 : base(Profile)
{
   NumOfSoilLayers     = Profile.NumOfSoilLayers;
   EvapExtCoef         = Profile.EvapExtCoef;
   EvaporationCapacity = Profile.EvaporationCapacity;
   EvaporationContent  = Profile.EvaporationContent;
   maxRootDepth        = Profile.maxRootDepth;
   first = NULL;
   last  = NULL;
   for (int i=0;i<MaxPlants;i++)
   	nitrogenUptake[i].Clear();
   int i = 0;
   if (&Profile)
   {
   	soilLayer * current;
   	soilLayer * tempLayer;
   	current = Profile.first;
      while (current)
      {
         tempLayer = new soilLayer(*current);
         AddItem(tempLayer);
         soilLayerArray[i]=tempLayer;
         soilLayerArray[i]->SetOwner(this);
         current = current->Next();
         i++;
      }
      for (int i=0;i<ExtraLayers;i++)             // extra layers
      {
      	soilLayerArray[NumOfSoilLayers+i] = new soilLayer(*Profile.soilLayerArray[NumOfSoilLayers+i]);    //  extraLayer
         soilLayerArray[NumOfSoilLayers+i]->SetOwner(this);
      }
      for (int i=0;i<(NumOfSoilLayers+ExtraLayers);i++)
      {
         distanceToPreviousLayer[i] = Profile.distanceToPreviousLayer[i];
         distanceToNextLayer[i]     = Profile.distanceToNextLayer[i];
         temperature[i]             = Profile.temperature[i];
      }
   }
}

/****************************************************************************\
Distances between layers considered equal.
This assumption is not tested though.
\****************************************************************************/
void soilProfile::Add(soilProfile* Profile,double fraction)
{
   if (NumOfSoilLayers!=Profile->NumOfSoilLayers)
   	theMessage->FatalError("soilProfile::profiles with different number of layers can't be added!");

   for (int i=0;i<MaxPlants;i++)
   	nitrogenUptake[i].Clear();
   EvapExtCoef         = (1.0-fraction)*EvapExtCoef+fraction*Profile->EvapExtCoef;
   EvaporationCapacity = (1.0-fraction)*EvaporationCapacity+fraction*Profile->EvaporationCapacity;
   EvaporationContent  = (1.0-fraction)*EvaporationContent+fraction*Profile->EvaporationContent;
   maxRootDepth        = (1.0-fraction)*maxRootDepth+fraction*Profile->maxRootDepth;

	for (int i=0; i<(NumOfSoilLayers+ExtraLayers); i++)
   {
      soilLayerArray[i]->Add(Profile->soilLayerArray[i],fraction);
      temperature[i]  = (1.0-fraction)*temperature[i]+fraction*Profile->temperature[i];
   }
}

/****************************************************************************\
\****************************************************************************/
soilProfile::~soilProfile()
{
	if (NumOfSoilLayers>0) // Otherwise never initialised
  		for (int i=0; i<(NumOfSoilLayers+ExtraLayers); i++)
   		delete soilLayerArray[i];
   if (SoilTempFile)
   	SoilTempFile.close();
}

/****************************************************************************\
\****************************************************************************/
void soilProfile::AssignMaxRootDepth()
{
   const double c1=0.06;
   const double c2=0.15;
   const double d1=600.0;
   const double d2=2000.0;
   UnsetCritical();
   if (!GetParameter("MaxRootDepth", &maxRootDepth))
   {
   	double Clay = GetClayContent(500.0,200.0);
   	if (Clay<c1)
   		maxRootDepth = d1;
   	else
   		maxRootDepth = min(d2,d1+(d2-d1)*(Clay-c1)/(c2-c1));
   }
}

/****************************************************************************\
FARM-N pig farm 96074: calls a NULL here at some point ???!!!
\****************************************************************************/
double soilProfile::GetMaximumDepth()
{
   if (soilLayerArray[NumOfSoilLayers-1]==NULL)
      theMessage->FatalError("soilProfile::GetMaximumDepth - array points at NULL");
   else
      return soilLayerArray[NumOfSoilLayers-1]->GetEndDepth();
}

/****************************************************************************\
\****************************************************************************/
void soilProfile::Initialize(fstream * f)
{
   Setfile(f);
   double startDepth = 0.0;
   SetCritical();
   FindSection("SoilParameters");
   GetParameter("Nodes",&NumOfSoilLayers);
   double nodeThickNess=100;
   UnsetCritical();
   GetParameter("NodeThickness",&nodeThickNess);
   double JBC[] = {0.030,0.03,0.025,0.02,0.02,0.015,0.015,0.01,0.01,0.01}; // �ndre til afh�ngighed af ler !!!!!
   int JB = 5; // Dette m� v�re en fejl !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

   EvapExtCoef = JBC[JB-1];

   soilLayer * aTempSoilLayer;
   for (int i=0; i<NumOfSoilLayers; i++)
   {
      aTempSoilLayer = new soilLayer("soilLayer",i,this);
      aTempSoilLayer->Initialize(startDepth,nodeThickNess,f);
      AddItem(aTempSoilLayer);
      soilLayerArray[i]=aTempSoilLayer;
      startDepth += nodeThickNess;
   }
   AssignMaxRootDepth();
   if (startDepth<maxRootDepth)
   	theMessage->FatalError("soilProfile::Initialize - The depth of defined layers is smaller than the root depth");
   for (int i=0;i<NumOfSoilLayers;i++)       // Calculates the distance between layers and converts to meter
   {
      temperature[i] = 8.0;                    // default temperature
      aTempSoilLayer = soilLayerArray[i];
      if (i==0)
      	distanceToPreviousLayer[i] = aTempSoilLayer->GetThickness()/2000; // 1000 ???!!!
      else
      	distanceToPreviousLayer[i] = (aTempSoilLayer->GetThickness()+soilLayerArray[i-1]->GetThickness())/2000.0;
      if (i==(NumOfSoilLayers-1))
      	distanceToNextLayer[i] =  aTempSoilLayer->GetThickness()/1000;
      else
      	distanceToNextLayer[i] = (aTempSoilLayer->GetThickness()+soilLayerArray[i+1]->GetThickness())/2000.0;
      }
   for (int i=0;i<ExtraLayers;i++)             // extra layers to ensure no heat flow at bottom (boundary condition)
   {
      soilLayerArray[NumOfSoilLayers+i] = new soilLayer(*soilLayerArray[NumOfSoilLayers-1]);    //  extraLayer
      temperature[i+NumOfSoilLayers] = 8.0;
      distanceToNextLayer[i+NumOfSoilLayers]     = distanceToNextLayer[NumOfSoilLayers-1]*(i+1)*10.0; // 50;
      soilLayerArray[NumOfSoilLayers+i]->SetStartDepth(soilLayerArray[NumOfSoilLayers+i-1]->GetStartDepth()+
                                                       1000.0*distanceToNextLayer[i+NumOfSoilLayers-1]);
      distanceToPreviousLayer[i+NumOfSoilLayers] = distanceToNextLayer[i+NumOfSoilLayers-1];
      soilLayerArray[NumOfSoilLayers+i]->SetThickness(distanceToNextLayer[i+NumOfSoilLayers]*1000.0);
      soilLayerArray[NumOfSoilLayers+i]->SetFieldCapacity(soilLayerArray[NumOfSoilLayers-1]->GetFieldCapacity()/
                                                          soilLayerArray[NumOfSoilLayers-1]->GetThickness()*
                                                          soilLayerArray[NumOfSoilLayers+i]->GetThickness());
      soilLayerArray[NumOfSoilLayers+i]->SetWater(soilLayerArray[NumOfSoilLayers+i]->GetFieldCapacity());
   }
   f=NULL;

   for (int i=0;i<(NumOfSoilLayers+ExtraLayers);i++)
   {
   	DistPreviousLayerInv[i] = 1.0/distanceToPreviousLayer[i];
      DistNextLayerInv[i]     = 1.0/distanceToNextLayer[i];
   }
   if (theControlParameters->GetWriteSoilTemperature())
   	SoilTempFile.open("soiltemp.txt",ios::out);

}

/****************************************************************************\
Adds nitrogen to the first layer in the profile
   addNitrate    - Nitrate to be added [g N/m�]
   addAmmonium   - Ammonium to be added [g N/m�]
\****************************************************************************/
void soilProfile::AddNutrient(nitrogen soilNitrate, nitrogen soilAmmonium)
{
 if (first != NULL)
  first->AddNutrient(soilNitrate,soilAmmonium);
}

/****************************************************************************\
Adds a soil layer to the profile
   data          - Soil layer object
\****************************************************************************/
void soilProfile::AddItem(soilLayer * data)
{
 if (first == NULL) first = data;
 else last->AddNext(data);
 last = data;
}

/****************************************************************************\
\****************************************************************************/
void soilProfile::CheckCoherence(int checkpoint)
{
   int TotalNumberOfLayers = NumOfSoilLayers + ExtraLayers;
   for (int i=0;i<TotalNumberOfLayers;i++)
      if (soilLayerArray[i]->GetIce()>soilLayerArray[i]->GetTotalWater()+0.0000001)
      {
         cout << "Checkpoint " << checkpoint << ", layer " << i << " is not coherent";
         cout << endl;
      }
}

/****************************************************************************\
Performs infiltration of water, nitrate and ammonium in profile
   surplus         - Water to be added to/returned from profile [mm]
   nitrateLeached  - Nitrate to the added to/returned from profile [g N/m�]
   ammoniumLeached - Ammonium to the added to/returned from profile [g N/m�]
\****************************************************************************/
void soilProfile::UpdateInfiltration(double *surplus, nitrogen * surfaceNitrate,
                                     nitrogen * surfaceAmmonium, nitrogen *NitrateLeached,
                                     nitrogen *AmmoniumLeached, double Chloride)

{

   double pressurePotential[MaxSoilLayers],waterConductivity[MaxSoilLayers];
   double Flux[MaxSoilLayers], totalFlux[MaxSoilLayers];
   double maxContent[MaxSoilLayers];
   double waterContent[MaxSoilLayers];
   //-------------- NEW --------------------
   double halfdesignated[MaxSoilLayers];
   double realisedFlow[MaxSoilLayers];
   double relWaterContent[MaxSoilLayers];
   double porosity[MaxSoilLayers];
   double FC[MaxSoilLayers];
   double surplusW[MaxSoilLayers];
   nitrogen surplusNH4[MaxSoilLayers];
   nitrogen surplusNO3[MaxSoilLayers];
   //---------------------------------------
   double
      excess,
      standingWater,
      relativeWater,
      maxFlux,
      stepLength,
      pressurePotentialSlope;
   double simulationTime = 0.0;

   timeConsumption->startProfiling(waterSimulation);

   int TotalNumberOfLayers = NumOfSoilLayers + ExtraLayers;
   pF_Curve * pF[MaxSoilLayers];
   int i;
	for (i=0;i<TotalNumberOfLayers;i++)
   {
		soilLayerArray[i]->ClearLeaching();             // set leaching to zero
      waterContent[i] = soilLayerArray[i]->GetTotalWater();
      pF[i] = soilLayerArray[i]->GetpF_Curve();
      porosity[i]=soilLayerArray[i]->GetPorosity();
      maxContent[i]=(soilLayerArray[i]->GetPorosity()*soilLayerArray[i]->GetThickness());
      totalFlux[i]=0.0;
      halfdesignated[i]=0.0;
      realisedFlow[i]=0.0;
      FC[i]=soilLayerArray[i]->GetFieldCapacity();
      surplusW[i]=0.0;
      surplusNH4[i].Clear();
      surplusNO3[i].Clear();
   }
   soilLayerArray[TotalNumberOfLayers-1]->SetWater(soilLayerArray[TotalNumberOfLayers-1]->GetFieldCapacity()); // Lower boundary always field capacity
   standingWater=*surplus;

   int flowMethod=theControlParameters->GetWaterFlowMethod();
   flowMethod=1; // ----------------------- TEST !!!!! -------------------------

   if ((flowMethod!=4) && (flowMethod !=5))
   {
      // Perform infiltration of top layer. This makes simulations sensitive to top layer thickness
      infiltration=min(standingWater,maxContent[0]-waterContent[0]);
      if (infiltration>0.0)
      {
         standingWater-=infiltration;
         waterContent[0]+=infiltration;
         EvaporationContent=min(EvaporationCapacity,infiltration+EvaporationContent);
      }
      else
         if (infiltration<(-1E-10))
            theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - top layer contains more water than physically possible");
   }

   if (flowMethod==1 || flowMethod==3)
   { // ---------------- TIPPING BUCKET ----------------------------------------
      for (i=0;i<TotalNumberOfLayers;i++)
      {
         totalFlux[i] = 0.0;
         double NotFrozenWater = soilLayerArray[i]->GetTotalWater()-soilLayerArray[i]->GetIce();
//         if (i<(TotalNumberOfLayers-1))
			if (waterContent[i]-soilLayerArray[i]->GetFieldCapacity()>0.0)   // drainage
         {
            totalFlux[i] = soilLayerArray[i]->GetDrainageConstant()*
            					min(NotFrozenWater,(max(0.0,waterContent[i]-soilLayerArray[i]->GetFieldCapacity())));
            if (totalFlux[i]<0.0) // TEST !!!!!!!!!!!!!!
            {
               cout << "totalFlux negative, it is: " << totalFlux[i];
               cout << endl;
            }
            if (i<(TotalNumberOfLayers-1))
            {
               totalFlux[i] = min(totalFlux[i],maxContent[i+1]-waterContent[i+1]);       //reduce totalflux according to capacity in next layer
	  //				totalFlux[i] = min(totalFlux[i],NotFrozenWater);
               if (totalFlux[i]<0.0) // TEST !!!!!!!!!!!!!!
               {
                  cout << "totalFlux negative, it is: " << totalFlux[i];
                  cout << endl;
               }
               waterContent[i+1] += totalFlux[i];                                      //add totalflux to next layer
               if (waterContent[i+1]>maxContent[i+1]) // TEST !!!!!!!!!!!!!
                  cout << "";
            }
            waterContent[i] -= totalFlux[i];                                         //remove totalflux from present layer
         }
         if (waterContent[i]>maxContent[i]) // TEST !!!!!!!!!!!!!
            cout << "";
      }

	}

   if (flowMethod==0 || flowMethod==3)
   { //----------------------- DARCY FLOW --------------------------------------
      while (simulationTime<86400)
      {

         // calculate potentials and conductivity ------------------------------
         for (i=0;i<TotalNumberOfLayers;i++)
         {
            relativeWater = min(1.0,waterContent[i]/soilLayerArray[i]->GetThickness());
            pressurePotential[i] = -pF[i]->GetPressurePotential(relativeWater);
            waterConductivity[i] = pF[i]->GetConductivity(pF[i]->GetpF(relativeWater));
            Flux[i] = 0.0;
         }
         for (i=0;i<TotalNumberOfLayers;i++)
         {
            if (soilLayerArray[i]->GetFrozenPart()>1e-4)
               Flux[i] = 0.0;                                              // no flow if ice present
            else
            {
               if (i==(TotalNumberOfLayers-1))
                  Flux[i] = waterConductivity[i]*1000.0;
               else
               {
                  pressurePotentialSlope = (pressurePotential[i]-pressurePotential[i+1])*DistNextLayerInv[i];
                  Flux[i] = -1000.0*waterConductivity[i]*(pressurePotentialSlope-1);          // includes gravity flow
               }  // Now converted to mm pr s. Positive value -> Downward flux.
               if (i<(TotalNumberOfLayers-1))
               {
                  if (Flux[i]>0.0)
                  {
                     if (soilLayerArray[i+1]->GetFrozenPart()>1e-4)
                        Flux[i] = 0.0;                                          // no flow downwards if ice present in layer below
                     else
                        Flux[i]=min(Flux[i],maxContent[i+1]-waterContent[i+1]); // Avoid oscillations
                  }
                  else
                     if (Flux[i]<0.0)
                        Flux[i]=-1.0*min(-Flux[i],maxContent[i]-waterContent[i]); // Avoid oscillations
               }
            }
         }
         // End calculate potentials and conductivity --------------------------

         // Calculate step length ----------------------------------------------
         maxFlux = 0.0;
         for (i=0;i<TotalNumberOfLayers-1;i++)
            maxFlux = max(maxFlux,fabs(Flux[i]));
         if (maxFlux==0)
            stepLength = 86400.0-simulationTime;
         else
            stepLength = min(86400.0/100.0,0.05/maxFlux);    // (0.05?)
         simulationTime += stepLength;
         // End calculate step length ------------------------------------------

         // Update watercontent vector -----------------------------------------
         for (i=0;i<TotalNumberOfLayers-1;i++)
         {
            Flux[i] *= stepLength;
            waterContent[i] -= Flux[i];
            if (i>0 && i<(TotalNumberOfLayers-1))
               waterContent[i] += Flux[i-1];
         }
         // End update watercontent vector -------------------------------------

         // Move possibly excessive water upwards ------------------------------
         for (i=TotalNumberOfLayers-1;i>=0;i--)
         {
            excess=waterContent[i]-maxContent[i];
            if (excess>0.01)
            {
               waterContent[i]=maxContent[i];
               if (i>0)
               {
                  waterContent[i-1]+=excess;
                  Flux[i-1]-=excess;
                  if (Flux[i-1]<-1E-5)
                     theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - inconsistency in function (1)");
               }
               else
               {
                  standingWater+=excess;
                  if (Flux[i]>0)
                     theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - inconsistency in function (2)");
               }
            }
            totalFlux[i]+=Flux[i];
            if (waterContent[i]<0)
               theMessage->FatalError("soilProfile::UpdateInfiltration - nummerical error in Darcy flow calculation");
         }
         // End move possibly excessive water upwards --------------------------
      }
   }

 double f=1.0;
/*
 if (flowMethod==4)
   { // ---------------- TIPPING BUCKET TEST METTE & BJ�RN ---------------------
      bool macropore=true;
      bool aliquots=false;
      bool testwrite=false;
      double bypassCapacity=30.0; // ????????????????!!!
      if (!macropore)
         bypassCapacity=0.0;
      bool eqilibrateOnce=true;
      // ---------------------------- Tracking -----------------------
      for (i=0;i<TotalNumberOfLayers;i++)
      {
         porosity[i]=soilLayerArray[i]->GetPorosity();
         maxContent[i]=(soilLayerArray[i]->GetPorosity()*soilLayerArray[i]->GetThickness());
         FC[i]=soilLayerArray[i]->GetFieldCapacity();
         waterContent[i]=soilLayerArray[i]->GetTotalWater();
      }

      double maxC;
      for (i=0;i<MaxSoilLayers;i++)
      {
         if (i<TotalNumberOfLayers)
         {
            maxC=soilLayerArray[i]->GetPorosity()*soilLayerArray[i]->GetThickness();
            relWaterContent[i]=soilLayerArray[i]->GetTotalWater()/maxC;
         }
         else
            relWaterContent[i]=0.0;
      }
      double content_before=0.0;
      double N_content_before=0.0;
      for (i=0;i<TotalNumberOfLayers-1;i++)
      {
         content_before+=soilLayerArray[i]->GetTotalWater();
         N_content_before=N_content_before+soilLayerArray[i]->GetNitrate().n+soilLayerArray[i]->GetAmmonium().n;
      }
      double added=*surplus;
      double percolated=0.0;
      double N_added=surfaceNitrate->n+surfaceAmmonium->n;
      double N_leached=0.0;
      // -------------------------------------------------------------

     fstream fout;
     if (testwrite)
     {
        fout.open("d:\\fasset\\testpercolation.txt",ios::out);
        fout << "Inhold f�r genneml�b" << endl;
        for (i=0;i<TotalNumberOfLayers-1;i++)
           fout << "Lag " << i << "\t" << soilLayerArray[i]->GetTotalWater() << endl;
        fout << endl << "Totalt vandinhold " << content_before;
        cout << endl << endl << "Surplus " << (*surplus) << endl;
        fout << endl << endl << "Surplus " << (*surplus) << endl;
     }
     f=0.0;
     standingWater=*surplus;
     int aliq=1;
     if (aliquots)
        aliq=(int)ceil(max(1.0,*surplus/5.0)); // 5 mm default
     for (int k=0;k<aliq;k++)
     {
        if (testwrite && aliquots)
        {
           cout << "Loop no " << (k+1) << " of " << aliq << endl;
           fout << "Loop no " << (k+1) << " of " << aliq << endl;
        }
        // Check/complete halogene tracer coding later !!!
        infiltration=min(*surplus/aliq,maxContent[0]-soilLayerArray[0]->GetTotalWater());
        if (infiltration>0.0)
        {
           standingWater-=infiltration;
           EvaporationContent=min(EvaporationCapacity,infiltration+EvaporationContent);
        }
        else
           if (infiltration<(-1E-10))
               theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - top layer contains more water than physically possible");
      double frac=1.0;
      double remainCap=0.5*bypassCapacity;
      if (infiltration>0)
      {
         frac=infiltration/(*surplus);
         // Overskud fyldes i toplag
         soilLayerArray[0]->AddWater(infiltration,*surfaceNitrate*frac,*surfaceAmmonium*frac,0);
         if ((maxContent[0]+1E-12)<soilLayerArray[0]->GetTotalWater())
            theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - more water in top layer than possible!");
         f+=frac;
         if (testwrite)
         {
            cout << endl << "Lag 0 tilf�rt " << infiltration << endl;
            fout << endl << "Lag 0 tilf�rt " << infiltration << endl;
         }
      }
      // Mobilt vand i lag 0. "Rest" skal tilbage til overfladen, sker via f nederst denne funktion
      // Regn "overskud" ud
		halfdesignated[0] = 0.0;
      if (soilLayerArray[0]->GetTotalWater()>soilLayerArray[0]->GetFieldCapacity())
         halfdesignated[0] = 0.5*soilLayerArray[0]->GetDrainageConstant()*(soilLayerArray[0]->GetTotalWater()-soilLayerArray[0]->GetFieldCapacity());
      // Gem � "overskud" og fjern fra lag, hvis det kan for n�ste lag, ellers flyt s� meget som muligt.
      // W og S fjernes
      realisedFlow[0] = min(halfdesignated[0],0.5*(maxContent[1]-soilLayerArray[1]->GetTotalWater()));
      //surplusW[0]=max(0.0,min(remainCap,halfdesignated[0]-0.5*(maxContent[1]-soilLayerArray[1]->GetWater())));
      surplusW[0]=max(0.0,min(remainCap,halfdesignated[0]-realisedFlow[0]));
      remainCap-=surplusW[0];
      if (remainCap<-1E-10)
         theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - remaining bypass capacity can not be negative");
      nitrogen nitrate,ammonium;
      if (realisedFlow[0]>0.0)
      {
         soilLayerArray[0]->RemoveWaterSimple(realisedFlow[0],&nitrate,&ammonium,&Chloride);
         if (soilLayerArray[0]->GetTotalWater()<soilLayerArray[0]->GetFieldCapacity())
            cout << "To much water removed from layer 0" << endl;
         totalFlux[0]=realisedFlow[0];
         if (testwrite)
         {
            cout << "Lag 0 fraf�rt " << realisedFlow[0] << endl;
            fout << "Lag 0 fraf�rt " << realisedFlow[0] << endl;
         }
      }
      if (surplusW[0]>0.0)
      {
         nitrogen NO3,NH4;
         soilLayerArray[0]->RemoveWaterSimple(surplusW[0],&NO3,&NH4,&Chloride,false);
         surplusNH4[0]=NH4;
         surplusNO3[0]=NO3;
         if (soilLayerArray[0]->GetTotalWater()<soilLayerArray[0]->GetFieldCapacity())
            cout << "To much water removed from layer 0" << endl;
         if (testwrite)
         {
            cout << "Lag 0 til transit " << surplusW[0] << endl;
            fout << "Lag 0 til transit " << surplusW[0] << endl;
         }
      }

      for (i=1;i<TotalNumberOfLayers;i++) // Phase 1-3;
      {
         if (halfdesignated[i-1]>0.0) // Downward movement
         {
            if (i==TotalNumberOfLayers-1)
            {
               *NitrateLeached = nitrate;
               *AmmoniumLeached = ammonium;
               percolated+=realisedFlow[i-1];
            }
            // Tilf�j vand fra ovenliggende lag
            soilLayerArray[i]->AddWater(realisedFlow[i-1],nitrate,ammonium,0);
            if ((maxContent[i]+1E-12)<soilLayerArray[i]->GetTotalWater())
               theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - more water in layer than possible!");
            if (testwrite)
            {
               cout << "Lag " << i << " tilf�rt " << realisedFlow[i-1] << endl;
               fout << "Lag " << i << " tilf�rt " << realisedFlow[i-1] << endl;
            }
         }
        // Mobilt vand i lag i - regn "overskud" ud
         halfdesignated[i]=0.0;
         realisedFlow[i]=0.0;
         if (soilLayerArray[i]->GetTotalWater()>soilLayerArray[i]->GetFieldCapacity())
         {
            halfdesignated[i] = 0.5*soilLayerArray[i]->GetDrainageConstant()*(soilLayerArray[i]->GetTotalWater()-soilLayerArray[i]->GetFieldCapacity());
            realisedFlow[i]=halfdesignated[i];
            // Gem � "overskud" og fjern fra lag, hvis det kan for n�ste lag, ellers flyt s� meget som muligt.
            if (i<(TotalNumberOfLayers-1))
            {
               realisedFlow[i] = min(realisedFlow[i],maxContent[i+1]-soilLayerArray[i+1]->GetTotalWater());
               //surplusW[i]=max(0.0,min(remainCap,halfdesignated[i]-0.5*(maxContent[i+1]-soilLayerArray[i+1]->GetWater())));
               surplusW[i]=max(0.0,min(remainCap,halfdesignated[i]-realisedFlow[i]));
               remainCap-=surplusW[i];
               if (remainCap<-1E-10)
                  theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - remaining bypass capacity can not be negative");
            }
            // W og S fjernes
            soilLayerArray[i]->RemoveWaterSimple(realisedFlow[i],&nitrate,&ammonium,&Chloride);
            if (soilLayerArray[i]->GetTotalWater()<soilLayerArray[i]->GetFieldCapacity())
               cout << "To much water removed from layer " << i << endl;
            totalFlux[i]+=realisedFlow[i];
            if (testwrite)
            {
               cout << "Lag " << i << " fraf�rt " << realisedFlow[i] << endl;
               fout << "Lag " << i << " fraf�rt " << realisedFlow[i] << endl;
            }
            if (surplusW[i]>0.0)
            {
               nitrogen NO3,NH4;
               soilLayerArray[i]->RemoveWaterSimple(surplusW[i],&NO3,&NH4,&Chloride,false);
               surplusNH4[i]=NH4;
               surplusNO3[i]=NO3;
               if (soilLayerArray[i]->GetTotalWater()<soilLayerArray[i]->GetFieldCapacity())
                  cout << "To much water removed from layer" <<  i << endl;
               if (testwrite)
               {
                  cout << "Lag " << i << " til transit " << surplusW[i] << endl;
                  fout << "Lag " << i << " til transit " << surplusW[i] << endl;
               }
            }
         }
      }

      // Nu er � vand flyttet i alle lag ---------------------------------------

      double bucketW=0.0;
      nitrogen bucketNO3;
      nitrogen bucketNH4;
      double extraInfiltration;

      // Udf�r 1. halvdel af bypass flow, f�rst fyldes "flasken" op med evt. st�ende vand fra overfladen
      if (macropore)
      {
         extraInfiltration=min(remainCap,standingWater);
         if (extraInfiltration<(-1E-10))
            theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - extra infiltration can not be negative");
         remainCap-=extraInfiltration;
         if (remainCap<-1E-10)
            theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - remaining bypass capacity can not be negative");
         if (*surplus>0.0)
            frac=extraInfiltration/(*surplus);
         else
            frac=0.0;
         standingWater-=extraInfiltration;
         bucketW+=extraInfiltration;
         bucketNO3=bucketNO3+*surfaceNitrate*frac;
         bucketNH4=bucketNH4+*surfaceAmmonium*frac;
         if (testwrite)
         {
            cout << "Overfladevand til flaske " << extraInfiltration << endl;
            fout << "Overfladevand til flaske " << extraInfiltration << endl;
         }

         f+=frac;

         if (remainCap<0.5*bypassCapacity) // Er der "overskydende" vand
            for (i=0;i<TotalNumberOfLayers;i++) // Bev�g nu "flasken" ned gennem alle lag
            {
               totalFlux[i]+=bucketW;
               double move=min(bucketW,maxContent[i]-soilLayerArray[i]->GetTotalWater()); // Start med at h�lde af flasken
               if (move>0.0)
               {
                  double frac=move/bucketW;
                  soilLayerArray[i]->AddWater(move,bucketNO3*frac,bucketNH4*frac,0);
                  bucketW-=move;
                  bucketNO3=bucketNO3*(1.0-frac);
                  bucketNH4=bucketNH4*(1.0-frac);
                  if (testwrite)
                  {
                     cout << "Flaske til lag " << i << ": " << move << endl;
                     fout << "Flaske til lag " << i << ": " << move << endl;
                  }
               }
               if (surplusW[i]>0.0) // Hvis der er overskud, fyld p� flasken
               {
                  bucketW+=surplusW[i];
                  if (testwrite)
                  {
                     cout << "Overskudsvektor i lag " << i << " til flaske " << surplusW[i] << endl;
                     fout << "Overskudsvektor i lag " << i << " til flaske " << surplusW[i] << endl;
                  }
                  surplusW[i]=0.0;
                  bucketNO3=bucketNO3+surplusNO3[i];
                  surplusNO3[i].Clear();
                  bucketNH4=bucketNH4+surplusNH4[i];
                  surplusNH4[i].Clear();
                  if (bucketW>(bypassCapacity+1E-12))
                     cout << "Bypass algoritm error, current layer is " << i << endl ;
               }
               soilLayerArray[i]->AddLeaching(bucketNO3,bucketNH4); // Account for the passing nitrogen
            }
      }

      for (i=0;i<TotalNumberOfLayers;i++) // Phase 4 - Ekvilibrer lag i
      {
         if (!eqilibrateOnce || k==(aliq-1))
            soilLayerArray[i]->EquilibrateNitrogen(0.0); // Se p� hvad dette nul betyder - senere !!!
      }
      for (i=0;i<TotalNumberOfLayers;i++) // Phase 5
      {
         // Flyt direkte � overskud, hvis det kan for n�ste lag, ellers flyt s� meget som muligt, resten bliver i laget
         realisedFlow[i]=halfdesignated[i];
         surplusW[i]=0.0;
         if (i<(TotalNumberOfLayers-1))
         {
            realisedFlow[i] = min(halfdesignated[i],maxContent[i+1]-soilLayerArray[i+1]->GetTotalWater());
            //surplusW[i]=max(0.0,min(remainCap,halfdesignated[i]-0.5*(maxContent[i+1]-soilLayerArray[i+1]->GetWater())));
            surplusW[i]=max(0.0,min(remainCap,halfdesignated[i]-realisedFlow[i]));
            remainCap-=surplusW[i];
            if (remainCap<-1E-10)
               theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - remaining bypass capacity can not be negative");
         }
         if (realisedFlow[i]>0.0)
         {
            soilLayerArray[i]->RemoveWaterSimple(realisedFlow[i],&nitrate,&ammonium,&Chloride);
            if (soilLayerArray[i]->GetTotalWater()<soilLayerArray[i]->GetFieldCapacity())
               cout << "To much water removed from layer " << i << endl;
            totalFlux[i]+=realisedFlow[i];
            if (testwrite)
            {
               cout << "Lag " << i << " fraf�rt " << realisedFlow[i] << endl;
               fout << "Lag " << i << " fraf�rt " << realisedFlow[i] << endl;
            }
            if (i==TotalNumberOfLayers-2)
            {
               *NitrateLeached = *NitrateLeached+nitrate;
               *AmmoniumLeached = *AmmoniumLeached+ammonium;
               percolated+=realisedFlow[i];
            }
            if (i<(TotalNumberOfLayers-1))
            {
               soilLayerArray[i+1]->AddWater(realisedFlow[i],nitrate,ammonium,0);
               if ((maxContent[i+1]+1E-12)<soilLayerArray[i+1]->GetTotalWater())
                  theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - more water in layer than possible!");
               if (testwrite)
               {
                  cout << "Lag " << (i+1) << " tilf�rt " << realisedFlow[i] << endl;
                  fout << "Lag " << (i+1) << " tilf�rt " << realisedFlow[i] << endl;
               }
            }
         }
         if (surplusW[i]>0.0)
         {
            nitrogen NO3,NH4;
            soilLayerArray[i]->RemoveWaterSimple(surplusW[i],&NO3,&NH4,&Chloride,false);
            surplusNH4[i]=NH4;
            surplusNO3[i]=NO3;
            if (soilLayerArray[i]->GetTotalWater()<soilLayerArray[i]->GetFieldCapacity())
               cout << "To much water removed from layer " << i << endl;
         }
      }

      // Udf�r 2. halvdel af bypass flow, f�rst fyldes "flasken" op med evt. st�ende vand fra overfladen - hvis der er plads
      if (macropore)
      {
         bucketW=0.0;
         extraInfiltration=min(remainCap,standingWater);
         if (extraInfiltration<(-1E-10))
            theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - extra infiltration can not be negative");
         remainCap-=extraInfiltration;
         if (*surplus>0.0)
            frac=extraInfiltration/(*surplus);
         else
            frac=0.0;
         standingWater-=extraInfiltration;
         bucketW+=extraInfiltration;
         bucketNO3=bucketNO3+*surfaceNitrate*frac;
         bucketNH4=bucketNH4+*surfaceAmmonium*frac;
         if (testwrite)
         {
            cout << "Overfladevand til flaske " << extraInfiltration << endl;
            fout << "Overfladevand til flaske " << extraInfiltration << endl;
         }
         f+=frac;

         if (remainCap<0.5*bypassCapacity) // Er der "overskydende" vand
            for (i=0;i<TotalNumberOfLayers;i++) // Bev�g nu "flasken" ned gennem alle lag
            {
               totalFlux[i]+=bucketW;
               double move=min(bucketW,maxContent[i]-soilLayerArray[i]->GetTotalWater()); // Start med at h�lde af flasken
               if (move>0.0)
               {
                  double frac=move/bucketW;
                  soilLayerArray[i]->AddWater(move,bucketNO3*frac,bucketNH4*frac,0);
                  bucketW-=move;
                  bucketNO3=bucketNO3*(1.0-frac);
                  bucketNH4=bucketNH4*(1.0-frac);
                  if (testwrite)
                  {
                     cout << "Flaske til lag " << i << ": " << move << endl;
                     fout << "Flaske til lag " << i << ": " << move << endl;
                  }
               }
               if (surplusW[i]>0.0) // Hvis der er overskud, fyld p� flasken
               {
                  bucketW+=surplusW[i];
                  if (testwrite)
                  {
                     cout << "Overskudsvektor i lag " << i << " til flaske " << surplusW[i] << endl;
                     fout << "Overskudsvektor i lag " << i << " til flaske " << surplusW[i] << endl;
                  }
                  surplusW[i]=0.0;
                  bucketNO3=bucketNO3+surplusNO3[i];
                  surplusNO3[i].Clear();
                  bucketNH4=bucketNH4+surplusNH4[i];
                  surplusNH4[i].Clear();
                  if (bucketW>(bypassCapacity+1E-12))
                     cout << "Bypass algoritm error, current layer is " << i << endl;
               }
               soilLayerArray[i]->AddLeaching(bucketNO3,bucketNH4); // Account for the passing nitrogen
            }
      }
    }
*/
    for (i=0;i<TotalNumberOfLayers;i++)
    {
       soilLayerArray[i]->SetWaterFlux(totalFlux[i]);
    }

    /* --------------------- Bypass - gammel version --------------------
    if (macropore && (standingWater>0.0))
    {
       if (aliquots)
          theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - new bypass function is not intended for use together with aliquots option");

       // 1st remove surplus water in the profile
       for (i=0;i<TotalNumberOfLayers-1;i++)
       {
          double overload=halfdesignated[i]*2.0-realisedFlow[i];
          double move=min(overload,bypassCapacity-bucketW);
          if (move>0.0)
          {
             soilLayerArray[i]->RemoveWaterSimple(move,&nitrate,&ammonium,&Chloride);
             bucketW+=move;
             bucketNO3+=nitrate;
             bucketNH4+=ammonium;
             if (soilLayerArray[i]->GetWater()<soilLayerArray[i]->GetFieldCapacity())
                cout << "To much water removed from layer " << i << endl;
          }
          if (bucketW>0.0)
          {
             totalFlux[i]+=bucketW
             // Update bookkeeping for NO3 and NH4 ???!!!
          }
          if ()
       }

       // 2nd remove from surface if capacity left
       double bypassW=min(bypassCapacity,standingWater);
       double frac=bypassW/standingWater;
       standingWater-=bypassW;
       nitrogen bypassNO3=*surfaceNitrate*frac*(1.0-f);
       nitrogen bypassNH4=*surfaceAmmonium*frac*(1.0-f);
       f=(*surplus-standingWater)/(*surplus);
       nitrogen mNO3,mNH4;
       double spaceleft,move;
       for (i=0;i<TotalNumberOfLayers-1;i++)
       {
          spaceleft=maxContent[i]*maxFill-soilLayerArray[i]->GetWater();
          if (spaceleft<0.0)
             spaceleft=0.0;
          else
          {
             move=min(spaceleft,bypassW);
             if (move<-1E-20)
                theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - code error (2)!");
             frac=move/bypassW;
             if (move<0.0)
                theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - code error (3)!");
             mNO3=bypassNO3*frac;
             mNH4=bypassNH4*frac;
             soilLayerArray[i]->AddWater(move,mNO3,mNH4,0);
             spaceleft=maxContent[i]-soilLayerArray[i]->GetWater();
             if (spaceleft<0.0)
                theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - code error (4)!");
             bypassNO3=bypassNO3-mNO3;
             bypassNH4=bypassNH4-mNH4;
             bypassW-=move;
          }
          soilLayerArray[i]->AddWaterFlux(bypassW);
          if (bypassW<1E-20) // Exit loop if no more water left
             break;
       }
       if (bypassW>0.0)
          theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - code error (5)!");
       soilLayerArray[i]->SetWaterFlux(totalFlux[i]);
    }


      // ---------------------------- Tracking -----------------------
      double content_after=0.0;
      for (i=0;i<TotalNumberOfLayers-1;i++) // Udregn budget
         content_after+=soilLayerArray[i]->GetTotalWater();
      double return_surface=*surplus*(1.0-f);
      double balance=content_after-content_before-added+percolated+return_surface;
      if (testwrite)
      {
         fout << endl << "Inhold efter genneml�b" << endl;
         for (i=0;i<TotalNumberOfLayers-1;i++)
            fout << "Lag " << i << "\t" << soilLayerArray[i]->GetTotalWater() << endl;
         fout << endl << "Totalt vandinhold " << content_after;
         fout.close();
      }
      if (fabs(balance)>1E-6)
      {
         cout << endl << theTime << endl;
         cout << "Added               " << added << endl;
         cout << "Percolated          " << percolated << endl;
         cout << "Returned to surface " << return_surface << endl;
         cout << "Content before      " << content_before << endl;
         cout << "Content after       " << content_after << endl;
         cout << "Water balance error " << balance << endl;
         cout << endl;
      }

      double N_content_after=0.0;
      for (i=0;i<TotalNumberOfLayers-1;i++) // Udregn budget
         N_content_after=N_content_after+soilLayerArray[i]->GetNitrate().n+soilLayerArray[i]->GetAmmonium().n;
      N_leached=NitrateLeached->n+AmmoniumLeached->n;
      double N_return_surface=(surfaceNitrate->n+surfaceAmmonium->n)*(1.0-f);
      double N_balance=N_content_after-N_content_before-N_added+N_leached+N_return_surface;
      if (fabs(N_balance)>1E-10)
      {
         cout << endl << theTime << endl;
         cout << "Added               " << N_added << endl;
         cout << "Leached             " << N_leached << endl;
         cout << "Returned to surface " << N_return_surface << endl;
         cout << "Content before      " << N_content_before << endl;
         cout << "Content after       " << N_content_after << endl;
         cout << "N balance error     " << N_balance << endl;
         cout << endl;
      }
      // -------------------------------------------------------------
   }
*/
if ((flowMethod!=4) && (flowMethod!=5))
{
   // Transport of water and solutes -------------------------------------------
   f=1.0; // f is fraction of surface water that entered the soil
   if (*surplus>0)
      f=(*surplus-standingWater)/(*surplus);

   int times=1;
   // times=ceil(max(1.0,(*surplus-standingWater)/5.0)); // Aliquot option !
#ifdef TETRANS
	double WaterLeached = *surplus*f;
   nitrogen NO3Leached = *surfaceNitrate*f;
   nitrogen NH4Leached = *surfaceAmmonium*f;
   for (int i=0;i<TotalNumberOfLayers-1;i++)
   	soilLayerArray[i]->ExchangeSubstance(&WaterLeached,&NO3Leached,&NH4Leached,&Chloride);
   *NitrateLeached = NO3Leached;
   *AmmoniumLeached = NH4Leached;

   soilLayerArray[TotalNumberOfLayers-1]->AddWater(WaterLeached,NO3Leached,NH4Leached,Chloride);
#else
   nitrogen Nadd = *surfaceNitrate*f + (*surfaceAmmonium*f);
   for (int j=0;j<times;j++)
   {
      soilLayerArray[0]->AddWater(*surplus*f/times,*surfaceNitrate*f/times,*surfaceAmmonium*f/times,Chloride);
      for (i=0;i<TotalNumberOfLayers;i++)
      {
         if (waterContent[i]>(maxContent[i]+0.1))
            theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - water content in soil layer exceeds the possible");
         soilLayerArray[i]->SetWaterFlux(totalFlux[i]/times);
         soilLayerArray[i]->SetEquilibrated(false);
         nitrogen NO3;
         nitrogen NH4;
         if (totalFlux[i]>0.0) // Downward movement
         {
            soilLayerArray[i]->RemoveWater(totalFlux[i]/times,&NO3,&NH4,&Chloride);
            if (i==TotalNumberOfLayers-2)
            {
               *NitrateLeached = NO3;
               *AmmoniumLeached = NH4;
            }
            if (i<(TotalNumberOfLayers-1))
               soilLayerArray[i+1]->AddWater(totalFlux[i]/times,NO3,NH4,Chloride);
         }
         else
            if (totalFlux[i]<0.0) // Upward movement
            {
               if (i<TotalNumberOfLayers-1)
                  soilLayerArray[i+1]->RemoveWater(-totalFlux[i]/times,&NO3,&NH4,&Chloride);
               if (i==TotalNumberOfLayers-2) // No In of nitrogen from lower boundary
               {
                  NO3.Clear();
                  NH4.Clear();
               }
               soilLayerArray[i]->AddWater(-totalFlux[i]/times,NO3,NH4,Chloride);
            }
      }
      for (i=0;i<TotalNumberOfLayers;i++) // Equilibrate layers with no water movement
         if (!soilLayerArray[i]->GetEquilibrated())
            soilLayerArray[i]->EquilibrateNitrogen(0.0);
   }
#endif
}
if (flowMethod!=5)
{
   *surplus=*surplus*(1.0-f);
   *surfaceNitrate=*surfaceNitrate*(1.0-f);
   *surfaceAmmonium=*surfaceAmmonium*(1.0-f);
}
   // End transport of water and solute ----------------------------------------
   timeConsumption->endProfiling(waterSimulation);
}

/****************************************************************************\
Performs infiltration of water, nitrate and ammonium in profile using the SWAT
water model. Simulates the transport of water and solutes with timestep one hour.
Solutes are equilibrated between mobile and immobile domain only once a day.
Optional use of convection-dispersion equation for transport in mobile domain.
(MEL 2007)
   surplus         - Water to be added to/returned from profile [mm]
   nitrateLeached  - Nitrate to the added to/returned from profile [g N/m�]
   ammoniumLeached - Ammonium to the added to/returned from profile [g N/m�]
\****************************************************************************/
void soilProfile::UpdateInfiltrationSWAT(double *surplus, nitrogen * surfaceNitrate,
                                     nitrogen * surfaceAmmonium, nitrogen *NitrateLeached,
                                     nitrogen *AmmoniumLeached, double Chloride)
{

   double pressurePotential[MaxSoilLayers],
      totalFlux[MaxSoilLayers],
      maxContent[MaxSoilLayers],
      waterContent[MaxSoilLayers],
      porosity[MaxSoilLayers],
      FC[MaxSoilLayers],
      satHydraulicConductivity[MaxSoilLayers],
      layerThickness[MaxSoilLayers],
      theta[MaxSoilLayers],
      thetaOld[MaxSoilLayers],
      thetaBack[MaxSoilLayers],
      thetaForward[MaxSoilLayers],
      dispersion[MaxSoilLayers],
      dispersionBack[MaxSoilLayers],
      dispersionForward[MaxSoilLayers],
      flowBack[MaxSoilLayers],
      flowForward[MaxSoilLayers],
      upperVector[MaxSoilLayers],
      midVector[MaxSoilLayers],
      lowerVector[MaxSoilLayers],
      soluteOldVector[MaxSoilLayers],
      soluteOld[MaxSoilLayers],
      soluteNew[MaxSoilLayers],
      mobileNitrateFlux[MaxSoilLayers],
      mobileAmmoniumFlux[MaxSoilLayers],
      mobileNitrateN15Flux[MaxSoilLayers],
      mobileAmmoniumN15Flux[MaxSoilLayers],
      mobileChlorideFlux[MaxSoilLayers],
      mobileWaterNew[MaxSoilLayers],
      mobileWaterOld[MaxSoilLayers],
   	immobileWater[MaxSoilLayers];

   double molecularDiff,
   	soluteInFlux,
   	soluteIn,
      soluteInOld,
      nitrateIn,
      nitrateN15In,
      ammoniumIn,
      ammoniumN15In,
      chlorideIn,
      nitrateInOld,
      nitrateN15InOld,
      ammoniumInOld,
      ammoniumN15InOld,
      chlorideInOld,
      standingWater,
      relativeWater,
      maxFlux;

   bool IsThereSoluteFlux[MaxSoilLayers];

   nitrogen surplusNH4[MaxSoilLayers];
   nitrogen surplusNO3[MaxSoilLayers];

   double simulationTime = 0.0;
   double deltaT=3600;

   timeConsumption->startProfiling(waterSimulation);

   int TotalNumberOfLayers = NumOfSoilLayers + ExtraLayers;
   pF_Curve * pF[MaxSoilLayers];
   int i;
	for (i=0;i<TotalNumberOfLayers;i++)
   {
		soilLayerArray[i]->ClearLeaching();             // set leaching to zero
      waterContent[i] = soilLayerArray[i]->GetTotalWater();
      pF[i] = soilLayerArray[i]->GetpF_Curve();
      porosity[i]=soilLayerArray[i]->GetPorosity();
      maxContent[i]=(soilLayerArray[i]->GetPorosity()*soilLayerArray[i]->GetThickness());
      totalFlux[i]=0.0;
      FC[i]=soilLayerArray[i]->GetFieldCapacity();
      surplusNH4[i].Clear();
      surplusNO3[i].Clear();
   }
   standingWater=*surplus;


   int soluteTransportMethod=1;     //if 1 use SLIM transport option

   for (i=0;i<TotalNumberOfLayers;i++)
   {
      satHydraulicConductivity[i] = pF[i]->GetConductivity(pF[i]->GetpF(maxContent[i]/soilLayerArray[i]->GetThickness())); //in m/s
      layerThickness[i]=soilLayerArray[i]->GetThickness();
   }

   while (simulationTime<86400)     //simulates 24 hours
   {
      double fluxInTimeStep[MaxSoilLayers];

      // Perform infiltration of top layer.
      infiltration=max(0.0,min(standingWater,maxContent[0]-waterContent[0]));          // in mm
      if (infiltration>0.0)
      {
         standingWater-=infiltration;
         waterContent[0]+=infiltration;
         if (waterContent[0]>maxContent[0]+0.0001)
            theMessage->WarningWithDisplay("soilProfile::UpdateInfiltration - top layer contains more water than physically possible");
      }
      else
         if (infiltration<(-1E-10))
            theMessage->WarningWithDisplay("soilProfile::UpdateInfiltrationSWAT - top layer contains more water than physically possible");

      for (i=0;i<TotalNumberOfLayers;i++)
         fluxInTimeStep[i]=0.0;

      for (i=0;i<TotalNumberOfLayers;i++)
      {
         double relativeWater = min(1.0,waterContent[i]/soilLayerArray[i]->GetThickness()); 		//volumetric water content
         pressurePotential[i] = -pF[i]->GetPressurePotential(relativeWater);  						// in m WC
         double NotFrozenWater = waterContent[i]-soilLayerArray[i]->GetIce();   						//in mm
         double travelTime = (maxContent[i]-FC[i])/(satHydraulicConductivity[i]/1000); 			//in s
         double soilWaterExcess=max(0.0,min(NotFrozenWater,waterContent[i]- FC[i])); 					//in mm
         if (soilWaterExcess>0.0)   // drainage
         {
            fluxInTimeStep[i] = soilWaterExcess*(1.0-exp(-deltaT/travelTime));   //in mm per timestep
            if (i<(TotalNumberOfLayers-1))
            {
               fluxInTimeStep[i] = max(0.0,min(fluxInTimeStep[i],maxContent[i+1]-waterContent[i+1]));       //reduce flux according to capacity in next layer
               waterContent[i+1] += fluxInTimeStep[i];                                      		//add flux to next layer
            }
            waterContent[i] -= fluxInTimeStep[i];                                         		//remove flux from present layer
            totalFlux[i]+=fluxInTimeStep[i];
         }
         else
         fluxInTimeStep[i]=0.0;

         if (waterContent[i]>maxContent[i]+0.0001)
            theMessage->WarningWithDisplay("soilProfile::UpdateInfiltrationSWAT - soil layer contains more than physically possible");
      }

      // Transport of water and solutes -------------------------------------------
      double f=1.0; // f is fraction of surface water that enter the soil
      if (*surplus>0)
         f=(*surplus-standingWater)/(*surplus);

      if (soluteTransportMethod==1)
      {
         nitrogen Nadd = *surfaceNitrate*f + (*surfaceAmmonium*f);
         soilLayerArray[0]->AddWater(*surplus*f,*surfaceNitrate*f,*surfaceAmmonium*f,Chloride);
         for (i=0;i<TotalNumberOfLayers;i++)
         {
            if (waterContent[i]>(maxContent[i]+0.1))
               theMessage->WarningWithDisplay("soilProfile::UpdateInfiltrationSWAT - water content in soil layer exceeds the possible");
            soilLayerArray[i]->SetWaterFlux(fluxInTimeStep[i]);
            soilLayerArray[i]->SetEquilibrated(false);
            nitrogen NO3;
            nitrogen NH4;
            if (fluxInTimeStep[i]>0.0) // Downward movement
            {
               soilLayerArray[i]->RemoveWaterHourly(fluxInTimeStep[i],&NO3,&NH4,&Chloride);
               if (i==TotalNumberOfLayers-1)
               {
                  *NitrateLeached = NO3;
                  *AmmoniumLeached = NH4;
               }
               if (i<(TotalNumberOfLayers-1))
                  soilLayerArray[i+1]->AddWater(fluxInTimeStep[i],NO3,NH4,Chloride);
            }
         }

         *surplus=*surplus*(1.0-f);
         *surfaceNitrate=*surfaceNitrate*(1.0-f);
         *surfaceAmmonium=*surfaceAmmonium*(1.0-f);
         //hvad med chloride!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }
      else
      // Use convection-dispersion equation on mobile domain
      {
         for (i=0;i<TotalNumberOfLayers;i++)
         {
            mobileWaterOld[i]=max(0.0,soilLayerArray[i]->GetTotalWater()-FC[i]);
            immobileWater[i]= soilLayerArray[i]->GetTotalWater()-mobileWaterOld[i];
            mobileWaterNew[i]=max(0.0,waterContent[i]-immobileWater[i]);  //
         }
         //solve individually for nitrate, ammonium and chloride
         for (int soluteType=0;soluteType<5;soluteType++)
         {
            if (soluteType==0)        //nitrate N
            {
               molecularDiff=1.902e-9;        //m2 s-1               //Change to diffusion coefficient in dilute solutions  !!!!!!!!!!!!
               for (i=0;i<TotalNumberOfLayers;i++)
                  if (mobileWaterOld[i]>0.0)
                     soluteOld[i]=soilLayerArray[i]->GetMobileNitrate().n/(mobileWaterOld[i]/1000.0);
                  else
                     soluteOld[i]=0.0;
               soluteIn= surfaceNitrate->n*f;
               nitrateInOld= nitrateIn;
               soluteInOld=nitrateInOld;
               nitrateIn=soluteIn;
            }
            else if (soluteType==1)    //ammonium N
            {
               molecularDiff=1.957e-9;        //m2 s-1
               for (i=0;i<TotalNumberOfLayers;i++)
                  if (mobileWaterOld[i]>0.0)
                     soluteOld[i]=soilLayerArray[i]->GetMobileAmmonium().n/(mobileWaterOld[i]/1000.0);
                  else
                     soluteOld[i]=0.0;
               soluteIn= surfaceAmmonium->n*f;
               ammoniumInOld= ammoniumIn;
               soluteInOld=ammoniumInOld;
                ammoniumIn=soluteIn;
            }
            else if  (soluteType==2)    //chloride
            {
               molecularDiff=2.032e-9;        //m2 s-1
               for (i=0;i<TotalNumberOfLayers;i++)
                  if (mobileWaterOld[i]>0.0)
                     soluteOld[i]=soilLayerArray[i]->GetMobileChloride()/(mobileWaterOld[i]/1000.0);
                  else
                     soluteOld[i]=0.0;
               soluteIn=Chloride*f;
               chlorideInOld= chlorideIn;
               soluteInOld=chlorideInOld;
               chlorideIn=soluteIn;
            }
            else if  (soluteType==3)    //nitrate 15N
            {
               molecularDiff=1.902e-9;        //m2 s-1
               for (i=0;i<TotalNumberOfLayers;i++)
                  if (mobileWaterOld[i]>0.0)
                     soluteOld[i]=soilLayerArray[i]->GetMobileNitrate().n15/(mobileWaterOld[i]/1000.0);
                  else
                     soluteOld[i]=0.0;
               soluteIn=surfaceNitrate->n15*f;
               nitrateN15InOld= nitrateN15In;
               soluteInOld=nitrateN15InOld;
               nitrateN15In=soluteIn;
            }
            else if  (soluteType==4)    //ammonium 15N
            {
               molecularDiff=1.957e-9;        										  //m2 s-1
               for (i=0;i<TotalNumberOfLayers;i++)
                  if (mobileWaterOld[i]>0.0)
                     soluteOld[i]=soilLayerArray[i]->GetMobileAmmonium().n15/(mobileWaterOld[i]/1000.0);   //g/m2
                  else
                     soluteOld[i]=0.0;
               soluteIn=surfaceAmmonium->n15*f;
               ammoniumN15InOld= ammoniumN15In;
               soluteInOld=ammoniumN15InOld;
               ammoniumN15In=soluteIn;
            }

            bool nonZeroConcentration=false;
            for (i=0;i<TotalNumberOfLayers;i++)
               if (soluteOld[i]>0.0)
                   nonZeroConcentration=true;
            if (soluteIn>0.0)
               nonZeroConcentration=true;


               double tortuosity=0.6;             //
               double dispersionLength=0.02;       									  //m

               for (i=0;i<TotalNumberOfLayers;i++)
               {
                  theta[i]=min(1.0,mobileWaterNew[i]/soilLayerArray[i]->GetThickness());
                  thetaOld[i]=min(1.0,mobileWaterOld[i]/soilLayerArray[i]->GetThickness());
                  soluteNew[i]= soluteOld[i];
               }

            if(nonZeroConcentration)  //bypass if no concentration is above zero
            {
            //determine where there is a solute flux (calculated out of the layer)
               for (i=0;i<TotalNumberOfLayers;i++)
                  if ((fluxInTimeStep[i]>0.0) ||
                  ((mobileWaterOld[i+1]>0.0) &&
                  (mobileWaterOld[i]>0.0)))
                     IsThereSoluteFlux[i]=true;
                  else
                     IsThereSoluteFlux[i]=false;

               //calculate centered values
               for (i=0;i<TotalNumberOfLayers;i++)
               {
                  if (i==0)
                  {
                     thetaBack[i]=0.5*(theta[i]+thetaOld[i]);
                     thetaForward[i]=0.25*(theta[i]+theta[i+1]+thetaOld[i]+thetaOld[i+1]);
                     flowBack[i]=infiltration/deltaT/1000;
                     flowForward[i]=fluxInTimeStep[i]/deltaT/1000;
                     if (theta[i]==0.0||thetaOld[i]==0.0)
                        dispersion[i]=0.0;
                     else
                        dispersion[i]=dispersionLength*0.5*(flowForward[i]+flowBack[i])/(0.5*(theta[i]+thetaOld[i]))+molecularDiff*tortuosity;
//                     if ((theta[i]==0.0)||(theta[i-1]==0.0))
                        dispersionBack[i]=0.0;
//                     else
//                        dispersionBack[i]=dispersionLength*flowBack[i]/thetaBack[i]+molecularDiff*tortuosity;
                     if ((theta[i]==0.0) || (thetaOld[i]==0.0)||(theta[i+1]==0.0)||(thetaOld[i+1]==0.0))
                        dispersionForward[i]=0.0;
                     else
                        dispersionForward[i]=dispersionLength*flowForward[i]/thetaForward[i]+molecularDiff*tortuosity;
                  }
                  else if (i==TotalNumberOfLayers-1)
                  {
                     thetaBack[i]=0.25*(theta[i-1]+theta[i]+thetaOld[i-1]+thetaOld[i]);
                     thetaForward[i]=0.5*(theta[i]+thetaOld[i]);
                     flowBack[i]=fluxInTimeStep[i-1]/deltaT/1000;
                     flowForward[i]=fluxInTimeStep[i]/deltaT/1000;
                     if (theta[i]==0.0||thetaOld[i]==0.0)
                        dispersion[i]=0.0;
                     else
                        dispersion[i]=dispersionLength*0.5*(flowForward[i]+flowBack[i])/(0.5*(theta[i]+thetaOld[i]))+molecularDiff*tortuosity;
//                        if (thetaBack[i]==0.0)
                     if ((theta[i-1]==0.0) || (thetaOld[i-1]==0.0)||(theta[i]==0.0)||(thetaOld[i]==0.0))
                        dispersionBack[i]=0.0;
                     else
                        dispersionBack[i]=dispersionLength*flowBack[i]/thetaBack[i]+molecularDiff*tortuosity;
//                        if (thetaForward[i]==0.0)
                     if ((theta[i]==0.0) || (thetaOld[i]==0.0))
                        dispersionForward[i]=0.0;
                     else
                        dispersionForward[i]=dispersionLength*flowForward[i]/thetaForward[i]+molecularDiff*tortuosity;
                  }
                  else
                  {
                     thetaBack[i]=0.25*(theta[i-1]+theta[i]+thetaOld[i-1]+thetaOld[i]);
                     thetaForward[i]=0.25*(theta[i]+theta[i+1]+thetaOld[i]+thetaOld[i+1]);
                     flowBack[i]=fluxInTimeStep[i-1]/deltaT/1000;
                     flowForward[i]=fluxInTimeStep[i]/deltaT/1000;
                     if (theta[i]==0.0 ||thetaOld[i]==0.0)
                        dispersion[i]=0.0;
                     else
                        dispersion[i]=dispersionLength*0.5*(flowForward[i]+flowBack[i])/(0.5*(theta[i]+thetaOld[i]))+molecularDiff*tortuosity;
//                        if (thetaBack[i]==0.0)
                     if ((theta[i-1]==0.0) || (thetaOld[i-1]==0.0)||(theta[i]==0.0)||(thetaOld[i]==0.0))
                        dispersionBack[i]=0.0;
                     else
                        dispersionBack[i]=dispersionLength*flowBack[i]/thetaBack[i]+molecularDiff*tortuosity;
//                        if (thetaForward[i]==0.0)
                     if ((theta[i]==0.0) || (thetaOld[i]==0.0)||(theta[i+1]==0.0)||(thetaOld[i+1]==0.0))
                        dispersionForward[i]=0.0;
                     else
                        dispersionForward[i]=dispersionLength*flowForward[i]/thetaForward[i]+molecularDiff*tortuosity;
                  }
               }
               double a[MaxSoilLayers],b[MaxSoilLayers],c[MaxSoilLayers],
                        da[MaxSoilLayers],db[MaxSoilLayers],dc[MaxSoilLayers];

               for (i=0;i<TotalNumberOfLayers;i++)
               {
                  a[i]= dispersionBack[i]*DistPreviousLayerInv[i]/(2*layerThickness[i])
                        +flowBack[i]/(4*layerThickness[i]);

                  b[i]= -theta[i]/deltaT
                        -dispersionForward[i]*DistNextLayerInv[i]/(2*layerThickness[i])
                        -dispersionBack[i]*DistPreviousLayerInv[i]/(2*layerThickness[i])
                        -flowForward[i]/(4*layerThickness[i])
                        +flowBack[i]/(4*layerThickness[i]);

                  c[i]= dispersionForward[i]*DistNextLayerInv[i]/(2*layerThickness[i])
                        -flowForward[i]/(4*layerThickness[i]);

                  da[i]=-dispersionBack[i]*DistPreviousLayerInv[i]/(2*layerThickness[i])
                        -flowBack[i]/(4*layerThickness[i]);

                  db[i]=-thetaOld[i]/deltaT
                        +dispersionForward[i]*DistNextLayerInv[i]/(2*layerThickness[i])
                        +dispersionBack[i]*DistPreviousLayerInv[i]/(2*layerThickness[i])
                        +flowForward[i]/(4*layerThickness[i])
                        -flowBack[i]/(4*layerThickness[i]);

                  dc[i]= -dispersionForward[i]*DistNextLayerInv[i]/(2*layerThickness[i])
                         +flowForward[i]/(4*layerThickness[i]);

               }
               //Assign values to vectors
               for (i=0;i<TotalNumberOfLayers;i++)
               {
                  if (i==0)
                  {
                     double b_null,d_null;
                     if (soluteIn>0.0)
                     {
                        soluteInFlux = soluteIn/deltaT;
                        b_null=(dispersion[i]*DistPreviousLayerInv[i]-0.5*flowBack[i])/(2*(dispersion[i]*DistPreviousLayerInv[i]+0.5*flowBack[i]));
                        d_null=(soluteInFlux+(dispersion[i]*DistPreviousLayerInv[i]-0.5*flowBack[i]))*soluteOld[i]/(2*(dispersion[i]*DistPreviousLayerInv[i]+0.5*flowBack[i]));
                     }
                     else
                     {
                        b_null=0.0;
                        d_null=0.0;
                     }
                     lowerVector[i]= 0.0;
                     midVector[i]= b[i]+ b_null*a[i];
                     upperVector[i]= c[i];
                     soluteOldVector[i]= soluteOld[i]*(da[i]+db[i]) + soluteOld[i+1]* dc[i]
                                 -d_null*a[i];
                  }
                  else if (i==TotalNumberOfLayers-1)
                  {
                     lowerVector[i]= a[i];
                     midVector[i]=b[i]+c[i];
                     upperVector[i]= 0.0;
                     soluteOldVector[i]= soluteOld[i-1]* da[i]+ soluteOld[i]*(db[i]+dc[i]);
                  }
                  else
                  {
                     lowerVector[i]= a[i];
                     midVector[i]= b[i];
                     upperVector[i]= c[i];
                     soluteOldVector[i]= soluteOld[i-1]* da[i]+ soluteOld[i]*db[i] + soluteOld[i+1]* dc[i];
                  }
               }


               //correct vectors for start of mobile domain
               int k=0;
               int l=0;
               while (IsThereSoluteFlux[l]==false)
               {
                  k=l+1;
                  l=1+l;
               }

               if (k>0)
               {
                  for (i=0;i<TotalNumberOfLayers-k;i++)
                  {
                     if (i==0)
                     {
                        midVector[i]=midVector[i+k]+dispersion[i+k]*DistPreviousLayerInv[i+k]/(2*layerThickness[i+k]);
                        lowerVector[i]=0.0;
                        soluteOldVector[i]=soluteOldVector[i+k]+dispersion[i+k]*DistPreviousLayerInv[i+k]/(2*layerThickness[i+k])*soluteOld[i+k];
                        upperVector[i]= upperVector[i+k];
                     }
                     else
                     {
                        if ((IsThereSoluteFlux[i+k-1]==false)&&
                           (IsThereSoluteFlux[i+k]==false))
                        {
                           lowerVector[i]=0.0;
                           midVector[i]= 0.0;
                           upperVector[i]= 0.0;
                           soluteOldVector[i]=0.0;
                        }
                        else if ((IsThereSoluteFlux[i+k-1]==false)&&
                           (IsThereSoluteFlux[i+k]==true))
                        {
                           lowerVector[i]=0.0;
                           midVector[i]= midVector[i+k]+dispersion[i+k]*DistPreviousLayerInv[i+k]/(2*layerThickness[i+k])
                                    +flowBack[i]/(4*layerThickness[i]);
                           soluteOldVector[i]=soluteOldVector[i+k]+soluteOld[i]*(dispersion[i+k]*DistPreviousLayerInv[i+k]/(2*layerThickness[i+k])
                                    +flowBack[i]/(4*layerThickness[i]));
                           upperVector[i]= upperVector[i+k];
                       }
                        else if  ((IsThereSoluteFlux[i+k-1]==true)&&
                           (IsThereSoluteFlux[i+k]==false))
                        {
                           lowerVector[i]=lowerVector[i+k];
                           midVector[i]= midVector[i+k]+dispersion[i+k]*DistNextLayerInv[i+k]/(2*layerThickness[i+k])
                                    -flowForward[i+k]/(4*layerThickness[i+k]);
                           soluteOldVector[i]=soluteOldVector[i+k]+soluteOld[i+k]*(dispersion[i+k]*DistNextLayerInv[i+k]/(2*layerThickness[i+k])
                                    -flowForward[i+k]/(4*layerThickness[i+k]));
                           upperVector[i]= 0.0;
                        }
                        else
                        {
                           lowerVector[i]=lowerVector[i+k];
                           midVector[i]= midVector[i+k];
                           soluteOldVector[i]=soluteOldVector[i+k];
                           upperVector[i]= upperVector[i+k];
                        }
                     }
                  }
               }

               //Solve with the double sweep method (function tridag)
               Tridag(lowerVector, midVector, upperVector, soluteOldVector, soluteNew, TotalNumberOfLayers-k);

               if (k>0)
               {
                  for (i=TotalNumberOfLayers-1;i>k-1;i--)
                     soluteNew[i]=soluteNew[i-k];
                  for (i=0;i<k;i++)
                     soluteNew[i]=0.0;
               }

            for (i=0;i<TotalNumberOfLayers;i++)
               if (soluteNew[i]<0.0)
               {
                  theMessage->WarningWithDisplay("soilProfile::UpdateInfiltrationSWAT - negative concentration");
                  cout << "solutetype " << soluteType << " in layer " << i << endl;
               }

            //Calculate the fluxes between layers
            //mobileXXXXFlux[i] refer to the flux out of the layer can be both negative and positive

            if (soluteType==0)
               for (i=0;i<TotalNumberOfLayers;i++)
                  if(i==0)
                  mobileNitrateFlux[i]=nitrateIn-soluteNew[i]*mobileWaterNew[i]/1000.0+soluteOld[i]*mobileWaterOld[i]/1000.0;
                  else
                  mobileNitrateFlux[i]=mobileNitrateFlux[i-1]-soluteNew[i]*mobileWaterNew[i]/1000.0+soluteOld[i]*mobileWaterOld[i]/1000.0;

            else if (soluteType==1)
               for (i=0;i<TotalNumberOfLayers;i++)
                  if(i==0)
                  mobileAmmoniumFlux[i]=ammoniumIn-soluteNew[i]*mobileWaterNew[i]/1000.0+soluteOld[i]*mobileWaterOld[i]/1000.0;
                  else
                  mobileAmmoniumFlux[i]=mobileAmmoniumFlux[i-1]-soluteNew[i]*mobileWaterNew[i]/1000.0+soluteOld[i]*mobileWaterOld[i]/1000.0;

            else if  (soluteType==2)
               for (i=0;i<TotalNumberOfLayers;i++)
                  if(i==0)
                  mobileChlorideFlux[i]=chlorideIn-(soluteNew[i]*mobileWaterNew[i]-soluteOld[i]*mobileWaterOld[i])/1000.0;
                  else
                  mobileChlorideFlux[i]=mobileChlorideFlux[i-1]-(soluteNew[i]*mobileWaterNew[i]-soluteOld[i]*mobileWaterOld[i])/1000.0;

            else if  (soluteType==3)
               for (i=0;i<TotalNumberOfLayers;i++)
                  if(i==0)
                  mobileNitrateN15Flux[i]=nitrateN15In-(soluteNew[i]*mobileWaterNew[i]-soluteOld[i]*mobileWaterOld[i])/1000.0;
                  else
                  mobileNitrateN15Flux[i]=mobileNitrateN15Flux[i-1]-(soluteNew[i]*mobileWaterNew[i]-soluteOld[i]*mobileWaterOld[i])/1000.0;

            else if  (soluteType==4)
               for (i=0;i<TotalNumberOfLayers;i++)
                  if(i==0)
                  mobileAmmoniumN15Flux[i]=ammoniumN15In-(soluteNew[i]*mobileWaterNew[i]-soluteOld[i]*mobileWaterOld[i])/1000.0;
                  else
                  mobileAmmoniumN15Flux[i]=mobileAmmoniumN15Flux[i-1]-(soluteNew[i]*mobileWaterNew[i]-soluteOld[i]*mobileWaterOld[i])/1000.0;

            }  //End bypass if nonzeroconcentration is false
            else  //non-zero concentration true
            {
               if (soluteType==0)
                  for (i=0;i<TotalNumberOfLayers;i++)
                     mobileNitrateFlux[i]=0.0;

               else if (soluteType==1)
                  for (i=0;i<TotalNumberOfLayers;i++)
                     mobileAmmoniumFlux[i]=0.0;

               else if  (soluteType==2)
                  for (i=0;i<TotalNumberOfLayers;i++)
                     mobileChlorideFlux[i]=0.0;

               else if  (soluteType==3)
                  for (i=0;i<TotalNumberOfLayers;i++)
                     mobileNitrateN15Flux[i]=0.0;

               else if  (soluteType==4)
                  for (i=0;i<TotalNumberOfLayers;i++)
                     mobileAmmoniumN15Flux[i]=0.0;
            }
         }  //all solutes

         //update budgets and assign values
         nitrogen aMobileNitrateFlux, aMobileAmmoniumFlux, aNitrateIn, aAmmoniumIn;
         double aMobileChlorideFlux, aChlorideIn;

         aNitrateIn.SetBoth(nitrateIn, nitrateN15In);
         aAmmoniumIn.SetBoth(ammoniumIn, ammoniumN15In);
         aChlorideIn=chlorideIn;

         for (i=0;i<TotalNumberOfLayers;i++)
         {
            aMobileNitrateFlux.SetBoth(mobileNitrateFlux[i],mobileNitrateN15Flux[i]);
            aMobileAmmoniumFlux.SetBoth(mobileAmmoniumFlux[i],mobileAmmoniumN15Flux[i]);
            aMobileChlorideFlux= mobileChlorideFlux[i];
            if (i==0)
            {
               soilLayerArray[0]->AddWaterAndSolutes(infiltration,aNitrateIn,
                     aAmmoniumIn,aChlorideIn);
               soilLayerArray[1]->AddWaterAndSolutes(fluxInTimeStep[0],aMobileNitrateFlux,
                                 aMobileAmmoniumFlux,aMobileChlorideFlux);                    //Add to next
               soilLayerArray[0]->RemoveWaterAndSolutes(fluxInTimeStep[0],aMobileNitrateFlux,
                                 aMobileAmmoniumFlux,aMobileChlorideFlux);                    //remove from current
            }
            else if (i==TotalNumberOfLayers-1)
            {
               soilLayerArray[i]->RemoveWaterAndSolutes(fluxInTimeStep[i],aMobileNitrateFlux,
                              aMobileAmmoniumFlux,aMobileChlorideFlux);                    //remove from current
               *NitrateLeached = *NitrateLeached+aMobileNitrateFlux;                       //update leaching
               *AmmoniumLeached = *AmmoniumLeached+aMobileAmmoniumFlux;
            }
            else
            {
               soilLayerArray[i+1]->AddWaterAndSolutes(fluxInTimeStep[i],aMobileNitrateFlux,
                              aMobileAmmoniumFlux,aMobileChlorideFlux);                    //Add to next layer
               soilLayerArray[i]->RemoveWaterAndSolutes(fluxInTimeStep[i],aMobileNitrateFlux,
                              aMobileAmmoniumFlux,aMobileChlorideFlux);                    //remove from current
            }
         }
         *surplus=*surplus*(1.0-f);
         *surfaceNitrate=*surfaceNitrate*(1.0-f);
         *surfaceAmmonium=*surfaceAmmonium*(1.0-f);
         //hvad med chloride!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
      }

      simulationTime+=deltaT;
   }  // End transport of water and solute

   //Equilibrate between mobile and immobile water   (once a day)
   for (i=0;i<TotalNumberOfLayers;i++)
   if (waterContent[i]<FC[i])
     	soilLayerArray[i]->EquilibrateNitrogen(0.0);
   else
      soilLayerArray[i]->EquilibrateNitrogen(waterContent[i]-FC[i]);

   EvaporationContent=min(EvaporationCapacity,infiltration+EvaporationContent);

   for (i=0;i<TotalNumberOfLayers;i++)
   {
      soilLayerArray[i]->SetWaterFlux(totalFlux[i]);
   }

   // End transport of water and solute ----------------------------------------
   timeConsumption->endProfiling(waterSimulation);
}

/****************************************************************************\
Calculates soil water content for the profile.
   returns       - Water content [mm]
\****************************************************************************/
double soilProfile::GetTotalWater()
{
 double Content = 0;
 soilLayer * current = first;
 while (current != NULL)
 {
  Content+=current->GetTotalWater();
  current = current->Next();
 }
 return Content;
}

/****************************************************************************\
Calculates soil water content for a portion of the profile.
   startDep      - Start depth of layer to be calculated for [mm]
   thick         - Thickness of layer to be calculated for [mm]
   returns       - Plant available water [mm]
\****************************************************************************/
double soilProfile::GetTotalWater(double startDepth,
									  double thickness)
{
 double Content = 0;
 soilLayer * current = first;
 while (current != NULL)
 {
  Content+=current->GetTotalWater(startDepth,thickness);
  current = current->Next();
 }
 return Content;
}

/****************************************************************************\
Calculates plant available soil water content for a portion of the profile.
   startDep      - Start depth of layer to be calculated for [mm]
   thick         - Thickness of layer to be calculated for [mm]
   returns       - Plant available water [mm]
\****************************************************************************/
double soilProfile::GetAvailWater(double startDepth,double thickness)
{
 double Content = 0;
 soilLayer * current = first;
 while (current != NULL)
 {
  Content+=current->GetAvailWater(startDepth,thickness);
  current = current->Next();
 }
 return Content;
}

/****************************************************************************\
Calculates capacity for plant available water for a portion of the profile.
   startDep      - Start depth of layer to be calculated for [mm]
   thick         - Thickness of layer to be calculated for [mm]
   returns       - Capacity for plant available water [mm]
\****************************************************************************/
double soilProfile::GetAvailCapacity(double startDepth,
												 double thickness)
{
 double Capacity = 0.0;
 soilLayer * current = first;
 while (current != NULL)
 {
  Capacity+=current->GetAvailCapacity(startDepth,thickness);
  current = current->Next();
 }
 return Capacity;
}

/****************************************************************************\
Calculates field capacity for water for a portion of the profile.
   startDep      - Start depth of layer to be calculated for [mm]
   thick         - Thickness of layer to be calculated for [mm]
   returns       - Capacity for plant available water [mm]
\****************************************************************************/
double soilProfile::GetFieldCapacity(double startDepth,
												 double thickness)
{
 double Capacity = 0.0;
 soilLayer * current = first;
 while (current != NULL)
 {
  Capacity+=current->GetFieldCapacity(startDepth,thickness);
  current = current->Next();
 }
 return Capacity;
}

/****************************************************************************\
Calculates wilting capacity for for a portion of the profile.
   startDep      - Start depth of layer to be calculated for [mm]
   thick         - Thickness of layer to be calculated for [mm]
   returns       - Capacity for plant available water [mm]
\****************************************************************************/
double soilProfile::GetWiltCapacity(double startDepth,
												double thickness)
{
 double Capacity = 0.0;
 soilLayer * current = first;
 while (current != NULL)
 {
  Capacity+=current->GetWiltCapacity(startDepth,thickness);
  current = current->Next();
 }
 return Capacity;
}

/****************************************************************************\
\****************************************************************************/
double soilProfile::GetDiscreteDepth(double depth)
{
   int i=0;
	while (soilLayerArray[i]->GetEndDepth()<depth && i<NumOfSoilLayers)
      i++;
   return soilLayerArray[i]->GetEndDepth();
}

/****************************************************************************\
The temperature of a given soil layer is defined at the top of the layer
\****************************************************************************/
double soilProfile::GetTemperature(double depth)
{
	int i=0;
	while (soilLayerArray[i]->GetStartDepth()<=depth && i<NumOfSoilLayers)
		i++;
   if (i==(NumOfSoilLayers-1))
      return soilLayerArray[i]->GetTemperature();
	double a=depth-soilLayerArray[i-1]->GetStartDepth();
   double b=soilLayerArray[i]->GetStartDepth()-depth;
   return soilLayerArray[i-1]->GetTemperature()*b/(a+b)
          +soilLayerArray[i]->GetTemperature()*a/(a+b);
}

/****************************************************************************\
\****************************************************************************/
double soilProfile::GetDrainage(double depth)
{
	if (depth<=(soilLayerArray[0]->GetThickness()/2.0))
		return soilLayerArray[0]->GetWaterFlux();
	if (depth>=(soilLayerArray[NumOfSoilLayers-1]->GetStartDepth()
		 +soilLayerArray[NumOfSoilLayers-1]->GetThickness()/2.0))
		return soilLayerArray[NumOfSoilLayers-1]->GetWaterFlux();
	int i=0;
	while ((soilLayerArray[i]->GetStartDepth()+soilLayerArray[i]->GetThickness()/2.0)<depth)
		i++;
   if (i==(NumOfSoilLayers-1))
   	return soilLayerArray[i]->GetWaterFlux();
   return soilLayerArray[i-1]->GetWaterFlux();
}

/****************************************************************************\
Estimated maximum evaporation from whole profile
\****************************************************************************/
double soilProfile::GetMaximumEvaporation()
{
   soilLayer * current = first;
   double sum = 0.0;
	while (current != NULL)
  	{
		sum += max(0.,current->GetAvailWater())*exp(-EvapExtCoef*current->GetCenterDepth());       //current->GetThickness()*
		current = current->Next();
  	}
	return sum;
}

/****************************************************************************\
Subtracts evaporation from profile
   evaporation   - Evaporation to be subtracted [mm].
\****************************************************************************/
void soilProfile::SubtractEvaporation(double soilEvaporation)
{
 /*
 // Newest, but presently discarded method
 soilLayer * current = first;
 while (soilEvaporation>0)
  {
  	double subtractAmount = min(soilEvaporation,current->GetAvailWater());
   soilEvaporation -= subtractAmount;
   current->SubtractEvaporation(subtractAmount);
	current = current->Next();
 }
 */
 // Original method - extended with the EvaporationContent part
 soilLayer * current = first;
 double a;

 double directEvaporation=min(min(soilEvaporation,EvaporationContent),soilLayerArray[0]->GetAvailWater());
 soilEvaporation-=directEvaporation;
 EvaporationContent-=directEvaporation;
 soilLayerArray[0]->SubtractEvaporation(directEvaporation);

 if (soilEvaporation>0)
 {
  double sum = GetMaximumEvaporation();
  while (current != NULL && sum>0)
  {
	a = //current->GetThickness()*
		 max(0.,current->GetAvailWater())*exp(-EvapExtCoef*current->GetCenterDepth());
   current->SubtractEvaporation(soilEvaporation*a/sum);
	current = current->Next();
  }
 }

}

/****************************************************************************\
Returns the water pressure potential in a given depth
\****************************************************************************/
double soilProfile::GetPressurePotential(double depth)
{
	if (depth<=(soilLayerArray[0]->GetThickness()/2.0))
		return soilLayerArray[0]->GetPressurePotential();
	if (depth>=(soilLayerArray[NumOfSoilLayers-1]->GetStartDepth()
		 +soilLayerArray[NumOfSoilLayers-1]->GetThickness()/2.0))
		return soilLayerArray[NumOfSoilLayers-1]->GetPressurePotential();
	int i=0;
	while ((soilLayerArray[i]->GetStartDepth()+soilLayerArray[i]->GetThickness()/2.0)<depth)
		i++;
   if (i==(NumOfSoilLayers-1))
   	return soilLayerArray[i]->GetPressurePotential();
   return soilLayerArray[i-1]->GetPressurePotential();
}

/****************************************************************************\
Calculates the maximum transpiration from the soil profile
   rootRadius     - Root radius [m]
   rootpF         - Water potential at root surface [pF]
   rootLengthList - List of root lengths for each layer [m/m�]
   returns        - Maximum transpiration [mm]
\****************************************************************************/
double soilProfile::MaxTranspiration(linkList <rootStructure>* rootList)
{
 if (rootList==NULL)
  return 0.0;   // No transpiration without roots
 double maxTranspiration = 0.0;
 double rootRadius, rootpF;
 double* rootLengthList;
 linkList<rootStructure>::PS currentRoot;
 rootList->PeekHead(currentRoot);
 while (currentRoot != NULL)
 {
  rootRadius = currentRoot->element->rootRadius;
  rootpF = currentRoot->element->rootpF;
  rootLengthList = currentRoot->element->rootLengthList;
  soilLayer * current = first;
  while (current != NULL)
  {
   maxTranspiration += current->MaxTranspiration(rootRadius,rootpF,*rootLengthList);
   rootLengthList++;
   current = current->Next();
  }
  rootList->OneStep(currentRoot);
 }
 return maxTranspiration;
}

/****************************************************************************\
Generates average total root
\****************************************************************************/
void soilProfile::EstimateTotalRoot(rootStructure* TotalRoot,linkList<rootStructure>* rootList)
{
	TotalRoot->rootRadius = TotalRoot->rootpF = TotalRoot->NitrateUptakeRate = TotalRoot->AmmoniumUptakeRate= 0.0;
   TotalRoot->MinimumSoilNitrate = TotalRoot->MinimumSoilAmmonium= 0.0;
   TotalRoot->rootLengthList = new double[MaxSoilLayers]; // LEAK !!!
   for (int j=0;j<MaxSoilLayers;j++) TotalRoot->rootLengthList[j] = 0.0;

   double TotalPlantRootLength[MaxPlants];
   double TotalRootLength = 0.0;
   int AttackerNumber = 0;
   if (rootList) AttackerNumber = rootList->NumOfNodes();

   for (int i=0;i<AttackerNumber;i++)
   {
      rootStructure* currentRoot = rootList->ElementAtNumber(i);
      TotalPlantRootLength[i] = 0.0;
      for (int j=0;j<MaxSoilLayers;j++)
   		TotalPlantRootLength[i] += currentRoot->rootLengthList[j];
      TotalRootLength += TotalPlantRootLength[i];
   }
   if (TotalRootLength>0)
   {
	   for (int i=0;i<AttackerNumber;i++)
	   {
	   	rootStructure* currentRoot = rootList->ElementAtNumber(i);
	      TotalRoot->rootRadius += TotalPlantRootLength[i]/TotalRootLength*currentRoot->rootRadius;
	   	TotalRoot->rootpF += TotalPlantRootLength[i]/TotalRootLength*currentRoot->rootpF;
			TotalRoot->NitrateUptakeRate += TotalPlantRootLength[i]/TotalRootLength*currentRoot->NitrateUptakeRate;
			TotalRoot->AmmoniumUptakeRate += TotalPlantRootLength[i]/TotalRootLength*currentRoot->AmmoniumUptakeRate;
			TotalRoot->MinimumSoilNitrate += TotalPlantRootLength[i]/TotalRootLength*currentRoot->MinimumSoilNitrate;
			TotalRoot->MinimumSoilAmmonium += TotalPlantRootLength[i]/TotalRootLength*currentRoot->MinimumSoilAmmonium;

	      for (int j=0;j<MaxSoilLayers;j++)
	   		TotalRoot->rootLengthList[j] += currentRoot->rootLengthList[j];
	   }
   }
}

/****************************************************************************\
Subtracts estimated transpiration from the water in the layer.
\****************************************************************************/
void soilProfile::SubtractTranspiration(linkList<rootStructure>* rootList)
{
   double waterBefore = soilLayerArray[0]->GetAvailWater();
   double PlantDemand[MaxPlants];
   double WaterUptakeAbility[MaxSoilLayers*MaxPlants];
   double TotalWaterUptakeAbility[MaxSoilLayers*MaxPlants];
   double WaterResource[MaxSoilLayers];
   double PlantUptake[MaxPlants*MaxSoilLayers];

   int AttackerNumber = rootList->NumOfNodes();
   if (AttackerNumber>MaxPlants)
   {
      cout << GetLongName() << endl;
		theMessage->FatalError("soilProfile::SubtractTranspiration maximum number of crops exceeded.");
   }
   for (int i=0;i<AttackerNumber;i++)
   	PlantDemand[i] = max(0.0,rootList->ElementAtNumber(i)->transpirationDemand);

   rootStructure *TotalRoot = new rootStructure;
   EstimateTotalRoot(TotalRoot,rootList);

   int ResourceNumber = 0;
   soilLayer * currentSoilLayer = first;
   while (currentSoilLayer != NULL)
   {
		currentSoilLayer = currentSoilLayer->Next();
	   ResourceNumber++;
	}
	double TotalPlantUptake[MaxPlants];
   for (int j=0; j<AttackerNumber;j++)
   	TotalPlantUptake[j] = 0.0;
   linkList <rootStructure>::PS currentRoot;
   rootList->PeekHead(currentRoot);
   int PlantIndex = 0;
   while (currentRoot != NULL)
   {
   	int SoilLayerIndex = 0;
      soilLayer * currentSoilLayer = first;
      while (currentSoilLayer != NULL)
      {
      	WaterUptakeAbility[SoilLayerIndex+PlantIndex*ResourceNumber] = currentSoilLayer->MaxTranspiration(currentRoot->element);
         currentSoilLayer = currentSoilLayer->Next();
         SoilLayerIndex++;
      }
      rootList->OneStep(currentRoot);
      PlantIndex++;
   }
   currentSoilLayer = first;
   int SoilLayerIndex = 0;
   while (currentSoilLayer != NULL)
   {
   	TotalWaterUptakeAbility[SoilLayerIndex] = currentSoilLayer->MaxTranspiration(TotalRoot);
      WaterResource[SoilLayerIndex] = min(currentSoilLayer->GetTotalWater()-currentSoilLayer->GetIce(),TotalWaterUptakeAbility[SoilLayerIndex]);
      currentSoilLayer = currentSoilLayer->Next();
      SoilLayerIndex++;
   }

   ResourceCompetition(ResourceNumber, AttackerNumber, PlantDemand, &WaterUptakeAbility[0], &WaterResource[0], &PlantUptake[0]);
#ifdef __ANSICPP__
   delete [] TotalRoot->rootLengthList; // BMP added June 2002
#else
   delete [sizeof(double)*MaxSoilLayers] TotalRoot->rootLengthList; // BMP added June 2002
#endif
   delete TotalRoot;

   if (rootList!=NULL) // Perform only if plants has roots
   {
   	soilLayer * currentSoilLayer = first;
      int SoilLayerIndex = 0;
      while (currentSoilLayer != NULL)
      {
      	double subtractTransp = 0.0;
         for (int j=0;j<AttackerNumber;j++)
         {
         	double  transpirationRate = PlantUptake[SoilLayerIndex+j*ResourceNumber];
            transpirationRate = min(transpirationRate,currentSoilLayer->MaxTranspiration(rootList->ElementAtNumber(j)));
            subtractTransp += transpirationRate;
            TotalPlantUptake[j] += transpirationRate;
         }
         currentSoilLayer->SubtractTranspiration(subtractTransp);
         currentSoilLayer = currentSoilLayer->Next();
         SoilLayerIndex++;
      }
   }

   for (int i=0;i<AttackerNumber;i++)
   {
   	rootList->ElementAtNumber(i)->actualTranspiration = TotalPlantUptake[i];
      if (TotalPlantUptake[i]<0)
      	theMessage->WarningWithDisplay("soilProfile::SubtractTranspiration negative ",TotalPlantUptake[i]);
      if (TotalPlantUptake[i]-rootList->ElementAtNumber(i)->transpirationDemand>1e-5)
      {
         cout << theTime << endl;
         cout << "Layer: " << i << "  TotalPlantUptake: " << TotalPlantUptake[i] << "  RootList_transpirationDemand: " << rootList->ElementAtNumber(i)->transpirationDemand;
      	theMessage->WarningWithDisplay("soilProfile::SubtractTranspiration exceeds demand by ",TotalPlantUptake[i]-rootList->ElementAtNumber(i)->transpirationDemand);
      }
   }
   if (waterBefore>1E-5)
   	EvaporationContent *= soilLayerArray[0]->GetAvailWater()/waterBefore;
}
/****************************************************************************\
Subtracts estimated nitrogen uptake by the plants from the mineral
nitrogen pools in the layer.
\****************************************************************************/
nitrogen* soilProfile::SubtractNitrogenUptake(linkList<rootStructure>* rootList)
{
   double PlantDemand[MaxPlants];
   double NitrogenUptakeAbility[2*MaxSoilLayers*MaxPlants];  
   double NitrogenResource[2*MaxSoilLayers];
   double PlantUptake[MaxPlants*2*MaxSoilLayers];
   nitrogen TotalPlantUptake[MaxPlants];

   int AttackerNumber = rootList->NumOfNodes();
   for (int i=0;i<AttackerNumber;i++)
   	PlantDemand[i] = rootList->ElementAtNumber(i)->NitrogenDemand;
	for (int j=0; j<AttackerNumber;j++)
   	TotalPlantUptake[j].Clear();

   rootStructure * TotalRoot = new rootStructure;
   EstimateTotalRoot(TotalRoot,rootList);

	int ResourceNumber = 0;
   soilLayer * currentSoilLayer = first;
   while (currentSoilLayer != NULL)
   {
   	currentSoilLayer = currentSoilLayer->Next();
      ResourceNumber++;
   }
   ResourceNumber = ResourceNumber*2;                                 // 2 as both nitrate and ammonium are substrates

   linkList <rootStructure>::PS currentRoot;
   rootList->PeekHead(currentRoot);
   int PlantIndex = 0;
   while (currentRoot != NULL)
   {
   	int SoilLayerIndex = 0;
      soilLayer * currentSoilLayer = first;
      while (currentSoilLayer != NULL)
      {
      	NitrogenUptakeAbility[2*SoilLayerIndex+PlantIndex*ResourceNumber]   = currentSoilLayer->MaxNitrogenFlux(false,currentRoot->element);
			NitrogenUptakeAbility[2*SoilLayerIndex+PlantIndex*ResourceNumber+1] = currentSoilLayer->MaxNitrogenFlux(true ,currentRoot->element);
         currentSoilLayer = currentSoilLayer->Next();
         SoilLayerIndex++;
      }
      rootList->OneStep(currentRoot);
      PlantIndex++;
   }

   int SoilLayerIndex = 0;
   currentSoilLayer = first;
   while (currentSoilLayer != NULL)
   {
      double TotalAmmoniumUptake = currentSoilLayer->MaxNitrogenFlux(false,TotalRoot);
      double LayerAmmonium = currentSoilLayer->GetAmmonium().n*currentSoilLayer->FracAmmoniumInWater();
   	NitrogenResource[2*SoilLayerIndex]   = min(LayerAmmonium,TotalAmmoniumUptake);

      double TotalNitrateUptake = currentSoilLayer->MaxNitrogenFlux(true,TotalRoot);
      double LayerNitrate = currentSoilLayer->GetNitrate().n;
   	NitrogenResource[2*SoilLayerIndex+1] = min(LayerNitrate,TotalNitrateUptake);

      currentSoilLayer = currentSoilLayer->Next();
      SoilLayerIndex++;
   }
   ResourceCompetition(ResourceNumber, AttackerNumber, PlantDemand, &NitrogenUptakeAbility[0], &NitrogenResource[0], &PlantUptake[0]);
   //delete [sizeof(double)*MaxSoilLayers] TotalRoot->rootLengthList; // BMP added June 2002
   delete [] TotalRoot->rootLengthList; // BMP added June 2002
   delete TotalRoot;

   if (rootList!=NULL) // Perform only if plants has roots
 	{
 		soilLayer * currentSoilLayer = first;
   	int SoilLayerIndex = 0;
   	while (currentSoilLayer != NULL)
   	{
   		for (int j=0;j<AttackerNumber;j++)
         {
            nitrogen NH4LayerUptake = currentSoilLayer->SubtractAmmoniumUptake(PlantUptake[2*SoilLayerIndex+j*ResourceNumber]);
            nitrogen NO3LayerUptake = currentSoilLayer->SubtractNitrateUptake(PlantUptake[2*SoilLayerIndex+j*ResourceNumber+1]);
            TotalPlantUptake[j] = TotalPlantUptake[j] + NH4LayerUptake + NO3LayerUptake;
  			}
         currentSoilLayer = currentSoilLayer->Next();
         SoilLayerIndex++;
   	}
 	}
   for (int i=0;i<AttackerNumber;i++)
   	nitrogenUptake[i] = TotalPlantUptake[i];
   return nitrogenUptake;
}

/****************************************************************************\
\****************************************************************************/
void soilProfile::ResourceCompetition(int ResourceNumber, int AttackerNumber, double* Demand, double* UptakeAbility, double* Resource, double* ResourceRemoved)
{
   double PossibleUptake[MaxPlants];
   double MaxResourceUptake[2*MaxSoilLayers];
   for (int j=0; j<AttackerNumber; j++) PossibleUptake[j] = 0.0;
   for (int i=0; i<ResourceNumber; i++) MaxResourceUptake[i] = 0.0;
   for (int j=0; j<AttackerNumber; j++)
   	for (int i=0; i<ResourceNumber; i++)
      	ResourceRemoved[i+j*ResourceNumber] = 0.0;
   bool DemandFullfilled = false;

   while (!DemandFullfilled)
   {

      DemandFullfilled = true;
   	for (int j=0; j<AttackerNumber; j++)
	   	for (int i=0; i<ResourceNumber; i++)
	      	MaxResourceUptake[i] += UptakeAbility[i+j*ResourceNumber];

   	for (int j=0; j<AttackerNumber; j++)
      {
	   	for (int i=0; i<ResourceNumber; i++)
            if (MaxResourceUptake[i]>0)
            	PossibleUptake[j] += UptakeAbility[i+j*ResourceNumber]*min(1.0,Resource[i]/MaxResourceUptake[i]);
         DemandFullfilled = DemandFullfilled & (PossibleUptake[j]>Demand[j]);
      }
      if (DemandFullfilled)
      {
      	for (int j = 0; j<AttackerNumber; j++)
	   		for (int i=0; i<ResourceNumber; i++)
            	if ((PossibleUptake[j]>0) && (MaxResourceUptake[i]>0) && (Demand[j]>0))
	         		ResourceRemoved[i+j*ResourceNumber] = Demand[j]/PossibleUptake[j]*UptakeAbility[i+j*ResourceNumber]*min(1.0,Resource[i]/MaxResourceUptake[i]);
      }
      else
      {
      	bool singleDemand = false;
      	for (int j = 0; j<AttackerNumber; j++)
         	if ((PossibleUptake[j]>Demand[j]) && (Demand[j]>0))                   // single plant demand can be fullfilled !!
            {
            	for (int i=0; i<ResourceNumber; i++)
               {
            		if ((PossibleUptake[j]>0) && (MaxResourceUptake[i]>0))
         				ResourceRemoved[i+j*ResourceNumber] = Demand[j]/PossibleUptake[j]*UptakeAbility[i+j*ResourceNumber]*min(1.0,Resource[i]/MaxResourceUptake[i]);
                  UptakeAbility[i+j*ResourceNumber] = 0.0;
               }
               singleDemand = true;
               Demand[j] = 0.0;
            }
         if (!singleDemand)                                    // all plants uptake are below demand !!
         {
         	for (int j=0; j<AttackerNumber; j++)
	   			for (int i=0; i<ResourceNumber; i++)
               	if ((PossibleUptake[j]>0) && (MaxResourceUptake[i]>0) && (Demand[j]>0))
                  	ResourceRemoved[i+j*ResourceNumber] = UptakeAbility[i+j*ResourceNumber]*min(1.0,Resource[i]/MaxResourceUptake[i]);
            DemandFullfilled = true;
         }
      }
   }
}

/****************************************************************************\
Calculates soil temperature using a simple algorithm.
Returned values for periods without frost are good estimates, returned
values for periods with frost are fair estimates as frost and snow influence
are not accounted for in a fully satisfactory fashion using this simple method.
Konstants are presently hand-fitted (8.3.1999).
   soilTemperature    -
   airTemperature     - mean air temperature of the current day (Celsius)
   depth              - soil depth in mm
   meanAirTemperature - yearly mean air temperature for the site (Celsius)
\****************************************************************************/
void soilProfile::UpdateT(double * soilTemperature,double airTemperature,double depth,double meanAirTemperature)
{
   const double k1      =  30000.0;
   const double k2      = -0.00033;
   const double k3      =  0.006;
   const double k4      =  0.12;

   if (depth>0)
   {
      double a1=k1/(k1+(depth*depth));
      if (*soilTemperature<k3*depth)
         a1=a1*k4;
      double dampenFactor=exp(k2*depth);
      double Tdampened=((airTemperature-meanAirTemperature)*dampenFactor)+meanAirTemperature; // Dampen amplitude. Tdampened is target value for this depth
      *soilTemperature+=a1*(Tdampened-*soilTemperature); // Pursue target value
   }
   else
      theMessage->FatalError("soil::UpdateT - depth must be > 0.0");
}

/******************************************************
Updates temperature from "master" soil
******************************************************/
void soilProfile::UpdateTemperature(const base* aOwner)
{
	soilLayer* currentLayer = first;
   soil* aSoil = (soil*) aOwner;
   double depth;
   while (currentLayer!=NULL)
   {
   	depth = currentLayer->GetStartDepth()+0.5*currentLayer->GetThickness();
      currentLayer->PutTemperature(aSoil->GetTemperature(depth));
      currentLayer = currentLayer->Next();
   }
}

/*******************************************************
Alternative 2 (JB): UpdateTemperature calculates the soil temperature according
to an explicit differential method (se description)
The upperboundary condition consist terms with
both airtemperature, radiation and snow cover.
fractionEvaporation is used to correct radiation for
reflection by leaves.
Alteratively a very simple method can be utilised.
*******************************************************/
void soilProfile::UpdateTemperature(double AirTemperature, double snow)
{
   CheckCoherence(-10);
   bool showFlag=true;  // TEST REMOVE !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   timeConsumption->startProfiling(temperatureSimulation);
   if (theControlParameters->GetSimpleTemperatureModel() || theControlParameters->GetMeasuredSoilTemperature())
   {
   	if (theControlParameters->GetSimpleTemperatureModel())
      {
      	double soilTemperature;
	      double depth;
   	   for (int i=1;i<NumOfSoilLayers;i++)
      	{
         	soilTemperature=soilLayerArray[i]->GetTemperature();
	         if ((soilTemperature<-50) || (soilTemperature>50))
	            Terminate("soilProfile::UpdateTemperature - soil temperature not within realistic range");
	         depth = soilLayerArray[i]->GetStartDepth()+ soilLayerArray[i]->GetThickness()*0.5;
	         UpdateT(&soilTemperature,AirTemperature,depth,8.0);
	      }
	      soilLayerArray[0]->PutTemperature(AirTemperature);
      }
      else
      {
      	for (int i=0;i<NumOfSoilLayers;i++)
         	soilLayerArray[i]->PutTemperature(theClimate->GetNextSoilTemperature());
      }
   }
   else

   {
      int i;
      soilLayer * currentLayer;
      double iceIncrease = 0;
      double deltaTime =  0;
      double currentTime = 0;
      double deltaTemp[MaxSoilLayers];
      double waterFlux[MaxSoilLayers], waterFluxSlope[MaxSoilLayers], temperatureIncrease[MaxSoilLayers];
      bool LowTemperature = (AirTemperature<2.0);
      int numberOfTotalLayers = NumOfSoilLayers + ExtraLayers;

      for (i=0;i<numberOfTotalLayers;i++)
         waterFlux[i] = WaterDensity*soilLayerArray[i]->GetWaterFlux()/86400/1000;            // unit kg m-2 s-1

      for (i=0;i<numberOfTotalLayers;i++)      // calculates the slope of the waterfluxes
         if (i==0)
            waterFluxSlope[0] =  (waterFlux[1]-waterFlux[0])*DistNextLayerInv[0];
         else
            if (i==(numberOfTotalLayers-1))
               waterFluxSlope[i] = (waterFlux[i]-waterFlux[i-1])*DistPreviousLayerInv[i];
            else
               waterFluxSlope[i] =  ((waterFlux[i+1]-waterFlux[i])*DistNextLayerInv[i]+
                                     (waterFlux[i]-waterFlux[i-1])*DistPreviousLayerInv[i])*0.5;

      // JB moved calculation of heatcondictivity and heat capacity outside loop and thereby assumed constant within same day!
   	for (i=0;i<numberOfTotalLayers;i++)
      {
   		heatConductivity[i] = soilLayerArray[i]->HeatConductivity();
   		heatCapacity[i] = soilLayerArray[i]->HeatCapacity();
         SpecWaterCapacity[i] = soilLayerArray[i]->SpecWaterCapacity();
         LowTemperature = LowTemperature || (temperature[i]<2.0);
      }

   	for (i=0;i<numberOfTotalLayers;i++)      // calculates the slope of the heatconductivity
   		if (i==0)
            heatConductivitySlope[0] =  (0.025+(heatConductivity[1]-heatConductivity[0])*DistNextLayerInv[0])*0.5;    // mean between soil and air
         else
            if (i==(numberOfTotalLayers-1))
               heatConductivitySlope[i] =  (heatConductivity[i]-heatConductivity[i-1])*DistPreviousLayerInv[i];
            else
               heatConductivitySlope[i] =  ((heatConductivity[i+1]-heatConductivity[i])*DistNextLayerInv[i]+
                                            (heatConductivity[i]-heatConductivity[i-1])*DistPreviousLayerInv[i])*0.5;

      double *stepSize = new double;
      while (currentTime<86400)                    // simulates 24 hours
      {
			// 4th order Runge Kutta method

         if (LowTemperature)
	         *stepSize = 288;                //MEL 2007: this resulted in large numerical errors in the simulation of some combinations of climate and soil
         else
         	*stepSize = 1728;

         double DeltaTemp1[MaxSoilLayers], DeltaTemp2[MaxSoilLayers], DeltaTemp3[MaxSoilLayers], DeltaTemp4[MaxSoilLayers];
         double Temperature1[MaxSoilLayers];
         CalculateDeltaTemp(temperature,waterFluxSlope,waterFlux,DeltaTemp1);
         for (int i=0;i<numberOfTotalLayers;i++)
   			Temperature1[i] = temperature[i]+*stepSize*DeltaTemp1[i]/2.0;

         CalculateDeltaTemp(Temperature1,waterFluxSlope,waterFlux,DeltaTemp2);
         for (int i=0;i<numberOfTotalLayers;i++)
   			Temperature1[i] = temperature[i]+*stepSize*DeltaTemp2[i]/2.0;

         CalculateDeltaTemp(Temperature1,waterFluxSlope,waterFlux,DeltaTemp3);
         for (int i=0;i<numberOfTotalLayers;i++)
   			Temperature1[i] = temperature[i]+*stepSize*DeltaTemp3[i];

         CalculateDeltaTemp(Temperature1,waterFluxSlope,waterFlux,DeltaTemp4);
         for (int i=0;i<numberOfTotalLayers;i++)
         	deltaTemp[i] = DeltaTemp1[i]/6.0+DeltaTemp2[i]/3.0+DeltaTemp3[i]/3.0+DeltaTemp4[i]/6.0;

			currentTime += *stepSize;
         for (int i=0;i<numberOfTotalLayers;i++)
   			temperature[i]+= *stepSize*deltaTemp[i];

         for (int i=0;i<numberOfTotalLayers;i++)
   		{
      		currentLayer = soilLayerArray[i];
      		if (temperature[i]<0.0)                                    // frost
      		{
         		if (currentLayer->GetFrozenPart()<1.0)                  // all frozen?
         		{
            		double iceIncrease = WaterDensity/IceDensity*(-1.0/WaterDensity*waterFluxSlope[i]-
                          (specCapacity*SpecWaterCapacity[i]/273.0*deltaTemp[i]));
                  double iceContent=currentLayer->GetIce()+*stepSize*iceIncrease*GetLayerThickness(i);
/*                  if (iceContent<0 && showFlag)
                  {
                     cout << iceContent;
                     cout << endl;
                     showFlag=false;
                  }    */
            		currentLayer->SetIceContent(min(0.0,max(iceContent,currentLayer->GetTotalWater())));
         		}
      		}
      		else
         		currentLayer->SetIceContent(0.0);
   		}
         if (snow>0)
            temperature[0] = (AirTemperature*snowConductivity/(snow/1000.0)+temperature[1]*heatConductivity[0]*DistNextLayerInv[0])
                             /(snowConductivity/(snow/1000.0)+heatConductivity[0]*DistNextLayerInv[0]);
         else
            temperature[0] = AirTemperature;

         temperature[numberOfTotalLayers-1] = theClimate->YearlyAverage;          // lower boundary condition
      }
      //check if the temperature is witin realistic boundaries
      int month=theTime.GetMonth()-1;
      int year=theTime.GetYear();
      for (int i=0;i<numberOfTotalLayers;i++)
      {
         if (i>9 && i<19 && temperature[i]<minTemp[month] && year!=lastErrorYear)
         {
            lastErrorYear=year;
            cout << "Layer no " << i << " temperature is " << temperature[i];
            theMessage->WarningWithDisplay("soilProfile::UpdateTemperature - soil layer temperature below expert opinion monthly threshold");
         }
         if (temperature[i]>100)
            theMessage->FatalError("soilProfile::UpdateTemperature - soil layer temperature above boiling point");
         if (temperature[i]<-50)
            theMessage->FatalError("soilProfile::UpdateTemperature - soil layer temperature below possible value");
         soilLayerArray[i]->PutTemperature(temperature[i]);
      }
      if (theControlParameters->GetWriteSoilTemperature())
      {
      	SoilTempFile << theTime;
         for (int i=0;i<numberOfTotalLayers;i++)
      	   SoilTempFile << "\t" << temperature[i];
         SoilTempFile << endl;
      }
   }
   timeConsumption->endProfiling(temperatureSimulation);
   CheckCoherence(-20);
}

/****************************************************************************\
Calculates deltaT's used in the Runge Kutta solution.
\****************************************************************************/
void soilProfile::CalculateDeltaTemp(double* temperature,double *waterFluxSlope,
				double *waterFlux, double* deltaTemp)
{
	double temperatureSlope[MaxSoilLayers];
   int numberOfTotalLayers = NumOfSoilLayers + ExtraLayers;
   soilLayer* currentLayer;

	for (int i=0; i<numberOfTotalLayers;i++)      // calculates the temperature difference coefficient
      if (i==0)
         temperatureSlope[0] = (temperature[1]-temperature[0])*DistNextLayerInv[0];
      else
         if (i==(numberOfTotalLayers-1))
            temperatureSlope[i] = 0.0; //  boundary condition
         else
            temperatureSlope[i] = ((temperature[i+1]-temperature[i])*DistNextLayerInv[i]+
                                   (temperature[i]-temperature[i-1])*DistPreviousLayerInv[i])*0.5;

   double coefficientA, coefficientB;
   double temperatureSlopeDeriv;

   for (int i=0;i<numberOfTotalLayers;i++)
   {
      currentLayer = soilLayerArray[i];
      coefficientA = heatCapacity[i];
      coefficientB = 0.0;
      if (((temperature[i]<0.0) && (currentLayer->GetFrozenPart()<1.0)) ||
         ((temperature[i]>0.0) && (currentLayer->GetFrozenPart()>1e-10)))      // is there frost
      {
         coefficientA += specCapacity*specCapacity*SpecWaterCapacity[i]/273.0;
         coefficientB = specCapacity*waterFluxSlope[i];
      }
      if (i==0)
          temperatureSlopeDeriv =  (temperature[1]-temperature[0])*DistNextLayerInv[0]*DistNextLayerInv[0];
      else
         if (i==(numberOfTotalLayers-1))
            temperatureSlopeDeriv =  -(temperature[i]-temperature[i-1])*DistPreviousLayerInv[i]*DistPreviousLayerInv[i];   // correct ??
         else
            temperatureSlopeDeriv =  ((temperature[i+1]-temperature[i])*DistNextLayerInv[i]-
                                     (temperature[i]-temperature[i-1])*DistPreviousLayerInv[i])/
                                     (distanceToNextLayer[i]+distanceToPreviousLayer[i])*2;

      deltaTemp[i] = (heatConductivity[i]*temperatureSlopeDeriv-coefficientB
                                + temperatureSlope[i]*(heatConductivitySlope[i]-HeatCapacityWater*waterFlux[i]))
                                /coefficientA;
   }
}

/*******************************************************
Alternative 3 (MEL 2007): UpdateFinDiffTemperature calculates
the soil temperature according the finite difference sceme
The upper boundary condition consist terms with
both airtemperature, radiation and snow cover.
fractionEvaporation is used to correct radiation for
reflection by leaves.
*******************************************************/
void soilProfile::UpdateFinDiffTemperature(double AirTemperature, double snow)
{
	CheckCoherence(-100);
   timeConsumption->startProfiling(temperatureSimulation);
   if (theControlParameters->GetSimpleTemperatureModel() || theControlParameters->GetMeasuredSoilTemperature())
   {
   	if (theControlParameters->GetSimpleTemperatureModel())
      {
      	double soilTemperature;
	      double depth;
   	   for (int i=1;i<NumOfSoilLayers;i++)
      	{
         	soilTemperature=soilLayerArray[i]->GetTemperature();
	         if ((soilTemperature<-50) || (soilTemperature>50))
	            Terminate("soilProfile::UpdateTemperature - soil temperature not within realistic range");
	         depth = soilLayerArray[i]->GetStartDepth()+ soilLayerArray[i]->GetThickness()*0.5;
	         UpdateT(&soilTemperature,AirTemperature,depth,8.0);
	      }
	      soilLayerArray[0]->PutTemperature(AirTemperature);
      }
      else
      {
      	for (int i=0;i<NumOfSoilLayers;i++)
         	soilLayerArray[i]->PutTemperature(theClimate->GetNextSoilTemperature());
      }
   }
   else

   {
      int i;
      soilLayer * currentLayer;
      double TimeDerivIce = 0.0;
      double deltaTime =  0.0;
      double currentTime = 0.0;
		double moreFrozen[MaxSoilLayers];
      double waterFlux[MaxSoilLayers], waterFluxMean[MaxSoilLayers], waterFluxSlope[MaxSoilLayers];
   	double heatConductivity[MaxSoilLayers], heatConductivityMean[MaxSoilLayers], heatConductivitySlope[MaxSoilLayers], heatCapacity[MaxSoilLayers], SpecWaterCapacity[MaxSoilLayers];
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
      bool frostChange[MaxSoilLayers];
      double TLB, TUB;
      bool reduceTimestep=false;
      double averageThermalCond;
      double averageHeatCapacity;
      double WaterInFlux=WaterDensity*infiltration/86400/1000;            // unit kg m-2 s-1

      int numberOfTotalLayers = NumOfSoilLayers + ExtraLayers;

   	for (i=0;i<numberOfTotalLayers;i++)
         moreFrozen[i]=0.0;


      for (i=0;i<numberOfTotalLayers;i++)
      {
         waterFlux[i] = WaterDensity*soilLayerArray[i]->GetWaterFlux()/86400/1000;            // unit kg m-2 s-1
        	if (fabs(waterFlux[i])>0.01)
         	theMessage->WarningWithDisplay("soilProfile::UpdateTemperature waterflux is larger than 0.01 kg m-2 s-1");
        	if (waterFlux[i]<0.0)
         	theMessage->WarningWithDisplay("soilProfile::UpdateTemperature waterflux is negative");
      }
      for (i=0;i<numberOfTotalLayers;i++)
      {
      	if (waterFlux[i]> 10)
         	theMessage->WarningWithDisplay("soilProfile::UpdateTemperature - Waterflux unrealistic high");
      }
      waterFluxMean[0]= (WaterInFlux+waterFlux[0])*0.5;
      for (i=1;i<numberOfTotalLayers-1;i++)
      	waterFluxMean[i]= (waterFlux[i-1]+waterFlux[i])*0.5;
      waterFluxMean[numberOfTotalLayers-1]= (waterFlux[numberOfTotalLayers-1]+waterFlux[numberOfTotalLayers-2])*0.5;

      // calculate the slope of the waterfluxes
      waterFluxSlope[0] =  (waterFlux[0]-WaterInFlux)*DistPreviousLayerInv[0];
      for (i=1;i<numberOfTotalLayers-1;i++)
      	waterFluxSlope[i] = (waterFlux[i]-waterFlux[i-1])*DistPreviousLayerInv[i];
      waterFluxSlope[numberOfTotalLayers-1] = (waterFlux[numberOfTotalLayers-1]-waterFlux[numberOfTotalLayers-2])
      														*DistPreviousLayerInv[numberOfTotalLayers-1];

      //get new LB and UB conditions
      //LB from analytical solution (as in DAISY)
      double maxDepth = (GetMaximumDepth()+soilLayerArray[numberOfTotalLayers-2]->GetThickness()+soilLayerArray[numberOfTotalLayers-1]->GetThickness())/1000;           //change to actual depth!!!!!!!!!!!!!!!!!!!!!!!
      if (calculateAverage)
      {
         averageThermalCond=0.0;
         averageHeatCapacity=0.0;
      	for (i=0;i<numberOfTotalLayers;i++)
         {
      		averageThermalCond += soilLayerArray[i]->HeatConductivity()*GetLayerThickness(i)/1000/maxDepth;
      		averageHeatCapacity += soilLayerArray[i]->HeatCapacity()*GetLayerThickness(i)/1000/maxDepth;
      	}
         calculateAverage =false;
      depthDampning = pow(2*averageThermalCond/(averageHeatCapacity*0.0172142/(3600*24)),0.5);                 //approximately 2.4
      }
      double averageAirTemp = 7.7;
      double amplitudeAirTemp = 8.0;
      double dayInYear=theTime.GetDayInYear();
      double dayZero=212.0;   //august 1th the hottest day january 30th the coldest day
      double daysFromZero=dayInYear-dayZero;
      TLB=averageAirTemp + amplitudeAirTemp*exp(-maxDepth/depthDampning)*cos(0.0172142*(daysFromZero)-maxDepth/depthDampning);

      if (snow>0.0)
         {
         	if (AirTemperature>0.0)      //snow contains liquid water
            	TUB = 0.0;
				else                         //no water in snowpack
            	TUB = (AirTemperature*snowConductivity/(snow/1000.0)+
               		temperature[0]*soilLayerArray[0]->HeatConductivity()*DistPreviousLayerInv[0])
                          /(snowConductivity/(snow/1000.0)+soilLayerArray[0]->HeatConductivity()*DistPreviousLayerInv[0]);
         }
      else
         TUB = AirTemperature;
      // Initialise starting condition
   	for(i=0;i<numberOfTotalLayers;++i)
      {
         TNew[i]=temperature[i];
         if ((temperature[i]<0.0) && (reduceTimestep==false))
         	reduceTimestep=true;
         if ((soilLayerArray[i]-> GetIce()>0.0) && (reduceTimestep==false))
            reduceTimestep=true;
      }
      if ((AirTemperature < 0.0) &&  (reduceTimestep==false))
      	reduceTimestep=true;

      deltaTime=3600;         //timestep  1 hour
      if (reduceTimestep)
      deltaTime=100;           //timestep 5 min

      bool firstTime=true;
      // simulate 24 hours
      while (currentTime<86400)
      {
  			//transfer of new temperature to old temperature
			for(i=0;i<numberOfTotalLayers;++i)
         {
				TOld[i]=TNew[i];
            frostChange[i]=fabs(moreFrozen[i])>1E-30; // True if change in icecontent
         }


            //calculate heatconductivity etc.
         for (i=0;i<numberOfTotalLayers;i++)
            if (frostChange[i] || firstTime)
            {
               heatConductivity[i] = soilLayerArray[i]->HeatConductivity();
               heatCapacity[i] = soilLayerArray[i]->HeatCapacity();
               SpecWaterCapacity[i] = soilLayerArray[i]->SpecWaterCapacity();
            }

         for (i=0;i<numberOfTotalLayers;i++)      // calculates the slope of the heatconductivity
            if (frostChange[i] || firstTime)
               if (i==0)
               {
                  heatConductivitySlope[i] = (0.025+(heatConductivity[i+1]-heatConductivity[i])*DistNextLayerInv[i])*0.5;    // mean between soil and air
                  heatConductivityMean[i]=  (heatConductivity[i+1]+heatConductivity[i])*0.5;
               }
               else
                  if (i==(numberOfTotalLayers-1))
                  {
                     heatConductivitySlope[i] =  (heatConductivity[i]-heatConductivity[i-1])*DistPreviousLayerInv[i];
                		heatConductivityMean[i]=  heatConductivity[i];
                  }
                  else
                  {
                     heatConductivitySlope[i] =  ((heatConductivity[i+1]-heatConductivity[i])*DistNextLayerInv[i]+
                                                  (heatConductivity[i]-heatConductivity[i-1])*DistPreviousLayerInv[i])*0.5;
                     heatConductivityMean[i]=  (heatConductivity[i+1]+heatConductivity[i])*0.5;;
                  }

         for (i=0;i<numberOfTotalLayers;i++)
            if (frostChange[i] || firstTime)
            {
               currentLayer = soilLayerArray[i];
            if (((TNew[i] < 0.0) && (currentLayer->GetIce()<currentLayer->GetTotalWater())) || //freezing
               ((TNew[i] > 0.0) && (currentLayer->GetIce()>0.0)))    //thawing
               {
                  A[i]=specCapacity*specCapacity/273.0*SpecWaterCapacity[i]+currentLayer->HeatCapacity();
                  B[i]=specCapacity*waterFluxSlope[i];
               }
               else
               {
                  A[i]=currentLayer->HeatCapacity();
                  B[i]=0;
               }
               Ka[i]=A[i]/deltaTime;
               Kb[i]=heatConductivityMean[i]/(distanceToPreviousLayer[i]+distanceToNextLayer[i]);
               Kc[i]=(heatConductivitySlope[i]-HeatCapacityWater*waterFluxMean[i])/
                  (2*(distanceToPreviousLayer[i]+distanceToNextLayer[i]));
         	}

         //Initialization of diagonal vectors

         for(i=0; i<numberOfTotalLayers; ++i)
            if (frostChange[i] || firstTime)
            {
               UpperVector[i]=-Kb[i]*DistNextLayerInv[i]-Kc[i];          					//c
               MidVector[i]=Ka[i]+Kb[i]*(DistNextLayerInv[i]+DistPreviousLayerInv[i]); //b
               LowerVector[i]= -Kb[i]*DistPreviousLayerInv[i]+Kc[i];                   //a
               LowerVector[0]= 0.0;
               UpperVector[numberOfTotalLayers-1]=0.0;
               if (MidVector[0]==0.0)
               	theMessage->WarningWithDisplay("soilProfile::UpdateTemperature - Error 1 in tridag vectors");
            }


   		//Initialization of vector with old values

			i=0;
         TOldArray[i]=Ka[i]*TOld[i]
            +Kb[i]*((TOld[i+1]-TOld[i])*DistNextLayerInv[i]-(TOld[i]-TUB)*DistPreviousLayerInv[i])
            +Kc[i]*(TOld[i+1]-TUB)-B[i]-(-Kb[i]*DistPreviousLayerInv[i]+Kc[i])*TUB;

      	for (i=1;i<numberOfTotalLayers-1;++i)
         	TOldArray[i]=Ka[i]*TOld[i]
            	+Kb[i]*((TOld[i+1]-TOld[i])*DistNextLayerInv[i]-(TOld[i]-TOld[i-1])*DistPreviousLayerInv[i])
               +Kc[i]*(TOld[i+1]-TOld[i-1])-B[i];

         TOldArray[numberOfTotalLayers-1]=Ka[numberOfTotalLayers-1]*TOld[numberOfTotalLayers-1]
         											+Kb[numberOfTotalLayers-1]*((TLB-TOld[numberOfTotalLayers-1])*DistNextLayerInv[numberOfTotalLayers-1]-(TOld[numberOfTotalLayers-1]-TOld[numberOfTotalLayers-2])*DistPreviousLayerInv[numberOfTotalLayers-1])
                                          +Kc[numberOfTotalLayers-1]*(TLB-TOld[numberOfTotalLayers-2])-B[numberOfTotalLayers-1]+(Kb[numberOfTotalLayers-1]*DistNextLayerInv[numberOfTotalLayers-1]+Kc[numberOfTotalLayers-1])*TLB;

         //Solve with the double sweep method (function tridag)
         Tridag(LowerVector, MidVector, UpperVector, TOldArray, TNew, numberOfTotalLayers);

/*         int day=theTime.GetDay();
         int month=theTime.GetMonth();
         int year=theTime.GetYear();
         if ((day==20) && (month==4) && (year==1984))
				cout << "";
*/
         //update IceContent
			for (int i=0;i<numberOfTotalLayers;i++)
   		{
            currentLayer = soilLayerArray[i];
               // is there freezing or thawing
            if (((TNew[i] < 0.0) && (currentLayer->GetIce()<currentLayer->GetTotalWater())) || //freezing
                  ((TNew[i] > 0.0) && (currentLayer->GetIce()>0.0)))    //thawing
            {
            	double TimeDerivTemp=(TNew[i]-TOld[i])/deltaTime;
               double TimeDerivIce = WaterDensity/IceDensity*(-1.0/WaterDensity*waterFluxSlope[i]-
                       (specCapacity*SpecWaterCapacity[i]/273.0*TimeDerivTemp));
               moreFrozen[i] =  deltaTime*TimeDerivIce*currentLayer->GetThickness();     //in mm
               if (moreFrozen[i]+ currentLayer->GetIce() > currentLayer->GetTotalWater())      //frozen water exceeds water
               {
               	TNew[i]=-0.001;
                  moreFrozen[i]=currentLayer->GetTotalWater()-currentLayer->GetIce();
               }
               else if (moreFrozen[i]+ currentLayer->GetIce()<0.0)                             //frozen water negative
               {
               	TNew[i]=0.001;
                  moreFrozen[i]= -currentLayer->GetIce();
               }
               double ice=currentLayer->GetIce();
               currentLayer->SetIceContent(max(0.0,min(currentLayer->GetTotalWater(),ice+ moreFrozen[i])));
					CheckCoherence(-90);
            }
            else
            	moreFrozen[i]=0.0;
            if (TNew[i]>50)
               theMessage->WarningWithDisplay("soilProfile::UpdateTemperature - soil layer temperature above realistic temperature");
            if (TNew[i]<-50)
            	theMessage->WarningWithDisplay("soilProfile::UpdateTemperature - soil layer temperature below realistic value");
         }
         currentTime += deltaTime;
         firstTime=false;
   	}

      for (int i=0;i<numberOfTotalLayers;i++)
         temperature[i]=TNew[i];

      //check if the temperature is witin realistic boundaries
      int month=theTime.GetMonth();
      int year=theTime.GetYear();
      for (int i=0;i<numberOfTotalLayers;i++)
      {
         if (i>9 && i<19 && temperature[i]<minTemp[month-1] && year!=lastErrorYear)
         {
            lastErrorYear=year;
            cout << "Layer no " << i << " temperature is " << temperature[i];
            theMessage->WarningWithDisplay("soilProfile::UpdateTemperature - soil layer temperature below expert opinion monthly threshold");
         }
         if (temperature[i]>100)
            theMessage->FatalError("soilProfile::UpdateTemperature - soil layer temperature above boiling point");
         if (temperature[i]>50)
            theMessage->WarningWithDisplay("soilProfile::UpdateTemperature - soil layer temperature above realistic temperature");
         if (temperature[i]<-50)
            theMessage->FatalError("soilProfile::UpdateTemperature - soil layer temperature below possible value");
         soilLayerArray[i]->PutTemperature(temperature[i]);
      }
      if (theControlParameters->GetWriteSoilTemperature())
      {
      	SoilTempFile << theTime;
         for (int i=0;i<numberOfTotalLayers;i++)
      	   SoilTempFile << "\t" << temperature[i];
         SoilTempFile << endl;
      }
   }
   timeConsumption->endProfiling(temperatureSimulation);
}

/****************************************************************************\
 Terminates the execution with an error message
\****************************************************************************/
void soilProfile::Terminate(char * c)
{
   if (strlen(c)>0)
      cout << endl << c << endl;
   cout << endl << "Program terminated. Press any key." << endl;
#ifdef __ANSICPP__
	char ch=getchar();
#else
   char ch=getch();
#endif
   exit(99);
}

/****************************************************************************\
Solves for a vector u[0..n-1] the tridiagonal linear set. The input vectors
a,b,c and r are unmodified, and u contains the result
Slightly modified after Numerical Recipies in C (Press, 1992), main modification
is shift from [1..n] to [0..n-1], in concordance with mainstream C++ conventions.
\****************************************************************************/
void soilProfile::Tridag(double a[], double b[], double c[], double r[], double u[], unsigned long n)
{
   unsigned long j;
   double bet, *gam;
   gam=new double [n]; //
   if (0==b[0])
      Terminate("Error 1 in function Tridag");
   bet=b[0];
   u[0]=r[0]/bet;
   for (j=1;j<n;j++)
   {  // Decomposition and forward substitution
      gam[j]=c[j-1]/bet;      // e[j]
      bet=b[j]-a[j]*gam[j];   // g[j]
      if (0==bet)
         Terminate("Error 2 in function Tridag");
      u[j]=(r[j]-a[j]*u[j-1])/bet;
   }
   for (j=n-2;j+1>0;j--)
      u[j]-=gam[j+1]*u[j+1]; // Backsubstitution
   delete[] gam;
}

/****************************************************************************\
Updates organic matter pools in profile.
\****************************************************************************/
double soilProfile::UpdateOrganicMatter()
{
   double CO2Evolution = 0.0;
   soilLayer * current = first;
   while (current != NULL)
   {
      CO2Evolution += current->UpdateOrganicMatter();
      current = current->Next();
   }
   return CO2Evolution;
}

/****************************************************************************\
Performs nitrification and updates contents of ammonia and nitrate.
   returns       - Nitrification [g N/m�/d]
\****************************************************************************/
nitrogen soilProfile::UpdateNitrification()
{
   nitrogen sum;
   soilLayer * current = first;
   while (current!=NULL)
   {
   	sum = sum + current->UpdateNitrification();
   	current = current->Next();
   }
   return sum;
}

/****************************************************************************\
Performs denitrification and updates contents of nitrate.
//  returns       - Nitrification [g N/m�/d]
\****************************************************************************/
nitrogen soilProfile::UpdateDenitrification(double TotalCO2Emission)
{
   nitrogen sum;
   soilLayer * current = first;
   while (current!=NULL)
   {
      current->UpdateN2OFromDenitrification(TotalCO2Emission);
      sum = sum + current->UpdateN2Production();
      current = current->Next();
   }
   return sum;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetAmmonium(double startDep,double thick)
{
   nitrogen Content;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content = Content + current->GetAmmonium(startDep,thick);
      current = current->Next();
   }
   return Content;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetNitrate(double startDep,double thick)
{
   nitrogen Content;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content = Content + current->GetNitrate(startDep,thick);
      current = current->Next();
   }
   if (Content.n<-1e6)
    	theMessage->FatalError("soilProfile::GetNitrate - returns negative value");
   return Content;
}
/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetN2OFromNitrification()
{
   nitrogen Content;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content = Content + current->GetN2OFromNitrification();
      current = current->Next();
   }
   return Content;
}
/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetN2OFromDenitrification()
{
   nitrogen Content;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content = Content + current->GetN2OFromDenitrification();
      current = current->Next();
   }
   return Content;
}
/****************************************************************************\
\****************************************************************************/
double soilProfile::GetChloride(double startDep,double thick)
{
   double Content = 0.0;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content += current->GetChloride(startDep,thick);
      current = current->Next();
   }
   if (Content<-1e6)
    	theMessage->FatalError("soilProfile::GetChloride - returns negative value");
   return Content;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetAmmonium()
{
   nitrogen Content;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content = Content + current->GetAmmonium();
      current = current->Next();
   }
   if (Content.n<0.0)
   	theMessage->FatalError("soilProfile::GetAmmonium - returns negative value");
   return Content;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetNitrate()
{
   nitrogen Content;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content = Content + current->GetNitrate();
      current = current->Next();
   }
   return Content;
}

/****************************************************************************\
\****************************************************************************/
int soilProfile::GetLayers()
{
   int n = 0;
   soilLayer * current = first;
   while (current != NULL)
   {
      n++;
      current = current->Next();
   }
   return n;
}

/****************************************************************************\
\****************************************************************************/
double soilProfile::GetLayerStartDepth(int No)
{
   int n = 0;
   double depth = 0.;
   soilLayer * current = first;
   while (current != NULL)
   {
      if (n==No) depth = current->GetStartDepth();
      n++;
      current = current->Next();
   }
 	return depth;
}

/****************************************************************************\
\****************************************************************************/
double soilProfile::GetLayerThickness(int No)
{
   int n = 0;
   double thick = 0.;
   soilLayer * current = first;
   while (current != NULL)
   {
      if (n==No) thick = current->GetThickness();
      n++;
      current = current->Next();
   }
   return thick;
}

/****************************************************************************\
\****************************************************************************/
double soilProfile::GetCarbon()
{
   double Content = 0;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content+=current->GetCarbon();
      current = current->Next();
   }
   return Content;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetOrganicNitrogen()
{
   nitrogen Content;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content = Content + current->GetOrganicNitrogen();
      current = current->Next();
   }
   if (Content.n<0.0)
   	theMessage->FatalError("soilProfile::GetOrganicNitrogen - returns negative value");
   return Content;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetOrganicNitrogen(double startDep, double thick)
{
   nitrogen Content;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content = Content + current->GetOrganicNitrogen(startDep,thick);
      current = current->Next();
   }
   if (Content.n<0.0)
   	theMessage->FatalError("soilProfile::GetOrganicNitrogen - returns negative value");
 return Content;
}

/****************************************************************************\
\****************************************************************************/
double soilProfile::GetOrganicCarbon(double startDep, double thick)
{
 double Content = 0;
 soilLayer * current = first;
 while (current != NULL)
 {
  Content+=current->GetOrganicCarbon(startDep,thick);
  current = current->Next();
 }
 return Content;
}

/****************************************************************************\
\****************************************************************************/
double soilProfile::GetSoilMass(double startDep, double thick)
{
 double Content = 0;
 soilLayer * current = first;
 while (current != NULL)
 {
  Content+=current->GetSoilMass(startDep,thick);
  current = current->Next();
 }
 return Content;
}

/****************************************************************************\
\****************************************************************************/
double soilProfile::GetClayContent(double startdepth, double thick)
{
   double Content=0.0;
   double TotalWeight=0.0;
   soilLayer * current=first;
   while (current != NULL)
   {
      double top=current->GetStartDepth();
      double th=current->GetThickness();
      if ((top+th)>startdepth && top<(startdepth+thick)) // Test if within range
      {
      double slice=th;
      if (top<startdepth)
      	slice=max(0.0,top+th-startdepth);
      if ((top+th)>(startdepth+thick))
      	slice=max(0.0,startdepth+thick-top);
      TotalWeight+=slice*current->GetDryBulkDensity();
      Content+=slice*current->GetDryBulkDensity()*current->GetClayContent();
      }
      current = current->Next();
   }
   Content=Content/TotalWeight;
   return Content;
}

/****************************************************************************\
\****************************************************************************/
double soilProfile::GetPoolCarbon(const char * Name)
{
 double Content = 0;
 soilLayer * current = first;
 while (current != NULL)
 {
  Content+=current->GetPoolCarbon(Name);
  current = current->Next();
 }
 return Content;
}

/****************************************************************************\
\****************************************************************************/
double soilProfile::GetPoolCarbon(const char * Name,double startDep,double thick)
{
 double Content = 0;
 soilLayer * current = first;
 while (current != NULL)
 {
  Content+=current->GetPoolCarbon(Name,startDep,thick);
  current = current->Next();
 }
 return Content;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetPoolNitrogen(const char * Name)
{
   nitrogen Content;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content = Content + current->GetPoolNitrogen(Name);
      current = current->Next();
   }
   return Content;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetPoolNitrogen(const char * Name,double startDep,double thick)
{
   nitrogen Content;
   soilLayer * current = first;
   while (current != NULL)
   {
      Content = Content + current->GetPoolNitrogen(Name,startDep,thick);
      current = current->Next();
   }
   return Content;
}
/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetTotalNitrogen()
{
   nitrogen TotalN = GetAmmonium()+GetNitrate()+GetOrganicNitrogen();
	return TotalN;
}
/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetAmmoniumLeaching(double depth)
{
   nitrogen N;
   bool found = false;
   soilLayer * current = first;
   while (!found && current !=NULL)
   {
      if (depth>=current->GetStartDepth() && depth<=current->GetStartDepth()+current->GetThickness())
      {
	      N = current->GetAmmoniumLeaching();
         found = true;
      }
      current = current->Next();
   }
   if (N.n<-1e15)
   	theMessage->WarningWithDisplay("SoilProfile: negative ammonium leaching");
   return N;
}
/****************************************************************************\
\****************************************************************************/
nitrogen soilProfile::GetNitrateLeaching(double depth)
{
   nitrogen N;
   bool found = false;
   soilLayer * current = first;
   while (!found && current !=NULL)
   {
      if (depth>=current->GetStartDepth() && depth<=current->GetStartDepth()+current->GetThickness())
      {
	      N = current->GetNitrateLeaching();
         found = true;
      }
      current = current->Next();
   }
   if (N.n<-1e15)
   	theMessage->WarningWithDisplay("SoilProfile: negative nitrate leaching");
   return N;
}

/****************************************************************************\
\****************************************************************************/
void soilProfile::AddProduct(const organicProduct& product,
									  double * rootLengthList)
{
   if (rootLengthList!=NULL)     // Perform only if a root distribution is defined
   {
      int n = GetLayers();
      double * L = rootLengthList;
      double sumL = 0.;
      for (int i=0; i<n; i++)
         sumL += *(L++);
      soilLayer * current = first;
      organicProduct * prod = (organicProduct *)&product; // To avoid warning
      organicProduct * p;
      L = rootLengthList;
      int k=0; // Remove later !!!
      while (current)
      {
         if (*L>0)
         {
         	p = prod->GetFraction((*L)/sumL);
         	current->AddProduct(*p);
         	delete p;
      	}
      	current = current->Next();
      	L++;
         k++;
      }
   }
   else
   	theMessage->FatalError("SoilProfile::Product not added");
}

/****************************************************************************\
\****************************************************************************/
void soilProfile::StartBudget()
{
	soilLayer * current = first;
	while (current != NULL)
 	{
  		current->StartBudget();
  		current = current->Next();
	}
}

/****************************************************************************\
\****************************************************************************/
void soilProfile::EndBudget()
{
	soilLayer * current = first;
	while (current != NULL)
 	{
  		current->EndBudget();
  		current = current->Next();
	}
}

