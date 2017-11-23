/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// Soil surface
// (c) Jørgen E. Olesen, Statens Planteavlsforsøg
// E-mail: jeo@pvf.sp.dk
// Changes:
// JEO 27-07-1997: Comments added
// JEO 30-07-1997: Snow accumulation removed
// JEO 27-10-1997: New soil evaporation formula
// =============================================================================

#include "../base/common.h"
// =============================================================================
// Soil surface
// =============================================================================

#include "soilSurface.h"
#include "../base/message.h"
#include "../base/bstime.h"

/****************************************************************************\
\****************************************************************************/
soilSurface::soilSurface()
: base()
{
   firstProduct=NULL;
   lastProduct=NULL;
   ammoniumSolid.Clear();
   nitrateSolid.Clear();
   ammoniumDisolved.Clear();
   nitrateDisolved.Clear();
   ammoniaVolatilisation.Clear();
   RunOff_N.Clear();
   nitrogenDisolvedConstant = 0.05;
   nitrogenLeachedConstant  = 0.05;
   evaporation              = 0;
   temperature              = 0;
   water                    = 0;
   RunOff                   = 0;
   Chloride                 = 0;
}

/****************************************************************************\
Copy constructor
surface - Source object
\****************************************************************************/
soilSurface::soilSurface(const soilSurface& surface)
 : base(surface)
{
 firstProduct = NULL;
 lastProduct = NULL;
 if (&surface)
 {
  ammoniumSolid            = surface.ammoniumSolid;
  nitrateSolid             = surface.nitrateSolid;
  ammoniumDisolved         = surface.ammoniumDisolved;
  nitrateDisolved          = surface.nitrateDisolved;
  nitrogenDisolvedConstant = surface.nitrogenDisolvedConstant;
  nitrogenLeachedConstant  = surface.nitrogenLeachedConstant;
  evaporation              = surface.evaporation;
  ammoniaVolatilisation    = surface.ammoniaVolatilisation;
  temperature              = surface.temperature;
  water                    = surface.water;
  RunOff                   = surface.RunOff;
  RunOff_N                 = surface.RunOff_N;
  Chloride                 = surface.Chloride;
  organicProduct * current;
  organicProduct * tempProduct;
  current = surface.firstProduct;
  while (current)
  {
	tempProduct = new organicProduct(*current);
	AddProduct(tempProduct);
	current = current->Next();
	delete tempProduct;
  }
 }
}

/****************************************************************************\
Destructor
\****************************************************************************/
soilSurface::~soilSurface()
{
 organicProduct * p1;
 organicProduct * p2;
 p1 = firstProduct;
 while (p1)
 {
  p2=p1;
  p1 = p1->Next();
  delete p2;
 }
}

/****************************************************************************\
\****************************************************************************/
void soilSurface::Add(soilSurface* surface, double fraction)
{
 if (&surface)
 {
  ammoniumSolid            = ammoniumSolid*(1.0-fraction) + surface->ammoniumSolid*fraction;
  nitrateSolid             = nitrateSolid*(1.0-fraction) + surface->nitrateSolid*fraction;
  ammoniumDisolved         = ammoniumDisolved*(1.0-fraction) + surface->ammoniumDisolved*fraction;
  nitrateDisolved          = nitrateDisolved*(1.0-fraction) + surface->nitrateDisolved*fraction;
  ammoniaVolatilisation    = ammoniaVolatilisation*(1.0-fraction) + surface->ammoniaVolatilisation*fraction;
  RunOff_N                 = RunOff_N*(1.0-fraction)+surface->RunOff_N*fraction;

  nitrogenDisolvedConstant = (1.0-fraction)*nitrogenDisolvedConstant+fraction*surface->nitrogenDisolvedConstant;
  nitrogenLeachedConstant  = (1.0-fraction)*nitrogenLeachedConstant+fraction*surface->nitrogenLeachedConstant;
  evaporation              = (1.0-fraction)*evaporation+fraction*surface->evaporation;
  temperature              = (1.0-fraction)*temperature+fraction*surface->temperature;
  water                    = (1.0-fraction)*water+fraction*surface->water;
  RunOff                   = (1.0-fraction)*RunOff+fraction*surface->RunOff;
  Chloride                 = (1.0-fraction)*Chloride+fraction*surface->Chloride;

  organicProduct * current;
  current = firstProduct;                           // first adjust original products
  while (current)
  {
   current->Scale(1.0-fraction);
	current = current->Next();
  }
  current = surface->firstProduct;                 // add products from surface
  while (current)
  {
   current->Scale(fraction);
	AddProduct(current);
	current = current->Next();
  }
 }
}

