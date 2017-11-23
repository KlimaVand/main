/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
#ifndef __FIOPGR_H
  #define __FIOPGR_H

#include "fieldOperationFields.h"
/**
 * Describe what happens when animals are grazing
 */
class grazingFields: public fieldOperationFields
{
   private:
      grazingFields& operator=(const grazingFields& f); // Dissable the compilers generation of default assignment operator.
      double
         nL,
         grazingEffiency;

	public:
		grazingFields();
      virtual ~grazingFields();
      grazingFields(const grazingFields& g);
      virtual grazingFields* clone() const;
      void DefineGrazingOp(operationNames op,id_string crop,id_string cropNm,int yr,int mon,int d,double ar,int fieldn,double nLoad, double eff);

};

#endif

