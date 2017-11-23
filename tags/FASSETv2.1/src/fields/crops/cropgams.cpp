/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include <common.h>
#include <croprot.h>
#include <legislat.h>
#include <technics.h>

int cropRotation::TransformLevel(int i)
{
   int retVal=0;
   switch(i)
   {
      case 0 : retVal=4;break;
      case 1 : retVal=6;break;
      case 2 : retVal=9;
   }
   return retVal;
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurYield(int a,int b,string9 & s,fstream * f,int fieldno,int mode)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurYield(a,b,s,f,fieldno,mode);
		else
			if (IsCropLegal(fieldno,s))
         {
            int N=max_n_lev_multper;
         	int NN=min(N,NitrogenLevels(id));
				for (int k=0;k<NN;k++)
            {
					double EY=0;
               SetCropPointer(id);
					if (mode==0)
						EY=ExpectedYield(fieldno,s,TransformLevel(k),2);
					else
						EY=ExpectedStrawYield(fieldno,s,TransformLevel(k),2);
					if (EY>0)
               {
						*f << s;
						if (fieldno<10)
							*f << "0";
						*f << fieldno << "." << FertLevName;
						*f << k << "          " << EY << endl;
					}
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurYieldWithPV(int a,int b,string9 & s,fstream * f,int fieldno,int mode)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurYieldWithPV(a,b,s,f,fieldno,mode);
		else
			if (IsCropLegal(fieldno,s))
         {
         	int NN=NitrogenLevels(id);
            int PV=NumOfProtectStrat(id);
            for (int i=0;i<PV;i++)
            {
   				for (int k=0;k<NN;k++)
               {
   					double EY=0;
   					if (mode==0)
   						EY=ExpectedYield(fieldno,id,k,i);
   					else
   						EY=ExpectedStrawYield(fieldno,id,k,i);
   					if (EY>0)
                  {
   						*f << s;
   						if (fieldno<10)
   							*f << "0";
   						*f << fieldno << "." << PvName << i << "." << FertLevName;
   						*f << k << "          " << EY << endl;
                  }
					}
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
// mode=0 -> yield; mode=1 -> yield of straw
void cropRotation::WriteYieldCropsGams(char * filename,int firstSeason,int lastSeason,int mode)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
         if (lastSeason>1)
   			RecurYield(0,j,str,&fs,i,mode);
         else
   			RecurYieldWithPV(0,j,str,&fs,i,mode);
 	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurFertilizer(int a,int b,string9 & s,fstream * f,int fieldno,int mode,int N_levels)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurFertilizer(a,b,s,f,fieldno,mode,N_levels);
		else
         if (IsCropLegal(fieldno,s))
         {
            int nl=0;
            int NN=min(N_levels,NitrogenLevels(id));
				for (int k=0;k<NN;k++)
            {
               if (NitrogenLevels(id)!=N_levels)
                  nl=TransformLevel(k);
               else
                  nl=k;
					*f << s;
					if (fieldno<10)
						*f << "0";
					*f << fieldno << "." << FertLevName << k << "        ";
					if (mode==1)
						*f << NitrogenAmount(fieldno,id,nl,a);
					if (mode==2)
						*f << PhosphorAmount(id);
					if (mode==3)
						*f << PotassiumAmount(id);
					*f << endl;
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteFertilizerGams(char * filename,int firstSeason,int lastSeason,int mode)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
   int NL=max_n_lev;
   if (lastSeason>1)
      NL=max_n_lev_multper;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurFertilizer(0,j,str,&fs,i,mode,NL);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurNitrFix(int a,int b,string9 & s,fstream * f,int fieldno,int N_levels)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurNitrFix(a,b,s,f,fieldno,N_levels);
		else
         if (IsCropLegal(fieldno,s))
         {
            int nl=0;
            int NN=min(N_levels,NitrogenLevels(id));
				for (int k=0;k<NN;k++)
            {
               if (strcmp(id,"PE")==0)
               {
                  if (NitrogenLevels(id)!=N_levels)
                     nl=TransformLevel(k);
                  else
                     nl=k;
						*f << s;
						if (fieldno<10)
							*f << "0";
						*f << fieldno << "." << FertLevName << k << "        ";
	   				*f << ExpectedYield(fieldno,id,nl,2)*45.0 << endl; // Make more elegant later
               }
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteNitrFixGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
   int NL=max_n_lev;
   if (lastSeason>1)
      NL=max_n_lev_multper;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurNitrFix(0,j,str,&fs,i,NL);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteFieldSetToGams(char *filename)
{
	fstream fs;
	fs.open(filename,ios::out);
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		fs << FieldName << i << endl;
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WritePVSetToGams(char *filename)
{
	fstream fs;
	fs.open(filename,ios::out);
	for (int i=0;i<max_pv_strat;i++)
		fs << PvName << i << endl;
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurCropSet(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
   int NC;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurCropSet(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				*f << s;
				if (fieldno<10)
					*f << "0";
				*f << fieldno << endl;
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteCropSetToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurCropSet(0,j,str,&fs,i);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurStrawCropSet(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
   int NC;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurStrawCropSet(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s) && (ExpectedStrawYield(fieldno,id,1,1)>0))
         {
				*f << s;
				if (fieldno<10)
					*f << "0";
				*f << fieldno << endl;
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteStrawCropSetToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurStrawCropSet(0,j,str,&fs,i);
	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurFallowCropSet(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
   int NC;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurFallowCropSet(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s) && IsCropFallow(id))
         {
				*f << s;
				if (fieldno<10)
					*f << "0";
				*f << fieldno << endl;
			}
	}
}

/****************************************************************************\
Subset of C crop fallow
\****************************************************************************/
void cropRotation::WriteFallowCropSetToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
  			RecurFallowCropSet(0,j,str,&fs,i);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurGreenCropSet(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurGreenCropSet(a,b,s,f,fieldno);
		else
      {
         SetCropPointer(id);
			if (IsCropLegal(fieldno,s) && theLegislation->IsGreen(CropPointer->GetCropName()))
         {
				*f << s;
				if (fieldno<10)
					*f << "0";
				*f << fieldno << endl;
			}
      }
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteGreenCropSetToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurGreenCropSet(0,j,str,&fs,i);
	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurRotation(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
   int NC;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   NC=NumOfCrops();
	for (int i=0;i<NC;i++)
   {
      CropId(i,id);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurRotation(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				string9 st;
				char * c;
				for (int i=0;i<NC;i++)
            {
					CropId(i,id);
					strcpy(st,s);
					c=(char*)&st;
					for (int k=0;k<2*b;k++)
						c++;
					*c=id[0];
					c++;
					*c=id[1];
					if (IsCropLegal(fieldno,st) && (a==b))
               {
						*f << s;
						if (fieldno<10)
							*f << "0";
						*f << fieldno << "." << st;
						if (fieldno<10)
							*f << "0";
						*f << fieldno << "        1" << endl;
					}
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteRotationToGams(char * filename,int firstSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
   {
		strcpy(str,"______");
		RecurRotation(0,firstSeason,str,&fs,i);
	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurCCB(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
   int NC;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurCCB(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s) && (ExpectedStrawYield(fieldno,id,1,1)>0))
         {
				*f << s;
				if (fieldno<10)
					*f << "0";
				*f << fieldno << ".STRAW.T" << a << "        1" << endl;
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteCCB_ToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurCCB(0,j,str,&fs,i);
	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurCCT(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
      int j=GiveCropNo(id);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurCCT(a,b,s,f,fieldno);
		else
         if (IsCropLegal(fieldno,s))
         {
				*f << s;
				if (fieldno<10)
					*f << "0";
				*f << fieldno << "." << ProductName(j) << ".T" << a << "      1"
					<< endl;

			}
	}
}

/****************************************************************************\
\****************************************************************************/
// Version for (periods>1).
void cropRotation::WriteCCT_ToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurCCT(0,j,str,&fs,i);
	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
// Version for one period.
void cropRotation::WriteCCT_ToGams(char *filename)
{
	fstream fs;
	linkList<field>::PS P;
	fs.open(filename,ios::out);
	char * crop_id;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
   {
     	fieldList.SearchByNumber(P,i);
      int NC=P->element->NumOfFixedCrops();
   	for (int j=0;j<NC;j++)
      {
         crop_id=(char*)P->element->GiveFixedCrop(j);
			fs << crop_id << "____";
			if (i<10)
				fs << "0";
			fs << i << "." << ProductName(GiveCropNo(crop_id)) << "      1" << endl;
		}
	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
// Version for one period.
void cropRotation::WriteCCB_ToGams(char *filename)
{
	fstream fs;
	fs.open(filename,ios::out);
	char * crop_id;
	linkList<field>::PS P;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
   {
     	fieldList.SearchByNumber(P,i);
      int NC=P->element->NumOfFixedCrops();
   	for (int j=0;j<NC;j++)
      {
         crop_id=(char*)P->element->GiveFixedCrop(j);
			if (ExpectedStrawYield(i,crop_id,1,1)>0)
         {
				fs << crop_id << "____";
				if (i<10)
					fs << "0";
				fs << i << ".STRAW      1" << endl;
			}
		}
	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurPremium(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurPremium(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
           SetCropPointer(id);
				*f << s;
				if (fieldno<10)
					*f << "0";
				*f << fieldno;
            *f << "     " << theLegislation->GetContribution(CropPointer->GetCropName());
				*f << endl;
			}
	}
}

/****************************************************************************\
Hectarpremium
\****************************************************************************/
void cropRotation::WritePremiumsToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurPremium(0,j,str,&fs,i);
	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurReform(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurReform(a,b,s,f,fieldno);
		else
			// ------------- TO BE CHANGED ---------------------
			if (IsCropLegal(fieldno,s))
         {
				*f << s;
				if (fieldno<10)
					*f << "0";
				*f << fieldno;

				// TO BE CHANGED
				*f <<	"      1";

				*f << endl;
			}
	}
}

/****************************************************************************\
Reform crops included in fallowcalculation
\****************************************************************************/
void cropRotation::WriteReformToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurReform(0,j,str,&fs,i);
	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurManureIntervals(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurManureIntervals(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				int Intervals=5;
				for (int k=0;k<Intervals;k++)
            {
					*f << s;
					if (fieldno<10)
						*f << "0";
					*f << fieldno << ".F" << k << "      " << (120 + 30*k) << endl;
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteManureIntervalsToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurManureIntervals(0,j,str,&fs,i);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurLabCapUse(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurLabCapUse(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				int Periods=theTechnics->GetTechnicsPeriods()->NumOfPeriods();
				for (int k=0;k<Periods;k++)
            {
               period * per=theTechnics->GetTechnicsPeriods()->GetPeriod(k);
  					linkList<field>::PS P;
  					fieldList.SearchByNumber(P,fieldno);
  					int soil_type=P->element->JBno();
  					SetCropPointer(id);
               fieldOrder * FO = new fieldOrder("",0,NULL);
               FO->SetArea(P->element->FieldArea_());
               FO->SetStartYear(theTime.GetYear());
               FO->SetDistance(DistanceToField(fieldno));
               FO->SetPVStrat(2);
               FO->SetNLevel(max_n_lev);
  					cloneList<fieldOperationFields> * OP=CropPointer->MakeOpList(FO,soil_type,1.0);
               double hours=theTechnics->CalcLPFields(OP,hoursTotalNotCombineNotFert,per);
   			   delete OP;
               if (hours>0.0)
               {
						*f << s;
						if (fieldno<10)
							*f << "0";
						*f << fieldno << ".";
	               *f << per->GetName();
	               *f << "        ";
                  *f << hours << endl;
               }
				}
			}
	}
}

/****************************************************************************\
Labour capacity use hour per ha
\****************************************************************************/
void cropRotation::WriteLabCapUseToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurLabCapUse(0,j,str,&fs,i);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurCombCapUse(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
   int NC;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurCombCapUse(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
            int nl=0;
				int Periods=theTechnics->GetTechnicsPeriods()->NumOfPeriods();
				int NN=min((int) max_n_lev_multper,NitrogenLevels(id));
				for (int k=0;k<NN;k++)
            {
					for (int m=0;m<Periods;m++)
               {
                  nl=TransformLevel(k);
                  period * per=theTechnics->GetTechnicsPeriods()->GetPeriod(m);
  						linkList<field>::PS P;
  						fieldList.SearchByNumber(P,fieldno);
  						int soil_type=P->element->JBno();
  						SetCropPointer(id);
                  fieldOrder * FO = new fieldOrder("",0,NULL);
                  FO->SetArea(P->element->FieldArea_());
                  FO->SetStartYear(theTime.GetYear());
                  FO->SetDistance(DistanceToField(fieldno));
                  FO->SetPVStrat(2);
                  FO->SetNLevel(nl);
  						cloneList<fieldOperationFields> * OP=CropPointer->MakeOpList(FO,soil_type,1.0);
			   		double hours=theTechnics->CalcLPFields(OP,hoursCombine,per);
				   	delete OP;
                  if (hours>0)
                  {
							*f << s;
							if (fieldno<10)
								*f << "0";
							*f << fieldno << "." << FertLevName;
							*f << k << ".";
	                  *f << per->GetName();
	                  *f << "        ";
	                  *f << hours << endl;
                  }
					}
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurCombCapUseWithPV(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurCombCapUseWithPV(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				int Periods=theTechnics->GetTechnicsPeriods()->NumOfPeriods();
				int NN=NitrogenLevels(id);
            int PV=NumOfProtectStrat(id);
            for (int i=0;i<PV;i++)
            {
   				for (int k=0;k<NN;k++)
               {
	   				for (int m=0;m<Periods;m++)
                  {
                     period * per=theTechnics->GetTechnicsPeriods()->GetPeriod(m);
  			   			linkList<field>::PS P;
     						fieldList.SearchByNumber(P,fieldno);
     						int soil_type=P->element->JBno();
  	   					SetCropPointer(id);
                     fieldOrder * FO = new fieldOrder("",0,NULL);
                     FO->SetArea(P->element->FieldArea_());
                     FO->SetStartYear(theTime.GetYear());
                     FO->SetDistance(DistanceToField(fieldno));
                     FO->SetPVStrat(i);
                     FO->SetNLevel(k);
  						   cloneList<fieldOperationFields> * OP=CropPointer->MakeOpList(FO,soil_type,1.0);
  				   		double hours=theTechnics->CalcLPFields(OP,hoursCombine,per);
   				   	delete OP;
                     if (hours>0)
                     {
			   				*f << s;
				   			if (fieldno<10)
					   			*f << "0";
						   	*f << fieldno << "." << PvName << i << "." << FertLevName;
							   *f << k << ".";
	                     *f << per->GetName();
	                     *f << "        ";
	   						*f << hours << endl;
                     }
                  }
					}
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteCombCapUseGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
         if (lastSeason>1)
   			RecurCombCapUse(0,j,str,&fs,i);
         else
   			RecurCombCapUseWithPV(0,j,str,&fs,i);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurSprayCapUse(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurSprayCapUse(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				int Periods=theTechnics->GetTechnicsPeriods()->NumOfPeriods();
				int NP=NumOfProtectStrat(id);
				for (int l=0;l<NP;l++)
            {
      			for (int m=0;m<Periods;m++)
               {
                  period * per=theTechnics->GetTechnicsPeriods()->GetPeriod(m);
           			linkList<field>::PS P;
		   			fieldList.SearchByNumber(P,fieldno);
		   			int soil_type=P->element->JBno();
		   			SetCropPointer(id);
                  fieldOrder * FO = new fieldOrder("",0,NULL);
                  FO->SetArea(P->element->FieldArea_());
                  FO->SetStartYear(theTime.GetYear());
                  FO->SetDistance(DistanceToField(fieldno));
                  FO->SetPVStrat(l);
                  FO->SetNLevel(max_n_lev);
					   cloneList<fieldOperationFields> * OP=CropPointer->MakeOpList(FO,soil_type,1.0);
		   			double hours=theTechnics->CalcLPFields(OP,hoursPlantCare,per);
		   			delete OP;
                  if (hours>0)
                  {
							*f << s;
							if (fieldno<10)
								*f << "0";
							*f << fieldno << "." << PvName << l << ".";
	                  *f << per->GetName();
	                  *f << "        ";
	           			*f << hours << endl;
                  }
					}
				}
			}
	}
}

/****************************************************************************\
hours per ha
\****************************************************************************/
void cropRotation::WriteSprayCapUseGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurSprayCapUse(0,j,str,&fs,i);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurCostParameters(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
   int NC;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurCostParameters(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
            int nl=0;
				int NN=min((int) max_n_lev_multper,NitrogenLevels(id));
				for (int k=0;k<NN;k++)
            {
               nl=TransformLevel(k);
  					linkList<field>::PS P;
  					fieldList.SearchByNumber(P,fieldno);
  					int soil_type=P->element->JBno();
  					SetCropPointer(id);
               period * per=new period("",1,1,12,31);
               fieldOrder * FO = new fieldOrder("",0,NULL);
               FO->SetArea(P->element->FieldArea_());
               FO->SetStartYear(theTime.GetYear());
               FO->SetDistance(DistanceToField(fieldno));
               FO->SetPVStrat(2);
               FO->SetNLevel(nl);
  					cloneList<fieldOperationFields> * OP=CropPointer->MakeOpList(FO,soil_type,1.0);
//            	double cost=theTechnics->CalcLPFields(OP,varCostTotalNotFert,per);
               delete per;
  					delete OP;
#ifdef __ANSICPP__
  				cout << "error in cropRotation::RecurCostParameters " << endl;
  				cout << "press any key ";
  				char dum;
  				cin >> dum;
#else
/*  				if (abs(cost)>0.0)
  				{
						*f << s;
						if (fieldno<10)
							*f << "0";
						*f << fieldno << "." << FertLevName;
						*f << k << "        ";
						*f << cost << endl;
               }*/
#endif
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurCostParametersWithPV(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurCostParametersWithPV(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				int NN=NitrogenLevels(id);
            int PV=NumOfProtectStrat(id);
            for (int j=0;j<PV;j++)
            {
   				for (int k=0;k<NN;k++)
               {
  	   				linkList<field>::PS P;
  		   			fieldList.SearchByNumber(P,fieldno);
  			   		int soil_type=P->element->JBno();
  				   	SetCropPointer(id);
                  period * per=new period("",1,1,12,31);
                  fieldOrder * FO = new fieldOrder("",0,NULL);
                  FO->SetArea(P->element->GiveFixedSize(i));
                  FO->SetStartYear(theTime.GetYear());
                  FO->SetDistance(DistanceToField(fieldno));
                  FO->SetPVStrat(j);
                  FO->SetNLevel(k);
  						cloneList<fieldOperationFields> * OP=CropPointer->MakeOpList(FO,soil_type,1.0);
  		   			//double cost=theTechnics->CalcLPFields(OP,varCostTotalNotFert,per);
                  delete per;
  			   		delete OP;
#ifdef __ANSICPP__
  				cout << "error in cropRotation::RecurCostParameters " << endl;
  				cout << "press any key ";
  				char dum;
  				cin >> dum;
#else
/*                  if (abs(cost)>0)
                  {
		   				*f << s;
			   			if (fieldno<10)
				   			*f << "0";
					   	*f << fieldno << "." << PvName << j << "." << FertLevName;
	   					*f << k << "        ";
		   				*f << cost << endl;
                  }*/
#endif
                  }
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteCostParametersGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
         if (lastSeason>1)
   			RecurCostParameters(0,j,str,&fs,i);
         else
   			RecurCostParametersWithPV(0,j,str,&fs,i);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurAllowed(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
   int NC;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurAllowed(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				*f << s;
				if (fieldno<10)
					*f << "0";
				*f << fieldno << endl;
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteAllowedToGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
   {
		fs << FieldName << i << "." << endl << "(" << endl;
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurAllowed(0,j,str,&fs,i);
		}
		fs << ")" << endl;
	}
   fs.close();
}

/****************************************************************************\
Maximal set of fertilizer levels
\****************************************************************************/
void cropRotation::WriteNlevSetToGams(char *filename,int seasons)
{
	fstream fs;
	fs.open(filename,ios::out);
   int NN=max_n_lev;
   if (seasons>1)
      NN=max_n_lev_multper;
	for (int i=0;i<NN;i++)
   {
		fs << FertLevName << (i);
		fs << endl;
	}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WriteAreasToGams(char *filename)
{
	fstream fs;
	fs.open(filename,ios::out);
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		fs << FieldName << (i) << "    " << FieldArea(i) << endl;
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurPCT1(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurPCT1(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				int Periods=theTechnics->GetTechnicsPeriods()->NumOfPeriods();
				int Intervals=5;
				for (int k=0;k<Periods;k++)
            {
					for (int m=0;m<Intervals;m++)
               {
						*f << s;
						if (fieldno<10)
							*f << "0";
						*f << fieldno << ".";
                  *f << theTechnics->GetTechnicsPeriods()->GetPeriod(k)->GetName();
                  *f << ".H" << m;
						*f << "        " << (0.8-0.1*m) << endl;
					}
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WritePCT1NGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurPCT1(0,j,str,&fs,i);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurPCT1NPK(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurPCT1NPK(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				int Periods=theTechnics->GetTechnicsPeriods()->NumOfPeriods();
				int NPK=3;
				for (int k=0;k<Periods;k++)
            {
					for (int m=1;m<=NPK;m++)
               {
						*f << s;
						if (fieldno<10)
							*f << "0";
                  *f << fieldno;
                  if (m==1)
                     *f << ".N";
                  if (m==2)
                     *f << ".P";
                  if (m==3)
                     *f << ".K";
						*f << ".";
                  *f << theTechnics->GetTechnicsPeriods()->GetPeriod(k)->GetName();
                  if (m==1)
                     *f << "        0.55" << endl; // Expand !!!
                  else
   						*f << "        0.85" << endl;
					}
				}
			}
	}
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::WritePCT1NPKGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurPCT1NPK(0,j,str,&fs,i);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurNRHS2(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   int NC;
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurNRHS2(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
				int levels=5;
				for (int m=0;m<levels;m++)
            {
					*f << s;
					if (fieldno<10)
						*f << "0";
					*f << fieldno << ".H" << m;
					*f << "        " << (4.0/(4.0+m)) << endl;
				}
			}
	}
}

/****************************************************************************\
Utilize of N from pigs manure in diff. periods
\****************************************************************************/
void cropRotation::WriteNRHS2Gams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurNRHS2(0,j,str,&fs,i);
		}
   fs.close();
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::RecurPCTNPK(int a,int b,string9 & s,fstream * f,int fieldno)
{
	a++;
	char * id;
   int NC;
	linkList<field>::PS P;
  	fieldList.SearchByNumber(P,fieldno);
   if (b>1)
      NC=NumOfCrops();
   else
      NC=P->element->NumOfFixedCrops();
	for (int i=0;i<NC;i++)
   {
      if (b>1)
         CropId(i,id);
      else
         id=(char*)P->element->GiveFixedCrop(i);
		s[(a-1)*2]=*id;
		id++;
		s[(a-1)*2+1]=*id;
		id--;
		if (a<b)
			RecurPCTNPK(a,b,s,f,fieldno);
		else
			if (IsCropLegal(fieldno,s))
         {
		      SetCropPointer(id);
				int NPK=3;
				int Periods=thePeriods.NumOfPeriods();
				for (int l=0;l<NPK;l++)
            {
					for (int m=0;m<Periods;m++)
               {
						*f << s;
						if (fieldno<10)
							*f << "0";
						*f << fieldno << ".";
						if (l==0)
							*f << "N.";
						if (l==1)
							*f << "P.";
						if (l==2)
							*f << "K.";
                  *f << thePeriods.GetPeriod(m)->GetName() << "        ";
                  if (l>0)
							*f << "0.85";
                  else
                  {
                     if (strcmp(thePeriods.GetPeriod(m)->GetName(),"PS")==0)
	                     *f << CropPointer->NUseSlurrySpring();
                     else
	                     *f << CropPointer->NUseSlurryAutumn();
                  }
                  *f << endl;
					}
				}
			}
	}
}

/****************************************************************************\
Utilize of NPK from pigs manure in diff. periods
\****************************************************************************/
void cropRotation::WritePCTNPKGams(char * filename,int firstSeason,int lastSeason)
{
	fstream fs;
	fs.open(filename,ios::out);
	string9 str;
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
		for (int j=firstSeason;j<=lastSeason;j++)
      {
			strcpy(str,"______");
			RecurPCTNPK(0,j,str,&fs,i);
		}
   fs.close();
}

void cropRotation::WritePXPToGams(char * filename)
{
   int numFieldPeriods=thePeriods.NumOfPeriods();
   int numTechPeriods=theTechnics->GetTechnicsPeriods()->NumOfPeriods();
	fstream fs;
	fs.open(filename,ios::out);
   for (int i=0;i<numFieldPeriods;i++)
   {
      period * fieldPeriod=thePeriods.GetPeriod(i);
      for (int j=0;j<numTechPeriods;j++)
      {
         period * techPeriod=theTechnics->GetTechnicsPeriods()->GetPeriod(j);
         if (fieldPeriod->FullyContained(*techPeriod))
            fs << techPeriod->GetName() << "."
               << fieldPeriod->GetName() << "      1" << endl;
      }
   }
   fs.close();
}
/****************************************************************************\
\****************************************************************************/

void cropRotation::WriteFieldPeriodsToGams(char * filename)
{
   int numFieldPeriods=thePeriods.NumOfPeriods();
   fstream fs;
	fs.open(filename,ios::out);
   for (int i=0;i<numFieldPeriods;i++)
      fs << thePeriods.GetPeriod(i)->GetName() << endl;
   fs.close();
}
/****************************************************************************\
\****************************************************************************/

void cropRotation::writeMacpphaToGams(char* filename)
{
	fstream fs;
	fs.open(filename,ios::out);
   staticCrop * CropPointer;
   linkList <string> *cropList;

   period * techPeriod;
   char * id;
   string* cropString;
   bool found;
   int NC=NumOfCrops();
   int numPeriods=theTechnics->GetTechnicsPeriods()->NumOfPeriods();
   string machinery[] = {"SPRAYER","SLURRYSP","DUNGSP","LIQMSP"};
   for (int k=0;k<4; k++)
   {
   	cropList = new linkList <string>;
		for (int i=0;i<NC;i++)
      {
	   	CropPointer=CropArray[i].CP;
	      CropId(i,id);
         cropString = new string;
         *cropString = (string) ProductName(GiveCropNo(id));
         found = false;
         for (int j=0; j<cropList->NumOfNodes(); j++)
      		if (cropString->compare(*cropList->ElementAtNumber(j))==0) found = true;
         if (!found)
         {
            cropList->InsertLast(cropString);
		      for (int j=0;j<numPeriods;j++)
		      {
            	techPeriod=theTechnics->GetTechnicsPeriods()->GetPeriod(j);
		         fs << machinery[k] << ".";
			      fs << *cropString << ".";
		         fs << techPeriod->GetName() << "   ";
		         if (k>0)												// involving organic manure
               	if (CropPointer->SlyrryPossibleThisPeriod(techPeriod))
						   fs << "0.027" << endl;
                  else
                  	fs << "10000" << endl;              // equals infinity
		         else
		         	fs << "0.027" << endl;

		      }
         }
         else
         	delete cropString;
	   }
      delete cropList;
   }
   fs.close();
}
/****************************************************************************\
\****************************************************************************/
void cropRotation::SetFixedCrops(char * filename)
{
	const int strlen=255;
	typedef char strtyp[strlen];
	linkList<field>::PS P;
	strtyp s;
   string9 crop_id;
   double size;
	fstream * f = new fstream;;
	f->open(filename,ios::in);
	int NF=NumOfFields();
	for (int i=0;i<NF;i++)
   {
     	fieldList.SearchByNumber(P,i);
  		P->element->ResetFixedCrops();
   }
   while (*f)
   {
      *f >> crop_id >> size;
      if (strcmp(crop_id,"")!=0)
      {
         strcpy(s,crop_id);
			for (int i=0;i<6;i++)
				s[i]=48;
      	fieldList.SearchByNumber(P,atoi(s));
         strcpy(s,crop_id);
         s[2]=0;
	      if (P!=NULL)
      		P->element->AddFixedCrop(CropArray[GiveCropNo(s)].CrpNme,size);
         else
            theMessage->WarningWithDisplay("cropRotation::SetFixedCrops - Illegal field number");
      }
   }
   f->close();
   delete f;
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::SetFixedCropsAll()
{
	char * id;
	linkList<field>::PS P;
	int NF=NumOfFields();
  	int NC=NumOfCrops();
	for (int i=0;i<NF;i++)
   {
     	fieldList.SearchByNumber(P,i);
  		P->element->ResetFixedCrops();
   	for (int j=0;j<NC;j++)
      {
   		CropId(j,id);
			if (IsCropLegal(i,id))
         {
            if (P!=NULL)
        	   	P->element->AddFixedCrop(id,P->element->FieldArea_());
            else
               theMessage->WarningWithDisplay("cropRotation::SetFixedCrops - Illegal field number");
         }
      }
   }
}

/****************************************************************************\
\****************************************************************************/
void cropRotation::CalcLP(int seasons)
{
  	WriteFieldSetToGams("i.inc");Dot();
   WritePXPToGams("pxp_pct.inc");Dot();
   if (seasons==1)
   {
      SetFixedCrops("x_1.inc");
 		WriteFallowCropSetToGams("cfllw_1.inc",1,seasons);Dot();
		WriteGreenCropSetToGams("cgr_1.inc",1,seasons);Dot();
   	WriteNlevSetToGams("f_1.inc",seasons);Dot();
   	WritePVSetToGams("q_1.inc");
		WriteCCT_ToGams("cct_1.inc");Dot();
   	WriteCCB_ToGams("ccb_1.inc");Dot();
   	WriteYieldCropsGams("yc_1.inc",1,seasons,0);Dot(); // YC(C,Q,F) Yield crops in tonnes per hectar
   	WriteYieldCropsGams("yc2_1.inc",1,seasons,1);Dot(); // YC2(C,Q,F) Yield straw in tonnes per hectar
   	WriteFertilizerGams("napp_1.inc",1,seasons,1);Dot(); // NAPP(C,F) Nitrogen amount kg per ha
   	WriteFertilizerGams("papp_1.inc",1,seasons,2);Dot(); // PAPP(C,F) Phosphorus amount kg per ha
   	WriteFertilizerGams("kapp_1.inc",1,seasons,3);Dot(); // KAPP(C,F) Potassium amount kg per ha
   	WriteNitrFixGams("nfix_1.inc",1,seasons);Dot();
      // WritePCT1NPKGams("pct_npk.inc",1,seasons);Dot(); // PCT_PK(C,CONTENT,P) Utilize of N,P and K from manure in diff. periods
		WritePCTNPKGams("pct_npk.inc",1,seasons);Dot(); // Used by both !!!
   	WritePremiumsToGams("premi_1.inc",1,seasons);Dot();
   	WriteReformToGams("refrm_1.inc",1,seasons);Dot();
   	WriteCostParametersGams("vcc_1.inc",1,seasons);Dot(); // VCC(C,Q,F) Variable costs for field activities
   	WriteLabCapUseToGams("lcap_1.inc",1,seasons);Dot();
   	WriteCombCapUseGams("mccomb_1.inc",1,seasons);Dot(); // MCCOMBI(C,Q,F,P) Combine capacity use hours per ha
   	WriteSprayCapUseGams("mcspray.inc",1,seasons);Dot();
//    "H_1.INC" - WAIT
//    "REDUK.INC" - WAIT
//    WritePCT1NGams("pct_n.inc",1,seasons);Dot(); // PCT_N(C,P,H) Utilize of N from manure in diff. periods
//   	WriteManureIntervalsToGams("nrhs.inc",1,seasons);Dot();
//   	WriteNRHS2Gams("nrhs2.inc",1,seasons);Dot();
//	   WriteStrawCropSetToGams("cstraw.inc","CSTRAW(C)",1,seasons);Dot();
   }
	if (seasons==3)
   {
      SetFixedCropsAll();
		WriteCropSetToGams("c2_3.inc",2,2);Dot(); // C2 (C) Subset of C crop 2.nd. year
		WriteFallowCropSetToGams("cfllw2_3.inc",2,2);Dot(); // CFALLOW2(C2) Subset of C2 crop fallow 2.nd. year
		WriteGreenCropSetToGams("cgr2_3.inc",2,2);Dot(); // CGREEN2(C2) Subset of C2 crop green
		WriteRotationToGams("crot12_3.inc",1);Dot(); // CROT12(C1,C2)
		WriteCCT_ToGams("cct_3.inc",1,seasons);Dot();
		WriteCropSetToGams("c3_3.inc",3,3);Dot(); // C3 (C) Subset of C crop 3.rd. year
		WriteFallowCropSetToGams("cfllw3_3.inc",3,3);Dot(); // CFALLOW3(C3) Subset of C3 crop fallow 3.rd. year
		WriteGreenCropSetToGams("cgr3_3.inc",3,3);Dot(); // CGREEN3(C3) Subset of C3 crop green
		WriteRotationToGams("crot23_3.inc",2);Dot(); // CROT23(C2,C3)
   	WriteNlevSetToGams("f_3.inc",seasons);Dot();
	   WriteCropSetToGams("c_3.inc",1,seasons);Dot(); // C crops
   	WriteCropSetToGams("c1_3.inc",1,1);Dot(); // C1 (C) Subset of C crop 1.st. year
   	WriteFallowCropSetToGams("cfllw_3.inc",1,seasons);Dot();
   	WriteFallowCropSetToGams("cfllw1_3.inc",1,1);Dot(); // CFALLOW1(C1) Subset of C1 crop fallow 1.st. year
   	WriteGreenCropSetToGams("cgr1_3.inc",1,1);Dot(); // CGREEN1(C1) Subset of C1 crop green
   	WriteAllowedToGams("ixc_3.inc",1,seasons);Dot();
   	WriteAreasToGams("arrhs_3.inc");Dot();
   	WriteYieldCropsGams("yc_3.inc",1,seasons,0);Dot(); // YC(C,F) Yield crops in tonnes per hectar
   	WriteYieldCropsGams("yc2_3.inc",1,seasons,1);Dot(); // YC2(C,F) Yield straw in tonnes per hectar
   	WritePremiumsToGams("premi_3.inc",1,seasons);Dot();
   	WriteCCB_ToGams("ccb_3.inc",1,seasons);Dot();
   	WriteReformToGams("refrm_3.inc",1,seasons);Dot();
   	WriteFertilizerGams("napp_3.inc",1,seasons,1);Dot(); // NAPP(C,F) Nitrogen amount kg per ha
   	WriteFertilizerGams("papp_3.inc",1,seasons,2);Dot(); // PAPP(C,F) Phosphorus amount kg per ha
   	WriteFertilizerGams("kapp_3.inc",1,seasons,3);Dot(); // KAPP(C,F) Potassium amount kg per ha
   	WriteNitrFixGams("nfix_3.inc",1,seasons);Dot();
   	WritePCTNPKGams("pct1_3.inc",1,seasons);Dot(); // Used by both !!!
   	WriteCombCapUseGams("mccomb_3.inc",1,seasons);Dot(); // MCCOMBI(C,F,P) Combine capacity use hours per ha
    	WriteLabCapUseToGams("lcap_3.inc",1,seasons);Dot();
   	WriteCostParametersGams("vcc_3.inc",1,seasons);Dot(); // VCC(C,F) Variable costs for field activities
      WriteFieldPeriodsToGams("p_pct.inc"); Dot();
      writeMacpphaToGams("mcappha.inc"); Dot();
	}
}

