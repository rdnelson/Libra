/*-------------------------------------*\
|
|  File Name: Instruction.cpp
|
|  Creation Date: 25-09-2012
|
|  Last Modified: Thu, Oct 18, 2012 11:02:28 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Instruction.hpp"

#include "opcodes/AllOpcodes.hpp"
#include "Processor.hpp"

#ifdef UNIT_TEST
#define OPCODE(op)
#else
#define OPCODE(op) Instruction::AllInstructions.push_back(&op::CreateInstruction)
#endif

unsigned int Instruction::NumOpcodes = 0;


class Processor;

//Vector to contain the create functions for all mnemonics
std::vector<Instruction::PCreateInst> Instruction::AllInstructions;


Instruction::Instruction() : mValid(false), mOpcode(-1), mInst(""), mText(""), modrm(0), mPrefix(0) {
	mOperands[0] = 0;
	mOperands[1] = 0;
	mOperands[2] = 0;
	mOperands[3] = 0;

}

Instruction::Instruction(Prefix* pre, std::string text, std::string inst, int op)
	: mValid(true), mOpcode(op), mInst(inst), mText(text), modrm(0), mPrefix(pre)
{
	mOperands[0] = 0;
	mOperands[1] = 0;
	mOperands[2] = 0;
	mOperands[3] = 0;
}

Instruction::~Instruction() {
	if(mOperands[0])
		delete mOperands[0];
	if(mOperands[1])
		delete mOperands[1];
	delete mOperands[2];
	delete mOperands[3];
}

//Try to build each mnemonic until one succeeds
//If No mnemonic succeeds, opcode is not implemented
Instruction* Instruction::ReadInstruction(unsigned char* memLoc, Processor* proc) {
	Instruction* instr = NULL;

	for(unsigned int i = 0; i < NumOpcodes; i++) {
		if((instr = AllInstructions[i](memLoc, proc)) != NULL) {
            instr->SetAddress(proc->GetRegister(REG_IP));
			break;
		}
	}
	return instr;
}

void Instruction::SetOperand(const unsigned int opcode, Operand* newOp) {
	if(opcode >= 4) 
		return;
	mOperands[opcode] = newOp;
}

bool Instruction::Parity(unsigned int parity) {

	parity &= 0xFF;
	parity ^= parity >> 4;
	parity &= 0x0F;
	return (0x9669 >> parity) & 1;
}

bool Instruction::OverflowSub(unsigned int dst, unsigned int src, unsigned int size) {

	return OverflowAdd(dst, ~src + 1, size);
}

bool Instruction::OverflowAdd(unsigned int dst, unsigned int src, unsigned int size) {

	unsigned int msb = 1 << (8 * size - 1);
	return (((src ^ dst) ^ msb) & ((src + dst) ^ src) & msb) != 0;
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
	OPCODE(IMul);
	OPCODE(Sub);
	OPCODE(Push);
	OPCODE(Neg);
	OPCODE(Not);
	OPCODE(Or);
	OPCODE(Pop);
	OPCODE(Lahf);
	OPCODE(Mul);
	OPCODE(Sahf);
	OPCODE(Sxx);
	OPCODE(Lxs);
	OPCODE(Lea);
	OPCODE(Out);
	OPCODE(In);

	NumOpcodes = AllInstructions.size();
}	
