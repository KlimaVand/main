/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// soilsrfc.h
// Soil surface
// (c) Jørgen E. Olesen, Statens Planteavlsforsøg
// E-mail: jeo@pvf.sp.dk
// Changes:
// JEO 27-07-1997: Comments added
// JEO 30-07-1997: Snow accumulation removed
// JEO 27-10-1997: New soil evaporation formula
// =============================================================================

#ifndef SOILSRFC
#define SOILSRFC

#include "../base/base.h"
#include "../base/bstime.h"
#include "../data/systemData.h"
//#include "soldscrp.h" !!!
#include "../products/organicProduct.h"

class soilSurface : public base
{
 protected:
 organicProduct * firstProduct;   // Pointer to first organic product on surface
 organicProduct * lastProduct;    // Pointer to last organic product on surface
 // Dynamic variables
 nitrogen ammoniumSolid;          // Undisolved ammonium content [gN/m2]
 nitrogen nitrateSolid;           // Undisolved nitrate content [gN/m2]
 nitrogen ammoniumDisolved;       // Disolved ammonium content [gN/m2]
 nitrogen nitrateDisolved;        // Disolved nitrate content [gN/m2]
 double Chloride;                 // Added chloride (g m-2)
 nitrogen RunOff_N;
 double water;                    // Surface water [mm]
 double RunOff;
 // Parameters
 double nitrogenDisolvedConstant; // Nitrogen leaching constant [/d]
 double nitrogenLeachedConstant;  // Nitrogen leaching constant [g N/m²/mm]
 // Temporary variables
 double evaporation;              // Soil evaporation [mm/d]
 nitrogen ammoniaVolatilisation;  // Ammonia volatilisation [g N/m²]
 double temperature;              // Air temperature [°C]

 public:
 soilSurface();
 // Constructor
 soilSurface(const soilSurface& surface);
 // Copy constructor
 ~soilSurface();
 // Destructor

