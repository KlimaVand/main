
#include "../base/common.h"
#include "cropRotation.h"
#include "../base/bstime.h"
#include "../base/climate.h"
#include "../base/IndicatorOutput.h"
#include "staticCrops/staticWwheat.h"
#include "staticCrops/staticSwheat.h"
#include "staticCrops/staticSbarley.h"
#include "staticCrops/staticWbarley.h"
#include "staticCrops/staticWrape.h"
#include "staticCrops/staticSrape.h"
#include "staticCrops/staticFallow.h"
#include "staticCrops/staticFallowPermanent.h"
#include "staticCrops/staticPea.h"
#include "staticCrops/staticBarleyPea.h"
#include "staticCrops/staticCloverGrass.h"
#include "staticCrops/staticSeedGrass.h"
#include "staticCrops/staticBeet.h"
#include "staticCrops/staticMaize.h"
#include "staticCrops/staticGrass.h"
#include "staticCrops/staticGrassEstablished.h"
#include "staticCrops/staticGrassPermanent.h"
#include "staticCrops/staticOat.h"
#include "staticCrops/staticRye.h"
#include "staticCrops/staticPotato.h"
#include "staticCrops/staticTriticale.h"
#include "staticCrops/staticCrimsonClover.h"
#include "staticCrops/staticFabaBean.h"
#include "../legislation/legislation.h"
#include "../base/message.h"
#include "staticCrops/staticClover.h"
#include "staticCrops/staticGrazingGrass.h"
#include "staticCrops/staticItalianRyeGrass.h"
#include "../base/controlParameters.h"
#include "staticCrops/staticLucerneEstablished.h"
#include "../base/settings.h"

cropRotation::cropRotation() {
    CropPointer = &staticBase;

    InitLegalTable();
    InitReducTable();

    Name        = "cropRotation";
    PvName      = "PEST";
    FertLevName = "F";
    FieldName   = "FIELD";
    Index       = 0;
}

cropRotation::~cropRotation() {
    fieldList.Reset();

    for (int i = 0; i < NumOfCrops(); i++) {
        if (CropArray[i].CP != &staticBase) {
            delete CropArray[i].CP;
        }
    }
}

double cropRotation::GetTotalArea() {
    double area = 0.0;
    int    NF   = NumOfFields();

    for (int i = 0; i < NF; i++) {
        linkList<field>::PS P;
        fieldList.SearchByNumber(P, i);

        area += P -> element -> FieldArea_();
    }

    return area;
}

/*
 * Updates the field object pointed to with currrent date and climate data.
 * PP   Pointer to field object
 */
void cropRotation::UpdateFld(field * PP) {
    field * fP = nullptr;

    PP -> SplitMe(fP);
    PP -> UpdateField();

    while (fP) {
        ReplaceField(fP);
        PP -> SplitMe(fP);
    }
}

/*
 * Inserts a field, whith no area. This hack (!!!) is made only to "reserve" the
 * field number for later, which ensures that the order in which crops occupy fields
 * does not depend on crop development (climate).
 */
void cropRotation::InsertGhostField() {
    field * f = new field("field", fieldList.NumOfNodes(), this);

    f -> SetActive(false);
    fieldList.InsertLast(f);
}

/*
 * Replaces a "ghost" field with a real field. See "InsertGhostField()" for
 * rationale.
 */
void cropRotation::ReplaceField(field * fP) {
    int num = fP -> ReturnNumber();

    linkList<field>::PS P;
    fieldList.SearchByNumber(P, num);

    if (P -> element -> GetActive()) {
        theMessage -> FatalError("cropRotation::ReplaceField - attempt to replace active field");
    }

    delete (P -> element);

    P -> element = fP;
}

/*
 * Creates cropping plan.
 * PP   object with specifications
 */
