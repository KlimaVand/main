/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOP_H
  #define __FIOP_H

#include "../../products/product.h"
#include "../../base/operationNames.h"
#include "../../technics/operation.h"
#include "../../technics/fieldOperationTech.h"
/**
 * Field operation is things that you can do on any field such as irrigate and harvest
 */
class fieldOperationFields
{
	protected:
		typedef char   id_string[20];
		//! The field that need this operation
		int            fieldno;
		//! The area of the field that need this operation
		double         area;
		//! The distance to the field that need this operation
		double         distance;
		//! the name of the operation
		operationNames op_id;
		//! Detail information about name of the crop
		id_string      crop_id;
		//! Name of the crop
		id_string		cropName;
		//! time it takes for the operarion
      bsTime*        opTime;
      


		fieldOperationFields& operator=(const fieldOperationFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		fieldOperationFields();
		fieldOperationFields(const fieldOperationFields& f);
      virtual ~fieldOperationFields();
      virtual fieldOperationFields* clone() const=0;
		virtual void DefineOp(operationNames op,string crop,string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist);
		friend ostream &operator<<(ostream &os,const fieldOperationFields &aFieldOp);
		//! returning false
		virtual bool IsHarvestOp(){return false;};
		//! returning false
		virtual bool IsForageHarvestOp(){return false;};
		//! returning false
      virtual bool IsFertilizingOp(){return false;};
      //! returning false
      virtual bool IsSowingOp(){return false;};
      //! returning false
      virtual bool HarvestAndForced(){return false;};
      //! returning false
      virtual bool IsIrrigationOp(){return false;};
      //! returning false
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

	  void UpdateProducts(){};
		virtual void Output(ofstream * fs);
		//! returning false
   	virtual bool CalcCost(linkList<operation> * aOperationList,
                         linkList<techEquip> * aTechFarmList,
                         calcLPType            mode,
                         double              * cost,
                         bool                  update){return false;};
		virtual fieldOperationTech* GetfieldOperationTech(linkList<operation>    * aOperationList) {return nullptr;};
      virtual void DelayOneDay() {opTime->AddOneDay();};
//!Sets operation identifier
/*!
\param aVal The value of the operation identifier
*/
		virtual void SetOperationId(int aVal){op_id = operationNames(aVal);};



};

#endif