 // Update functions
 void   Update(double * surplus,
					double   ePotToSoil,
					double   relativeSurfaceWater,
					double   maxEvaporation,
               double   evaporationContent,
					double   airTemperature);
 void SurfaceRunOff();
 void   Add(soilSurface* surface, double fraction);
 void   AddSolidNitrogen(nitrogen addNitrate,nitrogen addAmmonium);      // add N to surface (units: g-N/m2)
 void   AddDisolvedNitrogen(nitrogen addNitrate,nitrogen addAmmonium);      // add N to surface (units: g-N/m2)
 void   AddProduct(const organicProduct* product);
 // Adds an organic (or inorganic) product to the the soil surface
 //  product - Organic product object
 organicProduct * RemoveProduct(int No, double frac);
 // Removes part of an organic product from the soil surface. The removed product
 // is returned as an object in the function call.
 //  No   - Product number in list to remove (0..)
 //  frac - Fraction of product to be removed
 int GetProduct(string productName);
 // Access functions
 int    GetProducts();
 // GetProducts
 // Returns number of organic product objects stored on soil surface
 double GetEvaporation() {return evaporation;};
 // Returns soil evaporation [mm/d]
 //double GetNitrateLeaching() {return nitrateLeaching;};
 // Returns nitrate leached in previous timestep [g N/m²/d]
 //double GetAmmoniumLeaching() {return ammoniumLeaching;};
 // Returns ammonium leached in previous timestep [g N/m²/d]
 nitrogen GetAmmoniaVolatilisation() {return ammoniaVolatilisation;};
 double beforeAmmoniumSolid;
 void BeforeAmmoniumSolid() {beforeAmmoniumSolid= ammoniumSolid.n;};
  void PrintAmmoniumSolid() {
	 
 static ofstream AmmoniumSolidconent;
 static bool AmmoniumSolidUsed=false;
	if(AmmoniumSolidUsed==false)
	{

		AmmoniumSolidconent.open(theSystemData->getOutputDirectory()+"AmmoniumSolidUsed.xls",ios::out);
		AmmoniumSolidconent<<theTime<<"\t";
		AmmoniumSolidUsed=true;
		
	}
	else
	{
	AmmoniumSolidconent.open(theSystemData->getOutputDirectory()+"AmmoniumSolidUsed.xls",ios::app);
	AmmoniumSolidconent<<theTime<<"\t";
	}
	AmmoniumSolidconent<<beforeAmmoniumSolid<<"\t"<<ammoniumSolid.n<<endl;
	AmmoniumSolidconent.close();
};
 // Returns ammonia volatilisation [g N/m²]
 nitrogen GetAmmoniumSolid() {return ammoniumSolid;};
 double BeforenitrateSolids;
 void BeforeNitrateSolid() {BeforenitrateSolids=nitrateSolid.n;};
 // Returns ammonium nitrogen in the surface layer [g N/m²]
 nitrogen GetNitrateSolid() {return nitrateSolid;};
 // Returns nitrate nitrogen in the surface layer [g N/m²]
 void AddAmmoniumDissolved(nitrogen ad) {ammoniumDisolved = ammoniumDisolved + ad;};
 double beforeAmmoniumDisolved;
  void BeforeAmmoniumDisolved() {beforeAmmoniumDisolved=ammoniumDisolved.n;};
  void PrintAmmoniumDisolved() {
	   static ofstream ammoniumDisolvedconent;
 static bool ammoniumDisolvedUsed=false;
	if(ammoniumDisolvedUsed==false)
	{

		ammoniumDisolvedconent.open(theSystemData->getOutputDirectory()+"ammoniumDisolved.xls",ios::out);
		ammoniumDisolvedconent<<theTime<<"\t";
		ammoniumDisolvedUsed=true;
	}
	else
	{
	ammoniumDisolvedconent.open(theSystemData->getOutputDirectory()+"ammoniumDisolved.xls",ios::app);
	ammoniumDisolvedconent<<theTime<<"\t";
	}
	ammoniumDisolvedconent<<beforeAmmoniumDisolved<<"\t"<<ammoniumDisolved.n<<endl;
	ammoniumDisolvedconent.close();
	 };
 nitrogen GetAmmoniumDisolved() {return ammoniumDisolved;};
 // Returns disolved ammonium nitrogen in the surface layer [g N/m²]
 nitrogen ExtractAmmoniumDisolved();
 void AddNitrateDissolved(nitrogen nd) {nitrateDisolved = nitrateDisolved + nd;};
 double BeforenitrateDisolved;
  void BeforeNitrateDisolved() { BeforenitrateDisolved=nitrateDisolved.n;};
    void PrintNitrateDisolved() {
	   static ofstream NitrateDisolvedContent;
 static bool NitroDisolvedUsed=false;
	if(NitroDisolvedUsed==false)
	{

		NitrateDisolvedContent.open(theSystemData->getOutputDirectory()+"nitrateDisolved.xls",ios::out);
		NitrateDisolvedContent<<theTime<<"\t";
		NitroDisolvedUsed=true;
	}
	else
	{
	NitrateDisolvedContent.open(theSystemData->getOutputDirectory()+"nitrateDisolved.xls",ios::app);
	NitrateDisolvedContent<<theTime<<"\t";
	}
	NitrateDisolvedContent<<BeforenitrateDisolved<<"\t"<<nitrateDisolved.n<<endl;
	NitrateDisolvedContent.close();
	 };
    void PrintNitrateSolid() {
	 
 static ofstream NitrateSolidContent;
 static bool NitrateSolidUsed=false;
	if(NitrateSolidUsed==false)
	{

		NitrateSolidContent.open(theSystemData->getOutputDirectory()+"NitrateSolid.xls",ios::out);
		NitrateSolidContent<<theTime<<"\t";
		NitrateSolidUsed=true;
		
	}
	else
	{
	NitrateSolidContent.open(theSystemData->getOutputDirectory()+"NitrateSolid.xls",ios::app);
	NitrateSolidContent<<theTime<<"\t";
	}
	NitrateSolidContent<<BeforenitrateSolids<<"\t"<<nitrateSolid.n<<endl;
	NitrateSolidContent.close();
};
 nitrogen GetNitrateDisolved() {return nitrateDisolved;};
 // Returns disolved nitrate nitrogen in the surface layer [g N/m²]
 nitrogen ExtractNitrateDisolved();
 double ExtractChloride();
 void AddChloride(double amount) {Chloride += amount;};
 double GetCarbon();
 // Returns carbon in the products at the soil surface [g C/m²]
 nitrogen GetOrganicNitrogen();
 // Returns organic nitrogen in the products at the soil surface [g N/m²]
 nitrogen GetTotalNitrogen();
 // Returns total nitrogen in the soil surface [g N/m²]

 double GetWater() {return water;};
 void ExtractWater(double *ew);
 double ExtractWater();
 double GetRunOff() {return RunOff;};
 nitrogen GetRunOff_N() {return RunOff_N;};
 void AddWater(double aw) {water+=aw;};
};

#endif
