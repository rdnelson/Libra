/*-------------------------------------*\
|
|  File Name: Push.cpp
|
|  Creation Date: 18-10-2012
|
|  Last Modified: Thu, Oct 18, 2012 10:38:51 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Push.hpp"

#include "../ModrmOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>
#include <string>

Push::Push(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Push::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {

	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newPush = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case PUSH_MOD16:
		{
			if((unsigned int)((*(opLoc + 1) & 0x38) >> 3) != PUSH_SUB_OPCODE)
				return newPush;
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, 2);
			snprintf(buf, 65, "PUSH %s", dst->GetDisasm().c_str());
			GETINST(preSize + 2 + dst->GetBytecodeLen());
			newPush = new Push(pre, buf, inst, (unsigned char)*opLoc);
			newPush->SetOperand(Operand::DST, dst);
			break;
		}
		case PUSH_REG_AX:
		case PUSH_REG_CX:
		case PUSH_REG_DX:
		case PUSH_REG_BX:
		case PUSH_REG_SP:
		case PUSH_REG_BP:
		case PUSH_REG_SI:
		case PUSH_REG_DI:
		{
			Operand* dst = new RegisterOperand((eRegisters)(*opLoc - PUSH_REG_AX + REG_AX),
				       proc);
			snprintf(buf, 65, "PUSH %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());
			newPush = new Push(pre, buf, inst, (unsigned char)*opLoc);
			newPush->SetOperand(Operand::DST, dst);
			break;
		}
		case PUSH_IMM8:
		case PUSH_IMM16:
		{
			unsigned int val = *(opLoc + 1);
			unsigned int size = *opLoc == PUSH_IMM8 ? 1 : 2;

			if(size == 2) {
				val += *(opLoc + 2) << 0x8;
			} else {
				val += (val >= 0x80) ? 0xFF00 : 0x0000;
			}

			Operand* dst = new ImmediateOperand(val, 2);
			snprintf(buf, 65, "PUSH %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + size);
			newPush = new Push(pre, buf, inst, (unsigned char)*opLoc);
			newPush->SetOperand(Operand::DST, dst);
			break;
		}
		case PUSH_CS:
		case PUSH_SS:
		case PUSH_DS:
		case PUSH_ES:
		{
			eRegisters reg = REG_CS;
			if(*opLoc == PUSH_CS)
				reg = REG_CS;
			else if(*opLoc == PUSH_DS)
				reg = REG_DS;
			else if(*opLoc == PUSH_SS)
				reg = REG_SS;
			else if(*opLoc == PUSH_ES)
				reg = REG_ES;

			Operand* dst = new RegisterOperand(reg, proc);
			snprintf(buf, 65, "PUSH %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());
			newPush = new Push(pre, buf, inst, (unsigned char)*opLoc);
			newPush->SetOperand(Operand::DST, dst);
			break;
		}
		case PUSHF:
		{
			Operand* dst = new RegisterOperand(REG_FLAGS, proc);
			snprintf(buf, 65, "PUSH %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());
			newPush = new Push(pre, buf, inst, (unsigned char)*opLoc);
			newPush->SetOperand(Operand::DST, dst);
			break;
		}

	}
	return newPush;

}

int Push::Execute(Processor* proc) {

	Operand* dst = mOperands[Operand::DST];
	if(!dst) {
		return INVALID_ARGS;
	}

	proc->PushValue(dst->GetValue());
	return 0;
}
