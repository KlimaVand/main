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
 static int indicatIndex =-1;
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


   	for (int j=economicIndicatorStart; j<economicIndicatorStop;j++)
   		indicatorList[j] = NULL;
   	for (int j=environmentalIndicatorStart; j<SubIndex;j++)
   	   		indicatorList[j] = NULL;
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
   	for (int j=environmentalIndicatorStart; j<SubIndex;j++)
   		if (indicatorList[j])
      		delete  indicatorList[j];
   	for (int j=economicIndicatorStart; j<economicIndicatorStop;j++)
   	   		if (indicatorList[j])
   	      		delete  indicatorList[j];
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::InitializeIndicators()
{
   makingList=true;

	// Headlines:
   AddIndicator(  0,  "01	","",0);
   AddIndicator(  100, "01    -Farm characteristics-","",0);
   AddIndicator(  300, "03.00    -Capital-","",0);
   AddIndicator(  500, "05.00    -Gross marg�n-","",0);
   AddIndicator(  700, "07.00    -Farm profit -","",0);
   AddIndicator(  900, "09.00    -Net profit -","",0);
   AddIndicator(  1100,"11.00    -Store-","",0);
   AddIndicator(  1300,"13.00    -Labour-","",0);
   AddIndicator(  1500,"15.00    -Areas-","",0);
   AddIndicator(  1700,"17.00    -Yields-","",0);
   AddIndicator(  1900,"19.00    -Livestock production-","",0);
   AddIndicator(  2000,"20.00    -Energy-","",0);
   AddIndicator(3000,"30.00    -Nitrogen-","",0);
   AddIndicator(4000,"40.00    -Carbon-","",0);
   AddIndicator(5000,"50.00    -Phosporus-","",0);
   AddIndicator(6000,"60.00    -Potassium-","",0);
   AddIndicator(7000,"70.00    -Water-","",0);
   AddIndicator(8000,"80.00    -Pesticides-","",0);

    // The code below ensures equal amount of rows for each simulated year
	AddIndicator( 101, "01.01 Total area","ha",0);
   AddIndicator(  102, "01.02 DE (old)","no",0);
   AddIndicator(  103, "01.03 DE/ha (old)","no/ha",0);
   AddIndicator(  104, "01.04 DE (new)","no",0);
   AddIndicator(  105, "01.05 JB 1-4","ha",0);
   AddIndicator(  106, "01.06 JB 5-7","ha",0);

   // Capital
   AddIndicator(  301,   "03.01 Value of arable land","Dkr",0);
	AddIndicator( 302,    "03.02 Value of buildings & inventory","Dkr",0);
   AddIndicator(  303,  "03.03 Value of machinery","Dkr",0);
   AddIndicator(  305,   "03.05 Value of stored products","Dkr",0);
	AddIndicator( 306,   "03.06 Value of livestock","Dkr",0);
	AddIndicator( 307,    "03.07 Assets","Dkr",0);
	AddIndicator( 350,    "03.50 Value young cattle","Dkr",0);
  	AddIndicator( 351,   "03.51 Value mature cattle","Dkr",0);

   // Gross margin
	AddIndicator(  501,   "05.01 Gross margin","Dkr",0);
   AddIndicator(   510,  "05.10 Plant products sold","Dkr",0);
	AddIndicator(  511,   "05.11 W.wheat sold","Dkr",0);
	AddIndicator(  512,   "05.12 S.wheat sold","Dkr",0);
	AddIndicator(  513,   "05.13 W.rape sold","Dkr",0);
	AddIndicator(  514,   "05.14 S.rape sold","Dkr",0);
	AddIndicator(  515,   "05.15 W.barley sold","Dkr",0);
	AddIndicator(  516,   "05.16 S.barley sold","Dkr",0);
	AddIndicator(  517,   "05.17 Rye sold","Dkr",0);
	AddIndicator(  518,   "05.18 Oat sold","Dkr",0);
	AddIndicator(  519,   "05.19 Pea sold","Dkr",0);
	AddIndicator(  520,   "05.20 Potato sold","Dkr",0);
	AddIndicator(  521,   "05.21 Grass for seed sold","Dkr",0);
	AddIndicator(  522,   "05.22 Maize sold","Dkr",0);
	AddIndicator(  523,    "05.23 Beet sold","Dkr",0);
	AddIndicator(  524,  "05.24 Straw sold","DKr",0);
	AddIndicator(  525,  "05.25 Roughage sold","DKr",0);

   AddIndicator(   530,  "05.30 Sows sold","Dkr",0);
	AddIndicator(  531,   "05.31 Baconers sold","Dkr",0);
	AddIndicator(  532,   "05.32 Piglets sold","Dkr",0);
	AddIndicator(  533,  "05.33 Cast sows sold","Dkr",0);
	AddIndicator(  534,   "05.34 Weaners sold","Dkr",0);
	AddIndicator(  550,   "05.50 Young 1st year heifers sold","Dkr",0);
	AddIndicator(  551,   "05.51 Young 2nd year heifers sold","Dkr",0);
	AddIndicator(  552,  "05.52 Dairy cattle sold","Dkr",0);
	AddIndicator(  553,   "05.53 Beef cattle calves sold","Dkr",0);
	AddIndicator(  554,   "05.54 Mature beef cattle sold","Dkr",0);
	AddIndicator(  555,   "05.55 Milk sold","Dkr",0);


	AddIndicator(  610,   "06.10 Plant production misc. costs","Dkr",0);
   AddIndicator(   611,  "06.11 Costs seed","Dkr",0);
	AddIndicator(  612,   "06.12 Costs mineral fertilizer","Dkr",0);
	AddIndicator(  613,   "06.13 Costs chemicals","Dkr",0);
	AddIndicator(  614,   "06.14 Costs fuel","Dkr",0);
	AddIndicator(  615,   "06.15 Costs electricity","Dkr",0);
   AddIndicator(   616,  "06.16 Costs imported manure","Dkr",0);

	AddIndicator(  628,   "06.28 Weaners bought","Dkr",0);
	AddIndicator(  629,   "06.29 Piglets bought","Dkr",0);
	AddIndicator(  630,   "06.30 Baconers bought","Dkr",0);
	AddIndicator(  631,   "06.31 Sows bought","Dkr",0);
   AddIndicator(   632,  "06.32 Costs feed and bedding","Dkr",0);
	AddIndicator(  633,   "06.33 Cost of vet and production control","Dkr",0);
	AddIndicator(  650,   "06.50 Young 1st year heifers bought","Dkr",0);
	AddIndicator(  651,   "06.51 Young 2nd year heifers bought","Dkr",0);
	AddIndicator(  652,   "06.52 Dairy cattle bought","Dkr",0);
	AddIndicator(  655,   "06.54 Beef cattle calves bought","Dkr",0);
	AddIndicator(  656,   "06.55 Mature beef cattle bought","Dkr",0);


	AddIndicator(  670,   "06.70 Costs hired hours","Dkr",0);
;

	// Farm profit
	AddIndicator( 701,    "07.01 Net profit","Dkr",0);
	AddIndicator( 702,    "07.02 Depreciation buildings & inventory","Dkr",0);
	AddIndicator( 703,    "07.03 Depreciation machinery","Dkr",0);
	AddIndicator( 704,    "07.04 Insurance buildings & inventory","Dkr",0);
	AddIndicator( 705,    "07.05 Insurance machinery","Dkr",0);
	AddIndicator( 706,    "07.06 Maintenance buildings & inventory","Dkr",0);
	AddIndicator( 707,    "07.07 Maintenance machinery","Dkr",0);
	AddIndicator( 708,    "07.08 Cost of machine contractors","Dkr",0);

   // Net profit
	AddIndicator( 901,   "09.01 Total interest","Dkr",0);
	AddIndicator( 902,    "09.02 Total salery","Dkr",0);
	AddIndicator( 910,    "09.10 Total savings","Dkr",0);
   AddIndicator(  911,   "09.11 Earnings per hour","Dkr",0);
   AddIndicator(  912,   "09.12 Return on capital","\%",0);

   // Store
	AddIndicator( 1110,    "11.10 W.wheat store","t",0);
	AddIndicator( 1111,    "11.11 S.wheat store","t",0);
	AddIndicator( 1112,    "11.12 W.rape store","t",0);
	AddIndicator( 1113,    "11.13 S.rape store","t",0);
	AddIndicator( 1114,    "11.14 W.barley store","t",0);
	AddIndicator( 1115,    "11.15 S.barley store","t",0);
	AddIndicator( 1116,    "11.16 Rye store","t",0);
	AddIndicator( 1117,    "11.17 Oat store","t",0);
	AddIndicator( 1118,    "11.18 Pea store","t",0);
	AddIndicator( 1119,    "11.19 Potato store","t",0);
	AddIndicator( 1120,    "11.20 Grass for seed store","t",0);
	AddIndicator( 1121,    "11.21 Maize store","t",0);
	AddIndicator( 1122,    "11.22 Beet store","t",0);
	AddIndicator( 1130,   "11.30 Sows present","no",0);
	AddIndicator( 1131,    "11.31 Baconers present","no",0);
	AddIndicator( 1132,    "11.32 Piglets present","no",0);
	AddIndicator( 1133,    "11.33 New sows present","no",0);
	AddIndicator( 1150,    "11.50 Young cattle present","no",0);
	AddIndicator( 1151,    "11.51 Mature cattle present","no",0);
	AddIndicator( 1170,    "11.70 Slurry in slurrystore","t",0);
	AddIndicator( 1171,    "11.71 Dung in dungheap","t",0);

   // Labour
	AddIndicator( 1301,    "13.01 Total labour","hours",0);
	AddIndicator( 1310,    "13.10 Labour, field","hours",0);
   AddIndicator(  1311,    "13.11 Field-labour 0","hours",0);
   AddIndicator(  1312,   "13.12 Field-labour 1","hours",0);
   AddIndicator(  1313,   "13.13 Field-labour 2","hours",0);
   AddIndicator(  1314,   "13.14 Field-labour 3","hours",0);
   AddIndicator(  1315,   "13.15 Field-labour 4","hours",0);
   AddIndicator(  1316,   "13.16 Field-labour 5","hours",0);
   AddIndicator(  1317,   "13.17 Field-labour 6","hours",0);
   AddIndicator(  1318,   "13.18 Field-labour 7","hours",0);
   AddIndicator(  1319,   "13.19 Field-labour 8","hours",0);
   AddIndicator(  1320,  "13.20 Field-labour 9","hours",0);
   AddIndicator(  1321,   "13.21 Field-labour 10","hours",0);
   AddIndicator(  1322,   "13.22 Field-labour 11","hours",0);
	AddIndicator( 1330,    "13.30 Labour, stable","hours",0);
	AddIndicator( 1350,    "13.50 Labour, maintance and management","hours",0);

   // Areas
	AddIndicator( 1510,    "15.10 W.wheat area","ha",0);
	AddIndicator( 1511,    "15.11 S.wheat area","ha",0);
	AddIndicator( 1512,    "15.12 W.rape area","ha",0);
	AddIndicator( 1513,    "15.13 S.rape area","ha",0);
	AddIndicator( 1514,    "15.14 W.barley area","ha",0);
	AddIndicator( 1515,    "15.15 S.barley area","ha",0);
	AddIndicator( 1516,    "15.16 Rye area","ha",0);
	AddIndicator( 1517,    "15.17 Oat area","ha",0);
	AddIndicator( 1518,    "15.18 Pea area","ha",0);
	AddIndicator( 1519,    "15.19 Potato area","ha",0);
	AddIndicator( 1520,    "15.20 Grass for seed area","ha",0);
	AddIndicator( 1521,    "15.21 Maize area","ha",0);
	AddIndicator( 1522,    "15.22 Beet area","ha",0);
	AddIndicator( 1523,    "15.23 Barley/Pea area","ha",0);
	AddIndicator( 1524,    "15.24 Grass ley area","ha",0);
	AddIndicator( 1525,    "15.25 Grass/clover ley area","ha",0);
	AddIndicator( 1526,    "15.26 Fallow area","ha",0);

   AddIndicator(  1610,   "16.10 W.wheat area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1611,    "16.11 S.wheat area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1612,    "16.12 W.rape area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1613,    "16.13 S.rape area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1614,    "16.14 W.barley area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1615,    "16.15 S.barley area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1616,    "16.16 Rye area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1617,    "16.17 Oat area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1618,    "16.18 Pea area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1619,    "16.19 Potato area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1620,    "16.20 Grass for seed area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1621,    "16.21 Maize area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1622,    "16.22 Beet area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1623,    "16.23 Barley/Pea area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1624,  "16.24 Grass ley area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1625,    "16.25 Grass/clover ley area (harv. bef. 1/8)","ha",0);
	AddIndicator( 1626,    "16.26 Fallow area (harv. bef. 1/8)","ha",0);

   // Yields
	AddIndicator(  1701,   "17.01 Gross output of plant production","Dkr",0);
    AddIndicator(  1702,   "17.02 Grain yield at economic indicator date","t",0);
    AddIndicator(  1703,   "17.03 Straw yield at economic indicator date","t",0);
    AddIndicator(  1704,   "17.04 Maximum root depth","m",0);
	AddIndicator(  1710,   "17.10 W.wheat yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1711,   "17.11 S.wheat yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1712,   "17.12 W.rape yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1713,   "17.13 S.rape yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1714,   "17.14 W.barley yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1715,  "17.15 S.barley yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1716,   "17.16 Rye yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1717,   "17.17 Oat yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1718,   "17.18 Pea yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1719,   "17.19 Potato yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1720,   "17.20 Grass for seed yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1721,   "17.21 Grain maize yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1722,   "17.22 Beet yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1723,   "17.23 Ryegrass yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1724,   "17.24 Clover yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1725,   "17.25 Fallow yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1726,   "17.26 Maize wholecrop yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1727,  "17.27 Italian ryegrass yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1728,   "17.28 Soybean yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1729,   "17.29 Sunflower yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1730,  "17.30 Wheat silage yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1731,   "17.31 Lucerne yield (harv. bef. 1/8)","t",0);

   AddIndicator(   1810,  "18.10 Exp. W.wheat yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1811,   "18.11 Exp. S.wheat yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1812,   "18.12 Exp. W.rape yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1813,   "18.13 Exp. S.rape yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1814,   "18.14 Exp. W.barley yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1815,   "18.15 Exp. S.barley yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1816,   "18.16 Exp. Rye yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1817,   "18.17 Exp. Oat yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1818,   "18.18 Exp. Pea yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1819,  "18.19 Exp. Potato yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1820,   "18.20 Exp. Grass for seed yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1821,   "18.21 Exp. Maize yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1822,   "18.22 Exp. Beet yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1823,   "18.23 Exp. Barley/Pea yield (harv. bef. 1/8)","t",0);
	AddIndicator(  1824,   "18.24 Exp. Grass/clover yield (harv. bef. 1/8)","t",0);
   AddIndicator(   1825,  "18.25 Exp. Grass ley yield (harv. bef. 1/8)","ha",0);

   // Livestock production
   AddIndicator(  1901,   "19.01 Bought sows","no",0);
   AddIndicator(  1902,   "19.02 Bought weaners","no",0);
   AddIndicator(  1903,   "19.03 Bought piglets","no",0);
   AddIndicator(  1904,  "19.04 Bought baconers","no",0);
   AddIndicator(   1905,  "19.05 Sold weaners","no",0);
	AddIndicator(  1906,   "19.06 Sold piglets","no",0);
   AddIndicator(   1907,  "19.07 Sold baconers","no",0);
   AddIndicator(   1908,  "19.08 Sold new sows","no",0);
	AddIndicator(  1909,   "19.09 Sold cast sows","no",0);
	AddIndicator(  1910,   "19.10 Feed for sows","t",0);
	AddIndicator(  1911,   "19.11 Feed for baconers","t",0);
	AddIndicator(  1912,   "19.12 Feed for piglets","t",0);
	AddIndicator(  1930,   "19.30 Bought 1st year heifers","no",0);
	AddIndicator(  1931,   "19.31 Bought 2nd year heifers","no",0);
	AddIndicator(  1932,   "19.32 Bought dairy cattle","no",0);
	AddIndicator(  1933,   "19.33 Bought beef calves","no",0);
	AddIndicator(  1934,   "19.34 Bought beef cattle","no",0);
	AddIndicator(  1935,   "19.35 Sold 1st year heifers","no",0);
	AddIndicator(  1936,   "19.36 Sold 2nd year heifers","no",0);
	AddIndicator(  1937,   "19.37 Sold dairy cattle","no",0);
	AddIndicator(  1938,   "19.38 Sold beef calves","no",0);
	AddIndicator(  1939,  "19.39 Sold beef cattle","no",0);
	AddIndicator(  1940,  "19.40 Milk sold","litres",0);
	AddIndicator(  1950,   "19.50 Imported manure","t",0);
	AddIndicator(  1951,   "19.51 Imported cereals","t",0);
	AddIndicator(  1952,   "19.52 Imported roughage","t",0);
	AddIndicator(  1960,  "19.60 Manure from sows","t",0);
	AddIndicator(  1961,   "19.61 Manure from baconers","t",0);
	AddIndicator(  1962,   "19.62 Manure from piglets","t",0);
	AddIndicator(  1963,   "19.63 Manure produced by young cattle","t",0);
	AddIndicator(  1964,   "19.64 Manure produced by dairy cattle","t",0);
	AddIndicator(  1965,  "19.65 Manure spread on field","t",0);
   AddIndicator(   1970,  "19.70 Exported roughage","t",0);
	AddIndicator(  1971,   "19.71 Exported plant products","t",0);
	AddIndicator(  1972,   "19.72 Slurry produced","t",0);
	AddIndicator(  1973,   "19.73 Solid manure produced","t",0);

   // Energy
	AddIndicator(  2001,   "20.01 Diesel consumption","l",0);
	AddIndicator(  2002,   "20.02 Electricity consumption","KWh",0);
   AddIndicator(   2003,  "20.03 Diesel consumption by contractor","l",0);

   // Nitrogen - field .0X input .2X output .4X loss .6X status
	AddIndicator(3101,"31.01 N from mineral fertilizer","kg N",0);
	AddIndicator(3102,"31.02 N from deposition","kg N",0);
	AddIndicator(3103,"31.03 N from seed","kg N",0);
	AddIndicator(3104,"31.04 N from fixation","kg N",0);
	AddIndicator(3105,"31.05 N from irrigation water","kg N",0);
	AddIndicator(3106,"31.06 N from spread manure","kg N",0);
   AddIndicator(3107,"31.07 N in urine from grazing","kg N",0);
	AddIndicator(3108,"31.08 N in faeces from grazing","kg N",0);
   AddIndicator(3120,"31.20 N removed in grain","kg N",0);
	AddIndicator(3121,"31.21 N removed in straw","kg N",0);
	AddIndicator(3122,"31.22 N removed in cut","kg N",0);
	AddIndicator(3123,"31.23 N removed by cattle","kg N",0);
	AddIndicator(3140,"31.40 Nitrate N leached","kg N",0);
	AddIndicator(3141,"31.41 Ammonium N leached","kg N",0);
	AddIndicator(3142,"31.42 N2 from denitrification","kg N",0);
	AddIndicator(3143,"31.43 N2O from denitrification","kg N",0);
   AddIndicator(3144,"31.44 N2O from nitrification","kg N",0);
	AddIndicator(3145,"31.45 NH3-N loss by spreading","kg N",0);
	AddIndicator(3146,"31.46 NH3-N loss during grazing","kg N",0);
	AddIndicator(3147,"31.47 N in surface run-off","kg N",0);
	AddIndicator(3148,"31.48 NH3-N loss from mineral fertiliser","kg N",0);
	AddIndicator(3160,"31.60 N in standing crop/straw","kg N",0);
	AddIndicator(3161,"31.61 Total soil/soilsurface N","kg N",0);
	AddIndicator(3199,"31.99 Total soil/soilsurface nitrogen to 1 m depth","kg N",0);     //present in patch.cpp - still used?? NJH Nov 2007


   AddIndicator(3210,"32.10 N uptake from soil","kg N",0);
	AddIndicator(3220,"32.20 N from roots incorporated into soil","kg N",0);
	AddIndicator(3221,"32.21 N from straw incorporated into soil","kg N",0);
	AddIndicator(3222,"32.22 N from leaf litterfall incorporated into soil","kg N",0);
	AddIndicator(3223,"32.23 N from storage incorporated into soil","kg N",0);
//   AddIndicator("32.23 Plant top res. N incorp. in org. matt.","kg N",0);
   AddIndicator(3230,"32.30 Gross N-mineralisation from soil org. matter","kg N",0);
	AddIndicator(3240,"32.40 Nitrate N leached from leaching depth","kg N",0);
	AddIndicator(3241,"32.41 Ammonium N leached from leaching depth","kg N",0);
	AddIndicator(3260,"32.60 Organic N in soil","kg N",0);
	AddIndicator(3261,"32.61 Mineral N in soil","kg N",0);
	AddIndicator(3262,"32.62 AOM1 N in soil","kg N",0);
	AddIndicator(3263,"32.63 AOM2 N in soil","kg N",0);
	AddIndicator(3264,"32.64 SMB1 N in soil","kg N",0);
	AddIndicator(3265,"32.65 SMB2 N in soil","kg N",0);
	AddIndicator(3266,"32.66 SMR N in soil","kg N",0);
	AddIndicator(3267,"32.67 NOM N in soil","kg N",0);
	AddIndicator(3268,"32.68 IOM N in soil","kg N",0);
	AddIndicator(3269,"32.69 Organic N in soil 0-25 cm","kg N",0);
	AddIndicator(3270,"32.70 Mineral N in soil 0-25 cm","kg N",0);
	AddIndicator(3271,"32.71 Organic N in soil 25-50 cm","kg N",0);
	AddIndicator(3272,"32.72 Mineral N in soil 25-50 cm","kg N",0);
	AddIndicator(3273,"32.73 Organic N in soil 50-75 cm","kg N",0);
	AddIndicator(3274,"32.74 Mineral N in soil 50-75 cm","kg N",0);
	AddIndicator(3275,"32.75 Organic N in soil 75-250 cm","kg N",0);
	AddIndicator(3276,"32.76 Mineral N in soil 75-250 cm","kg N",0);
	AddIndicator(3277,"32.77 N in AOM+SMB 0-25 cm","kg N",0);
		AddIndicator(3278,"32.78 N in AOM+SMB 25-50 cm","kg N",0);
		AddIndicator(3279,"32.79 N in AOM+SMB 50-75 cm","kg N",0);
		AddIndicator(3280,"32.80 N in AOM+SMB 75-250 cm","kg N",0);
		AddIndicator(3281,"32.81 N in NOM 0-25 cm","kg N",0);
		AddIndicator(3282,"32.82 N in NOM 25-50 cm","kg N",0);
		AddIndicator(3290,"32.90 N in NOM 50-75 cm","kg N",0);
		AddIndicator(3291,"32.91 N in NOM 75-250 cm","kg N",0);
		AddIndicator(3292,"32.92 N in IOM 0-25 cm","kg N",0);
		AddIndicator(3293,"32.93 N in IOM 25-50 cm","kg N",0);
		AddIndicator(3294,"32.94 N in IOM 50-75 cm","kg N",0);
		AddIndicator(3295,"32.95 N in IOM 75-250 cm","kg N",0);

   AddIndicator(3283,"32.83 Nmin 0-500","kg N",0);
	AddIndicator(3284,"32.84 Nmin 0-750","kg N",0);
	AddIndicator(3285,"32.85 Nmin 0-1000","kg N",0);

   // Nitrogen in livestock
   AddIndicator(3301,"33.01 N in feed for young cattle","kg N",0);
   AddIndicator(3302,"33.02 N in feed for adult cattle","kg N",0);
	AddIndicator(3303,"33.03 N in bought cattle","kg N",0);
	AddIndicator(3304,"33.04 N grazed by cattle","kg N",0);
   AddIndicator(3320,"33.20 N in milk","kg N",0);
   AddIndicator(3321,"33.21 N in sold cattle","kg N",0);
   AddIndicator(3340,"33.40 N in dairy cattle waste","kg N",0);
   AddIndicator(3341,"33.41 N in young cattle manure","kg N",0);
   AddIndicator(3342,"33.42 N in dairy cattle manure","kg N",0);
	AddIndicator(3360,"33.60 N bound in cattle","kg N",0);
   AddIndicator(3380,"33.80 Cattle manure N to pasture","kg N",0);
   AddIndicator(3381,"33.81 Cattle manure N to animal housing","kg N",0);

	AddIndicator(3401,"34.01 N in feed for piglets","kg N",0);
   AddIndicator(3402,"34.02 N in feed for sows","kg N",0);
	AddIndicator(3403,"34.03 N in feed for baconers","kg N",0);
	AddIndicator(3404,"34.04 N in bought pigs","kg N",0);
   AddIndicator(3420,"34.20 N in sold pigs","kg N",0);
	AddIndicator(3440,"34.40 N in sows manure","kg N",0);
	AddIndicator(3441,"34.41 N in baconers manure","kg N",0);
	AddIndicator(3442,"34.42 N in piglets manure","kg N",0);
	AddIndicator(3460,"34.60 N bound in pigs","kg N",0);

	// Nitrogen in storage/stable
 	AddIndicator(3501,"35.01 N from imported feed","kg N",0);
	AddIndicator(3502,"35.02 N from imported manure","kg N",0);
	AddIndicator(3503,"35.03 N from imported bedding","kg N",0);
   AddIndicator(3504,"35.04 N from harvest to store","kg N",0);
   AddIndicator(3520,"35.20 N in sold plant products","kg N",0);
   AddIndicator(3521,"35.21 N removed from manure storage","kg N",0);
   AddIndicator(3522,"35.22 N in exported manure","kg N",0);
	AddIndicator(3540,"35.40 NH3-N volatilisation from animal house","kg N",0);
	AddIndicator(3541,"35.41 N2-N volatilisation from animal house","kg N",0);
	AddIndicator(3542,"35.42 N2O-N volatilisation from animal house","kg N",0);
	AddIndicator(3543,"35.43 NH3-N volatilisation from manure storage","kg N",0);
	AddIndicator(3544,"35.44 N2-N volatilisation from manure storage","kg N",0);
	AddIndicator(3545,"35.45 N2O-N volatilisation from manure storage","kg N",0);
	AddIndicator(3560,"35.60 N bound in manure storage","kg N",0);
	AddIndicator(3561,"35.61 N bound in stable manure","kg N",0);
   AddIndicator(3562,"35.62 N in stored plant products","kg N",0);
  	AddIndicator(3563,"35.63 N in bedding","kg N",0);
	AddIndicator(3580,"35.80 N transf. from stable to store","kg N",0);
   AddIndicator(3581,"35.81 N removed from stored plant products","kg N",0);
   AddIndicator(3582,"35.82 N removed from stored feed products","kg N",0);

   // Nitrogen in yields
   AddIndicator(3600,"36.00 W.wheat yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3601,"36.01 S.wheat yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3602,"36.02 W.rape yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3603,"36.03 S.rape yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3604,"36.04 W.barley yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3605,"36.05 S.barley yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3606,"36.06 Rye yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3607,"36.07 Oat yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3608,"36.08 Pea yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3609,"36.09 Potato yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3610,"36.10 Grass for seed yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3611,"36.11 Grain maize yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3612,"36.12 Beet yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3613,"36.13 Ryegrass yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3614,"36.14 Clover yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3615,"36.15 Fallow yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3616,"36.16 Maize wholecrop yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3617,"36.17 Italian ryegrass yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3618,"36.18 Soybean yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3619,"36.19 Sunflower yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3620,"36.20 Wheat silage yield (harv. bef. 1/8)","kg N",0);
	AddIndicator(3621,"36.21 Lucerne yield (harv. bef. 1/8)","kg N",0);

   // Nitrogen adjustment by area alterations
   AddIndicator(3700,"37.00 N difference from area adjustment","kg N",0);
   AddIndicator(3701,"37.01 N crop difference from area adjustment","kg N",0);
   AddIndicator(3702,"37.02 N soil difference from area adjustment","kg N",0);

   AddIndicator(3810,"38.10 N15 harvest in grain","kg N",0);
	AddIndicator(3811,"38.11 Total N15 harvest","kg N",0);
   AddIndicator(3812,"38.12 N15 leached from leaching depth","kg N",0);
	AddIndicator(3821,"38.21 N15-N2O from denitrification","kg N",0);
   AddIndicator(3822,"38.22 N15-N2O from nitrification","kg N",0);
   AddIndicator(3823,"38.23 N15-Denitrification","kg N",0);
 	AddIndicator(3824,"38.24 Total soil/soilsurface 15N","kg N",0);

   AddIndicator(3901,"39.01 Catch crop DM (1/9)","kg DM",0);
	AddIndicator(3903,"39.03 Catch crop N (1/9)","kg N",0);
	AddIndicator(3902,"39.02 Catch crop DM (1/11)","kg DM",0);
	AddIndicator(3904,"39.04 Catch crop N (1/11)","kg N",0);
	AddIndicator(3910,"39.10 Grain yield at environmental indicator date","t",0);
	AddIndicator(3911,"39.11 Straw yield at environmental indicator date","t",0);
	AddIndicator(3912,"39.12 Cut at environmental indicator date","t",0);
	AddIndicator(3920,	"39.20 Date of emergence of main crop","day of year",0);
		AddIndicator(3921,	"39.21 Date of flowering of main crop","day of year",0);
		AddIndicator(3922,	"39.22 Date of end of grain filling of main crop","day of year",0);
		AddIndicator(3923,	"39.23 Date of ripeness of main crop","day of year",0);

   // Carbon
	AddIndicator(4001,"40.01 Net crop carbon fixation","kg C",0);
	AddIndicator(4002,"40.02 C from field-applied manure","kg C",0);
	AddIndicator(4003,"40.03 C from seed","kg C",0);
    AddIndicator(4020,"40.20 C removed in grain","kg C",0);
	AddIndicator(4021,"40.21 C removed in straw","kg C",0);
	AddIndicator(4022,"40.22 C removed in cut","kg C",0);
	AddIndicator(4023,"40.23 C removed by cattle","kg C",0);
	AddIndicator(4040,"40.40 CO2 soil respiration","kg C",0);
	AddIndicator(4060,"40.60 Total C in soil","kg C",0);
	AddIndicator(4061,"40.61 C in standing crop/straw","kg C",0);
	AddIndicator(4062,"40.62 Total C in soil 0-25 cm","kg C",0);
	AddIndicator(4063,"40.63 Total C in soil 25-50 cm","kg C",0);
	AddIndicator(4064,"40.64 Total C in soil 50-75 cm","kg C",0);
	AddIndicator(4065,"40.65 Total C in soil 75-250 cm","kg C",0);
	AddIndicator(4066,"40.66 C in AOM+SMB 0-25 cm","kg C",0);
	AddIndicator(4067,"40.67 C in AOM+SMB 25-50 cm","kg C",0);
	AddIndicator(4068,"40.68 C in AOM+SMB 50-75 cm","kg C",0);
	AddIndicator(4069,"40.69 C in AOM+SMB 75-250 cm","kg C",0);
	AddIndicator(4070,"40.70 C in NOM 0-25 cm","kg C",0);
	AddIndicator(4071,"40.71 C in NOM 25-50 cm","kg C",0);
	AddIndicator(4072,"40.72 C in NOM 50-75 cm","kg C",0);
	AddIndicator(4073,"40.73 C in NOM 75-250 cm","kg C",0);
	AddIndicator(4074,"40.74 C in IOM 0-25 cm","kg C",0);
	AddIndicator(4075,"40.75 C in IOM 25-50 cm","kg C",0);
	AddIndicator(4076,"40.76 C in IOM 50-75 cm","kg C",0);
	AddIndicator(4077,"40.77 C in IOM 75-250 cm","kg C",0);
	AddIndicator(4080,"40.80 C from roots incorporated into soil","kg C",0);
	AddIndicator(4081,"40.81 C from straw incorporated into soil","kg C",0);
	AddIndicator(4082,"40.82 C from leaf litterfall incorporated into soil","kg C",0);
	AddIndicator(4083,"40.83 C from storage incorporated into soil","kg N",0);
	AddIndicator(4090,"40.90 AOM1 C in soil","kg C",0);
	AddIndicator(4091,"40.91 AOM2 C in soil","kg C",0);
	AddIndicator(4092,"40.92 SMB1 C in soil","kg C",0);
	AddIndicator(4093,"40.93 SMB2 C in soil","kg C",0);
	AddIndicator(4094,"40.94 SMR C in soil","kg C",0);
	AddIndicator(4095,"40.95 NOM C in soil","kg C",0);
	AddIndicator(4096,"40.96 IOM C in soil","kg C",0);

	AddIndicator(4101,"41.01 C input into cattle","kg C",0);
	AddIndicator(4102,"41.02 C in cattle CH4 production","kg C",0);
	AddIndicator(4103,"41.03 C in cattle urine","kg C",0);
	AddIndicator(4104,"41.04 C in cattle faeces","kg C",0);
	AddIndicator(4105,"41.05 C in cattle CO2 production","kg C",0);
	AddIndicator(4106,"41.06 C in milk production","kg C",0);
	AddIndicator(4107,"41.07 C retained in cattle body","kg C",0);
	AddIndicator(4108,"41.08 C in urine from grazing","kg C",0);
	AddIndicator(4109,"41.09 C in faeces from grazing","kg C",0);
   AddIndicator(4110,"41.10 C from grazing animals","kg C",0);

   AddIndicator(4301,"43.01 C in feed for young cattle","kg C",0);
   AddIndicator(4302,"43.02 C in feed for adult cattle","kg C",0);
	AddIndicator(4303,"43.03 C in bought cattle","kg C",0);
	AddIndicator(4304,"43.04 C grazed by cattle","kg C",0);
   AddIndicator(4321,"43.21 C in sold cattle","kg C",0);
   AddIndicator(4341,"43.41 4 in young cattle manure","kg C",0);
   AddIndicator(4342,"43.42 C in dairy cattle manure","kg C",0);
	AddIndicator(4360,"43.60 C bound in cattle","kg C",0);
   AddIndicator(4380,"43.80 Cattle manure C to pasture","kg C",0);
   AddIndicator(4381,"43.81 Cattle manure C to animal housing","kg C",0);

	AddIndicator(4401,"44.01 C in feed for piglets","kg C",0);
   AddIndicator(4402,"44.02 C in feed for sows","kg C",0);
	AddIndicator(4403,"44.03 C in feed for baconers","kg C",0);
	AddIndicator(4404,"44.04 C in bought pigs","kg C",0);
   AddIndicator(4420,"44.20 C in sold pigs","kg C",0);
	AddIndicator(4440,"44.40 C in sows manure","kg C",0);
	AddIndicator(4441,"44.41 C in baconers manure","kg C",0);
	AddIndicator(4442,"44.42 C in piglets manure","kg C",0);
	AddIndicator(4460,"44.60 C bound in pigs","kg C",0);
	AddIndicator(4462,"44.62 C in sold pigs","kg C",0);
	AddIndicator(4463,"44.63 C in pig CO2 production","kg C",0);
	AddIndicator(4464,"44.64 C in pig CH4 production","kg C",0);

	// Carbon in storage/stable
 	AddIndicator(4501,"45.01 C from imported feed","kg C",0);
	AddIndicator(4502,"45.02 C from imported manure","kg C",0);
	AddIndicator(4503,"45.03 C from imported bedding","kg C",0);
   AddIndicator(4504,"45.04 C from harvest to store","kg C",0);
   AddIndicator(4520,"45.20 C in sold plant products","kg C",0);
   AddIndicator(4521,"45.21 C removed from manure storage","kg C",0);
   AddIndicator(4522,"45.22 C in exported manure","kg C",0);
	AddIndicator(4540,"45.40 CO2-C volatilisation from animal house","kg C",0);
	AddIndicator(4541,"45.41 CH4-C volatilisation from animal house","kg C",0);
	AddIndicator(4543,"45.43 CO2-C volatilisation from manure storage","kg C",0);
	AddIndicator(4544,"45.44 CH4-C volatilisation from manure storage","kg C",0);
	AddIndicator(4560,"45.60 C bound in manure storage","kg C",0);
	AddIndicator(4561,"45.61 C bound in stable manure","kg C",0);
   AddIndicator(4562,"45.62 C in stored plant products","kg C",0);
  	AddIndicator(4563,"45.63 C in bedding","kg C",0);
	AddIndicator(4580,"45.80 C transf. from stable to store","kg C",0);
   AddIndicator(4581,"45.81 C removed from stored plant products","kg C",0);
   AddIndicator(4582,"45.82 C removed from stored feed products","kg C",0);


   // Phosphorus
	AddIndicator(5001,"50.01 Phosphorus from mineral fertilizer","kg P",0);
   AddIndicator(5002,"50.02 feed-P for sows","kg P",0);
	AddIndicator(5003,"50.03 feed-P for baconers","kg P",0);
	AddIndicator(5004,"50.04 feed-P for piglets","kg P",0);
	AddIndicator(5010,"50.10 manure-P from sows","kg P",0);
	AddIndicator(5011,"50.11 manure-P from baconers","kg P",0);
	AddIndicator(5012,"50.12 manure-P from piglets","kg P",0);

   // Potassium
	AddIndicator(6001,"60.01 Potassium from mineral fertilizer","kg K",0);
   AddIndicator(6002,"60.02 feed-K for sows","kg K",0);
	AddIndicator(6003,"60.03 feed-K for baconers","kg K",0);
	AddIndicator(6004,"60.04 feed-K for piglets","kg K",0);
	AddIndicator(6010,"60.10 manure-K from sows","kg K",0);
	AddIndicator(6011,"60.11 manure-K from baconers","kg K",0);
	AddIndicator(6012,"60.12 manure-K from piglets","kg K",0);

   // Water
	AddIndicator(7001,"70.01 Rainwater","cubic meter",0);
	AddIndicator(7002,"70.02 Irrigation","cubic meter",0);
	AddIndicator(7010,"70.10 Percolation","cubic meter",0);
	AddIndicator(7011,"70.11 Surface run-off","cubic meter",0);
	AddIndicator(7012,"70.12 Evapotranspiration","cubic meter",0);
  	AddIndicator(7015,"70.15 Water in field to balancedepth","cubic meter",0);  //check!
  	AddIndicator(7020,"70.20 Water in soil","cubic meter",0);
	AddIndicator(7040,"70.40 Percolation from leaching depth","cubic meter",0);
	AddIndicator(7041,"70.41 Water in field to leaching depth","cubic meter",0);
	AddIndicator(7042,"70.42 Bare soil evaporation","cubic meter",0);
	AddIndicator(7050,"70.50 Precipitation","mm",0);

   // Pesticides
	AddIndicator(8001,"80.01 Herbicide consumption","TFI",0);
	AddIndicator(8002,"80.02 Fungicide consumption","TFI",0);
	AddIndicator(8003,"80.03 Insecticide consumption","TFI",0);


   // REMOVE LATER !!!!!!!!!!!!!!!!!
   AddIndicator(9890,"98.90 Total carbon in soil","kg C",0);
   AddIndicator(9891,"98.91 current year"," ",0);
   AddIndicator(9892,"98.92 N2O from denitrification","kg N",0);
   AddIndicator(9893,"98.93 N2O from nitrification","kg N",0);
   AddIndicator(9990,"99.90 Total carbon in soil","kg C",0);
   AddIndicator(9991,"99.91 Nitrate N leached","kg N",0);
   AddIndicator(9992,"99.92 N2O from denitrification","kg N",0);
   AddIndicator(9993,"99.93 N2O from nitrification","kg N",0);
   AddIndicator(9895,"98.95 Number of patches"," ",0);

   makingList=false;
}

/****************************************************************************\
\****************************************************************************/
double IndicatorOutput::GetAmount(int index, string aName)
{


   indicatIndex=index;

   if ( indicatorList[index]!=NULL)
      return indicatorList[index]->GetAmount();
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
void IndicatorOutput::AddStateIndicator(int index, const char * aName, const char * aUnit, double aAmount)
{


   indicatIndex=index;


   if (indicatorList[index]!=NULL)
   {
	   indicatorList[index]->AddAmount(aAmount);
	   indicatorList[index]->SetDailyAddIndicator(true);
   }
   else
   {

      if (!makingList)
      {
      	theMessage->WarningWithDisplay("Indicator - ",aName);
         theMessage->WarningWithDisplay("IndicatorOutput::AddIndicator - no new indicators should be added at this point");
      }

      indicatorList[index]=new item(index,aName,aUnit,aAmount,true);

   }

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
   if (!aItem)
   {
	   cout<<"return 0"<<endl;
	   return 0;
   }


   return (aItem->GetNameInt()==indicatIndex);
}

/****************************************************************************\
\****************************************************************************/

int IndicatorOutput::BiggerThan(item * aItem)
{


	return (aItem->GetNameInt()>indicatIndex);

}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::AddIndicator(int index,const char * aName, const char * aUnit, double aAmount)
{


   indicatIndex=index;




   if (indicatorList[index]!=NULL)
	   indicatorList[index]->AddAmount(aAmount);
   else
   {
   	if (makingList==true)
      {
   			indicatorList[index]=new item(index,aName,aUnit,aAmount,false);

      }
      else
			theMessage->FatalError("indicat::AddIndicator - attempting to add an indicator when MakeList is false");
   }


}



/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::FlushIndicator(int indicatorType, int aYearNumber)
{
	yearNumber = aYearNumber;
	int start=0,stop=0;
	if(economicIndicator==indicatorType)
	{
		start=economicIndicatorStart;
		stop=economicIndicatorStop;
	}
	else if(environmentalIndicator==indicatorType)
	{
		start=environmentalIndicatorStart;
		stop=SubIndex;
	}

   for (int j=start; j<stop;j++)
   {
	   if(indicatorList[j]!=NULL)
		   {
			   	  indicatorList[j]->Out(*fileStreamCSV,* indicatorList[j],yearNumber,"\t","");;
			   	indicatorList[j]->SetAmount(0);

		   }


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

      //cout << endl << GetAmount("37.00 N difference from area adjustment") << endl;
      AddIndicator(3700,"37.00 N difference from area adjustment","kg N",-1.0*GetAmount(3700,"37.00 N difference from area adjustment"));
      //cout << GetAmount("37.00 N difference from area adjustment") << endl;
      //cout << GetAmount("37.01 N crop difference from area adjustment") << endl;
      AddIndicator(3701,"37.01 N crop difference from area adjustment","kg N",-1.0*GetAmount(3701,"37.01 N crop difference from area adjustment"));
      //cout << GetAmount("37.01 N crop difference from area adjustment") << endl;
      //cout << GetAmount("37.02 N soil difference from area adjustment") << endl;
      AddIndicator(3702,"37.02 N soil difference from area adjustment","kg N",-1.0*GetAmount(3702,"37.02 N soil difference from area adjustment"));
      //cout << GetAmount("37.02 N soil difference from area adjustment") << endl;
      //cout << endl;
   }
}

/****************************************************************************\
\****************************************************************************/
void IndicatorOutput::CheckBalance()
{

// ------------------------ Water ----------------------------
   storeW=GetAmount(7020,"70.20 Water in soil");
   if (storeW>1)
   {
      double ppt = GetAmount(7001,"70.01 Rainwater");
      double irrig = GetAmount(7002,"70.02 Irrigation");
      double gainW= ppt + irrig;
      double perc = GetAmount(7010,"70.10 Percolation");
      double runoff = GetAmount(7011,"70.11 Surface run-off");
      double evapotrans =GetAmount(7012,"70.12 Evapotranspiration");
// ???!!!     double bare_evap = GetAmount("70.42 Bare soil evaporation");
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
   double sentProductN    = GetAmount(3504,"35.04 N from harvest to store");
   double storedProductN  = GetAmount(3562,"35.62 N in stored plant products");
   double removedProductN = GetAmount(3581,"35.81 N removed from stored plant products")
   										+ GetAmount(3582,"35.82 N removed from stored feed products");

	// Crop
   double storeCropN = GetAmount(3160,"31.60 N in standing crop/straw");

	// Soil
//	double minSoilN = GetAmount("32.61 Mineral N in soil");
   storeSoilN      = GetAmount(3161,"31.61 Total soil/soilsurface N");
 //  double orgSoilN = GetAmount("32.60 Organic N in soil");

	//Manure in housing and storage
   double manurestoreN = GetAmount(3560,"35.60 N bound in manure storage");
	double housingN     = GetAmount(3561,"35.61 N bound in stable manure");
   double storeHouseEtcN = manurestoreN + housingN;

	//Livestock
   double cattleN = GetAmount(3360,"33.60 N bound in cattle");
   double pigN    = GetAmount(3460,"34.60 N bound in pigs");
   storeLivestockN=cattleN + pigN;

	//Total stored in system
   storeN=storeSoilN+storeCropN+storeLivestockN+storeHouseEtcN+storedProductN;

	//Livestock inputs
   double youngCattleFeedN = GetAmount(3301,"33.01 N in feed for young cattle");
   double dairyCattleFeedN = GetAmount(3302,"33.02 N in feed for adult cattle");
  	double boughtCattleN    = GetAmount(3303,"33.03 N in bought cattle");
   double cattleFeedN = youngCattleFeedN + dairyCattleFeedN;
   double cattleGainN = boughtCattleN + cattleFeedN;

   double pigletfeedN = GetAmount(3401,"34.01 N in feed for piglets");
   double sowFeedN    = GetAmount(3402,"34.02 N in feed for sows");
   double baconerfeedN= GetAmount(3403,"34.03 N in feed for baconers");
   double boughtPigN  = GetAmount(3404,"34.04 N in bought pigs");
   double pigFeedN = baconerfeedN + sowFeedN + pigletfeedN;
   double pigGainN = pigFeedN + boughtPigN;

   double boughtLivestockN = boughtCattleN + boughtPigN;
   //double grazedN = GetAmount("33.04 N grazed by cattle");
   double gainLivestockN = pigGainN + cattleGainN ;//+ grazedN;

	//Livestock outputs
   double soldmilkN          = GetAmount(3320,"33.20 N in milk");
   double soldCattleN        = GetAmount(3321,"33.21 N in sold cattle");
   double dairyCattleWasteN  = GetAmount(3340,"33.40 N in dairy cattle waste"); // Livmoder, evt. d�de kalve, tab fra system
   double youngCattleManureN = GetAmount(3341,"33.41 N in young cattle manure");
   double dairyCattleManureN = GetAmount(3342,"33.42 N in dairy cattle manure");
   double cattleManureN = youngCattleManureN + dairyCattleManureN;
   double cattleLossN = dairyCattleWasteN + cattleManureN + soldmilkN + soldCattleN;

   double soldPigN       = GetAmount(3420,"34.20 N in sold pigs");
   double sowManureN     = GetAmount(3440,"34.40 N in sows manure");
   double baconerManureN = GetAmount(3441,"34.41 N in baconers manure");
   double pigletManureN  = GetAmount(3442,"34.42 N in piglets manure");
   double pigManureN = sowManureN + baconerManureN + pigletManureN;
   double pigLossN   = pigManureN + soldPigN;

   double soldAnimalN = soldCattleN + soldmilkN + soldPigN;
   double lossLivestockN = cattleLossN + pigLossN;

	//Housing inputs
   double cattleNToHousing = GetAmount(3381,"33.81 Cattle manure N to animal housing");
   double beddingN         = GetAmount(3563,"35.63 N in bedding");
   double gainHouseEtcN = pigManureN + cattleNToHousing + beddingN;

	//Housing outputs
   double housevolat      = GetAmount(3540,"35.40 NH3-N volatilisation from animal house");
   double houseN2      = GetAmount(3541,"35.41 N2-N volatilisation from animal house");
   double houseN2O      = GetAmount(3542,"35.42 N2O-N volatilisation from animal house");

	//Housing and storage outputs
   //note that extractedStorageN could include N that is removed directly from animal housing (eg FYM)
   double storevolat        = GetAmount(3543,"35.43 NH3-N volatilisation from manure storage");
   double storeN2        = GetAmount(3544,"35.44 N2-N volatilisation from manure storage");
   double storeN2O        = GetAmount(3545,"35.45 N2O-N volatilisation from manure storage");
   double extractedStorageN = GetAmount(3521,"35.21 N removed from manure storage");
   double exportedManureN =GetAmount(3522,"35.22 N in exported manure");
   double deNHouse = houseN2 + houseN2O;
   double deNStorage	= storeN2 + storeN2O;
   double lossHouseEtcN = storevolat + housevolat + deNHouse + deNStorage + extractedStorageN;

	//Soil inputs
   double fertN          = GetAmount(3101,"31.01 N from mineral fertilizer");
   double depositN       = GetAmount(3102,"31.02 N from deposition");
   double irrigationN    = GetAmount(3105,"31.05 N from irrigation water");
   double fieldmanureN   = GetAmount(3106,"31.06 N from spread manure");
   double pastureurineN  = GetAmount(3107,"31.07 N in urine from grazing");
   double pasturefaecesN = GetAmount(3108,"31.08 N in faeces from grazing");
   double rootresN       = GetAmount(3220,"32.20 N from roots incorporated into soil");
   double plantresN      = GetAmount(3221,"32.21 N from straw incorporated into soil") + GetAmount(3223,"32.23 N from storage incorporated into soil");
   double plantlitterN   = GetAmount(3222,"32.22 N from leaf litterfall incorporated into soil");
   double gainSoilN=depositN+irrigationN+fertN+rootresN+plantresN+plantlitterN+fieldmanureN+pastureurineN+pasturefaecesN;

	//Soil outputs
   double leachednitrateN = GetAmount(3140,"31.40 Nitrate N leached");
   double leachedNH4N     = GetAmount(3141,"31.41 Ammonium N leached");
   double denN            = GetAmount(3142,"31.42 N2 from denitrification")
  						         +GetAmount(3143,"31.43 N2O from denitrification")
				   	         +GetAmount(3144,"31.44 N2O from nitrification");
   double spreadvolat     = GetAmount(3145,"31.45 NH3-N loss by spreading");
   double grazingvolat     = GetAmount(3146,"31.46 NH3-N loss during grazing");
   double fertvolat			= GetAmount(3148,"31.48 NH3-N loss from mineral fertiliser");

   double runoffN         = GetAmount(3147,"31.47 N in surface run-off");
   double uptakeN         = GetAmount(3210,"32.10 N uptake from soil");
   double areaAdjustmentSoilN  = GetAmount(3702,"37.02 N soil difference from area adjustment");
   double lossSoilN = leachednitrateN+leachedNH4N+uptakeN+denN +spreadvolat + grazingvolat + fertvolat + runoffN;

	//Crop inputs
   double seedN   = GetAmount(3103,"31.03 N from seed");
   double fixedN  = GetAmount(3104,"31.04 N from fixation");
   double Nuptake = GetAmount(3210,"32.10 N uptake from soil");
   double gainCropN= seedN +fixedN + Nuptake;

	//Crop outputs
   double GrainN      = GetAmount(3120,"31.20 N removed in grain");
   double StrawN      = GetAmount(3121,"31.21 N removed in straw");
   double CutN        = GetAmount(3122,"31.22 N removed in cut");
   double CattleN     = GetAmount(3123,"31.23 N removed by cattle");
   double incorpRootN = GetAmount(3220,"32.20 N from roots incorporated into soil");
   double incorpTopN  = GetAmount(3221,"32.21 N from straw incorporated into soil") + GetAmount(3223,"32.23 N from storage incorporated into soil");
   double litterTopN  = GetAmount(3222,"32.22 N from leaf litterfall incorporated into soil");
   double areaAdjustmentCropN  = GetAmount(3701,"37.01 N crop difference from area adjustment");
   double lossCropN   = GrainN + StrawN + CutN + CattleN + incorpRootN + incorpTopN + litterTopN;

	//Whole system balance

   double areaAdjustmentN  = GetAmount(3700,"37.00 N difference from area adjustment");
   double importedFeedN    = GetAmount(3501,"35.01 N from imported feed");
   double importedManureN  = GetAmount(3502,"35.02 N from imported manure");
   double importedBeddingN = GetAmount(3503,"35.03 N from imported bedding");
   double soldPlantN       = GetAmount(3520,"35.20 N in sold plant products");
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
   double sentProductC    = GetAmount(4504,"45.04 C from harvest to store");
   double storedProductC  = GetAmount(4562,"45.62 C in stored plant products");
   double removedProductC = GetAmount(4581,"45.81 C removed from stored plant products")
   									+ GetAmount(4582,"45.82 C removed from stored feed products");

	// Crop
   double storeCropC = GetAmount(4061,"40.61 C in standing crop/straw");

	// Soil
	double storeSoilC = GetAmount(4060,"40.60 Total C in soil");

	//Manure in housing and storage
   double manurestoreC = GetAmount(4560,"45.60 C bound in manure storage");
	double housingC     = GetAmount(4561,"45.61 C bound in stable manure");
   double storeHouseEtcC = manurestoreC + housingC;

	//Livestock
   double cattleC = GetAmount(4360,"43.60 C bound in cattle");
   double pigC    = 0.0;//GetAmount("34.60 C bound in pigs");
   double storeLivestockC=cattleC + pigC;

	//Total stored in system
   double storeC=storeSoilC+storeCropC+storeLivestockC+storeHouseEtcC+storedProductC;

	//Livestock inputs
  	double boughtCattleC    = GetAmount(4303,"43.03 C in bought cattle");


   double cattleTotalFeedC = GetAmount(4101,"41.01 C input into cattle");
   //double grazedC = GetAmount("43.04 C grazed by cattle");
   double cattleGainC = boughtCattleC + cattleTotalFeedC;

   double pigletfeedC = GetAmount(4401,"44.01 C in feed for piglets");
   double sowFeedC    = GetAmount(4402,"44.02 C in feed for sows");
   double baconerfeedC= GetAmount(4403,"44.03 C in feed for baconers");
   double boughtPigC  = GetAmount(4404,"44.04 C in bought pigs");
   double pigFeedC = baconerfeedC + sowFeedC + pigletfeedC;
   double pigGainC = pigFeedC + boughtPigC;

   double boughtLivestockC = boughtCattleC + boughtPigC;
   double gainLivestockC = pigGainC + cattleGainC ;//+ grazedC;

	//Livestock outputs
   double soldmilkC          = GetAmount(4106,"41.06 C in milk production");
   double cattleCO2C          = GetAmount(4105,"41.05 C in cattle CO2 production");
   double cattleCH4C          = GetAmount(4102,"41.02 C in cattle CH4 production");
   double soldCattleC        = GetAmount(4321,"43.21 C in sold cattle");
   double cattleManureC = GetAmount(4103,"41.03 C in cattle urine") + GetAmount(4104,"41.04 C in cattle faeces");
   double cattleLossC = cattleCO2C + cattleCH4C + cattleManureC + soldmilkC + soldCattleC;

   double soldPigC       = GetAmount(4462,"44.62 C in sold pigs");
   double pigCO2C          = GetAmount(4463,"44.63 C in pig CO2 production");
   double pigCH4C          = GetAmount(4464,"44.64 C in pig CH4 production");
   double sowManureC     = GetAmount(4440,"44.40 C in sows manure");
   double baconerManureC = GetAmount(4441,"44.41 C in baconers manure");
   double pigletManureC  = GetAmount(4442,"44.42 C in piglets manure");
   double pigManureC = sowManureC + baconerManureC + pigletManureC;
   double pigLossC   = pigManureC + soldPigC + pigCO2C + pigCH4C;

//   double soldAnimalC = soldCattleC + soldmilkC + soldPigC;
   double lossLivestockC = cattleLossC + pigLossC;

	//Housing inputs
   double cattleCToHousing = GetAmount(4381,"43.81 Cattle manure C to animal housing");
   double beddingC         = GetAmount(4563,"45.63 C in bedding");
   double gainHouseEtcC = pigManureC + cattleCToHousing + beddingC;

	//Housing outputs
   double houseCO2      = GetAmount(4540,"45.40 CO2-C volatilisation from animal house");
   double houseCH4      = GetAmount(4541,"45.41 CH4-C volatilisation from animal house");
//   double houseToStorageC = GetAmount("45.80 C transf. from stable to store");

	//Housing and storage outputs
   //Note that extractedStorageC could include C that is removed directly from animal housing (eg FYM)
   double storeCO2        = GetAmount(4543,"45.43 CO2-C volatilisation from manure storage");
   double storeCH4        = GetAmount(4544,"45.44 CH4-C volatilisation from manure storage");
   double extractedStorageC = GetAmount(4521,"45.21 C removed from manure storage");

   double lossHouseEtcC = storeCO2 + houseCO2 + houseCH4 + storeCH4 + extractedStorageC;

	//Soil inputs
   double fieldmanureC   = GetAmount(4002,"40.02 C from field-applied manure");
   double pastureurineC  = GetAmount(4108,"41.08 C in urine from grazing");
   double pasturefaecesC = GetAmount(4109,"41.09 C in faeces from grazing");
   double rootresC       = GetAmount(4080,"40.80 C from roots incorporated into soil");
   double plantresC      = GetAmount(4081,"40.81 C from straw incorporated into soil") + GetAmount(4083,"40.83 C from storage incorporated into soil");
   double plantlitterC   = GetAmount(4082,"40.82 C from leaf litterfall incorporated into soil");
   double gainSoilC=rootresC+plantresC+plantlitterC+fieldmanureC+pastureurineC+pasturefaecesC;

	//Soil outputs
   double soilCO2 = GetAmount(4040,"40.40 CO2 soil respiration");
   double soilCH4     = 0.0;//GetAmount("31.41 Ammonium C leached");
   double lossSoilC = soilCH4 + soilCO2;

	//Crop inputs
   double seedC   = GetAmount(4003,"40.03 C from seed");
   double fixedC = GetAmount(4001,"40.01 Net crop carbon fixation");
   double gainCropC= seedC +fixedC ;

	//Crop outputs
   double GrainC      = GetAmount(4020,"40.20 C removed in grain");
   double StrawC      = GetAmount(4021,"40.21 C removed in straw");
   double CutC        = GetAmount(4022,"40.22 C removed in cut");
   double CattleGrazedC     = GetAmount(4023,"40.23 C removed by cattle");
   double incorpRootC = GetAmount(4080,"40.80 C from roots incorporated into soil");
   double incropTopC  = GetAmount(4081,"40.81 C from straw incorporated into soil") + GetAmount(4083,"40.83 C from storage incorporated into soil");
   double litterTopC  = GetAmount(4082,"40.82 C from leaf litterfall incorporated into soil");
   double lossCropC   = GrainC + StrawC + CutC + CattleGrazedC + incorpRootC + incropTopC + litterTopC;

	//Whole system balance
   double importedFeedC    = GetAmount(4501,"45.01 C from imported feed");
   double importedManureC  = GetAmount(4502,"45.02 C from imported manure");
   double importedBeddingC = GetAmount(4503,"45.03 C from imported bedding");
 //  double soldPlantC       = GetAmount("45.20 C in sold plant products");
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


   	for (int j=economicIndicatorStart;j<economicIndicatorStop;j++)
	   	if (indicatorList[j])
	      	{

	   			if (indicatorList[j]->GetDailyAddIndicator())
	   				indicatorList[j]->SetAmount(0.0);
	      	}
 	for (int j=environmentalIndicatorStart;j<SubIndex;j++)
	   	if (indicatorList[j])
	      	{

	   			if (indicatorList[j]->GetDailyAddIndicator())
	   				indicatorList[j]->SetAmount(0.0);
	      	}
}

