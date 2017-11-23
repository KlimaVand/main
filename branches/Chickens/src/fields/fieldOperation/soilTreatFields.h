/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPST_H
  #define __FIOPST_H

#include "fieldOperationFields.h"
#include "../../technics/soilTreatTech.h"

class soilTreatFields: public fieldOperationFields
{
   private:
	 //! Tillage Type
      string Type;
      //! Tillage Depth
      double Depth;
      //! Fraction of surface organic matter incorporated
      double FractionIncorporated;

      soilTreatFields& operator=(const soilTreatFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		soilTreatFields();
      virtual ~soilTreatFields();
      virtual void SetType(string aType) {Type = aType;};
      virtual void SetDepth(double aDepth) {Depth = aDepth;};
      virtual void SetFractionIncorporated(double aFractionIncorporated) {FractionIncorporated = aFractionIncorporated;};

      virtual string GetType() {return Type;};
      virtual double GetDepth() { return Depth;};
      virtual double GetFractionIncorporated() {return FractionIncorporated;};
		virtual soilTreatFields* clone() const;

     	virtual bool CalcCost(linkList<operation>    * aOperationList,
                              linkList<techEquip>     * aTechFarmList,
                              double                 * cost,
                              bool                  update);

};

#endif