void cropRotation::MakeSequenceInstance(fieldOrder * PP) {
    cout << "cropRotation::MakeSequenceInstance - processing field no: " << PP -> GetFieldNo() << endl;

    SetCropPointer(PP -> GetCropId());

    int num;

    num = fieldList.ElementAtNumber(PP -> GetFieldNo()) -> ReturnNumber();

    int  futureNum = num;
    bool allreadyTaken;

    allreadyTaken = fieldList.ElementAtNumber(PP -> GetFieldNo()) -> FuturePlanExists();

    if (allreadyTaken) {
        futureNum = NumOfFields();

        InsertGhostField();
        theMessage -> WarningWithDisplay("cropRotation::MakeSequenceInstance - 'Ghost' field inserted");
    }

    // string name =PP->GetCropId();
    cloneList<fieldOperationFields> * OP = CropPointer -> MakeOpList(PP,
                                           fieldList.ElementAtNumber(PP -> GetFieldNo()) -> JBno());

    if (PP -> GetAdjustArea() > 0) {
        CropPointer -> SetAdjustArea(OP, PP);
    }

    fieldList.ElementAtNumber(PP -> GetFieldNo()) -> PlanField_(OP, PP -> GetCropId(), futureNum);

    // ths->AddAreaToIndicators(PP->GetCropId(),PP->GetArea(),soil_type_);
}

/*
 * Sets the "CropPointer" to point at the crop given by "crop_id".
 * crop_id   crop identification
 */
void cropRotation::SetCropPointer(string crop_id) {
    cout << "SetCropPointer:" << crop_id << endl;

    int No = GiveCropNo(crop_id);

    if ((No >= 0) && (No <= cattle_cropnum)) {
        CropPointer = CropArray[No].CP;
    } else {
        CropPointer = &staticBase;

        theMessage -> WarningWithDisplay("cropRotation::SetCropPointer - ", crop_id, " is illegal cropname ");
    }
}

/*
 * The update function which must be called on a daily basis.
 */
void cropRotation::DailyUpdate() {
    theOutput -> AddIndicator(7050, "70.50 Precipitation", "mm", theClimate -> precip);

    int n = fieldList.NumOfNodes();

    for (int i = 0; i < n; i++) {
        UpdateFld(fieldList.ElementAtNumber(i));
    }
}

/*
 * Initiates the table containing information about the legal crop sequences.
 * Colums: Old crop(s). Rows: New crop.
 * Extend with Spring wheat W5 !!!
 * Not updatet properly !!!
 */
void cropRotation::InitLegalTable() {
    int Lgl[cattle_cropnum][cattle_cropnum] =

        // W1 W2 RW F1 B1 B2 F2 B5 B6 PE BP C1 C2 BE B8 G1 G2 G4 G5 W3 B7 B8 B9
        {
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 2
            },    // W1
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 2
            },    // W2
            {
                1, 1, 4, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 8, 0, 0, 0, 0, 0, 2, 1, 2, 2
            },    // RW
            {
                0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 0, 2, 2
            },    // F1
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 2
            },    // B1
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 2
            },    // B2
            {
                1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2
            },    // F2
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 1, 2, 2
            },    // B5
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 1, 2, 2
            },    // B6
            {
                0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            },    // PE
            {
                0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
            },    // BP
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 2, 2, 2, 2, 2, 2, 0, 2, 2
            },    // C1
            {
                1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 2, 1, 2, 2
            },    // C2
            {
                0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 1, 2, 2
            },    // BE
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 2
            },    // B8
            {
                1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0
            },    // G1
            {
                1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0
            },    // G2
            {
                1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0
            },    // G4
            {
                1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0
            },    // G5
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 4, 4, 4, 4, 4, 5, 1, 5, 5
            },    // W3
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1
            },    // B7
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 5, 1, 5, 5
            },    // B8
            {
                0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 5, 1, 5, 5
            }
        };        // B9

    for (int i = 0; i < cattle_cropnum; i++) {
        for (int j = 0; j < cattle_cropnum; j++) {
            Legal[i][j] = Lgl[i][j];
        }
    }
}

/*
 *
 * A simple approach to the effects of the previous crop not related
 * to nutrient balance and release.
 * Colums: previous crop. Rows: New crop.
 * Extend with Spring wheat W5 !!!
 * Not updatet properly !!!
 *
 */
