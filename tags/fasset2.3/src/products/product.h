/****************************************************************************\
 $URL$
 $LastChangedDate$
 $LastChangedRevision$
 $LastChangedBy$
\****************************************************************************/
/****************************************************************************\
 Revision		july major revision of all product's.         (JB)
               	All operators calls the superclass
                  operator check's
                  	if similar types, storage etc  added/subtracted
                     if amount=0 the object gets the same unit, storage etc
                     	as the object that is added to it

 					new function AddAmount
\****************************************************************************/

#ifndef __PRODUCT_H
  #define __PRODUCT_H

#include "price.h"
#ifdef _STANDALONE
	#include "../base/common.h"
#else
	#include "../base/message.h"
#endif
#include <string.h>
class product: public base
{
protected:
	double amount;
	int mainUnitName;
	price* aPrice;
	int ObjType;

	int GetUnitType(string aMainUnitType);
	void InitVariables();

	// Copy Constructor
	product(const product& aproduct);
public:
	// Default Constructor
	product();
	// Constructor with arguments
	product(const char * aName, const int aIndex = -1, const base * aOwner =
			NULL);
	// Destructor
	virtual ~product(); // virtual to ensure correct deletion of derivered objects
	// Operator for output


	// Set functions
	void Setname(string aname)
	{
		strcpy(Name, aname.c_str());
	} //
	void Setamount(double aamount)
	{
		amount = aamount;
	}

	void SetmainUnitName(int amainUnitName)
	{
		mainUnitName = amainUnitName;
	}
	void SetaPrice(price* aaPrice)
	{
		aPrice = aaPrice;
	}
	void SetObjType(int aObjType)
	{
		ObjType = aObjType;
	}

	double GetAmount(void) const
	{
		return amount;
	}
	int GetmainUnitName(void) const
	{
		return mainUnitName;
	}
	string GetUnitString(int amainUnitName);
	price* GetaPrice(void) const
	{
		return aPrice;
	}
	int GetObjType(void) const
	{
		return ObjType;
	}

	// Other functions
	virtual string DerivedFrom() const; // Returns name of product that can be transformed to this class
	virtual product& operator=(const product& aProduct);
	virtual product& operator+(const product& aProduct);
	virtual product& operator-(const product& aProduct);
	virtual product& operator*(const double& aFactor);

	virtual void ReadParameters(fstream * file, bool criticalFile,const char * sectionName);


	virtual product* clone() const;
};

#endif

