/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
Water and nitrogen in soil
\****************************************************************************/
#include "../base/settings.h"
#include "../base/common.h"
#include "soil.h"

#include "../base/commonData.h"
#include "../base/bstime.h"


/****************************************************************************\
\****************************************************************************/
soil::soil(const char * Pname,const int Index,const base * owner)
   : base(Pname,Index,owner)
{
   AccumulatedWaterDrainage   = 0.0;
   AccumulatedNitrateDrainage = 0.0;
   soilEvaporation            = 0.0;
   CO2Evolution               = 0.0;
   nitrateLeached.Clear();
   ammoniumLeached.Clear();
   denitrification.Clear();
   nitrification.Clear();

   evaporationDepth = 200.0;
   totalTranspiration        = 0.;
   NitrogenNetMineralisation.Clear();
   nitrogenUptake=nullptr;

   Nbudget.SetNames("soil","N");
   WaterBudget.SetNames("soil","water");

   Surface              = new soilSurface;
   Profile              = new soilProfile("SoilProfile",0,this);
   MasterSoil = nullptr;
   Potassium = 0;
   Phosphorus = 0;
   ObsDate = 0;
   ObsStartDepth = 0;
   ObsEndDepth = 0;
   ObsNmin = 0;
}

/****************************************************************************\
\****************************************************************************/
soil::soil(const soil& Soil)
 : base(Soil)
{
   if (&Soil)
   {
      evaporationDepth          = Soil.evaporationDepth;
      soilEvaporation           = Soil.soilEvaporation;
      CO2Evolution              = Soil.CO2Evolution;
      nitrateLeached            = Soil.nitrateLeached;
      ammoniumLeached           = Soil.ammoniumLeached;
      denitrification           = Soil.denitrification;
      nitrification             = Soil.nitrification;
      totalTranspiration        = Soil.totalTranspiration;
      NitrogenNetMineralisation = Soil.NitrogenNetMineralisation;
      MasterSoil 	              = Soil.MasterSoil;
      Potassium                 = Soil.Potassium;
      Phosphorus                = Soil.Phosphorus;
      AccumulatedWaterDrainage  = Soil.AccumulatedWaterDrainage;
      AccumulatedNitrateDrainage= Soil.AccumulatedNitrateDrainage;
      nitrogenUptake            = Soil.nitrogenUptake;
      ObsDate                   = Soil.ObsDate;
      ObsStartDepth             = Soil.ObsStartDepth;
      ObsEndDepth               = Soil.ObsEndDepth;
      ObsNmin                   = Soil.ObsNmin;

      Nbudget	  	              = budget(Soil.Nbudget);
      WaterBudget	              = budget(Soil.WaterBudget);
      WaterBudget.Reset(); // History ignored!
      Surface                   = new soilSurface(*Soil.Surface);
      Profile                   = new soilProfile(*Soil.Profile);
      Profile->SetOwner(this);
      Nbudget.Reset(); // History ignored!
   }
}

/****************************************************************************\
\****************************************************************************/
soil::~soil()
{


   delete Surface;
   delete Profile;
   if ( globalSettings->DetailsData.getSoilDetails()==true)
   { 
   	f.close();
   }
}

