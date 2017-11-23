/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
// =============================================================================
// soiltxtr.cpp
// Soil texture
// (c) Jørgen E. Olesen, Statens Planteavlsforsøg
// E-mail: jeo@pvf.sp.dk
// Changes:
// JEO 26-07-1997: Copy constructor added.
// =============================================================================

#include "../base/common.h"
#include "soilTexture.h"

/****************************************************************************\
\****************************************************************************/
soilTexture::soilTexture(const soilTexture& texture)
 : base(texture)
{
 if (&texture)
 {
  clay = texture.clay;
  coarseSand = texture.coarseSand;
  fineSand = texture.fineSand;
  humus = texture.humus;
  silt = texture.silt;
 }
}

/****************************************************************************\
\****************************************************************************/
int soilTexture::GetSoilType()
{
 int JB = -1;
 if (clay>=0 && silt>=0 && coarseSand>=0 && fineSand>=0 && humus>=0)
 {
  if (humus>=0.10) JB = 11;
  else if (clay<5)
  {
	if (silt>=0.20) JB = 10;
	else if (fineSand<0.50) JB = 1;
	else JB = 2;
  }
  else if (clay<0.10)
  {
	if (silt>=0.25) JB = 10;
	else if (fineSand<0.40) JB = 3;
	else JB = 4;
  }
  else if (clay<0.15)
  {
	if (silt>=0.30) JB = 10;
	else if (fineSand<0.40) JB = 5;
	else JB = 6;
  }
  else if (clay<0.25)
  {
	if (silt>=0.35) JB = 10;
	else JB = 7;
  }
  else if (clay<0.45)
  {
	if (silt>=0.45) JB = 10;
	else JB = 8;
  }
  else if (silt>=0.45) JB = 10;
  else JB = 9;
 };
 return JB;
}

/****************************************************************************\
\****************************************************************************/
double soilTexture::GetFieldCapacity()
{
 double FC = -1;
 if (clay>=0 && silt>=0 && fineSand>=0 && humus>=0)
  FC = 0.0368+2.35*humus+0.70*clay+0.47*silt+0.18*fineSand;
 return FC;
}

/****************************************************************************\
\****************************************************************************/
double soilTexture::GetWiltCapacity()
{
 double WC = -1;
 if (clay>=0 && silt>=0 && humus>=0)
  WC = 0.0112+0.55*humus+0.63*clay+0.18*silt;
 return WC;
}

