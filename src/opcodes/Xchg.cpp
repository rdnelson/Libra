/*-------------------------------------*\
|
|  File Name: Xchg.cpp
|
|  Creation Date: 18-10-2012
|
|  Last Modified: Thu, Oct 18, 2012 10:38:51 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Xchg.hpp"

#include "../ModrmOperand.hpp"
#include "../RegisterOperand.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>
#include <string>

Xchg::Xchg(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Xchg::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {

	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newXchg = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case XCHG_REG_AX:
		case XCHG_REG_CX:
		case XCHG_REG_DX:
		case XCHG_REG_BX:
		case XCHG_REG_SP:
		case XCHG_REG_BP:
		case XCHG_REG_SI:
		case XCHG_REG_DI:
		{
			Operand* src = new RegisterOperand(REG_AX, proc);
			Operand* dst = new RegisterOperand((eRegisters)(*opLoc - XCHG_REG_AX + REG_AX),
				       proc);
			snprintf(buf, 65, "XCHG AX, %s", dst->GetDisasm().c_str());
			GETINST(preSize + 1);
			newXchg = new Xchg(pre, buf, inst, (unsigned char)*opLoc);
			newXchg->SetOperand(Operand::DST, dst);
			newXchg->SetOperand(Operand::SRC, src);
			break;
		}
		case XCHG_MOD8_REG8:
		case XCHG_MOD16_REG16:
		{
			unsigned int size = ((*opLoc == XCHG_MOD8_REG8) ? 1 : 2);
			Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, size);
			Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::REG, size);
			snprintf(buf, 65, "XCHG %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());
			GETINST(preSize + 2);
			newXchg = new Xchg(pre, buf, inst, (unsigned char)*opLoc);
			newXchg->SetOperand(Operand::DST, dst);
			newXchg->SetOperand(Operand::SRC, src);
			break;
		}

	}
	return newXchg;

}

int Xchg::Execute(Processor*) {

	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];
	if(!dst || !src) {
		return INVALID_ARGS;
	}

	unsigned int tmp = dst->GetValue();
	dst->SetValue(src->GetValue());
	src->SetValue(tmp);

	return 0;
}
