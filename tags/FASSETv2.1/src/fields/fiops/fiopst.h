/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPST_H
  #define __FIOPST_H

#include <fiop.h>
#include <teopfist.h>

class soilTreatFields: public fieldOperationFields
{
   private:
      string Type;                                          // Tillage Type
      double Depth;                                         // Tillage Depth
      double FractionIncorporated;                          // Fraction of surface organic matter incorporated

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
                              calcLPType               mode,
                              double                 * cost,
                              bool                  update);

};

#endif
