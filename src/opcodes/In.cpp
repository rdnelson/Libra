/*-------------------------------------*\
|
|  File Name: In.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "In.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

In::In(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* In::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newIn = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case IN_AL_IMM8:
		case IN_AX_IMM8:
		{
			Processor8086::eRegisters reg = *opLoc == IN_AL_IMM8 ? Processor8086::REG_AL : Processor8086::REG_AX;
			Operand* src = new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset());
			Operand* dst = new RegisterOperand(reg, mProc);
			GETINST(preSize + 2);
			snprintf(buf, 65, "IN %s, %s",reg == Processor8086::REG_AX ? "AX" : "AL", src->GetDisasm().c_str());

			newIn = new In(pre, buf, inst, (int)*opLoc);
			newIn->SetProc(mProc);
			newIn->SetOperand(Operand::SRC, src);
			newIn->SetOperand(Operand::DST, dst);
			break;
		}
		case IN_AL_DX:
		case IN_AX_DX:
		{
			Processor8086::eRegisters reg = *opLoc == IN_AL_DX ? Processor8086::REG_AL : Processor8086::REG_AX;
			Operand* src = new RegisterOperand(Processor8086::REG_DX, mProc);
			Operand* dst = new RegisterOperand(reg, mProc);
			GETINST(preSize + 1);
			snprintf(buf, 65, "IN %s, DX", reg == Processor8086::REG_AX ? "AX" : "AL");
			newIn = new In(pre, buf, inst, (int)*opLoc);
			newIn->SetProc(mProc);
			newIn->SetOperand(Operand::SRC, src);
			newIn->SetOperand(Operand::DST, dst);
			break;
		}
	}

	return newIn;

}

int In::Execute() {
	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	if(!dst || !src) {
		return INVALID_ARGS;
	}

	if(mOpcode == IN_AL_IMM8 || mOpcode == IN_AL_DX) {
		dst->SetValue(mProc->Inb(src->GetValue()));
		return 0;
	} else if(mOpcode == IN_AX_IMM8 || mOpcode == IN_AX_DX) {
		dst->SetValue(mProc->Inw(src->GetValue()));
		return 0;
	}

	return INVALID_ARGS;
}
