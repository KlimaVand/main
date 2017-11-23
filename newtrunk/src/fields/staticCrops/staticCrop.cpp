
#include "../../base/common.h"
#include "staticCrop.h"
#include "../fieldOperation/silageFields.h"
#include "../fieldOperation/removeAnimalOnField.h"
#ifdef MEASCOPE

#include "../../base/controlParameters.h"

#endif

static int sort_day, sort_month, sort_year;

staticCrop::staticCrop() {
    cropName       = "";
    day            = month = 0;
    irrigation     = 0;
    OpListItems    = 0;
    gatheringStraw = true;    // Default, can be modified
    LP             = nullptr;
    aPlantItem1    = nullptr;
    aPlantItem2    = nullptr;
}

void staticCrop::SetSorting(int year,
                            int month,
                            int day) {
    sort_month = month;
    sort_day   = day;
    sort_year  = year;
}

int staticCrop::SortFunc(fieldOperationFields * PP) {
    long idx = sort_year * 512 + sort_month * 32 + sort_day;    // �
    long searchindex;

#ifdef MEASCOPE
    if (PP -> GetopStart()) {
        searchindex = PP -> GetStartYear() * 512 + PP -> GetStartMonth() * 32 + PP -> GetStartDay();    // �
    } else
#endif

        searchindex = PP -> GetYear() * 512 + PP -> GetMonth() * 32 + PP -> GetDay();                   // �

    return (searchindex > idx);
}

string staticCrop::GetSeedName(string crop_id) {
    string output = "";

    CorrectCropId(crop_id);    // Hack for FARM-N !!!

    if ((crop_id == "W1") || (crop_id == "W2")) {
        output = "SEEDWNWHT";
    } else if (crop_id == "RW") {
        output = "SEEDWNRPE";
    } else if (crop_id == "RS") {
        output = "SEEDSPRPE";
    } else if ((crop_id == "B1") || (crop_id == "B2")) {
        output = "SEEDSPBARL";
    } else if ((crop_id == "F1") || (crop_id == "G1") || (crop_id == "GR") || (crop_id == "S1")) {
        output = "SEEDRYEGRASS";
    } else if ((crop_id == "B5") || (crop_id == "B6")) {
        output = "SEEDWNBRL";
    } else if (crop_id == "PE") {
        output = "SEEDPEA";
    } else if (crop_id == "W5") {
        output = "SEEDSPWHT";
    } else if (crop_id == "BE") {
        output = "SEEDBEET";
    } else if ((crop_id == "K1") || (crop_id == "C1")) {
        output = "SEEDCLOVER";
    } else if (crop_id == "MA"  || crop_id == "MK") {
        output = "SEEDMAIZE";
    } else if (crop_id == "PO") {
        output = "SEEDPOTATO";
    } else if (crop_id == "RY") {
        output = "SEEDRYE";
    } else if ((crop_id == "O1") || (crop_id == "O2")) {
        output = "SEEDOAT";
    } else if (crop_id == "OR") {
        output = "SEEDOILRADISH";
    } else if (crop_id == "CH") {
        output = "SEEDCHICKORY";
    } else if (crop_id == "DW") {
        output = "SEEDDYERSWOAD";
    } else if (crop_id == "IR") {
        output = "SEEDITALRYEGRASS";    // added NJH May 2009
    } else if (crop_id == "HV") {
        output = "SEEDHAIRYVETCH";
    } else if (crop_id == "SF") {
        output = "SEEDSUNFLOWER";       // added NJH May 2009
    } else if (crop_id == "SB") {
        output = "SEEDSOYBEAN";        // added NJH May 2009
    } else if (crop_id == "L1") {
        output = "SEEDLUCERNE";         // added NJH May 2009
    } else if (crop_id == "TR") {
        output = "SEEDTriticale";
    } else if (crop_id == "FB") {
        output = "SEEDFabaBean";
    } else if (crop_id == "CR") {
        output = "SEEDCrimsonClover";
    } else if ((crop_id == "F2") || (crop_id == "BP") || (crop_id == "C2") || (crop_id == "B8") || (crop_id == "G2")
               || (crop_id == "G3") || (crop_id == "K2") || (crop_id == "W8") || (crop_id == "B9")) {
        theMessage -> FatalError("staticCrop::GetSeedName - crop ", crop_id, " can not be sown directly");
    } else {
        theMessage -> FatalError("staticCrop::GetSeedName - crop ", crop_id, " not found");
    }

    return output;
}

void staticCrop::AddOpList(fieldOperationFields * action) {
    cloneList<fieldOperationFields>::PS P, P1;

#ifdef MEASCOPE
    if (action -> GetopStart()) {
        SetSorting(action -> GetStartYear(), action -> GetStartMonth(), action -> GetStartDay());
    } else
#endif

        SetSorting(action -> GetYear(), action -> GetMonth(), action -> GetDay());

    LP -> FirstThat(P, SortFunc);

    if (P) {
        LP -> InsertBefore(P, P1, action);
    } else {
        LP -> InsertLast(action);
    }
}

void staticCrop::InitCrop(int irrigt) {
    irrigation = irrigt;
}

void staticCrop::ResetOpList() {
    LP = new cloneList<fieldOperationFields>;
}

