/*-------------------------------------*\
|
|  File Name: Xlat.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Xlat.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"
#include "../RegisterOperand.hpp"

#include <cstdio>

Xlat::Xlat(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Xlat::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newXlat = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case XLAT:
		{
			GETINST(preSize + 1);
			snprintf(buf, 65, "XLAT");
			newXlat = new Xlat(pre, buf, inst, (int)*opLoc);
			break;
		}
	}

	return newXlat;

}

int Xlat::Execute(Processor* proc) {
	unsigned int al = proc->GetRegister(REG_AL);
	al = proc->GetRegister(REG_BX) + al;
	al = proc->GetMemory(al, 1);
	proc->SetRegister(REG_AL, al);
	return 0;
}
