/*-------------------------------------*\
|
|  File Name: Mul.cpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified: Fri, Oct 19, 2012  9:19:00 AM
|
|  Created By: Darren Stahl
|
\*-------------------------------------*/

#include "Mul.hpp"

#include "../Processor.hpp"
#include "../ModrmOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Mul::Mul(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op)
{}

Instruction* Mul::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {

	Memory::MemoryOffset opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	std::string inst;
	Instruction* newMul = 0;

	Prefix* pre = Prefix::GetPrefix(memLoc);

	if(pre) {
		opLoc += preLen = pre->GetLength();
	}

	switch(*opLoc) {
		case MUL_MOD8:
		case MUL_MOD16:
		{
			unsigned int modrm = (*(opLoc + 1) & 0x38) >> 3;
			if(modrm == 4) {
				unsigned int size = (*opLoc == MUL_MOD8 ? 1 : 2);
				Operand* src = ModrmOperand::GetModrmOperand
					(proc, opLoc, ModrmOperand::MOD, size);
				snprintf(buf, 65, "MUL %s", src->GetDisasm().c_str());
				GETINST(preLen + 2 + src->GetBytecodeLen());
				newMul = new Mul(pre, buf, inst, (int)*opLoc);
				newMul->SetOperand(Operand::SRC, src);
				break;
			}
		}
	}
	return newMul;

}

int Mul::Execute(Processor* proc) {
	Operand* src = mOperands[Operand::SRC];

	if(!src) {
		return INVALID_ARGS;
	}

	unsigned int srcVal = src->GetValue();
	unsigned int dstVal;
	unsigned int newVal;
	eRegisters overflowReg;

	if (mOpcode == MUL_MOD8) {
		dstVal = proc->GetRegister(REG_AL);
		newVal = dstVal * srcVal;
		proc->SetRegister(REG_AX, newVal);
		overflowReg = REG_AH;
	}
	else {
		dstVal = proc->GetRegister(REG_AX);
		newVal = dstVal * srcVal;
		proc->SetRegister(REG_AX, newVal & 0xFFFF);
		proc->SetRegister(REG_DX, newVal >> 16 & 0xFFFF);
		overflowReg = REG_DX;
	}

	proc->SetFlag(FLAGS_CF, proc->GetRegister(overflowReg) != 0);
	proc->SetFlag(FLAGS_OF, proc->GetRegister(overflowReg) != 0);
	return 0;
}
