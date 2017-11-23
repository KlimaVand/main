/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Created by NJH - 2.6.99
  ----------------------------------------------------------------------------
      Log
 Describes a manure store (can be on the floor of animal housing, an in-house slurry pit
 or a separate free-standing store)
\****************************************************************************/
#include "../building/manstore.h"
#include "../base/controlParameters.h"
#include "../base/settings.h"
#
#ifdef _STANDALONE
	#include "../base/common.h"
#else
   #include "../base/IndicatorOutput.h"
   #include "../base/bstime.h"
#endif



/****************************************************************************\
  Destructor
\****************************************************************************/
manurestore::~manurestore()
{
	delete theStore;
	delete aBase;
	if (detailsFile)
		CloseDetailsFile();
}

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
manurestore::manurestore(const char *aName,const int aIndex,const base* aOwner,commonData *  &runData,commonData * &orignal)
{
	aBase=new base(aName,aIndex,aOwner);
	theStore= new storage(aName,aIndex,aOwner,0,runData,orignal);
	manureToName=" ";
	nextStore=NULL;

	Nbudget.SetNames(aName,"N");
	Cbudget.SetNames(aName,"C");
	cumvolat=0;

	external = false;
	postCleanDepth = 0.0;  ////tonnes manure remaining after cleaning


	detailsFile=NULL;



	runData->setCritical(true);
	runData->FindSection(aName,aIndex);
	runData->FindItem("Alias",&Alias);
	runData->setCritical(false);
	orignal->setCritical(true);
	int indexInOld;
		string place;
		orignal->findSection(aName,0,"Alias",Alias,&place,&indexInOld);
		orignal->FindSection(place,indexInOld);

	orignal->FindItem("ManureStoreID",&manureStoreID);
	runData->FindItem("ManureStoreID",&manureStoreID);

	orignal->FindItem("PostCleanDepth",&postCleanDepth);
	runData->FindItem("PostCleanDepth",&postCleanDepth);


	string manureType;
	orignal->FindItem("Manuretype",&manureType);
	runData->FindItem("Manuretype",&manureType);

	manure * aManure=new manure(manureType.c_str(),0,NULL);
	theProducts->GiveProductInformation(aManure);
	aManure->Setamount(0);

	theStore->SetProductPointer(aManure);
	orignal->FindItem("Locked",&locked);
	runData->FindItem("Locked",&locked);

	switch (theControlParameters->GetvolatModel())
	{
		case 0:
		{
			orignal->FindItem("NH3Emissionfactor",&NH3Emissionfactor);
			runData->FindItem("NH3Emissionfactor",&NH3Emissionfactor);

			orignal->FindItem("N2Emissionfactor",&N2Emissionfactor);
			runData->FindItem("N2Emissionfactor",&N2Emissionfactor);

			orignal->FindItem("N2OEmissionfactor",&N2OEmissionfactor);
			runData->FindItem("N2OEmissionfactor",&N2OEmissionfactor);

			orignal->FindItem("CO2Emissionfactor",&CO2Emissionfactor);
			runData->FindItem("CO2Emissionfactor",&CO2Emissionfactor);

			orignal->FindItem("CH4Emissionfactor",&CH4Emissionfactor);
			runData->FindItem("CH4Emissionfactor",&CH4Emissionfactor);

			orignal->FindItem("NMinfactor",&NMinfactor);
			runData->FindItem("NMinfactor",&NMinfactor);

			break;
		}
		case 1:
		{
			orignal->FindItem("Area",&surfaceArea);
			runData->FindItem("Area",&surfaceArea);

			orignal->FindItem("Depth",&depth);
			runData->FindItem("Depth",&depth);

			orignal->FindItem("OMdegRateConst",&OMdegRateConst);
			runData->FindItem("OMdegRateConst",&OMdegRateConst);

			break;
		}
	}
	orignal->FindItem("ManureToName",&manureToName);
	runData->FindItem("ManureToName",&manureToName);

	orignal->FindItem("ManureToIndex",&manureToIndex);
	runData->FindItem("ManureToIndex",&manureToIndex);


}