void staticCrop::SpecifiedPlan(fieldOrder * PP,
                               int          JB_NO,
                               string       crop_id) {
    int yearOffset = 0;

    if (aPlantItem1) {
        theProducts -> GiveProductInformation(aPlantItem1);
    }

    if (aPlantItem2) {
        theProducts -> GiveProductInformation(aPlantItem2);
    }

#ifdef MEASCOPE
    SpecifiedPeriods(PP, JB_NO, crop_id);

    return;    // The code below is ignored with this compiler option
#endif

    if ((PP -> GetNumOfHarvestDates() > 0)) {
        gatheringStraw = PP -> GetHarvestRemoveStraw(0);    // is straw gathered
    }

    for (int i = 0; i < PP -> GetNumOfTillageDates(); i++) {
        soilTreatFields * FO   = new soilTreatFields;
        string *          date = PP -> GetTillageDate(i);
        string *          Type = PP -> GetTillageType(i);

        if ("PLOUGH" == *Type || "PLOUGHING" == *Type
                || "Plough" == *Type  || "Ploughing" == *Type) {
            FO -> DefineOp(Ploughing, crop_id, cropName, PP -> GetYear(date) + yearOffset, PP -> GetMonth(date),
                           PP -> GetDay(date), PP -> GetArea(), 0, PP -> GetDistance());
        } else {
            FO -> DefineOp(StubHarrowing, crop_id, cropName, PP -> GetYear(date) + yearOffset, PP -> GetMonth(date),
                           PP -> GetDay(date), PP -> GetArea(), 0, PP -> GetDistance());
        }

        FO -> SetType(*Type);
        FO -> SetDepth(*PP -> GetTillageDepth(i));
        FO -> SetMinDepth(*PP -> GetTillageMinDepth(i));

        // FO->SetPlantName(*PP->GetTillagePlantItem(i));
        FO -> SetFractionIncorporated(*PP -> GetTillageIncorporation(i));
        AddOpList(FO);
    }

    sowFields * FO        = new sowFields;
    int         oldyear   = 1900;
    int         oldmonth  = 1;
    int         oldday    = 1;
    bool        anySowing = false;

    for (int i = 0; i < PP -> GetNumOfSowCrops(); i++) {    // Sow
        anySowing = true;

        string * date   = PP -> GetSowDate(i);
        double   amount = PP -> GetSowAmount(i);
        int      year   = PP -> GetYear(date) + yearOffset;
        int      month  = PP -> GetMonth(date);
        int      day    = PP -> GetDay(date);
        seed *   aSeed  = new seed;

        /*
         * NJH May 2009, crop_identifier truncates the name of the crop sown to identify a particular variety. This allows
         * the variety to be passed via seed name for specific operations. The variety name must start with two characters that
         * are a valid crop type
         */
        string crop_identifier = PP -> GetSowCrop(i) -> c_str();

        crop_identifier.resize(2);
        aSeed -> Setname(GetSeedName(crop_identifier));
        theProducts -> GiveProductInformation(aSeed);
        aSeed -> Setamount(amount);

        if ((oldyear == year) && (oldmonth == month) && (oldday == day)) {
            FO -> AddSeed(aSeed, PP -> GetSowCrop(i) -> c_str());
        } else {
            if (oldyear > 1900) {
                AddOpList(FO);

                FO = new sowFields;
            }

            bsTime t1, t2;

            t1.SetTime(oldday, oldmonth, oldyear);
            t2.SetTime(day, month, year);

            if (!(t1 <= t2)) {
                theMessage -> FatalError(
                    "staticCrop::SpecifiedPlan - sowing dates must be given in non-decreasing order");
            }

              FO -> DefineSowFieldsOp(Sowing, crop_identifier, (string) PP -> GetSowCrop(i) -> c_str(), year, month, day,
                                    PP -> GetArea(), 0, PP -> GetDistance(), PP -> GetPVStrat(), 1.0, aSeed);
        }

        delete aSeed;

        oldyear  = year;
        oldmonth = month;
        oldday   = day;
    }

    if (anySowing) {
        AddOpList(FO);
    } else {
        delete FO;
    }

    int mincount = 0;
    int mancount = 0;

    for (int i = 0; i < PP -> GetNumOfFertilizerDates(); i++) {    // Fertilize
        string * date = PP -> GetFertilizerDate(i);
        string * type = PP -> GetFertilizerType(i);

        if (*type == "MINERAL") {
            fertilizer * aFertilizer1 = new fertilizer;

            aFertilizer1 -> Setname("MINERAL-N");
            theProducts -> GiveProductInformation(aFertilizer1);
            aFertilizer1 -> Setamount(0.0);
            aFertilizer1 -> SetMineralForm(PP -> GetMineralForm(mincount));

            if (PP -> GetFertilizerN(mincount).n > 0.0) {
                aFertilizer1 -> Setamount(PP -> GetFertilizerN(mincount) / aFertilizer1 -> GetN_content());

                double   N15ratio = PP -> GetFertilizerN(mincount).Get15NRatio();
                nitrogen Nfertilizer;

                Nfertilizer.SetBoth(aFertilizer1 -> GetN_content().n, N15ratio * aFertilizer1 -> GetN_content().n);
                aFertilizer1 -> SetN_content(Nfertilizer);
                aFertilizer1 -> SetNH4_fraction(PP -> GetNH4_fraction(mincount));
            }

            if (PP -> GetFertilizerCl(mincount) > 0.0) {    // Chloride is anly used as a tracer in leaching studies
                if (aFertilizer1 -> GetAmount() == 0) {
                    aFertilizer1 -> Setamount(1.0);
                }

                aFertilizer1 -> SetCl_content(PP -> GetFertilizerCl(mincount) / aFertilizer1 -> GetAmount());
            }

            fertilizer * aFertilizer2 = new fertilizer;

            aFertilizer2 -> Setname("MINERAL-P");
            theProducts -> GiveProductInformation(aFertilizer2);
            aFertilizer2 -> Setamount(0.0);

            if (PP -> GetFertilizerP(mincount) > 0.0) {
                aFertilizer2 -> Setamount(PP -> GetFertilizerP(mincount) / aFertilizer2 -> GetP_content());
            }

            fertilizer * aFertilizer3 = new fertilizer;

            aFertilizer3 -> Setname("MINERAL-K");
            theProducts -> GiveProductInformation(aFertilizer3);
            aFertilizer3 -> Setamount(0.0);

            if (PP -> GetFertilizerK(mincount) > 0.0) {
                aFertilizer3 -> Setamount(PP -> GetFertilizerK(mincount) / aFertilizer3 -> GetK_content());
            }

            mineralFertFields * FO = new mineralFertFields;

            FO -> DefineMineralFertFieldsOp(MineralFertilizing, crop_id, cropName, PP -> GetYear(date) + yearOffset,
                                            PP -> GetMonth(date), PP -> GetDay(date), PP -> GetArea(), 0,
                                            PP -> GetDistance(), aFertilizer1, aFertilizer2, aFertilizer3);
            AddOpList(FO);

            delete aFertilizer1;
            delete aFertilizer2;
            delete aFertilizer3;

            mincount++;
        } else {
            manure * aManure = new manure;

            aManure -> Setname(*type);
            theProducts -> GiveProductInformation(aManure);

            *aManure = *PP -> GetManure(mancount);    // Problems here !!!

            animalFertFields * FO = new animalFertFields;

            FO -> DefineAnimalFertFieldsOp(SpreadingLiquidManure, crop_id, cropName, PP -> GetYear(date) + yearOffset,
                                           PP -> GetMonth(date), PP -> GetDay(date), PP -> GetArea(), 0,
                                           PP -> GetDistance(), aManure);
            AddOpList(FO);

            delete aManure;

            mancount++;
        }
    }

    for (int i = 0; i < PP -> GetNumOfIrrigationDates(); i++) {    // Irrigate
        string * date   = PP -> GetIrrigationDate(i);
        water *  aWater = new water;

        aWater -> Setname("WATER");
        theProducts -> GiveProductInformation(aWater);
        aWater -> Setamount(PP -> GetIrrigationAmount(i) * 10);    // convert to 1 ha
        aWater -> SetN_content(PP -> GetIrrigationNcontent(i));

        irrigateFields * FO = new irrigateFields;

        FO -> DefineIrrigateFieldsOp(Irrigating, crop_id, cropName, PP -> GetYear(date) + yearOffset,
                                     PP -> GetMonth(date), PP -> GetDay(date), PP -> GetArea(), 0, PP -> GetDistance(),
                                     aWater, false);
        AddOpList(FO);

        delete aWater;
    }

    if ((PP -> GetNumOfIrrigationDates() == 0) && (JB_NO <= 4)
            && irrigation) {    // Automatic irrigate 30 mm on sandy soils
        water * aWater = new water;

        aWater -> Setname("WATER");
        theProducts -> GiveProductInformation(aWater);
        aWater -> Setamount(30 * 10);    // 30mm, 1ha

        irrigateFields * FO = new irrigateFields;

        FO -> DefineIrrigateFieldsOp(Irrigating, crop_id, cropName, PP -> GetStartYear() + 1, 6, 20, PP -> GetArea(),
                                     0, PP -> GetDistance(), aWater, true);
        AddOpList(FO);

        delete aWater;
    }

    // Pesticides (currently only implemented to support killing of crops by round-up
    for (int i = 0; i < PP -> GetNumOfPesticidesDates(); i++) {
        string *   date          = PP -> GetPesticideDate(i);
        chemical * aChemical1    = new chemical;
        string *   PesticideName = PP -> GetPesticideType(i);
        string     aName         = *PesticideName;

        aChemical1 -> Setname(aName);
        theProducts -> GiveProductInformation(aChemical1);

        water * aWater = new water;

        aWater -> Setname("WATER");
        theProducts -> GiveProductInformation(aWater);
        aWater -> Setamount(400);    // ???

        plantCareFields * FO = new plantCareFields;

        FO -> DefinePlantCareFieldsOp(Spraying, crop_id, cropName, PP -> GetYear(date) + yearOffset,
                                      PP -> GetMonth(date), PP -> GetDay(date), PP -> GetArea(), 0,
                                      PP -> GetDistance(), aChemical1, nullptr, nullptr, aWater);
        AddOpList(FO);

        delete aChemical1;
        delete aWater;
    }

    // Cutting of clover-grass or other green manure
    for (int i = 0; i < PP -> GetNumOfCuttingDates(); i++) {
        string * date = PP -> GetCuttingDate(i);

        // aPlantItem1->Setamount(ExpectedYield_(JB_NO,PP->GetNLevel(),PP->GetPVStrat()));
        harvestFields * FO            = new harvestFields;
        double          CuttingHeight = *PP -> GetCuttingHeights(i);

        // NJH wrote from here - BMP changed meaning of "GetCuttingProcessProducts" May 2007 - NHJ CHECK this !!!!!!
        harvestFields * FO1 = nullptr;

        // NJH removed next line (duplication of subsequent lines in if/case)
        // FO->DefineHarvestFieldsOper(ForageHarvesting,crop_id,cropName,PP->GetYear(date)+yearOffset,PP->GetMonth(date),PP->GetDay(date),PP->GetArea(),0,PP->GetDistance(),1.0,true,PP->GetFractionStrawHarvested(0),PP->GetFractionGrainHarvested(0));
        if (PP -> GetCuttingRemovesStraw(i)) {
            FO -> DefineHarvestFieldsOper(ForageHarvesting, crop_id, cropName, PP -> GetYear(date) + yearOffset,
                                          PP -> GetMonth(date), PP -> GetDay(date), PP -> GetArea(), 0,
                                          PP -> GetDistance(), 1.0, true, PP -> GetFractionStrawHarvested(0),
                                          PP -> GetFractionGrainHarvested(0));
            FO -> SetForageProduct(aPlantItem1, 0);

            if (aPlantItem2) {
                FO -> SetForageProduct(aPlantItem2, 1);
            }
        } else {
            switch (*PP -> GetCuttingProcessProducts(i)) {
                case 0 :
                    FO -> DefineHarvestFieldsOper(CutSetAside, crop_id, cropName, PP -> GetYear(date) + yearOffset,
                                                  PP -> GetMonth(date), PP -> GetDay(date), PP -> GetArea(), 0,
                                                  PP -> GetDistance(), 1.0, true, PP -> GetFractionStrawHarvested(0),
                                                  PP -> GetFractionGrainHarvested(0));

                    break;

                case 1 :
                    FO -> DefineHarvestFieldsOper(Mowing, crop_id, cropName, PP -> GetYear(date) + yearOffset,
                                                  PP -> GetMonth(date), PP -> GetDay(date), PP -> GetArea(), 0,
                                                  PP -> GetDistance(), 1.0, true, PP -> GetFractionStrawHarvested(0),
                                                  PP -> GetFractionGrainHarvested(0));
                    FO -> SetForageProduct(aPlantItem1, 0);

                    if (aPlantItem2) {
                        FO -> SetForageProduct(aPlantItem2, 1);
                    }

                    // collect silage
                    FO1 = new harvestFields;

                    FO1 -> DefineHarvestFieldsOper(GatheringSilage, crop_id, cropName,
                                                   PP -> GetYear(date) + yearOffset, PP -> GetMonth(date),
                                                   PP -> GetDay(date) + 1, PP -> GetArea(), 0, PP -> GetDistance(),
                                                   1.0, true, PP -> GetFractionStrawHarvested(0),
                                                   PP -> GetFractionGrainHarvested(0));

                    break;

                case 2 :
                    FO -> DefineHarvestFieldsOper(Mowing, crop_id, cropName, PP -> GetYear(date) + yearOffset,
                                                  PP -> GetMonth(date), PP -> GetDay(date), PP -> GetArea(), 0,
                                                  PP -> GetDistance(), 1.0, true, PP -> GetFractionStrawHarvested(0),
                                                  PP -> GetFractionGrainHarvested(0));
                    FO -> SetForageProduct(aPlantItem1, 0);

                    if (aPlantItem2) {
                        FO -> SetForageProduct(aPlantItem2, 1);
                    }

                    FO1 = new harvestFields;

                    FO1 -> DefineHarvestFieldsOper(GatheringHay, crop_id, cropName, PP -> GetYear(date) + yearOffset,
                                                   PP -> GetMonth(date), PP -> GetDay(date) + 1, PP -> GetArea(), 0,
                                                   PP -> GetDistance(), 1.0, true, PP -> GetFractionStrawHarvested(0),
                                                   PP -> GetFractionGrainHarvested(0));

                    break;
				case 3:
					FO->DefineHarvestFieldsOper(Mowing, crop_id, cropName, PP->GetYear(date) + yearOffset,
						PP->GetMonth(date), PP->GetDay(date), PP->GetArea(), 0,
						PP->GetDistance(), 1.0, true, PP->GetFractionStrawHarvested(0),
						PP->GetFractionGrainHarvested(0));
					FO->SetForageProduct(aPlantItem1, 0);

					if (aPlantItem2) {
						FO->SetForageProduct(aPlantItem2, 1);
					}

					// collect silage
					FO1 = new harvestFields;

					FO1->DefineHarvestFieldsOper(StrawToField, crop_id, cropName,
						PP->GetYear(date) + yearOffset, PP->GetMonth(date),
						PP->GetDay(date) + 1, PP->GetArea(), 0, PP->GetDistance(),
						1.0, true, PP->GetFractionStrawHarvested(0),
						PP->GetFractionGrainHarvested(0));

					break;
                default :
                    theMessage -> FatalError("static crop: process product code invalid");
            }
        }

        // - to here
        FO -> SetCuttingHeight(CuttingHeight);

        // FO->SetForageProduct(aPlantItem1);
        AddOpList(FO);

        if (FO1) {    // NJH added this
            AddOpList(FO1);
        }
    }

    {                 // manual registrations - hour for spring and summer
        manualLabourFields * FO           = new manualLabourFields;
        double               HoursPerUnit = 0.7;

        if (PP -> GetPVStrat() == 0) {
            HoursPerUnit = 1.0;
        }

        FO -> DefineManualLabourFieldsOp(MonitoringCrop, crop_id, cropName, PP -> GetStartYear() + 1, 5, 1,
                                         PP -> GetArea(), 0, PP -> GetDistance(), 0, HoursPerUnit);
        AddOpList(FO);
    }

    if ((PP -> GetNumOfHarvestDates() > 0) && (crop_id != "BE")
            && (crop_id != "PO")) {    // Beet is not harvested with combine harvester
        string s = crop_id;

        if ((s == "F1") || (s == "F2") || (s == "G1") || (s == "G2") || (s == "C1") || (s == "C2") || (s == "K1")
                || (s == "K2")) {
            for (int i = 0; i < PP -> GetNumOfHarvestDates(); i++) {
                string * date = PP -> GetHarvestDate(i);

                // aPlantItem1->Setamount(ExpectedYield_(JB_NO,PP->GetNLevel(),PP->GetPVStrat()));
                harvestFields * FO = new harvestFields;

                if ((s == "G1") || (s == "G2")) {    // Special option for seedgrass !!!!!!!!!!!!!!!!!!!!!! -> Harvest

                    // aPlantItem1->Setamount(ExpectedYield_(JB_NO,PP->GetNLevel(),PP->GetPVStrat()));
                    harvestFields * FO = new harvestFields;

                    if (gatheringStraw) {
                        FO -> DefineHarvestFieldsOper(CombineHarvesting, crop_id, cropName,
                                                      PP -> GetYear(date) + yearOffset, PP -> GetMonth(date),
                                                      PP -> GetDay(date), PP -> GetArea(), 0, PP -> GetDistance(), 1.0,
                                                      true, PP -> GetFractionStrawHarvested(0),
                                                      PP -> GetFractionGrainHarvested(0));
                    } else {
                        FO -> DefineHarvestFieldsOper(CombineHarvestingShred, crop_id, cropName,
                                                      PP -> GetYear(date) + yearOffset, PP -> GetMonth(date),
                                                      PP -> GetDay(date), PP -> GetArea(), 0, PP -> GetDistance(), 1.0,
                                                      true, PP -> GetFractionStrawHarvested(0),
                                                      PP -> GetFractionGrainHarvested(0));
                    }

                    AddOpList(FO);
                } else {
                    double CuttingHeight = 0.1;

                    if (PP -> GetCuttingHeights(i)) {
                        CuttingHeight = *PP -> GetCuttingHeights(i);
                    }

                    if ((s == "F1") || (s == "F2")) {
                        theMessage -> WarningWithDisplay(
                            "staticCrop::SpecifiedPlan - combine harvesting on fallow. Changed to cutting");
                        FO -> DefineHarvestFieldsOper(CutSetAside, crop_id, cropName, PP -> GetYear(date) + yearOffset,
                                                      PP -> GetMonth(date), PP -> GetDay(date), PP -> GetArea(), 0,
                                                      PP -> GetDistance(), 1.0, true,
                                                      PP -> GetFractionStrawHarvested(0),
                                                      PP -> GetFractionGrainHarvested(0));
                    } else {
                        theMessage -> WarningWithDisplay(
                            "staticCrop::SpecifiedPlan - combine harvesting on grass, grass clover or clover. Changed to forage harvesting");
                        FO -> DefineHarvestFieldsOper(ForageHarvesting, crop_id, cropName,
                                                      PP -> GetYear(date) + yearOffset, PP -> GetMonth(date),
                                                      PP -> GetDay(date), PP -> GetArea(), 0, PP -> GetDistance(), 1.0,
                                                      true, PP -> GetFractionStrawHarvested(0),
                                                      PP -> GetFractionGrainHarvested(0));
                    }

                    FO -> SetCuttingHeight(CuttingHeight);
                    FO -> SetForageProduct(aPlantItem1);
                    AddOpList(FO);
                }
            }
        } else {
			for (int i = 0; i < PP->GetNumOfHarvestDates(); i++)
			{
				int      year, month, day;
				string * date = PP->GetHarvestDate(i);    // is always the first!!!!
			
				year = PP->GetYear(date) + yearOffset;
				month = PP->GetMonth(date);
				day = PP->GetDay(date);

				if (!aPlantItem1) {
					theMessage->FatalError("staticCrop::SpecifiedPlan - about to call a method with a nullptr pointer");
				}

				// aPlantItem1->Setamount(ExpectedYield_(JB_NO,PP->GetNLevel(),PP->GetPVStrat()));
				harvestFields * FO = new harvestFields;

				if (gatheringStraw) {
					FO->DefineHarvestFieldsOper(CombineHarvesting, crop_id, cropName, year, month, day, PP->GetArea(),
						0, PP->GetDistance(), 1.0, true, PP->GetFractionStrawHarvested(i),
						PP->GetFractionGrainHarvested(i));
				}
				else {
					FO->DefineHarvestFieldsOper(CombineHarvestingShred, crop_id, cropName, year, month, day,
						PP->GetArea(), 0, PP->GetDistance(), 1.0, true,
						PP->GetFractionStrawHarvested(i), PP->GetFractionGrainHarvested(i));
				}

				FO->SetMainProduct(aPlantItem1);
				FO->SetStrawProduct(aPlantItem2);
				AddOpList(FO);

				if (gatheringStraw) {    // Gathering straw
					bsTime bs;

					bs.SetTime(day, month, year);
					bs.AddOneDay();

					harvestFields * FO1 = new harvestFields;

					FO1->DefineHarvestFieldsOper(GatheringBales, crop_id, cropName, bs.GetYear(), bs.GetMonth(),
						bs.GetDay(), PP->GetArea(), 0, PP->GetDistance(), 1.0, true,
						PP->GetFractionStrawHarvested(0), PP->GetFractionGrainHarvested(0));
					FO1->SetStrawProduct(aPlantItem2);
					AddOpList(FO1);
				}
			}
        }
    }

    for (int i = 0; i < PP -> GetNumSilageDates(); i++) {    // NJH wants this loop removed - why ???!!!
        string *       date = PP -> GetSilageDate(i);
        silageFields * FO   = new silageFields;

        // aPlantItem2->Setamount(ExpectedYield_(JB_NO,PP->GetNLevel(),PP->GetPVStrat()));
        FO -> DefinesilageFieldsOper(MakeSilage, crop_id, cropName, PP -> GetYear(date), PP -> GetMonth(date),
                                     PP -> GetDay(date), PP -> GetArea(), 0, PP -> GetDistance(), aPlantItem2);
        AddOpList(FO);
    }

    // JBE + BMP simple grazing
    for (int i = 0; i < PP -> GetNumOfGrazingDates(); i++) {    // Grazing
        grazingFields * FO   = new grazingFields;
        string *        date = PP -> GetGrazingStartDate(i);

        FO -> DefineGrazingOp(StartGrazing, crop_id, cropName, PP -> GetYear(date), PP -> GetMonth(date),
                              PP -> GetDay(date), PP -> GetArea(), 0, PP -> GetNitrogenLoad(i),
                              PP -> GetGrazingEfficiency(i));
        AddOpList(FO);

        grazingFields * FO1 = new grazingFields;

        date = PP -> GetGrazingEndDate(i);

        FO1 -> DefineGrazingOp(EndGrazing, crop_id, cropName, PP -> GetYear(date), PP -> GetMonth(date),
                               PP -> GetDay(date), PP -> GetArea(), 0, 0, 0);
        AddOpList(FO1);
    }
	for (int i = 0; i < PP->GetNumOfAnimalOnFieldStart(); i++) {
		AnimalonField * PutAnimalOnField = new AnimalonField();
		bsTime convert;
		convert.SetTime(*PP->getAnimalOnFieldStart(i));
		PutAnimalOnField->DefineAnimalonFieldOp(PutAnimalonField, convert.GetYear(), convert.GetMonth(), convert.GetDay(),*PP->getAnimalOnFieldType(i),PP->getAnimalOnFieldAmount(i));
		AddOpList(PutAnimalOnField);
	}
	for (int i = 0; i < PP->GetNumOfAnimalOnFieldStop(); i++) {
		removeAnimalOnField * removeAnimal = new removeAnimalOnField();
	
		bsTime convert;
		convert.SetTime(*PP->getAnimalOnFieldStop(i));
		removeAnimal->DefineAnimalonFieldOp(removeAnimalFromField, convert.GetYear(), convert.GetMonth(), convert.GetDay(), *PP->getAnimalOnFieldType(i), PP->getAnimalOnFieldAmountStop(i));
		AddOpList(removeAnimal);
	}
}