/****************************************************************************\
\****************************************************************************/
void soil::Initialize(char * soilFileName)
{

	depth = 1000;
   cout << "Initialising soil no " << GetLongNameInNumbers() << " with the file " << soilFileName << endl;  //a problem with patches can occure at this point
   commonData data;
	data.setCritical(false);
	data.readFile(soilFileName);
   data.FindSection("SoilParameters");
   data.FindItem("DepthForOutput",&depth);

   Profile->Initialize(soilFileName);
   if (globalSettings->DetailsData.getSoilDetails()==true)
   {

      string fileName=globalSettings->getOutputDirectory()+"soil"+GetLongNameInNumbers()+".xls";

     	f.open(fileName.c_str(),ios::out);
      f.precision(4);
      f << "date";

      for(unsigned int i=0;i<globalSettings->WaterData.size();i++)
      {
         f << "\twater"<< globalSettings->WaterData[i].getStartDepth() << "-"<<globalSettings->WaterData[i].getEndDepth() ;
      }

      for(unsigned int i=0;i<globalSettings->NminData.size();i++)
      {
         f << "\tNmin"<< globalSettings->NminData[i].getStartDepth() << "-"<< globalSettings->NminData[i].getEndDepth();
         f << "\tNO3-"<< globalSettings->NminData[i].getStartDepth() << "-"<< globalSettings->NminData[i].getEndDepth();
         f << "\tNH4-"<< globalSettings->NminData[i].getStartDepth()  << "-"<< globalSettings->NminData[i].getEndDepth();
         f << "\tTotal15N-"<< globalSettings->NminData[i].getStartDepth() << "-"<< globalSettings->NminData[i].getEndDepth();
      }

	  cout<<globalSettings->SoilOrgCData.size()<<endl;
      for(unsigned int i=0;i<globalSettings->SoilOrgCData.size();i++)
      {

         f << "\tSMB-C-"<< globalSettings->SoilOrgCData[i].getStartDepth() << "-"<<  globalSettings->SoilOrgCData[i].getEndDepth();

      }

      for(unsigned int i=0;i<globalSettings->PoreWaterData.size();i++)
      {

         f << "\t" << globalSettings->PoreWaterData[i].getDepth()/10 <<"cmNit-N"
         << "\t" << globalSettings->PoreWaterData[i].getDepth()/10 <<"cmAm-N"
         << "\t" << globalSettings->PoreWaterData[i].getDepth()/10 <<"cmN"
       	<< "\t" << globalSettings->PoreWaterData[i].getDepth()/10 <<"cmCL"
         << "\t" << globalSettings->PoreWaterData[i].getDepth()/10 <<"cmN15";

      }

      for(unsigned int i=0;i<globalSettings->TemperatureData.size();i++)
      {
    	  f << "\tt" <<globalSettings->TemperatureData[i].getDepth();
      }
      f << "\tNH4\tNO3\tNH4-N15\tNO3-N15\tAOM1\tAOM2\tSMB1\tSMB2\tSMR\tNOM\tIOM\tAOM1-N15\tAOM2-N15\tSMB1-N15\tSMB2-N15\tSMR-N15\tNOM-N15";
      f << "\tNOM-C\tIOM-C\tAOM1-C\tAOM2-C\tSMB1-C\tSMB2-C\tSMR-C\tTOC200\tTOC400\tTOC600\tTON200\tTOC1000\tTot15N-500\tTot15N-1500";
      f << "\tnetMineralisation\tN2O-N\tN2O-D\tN2O\tN2Oratio\tDrainage\tDrainage-N\tSoilEvap\tPlantT";
      f << "\tDenitrification\tNitri\tCO2";
      f << "\tPotC\trN2OD";
      for (int i=0;i<7;i++)
      {
      	f << "\t" << "N2OFromDen-" <<i;
	      f << "\t" << "PotCTurn-" <<i;
         f << "\t" << "WaterEffect-" <<i;
         f << "\t" << "TempEffect-" <<i;
         f << "\t" << "NO3Effect-" <<i;
      }
      f << "\tSoilsurface-C\tSoilSurface-OrgN\tSoilSurface->TotN";
      f << endl;
   }
   

}

/****************************************************************************\
"nitrogenUptake" not included
\****************************************************************************/
void soil::Add(soil* addSoil, double fraction)
{
   evaporationDepth          = (1.0-fraction)*evaporationDepth+fraction*addSoil->evaporationDepth;
   CO2Evolution              = (1.0-fraction)*CO2Evolution+fraction*addSoil->CO2Evolution;

   nitrateLeached            = nitrateLeached*(1.0-fraction)+addSoil->nitrateLeached*fraction;
   ammoniumLeached           = ammoniumLeached*(1.0-fraction)+addSoil->ammoniumLeached*fraction;
   denitrification           = denitrification*(1.0-fraction)+addSoil->denitrification*fraction;
   nitrification             = nitrification*(1.0-fraction)+addSoil->nitrification*fraction;

   NitrogenNetMineralisation = NitrogenNetMineralisation*(1.0-fraction)+addSoil->NitrogenNetMineralisation*fraction;

   Potassium                 = (1.0-fraction)*Potassium+fraction*addSoil->Potassium;
   Phosphorus                = (1.0-fraction)*Phosphorus+fraction*addSoil->Phosphorus;
   totalTranspiration        = (1.0-fraction)*totalTranspiration+fraction*addSoil->totalTranspiration;
   soilEvaporation           = (1.0-fraction)*soilEvaporation+fraction*addSoil->soilEvaporation;
   AccumulatedWaterDrainage  = (1.0-fraction)*AccumulatedWaterDrainage+fraction*addSoil->AccumulatedWaterDrainage;
   AccumulatedNitrateDrainage= (1.0-fraction)*AccumulatedNitrateDrainage+fraction*addSoil->AccumulatedNitrateDrainage;

   Surface->Add(addSoil->GetSoilSurface(),fraction);
   Profile->Add(addSoil->GetSoilProfile(),fraction);
   Nbudget.Add(addSoil->Nbudget,fraction);
   WaterBudget.Add(addSoil->WaterBudget,fraction);
}

/****************************************************************************\
Adds nitrogen to soil
   surfNitrate   - Nitrate to be added to soil surface [g N/mï¿½]
   surfAmmonium  - Ammonium to be added to soil surface [g N/mï¿½]
   soilNitrate   - Nitrate to be added to top of soil profile [g N/mï¿½]
   soilAmmonium  - Ammonium to be added to top of soil profile [g N/mï¿½]
\****************************************************************************/
void soil::AddNutrient(nitrogen surfNitrate,
							  nitrogen surfAmmonium,
							  nitrogen soilNitrate,
							  nitrogen soilAmmonium)
{
   Surface->AddSolidNitrogen(surfNitrate,surfAmmonium);
   Profile->AddNutrient(soilNitrate,soilAmmonium);
   Nbudget.AddInput(surfNitrate.n+surfAmmonium.n+soilNitrate.n+soilAmmonium.n);
}

