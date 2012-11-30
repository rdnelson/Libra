/*-------------------------------------*\
|
|  File Name: Sahf.cpp
|
|  Creation Date: 19-10-2012
|
|  Last Modified: Fri, Oct 19, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Sahf.hpp"
#include "../Processor.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Sahf::Sahf(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* Sahf::CreateInstruction(Memory& memLoc, Processor* proc) {
	Memory opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction* newSahf = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == SAHF) {

		GETINST(preSize + 1);
		snprintf(buf, 65, "SAHF");

		newSahf = new Sahf(pre, buf, inst, (int)*opLoc);
	}

	return newSahf;

}

int Sahf::Execute(Processor* proc) {

	unsigned int ah = proc->GetRegister(REG_AH);
	proc->SetFlag(FLAGS_SF, ah & 0x80);
	proc->SetFlag(FLAGS_ZF, ah & 0x40);
	proc->SetFlag(FLAGS_AF, ah & 0x10);
	proc->SetFlag(FLAGS_PF, ah & 0x04);
	proc->SetFlag(FLAGS_CF, ah & 0x01);
	return 0;
}
