
#ifndef __removeAnimalOnField_H
#define __removeAnimalOnField_H

#include "fieldOperationFields.h"
#include "../../products/manure.h"
#include "../../technics/animalFertTech.h"

/*
 * 
 */
class removeAnimalOnField :public fieldOperationFields

{
private:
	removeAnimalOnField & operator = (
		const removeAnimalOnField & f);    // Dissable the compilers generation of default assignment operator.
	string TypeOfAnimal;
	int NumberOfAnimalsRemoved;
public:
	removeAnimalOnField();

	virtual ~removeAnimalOnField();
public:
	int  getNumberOfAnimalsRemoved(){
		return NumberOfAnimalsRemoved;
	}
	removeAnimalOnField(const removeAnimalOnField & g);
	string getTypeOfAnimal(){ return TypeOfAnimal; }
	virtual removeAnimalOnField * clone() const;

	 void DefineAnimalonFieldOp(operationNames op,
		 int                                          yr,
		 int                                          mon,
		 int                                          d,
		 string                                       type,
		 int	amount);    // , string applicMethod);
 } ;
#endif

