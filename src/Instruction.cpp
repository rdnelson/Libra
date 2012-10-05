/*-------------------------------------*\
|
|  File Name: Instruction.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Wed, Oct  3, 2012  4:08:11 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Instruction.hpp"

#include "opcodes/AllOpcodes.hpp"

#define OPCODE(op) Instruction::AllInstructions.push_back(&op::CreateInstruction)

class Processor;

//Vector to contain the create functions for all mnemonics
std::vector<Instruction::PCreateInst> Instruction::AllInstructions;


Instruction::Instruction() : mValid(false), mOpcode(-1), mInst(""), mText(""), modrm(0) {
	mOperands[0] = 0;
	mOperands[1] = 0;

}

Instruction::~Instruction() {
	if(mOperands[0])
		delete mOperands[0];
	if(mOperands[1])
		delete mOperands[1];
}

//Try to build each mnemonic until one succeeds
//If No mnemonic succeeds, opcode is not implemented
Instruction* Instruction::ReadInstruction(unsigned char* memLoc, Processor* proc) {
	Instruction* instr = NULL;

	for(unsigned int i = 0; i < AllInstructions.size(); i++) {
		if((instr = AllInstructions[i](memLoc, proc)) != NULL) {
			break;
		}
	}
	return instr;
}

void Instruction::SetOperand(const unsigned int opcode, Operand* newOp) {
	switch(opcode) {
		case Operand::SRC:
			mOperands[opcode] = newOp;
			break;
		case Operand::DST:
			mOperands[opcode] = newOp;
			break;
		default:
			break;
	}
}

//New mnemonics need to be added here to be registered
void Instruction::InitializeOpcodes() {

	OPCODE(Add);
	OPCODE(Mov);
}	
