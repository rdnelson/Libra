/*-------------------------------------*\
|
|  File Name: Daa.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  1:10:05 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Daa.hpp"
#include "../Processor.hpp"

#include <cstdio>

Daa::Daa(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Daa::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newDaa = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == DAA) {

		GETINST(preSize + 1);
		snprintf(buf, 65, "DAA");

		newDaa = new Daa(pre, buf, inst, (int)*opLoc);
	}

	return newDaa;

}

int Daa::Execute(Processor* proc) {
	unsigned int al = proc->GetRegister(REG_AL);
	bool cf = proc->GetFlag(FLAGS_CF);
	proc->SetFlag(FLAGS_CF, 0);
	if((al & 0xF) > 9 || proc->GetFlag(FLAGS_AF)) {
		proc->SetRegister(REG_AL, (al + 6) & 0xFF);
		proc->SetFlag(FLAGS_AF, 1);
		proc->SetFlag(FLAGS_CF, (cf || (al + 6 > 0xFF)) ? 1 : 0);
	} else {
		proc->SetFlag(FLAGS_AF, 0);
	}
	if(al > 0x99 || cf) {
		proc->SetRegister(REG_AL, proc->GetRegister(REG_AL) + 0x60);
		proc->SetFlag(FLAGS_CF, 1);
	} else {
		proc->SetFlag(FLAGS_CF, 0);
	}
	return 0;
}
