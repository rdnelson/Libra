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
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

In::In(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* In::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newIn = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case IN_AL_IMM8:
		case IN_AX_IMM8:
		{
			eRegisters reg = *opLoc == IN_AL_IMM8 ? REG_AL : REG_AX;
			Operand* src = new ImmediateOperand(*(opLoc + 1), 1);
			Operand* dst = new RegisterOperand(reg, proc);
			GETINST(preSize + 2);
			snprintf(buf, 65, "IN %s, %s",reg == REG_AX ? "AX" : "AL", src->GetDisasm().c_str());

			newIn = new In(pre, buf, inst, (int)*opLoc);
			newIn->SetOperand(Operand::SRC, src);
			newIn->SetOperand(Operand::DST, dst);
			break;
		}
		case IN_AL_DX:
		case IN_AX_DX:
		{
			eRegisters reg = *opLoc == IN_AL_DX ? REG_AL : REG_AX;
			Operand* src = new RegisterOperand(REG_DX, proc);
			Operand* dst = new RegisterOperand(reg, proc);
			GETINST(preSize + 1);
			snprintf(buf, 65, "IN %s, DX", reg == REG_AX ? "AX" : "AL");
			newIn = new In(pre, buf, inst, (int)*opLoc);
			newIn->SetOperand(Operand::SRC, src);
			newIn->SetOperand(Operand::DST, dst);
			break;
		}
	}

	return newIn;

}

int In::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	if(!dst || !src) {
		return INVALID_ARGS;
	}

	if(mOpcode == IN_AL_IMM8 || mOpcode == IN_AL_DX) {
		dst->SetValue(proc->Inb(src->GetValue()));
		return 0;
	} else if(mOpcode == IN_AX_IMM8 || mOpcode == IN_AX_DX) {
		dst->SetValue(proc->Inw(src->GetValue()));
		return 0;
	}

	return INVALID_ARGS;
}
