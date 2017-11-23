/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOP_H
  #define __FIOP_H

#include <product.h>
#include <opnames.h>
#include <teoperat.h>
#include <teopfi.h>

class fieldOperationFields
{
	protected:
		typedef char   id_string[20];
		int            fieldno;
		double         area,
							distance;
		operationNames op_id;
		id_string      crop_id,
							cropName;
      bsTime*        opTime;
      
#ifdef MEASCOPE
      bsTime*        opStart;
      bsTime*        opEnd;
      double         TSum,
                     soilMoisture;
      bool           upper;
#endif

		fieldOperationFields& operator=(const fieldOperationFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		fieldOperationFields();
		fieldOperationFields(const fieldOperationFields& f);
      virtual ~fieldOperationFields();
      virtual fieldOperationFields* clone() const=0;
		virtual void DefineOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist);
		friend ostream &operator<<(ostream &os,const fieldOperationFields &aFieldOp);
		virtual bool IsHarvestOp(){return false;};
		virtual bool IsForageHarvestOp(){return false;};
      virtual bool IsFertilizingOp(){return false;};
      virtual bool IsSowingOp(){return false;};
      virtual bool HarvestAndForced(){return false;};
      virtual bool IsIrrigationOp(){return false;};
      virtual bool AutomaticIrrigation(){return false;};
		virtual int GetYear(){return opTime->GetYear();};
		virtual int GetMonth(){return opTime->GetMonth();};
		virtual int GetDay(){return opTime->GetDay();};
		virtual int GetFieldNumber(){return fieldno;};
		virtual double GetArea(){return area;};
		virtual double GetDistance(){return distance;}
		virtual int GetOperationId(){return op_id;};
		virtual const char * GetCropId(){return (char*)&crop_id;};
		virtual const char * GetCropName(){return (char*)&cropName;};
      virtual bsTime * GetOpTime(){return opTime;};

		virtual void Show();
		virtual void Output(fstream * fs);
   	virtual bool CalcCost(linkList<operation> * aOperationList,
                         linkList<techEquip> * aTechFarmList,
                         calcLPType            mode,
                         double              * cost,
                         bool                  update){return false;};
		virtual fieldOperationTech* GetfieldOperationTech(linkList<operation>    * aOperationList) {return NULL;};
      virtual void DelayOneDay() {opTime->AddOneDay();};
#ifdef MEASCOPE
      void SetopStart(bsTime * aDate){opStart=aDate;};
      bsTime * GetopStart(){return opStart;};
      void SetopEnd(bsTime * aDate){opEnd=aDate;};
      bsTime * GetopEnd(){return opEnd;};
      void SetTSum(double aTSum){TSum = aTSum;};
      double GetTSum(){return TSum;};
      bool Getupper(){return upper;};
      void SetsoilMoisture(double aSoilMoisture){soilMoisture= aSoilMoisture;};
      double GetsoilMoisture(){return soilMoisture;};
		int GetStartYear(){return opStart->GetYear();};
		int GetStartMonth(){return opStart->GetMonth();};
		int GetStartDay(){return opStart->GetDay();};
		int GetEndYear(){return opEnd->GetYear();};
		int GetEndMonth(){return opEnd->GetMonth();};
		int GetEndDay(){return opEnd->GetDay();};
      virtual void DefineOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
      				      	 int Endyr,int Endmon,int Endday,double aTSum, double aSoilMoisture, bool anupper, double ar,int fieldn,double dist);
#endif

};

#endif

