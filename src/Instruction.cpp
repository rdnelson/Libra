/*-------------------------------------*\
|
|  File Name: Instruction.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Wed, Sep 26, 2012 10:18:25 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Instruction.hpp"

#include "opcodes/AllOpcodes.hpp"

#define OPCODE(op) Instruction::AllInstructions.push_back(&op::CreateInstruction)

std::vector<Instruction::PCreateInst> Instruction::AllInstructions;

Instruction::Instruction() : mValid(false), mOpcode(-1), mInst("") {

	OPCODE(Add);
	
}

Instruction* Instruction::ReadInstruction(char* memLoc) {

	return 0;
}
