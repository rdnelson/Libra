/*-------------------------------------*\
|
|  File Name: Lxs.cpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified: Fri, Oct 19, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Lxs.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../ModrmOperand.hpp"

#include <cstdio>

Lxs::Lxs(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Lxs::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newLxs = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == LDS || *opLoc == LES) {

		Operand* dst = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::REG, 2);
		Operand* src = ModrmOperand::GetModrmOperand(proc, opLoc, ModrmOperand::MOD, 2);
		GETINST(preSize + 2 + src->GetBytecodeLen());
		snprintf(buf, 65, "LDS %s, %s", dst->GetDisasm().c_str(), src->GetDisasm().c_str());

		newLxs = new Lxs(pre, buf, inst, (int)*opLoc);
		newLxs->SetOperand(Operand::SRC, src);
		newLxs->SetOperand(Operand::DST, dst);
	} 


	return newLxs;

}

int Lxs::Execute(Processor* proc) {
	Operand* dst = mOperands[Operand::DST];
	Operand* src = mOperands[Operand::SRC];

	if(!dst || !src) {
		return INVALID_ARGS;
	}

	unsigned int val = src->GetValue(4);

	dst->SetValue(val & 0xFFFF);

	if(mOpcode == LDS) {
		proc->SetRegister(REG_DS, (val & 0xFFFF0000) >> 0x10);
	} else if(mOpcode == LES) {
		proc->SetRegister(REG_ES, (val & 0xFFFF0000) >> 0x10);
	} else {
		return INVALID_ARGS;
	}

	return 0;
}
