/*-------------------------------------*\
|
|  File Name: Cbw.cpp
|
|  Creation Date: 11-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  6:15:46 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Cbw.hpp"

#include "../Processor8086.hpp"

#include <cstdio>

Cbw::Cbw(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Cbw::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == CBW) {
		snprintf(buf, 65, "CBW");
		GETINST(preSize + 1);
		return new Cbw(pre, buf, inst, (int)*opLoc);
	}
	return 0;
}

int Cbw::Execute() {

	unsigned int al = mProc->GetRegister(Processor8086::REG_AL);
	if(al >= 0x80) {
		mProc->SetRegister(Processor8086::REG_AH, 0xFF);
	} else {
		mProc->SetRegister(Processor8086::REG_AH, 0x00);
	}
	return 0;
}
