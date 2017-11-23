/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// pfcurve.cpp
// Soil water retension curve
// (c) Jørgen E. Olesen, Statens Planteavlsforsøg
// E-mail: jeo@pvf.sp.dk
// =============================================================================

#include "../base/common.h"
#include "pFCurve.h"

/****************************************************************************\
\****************************************************************************/
pF_Curve::pF_Curve(int intPool) : base()
{
	aPF_Curve = new pF_Curve_Implementation(intPool);
}

/****************************************************************************\
\****************************************************************************/
pF_Curve::pF_Curve(const pF_Curve& pFSource)
{
	aPF_Curve = pFSource.aPF_Curve;
   aPF_Curve->AddOnePFCurve();
}

/****************************************************************************\
\****************************************************************************/
pF_Curve::~pF_Curve()
{
	if (aPF_Curve->GetNumberOfPFCurves()==0)
   	delete aPF_Curve;
   else
      aPF_Curve->RemoveOnePFCurve();
}


