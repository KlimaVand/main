/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPHA_H
  #define __FIOPHA_H

#include <fiop.h>
#include <pplantit.h>
#include <teopfiha.h>


class harvestFields: public fieldOperationFields
{
	private:
		plantItem * MainProduct[MaxPlants];
      plantItem * StrawProduct[MaxPlants];
      plantItem * ForageProduct[MaxPlants];

      double reduction;
      bool forcedHarvest;
      double CuttingHeight;
      double FractionStrawHarvested;
      double FractionGrainHarvested;

		harvestFields& operator=(const harvestFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		harvestFields();
		harvestFields(const harvestFields& h);
		virtual ~harvestFields();
      virtual harvestFields* clone() const;
		friend ostream &operator<<(ostream &os,const harvestFields &aFieldOp);

		virtual void DefineHarvestFieldsOper(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,double reduc,bool forced,double StrWaste = 0.0,double GrainWaste=0.0);

      virtual bool HarvestAndForced(){return forcedHarvest;};
		virtual plantItem* GetMainProduct(int i=0) {return MainProduct[i];};
      virtual plantItem* GetStrawProduct(int i=0) {return StrawProduct[i];};
      virtual plantItem* GetForageProduct(int i=0) {return ForageProduct[i];};

		virtual void SetMainProduct(plantItem* aPlantItem,int i=0);
		virtual void AddMainProduct(plantItem* aPlantItem,int i=0);
      virtual void SetStrawProduct(plantItem* aPlantItem,int i=0);
		virtual void AddStrawProduct(plantItem* aPlantItem,int i=0);
      virtual void SetForageProduct(plantItem* aPlantItem,int i=0);
      virtual void AddForageProduct(plantItem* aPlantItem,int i=0);

		virtual void SetCuttingHeight(double height){CuttingHeight = height;};
      virtual double GetReduction(){return reduction;};
		virtual double GetCuttingHeight(){return CuttingHeight;};
		virtual double GetFractionStrawHarvested(){return FractionStrawHarvested;};
		virtual double GetFractionGrainHarvested(){return FractionGrainHarvested;};

      virtual bool CalcCost(linkList<operation>    * aOperationList,
                              linkList<techEquip>     * aTechFarmList,
                              calcLPType               mode,
                              double                 * cost,
                              bool                  update);
      virtual void ClearProducts();
#ifndef MEASCOPE
		virtual bool IsHarvestOp(){return true;}; // Nick defines this differently - why ???
#else  //only for MEASCOPE
      virtual void DefineHarvestFieldsOper(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
						 int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper,double ar,
                   int fieldn,double dist,double reduc,bool forced,double StrHarvest,double GrnHarvest);
		void Output(fstream * fs);
      virtual bool IsHarvestOp();
#endif
};

#endif

