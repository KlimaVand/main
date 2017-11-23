/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPSI_H
  #define __FIOPSI_H

#include "fieldOperationFields.h"
#include "../../products/plantItem.h"

class silageFields: public fieldOperationFields
{
	private:
		plantItem * plantItems[MaxPlants];
		silageFields& operator=(const silageFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		silageFields();
		silageFields(const silageFields& h);
		virtual ~silageFields();
      virtual silageFields* clone() const;
		void DefinesilageFieldsOper(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,
                                 plantItem * thePlantItem);
		plantItem * GetPlantItem(){return plantItems[0];};
      void SetPlantItem(plantItem * newPlantItem) {plantItems[0]=newPlantItem;};
      virtual bool CalcCost(linkList<operation> * aOperationList, // A HACK !!!!!
                         linkList<techEquip> * aTechFarmList,
                         double              * cost,
                         bool                  update);

#ifdef MEASCOPE
      void DefinesilageFieldsOper(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
  						       int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper,double ar,int fieldn,double dist,
                         plantItem * thePlantItem);
#endif
};

#endif

