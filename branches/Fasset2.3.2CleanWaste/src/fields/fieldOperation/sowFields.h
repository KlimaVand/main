/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPSO_H
  #define __FIOPSO_H

#include "fieldOperationFields.h"
#include "../../products/seed.h"
#include "../../technics/sowTech.h"


class sowFields: public fieldOperationFields
{
	protected:
   	struct sowStruct
      {
         char crop_id[20];
   	 	seed * seeds;
      };
   	//!Optional reduction in radiation use efficiency
      double reduction; //
		sowStruct plants[MaxPlants];
		//! Plant Protection strategy
      int PVStrategy;
      //! number of plants in that area
      int numOfPlants;

		sowFields& operator=(const sowFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		sowFields();
		sowFields(const sowFields& h);
		virtual ~sowFields();
      virtual sowFields* clone() const;
		virtual void DefineSowFieldsOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,int PVStrategy,double reduc,seed * theSeed);
      virtual seed * GetSeed(int i) {return plants[i].seeds;};
      virtual const char * GetSpecificCropId(int i) {return plants[i].crop_id;};

      virtual double GetReduction() {return reduction;};
      virtual void AddSeed(seed * s,string  crop);

      virtual int GetSeedNum(){return numOfPlants;};
  		virtual bool CalcCost(linkList<operation>    * aOperationList,
                           linkList<techEquip>     * aTechFarmList,
                           double                 * cost,
                           bool                  update);

		virtual void Output(ofstream * fs);

#ifdef MEASCOPE
      virtual void DefineSowFieldsOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
      				 int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper, double ar,int fieldn,double dist,int aPVStrategy,double reduc,seed * theSeed);
#endif
};

#endif

