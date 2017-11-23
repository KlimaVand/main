/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
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

#include "../base/message.h"
#include "staticCrops/staticClover.h"
#include "staticCrops/staticGrazingGrass.h"
#include "staticCrops/staticItalianRyeGrass.h"
#include "../base/controlParameters.h"
#include "staticCrops/staticLucerneEstablished.h"
#include "../base/settings.h"





// ---------------------------------------------------------------------------

/****************************************************************************\
Constructor.
\****************************************************************************/
cropRotation::cropRotation()
{
	CropPointer=&staticBase;
	InitLegalTable();
	InitReducTable();
strcpy(Name,"cropRotation");
   Index=0;
}

/****************************************************************************\
Destructor.
\****************************************************************************/
cropRotation::~cropRotation()
{
	fieldList.Reset();
   for (int i=0;i<NumOfCrops();i++)
   {
		if (CropArray[i].CP != &staticBase)
			delete CropArray[i].CP;
		delete []  CropArray[i].CrpNme;
   }

}


/****************************************************************************\
\****************************************************************************/
double cropRotation::GetDiversePlant()
{
   double diverse = 0.0;
   double costsPrHa = 0.0;
  	char * id;
   int NF=NumOfFields();
	for (int i=0;i<NF;i++)
   {
		linkList<field>::PS P;
	  	fieldList.SearchByNumber(P,i);
      if (P->element->GetActive())
      {
	      P->element->PresentCrop_(id);
	    	SetCropPointer(id);
	      if (strcmp(CropPointer->GetCropName(),"FLLW")==0)
	      	costsPrHa = 0.0;
	      else
	      {
	         if (P->element->JBno()<=3)
	         	costsPrHa = 300.0;
	         else
	         	costsPrHa = 400.0;
	      }
	      diverse+=costsPrHa*P->element->FieldArea_();
      }
	}
   return diverse;
}
/****************************************************************************\
\****************************************************************************/
double cropRotation::GetTotalArea()
{
   double area=0.0;
  	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
   {
		linkList<field>::PS P;
	  	fieldList.SearchByNumber(P,i);
      area+=P->element->FieldArea_();
	}
   return area;
}

/****************************************************************************\
\****************************************************************************/
double cropRotation::GetValueofLand()
{
   double value=0.0;
  	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
   {
		linkList<field>::PS P;
	  	fieldList.SearchByNumber(P,i);
      int JB=P->element->JBno();
      double haPrice=30000;
      if (JB<4)
         haPrice=20000;
      value+=haPrice*P->element->FieldArea_();
	}
   return value;
}



/****************************************************************************\
Updates the field object pointed to with currrent date and climate data.
Parameters:
   PP   Pointer to field object
\****************************************************************************/
void cropRotation::UpdateFld(field * PP)
{
	field * fP=nullptr;
	PP->SplitMe(fP);
	PP->UpdateField();
   while (fP!=nullptr)
   {
      ReplaceField(fP);
		PP->SplitMe(fP);
	}
}

/****************************************************************************\
Inserts a field, whith no area. This hack (!!!) is made only to "reserve" the
field number for later, which ensures that the order in which crops occupy fields
does not depend on crop development (climate).
\****************************************************************************/
void cropRotation::InsertGhostField()
{
   field * f=new field("field",fieldList.NumOfNodes(),this);
   f->SetActive(false);
   fieldList.InsertLast(f);
}

/****************************************************************************\
Replaces a "ghost" field with a real field. See "InsertGhostField()" for
rationale.
\****************************************************************************/
void cropRotation::ReplaceField(field * fP)
{
   int num=fP->ReturnNumber();
	linkList<field>::PS P;
 	fieldList.SearchByNumber(P,num);
   if (P->element->GetActive())
      theMessage->FatalError("cropRotation::ReplaceField - attempt to replace active field");
   delete(P->element);
   P->element=fP;
}

