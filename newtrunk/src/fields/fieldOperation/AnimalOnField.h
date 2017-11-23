
#ifndef __AnimalOnField_H
#define __AnimalOnField_H

#include "fieldOperationFields.h"
#include "../../products/manure.h"
#include "../../technics/animalFertTech.h"
#include "../field.h"
/*
 * 
 */
class AnimalonField :public fieldOperationFields

{
private:
	AnimalonField & operator = (
		const AnimalonField & f);    // Dissable the compilers generation of default assignment operator.
	string TypeOfAnimal;
	int numberOfAnimals;
public:
	AnimalonField();
	field * fieldForAnimals;
	virtual ~AnimalonField();

	AnimalonField(const AnimalonField & g);
	string getTypeOfAnimal(){ return TypeOfAnimal; }
	int getNumberOfAnimals(){ return numberOfAnimals; }
	virtual AnimalonField * clone() const;

	 void DefineAnimalonFieldOp(operationNames op,
		 int                                          yr,
		 int                                          mon,
		 int                                          d,
		 string                                       type,
		 int numberOfAnimals);    // , string applicMethod);
 } ;
#endif