/*
 *
 * Parameters values per ha.
 *
 */
cloneList<fieldOperationFields> * staticCrop::MakeOpList(fieldOrder * PP,
        int                                                           JB_NO) {

#ifdef DEVELOPING
    reduction = 0;
#endif

    ResetOpList();

    if (!PP -> GetAutomatic()) {
        string tmp = "NO";

        SpecifiedPlan(PP, JB_NO, tmp);    // This should only be called with NO crops on field!
    }

    if (LP -> NumOfNodes() == 0) {
        int yearOffset = 0;

#ifdef __ROTATIONGENERATION
        yearOffset = theControlParameters -> GetoperationOffset();
#endif

        soilTreatFields * FO = new(soilTreatFields);

        FO -> DefineOp(EmptyOperation, "  ", cropName, PP -> GetStartYear() + yearOffset, 9, 1, PP -> GetArea(), 0,
                       PP -> GetDistance());
        AddOpList(FO);
    }

    return LP;
}

double staticCrop::NitrogenAmount(int    JB_NO,
                                  double free_N,
                                  int    N_level) {
    return 0;
}

double staticCrop::ExpectedYield_(int JB_NO,
                                  int N_level,
                                  int PV_strat) {
    return 0;
}

double staticCrop::ExpectedStrawYield_(int JB_NO,
        int                                N_level,
        int                                PV_strat) {
    return 0;
}

