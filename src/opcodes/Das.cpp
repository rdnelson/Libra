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
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Das::Das(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Das::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newDas = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == DAS) {

		GETINST(preSize + 1);
		snprintf(buf, 65, "DAS");

		newDas = new Das(pre, buf, inst, (int)*opLoc);
		newDas->SetProc(mProc);
	}

	return newDas;

}

int Das::Execute() {
	unsigned int val = mProc->GetRegister(Processor8086::REG_AL);
	unsigned int cf = mProc->GetFlag(Processor8086::FLAGS_CF);
	mProc->SetFlag(Processor8086::FLAGS_CF, 0);
	if((val & 0xF) > 9 || mProc->GetFlag(Processor8086::FLAGS_AF)) {
		mProc->SetRegister(Processor8086::REG_AL, val - 6);
		mProc->SetFlag(Processor8086::FLAGS_CF, cf || ((int)val - 6) < 0);
		mProc->SetFlag(Processor8086::FLAGS_AF, 1);
	} else {
		mProc->SetFlag(Processor8086::FLAGS_AF, 0);
	}
	if(val > 0x99 || cf) {
		mProc->SetRegister(Processor8086::REG_AL, mProc->GetRegister(Processor8086::REG_AL) - 0x60);
		mProc->SetFlag(Processor8086::FLAGS_CF, 1);
	}
	return 0;
}
