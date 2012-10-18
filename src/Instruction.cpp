/*-------------------------------------*\
|
|  File Name: Instruction.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Tue, Oct 16, 2012  1:27:06 PM
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

	parity &= 0xFF;
	parity ^= parity >> 4;
	parity &= 0x0F;
	return (0x9669 >> parity) & 1;
}

bool Instruction::OverflowSub(unsigned int val, unsigned int dst, unsigned int src, unsigned int size) {

	unsigned int msb = 1 << (8 * size - 1);

	if ((src & msb != 0) ? ~src + 1 : src > (dst & msb != 0) ? dst + 1 : dst) {
		if(((dst & msb) != 0) && ((src & msb) != 0) && !((val & msb) != 0)) {
			return true;
		}
		if(!((dst & msb) != 0) && !((src & msb) != 0) && ((val & msb) != 0)) {
			return true;
		}
	} else if ((src & msb != 0) ? ~src + 1 : src < (dst & msb != 0) ? dst + 1 : dst) {
		if(((dst & msb) != 0) && ((src & msb) != 0) && ((val & msb) != 0)) {
			return true;
		}
		if(!((dst & msb) != 0) && !((src & msb) != 0) && !((val & msb) != 0)) {
			return true;
		}
	}
	if(((dst & msb) != 0) && !((src & msb) != 0) && !((val & msb) != 0)) {
		return true;
	}
	if(!((dst & msb) != 0) && ((src & msb) != 0) && ((val & msb) != 0)) {
		return true;
	}
	return false;
}

bool Instruction::OverflowAdd(unsigned int val, unsigned int dst, unsigned int src, unsigned int size) {

	unsigned int msb = 1 << (8 * size - 1);
	if(~(dst & msb) && ~(src & msb) && (val & msb)) {
		return true;
	}
	if((dst & msb) && (src & msb) && ~(val & msb)) {
		return true;
	}
	return false;
}

bool Instruction::AdjustAdd(unsigned int op1, unsigned int op2) {
	return (((op1 & 0x0F) + (op2 & 0x0F)) & ~0x0F) != 0;
}

bool Instruction::AdjustSub(unsigned int op1, unsigned int op2) {
	return (((op1 & 0x0F) - (op2 & 0x0F)) & ~0x0F) != 0;
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
	OPCODE(Cmp);
	OPCODE(And);
	OPCODE(Adc);
	OPCODE(Cbw);
	OPCODE(CmpsX);
	OPCODE(Cwd);
	OPCODE(IncDec);
	OPCODE(Div);
	OPCODE(Jmp);
	OPCODE(IDiv);
	OPCODE(Sub);
}	
