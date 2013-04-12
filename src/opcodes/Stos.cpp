/*-------------------------------------*\
|
|  File Name: Stos.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Stos.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Stos::Stos(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Stos::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newStos = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case STOSB:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "STOSB");
			newStos = new Stos(pre, buf, inst, (int)*opLoc);
			newStos->SetProc(mProc);
			break;
		}
		case STOSW:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "STOSW");
			newStos = new Stos(pre, buf, inst, (int)*opLoc);
			newStos->SetProc(mProc);
			break;
		}
	}

	return newStos;

}

int Stos::Execute() {
	switch(mOpcode) {
	case STOSB:
		mProc->SetMemory(mProc->GetRegister(Processor8086::REG_DI), 1, mProc->GetRegister(Processor8086::REG_AL));
		break;
	case STOSW:
		mProc->SetMemory(mProc->GetRegister(Processor8086::REG_DI), 2, mProc->GetRegister(Processor8086::REG_AX));
		break;
	}

	return 0;
}