/**
 *  Copy Constructor
 */
manurestore::manurestore(const manurestore& s)
{
	aBase = new base(s.aBase->GetName(),s.aBase->GetIndex(),s.aBase->GetOwner());
	theStore = new storage(*s.theStore);
	manureToName=s.manureToName;
	nextStore=s.nextStore;
	NH3Emissionfactor=s.NH3Emissionfactor;
	N2Emissionfactor=s.N2Emissionfactor;
	N2OEmissionfactor=s.N2OEmissionfactor;
	CO2Emissionfactor=s.CO2Emissionfactor;
	CH4Emissionfactor=s.CH4Emissionfactor;

	NMinfactor=s.NMinfactor;
	OMdegRateConst=s.OMdegRateConst;
	Nbudget=s.Nbudget;
	Cbudget=s.Cbudget;
	cumvolat=s.cumvolat;

	external = s.external;
	postCleanDepth = s.postCleanDepth;  ////tonnes manure remaining after cleaning
	surfaceArea = s.surfaceArea;       //in square metres
	manureToIndex = s.manureToIndex;
	locked=s.locked;
	detailsFile=s.detailsFile;
	manureStoreID = s.manureStoreID;
}

/**
	Get total amount stored.
 */
double manurestore::GetAmountManure()
{
	return theStore->Getamountstored();
};


/**
	Get total N stored.
 */

double manurestore::GetTotalNStored()
{
	double Vt=0,N=0;
	manure * aManure=(manure *) theStore->GetProductPointer();

	if (aManure)
	{
		Vt=aManure->GetAmount();           //The total amount of slurry in kg
		N=Vt*aManure->GetTotalN().n  ;            //The total N-fraction
	}
	return N;
}

/**
	Get total C stored.
 */

double manurestore::GetTotalCStored()
{
	double Vt=0,C=0;
	manure * aManure=(manure *) theStore->GetProductPointer();

	if (aManure)
	{
		Vt=aManure->GetAmount();           //The total amount of slurry in kg
		C=Vt*aManure->GetC_content();            //The total C-fraction
	}
	return C;
}

/**
 * Get total DryMatter stored.
 */

double manurestore::GetTotalDMStored()
{
	double Vt=0,DM=0;
	manure * aManure=(manure *) theStore->GetProductPointer();

	if (aManure)
	{
		Vt=aManure->GetAmount();           //The total amount of slurry in kg
		DM=Vt*aManure->CalcDryMatterConc();            //The total N-fraction
	}
	return DM;
}



/**
 * Dailyactions
 */

void manurestore::DailyUpdate(double volatAmmonia, double N2Emission, double N2OEmission, double CO2Emission, double CH4Emission)
{
	if (detailsFile)
	{
		ShowManureStore(detailsFile, false);
		*detailsFile << endl;
	}
}

/**
 * 	Adding the manure to store.
 * If the store already contain "SLURRY" the manure must be a following types FLUID-MANURE,SOLID-MANURE, WASHWATER, STRAW, WATER or SLURRY.
 * If the store containing something else that SLURRY those manture can be added FLUID-MANURE FLUID-MANURE,SOLID-MANURE, WASHWATER, STRAW, WATER or CATTLE-FYM
 * Nbuget and CBudget is updated
 */

