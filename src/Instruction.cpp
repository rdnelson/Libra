/*-------------------------------------*\
|
|  File Name: Instruction.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Oct 11, 2012  3:35:38 PM
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


Instruction::Instruction() : mValid(false), mOpcode(-1), mInst(""), mText(""), modrm(0), mPrefix(0) {
	mOperands[0] = 0;
	mOperands[1] = 0;

}

Instruction::Instruction(Prefix* pre, std::string text, std::string inst, int op)
	: mValid(true), mOpcode(op), mInst(inst), mText(text), modrm(0), mPrefix(pre)
{
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

bool Instruction::Parity(unsigned int parity) {

	parity ^= parity >> 16;
	parity ^= parity >> 8;
	parity ^= parity >> 4;
	parity &= 0x0F;
	return (0x6996 >> parity) & 1;
}

//New mnemonics need to be added here to be registered
void Instruction::InitializeOpcodes() {

	OPCODE(Add);
	OPCODE(Mov);
	OPCODE(Jcc);
	OPCODE(Test);
	OPCODE(Call);
	OPCODE(Ret);
	OPCODE(Aam);
	OPCODE(Xor);
	OPCODE(Aaa);
	OPCODE(CLSTX);
	OPCODE(Aad);
	OPCODE(Aas);
	OPCODE(And);
}	
