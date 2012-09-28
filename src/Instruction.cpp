/*-------------------------------------*\
|
|  File Name: Instruction.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Fri, Sep 28, 2012 10:35:26 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Instruction.hpp"

#include "opcodes/AllOpcodes.hpp"

#define OPCODE(op) Instruction::AllInstructions.push_back(&op::CreateInstruction)

//Vector to contain the create functions for all mnemonics
std::vector<Instruction::PCreateInst> Instruction::AllInstructions;


Instruction::Instruction() : mValid(false), mOpcode(-1), mInst(""), mText("") {

}

//Try to build each mnemonic until one succeeds
//If No mnemonic succeeds, opcode is not implemented
Instruction* Instruction::ReadInstruction(unsigned char* memLoc) {
	Instruction* instr = NULL;

	for(unsigned int i = 0; i < AllInstructions.size(); i++) {
		if((instr = AllInstructions[i](memLoc)) != NULL) {
			break;
		}
	}
	return instr;
}

//New mnemonics need to be added here to be registered
void Instruction::InitializeOpcodes() {

	OPCODE(Add);
}	