void manurestore::RcvManure(const manure* someManure)
{
	manure *tmpManure=(manure*) someManure->clone();
	if (strcmp(theStore->GetProductPointer()->GetName(),"SLURRY")==0)
	{
		if ((strcmp(tmpManure->GetName(),"FLUID-MANURE")==0)||(strcmp(tmpManure->GetName(),"SOLID-MANURE")==0)
				|| (strcmp(tmpManure->GetName(),"CATTLE-URINE")==0)||(strcmp(tmpManure->GetName(),"CATTLE-FAECES")==0)
				|| (strcmp(tmpManure->GetName(),"PIG-URINE")==0)||(strcmp(tmpManure->GetName(),"PIG-FAECES")==0)
				||(strcmp(tmpManure->GetName(),"WASHWATER")==0)||(strcmp(tmpManure->GetName(),"STRAW")==0)
				||(strcmp(tmpManure->GetName(),"WATER")==0)||(strcmp(tmpManure->GetName(),"SLURRY")==0))
		{
			tmpManure->Setname("SLURRY");
			theStore->AddToStore(tmpManure);
		}
		else
			theMessage->FatalError(GetName()," ","manurestore: attempt to add wrong manure type");
	}
	else
	{
		if ((strcmp(tmpManure->GetName(),"FLUID-MANURE")==0)||(strcmp(tmpManure->GetName(),"SOLID-MANURE")==0)
				||(strcmp(tmpManure->GetName(),"WASHWATER")==0)||(strcmp(tmpManure->GetName(),"STRAW")==0)
				||(strcmp(tmpManure->GetName(),"WATER")==0)|| (strcmp(tmpManure->GetName(),"CATTLE-FYM")==0))
		{
			tmpManure->Setname("CATTLE-FYM");
			theStore->AddToStore(tmpManure);
		}
		else
			theMessage->FatalError(GetName()," ","manurestore: attempt to add wrong manure type");
	}

	Nbudget.AddInput(tmpManure->GetAmount()*tmpManure->GetAllN().n);
	Cbudget.AddInput(tmpManure->GetAmount()*tmpManure->GetC_content());
	/*#ifdef STALDDEBUG
 cout << " Receive manure " << Nbudget << endl;
#endif*/
	delete tmpManure;  //
}

/**
 * Send manure to the next link in the manure handling chain
 */

void manurestore::SendManure(const double amount, manure *aManure)
{
	product *tmpManure;
	tmpManure=GetFromManureStore(amount,(product *) aManure);
	if (tmpManure)
	{
		GetnextStore()->RcvManure((manure *)tmpManure);
		*aManure + *tmpManure;
		delete tmpManure;
	}
}

/**
 * Empty the store of manure, leaving a residue - manure sent to the relevant store
 */
bool manurestore::CleanStore(manure *aManure)
{
	bool cleaned = false;
	double amountCleaned=0;
	double postCleaningResidue=postCleanDepth*surfaceArea;
	if (GetAmountManure()>postCleaningResidue)   //cleaning efficiency may not be 100%
	{
		amountCleaned = GetAmountManure()- postCleaningResidue;
		SendManure(amountCleaned,aManure);
		cleaned=true;
	}
	else {aManure->Setamount(0);}
	return cleaned;
}

/**
 * Empty the store of all manure - manure sent to the relevant store
*/
void manurestore::FullyCleanStore(manure *aManure)
{
	SendManure(GetAmountManure(),aManure);
}

/**
Volatize ammonia from the manurestore - used for static (emission factor) ammonia modelling
 */
