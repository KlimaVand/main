
#include "../../base/common.h"
#include "AnimalOnField.h"


void AnimalonField::DefineAnimalonFieldOp(operationNames op,
	int                                          yr,
	int                                          mon,
	int                                          d,
	string                                       type,
	int aNumberOfAnimals)
{
	
	op_id = op;
	crop_id = "no rop"; 

	opTime->SetTime(d, mon, yr);
	TypeOfAnimal = type;
	numberOfAnimals = aNumberOfAnimals;
}


AnimalonField::AnimalonField() {

}

AnimalonField::~AnimalonField() {}

AnimalonField::AnimalonField(const AnimalonField & g) :
fieldOperationFields(g)
{

}

AnimalonField * AnimalonField::clone() const {
	AnimalonField * s = new AnimalonField(*this);

	return s;
}