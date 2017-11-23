/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#include "../base/common.h"
#include "harvestTech.h"
#include "../products/energy.h"

/****************************************************************************\
  Constructor with arguments
\****************************************************************************/
harvestTech::harvestTech(const char * aName, const int aIndex, const base * aOwner)
   : fieldOperationTech(aName, aIndex, aOwner)
{
   loadClampCapacity = 0.0;
   reloadingCapacity = 0.0;
   unloadCapacity = 0.0;
   unloadRoothouseCapacity = 0.0;
   depositCapacity = 0.0;
   depositRoothouseCapacity = 0.0;
   strawBalerCapacity = 0.0;
   prepUnloadingStorage = 0.0;
   prepUnloadingRoothouse = 0.0;
   prepReloading = 0.0;
   prepReloadingHarvester = 0.0;
   prepClamp = 0.0;
   uncoverClamp = 0.0;
   stops = 0;
   roadSpeed = 0.0;
   balesLost = 0.0;
   balesLoad = 0.0;
   bindTime = 0.0;
   operationID = 0;
   baleType = -1;
   loadCapacity = 0.0;
	reloading = 0.0;
	moweCarriage = 0.0;
   for (int i=0;i<MaxPlants;i++)
   	MainProduct[i] = StrawProduct[i] = ForageProduct[i] = NULL;
}

/****************************************************************************\
  Destructor
\****************************************************************************/
harvestTech::~harvestTech()
{
}


/****************************************************************************\
\****************************************************************************/
void harvestTech::ReadParameters(fstream * file)
{
   fieldOperationTech::ReadParameters(file);
   SetCritical();
   Setfile(file);
   GetParameter("roadSpeed",&roadSpeed);
   UnsetCritical();
   GetParameter("unloadCapacity",&unloadCapacity);
   GetParameter("prepUnloadingStorage",&prepUnloadingStorage);
   GetParameter("prepReloadingHarvester",&prepReloadingHarvester);
   GetParameter("prepReloading",&prepReloading);
	GetParameter("moweCarriage",&moweCarriage);
	GetParameter("reloading",&reloading);
   GetParameter("strawBalerCapacity",&strawBalerCapacity);        // new parameters added by JB
   GetParameter("loadCapacity",&loadCapacity);                    // new parameters added by JB
   GetParameter("baleType",&baleType);
   GetParameter("bindTime",&bindTime);
   GetParameter("balesLoad",&balesLoad);
   GetParameter("balesLost",&balesLost);
   GetParameter("stops",&stops);
   GetParameter("uncoverClamp",&uncoverClamp);
   GetParameter("prepClamp",&prepClamp);
   GetParameter("prepUnloadingRoothouse",&prepUnloadingRoothouse);
   GetParameter("depositCapacity",&depositCapacity);
   GetParameter("loadClampCapacity",&loadClampCapacity);
   GetParameter("reloadingCapacity",&reloadingCapacity);
   GetParameter("unloadRoothouseCapacity",&unloadRoothouseCapacity);
   GetParameter("depositRoothouseCapacity",&depositRoothouseCapacity);
   Setfile(NULL);
}



