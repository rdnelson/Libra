/*-------------------------------------*\
|
|  File Name: Movs.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Movs.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Movs::Movs(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Movs::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newMovs = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case MOVSB:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "MOVSB");
			newMovs = new Movs(pre, buf, inst, (int)*opLoc);
			newMovs->SetProc(mProc);
			break;
		}
		case MOVSW:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "MOVSW");
			newMovs = new Movs(pre, buf, inst, (int)*opLoc);
			newMovs->SetProc(mProc);
			break;
		}
	}

	return newMovs;

}

int Movs::Execute() {
	switch(mOpcode) {
	case MOVSB:
		mProc->SetMemory(mProc->GetRegister(Processor8086::REG_DI), 1,
				mProc->GetMemory(mProc->GetRegister(Processor8086::REG_SI), 1));
		break;
	case MOVSW:
		mProc->SetMemory(mProc->GetRegister(Processor8086::REG_DI), 2,
				mProc->GetMemory(mProc->GetRegister(Processor8086::REG_SI), 2));
		break;
	}

	return 0;
}
