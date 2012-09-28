/*-------------------------------------*\
|
|  File Name: Instruction.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Sep 27, 2012  4:24:12 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Instruction.hpp"

#include "opcodes/AllOpcodes.hpp"

#define OPCODE(op) Instruction::AllInstructions.push_back(&op::CreateInstruction)

std::vector<Instruction::PCreateInst> Instruction::AllInstructions;

Instruction::Instruction() : mValid(false), mOpcode(-1), mInst(""), mText("") {

}

Instruction* Instruction::ReadInstruction(unsigned char* memLoc) {
	Instruction* instr = NULL;

	for(unsigned int i = 0; i < AllInstructions.size(); i++) {
		if((instr = AllInstructions[i](memLoc)) != NULL) {
			break;
		}
	}
	return instr;
}

void Instruction::InitializeOpcodes() {

	OPCODE(Add);
}	
