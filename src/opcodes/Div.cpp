/*-------------------------------------*\
|
|  File Name: Div.cpp
|
|  Creation Date: 12-10-2012
|
|  Last Modified: Thu, Oct 18, 2012  9:55:02 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Div.hpp"

#include "../Processor8086.hpp"
#include "../ModrmOperand.hpp"

#include <cstdio>

Div::Div(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Div::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	std::string inst;
	Prefix* pre = Prefix::GetPrefix(memLoc);
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	if(pre) {
		opLoc += preLen = pre->GetLength();
	}

	Instruction8086* newDiv = 0;

	if((*opLoc == DIV_MOD8 || *opLoc == DIV_MOD16) &&
			((unsigned int)((*(opLoc + 1) & 0x38) >> 3) == DIV_SUB_OPCODE)) {
		unsigned int size = *opLoc == DIV_MOD8 ? 1 : 2;
		Operand* dst = ModrmOperand::GetModrmOperand(mProc, opLoc, ModrmOperand::MOD, size);
		snprintf(buf, 65, "DIV %s", dst->GetDisasm().c_str());
		GETINST(preLen + 2 + dst->GetBytecodeLen());
		newDiv = new Div(pre, buf, inst, (int)*opLoc);
		newDiv->SetProc(mProc);
		newDiv->SetOperand(Operand::DST, dst);
	}
	return newDiv;

}

int Div::Execute() {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0)
		return INVALID_ARGS;

	if(dst->GetValue() == 0) {
		return DIV_BY_ZERO;
	}

	if(mOpcode == DIV_MOD8) {
		unsigned int val = mProc->GetRegister(Processor8086::REG_AX) / dst->GetValue();
		unsigned int rem = mProc->GetRegister(Processor8086::REG_AX) % dst->GetValue();
		mProc->SetRegister(Processor8086::REG_AL, val);
		mProc->SetRegister(Processor8086::REG_AH, rem);

	} else if (mOpcode == DIV_MOD16) {
		unsigned int div = mProc->GetRegister(Processor8086::REG_AX) + (mProc->GetRegister(Processor8086::REG_DX) << 0x10);
		unsigned int val = div / dst->GetValue();
		unsigned int rem = div % dst->GetValue();
		mProc->SetRegister(Processor8086::REG_AX, val);
		mProc->SetRegister(Processor8086::REG_DX, rem);
	} else {
		return INVALID_ARGS;
	}

	return 0;
}
