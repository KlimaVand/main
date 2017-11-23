/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPHA_H
  #define __FIOPHA_H

#include "fieldOperationFields.h"
#include "../../products/plantItem.h"
#include "../../technics/harvestTech.h"

/**
 * Describe the outcome of a harvest
 */
class harvestFields: public fieldOperationFields
{
	private:
		plantItem * MainProduct[MaxPlants];
      plantItem * StrawProduct[MaxPlants];
      plantItem * ForageProduct[MaxPlants];


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


		virtual void DefineHarvestFieldsOper(operationNames op,string crop,string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,bool forced,double StrWaste = 0.0,double GrainWaste=0.0);

      virtual bool HarvestAndForced(){return forcedHarvest;};
      virtual plantItem* GetForageProduct(int i=0) {return ForageProduct[i];};


		virtual void SetMainProduct(plantItem* aPlantItem,int i=0);
		virtual void AddMainProduct(plantItem* aPlantItem,int i=0);
      virtual void SetStrawProduct(plantItem* aPlantItem,int i=0);
		virtual void AddStrawProduct(plantItem* aPlantItem,int i=0);
      virtual void SetForageProduct(plantItem* aPlantItem,int i=0);


		virtual void SetCuttingHeight(double height){CuttingHeight = height;};

		virtual double GetCuttingHeight(){return CuttingHeight;};
		virtual double GetFractionStrawHarvested(){return FractionStrawHarvested;};
		virtual double GetFractionGrainHarvested(){return FractionGrainHarvested;};

  /*    virtual bool CalcCost(linkList<operation>    * aOperationList,
                              linkList<techEquip>     * aTechFarmList,
                              double                 * cost,
                              bool                  update);
*/
		void Output(ofstream * fs);
		void UpdateProducts();
		virtual bool IsHarvestOp(){return true;}; // Nick defines this differently - why ???

};

#endif