/****************************************************************************\
Creates cropping plan.
Parameters
   PP   object with specifications
\****************************************************************************/
void cropRotation::MakeSequenceInstance(fieldOrder * PP)
{
   cout << "cropRotation::MakeSequenceInstance - processing field no: " << PP->GetFieldNo() << endl;

	SetCropPointer(PP->GetCropId());
	int num;

   num = fieldList.ElementAtNumber(PP->GetFieldNo() )->ReturnNumber();
   int futureNum=num;
   bool allreadyTaken;
   allreadyTaken=fieldList.ElementAtNumber(PP->GetFieldNo() )->FuturePlanExists();
   if (allreadyTaken)
   {
      futureNum=NumOfFields();
      InsertGhostField();
      theMessage->WarningWithDisplay("cropRotation::MakeSequenceInstance - 'Ghost' field inserted");
   }
  // char * name =PP->GetCropId();


	cloneList<fieldOperationFields> * OP=
		CropPointer->MakeOpList(PP,fieldList.ElementAtNumber(PP->GetFieldNo())->JBno());
   if (PP->GetAdjustArea()>0)
      CropPointer->SetAdjustArea(OP,PP);
	fieldList.ElementAtNumber(PP->GetFieldNo() )->PlanField_(OP,PP->GetCropId(),futureNum);
   //ths->AddAreaToIndicators(PP->GetCropId(),PP->GetArea(),soil_type_);
}

/****************************************************************************\
\****************************************************************************/
/*
void cropRotation::AddAreaToIndicators(char * cid, double a, int JBNO)
{

   if (JBNO<=4)
   	theOutput->AddIndicator(105,"01.05 JB 1-4","ha",a);
   else
      theOutput->AddIndicator(106,"01.06 JB 5-7","ha",a);


	if (strcmp(cid,"W1")==0 || strcmp(cid,"W2")==0)
      theOutput->AddIndicator(1510,"15.10 W.wheat area","ha",a);
	if (strcmp(cid,"W5")==0 || strcmp(cid,"W6")==0)
      theOutput->AddIndicator(1511,"15.11 S.wheat area","ha",a);
   if (strcmp(cid,"RW")==0)
      theOutput->AddIndicator(1512,"15.12 W.rape area","ha",a);
   if (strcmp(cid,"RS")==0)
      theOutput->AddIndicator(1513,"15.13 S.rape area","ha",a);
   if (strcmp(cid,"B5")==0 || strcmp(cid,"B6")==0)
      theOutput->AddIndicator(1514,"15.14 W.barley area","ha",a);
   if (strcmp(cid,"B1")==0 || strcmp(cid,"B2")==0)
      theOutput->AddIndicator(1515,"15.15 S.barley area","ha",a);
   if (strcmp(cid,"RY")==0)
      theOutput->AddIndicator(1516,"15.16 Rye area","ha",a);
   if (strcmp(cid,"OA")==0)
      theOutput->AddIndicator(1517,"15.17 Oat area","ha",a);
   if (strcmp(cid,"PE")==0)
      theOutput->AddIndicator(1518,"15.18 Pea area","ha",a);
   if (strcmp(cid,"PO")==0)
      theOutput->AddIndicator(1519,"15.19 Potato area","ha",a);
   if (strcmp(cid,"SG")==0)
      theOutput->AddIndicator(1520,"15.20 Grass for seed area","ha",a);
   if (strcmp(cid,"MA")==0)
      theOutput->AddIndicator(1521,"15.21 Maize area","ha",a);
   if (strcmp(cid,"BP")==0)
      theOutput->AddIndicator(1523,"15.23 Barley/Pea area","ha",a);
   if (strcmp(cid,"G1")==0 || strcmp(cid,"G2")==0 || strcmp(cid,"G3")==0)
      theOutput->AddIndicator(1524,"15.24 Grass ley area","ha",a);
   if (strcmp(cid,"C1")==0 || strcmp(cid,"C2")==0)
      theOutput->AddIndicator(1525,"15.25 Grass/clover ley area","ha",a);
   if (strcmp(cid,"F1")==0 || strcmp(cid,"F2")==0)
      theOutput->AddIndicator(1526,"15.26 Fallow area","ha",a);
   if (strcmp(cid,"BE")==0)
      theOutput->AddIndicator(1522,"15.22 Beet area","ha",a);
/*   if (strcmp(cid,"L2")==0)
      theOutput->AddIndicator("15.23 Lucerne area","ha",a);  */
//}

/****************************************************************************\
Sets the "CropPointer" to point at the crop given by "crop_id".
Parameters:
   crop_id   crop identification
\****************************************************************************/
void cropRotation::SetCropPointer(char * crop_id)
{
	int No=GiveCropNo(crop_id);
	if ((No>=0) && (No<=cattle_cropnum))
		CropPointer=CropArray[No].CP;
	else
   {
		CropPointer=&staticBase;
		theMessage->WarningWithDisplay("cropRotation::SetCropPointer - ",crop_id," is illegal cropname ");
	}
}

