
#include "../../base/common.h"
#include "removeAnimalOnField.h"


void removeAnimalOnField::DefineAnimalonFieldOp(operationNames op,
	int                                          yr,
	int                                          mon,
	int                                          d,
	string                                       type,
	int amount)
{
	
	op_id = op;
	crop_id = "no rop";

	opTime->SetTime(d, mon, yr);
	TypeOfAnimal = type;
	NumberOfAnimalsRemoved=amount;
}


removeAnimalOnField::removeAnimalOnField() {

}

removeAnimalOnField::~removeAnimalOnField() {}

removeAnimalOnField::removeAnimalOnField(const removeAnimalOnField & g) :
fieldOperationFields(g)
{

}

removeAnimalOnField * removeAnimalOnField::clone() const {
	removeAnimalOnField * s = new removeAnimalOnField(*this);

	return s;
}