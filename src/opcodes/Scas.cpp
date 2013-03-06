/*-------------------------------------*\
|
|  File Name: Scas.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Scas.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Scas::Scas(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Scas::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newScas = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case SCASB:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "SCASB");
			newScas = new Scas(pre, buf, inst, (int)*opLoc);
			break;
		}
		case SCASW:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "SCASW");
			newScas = new Scas(pre, buf, inst, (int)*opLoc);
			break;
		}
	}

	return newScas;

}

int Scas::Execute(Processor* proc) {
	unsigned int ax = 0;
	unsigned int mem_di = 0;
	unsigned int sign = 0;
	unsigned int newVal = 0;
	unsigned int bitmask = 0;

	switch(mOpcode) {
	case SCASB:
		ax = proc->GetRegister(REG_AL);
		mem_di = proc->GetMemory(proc->GetRegister(REG_DI), 1);
		sign = 0x80;
		bitmask = 0xFF;
		break;
	case SCASW:
		ax = proc->GetRegister(REG_AX);
		mem_di = proc->GetMemory(proc->GetRegister(REG_DI), 2);
		sign = 0x8000;
		bitmask =0xFFFF;
		break;
	default:
		return INVALID_ARGS;
	}

	newVal = ax - mem_di;

	proc->SetFlag(FLAGS_CF, newVal > ax);
	newVal &= bitmask;

	proc->SetFlag(FLAGS_OF, OverflowSub(ax, mem_di, sign == 0x80 ? 1 : 2));
	proc->SetFlag(FLAGS_SF, newVal >= sign);
	proc->SetFlag(FLAGS_ZF, newVal == 0x00);
	proc->SetFlag(FLAGS_AF, AdjustSub(ax, mem_di));
	proc->SetFlag(FLAGS_PF, Parity(newVal));

	if(proc->GetFlag(FLAGS_DF))
		proc->SetRegister(REG_DI, proc->GetRegister(REG_DI) - (sign == 0x80 ? 1 : 2));
	else
		proc->SetRegister(REG_DI, proc->GetRegister(REG_DI) + (sign == 0x80 ? 1 : 2));

	return 0;
}
