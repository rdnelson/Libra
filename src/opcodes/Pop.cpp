/*-------------------------------------*\
|
|  File Name: Pop.cpp
|
|  Creation Date: 18-10-2012
|
|  Last Modified: Thu, Oct 18, 2012 10:38:51 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Pop.hpp"

#include "../ModrmOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>
#include <string>

Pop::Pop(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Pop::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {

	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newPop = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case POP_MOD16:
		{
			if(((*(opLoc + 1) & 0x38) >> 3) != POP_SUB_OPCODE)
				return newPop;

			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, 2);
			snprintf(buf, 65, "POP %s", dst->GetDisasm().c_str());
			GETINST(preSize + 2 + dst->GetBytecodeLen());
			newPop = new Pop(pre, buf, inst, (unsigned char)*opLoc);
			newPop->SetOperand(Operand::DST, dst);
			break;
		}
		case POP_REG_AX:
		case POP_REG_CX:
		case POP_REG_DX:
		case POP_REG_BX:
		case POP_REG_SP:
		case POP_REG_BP:
		case POP_REG_SI:
		case POP_REG_DI:
		{
			Operand* dst = new RegisterOperand((eRegisters)(*opLoc - POP_REG_AX + REG_AX),
				       proc);
			snprintf(buf, 65, "POP %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());
			newPop = new Pop(pre, buf, inst, (unsigned char)*opLoc);
			newPop->SetOperand(Operand::DST, dst);
			break;
		}
		case POP_SS:
		case POP_DS:
		case POP_ES:
		{
			eRegisters reg = REG_DS;
			if(*opLoc == POP_DS)
				reg = REG_DS;
			else if(*opLoc == POP_SS)
				reg = REG_SS;
			else if(*opLoc == POP_ES)
				reg = REG_ES;

			Operand* dst = new RegisterOperand(reg, proc);
			snprintf(buf, 65, "POP %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());
			newPop = new Pop(pre, buf, inst, (unsigned char)*opLoc);
			newPop->SetOperand(Operand::DST, dst);
			break;
		}
		case POPF:
		{
			Operand* dst = new RegisterOperand(REG_FLAGS, proc);
			snprintf(buf, 65, "POP %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());
			newPop = new Pop(pre, buf, inst, (unsigned char)*opLoc);
			newPop->SetOperand(Operand::DST, dst);
			break;
		}

	}
	return newPop;

}

int Pop::Execute(Processor* proc) {

	Operand* dst = mOperands[Operand::DST];
	if(!dst) {
		return INVALID_ARGS;
	}

	dst->SetValue(proc->PopValue());
	return 0;
}