/*
double manurestore::Volatize()
{
	double volat=0;
// Volatilisation only occurs from ammonium in most manures but this system is not clever enough to handle that.
// Volat occurs from all N forms in proportion to their presence. Try to lose ammonium only and you will sooner or later have volat > ammonium present
   double ammoniumVolat=0, organicVolat=0, nitrateVolat=0;
   ammoniumVolat=(Emissionfactor/100.0) * (GetManure()->GetAmount() * GetManure()->GetNH4_content().n);       //in t
   organicVolat=(Emissionfactor/100.0) * (GetManure()->GetAmount() * GetManure()->GetorgN_content().n);       //in t
   nitrateVolat=(Emissionfactor/100.0) * (GetManure()->GetAmount() * GetManure()->GetNO3_content().n);       //in t

   volat=ammoniumVolat + organicVolat + nitrateVolat;       //in t

   GetManure()->SetNH4_content(GetManure()->GetNH4_content().n-ammoniumVolat/GetManure()->GetAmount());
   GetManure()->SetorgN_content(GetManure()->GetorgN_content().n-organicVolat/GetManure()->GetAmount());
   GetManure()->SetNO3_content(GetManure()->GetNO3_content().n-nitrateVolat/GetManure()->GetAmount());
   if (GetManure()->GetNH4_content().n<0)
#ifdef _STANDALONE
					{
						cout << "manurestore: ammonia volat exceeds NH4 N";
                  exit(0);
               }
#else
   		theMessage->FatalError(GetName()," ","manurestore: ammonia volat exceeds NH4 N");
#endif
   if (GetManure()->GetorgN_content().n<0)
#ifdef _STANDALONE
					{
						cout << "manurestore: ammonia volat exceeds organic N";
                  exit(0);
               }
#else
   		theMessage->FatalError(GetName()," ","manurestore: ammonia volat exceeds organic N");
#endif
   if (GetManure()->GetNO3_content().n<0)
#ifdef _STANDALONE
					{
						cout << "manurestore: ammonia volat exceeds nitrate N";
                  exit(0);
               }
#else
   		theMessage->FatalError(GetName()," ","manurestore: ammonia volat exceeds nitrate N");
#endif
   Nbudget.AddOutput(volat);
   return volat;
}
 */
/**
 * Volatize ammonia and other N gases from the manurestore - used for dynamic modelling
 */

void manurestore::Volatize(double Ameantemp, double relHumidity, double precipitation, double surfaceArea, double resistance,
		double *volatAmmonia, double *waterEvap, double *N2Emission, double *N2OEmission, double *CH4Emission, double *CO2Emission)
{
	if ((surfaceArea>0.0)&& (GetAmountInManureStore()>0.0))
	{
		manure * aManure=(manure *) theStore->GetProductPointer();
		double duration = 24.0; //hours per day
		precipitation/=duration;  //convert from mm per day to mm/hr
		double evaporation = aManure->GetWaterEvap(Ameantemp, relHumidity, surfaceArea, resistance, duration);
		evaporation*= 1000.0/(surfaceArea * duration);  //convert to kg per sq metre per hr
		double infiltration = 0;
		double Nmineralisation =0.0;
		//      aManure->VolatNitrogen(Ameantemp, evaporation, infiltration, precipitation, surfaceArea,
		//    	resistance, duration, volatAmmonia, waterEvap, N2Emission, N2OEmission);
		*volatAmmonia=aManure->VolatAmmonia(Ameantemp, evaporation, infiltration, precipitation, surfaceArea,
				resistance, duration, &evaporation, NULL);
		aManure->CalcOMdegradation(Ameantemp, duration, OMdegRateConst, CO2Emission, CH4Emission, &Nmineralisation);
		aManure->doDenitrification(Ameantemp, duration, N2Emission, N2OEmission);
		Nbudget.AddOutput(*volatAmmonia + *N2Emission + * N2OEmission);
		Cbudget.AddOutput(*CH4Emission + *CO2Emission);
		if (evaporation>0.0)
			aManure->SubtractWater(evaporation * surfaceArea * duration/1000.0);
	}
}


/**
 * Returns amount if the product p is a manure - provided for compatibility with theBuild
 */
double manurestore::GetAvailableAmountOf(const product * p)
{
	//	int index;
	double AvailAmount=0.0;
	if (theStore->ProductCompare(p))
		AvailAmount=theStore->Getamountstored();
	return AvailAmount;
}

/**
 * Extracts the requested product from the manure store
 */
