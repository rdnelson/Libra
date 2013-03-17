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
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Out::Out(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Out::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newOut = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case OUT_IMM8_AL:
		case OUT_IMM8_AX:
		{
			eRegisters reg = *opLoc == OUT_IMM8_AL ? REG_AL : REG_AX;
			Operand* dst = new ImmediateOperand(*(opLoc + 1), 1);
			Operand* src = new RegisterOperand(reg, proc);
			GETINST(preSize + 2);
			snprintf(buf, 65, "OUT %s, %s", dst->GetDisasm().c_str(), reg == REG_AX ? "AX" : "AL");

			newOut = new Out(pre, buf, inst, (int)*opLoc);
			newOut->SetOperand(Operand::SRC, src);
			newOut->SetOperand(Operand::DST, dst);
			break;
		}
		case OUT_DX_AL:
		case OUT_DX_AX:
		{
			eRegisters reg = *opLoc == OUT_DX_AL ? REG_AL : REG_AX;
			Operand* dst = new RegisterOperand(REG_DX, proc);
			Operand* src = new RegisterOperand(reg, proc);
			GETINST(preSize + 1);
			snprintf(buf, 65, "OUT DX, %s", reg == REG_AX ? "AX" : "AL");
			newOut = new Out(pre, buf, inst, (int)*opLoc);
			newOut->SetOperand(Operand::SRC, src);
			newOut->SetOperand(Operand::DST, dst);
			break;
		}
	}

	return newOut;

}

int Out::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	if(!dst || !src) {
		return INVALID_ARGS;
	}

	if(mOpcode == OUT_IMM8_AL || mOpcode == OUT_DX_AL) {
		proc->Outb(dst->GetValue(), src->GetValue());
		return Instruction::PERIPH_WRITE;
	} else if(mOpcode == OUT_IMM8_AX || mOpcode == OUT_DX_AX) {
		proc->Outw(dst->GetValue(), src->GetValue());
		return Instruction::PERIPH_WRITE;
	}

	return INVALID_ARGS;
}
