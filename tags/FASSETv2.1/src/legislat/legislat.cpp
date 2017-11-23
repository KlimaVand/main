/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "common.h"
#include "legislat.h"
#include <product.h>
#include <products.h>
#include <premium.h>

/****************************************************************************\
  Default Constructor
\****************************************************************************/
legislate::legislate()
	: base()
{
   InitVariables();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
legislate::legislate(char * aName, int aIndex, base * aOwner)
	: base(aName, aIndex, aOwner)
{
   InitVariables();
}

/****************************************************************************\
  Destructor
\****************************************************************************/
legislate::~legislate()
{
	delete fieldAttribList;
   delete taxList;
}

/****************************************************************************\
\****************************************************************************/
void legislate::InitVariables()
{
	fieldAttribList = new linkList <product>;
   taxList = new linkList <taxLegislation>;
   simulationYear = 0;
   GreenPrecent = 0.0;
   Fallow = 0.05;
   Nprice = -5.0;
   Pprice = -8.0;
   Kprice = -3.0;
   Nmax = 10000;
}

/****************************************************************************\
\****************************************************************************/
void legislate::Initialize( char* LawApplyFileName)
{
	OpenInputFile(LawApplyFileName);
   if (fieldAttribList) delete fieldAttribList;
   fieldAttribList = theProducts->GetSubsetofProductList(premiumObj);
   InitializeTax();
   FindSection("general");
   GetParameter("GreenPrecent",&GreenPrecent);
	GetParameter("Fallow",&Fallow);
	GetParameter("Nprice",&Nprice);
	GetParameter("Pprice",&Pprice);
	GetParameter("Kprice",&Kprice);
	GetParameter("Nmax",&Nmax);
	CloseInputFile();
}

/****************************************************************************\
\****************************************************************************/
void legislate::InitializeTax()
{
   SetCritical();
   int first,num;
   GetSectionNumbers("legislat",&first,&num);
	for(int index=first;index<(first+num);index++)
   {
      taxLegislation* Alegislation = new taxLegislation("legislat",index,this);
      Alegislation->ReadParameters(file);
      taxList->InsertLast(Alegislation);
   }
   taxLegislation* Alegislation = new taxLegislation(0,0,0,1000,"legislat",-1,this);      // default tax
   taxList->InsertLast(Alegislation);
}

/****************************************************************************\
\****************************************************************************/
bool legislate::IsGreen(char* aFieldName)
{
   premium* aPremium;
	for (int i=0; (i<fieldAttribList->NumOfNodes()); i++)
   {
   	aPremium = (premium*) fieldAttribList->ElementAtNumber(i);
      if (strcmp(aPremium->GetPlantName().c_str(),aFieldName)==0)
      	return aPremium->GetGreen();
   }
   return false;
}

/****************************************************************************\
\****************************************************************************/
double legislate::GetContribution(char* aFieldName)
{
	premium* aPremium;
	for (int i=0; (i<fieldAttribList->NumOfNodes()); i++)
   {
   	aPremium = (premium*) fieldAttribList->ElementAtNumber(i);
      if (strcmp(aPremium->GetPlantName().c_str(),aFieldName)==0)
      	return aPremium->GetaPrice()->GetExpectedSellPriceYear(1);
   }
   return 0.0;
}

/****************************************************************************\
\****************************************************************************/
taxLegislation* legislate::GetCurrentTax(int type)
{
   taxLegislation* aTaxLegislation;
	for (int i=0;i<taxList->NumOfNodes();i++)
   {
      aTaxLegislation = taxList->ElementAtNumber(i);
   	if (((aTaxLegislation->getType())==(type+1)) && (aTaxLegislation->taxThisYear(simulationYear)))
      	return aTaxLegislation;
   }
   return aTaxLegislation;                                     // default element at end of taxlist
}

/****************************************************************************\
\****************************************************************************/
void legislate::CalcLP()
{
	fstream* fpar = new fstream;
	fstream* fset = new fstream;
   enum {NumberOfNlevy = 4};
   double Nlevy[NumberOfNlevy];
   double level = 0.0;
   string NLevyString[] = {"Nlevy1","Nlevy2","Nlevy3","Nlevy4"};
   for (int i=0;i<NumberOfNlevy; i++)
   	Nlevy[i] = 0.0;
   for (int i=0;i<NumberOfNlevy; i++)
   {
   	Nlevy[i] += GetCurrentTax(i)->getTax();
      if (i==3) level += GetCurrentTax(i)->getLevel();
   }
   fset->open("Nlevys.inc",ios::out);
	fpar->open("Nlevyp.inc",ios::out);
	for (int i=0; i<NumberOfNlevy; i++)
   {
   	*fset << NLevyString[i] << endl;
      *fpar << NLevyString[i] << "      " << Nlevy[i] << endl;
   }
	fset->close();
   fpar->close();
   delete fpar;
   delete fset;
   fstream* f = new fstream;
	f->open("Nluse.INC",ios::out);
   *f << "Nluse minimum level on N tax" << endl;
   *f << "/ " << level << " /" << endl;
   f->close();
   f->open("FALLOWP.INC",ios::out);
   *f << "FALLOWP The requirede percentage of fallow land" << endl;
   *f << "/ " << Fallow << " /" << endl;
   f->close();
   f->open("GREENPCT.INC",ios::out);
   *f << "GREENPCT The minimum fraction of green areas" << endl;
   *f << "/ " << GreenPrecent << " /" << endl;
   f->close();
   f->open("NPRICE.INC",ios::out);
   *f << "NPRICE price on nitrogen" << endl;
   *f << "/ " << Nprice << " /" << endl;
   f->close();
   f->open("PPRICE.INC",ios::out);
   *f << "PPRICE price on phosphor" << endl;
   *f << "/ " << Pprice << " /" << endl;
   f->close();
   f->open("KPRICE.INC",ios::out);
   *f << "KPRICE price on kalium" << endl;
   *f << "/ " << Kprice << " /" << endl;
   f->close();
   f->open("NMAX.INC",ios::out);
   *f << "NMAX maximum surplus on farm" << endl;
   *f << "/ " << Nmax << " /" << endl;
   f->close();
   delete f;
}

/****************************************************************************\
\****************************************************************************/
void legislate::Update()
{
   simulationYear++;
}

