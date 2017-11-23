/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPAN_H
  #define __FIOPAN_H

#include <fiop.h>
#include <pmanure.h>
#include <teopfian.h>

class animalFertFields: public fieldOperationFields
{
	private:
		manure * aManure;
      double exposeTime;

		animalFertFields& operator=(const animalFertFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		animalFertFields();
		animalFertFields(const animalFertFields& a);
		virtual ~animalFertFields();
      virtual animalFertFields* clone() const;
      virtual bool IsFertilizingOp(){return true;};
		virtual void DefineAnimalFertFieldsOp(operationNames op,id_string crop,id_string cropNm,
		                              int yr,int mon,int d,double ar,int fieldn,double dist,manure * theManure, double anExposeTime = 24.0);//, id_string applicMethod);
      virtual manure * GetManure(){return aManure;};
      virtual double GetExposeTime(){return exposeTime;};

		virtual bool CalcCost(linkList<operation>    * aOperationList,
                              linkList<techEquip>     * aTechFarmList,
                              calcLPType               mode,
                              double                 * cost,
                              bool                  update);
		virtual fieldOperationTech* GetfieldOperationTech(linkList<operation>    * aOperationList);
      virtual void AdjustManure(double adjustmentFactor);
		void Output(fstream * fs);
#ifdef MEASCOPE
      virtual void DefineAnimalFertFieldsOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
      					int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper,double ar,int fieldn,double dist,manure * theManure, double anExposeTime = 24.0);
#endif
};

#endif

