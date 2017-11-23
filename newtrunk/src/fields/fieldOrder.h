
#ifndef FIELDORDER
#define FIELDORDER

#include "../base/message.h"
#include "../base/linklist.h"
#include "../base/bstime.h"
#include "../products/nitrogen.h"
#include "../products/manure.h"

class fieldOrder:
    public base
{
    private:
        string           crop_id;
        int              last_idx, field_no, start_year, pv_strat, fluid_hug_is_slurry, N_level, grazable;
        bool             Automatic;
        double           area, newArea, dist, N, P, K, solid_hug, fluid_hug_fall, fluid_hug_spring;

#ifdef TUPLE
        bool             tuple_file_open;
        string           last_date;
        fstream          tuple;
#endif

        linkList<string> TillageDate;
        linkList<string> TillageType;
        linkList<double> TillageIncorporation;
        linkList<double> TillageDepth;
        linkList<double> TillageMinDepth;

        // linkList<string> PlantItem;
        linkList<string>   SowCrops;
        linkList<string>   SowDates;
        linkList<double>   SowAmounts;
        linkList<string>   FertilizerDates;
        linkList<string>   FertilizerTypes;
        linkList<manure>   Manure;
        linkList<nitrogen> FertilizerN;
        linkList<string>   MineralForm;
        linkList<double>   NH4_fractions;
        linkList<double>   FertilizerP;
        linkList<double>   FertilizerK;
        linkList<double>   FertilizerCl;
        linkList<string>   IrrigationDates;
        linkList<double>   IrrigationAmounts;
        linkList<double>   IrrigationNContents;
        linkList<string>   HarvestDates;
        linkList<bool>     HarvestRemoveStraw;
        linkList<double>   FractionStrawHarvested;
        linkList<double>   FractionGrainHarvested;
        linkList<string>   CuttingDates;
        linkList<double>   CuttingHeights;
        linkList<int>      CuttingProcessProducts;
        linkList<bool>     CuttingRemovesStraw;
        linkList<string>   PesticidesDates;
        linkList<string>   PesticidesTypes;

        // ----- JBE + BMP simple grazing method ------------------------
        linkList<string> GrazingStartDates;
        linkList<string> GrazingStopDates;
        linkList<string> GrazingEndDates;    // What is the differece between this and the above ???!!!
        linkList<double> NitrogenLoad;
        linkList<double> GrazingEfficiency;


		linkList<string> AnimalOnFieldStart;
		linkList<string> AnimalOnFieldType;
		linkList<int> AnimalOnFieldAmount;

		linkList<string> AnimalOnFieldStop;
		linkList<string> AnimalOnFieldTypeStop;
		linkList<int> AnimalOnFieldAmountStop;
        // --------------------------------------------------------------
        linkList<string> SilageDates;
        linkList<double> IncorporationTimes;    // hours between manure application and incorporation

        fieldOrder();                           // Dissable use of default constructor

        fieldOrder(const fieldOrder & o);       // Dissable the compilers generation of default copy constructor.

        fieldOrder & operator = (
                const fieldOrder & o);          // Dissable the compilers generation of default assignment operator.

#ifdef TUPLE
        int GetOperationIndex(string date);

        void CloseTupleFile();

        void OpenTupleFile(string s);

        void AddTuple(string date,
                      double n,
                      double amount,
                      string op,
                      string type,
                      double SFU);
#endif

    public:
        fieldOrder(string       aName,
                   const int,
                   const base * aOwner = nullptr);

        ~fieldOrder() {

#ifdef TUPLE
            if (tuple_file_open) {
                tuple.close();
            }
#endif

        }

        void SetCropId(string c) {
            crop_id = c;
        }

        void SetStartYear(int s) {
            start_year = s;
        }

        void SetPVStrat(int pv) {
            pv_strat = pv;
        }

        void SetNLevel(int nl) {
            N_level = nl;
        }

        void SetArea(double a) {
            area = a;
        }

        void SetDistance(double d) {
            dist = d;
        }

        void SetN(double n) {
            N = n;
        }

        void SetP(double p) {
            P = p;
        }

        void SetK(double k) {
            K = k;
        }

        void SetSolidManure(double s) {
            solid_hug = s;
        }

        void SetFluidManureFall(double f) {
            fluid_hug_fall = f;
        }

        void SetFluidManureSpring(double f) {
            fluid_hug_spring = f;
        }

        void SetFieldNo(int fn) {
            field_no = fn;
        }

        int GetFieldNo() {
            return field_no;
        }

        string GetCropId() {
            return crop_id;
        }

        int GetStartYear() {
            return start_year;
        }

        int GetPVStrat() {
            return pv_strat;
        }

        int GetNLevel() {
            return N_level;
        }

        double GetArea() {
            return area;
        }

        double GetAdjustArea() {
            return newArea;
        }

        double GetDistance() {
            return dist;
        }

        double GetN() {
            return N;
        }

        double GetP() {
            return P;
        }

        double GetK() {
            return K;
        }

        double GetSolidManure() {
            return solid_hug;
        }

        double GetFluidManureFall() {
            return fluid_hug_fall;
        }

        double GetFluidManureSpring() {
            return fluid_hug_spring;
        }

        int GetFluidHugIsSlurry() {
            return fluid_hug_is_slurry;
        }

        bool GetAutomatic() {
            return Automatic;
        }

        int GetNumOfTillageDates() {
            return TillageDate.NumOfNodes();
        }

        int GetNumOfSowCrops() {
            return SowCrops.NumOfNodes();
        }

        int GetNumOfFertilizerDates() {
            return FertilizerDates.NumOfNodes();
        }

        int GetNumOfIrrigationDates() {
            return IrrigationDates.NumOfNodes();
        }

        int GetNumOfHarvestDates() {
            return HarvestDates.NumOfNodes();
        }

        int GetNumOfCuttingDates() {
            return CuttingDates.NumOfNodes();
        }

        int GetNumOfPesticidesDates() {
            return PesticidesDates.NumOfNodes();
        }

        int GetNumOfGrazingDates() {
            return GrazingStartDates.NumOfNodes();
        }

        string * GetGrazingStartDate(int nr) {
            return GrazingStartDates.ElementAtNumber(nr);
        }

        string * GetGrazingEndDate(int nr) {
            return GrazingEndDates.ElementAtNumber(nr);
        }

        string * GetGrazingStopDate(int nr) {
            return GrazingStopDates.ElementAtNumber(nr);
        }

        double GetNitrogenLoad(int nr) {
            return *NitrogenLoad.ElementAtNumber(nr);
        }

        double GetGrazingEfficiency(int nr) {
            return *GrazingEfficiency.ElementAtNumber(nr);
        }

        int GetNumOfGrazingStartDates() {
            return GrazingStartDates.NumOfNodes();
        }

        int GetNumOfGrazingStopDates() {
            return GrazingStopDates.NumOfNodes();
        }

        string * GetTillageDate(int nr) {
            return TillageDate.ElementAtNumber(nr);
        }

        double * GetTillageDepth(int nr) {
            return TillageDepth.ElementAtNumber(nr);
        }

        double * GetTillageMinDepth(int nr) {
            return TillageMinDepth.ElementAtNumber(nr);
        }

        double * GetTillageIncorporation(int nr) {
            return TillageDepth.ElementAtNumber(nr);
        }

        string * GetTillageType(int nr) {
            return TillageType.ElementAtNumber(nr);
        }

        string * GetSowCrop(int nr) {
            return SowCrops.ElementAtNumber(nr);
        }

        string * GetSowDate(int nr) {
            return SowDates.ElementAtNumber(nr);
        }

        double GetSowAmount(int nr) {
            return *SowAmounts.ElementAtNumber(nr);
        }

        string * GetFertilizerDate(int nr) {
            return FertilizerDates.ElementAtNumber(nr);
        }

        string * GetFertilizerType(int nr) {
            return FertilizerTypes.ElementAtNumber(nr);
        }

        nitrogen GetFertilizerN(int nr) {
            return *FertilizerN.ElementAtNumber(nr);
        }

        string GetMineralForm(int nr) {
            return *MineralForm.ElementAtNumber(nr);
        }

        double GetNH4_fraction(int nr) {
            return *NH4_fractions.ElementAtNumber(nr);
        }

        double GetFertilizerP(int nr) {
            return *FertilizerP.ElementAtNumber(nr);
        }

        double GetFertilizerK(int nr) {
            return *FertilizerK.ElementAtNumber(nr);
        }

        double GetFertilizerCl(int nr) {
            return *FertilizerCl.ElementAtNumber(nr);
        }

        manure * GetManure(int nr) {
            return Manure.ElementAtNumber(nr);
        }

        string * GetIrrigationDate(int nr) {
            return IrrigationDates.ElementAtNumber(nr);
        }

        double GetIrrigationAmount(int nr) {
            return *IrrigationAmounts.ElementAtNumber(nr);
        }

        double GetIrrigationNcontent(int nr) {
            return *IrrigationNContents.ElementAtNumber(nr);
        }

        string * GetHarvestDate(int nr) {
            return HarvestDates.ElementAtNumber(nr);
        }

        bool GetHarvestRemoveStraw(int nr) {
            return *HarvestRemoveStraw.ElementAtNumber(nr);
        }

        double GetFractionStrawHarvested(int nr);

        double GetFractionGrainHarvested(int nr);

        string * GetCuttingDate(int nr) {
            return CuttingDates.ElementAtNumber(nr);
        }

        double * GetCuttingHeights(int nr) {
            return CuttingHeights.ElementAtNumber(nr);
        }

        bool GetCuttingRemovesStraw(int nr) {
            return *CuttingRemovesStraw.ElementAtNumber(nr);
        }

        int * GetCuttingProcessProducts(int nr) {
            return CuttingProcessProducts.ElementAtNumber(nr);
        }

        string * GetPesticideDate(int nr) {
            return PesticidesDates.ElementAtNumber(nr);
        }

        string * GetPesticideType(int nr) {
            return PesticidesTypes.ElementAtNumber(nr);
        }

        int GetNumSilageDates() {
            return SilageDates.NumOfNodes();
        }

        int GetNumIncorporationTimes() {
            return IncorporationTimes.NumOfNodes();
        }

        double * GetIncorporationTimes(int nr) {
            return IncorporationTimes.ElementAtNumber(nr);
        }

        int GetYear(string * date);

        int GetMonth(string * date);

        int GetDay(string * date);

        void AddFluidManureSpring(double f) {
            fluid_hug_spring += f;
        }

        void ReadParameters(commonData *& data, int yearIndex);

        string * GetSilageDate(int nr) {
            return SilageDates.ElementAtNumber(nr);
        }

        void CheckDateStringForEmpty(string * DateString);


	
		string * getAnimalOnFieldStart(int nr)
		{
			return AnimalOnFieldStart.ElementAtNumber(nr);
		}
		
		string * getAnimalOnFieldType(int nr)
		{
			return AnimalOnFieldType.ElementAtNumber(nr);
		}
		int GetNumOfAnimalOnFieldStart()
		{
			return AnimalOnFieldStart.NumOfNodes();
		}
		int getAnimalOnFieldAmount(int nr)
		{
		
			return *AnimalOnFieldAmount.ElementAtNumber(nr);
		}

		string * getAnimalOnFieldStop(int nr)
		{
			return AnimalOnFieldStop.ElementAtNumber(nr);
		}

		string * getAnimalOnFieldTypeStop(int nr)
		{
			return AnimalOnFieldTypeStop.ElementAtNumber(nr);
		}
		int GetNumOfAnimalOnFieldStop()
		{
			return AnimalOnFieldStop.NumOfNodes();
		}
		int getAnimalOnFieldAmountStop(int nr)
		{
		
			return *AnimalOnFieldAmountStop.ElementAtNumber(nr);
		}
};
#endif

