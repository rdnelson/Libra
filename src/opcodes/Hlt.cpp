/*-------------------------------------*\
|
|  File Name: Hlt.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Hlt.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Hlt::Hlt(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Hlt::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newHlt = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case HLT:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "HLT");
			newHlt = new Hlt(pre, buf, inst, (int)*opLoc);
			break;
		}
	}

	return newHlt;

}

int Hlt::Execute(Processor* proc) {

	proc->Halt();
	return 0;
}
