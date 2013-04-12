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

Pop::Pop(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Pop::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newPop = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case POP_MOD16:
		{
			if((unsigned int)((*(opLoc + 1) & 0x38) >> 3) != POP_SUB_OPCODE)
				return newPop;

			Operand* dst = ModrmOperand::GetModrmOperand(mProc, opLoc, ModrmOperand::MOD, 2);
			snprintf(buf, 65, "POP %s", dst->GetDisasm().c_str());
			GETINST(preSize + 2 + dst->GetBytecodeLen());
			newPop = new Pop(pre, buf, inst, (unsigned char)*opLoc);
			newPop->SetProc(mProc);
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
			Operand* dst = new RegisterOperand((Processor8086::eRegisters)(*opLoc - POP_REG_AX + Processor8086::REG_AX),
				       mProc);
			snprintf(buf, 65, "POP %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());
			newPop = new Pop(pre, buf, inst, (unsigned char)*opLoc);
			newPop->SetProc(mProc);
			newPop->SetOperand(Operand::DST, dst);
			break;
		}
		case POP_SS:
		case POP_DS:
		case POP_ES:
		{
			Processor8086::eRegisters reg = Processor8086::REG_DS;
			if(*opLoc == POP_DS)
				reg = Processor8086::REG_DS;
			else if(*opLoc == POP_SS)
				reg = Processor8086::REG_SS;
			else if(*opLoc == POP_ES)
				reg = Processor8086::REG_ES;

			Operand* dst = new RegisterOperand(reg, mProc);
			snprintf(buf, 65, "POP %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1 + dst->GetBytecodeLen());
			newPop = new Pop(pre, buf, inst, (unsigned char)*opLoc);
			newPop->SetProc(mProc);
			newPop->SetOperand(Operand::DST, dst);
			break;
		}
		case POPF:
		{
			Operand* dst = new RegisterOperand(Processor8086::REG_FLAGS, mProc);
			snprintf(buf, 65, "POPF");
			GETINST(preSize + 1);
			newPop = new Pop(pre, buf, inst, (unsigned char)*opLoc);
			newPop->SetProc(mProc);
			newPop->SetOperand(Operand::DST, dst);
			break;
		}
		case POPA:
			snprintf(buf, 65, "POPA");
			GETINST(preSize + 1);
			newPop = new Pop(pre, buf, inst, (unsigned char)*opLoc);
			newPop->SetProc(mProc);
			break;

	}
	return newPop;

}

int Pop::Execute() {

	if(mOpcode == POPA) {
		mProc->PopRegister(Processor8086::REG_DI);
		mProc->PopRegister(Processor8086::REG_SI);
		mProc->PopRegister(Processor8086::REG_BP);
		mProc->PopSize(2);
		mProc->PopRegister(Processor8086::REG_BX);
		mProc->PopRegister(Processor8086::REG_DX);
		mProc->PopRegister(Processor8086::REG_CX);
		mProc->PopRegister(Processor8086::REG_AX);

	} else {

		Operand* dst = mOperands[Operand::DST];
		if(!dst) {
			return INVALID_ARGS;
		}

		dst->SetValue(mProc->PopValue());
	}
	return 0;
}
