/*-------------------------------------*\
|
|  File Name: Out.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Out.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Out::Out(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Out::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newOut = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case OUT_IMM8_AL:
		case OUT_IMM8_AX:
		{
			Processor8086::eRegisters reg = *opLoc == OUT_IMM8_AL ? Processor8086::REG_AL : Processor8086::REG_AX;
			Operand* dst = new ImmediateOperand(*(opLoc + 1), 1, (opLoc + 1).getOffset());
			Operand* src = new RegisterOperand(reg, mProc);
			GETINST(preSize + 2);
			snprintf(buf, 65, "OUT %s, %s", dst->GetDisasm().c_str(), reg == Processor8086::REG_AX ? "AX" : "AL");

			newOut = new Out(pre, buf, inst, (int)*opLoc);
			newOut->SetProc(mProc);
			newOut->SetOperand(Operand::SRC, src);
			newOut->SetOperand(Operand::DST, dst);
			break;
		}
		case OUT_DX_AL:
		case OUT_DX_AX:
		{
			Processor8086::eRegisters reg = *opLoc == OUT_DX_AL ? Processor8086::REG_AL : Processor8086::REG_AX;
			Operand* dst = new RegisterOperand(Processor8086::REG_DX, mProc);
			Operand* src = new RegisterOperand(reg, mProc);
			GETINST(preSize + 1);
			snprintf(buf, 65, "OUT DX, %s", reg == Processor8086::REG_AX ? "AX" : "AL");
			newOut = new Out(pre, buf, inst, (int)*opLoc);
			newOut->SetProc(mProc);
			newOut->SetOperand(Operand::SRC, src);
			newOut->SetOperand(Operand::DST, dst);
			break;
		}
	}

	return newOut;

}

int Out::Execute() {
	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	if(!dst || !src) {
		return INVALID_ARGS;
	}

	if(mOpcode == OUT_IMM8_AL || mOpcode == OUT_DX_AL) {
		mProc->Outb(dst->GetValue(), src->GetValue());
		return Instruction8086::PERIPH_WRITE;
	} else if(mOpcode == OUT_IMM8_AX || mOpcode == OUT_DX_AX) {
		mProc->Outw(dst->GetValue(), src->GetValue());
		return Instruction8086::PERIPH_WRITE;
	}

	return INVALID_ARGS;
}
