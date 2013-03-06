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
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Movs::Movs(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Movs::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newMovs = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case MOVSB:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "MOVSB");
			newMovs = new Movs(pre, buf, inst, (int)*opLoc);
			break;
		}
		case MOVSW:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "MOVSW");
			newMovs = new Movs(pre, buf, inst, (int)*opLoc);
			break;
		}
	}

	return newMovs;

}

int Movs::Execute(Processor* proc) {
	switch(mOpcode) {
	case MOVSB:
		proc->SetMemory(proc->GetRegister(REG_DI), 1,
				proc->GetMemory(proc->GetRegister(REG_SI), 1));
		break;
	case MOVSW:
		proc->SetMemory(proc->GetRegister(REG_DI), 2,
				proc->GetMemory(proc->GetRegister(REG_SI), 2));
		break;
	}

	return 0;
}
