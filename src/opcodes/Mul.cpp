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

#include "../Processor8086.hpp"
#include "../ModrmOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Mul::Mul(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op)
{}

Instruction* Mul::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Memory::MemoryOffset opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	std::string inst;
	Instruction8086* newMul = 0;

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
					(mProc, opLoc, ModrmOperand::MOD, size);
				snprintf(buf, 65, "MUL %s", src->GetDisasm().c_str());
				GETINST(preLen + 2 + src->GetBytecodeLen());
				newMul = new Mul(pre, buf, inst, (int)*opLoc);
				newMul->SetProc(mProc);
				newMul->SetOperand(Operand::SRC, src);
				break;
			}
		}
	}
	return newMul;

}

int Mul::Execute() {
	Operand* src = mOperands[Operand::SRC];

	if(!src) {
		return INVALID_ARGS;
	}

	unsigned int srcVal = src->GetValue();
	unsigned int dstVal;
	unsigned int newVal;
	Processor8086::eRegisters overflowReg;

	if (mOpcode == MUL_MOD8) {
		dstVal = mProc->GetRegister(Processor8086::REG_AL);
		newVal = dstVal * srcVal;
		mProc->SetRegister(Processor8086::REG_AX, newVal);
		overflowReg = Processor8086::REG_AH;
	}
	else {
		dstVal = mProc->GetRegister(Processor8086::REG_AX);
		newVal = dstVal * srcVal;
		mProc->SetRegister(Processor8086::REG_AX, newVal & 0xFFFF);
		mProc->SetRegister(Processor8086::REG_DX, newVal >> 16 & 0xFFFF);
		overflowReg = Processor8086::REG_DX;
	}

	mProc->SetFlag(Processor8086::FLAGS_CF, mProc->GetRegister(overflowReg) != 0);
	mProc->SetFlag(Processor8086::FLAGS_OF, mProc->GetRegister(overflowReg) != 0);
	return 0;
}
