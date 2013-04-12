/*-------------------------------------*\
|
|  File Name: Aas.cpp
|
|  Creation Date: 10-10-2012
|
|  Last Modified: Thu, Oct 11, 2012  2:43:23 PM
|
|  Created By: Robert Nelson
|
\*-------------------------------------*/

#include "Aas.hpp"
#include "../Processor8086.hpp"
#include "../ImmediateOperand.hpp"

#include <cstdio>

Aas::Aas(Prefix* pre, std::string text, std::string inst, int op) : Instruction8086(pre,text,inst,op) {}

Instruction* Aas::CreateInstruction(Memory::MemoryOffset& memLoc, Processor* proc) {
	Memory::MemoryOffset opLoc = memLoc;
	char buf[65];
	std::string inst;
	if(proc == 0 || proc->GetModel() != Processor::MODEL_8086) return 0;
	Processor8086* mProc = (Processor8086*)proc;

	Prefix* pre = Prefix::GetPrefix(memLoc);
	unsigned int preSize = 0;
	Instruction8086* newAas = 0;

	if(pre) {
		opLoc += preSize = pre->GetLength();
	}

	if(*opLoc == AAS) {

		GETINST(preSize + 1);
		snprintf(buf, 65, "AAS");

		newAas = new Aas(pre, buf, inst, (int)*opLoc);
		newAas->SetProc(mProc);
	}

	return newAas;

}

int Aas::Execute() {
	unsigned int val = mProc->GetRegister(Processor8086::REG_AX);
	if((val & 0xF) > 9 || mProc->GetFlag(Processor8086::FLAGS_AF)) {
		mProc->SetRegister(Processor8086::REG_AX, val - 6);
		mProc->SetRegister(Processor8086::REG_AH, mProc->GetRegister(Processor8086::REG_AH) - 1);
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
