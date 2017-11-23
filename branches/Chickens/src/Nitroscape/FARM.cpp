/*PALM_UNIT  -name fasset\
           -functions {c++ fasset}\
           -object_files {fasset.o libFarm.a}\
           -parallel mpi\
           -minproc 1\
           -maxproc 16
*/

/*PALM_SPACE  -name intall\
            -shape (nb_eco)\
            -element_size PL_INTEGER 
*/

/*PALM_SPACE  -name fl3\
            -shape (3)\
            -element_size PL_REAL 
*/

/*PALM_SPACE  -name fl6\
            -shape (6)\
            -element_size PL_REAL
*/

/*PALM_SPACE  -name fl2x4\
            -shape (2,4)\
            -element_size PL_REAL 
*/

/*PALM_SPACE  -name fl12all\
            -shape (nb_eco,12)\
            -element_size PL_REAL 
*/

/*PALM_SPACE  -name FarmToLinkerS\
            -shape (1)\
            -element_size PL_AUTO_SIZE\
            -items { {itk intall} {itkdet fl12all} {em fl3} } 

*/

/*PALM_SPACE  -name LinkerToFarmS\
            -shape (nb_eco)\
            -element_size PL_AUTO_SIZE\
            -items { {swc fl6} {prod fl2x4} }
*/

/*PALM_OBJECT  -name LinkerToFarm\
             -space LinkerToFarmS\
             -localisation REPLICATED_ON_ALL_PROCS\
             -intent IN
*/

/*PALM_OBJECT  -name FarmToLinker\
             -space FarmToLinkerS\
             -tag ON\
             -localisation REPLICATED_ON_ALL_PROCS\
             -intent OUT
*/
#include <stdio.h>
#include <stdlib.h>


//FASSET includes
#include "../data/systemData.h"
#include "../data/outputData.h"
#include "../base/common.h"
#include "../base/bstime.h"
#include "../products/products.h"
#include "../base/message.h"
#include "../legislation/legislation.h"
#include "../base/climate.h"

#include "../base/timeuse.h"
#include "../base/controlParameters.h"
#include "../technics/technics.h"
#include "../tools/fileAccess.h"
#include "../data/farmData.h"
#include "../data/environData.h"
#include "../data/outputData.h"
#include "../data/commonData.h"
#include "../data/pixiData.h"
#include "../base/linklist.h"
#include "../Nitroscape/MMSClass.h"
#include "../Nitroscape/ecosystinterface.h"
#include "../data/pixiData.h"

typedef char string100[100];
#ifdef NITROSCAPE
bsTime theTime;
products * theProducts;
message * theMessage;
timeUse* timeConsumption;
IndicatorOutput * theOutput;
legislate * theLegislation;
technics * theTechnics;
buildings * theBuildings;
climate * theClimate;
controlParameters * theControlParameters;
manureManager *  theManure_manager;
outputData * theOutputData;
systemData * theSystemData;
farmData * theFarmData;
environData * theEnvironData;
string100
      InputDirectory,
      OutputDirectory,
      ClimateDirectory,
      ClimateFileName,
      baseOutputDir;

pixiData *thePixiData;

//NJH number of farms - could be replaced by Palm variable
int numOfFarms;
string orgPath;
//FASSET definitions
void SetEcoStatus(MMSClass *theMMS, patchInterfaceClass * apatchInterfaceClass , double ReadyForHarvestOrLater, double IrrigationDemand, float prod[4][2]);
#ifndef PALM
void GetEcoStatus(ecosystinterfaceClass * anecosystinterfaceClass,float prod[4][2]);
void CheckTillage(MMSClass *aMMS,patchInterfaceClass * thePatchInterface,ecosystinterfaceClass * anecosystinterfaceClass);
void CheckSowing(MMSClass *aMMS,patchInterfaceClass * thePatchInterface, ecosystinterfaceClass * anecosystinterfaceClass);
void CheckAddManure(MMSClass *aMMS,patchInterfaceClass * thePatchInterface, ecosystinterfaceClass * anecosystinterfaceClass);
void CheckFertilisation(MMSClass *aMMS,patchInterfaceClass * thePatchInterface, ecosystinterfaceClass * anecosystinterfaceClass);
void CheckHarvest(MMSClass *aMMS,patchInterfaceClass * thePatchInterface, ecosystinterfaceClass * anecosystinterfaceClass);
void CheckGrazed(MMSClass *aMMS,patchInterfaceClass *  apatchInterfaceClass,  ecosystinterfaceClass * anecosystinterfaceClass);
//end FASSET definitions