/****************************************************************************\
\****************************************************************************/
nitrogen soil::GetNitrogenNetMineralisation()
{
   nitrogen Mineralisation = NitrogenNetMineralisation;
   NitrogenNetMineralisation.Clear();
   return Mineralisation;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soil::GetAmmonium()
{
   nitrogen Ammonium = Surface->GetAmmoniumSolid()+Surface->GetAmmoniumDisolved()
                       +Profile->GetAmmonium();
	return Ammonium;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soil::GetNitrate()
{
	return Surface->GetNitrateSolid()+Surface->GetNitrateDisolved()+Profile->GetNitrate();
}

/****************************************************************************\
\****************************************************************************/
nitrogen soil::GetMinNitrogen()
{
	return GetAmmonium()+GetNitrate();
}

/****************************************************************************\
\****************************************************************************/
nitrogen soil::GetMinNitrogen(double startDep, double thick)
{
	return GetAmmonium(startDep,thick)+GetNitrate(startDep,thick);
}
double soil::GetMinNitrogenN(double startDep, double thick)
{
	return GetAmmoniumN(startDep,thick)+GetNitrateN(startDep,thick);
}

double soil::GetMinNitrogenN15(double startDep, double thick)
{
	return GetAmmoniumN15(startDep,thick)+GetNitrateN15(startDep,thick);
}
/****************************************************************************\
\****************************************************************************/
nitrogen soil::GetTotalNitrogen()
{
	return Surface->GetTotalNitrogen()+Profile->GetTotalNitrogen();
}
double soil::GetTotalNitrogenN()
{
	return Surface->GetTotalNitrogenN()+Profile->GetTotalNitrogenN();
}

/****************************************************************************\
\****************************************************************************/
nitrogen soil::GetTotalNitrogen(double startDep, double thick)
{
	return GetMinNitrogen(startDep,thick) + Profile->GetOrganicNitrogen(startDep,thick);
}
double soil::GetTotalNitrogenN15(double startDep, double thick)
{
		return GetMinNitrogenN15(startDep,thick) + Profile->GetOrganicNitrogenN15(startDep,thick);
}
double soil::GetTotalNitrogenN(double startDep, double thick)
{
	return GetMinNitrogenN(startDep,thick) + Profile->GetOrganicNitrogenN(startDep,thick);
}
double soil::GetTotalNitrogenN(double depth)
{
	return Surface->GetTotalNitrogenN()+ GetTotalNitrogenN(0, depth);
}
double soil::GetTotalNitrogenN15(double depth)
{
	return Surface->GetTotalNitrogenN15()+ GetTotalNitrogenN15(0, depth);
}
/****************************************************************************\
\****************************************************************************/
nitrogen soil::GetTotalNitrogen(double depth)
{
	return Surface->GetTotalNitrogen()+ GetTotalNitrogen(0, depth);
}

/****************************************************************************\
Updates the soil corresponding to the passing of one day.
   EPotToSoil      - potential evapotranspiration allocated to soil
   surplus         - water added to soil [mm/d]
   airTemperature  - air temperature [ï¿½C]
   snowContent     - snow content
   roots           - root descriptor list

\****************************************************************************/
void soil::Update(double surplus,
   		  			double airTemperature,
                  double EPotToSoil,
                  double snowContent,
                  linkList<rootStructure>* rootInfo)
{

   WaterBudget.AddInput(surplus);
UseObservations();
   

   for (int i=0; i<(rootInfo->NumOfNodes()); i++) // Convert root radius from mm to m
      rootInfo->ElementAtNumber(i)->rootRadius=rootInfo->ElementAtNumber(i)->rootRadius/1000.0;

   // Update soil temperature
   if (MasterSoil!=nullptr) // If more than one patch then speed up temperature calculation by using only one master patch
      Profile->UpdateTemperature(MasterSoil);
   else
      Profile->UpdateFinDiffTemperature(airTemperature,snowContent);
      // Profile->UpdateTemperature(airTemperature,snowContent); // Old version

   // Update soil organic matter
	  	  if(globalSettings->DetailsData.getDebugSoil())
	  Before();
   nitrogen startMinN = GetMinNitrogen();
   CO2Evolution = Profile->UpdateOrganicMatter();
   NitrogenNetMineralisation = NitrogenNetMineralisation + GetMinNitrogen()-startMinN;
        if(globalSettings->DetailsData.getDebugSoil())
    print();

   // Update nitrate by denitrification and nitrification
   nitrification = Profile->UpdateNitrification();
   denitrification = Profile->UpdateDenitrification(CO2Evolution);

   Nbudget.AddOutput(denitrification.n+Profile->GetN2OFromDenitrification().n);
   Nbudget.AddOutput(Profile->GetN2OFromNitrification().n);

   // Update soil evaporation and nitrogen movement to soil
   double relativeSurfaceWater = min(1.0,Profile->GetAvailWater(0.,evaporationDepth)/
                                     Profile->GetAvailCapacity(0.,evaporationDepth));
   double maxEvaporation = Profile->GetMaximumEvaporation();
   Surface->Update(&surplus,
                   EPotToSoil,
                   relativeSurfaceWater,
                   maxEvaporation,
                   Profile->GetEvaporationContent(),
					    airTemperature);
   soilEvaporation = Surface->GetEvaporation();
   Profile->SubtractEvaporation(soilEvaporation);
   WaterBudget.AddOutput(soilEvaporation);


   Profile->SubtractTranspiration(rootInfo);


   totalTranspiration = 0;
   for (int i=0; i<rootInfo->NumOfNodes(); i++)
      totalTranspiration  += rootInfo->ElementAtNumber(i)->actualTranspiration;
   WaterBudget.AddOutput(totalTranspiration);


   nitrogen surfaceNitrate  = Surface->ExtractNitrateDisolved();
   nitrogen surfaceAmmonium = Surface->ExtractAmmoniumDisolved();
   surplus += Surface->ExtractWater(); // Code review 2011 Tjek om det er korrekt regnet ud, eller om alt vand og N forsvinder på een gang!!!!!

   double Chloride = Surface->ExtractChloride();
   // Water infiltration and nitrogen leaching

   Profile->UpdateInfiltrationSWAT(&surplus,&surfaceNitrate,&surfaceAmmonium,&nitrateLeached,&ammoniumLeached,Chloride);

   // Deposit possibly "unused" water and N on soil surface
   // Code review Kan det nogensinde ske????!!!
   Surface->AddWater(surplus);
   Surface->AddNitrateDissolved(surfaceNitrate);
   Surface->AddAmmoniumDissolved(surfaceAmmonium);
   // Perform surface run-off
   Surface->SurfaceRunOff();

   // Nitrogen uptake by the plants
   nitrogenUptake = Profile->SubtractNitrogenUptake(rootInfo);

   // Perform budgeting
   WaterBudget.AddOutput(Profile->GetDrainage(GetMaximumDepth())+Surface->GetRunOff());
   if (theTime.GetMonth()==4 && theTime.GetDay()==1)
 	   AccumulatedWaterDrainage = AccumulatedNitrateDrainage = 0.0;
   AccumulatedWaterDrainage   += Profile->GetDrainage(1000.0);
   AccumulatedNitrateDrainage += Profile->GetNitrateLeaching(1000.0).n;

   for (int i=0; i<rootInfo->NumOfNodes(); i++)
 	   Nbudget.AddOutput(nitrogenUptake[i].n);
   Nbudget.AddOutput(Surface->GetRunOff_N().n);
   Nbudget.AddOutput(GetNitrateLeaching(GetMaximumDepth()).n+GetAmmoniumLeaching(GetMaximumDepth()).n);

   // EndBudget(&NRemain,&WaterRemain); !!!???


   WriteOutput();
}

/****************************************************************************\
Incorporates organic matter from the surface and roots into the soil
(depth: mm, fraction: 0-1)
\****************************************************************************/
void soil::Incorporate(double MaxDepth, double frac, bool tillage, const char * tillagetype,double MinDepth, string plantName)
{

   if ((MaxDepth>0 && frac>0))
   {
	   commonData data;
	   data.setCritical(false);
	   if(data.readFile("Tillage.dat")) // Only comes in effect when tillage.dat is present
		{
      if (tillage)
      {
         double p1=0.1;
         double p2=0.5;
         double TurnoverFactor=2;
         data.FindSection(tillagetype);
         data.FindItem("KillSMBFraction",&p1);
         data.FindItem("MoveToFasterFraction",&p2);
        

        		if (p1 > 1.0 || p1 < 0.0 || p2 > 1.0 || p2 < 0.0)
            	theMessage->FatalError("Illegal parameter values for soil tillage effect");
        		if (p1 > 0.0)
            	KillSMB(p1,MaxDepth);
         	if (p2 > 0.0)
            	ModifyAOM(p2,2.0,MaxDepth);
      	}



	}
      int n = Profile->GetNumberOfLayers();
      double LengthList[300];
      //double * p = &LengthList[0];
	  int startLayer,endLayer;
	  if(tillagetype=="Ploughing")
	  {
		  startLayer=0;
		  endLayer=0;
		  double CurrentDepth = 0;
		  for (int i=0; i<n; i++)
		  {
			  double LayerThickness = Profile->GetLayerThickness(i+1);
			  if(MinDepth>=CurrentDepth)
			  {
					  startLayer=i;
			  }
				CurrentDepth+=LayerThickness;
				 LengthList[i]=0;
		  }
		  CurrentDepth=0;
		  for (int i=0; i<n; i++)
		  {
			  double LayerThickness = Profile->GetLayerThickness(i+1);
			  	CurrentDepth+=LayerThickness;
			  if(MaxDepth<=CurrentDepth)
			  {
					  endLayer=i;
					  cout<<theTime<<endl;
					  break;
			  }
			
		  }
		  LengthList[startLayer]=Profile->GetLayerThickness(endLayer+1);
	  }
	  else
	  {
		  double CurrentDepth = 0;
		  
		  for (int i=0; i<n; i++)
		  {
			 double LayerThickness = Profile->GetLayerThickness(i+1);
			 CurrentDepth += LayerThickness;
			 LengthList[i] = 0.;
			if (CurrentDepth<=MaxDepth&&((CurrentDepth-LayerThickness)>=MinDepth))
				LengthList[i] = LayerThickness;
			else if(CurrentDepth<=MaxDepth&&((CurrentDepth-LayerThickness)<MinDepth)&&((CurrentDepth-MinDepth)>=0))
			{
				LengthList[i] = CurrentDepth-MinDepth;
			}
			 else if ((CurrentDepth-LayerThickness<MaxDepth)&&((CurrentDepth-LayerThickness)>=MinDepth))
				   LengthList[i] = MaxDepth-(CurrentDepth-LayerThickness);
				else if((CurrentDepth-LayerThickness)<MaxDepth&&CurrentDepth>MaxDepth&&(CurrentDepth-LayerThickness)<MinDepth&&CurrentDepth>MinDepth)
			 {
				 LengthList[i] = LayerThickness-(MaxDepth-MinDepth);
			 }
		  }
	  }
	  if(plantName.compare("all")==0)
	  {
		  organicProduct * prod;
		  n = Surface->GetNumberOfProducts();
		  for (int j=0; j<n; j++)
		  {
			 prod = Surface->RemoveProduct(j,frac);

			 if (prod->GetAmount().c>0)
 				 AddOrganicProduct(prod,&LengthList[0]);
			 delete prod;
		  }
	  }
	  else
	  {
		  organicProduct * prod;
		  int number=Surface->GetProduct(plantName);
		  if(number!=-1)
		  {
			   prod = Surface->RemoveProduct(number,frac);
		
			 if (prod->GetAmount().c>0)
 				 AddOrganicProduct(prod,&LengthList[0]);
			 delete prod;
		  }
		  else
			  theMessage->FatalError("cannot find part in surface layer");
	  }
	  if(tillagetype=="Ploughing")
	  {
	   Profile->changeLayer(startLayer,endLayer);
	  }
   }
   
}

/****************************************************************************\
depth: mm

\****************************************************************************/
void soil::TillageEffect(int Type,double depth, double effect)
{
  for (int i=0; i<Profile->GetNumberOfLayers(); i++)
  		if (Profile->GetLayer(i)->GetStartDepth()>depth) // Actually a simplification !!!
      	Profile->GetLayer(i)->SetTillageEffect(Type, effect);
}
/****************************************************************************\
Fraction (0-1) : fraction of SMB killed
Depth: mm
\****************************************************************************/
void soil::KillSMB(double Fraction,double Depth)
{
  for (int i=0; i<Profile->GetNumberOfLayers(); i++)
  		if (Profile->GetLayer(i)->GetStartDepth()<Depth) // Actually a simplification !!!
      	Profile->GetLayer(i)->KillSMB(Fraction);
}

/****************************************************************************\
Fraction (0-1) : fraction of AOM transferred to pools with a modified
                 turnover rate
TurnoverFactor : Factor for modification of turnover rate
Depth: mm
\****************************************************************************/
void soil::ModifyAOM(double Fraction,double TurnoverFactor, double Depth)
{
  for (int i=0; i<Profile->GetNumberOfLayers(); i++)
  		if (Profile->GetLayer(i)->GetStartDepth()<Depth) // Actually a simplification !!!
      {
         //cout << "soil::ModifyAOM - layer no " << i << endl;
      	Profile->GetLayer(i)->ModifyAOM(Fraction,TurnoverFactor);
      }
}

/****************************************************************************\
\****************************************************************************/
void soil::AddOrganicProduct(const organicProduct* product,
									  double * rootLengthList)
{
 organicProduct * p = new organicProduct(*product);
if (p->GetOrganicNitrogen().n<0.0)
    theMessage->FatalError("soil::AddOrganicProduct - negative amount of organic N added");
 if (p->GetNitrate().n<0.0)
    theMessage->FatalError("soil::AddOrganicProduct - negative amount of NH4-N added");
 if (p->GetAmmonium().n<0.0)
    theMessage->FatalError("soil::AddOrganicProduct - negative amount of NO3-N added");
 if (rootLengthList==nullptr)
    Surface->AddProduct(p);
 else
    Profile->AddProduct(*p, rootLengthList);
 delete p;
}


void soil::ReadObservations(string FileName)
{
	commonData data;

	data.setCritical(false);
   if (data.readFile(FileName))
   {
      if (data.FindSection("Nmin.Measured"))
      {
      	bsTime aDate;
      	string aDateString;
      	data.FindItem("StartDepth",&ObsStartDepth);
			data.FindItem("EndDepth",&ObsEndDepth);
			data.FindItem("Date",&aDateString);
			data.FindItem("Nmin",&ObsNmin);
         aDate.SetTime(aDateString);
         ObsDate = aDate.GetDateValue();
      }
   }

}
void soil::UseObservations()
{
	if (theTime.GetDateValue()==ObsDate)
   {
   	//AddNutrient(0,0,0,ObsNmin-GetMinNitrogen(ObsStartDepth,ObsEndDepth));
      double factor=0.0;
      if (GetMinNitrogen(ObsStartDepth,ObsEndDepth).n>0)
      	factor = ObsNmin/GetMinNitrogen(ObsStartDepth,ObsEndDepth).n;
      soilLayer* currentLayer =Profile->GetLayer(0);
      while (currentLayer)
      {
         if (currentLayer->GetStartDepth()>=ObsStartDepth && currentLayer->GetEndDepth()<=ObsEndDepth)
         {
         	currentLayer->AddNutrient(currentLayer->GetNitrate()*(factor-1),currentLayer->GetAmmonium()*(factor-1));
         }
         currentLayer = currentLayer->Next();
      }
   }
}
/****************************************************************************\
\****************************************************************************/
void soil::StartBudget()
{
   nitrogen N = GetTotalNitrogen();
   Nbudget.SetInput(N.n);
   WaterBudget.SetInput(GetWater(0,GetMaximumDepth())+GetSurfaceWater());
   Profile->StartBudget();
}

/****************************************************************************\
\****************************************************************************/
bool soil::EndBudget(double * NRemain, double *WaterRemain)
{
   bool retVal=true;
   *NRemain = GetTotalNitrogen().n;
   if (!Nbudget.Balance(*NRemain))
	   retVal=false;
   *WaterRemain = GetWater(0,GetMaximumDepth())+GetSurfaceWater();
   if (!WaterBudget.Balance(*WaterRemain))
      retVal=false;
   Profile->EndBudget();
   return retVal;
}
/****************************************************************************\
\****************************************************************************/
void soil::WriteOutput()
{

 	if(globalSettings->DetailsData.getSoilDetails()==true)
	{
 		f << theTime.GetYear() << "-" << theTime.GetMonth() << "-" << theTime.GetDay();


 		for(unsigned int i=0;i<globalSettings->WaterData.size();i++)
	 	{
	 		f << "\t" << GetWater(globalSettings->WaterData[i].getStartDepth(), globalSettings->WaterData[i].getEndDepth()-globalSettings->WaterData[i].getStartDepth());

	 	}

 		for(unsigned int i=0;i<globalSettings->NminData.size();i++)
 		{
 			double StartDepth =globalSettings->NminData[i].getStartDepth();
 			double Thickness = globalSettings->NminData[i].getEndDepth()-StartDepth;



	 		f << "\t" << GetMinNitrogen(StartDepth, Thickness).n;
	 		f << "\t" << GetNitrate(StartDepth, Thickness).n;
	 		f << "\t" << GetAmmonium(StartDepth, Thickness).n;
	 		f << "\t" << GetTotalNitrogenN15(StartDepth, Thickness);
	 	}

 		for(unsigned int i=0;i<globalSettings->SoilOrgCData.size();i++)
 		{
 			double StartDepth =globalSettings->SoilOrgCData[i].getStartDepth();
 			double Thickness = globalSettings->SoilOrgCData[i].getEndDepth()-StartDepth;
 			double SoilMass = GetSoilMass(StartDepth,Thickness)/10000000.0; // BMP 29.10.2008 !!!!!!
	 		f << "\t" << (GetPoolCarbon(2,StartDepth,Thickness) + GetPoolCarbon(3,StartDepth,Thickness))/SoilMass;

	 	}
 		for(unsigned int i=0;i<globalSettings->PoreWaterData.size();i++)
 		{
 			double depth = globalSettings->PoreWaterData[i].getDepth();
 	 		f << "\t" << 1000.0*GetNitrate(depth-5,10).n/GetWater(depth-5,10)
         	<< "\t" << 1000.0*GetAmmonium(depth-5,10).n/GetWater(depth-5,10)
            << "\t" << 1000.0*(GetNitrate(depth-5,10).n+GetAmmonium(depth-5,10).n)/GetWater(depth-5,10)         // Convert to mg/l
         	<< "\t" << 1000.0*Profile->GetChloride(depth-5,10)/GetWater(depth-5,10)    // Convert to mg/l
            << "\t" << 1e5*GetNitrate(depth-5,10).n15/GetWater(depth-5,10);

  		}

      for(unsigned int i=0;i<globalSettings->TemperatureData.size();i++)
 		{

 			double depth = globalSettings->TemperatureData[i].getDepth();
 	 		f << "\t" << Profile->GetTemperature(depth);

  		}

      f  << "\t" << GetAmmonium(0,250).n
         << "\t" << GetNitrate(0,250).n
	      << "\t" << GetAmmonium(0,250).n15
         << "\t" << GetNitrate(0,250).n15;
	   f  << "\t" << GetPoolNitrogenN(0,0,depth);
	    f  << "\t" << GetPoolNitrogenN(1,0,depth)
	      << "\t" << GetPoolNitrogenN(2,0,depth)
	      << "\t" << GetPoolNitrogenN(3,0,depth)
	      << "\t" << GetPoolNitrogenN(4,0,depth);
      f.precision(5);
	   f  << "\t" << GetPoolNitrogenN(5,0,depth);
	   f.precision(4);
	   f  << "\t" << GetPoolNitrogenN(6,0,depth)
	      << "\t" << GetPoolNitrogenN15(0,0,depth)
	      << "\t" << GetPoolNitrogenN15(1,0,depth)
	      << "\t" << GetPoolNitrogenN15(2,0,depth)
		  << "\t" << GetPoolNitrogenN15(3,0,depth)
	      << "\t" << GetPoolNitrogenN15(4,0,depth)
	      << "\t" << GetPoolNitrogenN15(5,0,depth);
   	f.precision(5);
	   f  << "\t" << GetPoolCarbon(5,0,depth);
      double N2ORatio = 1;
      if (Profile->GetN2OFromNitrification().n+Profile->GetN2OFromDenitrification().n>1e-10)
      	N2ORatio = denitrification.n/(Profile->GetN2OFromNitrification().n+Profile->GetN2OFromDenitrification().n);
      f.precision(4);
	   f  << "\t" << GetPoolCarbon(6,0,depth)
      	<< "\t" << GetPoolCarbon(0,0,depth)
	      << "\t" << GetPoolCarbon(1,0,depth)
	      << "\t" << GetPoolCarbon(2,0,depth)
	      << "\t" << GetPoolCarbon(3,0,depth)
         << "\t" << GetPoolCarbon(4,0,depth)
         << "\t" << GetTotalCarbon(0,200)
         << "\t" << GetTotalCarbon(200,200)
         << "\t" << GetTotalCarbon(400,200)
         << "\t" << GetTotalNitrogen(0,200).n
         << "\t" << GetTotalCarbon(0,1000)
         << "\t" << GetTotalNitrogen(0,500).n15
         << "\t" << GetTotalNitrogen(2000).n15
	      << "\t" << NitrogenNetMineralisation.n
         << "\t" << Profile->GetN2OFromNitrification().n
	      << "\t" << Profile->GetN2OFromDenitrification().n
	     	<< "\t" << Profile->GetN2OFromNitrification().n+Profile->GetN2OFromDenitrification().n
         << "\t" << N2ORatio
         << "\t" << AccumulatedWaterDrainage
         << "\t" << AccumulatedNitrateDrainage
         << "\t" << soilEvaporation
         << "\t" << totalTranspiration
         << "\t" << denitrification.n
         << "\t" << nitrification.n
         << "\t" << CO2Evolution;


      double tempAdjust = 1.0/exp(-3.432+0.168*10*(1.0-0.5*10/36.9));
   	double TemperatureEffect = tempAdjust*exp(-3.432+0.168*20.0*(1.0-0.5*20.0/36.9));                   // Note check if temperature effect on organimc matter
      double PotC = 0.0;
   	for (int i=0;i<20;i++)
      	PotC += Profile->GetLayer(i)->PotentialCarbonTurnover();
      f << "\t" << TemperatureEffect*PotC;
      if (Profile->GetN2OFromDenitrification().n>1e-10)
	      f << "\t" << denitrification.n/Profile->GetN2OFromDenitrification().n;
      else
			f << "\t0";
      for (int i=0;i<7;i++)
      {
      	f << "\t" << Profile->GetLayer(i)->GetN2OFromDenitrification().n;
      	f << "\t" << Profile->GetLayer(i)->PotentialCarbonTurnover();

         double WaterEffect = max(0.0,min(1.0,0.0116+1.36/(1+exp(-(Profile->GetLayer(i)->GetWaterFilledPorosity()-0.815)/0.0896))));
      	f << "\t" << WaterEffect;

         double tempAdjust = 1.0/exp(-3.432+0.168*10*(1.0-0.5*10/36.9));
		   double TemperatureEffect = tempAdjust*exp(-3.432+0.168*Profile->GetLayer(i)->GetTemperature()*(1.0-0.5*Profile->GetLayer(i)->GetTemperature()/36.9));
         f << "\t" << TemperatureEffect;

         double NitrateConc = Profile->GetLayer(i)->GetNitrate().n/Profile->GetLayer(i)->GetThickness()*1000/Profile->GetLayer(i)->GetDryBulkDensity()*1e6; //unit mg kg-1
		   double NitrateEffect = max(0.0,min(1.0,1.17*NitrateConc/(32.7+NitrateConc)));

      	f << "\t" << NitrateEffect;
		}
      f << "\t" << Surface->GetCarbon() << "\t" << Surface->GetOrganicNitrogen().n << "\t" <<  Surface->GetTotalNitrogen().n;
      f << endl;
	  if(globalSettings->DetailsData.DebugSoil==true)
	  {
	   WriteC();
	  static bool usedbefore2=false;
	  WriteCarbon(0,usedbefore2);
	  WriteCarbon(1,usedbefore2);
	  WriteCarbon(2,usedbefore2);
	WriteCarbon(3,usedbefore2);

	WriteCarbon(4,usedbefore2);
	WriteCarbon(5,usedbefore2);

	  WriteN(0,usedbefore2);
	  WriteN(1,usedbefore2);
	  WriteN(2,usedbefore2);
	  WriteN(3,usedbefore2);

	  WriteN(4,usedbefore2);
	  WriteN(5,usedbefore2);


	  WriteCN(0,usedbefore2);
	  WriteCN(1,usedbefore2);
	  WriteCN(2,usedbefore2);
	  WriteCN(3,usedbefore2);

	  WriteDecompositionRate(4,usedbefore2);
	  WriteDecompositionRate(5,usedbefore2);
	    WriteDecompositionRate(0,usedbefore2);
	  WriteDecompositionRate(1,usedbefore2);
	  WriteDecompositionRate(2,usedbefore2);
	  WriteDecompositionRate(3,usedbefore2);

	usedbefore2=true;
	  }
   }
}

/****************************************************************************\
\****************************************************************************/
void soil::AddDecomposable(decomposable* produc,double * rootLengthList)
{
   produc->RemoveWater();                                          // To cope with dry matters less than 1
  	if (produc->GetAmount()<-1E-10)
  		theMessage->FatalError("soil::AddDecomposable - negative amount added");
 	if (produc->GetAmount()>0)
 	{
      if (produc->GetorgN_content().n<0.0)
      {
    	  cout<<produc->GetorgN_content().n<<endl;
   		theMessage->FatalError("soil::AddDecomposable - negative amount of organic N added");
      }
  		//if ((rootLengthList) && (*rootLengthList<=0))
   	//	theMessage->FatalError("soil::AddDecomposable - rootLengthList can not contain zero in first element");
  		organicProduct * p = new organicProduct;

  		p->SetName(produc->GetName());
  		p->SetCarbon(produc->GetC_content()*produc->GetAmount()*100);
  		p->SetNitrogen(produc->GetorgN_content()*produc->GetAmount()*100);
  		p->SetNO3_content(produc->GetNO3_content()*produc->GetAmount()*100);
  		p->SetNH4_content(produc->GetNH4_content()*produc->GetAmount()*100);
  		p->SetkAOM1(produc->GetkAOM1());
  		p->SetkAOM2(produc->GetkAOM2());
      p->SetReduceTurnover(produc->GetReduceTurnover());
  		if (produc->GetAOM1_fraction()>0.0)
   		p->SetAOM1_fraction(produc->GetAOM1_fraction());
  		if (produc->GetAOM2_fraction()>0.0)
   		p->SetAOM2_fraction(produc->GetAOM2_fraction());

      // N allocations
       double sum=0.0;
  	   if ((produc->GetAOM1_Nfraction()>0.0) || (produc->GetAOM2_Nfraction()>0.0))
       {
   		p->SetNFraction(0,produc->GetAOM1_Nfraction());           	// AOM1
         sum+=produc->GetAOM1_Nfraction();
   		p->SetNFraction(1,produc->GetAOM2_Nfraction());           	// AOM2
        sum+=produc->GetAOM2_Nfraction();
        p->SetNFraction(2,1.0-sum);     // NOM
        if (sum<0 || sum>1)
        	theMessage->FatalError("soil::AddDecomposable - inconsistency in values for fAOM1 and/or fAOM2");
       }
  		AddOrganicProduct(p,rootLengthList);
  		Nbudget.AddInput(p->GetTotalNitrogen().n);
   	delete p;
 	}
}

	void soil::Before()
{
	BeforeAmmonium();
	BeforeNitrate();
}
void soil::print()
{
	PrintAmmonium();
	PrintNitrate();
}
void soil::BeforeNitrate()
{
	Surface->BeforeNitrateSolid();
	Surface->BeforeNitrateDisolved();
	Profile->BeforeNitrate();
}
void soil::PrintNitrate()
{
	Surface->PrintNitrateSolid();
	Surface->PrintNitrateDisolved();
	Profile->PrintNitrate();
}
void soil::BeforeAmmonium()
{
	Surface->BeforeAmmoniumSolid();
	Surface->BeforeAmmoniumDisolved();
	Profile->BeforeAmmonium();

}
void soil::PrintAmmonium()
{
	Surface->PrintAmmoniumSolid();
	Surface->PrintAmmoniumDisolved();
	Profile->PrintAmmonium();


}