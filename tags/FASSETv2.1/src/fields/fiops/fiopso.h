/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPSO_H
  #define __FIOPSO_H

#include <fiop.h>
#include <pseed.h>
#include <teopfiso.h>


class sowFields: public fieldOperationFields
{
	protected:
   	struct sowStruct
      {
         char crop_id[20];
   	 	seed * seeds;
      };
      double reduction; // Optional reduction in radiation use efficiency
		sowStruct plants[MaxPlants];
      int PVStrategy;                      // Plant Protection strategy
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
      virtual int GetPVStrat() {return PVStrategy;};
      virtual double GetReduction() {return reduction;};
      virtual void AddSeed(seed * s,char * crop);
      virtual bool IsSowingOp(){return true;};
      virtual int GetSeedNum(){return numOfPlants;};
  		virtual bool CalcCost(linkList<operation>    * aOperationList,
                           linkList<techEquip>     * aTechFarmList,
                           calcLPType               mode,
                           double                 * cost,
                           bool                  update);

		virtual void Output(fstream * fs);

#ifdef MEASCOPE
      virtual void DefineSowFieldsOp(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
      				 int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper, double ar,int fieldn,double dist,int aPVStrategy,double reduc,seed * theSeed);
#endif
};

#endif

