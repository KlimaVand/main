/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPGRAZ_H
  #define __FIOPGRAZ_H

#include "harvestFields.h"

class makeGrazable: public harvestFields
{
	private:

		makeGrazable& operator=(const makeGrazable& f); // Dissable the compilers generation of default assignment operator.

	public:
		makeGrazable();
		makeGrazable(const makeGrazable& h);
		virtual ~makeGrazable();
      virtual makeGrazable* clone() const;
		virtual void DefinemakeGrazableOper(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double dist,bool makePossible);


      virtual bool CalcCost(linkList<operation> * aOperationList, // A HACK !!!!!
                         linkList<techEquip> * aTechFarmList,
                         double              * cost,
                         bool                  update){return false;};
  virtual bool IsHarvestOp(){return false;}; // Otherwise this can block the entire set of field operations - BMP added 15.12.2006 !!!
#ifdef MEASCOPE
		virtual void DefinemakeGrazableOper(operationNames op,id_string crop,id_string cropNm,int Startyr,int Startmon,int Startday,
						int Endyr,int Endmon,int Endday,double aTSum, double asoilMoisture,bool anupper, double ar,int fieldn,double dist, bool makePossible);
#endif
};

#endif

