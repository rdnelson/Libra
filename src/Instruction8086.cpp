/*-------------------------------------*\
|
|  File Name: Instruction8086.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Oct 18, 2012 11:02:28 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Instruction8086.hpp"

#include "opcodes/AllOpcodes.hpp"
#include "Processor.hpp"

#ifdef UNIT_TEST
#define OPCODE(op)
#else
#define OPCODE(op) Instruction8086::AllInstructions.push_back(&op::CreateInstruction)
#endif

unsigned int Instruction8086::NumOpcodes = 0;


class Processor;

//Vector to contain the create functions for all mnemonics
Instruction8086::PCreateInst Instruction8086::AllInstructions8086[0x100][9];

Instruction8086::Instruction8086() : mProc(0), mValid(false), mOpcode(-1), mInst(""), mText(""), modrm(0), mPrefix(0) {
	mOperands[0] = 0;
	mOperands[1] = 0;
	mOperands[2] = 0;
	mOperands[3] = 0;

}

Instruction8086::Instruction8086(Prefix* pre, std::string text, std::string inst, int op)
	: mValid(true), mOpcode(op), mInst(inst), mText(text), modrm(0), mPrefix(pre)
{
	mOperands[0] = 0;
	mOperands[1] = 0;
	mOperands[2] = 0;
	mOperands[3] = 0;
}

Instruction8086::~Instruction8086() {
	if(mOperands[0])
		delete mOperands[0];
	if(mOperands[1])
		delete mOperands[1];
	delete mOperands[2];
	delete mOperands[3];
}

//Try to build each mnemonic until one succeeds
//If No mnemonic succeeds, opcode is not implemented
Instruction* Instruction8086::ReadInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Instruction* instr = NULL;

	bool memLog = memLoc.IsMemReadLogEnabled();
	memLoc.DisableMemReadLog();

	Prefix* pre = Prefix::GetPrefix(memLoc);
	if(pre == 0 && AllInstructions8086[*memLoc][0]) {
		instr = AllInstructions8086[*memLoc][0](memLoc, proc);
	} else if(pre && AllInstructions8086[*(memLoc + pre->GetLength())][0]) {
		instr = AllInstructions8086[*(memLoc + pre->GetLength())][0](memLoc, proc);
	}
	delete pre;
	if(memLog)
		memLoc.EnableMemReadLog();
	return instr;
}

Instruction* Instruction8086::CreateSubcodeInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Instruction* instr = NULL;
	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int offset = 0;
	if(pre) {
		offset = pre->GetLength();
	}
	delete pre;

	unsigned int subcodeIndex = ((*(memLoc + offset + 1) & 0x38) >> 3) + 1;

	if(AllInstructions8086[*(memLoc + offset)][subcodeIndex]) {
		instr = AllInstructions8086[*(memLoc + offset)][subcodeIndex](memLoc, proc);
	}
	return instr;
}

void Instruction8086::SetOperand(const unsigned int opcode, Operand* newOp) {
	if(opcode >= 4)
		return;
	mOperands[opcode] = newOp;
}

bool Instruction8086::Parity(unsigned int parity) {

	parity &= 0xFF;
	parity ^= parity >> 4;
	parity &= 0x0F;
	return (0x9669 >> parity) & 1;
}

bool Instruction8086::OverflowSub(unsigned int dst, unsigned int src, unsigned int size) {

	return OverflowAdd(dst, ~src + 1, size);
}

bool Instruction8086::OverflowAdd(unsigned int dst, unsigned int src, unsigned int size) {

	unsigned int msb = 1 << (8 * size - 1);
	return (((src ^ dst) ^ msb) & ((src + dst) ^ src) & msb) != 0;
}

bool Instruction8086::AdjustAdd(unsigned int op1, unsigned int op2) {
	return (((op1 & 0x0F) + (op2 & 0x0F)) & ~0x0F) != 0;
}

bool Instruction8086::AdjustSub(unsigned int op1, unsigned int op2) {
	return (((op1 & 0x0F) - (op2 & 0x0F)) & ~0x0F) != 0;
}