#endif PALM
ecosystinterfaceClass * findEcoModel(int ecoNr,linkList<ecosystinterfaceClass> * theEcosystems)
{
	 ecosystinterfaceClass * anecosystinterfaceClass=NULL;
	 for(int e=0;e<theEcosystems->NumOfNodes();e++)
	  {
		  if(theEcosystems->ElementAtNumber(e)->GetIndex()==ecoNr)
		  {
			  anecosystinterfaceClass=theEcosystems->ElementAtNumber(e);
			  break;
		  }
	  }
	  if(!anecosystinterfaceClass)
	  {
		  cout<<"extern \"C\" int main():ecosystem interface not found"<<endl;
		  exit(99);
	  }
	  return anecosystinterfaceClass;
}
extern "C" int main(){
//NJH call to this class constructor leads to input data being read from system.dat (must be in current working directory

	theSystemData =new systemData(true);
	orgPath=theSystemData->getOrgPath();
	cout << orgPath << endl;
//NJH read number of farms - could be replaced by Palm variable
   numOfFarms=theSystemData->getRunLevel()+1;

/*	char cl_object[15];
	char cl_space[16];
	char cl_item_itk[4]="itk";
	char cl_item_itkdet[7]="itkdet";
	char cl_item_em[3]="em";
	char cl_item_swc[4]="swc";
	char cl_item_prod[5]="prod";
	char buf[32];
	int itk[nb_eco];
	float itkdet[nb_eco][12];
	double itkdetdb[12];
*/

  	string inputDirectory;
	string outputDirectory;
	string climateDirectory;
	string climateFileName;
	string scenarioFileName;
	string baseOutputDir;
/*	int il_err, il_size, il_pos, i, e, rank, day;
	int p,a,b,c,d,f;
	int nb_field_f;
	int nb_eco_f;
	int cor_table_field_eco_pixel[nb_eco][2];
	float em[3];
	float swc[6];
*/
	float prod[4][2];
   /*
   prod[0][i] is product code
   prod[1][i] is amount of dry matter (DM) (grams per metre square)
   prod[2][i] is the percentage N in DM
   prod[3][i] is the percentage C in DM

   */
/*	void *buffer;
	int il_time,il_tag;
	int index;
	int size;
*/
	/*
	PALM_Write(PL_OUT,"#############   FARM    #############");
*/
	/*writting in log the number of eco_pixel*/
/*	sprintf(buf,"nb pixel eco %d ", nb_eco);
	PALM_Write(PL_OUT,buf);*/
/* writing in log, the palm_rank*/
/*        il_err=MPI_Comm_rank(PL_COMM_EXEC, &rank);
	sprintf(buf,"RANK INSTANCE eco = %d ",rank);
	PALM_Write(PL_OUT,buf);
	index=rank+1;*/
/* opening the file pixel.dist*/
/*       	FILE *dist;
	dist=fopen("./pixel.dist","r");
	if(dist==NULL){
		PALM_Write(PL_OUT,"problem \n");
	}*/
/* reading the two heading lines*/
/*	fgets(buf,32,dist);
	fgets(buf,32,dist);*/
/* reading for each pixel, the type it is*/
//	for(p=0;p<nb_pixel;p++){
//		fscanf(dist," %5d %5d %5d ",&a, &b, &c);
//		PALM_Write(PL_OUT," pixel: %d type: %d index: c ",a,b,c);
//	}
/* reading the two heading lines*/
//	fgets(buf,32,dist);
//	fgets(buf,32,dist);
///*reading for each farm the number of field*/
//	for(f=0;f<nb_farm;f++){
//		fscanf(dist," %5d %5d ",&a, &b);
//		PALM_Write(PL_OUT," farm %d : number of field: %d ",a,b);
//		if(a==index){
//			nb_field_f=b;
//		}
//	}
//
///* reading the first line*/
//	fgets(buf,32,dist);
//	PALM_Write(PL_OUT,buf);
///*reading the second line*/
//	fgets(buf,32,dist);
//	PALM_Write(PL_OUT,buf);
//	nb_eco_f=0;
//	for(e=0;e<nb_eco;e++){
//		fscanf(dist," %5d %5d %5d ",&a, &b, &c);
//		cor_table_field_eco_pixel[a][0]=b;
//		cor_table_field_eco_pixel[a][1]=c;
//		PALM_Write(PL_OUT," eco pixel %d belong to the field %d of the farm %d ",a,cor_table_field_eco_pixel[a][1],cor_table_field_eco_pixel[a][0]);
//		if(b==index){
//			nb_eco_f=nb_eco_f+1;
//		}
//	}
//
//  	sprintf(cl_item_itk,"itk");
//   	sprintf(cl_item_itkdet,"itkdet");
//   	sprintf(cl_item_em,"em");
//	sprintf(cl_item_swc,"swc");
//	sprintf(cl_item_prod,"prod");
//
//// create an instance of the MMS);
//
	thePixiData = new pixiData();
   linkList<MMSClass> *theFarms;
   theFarms = new linkList<MMSClass>;
 //following two lines are only needed if simulating ecosystem here
#ifndef PALM
	linkList<ecosystinterfaceClass> * theEcosystems;
   theEcosystems = new linkList<ecosystinterfaceClass>;

#endif PALM


   int startFarm = theSystemData->getJumpStart();  //first farm to simulate
   int stopFarm = theSystemData->getJumpStop();    //last farm to simulate
   for (int farmNo=0;farmNo<numOfFarms;farmNo++)
   {
      if ((farmNo>=startFarm)&&(farmNo<=stopFarm))  //if farm is to be simulated
      {
         MMSClass *aMMS = new MMSClass(farmNo);

         theFarms->InsertLast(aMMS);          //add to linklist
   //
   ////initialise MMS, giving names of directories as char *
   //	PALM_Write(PL_OUT," creation of MMS ");
   //
         inputDirectory = theSystemData->getInputDirectory();
         climateDirectory = theSystemData->getClimateDirectory();
         climateFileName = theSystemData->getClimateFileName();
         outputDirectory = theSystemData->getOutputDirectory();
         theSystemData->reload(orgPath);
         //initialise MMS
         theSystemData->setCurrentRun(farmNo);

         aMMS->initialise_MMS(theSystemData->getSystemRunData(farmNo));

   //	PALM_Write(PL_OUT," ok ");

   //MMS needs to know the initial status of the ECO
   // assume no crops present

   #ifndef PALM
         eventControl * localEventControl =aMMS->getEeventControl();
         linkList <field>* fieldsList=localEventControl->getCropRotation()->GetFields();
         for (int i=0;i<fieldsList->NumOfNodes(); i++)
         {
        	 cloneList<patch> * patchList=fieldsList->ElementAtNumber(i)->getPatchList();
             for(int j=0;j<patchList->NumOfNodes(); j++)
            {


				ecosystinterfaceClass * anecosystinterfaceClass = new ecosystinterfaceClass(patchList->ElementAtNumber(j)->getEcoModel(), patchList->ElementAtNumber(j)->getSoilFileName(), 1.0);
				anecosystinterfaceClass->OutputPlantDetails(true);  //comment out if no plant details needed
				anecosystinterfaceClass->UpdateStatus();
				theEcosystems->InsertLast(anecosystinterfaceClass);
				//get product details from ecosystem
				GetEcoStatus(anecosystinterfaceClass,prod);
				//send to MMS
				SetEcoStatus(aMMS, patchList->ElementAtNumber(j)->getPatchInterface(), 0.0, 0.0, prod);

			  }
         }
   //end of section that only works for one farm
   #endif PALM
   	}
   }

	delete theSystemData;

//
//	for(e=0;e<nb_eco;e++){
//		SetEcoStatus(theMMS,e, 1.0,0.0, prod);
//	}
//
	int day=0;
   numOfFarms=stopFarm - startFarm + 1;
#ifndef PALM
   bool finished = false;
 //  bsTime * ShouldStopTime = new bsTime(1, 8, mySystemData->getStartYear());
   while (!finished)       //MMSClass reads the start and end year from system.dat
   {

   	day++;
#else
 	for (day=0;day<365;day++)
   {
#endif
// 		PALM_Write(PL_OUT,"#################################");
// 		PALM_Write(PL_OUT,"              Day: %d",day);
// 		PALM_Write(PL_OUT,"#################################");
//
//		PALM_Write(PL_OUT,"********  IN   **************");
//
// 		sprintf(cl_space,"LinkerToFarmS");
//		sprintf(cl_object,"LinkerToFarm");
///* allocation in memory*/
//// 		size=PALM_Space_get_size(cl_space);
////  		buffer=malloc(size);
//
//
///*c getting input fromLinker*/
// 		il_tag=PL_NO_TAG;
//		il_time=PL_NO_TIME;
//
///*		il_err=PALM_Get(cl_space,cl_object, &il_time,&il_tag, buffer);*/
///*Unpacking swc  in LinkerToFarm*/
//
//		for(e=0;e<nb_eco;e++){
//			il_pos=e;
// 			PALM_Write(PL_OUT,"eco pixel: %d\n",il_pos);
//			il_err=PALM_Unpack(buffer,cl_space,cl_item_swc,&il_pos,swc);
// 			/********************
//			 soil water content
//			********************/
// 			PALM_Write(PL_OUT,"soil water content %f, %f, %f, %f, %f, %f \n",swc[0],swc[1],swc[2],swc[3],swc[4],swc[5]);
//
//			 /*c Unpacking prod in LinkerToFarm*/
//// 	       		il_err=PALM_Unpack(buffer,cl_space,cl_item_prod,&il_pos,prod);
//
//			/************************
//			prod see FARM_IO
//			************************/
//			for(i=0;i<2;i++){
//				PALM_Write(PL_OUT,"production : type: %f, amount: %f, %%n: %f, %%c %f",prod[0][i],prod[1][i],prod[2][i],prod[3][i]);
// 			}
//
//// tells farm model the status of each ECO. Two dummy parameters currently used for ReadyForHarvestOrLater and IrrigationDemand
//// SetEcoStatus is defined in this file because preprocessing of information is required before calling MMSClass function

//delete if using ecosystem interface
/*
 		prod[0][0]=0;
 		prod[0][1]=1;
 		prod[1][0]=prod[1][1]=10;
 		prod[2][0]=prod[2][1]=prod[3][0]=prod[3][1]=20;
*/
   for (int count=0;count<theFarms->NumOfNodes();count++)
   {
		MMSClass *aMMS = theFarms->ElementAtNumber(count);
//
////The manure management and livestock module can now perform its daily function
//

      cout << "Farm no " << aMMS->GetfarmNo() << " day " << day << " date " << theTime << endl;
      aMMS->DoMMSDaily();

#ifndef PALM
    	  if(day==366)
    		  cout<<"d"<<endl;
		finished = aMMS->GetfinishedRun();
#endif notPALM
#ifndef PALM
      eventControl * localEventControl =aMMS->getEeventControl();
      linkList <field>* fieldsList=localEventControl->getCropRotation()->GetFields();
      for (int i=0;i<fieldsList->NumOfNodes(); i++)
      {
    	  cloneList<patch> * patchList=fieldsList->ElementAtNumber(i)->getPatchList();
    	  for(int j=0;j<patchList->NumOfNodes(); j++)
    	  {
    		  int ecoNum=patchList->ElementAtNumber(j)->getEcoModel();

    		  ecosystinterfaceClass * anecosystinterfaceClass  = findEcoModel(ecoNum, theEcosystems);
			 CheckTillage(aMMS,patchList->ElementAtNumber(j)->getPatchInterface(), anecosystinterfaceClass);
			 CheckSowing(aMMS,patchList->ElementAtNumber(j)->getPatchInterface(), anecosystinterfaceClass);
			 CheckAddManure(aMMS,patchList->ElementAtNumber(j)->getPatchInterface(), anecosystinterfaceClass);
			 CheckFertilisation(aMMS,patchList->ElementAtNumber(j)->getPatchInterface(), anecosystinterfaceClass);
			 CheckHarvest(aMMS,patchList->ElementAtNumber(j)->getPatchInterface(), anecosystinterfaceClass);
			 CheckGrazed(aMMS,patchList->ElementAtNumber(j)->getPatchInterface(), anecosystinterfaceClass);
			 anecosystinterfaceClass->DoDailyEco();
			 anecosystinterfaceClass->OutputPlantDetails(false);
			 //get product details from ecosystem
			 GetEcoStatus(anecosystinterfaceClass,prod);

/*         for (int houseNo=0; houseNo<aMMS->GetNumberOfHouses();houseNo++)
         {
         	cout <<"House " << houseNo << " house NH3 " << aMMS->GetMMSHouseAmmonia() << " house N2O " << aMMS->GetMMSHouseN2O()
            	<< " house NO " << aMMS->GetMMSHouseNO() << endl;
         }
         for (int storeNo=0; storeNo<aMMS->GetNumberOfStores();storeNo++)
         {
         	cout << "Store " << storeNo << " store NH3 " << aMMS->GetMMSStoreN2O(storeNo) << " store N2O " << aMMS->GetMMSStoreN2O(storeNo)
            	<< " store NO " << aMMS->GetMMSStoreNO(storeNo) << endl;
         }
*/
#endif PALM
         //send to MMS
         SetEcoStatus(aMMS, patchList->ElementAtNumber(j)->getPatchInterface(), 0.0, 0.0, prod);
         ecoNum++;
			}

      }
//end of section that only works for one farm

	}


//		free(buffer);
//
//		PALM_Write(PL_OUT,"********  OUT   **************");
//
///* 		##################################################################
//        		               OUT
// 		###################################################################*/
//	     	sprintf(cl_space,"FarmToLinkerS");
//	     	sprintf(cl_object,"FarmToLinker");
//	     	il_pos=1;
//
///*  allocation in memory*/
//
//// 		size=PALM_Space_get_size(cl_space);
////  		buffer=malloc(size);
//
///*		buffer=malloc(PALM_Space_get_size("FarmToLinkerS"));*/
//
//		for(e=0;e<nb_eco_f;e++){
//			il_pos=0;
//			il_tag=rank;
// 			PALM_Write(PL_OUT,"eco: %d",e);
//
//			/*******************
//			  itk see FARM_IO
//			*******************/
// 			itk[e]=rank*e;
// 			PALM_Write(PL_OUT,"itk: %d",itk[e]);
//			/*******************
//  			itkdet see FARM_IO
//			*******************/
//			for (i=0;i<12;i++){
//				itkdetdb[i]=e*rank;
//			}
//
////previous three lines sets itkdet to dummy values
//
//         		itk[e]=0;  //initialise to no operation
//
////Find out from the manure management and livestock module what operations (if any) are to be performed on the ecosystem module
//         //check if a crop was sown
//
//
//         		if (theMMS->GetMMSDoSowing(e, &itkdetdb[0], &itkdetdb[2], &itkdetdb[3])){
//	            		itk[e]=1;
//			}
//         //check if fertiliser was applied
//		         double theNH4N;
//			 double theNO3N;
//         		if (theMMS->GetMMSDoFertilisation(e, &theNH4N, &theNO3N))
//         		{
//         			itk[e]= 2;
//         			itkdetdb[0]= theNH4N + theNO3N;
//         			itkdetdb[1]= 3.0;  //treats everything as ammonium nitrate at the moment
//            			itkdetdb[2]= 1.0;
//            			itkdetdb[3]= 100.0;
//            			itkdetdb[4]= 100.0 * theNH4N/(theNH4N + theNO3N);
//            			itkdetdb[5]= 100.0 * theNO3N/(theNH4N + theNO3N);
//            			itkdetdb[6]= 100.0;
//            			itkdetdb[7]= 0.0;
//         		}
//	         //check if the manure was applied
//        		double theDepth;
//			double theTotalN;
//         		double thepercentDM;
//			double theC;
//			if (theMMS->GetMMSAddOrganic(e, &itkdetdb[1], &itkdetdb[0], &theDepth, &theTotalN, &theNH4N, &theNO3N, &thepercentDM, &theC))
//         		{
//         			double amountInkg = itkdetdb[0]*1000.0;  //itkdetdb[0] is in tonnes
//         			itk[e]= 2;
//            			if (theDepth>0.0){
//            				itkdetdb[2]= 2.0;}
//            			else{
//            				itkdetdb[2]= 1.0;
//					itkdetdb[3]= 100.0 * theTotalN/amountInkg;
//            				itkdetdb[4]= 100.0 * theNH4N/amountInkg;
//            				itkdetdb[5]= 100.0 * theNO3N/amountInkg;
//            				itkdetdb[6]= thepercentDM;
//            				itkdetdb[7]= 100.0 * theC/amountInkg;
//				}
//
//			}
//         // check if harvest should be performed
//         		double propGrainHarvested; //not currently in data sent to ECO. Needs to be added
//         		double propStrawHarvested; //not currently in data sent to ECO. Needs to be added
//         		if (theMMS->GetMMSDoHarvesting(e, &itkdetdb[0], &propGrainHarvested, &propStrawHarvested))
//         		{
//         			itk[e]= 3;
//         		}
//
//         // need to insert code to interrogate irrigation
//
//         //check if tillage should be performed
//		        double theType;  //not used at present, may be necessary for some ecosystem models
//
//         		double theFractionIncorporated; //not used at present. May only be relevant for FASSET ecosystem model
//
//			if (theMMS->GetMMSTillage(e, &theType, &theDepth, &theFractionIncorporated))
//         		{
//         			itk[e]= 5;
//            			itkdetdb[0]= theDepth;
//         		}
//
//         //Get ammonia emission from field applied manure and fertiliser
//         //this is currently aggregated for all patches (and housing and storage - see below)
//			em[0]+=theMMS->GetMMSPatchAmmonia(e);
//
//
//// End of interrogation of ECO related variables from manure management and livestock module
//
//			if (itk[e]==0){
// 				PALM_Write(PL_OUT,"no management operation");
//	      		}
//
//			if (itk[e]==1){
// 				PALM_Write(PL_OUT,"sowing information: crop sown: %f, density, %f , depth: %f, %%C: %f, %%N: %f  ",itkdetdb[0], itkdetdb[1],itkdetdb[2], itkdetdb[3],itkdetdb[4]);
//			}
//
//			if (itk[e]==2){
// 				PALM_Write(PL_OUT,"fertilisation: amount: %f, type: %f, application method: %f, tot%%N: %f, Nh4_N%%: %f, NO3_N%% %f, %%DM: %f, %%C: %f",itkdetdb[0],itkdetdb[1],itkdetdb[2],itkdetdb[3],itkdetdb[4],itkdetdb[5],itkdetdb[6],itkdetdb[7]);
//			}
//			if (itk[e]==3){
// 				PALM_Write(PL_OUT,"harvesting : method: %f",itkdetdb[0]);
//			}
//			if(itk[e]==4){
// 				PALM_Write(PL_OUT, "irrigation : water in: %f, method: %f, %%N in water,%f ",itkdetdb[0],itkdetdb[1],itkdetdb[2]);
//			}
//		   	if(itk[e]==5){
// 				PALM_Write(PL_OUT,"tillage : depth: %f, incorportaion of residues: %f",itkdetdb[0],itkdetdb[1]);
//			}
//			if(itk[e]==6){
// 				PALM_Write(PL_OUT,"grazing : grazed grass: %f, amount of dung deposited: %f, tot %% N in dung: %f, Nh4_N%% in dung: %f, NO3_N%% in dung: %f, %%DM in dung: %f, %%C in dung: %f, amount of urine deposited: %f, tot%%N in urine: %f, Nh4_N%% in urine: %f, NO3_N%% in urine: %f, %%C in urine: %f",itkdetdb[0],itkdetdb[1],itkdetdb[2],itkdetdb[3],itkdetdb[4],itkdetdb[5],itkdetdb[6],itkdetdb[7],itkdetdb[8],itkdetdb[9]);
//			}
//
//		}
//
//		for(int i=0;i<12;i++){
//			itkdet[e][i]=itkdetdb[i];
//		}
//		il_err=PALM_Pack(buffer,cl_space,cl_item_itk,&il_pos,itk);
//		il_err=PALM_Pack(buffer,cl_space,cl_item_itkdet,&il_pos,itkdet);
//
//		/**********************
// 			NH3 emissions
//		**********************/
//
//		for (int i=0; i<theMMS->GetNumberOfHouses(); i++)
//         		em[0]+=theMMS->GetMMSHouseAmmonia(i);
//		for (int i=0; i<theMMS->GetNumberOfStore(); i++)
//			em[0]+=theMMS->GetMMSStoreAmmonia(i);
//
//		/**********************
//			NO2 emissions
//		**********************/
//	       	em[1]=24*rank;
//		/**********************
//		  NOx emissions
//		**********************/
//		em[2]=25*rank;
//		PALM_Write(PL_OUT,"emissions : NH3: %f, NO2: %f, NOx: %f ", em[0],em[1],em[2]);
///* packing emissions in FarmToLinker*/
//
//// 		il_err=PALM_Pack(buffer,cl_space,cl_item_em,&il_pos,em);
//
//		il_time=PL_NO_TIME;
//
///* 		il_err=PALM_Put(cl_space,cl_object,&il_time,&rank,buffer);
//
//		free(buffer);*/
	}
   //tidy up memory
#ifndef PALM

   for (int count=0;count<theFarms->NumOfNodes();count++)
   {
		MMSClass *aMMS = theFarms->ElementAtNumber(count);
		eventControl * localEventControl =aMMS->getEeventControl();
		linkList <field>* localFieldList=localEventControl->getCropRotation()->GetFields();
      for (int i=0;i<localFieldList->NumOfNodes(); i++)
      {

    	  cloneList<patch> * patchList=localFieldList->ElementAtNumber(i)->getPatchList();

		for(int j=0;j<patchList->NumOfNodes(); j++)
		{
			int ecoNum=patchList->ElementAtNumber(j)->getEcoModel();

			 ecosystinterfaceClass * anecosystinterfaceClass  = findEcoModel(ecoNum, theEcosystems);

			anecosystinterfaceClass->ClosePlantDetails();
		}
      }
   }
   delete theEcosystems;

#endif PALM
   delete theFarms;
   	cout<<"\n succesfully finish"<<endl;

 	return 0;
}

void SetEcoStatus(MMSClass *theMMS, patchInterfaceClass * apatchInterfaceClass , double ReadyForHarvestOrLater,
      double IrrigationDemand, float prod[4][2])

{
	linkList <harvInfoClass> *theHarvestProducts = new linkList <harvInfoClass>;
   for (int i=0; i<2; i++)
   {

   	harvInfoClass *aharvInfoClass = new harvInfoClass();
      aharvInfoClass->SetproductCode(int (prod[0][i]));
      aharvInfoClass->SetamountDM(prod[1][i]); //kg per ha
      aharvInfoClass->SetamountN(prod[1][i]*prod[2][i]/100.0);
      aharvInfoClass->SetamountC(prod[1][i]*prod[3][i]/100.0);
      theHarvestProducts->InsertLast(aharvInfoClass);
   }
   apatchInterfaceClass->SetEcoStatus(ReadyForHarvestOrLater,IrrigationDemand, theHarvestProducts);
}

#ifndef PALM

//Get status variables from ecosystem
void GetEcoStatus(ecosystinterfaceClass * anecosystinterfaceClass,float prod[4][2])
{
   linkList <harvInfoClass> *theHarvestProducts = anecosystinterfaceClass->GetharvestStuff();
   for (int harv=0; harv<2; harv++)
   {
      harvInfoClass *aharvInfoClass = theHarvestProducts->ElementAtNumber(harv);
      prod[0][harv] = aharvInfoClass->GetproductCode();
      prod[1][harv]= aharvInfoClass->GetamountDM() * 10.0; //convert from grams per metre square to kg per ha
      if (prod[1][harv]>0)
      {
         prod[2][harv] = 100.0 * aharvInfoClass->GetamountN()/aharvInfoClass->GetamountDM();
         prod[3][harv] = 100.0 * aharvInfoClass->GetamountC()/aharvInfoClass->GetamountDM();
      }
      else
      {
         prod[2][harv] = 0.0;
         prod[3][harv] = 0.0;
      }
   }
}

void CheckTillage(MMSClass *aMMS, patchInterfaceClass *  apatchInterfaceClass, ecosystinterfaceClass * anecosystinterfaceClass)
{
	double theType, theDepth, theFractionIncorporated;
	theType=apatchInterfaceClass->GettheType()+1.0;
	theDepth=apatchInterfaceClass->GettheDepth();
	theFractionIncorporated = apatchInterfaceClass->GettheFractionIncorporated();
	apatchInterfaceClass->SettheType(-1);
	if (theType!=0)
		anecosystinterfaceClass->Tillage((theType-1.0), theDepth, theFractionIncorporated);
}

void CheckSowing(MMSClass *aMMS,patchInterfaceClass *  apatchInterfaceClass, ecosystinterfaceClass * anecosystinterfaceClass)
{
   double  thecropID, amountSeedDM, seedN;
	thecropID = (double) apatchInterfaceClass->GetthecropID();
	amountSeedDM=apatchInterfaceClass->GetamountSeedDM();
	seedN = apatchInterfaceClass->GetseedN();
   apatchInterfaceClass->SetamountSeedDM(0.0);
	if (amountSeedDM>0.0)
		anecosystinterfaceClass->Sow(thecropID, amountSeedDM, seedN);
}
void CheckAddManure(MMSClass *aMMS,patchInterfaceClass *  apatchInterfaceClass, ecosystinterfaceClass * anecosystinterfaceClass)
{
	double theManureType=0, theManureamount=0,theManureDepth=0,theManureTotalN=0,theManureNH4N=0, theManureNO3N=0,
         theManurepercentDM=0, theManureC=0;
		if (apatchInterfaceClass->GetdecompAmount()>0.0)
	   {
			theManureamount = apatchInterfaceClass->GetdecompAmount();
			theManureDepth=0.0; //apatchInterfaceClass->GettheDepth();
			theManureNH4N = apatchInterfaceClass->GetdecompNH4_N();
			theManureNO3N = apatchInterfaceClass->GetdecompNO3_N();
			theManureTotalN = theManureNH4N + theManureNO3N + apatchInterfaceClass->GetdecompOrg_N();
			theManureC = apatchInterfaceClass->GetdecompC();
			theManurepercentDM = apatchInterfaceClass->GetdecompPercentDM();
			theManureType = apatchInterfaceClass->GetdecompType();
	      apatchInterfaceClass->SetdecompAmount(0.0);
	   }
   if (theManureamount>0.0)
      anecosystinterfaceClass->AddOrganic(theManureType, theManureamount,theManureDepth,theManureTotalN,theManureNH4N, theManureNO3N,
         theManurepercentDM, theManureC);
}

void CheckFertilisation(MMSClass *aMMS,patchInterfaceClass *  apatchInterfaceClass, ecosystinterfaceClass * anecosystinterfaceClass)
{
	double theFertAmmonium, theFertNitrate;
	theFertAmmonium = apatchInterfaceClass->GettheNitrate();
	theFertNitrate = apatchInterfaceClass->GettheAmmonium();
   if ((theFertAmmonium>0.0)||(theFertNitrate>0.0))
      anecosystinterfaceClass->SetFertilisation(theFertAmmonium,theFertNitrate);
}

void CheckHarvest(MMSClass *aMMS,patchInterfaceClass *  apatchInterfaceClass,  ecosystinterfaceClass * anecosystinterfaceClass)
{
	double harvestCode, propGrainHarvested, propStrawHarvested;
	harvestCode = (double) apatchInterfaceClass->GetharvestCode();
	propGrainHarvested = apatchInterfaceClass->GetpropGrainHarvested();
	propStrawHarvested = apatchInterfaceClass->GetpropStrawHarvested();
   apatchInterfaceClass->SetharvestCode(0);
   if (harvestCode>0)
      anecosystinterfaceClass->HarvestOperations(harvestCode, propGrainHarvested, propStrawHarvested);
}

void CheckGrazed(MMSClass *aMMS,patchInterfaceClass *  apatchInterfaceClass,  ecosystinterfaceClass * anecosystinterfaceClass)
{
	double grazedDM=0.0;
	grazedDM = (double) apatchInterfaceClass->GetdryMatterGrazed();
    anecosystinterfaceClass->Grazing(grazedDM);
}

#endif PALM

#endif
