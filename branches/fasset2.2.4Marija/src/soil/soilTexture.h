/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// soiltxtr.h
// Soil texture
// (c) Jørgen E. Olesen, Statens Planteavlsforsøg
// E-mail: jeo@pvf.sp.dk
// Changes:
// JEO 25-06-1997: Copy constructor added.
// =============================================================================

#ifndef SOILTXTR
#define SOILTXTR

#include "../base/base.h"

class soilTexture : public base
{
 protected:
 double clay;       // unit: g/g
 double coarseSand; // unit: g/g
 double fineSand;   // unit: g/g
 double humus;      // unit: g/g
 double silt;       // unit: g/g

 public:
 soilTexture() : base()
 { clay = coarseSand = fineSand = humus = silt = -1; };
 soilTexture(const soilTexture& texture);

 double GetClayContent() {return clay;};     // unit: g/g
 int    GetSoilType();                       // unit: Danish JB number
 double GetFieldCapacity();                  // unit: mm/mm
 double GetWiltCapacity();                   // unit: mm/mm
};

#endif
