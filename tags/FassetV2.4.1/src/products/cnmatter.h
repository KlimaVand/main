/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
 \****************************************************************************/
// =============================================================================
// Organic matter, only C and N considered
// (c) Bjørn Molt Petersen, Danmarks JordbrugsForskning 2001
// =============================================================================

#ifndef ORGMATTER_H
#define ORGMATTER_H



class cnMatter
{
public:
	double c, c_iso, n, n15;

	cnMatter();
	cnMatter(const cnMatter& source);
	cnMatter(double c1, double c1_iso, double n1, double n1_15);
	void Add(cnMatter* source, double fraction);
	virtual ~cnMatter(){};
	double Get15NAtomExcess();
	double Get14CAge(double halfTime);
	double Getdelta(double Rzs);
	double GetPercentModern();
	double GetTotalC();
	double GetTotalN();
	double GetCNratio();
	void SetAll(double c1, double c1_iso, double n1, double n1_15);
	void Clear(){SetAll(0, 0, 0, 0);};
	cnMatter& operator=(const cnMatter& cnm);
	cnMatter operator+(const cnMatter& cnm);
	cnMatter operator-(const cnMatter& cnm);
	cnMatter operator*(const double& factor);
	cnMatter operator/(const double& divisor);
};

#endif

