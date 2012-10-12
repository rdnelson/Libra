/*-------------------------------------*\
|
|  File Name: Div.cpp
|
|  Creation Date: 12-10-2012
|
|  Last Modified: Fri, Oct 12, 2012  5:26:32 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Div.hpp"

#include "../Processor.hpp"
#include "../ModrmOperand.hpp"

#include <cstdio>

Div::Div(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Div::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	int preLen = 0;
	char buf[65];
	std::string inst;
	Prefix* pre = Prefix::GetPrefix(memLoc);

	if(pre) {
		opLoc += preLen = pre->GetLength();
	}

	Instruction* newDiv = 0;

	if((*opLoc == DIV_MOD8 || *opLoc == DIV_MOD16) && 
			(((*(opLoc + 1) & 0x38) >> 3) == DIV_SUB_OPCODE)) {
		unsigned int size = *opLoc == DIV_MOD8 ? 1 : 2;
		Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);
		snprintf(buf, 65, "DIV %s", "");
		GETINST(preLen + 2 + dst->GetBytecodeLen());
		newDiv = new Div(pre, buf, inst, (int)*opLoc);
		newDiv->SetOperand(Operand::DST, dst);
	}
	return newDiv;

}

int Div::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	if(dst == 0)
		return -1;

	if(dst->GetValue() == 0) {
		return DIV_BY_ZERO;
	}

	if(mOpcode == DIV_MOD8) {
		unsigned int val = proc->GetRegister(REG_AX) / dst->GetValue();
		unsigned int rem = proc->GetRegister(REG_AX) % dst->GetValue();
		proc->SetRegister(REG_AL, val);
		proc->SetRegister(REG_AH, rem);

	} else if (mOpcode == DIV_MOD16) {
		unsigned int div = proc->GetRegister(REG_AX) + (proc->GetRegister(REG_DX) << 0x10);
		unsigned int val = div / dst->GetValue();
		unsigned int rem = div % dst->GetValue();
		proc->SetRegister(REG_AX, val);
		proc->SetRegister(REG_DX, rem);
	} else {
		return -1;
	}

	return 0;
}
