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

#ifndef __FIOPML_H
  #define __FIOPML_H

#include <fiop.h>
#include <teopfiml.h>


class manualLabourFields: public fieldOperationFields
{
	private:
		double length, hours;

		manualLabourFields& operator=(const manualLabourFields& f); // Dissable the compilers generation of default assignment operator.

	public:
		manualLabourFields();
		manualLabourFields(const manualLabourFields& h);
      virtual ~manualLabourFields();
      virtual manualLabourFields* clone() const;
		void DefineManualLabourFieldsOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,double len,double HoursPerUnit);
		double GetLength(){return length;};
      double GetHoursPerUnit(){return hours;};
     	virtual bool CalcCost(linkList<operation>    * aOperationList,
                            linkList<techEquip>    * aTechFarmList,
                            calcLPType               mode,
                            double                 * cost,
                            bool                  update);


};

#endif