/****************************************************************************\
\****************************************************************************/
void harvestTech::CalcGangHours()
{
   techEquip aMainTechEquip("maintechequip",0,this);
   double    size              = 0.0;
   double    carryCapacity     = 0.0;
   double    numberOfCarriages = 0.0;
   double    calcAmount        = 0.0;
   double    netCapacityWheat  = 0.0;

   aMainTechEquip =*(aGang->GetMainTechEquip());
   size           = aMainTechEquip.Getsize();
   for (int i=0;i<MaxPlants;i++)
   {
   	if ((operationID==GatheringBales)||(operationID==ForageHarvesting))
      {
      	if (StrawProduct[i])
            calcAmount += StrawProduct[i]->GetAmount();
      	if (ForageProduct[i])
            calcAmount += ForageProduct[i]->GetAmount();
      }
      else
   		if (MainProduct[i])
	   		calcAmount += MainProduct[i]->GetAmount();
   }

   implement aImplement("implement",0,this);
   selfPropelled aSelfPropelled("selfPropelled",0,this);

   linkList<techEquip> * aTechEquipList;
   linkList<techEquip>::PS aTechEquipNode;
   aTechEquipList=aGang->GetTechEquipList();
   aTechEquipList->PeekHead(aTechEquipNode);
   while(aTechEquipNode!=NULL)
   {
      if(strcmp(aTechEquipNode->element->GetName(),"selfPropelled")==0)
      {
         aSelfPropelled = *(selfPropelled *)(aTechEquipNode->element);
         netCapacityWheat = aSelfPropelled.GetNetCapacityWheat();
      }
      if((strcmp(aTechEquipNode->element->GetName(),"implement")==0) ||
      	(strcmp(aTechEquipNode->element->GetName(),"forageCutter")==0))
      {
         aImplement= *(implement *)(aTechEquipNode->element);
         carryCapacity+= aImplement.GetCarryCapacity();
         if(aImplement.GettechEquipName()=="Carriage")
            numberOfCarriages++;
      }
      aTechEquipNode=aTechEquipNode->next;
   }

   double A=0.0, A1=0.0, D1=0.0, D2=0.0, B=0.0, B1=0.0, B3=0.0, L=0.0;
   double L1=0.0, E=0.0, R=0.0, P1=0.0, P2=0.0, I=0.0, I1=0.0, I2=0.0;
   double H=0.0, O=0.0, T=0.0, P=0.0, M=0.0, V=0.0, V1=0.0, S=0.0;

   if(size==0.0)
      theMessage->FatalError("harvestTech::CalcGangHours - size = 0 in operation ",
                            (char*)operationName.c_str());

   if(operationID==CombineHarvesting || operationID==CombineHarvestingShred)
   	if(netCapacityWheat==0.0)
	      theMessage->FatalError("harvestTech::CalcGangHours - netCapacityWheat = 0 in operation ",
                            (char*)operationName.c_str());
   if(area==0.0)
      theMessage->FatalError("harvestTech::CalcGangHours - area = 0 in operation ",
                            (char*)operationName.c_str());
   if(carryCapacity==0.0)
      theMessage->FatalError("harvestTech::CalcGangHours - carryCapacity = 0 in operation ",
                            (char*)operationName.c_str());
   if(workingSpeed==0.0)
      theMessage->FatalError("harvestTech::CalcGangHours - fieldSpeed = 0 in operation ",
                            (char*)operationName.c_str());
   if(roadSpeed==0.0)
      theMessage->FatalError("harvestTech::CalcGangHours - roadSpeed = 0 in operation ",
                            (char*)operationName.c_str());
   if(balesLoad==0.0 && operationID==GatheringBales)
      theMessage->FatalError("harvestTech::CalcGangHours - balesLoad = 0 in operation ",
                            (char*)operationName.c_str());

   if(operationID==Mowing || operationID==CutSetAside || ForageHarvesting)
      A = (
          (area*600.0)/(workingSpeed*size) +
          (turningTime*sqrt(area*10000.0/2.0))/size +
          turningHeadland +
          (disturbanceAllowance*area)
          )/area;

	if(operationID==CombineHarvesting || operationID==CombineHarvestingShred)
   {
      double cropFactor = 1.0;
      if (MainProduct[0]==NULL)
         theMessage->FatalError("harvestTech::CalcGangHours - MainProduct[0] should not be NULL here");
      char * tmp=new char[100];
      strcpy(tmp,"PEAS");
      if (MainProduct[0]->GetName()==tmp)
         cropFactor=0.888;
      strcpy(tmp,"WNRPE");
      if (MainProduct[0]->GetName()==tmp)
         cropFactor=0.454;
      strcpy(tmp,"RYE");
      if (MainProduct[0]->GetName()==tmp)
         cropFactor=0.548;
      delete [] tmp;
      if (calcAmount<=0)
    		theMessage->WarningWithDisplay("harvestTech::CalcGangHours - amount harvested/gathered should be greater than zero in operation ",
                                (char*)operationName.c_str());
      A = (
          (area*calcAmount*60.0)*cropFactor/netCapacityWheat +
          (max(0.0,turningTime*(sqrt(area*10000.0/2.0)-(8.0*size)))/size) + /* Max function added, otherwise this will fall below zero at very low areas - BMP Dec. 2006 */
          turningHeadland +
          (disturbanceAllowance*area)
          )/area;
   }

   if(operationID==GatheringBales)
   {
      if (baleType==-1)
    		theMessage->FatalError("harvestTech::CalcGangHours - baleType not initialized");
      if (strawBalerCapacity<1E-10)
         theMessage->FatalError("harvestTech::CalcGangHours - strawBalerCapacity extremly low or zero");
      A = (
          (area*calcAmount*60.0)/strawBalerCapacity +
          (turningTime*sqrt(area*10000.0/2.0))/size +
           turningHeadland +
           (disturbanceAllowance*area)
           )/area;
      if (baleType==1)
      {
         if (bindTime<1E-10)
            theMessage->FatalError("harvestTech::CalcGangHours - bindTime extremely low or zero");
         if (balesLoad<1E-10)
            theMessage->FatalError("harvestTech::CalcGangHours - balesLoad extremely low or zero");
      	A1 = ((calcAmount*1000.0)/balesLoad)*bindTime;
  			if(A1<=0.0)
      		theMessage->WarningWithDisplay("harvestTech::CalcGangHours - A1 should be greater than zero in operation ",
                                (char*)operationName.c_str());
      }
	}
   if(A<=0.0)
      theMessage->FatalError("harvestTech::CalcGangHours - A should be greater than zero in operation ",
                            (char*)operationName.c_str());

   /* Preparation for reloading */
   switch(operationID)
   {
      case CombineHarvesting:
      case CombineHarvestingShred:
         if (prepReloading==0.0)
            theMessage->FatalError("harvestTech::CalcGangHours - prepReloading = 0");
         if (prepReloadingHarvester==0.0)
            theMessage->FatalError("harvestTech::CalcGangHours - prepReloadingHarvester = 0");
         D1 = (prepReloadingHarvester*calcAmount*1000.0) /
              ((9.12*size-17.83)*100);
         D2 = ((prepReloading*calcAmount*1000.0)/carryCapacity) +
              (calcAmount*(-27.10+30.61*size)/60.0);
         break;
      // case ForageHarvesting:       Add later when this junk works properly !!!
      // case GatheringCereals:       Add later when this junk works properly !!!
      case BeetHarvesting:
      case BeetTopHarvesting:
         if (reloadingCapacity==0.0)
            theMessage->FatalError("harvestTech::CalcGangHours - reloadingCapacity = 0");
         if (prepReloading==0.0)
            theMessage->FatalError("harvestTech::CalcGangHours - prepReloading = 0");
         if (prepReloadingHarvester==0.0)
            theMessage->FatalError("harvestTech::CalcGangHours - prepReloadingHarvester = 0");
         D1 = ((prepReloadingHarvester*calcAmount*1000.0)/carryCapacity) + (calcAmount*reloadingCapacity);
         D2 = ((prepReloading*calcAmount*1000.0)/carryCapacity) + (calcAmount*reloadingCapacity);
         break;
      case GatheringBales:
         if(baleType==0)
         {
            if (balesLost==0.0)
               theMessage->FatalError("harvestTech::CalcGangHours - balesLost = 0");
            if (prepReloading==0.0)
               theMessage->FatalError("harvestTech::CalcGangHours - prepReloading = 0");
            if (prepReloadingHarvester==0.0)
               theMessage->FatalError("harvestTech::CalcGangHours - prepReloadingHarvester = 0");
            L1 = 0.0;
            D1 = (prepReloadingHarvester*calcAmount*1000.0)/carryCapacity;
            D2 = ((prepReloading+balesLost)*calcAmount*1000.0)/carryCapacity;
         }
         if(baleType==1)
         {
            if (loadCapacity==0.0)
               theMessage->FatalError("harvestTech::CalcGangHours - loadCapacity = 0");
            if (reloading==0.0)
               theMessage->FatalError("harvestTech::CalcGangHours - reloading = 0");
            if (moweCarriage==0.0)
               theMessage->FatalError("harvestTech::CalcGangHours - moweCarriage = 0");
            double balesPerLoad = ceil(carryCapacity/balesLoad);
            L1 = (((loadCapacity*balesPerLoad) + reloading +
                   (moweCarriage*balesPerLoad/10.0))/
                    carryCapacity)*calcAmount*1000.0;
         }
         break;
   }

   if(operationID==CombineHarvestingShred)
      netCapacityWheat=0.85*netCapacityWheat;

   /* Transportation in field */
   if(operationID==CombineHarvesting || operationID==CombineHarvestingShred)
      B = (0.75*sqrt(area*10000.0/2.0) * calcAmount * 120.0) /
          (carryCapacity*workingSpeed);
   else
   	if (operationID==GatheringBales && baleType==0)
			B = 0.0;
      else
         B = (1.5*sqrt(area*10000.0/2.0) * calcAmount * 120.0) /
          (carryCapacity*workingSpeed);

   /* transportation on road */
   B1 = (distance*calcAmount*120.0)/(carryCapacity*roadSpeed);

   if(operationID==BeetHarvesting)
   {
      if (stops==0.0)
         theMessage->FatalError("harvestTech::CalcGangHours - stops = 0");
      B3 = (distance*calcAmount*120.0)/(carryCapacity*roadSpeed) +
           stops*calcAmount;
      E = stops*calcAmount;
   }

   /* Unloading */
   if(operationID==GatheringBales)
   {
      if (baleType==0)
    	  P1 = ((prepUnloadingStorage*calcAmount*1000.0)/carryCapacity) +
              (calcAmount*unloadCapacity);
      if (baleType==1)
	      P1 = ((prepUnloadingStorage*calcAmount*1000.0)/carryCapacity) +
              ((calcAmount*1000.0*unloadCapacity)/balesLoad);
   }
   else
      if (operationID!=BeetTopChopping && operationID!=Mowing && operationID!=CutSetAside)
         P1 = ((prepUnloadingStorage*calcAmount*1000.0)/carryCapacity) +
              (calcAmount*unloadCapacity);

   if(operationID==BeetHarvesting)
   {
      if (loadClampCapacity==0.0)
         theMessage->FatalError("harvestTech::CalcGangHours - loadClampCapacity = 0");
      if (unloadRoothouseCapacity==0.0)
         theMessage->FatalError("harvestTech::CalcGangHours - unloadRoothouseCapacity = 0");
      if (prepUnloadingRoothouse==0.0)
         theMessage->FatalError("harvestTech::CalcGangHours - prepUnloadingRoothouse = 0");
      if (prepClamp==0.0)
         theMessage->FatalError("harvestTech::CalcGangHours - prepClamp = 0");
      if (uncoverClamp==0.0)
         theMessage->FatalError("harvestTech::CalcGangHours - uncoverClamp = 0");
      P2 = ((prepUnloadingRoothouse*calcAmount*1000.0)/carryCapacity) +
           (calcAmount*unloadRoothouseCapacity);
      L = ((prepClamp*calcAmount*1000.0)/carryCapacity) + (calcAmount*loadClampCapacity);
      R = calcAmount*uncoverClamp;
   }

   /* Deposit */
   switch(operationID)
   {
      // case ForageHarvesting:      Add later when this junk works properly !!!
      case BeetTopHarvesting:
         if (depositCapacity==0.0)
            theMessage->FatalError("harvestTech::CalcGangHours - depositCapacity = 0");
         I1 = calcAmount*depositCapacity;
         break;
      case BeetHarvesting:
         if (depositCapacity==0.0)
            theMessage->FatalError("harvestTech::CalcGangHours - depositCapacity = 0");
         if (depositRoothouseCapacity==0.0)
            theMessage->FatalError("harvestTech::CalcGangHours - depositRoothouseCapacity = 0");
         I1 = calcAmount*depositCapacity;
         I2 = calcAmount*depositRoothouseCapacity;
         break;
      case GatheringBales:
         if(baleType==0)
            I1 = 0;
         break;
   }

   /* Harvest */
   if(operationID==GatheringBales)
      if(baleType==0)
         H = (A+D1+L1)*(1.0+relaxationAllowance);
      if(baleType==1)
         H = (A+A1+L1)*(1.0+relaxationAllowance);
   else
      H = (A+D1)*(1.0+relaxationAllowance);

   /* Preparation for unloading */
   if(operationID==CombineHarvesting || operationID==CombineHarvestingShred)
      O = D2*(1.0+relaxationAllowance);

   /* Transportation */
   switch(operationID)
   {
      case CombineHarvesting:
      case CombineHarvestingShred:
         T = (B+B1)*(1.0+relaxationAllowance);
         break;
      case ForageHarvesting:
      case BeetTopHarvesting:
         T = (B+B1+D2)*(1.0+relaxationAllowance);
         break;
      case BeetHarvesting:
         T = (B+B1+D2+E)*(1.0+relaxationAllowance);
         break;
      case GatheringBales:
         if(baleType==0)
            T = (B+B1+D2)*(1.0+relaxationAllowance);
         if(baleType==1)
            T = (B+B1)*(1.0+relaxationAllowance);
         break;
   }

   /* Unloading */
   P = P1*(1.0+relaxationAllowance);

   /* Deposit */
   if(operationID!=CombineHarvesting || operationID!=CombineHarvestingShred)
      I = I1*(1.0+relaxationAllowance);

   /* Number of workers */
   switch(operationID)
   {
      case CombineHarvesting:
      case CombineHarvestingShred:
         M = numberOfCarriages+1.0;
         break;
      case ForageHarvesting:
      case BeetHarvesting:
      case BeetTopHarvesting:
         M = numberOfCarriages+2.0;
         break;
      case GatheringBales:
         if(baleType==0)
            M = ceil((D2+L1+B+B1+P1)/(A+D1))+1.0;
         break;
   }

   /* Waiting time */
   switch(operationID)
   {
      case CombineHarvesting:
      case CombineHarvestingShred:
         V = (H*numberOfCarriages)-(O+T+P);
         break;
      case ForageHarvesting:
      case BeetHarvesting:
      case BeetTopHarvesting:
         if(H>=I)
            V = (H*M)-(H+T+P+I);
         else
            V = (I*M)-(H+T+P+I);
         break;
      case GatheringBales:
         if(baleType==0)
            V = (A+D1)*M - (A+D1+D2+B1+P1)*(1+relaxationAllowance);
         break;
   }

   if(operationID==BeetHarvesting)
   {
      if((L+R)>=I1)
         V1 = ((L+R)*M)-(L+R+B1+P1+I1);
      else
         V1 = (I1*M)-(L+R+B1+P1+I1);
   }
   /* Sum of all work */
   if(operationID==CombineHarvesting || operationID==CombineHarvestingShred)
      S = (H+O+T+P+fabs(V))*area*1.1/60.0;
   else
      S = (H+T+P+I+V)*area*1.1/60.0;

   if(operationID==BeetHarvesting)
      S += (L+R+B3+P2+I2+V1)*area*1.1/60.0;

   if(operationID==BeetTopChopping || operationID==Mowing || operationID==CutSetAside || ForageHarvesting)
      S = A*area*1.1*(1.0+relaxationAllowance)/60.0;

   if(operationID==GatheringBales)
   {
   	if(baleType==0)
      	S= (H+T+P+I+V)*1.1*area/60.0;
      if(baleType==1)
      	S= (H+T+P)*1.1*area/60.0;
   }
   aGang->SetgangHours(S);
}

