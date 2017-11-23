/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPMI_H
  #define __FIOPMI_H

#include "fieldOperationFields.h"
#include "../../products/fertilizer.h"
#include "../../technics/mineralFertTech.h"
/**
 * Describe what to do when fertilisering a field
 */
class mineralFertFields: public fieldOperationFields
{
	private:
		fertilizer * aFertilizer1;
		fertilizer * aFertilizer2;
		fertilizer * aFertilizer3;

		mineralFertFields& operator=(const mineralFertFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		mineralFertFields();
		mineralFertFields(const mineralFertFields& m);
		virtual ~mineralFertFields();
      virtual mineralFertFields* clone() const;
      virtual bool IsFertilizingOp(){return true;};
      void DefineMineralFertFieldsOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,fertilizer * theFertilizer1,fertilizer * theFertilizer2,fertilizer * theFertilizer3);
		virtual fertilizer * GetFertilizer1(){return aFertilizer1;};
		virtual fertilizer * GetFertilizer2(){return aFertilizer2;};
		virtual fertilizer * GetFertilizer3(){return aFertilizer3;};
     	virtual bool CalcCost(linkList<operation>    * aOperationList,
                              linkList<techEquip>     * aTechFarmList,
                              double                 * cost,
                              bool                  update);
      virtual void Output(ofstream * fs);
#ifdef MEASCOPE
      virtual void DefineMineralFertFieldsOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
						 int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper, double ar,int fieldn,double dist,fertilizer * theFertilizer1,fertilizer * theFertilizer2,fertilizer * theFertilizer3);
#endif
};

#endif