product * manurestore::GetFromManureStore(double RequestedAmount, product* p, bool checkAccess)
{
	double Extractedamount=0, NExtracted=0, CExtracted=0;
	product * aManure = NULL;
	bool permitAcccess = true;   //gatekeeper - ensures that fields do not steal manure from floors, inhouse pits etc
	if ((checkAccess) && (locked))
		permitAcccess=false;
	if (permitAcccess)
	{
		if (theStore->ProductCompare(p))
		{
			if (p->GetmainUnitName()==3)	//amount to be applied is expressed in kg N not t FW, NJH Feb 2009
			{
				manure * theManureStored = ((manure *) theStore->GetProductPointer());
				RequestedAmount = p->GetAmount()/(theManureStored->GetAllN().n * 1000.0);
			}
			aManure=theStore->GetFromStore(RequestedAmount);
			if (aManure)
			{
				Extractedamount=aManure->GetAmount();
				NExtracted=Extractedamount*((manure *) theStore->GetProductPointer())->GetAllN().n;
				Nbudget.AddOutput(NExtracted);
				CExtracted=Extractedamount*((manure *) theStore->GetProductPointer())->GetC_content();
				Cbudget.AddOutput(CExtracted);
			}
		}
	}
	/*#ifdef STALDDEBUG
 cout << " GetFromManureStore " << Nbudget << endl;
#endif*/
	return aManure;
}

double manurestore:: GetCapacity()
{
	return theStore->GetCapacity();
}


/**
Set manure not using init (used by flooring in animal housing)
 */

void manurestore:: SetManure(manure *aManure)
{
	theStore->SetProductPointer(aManure);
}


/**
 * Check for continuity in N
 */
void manurestore::EndBudget(bool show)
{
	manure * aManure = (manure *) theStore->GetProductPointer();
	if ((aManure->GetpH()<1.0)||(aManure->GetpH()>12.0))
	{
#ifdef _STANDALONE
		{
			cout << "manurestore: pH error";
			exit(0);
		}
#else
		theMessage->FatalError(GetName()," ","manurestore: pH error");
#endif
	}

	//N budget
	double NRemaining=aManure->GetAmount()*aManure->GetAllN().n;
	if (!Nbudget.Balance(NRemaining))
	{
		if (show)
		{
			cout << Nbudget;
			cout << "NRemaining " << NRemaining << endl;
		}
	}
	//C budget
	double CRemaining=aManure->GetAmount()*aManure->GetC_content();
	if (!Cbudget.Balance(CRemaining))
	{
		if (show)
			cout << Cbudget;
		cout << "CRemaining " << CRemaining << endl;
	}
}

/**
 * 	Operator << for output
*/

ostream& operator<< (ostream& os,const manurestore& s)
{
  os << setiosflags(ios::left) << setw(40) << "Manure store details for " << ((base*)&s)->GetName()<< endl;
  os << setiosflags(ios::left) << setw(40) << "Alias " << s.Alias << endl;
  switch (theControlParameters->GetvolatModel())
  {
 	case 0:
    {
//   	os << setiosflags(ios::left) << setw(40) << "emission factor " << s.Emissionfactor << endl;
   	break;
    }
   case 1:
   {
	  os << setiosflags(ios::left) << setw(40) << "surface area " << s.surfaceArea << endl;;
     break;
   }
  }

  os << setiosflags(ios::left) << setw(40) << "manure to " << s.manureToName << endl;
  os << setiosflags(ios::left) << setw(40) << "manure to index " << s.manureToIndex << endl;
  os << setiosflags(ios::left) << setw(40) << "next store name " << s.nextStore -> GetAlias() << endl;
  //os << *s.theStore;
  return os;
}

/**
 */
void manurestore::ReceiveBedding(plantItem *bedding)
{
	//	manure * aManure = (manure *) theStore->GetProductPointer();
	manure * dummyBedding = new manure(*bedding);
	RcvManure(dummyBedding);
	delete dummyBedding;
};

/**
	Scale the manure storage that receives material from this instore  Note this sets capacity
 */
void manurestore::Scale(double annualAmount)
{
	SetCapacity(GetCapacity()+annualAmount);
	surfaceArea += annualAmount/depth;
}


double manurestore::GetAmountInManureStore()
{
	double amountPresent = ((manure *)theStore->GetProductPointer())->GetAmount();
	return amountPresent;
}
/**
 * calculate how much Ammonia(NH3), N2, double, N2O,  CH4 that evaporate. Clost is the total amount of C that is evaporated and Clost is total amount of C that is evaporated
 */
