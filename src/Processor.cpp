/*-------------------------------------*\
|
|  File Name: Processor.cpp
|
|  Creation Date: 26-09-2012
|
|  Last Modified: Wed, Sep 26, 2012 10:40:34 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Processor.hpp"

#define ADD_REG(reg, val)	mRegisters[reg].addValue(val)
#define SET_REG(reg, val)	mRegisters[reg].setValue(val)
#define GET_REG(reg) 		mRegisters[reg].getValue()

Processor::Processor(char* mem) mMem(mem) {

}

int Processor::Initialize(unsigned int startAddr) {


	if(startAddr >= 0x10000) {
		return PROC_ERR_INV_ADDR;
	}

	SET_REG(REG_IP, startAddr);

}

int Processor::Step() {

	Instruction* inst = Instruction::ReadInstruction(GET_REG(REG_IP));
	if(inst && inst->IsValid()) {
		ADD_REG(REG_IP, inst->GetLength());
		inst->Execute(this);

	} else {
		return PROC_INVALID_INSTRUCTION;
	}

}