void staticCrop::SetAdjustArea(cloneList<fieldOperationFields> * OP,
                               fieldOrder *                      PP) {
    LP = OP;

    soilTreatFields * FO = new(soilTreatFields);

    FO -> DefineOp(EmptyOperation, "ADJUSTAREA", "", PP -> GetStartYear(), 8, 1, PP -> GetAdjustArea(), 0,
                   PP -> GetDistance());
    AddOpList(FO);
}

/*
 *
 * Replaces unknown crops with know ones.
 * Replacement specifications are read from a file.
 * A hack (!!!) to get movin' in FarmN.
 * Parameters
 * PP   object with specifications
 *
 */
void staticCrop::CorrectCropId(string crop_id) {
    string s1, s2, crp;

    crp = crop_id;

    fstream f;

    f.open("c:\\fasset_v2\\replacements.txt", ios::in);

    bool found = false;

    while (f) {
        f >> s1 >> s2;

        if (crp == s1) {
            crp   = s2;
            found = true;

            break;
        }
    }

    if (found) {
        crop_id[0] = s2[0];
        crop_id[1] = s2[1];
    }

    f.close();
}

#ifdef MEASCOPE
void staticCrop::GetMineralFertilizer(fieldOrder * PP,
        int                                        mincount,
        fertilizer *                               aFertilizer1,
        fertilizer *                               aFertilizer2,
        fertilizer *                               aFertilizer3) {
    aFertilizer1 -> Setname("MINERAL-N");
    theProducts -> GiveProductInformation(aFertilizer1);
    aFertilizer1 -> Setamount(0.0);
    aFertilizer1 -> SetMineralForm(PP -> GetMineralForm(mincount));

    if (PP -> GetFertilizerN(mincount).n > 0.0) {
        aFertilizer1 -> Setamount(PP -> GetFertilizerN(mincount) / aFertilizer1 -> GetN_content());

        double   N15ratio = PP -> GetFertilizerN(mincount).Get15NRatio();
        nitrogen Nfertilizer;

        Nfertilizer.SetBoth(aFertilizer1 -> GetN_content().n, N15ratio * aFertilizer1 -> GetN_content().n);
        aFertilizer1 -> SetN_content(Nfertilizer);
        aFertilizer1 -> SetNH4_fraction(PP -> GetNH4_fraction(mincount));
    }

    if (PP -> GetFertilizerCl(mincount) > 0.0) {    // Chloride is anly used as a tracer in leaching studies
        if (aFertilizer1 -> GetAmount() == 0) {
            aFertilizer1 -> Setamount(1.0);
        }

        aFertilizer1 -> SetCl_content(PP -> GetFertilizerCl(mincount) / aFertilizer1 -> GetAmount());
    }

    aFertilizer2 -> Setname("MINERAL-P");
    theProducts -> GiveProductInformation(aFertilizer2);
    aFertilizer2 -> Setamount(0.0);

    if (PP -> GetFertilizerP(mincount) > 0.0) {
        aFertilizer2 -> Setamount(PP -> GetFertilizerP(mincount) / aFertilizer2 -> GetP_content());
    }

    aFertilizer3 -> Setname("MINERAL-K");
    theProducts -> GiveProductInformation(aFertilizer3);
    aFertilizer3 -> Setamount(0.0);

    if (PP -> GetFertilizerK(mincount) > 0.0) {
        aFertilizer3 -> Setamount(PP -> GetFertilizerK(mincount) / aFertilizer3 -> GetK_content());
    }
}

