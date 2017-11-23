
#ifndef __TYPER_H
  #define __TYPER_H

enum
{
   BackScope=4,
   id_len=9
};

enum
{
   MaxPlants = 10
};

enum
{
	MaxSoilLayers=100
};



enum techEqpSourceType
{
   farm,
   external
};

enum chemicalType
{
   herbicide,
   fungicide,
   insecticide,
   pesticide,
   microfertilizer
};

enum indicatorTypes
{
   environmentalIndicator,
   economicIndicator,
   noType
};


enum unitType
{
   kwh,       // kilo watt hour
   tfi,       // treatment frequency index
   t,         // tons
   kg,
   g,
   m3,        // cubic meters
   l,
   ha,        // hectars
   m2,        // square meters
   m,
   mm,
   sfu,       // scandinavian feeding units
   day,
   hour,
   kr,
   fraction,
   pcs        // pieces
};

enum productObjType
{
   productObj,
   animalObj,
   chemicalObj,
   waterObj,
   seedObj,
   energyObj,
   decomposableObj,
   plantItemObj,
   feedItemObj,
   manureObj,
   milkObj,
   fertilizerObj,

};


typedef struct
{
   double rootRadius;
   double rootpF;
   double wiltingPoint;
   double NitrogenDemand;
   double NitrateUptakeRate;
   double AmmoniumUptakeRate;
   double MinimumSoilNitrate;
   double MinimumSoilAmmonium;
   double* rootLengthList;
   double transpirationDemand;
   double actualTranspiration;
} rootStructure;

enum
{
   MAX_TXT=255
};
#endif


