/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "fieldOrder.h"
#include "../products/products.h"
#include "../base/controlParameters.h"

/****************************************************************************\
\****************************************************************************/
fieldOrder::fieldOrder(const char * aName, const int aIndex, const base * aOwner)
   : base(aName, aIndex, aOwner)
{
	strcpy(crop_id,"                                  ");
   field_no=-1;
   start_year=0;

   newArea=0.0;

   area=0.0;
   dist=0.0;
   N=0.0;
	P=0.0;
   K=0.0;
	solid_hug=0.0;
   fluid_hug_fall=0.0;
   fluid_hug_spring=0.0;
   fluid_hug_is_slurry=1;
   Automatic=true;
#ifdef TUPLE
   tuple_file_open=false;
   last_date="";
#endif
}

/****************************************************************************\
\****************************************************************************/
int fieldOrder::GetYear(string * date)
{
   bsTime bs;
   bs.SetTime(*date);
   return bs.GetYear();
}

/****************************************************************************\
\****************************************************************************/
int fieldOrder::GetMonth(string * date)
{
   bsTime bs;
   bs.SetTime(*date);
   return bs.GetMonth();
}

/****************************************************************************\
\****************************************************************************/
int fieldOrder::GetDay(string * date)
{
   bsTime bs;
   bs.SetTime(*date);
   return bs.GetDay();
}


/****************************************************************************\
\****************************************************************************/
double fieldOrder::GetFractionStrawHarvested(int nr)
{
	if (FractionStrawHarvested.NumOfNodes()>nr)
   	return *FractionStrawHarvested.ElementAtNumber(nr);
   else
   	return 1.0-theControlParameters->GetStrawLeft();
}
/****************************************************************************\
\****************************************************************************/
double fieldOrder::GetFractionGrainHarvested(int nr)
{
	if (FractionStrawHarvested.NumOfNodes()>nr)
   	return *FractionGrainHarvested.ElementAtNumber(nr);
   else
   	return 1.0-theControlParameters->GetHarvestWaste();
}

/****************************************************************************\
\****************************************************************************/
void fieldOrder::CheckDateStringForEmpty(string * DateString)
{
   if (*DateString=="")
      theMessage->FatalError("fieldOrder::CheckDateStringForEmpty - date not specified");
}