void manurestore::Volatize(double *Nlost, double *Clost,double *volatAmmonia, double *N2Emission, double *N2OEmission, double *CH4Emission,
		double *CO2Emission)
{
	*Nlost=0.0;
	*Clost=0.0;
	double NH3vol=0.0, N2vol=0.0, N2Ovol=0.0, MinN;
	manure * aManure=(manure *) theStore->GetProductPointer();
	//  cout << "manstore vol in " << (aManure->GetAmount() * aManure->GetAllN().n) << endl;
	if (aManure->GetAmount()>0.0)
		aManure->VolatAndMinNitrogen(NH3Emissionfactor, N2Emissionfactor, N2OEmissionfactor, NMinfactor,
				&NH3vol, &N2vol, &N2Ovol, &MinN);
	//  cout << "manstore vol out " << (aManure->GetAmount() * aManure->GetAllN().n) << endl;
	//  cout << "manstore vol out + vol " << (aManure->GetAmount() * aManure->GetAllN().n + NH3vol) << endl;
	*volatAmmonia=NH3vol;
	*N2Emission=N2vol;
	*N2OEmission=N2Ovol;
	*Nlost+=NH3vol+N2vol+N2Ovol;
	//calculate how much C that is reacting with O. This is removed from manure
	double volat = aManure->VolatCarbon(CO2Emissionfactor);
	*Clost=volat;
	*CO2Emission=volat;
	//calculate how much C that is reacting with H. This is removed from manure
	volat = aManure->VolatCarbon(CH4Emissionfactor);
	*CH4Emission=volat;
	*Clost+=volat;
}

/**
 * remove everything from the manurestore
 */
product *manurestore::EmptyManureStore()
{
	//product *tmpManure;
	product *aManure;
	double amountStored=theStore->Getamountstored();
	//   tmpManure=theStore->GetProductPointer();
	//   aManure=theStore->GetFromStore(amountStored,tmpManure);
	aManure=theStore->GetFromStore(amountStored);
	return aManure;
}

void manurestore::OpenDetailsFile()
{
	fileAccess hd;
	string pathName=hd.getCurrentPath();
	hd.changeDir(globalSettings->getOutputDirectory());
#ifdef  __BCplusplus__
	char buffer[MAX_TXT];
	getcwd(buffer, MAX_TXT);
	int run=globalSettings->getCurrentRun();
	chdir(globalSettings->getOutputDirectory().c_str());

#endif
	string namestr=GetAlias();;
	char Indexstr[10];
	sprintf(Indexstr,"(%d)",aBase->GetIndex());
	namestr=namestr+Indexstr+".xls";
	detailsFile = new fstream(namestr.c_str(),ios::out);
	ShowManureStore(detailsFile,true);
	*detailsFile << endl;
#ifdef  __BCplusplus__
	chdir(buffer);
#endif
	hd.changeDir(pathName);

}

void manurestore::CloseDetailsFile()
{
	detailsFile->close();
	delete detailsFile;
	detailsFile = NULL;
}

void manurestore::ShowManureStore(fstream * afile, bool header)
{
	if (header)
	{
		*afile << "Day"<< "\t"<< "Name" << "\t"
				<< "StoredAmount" << "\t"<< "StoredDM" << "\t"<< "StoredC" << "\t"<< "StoredN" << "\t"
				<< "CH4Emission" << "\t" << "CO2Emission" << "\t" << "volatAmmonia" << "\t" << "N2Emission" << "\t" << "N2OEmission" << "NOEmission" << "\t" ;
	}
	else
	{
		*afile << theTime.GetJulianDay() << "\t"<< GetAlias() << "\t"
				<< GetAmountManure() << "\t"<< GetTotalDMStored() << "\t"<< GetTotalCStored() << "\t"<< GetTotalNStored() << "\t"
				<< CH4Emission << "\t" << CO2Emission << "\t" << NH3Emission << "\t" << N2Emission << "\t" << N2OEmission << "\t" << NOEmission<< "\t" ;
	}
}

