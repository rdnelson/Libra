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
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Lods::Lods(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Lods::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newLods = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case LODSB:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "LODSB");
			newLods = new Lods(pre, buf, inst, (int)*opLoc);
			break;
		}
		case LODSW:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "LODSW");
			newLods = new Lods(pre, buf, inst, (int)*opLoc);
			break;
		}
	}

	return newLods;

}

int Lods::Execute(Processor* proc) {
	switch(mOpcode) {
	case LODSB:
		proc->SetRegister(REG_AL, proc->GetMemory(proc->GetRegister(REG_DI), 1));
		break;
	case LODSW:
		proc->SetRegister(REG_AX, proc->GetMemory(proc->GetRegister(REG_DI), 2));
		break;
	}

	return 0;
}
