/*-------------------------------------*\
|
|  File Name: CLSTX.cpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  1:45:16 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/


#include "CLSTX.hpp"

#include "../Processor.hpp"

#include <cstdio>

CLSTX::CLSTX(Prefix* pre, std::string text, std::string inst, int op) : Instruction(pre,text,inst,op) {}

Instruction* CLSTX::CreateInstruction(unsigned char* memLoc, Processor* proc) {
	unsigned char* opLoc = memLoc;
	char buf[65];
	std::string inst;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case CLC:
			sprintf(buf, "CLC");
			break;
		case CLD:
			sprintf(buf, "CLD");
			break;
		case CLI:
			sprintf(buf, "CLI");
			break;
		case CMC:
			sprintf(buf, "CMC");
			break;
		case STC:
			sprintf(buf, "STC");
			break;
		case STD:
			sprintf(buf, "STD");
			break;
		case STI:
			sprintf(buf, "STI");
			break;
		default:
			return 0;
	}
	GETINST(preSize + 1);
	return new CLSTX(pre, buf, inst, (int)*opLoc);

}

int CLSTX::Execute(Processor* proc) {
	switch(mOpcode) {
		case CLC:
			proc->SetFlag(FLAGS_CF, 0);
			break;
		case CLD:
			proc->SetFlag(FLAGS_DF, 0);
			break;
		case CLI:
			proc->SetFlag(FLAGS_IF, 0);
			break;
		case CMC:
			proc->SetFlag(FLAGS_CF, !proc->GetFlag(FLAGS_CF));
			break;
		case STC:
			proc->SetFlag(FLAGS_CF, 1);
			break;
		case STD:
			proc->SetFlag(FLAGS_DF, 1);
			break;
		case STI:
			proc->SetFlag(FLAGS_IF, 1);
			break;
		default:
			return -1;
	}

	return 0;
}
