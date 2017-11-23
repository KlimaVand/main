/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "IndicatorOutput.h"
#include "../fields/field.h"



//--------------Static data member initialization-----------------------------
 fstream* IndicatorOutput::fileStreamCSV = NULL;
 static char* character =new char[1000];
 int IndicatorOutput::yearNumber = 0;
//----------------------------------------------------------------------------

/****************************************************************************\
  Constructor
\****************************************************************************/
IndicatorOutput::IndicatorOutput()
{
   oldstoreN=0.0;
   storeN=0.0;
   storeSoilN=0.0;
   oldstoreSoilN=0.0;
   oldstoreCropN=0.0;

   storeLivestockN=-99999.0;
   oldstoreLivestockN=-99999.0;
   storeHouseEtcN=0.0;
   oldstoreHouseEtcN=0.0;
   oldstoredProductN = 0.0;
   oldstoreW=0.0;
   storeW=0.0;

   lastErrorN=0.0;
   lastErrorSoilN=0.0;
   lastErrorCropN=0.0;
   lastErrorLivestockN=0.0;
   lastErrorHouseEtcN=0.0;
   lastErrorStoredN=0.0;

   oldstoreC=0.0;
   storeC=0.0;
   storeSoilC=0.0;
   oldstoreSoilC=0.0;
   oldstoreCropC=0.0;
   storeCropC=0.0;
   storeLivestockC=-99999.0;
   oldstoreLivestockC=-99999.0;
   storeHouseEtcC=0.0;
   oldstoreHouseEtcC=0.0;
   oldstoredProductC = 0.0;
   lastErrorC=0.0;
   lastErrorSoilC=0.0;
   lastErrorCropC=0.0;
   lastErrorLivestockC=0.0;
   lastErrorHouseEtcC=0.0;

   for (int i=0; i<NumberOfIndicatorTypes; i++)
   	for (int j=0; j<SubIndex;j++)
   		indicatorList[i][j] = new linkList <item>;
   maxErrorN=0.01;
   maxErrorC=0.01;
   makingList=false;

}