/****************************************************************************\
\****************************************************************************/
void harvestTech::CalcGangFuel()
{
   linkList<techEquip> * aTechEquipList;
   linkList<techEquip>::PS aTechEquipNode;

   selfPropelled aSelfPropelled("selfPropelled",0,this);
   implement aImplement("implement",0,this);

   double aGangFuel=0.0;
   double fuelConsumptionTransport = 0.0;

   aTechEquipList=aGang->GetTechEquipList();
   aTechEquipList->PeekHead(aTechEquipNode);

   double calcAmount = 0.0;
   for (int i=0;i<MaxPlants;i++)
   	if (MainProduct[i])
	   	calcAmount += MainProduct[i]->GetAmount();
   double factor= calcAmount*area;
   if (operationID==Mowing || operationID==CutSetAside)
      factor=area;

   while(aTechEquipNode!=NULL)
   {
      if(strcmp(aTechEquipNode->element->GetName(),"selfPropelled")==0)
      {
         aSelfPropelled = *(selfPropelled *)(aTechEquipNode->element);
         aGangFuel += aSelfPropelled.GetfuelConsumption()*factor;
     		fuelConsumptionTransport += aSelfPropelled.GetFuelConsumptionTransport();
      }
     if(strcmp(aTechEquipNode->element->GetName(),"implement")==0)
         {
            aImplement = *(implement *)(aTechEquipNode->element);
            aGangFuel += aImplement.GetfuelConsumption()*factor;
         }
      aTechEquipNode=aTechEquipNode->next;
   }

   // Fuel from transport
   aGangFuel+= calcAmount*(2.0*distance/1000)*fuelConsumptionTransport;

   aGang->SetgangFuel(aGangFuel);
}

