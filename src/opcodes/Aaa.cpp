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
#include "../Processor8086.hpp"

#include <cstdio>

Aaa::Aaa(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Aaa::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;


	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newAaa = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == AAA) {

		GETINST(preSize + 1);
		snprintf(buf, 65, "AAA");

		newAaa = new Aaa(pre, buf, inst, (int)*opLoc);
		newAaa->SetProc(mProc);
	}

	return newAaa;

}

int Aaa::Execute() {
	unsigned int val = mProc->GetRegister(Processor8086::REG_AL);
	if((val & 0xF) > 9 || mProc->GetFlag(Processor8086::FLAGS_AF)) {
		mProc->SetRegister(Processor8086::REG_AL, val + 6);
		mProc->SetRegister(Processor8086::REG_AH, mProc->GetRegister(Processor8086::REG_AH) + 1);
		mProc->SetFlag(Processor8086::FLAGS_AF, 1);
		mProc->SetFlag(Processor8086::FLAGS_CF, 1);
		mProc->SetRegister(Processor8086::REG_AL, mProc->GetRegister(Processor8086::REG_AL) & 0xF);
	} else {
		mProc->SetFlag(Processor8086::FLAGS_AF, 0);
		mProc->SetFlag(Processor8086::FLAGS_CF, 0);
		mProc->SetRegister(Processor8086::REG_AL, val & 0xF);
	}
	return 0;
}
