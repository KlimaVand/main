/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPIR_H
  #define __FIOPIR_H

#include "fieldOperationFields.h"
#include "../../products/water.h"
#include "../../technics/irrigateTech.h"

class irrigateFields: public fieldOperationFields
{
	private:
		water * aWater;
      bool automaticIrrigation;

		irrigateFields& operator=(const irrigateFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		irrigateFields();
		irrigateFields(const irrigateFields& i);
		virtual ~irrigateFields();
      virtual irrigateFields* clone() const;
		virtual void DefineIrrigateFieldsOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,water * theWater,bool automatic);
		virtual water * GetWater() {return aWater;};
      virtual bool IsIrrigationOp(){return true;};
      virtual bool AutomaticIrrigation(){return automaticIrrigation;};
     	virtual bool CalcCost(linkList<operation>    * aOperationList,
                              linkList<techEquip>     * aTechFarmList,
                              double                 * cost,
                              bool                  update);
#ifdef MEASCOPE
		virtual void DefineIrrigateFieldsOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
      					int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper, double ar,int fieldn,double dist, water * theWater,bool automatic);
#endif
};

#endif

