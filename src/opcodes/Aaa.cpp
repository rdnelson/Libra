/*-------------------------------------*\
|
|  File Name: Aaa.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  1:10:05 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Aaa.hpp"
#include "../Processor.hpp"

#include <cstdio>

Aaa::Aaa(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Aaa::CreateInstruction(Memory::MemoryOffset& memLoc, Processor*) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newAaa = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == AAA) {

		GETINST(preSize + 1);
		snprintf(buf, 65, "AAA");

		newAaa = new Aaa(pre, buf, inst, (int)*opLoc);
	}

	return newAaa;

}

int Aaa::Execute(Processor* proc) {
	unsigned int val = proc->GetRegister(REG_AL);
	if((val & 0xF) > 9 || proc->GetFlag(FLAGS_AF)) {
		proc->SetRegister(REG_AL, val + 6);
		proc->SetRegister(REG_AH, proc->GetRegister(REG_AH) + 1);
		proc->SetFlag(FLAGS_AF, 1);
		proc->SetFlag(FLAGS_CF, 1);
		proc->SetRegister(REG_AL, proc->GetRegister(REG_AL) & 0xF);
	} else {
		proc->SetFlag(FLAGS_AF, 0);
		proc->SetFlag(FLAGS_CF, 0);
		proc->SetRegister(REG_AL, val & 0xF);
	}
	return 0;
}
