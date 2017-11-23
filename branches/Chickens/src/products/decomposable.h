/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
 \****************************************************************************/
/****************************************************************************\
revision			july major revision of all product's.         (JB)
 All operators calls the superclass
 operator check's
 if similar types, storage etc  added/subtracted
 if amount=0 the object gets the same unit, storage etc
 as the object that is added to it
 \****************************************************************************/

#ifndef __PDECOMP_H
#define __PDECOMP_H

#include "../products/product.h"
#include "../products/liveMatter.h"

class decomposable: public liveMatter
{
protected:
	double kAOM1;
	double kAOM2;
	double AOM1_fraction;
	double AOM2_fraction;
	double VFA_fraction;	// The fraction of OM2 that is Volatile Fatty Acids
	double AOM1_Nfraction;
	double AOM2_Nfraction;
	double ReduceTurnover;

	void InitVariables();

public:
	// Default Constructor
	decomposable();
	// Constructor with arguments
	decomposable(const char * aName, const int aIndex = -1,
			const base * aOwner = NULL);
	// Copy Constructor
	decomposable(const decomposable& adecomposable);
	// Destructor
	virtual ~decomposable();


	// Set functions
	void SetkAOM1(const double k)
	{
		kAOM1 = k;
	}
	void SetkAOM2(const double k)
	{
		kAOM2 = k;
	}
	void SetAOM1_fraction(const double aFraction)
	{
		AOM1_fraction = aFraction;
	}
	void SetAOM2_fraction(const double aFraction)
	{
		AOM2_fraction = aFraction;
	}
	void SetVFA_fraction(const double aFraction)
	{
		VFA_fraction = aFraction;
	}
	void SetAOM1_Nfraction(const double aFraction)
	{
		AOM1_Nfraction = aFraction;
	}
	void SetAOM2_Nfraction(const double aFraction)
	{
		AOM2_Nfraction = aFraction;
	}
	void SetReduceTurnover(const double Reduce)
	{
		ReduceTurnover = Reduce;
	}

	// Get functions
	double GetkAOM1() const
	{
		return kAOM1;
	}
	double GetkAOM2() const
	{
		return kAOM2;
	}
	double GetAOM1_fraction() const
	{
		return AOM1_fraction;
	}
	double GetAOM2_fraction() const
	{
		return AOM2_fraction;
	}
	double GetVFA_fraction() const
	{
		return VFA_fraction;
	}
	double GetAOM1_Nfraction() const
	{
		return AOM1_Nfraction;
	}
	double GetAOM2_Nfraction() const
	{
		return AOM2_Nfraction;
	}
	double GetReduceTurnover() const
	{
		return ReduceTurnover;
	}

	// Other functions
	virtual product& operator=(const product& somedecomposable);
	product& operator+(const product& somedecomposable);
	virtual product& operator-(const product& somedecomposable);
	product* clone() const;
	virtual void ReadParameters(fstream * file, bool criticals,const char *  sectionName);
};

#endif