/****************************************************************************\
Sends products to storage via Products.cpp
This code is a compromise; technics handles products as an array whilst Products handles them as a linklist
\****************************************************************************/
void harvestTech::UpdateProducts(bool contractor)
{
   fieldOperationTech::UpdateProducts(contractor);

   if ((operationID!=CutSetAside)&&(operationID!=Mowing)&&(operationID!=BeetTopChopping))
   {
	   linkList<plantItem>* aList = new linkList<plantItem>;  //why is this copying necessary??
      for (int i=0;i<MaxPlants;i++)
      {
			if (MainProduct[i] && MainProduct[i]->GetAmount()>0) //pack MainProducts into a linklist and send to Products for transport to storage
			   aList->InsertLast(MainProduct[i]);
      }
      if (aList->NumOfNodes()>0)
      {
	      theProducts->AddHarvestGrainProducts(aList);
      	aList->ForgetAll();
      }
      for (int i=0;i<MaxPlants;i++)
      {
         if (StrawProduct[i] && StrawProduct[i]->GetAmount()>0)
			   aList->InsertLast(StrawProduct[i]);
      }
      if (aList->NumOfNodes()>0)
      {
	      theProducts->AddHarvestStrawProducts(aList);
   	   aList->ForgetAll();
      }
      for (int i=0;i<MaxPlants;i++)
      {
         if (ForageProduct[i] && ForageProduct[i]->GetAmount()>0)
			   aList->InsertLast(ForageProduct[i]);
      }
      if (aList->NumOfNodes()>0)
      {
	      theProducts->AddForageProducts(aList);
   	   aList->ForgetAll();
      }
      delete aList;
   }
}

