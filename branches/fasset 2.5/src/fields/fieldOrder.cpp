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
#include <string.h>
/****************************************************************************\
\****************************************************************************/
fieldOrder::fieldOrder(const char * aName, const int aIndex, const base * aOwner)
   : base(aName, aIndex, aOwner)
{
	strcpy(crop_id,"                                  ");
   field_no=-1;
   start_year=0;
	pv_strat=0;
   N_level=0;
   newArea=0.0;
   grazable=0;
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
void fieldOrder::ReadParameters(commonData * &data)
{

	data->FindSection(Name,Index);
	/*int periodCount = 0;

	char period[40];
	char temp[2];
    strcpy(period,Name);
	strcat(period,"(");
	sprintf(temp,"%01d",Index);
    strcat(period,temp);
	strcat(period,").Period");

	int first, num;
	data->setCritical(false);
	data->getSectionsNumbers(period,&first,&num);

	for(int i=first;i<=(first+num);i++)
	{
		if (num>=0)
			data->FindSection(period,periodCount);*/
	const int strlen=255;
	typedef char strtyp[strlen];
	strtyp s,s1;

	data->setCritical(true);

	data->FindItem("Ha",&area);
	data->setCritical(false);

   int master=-1;
   data->FindItem("Master",&master);
   if (master>=0)
      if (master!=Index && area<0.0002)
      {
    	  data->FindSection(Name,master);
    	  cout<<"find section "<<Name<<" "<<master<<endl;
      }

   if (!(area>0.0))
	   theMessage->FatalError("fieldOrder::ReadParameters - area must be bigger than zero");
   if (theControlParameters->GetUniSizeFields())
      area=1.0; // !!! For testing purposes

   data->setCritical(true);
   string st;
	data->FindItem("CROP",&st);
   strcpy(s,st.c_str());
   strcpy(s1,s);
   s1[2]=0;
	SetCropId(s1);
	for (int i=0;i<6;i++)
		s[i]=48;
   if (master<0)
   	field_no=atoi(s);
   else
      field_no=Index;

   data->setCritical(false);

   double factor=1.0;                                      // Factor for fertiliser and manure amount
   int unitType=theControlParameters->GetFertiliserUnit(); // Default value for unitType = 0 -> fertiliser given per ha in file
   if (unitType<0 || unitType>2)
      theMessage->FatalError("fieldOrder::ReadParameters - 'unitType' has illegal value");
   if (unitType==1)                                         // Total amount of fertiliser or manure applied to field, option for backward compatability
      factor=1.0/area;

   data->FindItem("Automatic",&Automatic);
   if (Automatic==0)
   {
      int first,size;

      data->FindVectorSize("TillageDate",&first,&size);
      for (int i=first;i<=(size);i++)
      {
         string * s = new string;
         data->FindItem("TillageDate",s,i);
         CheckDateStringForEmpty(s);
         TillageDate.InsertLast(s);
         s = new string("");
         data->FindItem("TillageType",s,i);
         string PLOUGH="PLOUGH";
         string HARROW="HARROW";
         if(s->compare(PLOUGH)!=0&&s->compare(HARROW)!=0)
         {

			 theMessage->WarningWithDisplay(*s+" has been read as TillageType");
				 theMessage->WarningWithDisplay(*s+" is change into HARROW");
        	theMessage->WarningWithDisplay("fieldOder::ReadParameters Fasset supports only PLOUGH and HARROW as TillageType");
			 s=new string("HARROW");
         }
         TillageType.InsertLast(s);
         double * a = new double(80.0);
         data->FindItem("TillageDepth",a,i);
         TillageDepth.InsertLast(a);
         double * minDepth=new double(0);

         data->FindItem("TillageMinDepth",minDepth,i);

         TillageMinDepth.InsertLast(minDepth);

         a = new double(0.95);
         data->FindItem("TillageIncorporation",a,i);
         TillageIncorporation.InsertLast(a);
      }



      data->FindVectorSize("SowCrop",&first,&size);
      for (int i=first;i<=(size);i++)
      {
         string * s=new string;
         data->FindItem("SowCrop",s,i);
         CheckDateStringForEmpty(s);
         SowCrops.InsertLast(s);
         s=new string;
         data->FindItem("SowDate",s,i);
         SowDates.InsertLast(s);
         double * a=new double(0.0);
         data->FindItem("SowAmount",a,i);
         SowAmounts.InsertLast(a);


         s=new string;
      	if (data->FindItem("SowStartDate",s,i))
               theMessage->FatalError("fieldOrder::ReadParameters - not using the MEASCOPE compiler option, you pillock");
         delete s;
      }

      data->FindVectorSize("FertilizerDate",&first,&size);
      for (int i=first;i<=(size);i++)
      {
         string * s=new string;
         data->FindItem("FertilizerDate",s,i);
         CheckDateStringForEmpty(s);
         FertilizerDates.InsertLast(s);
         string * type=new string;
         data->FindItem("FertilizerType",type,i);
         FertilizerTypes.InsertLast(type);
         double *incorpTime = new double(168.0); //time between application and incorporation in hours  (optional)
         data->FindItem("IncorporationTime",incorpTime,i);
         IncorporationTimes.InsertLast(incorpTime);
         if (*type=="")
            theMessage->FatalError("fieldOrder::ReadParameters - must specify type of fertilizer");

         if (*type=="MINERAL")
         {
            int found=0;
            nitrogen * aNitrogen = new nitrogen;

            string* MineralFormString = new string;
            *MineralFormString = "SOLID";
            data->FindItem("MineralForm",MineralFormString,i);
   			MineralForm.InsertLast(MineralFormString);
				double* NH4_fraction = new double(0.0);
				data->FindItem("NH4_fraction",NH4_fraction,i);
NH4_fractions.InsertLast(NH4_fraction);
            if (data->FindItem("FertilizerN",&aNitrogen->n,i))
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
            if (data->FindItem("FertilizerN15",&aNitrogen->n15,i))
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
            if (data->FindItem("FertilizerP",a,i))
               found++;
            *a=*a*factor;
            FertilizerP.InsertLast(a);
            a=new double(0.0);
            if (data->FindItem("FertilizerK",a,i))
               found++;
            *a=*a*factor;
            FertilizerK.InsertLast(a);
            a=new double(0.0);
            if (data->FindItem("FertilizerCl",a,i))             // Mainly used for leaching studies
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
               || (*type=="LIQUID-MANURE") || (*type=="SOLID-MANURE")||type->find("PIG-SLURRY-STORED")!=std::string::npos)
            {
               manure* aManure = new manure;
               aManure->Setname(*type);
theProducts->GiveProductInformation(aManure);
      			double amount = 0.0;
               double dm = 0.0;
               double organicC = 0.0;
               double volatisation = 0.0;
               data->FindItem("ManureAmount",&amount,i);
               data->FindItem("DryMatter",&dm,i);
               data->FindItem("OrganicCarbon",&organicC,i);                  // units amount pr amount fresh
               data->FindItem("Volatisation",&volatisation,i);

					if (amount>0)
               {
                  if (unitType==2)                                          // FarmN, minor screw-up
                     aManure->Setamount(amount*factor*0.1);
                  else
                     aManure->Setamount(amount*factor);
               }
               if (dm>0)
                  aManure->SetDryMatter(dm);
					if (organicC>0)
                  aManure->SetC_content(organicC);
				//	if (volatisation>0)
                 // aManure->Setvolatisation(volatisation);

               nitrogen am = aManure->GetNH4_content();
               data->FindItem("AmmoniumContent",&am.n,i);
               data->FindItem("AmmoniumN15Content",&am.n15,i);
               if (am.n>0)
                  aManure->SetNH4_content(am);
               nitrogen on = aManure->GetorgN_content();
               double orgN=0.0;
               if (!data->FindItem("OrganicNitrogen",&on.n,i))
               {
                  if (data->FindItem("OrganicN",&orgN,i))                        // FarmN project, minor screw-up
                  {
                     if (unitType==2)                                          // FarmN, minor screw-up
                     {
                        double a=aManure->GetAmount()*1000.0;
                        nitrogen Norg=aManure->GetorgN_content()*a;
                        nitrogen Nnh4=aManure->GetNH4_content()*a;
                        nitrogen Nno3=aManure->GetNO3_content()*a;
                        double f=100.0*orgN/(Norg.n+Nnh4.n+Nno3.n);
                        if (f>5.0)
                        {
                           cout << "f = " << f << endl;
                           theMessage->WarningWithDisplay("fieldOrder::ReadParameters - adjustment factor for N in animal manure very high");
                        }
                        if (f<0.2)
                        {
                           cout << "f = " << f << endl;
                           theMessage->WarningWithDisplay("fieldOrder::ReadParameters - adjustment factor for N in animal manure very low");
                        }
                        Norg=Norg*f/a;
                        Nnh4=Nnh4*f/a;
                        Nno3=Nno3*f/a;
                        if (Norg.n>1.0 || Nnh4.n>1.0 || Nno3.n>1.0)
                           theMessage->FatalError("fieldOrder::ReadParameters - adjustment of N in animal manure above possible range");
                        if ((Norg.n+Nnh4.n+Nno3.n)>0.2)
                           theMessage->WarningWithDisplay("fieldOrder::ReadParameters - adjustment of N in animal manure above plausible range");
                        aManure->SetorgN_content(Norg);
                        aManure->SetNH4_content(Nnh4);
                        aManure->SetNO3_content(Nno3);
                     }
                     else
                        theMessage->FatalError("fieldOrder::ReadParameters - 'OrganicN' can not be used in this mode");
                  }
               }
               data->FindItem("OrganicN15Nitrogen",&on.n15,i);
               if (on.n>0)
                  aManure->SetorgN_content(on);
               //Allow manure application to be defined as kg N/ha
               if (data->FindItem("FertilizerN",&amount,i))
               {
                  if (unitType!=0)
                     theMessage->FatalError("fieldOrder::ReadParameters - fertiliser unit type must be zero to use <FertiliserN> command");
                  aManure->SetmainUnitName(3);
                  aManure->Setamount(amount);
                  double* NH4_fraction = new double(0.0);
                  if (data->FindItem("NH4_fraction",NH4_fraction,i))
                  {
                     aManure->SetNH4_content(aManure->GetAllN().n*(*NH4_fraction));
                     aManure->SetorgN_content(aManure->GetAllN().n*(1-*NH4_fraction));
                  }
               }
              	double manureFW;
               if (aManure->GetmainUnitName()==3)
                  manureFW=amount/(aManure->GetAllN().n*1000.0);   //convert from kg N/ha to tonnes fresh manure/ha
               else
                  manureFW = aManure->GetAmount();
               if (manureFW>0.0)
               {
                  if (manureFW<5.0)
                  {
                     cout << endl << "Manure amount " << manureFW<< " t/ha, containing " << (1000.0*manureFW*aManure->GetTotalN().n) << " kg N/ha" << endl;
                     theMessage->WarningWithDisplay("fieldOrder::ReadParameters - manure amount below 5 tonnes per ha");
                  }
                  if (manureFW>100.0)
                  {
                     cout << endl << "Manure amount " << manureFW << " t/ha, containing " << (1000.0*manureFW*aManure->GetTotalN().n) << " kg N/ha" << endl;
                     theMessage->WarningWithDisplay("fieldOrder::ReadParameters - manure amount above 100 tonnes per ha");
                  }
   #ifdef TUPLE
                  if (area>0.001)
                     AddTuple(*s,aManure->GetTotalN().n*manureFW*area*1000.0,manureFW*area,"FERTILISATION","MANURE",0);
   #endif
                  Manure.InsertLast(aManure);
   				}
            }
            else
            {
               cout << "Fertilizer type: " << *type << endl;
               theMessage->FatalError("fieldOrder::ReadParameters - unknown fertilizer type");
            }
      }

      data->FindVectorSize("IrrigationDate",&first,&size);
      for (int i=first;i<=(size);i++)
      {
         string * s=new string;
         data->FindItem("IrrigationDate",s,i);
         CheckDateStringForEmpty(s);
         IrrigationDates.InsertLast(s);
         double * a=new double(0.0);
         data->FindItem("IrrigationAmount",a,i);
         if (*a==0.0)
            theMessage->FatalError("fieldOrder::ReadParameters - irrigation amount zero or unspecified");
         IrrigationAmounts.InsertLast(a);
         a=new double(0.0);
         data->FindItem("IrrigationNcontent",a,i);
         IrrigationNContents.InsertLast(a);
      }

      data->FindVectorSize("HarvestDate",&first,&size);
      for (int i=first;i<=(size);i++)
      {
         string * s=new string;
         data->FindItem("HarvestDate",s,i);
         CheckDateStringForEmpty(s);
         HarvestDates.InsertLast(s);
		 cout<<&s<<endl;
		 cout<<*s<<endl;
         bool removeRecorded=false;
#ifdef TUPLE
         double am,n;

         if (data->FindItem("GrainYield",&n,i))       // M�ngde N
         {
            am=0.0;
            data->FindItem("GrainYieldAmount",&am,i); // M�ngde vare

            if (area>0.001 && (n<0.0 || am>0.0))
               AddTuple(*s,n,am,"HARVEST","MAIN_PRODUCT",0);

         }
         if (data->FindItem("StrawYield",&n,i))       // M�ngde N
         {
            am=0.0;
            data->FindItem("StrawYieldAmount",&am,i); // M�ngde vare
            if (area>0.001 && (n<0.0 || am>0.0))
            {
               AddTuple(*s,n,am,"HARVEST","SECONDARY_PRODUCT",0);
               removeRecorded=true;
            }

         }
#endif
         bool HarvestRemoveStraws = true;                       // Default remove straw.

         data->FindItem("HarvestRemoveStraw",&HarvestRemoveStraws,i);
         bool *Remove = new bool(HarvestRemoveStraws!=0 || removeRecorded);
         HarvestRemoveStraw.InsertLast(Remove);


      	double * a = new double(0.0);
         data->FindItem("FractionStrawHarvested",a,i);
         FractionStrawHarvested.InsertLast(a);

         a = new double(0.0);
         data->FindItem("FractionGrainHarvested",a,i);
         FractionGrainHarvested.InsertLast(a);
      }

      data->FindVectorSize("CuttingDate",&first,&size);
      for (int i=first;i<=(size);i++)
      {
         string datas;
         data->FindItem("CuttingDate",&datas,i);

         string * s=new string(datas);
         CheckDateStringForEmpty(s);
         CuttingDates.InsertLast(s);
         bool CuttingRemoveStraw=false;
         data->FindItem("CuttingRemoveStraw",&CuttingRemoveStraw,i);
		 bool * remove=new bool(CuttingRemoveStraw);
         CuttingRemovesStraw.InsertLast(remove);;
         double * height=new double(0.07);

         data->FindItem("CuttingHeight",height,i);
         
         if (*height>1.0) //cutting height was input in cm
         {
	         theMessage->WarningWithDisplay("fieldOrder::ReadParameters - Cutting height converted from cm to m");
         	*height = *height/100;    //convert to meters
         }
         CuttingHeights.InsertLast(height);

         int *b = new int(1);		//default is forage harvesting
         data->FindItem("CuttingProcessProduct",b,i);
         CuttingProcessProducts.InsertLast(b);
#ifdef TUPLE
         double am,n;
         if (data->FindItem("ForageYield",&n,i))       // M�ngde N
         {
            am=0.0;
            data->FindItem("ForageYieldFeed",&am,i); // SFU

            if (area>0.001 && (n>0.0 || am>0.0))
               AddTuple(*s,n,0,"HARVEST","FORAGE/SILAGE",am);

         }
#endif
      }

      data->FindVectorSize("PesticideDate",&first,&size);
      for (int i=first;i<=(size);i++)
      {
         string * s=new string;
         data->FindItem("PesticideDate",s,i);
         CheckDateStringForEmpty(s);
         PesticidesDates.InsertLast(s);
         s = new string;
         data->FindItem("PesticideType",s,i);
         PesticidesTypes.InsertLast(s);
      }
#ifdef TUPLE
      data->FindVectorSize("GrazingDate",&first,&size);
      for (int i=first;i<=(size);i++)
      {
         string s;
         double am,n;
         data->FindItem("GrazingDate",&s,i);
         CheckDateStringForEmpty(&s);
         if (data->FindItem("GrazingYield",&n,i))       // M�ngde N
         {
            am=0.0;
            data->FindItem("GrazingYieldFeed",&am,i);   // M�ngde SFU

            if (area>0.001 && (n<0.0 || am>0.0))
               AddTuple(s,n,0,"GRAZING","GRAZED",am);

         }
      }
#endif

      data->FindVectorSize("GrazingStartDate",&first,&size);
      for (int i=first;i<=(size);i++)
      {
    	  data->setCritical(true);
         string * s = new string;
         data->FindItem("GrazingStartDate",s,i);
         CheckDateStringForEmpty(s);
         GrazingStartDates.InsertLast(s);
         s = new string;
         data->FindItem("GrazingEndDate",s,i);
         CheckDateStringForEmpty(s);
         GrazingEndDates.InsertLast(s);
         double * a=new double(0.0);
         data->FindItem("NitrogenLoad",a,i);
         if (*a<0.0)
            theMessage->FatalError("fieldOrder::ReadParameters - Nitrogen below zero or unspecified");
         NitrogenLoad.InsertLast(a);
         data->setCritical(false);
         double *Efficiency = new double(0.84);
         data->FindItem("GrazingEfficiency",Efficiency,i);
         GrazingEfficiency.InsertLast(Efficiency);
      }
   }
   else
   {
      fluid_hug_spring = 0.0;
   	data->FindItem("SLUR.PS",&fluid_hug_spring);
      fluid_hug_spring=fluid_hug_spring*factor;
     	bool solidHugUsed=false;
     	solidHugUsed=data->FindItem("DUNG.PS",&solid_hug);
      solid_hug=solid_hug*factor;
      double a=0.0;
      data->FindItem("LIQM.PS",&a);
      fluid_hug_spring+=a*factor;
      if (a>0)
         fluid_hug_is_slurry=0;
   	data->FindItem("SLUR.PA",&fluid_hug_fall);
      fluid_hug_fall=fluid_hug_fall*factor;
   	if(data->FindItem("DUNG.PA",&solid_hug))
		if(solidHugUsed==true)
			theMessage->FatalError("fieldOrder::ReadParameters: Cannot use both DUNG.PA and DUNG.PS");
      solid_hug=solid_hug*factor;
      a=0.0;
   	data->FindItem("LIQM.PA",&a);
    	fluid_hug_fall+=a*factor;
      if (a>0)
         fluid_hug_is_slurry=0;

      // Rigtige perioder til nedenst. For alle afledte af static_crop undtagen raps skal der ikke kunne gives gylle om efter�ret (warning). �rt m� ikke f� N-g�dn. overhovedet

   	if(data->FindItem("SLUR.P00",&a)==true)
   		theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
	if(data->FindItem("SLUR.P01",&a)==true)
		theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(data->FindItem("SLUR.P02",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(data->FindItem("SLUR.P03",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(data->FindItem("SLUR.P04",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(data->FindItem("SLUR.P05",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(data->FindItem("SLUR.P06",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(data->FindItem("SLUR.P07",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(data->FindItem("SLUR.P08",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(data->FindItem("SLUR.P09",&a)==true)
      theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(data->FindItem("SLUR.P10",&a)==true)
   		theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
   	if(data->FindItem("SLUR.P11",&a)==true)
   		theMessage->FatalError("fieldOrder::ReadParameters: this part of Fasset is not implemented");
      SetFluidManureSpring(fluid_hug_spring);
   	data->FindItem("kg-N-",&N);
      N=N*factor;
   	data->FindItem("kg-P-",&P);
      P=P*factor;
   	data->FindItem("kg-K-",&K);
      K=K*factor;

   }

 //  }
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