/****************************************************************************\
Updates the soil surface corresponding to the passing of one day.
   surplus            - Water to be added (and drained) from soil surface [mm/d]
   ePotToSoil         - Potential evapotranspiration from soil [mm/d]
   sumWaterContent    - Content of plant available water in the evaporation depth [mm]
   sumWaterCapacity   - Capacity for plant availble water in the evaporation depth [mm]
   airTemperature     - Air temperature [°C]
\****************************************************************************/
void soilSurface::Update(double * surplus,
								 double   ePotToSoil,
								 double   relativeSurfaceWater,
								 double   maxEvaporation,
                         double   evaporationContent,
								 double   airTemperature)
{
   temperature = airTemperature;
 	// Calculate soil evaporation
 	double E;
 	// Old method
 	/*
	if (RelWater>1)
		E=1;
 	else
  if (RelWater>0.33)
   E=0.9+0.1*(RelWater-0.33)/0.67;
  else
   if (RelWater>0.22)
    E=0.27+0.63*(RelWater-0.22)/0.11;
   else
    if (RelWater>0.2)
     E=0.05+0.22*(RelWater-0.2)/0.02;
    else
      E=0.05*RelWater/0.2;
 */
 // New method. Modified after Plauborg & Olesen (MVTOOL) and Plauborg (Ph.D. dissertation)
	if (ePotToSoil<=evaporationContent)
  		E=1.0;
 	else
  		E=0.65*relativeSurfaceWater;
  evaporation = min(maxEvaporation,E*ePotToSoil);

 	// Update mineral nitrogen contents
 	double disolved  = min(1.0,nitrogenDisolvedConstant*relativeSurfaceWater+nitrogenLeachedConstant*(*surplus));
 	nitrateDisolved  = nitrateDisolved + nitrateSolid*disolved;
 	nitrateSolid     = nitrateSolid - nitrateSolid*disolved;

 	disolved = min(1.0,nitrogenDisolvedConstant*relativeSurfaceWater+nitrogenLeachedConstant*(*surplus));
 	ammoniumDisolved = ammoniumDisolved + ammoniumSolid*disolved;
 	ammoniumSolid    = ammoniumSolid - ammoniumSolid*disolved;
   if(ammoniumDisolved.n<-1E-6 || nitrateDisolved.n<-1E-6 || ammoniumSolid.n<-1E-6 || nitrateSolid.n<-1E-6)
   	theMessage->WarningWithDisplay("SoilSurface::Update - negative n concentrations");
}

/****************************************************************************\
\****************************************************************************/
void soilSurface::SurfaceRunOff()
{
   const double maxWater=25.0; //125.0;
   RunOff = 0.0;
   RunOff_N.Clear();
   if (water>maxWater)
   {
      double f=max(0.0,(water-maxWater))*0.1/water;
      nitrateDisolved  = nitrateDisolved + nitrateSolid*0.5;
      ammoniumDisolved = ammoniumDisolved + ammoniumSolid*0.5;

      RunOff   = f*water;
      RunOff_N = (nitrateDisolved+ammoniumDisolved)*f;
      water   -= f*water;
      nitrateDisolved  = nitrateDisolved - nitrateDisolved*f;
      ammoniumDisolved = ammoniumDisolved - ammoniumDisolved*f;
      nitrateDisolved  = nitrateDisolved + nitrateSolid*0.5;
      ammoniumDisolved = ammoniumDisolved + ammoniumSolid*0.5;
      nitrateSolid.Clear();
      ammoniumSolid.Clear();
   }
   if(ammoniumDisolved.n<-1E-6 || nitrateDisolved.n<-1E-6 || ammoniumSolid.n<-1E-6 || nitrateSolid.n<-1E-6)
   	theMessage->WarningWithDisplay("SoilSurface::SurfaceRunOff - negative n concentrations");
}


/****************************************************************************\
\****************************************************************************/
double soilSurface::ExtractWater()
{
   double extract = water;
   water = 0.0;
   return extract;
}

/****************************************************************************\
\****************************************************************************/
double soilSurface::ExtractChloride()
{
	double extract = Chloride;
   Chloride = 0.0;
   return extract;
}

/****************************************************************************\
\****************************************************************************/
nitrogen soilSurface::GetTotalNitrogen()
{
	nitrogen TotalN = GetAmmoniumSolid()+GetNitrateSolid()+GetAmmoniumDisolved()+
   						GetNitrateDisolved()+GetOrganicNitrogen();
   return TotalN;
}

