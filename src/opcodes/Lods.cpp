/*-------------------------------------*\
|
|  File Name: Lods.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Lods.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Lods::Lods(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Lods::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newLods = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case LODSB:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "LODSB");
			newLods = new Lods(pre, buf, inst, (int)*opLoc);
			newLods->SetProc(mProc);
			break;
		}
		case LODSW:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "LODSW");
			newLods = new Lods(pre, buf, inst, (int)*opLoc);
			newLods->SetProc(mProc);
			break;
		}
	}

	return newLods;

}

int Lods::Execute() {
	switch(mOpcode) {
	case LODSB:
		mProc->SetRegister(Processor8086::REG_AL, mProc->GetMemory(mProc->GetRegister(Processor8086::REG_DI), 1));
		break;
	case LODSW:
		mProc->SetRegister(Processor8086::REG_AX, mProc->GetMemory(mProc->GetRegister(Processor8086::REG_DI), 2));
		break;
	}

	return 0;
}