/****************************************************************************\
  Destructor
\****************************************************************************/
IndicatorOutput::~IndicatorOutput()
{
   if(fileStreamCSV) delete fileStreamCSV;
	for (int i=0; i<NumberOfIndicatorTypes; i++)
   	for (int j=0; j<SubIndex;j++)
   		if (indicatorList[i][j])
      		delete  indicatorList[i][j];
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::InitializeIndicators()
{
   makingList=true;

	// Headlines:
   AddIndicator(economicIndicator,     "01	","",0);
   AddIndicator(economicIndicator,     "01    -Farm characteristics-","",0);
   AddIndicator(economicIndicator,     "03.00    -Capital-","",0);
   AddIndicator(economicIndicator,     "05.00    -Gross marg�n-","",0);
   AddIndicator(economicIndicator,     "07.00    -Farm profit -","",0);
   AddIndicator(economicIndicator,     "09.00    -Net profit -","",0);
   AddIndicator(economicIndicator,     "11.00    -Store-","",0);
   AddIndicator(economicIndicator,     "13.00    -Labour-","",0);
   AddIndicator(economicIndicator,     "15.00    -Areas-","",0);
   AddIndicator(economicIndicator,     "17.00    -Yields-","",0);
   AddIndicator(economicIndicator,     "19.00    -Livestock production-","",0);
   AddIndicator(economicIndicator,     "20.00    -Energy-","",0);
   AddIndicator(environmentalIndicator,"30.00    -Nitrogen-","",0);
   AddIndicator(environmentalIndicator,"40.00    -Carbon-","",0);
   AddIndicator(environmentalIndicator,"50.00    -Phosporus-","",0);
   AddIndicator(environmentalIndicator,"60.00    -Potassium-","",0);
   AddIndicator(environmentalIndicator,"70.00    -Water-","",0);
   AddIndicator(environmentalIndicator,"80.00    -Pesticides-","",0);

    // The code below ensures equal amount of rows for each simulated year
	AddIndicator(economicIndicator,     "01.01 Total area","ha",0);
   AddIndicator(economicIndicator,     "01.02 DE (old)","no",0);
   AddIndicator(economicIndicator,     "01.03 DE/ha (old)","no/ha",0);
   AddIndicator(economicIndicator,     "01.04 DE (new)","no",0);
   AddIndicator(economicIndicator,     "01.05 JB 1-4","ha",0);
   AddIndicator(economicIndicator,     "01.06 JB 5-7","ha",0);

   // Capital
   AddIndicator(economicIndicator,     "03.01 Value of arable land","Dkr",0);
	AddIndicator(economicIndicator,     "03.02 Value of buildings & inventory","Dkr",0);
   AddIndicator(economicIndicator,     "03.03 Value of machinery","Dkr",0);
   AddIndicator(economicIndicator,     "03.05 Value of stored products","Dkr",0);
	AddIndicator(economicIndicator,     "03.06 Value of livestock","Dkr",0);
	AddIndicator(economicIndicator,     "03.07 Assets","Dkr",0);
	AddIndicator(economicIndicator,     "03.50 Value young cattle","Dkr",0);
  	AddIndicator(economicIndicator,     "03.51 Value mature cattle","Dkr",0);

   // Gross margin
	AddIndicator(economicIndicator,     "05.01 Gross margin","Dkr",0);
   AddIndicator(economicIndicator,     "05.10 Plant products sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.11 W.wheat sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.12 S.wheat sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.13 W.rape sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.14 S.rape sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.15 W.barley sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.16 S.barley sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.17 Rye sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.18 Oat sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.19 Pea sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.20 Potato sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.21 Grass for seed sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.22 Maize sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.23 Beet sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.24 Straw sold","DKr",0);
	AddIndicator(economicIndicator,     "05.25 Roughage sold","DKr",0);

   AddIndicator(economicIndicator,     "05.30 Sows sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.31 Baconers sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.32 Piglets sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.33 Cast sows sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.34 Weaners sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.50 Young 1st year heifers sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.51 Young 2nd year heifers sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.52 Dairy cattle sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.53 Beef cattle calves sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.54 Mature beef cattle sold","Dkr",0);
	AddIndicator(economicIndicator,     "05.55 Milk sold","Dkr",0);


	AddIndicator(economicIndicator,     "06.10 Plant production misc. costs","Dkr",0);
   AddIndicator(economicIndicator,     "06.11 Costs seed","Dkr",0);
	AddIndicator(economicIndicator,     "06.12 Costs mineral fertilizer","Dkr",0);
	AddIndicator(economicIndicator,     "06.13 Costs chemicals","Dkr",0);
	AddIndicator(economicIndicator,     "06.14 Costs fuel","Dkr",0);
	AddIndicator(economicIndicator,     "06.15 Costs electricity","Dkr",0);
   AddIndicator(economicIndicator,     "06.16 Costs imported manure","Dkr",0);

	AddIndicator(economicIndicator,     "06.28 Weaners bought","Dkr",0);
	AddIndicator(economicIndicator,     "06.29 Piglets bought","Dkr",0);
	AddIndicator(economicIndicator,     "06.30 Baconers bought","Dkr",0);
	AddIndicator(economicIndicator,     "06.31 Sows bought","Dkr",0);
   AddIndicator(economicIndicator,     "06.32 Costs feed and bedding","Dkr",0);
	AddIndicator(economicIndicator,     "06.33 Cost of vet and production control","Dkr",0);
	AddIndicator(economicIndicator,     "06.50 Young 1st year heifers bought","Dkr",0);
	AddIndicator(economicIndicator,     "06.51 Young 2nd year heifers bought","Dkr",0);
	AddIndicator(economicIndicator,     "06.52 Dairy cattle bought","Dkr",0);
	AddIndicator(economicIndicator,     "06.54 Beef cattle calves bought","Dkr",0);
	AddIndicator(economicIndicator,     "06.55 Mature beef cattle bought","Dkr",0);


	AddIndicator(economicIndicator,     "06.70 Costs hired hours","Dkr",0);
;

	// Farm profit
	AddIndicator(economicIndicator,     "07.01 Net profit","Dkr",0);
	AddIndicator(economicIndicator,     "07.02 Depreciation buildings & inventory","Dkr",0);
	AddIndicator(economicIndicator,     "07.03 Depreciation machinery","Dkr",0);
	AddIndicator(economicIndicator,     "07.04 Insurance buildings & inventory","Dkr",0);
	AddIndicator(economicIndicator,     "07.05 Insurance machinery","Dkr",0);
	AddIndicator(economicIndicator,     "07.06 Maintenance buildings & inventory","Dkr",0);
	AddIndicator(economicIndicator,     "07.07 Maintenance machinery","Dkr",0);
	AddIndicator(economicIndicator,     "07.08 Cost of machine contractors","Dkr",0);

   // Net profit
	AddIndicator(economicIndicator,     "09.01 Total interest","Dkr",0);
	AddIndicator(economicIndicator,     "09.02 Total salery","Dkr",0);
	AddIndicator(economicIndicator,     "09.10 Total savings","Dkr",0);
   AddIndicator(economicIndicator,     "09.11 Earnings per hour","Dkr",0);
   AddIndicator(economicIndicator,     "09.12 Return on capital","\%",0);

   // Store
	AddIndicator(economicIndicator,     "11.10 W.wheat store","t",0);
	AddIndicator(economicIndicator,     "11.11 S.wheat store","t",0);
	AddIndicator(economicIndicator,     "11.12 W.rape store","t",0);
	AddIndicator(economicIndicator,     "11.13 S.rape store","t",0);
	AddIndicator(economicIndicator,     "11.14 W.barley store","t",0);
	AddIndicator(economicIndicator,     "11.15 S.barley store","t",0);
	AddIndicator(economicIndicator,     "11.16 Rye store","t",0);
	AddIndicator(economicIndicator,     "11.17 Oat store","t",0);
	AddIndicator(economicIndicator,     "11.18 Pea store","t",0);
	AddIndicator(economicIndicator,     "11.19 Potato store","t",0);
	AddIndicator(economicIndicator,     "11.20 Grass for seed store","t",0);
	AddIndicator(economicIndicator,     "11.21 Maize store","t",0);
	AddIndicator(economicIndicator,     "11.22 Beet store","t",0);
	AddIndicator(economicIndicator,     "11.30 Sows present","no",0);
	AddIndicator(economicIndicator,     "11.31 Baconers present","no",0);
	AddIndicator(economicIndicator,     "11.32 Piglets present","no",0);
	AddIndicator(economicIndicator,     "11.33 New sows present","no",0);
	AddIndicator(economicIndicator,     "11.50 Young cattle present","no",0);
	AddIndicator(economicIndicator,     "11.51 Mature cattle present","no",0);
	AddIndicator(economicIndicator,     "11.70 Slurry in slurrystore","t",0);
	AddIndicator(economicIndicator,     "11.71 Dung in dungheap","t",0);

   // Labour
	AddIndicator(economicIndicator,     "13.01 Total labour","hours",0);
	AddIndicator(economicIndicator,     "13.10 Labour, field","hours",0);
   AddIndicator(economicIndicator,     "13.11 Field-labour 0","hours",0);
   AddIndicator(economicIndicator,     "13.12 Field-labour 1","hours",0);
   AddIndicator(economicIndicator,     "13.13 Field-labour 2","hours",0);
   AddIndicator(economicIndicator,     "13.14 Field-labour 3","hours",0);
   AddIndicator(economicIndicator,     "13.15 Field-labour 4","hours",0);
   AddIndicator(economicIndicator,     "13.16 Field-labour 5","hours",0);
   AddIndicator(economicIndicator,     "13.17 Field-labour 6","hours",0);
   AddIndicator(economicIndicator,     "13.18 Field-labour 7","hours",0);
   AddIndicator(economicIndicator,     "13.19 Field-labour 8","hours",0);
   AddIndicator(economicIndicator,     "13.20 Field-labour 9","hours",0);
   AddIndicator(economicIndicator,     "13.21 Field-labour 10","hours",0);
   AddIndicator(economicIndicator,     "13.22 Field-labour 11","hours",0);
	AddIndicator(economicIndicator,     "13.30 Labour, stable","hours",0);
	AddIndicator(economicIndicator,     "13.50 Labour, maintance and management","hours",0);

   // Areas
	AddIndicator(economicIndicator,     "15.10 W.wheat area","ha",0);
	AddIndicator(economicIndicator,     "15.11 S.wheat area","ha",0);
	AddIndicator(economicIndicator,     "15.12 W.rape area","ha",0);
	AddIndicator(economicIndicator,     "15.13 S.rape area","ha",0);
	AddIndicator(economicIndicator,     "15.14 W.barley area","ha",0);
	AddIndicator(economicIndicator,     "15.15 S.barley area","ha",0);
	AddIndicator(economicIndicator,     "15.16 Rye area","ha",0);
	AddIndicator(economicIndicator,     "15.17 Oat area","ha",0);
	AddIndicator(economicIndicator,     "15.18 Pea area","ha",0);
	AddIndicator(economicIndicator,     "15.19 Potato area","ha",0);
	AddIndicator(economicIndicator,     "15.20 Grass for seed area","ha",0);
	AddIndicator(economicIndicator,     "15.21 Maize area","ha",0);
	AddIndicator(economicIndicator,     "15.22 Beet area","ha",0);
	AddIndicator(economicIndicator,     "15.23 Barley/Pea area","ha",0);
	AddIndicator(economicIndicator,     "15.24 Grass ley area","ha",0);
	AddIndicator(economicIndicator,     "15.25 Grass/clover ley area","ha",0);
	AddIndicator(economicIndicator,     "15.26 Fallow area","ha",0);

   AddIndicator(economicIndicator,     "16.10 W.wheat area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.11 S.wheat area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.12 W.rape area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.13 S.rape area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.14 W.barley area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.15 S.barley area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.16 Rye area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.17 Oat area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.18 Pea area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.19 Potato area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.20 Grass for seed area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.21 Maize area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.22 Beet area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.23 Barley/Pea area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.24 Grass ley area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.25 Grass/clover ley area (harv. bef. 1/8)","ha",0);
	AddIndicator(economicIndicator,     "16.26 Fallow area (harv. bef. 1/8)","ha",0);

   // Yields
	AddIndicator(economicIndicator,     "17.01 Gross output of plant production","Dkr",0);
   AddIndicator(economicIndicator,     "17.02 Grain yield at economic indicator date","t",0);
   AddIndicator(economicIndicator,     "17.03 Straw yield at economic indicator date","t",0);
AddIndicator(economicIndicator,     "17.04 Maximum root depth","m",0);
	AddIndicator(economicIndicator,     "17.10 W.wheat yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.11 S.wheat yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.12 W.rape yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.13 S.rape yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.14 W.barley yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.15 S.barley yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.16 Rye yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.17 Oat yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.18 Pea yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.19 Potato yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.20 Grass for seed yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.21 Grain maize yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.22 Beet yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.23 Ryegrass yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.24 Clover yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.25 Fallow yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.26 Maize wholecrop yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.27 Italian ryegrass yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.28 Soybean yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.29 Sunflower yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.30 Wheat silage yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "17.31 Lucerne yield (harv. bef. 1/8)","t",0);

   AddIndicator(economicIndicator,     "18.10 Exp. W.wheat yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.11 Exp. S.wheat yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.12 Exp. W.rape yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.13 Exp. S.rape yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.14 Exp. W.barley yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.15 Exp. S.barley yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.16 Exp. Rye yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.17 Exp. Oat yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.18 Exp. Pea yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.19 Exp. Potato yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.20 Exp. Grass for seed yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.21 Exp. Maize yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.22 Exp. Beet yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.23 Exp. Barley/Pea yield (harv. bef. 1/8)","t",0);
	AddIndicator(economicIndicator,     "18.24 Exp. Grass/clover yield (harv. bef. 1/8)","t",0);
   AddIndicator(economicIndicator,     "18.25 Exp. Grass ley yield (harv. bef. 1/8)","ha",0);

   // Livestock production
   AddIndicator(economicIndicator,     "19.01 Bought sows","no",0);
   AddIndicator(economicIndicator,     "19.02 Bought weaners","no",0);
   AddIndicator(economicIndicator,     "19.03 Bought piglets","no",0);
   AddIndicator(economicIndicator,     "19.04 Bought baconers","no",0);
   AddIndicator(economicIndicator,     "19.05 Sold weaners","no",0);
	AddIndicator(economicIndicator,     "19.06 Sold piglets","no",0);
   AddIndicator(economicIndicator,     "19.07 Sold baconers","no",0);
   AddIndicator(economicIndicator,     "19.08 Sold new sows","no",0);
	AddIndicator(economicIndicator,     "19.09 Sold cast sows","no",0);
	AddIndicator(economicIndicator,     "19.10 Feed for sows","t",0);
	AddIndicator(economicIndicator,     "19.11 Feed for baconers","t",0);
	AddIndicator(economicIndicator,     "19.12 Feed for piglets","t",0);
	AddIndicator(economicIndicator,     "19.30 Bought 1st year heifers","no",0);
	AddIndicator(economicIndicator,     "19.31 Bought 2nd year heifers","no",0);
	AddIndicator(economicIndicator,     "19.32 Bought dairy cattle","no",0);
	AddIndicator(economicIndicator,     "19.33 Bought beef calves","no",0);
	AddIndicator(economicIndicator,     "19.34 Bought beef cattle","no",0);
	AddIndicator(economicIndicator,     "19.35 Sold 1st year heifers","no",0);
	AddIndicator(economicIndicator,     "19.36 Sold 2nd year heifers","no",0);
	AddIndicator(economicIndicator,     "19.37 Sold dairy cattle","no",0);
	AddIndicator(economicIndicator,     "19.38 Sold beef calves","no",0);
	AddIndicator(economicIndicator,     "19.39 Sold beef cattle","no",0);
	AddIndicator(economicIndicator,     "19.40 Milk sold","litres",0);
	AddIndicator(economicIndicator,     "19.50 Imported manure","t",0);
	AddIndicator(economicIndicator,     "19.51 Imported cereals","t",0);
	AddIndicator(economicIndicator,     "19.52 Imported roughage","t",0);
	AddIndicator(economicIndicator,     "19.60 Manure from sows","t",0);
	AddIndicator(economicIndicator,     "19.61 Manure from baconers","t",0);
	AddIndicator(economicIndicator,     "19.62 Manure from piglets","t",0);
	AddIndicator(economicIndicator,     "19.63 Manure produced by young cattle","t",0);
	AddIndicator(economicIndicator,     "19.64 Manure produced by dairy cattle","t",0);
	AddIndicator(economicIndicator,     "19.65 Manure spread on field","t",0);
   AddIndicator(economicIndicator,     "19.70 Exported roughage","t",0);
	AddIndicator(economicIndicator,     "19.71 Exported plant products","t",0);
	AddIndicator(economicIndicator,     "19.72 Slurry produced","t",0);
	AddIndicator(economicIndicator,     "19.73 Solid manure produced","t",0);

   // Energy
	AddIndicator(economicIndicator,     "20.01 Diesel consumption","l",0);
	AddIndicator(economicIndicator,     "20.02 Electricity consumption","KWh",0);
   AddIndicator(economicIndicator,     "20.03 Diesel consumption by contractor","l",0);

   // Nitrogen - field .0X input .2X output .4X loss .6X status
	AddIndicator(environmentalIndicator,"31.01 N from mineral fertilizer","kg N",0);
	AddIndicator(environmentalIndicator,"31.02 N from deposition","kg N",0);
	AddIndicator(environmentalIndicator,"31.03 N from seed","kg N",0);
	AddIndicator(environmentalIndicator,"31.04 N from fixation","kg N",0);
	AddIndicator(environmentalIndicator,"31.05 N from irrigation water","kg N",0);
	AddIndicator(environmentalIndicator,"31.06 N from spread manure","kg N",0);
   AddIndicator(environmentalIndicator,"31.07 N in urine from grazing","kg N",0);
	AddIndicator(environmentalIndicator,"31.08 N in faeces from grazing","kg N",0);
   AddIndicator(environmentalIndicator,"31.20 N removed in grain","kg N",0);
	AddIndicator(environmentalIndicator,"31.21 N removed in straw","kg N",0);
	AddIndicator(environmentalIndicator,"31.22 N removed in cut","kg N",0);
	AddIndicator(environmentalIndicator,"31.23 N removed by cattle","kg N",0);
	AddIndicator(environmentalIndicator,"31.40 Nitrate N leached","kg N",0);
	AddIndicator(environmentalIndicator,"31.41 Ammonium N leached","kg N",0);
	AddIndicator(environmentalIndicator,"31.42 N2 from denitrification","kg N",0);
	AddIndicator(environmentalIndicator,"31.43 N2O from denitrification","kg N",0);
   AddIndicator(environmentalIndicator,"31.44 N2O from nitrification","kg N",0);
	AddIndicator(environmentalIndicator,"31.45 NH3-N loss by spreading","kg N",0);
	AddIndicator(environmentalIndicator,"31.46 NH3-N loss during grazing","kg N",0);
	AddIndicator(environmentalIndicator,"31.47 N in surface run-off","kg N",0);
	AddIndicator(environmentalIndicator,"31.48 NH3-N loss from mineral fertiliser","kg N",0);
	AddIndicator(environmentalIndicator,"31.60 N in standing crop/straw","kg N",0);
	AddIndicator(environmentalIndicator,"31.61 Total soil/soilsurface N","kg N",0);
	AddIndicator(environmentalIndicator,"31.99 Total soil/soilsurface nitrogen to 1 m depth","kg N",0);     //present in patch.cpp - still used?? NJH Nov 2007


   AddIndicator(environmentalIndicator,"32.10 N uptake from soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.20 N from roots incorporated into soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.21 N from straw incorporated into soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.22 N from leaf litterfall incorporated into soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.23 N from storage incorporated into soil","kg N",0);
//   AddIndicator(environmentalIndicator,"32.23 Plant top res. N incorp. in org. matt.","kg N",0);
   AddIndicator(environmentalIndicator,"32.30 Gross N-mineralisation from soil org. matter","kg N",0);
	AddIndicator(environmentalIndicator,"32.40 Nitrate N leached from leaching depth","kg N",0);
	AddIndicator(environmentalIndicator,"32.41 Ammonium N leached from leaching depth","kg N",0);
	AddIndicator(environmentalIndicator,"32.60 Organic N in soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.61 Mineral N in soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.62 AOM1 N in soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.63 AOM2 N in soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.64 SMB1 N in soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.65 SMB2 N in soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.66 SMR N in soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.67 NOM N in soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.68 IOM N in soil","kg N",0);
	AddIndicator(environmentalIndicator,"32.69 Organic N in soil 0-25 cm","kg N",0);
	AddIndicator(environmentalIndicator,"32.70 Mineral N in soil 0-25 cm","kg N",0);
	AddIndicator(environmentalIndicator,"32.71 Organic N in soil 25-50 cm","kg N",0);
	AddIndicator(environmentalIndicator,"32.72 Mineral N in soil 25-50 cm","kg N",0);
	AddIndicator(environmentalIndicator,"32.73 Organic N in soil 50-75 cm","kg N",0);
	AddIndicator(environmentalIndicator,"32.74 Mineral N in soil 50-75 cm","kg N",0);
	AddIndicator(environmentalIndicator,"32.75 Organic N in soil 75-250 cm","kg N",0);
	AddIndicator(environmentalIndicator,"32.76 Mineral N in soil 75-250 cm","kg N",0);
	AddIndicator(environmentalIndicator,"32.77 N in AOM+SMB 0-25 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.78 N in AOM+SMB 25-50 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.79 N in AOM+SMB 50-75 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.80 N in AOM+SMB 75-250 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.81 N in NOM 0-25 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.82 N in NOM 25-50 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.90 N in NOM 50-75 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.91 N in NOM 75-250 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.92 N in IOM 0-25 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.93 N in IOM 25-50 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.94 N in IOM 50-75 cm","kg N",0);
		AddIndicator(environmentalIndicator,"32.95 N in IOM 75-250 cm","kg N",0);

   AddIndicator(environmentalIndicator,"32.83 Nmin 0-500","kg N",0);
	AddIndicator(environmentalIndicator,"32.84 Nmin 0-750","kg N",0);
	AddIndicator(environmentalIndicator,"32.85 Nmin 0-1000","kg N",0);

   // Nitrogen in livestock
   AddIndicator(environmentalIndicator,"33.01 N in feed for young cattle","kg N",0);
   AddIndicator(environmentalIndicator,"33.02 N in feed for adult cattle","kg N",0);
	AddIndicator(environmentalIndicator,"33.03 N in bought cattle","kg N",0);
	AddIndicator(environmentalIndicator,"33.04 N grazed by cattle","kg N",0);
   AddIndicator(environmentalIndicator,"33.20 N in milk","kg N",0);
   AddIndicator(environmentalIndicator,"33.21 N in sold cattle","kg N",0);
   AddIndicator(environmentalIndicator,"33.40 N in dairy cattle waste","kg N",0);
   AddIndicator(environmentalIndicator,"33.41 N in young cattle manure","kg N",0);
   AddIndicator(environmentalIndicator,"33.42 N in dairy cattle manure","kg N",0);
	AddIndicator(environmentalIndicator,"33.60 N bound in cattle","kg N",0);
   AddIndicator(environmentalIndicator,"33.80 Cattle manure N to pasture","kg N",0);
   AddIndicator(environmentalIndicator,"33.81 Cattle manure N to animal housing","kg N",0);

	AddIndicator(environmentalIndicator,"34.01 N in feed for piglets","kg N",0);
   AddIndicator(environmentalIndicator,"34.02 N in feed for sows","kg N",0);
	AddIndicator(environmentalIndicator,"34.03 N in feed for baconers","kg N",0);
	AddIndicator(environmentalIndicator,"34.04 N in bought pigs","kg N",0);
   AddIndicator(environmentalIndicator,"34.20 N in sold pigs","kg N",0);
	AddIndicator(environmentalIndicator,"34.40 N in sows manure","kg N",0);
	AddIndicator(environmentalIndicator,"34.41 N in baconers manure","kg N",0);
	AddIndicator(environmentalIndicator,"34.42 N in piglets manure","kg N",0);
	AddIndicator(environmentalIndicator,"34.60 N bound in pigs","kg N",0);

	// Nitrogen in storage/stable
 	AddIndicator(environmentalIndicator,"35.01 N from imported feed","kg N",0);
	AddIndicator(environmentalIndicator,"35.02 N from imported manure","kg N",0);
	AddIndicator(environmentalIndicator,"35.03 N from imported bedding","kg N",0);
   AddIndicator(environmentalIndicator,"35.04 N from harvest to store","kg N",0);
   AddIndicator(environmentalIndicator,"35.20 N in sold plant products","kg N",0);
   AddIndicator(environmentalIndicator,"35.21 N removed from manure storage","kg N",0);
   AddIndicator(environmentalIndicator,"35.22 N in exported manure","kg N",0);
	AddIndicator(environmentalIndicator,"35.40 NH3-N volatilisation from animal house","kg N",0);
	AddIndicator(environmentalIndicator,"35.41 N2-N volatilisation from animal house","kg N",0);
	AddIndicator(environmentalIndicator,"35.42 N2O-N volatilisation from animal house","kg N",0);
	AddIndicator(environmentalIndicator,"35.43 NH3-N volatilisation from manure storage","kg N",0);
	AddIndicator(environmentalIndicator,"35.44 N2-N volatilisation from manure storage","kg N",0);
	AddIndicator(environmentalIndicator,"35.45 N2O-N volatilisation from manure storage","kg N",0);
	AddIndicator(environmentalIndicator,"35.60 N bound in manure storage","kg N",0);
	AddIndicator(environmentalIndicator,"35.61 N bound in stable manure","kg N",0);
   AddIndicator(environmentalIndicator,"35.62 N in stored plant products","kg N",0);
  	AddIndicator(environmentalIndicator,"35.63 N in bedding","kg N",0);
	AddIndicator(environmentalIndicator,"35.80 N transf. from stable to store","kg N",0);
   AddIndicator(environmentalIndicator,"35.81 N removed from stored plant products","kg N",0);
   AddIndicator(environmentalIndicator,"35.82 N removed from stored feed products","kg N",0);

   // Nitrogen in yields
   AddIndicator(environmentalIndicator,"36.00 W.wheat yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.01 S.wheat yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.02 W.rape yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.03 S.rape yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.04 W.barley yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.05 S.barley yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.06 Rye yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.07 Oat yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.08 Pea yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.09 Potato yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.10 Grass for seed yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.11 Grain maize yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.12 Beet yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.13 Ryegrass yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.14 Clover yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.15 Fallow yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.16 Maize wholecrop yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.17 Italian ryegrass yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.18 Soybean yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.19 Sunflower yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.20 Wheat silage yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(environmentalIndicator,"36.21 Lucerne yield (harv. bef. 1/8)","kg N",0);

   // Nitrogen adjustment by area alterations
   AddIndicator(environmentalIndicator,"37.00 N difference from area adjustment","kg N",0);
   AddIndicator(environmentalIndicator,"37.01 N crop difference from area adjustment","kg N",0);
   AddIndicator(environmentalIndicator,"37.02 N soil difference from area adjustment","kg N",0);

   AddIndicator(environmentalIndicator,"38.10 N15 harvest in grain","kg N",0);
	AddIndicator(environmentalIndicator,"38.11 Total N15 harvest","kg N",0);
   AddIndicator(environmentalIndicator,"38.12 N15 leached from leaching depth","kg N",0);
	AddIndicator(environmentalIndicator,"38.21 N15-N2O from denitrification","kg N",0);
   AddIndicator(environmentalIndicator,"38.22 N15-N2O from nitrification","kg N",0);
   AddIndicator(environmentalIndicator,"38.23 N15-Denitrification","kg N",0);
 	AddIndicator(environmentalIndicator,"38.24 Total soil/soilsurface 15N","kg N",0);

   AddIndicator(environmentalIndicator,"39.01 Catch crop DM (1/9)","kg DM",0);
	AddIndicator(environmentalIndicator,"39.03 Catch crop N (1/9)","kg N",0);
	AddIndicator(environmentalIndicator,"39.02 Catch crop DM (1/11)","kg DM",0);
	AddIndicator(environmentalIndicator,"39.04 Catch crop N (1/11)","kg N",0);
	AddIndicator(environmentalIndicator,"39.10 Grain yield at environmental indicator date","t",0);
	AddIndicator(environmentalIndicator,"39.11 Straw yield at environmental indicator date","t",0);
	AddIndicator(environmentalIndicator,"39.12 Cut at environmental indicator date","t",0);
	AddIndicator(environmentalIndicator,	"39.20 Date of emergence of main crop","day of year",0);
		AddIndicator(environmentalIndicator,	"39.21 Date of flowering of main crop","day of year",0);
		AddIndicator(environmentalIndicator,	"39.22 Date of end of grain filling of main crop","day of year",0);
		AddIndicator(environmentalIndicator,	"39.23 Date of ripeness of main crop","day of year",0);

   // Carbon
	AddIndicator(environmentalIndicator,"40.01 Net crop carbon fixation","kg C",0);
	AddIndicator(environmentalIndicator,"40.02 C from field-applied manure","kg C",0);
	AddIndicator(environmentalIndicator,"40.03 C from seed","kg C",0);
   AddIndicator(environmentalIndicator,"40.20 C removed in grain","kg C",0);
	AddIndicator(environmentalIndicator,"40.21 C removed in straw","kg C",0);
	AddIndicator(environmentalIndicator,"40.22 C removed in cut","kg C",0);
	AddIndicator(environmentalIndicator,"40.23 C removed by cattle","kg C",0);
	AddIndicator(environmentalIndicator,"40.40 CO2 soil respiration","kg C",0);
	AddIndicator(environmentalIndicator,"40.60 Total C in soil","kg C",0);
	AddIndicator(environmentalIndicator,"40.61 C in standing crop/straw","kg C",0);
	AddIndicator(environmentalIndicator,"40.62 Total C in soil 0-25 cm","kg C",0);
	AddIndicator(environmentalIndicator,"40.63 Total C in soil 25-50 cm","kg C",0);
	AddIndicator(environmentalIndicator,"40.64 Total C in soil 50-75 cm","kg C",0);
	AddIndicator(environmentalIndicator,"40.65 Total C in soil 75-250 cm","kg C",0);
	AddIndicator(environmentalIndicator,"40.66 C in AOM+SMB 0-25 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.67 C in AOM+SMB 25-50 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.68 C in AOM+SMB 50-75 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.69 C in AOM+SMB 75-250 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.70 C in NOM 0-25 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.71 C in NOM 25-50 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.72 C in NOM 50-75 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.73 C in NOM 75-250 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.74 C in IOM 0-25 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.75 C in IOM 25-50 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.76 C in IOM 50-75 cm","kg C",0);
		AddIndicator(environmentalIndicator,"40.77 C in IOM 75-250 cm","kg C",0);
	AddIndicator(environmentalIndicator,"40.80 C from roots incorporated into soil","kg C",0);
	AddIndicator(environmentalIndicator,"40.81 C from straw incorporated into soil","kg C",0);
	AddIndicator(environmentalIndicator,"40.82 C from leaf litterfall incorporated into soil","kg C",0);
	AddIndicator(environmentalIndicator,"40.83 C from storage incorporated into soil","kg N",0);
	AddIndicator(environmentalIndicator,"40.90 AOM1 C in soil","kg C",0);
	AddIndicator(environmentalIndicator,"40.91 AOM2 C in soil","kg C",0);
	AddIndicator(environmentalIndicator,"40.92 SMB1 C in soil","kg C",0);
	AddIndicator(environmentalIndicator,"40.93 SMB2 C in soil","kg C",0);
	AddIndicator(environmentalIndicator,"40.94 SMR C in soil","kg C",0);
	AddIndicator(environmentalIndicator,"40.95 NOM C in soil","kg C",0);
	AddIndicator(environmentalIndicator,"40.96 IOM C in soil","kg C",0);

	AddIndicator(environmentalIndicator,"41.01 C input into cattle","kg C",0);
	AddIndicator(environmentalIndicator,"41.02 C in cattle CH4 production","kg C",0);
	AddIndicator(environmentalIndicator,"41.03 C in cattle urine","kg C",0);
	AddIndicator(environmentalIndicator,"41.04 C in cattle faeces","kg C",0);
	AddIndicator(environmentalIndicator,"41.05 C in cattle CO2 production","kg C",0);
	AddIndicator(environmentalIndicator,"41.06 C in milk production","kg C",0);
	AddIndicator(environmentalIndicator,"41.07 C retained in cattle body","kg C",0);
	AddIndicator(environmentalIndicator,"41.08 C in urine from grazing","kg C",0);
	AddIndicator(environmentalIndicator,"41.09 C in faeces from grazing","kg C",0);
   AddIndicator(environmentalIndicator,"41.10 C from grazing animals","kg C",0);

   AddIndicator(environmentalIndicator,"43.01 C in feed for young cattle","kg C",0);
   AddIndicator(environmentalIndicator,"43.02 C in feed for adult cattle","kg C",0);
	AddIndicator(environmentalIndicator,"43.03 C in bought cattle","kg C",0);
	AddIndicator(environmentalIndicator,"43.04 C grazed by cattle","kg C",0);
   AddIndicator(environmentalIndicator,"43.21 C in sold cattle","kg C",0);
   AddIndicator(environmentalIndicator,"43.41 4 in young cattle manure","kg C",0);
   AddIndicator(environmentalIndicator,"43.42 C in dairy cattle manure","kg C",0);
	AddIndicator(environmentalIndicator,"43.60 C bound in cattle","kg C",0);
   AddIndicator(environmentalIndicator,"43.80 Cattle manure C to pasture","kg C",0);
   AddIndicator(environmentalIndicator,"43.81 Cattle manure C to animal housing","kg C",0);

	AddIndicator(environmentalIndicator,"44.01 C in feed for piglets","kg C",0);
   AddIndicator(environmentalIndicator,"44.02 C in feed for sows","kg C",0);
	AddIndicator(environmentalIndicator,"44.03 C in feed for baconers","kg C",0);
	AddIndicator(environmentalIndicator,"44.04 C in bought pigs","kg C",0);
   AddIndicator(environmentalIndicator,"44.20 C in sold pigs","kg C",0);
	AddIndicator(environmentalIndicator,"44.40 C in sows manure","kg C",0);
	AddIndicator(environmentalIndicator,"44.41 C in baconers manure","kg C",0);
	AddIndicator(environmentalIndicator,"44.42 C in piglets manure","kg C",0);
	AddIndicator(environmentalIndicator,"44.60 C bound in pigs","kg C",0);
	AddIndicator(environmentalIndicator,"44.62 C in sold pigs","kg C",0);
	AddIndicator(environmentalIndicator,"44.63 C in pig CO2 production","kg C",0);
	AddIndicator(environmentalIndicator,"44.64 C in pig CH4 production","kg C",0);

	// Carbon in storage/stable
 	AddIndicator(environmentalIndicator,"45.01 C from imported feed","kg C",0);
	AddIndicator(environmentalIndicator,"45.02 C from imported manure","kg C",0);
	AddIndicator(environmentalIndicator,"45.03 C from imported bedding","kg C",0);
   AddIndicator(environmentalIndicator,"45.04 C from harvest to store","kg C",0);
   AddIndicator(environmentalIndicator,"45.20 C in sold plant products","kg C",0);
   AddIndicator(environmentalIndicator,"45.21 C removed from manure storage","kg C",0);
   AddIndicator(environmentalIndicator,"45.22 C in exported manure","kg C",0);
	AddIndicator(environmentalIndicator,"45.40 CO2-C volatilisation from animal house","kg C",0);
	AddIndicator(environmentalIndicator,"45.41 CH4-C volatilisation from animal house","kg C",0);
	AddIndicator(environmentalIndicator,"45.43 CO2-C volatilisation from manure storage","kg C",0);
	AddIndicator(environmentalIndicator,"45.44 CH4-C volatilisation from manure storage","kg C",0);
	AddIndicator(environmentalIndicator,"45.60 C bound in manure storage","kg C",0);
	AddIndicator(environmentalIndicator,"45.61 C bound in stable manure","kg C",0);
   AddIndicator(environmentalIndicator,"45.62 C in stored plant products","kg C",0);
  	AddIndicator(environmentalIndicator,"45.63 C in bedding","kg C",0);
	AddIndicator(environmentalIndicator,"45.80 C transf. from stable to store","kg C",0);
   AddIndicator(environmentalIndicator,"45.81 C removed from stored plant products","kg C",0);
   AddIndicator(environmentalIndicator,"45.82 C removed from stored feed products","kg C",0);


   // Phosphorus
	AddIndicator(environmentalIndicator,"50.01 Phosphorus from mineral fertilizer","kg P",0);
   AddIndicator(environmentalIndicator,"50.02 feed-P for sows","kg P",0);
	AddIndicator(environmentalIndicator,"50.03 feed-P for baconers","kg P",0);
	AddIndicator(environmentalIndicator,"50.04 feed-P for piglets","kg P",0);
	AddIndicator(environmentalIndicator,"50.10 manure-P from sows","kg P",0);
	AddIndicator(environmentalIndicator,"50.11 manure-P from baconers","kg P",0);
	AddIndicator(environmentalIndicator,"50.12 manure-P from piglets","kg P",0);

   // Potassium
	AddIndicator(environmentalIndicator,"60.01 Potassium from mineral fertilizer","kg K",0);
   AddIndicator(environmentalIndicator,"60.02 feed-K for sows","kg K",0);
	AddIndicator(environmentalIndicator,"60.03 feed-K for baconers","kg K",0);
	AddIndicator(environmentalIndicator,"60.04 feed-K for piglets","kg K",0);
	AddIndicator(environmentalIndicator,"60.10 manure-K from sows","kg K",0);
	AddIndicator(environmentalIndicator,"60.11 manure-K from baconers","kg K",0);
	AddIndicator(environmentalIndicator,"60.12 manure-K from piglets","kg K",0);

   // Water
	AddIndicator(environmentalIndicator,"70.01 Rainwater","cubic meter",0);
	AddIndicator(environmentalIndicator,"70.02 Irrigation","cubic meter",0);
	AddIndicator(environmentalIndicator,"70.10 Percolation","cubic meter",0);
	AddIndicator(environmentalIndicator,"70.11 Surface run-off","cubic meter",0);
	AddIndicator(environmentalIndicator,"70.12 Evapotranspiration","cubic meter",0);
  	AddIndicator(environmentalIndicator,"70.15 Water in field to balancedepth","cubic meter",0);  //check!
  	AddIndicator(environmentalIndicator,"70.20 Water in soil","cubic meter",0);
	AddIndicator(environmentalIndicator,"70.40 Percolation from leaching depth","cubic meter",0);
	AddIndicator(environmentalIndicator,"70.41 Water in field to leaching depth","cubic meter",0);
	AddIndicator(environmentalIndicator,"70.42 Bare soil evaporation","cubic meter",0);
	AddIndicator(environmentalIndicator,"70.50 Precipitation","mm",0);

   // Pesticides
	AddIndicator(environmentalIndicator,"80.01 Herbicide consumption","TFI",0);
	AddIndicator(environmentalIndicator,"80.02 Fungicide consumption","TFI",0);
	AddIndicator(environmentalIndicator,"80.03 Insecticide consumption","TFI",0);


   // REMOVE LATER !!!!!!!!!!!!!!!!!
   AddIndicator(environmentalIndicator,"98.90 Total carbon in soil","kg C",0);
   AddIndicator(environmentalIndicator,"98.91 current year"," ",0);
   AddIndicator(environmentalIndicator,"98.92 N2O from denitrification","kg N",0);
   AddIndicator(environmentalIndicator,"98.93 N2O from nitrification","kg N",0);
   AddIndicator(environmentalIndicator,"99.90 Total carbon in soil","kg C",0);
   AddIndicator(environmentalIndicator,"99.91 Nitrate N leached","kg N",0);
   AddIndicator(environmentalIndicator,"99.92 N2O from denitrification","kg N",0);
   AddIndicator(environmentalIndicator,"99.93 N2O from nitrification","kg N",0);
   AddIndicator(environmentalIndicator,"98.95 Number of patches"," ",0);

   makingList=false;
}

/****************************************************************************\
\****************************************************************************/
double IndicatorOutput::GetAmount(int indicatorType, string aName)
{

   strcpy(character,aName.c_str());
   char * tmp=new char[aName.size()+1];
   strcpy(tmp,aName.c_str());
   linkList<item>::PS P;
   indicatorList[indicatorType][FindSubIndex(tmp)]->FirstThat(P,FindString);
   delete [] tmp;
   if (P)
      return P->element->GetAmount();
   else
   {
      theMessage->WarningWithDisplay("output::GetAmount - '",aName,"' not found.");
      return 0.0;
   }

}

/****************************************************************************\
This rather strange function divides the indicators into subgroups, depending on how
often they are called, so that searching through linklist is speeded up!
\****************************************************************************/
int IndicatorOutput::FindSubIndex(const char * aName)
{
	   int indexNumber1=(aName[0]-48)*10;
	   indexNumber1+=(aName[1]-48);
	   int indexNumber2=0;
	   if(aName[2]=='.')
	   {
		   indexNumber2=(aName[3]-48)*10;
		   indexNumber2+=(aName[4]-48);
	   }
   if (indexNumber1<20) return 0;                                            // range 0
   if (indexNumber1==20) return 1+div(indexNumber2,10).quot;                 // range 1-10
   if ((indexNumber1>20) && (indexNumber1<=25)) return (indexNumber1-20)+10; // range 11-15
   return 16;                                                                // range 16
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::AddStateIndicator(int indicatorType, string aName, string aUnit, double aAmount)
{

   strcpy(character,aName.c_str());
   char * CharaName=new char[aName.size()+1];
   strcpy(CharaName,aName.c_str());
   char * CharaUnit=new char[aUnit.size()+1];
   strcpy(CharaUnit,aUnit.c_str());
   linkList<item>::PS P,P1;
   linkList <item>* list = FindList(indicatorType,CharaName);
   list->FirstThat(P,FindString);
   if (P!=NULL)
   {
      P->element->AddAmount(aAmount);
      P->element->SetDailyAddIndicator(true);
   }
   else
   {

      if (!makingList)
      {
      	theMessage->WarningWithDisplay("Indicator - ",CharaName);
         theMessage->WarningWithDisplay("IndicatorOutput::AddIndicator - no new indicators should be added at this point");
      }

      item * Pi=new item(CharaName,CharaUnit,aAmount,true);
      list->FirstThat(P,BiggerThan);
      if (P!=NULL)
         list->InsertBefore(P,P1,Pi);
      else
         list->InsertLast(Pi);
   }

	delete [] CharaName;
	delete [] CharaUnit;
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::AddStateIndicator(int indicatorType, string aName, string aUnit, nitrogen aAmount)
{
	AddStateIndicator(indicatorType,aName,aUnit,aAmount.n);
}

/****************************************************************************\
\****************************************************************************/
linkList <item>* IndicatorOutput::FindList(int indicatorType, char* aName)
{
	return indicatorList[indicatorType][FindSubIndex(aName)];
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::Initialize(char * filenameCSV)
{
   InitializeIndicators();
	fileStreamCSV = new fstream;

 	fileStreamCSV->open(filenameCSV,ios::out);
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::OutCSV(item * aItem)
{
  aItem->Out(*fileStreamCSV,*aItem,yearNumber,"\t","");          // tabulated to ease handling by excel
}

/****************************************************************************\
\****************************************************************************/
int IndicatorOutput::FindString(item * aItem)
{
   if (!aItem) return 0;
   return (strcmp(aItem->GetName(),character)==0);
}

/****************************************************************************\
\****************************************************************************/
int IndicatorOutput::BiggerThan(item * aItem)
{
#ifndef __BCplusplus__
	return(strcmp(aItem->GetName(),character)>0);
#else
	return(stricmp(aItem->GetName(),character)>0);
#endif
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::AddIndicator(int indicatorType, string aName, string aUnit, double aAmount)
{

   strcpy(character,aName.c_str());
   char * CharaName=new char[aName.size()+1];
   strcpy(CharaName,aName.c_str());
   char * CharaUnit=new char[aUnit.size()+1];
   strcpy(CharaUnit,aUnit.c_str());
   linkList<item>::PS P,P1;
   linkList <item>* list = FindList(indicatorType,CharaName);
   list->FirstThat(P,FindString);
   if (P!=NULL)
      P->element->AddAmount(aAmount);
   else
   {
   	if (makingList==true)
      {
         item * Pi=new item(CharaName,CharaUnit,aAmount,false);
         list->FirstThat(P,BiggerThan);
         if (P!=NULL)
            list->InsertBefore(P,P1,Pi);
         else
            list->InsertLast(Pi);
      }
      else
			theMessage->FatalError("indicat::AddIndicator - attempting to add an indicator when MakeList is false");
   }

   delete [] CharaName;
   delete [] CharaUnit;
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::AddIndicator(int indicatorType, string aName, string aUnit, nitrogen aAmount)
{
	AddIndicator(indicatorType,aName,aUnit,aAmount.n);
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::FlushIndicator(int indicatorType, int aYearNumber)
{
	yearNumber = aYearNumber;
   string IndicatorString = (indicatorType==economicIndicator)? "economic " : "environmental ";
   for (int j=0; j<SubIndex;j++)
   {
	   indicatorList[indicatorType][j]->ForEach(OutCSV);
	   indicatorList[indicatorType][j]->Reset();
   }
   fileStreamCSV->flush();
   InitializeIndicators();

   if (indicatorType==environmentalIndicator)
   {
      oldstoreW           =-1e11;
      oldstoreN           =-1e11;
      oldstoreSoilN       =-1e11;
      oldstoreCropN       =-1e11;
      oldstoreLivestockN  =-1e11;
      oldstoreHouseEtcN   =-1e11;
      oldstoredProductN   =-1e11;
      oldstoreC           =-1e11;
      oldstoreCropC       =-1e11;
      oldstoreSoilC       =-1e11;
      oldstoreHouseEtcC   =-1e11;
      oldstoredProductC   =-1e11;
      oldstoreLivestockC  =-1e11;


      lastErrorLivestockN =0.0;
      lastErrorSoilN      =0.0;
      lastErrorN          =0.0;
      lastErrorCropN      =0.0;
      lastErrorStoredN    =0.0;
      lastErrorHouseEtcN  =0.0;
      lastErrorStoredN=0.0;
      lastErrorC          =0.0;
      lastErrorSoilC      =0.0;
      lastErrorCropC      =0.0;
      lastErrorLivestockC =0.0;
      lastErrorHouseEtcC  =0.0;

      //cout << endl << GetAmount(environmentalIndicator,"37.00 N difference from area adjustment") << endl;
      AddIndicator(environmentalIndicator,"37.00 N difference from area adjustment","kg N",-1.0*GetAmount(environmentalIndicator,"37.00 N difference from area adjustment"));
      //cout << GetAmount(environmentalIndicator,"37.00 N difference from area adjustment") << endl;
      //cout << GetAmount(environmentalIndicator,"37.01 N crop difference from area adjustment") << endl;
      AddIndicator(environmentalIndicator,"37.01 N crop difference from area adjustment","kg N",-1.0*GetAmount(environmentalIndicator,"37.01 N crop difference from area adjustment"));
      //cout << GetAmount(environmentalIndicator,"37.01 N crop difference from area adjustment") << endl;
      //cout << GetAmount(environmentalIndicator,"37.02 N soil difference from area adjustment") << endl;
      AddIndicator(environmentalIndicator,"37.02 N soil difference from area adjustment","kg N",-1.0*GetAmount(environmentalIndicator,"37.02 N soil difference from area adjustment"));
      //cout << GetAmount(environmentalIndicator,"37.02 N soil difference from area adjustment") << endl;
      //cout << endl;
   }
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::CheckBalance()
{

// ------------------------ Water ----------------------------
   storeW=GetAmount(environmentalIndicator,"70.20 Water in soil");
   if (storeW>1)
   {
      double ppt = GetAmount(environmentalIndicator,"70.01 Rainwater");
      double irrig = GetAmount(environmentalIndicator,"70.02 Irrigation");
      double gainW= ppt + irrig;
      double perc = GetAmount(environmentalIndicator,"70.10 Percolation");
      double runoff = GetAmount(environmentalIndicator,"70.11 Surface run-off");
      double evapotrans =GetAmount(environmentalIndicator,"70.12 Evapotranspiration");
// ???!!!     double bare_evap = GetAmount(environmentalIndicator,"70.42 Bare soil evaporation");
      double lossW= perc + runoff + evapotrans;
      if (oldstoreW<1E-10) // Initialize
         oldstoreW=storeW+lossW-gainW;
//      double balanceW=storeW-oldstoreW+lossW-gainW-lastErrorW;
      /* !!!!!!!!!!!!!!!!!!!!!!!!!!!!
      if (fabs(balanceW)>1)
      {
         theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in water balance",balanceW);
         lastErrorW=balanceW+lastErrorW; // Avoid getting the "same" error more than once
         error=true;
      }
      */
   }
// ------------------------ Nitrogen -------------------------
	//Product store
   double sentProductN    = GetAmount(environmentalIndicator,"35.04 N from harvest to store");
   double storedProductN  = GetAmount(environmentalIndicator,"35.62 N in stored plant products");
   double removedProductN = GetAmount(environmentalIndicator,"35.81 N removed from stored plant products")
   										+ GetAmount(environmentalIndicator,"35.82 N removed from stored feed products");

	// Crop
   double storeCropN = GetAmount(environmentalIndicator,"31.60 N in standing crop/straw");

	// Soil
//	double minSoilN = GetAmount(environmentalIndicator,"32.61 Mineral N in soil");
   storeSoilN      = GetAmount(environmentalIndicator,"31.61 Total soil/soilsurface N");
 //  double orgSoilN = GetAmount(environmentalIndicator,"32.60 Organic N in soil");

	//Manure in housing and storage
   double manurestoreN = GetAmount(environmentalIndicator,"35.60 N bound in manure storage");
	double housingN     = GetAmount(environmentalIndicator,"35.61 N bound in stable manure");
   double storeHouseEtcN = manurestoreN + housingN;

	//Livestock
   double cattleN = GetAmount(environmentalIndicator,"33.60 N bound in cattle");
   double pigN    = GetAmount(environmentalIndicator,"34.60 N bound in pigs");
   storeLivestockN=cattleN + pigN;

	//Total stored in system
   storeN=storeSoilN+storeCropN+storeLivestockN+storeHouseEtcN+storedProductN;

	//Livestock inputs
   double youngCattleFeedN = GetAmount(environmentalIndicator,"33.01 N in feed for young cattle");
   double dairyCattleFeedN = GetAmount(environmentalIndicator,"33.02 N in feed for adult cattle");
  	double boughtCattleN    = GetAmount(environmentalIndicator,"33.03 N in bought cattle");
   double cattleFeedN = youngCattleFeedN + dairyCattleFeedN;
   double cattleGainN = boughtCattleN + cattleFeedN;

   double pigletfeedN = GetAmount(environmentalIndicator,"34.01 N in feed for piglets");
   double sowFeedN    = GetAmount(environmentalIndicator,"34.02 N in feed for sows");
   double baconerfeedN= GetAmount(environmentalIndicator,"34.03 N in feed for baconers");
   double boughtPigN  = GetAmount(environmentalIndicator,"34.04 N in bought pigs");
   double pigFeedN = baconerfeedN + sowFeedN + pigletfeedN;
   double pigGainN = pigFeedN + boughtPigN;

   double boughtLivestockN = boughtCattleN + boughtPigN;
   //double grazedN = GetAmount(environmentalIndicator,"33.04 N grazed by cattle");
   double gainLivestockN = pigGainN + cattleGainN ;//+ grazedN;

	//Livestock outputs
   double soldmilkN          = GetAmount(environmentalIndicator,"33.20 N in milk");
   double soldCattleN        = GetAmount(environmentalIndicator,"33.21 N in sold cattle");
   double dairyCattleWasteN  = GetAmount(environmentalIndicator,"33.40 N in dairy cattle waste"); // Livmoder, evt. d�de kalve, tab fra system
   double youngCattleManureN = GetAmount(environmentalIndicator,"33.41 N in young cattle manure");
   double dairyCattleManureN = GetAmount(environmentalIndicator,"33.42 N in dairy cattle manure");
   double cattleManureN = youngCattleManureN + dairyCattleManureN;
   double cattleLossN = dairyCattleWasteN + cattleManureN + soldmilkN + soldCattleN;

   double soldPigN       = GetAmount(environmentalIndicator,"34.20 N in sold pigs");
   double sowManureN     = GetAmount(environmentalIndicator,"34.40 N in sows manure");
   double baconerManureN = GetAmount(environmentalIndicator,"34.41 N in baconers manure");
   double pigletManureN  = GetAmount(environmentalIndicator,"34.42 N in piglets manure");
   double pigManureN = sowManureN + baconerManureN + pigletManureN;
   double pigLossN   = pigManureN + soldPigN;

   double soldAnimalN = soldCattleN + soldmilkN + soldPigN;
   double lossLivestockN = cattleLossN + pigLossN;

	//Housing inputs
   double cattleNToHousing = GetAmount(environmentalIndicator,"33.81 Cattle manure N to animal housing");
   double beddingN         = GetAmount(environmentalIndicator,"35.63 N in bedding");
   double gainHouseEtcN = pigManureN + cattleNToHousing + beddingN;

	//Housing outputs
   double housevolat      = GetAmount(environmentalIndicator,"35.40 NH3-N volatilisation from animal house");
   double houseN2      = GetAmount(environmentalIndicator,"35.41 N2-N volatilisation from animal house");
   double houseN2O      = GetAmount(environmentalIndicator,"35.42 N2O-N volatilisation from animal house");

	//Housing and storage outputs
   //note that extractedStorageN could include N that is removed directly from animal housing (eg FYM)
   double storevolat        = GetAmount(environmentalIndicator,"35.43 NH3-N volatilisation from manure storage");
   double storeN2        = GetAmount(environmentalIndicator,"35.44 N2-N volatilisation from manure storage");
   double storeN2O        = GetAmount(environmentalIndicator,"35.45 N2O-N volatilisation from manure storage");
   double extractedStorageN = GetAmount(environmentalIndicator,"35.21 N removed from manure storage");
   double exportedManureN =GetAmount(environmentalIndicator,"35.22 N in exported manure");
   double deNHouse = houseN2 + houseN2O;
   double deNStorage	= storeN2 + storeN2O;
   double lossHouseEtcN = storevolat + housevolat + deNHouse + deNStorage + extractedStorageN;

	//Soil inputs
   double fertN          = GetAmount(environmentalIndicator,"31.01 N from mineral fertilizer");
   double depositN       = GetAmount(environmentalIndicator,"31.02 N from deposition");
   double irrigationN    = GetAmount(environmentalIndicator,"31.05 N from irrigation water");
   double fieldmanureN   = GetAmount(environmentalIndicator,"31.06 N from spread manure");
   double pastureurineN  = GetAmount(environmentalIndicator,"31.07 N in urine from grazing");
   double pasturefaecesN = GetAmount(environmentalIndicator,"31.08 N in faeces from grazing");
   double rootresN       = GetAmount(environmentalIndicator,"32.20 N from roots incorporated into soil");
   double plantresN      = GetAmount(environmentalIndicator,"32.21 N from straw incorporated into soil") + GetAmount(environmentalIndicator,"32.23 N from storage incorporated into soil");
   double plantlitterN   = GetAmount(environmentalIndicator,"32.22 N from leaf litterfall incorporated into soil");
   double gainSoilN=depositN+irrigationN+fertN+rootresN+plantresN+plantlitterN+fieldmanureN+pastureurineN+pasturefaecesN;

	//Soil outputs
   double leachednitrateN = GetAmount(environmentalIndicator,"31.40 Nitrate N leached");
   double leachedNH4N     = GetAmount(environmentalIndicator,"31.41 Ammonium N leached");
   double denN            = GetAmount(environmentalIndicator,"31.42 N2 from denitrification")
  						         +GetAmount(environmentalIndicator,"31.43 N2O from denitrification")
				   	         +GetAmount(environmentalIndicator,"31.44 N2O from nitrification");
   double spreadvolat     = GetAmount(environmentalIndicator,"31.45 NH3-N loss by spreading");
   double grazingvolat     = GetAmount(environmentalIndicator,"31.46 NH3-N loss during grazing");
   double fertvolat			= GetAmount(environmentalIndicator,"31.48 NH3-N loss from mineral fertiliser");

   double runoffN         = GetAmount(environmentalIndicator,"31.47 N in surface run-off");
   double uptakeN         = GetAmount(environmentalIndicator,"32.10 N uptake from soil");
   double areaAdjustmentSoilN  = GetAmount(environmentalIndicator,"37.02 N soil difference from area adjustment");
   double lossSoilN = leachednitrateN+leachedNH4N+uptakeN+denN +spreadvolat + grazingvolat + fertvolat + runoffN;

	//Crop inputs
   double seedN   = GetAmount(environmentalIndicator,"31.03 N from seed");
   double fixedN  = GetAmount(environmentalIndicator,"31.04 N from fixation");
   double Nuptake = GetAmount(environmentalIndicator,"32.10 N uptake from soil");
   double gainCropN= seedN +fixedN + Nuptake;

	//Crop outputs
   double GrainN      = GetAmount(environmentalIndicator,"31.20 N removed in grain");
   double StrawN      = GetAmount(environmentalIndicator,"31.21 N removed in straw");
   double CutN        = GetAmount(environmentalIndicator,"31.22 N removed in cut");
   double CattleN     = GetAmount(environmentalIndicator,"31.23 N removed by cattle");
   double incorpRootN = GetAmount(environmentalIndicator,"32.20 N from roots incorporated into soil");
   double incorpTopN  = GetAmount(environmentalIndicator,"32.21 N from straw incorporated into soil") + GetAmount(environmentalIndicator,"32.23 N from storage incorporated into soil");
   double litterTopN  = GetAmount(environmentalIndicator,"32.22 N from leaf litterfall incorporated into soil");
   double areaAdjustmentCropN  = GetAmount(environmentalIndicator,"37.01 N crop difference from area adjustment");
   double lossCropN   = GrainN + StrawN + CutN + CattleN + incorpRootN + incorpTopN + litterTopN;

	//Whole system balance

   double areaAdjustmentN  = GetAmount(environmentalIndicator,"37.00 N difference from area adjustment");
   double importedFeedN    = GetAmount(environmentalIndicator,"35.01 N from imported feed");
   double importedManureN  = GetAmount(environmentalIndicator,"35.02 N from imported manure");
   double importedBeddingN = GetAmount(environmentalIndicator,"35.03 N from imported bedding");
   double soldPlantN       = GetAmount(environmentalIndicator,"35.20 N in sold plant products");
   double gainN=depositN+ seedN + fixedN + importedFeedN + importedBeddingN + importedManureN + fertN +
                        boughtLivestockN + irrigationN;
   double lossN=leachednitrateN + leachedNH4N + denN + spreadvolat + runoffN + storevolat + housevolat + deNHouse + deNStorage + exportedManureN
   +soldPlantN + soldAnimalN + dairyCattleWasteN + fertvolat + grazingvolat;  //dairyCattleWasteN should maybe be deposited in manure


   if (oldstoreN<1E-10) // Initialize
      oldstoreN=storeN+lossN-gainN+areaAdjustmentN;
   if (oldstoreSoilN<1E-10) // Initialize
      oldstoreSoilN=storeSoilN+lossSoilN-gainSoilN+areaAdjustmentSoilN;
   if (oldstoreCropN<1E-10) // Initialize
      oldstoreCropN=storeCropN+lossCropN-gainCropN+areaAdjustmentCropN;
   if (oldstoreLivestockN<1E-10) // Initialize
      oldstoreLivestockN=storeLivestockN+ lossLivestockN-gainLivestockN;
   if (oldstoreHouseEtcN<1E-10) // Initialize
      oldstoreHouseEtcN=storeHouseEtcN+lossHouseEtcN-gainHouseEtcN;
   if (oldstoredProductN<1E-10) // Initialize
      oldstoredProductN=storedProductN+removedProductN-sentProductN;

   double balanceN=storeN-oldstoreN+lossN-gainN-lastErrorN+areaAdjustmentN;
   double balanceSoilN=(storeSoilN-oldstoreSoilN)+lossSoilN-gainSoilN-lastErrorSoilN+areaAdjustmentSoilN;
   double balanceCropN=storeCropN-oldstoreCropN+lossCropN-gainCropN-lastErrorCropN+areaAdjustmentCropN;
   double balanceLivestockN=storeLivestockN-oldstoreLivestockN+lossLivestockN-gainLivestockN-lastErrorLivestockN;
   double balanceHouseEtcN=storeHouseEtcN-oldstoreHouseEtcN+lossHouseEtcN-gainHouseEtcN-lastErrorHouseEtcN;
   double balanceProductStore = storedProductN - oldstoredProductN + removedProductN - sentProductN -lastErrorStoredN;



   if (fabs(balanceSoilN)>maxErrorN)
   {
      theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in soil N balance",balanceSoilN);

      lastErrorSoilN=balanceSoilN+lastErrorSoilN; // Avoid getting the "same" error more than once
   }
   if (fabs(balanceCropN)>maxErrorN)
   {
      theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in crop N balance",balanceCropN);

      lastErrorCropN=balanceCropN+lastErrorCropN; // Avoid getting the "same" error more than once
   }
   if (fabs(balanceLivestockN)>maxErrorN)
   {
      theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in livestock N balance",balanceLivestockN);

      lastErrorLivestockN=balanceLivestockN+lastErrorLivestockN; // Avoid getting the "same" error more than once
   }
   if (fabs(balanceHouseEtcN)>maxErrorN)
   {
      theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in housing and storage N balance",balanceHouseEtcN);

      lastErrorHouseEtcN=balanceHouseEtcN+lastErrorHouseEtcN; // Avoid getting the "same" error more than once
   }
   if (fabs(balanceProductStore)>maxErrorN)
   {
      theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in stored product N balance",balanceProductStore);

      lastErrorStoredN=balanceProductStore+lastErrorStoredN; // Avoid getting the "same" error more than once
   }

   if (fabs(balanceN)>maxErrorN)
   {
      theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in total N balance",balanceN);

      lastErrorN=balanceN+lastErrorN; // Avoid getting the "same" error more than once
   }

// ------------------------ Carbon -------------------------

	//Product store
   double sentProductC    = GetAmount(environmentalIndicator,"45.04 C from harvest to store");
   double storedProductC  = GetAmount(environmentalIndicator,"45.62 C in stored plant products");
   double removedProductC = GetAmount(environmentalIndicator,"45.81 C removed from stored plant products")
   									+ GetAmount(environmentalIndicator,"45.82 C removed from stored feed products");

	// Crop
   double storeCropC = GetAmount(environmentalIndicator,"40.61 C in standing crop/straw");

	// Soil
	double storeSoilC = GetAmount(environmentalIndicator,"40.60 Total C in soil");

	//Manure in housing and storage
   double manurestoreC = GetAmount(environmentalIndicator,"45.60 C bound in manure storage");
	double housingC     = GetAmount(environmentalIndicator,"45.61 C bound in stable manure");
   double storeHouseEtcC = manurestoreC + housingC;

	//Livestock
   double cattleC = GetAmount(environmentalIndicator,"43.60 C bound in cattle");
   double pigC    = 0.0;//GetAmount(environmentalIndicator,"34.60 C bound in pigs");
   double storeLivestockC=cattleC + pigC;

	//Total stored in system
   double storeC=storeSoilC+storeCropC+storeLivestockC+storeHouseEtcC+storedProductC;

	//Livestock inputs
  	double boughtCattleC    = GetAmount(environmentalIndicator,"43.03 C in bought cattle");


   double cattleTotalFeedC = GetAmount(environmentalIndicator,"41.01 C input into cattle");
   //double grazedC = GetAmount(environmentalIndicator,"43.04 C grazed by cattle");
   double cattleGainC = boughtCattleC + cattleTotalFeedC;

   double pigletfeedC = GetAmount(environmentalIndicator,"44.01 C in feed for piglets");
   double sowFeedC    = GetAmount(environmentalIndicator,"44.02 C in feed for sows");
   double baconerfeedC= GetAmount(environmentalIndicator,"44.03 C in feed for baconers");
   double boughtPigC  = GetAmount(environmentalIndicator,"44.04 C in bought pigs");
   double pigFeedC = baconerfeedC + sowFeedC + pigletfeedC;
   double pigGainC = pigFeedC + boughtPigC;

   double boughtLivestockC = boughtCattleC + boughtPigC;
   double gainLivestockC = pigGainC + cattleGainC ;//+ grazedC;

	//Livestock outputs
   double soldmilkC          = GetAmount(environmentalIndicator,"41.06 C in milk production");
   double cattleCO2C          = GetAmount(environmentalIndicator,"41.05 C in cattle CO2 production");
   double cattleCH4C          = GetAmount(environmentalIndicator,"41.02 C in cattle CH4 production");
   double soldCattleC        = GetAmount(environmentalIndicator,"43.21 C in sold cattle");
   double cattleManureC = GetAmount(environmentalIndicator,"41.03 C in cattle urine") + GetAmount(environmentalIndicator,"41.04 C in cattle faeces");
   double cattleLossC = cattleCO2C + cattleCH4C + cattleManureC + soldmilkC + soldCattleC;

   double soldPigC       = GetAmount(environmentalIndicator,"44.62 C in sold pigs");
   double pigCO2C          = GetAmount(environmentalIndicator,"44.63 C in pig CO2 production");
   double pigCH4C          = GetAmount(environmentalIndicator,"44.64 C in pig CH4 production");
   double sowManureC     = GetAmount(environmentalIndicator,"44.40 C in sows manure");
   double baconerManureC = GetAmount(environmentalIndicator,"44.41 C in baconers manure");
   double pigletManureC  = GetAmount(environmentalIndicator,"44.42 C in piglets manure");
   double pigManureC = sowManureC + baconerManureC + pigletManureC;
   double pigLossC   = pigManureC + soldPigC + pigCO2C + pigCH4C;

//   double soldAnimalC = soldCattleC + soldmilkC + soldPigC;
   double lossLivestockC = cattleLossC + pigLossC;

	//Housing inputs
   double cattleCToHousing = GetAmount(environmentalIndicator,"43.81 Cattle manure C to animal housing");
   double beddingC         = GetAmount(environmentalIndicator,"45.63 C in bedding");
   double gainHouseEtcC = pigManureC + cattleCToHousing + beddingC;

	//Housing outputs
   double houseCO2      = GetAmount(environmentalIndicator,"45.40 CO2-C volatilisation from animal house");
   double houseCH4      = GetAmount(environmentalIndicator,"45.41 CH4-C volatilisation from animal house");
//   double houseToStorageC = GetAmount(environmentalIndicator,"45.80 C transf. from stable to store");

	//Housing and storage outputs
   //Note that extractedStorageC could include C that is removed directly from animal housing (eg FYM)
   double storeCO2        = GetAmount(environmentalIndicator,"45.43 CO2-C volatilisation from manure storage");
   double storeCH4        = GetAmount(environmentalIndicator,"45.44 CH4-C volatilisation from manure storage");
   double extractedStorageC = GetAmount(environmentalIndicator,"45.21 C removed from manure storage");

   double lossHouseEtcC = storeCO2 + houseCO2 + houseCH4 + storeCH4 + extractedStorageC;

	//Soil inputs
   double fieldmanureC   = GetAmount(environmentalIndicator,"40.02 C from field-applied manure");
   double pastureurineC  = GetAmount(environmentalIndicator,"41.08 C in urine from grazing");
   double pasturefaecesC = GetAmount(environmentalIndicator,"41.09 C in faeces from grazing");
   double rootresC       = GetAmount(environmentalIndicator,"40.80 C from roots incorporated into soil");
   double plantresC      = GetAmount(environmentalIndicator,"40.81 C from straw incorporated into soil") + GetAmount(environmentalIndicator,"40.83 C from storage incorporated into soil");
   double plantlitterC   = GetAmount(environmentalIndicator,"40.82 C from leaf litterfall incorporated into soil");
   double gainSoilC=rootresC+plantresC+plantlitterC+fieldmanureC+pastureurineC+pasturefaecesC;

	//Soil outputs
   double soilCO2 = GetAmount(environmentalIndicator,"40.40 CO2 soil respiration");
   double soilCH4     = 0.0;//GetAmount(environmentalIndicator,"31.41 Ammonium C leached");
   double lossSoilC = soilCH4 + soilCO2;

	//Crop inputs
   double seedC   = GetAmount(environmentalIndicator,"40.03 C from seed");
   double fixedC = GetAmount(environmentalIndicator,"40.01 Net crop carbon fixation");
   double gainCropC= seedC +fixedC ;

	//Crop outputs
   double GrainC      = GetAmount(environmentalIndicator,"40.20 C removed in grain");
   double StrawC      = GetAmount(environmentalIndicator,"40.21 C removed in straw");
   double CutC        = GetAmount(environmentalIndicator,"40.22 C removed in cut");
   double CattleGrazedC     = GetAmount(environmentalIndicator,"40.23 C removed by cattle");
   double incorpRootC = GetAmount(environmentalIndicator,"40.80 C from roots incorporated into soil");
   double incropTopC  = GetAmount(environmentalIndicator,"40.81 C from straw incorporated into soil") + GetAmount(environmentalIndicator,"40.83 C from storage incorporated into soil");
   double litterTopC  = GetAmount(environmentalIndicator,"40.82 C from leaf litterfall incorporated into soil");
   double lossCropC   = GrainC + StrawC + CutC + CattleGrazedC + incorpRootC + incropTopC + litterTopC;

	//Whole system balance
   double importedFeedC    = GetAmount(environmentalIndicator,"45.01 C from imported feed");
   double importedManureC  = GetAmount(environmentalIndicator,"45.02 C from imported manure");
   double importedBeddingC = GetAmount(environmentalIndicator,"45.03 C from imported bedding");
 //  double soldPlantC       = GetAmount(environmentalIndicator,"45.20 C in sold plant products");
   double gainC=seedC + fixedC + importedFeedC + importedBeddingC + importedManureC +
                        boughtLivestockC;
   double lossC= lossLivestockC + soilCH4 + soilCO2 + storeCO2 + houseCO2 + houseCH4 + storeCH4;


   if (oldstoreC<1.0) // Initialize
      oldstoreC=storeC+lossC-gainC;
   if (oldstoreSoilC<1.0) // Initialize
      oldstoreSoilC=storeSoilC+lossSoilC-gainSoilC;
   if (oldstoreCropC<1.0) // Initialize
      oldstoreCropC=storeCropC+lossCropC-gainCropC;
   if (oldstoreLivestockC<1.0) // Initialize
      oldstoreLivestockC=storeLivestockC+ lossLivestockC-gainLivestockC;
   if (oldstoreHouseEtcC<1.0) // Initialize
      oldstoreHouseEtcC=storeHouseEtcC+lossHouseEtcC-gainHouseEtcC;
   if (oldstoredProductC<1.0) // Initialize
      oldstoredProductC=storedProductC+removedProductC-sentProductC;

//   double balanceC=storeC-oldstoreC+lossC-gainC-lastErrorC;
   double balanceSoilC=(storeSoilC-oldstoreSoilC) + lossSoilC - gainSoilC - lastErrorSoilC;
   //double balanceCropC=storeCropC-oldstoreCropC+lossCropC-gainCropC-lastErrorCropC;
   double balanceLivestockC=storeLivestockC-oldstoreLivestockC+lossLivestockC-gainLivestockC-lastErrorLivestockC;
   double balanceHouseEtcC=storeHouseEtcC-oldstoreHouseEtcC+lossHouseEtcC-gainHouseEtcC-lastErrorHouseEtcC;
   double balanceProductStoreC = storedProductC - oldstoredProductC + removedProductC - sentProductC;

   if (areaAdjustmentN<1E-20 && areaAdjustmentN>-1E-20) // Add code for this later !!!
   {
      if (fabs(balanceSoilC)>maxErrorC)
      {
         theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in soil C balance",balanceSoilC);

         lastErrorSoilC=balanceSoilC+lastErrorSoilC; // Avoid getting the "same" error more than once
      }
   }
   /*
   if (fabs(balanceCropC)>maxErrorC) Remove this part for now !!!
   {
      theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in crop C balance",balanceCropC);
      error=true;
      lastErrorCropC=balanceCropC+lastErrorCropC; // Avoid getting the "same" error more than once
   }
   */
   if (fabs(balanceLivestockC)>maxErrorC)
   {
      theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in livestock C balance",balanceLivestockC);

      lastErrorLivestockC=balanceLivestockC+lastErrorLivestockC; // Avoid getting the "same" error more than once
   }
   if (fabs(balanceHouseEtcC)>maxErrorC)
   {
      theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in housing and storage C balance",balanceHouseEtcC);

      lastErrorHouseEtcC=balanceHouseEtcC+lastErrorHouseEtcC; // Avoid getting the "same" error more than once
   }
   if (fabs(balanceProductStoreC)>maxErrorC)
   {
      theMessage->WarningWithDisplay("IndicatorOutput::CheckBalance - error in stored product C balance",balanceProductStore);

   }

}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::DailyUpdate()
{
   CheckBalance();

	for (int i=0;i<NumberOfIndicatorTypes;i++)
   	for (int j=0;j<SubIndex;j++)
	   	if (indicatorList[i][j])
	      	{


	   		linkList<item>::PS aOperationNode;

	   		indicatorList[i][j]->PeekHead(aOperationNode);
	   		   while(aOperationNode!=NULL)
	   		   {

	   			if (aOperationNode->element->GetDailyAddIndicator())
	   				aOperationNode->element->SetAmount(0.0);
	   			aOperationNode=aOperationNode->next;
	   		   }
	   		/*for (int k=0;k<indicatorList[i][j]->NumOfNodes();k++)
	         {
	         	aElement = indicatorList[i][j]->ElementAtNumber(k);
	            if (aElement->GetDailyAddIndicator())
	            	aElement->SetAmount(0.0);
	         }*/
	      	}
}

