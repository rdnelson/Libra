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

#include "../Processor8086.hpp"

#include <cstdio>

CLSTX::CLSTX(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* CLSTX::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	switch(*opLoc) {
		case CLC:
			snprintf(buf, 65, "CLC");
			break;
		case CLD:
			snprintf(buf, 65, "CLD");
			break;
		case CLI:
			snprintf(buf, 65, "CLI");
			break;
		case CMC:
			snprintf(buf, 65, "CMC");
			break;
		case STC:
			snprintf(buf, 65, "STC");
			break;
		case STD:
			snprintf(buf, 65, "STD");
			break;
		case STI:
			snprintf(buf, 65, "STI");
			break;
		default:
			return 0;
	}
	GETINST(preSize + 1);
	return new CLSTX(pre, buf, inst, (int)*opLoc);

}

int CLSTX::Execute() {
	switch(mOpcode) {
		case CLC:
			mProc->SetFlag(Processor8086::FLAGS_CF, 0);
			break;
		case CLD:
			mProc->SetFlag(Processor8086::FLAGS_DF, 0);
			break;
		case CLI:
			mProc->SetFlag(Processor8086::FLAGS_IF, 0);
			break;
		case CMC:
			mProc->SetFlag(Processor8086::FLAGS_CF, !mProc->GetFlag(Processor8086::FLAGS_CF));
			break;
		case STC:
			mProc->SetFlag(Processor8086::FLAGS_CF, 1);
			break;
		case STD:
			mProc->SetFlag(Processor8086::FLAGS_DF, 1);
			break;
		case STI:
			mProc->SetFlag(Processor8086::FLAGS_IF, 1);
			break;
		default:
			return INVALID_ARGS;
	}

	return 0;
}
