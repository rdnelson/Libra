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
#include "../Processor8086.hpp"

#include <cstdio>

Daa::Daa(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Daa::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newDaa = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == DAA) {

		GETINST(preSize + 1);
		snprintf(buf, 65, "DAA");

		newDaa = new Daa(pre, buf, inst, (int)*opLoc);
		newDaa->SetProc(mProc);
	}

	return newDaa;

}

int Daa::Execute() {
	unsigned int al = mProc->GetRegister(Processor8086::REG_AL);
	bool cf = mProc->GetFlag(Processor8086::FLAGS_CF);
	mProc->SetFlag(Processor8086::FLAGS_CF, 0);
	if((al & 0xF) > 9 || mProc->GetFlag(Processor8086::FLAGS_AF)) {
		mProc->SetRegister(Processor8086::REG_AL, (al + 6) & 0xFF);
		mProc->SetFlag(Processor8086::FLAGS_AF, 1);
		mProc->SetFlag(Processor8086::FLAGS_CF, (cf || (al + 6 > 0xFF)) ? 1 : 0);
	} else {
		mProc->SetFlag(Processor8086::FLAGS_AF, 0);
	}
	if(al > 0x99 || cf) {
		mProc->SetRegister(Processor8086::REG_AL, mProc->GetRegister(Processor8086::REG_AL) + 0x60);
		mProc->SetFlag(Processor8086::FLAGS_CF, 1);
	} else {
		mProc->SetFlag(Processor8086::FLAGS_CF, 0);
	}
	return 0;
}