/****************************************************************************\
The update function which must be called on a daily basis.
\****************************************************************************/
void cropRotation::DailyUpdate()
{
	//theOutput->AddIndicator(7050,"70.50 Precipitation","mm",theClimate->precip);

   int n=fieldList.NumOfNodes();
   for (int i=0;i<n;i++)
   	UpdateFld(fieldList.ElementAtNumber(i));
}

/****************************************************************************\
Initiates the table containing information about the legal crop sequences.
Colums: Old crop(s). Rows: New crop.
Extend with Spring wheat W5 !!!
Not updatet properly !!!
\****************************************************************************/
void cropRotation::InitLegalTable()
{
	int Lgl[cattle_cropnum][cattle_cropnum] =
		//W1 W2 RW F1 B1 B2 F2 B5 B6 PE BP C1 C2 BE B8 G1 G2 G4 G5 W3 B7 B8 B9
		{{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 2},   // W1
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 2},   // W2
		 {1, 1, 4, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 8, 0, 0, 0, 0, 0, 2, 1, 2, 2},   // RW
		 {0, 0, 0, 1, 0, 0, 2, 0, 0, 0, 0, 1, 1, 1, 2, 2, 2, 2, 2, 2, 0, 2, 2},   // F1
       {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 2},   // B1
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 2},   // B2
		 {1, 1, 1, 0, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1, 2, 2},   // F2
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 1, 2, 2},   // B5
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 2, 1, 2, 2},   // B6
		 {0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // PE
		 {0, 0, 1, 0, 0, 0, 0, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},   // BP
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 2, 2, 2, 2, 2, 2, 0, 2, 2},   // C1
		 {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 2, 1, 2, 2},   // C2
		 {0, 0, 8, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 2, 1, 2, 2},   // BE
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 2, 2},   // B8
		 {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},   // G1
		 {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},   // G2
		 {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},   // G4
		 {1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0},   // G5
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 4, 4, 4, 4, 4, 5, 1, 5, 5},   // W3
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1},   // B7
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 5, 1, 5, 5},   // B8
		 {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 4, 4, 5, 1, 5, 5}};  // B9
	for (int i=0;i<cattle_cropnum;i++)
		for (int j=0;j<cattle_cropnum;j++)
			Legal[i][j]=Lgl[i][j];
}

