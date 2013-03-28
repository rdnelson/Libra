/*-------------------------------------*\
|
|  File Name: Das.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Das.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Das::Das(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Das::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	proc += 0;
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newDas = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == DAS) {

		GETINST(preSize + 1);
		snprintf(buf, 65, "DAS");

		newDas = new Das(pre, buf, inst, (int)*opLoc);
	}

	return newDas;

}

int Das::Execute(Processor* proc) {
	unsigned int val = proc->GetRegister(REG_AL);
	unsigned int cf = proc->GetFlag(FLAGS_CF);
	proc->SetFlag(FLAGS_CF, 0);
	if((val & 0xF) > 9 || proc->GetFlag(FLAGS_AF)) {
		proc->SetRegister(REG_AL, val - 6);
		proc->SetFlag(FLAGS_CF, cf || ((int)val - 6) < 0);
		proc->SetFlag(FLAGS_AF, 1);
	} else {
		proc->SetFlag(FLAGS_AF, 0);
	}
	if(val > 0x99 || cf) {
		proc->SetRegister(REG_AL, proc->GetRegister(REG_AL) - 0x60);
		proc->SetFlag(FLAGS_CF, 1);
	}
	return 0;
}