/****************************************************************************\
Adds nitrogen to the soil surface
   addNitrate    - Nitrate to be added [g N/m²]
   addAmmonium   - Ammonium to be added [g N/m²]
\****************************************************************************/
void soilSurface::AddSolidNitrogen(nitrogen addNitrate, nitrogen addAmmonium)
{
   nitrateSolid  = nitrateSolid + addNitrate;
   ammoniumSolid = ammoniumSolid + addAmmonium;
   if(ammoniumDisolved.n<-1E-6 || nitrateDisolved.n<-1E-6 || ammoniumSolid.n<-1E-6 || nitrateSolid.n<-1E-6)
	   theMessage->WarningWithDisplay("SoilSurface::AddNitrogen - negative n concentrations");
}

/****************************************************************************\
\****************************************************************************/
void soilSurface::AddDisolvedNitrogen(nitrogen addNitrate, nitrogen addAmmonium)
{
   nitrateDisolved  = nitrateDisolved + addNitrate;
   ammoniumDisolved = ammoniumDisolved + addAmmonium;
   if(ammoniumDisolved.n<-1E-6 || nitrateDisolved.n<-1E-6 || ammoniumSolid.n<-1E-6 || nitrateSolid.n<-1E-6)
	   theMessage->WarningWithDisplay("SoilSurface::AddNitrogen - negative n concentrations");
}

/****************************************************************************\
Adds an organic (or inorganic) product to the the soil surface
   product - Organic product object
\****************************************************************************/
void soilSurface::AddProduct(const organicProduct* product)
{
	organicProduct * p = new organicProduct(*product);
 	AddDisolvedNitrogen(p->GetNitrate(),p->GetAmmonium());
 	p->ClearNO3_content();
 	p->ClearNH4_content();
 	if (!firstProduct)
 		firstProduct = lastProduct = p;
 	else
 	{
  		bool poolResemblance = false;
  		organicProduct* aProduct = firstProduct;
  		while (aProduct)
  		{
  			if (!poolResemblance && (aProduct->ProductSimilarity(p)))
   		{
    			aProduct->AddNext(p);
    			poolResemblance = true;
   		}
   		aProduct = aProduct->Next();
  		}
  		if (!poolResemblance)
  		{
	  		bool productAdded = lastProduct->AddNext(p);
	  		if (productAdded) lastProduct = p;
  		}
 	}
}

/****************************************************************************\
Removes part of an organic product from the soil surface. The removed product
is returned as an object in the function call.
   No   - Product number in list to remove (0..)
   frac - Fraction of product to be removed
\****************************************************************************/
organicProduct * soilSurface::RemoveProduct(int No, double frac)
{
	organicProduct * p = firstProduct;
 	organicProduct * np = NULL;
 	for (int i=0; i<No; i++)
      if (p)
         p = p->Next();
 	if (p)
 	{
  		np = p->GetFraction(frac);
  		p->Scale(1.0-frac);
 	}
 	return np;
}

/****************************************************************************\
Extract ammonium
\****************************************************************************/
nitrogen soilSurface::ExtractAmmoniumDisolved()
{
	nitrogen am=ammoniumDisolved;
   ammoniumDisolved.Clear();
   return am;
}

/****************************************************************************\
Extract nitrate
\****************************************************************************/
nitrogen soilSurface::ExtractNitrateDisolved()
{
	nitrogen n = nitrateDisolved;
   nitrateDisolved.Clear();
   return n;
}

/****************************************************************************\
Returns carbon content in products stored on soil surface [g C/m2].
\****************************************************************************/
double soilSurface::GetCarbon()
{
   organicProduct * p = firstProduct;
   double sum = 0.;
   while (p)
   {
      sum += p->GetCarbon();
      p = p->Next();
   }
   return sum;
}

/****************************************************************************\
Returns content of organic nitrogen in products on soil surface [g N/m2].
\****************************************************************************/
nitrogen soilSurface::GetOrganicNitrogen()
{
   organicProduct * p = firstProduct;
   nitrogen sum;
   sum.Clear();
   while (p)
   {
      sum = sum +p->GetOrganicNitrogen();
      p = p->Next();
   }
   return sum;
}

/****************************************************************************\
Returns number of organic product objects stored on soil surface
\****************************************************************************/
int soilSurface::GetNumberOfProducts()
{
   int n=0;
   organicProduct * current = firstProduct;
   while (current != NULL)
   {
      n++;
      current = current->Next();
   }
   return n;
}