/****************************************************************************\
There is not a totally satisfactory solution concerning manure in this
function.
\****************************************************************************/
void fieldOrder::ReadParameters(fstream * f)
{
   Setfile(f);
   SetCritical();
   FindSection(Name,Index);

	const int strlen=255;
	typedef char strtyp[strlen];
	strtyp s,s1;

   GetParameter("Ha",&area);

   UnsetCritical();
   int master=-1;
   GetParameter("Master",&master);
   if (master>=0)
      if (master!=Index && area<0.0002)
         FindSection(Name,master);

   if (!(area>0.0))
	   theMessage->FatalError("fieldOrder::ReadParameters - area must be bigger than zero");
   if (theControlParameters->GetUniSizeFields())
      area=1.0; // !!! For testing purposes

   SetCritical();
	GetParameter("CROP",s);
   string st=s;
   strcpy(s1,s);
   s1[2]=0;
	SetCropId(s1);
	for (int i=0;i<6;i++)
		s[i]=48;
   if (master<0)
   	field_no=atoi(s);
   else
      field_no=Index;

   UnsetCritical();

   double factor=1.0;                                      // Factor for fertiliser and manure amount
   int unitType=theControlParameters->GetFertiliserUnit(); // Default value for unitType = 0 -> fertiliser given per ha in file
   if (unitType<0 || unitType>2)
      theMessage->FatalError("fieldOrder::ReadParameters - 'unitType' has illegal value");
   if (unitType==1)                                         // Total amount of fertiliser or manure applied to field, option for backward compatability
      factor=1.0/area;

   GetParameter("Automatic",&Automatic);
   if (Automatic==0)
   {
      int first,size;

      GetVectorInfo("TillageDate",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         string * s = new string;
         GetParameter("TillageDate",s,i);
         CheckDateStringForEmpty(s);
         TillageDate.InsertLast(s);
         s = new string("");
         GetParameter("TillageType",s,i);
         TillageType.InsertLast(s);
         double * a = new double(80.0);
         GetParameter("TillageDepth",a,i);
         TillageDepth.InsertLast(a);
         a = new double(0.95);
         GetParameter("TillageIncorporation",a,i);
         TillageIncorporation.InsertLast(a);
      }

      // Added for backward compatibility (november 2006 BMP)
      GetVectorInfo("PloughDate",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         string * s = new string;
         GetParameter("PloughDate",s,i);
         CheckDateStringForEmpty(s);
         TillageDate.InsertLast(s);
         s = new string("Ploughing");
         TillageType.InsertLast(s);
         double * a = new double(250.0);
         TillageDepth.InsertLast(a);
         a = new double(0.95);
         TillageIncorporation.InsertLast(a);
      }

      // Added for backward compatibility (november 2006 BMP)
      GetVectorInfo("HarrowDate",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         string * s = new string;
         GetParameter("HarrowDate",s,i);
         CheckDateStringForEmpty(s);
         TillageDate.InsertLast(s);
         s = new string("Harrowing");
         TillageType.InsertLast(s);
         double * a = new double(80.0);
         TillageDepth.InsertLast(a);
         a = new double(0.95);
         TillageIncorporation.InsertLast(a);
      }

      GetVectorInfo("SowCrop",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         string * s=new string;
         GetParameter("SowCrop",s,i);
         CheckDateStringForEmpty(s);
         SowCrops.InsertLast(s);
         s=new string;
         GetParameter("SowDate",s,i);
         SowDates.InsertLast(s);
         double * a=new double(0.0);
         GetParameter("SowAmount",a,i);
         SowAmounts.InsertLast(a);
#ifdef TUPLE
         if (*a>0.0 && area>0.001) // This is a crude accounting, as 1) zero amount is corrected later, and 2) 2% N is assumed for all seeds
            if (0==i)
               AddTuple(*s,*a*0.02*area,*a*area,"SOWING","SEED",0);
            else
               AddTuple(*s,*a*0.02*area,*a*area,"SOWING","SEED_SUB",0);
#endif

         s=new string;
      	if (GetParameter("SowStartDate",s,i))
               theMessage->FatalError("fieldOrder::ReadParameters - not using the MEASCOPE compiler option, you pillock");
         delete s;
      }

      GetVectorInfo("FertilizerDate",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         string * s=new string;
         GetParameter("FertilizerDate",s,i);
         CheckDateStringForEmpty(s);
         FertilizerDates.InsertLast(s);
         string * type=new string;
         GetParameter("FertilizerType",type,i);
         FertilizerTypes.InsertLast(type);
         double *incorpTime = new double(168.0); //time between application and incorporation in hours  (optional)
         GetParameter("IncorporationTime",incorpTime,i);
         IncorporationTimes.InsertLast(incorpTime);
         if (*type=="")
            theMessage->FatalError("fieldOrder::ReadParameters - must specify type of fertilizer");

         if (*type=="MINERAL")
         {
            int found=0;
            nitrogen * aNitrogen = new nitrogen;

            string* MineralFormString = new string;
            *MineralFormString = "SOLID";
            GetParameter("MineralForm",MineralFormString,i);
   			MineralForm.InsertLast(MineralFormString);


            if (GetParameter("FertilizerN",&aNitrogen->n,i))
               found++;
            if (aNitrogen->n<0.0)
            {
               theMessage->FatalError("fieldOrder::ReadParameters - negative amount of fertiliser");
               aNitrogen->n=0;
            }
            if (unitType==2)                                       // FarmN, minor screw-up !!!
               aNitrogen->n=aNitrogen->n*factor*100.0;
            else
               aNitrogen->n=aNitrogen->n*factor;
            if (GetParameter("FertilizerN15",&aNitrogen->n15,i))
               found++;
            aNitrogen->n15=aNitrogen->n15*factor;
            if (unitType!=1 && aNitrogen->n<10.0)
            {
               cout << " N: " << aNitrogen->n << endl;
               theMessage->WarningWithDisplay("fieldOrder::ReadParameters - N from mineral fertiliser below 10 kg per ha");
            }
            if (unitType!=1 && aNitrogen->n>400.0)
            {
               cout << " N: " << aNitrogen->n << endl;
               theMessage->WarningWithDisplay("fieldOrder::ReadParameters - N from mineral fertiliser above 400 kg per ha");
            }
#ifdef TUPLE
            if (area>0.001)
               AddTuple(*s,aNitrogen->n*area,0,"FERTILISATION","MINERAL_N",0);
#endif
            FertilizerN.InsertLast(aNitrogen);
            double* a = new double(0.0);
            if (GetParameter("FertilizerP",a,i))
               found++;
            *a=*a*factor;
            FertilizerP.InsertLast(a);
            a=new double(0.0);
            if (GetParameter("FertilizerK",a,i))
               found++;
            *a=*a*factor;
            FertilizerK.InsertLast(a);
            a=new double(0.0);
            if (GetParameter("FertilizerCl",a,i))             // Mainly used for leaching studies
               found++;
            *a=*a*factor;
            FertilizerCl.InsertLast(a);
            if (found==0)
               theMessage->FatalError("fieldOrder::ReadParameters - must specify N, P or K content of mineral fertilizer");
         }
         else
            if ((*type=="CATTLE-SLURRY-FRESH") || (*type=="CATTLE-SLURRY-STORED") || (*type=="CATTLE-SLURRY-ANAEROB") || (*type=="CATTLE-COMPOST")
            	|| (*type=="CATTLE-FYM") || (*type=="PIG-SLURRY-STORED") || (*type=="PIG-SLURRY-ANAEROB")
               || (*type=="PIG-COMPOST") || (*type=="PIG-FYM") || (*type=="PIG-SLURRY-FRESH") || (*type=="PIG-SLURRY-DEGAS") || (*type=="SLURRY")
               || (*type=="LIQUID-MANURE") || (*type=="SOLID-MANURE"))
            {
               manure* aManure = new manure;
               aManure->Setname(*type);
               //!call to check if using manureParameters.dat

	      		theProducts->GiveProductInformation(aManure);
//!Get the amount of manure applied in tonnes fresh weight (ManureAmount) or in kilograms of nitrogen (FertilizerN)
//!amounts could be per hectare or per field (will be adjusted by 'factor' later)
      			double amount = 0.0;
               double amountTotalN = 0.0;
            	GetParameter("FertilizerN",&amountTotalN,i);
               GetParameter("ManureAmount",&amount,i);
	            if ((amountTotalN==0.0)&& (amount==0.0))
	               theMessage->FatalError("fieldOrder::ReadParameters - amount of manure must be input in tonnes fresh weight (ManureAmount) or kilograms N (FertilizerN)");
	            if ((amountTotalN>0.0)&& (amount>0.0))
	               theMessage->FatalError("fieldOrder::ReadParameters - amount of manure cannot be input in both tonnes fresh weight (ManureAmount) and kilograms N (FertilizerN)");

               if (amountTotalN>0.0)
                  amount=amountTotalN/(aManure->GetAllN().n*1000.0);   //calculate tonnes fresh manure from total N applied (kg) and manure N content
               if (unitType==2)   // FarmN, minor screw-up
                  aManure->Setamount(amount*factor*0.1);
               else
                  aManure->Setamount(amount*factor);
//!do some primitive quality control
               if (amount<5.0)
               {
                  cout << endl << "Manure amount " << amount<< " t/ha, containing " << (1000.0*amount*aManure->GetTotalN().n) << " kg N/ha" << endl;
                  theMessage->WarningWithDisplay("fieldOrder::ReadParameters - manure amount below 5 tonnes per ha");
               }
               if (amount>100.0)
               {
                  cout << endl << "Manure amount " << amount << " t/ha, containing " << (1000.0*amount*aManure->GetTotalN().n) << " kg N/ha" << endl;
                  theMessage->WarningWithDisplay("fieldOrder::ReadParameters - manure amount above 100 tonnes per ha");
               }
#ifdef TUPLE
               if (area>0.001)
                  AddTuple(*s,aManure->GetTotalN().n*amount*area*1000.0,amount*area,"FERTILISATION","MANURE",0);
#endif
               Manure.InsertLast(aManure);
            }
            else
            {
               cout << "Fertilizer type: " << *type << endl;
               theMessage->FatalError("fieldOrder::ReadParameters - unknown fertilizer type");
            }
      }

      GetVectorInfo("IrrigationDate",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         string * s=new string;
         GetParameter("IrrigationDate",s,i);
         CheckDateStringForEmpty(s);
         IrrigationDates.InsertLast(s);
         double * a=new double(0.0);
         GetParameter("IrrigationAmount",a,i);
         if (*a==0.0)
            theMessage->FatalError("fieldOrder::ReadParameters - irrigation amount zero or unspecified");
         IrrigationAmounts.InsertLast(a);
         a=new double(0.0);
         GetParameter("IrrigationNcontent",a,i);
         IrrigationNContents.InsertLast(a);
      }

      GetVectorInfo("HarvestDate",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         string * s=new string;
         GetParameter("HarvestDate",s,i);
         CheckDateStringForEmpty(s);
         HarvestDates.InsertLast(s);
         bool removeRecorded=false;
#ifdef TUPLE
         double am,n;

         if (GetParameter("GrainYield",&n,i))       // M�ngde N
         {
            am=0.0;
            GetParameter("GrainYieldAmount",&am,i); // M�ngde vare

            if (area>0.001 && (n<0.0 || am>0.0))
               AddTuple(*s,n,am,"HARVEST","MAIN_PRODUCT",0);

         }
         if (GetParameter("StrawYield",&n,i))       // M�ngde N
         {
            am=0.0;
            GetParameter("StrawYieldAmount",&am,i); // M�ngde vare
            if (area>0.001 && (n<0.0 || am>0.0))
            {
               AddTuple(*s,n,am,"HARVEST","SECONDARY_PRODUCT",0);
               removeRecorded=true;
            }

         }
#endif
         bool HarvestRemoveStraws = true;                       // Default remove straw.

         GetParameter("HarvestRemoveStraw",&HarvestRemoveStraws,i);
         bool *Remove = new bool(HarvestRemoveStraws>0 || removeRecorded);
         HarvestRemoveStraw.InsertLast(Remove);


      	double * a = new double(0.0);
         GetParameter("FractionStrawHarvested",a,i);
         FractionStrawHarvested.InsertLast(a);

         a = new double(0.0);
         GetParameter("FractionGrainHarvested",a,i);
         FractionGrainHarvested.InsertLast(a);
      }

      GetVectorInfo("CuttingDate",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         string * s=new string;
         GetParameter("CuttingDate",s,i);
         CheckDateStringForEmpty(s);
         CuttingDates.InsertLast(s);
         bool CuttingRemoveStraw=false;
         GetParameter("CuttingRemoveStraw",&CuttingRemoveStraw,i);

         CuttingRemovesStraw.InsertLast(&CuttingRemoveStraw);
         double * height=new double(0.07);

         GetParameter("CuttingHeight",height,i);
         
         if (*height>1.0) //cutting height was input in cm
         {
	         theMessage->WarningWithDisplay("fieldOrder::ReadParameters - Cutting height converted from cm to m");
         	*height = *height/100;    //convert to meters
         }
         CuttingHeights.InsertLast(height);

         int *b = new int(1);		//default is forage harvesting
         GetParameter("CuttingProcessProduct",b,i);
         CuttingProcessProducts.InsertLast(b);
#ifdef TUPLE
         double am,n;
         if (GetParameter("ForageYield",&n,i))       // M�ngde N
         {
            am=0.0;
            GetParameter("ForageYieldFeed",&am,i); // SFU

            if (area>0.001 && (n>0.0 || am>0.0))
               AddTuple(*s,n,0,"HARVEST","FORAGE/SILAGE",am);

         }
#endif
      }

      GetVectorInfo("PesticideDate",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         string * s=new string;
         GetParameter("PesticideDate",s,i);
         CheckDateStringForEmpty(s);
         PesticidesDates.InsertLast(s);
         s = new string;
         GetParameter("PesticideType",s,i);
         PesticidesTypes.InsertLast(s);
      }
#ifdef TUPLE
      GetVectorInfo("GrazingDate",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         string s;
         double am,n;
         GetParameter("GrazingDate",&s,i);
         CheckDateStringForEmpty(&s);
         if (GetParameter("GrazingYield",&n,i))       // M�ngde N
         {
            am=0.0;
            GetParameter("GrazingYieldFeed",&am,i);   // M�ngde SFU

            if (area>0.001 && (n<0.0 || am>0.0))
               AddTuple(s,n,0,"GRAZING","GRAZED",am);

         }
      }
#endif

      GetVectorInfo("GrazingStartDate",&first,&size);
      for (int i=first;i<(first+size);i++)
      {
         SetCritical();
         string * s = new string;
         GetParameter("GrazingStartDate",s,i);
         CheckDateStringForEmpty(s);
         GrazingStartDates.InsertLast(s);
         s = new string;
         GetParameter("GrazingEndDate",s,i);
         CheckDateStringForEmpty(s);
         GrazingEndDates.InsertLast(s);
         double * a=new double(0.0);
         GetParameter("NitrogenLoad",a,i);
         if (*a<0.0)
            theMessage->FatalError("fieldOrder::ReadParameters - Nitrogen below zero or unspecified");
         NitrogenLoad.InsertLast(a);
         UnsetCritical();
         double *Efficiency = new double(0.84);
         GetParameter("GrazingEfficiency",Efficiency,i);
         GrazingEfficiency.InsertLast(Efficiency);
      }
   }
   else
   {
      fluid_hug_spring = 0.0;
   	GetParameter("SLUR.PS",&fluid_hug_spring);
      fluid_hug_spring=fluid_hug_spring*factor;
     	bool solidHugUsed=false;
     	solidHugUsed=GetParameter("DUNG.PS",&solid_hug);
      solid_hug=solid_hug*factor;
      double a=0.0;
      GetParameter("LIQM.PS",&a);
      fluid_hug_spring+=a*factor;
      if (a>0)
         fluid_hug_is_slurry=0;
   	GetParameter("SLUR.PA",&fluid_hug_fall);
      fluid_hug_fall=fluid_hug_fall*factor;
   	if(GetParameter("DUNG.PA",&solid_hug));
		if(solidHugUsed==true)
			theMessage->FatalError("fieldOrder::ReadParameters: Cannot use both DUNG.PA and DUNG.PS");
      solid_hug=solid_hug*factor;
      a=0.0;
   	GetParameter("LIQM.PA",&a);
    	fluid_hug_fall+=a*factor;
      if (a>0)
         fluid_hug_is_slurry=0;

      // Rigtige perioder til nedenst. For alle afledte af static_crop undtagen raps skal der ikke kunne gives gylle om efter�ret (warning). �rt m� ikke f� N-g�dn. overhovedet

   	if(GetParameter("SLUR.P00",&a)==true)
   		theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
	if(GetParameter("SLUR.P01",&a)==true)
		theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(GetParameter("SLUR.P02",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(GetParameter("SLUR.P03",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(GetParameter("SLUR.P04",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(GetParameter("SLUR.P05",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(GetParameter("SLUR.P06",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(GetParameter("SLUR.P07",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(GetParameter("SLUR.P08",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(GetParameter("SLUR.P09",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(GetParameter("SLUR.P10",&a)==true)
   		theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(GetParameter("SLUR.P11",&a)==true)
   		theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
      SetFluidManureSpring(fluid_hug_spring);
   	GetParameter("kg-N-",&N);
      N=N*factor;
   	GetParameter("kg-P-",&P);
      P=P*factor;
   	GetParameter("kg-K-",&K);
      K=K*factor;

   }
	Setfile(NULL);
}



#ifdef TUPLE
/****************************************************************************\
\****************************************************************************/
int fieldOrder::GetOperationIndex(string date)
{
   if (last_date==date)
      last_idx++;
   else
      last_idx=1;
   last_date=date;
   return last_idx;
}

/****************************************************************************\
\****************************************************************************/
void fieldOrder::CloseTupleFile()
{
   if (tuple_file_open)
   {
      tuple.close();
      tuple_file_open=false;
   }
}

/****************************************************************************\
\****************************************************************************/
void fieldOrder::OpenTupleFile(string s)
{
   string s1="c:\\fasset_v2\\F_"+s+"_obs.txt";
   if (theControlParameters->GetObsTupleHasBeenOpened())
      tuple.open(s1.c_str(),ios::app);
   else
   {
      tuple.open(s1.c_str(),ios::out);
      tuple << "Key\tFarmId\tFieldno\tDate\tIndex\tMainCrop\tArea\tOperation\tProduct\tN\tAmount\tSFU" << endl;
      theControlParameters->SetObsTupleHasBeenOpened(true);
      theControlParameters->SetWriteTuples(true);
   }
   tuple_file_open=true;
}

/****************************************************************************\
Parametre: dato,N,m�ngde,operation,type
N�gle (unik)	Bedrifts-id	Marknummer	Dato	Index (ved flere op. samme dato)	Hovedafgr�de	Areal (ha)	Operation	Produktnavn	H�stet N (kg)	M�ngde (kg - ikke sl�t)	H�stet FE
Eksempel: 95023_8_15-08-2001_1_RY	95023	8	15-08-2001	1	RY	6.4	Combineharvesting	RYEGRAIN	478.9	29440	-
\****************************************************************************/
void fieldOrder::AddTuple(string date,double n,double amount,string op,string type,double SFU)
{
   string farmId=theControlParameters->GetFarmID();
   if (farmId=="")
      theMessage->FatalError("fieldOrder::AddTuple - attempt to add tuples but no farm ID specified");
   if (!tuple_file_open)
      OpenTupleFile(farmId);
   int idx = GetOperationIndex(date);
   tuple << farmId << "_" << field_no << "_" <<  date << "_" << idx << "_" << crop_id << "\t";
   tuple << farmId << "\t" << field_no << "\t" << date << "\t" << idx << "\t";
   tuple << crop_id << "\t" << area << "\t" << op << "\t" << type << "\t" << n;
   tuple << "\t" << amount << "\t" << SFU << endl;
}

#endif

//---------------------  MEASCOPE related code below -------------------------
#ifdef MEASCOPE

/****************************************************************************\
There is not a totally satisfactory solution concerning manure in this
function.
This is the MEASCOPE version of "ReadParameters"
\****************************************************************************/
void fieldOrder::ReadParameters(fstream * f, bool start)   //horrible fudge to solve error in MEASCOPE input
{
   Setfile(f);
   SetCritical();
   FindSection(Name,Index);

	const int strlen=255;
	typedef char strtyp[strlen];
	strtyp s,s1;

   GetParameter("Ha",&area);

   UnsetCritical();
   int master=-1;
   GetParameter("Master",&master);
   if (master>=0)
      if (master!=Index && area<0.0002)
         FindSection(Name,master);

   if (!(area>0.0))
	   theMessage->FatalError("fieldOrder::ReadParameters - area must be bigger than zero");
   if (theControlParameters->GetUniSizeFields())
      area=1.0; // !!! For testing purposes

   SetCritical();
	GetParameter("CROP",s);
   string st=s;
   strcpy(s1,s);
   s1[2]=0;
	SetCropId(s1);
   GetParameter("Automatic",&Automatic);
	if ((start) && (!strcmp(s1,"C2")) )
   {
   	Automatic=true;
      strcpy(s1,"C1");
	}
	for (int i=0;i<6;i++)
		s[i]=48;
   if (master<0)
   	field_no=atoi(s);
   else
      field_no=Index;

   UnsetCritical();
   strcpy(s,"\0");
   if (!GetParameter("N-LEVEL",s))
   	SetNLevel(0);
   else
   	SetNLevel(s[1]-48);
   pv_strat = 2;                                 // by default high level of protection
   GetParameter("PEST-LEVEL",s);
   if (strcmp(s,"\0")!=0)
   	pv_strat=s[4]-48;

   double factor=1.0;                                  // Factor for fertiliser amount
   int unitType=theControlParameters->GetFertiliserUnit(); // Default value for unitType = 0 -> fertiliser given per ha in file
   if (unitType<0 || unitType>2)
      theMessage->FatalError("fieldOrder::ReadParameters - 'unitType' has illegal value");
   if (unitType==1)                                    // Total amount of fertiliser given in file, option for backward compatability
      factor=1.0/area;

   if (Automatic==0)
   {
	   //GetParameter("TillageStartDate",s,0);

		ReadPeriods();
   }
 else
   {
      fluid_hug_spring = 0.0;
   	GetParameter("SLUR.PS",&fluid_hug_spring);
      fluid_hug_spring=fluid_hug_spring*factor;
   	GetParameter("DUNG.PS",&solid_hug);
      solid_hug=solid_hug*factor;
      double a=0.0;
      GetParameter("LIQM.PS",&a);
      fluid_hug_spring+=a*factor;
      if (a>0)
         fluid_hug_is_slurry=0;
   	GetParameter("SLUR.PA",&fluid_hug_fall);
      fluid_hug_fall=fluid_hug_fall*factor;
   	GetParameter("DUNG.PA",&solid_hug);
      solid_hug=solid_hug*factor;
      a=0.0;
   	GetParameter("LIQM.PA",&a);
    	fluid_hug_fall+=a*factor;
      if (a>0)
         fluid_hug_is_slurry=0;

      // Rigtige perioder til nedenst. For alle afledte af static_crop undtagen raps skal der ikke kunne gives gylle om efter�ret (warning). �rt m� ikke f� N-g�dn. overhovedet
   	GetParameter("SLUR.P00",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P01",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P02",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P03",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P04",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P05",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P06",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P07",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P08",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P09",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P10",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
   	GetParameter("SLUR.P11",&a);
      fluid_hug_spring=fluid_hug_spring+a*factor;
      SetFluidManureSpring(fluid_hug_spring);

   	GetParameter("kg-N-",&N);
      N=N*factor;
   	GetParameter("kg-P-",&P);
      P=P*factor;
   	GetParameter("kg-K-",&K);
      K=K*factor;
   }
	Setfile(NULL);
}

/****************************************************************************\
\****************************************************************************/
//void fieldOrder::ReadPeriods(fstream * f)
void fieldOrder::ReadPeriods()
{
   int periodcount;
   int first,size;
   GetVectorInfo("TillageStartDate",&first,&periodcount);
   file->seekg(0);
   for (int i=0;i<periodcount;i++)
   {
      fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Tillage",i,this);
      aPeriod->Setcur_pos(Getcur_pos());
      aPeriod->ReadPeriod(file);
      TillagePeriods.InsertLast(aPeriod);
      string * s = new string;
      GetParameter("TillageType",s,i);
      TillageType.InsertLast(s);
      double * a = new double(80.0);
      GetParameter("TillageDepth",a,i);
      TillageDepth.InsertLast(a);
      a = new double(0.95);
      GetParameter("TillageIncorporation",a,i);
      TillageIncorporation.InsertLast(a);
   }

   GetVectorInfo("SowCrop",&first,&size);
   for (int i=first;i<(first+size);i++)
   {
      string * s=new string;
      GetParameter("SowCrop",s,i);
      SowCrops.InsertLast(s);
      s=new string;
      int dumint;
      GetVectorInfo("SowStartDate",&dumint,&periodcount);
      fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Sow",i,this);
      aPeriod->Setcur_pos(Getcur_pos());
      aPeriod->ReadPeriod(file);
      SowPeriods.InsertLast(aPeriod);
      double * a=new double(0.0);
      GetParameter("SowAmount",a,i);
      SowAmounts.InsertLast(a);
   }
   GetVectorInfo("FertilizerStartDate",&first,&periodcount);
   for (int i=0;i<periodcount;i++)  //number of fertiliser or manure applications
   {
      fieldOrderPeriod *aPeriod = new fieldOrderPeriod("Fertilizer",i,this);
      aPeriod->Setcur_pos(Getcur_pos());
      aPeriod->ReadPeriod(file);
      FertilizerPeriods.InsertLast(aPeriod);
      string * type=new string;
      GetParameter("FertilizerType",type,i);
      FertilizerTypes.InsertLast(type);
      if (*type=="")
      {
      	cout << "Field " << GetIndex() << " crop " << crop_id;
         theMessage->FatalError("fieldOrder::ReadParameters - must specify type of fertilizer");
      }
      double *incorpTime = new double(168.0); //time between application and incorporation in hours  (optional)
      GetParameter("IncorporationTime",incorpTime,i);
      IncorporationTimes.InsertLast(incorpTime);
      SetSignPositive();
      if (*type=="MINERAL")
      {
         int found=0;
         nitrogen * aNitrogen = new nitrogen;

         string* MineralFormString = new string;
         *MineralFormString = "SOLID";
         GetParameter("MineralForm",MineralFormString,i);
         MineralForm.InsertLast(MineralFormString);
         double* NH4_fraction = new double(0.0);
         GetParameter("NH4_fraction",NH4_fraction,i);
         NH4_fractions.InsertLast(NH4_fraction);

         if (GetParameter("FertilizerN",&aNitrogen->n,i))
            found++;
         if (GetParameter("FertilizerN15",&aNitrogen->n15,i))
            found++;
         FertilizerN.InsertLast(aNitrogen);
         double* a = new double(0.0);
         if (GetParameter("FertilizerP",a,i))
            found++;
         FertilizerP.InsertLast(a);
         a=new double(0.0);
         if (GetParameter("FertilizerK",a,i))
            found++;
         FertilizerK.InsertLast(a);
         a=new double(0.0);
         if (GetParameter("FertilizerCl",a,i))             // Mainly used for leaching studies
            found++;
         FertilizerCl.InsertLast(a);
         if (found==0)
            theMessage->FatalError("fieldOrder::ReadParameters - must specify N, P or K content of mineral fertilizer");
      }
      else
         if ((*type=="CATTLE-SLURRY-FRESH") || (*type=="CATTLE-SLURRY-STORED") || (*type=="CATTLE-SLURRY-ANAEROB") || (*type=="CATTLE-COMPOST")
            || (*type=="CATTLE-FYM") || (*type=="PIG-SLURRY-STORED") || (*type=="PIG-SLURRY-ANAEROB")
            || (*type=="PIG-COMPOST") || (*type=="PIG-FYM") || (*type=="PIG-SLURRY-FRESH") || (*type=="PIG-SLURRY-DEGAS") || (*type=="SLURRY"))
         {
            manure* aManure = new manure;
            aManure->Setname(*type);
            theProducts->GiveProductInformation(aManure);
            double amount = 0.0;
            double dm = 0.0;
            double organicC = 0.0;
            double volatisation = 0.0;
            GetParameter("DryMatter",&dm,i);
            GetParameter("OrganicCarbon",&organicC,i);                  // units amount pr amount fresh
            GetParameter("Volatisation",&volatisation,i);
            if (dm>0)
               aManure->SetDryMatter(dm);
            if (organicC>0)
               aManure->SetC_content(organicC);
            if (volatisation>0)
               aManure->Setvolatisation(volatisation);

            nitrogen am = aManure->GetNH4_content();
            GetParameter("AmmoniumContent",&am.n,i);
            GetParameter("AmmoniumN15Content",&am.n15,i);
            if (am.n>0)
               aManure->SetNH4_content(am);
            nitrogen on = aManure->GetorgN_content();;
            GetParameter("OrganicNitrogen",&on.n,i);
            GetParameter("OrganicN15Nitrogen",&on.n15,i);
            if (on.n>0)
               aManure->SetorgN_content(on);
            GetParameter("ManureAmount",&amount,i);
            if (GetParameter("FertilizerN",&amount,i)) //allow manure application to be defined as kg N/ha
            {
	            amount/=aManure->GetAllN().n*1000.0;   //convert from kg N/ha to tonnes fresh manure/ha
		         double* NH4_fraction = new double(0.0);
		         if (GetParameter("NH4_fraction",NH4_fraction,i))
               {
						aManure->SetNH4_content(aManure->GetAllN().n*(*NH4_fraction));
						aManure->SetorgN_content(aManure->GetAllN().n*(1-*NH4_fraction));
               }
            }
            if (amount>0)
               aManure->Setamount(amount);

            Manure.InsertLast(aManure);
         }
         else
            theMessage->FatalError("fieldOrder::ReadParameters - unknown fertilizer type");
   }  //end of fertiliser

   GetVectorInfo("IrrigationStartDate",&first,&periodcount);
   for (int i=0;i<periodcount;i++)
   {
      theMessage->FatalError("fieldOrder::ReadParameters - irrigation periods not implemented");
      fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Irrigation",i,this);
      aPeriod->Setcur_pos(Getcur_pos());
      aPeriod->ReadPeriod(file);
      IrrigationPeriods.InsertLast(aPeriod);
      double * a = new double(0.0);
      GetParameter("IrrigationAmount",a,i);
      if (*a==0.0)
         theMessage->FatalError("fieldOrder::ReadParameters - irrigation amount zero or unspecified");
      IrrigationAmounts.InsertLast(a);
      a=new double(0.0);
      GetParameter("IrrigationNcontent",a,i);
      IrrigationNContents.InsertLast(a);
  }
   GetVectorInfo("HarvestStartDate",&first,&periodcount);
   for (int i=0;i<periodcount;i++)
   {
      fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Harvest",i,this);
      aPeriod->Setcur_pos(Getcur_pos());
      aPeriod->ReadPeriod(file);
      HarvestPeriods.InsertLast(aPeriod);
      double *a = new double(1.0);                       // Default remove straw
      GetParameter("HarvestRemoveStraw",a,i);
      bool *Remove = new bool(*a>0);
      HarvestRemoveStraw.InsertLast(Remove);
      delete a;
      a = new double(0.0);
      GetParameter("FractionStrawHarvested",a,i);
      FractionStrawHarvested.InsertLast(a);
      a = new double(0.0);
      GetParameter("FractionGrainHarvested",a,i);
      FractionGrainHarvested.InsertLast(a);
   }
   GetVectorInfo("CuttingStartDate",&first,&periodcount);
   for (int i=0;i<periodcount;i++)
   {
      fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Cutting",i,this);
      aPeriod->Setcur_pos(Getcur_pos());
      aPeriod->ReadPeriod(file);
      CuttingPeriods.InsertLast(aPeriod);
      double *a = new double(0.0);
      GetParameter("CuttingRemoveStraw",a,i);
      bool *Remove = new bool(*a>0);
      delete a;
      CuttingRemovesStraw.InsertLast(Remove);

      double * height=new double(0.07);
      GetParameter("CuttingHeight",height,i);
      if (*height>1.0) //cutting height was input in cm
         *height/=100.0;
      CuttingHeights.InsertLast(height);

      int *b = new int(0);
      GetParameter("CuttingProcessProduct",b,i);
      CuttingProcessProducts.InsertLast(b);
   }
   GetVectorInfo("PesticideStartDate",&first,&periodcount);
   for (int i=0;i<(first+periodcount);i++)
   {
      theMessage->FatalError("fieldOrder::ReadParameters - pesticide periods not implemented");
      fieldOrderPeriod * aPeriod = new fieldOrderPeriod("Pesticide",i,this);
      aPeriod->Setcur_pos(Getcur_pos());
      aPeriod->ReadPeriod(file);
      PesticidePeriods.InsertLast(aPeriod);
      string *s = new string;
      GetParameter("PesticideType",s,i);
      PesticidesTypes.InsertLast(s);
   }
   {
      string * s = new string;
      int i;
      if (GetParameter("PesticideDate",s,i)||
         GetParameter("CuttingDate",s,i)||
         GetParameter("HarvestDate",s,i)||
         GetParameter("IrrigationDate",s,i)||
         GetParameter("FertilizerDate",s,i)||
         GetParameter("SowDate",s,i)||
         GetParameter("TillageDate",s,i))
               theMessage->FatalError("fieldOrder::ReadParameters - field operations input with fixed date format");
   }

}
#endif

