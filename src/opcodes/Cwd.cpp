/*-------------------------------------*\
|
|  File Name: Cwd.cpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Fri, Oct 12, 2012 10:08:34 AM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Cwd.hpp"

#include "../Processor.hpp"

#include <cstdio>

Cwd::Cwd(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Cwd::CreateInstruction(Memory& memLoc, Processor* proc) {
	Memory opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == CWD) {
		snprintf(buf, 65, "CWD");
		GETINST(preSize + 1);
		return new Cwd(pre, buf, inst, (int)*opLoc);
	}
	return 0;
}

int Cwd::Execute(Processor* proc) {

	unsigned int ax = proc->GetRegister(REG_AX);
	if(ax >= 0x8000) {
		proc->SetRegister(REG_DX, 0xFFFF);
	} else {
		proc->SetRegister(REG_DX, 0x0000);
	}
	return 0;
}