/*
 *
 * This function is not finished!!!@@@
 *
 */
void staticCrop::SpecifiedPeriods(fieldOrder * PP,
                                  int          JB_NO,
                                  string       crop_id) {
    theMessage -> FatalError("staticCrop::SpecifiedPeriods - needs to be modified and tested for new cutting process");

    int yearOffset = theControlParameters -> GetoperationOffset();

    if (PP -> GetNumOfHarvestPeriods() > 0) {
        gatheringStraw = PP -> GetHarvestRemoveStraw(0);    // is straw gathered
    }

    for (int i = 0; i < PP -> GetNumOfTillagePeriods(); i++) {    // Plough
        soilTreatFields *  FO      = new soilTreatFields;
        fieldOrderPeriod * aPeriod = PP -> GetTillagePeriod(i);
        string *           sdate   = aPeriod -> GetStartDate();
        string *           edate   = aPeriod -> GetEndDate();
        string *           Type    = PP -> GetTillageType(i);

        if ("PLOUGH" == *Type || "PLOUGHING" == *Type
                        || "Plough" == *Type  || "Ploughing" == *Type) {
            FO -> DefineOp(Ploughing, crop_id, cropName, PP -> GetYear(sdate) + yearOffset, PP -> GetMonth(sdate),
                           PP -> GetDay(sdate), PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                           PP -> GetDay(edate), aPeriod -> GetTSum(), aPeriod -> GetsoilMoisture(),
                           aPeriod -> Getupper(), PP -> GetArea(), 0, PP -> GetDistance());
        } else {
            FO -> DefineOp(StubHarrowing, crop_id, cropName, PP -> GetYear(sdate) + yearOffset, PP -> GetMonth(sdate),
                           PP -> GetDay(sdate), PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                           PP -> GetDay(edate), aPeriod -> GetTSum(), aPeriod -> GetsoilMoisture(),
                           aPeriod -> Getupper(), PP -> GetArea(), 0, PP -> GetDistance());
        }

        AddOpList(FO);
        FO -> SetType(*Type);
        FO -> SetDepth(*PP -> GetTillageDepth(i));
        FO -> SetFractionIncorporated(*PP -> GetTillageIncorporation(i));
    }

    if (PP -> GetNumOfSowCrops() > 0) {
        sowFields *        FO         = new sowFields;
        bool               anySowing  = false;
        fieldOrderPeriod * tempPeriod = nullptr;

        for (int i = 0; i < PP -> GetNumOfSowCrops(); i++) {    // Sow
            fieldOrderPeriod * aPeriod;

            anySowing = true;

            seed * aSeed = new seed;

            aSeed -> Setname(GetSeedName((string) PP -> GetSowCrop(i) -> c_str()));
            theProducts -> GiveProductInformation(aSeed);

            double amount = PP -> GetSowAmount(i);

            aSeed -> Setamount(amount);

            // string * date;
            aPeriod = PP -> GetSowPeriod(i);

            bool newSowing = true;

            if (tempPeriod) {
                if (*aPeriod != *tempPeriod) {
                    FO = new sowFields;

                    delete tempPeriod;

                    tempPeriod = new fieldOrderPeriod(*aPeriod);
                } else {
                    FO -> AddSeed(aSeed, (string) PP -> GetSowCrop(i) -> c_str());

                    newSowing = false;
                }
            } else {
                tempPeriod = new fieldOrderPeriod(*aPeriod);
            }

            if (newSowing) {
                string * sdate = aPeriod -> GetStartDate();
                string * edate = aPeriod -> GetEndDate();

                FO -> DefineSowFieldsOp(Sowing, (string) PP -> GetSowCrop(i) -> c_str(),
                                        (string) PP -> GetSowCrop(i) -> c_str(), PP -> GetYear(sdate) + yearOffset,
                                        PP -> GetMonth(sdate), PP -> GetDay(sdate), PP -> GetYear(edate) + yearOffset,
                                        PP -> GetMonth(edate), PP -> GetDay(edate), aPeriod -> GetTSum(),
                                        aPeriod -> GetsoilMoisture(), aPeriod -> Getupper(), PP -> GetArea(), 0,
                                        PP -> GetDistance(), PP -> GetPVStrat(), 1.0, aSeed);
                AddOpList(FO);
            }

            delete aSeed;
        }

        if (!anySowing) {
            delete FO;
        }

        if (tempPeriod) {
            delete tempPeriod;
        }
    }

    int mincount  = 0;
    int mancount  = 0;
    int fertcount = PP -> GetNumOfFertilizerPeriods();

    for (int i = 0; i < fertcount; i++) {    // Fertilize
        fieldOrderPeriod * aPeriod    = PP -> GetFertilizerPeriod(i);
        string *           sdate      = aPeriod -> GetStartDate();
        string *           edate      = aPeriod -> GetEndDate();
        string *           type       = PP -> GetFertilizerType(i);
        double *           incorpTime = PP -> GetIncorporationTimes(i);

        if (*type == "MINERAL") {
            fertilizer * aFertilizer1 = new fertilizer;
            fertilizer * aFertilizer2 = new fertilizer;
            fertilizer * aFertilizer3 = new fertilizer;

            GetMineralFertilizer(PP, mincount, aFertilizer1, aFertilizer2, aFertilizer3);

            mineralFertFields * FO = new mineralFertFields;

            FO -> DefineMineralFertFieldsOp(MineralFertilizing, crop_id, cropName, PP -> GetYear(sdate) + yearOffset,
                                            PP -> GetMonth(sdate), PP -> GetDay(sdate),
                                            PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                                            PP -> GetDay(edate), aPeriod -> GetTSum(), aPeriod -> GetsoilMoisture(),
                                            aPeriod -> Getupper(), PP -> GetArea(), 0, PP -> GetDistance(),
                                            aFertilizer1, aFertilizer2, aFertilizer3);
            AddOpList(FO);

            delete aFertilizer1;
            delete aFertilizer2;
            delete aFertilizer3;

            mincount++;
        } else {
            manure * aManure = new manure;

            aManure -> Setname(*type);
            theProducts -> GiveProductInformation(aManure);

            *aManure = *PP -> GetManure(mancount);

            animalFertFields * FO = new animalFertFields;

            if (*type == "SLURRY") {
                FO -> DefineAnimalFertFieldsOp(SpreadingLiquidManure, crop_id, cropName,
                                               PP -> GetYear(sdate) + yearOffset, PP -> GetMonth(sdate),
                                               PP -> GetDay(sdate), PP -> GetYear(edate) + yearOffset,
                                               PP -> GetMonth(edate), PP -> GetDay(edate), aPeriod -> GetTSum(),
                                               aPeriod -> GetsoilMoisture(), aPeriod -> Getupper(), PP -> GetArea(), 0,
                                               PP -> GetDistance(), aManure, *incorpTime);
            } else {
                FO -> DefineAnimalFertFieldsOp(SpreadingSolidManure, crop_id, cropName,
                                               PP -> GetYear(sdate) + yearOffset, PP -> GetMonth(sdate),
                                               PP -> GetDay(sdate), PP -> GetYear(edate) + yearOffset,
                                               PP -> GetMonth(edate), PP -> GetDay(edate), aPeriod -> GetTSum(),
                                               aPeriod -> GetsoilMoisture(), aPeriod -> Getupper(), PP -> GetArea(), 0,
                                               PP -> GetDistance(), aManure, *incorpTime);
            }

            AddOpList(FO);

            delete aManure;

            mancount++;
        }
    }

    for (int i = 0; i < PP -> GetNumOfIrrigationPeriods(); i++) {    // Irrigate
        water * aWater = new water;

        aWater -> Setname("WATER");
        theProducts -> GiveProductInformation(aWater);
        aWater -> Setamount(PP -> GetIrrigationAmount(i) * 10);      // convert to 1 ha
        aWater -> SetN_content(PP -> GetIrrigationNcontent(i));

        irrigateFields *   FO      = new irrigateFields;
        fieldOrderPeriod * aPeriod = PP -> GetIrrigationPeriod(i);

        FO -> DefineIrrigateFieldsOp(Irrigating, crop_id, cropName,
                                     PP -> GetYear(aPeriod -> GetStartDate()) + yearOffset,
                                     PP -> GetMonth(aPeriod -> GetStartDate()),
                                     PP -> GetDay(aPeriod -> GetStartDate()),
                                     PP -> GetYear(aPeriod -> GetEndDate()) + yearOffset,
                                     PP -> GetMonth(aPeriod -> GetEndDate()), PP -> GetDay(aPeriod -> GetEndDate()),
                                     aPeriod -> GetTSum(), aPeriod -> GetsoilMoisture(), aPeriod -> Getupper(),
                                     PP -> GetArea(), 0, PP -> GetDistance(), aWater, false);
        AddOpList(FO);

        delete aWater;
    }

    for (int i = 0; i < PP -> GetNumOfCuttingPeriods(); i++) {
        fieldOrderPeriod * aPeriod        = PP -> GetCuttingPeriod(i);
        string *           sdate          = aPeriod -> GetStartDate();
        string *           edate          = aPeriod -> GetEndDate();
        harvestFields *    FO             = new harvestFields;
        harvestFields *    FO1            = nullptr;
        double             CuttingHeight  = *PP -> GetCuttingHeights(i);
        int                processProduct = *PP -> GetCuttingProcessProducts(i);

        FO -> SetMainProduct(aPlantItem1);

        if (PP -> GetCuttingRemovesStraw(i)) {
            FO -> DefineHarvestFieldsOper(ForageHarvesting, crop_id, cropName, PP -> GetYear(sdate),
                                          PP -> GetMonth(sdate) + yearOffset, PP -> GetDay(sdate),
                                          PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                                          PP -> GetDay(edate), aPeriod -> GetTSum(), aPeriod -> GetsoilMoisture(),
                                          aPeriod -> Getupper(), PP -> GetArea(), 0, PP -> GetDistance(), 1.0, true,
                                          PP -> GetFractionStrawHarvested(0), PP -> GetFractionGrainHarvested(0));
        } else {
            switch (processProduct) {    // 0=cut set-aside, 1 = silage, 2 = hay
                case 0 :
                    FO -> DefineHarvestFieldsOper(CutSetAside, crop_id, cropName, PP -> GetYear(sdate) + yearOffset,
                                                  PP -> GetMonth(sdate), PP -> GetDay(sdate),
                                                  PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                                                  PP -> GetDay(edate), aPeriod -> GetTSum(),
                                                  aPeriod -> GetsoilMoisture(), aPeriod -> Getupper(), PP -> GetArea(),
                                                  0, PP -> GetDistance(), 1.0, true,
                                                  PP -> GetFractionStrawHarvested(0),
                                                  PP -> GetFractionGrainHarvested(0));

                    break;

                case 1 :
                    FO -> DefineHarvestFieldsOper(Mowing, crop_id, cropName, PP -> GetYear(sdate) + yearOffset,
                                                  PP -> GetMonth(sdate), PP -> GetDay(sdate),
                                                  PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                                                  PP -> GetDay(edate), aPeriod -> GetTSum(),
                                                  aPeriod -> GetsoilMoisture(), aPeriod -> Getupper(), PP -> GetArea(),
                                                  0, PP -> GetDistance(), 1.0, true,
                                                  PP -> GetFractionStrawHarvested(0),
                                                  PP -> GetFractionGrainHarvested(0));

                    // collect silage
                    FO1 = new harvestFields;

                    FO1 -> DefineHarvestFieldsOper(GatheringSilage, crop_id, cropName,
                                                   PP -> GetYear(sdate) + yearOffset, PP -> GetMonth(sdate),
                                                   PP -> GetDay(sdate) + 1, PP -> GetYear(edate) + yearOffset,
                                                   PP -> GetMonth(edate), PP -> GetDay(edate) + 1,
                                                   aPeriod -> GetTSum(), aPeriod -> GetsoilMoisture(),
                                                   aPeriod -> Getupper(), PP -> GetArea(), 0, PP -> GetDistance(), 1.0,
                                                   true, PP -> GetFractionStrawHarvested(0),
                                                   PP -> GetFractionGrainHarvested(0));
                    FO1 -> SetMainProduct(aPlantItem1);

                    break;

                case 2 :
                    FO -> DefineHarvestFieldsOper(Mowing, crop_id, cropName, PP -> GetYear(sdate) + yearOffset,
                                                  PP -> GetMonth(sdate), PP -> GetDay(sdate),
                                                  PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                                                  PP -> GetDay(edate), aPeriod -> GetTSum(),
                                                  aPeriod -> GetsoilMoisture(), aPeriod -> Getupper(), PP -> GetArea(),
                                                  0, PP -> GetDistance(), 1.0, true,
                                                  PP -> GetFractionStrawHarvested(0),
                                                  PP -> GetFractionGrainHarvested(0));

                    // collect silage
                    FO1 = new harvestFields;

                    FO1 -> DefineHarvestFieldsOper(GatheringHay, crop_id, cropName, PP -> GetYear(sdate) + yearOffset,
                                                   PP -> GetMonth(sdate), PP -> GetDay(sdate) + 1,
                                                   PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                                                   PP -> GetDay(edate) + 1, aPeriod -> GetTSum(),
                                                   aPeriod -> GetsoilMoisture(), aPeriod -> Getupper(),
                                                   PP -> GetArea(), 0, PP -> GetDistance(), 1.0, true,
                                                   PP -> GetFractionStrawHarvested(0),
                                                   PP -> GetFractionGrainHarvested(0));
                    FO1 -> SetMainProduct(aPlantItem1);

                    break;

                default :
                    theMessage -> FatalError(
                        "static crop - invalid process product in field no");    // , FO->GetFieldNumber());
            }
        }

        FO -> SetCuttingHeight(CuttingHeight);
        AddOpList(FO);

        if (FO1) {
            AddOpList(FO1);
        }
    }

    if (PP -> GetNumOfHarvestPeriods() > 0) {
        fieldOrderPeriod * aPeriod = PP -> GetHarvestPeriod(0);
        string *           sdate   = aPeriod -> GetStartDate();
        string *           edate   = aPeriod -> GetEndDate();
        harvestFields *    FO      = new harvestFields;

        if (gatheringStraw) {
            FO -> DefineHarvestFieldsOper(CombineHarvesting, crop_id, cropName, PP -> GetYear(sdate) + yearOffset,
                                          PP -> GetMonth(sdate), PP -> GetDay(sdate),
                                          PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                                          PP -> GetDay(edate), aPeriod -> GetTSum(), aPeriod -> GetsoilMoisture(),
                                          aPeriod -> Getupper(), PP -> GetArea(), 0, PP -> GetDistance(), 1.0, true,
                                          PP -> GetFractionStrawHarvested(0), PP -> GetFractionGrainHarvested(0));
        } else {
            FO -> DefineHarvestFieldsOper(CombineHarvestingShred, crop_id, cropName, PP -> GetYear(sdate) + yearOffset,
                                          PP -> GetMonth(sdate), PP -> GetDay(sdate),
                                          PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                                          PP -> GetDay(edate), aPeriod -> GetTSum(), aPeriod -> GetsoilMoisture(),
                                          aPeriod -> Getupper(), PP -> GetArea(), 0, PP -> GetDistance(), 1.0, true,
                                          PP -> GetFractionStrawHarvested(0), PP -> GetFractionGrainHarvested(0));
        }

        FO -> SetMainProduct(aPlantItem1);
        AddOpList(FO);

        if (gatheringStraw) {    // Gathering straw
            harvestFields * FO1 = new harvestFields;

            FO1 -> DefineHarvestFieldsOper(GatheringBales, crop_id, cropName, PP -> GetYear(sdate) + yearOffset,
                                           PP -> GetMonth(sdate), PP -> GetDay(sdate) + 1,
                                           PP -> GetYear(edate) + yearOffset, PP -> GetMonth(edate),
                                           PP -> GetDay(edate), aPeriod -> GetTSum(), aPeriod -> GetsoilMoisture(),
                                           aPeriod -> Getupper(), PP -> GetArea(), 0, PP -> GetDistance(), 1.0, true,
                                           PP -> GetFractionStrawHarvested(0), PP -> GetFractionGrainHarvested(0));
            FO1 -> SetStrawProduct(aPlantItem2);
            AddOpList(FO1);
        }
    }
}
#endif

