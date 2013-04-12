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
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Scas::Scas(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Scas::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newScas = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case SCASB:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "SCASB");
			newScas = new Scas(pre, buf, inst, (int)*opLoc);
			newScas->SetProc(mProc);
			break;
		}
		case SCASW:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "SCASW");
			newScas = new Scas(pre, buf, inst, (int)*opLoc);
			newScas->SetProc(mProc);
			break;
		}
	}

	return newScas;

}

int Scas::Execute() {
	unsigned int ax = 0;
	unsigned int mem_di = 0;
	unsigned int sign = 0;
	unsigned int newVal = 0;
	unsigned int bitmask = 0;

	switch(mOpcode) {
	case SCASB:
		ax = mProc->GetRegister(Processor8086::REG_AL);
		mem_di = mProc->GetMemory(mProc->GetRegister(Processor8086::REG_DI), 1);
		sign = 0x80;
		bitmask = 0xFF;
		break;
	case SCASW:
		ax = mProc->GetRegister(Processor8086::REG_AX);
		mem_di = mProc->GetMemory(mProc->GetRegister(Processor8086::REG_DI), 2);
		sign = 0x8000;
		bitmask =0xFFFF;
		break;
	default:
		return INVALID_ARGS;
	}

	newVal = ax - mem_di;

	mProc->SetFlag(Processor8086::FLAGS_CF, newVal > ax);
	newVal &= bitmask;

	mProc->SetFlag(Processor8086::FLAGS_OF, OverflowSub(ax, mem_di, sign == 0x80 ? 1 : 2));
	mProc->SetFlag(Processor8086::FLAGS_SF, newVal >= sign);
	mProc->SetFlag(Processor8086::FLAGS_ZF, newVal == 0x00);
	mProc->SetFlag(Processor8086::FLAGS_AF, AdjustSub(ax, mem_di));
	mProc->SetFlag(Processor8086::FLAGS_PF, Parity(newVal));

	if(mProc->GetFlag(Processor8086::FLAGS_DF))
		mProc->SetRegister(Processor8086::REG_DI, mProc->GetRegister(Processor8086::REG_DI) - (sign == 0x80 ? 1 : 2));
	else
		mProc->SetRegister(Processor8086::REG_DI, mProc->GetRegister(Processor8086::REG_DI) + (sign == 0x80 ? 1 : 2));

	return 0;
}
