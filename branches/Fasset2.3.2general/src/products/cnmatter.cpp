/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
 \****************************************************************************/
#include "../base/common.h"
#include "cnmatter.h"

/****************************************************************************\
Default constructor
 \****************************************************************************/
cnMatter::cnMatter()
{
	c = 0.0;
	c_iso = 0.0;
	n = 0.0;
	n15 = 0.0;
}

/****************************************************************************\
Copy constructor
 \****************************************************************************/
cnMatter::cnMatter(const cnMatter& source)
{
	if (&source)
	{
		c = source.c;
		c_iso = source.c_iso;
		n = source.n;
		n15 = source.n15;
	}
}

/****************************************************************************\
  Constructor with arguments
 \****************************************************************************/
cnMatter::cnMatter(double c1, double c1_iso, double n1, double n1_15)
{
	c = c1;
	c_iso = c1_iso;
	n = n1;
	n15 = n1_15;
}

/****************************************************************************\
\****************************************************************************/
void cnMatter::Add(cnMatter* source, double fraction)
{
	c = (1.0 - fraction) * c + fraction * source->c;
	c_iso = (1.0 - fraction) * c_iso + fraction * source->c_iso;
	n = (1.0 - fraction) * n + fraction * source->n;
	n15 = (1.0 - fraction) * n15 + fraction * source->n15;
}

/****************************************************************************\
\****************************************************************************/
double cnMatter::Get14CAge(double halfTime)
{
	double retVal = 1E9;
	if (c_iso > 0.0)
		retVal = halfTime * log(c_iso / c) / log(0.5);
	return retVal;
}

/****************************************************************************\
\****************************************************************************/
double cnMatter::Get15NAtomExcess()
{
	double retVal = 0.0;
	if (n > 1E-9)
		retVal = 100.0 * n15 / n - 0.3663033;
	return retVal;
}

/****************************************************************************\
\****************************************************************************/
double cnMatter::Getdelta(double Rzs)
{
	if (c < 1E-20)
		return 0.0;
	else
		return 1000.0 * (c_iso
				/ ((1.0 + Rzs) * (c - c_iso * Rzs / (1.0 + Rzs))) - 1.0);
}

/****************************************************************************\
\****************************************************************************/
void cnMatter::SetAll(double c1, double c1_iso, double n1, double n1_15)
{
	c = c1;
	c_iso = c1_iso;
	n = n1;
	n15 = n1_15;
}

/****************************************************************************\
\****************************************************************************/
double cnMatter::GetTotalC()
{
	return c + c_iso;
}

/****************************************************************************\
\****************************************************************************/
double cnMatter::GetTotalN()
{
	return n + n15;
}

/****************************************************************************\
\****************************************************************************/
double cnMatter::GetCNratio()
{
	double ctot = GetTotalC();
	double ntot = GetTotalN();
	if (ntot < 1E-20)
		return -1.0;
	else
		return ctot / ntot;
}

/****************************************************************************\
\****************************************************************************/
cnMatter& cnMatter::operator=(const cnMatter& cnm)
{
	c = cnm.c;
	c_iso = cnm.c_iso;
	n = cnm.n;
	n15 = cnm.n15;
	return *this;
}

/****************************************************************************\
\****************************************************************************/
cnMatter cnMatter::operator+(const cnMatter& cnm)
{
	cnMatter copy = *this;
	copy.c += cnm.c;
	copy.c_iso += cnm.c_iso;
	copy.n += cnm.n;
	copy.n15 += cnm.n15;
	return copy;
}

/****************************************************************************\
\****************************************************************************/
cnMatter cnMatter::operator-(const cnMatter& cnm)
{
	cnMatter copy = *this;
	copy.c -= cnm.c;
	copy.c_iso -= cnm.c_iso;
	copy.n -= cnm.n;
	copy.n15 -= cnm.n15;
	return copy;
}

/****************************************************************************\
\****************************************************************************/
cnMatter cnMatter::operator*(const double& factor)
{
	cnMatter copy = *this;
	copy.c = copy.c * factor;
	copy.c_iso = copy.c_iso * factor;
	copy.n = copy.n * factor;
	copy.n15 = copy.n15 * factor;
	return copy;
}

/****************************************************************************\
\****************************************************************************/
cnMatter cnMatter::operator/(const double& divisor)
{
	cnMatter copy = *this;
	copy.c = copy.c / divisor;
	copy.c_iso = copy.c_iso / divisor;
	copy.n = copy.n / divisor;
	copy.n15 = copy.n15 / divisor;
	return copy;
}

