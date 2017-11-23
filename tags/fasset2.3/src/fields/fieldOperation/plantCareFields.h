/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
  $Archive$
   $Author$
     $Date$
 $Revision$
 ****************************************************************************
      $Log$
\****************************************************************************/

#ifndef __FIOPPC_H
  #define __FIOPPC_H

#include "fieldOperationFields.h"
#include "../../products/chemical.h"
#include "../../products/water.h"
#include "../../technics/plantCareTech.h"


class plantCareFields: public fieldOperationFields
{
	private:
		water * aWater;
		chemical * aChemical1;
		chemical * aChemical2;
		chemical * aChemical3;

		plantCareFields& operator=(const plantCareFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		plantCareFields();
		plantCareFields(const plantCareFields& p);
		virtual ~plantCareFields();
      virtual plantCareFields* clone() const;
		void DefinePlantCareFieldsOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,chemical * chem1,chemical * chem2,chemical * chem3,water * wat);
		chemical * Get1stChemical(){return aChemical1;};
		chemical * Get2ndChemical(){return aChemical2;};
		chemical * Get3rdChemical(){return aChemical3;};
		water * GetWater(){return aWater;};   
     virtual bool CalcCost(linkList<operation>    * aOperationList,
                              linkList<techEquip>     * aTechFarmList,
                              double                 * cost,
                              bool                  update);

};

#endif