/****************************************************************************\
A simple approach to the effects of the previous crop not related
to nutrient balance and release.
Colums: previous crop. Rows: New crop.
Extend with Spring wheat W5 !!!
Not updatet properly !!!
\****************************************************************************/
void cropRotation::InitReducTable()
{
	double Red[cattle_cropnum][cattle_cropnum] =
	//  W1   W2   RW   F1   B1   B2   F2   B5   B6   PE   BP   C1   C2   X2   G1   G2   G3   G4   G5   W3   B7   B8   B9
	{{0.90,0.90,1.00,1.00,0.90,0.90,1.00,0.90,0.90,1.00,1.00,0.90,0.90,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // W1
	 {0.90,0.90,1.00,1.00,0.90,0.90,1.00,0.90,0.90,1.00,1.00,0.90,0.90,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // W2
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // RW
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // F1
	 {0.95,0.95,1.00,1.00,0.95,0.95,1.00,0.95,0.95,1.00,1.00,0.95,0.95,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // B1
	 {0.95,0.95,1.00,1.00,0.95,0.95,1.00,0.95,0.95,1.00,1.00,0.95,0.95,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // B2
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // F2
	 {0.95,0.95,1.00,1.00,0.95,0.95,1.00,0.95,0.95,1.00,1.00,0.95,0.95,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // B5
	 {0.95,0.95,1.00,1.00,0.95,0.95,1.00,0.95,0.95,1.00,1.00,0.95,0.95,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // B6
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // PE
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // BP
    {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // C1
    {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // C2
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // X2
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // G1
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // G2
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // G3
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // G4
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // G5
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // W3
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // B7
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}, // B8
	 {1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00,1.00}};// B9
	for (int i=0;i<cattle_cropnum;i++)
		for (int j=0;j<cattle_cropnum;j++)
			Reduc[i][j]=Red[i][j];
}

/****************************************************************************\
Initiates all single fields with data which each instance of field reads
from the disk.
"min_area" contains the minimum area for one field.
Parameters:
   filename   input file for single fields initialization
   min_area   minimum area of a single field (ha)
\****************************************************************************/
void cropRotation::InitSingleFields(bool WithoutFields,string filename,int daysBetweenIrrigation, int irrigationDelay,int FingerFlow)
{
   if (WithoutFields==false)
   {
      int index,first;
      int num = 0;
      commonData * data=new commonData();
      data->setCritical(true);
      data->readFile(filename);
      data->getSectionsNumbers("field",&first,&num);
      int singleField=theControlParameters->GetSingleField();
      if (singleField>=num)
			theMessage->FatalError("croprot::InitSingleFields - singleField requested for non-existent field");
      for(index=first;index<=(first+num);index++)
      {
         if (singleField==index || singleField<0)
         {
            field * fld=new field("field",index,this);
            fld->SetFingerFlow(FingerFlow);
            fld->ReadParameters(data);

            fld->SetDaysBetweenIrrigation(daysBetweenIrrigation);
            fld->SetIrrigationDelay(irrigationDelay);
            fld->StartBudget();
            if (singleField>0)
               fld->SetIndex(0);
            fieldList.InsertLast(fld);
         }
      }
      delete data;
      theOutput->SetMaxErrorN(0.01*(1.0+num-first)); // Change later - add 0.002 per ha !!!
      theOutput->SetMaxErrorC(0.01*(1.0+num-first)); // Change later - add 0.002 per ha !!!
   }

}

/****************************************************************************\
The initialization function to be called prior to simulation.
Parameters:
irrigat           0->irrigation not possible;  1->irrigation possible.
cattl             0->No cattle on farm; 1->cattle on farm.
MinArea           minimum area for one field (ha).
\****************************************************************************/
void cropRotation::Initialize(bool WithoutFields,bool irrigat,int daysBetweenIrrigation,int irrigationDelay,
		int FingerFlow)
{
	irrigate=irrigat;
   InitStaticCrops();
   InitSingleFields(WithoutFields,"fields.dat",daysBetweenIrrigation,irrigationDelay,FingerFlow);

}

/****************************************************************************\
Returns the number corresponding to the crop name given by "crop_id".
Returns -1 by illegal "crop_id".
Parameters:
   crop_id   crop identification
\****************************************************************************/
int cropRotation::GiveCropNo(char * crop_id)
{
   char * s;
   int j = -1;
   for (int i=0;i<cattle_cropnum;i++)
   {
	   CropId(i,s);

	   if (strcmp(crop_id,s) == 0)
		   j = i;
   }
   return j;
}

/****************************************************************************\
Gives the name corresponding to the crop number given by "crop_no".
References an empty string by illegal "crop_no".
Parameters:
   crop_no   crop number
   crop_id   reference to crop identification
\****************************************************************************/
void cropRotation::CropId(int crop_no,char * &crop_id)
{
	if ((crop_no >= 0) && (crop_no < cattle_cropnum))
		crop_id=CropArray[crop_no].CrpNme;
	else
   {
		theMessage->WarningWithDisplay("cropRotation::CropId - Illegal cropnumber ",crop_id);

		strcpy(crop_id,"");
	}
}

/****************************************************************************\
Initializes all static crop objects
\****************************************************************************/
void cropRotation::InitStaticCrops()
{
   for (int i=0;i<cattle_cropnum;i++)
   {
	   CropArray[i].CrpNme=new char[10];
		CropArray[i].CP=&staticBase;
   }

   strcpy(CropArray[0].CrpNme,"W1");
	CropArray[0].CP=new staticWwheat;
	CropArray[0].CP->SetGatheringStraw(false);
	 strcpy(CropArray[1].CrpNme,"W2");
	CropArray[1].CP=new staticWwheat;
	CropArray[1].CP->SetGatheringStraw(true);
	 strcpy(CropArray[2].CrpNme,"RW");
	CropArray[2].CP=new staticWrape;
	strcpy(CropArray[3].CrpNme,"F1");
	CropArray[3].CP=new staticFallow;

	strcpy(CropArray[4].CrpNme,"B1");
	CropArray[4].CP=new staticSbarley();
   CropArray[4].CP->SetGatheringStraw(false);

	strcpy(CropArray[5].CrpNme,"B2");
	CropArray[5].CP=new staticSbarley();
   CropArray[5].CP->SetGatheringStraw(true);

	strcpy(CropArray[6].CrpNme,"F2");
	CropArray[6].CP=new staticFallowPermanent;

	strcpy(CropArray[7].CrpNme,"B5");
	CropArray[7].CP=new staticWbarley;
   CropArray[7].CP->SetGatheringStraw(false);

	strcpy(CropArray[8].CrpNme,"B6");
	CropArray[8].CP=new staticWbarley;
   CropArray[8].CP->SetGatheringStraw(true);

	strcpy(CropArray[9].CrpNme,"PE");
	CropArray[9].CP=new staticPea;

	strcpy(CropArray[10].CrpNme,"BP");
	CropArray[10].CP=new staticBarleyPea;
	strcpy(CropArray[11].CrpNme,"C1");
	CropArray[11].CP=new staticCloverGrass(false);
	strcpy(CropArray[12].CrpNme,"C2");
	CropArray[12].CP=new staticCloverGrass(true);

	strcpy(CropArray[13].CrpNme,"BE");
   CropArray[13].CP=new staticBeet;

	strcpy(CropArray[14].CrpNme,"B8");
   CropArray[14].CP=new staticSbarley();              // spring barley, undersown with grass and clover
   CropArray[14].CP->SetGatheringStraw(true);
   staticSbarley * sp = (staticSbarley*)CropArray[14].CP;
   sp->SetUnderSown(true);

   strcpy(CropArray[15].CrpNme,"G1");
	CropArray[15].CP=new staticGrass;

	strcpy(CropArray[16].CrpNme,"G2");
	CropArray[16].CP=new staticGrassEstablished;

	strcpy(CropArray[17].CrpNme,"G3");
	CropArray[17].CP=new staticGrassPermanent;

	strcpy(CropArray[18].CrpNme,"K1");
	CropArray[18].CP=new staticClover;
	strcpy(CropArray[19].CrpNme,"W5");
	CropArray[19].CP=new staticSwheat;
	strcpy(CropArray[20].CrpNme,"K2"); // established clover
   CropArray[20].CP=new staticGrassPermanent;

	strcpy(CropArray[21].CrpNme,"W8");  // winter wheat with grass
	CropArray[21].CP=new staticWwheat;

   strcpy(CropArray[22].CrpNme,"MA");
	CropArray[22].CP=new staticMaize;


	strcpy(CropArray[23].CrpNme,"NO");
	CropArray[23].CP=new staticCrop;


	strcpy(CropArray[24].CrpNme,"B9");
   CropArray[24].CP=new staticSbarley();                 // spring barley, undersown with grass
   CropArray[24].CP->SetGatheringStraw(true);            // NOT IMPLEMENTED PROPERLY !!!!!!!!!!!!!!!
   staticSbarley * sp1 = (staticSbarley*)CropArray[24].CP;
   sp1->SetUnderSown(true);
   // Implement "O1" !!!

	strcpy(CropArray[25].CrpNme,"O2");
   CropArray[25].CP=new staticOat();                    // Oat
   CropArray[25].CP->SetGatheringStraw(true);            // NOT IMPLEMENTED PROPERLY ???!!!


	strcpy(CropArray[26].CrpNme,"RY");
   CropArray[26].CP=new staticRye();                    // Rye
   CropArray[26].CP->SetGatheringStraw(true);            // NOT IMPLEMENTED PROPERLY ???!!!


   strcpy(CropArray[27].CrpNme,"S1");
   CropArray[27].CP=new staticGrass();                  // grass for seed
   CropArray[27].CP->SetGatheringStraw(true);            // NOT IMPLEMENTED PROPERLY ???!!!


   strcpy(CropArray[28].CrpNme,"S2");
   CropArray[28].CP=new staticSeedGrass();              // grass for seed
   CropArray[28].CP->SetGatheringStraw(true);            // NOT IMPLEMENTED PROPERLY ???!!!


   strcpy(CropArray[29].CrpNme,"RS");
   CropArray[29].CP=new staticSrape();              // spring rape


   strcpy(CropArray[30].CrpNme,"PO");
   CropArray[30].CP=new staticPotato();              // potato


   strcpy(CropArray[31].CrpNme,"G4");
   CropArray[31].CP=new staticGrazingGrass();              // grass for seed
   CropArray[31].CP->SetGatheringStraw(true);            // NOT IMPLEMENTED PROPERLY ???!!!


	strcpy(CropArray[32].CrpNme,"IR");//Italian ryegrass - NJH May 2009
	CropArray[32].CP=new staticItalianRyeGrass;


	strcpy(CropArray[33].CrpNme,"SB");//Soybean NJH May 2009
	CropArray[33].CP=new staticPea;


	strcpy(CropArray[34].CrpNme,"SF");   //Sunflower NJH May 2009
	CropArray[34].CP=new staticMaize;

	strcpy(CropArray[35].CrpNme,"L1");
   CropArray[35].CP=new staticLucerneEstablished();              // lucerne/alfalfa */

   strcpy(CropArray[36].CrpNme,"L2");
   CropArray[36].CP=new staticLucerneEstablished();              // lucerne/alfalfa */

   /* !!!
	for (int i=27;i<cattle_cropnum;i++)   // Neccesary as some crops are undefinded at this stage
		CropArray[i].CP=CropArray[4].CP;
   */

	staticBase.InitCrop(irrigate); //???!!!@@@
	for (int i=0;i<NumOfCrops();i++)
		CropArray[i].CP->InitCrop(irrigate);
}


/****************************************************************************\
Returns the number of field objects.
\****************************************************************************/
int cropRotation::NumOfFields()
{
	return fieldList.NumOfNodes();
}



/****************************************************************************\
Returns the number of crops.
\****************************************************************************/
int cropRotation::NumOfCrops()
{

/* !!!
	if (cattle)
		return cattle_cropnum;
	else
		return nocattle_cropnum;
*/
	return cattle_cropnum;
}


/****************************************************************************\
Returns the distance (meter) to the field in question.
Parameters:
   field_no   number of field
\****************************************************************************/
double cropRotation::DistanceToField(int field_no)
{
	linkList<field>::PS P;
	fieldList.SearchByNumber(P,field_no);
	if (P!=nullptr)
		return P->element->DistanceToField_();
	else
   {
		theMessage->WarningWithDisplay("cropRotation::DistanceToField - Illegal fieldnumber");
		return 0;
	}
}








/****************************************************************************\
Returns 1 if the crop is a grass crop, otherwise returns 0.
Parameters:
   crop_id   crop identification
Presently clumsy hardcoding
\****************************************************************************/
int cropRotation::IsCropGrass(int crop_num)
{
	return ((crop_num>=15) && (crop_num<=17));
}





/****************************************************************************\
Create cropping plans for the comming season.
Parameters:
   field_no         field number
   FieldOrderList   chain of field operations.
\****************************************************************************/
void cropRotation::PlanField(fieldOrder * FOL)
{

	int field_no=FOL->GetFieldNo();
	if (fieldList.ElementAtNumber(field_no)!=nullptr)
   {

		MakeSequenceInstance(FOL);
	}
	else
		theMessage->WarningWithDisplay("cropRotation::PlanField - Illegal fieldnumber");
}

/****************************************************************************\
Read plans for the comming season.
Parameters:
   filename   name of input file
\****************************************************************************/
void cropRotation::ReceivePlan(char * filename)
{
	if (fieldList.NumOfNodes()>0)
   {
      theMessage->LogEvent("------------ Reading field allocation plan ------------");
      int index;

      commonData * data=new commonData;
      data->setCritical(true);

      data->readFile(filename);
      cout << "----------------- Reading " << filename << endl; // REMOVE LATER !!!!!!!!!!!!!!!!!!
      int first,num;
      int singleField=theControlParameters->GetSingleField();
      data->getSectionsNumbers("fieldOrder",&first,&num);
      for(index=first;index<=(first+num);index++)
      {
         if (singleField==index || singleField<0)
         {
            cout << "Reading field plan no: " << index << endl; // REMOVE LATER !!!!!!!!!!!!!!!
            //int fieldno=index;
            fieldOrder * FO = new fieldOrder("fieldOrder",index);

            FO->ReadParameters(data);

            if (singleField>=0)
               FO->SetFieldNo(0);
            FO->SetStartYear(theTime.GetYear());
            FO->SetDistance(DistanceToField(FO->GetFieldNo()));


               PlanField(FO);

         }
      }
      delete data;

   }
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::EndBudget()
{
	for (int i=0;i<fieldList.NumOfNodes();i++)
   	fieldList.ElementAtNumber(i)->EndBudget();
}