void cropRotation::InitReducTable() {
    double Red[cattle_cropnum][cattle_cropnum] =

        // W1   W2   RW   F1   B1   B2   F2   B5   B6   PE   BP   C1   C2   X2   G1   G2   G3   G4   G5   W3   B7   B8   B9
        {
            {
                0.90, 0.90, 1.00, 1.00, 0.90, 0.90, 1.00, 0.90, 0.90, 1.00, 1.00, 0.90, 0.90, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // W1
            {
                0.90, 0.90, 1.00, 1.00, 0.90, 0.90, 1.00, 0.90, 0.90, 1.00, 1.00, 0.90, 0.90, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // W2
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // RW
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // F1
            {
                0.95, 0.95, 1.00, 1.00, 0.95, 0.95, 1.00, 0.95, 0.95, 1.00, 1.00, 0.95, 0.95, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // B1
            {
                0.95, 0.95, 1.00, 1.00, 0.95, 0.95, 1.00, 0.95, 0.95, 1.00, 1.00, 0.95, 0.95, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // B2
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // F2
            {
                0.95, 0.95, 1.00, 1.00, 0.95, 0.95, 1.00, 0.95, 0.95, 1.00, 1.00, 0.95, 0.95, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // B5
            {
                0.95, 0.95, 1.00, 1.00, 0.95, 0.95, 1.00, 0.95, 0.95, 1.00, 1.00, 0.95, 0.95, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // B6
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // PE
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // BP
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // C1
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // C2
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // X2
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // G1
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // G2
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // G3
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // G4
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // G5
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // W3
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // B7
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            },    // B8
            {
                1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00, 1.00,
                1.00, 1.00, 1.00, 1.00, 1.00
            }
        };    // B9

    for (int i = 0; i < cattle_cropnum; i++) {
        for (int j = 0; j < cattle_cropnum; j++) {
            Reduc[i][j] = Red[i][j];
        }
    }
}

/*
 * Initiates all single fields with data which each instance of field reads
 * from the disk.
 * "min_area" contains the minimum area for one field.
 * Parameters:
 * filename   input file for single fields initialization
 * min_area   minimum area of a single field (ha)
 */
void cropRotation::InitSingleFields(bool WithoutFields,
                                    double                           min_area,
                                    string                           filename,
                                    int                              daysBetweenIrrigation,
                                    int                              irrigationDelay,
                                    int                              FingerFlow) {
    if (!WithoutFields) {
        int          index, first;
        int          num  = 0;
        commonData * data = new commonData();

        data -> setCritical(true);
        data -> readFile(filename);
        data -> getSectionsNumbers("field", &first, &num);

        int singleField = theControlParameters -> GetSingleField();

        if (singleField >= num) {
            theMessage -> FatalError("croprot::InitSingleFields - singleField requested for non-existent field");
        }

        for (index = first; index <= (first + num); index++) {
            if ((singleField == index) || (singleField < 0)) {
                field * fld = new field("field", index, this);

                fld -> SetFingerFlow(FingerFlow);
                fld -> ReadParameters(data);
                fld -> SetMinimumArea(min_area);
                fld -> SetDaysBetweenIrrigation(daysBetweenIrrigation);
                fld -> SetIrrigationDelay(irrigationDelay);
                fld -> StartBudget();

                if (singleField > 0) {
                    fld -> SetIndex(0);
                }

                fieldList.InsertLast(fld);
            }
        }

        delete data;

        theOutput -> SetMaxErrorN(0.01 * (1.0 + num - first));    // Change later - add 0.002 per ha !!!
        theOutput -> SetMaxErrorC(0.01 * (1.0 + num - first));    // Change later - add 0.002 per ha !!!
    }
}

/*
 *
 * The initialization function to be called prior to simulation.
 * Parameters:
 * irrigat           0->irrigation not possible;  1->irrigation possible.
 * cattl             0->No cattle on farm; 1->cattle on farm.
 * MinArea           minimum area for one field (ha).
 *
 */
void cropRotation::Initialize(bool   WithoutFields,
                              bool   irrigat,
                              int    daysBetweenIrrigation,
                              int    irrigationDelay,
                              int    FingerFlow,
                              double MinArea) {
    irrigate = irrigat;

    InitStaticCrops();
    InitSingleFields(WithoutFields, MinArea, "fields.dat", daysBetweenIrrigation, irrigationDelay, FingerFlow);
    thePeriods.Initialize("per_crpr.dat");
}

/*
 *
 * Returns the number corresponding to the crop name given by "crop_id".
 * Returns -1 by illegal "crop_id".
 * Parameters:
 * crop_id   crop identification
 *
 */
int cropRotation::GiveCropNo(string crop_id) {
    string s;
    int    j = -1;

    for (int i = 0; i < cattle_cropnum; i++) {
        CropId(i, s);

        if (crop_id == s) {
            j = i;

            break;
        }
    }

    return j;
}

/*
 *
 * Gives the name corresponding to the crop number given by "crop_no".
 * References an empty string by illegal "crop_no".
 * Parameters:
 * crop_no   crop number
 * crop_id   reference to crop identification
 *
 */
void cropRotation::CropId(int      crop_no,
                          string & crop_id) {
    if ((crop_no >= 0) && (crop_no < cattle_cropnum)) {
        crop_id = CropArray[crop_no].CrpNme;
    } else {
        theMessage -> WarningWithDisplay("cropRotation::CropId - Illegal cropnumber ", crop_id);

        crop_id = "";
    }
}

/*
 *
 * Initializes all static crop objects
 *
 */
void cropRotation::InitStaticCrops() {
    for (int i = 0; i < cattle_cropnum; i++) {
        CropArray[i].CrpNme = "";
        CropArray[i].CP     = &staticBase;
    }

    CropArray[0].CrpNme = "W1";
    CropArray[0].CP     = new staticWwheat;

    CropArray[0].CP -> SetGatheringStraw(false);

    CropArray[1].CrpNme = "W2";
    CropArray[1].CP     = new staticWwheat;

    CropArray[1].CP -> SetGatheringStraw(true);

    CropArray[2].CrpNme = "RW";
    CropArray[2].CP     = new staticWrape;
    CropArray[3].CrpNme = "F1";
    CropArray[3].CP     = new staticFallow;
    CropArray[4].CrpNme = "B1";
    CropArray[4].CP     = new staticSbarley();

    CropArray[4].CP -> SetGatheringStraw(false);

    CropArray[5].CrpNme = "B2";
    CropArray[5].CP     = new staticSbarley();

    CropArray[5].CP -> SetGatheringStraw(true);

    CropArray[6].CrpNme = "F2";
    CropArray[6].CP     = new staticFallowPermanent;
    CropArray[7].CrpNme = "B5";
    CropArray[7].CP     = new staticWbarley;

    CropArray[7].CP -> SetGatheringStraw(false);

    CropArray[8].CrpNme = "B6";
    CropArray[8].CP     = new staticWbarley;

    CropArray[8].CP -> SetGatheringStraw(true);

    CropArray[9].CrpNme  = "PE";
    CropArray[9].CP      = new staticPea;
    CropArray[10].CrpNme = "BP";
    CropArray[10].CP     = new staticBarleyPea;
    CropArray[11].CrpNme = "C1";
    CropArray[11].CP     = new staticCloverGrass(false);
    CropArray[12].CrpNme = "C2";
    CropArray[12].CP     = new staticCloverGrass(true);
    CropArray[13].CrpNme = "BE";
    CropArray[13].CP     = new staticBeet;
    CropArray[14].CrpNme = "B8";
    CropArray[14].CP     = new staticSbarley();    // spring barley, undersown with grass and clover

    CropArray[14].CP -> SetGatheringStraw(true);

    staticSbarley * sp = (staticSbarley *) CropArray[14].CP;

    sp -> SetUnderSown(true);

    CropArray[15].CrpNme = "G1";
    CropArray[15].CP     = new staticGrass;
    CropArray[16].CrpNme = "G2";
    CropArray[16].CP     = new staticGrassEstablished;
    CropArray[17].CrpNme = "G3";
    CropArray[17].CP     = new staticGrassPermanent;
    CropArray[18].CrpNme = "K1";
    CropArray[18].CP     = new staticClover;
    CropArray[19].CrpNme = "W5";
    CropArray[19].CP     = new staticSwheat;
    CropArray[20].CrpNme = "K2";                   // established clover
    CropArray[20].CP     = new staticGrassPermanent;
    CropArray[21].CrpNme = "W8";                   // winter wheat with grass
    CropArray[21].CP     = new staticWwheat;
    CropArray[22].CrpNme = "MA";
    CropArray[22].CP     = new staticMaize;
    CropArray[23].CrpNme = "NO";
    CropArray[23].CP     = new staticCrop;
    CropArray[24].CrpNme = "B9";
    CropArray[24].CP     = new staticSbarley();    // spring barley, undersown with grass

    CropArray[24].CP -> SetGatheringStraw(true);    // NOT IMPLEMENTED PROPERLY !!!!!!!!!!!!!!!

    staticSbarley * sp1 = (staticSbarley *) CropArray[24].CP;

    sp1 -> SetUnderSown(true);

    // Implement "O1" !!!
    CropArray[25].CrpNme = "O2";
    CropArray[25].CP     = new staticOat();    // Oat

    CropArray[25].CP -> SetGatheringStraw(true);    // NOT IMPLEMENTED PROPERLY ???!!!

    CropArray[26].CrpNme = "RY";
    CropArray[26].CP     = new staticRye();    // Rye

    CropArray[26].CP -> SetGatheringStraw(true);    // NOT IMPLEMENTED PROPERLY ???!!!

    CropArray[27].CrpNme = "S1";
    CropArray[27].CP     = new staticGrass();    // grass for seed

    CropArray[27].CP -> SetGatheringStraw(true);    // NOT IMPLEMENTED PROPERLY ???!!!

    CropArray[28].CrpNme = "S2";
    CropArray[28].CP     = new staticSeedGrass();    // grass for seed

    CropArray[28].CP -> SetGatheringStraw(true);    // NOT IMPLEMENTED PROPERLY ???!!!

    CropArray[29].CrpNme = "RS";
    CropArray[29].CP     = new staticSrape();           // spring rape
    CropArray[30].CrpNme = "PO";
    CropArray[30].CP     = new staticPotato();          // potato
    CropArray[31].CrpNme = "G4";
    CropArray[31].CP     = new staticGrazingGrass();    // grass for seed

    CropArray[31].CP -> SetGatheringStraw(true);    // NOT IMPLEMENTED PROPERLY ???!!!

    CropArray[32].CrpNme = "IR";                              // Italian ryegrass - NJH May 2009
    CropArray[32].CP     = new staticItalianRyeGrass;
    CropArray[33].CrpNme = "SB";                              // Soybean NJH May 2009
    CropArray[33].CP     = new staticPea;
    CropArray[34].CrpNme = "SF";                              // Sunflower NJH May 2009
    CropArray[34].CP     = new staticMaize;
    CropArray[35].CrpNme = "L1";
    CropArray[35].CP     = new staticLucerneEstablished();    // lucerne/alfalfa */
    CropArray[36].CrpNme = "L2";
    CropArray[36].CP     = new staticLucerneEstablished();    // lucerne/alfalfa */
    CropArray[37].CrpNme = "MK";
    CropArray[37].CP     = new staticMaize;
    CropArray[38].CrpNme = "TR";
    CropArray[38].CP     = new staticTriticale();
    CropArray[39].CrpNme = "FB";
    CropArray[39].CP     = new staticFabaBean();
    CropArray[40].CrpNme = "CR";
    CropArray[40].CP     = new staticTriticale();
    CropArray[41].CrpNme = "OR";
    CropArray[41].CP     = new staticBeet();

    /*
     *  !!!
     * for (int i=27;i<cattle_cropnum;i++)   // Neccesary as some crops are undefinded at this stage
     * CropArray[i].CP=CropArray[4].CP;
     */
    staticBase.InitCrop(irrigate);    // ???!!!@@@

    for (int i = 0; i < NumOfCrops(); i++) {
        CropArray[i].CP -> InitCrop(irrigate);
    }
}

/*
 *
 * Returns the number of field objects.
 *
 */
int cropRotation::NumOfFields() {
    return fieldList.NumOfNodes();
}

/*
 *
 * Returns the number of crops.
 *
 */
int cropRotation::NumOfCrops() {

    /*
     *  !!!
     * if (cattle)
     * return cattle_cropnum;
     * else
     * return nocattle_cropnum;
     */
    return cattle_cropnum;
}

/*
 * Returns the distance (meter) to the field in question.
 * field_no   number of field
 */
double cropRotation::DistanceToField(int field_no) {
    linkList<field>::PS P;
    fieldList.SearchByNumber(P, field_no);

    if (P) {
        return P -> element -> DistanceToField_();
    } else {
        theMessage -> WarningWithDisplay("cropRotation::DistanceToField - Illegal fieldnumber");

        return 0;
    }
}

/*
 * Create cropping plans for the comming season.
 * Parameters:
 * field_no         field number
 * FieldOrderList   chain of field operations.
 */
void cropRotation::PlanField(fieldOrder * FOL) {
    int field_no = FOL -> GetFieldNo();

    if (fieldList.ElementAtNumber(field_no)) {
        MakeSequenceInstance(FOL);
    } else {
        theMessage -> WarningWithDisplay("cropRotation::PlanField - Illegal fieldnumber");
    }
}

/*
 * Read plans for the comming season.
 * filename   name of input file
 */
void cropRotation::ReceivePlan(string filename, int yearIndex) {
    if (fieldList.NumOfNodes() > 0) {
        theMessage -> LogEvent("------------ Reading field allocation plan ------------");

        int          index;
        commonData * data = new commonData;

        data -> setCritical(true);
        data -> readFile(filename);

        cout << "----------------- Reading " << filename << endl;      // REMOVE LATER !!!!!!!!!!!!!!!!!!

        int first, num;
        int singleField = theControlParameters -> GetSingleField();

        data -> getSectionsNumbers("fieldOrder", &first, &num);

        for (index = first; index <= (first + num); index++) {
            if ((singleField == index) || (singleField < 0)) {
                cout << "Reading field plan no: " << index << endl;    // REMOVE LATER !!!!!!!!!!!!!!!

                // int fieldno=index;
                fieldOrder * FO = new fieldOrder("fieldOrder", index);

                FO -> ReadParameters(data,yearIndex);

                if (singleField >= 0) {
                    FO -> SetFieldNo(0);
                }

                FO -> SetStartYear(theTime.GetYear());
                FO -> SetDistance(DistanceToField(FO -> GetFieldNo()));
                PlanField(FO);
            }
        }

        delete data;
    }
}

void cropRotation::EndBudget() {
    for (int i = 0; i < fieldList.NumOfNodes(); i++) {
        fieldList.ElementAtNumber(i) -> EndBudget();
    }
}

/*
 * Replaces unknown crops with know ones.
 * Replacement specifications are read from a file.
 * A hack to get movin' in FarmN.
 * Parameters
 * PP   object with specifications
 */
void cropRotation::CorrectCropId(fieldOrder * PP) {
    string crop_id = PP -> GetCropId();
    int    crop_no = GiveCropNo(crop_id);

    if (crop_no < 0) {
        string s1, s2, crp;

        crp = crop_id;

        cout << "Crop not found " << crop_id << " - searching in crop replacement file" << endl;

        fstream f;
        string  inputDir = globalSettings -> getInputDirectory() + "/replacements.txt";

        // Although some compilers may accept \ in an include path,
        // they are all required to accept /.
        // so / should be  a valid path separator in Windows too
        // root/path is equivalent to root//path


        f.open(inputDir.c_str(), ios::in);

        bool found = false;

        if (!f) {
            theMessage -> FatalError("cropRotation::CorrectCropId - could not open crop replacement file");
        }

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

            PP -> SetCropId(crop_id);    // Overfl?dig ???!!!
        } else {
            theMessage -> WarningWithDisplay("No replacement found for unknown crop ", crop_id);
        }

        f.close();
    }
}
