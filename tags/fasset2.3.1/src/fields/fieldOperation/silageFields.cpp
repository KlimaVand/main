/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Revision: JB added support for several species
\****************************************************************************/

#include "../../base/common.h"
#include "silageFields.h"

/****************************************************************************\
\****************************************************************************/
silageFields::silageFields()
{
	for (int i=0;i<MaxPlants;i++)
   	plantItems[i] = NULL;
}

/****************************************************************************\
\****************************************************************************/
silageFields::silageFields(const silageFields& h)
   : fieldOperationFields(h)
{
	for (int i=0;i<MaxPlants;i++)
   {
      if (h.plantItems[i]!=NULL)
      	plantItems[i] = new plantItem(*h.plantItems[i]);
      else
         plantItems[i]=NULL;
   }
}

/****************************************************************************\
\****************************************************************************/
silageFields::~silageFields()
{
	for (int i=0;i<MaxPlants;i++)
   	if (plantItems[i]) delete plantItems[i];
}

/****************************************************************************\
\****************************************************************************/
silageFields* silageFields::clone() const
{
	silageFields* h=new silageFields(*this);
	return h;
}

/****************************************************************************\
\****************************************************************************/
void silageFields::DefinesilageFieldsOper(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,
                                          plantItem * thePlantItem)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,yr, mon, d, ar,fieldn,dist);
	for (int i=0;i<MaxPlants;i++)
      plantItems[i]=NULL;
   if (thePlantItem)
   	plantItems[0]=new plantItem(*thePlantItem);
}

/**
 * is not implemented
 */
bool silageFields::CalcCost(linkList<operation> * aOperationList, // A HACK !!!!!
                         linkList<techEquip> * aTechFarmList,
                         double              * cost,
                         bool                  update)
{
   theMessage->Warning("Currently no cost information for silage making");

   //Not implemented!!!!!!!!!!!!!

   #ifdef DEVELOPING
	aOperationList=aOperationList;
   aTechFarmList=aTechFarmList;
   mode=mode;
   cost=cost;
   update=update;
   #endif
	return true;
}

#ifdef MEASCOPE
/****************************************************************************\
\****************************************************************************/
void silageFields::DefinesilageFieldsOper(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
						 int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper,double ar,int fieldn,double dist,
                   plantItem * thePlantItem)
{
	fieldOperationFields::DefineOp(op,crop,cropNm,Startyr,Startmon,Startday,Endyr,Endmon,Endday,aTSum,asoilMoisture,anupper, ar,fieldn,dist);
	for (int i=0;i<MaxPlants;i++)
      plantItems[i]=NULL;
   if (thePlantItem)
   	plantItems[0]=new plantItem(*thePlantItem);
}
#endif


