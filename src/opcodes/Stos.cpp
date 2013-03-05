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
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Stos::Stos(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Stos::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newStos = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case STOSB:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "STOSB");
			newStos = new Stos(pre, buf, inst, (int)*opLoc);
			break;
		}
		case STOSW:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "STOSW");
			newStos = new Stos(pre, buf, inst, (int)*opLoc);
			break;
		}
	}

	return newStos;

}

int Stos::Execute(Processor* proc) {
	switch(mOpcode) {
	case STOSB:
		proc->SetMemory(proc->GetRegister(REG_DI), 1, proc->GetRegister(REG_AL));
		break;
	case STOSW:
		proc->SetMemory(proc->GetRegister(REG_DI), 2, proc->GetRegister(REG_AX));
		break;
	}

	return 0;
}
